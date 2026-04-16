object fRFID: TfRFID
  Left = 464
  Top = 237
  Width = 675
  Height = 675
  Caption = 'fRFID'
  Color = 12761254
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
    Width = 659
    Height = 593
    ActivePage = tsRFID
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    object tsRFID: TTabSheet
      Caption = 'RFID'
      object Label395: TLabel
        Left = 13
        Top = 53
        Width = 160
        Height = 20
        Caption = 'Decode time out (ms): '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labRetryCount: TLabel
        Left = 13
        Top = 91
        Width = 113
        Height = 20
        Caption = 'Auto retry count'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object cbEnableRFID: TCheckBox
        Left = 12
        Top = 14
        Width = 273
        Height = 19
        Caption = 'Enable RFID function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 0
      end
      object edRFIDScanDelayTime: TEdit
        Left = 241
        Top = 49
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Text = '1'
      end
      object gbRFIDTest: TGroupBox
        Left = 0
        Top = 259
        Width = 651
        Height = 299
        Align = alBottom
        Caption = 'RFID Test'
        TabOrder = 2
        object spbResetCom: TSpeedButton
          Left = 439
          Top = 19
          Width = 180
          Height = 55
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'Stop COM'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Glyph.Data = {
            660F0000424D660F000000000000360000002800000024000000240000000100
            180000000000300F000001000000010000000000000000000000D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2B9
            A596AD9D8E9885758B796685726083705D816D5A806D59806D59806D59806D59
            806D59806D59806D59806D59806D59806D59806D59806D59806D59806D59806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2BAA596FCF8F6FBF8F5FCF6F4FCF6F4FBF5F2FBF4F1
            FAF3F0FAF3EFF9F2EEF9F1EDF9F1ECF9F0EBF8EFEAF9EFEAF8EEE9F7EDE8F7EE
            E7F7EDE7F7ECE7F7ECE6806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BBA697FCF8F7FCF7F5
            FBF7F4FBF6F3FBF5F2FBF5F1FAF4F0FAF3EFFAF2EEFAF2EDF9F1ECF9F0EBF8EF
            EAF8EEE9F8EFE9F8EEE8F7EDE7F8EDE7F7EDE7F7ECE7806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2BBA798FCF9F6FCF8F52D92C72B8FC7298DC7278AC62485C72181C71E7D
            C71A78C61673C6126EC60F69C60C65C60961C6075DC60459C70256C6F7EDE7F7
            EDE7806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BCA899FDF9F7FCF7F52D92C7CBE8F6C7E7
            F5C1E4F5BBE1F5B4DEF5ACDBF4A4D7F49BD4F393D0F28ACDF283CAF17CC7F175
            C4F170C2F10054C6F8EDE8F8EDE7806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BDA89AFDF8
            F6FCF7F52D92C7CBE8F62A9BF92899F92697F82394F82191F81D8EF71A89F616
            86F51483F5107FF50E7CF40C7AF370C2F10054C6F7EEE8F8EEE8806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2BEA99BFDF9F7FDF8F62D92C7CBE8F6C7E6F6C2E5F5BBE1F5B4
            DEF4ACDBF4A4D8F49BD4F393D0F28BCDF283CAF27CC7F176C4F070C2F00054C6
            F8EFE9F8EEE9806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BFAA9CFCF8F7FDF8F62D92C72B
            8FC7298DC7278AC62485C72181C71E7DC71A78C61673C6126EC60F69C60C65C6
            0961C6075DC60459C70256C6F8EFE9F8EFE9806D59D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C0
            AC9DFCF9F7FCF9F6FCF7F5FCF7F4FBF7F3FBF6F2FBF5F2FAF4F1FAF4F0FAF4EF
            FAF3EEFAF1EEF9F1EDF9F1ECF9F0ECF8F0ECF8F0EBF8F0EAF9EFEAF8EFEA806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2C1AD9EFCF9F7FCF9F7D0B3A5CFAFA2CCAC9DCAA999
            C9A595C6A190C49E8DC29B89C19986BF9684BE9481BC927FBB907DBB8F7BBA8F
            7BBA8E7AF8F0EBF8F0EB806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C2AE9FFDF9F8FCF9F8
            FCF9F7FCF8F6FCF7F5FBF6F4FBF6F3FBF6F2FBF5F1FBF4F1FAF4F0FAF3EFF9F2
            EFFAF2EDFAF2EEF9F2EDF9F1EDF9F0EDF9F1ECF9F1EC806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2C3AFA0FDFBF9FDF9F7D3B7AAD2B5A7CFB2A4CEAF9FCCAA9CC9A697C7A4
            93C5A090C39C8CC19A88BF9785BE9582BD937FBC917DBB907CBA8F7BF9F1EDF9
            F2ED806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C4B0A2FDFAF8FCFAF8FDFAF7FCF8F6FCF8
            F6FCF7F5FBF6F4FBF6F3FBF5F3FBF5F2FBF4F2FAF4F0FAF3F0FAF4F0FAF3EFF9
            F2EEFAF2EFFAF2EEF9F2EEF9F2EE806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C5B1A3FDFA
            F9FDFAF8D5BAAED4B9ACD2B6A9D1B3A6CFB1A2CCAD9ECAAA9AC8A595C7A192C4
            9F8DC29B8AC19986BF9684BD9481BD927EBB907DFAF3EFF9F3F0806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2C6B3A4FDFBFAFDFBF9FDFAF8FCF9F7FCF9F7FCF8F6FCF8F6FC
            F7F5FBF7F4FBF6F4FBF6F3FBF5F2FBF5F1FBF5F1FBF4F1FBF4F1FBF4F1FBF4F0
            FAF4F0FAF4F0806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C7B4A5FDFCFAFDFBFAD7BDB1D6
            BCB0D4BAADD3B7ABD2B5A8CFB2A5CEAFA1CCAB9DCAA898C7A494C5A090C39E8C
            C19A88C09885BF9582BD937FFBF5F1FBF4F1806D59D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C8
            B5A7FEFCFBFDFBFAFDFBF9FDFBF8FDF9F8FDFAF7FCF9F7FDF8F7FCF8F6FCF7F5
            FBF7F4FCF7F4FCF6F4FCF6F3FBF6F3FBF6F3FBF5F3FBF5F2FBF5F2FBF5F2806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2C9B6A8FEFCFCFEFBFBD8C0B3D7BEB2D6BCB1D5BBAE
            D4B9ACD2B7AAD2B4A6CFB1A3CDAD9ECAAA9BC9A595C7A292C49F8EC39C8AC199
            87BF9784FBF6F3FCF6F3806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CAB7A9FEFDFCFEFCFB
            FEFBFBFEFBFAFDFBF9FDFAF9FDFAF8FDF9F8FCF9F8FCF9F7FCF9F6FDF9F6FCF7
            F6FCF7F5FCF7F5FCF7F4FBF7F5FCF7F4FCF6F4FBF6F4806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2CBB8AAFEFDFCFEFDFCD9C1B6D8C0B5D8BFB4D7BEB2D6BCB0D5BBADD3B8
            ACD2B6A9D0B3A6FCF9F7FCF8F6FDF9F6FCF8F6FCF8F6FCF8F6FCF7F5FCF7F5FC
            F8F5806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CCBAABFFFDFDFEFDFCFEFDFCFEFCFCFEFC
            FAFDFBFAFDFBF9FDFBF9FDFAF9FDFAF8FCFAF8FDFAF8FCF9F7FDF9F7FCF8F7FC
            F8F7FDF9F6FDF8F6FCF8F7FCF9F6806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CDBBACFEFE
            FDFEFEFD2B8FC7278BC62385C71E7DC61875C7126DC60D66C70860C60256C6FD
            FAF8FDFAF8BEAA9A9C8674836E59806D59806D59806D59806D59806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2CEBCADFEFEFEFEFEFD2B8FC7C3E5F6B9E1F5ACDBF49ED5F38F
            D0F282C9F277C6F10054C6FEFBF9FDFBF9BEAA9AFFFFFFEEDBCEECD2BFE4C5B0
            D9BAA4806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CFBCAEFEFEFEFEFEFE2B8FC7C3
            E5F62798F92393F81C8DF61786F61280F578C5F10054C6FEFBFAFEFBFABEAA9A
            FFFFFFFFEEE4F5E1D3E1C6B2806D59D2C2B4D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D0
            BDAFFFFFFFFFFEFF2B8FC7C3E5F6B9E0F5ACDBF49ED5F38FCFF383C9F278C5F1
            0054C6FDFCFBFEFCFABEAA9AFFFFFFFFEEE4E9D4C5806D59D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D0BEB0FFFFFFFFFFFF2B8FC7278BC62385C71E7DC6
            1875C7126DC60D66C70860C60256C6FEFCFBFEFDFBBEAA9AFFFFFFF3E0D5836E
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D1BFB1FFFFFFFFFFFF
            FFFFFFFFFEFEFEFFFEFFFFFEFEFEFEFEFEFDFFFEFDFFFDFDFEFDFDFEFDFCFEFD
            FCBEAA9AF3E0D59C8674D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D2BFB1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFEFEFFFEFEFFFF
            FEFFFEFEFFFDFDFEFEFDFEFEFDBEAA9ABDA89AD7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D2C0B2D2C0B2D1BFB1D0BEB0CFBCAECDBB
            ADCCB9ABCAB7A9C8B5A7C7B3A5C5B1A3C3AFA1C2AD9FC0AC9EBFAA9CD7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2}
          Margin = 2
          ParentFont = False
          OnClick = spbResetComClick
        end
        object spbStartCom: TSpeedButton
          Left = 439
          Top = 81
          Width = 180
          Height = 55
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'Start COM'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Glyph.Data = {
            660F0000424D660F000000000000360000002800000024000000240000000100
            180000000000300F000001000000010000000000000000000000D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2B9
            A596AD9D8E9885758B796685726083705D816D5A806D59806D59806D59806D59
            806D59806D59806D59806D59806D59806D59806D59806D59806D59806D59806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2BAA596FCF8F6FBF8F5FCF6F4FCF6F4FBF5F2FBF4F1
            FAF3F0FAF3EFF9F2EEF9F1EDF9F1ECF9F0EBF8EFEAF9EFEAF8EEE9F7EDE8F7EE
            E7F7EDE7F7ECE7F7ECE6806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BBA697FCF8F7FCF7F5
            FBF7F4FBF6F3FBF5F2FBF5F1FAF4F0FAF3EFFAF2EEFAF2EDF9F1ECF9F0EBF8EF
            EAF8EEE9F8EFE9F8EEE8F7EDE7F8EDE7F7EDE7F7ECE7806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2BBA798FCF9F6FCF8F52D92C72B8FC7298DC7278AC62485C72181C71E7D
            C71A78C61673C6126EC60F69C60C65C60961C6075DC60459C70256C6F7EDE7F7
            EDE7806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BCA899FDF9F7FCF7F52D92C7CBE8F6C7E7
            F5C1E4F5BBE1F5B4DEF5ACDBF4A4D7F49BD4F393D0F28ACDF283CAF17CC7F175
            C4F170C2F10054C6F8EDE8F8EDE7806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BDA89AFDF8
            F6FCF7F52D92C7CBE8F62A9BF92899F92697F82394F82191F81D8EF71A89F616
            86F51483F5107FF50E7CF40C7AF370C2F10054C6F7EEE8F8EEE8806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2BEA99BFDF9F7FDF8F62D92C7CBE8F6C7E6F6C2E5F5BBE1F5B4
            DEF4ACDBF4A4D8F49BD4F393D0F28BCDF283CAF27CC7F176C4F070C2F00054C6
            F8EFE9F8EEE9806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2BFAA9CFCF8F7FDF8F62D92C72B
            8FC7298DC7278AC62485C72181C71E7DC71A78C61673C6126EC60F69C60C65C6
            0961C6075DC60459C70256C6F8EFE9F8EFE9806D59D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C0
            AC9DFCF9F7FCF9F6FCF7F5FCF7F4FBF7F3FBF6F2FBF5F2FAF4F1FAF4F0FAF4EF
            FAF3EEFAF1EEF9F1EDF9F1ECF9F0ECF8F0ECF8F0EBF8F0EAF9EFEAF8EFEA806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2C1AD9EFCF9F7FCF9F7D0B3A5CFAFA2CCAC9DCAA999
            C9A595C6A190C49E8DC29B89C19986BF9684BE9481BC927FBB907DBB8F7BBA8F
            7BBA8E7AF8F0EBF8F0EB806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C2AE9FFDF9F8FCF9F8
            FCF9F7FCF8F6FCF7F5FBF6F4FBF6F3FBF6F2FBF5F1FBF4F1FAF4F0FAF3EFF9F2
            EFFAF2EDFAF2EEF9F2EDF9F1EDF9F0EDF9F1ECF9F1EC806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2C3AFA0FDFBF9FDF9F7D3B7AAD2B5A7CFB2A4CEAF9FCCAA9CC9A697C7A4
            93C5A090C39C8CC19A88BF9785BE9582BD937FBC917DBB907CBA8F7BF9F1EDF9
            F2ED806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C4B0A2FDFAF8FCFAF8FDFAF7FCF8F6FCF8
            F6FCF7F5FBF6F4FBF6F3FBF5F3FBF5F2FBF4F2FAF4F0FAF3F0FAF4F0FAF3EFF9
            F2EEFAF2EFFAF2EEF9F2EEF9F2EE806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C5B1A3FDFA
            F9FDFAF8D5BAAED4B9ACD2B6A9D1B3A6CFB1A2CCAD9ECAAA9AC8A595C7A192C4
            9F8DC29B8AC19986BF9684BD9481BD927EBB907DFAF3EFF9F3F0806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2C6B3A4FDFBFAFDFBF9FDFAF8FCF9F7FCF9F7FCF8F6FCF8F6FC
            F7F5FBF7F4FBF6F4FBF6F3FBF5F2FBF5F1FBF5F1FBF4F1FBF4F1FBF4F1FBF4F0
            FAF4F0FAF4F0806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C7B4A5FDFCFAFDFBFAD7BDB1D6
            BCB0D4BAADD3B7ABD2B5A8CFB2A5CEAFA1CCAB9DCAA898C7A494C5A090C39E8C
            C19A88C09885BF9582BD937FFBF5F1FBF4F1806D59D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2C8
            B5A7FEFCFBFDFBFAFDFBF9FDFBF8FDF9F8FDFAF7FCF9F7FDF8F7FCF8F6FCF7F5
            FBF7F4FCF7F4FCF6F4FCF6F3FBF6F3FBF6F3FBF5F3FBF5F2FBF5F2FBF5F2806D
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2C9B6A8FEFCFCFEFBFBD8C0B3D7BEB2D6BCB1D5BBAE
            D4B9ACD2B7AAD2B4A6CFB1A3CDAD9ECAAA9BC9A595C7A292C49F8EC39C8AC199
            87BF9784FBF6F3FCF6F3806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CAB7A9FEFDFCFEFCFB
            FEFBFBFEFBFAFDFBF9FDFAF9FDFAF8FDF9F8FCF9F8FCF9F7FCF9F6FDF9F6FCF7
            F6FCF7F5FCF7F5FCF7F4FBF7F5FCF7F4FCF6F4FBF6F4806D59D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2CBB8AAFEFDFCFEFDFCD9C1B6D8C0B5D8BFB4D7BEB2D6BCB0D5BBADD3B8
            ACD2B6A9D0B3A6FCF9F7FCF8F6FDF9F6FCF8F6FCF8F6FCF8F6FCF7F5FCF7F5FC
            F8F5806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CCBAABFFFDFDFEFDFCFEFDFCFEFCFCFEFC
            FAFDFBFAFDFBF9FDFBF9FDFAF9FDFAF8FCFAF8FDFAF8FCF9F7FDF9F7FCF8F7FC
            F8F7FDF9F6FDF8F6FCF8F7FCF9F6806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CDBBACFEFE
            FDFEFEFD2B8FC7278BC62385C71E7DC61875C7126DC60D66C70860C60256C6FD
            FAF8FDFAF8BEAA9A9C8674836E59806D59806D59806D59806D59806D59D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2CEBCADFEFEFEFEFEFD2B8FC7C3E5F6B9E1F5ACDBF49ED5F38F
            D0F282C9F277C6F10054C6FEFBF9FDFBF9BEAA9AFFFFFFEEDBCEECD2BFE4C5B0
            D9BAA4806D59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2CFBCAEFEFEFEFEFEFE2B8FC7C3
            E5F62798F92393F81C8DF61786F61280F578C5F10054C6FEFBFAFEFBFABEAA9A
            FFFFFFFFEEE4F5E1D3E1C6B2806D59D2C2B4D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D0
            BDAFFFFFFFFFFEFF2B8FC7C3E5F6B9E0F5ACDBF49ED5F38FCFF383C9F278C5F1
            0054C6FDFCFBFEFCFABEAA9AFFFFFFFFEEE4E9D4C5806D59D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D0BEB0FFFFFFFFFFFF2B8FC7278BC62385C71E7DC6
            1875C7126DC60D66C70860C60256C6FEFCFBFEFDFBBEAA9AFFFFFFF3E0D5836E
            59D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D1BFB1FFFFFFFFFFFF
            FFFFFFFFFEFEFEFFFEFFFFFEFEFEFEFEFEFDFFFEFDFFFDFDFEFDFDFEFDFCFEFD
            FCBEAA9AF3E0D59C8674D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D2BFB1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFFFEFEFFFEFEFFFF
            FEFFFEFEFFFDFDFEFEFDFEFEFDBEAA9ABDA89AD7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D2C0B2D2C0B2D1BFB1D0BEB0CFBCAECDBB
            ADCCB9ABCAB7A9C8B5A7C7B3A5C5B1A3C3AFA1C2AD9FC0AC9EBFAA9CD7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7
            E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2
            D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3F2D7E3
            F2D7E3F2D7E3F2D7E3F2}
          Margin = 2
          ParentFont = False
          OnClick = spbStartComClick
        end
        object gb1: TGroupBox
          Left = 12
          Top = 26
          Width = 397
          Height = 50
          Caption = 'Load Port'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object cbCommReader1: TCheckBox
            Left = 12
            Top = 18
            Width = 173
            Height = 17
            Caption = 'Load Port COM Port: '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object coCommRFID1: TComboBox
            Left = 160
            Top = 15
            Width = 117
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemHeight = 16
            ParentFont = False
            TabOrder = 1
            Text = 'COM15'
            Items.Strings = (
              'COM1'
              'COM2'
              'COM3'
              'COM4'
              'COM5'
              'COM6'
              'COM7'
              'COM8'
              'COM9'
              'COM10'
              'COM11'
              'COM12'
              'COM13'
              'COM14'
              'COM15'
              'COM16'
              'COM17'
              'COM18'
              'COM19')
          end
          object btRFID1On: TButton
            Left = 288
            Top = 14
            Width = 97
            Height = 25
            Caption = 'RFID 1 On'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btRFID1OnClick
          end
        end
        object GroupBox1: TGroupBox
          Left = 12
          Top = 82
          Width = 397
          Height = 50
          Caption = 'Conversion'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          object CheckBox1: TCheckBox
            Left = 12
            Top = 18
            Width = 165
            Height = 17
            Caption = 'Conversion COM Port: '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object coCommRFID2: TComboBox
            Left = 160
            Top = 15
            Width = 117
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemHeight = 16
            ParentFont = False
            TabOrder = 1
            Text = 'COM16'
            Items.Strings = (
              'COM1'
              'COM2'
              'COM3'
              'COM4'
              'COM5'
              'COM6'
              'COM7'
              'COM8'
              'COM9'
              'COM10'
              'COM11'
              'COM12'
              'COM13'
              'COM14'
              'COM15'
              'COM16'
              'COM17'
              'COM18'
              'COM19')
          end
          object btRFID2On: TButton
            Tag = 1
            Left = 288
            Top = 14
            Width = 97
            Height = 25
            Caption = 'RFID 2 On'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btRFID1OnClick
          end
        end
        object Memo1: TMemo
          Left = 2
          Top = 152
          Width = 647
          Height = 145
          Align = alBottom
          Color = 14670284
          ScrollBars = ssVertical
          TabOrder = 2
        end
      end
      object edRFIDRetryCount: TEdit
        Left = 241
        Top = 87
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = '1'
      end
      object cbRFIDSimulate: TCheckBox
        Left = 408
        Top = 232
        Width = 177
        Height = 17
        Caption = 'Simulate'
        TabOrder = 4
      end
    end
    object tsDelta: TTabSheet
      Caption = 'Delta'
      ImageIndex = 1
      object Label1: TLabel
        Left = 272
        Top = 12
        Width = 15
        Height = 20
        Caption = 'IP'
      end
      object Label11: TLabel
        Left = 0
        Top = 104
        Width = 96
        Height = 20
        Caption = 'Action Status'
      end
      object Label10: TLabel
        Left = 0
        Top = 72
        Width = 112
        Height = 20
        Caption = 'Response Data'
      end
      object Label9: TLabel
        Left = 0
        Top = 40
        Width = 100
        Height = 20
        Caption = 'Request Data'
      end
      object Label2: TLabel
        Left = 40
        Top = 136
        Width = 89
        Height = 20
        Caption = 'Cassette ID:'
      end
      object Label3: TLabel
        Left = 200
        Top = 136
        Width = 43
        Height = 20
        Caption = 'Data :'
      end
      object Label4: TLabel
        Left = 360
        Top = 136
        Width = 48
        Height = 20
        Caption = 'Lot ID:'
      end
      object Label5: TLabel
        Left = 21
        Top = 163
        Width = 9
        Height = 20
        Caption = '1'
      end
      object Label6: TLabel
        Left = 21
        Top = 197
        Width = 9
        Height = 20
        Caption = '2'
      end
      object Label7: TLabel
        Left = 21
        Top = 231
        Width = 9
        Height = 20
        Caption = '3'
      end
      object Label8: TLabel
        Left = 21
        Top = 264
        Width = 9
        Height = 20
        Caption = '4'
      end
      object Label12: TLabel
        Left = 21
        Top = 298
        Width = 9
        Height = 20
        Caption = '5'
      end
      object Label13: TLabel
        Left = 21
        Top = 332
        Width = 9
        Height = 20
        Caption = '6'
      end
      object Label14: TLabel
        Left = 21
        Top = 366
        Width = 9
        Height = 20
        Caption = '7'
      end
      object Label15: TLabel
        Left = 21
        Top = 399
        Width = 9
        Height = 20
        Caption = '8'
      end
      object Label16: TLabel
        Left = 21
        Top = 433
        Width = 9
        Height = 20
        Caption = '9'
      end
      object Label17: TLabel
        Left = 19
        Top = 467
        Width = 18
        Height = 20
        Caption = '10'
      end
      object Edit1: TEdit
        Left = 304
        Top = 8
        Width = 145
        Height = 28
        TabOrder = 0
        Text = '172.16.8.109'
      end
      object ReqData: TEdit
        Left = 136
        Top = 39
        Width = 241
        Height = 28
        TabOrder = 1
        Text = '010307D0000A'
      end
      object ResData: TEdit
        Left = 136
        Top = 72
        Width = 241
        Height = 28
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 2
      end
      object ActStatus: TEdit
        Left = 136
        Top = 104
        Width = 241
        Height = 28
        Color = clBtnFace
        ReadOnly = True
        TabOrder = 3
      end
      object Edit4: TEdit
        Left = 200
        Top = 159
        Width = 137
        Height = 28
        TabOrder = 4
      end
      object Edit5: TEdit
        Left = 360
        Top = 159
        Width = 137
        Height = 28
        TabOrder = 5
      end
      object Edit7: TEdit
        Left = 200
        Top = 193
        Width = 137
        Height = 28
        TabOrder = 6
      end
      object Edit8: TEdit
        Left = 360
        Top = 193
        Width = 137
        Height = 28
        TabOrder = 7
      end
      object Edit10: TEdit
        Left = 200
        Top = 227
        Width = 137
        Height = 28
        TabOrder = 8
      end
      object Edit11: TEdit
        Left = 360
        Top = 227
        Width = 137
        Height = 28
        TabOrder = 9
      end
      object Edit13: TEdit
        Left = 200
        Top = 260
        Width = 137
        Height = 28
        TabOrder = 10
      end
      object Edit14: TEdit
        Left = 360
        Top = 260
        Width = 137
        Height = 28
        TabOrder = 11
      end
      object Edit16: TEdit
        Left = 200
        Top = 294
        Width = 137
        Height = 28
        TabOrder = 12
      end
      object Edit17: TEdit
        Left = 360
        Top = 294
        Width = 137
        Height = 28
        TabOrder = 13
      end
      object Edit19: TEdit
        Left = 200
        Top = 328
        Width = 137
        Height = 28
        TabOrder = 14
      end
      object Edit20: TEdit
        Left = 360
        Top = 328
        Width = 137
        Height = 28
        TabOrder = 15
      end
      object Edit22: TEdit
        Left = 200
        Top = 362
        Width = 137
        Height = 28
        TabOrder = 16
      end
      object Edit23: TEdit
        Left = 360
        Top = 362
        Width = 137
        Height = 28
        TabOrder = 17
      end
      object Edit25: TEdit
        Left = 200
        Top = 395
        Width = 137
        Height = 28
        TabOrder = 18
      end
      object Edit26: TEdit
        Left = 360
        Top = 395
        Width = 137
        Height = 28
        TabOrder = 19
      end
      object Edit28: TEdit
        Left = 200
        Top = 429
        Width = 137
        Height = 28
        TabOrder = 20
      end
      object Edit29: TEdit
        Left = 360
        Top = 429
        Width = 137
        Height = 28
        TabOrder = 21
      end
      object Edit31: TEdit
        Left = 200
        Top = 463
        Width = 137
        Height = 28
        TabOrder = 22
      end
      object Edit32: TEdit
        Left = 360
        Top = 463
        Width = 137
        Height = 28
        TabOrder = 23
      end
      object Edit3: TEdit
        Left = 40
        Top = 159
        Width = 137
        Height = 28
        TabOrder = 24
      end
      object Edit6: TEdit
        Left = 40
        Top = 193
        Width = 137
        Height = 28
        TabOrder = 25
      end
      object Edit9: TEdit
        Left = 40
        Top = 227
        Width = 137
        Height = 28
        TabOrder = 26
      end
      object Edit12: TEdit
        Left = 40
        Top = 260
        Width = 137
        Height = 28
        TabOrder = 27
      end
      object Edit15: TEdit
        Left = 40
        Top = 294
        Width = 137
        Height = 28
        TabOrder = 28
      end
      object Edit18: TEdit
        Left = 40
        Top = 328
        Width = 137
        Height = 28
        TabOrder = 29
      end
      object Edit21: TEdit
        Left = 40
        Top = 362
        Width = 137
        Height = 28
        TabOrder = 30
      end
      object Edit24: TEdit
        Left = 40
        Top = 395
        Width = 137
        Height = 28
        TabOrder = 31
      end
      object Edit27: TEdit
        Left = 40
        Top = 429
        Width = 137
        Height = 28
        TabOrder = 32
      end
      object Edit30: TEdit
        Left = 40
        Top = 463
        Width = 137
        Height = 28
        TabOrder = 33
      end
      object cbEnableView: TCheckBox
        Left = 3
        Top = 10
        Width = 238
        Height = 19
        Caption = 'Enable View function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 34
      end
      object Memo2: TMemo
        Left = 0
        Top = 528
        Width = 651
        Height = 30
        Align = alBottom
        TabOrder = 35
      end
      object Edit34: TEdit
        Left = 40
        Top = 495
        Width = 137
        Height = 28
        TabOrder = 36
      end
      object Edit35: TEdit
        Left = 200
        Top = 495
        Width = 137
        Height = 28
        TabOrder = 37
      end
      object Edit36: TEdit
        Left = 360
        Top = 495
        Width = 137
        Height = 28
        TabOrder = 38
      end
      object GroupBox2: TGroupBox
        Left = 512
        Top = 152
        Width = 137
        Height = 217
        Caption = 'Test Mode'
        TabOrder = 39
        object Button1: TButton
          Left = 8
          Top = 24
          Width = 120
          Height = 25
          Caption = 'Write data'
          TabOrder = 0
          OnClick = Button1Click
        end
        object Button3: TButton
          Left = 8
          Top = 56
          Width = 120
          Height = 25
          Caption = 'Read data'
          TabOrder = 1
          OnClick = Button3Click
        end
        object Button4: TButton
          Left = 8
          Top = 88
          Width = 120
          Height = 25
          Caption = 'Clean data'
          TabOrder = 2
          OnClick = Button4Click
        end
        object Button2: TButton
          Left = 8
          Top = 144
          Width = 120
          Height = 25
          Caption = 'Close Connect'
          TabOrder = 3
          OnClick = Button2Click
        end
        object Button9: TButton
          Left = 8
          Top = 176
          Width = 120
          Height = 25
          Caption = 'start'
          TabOrder = 4
          Visible = False
          OnClick = Button9Click
        end
      end
    end
    object tsE84: TTabSheet
      Caption = 'E84 Sensor'
      ImageIndex = 2
      object gbE84TimeOut: TGroupBox
        Left = 8
        Top = 16
        Width = 185
        Height = 265
        Caption = 'E84 Time Out Setting'
        TabOrder = 0
        object labTP1: TLabel
          Left = 8
          Top = 24
          Width = 28
          Height = 20
          Caption = 'TP1'
        end
        object labTP2: TLabel
          Left = 8
          Top = 64
          Width = 28
          Height = 20
          Caption = 'TP2'
        end
        object labTP3: TLabel
          Left = 8
          Top = 104
          Width = 28
          Height = 20
          Caption = 'TP3'
        end
        object labTP4: TLabel
          Left = 8
          Top = 144
          Width = 28
          Height = 20
          Caption = 'TP4'
        end
        object labTP5: TLabel
          Left = 8
          Top = 184
          Width = 28
          Height = 20
          Caption = 'TP5'
        end
        object labTP6: TLabel
          Left = 8
          Top = 224
          Width = 28
          Height = 20
          Caption = 'TP6'
        end
        object edTP1: TEdit
          Left = 72
          Top = 20
          Width = 49
          Height = 28
          TabOrder = 0
          Text = '2'
        end
        object edTP2: TEdit
          Left = 72
          Top = 60
          Width = 49
          Height = 28
          TabOrder = 1
          Text = '2'
        end
        object edTP3: TEdit
          Left = 72
          Top = 100
          Width = 49
          Height = 28
          TabOrder = 2
          Text = '60'
        end
        object edTP4: TEdit
          Left = 72
          Top = 140
          Width = 49
          Height = 28
          TabOrder = 3
          Text = '60'
        end
        object edTP5: TEdit
          Left = 72
          Top = 180
          Width = 49
          Height = 28
          TabOrder = 4
          Text = '2'
        end
        object edTP6: TEdit
          Left = 72
          Top = 220
          Width = 49
          Height = 28
          TabOrder = 5
          Text = '2'
        end
      end
      object gbE84Log: TGroupBox
        Left = 408
        Top = 0
        Width = 243
        Height = 558
        Align = alRight
        Caption = 'E84 Log'
        TabOrder = 1
        object Memo3: TMemo
          Left = 2
          Top = 22
          Width = 239
          Height = 534
          Align = alClient
          TabOrder = 0
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 593
    Width = 659
    Height = 44
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object spbSave: TSpeedButton
      Left = 46
      Top = 4
      Width = 227
      Height = 40
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
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
      ParentFont = False
      OnClick = spbSaveClick
    end
    object sbtExit: TSpeedButton
      Left = 396
      Top = 3
      Width = 241
      Height = 41
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        0E060000424D0E06000000000000360000002800000016000000160000000100
        180000000000D805000001000000010000000000000000000000EFEFEFEFEFEF
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
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF7474A4
        4C4C946C6CA4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF6C6CA44C4C947474
        A4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EF2C2C8C5454C45454C4645C9CEFEFEFEFEFEFEFEFEFEFEFEF645C9C3C3CAC6C
        6CDC4C4C94EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEF8C8CAC2424946464D45454C4645C9CEFEFEFEFEFEF645C9C3C3CAC
        6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEF8C8CAC2424946464D45454C4645C9C645C9C3C3C
        AC6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC2424946464D45454C43C
        3CAC6C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC242494
        6464D46C6CDC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF645C
        9C3C3CAC6C6CDC6464D45454C4645C9CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF64
        5C9C3C3CAC6C6CDC2C2C9C2424946464D45454C4645C9CEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        645C9C3C3CAC6C6CDC2C2C9C8C8CAC8C8CAC2424946464D45454C4645C9CEFEF
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EF6C6CA43C3CAC6C6CDC2C2C9C8C8CACEFEFEFEFEFEF8C8CAC2424946464D454
        54C46C6CA4EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEF2C2C8C4C4CBC2C2C9C8C8CACEFEFEFEFEFEFEFEFEFEFEFEF8C8CAC
        2424945454C44C4C94EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEFEFEFEFEFEF8C8CAC3434948C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEF
        EFEFEFEF8C8CAC3434948C8CACEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
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
        EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF}
      ParentFont = False
      OnClick = sbtExitClick
    end
  end
  object RFID_1: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = True
    Inx_XonXoffFlow = True
    ReplaceWhenParityError = False
    IgnoreNullChar = False
    RtsControl = RtsEnable
    XonLimit = 500
    XoffLimit = 500
    ByteSize = _8
    Parity = None
    StopBits = _1
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 10
    ReadTotalTimeoutConstant = 1000
    WriteTotalTimeoutMultiplier = 10
    WriteTotalTimeoutConstant = 1000
    OnReceiveData = RFID_1ReceiveData
    Left = 536
    Top = 36
  end
  object RFID_2: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = True
    Inx_XonXoffFlow = True
    ReplaceWhenParityError = False
    IgnoreNullChar = False
    RtsControl = RtsEnable
    XonLimit = 500
    XoffLimit = 500
    ByteSize = _8
    Parity = None
    StopBits = _1
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 10
    ReadTotalTimeoutConstant = 1000
    WriteTotalTimeoutMultiplier = 10
    WriteTotalTimeoutConstant = 1000
    OnReceiveData = RFID_2ReceiveData
    Left = 565
    Top = 36
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 616
    Top = 32
  end
end
