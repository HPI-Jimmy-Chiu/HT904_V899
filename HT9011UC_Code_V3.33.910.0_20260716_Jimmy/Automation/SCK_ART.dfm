object fSCKART: TfSCKART
  Left = 542
  Top = 58
  Align = alRight
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'Check QTY'
  ClientHeight = 1002
  ClientWidth = 798
  Color = 12761254
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
  object palARTLeft: TPanel
    Left = 0
    Top = 0
    Width = 402
    Height = 1002
    Align = alClient
    BevelInner = bvRaised
    BevelOuter = bvLowered
    Color = 12761254
    TabOrder = 0
    object palARTCount: TPanel
      Left = 2
      Top = 2
      Width = 398
      Height = 591
      Align = alTop
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 0
      object gbCountInfo: TGroupBox
        Left = 0
        Top = 229
        Width = 398
        Height = 200
        Align = alTop
        Caption = 'Count Info'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        object lblInputPickCount: TLabel
          Left = 5
          Top = 56
          Width = 91
          Height = 13
          Caption = 'Input Pickup Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblInShtCount: TLabel
          Left = 5
          Top = 97
          Width = 91
          Height = 13
          Caption = 'Input Shuttle Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblOutCnt: TLabel
          Left = 5
          Top = 119
          Width = 99
          Height = 13
          Caption = 'Output Loader Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblMergeCount: TLabel
          Left = 5
          Top = 160
          Width = 129
          Height = 13
          Caption = 'Merged(Auto) Reject Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblGap: TLabel
          Left = 198
          Top = 17
          Width = 20
          Height = 13
          Caption = 'Gap'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblInputQty: TLabel
          Left = 5
          Top = 34
          Width = 58
          Height = 13
          Caption = 'INPUT QTY'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblInputJamCount: TLabel
          Left = 5
          Top = 77
          Width = 77
          Height = 13
          Caption = 'Input Jam Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblOutputJamCount: TLabel
          Left = 5
          Top = 140
          Width = 85
          Height = 13
          Caption = 'Output Jam Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object btnApplyQty: TSpeedButton
          Left = 236
          Top = 52
          Width = 121
          Height = 29
          Caption = 'Apply QTY'
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
          OnClick = btnApplyQtyClick
        end
        object lblManul: TLabel
          Left = 5
          Top = 180
          Width = 99
          Height = 13
          Caption = 'Manual Reject count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object palRejectCnt: TPanel
          Left = 136
          Top = 156
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object palOutputCnt: TPanel
          Left = 136
          Top = 115
          Width = 50
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object palInShtCnt: TPanel
          Left = 136
          Top = 93
          Width = 50
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object palInputCnt: TPanel
          Left = 136
          Top = 52
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object palOutputGap: TPanel
          Left = 186
          Top = 115
          Width = 50
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object palInShtCntGap: TPanel
          Left = 186
          Top = 93
          Width = 50
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object palInputCount: TPanel
          Left = 136
          Top = 30
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnMouseDown = palInputCountMouseDown
        end
        object palInputJamCount: TPanel
          Left = 136
          Top = 73
          Width = 100
          Height = 19
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object palOutputJamCount: TPanel
          Left = 136
          Top = 136
          Width = 100
          Height = 19
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object edtInputCount: TEdit
          Left = 236
          Top = 30
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = edLotCountClick
        end
        object edtMRejectCnt: TEdit
          Left = 236
          Top = 174
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          OnClick = edtMRejectCntClick
        end
        object palManualRejectCnt: TPanel
          Left = 136
          Top = 176
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
          OnMouseDown = palInputCountMouseDown
        end
      end
      object gbLotStatus: TGroupBox
        Left = 0
        Top = 0
        Width = 398
        Height = 229
        Align = alTop
        Caption = 'Lot Info'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object lblTestCount: TLabel
          Left = 5
          Top = 134
          Width = 52
          Height = 13
          Caption = 'Test Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblLotNumber: TLabel
          Left = 5
          Top = 45
          Width = 55
          Height = 13
          Caption = 'Lot Number'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblMesInputQty: TLabel
          Left = 5
          Top = 24
          Width = 116
          Height = 13
          Caption = 'INPUT QTY (From MES)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblHandlerStatus: TLabel
          Left = 5
          Top = 68
          Width = 70
          Height = 13
          Caption = 'Handler Status'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblTesterType: TLabel
          Left = 5
          Top = 90
          Width = 57
          Height = 13
          Caption = 'Tester Type'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblRTTryCount: TLabel
          Left = 5
          Top = 111
          Width = 64
          Height = 13
          Caption = 'RT Try Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblPassRate: TLabel
          Left = 5
          Top = 157
          Width = 49
          Height = 13
          Caption = 'Pass Rate'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblCurrentRate: TLabel
          Left = 5
          Top = 180
          Width = 60
          Height = 13
          Caption = 'Current Rate'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object btnApplyLotInfo: TSpeedButton
          Left = 235
          Top = 176
          Width = 121
          Height = 25
          Caption = 'Apply Lot'
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
          OnClick = btnApplyLotInfoClick
        end
        object lblProcessCode: TLabel
          Left = 5
          Top = 202
          Width = 66
          Height = 13
          Caption = 'Process Code'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object palTestCnt: TPanel
          Left = 136
          Top = 130
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object palLotNumber: TPanel
          Left = 68
          Top = 42
          Width = 168
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object palInputQty: TPanel
          Left = 140
          Top = 17
          Width = 96
          Height = 25
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object palLotStatus: TPanel
          Left = 136
          Top = 64
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object palTesterType: TPanel
          Left = 136
          Top = 86
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object palRTTryCnt: TPanel
          Left = 136
          Top = 108
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object cbLotStatus: TComboBox
          Left = 236
          Top = 62
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 13
          ItemIndex = 0
          ParentFont = False
          TabOrder = 3
          Text = 'NONE'
          Items.Strings = (
            'NONE'
            'LOTSTATUS_W'
            'LOTSTATUS_T'
            'LOTSTATUS_L'
            'LOTSTATUS_R'
            'LOTSTATUS_F'
            'LOTSTATUS_A')
        end
        object palARTPassRate: TPanel
          Left = 136
          Top = 153
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object edlRTTryCnt: TEdit
          Left = 237
          Top = 108
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          OnClick = edlRTTryCntClick
        end
        object edARTPassRate: TEdit
          Left = 236
          Top = 153
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
          OnClick = edARTPassRateClick
        end
        object palCurrentRate: TPanel
          Left = 136
          Top = 176
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object edLotCount: TEdit
          Left = 236
          Top = 20
          Width = 110
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = edLotCountClick
        end
        object pnlProcessCode: TPanel
          Left = 136
          Top = 198
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
      end
      object gbUserFix: TGroupBox
        Left = 0
        Top = 429
        Width = 398
        Height = 150
        Align = alTop
        Caption = 'User Fix'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        object lblInputJamCountFix: TLabel
          Left = 5
          Top = 24
          Width = 77
          Height = 13
          Caption = 'Input Jam Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblInputRemoveCountFix: TLabel
          Left = 5
          Top = 46
          Width = 107
          Height = 13
          Caption = 'Remove Device(Input)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblOutputJamCountFix: TLabel
          Left = 5
          Top = 68
          Width = 85
          Height = 13
          Caption = 'Output Jam Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblOutputRemoveCountFix: TLabel
          Left = 5
          Top = 90
          Width = 115
          Height = 13
          Caption = 'Remove Device(Output)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object btnApplyCount: TSpeedButton
          Left = 5
          Top = 108
          Width = 164
          Height = 37
          Caption = 'Apply Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
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
          OnClick = btnApplyCountClick
        end
        object btnExit1: TSpeedButton
          Left = 180
          Top = 108
          Width = 149
          Height = 37
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Exit'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -21
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
          OnClick = btnExitClick
        end
        object palInRemoveCnt: TPanel
          Left = 136
          Top = 42
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = palInputJamCntClick
        end
        object palInputJamCnt: TPanel
          Left = 136
          Top = 20
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = palInputJamCntClick
        end
        object palOutRemoveCnt: TPanel
          Left = 136
          Top = 86
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = palInputJamCntClick
        end
        object palOutputJamCnt: TPanel
          Left = 136
          Top = 64
          Width = 100
          Height = 20
          BevelInner = bvLowered
          Caption = '0'
          Color = clWhite
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = palInputJamCntClick
        end
        object edTemp: TEdit
          Left = 236
          Top = 84
          Width = 29
          Height = 21
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Visible = False
          OnClick = edtMRejectCntClick
        end
      end
    end
    object pgARTFlow: TPageControl
      Left = 2
      Top = 605
      Width = 398
      Height = 395
      ActivePage = ts93KFlow
      Align = alBottom
      TabIndex = 0
      TabOrder = 1
      object ts93KFlow: TTabSheet
        Caption = '93KFlow'
        object grp93KARTFlow: TGroupBox
          Left = 0
          Top = 0
          Width = 390
          Height = 367
          Align = alClient
          Caption = 'ART Flow'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object aledInitART: TALed
            Left = 183
            Top = 22
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledFTLotStart: TALed
            Left = 183
            Top = 47
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledWaitingTestercommand1: TALed
            Left = 183
            Top = 73
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledFTTesting: TALed
            Left = 183
            Top = 99
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledFTLotEnd: TALed
            Left = 183
            Top = 125
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledWaitingTestercommand2: TALed
            Left = 183
            Top = 150
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledRTLotStart: TALed
            Left = 183
            Top = 199
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledWaitingTestercommand3: TALed
            Left = 183
            Top = 225
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledRTTesting: TALed
            Left = 183
            Top = 250
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledRTLotEnd: TALed
            Left = 183
            Top = 275
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledWaitingTestercommand4: TALed
            Left = 183
            Top = 301
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledARTFinish: TALed
            Left = 183
            Top = 326
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object aledMoveTrayToLoader: TALed
            Left = 183
            Top = 175
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledShowFTCTData: TALed
            Left = 11
            Top = 344
            Width = 22
            Height = 14
            FalseColor = clRed
            LEDStyle = LEDHorizontal
            OnClick = ledShowFTCTDataClick
          end
          object paInitART: TPanel
            Left = 12
            Top = 19
            Width = 161
            Height = 20
            Caption = 'Init ART'
            Color = 5812224
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object paFTLotStart: TPanel
            Left = 12
            Top = 44
            Width = 161
            Height = 20
            Caption = 'FT Lot Start'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object paWaitingTestercommand1: TPanel
            Left = 12
            Top = 70
            Width = 161
            Height = 20
            Caption = 'Waiting Tester command'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object paFTTesting: TPanel
            Left = 12
            Top = 96
            Width = 161
            Height = 20
            Caption = 'FT Testing'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object paFTLotEnd: TPanel
            Left = 12
            Top = 122
            Width = 161
            Height = 20
            Caption = 'FT Lot End'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object paWaitingTestercommand2: TPanel
            Left = 12
            Top = 147
            Width = 161
            Height = 20
            Caption = 'Waiting Tester command'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object paRTLotStart: TPanel
            Left = 12
            Top = 196
            Width = 161
            Height = 20
            Caption = 'RT Lot Start'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object paWaitingTestercommand3: TPanel
            Left = 12
            Top = 222
            Width = 161
            Height = 20
            Caption = 'Waiting Tester command'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object paRTTesting: TPanel
            Left = 12
            Top = 247
            Width = 161
            Height = 20
            Caption = 'RT Testing'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object paRTLotEnd: TPanel
            Left = 12
            Top = 272
            Width = 161
            Height = 20
            Caption = 'RT Lot End'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object paWaitingTestercommand4: TPanel
            Left = 12
            Top = 298
            Width = 161
            Height = 20
            Caption = 'Waiting Tester command'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object paARTFinish: TPanel
            Left = 12
            Top = 323
            Width = 161
            Height = 20
            Caption = 'ART Finish'
            Color = 5812224
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object paARTMoveTray: TPanel
            Left = 12
            Top = 172
            Width = 161
            Height = 20
            Caption = 'Move Tray to Loader'
            Color = 5812224
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
        end
      end
      object tsFlexFlow: TTabSheet
        Caption = 'FlexFlow'
        ImageIndex = 1
        object grpFlexARTFlow: TGroupBox
          Left = 0
          Top = 0
          Width = 390
          Height = 367
          Align = alClient
          Caption = 'ART Flow'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object ledFlexInitART: TALed
            Left = 88
            Top = 22
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexFTQTY: TALed
            Left = 88
            Top = 47
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexLotClear: TALed
            Left = 88
            Top = 72
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexLotQty: TALed
            Left = 88
            Top = 97
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexFTTesting: TALed
            Left = 88
            Top = 122
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexFTStsL: TALed
            Left = 88
            Top = 147
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexFTAlarm: TALed
            Left = 88
            Top = 172
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexFTStsR: TALed
            Left = 88
            Top = 197
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexLotRTClear: TALed
            Left = 115
            Top = 72
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexRTTesting: TALed
            Left = 115
            Top = 122
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexRTStsL: TALed
            Left = 115
            Top = 147
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexRTAlarm: TALed
            Left = 115
            Top = 172
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexRTStsR: TALed
            Left = 115
            Top = 197
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexStsF: TALed
            Left = 115
            Top = 221
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object ledFlexRTQTY: TALed
            Left = 115
            Top = 47
            Width = 22
            Height = 14
            LEDStyle = LEDHorizontal
          end
          object palFlexInitART: TPanel
            Left = 4
            Top = 19
            Width = 80
            Height = 20
            Caption = 'Init ART'
            Color = 5812224
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object palFlexFTQTY: TPanel
            Left = 4
            Top = 44
            Width = 80
            Height = 20
            Caption = 'QTY?'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object palFlexLotClear: TPanel
            Left = 4
            Top = 69
            Width = 80
            Height = 20
            Caption = 'LOTCLEAR?'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object palFlexLotQty: TPanel
            Left = 4
            Top = 94
            Width = 80
            Height = 20
            Caption = 'INPUTQTY'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object palFlexFTTesting: TPanel
            Left = 4
            Top = 119
            Width = 80
            Height = 20
            Caption = 'FT Testing'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object palFlexFTStsL: TPanel
            Left = 4
            Top = 144
            Width = 80
            Height = 20
            Caption = 'LOTSTATUS_L'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object palFlexFTAlarm: TPanel
            Left = 4
            Top = 169
            Width = 80
            Height = 20
            Caption = 'ALARM?'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object palFlexFTStsR: TPanel
            Left = 4
            Top = 194
            Width = 80
            Height = 20
            Caption = 'LOTSTATUS_R'
            Color = clSkyBlue
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -8
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object palFlexLotRTClear: TPanel
            Left = 141
            Top = 69
            Width = 80
            Height = 20
            Caption = 'LOTRTCLEAR?'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object palFlexRTTesting: TPanel
            Left = 141
            Top = 119
            Width = 80
            Height = 20
            Caption = 'RT Testing'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object palFlexRTStsL: TPanel
            Left = 141
            Top = 144
            Width = 80
            Height = 20
            Caption = 'LOTSTATUS_L'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object palFlexRTAlarm: TPanel
            Left = 141
            Top = 169
            Width = 80
            Height = 20
            Caption = 'ALARM?'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object palFlexRTStsR: TPanel
            Left = 141
            Top = 194
            Width = 80
            Height = 20
            Caption = 'LOTSTATUS_R'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object palFlexStsF: TPanel
            Left = 141
            Top = 218
            Width = 80
            Height = 20
            Caption = 'LOTSTATUS_F'
            Color = 5812224
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object palFlexRTQTY: TPanel
            Left = 141
            Top = 44
            Width = 80
            Height = 20
            Caption = 'QTY?'
            Color = 13606558
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
        end
      end
    end
  end
  object palARTSetting: TPanel
    Left = 402
    Top = 0
    Width = 396
    Height = 1002
    Align = alRight
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object btnApplySetting: TSpeedButton
      Left = 4
      Top = 842
      Width = 380
      Height = 37
      Caption = 'Apply Setting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
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
      OnClick = btnApplySettingClick
    end
    object btnExit: TSpeedButton
      Left = 4
      Top = 882
      Width = 380
      Height = 37
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -21
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
      OnClick = btnExitClick
    end
    object gbARTMode: TGroupBox
      Left = 0
      Top = 157
      Width = 396
      Height = 236
      Align = alTop
      Caption = 'ART Mode'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object lblSortMethod: TLabel
        Left = 9
        Top = 28
        Width = 92
        Height = 20
        Caption = 'Sort method '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblRTStartMode: TLabel
        Left = 9
        Top = 62
        Width = 101
        Height = 20
        Caption = 'RT start mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblYield: TLabel
        Left = 120
        Top = 133
        Width = 63
        Height = 20
        Caption = 'Yield (%)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object cbSortMode: TComboBox
        Left = 188
        Top = 24
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 20
        ItemIndex = 1
        ParentFont = False
        TabOrder = 0
        Text = 'Category R'
        Items.Strings = (
          'Assigned Category'
          'Category R')
      end
      object cbRTStartMode: TComboBox
        Left = 188
        Top = 58
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 20
        ItemIndex = 0
        ParentFont = False
        TabOrder = 1
        Text = 'Auto progress'
        Items.Strings = (
          'Auto progress'
          'Manual progress')
      end
      object cbSPBinAlarm: TCheckBox
        Left = 9
        Top = 97
        Width = 151
        Height = 17
        Caption = 'Special Bin Alarm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object cbSPBinSelect: TComboBox
        Left = 188
        Top = 92
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 20
        ParentFont = False
        TabOrder = 2
        Text = 'Bin Select'
        Items.Strings = (
          'Bin0'
          'Bin1'
          'Bin2'
          'Bin3'
          'Bin4'
          'Bin5'
          'Bin6'
          'Bin7'
          'Bin8'
          'Bin9'
          'Bin10'
          'Bin11'
          'Bin12'
          'Bin13'
          'Bin14'
          'Bin15'
          'Bin16')
      end
      object edSPBinYield: TEdit
        Left = 188
        Top = 127
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Text = '5'
        OnClick = edSPBinYieldClick
      end
      object chkLowYieldForFT: TCheckBox
        Left = 9
        Top = 173
        Width = 176
        Height = 17
        Caption = 'Low Yield Limit for FT'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object edtLowYieldForFT: TEdit
        Left = 188
        Top = 167
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Text = '5'
        OnClick = edSPBinYieldClick
      end
      object chkAlarmKIND2: TCheckBox
        Left = 9
        Top = 204
        Width = 284
        Height = 17
        Caption = 'Alarm after send SRQKIND2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
    end
    object gbSocketOff: TGroupBox
      Left = 0
      Top = 393
      Width = 396
      Height = 144
      Align = alTop
      Caption = 'Socket Off'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 2
      object lblSocketOffMode: TLabel
        Left = 9
        Top = 49
        Width = 89
        Height = 20
        Caption = 'Select mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblBestYieldRate: TLabel
        Left = 9
        Top = 81
        Width = 124
        Height = 20
        Caption = 'Best yield rate(%)'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblAlmSiteLess: TLabel
        Left = 9
        Top = 113
        Width = 227
        Height = 20
        Caption = 'Alarm if auto close site less than'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object cbRTOffMode: TComboBox
        Left = 188
        Top = 45
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 20
        ItemIndex = 0
        ParentFont = False
        TabOrder = 1
        Text = 'Auto Head'
        Items.Strings = (
          'Auto Head'
          'Auto Socket'
          'Manual')
      end
      object edBestYield: TEdit
        Left = 188
        Top = 78
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = edSPBinYieldClick
      end
      object cbAutoSocketOff: TCheckBox
        Left = 9
        Top = 25
        Width = 229
        Height = 17
        Caption = 'RT auto socket off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object edtAlmAutoCloseSite: TEdit
        Left = 236
        Top = 111
        Width = 152
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        Text = '5'
        OnClick = edtAlmAutoCloseSiteClick
      end
    end
    object gdAlarmList: TGroupBox
      Left = 0
      Top = 919
      Width = 396
      Height = 83
      Align = alBottom
      Caption = 'Alarm List'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      object memoAlarmList: TMemo
        Left = 2
        Top = 22
        Width = 392
        Height = 59
        Align = alClient
        Color = 14670284
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object btnDeleteLotInfo: TButton
      Left = 4
      Top = 815
      Width = 149
      Height = 25
      Caption = 'Delete Lot Info'
      TabOrder = 6
      Visible = False
      OnClick = btnDeleteLotInfoClick
    end
    object grpARTSetting: TGroupBox
      Left = 0
      Top = 0
      Width = 396
      Height = 157
      Align = alTop
      Caption = 'ART Setting'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      object chkEnableART: TCheckBox
        Left = 12
        Top = 40
        Width = 209
        Height = 17
        Caption = 'Enable ART'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object edtVersion: TEdit
        Left = 12
        Top = 85
        Width = 376
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        Text = 'ADVANTEST M4871  Rev. 1.1P3 FULLSITEB'
        OnMouseUp = edtVersionMouseUp
      end
      object chkVersion: TCheckBox
        Left = 12
        Top = 64
        Width = 333
        Height = 17
        Caption = 'Over write VERSION? command'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        Visible = False
      end
      object chkNoTesterCmd: TCheckBox
        Left = 12
        Top = 124
        Width = 349
        Height = 17
        Caption = 'Run ART without tester command'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 3
      end
      object chkEnableFTCT: TCheckBox
        Left = 168
        Top = 40
        Width = 149
        Height = 17
        Caption = 'Enable FT-CT'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 4
        Visible = False
      end
      object btnFTCTReset: TButton
        Left = 320
        Top = 36
        Width = 45
        Height = 25
        Caption = 'Reset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Visible = False
        OnClick = btnFTCTResetClick
      end
    end
    object gbAutoSkip: TGroupBox
      Left = 0
      Top = 669
      Width = 396
      Height = 80
      Align = alTop
      Caption = 'Auto Tray Skip'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 4
      object lblAutoSkipCount: TLabel
        Left = 9
        Top = 50
        Width = 162
        Height = 20
        Caption = 'Loader auto skip count'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edARTAutoSkip: TEdit
        Left = 188
        Top = 46
        Width = 200
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Text = '5'
        OnClick = edARTAutoSkipClick
      end
      object cbAutoSkip: TCheckBox
        Left = 12
        Top = 24
        Width = 229
        Height = 17
        Caption = 'Enable auto tray skip function'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
    object gbDevice: TGroupBox
      Left = 0
      Top = 537
      Width = 396
      Height = 132
      Align = alTop
      Caption = 'Devices'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 3
      object lblLdCntLimP: TLabel
        Left = 301
        Top = 96
        Width = 14
        Height = 29
        Caption = '+'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object lblLdCntLimN: TLabel
        Left = 229
        Top = 96
        Width = 8
        Height = 29
        Caption = '-'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object cbAutoCleanOut: TCheckBox
        Left = 12
        Top = 28
        Width = 209
        Height = 17
        Caption = 'Auto clean out'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object cbAutoLotEnd: TCheckBox
        Left = 12
        Top = 52
        Width = 209
        Height = 17
        Caption = 'Auto lot end'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object cbFinalLotEndMsg: TCheckBox
        Left = 12
        Top = 76
        Width = 209
        Height = 17
        Caption = 'Final lot end message'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        Visible = False
      end
      object cbLotDeviceCheck: TCheckBox
        Left = 13
        Top = 101
        Width = 152
        Height = 17
        Caption = 'Lot device check'
        Checked = True
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        State = cbChecked
        TabOrder = 3
      end
      object edtLdCntLimP: TEdit
        Left = 316
        Top = 97
        Width = 49
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Text = '0'
        Visible = False
        OnClick = edtLdCntLimNClick
      end
      object edtLdCntLimN: TEdit
        Left = 244
        Top = 97
        Width = 49
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Text = '0'
        Visible = False
        OnClick = edtLdCntLimNClick
      end
    end
    object gbOutputLoader: TGroupBox
      Left = 0
      Top = 749
      Width = 396
      Height = 64
      Align = alTop
      Caption = 'ART Output Count'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 5
      object lblAuto1: TLabel
        Left = 27
        Top = 22
        Width = 31
        Height = 13
        Caption = 'Auto 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblAuto2: TLabel
        Left = 105
        Top = 22
        Width = 31
        Height = 13
        Caption = 'Auto 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblAuto3: TLabel
        Left = 192
        Top = 22
        Width = 31
        Height = 13
        Caption = 'Auto 3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object palAutoTrayCnt1: TPanel
        Left = 12
        Top = 36
        Width = 60
        Height = 20
        BevelInner = bvLowered
        Caption = '0'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object palAutoTrayCnt2: TPanel
        Left = 90
        Top = 36
        Width = 60
        Height = 20
        BevelInner = bvLowered
        Caption = '0'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object palAutoTrayCnt3: TPanel
        Left = 177
        Top = 36
        Width = 60
        Height = 20
        BevelInner = bvLowered
        Caption = '0'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
    end
  end
  object TimerSCKARTFlow: TTimer
    Enabled = False
    OnTimer = TimerSCKARTFlowTimer
    Left = 332
    Top = 428
  end
  object srvrscktTSV: TServerSocket
    Active = False
    Port = 0
    ServerType = stNonBlocking
    OnClientRead = srvrscktTSVClientRead
    Left = 712
    Top = 16
  end
  object TimerTSV: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerTSVTimer
    Left = 332
    Top = 456
  end
end
