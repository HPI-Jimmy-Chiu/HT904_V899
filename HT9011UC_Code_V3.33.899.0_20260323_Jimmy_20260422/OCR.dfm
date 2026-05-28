object fOCR: TfOCR
  Left = 777
  Top = 123
  Width = 870
  Height = 662
  Caption = 'OCR'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 854
    Height = 623
    ActivePage = tsOCR_Cognex_Setting
    Align = alClient
    TabIndex = 2
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'TabSheet1'
      object palLeft: TPanel
        Left = 0
        Top = 0
        Width = 437
        Height = 592
        Align = alLeft
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object gbFileName: TGroupBox
          Left = 2
          Top = 0
          Width = 430
          Height = 49
          Caption = 'FileName'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object edFileName: TEdit
            Left = 8
            Top = 20
            Width = 410
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = 'edFileName'
          end
        end
        object gbInsp: TGroupBox
          Left = 2
          Top = 56
          Width = 430
          Height = 49
          Caption = 'Inspection Text'
          TabOrder = 1
          object edInsp: TEdit
            Left = 8
            Top = 16
            Width = 410
            Height = 24
            TabOrder = 0
            Text = 'edInsp'
          end
        end
        object btSend: TPanel
          Left = 0
          Top = 551
          Width = 437
          Height = 41
          Align = alBottom
          BevelInner = bvRaised
          BevelOuter = bvNone
          Caption = 'Send Command'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btSendClick
          OnMouseDown = btSendMouseDown
          OnMouseUp = btSendMouseUp
        end
        object rgCommadn: TRadioGroup
          Left = 0
          Top = 267
          Width = 437
          Height = 284
          Align = alBottom
          Caption = 'SendCommand'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Courier New'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            '#VISIONON+ //'#35426#21839#31995#32113#19978#32218
            '#START+    //Handler'#36890#30693#21855#21205#27298#28204
            '#END+      //Handler'#36890#30693#38364#38281#27298#28204
            '#FILE+     //Handler'#36890#30693#25563#26009#27284#21517
            '#CLEAR+    //Handler'#36890#30693#32080#25209#23436#25104
            '#INSP+     //Handler'#36865#20986#27604#23565#21629#20196#33287#23383#20018)
          ParentFont = False
          TabOrder = 4
        end
        object cbEnableOcr: TCheckBox
          Left = 4
          Top = 132
          Width = 261
          Height = 17
          Caption = 'Enable OCR Check'
          Enabled = False
          TabOrder = 2
        end
        object Button1: TButton
          Left = 56
          Top = 160
          Width = 75
          Height = 25
          Caption = 'LOTID?'
          TabOrder = 3
          OnClick = Button1Click
        end
      end
      object palRight: TPanel
        Left = 437
        Top = 0
        Width = 409
        Height = 592
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object memoOcrCommand: TMemo
          Left = 0
          Top = 442
          Width = 417
          Height = 158
          Align = alClient
          Color = 14670284
          TabOrder = 2
        end
        object gbMatchImage: TGroupBox
          Left = 0
          Top = 221
          Width = 417
          Height = 221
          Align = alTop
          Caption = 'Match Image'
          TabOrder = 1
          object imgMatch: TImage
            Left = 2
            Top = 18
            Width = 413
            Height = 201
            Align = alClient
            AutoSize = True
            Center = True
          end
        end
        object gbOCRImage: TGroupBox
          Left = 0
          Top = 0
          Width = 417
          Height = 221
          Align = alTop
          Caption = 'Golden Image'
          TabOrder = 0
          object imgOCR: TImage
            Left = 2
            Top = 18
            Width = 413
            Height = 201
            Align = alClient
            AutoSize = True
            Center = True
          end
        end
      end
    end
    object tsOCR_Cognex: TTabSheet
      Caption = 'OCR_Cognex'
      ImageIndex = 1
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 425
        Height = 364
        Align = alLeft
        Color = 12761254
        TabOrder = 0
        object Label14: TLabel
          Left = 12
          Top = 36
          Width = 67
          Height = 16
          Caption = 'IP Address '
        end
        object Label15: TLabel
          Left = 12
          Top = 68
          Width = 69
          Height = 16
          Caption = 'TCP/IP Port'
        end
        object spInternetConnect: TSpeedButton
          Left = 284
          Top = 27
          Width = 97
          Height = 25
          Caption = 'Connect'
          OnClick = spInternetConnectClick
        end
        object spInternetDisConnec: TSpeedButton
          Left = 284
          Top = 63
          Width = 97
          Height = 25
          Caption = 'DisConnect'
          OnClick = spInternetDisConnecClick
        end
        object IPAddress1: TEdit
          Left = 104
          Top = 30
          Width = 149
          Height = 24
          TabOrder = 0
          Text = '172.16.8.104'
        end
        object IPPort1: TEdit
          Left = 104
          Top = 62
          Width = 149
          Height = 24
          TabOrder = 1
          Text = '3000'
          OnMouseDown = IPPort1MouseDown
        end
        object Button2: TButton
          Left = 16
          Top = 131
          Width = 97
          Height = 25
          Caption = 'Reader  On'
          TabOrder = 2
          OnClick = Button2Click
        end
        object Button3: TButton
          Left = 127
          Top = 131
          Width = 97
          Height = 25
          Caption = 'Reader  Off'
          TabOrder = 3
          OnClick = Button3Click
        end
        object Button4: TButton
          Left = 248
          Top = 288
          Width = 75
          Height = 25
          Caption = 'Save Log'
          TabOrder = 4
          OnClick = Button4Click
        end
      end
      object Memo2: TMemo
        Left = 0
        Top = 364
        Width = 846
        Height = 228
        Align = alBottom
        TabOrder = 2
      end
      object Panel2: TPanel
        Left = 421
        Top = 0
        Width = 425
        Height = 364
        Align = alRight
        Color = 12761254
        TabOrder = 1
        object SpeedButton2: TSpeedButton
          Left = 224
          Top = 108
          Width = 97
          Height = 25
          Caption = 'Send'
          OnClick = SpeedButton2Click
        end
        object Label1: TLabel
          Left = 12
          Top = 36
          Width = 67
          Height = 16
          Caption = 'IP Address '
        end
        object Label2: TLabel
          Left = 12
          Top = 68
          Width = 69
          Height = 16
          Caption = 'TCP/IP Port'
        end
        object SpeedButton4: TSpeedButton
          Left = 324
          Top = 108
          Width = 97
          Height = 25
          Caption = 'Change File'
          OnClick = SpeedButton4Click
        end
        object RadioGroup1: TRadioGroup
          Left = 1
          Top = 132
          Width = 423
          Height = 119
          Align = alBottom
          Caption = 'Item'
          Items.Strings = (
            'User'#9'          admin + /r/n '
            'Password           /r/n'
            'Off-line'#9'          SO0 + /r/n'
            'Load Job File     LF + JobFileName + /r/n'
            'On-line'#9'          SO1 +/r/n')
          TabOrder = 3
          OnClick = RadioGroup1Click
        end
        object edtSendCmd: TEdit
          Left = 12
          Top = 108
          Width = 189
          Height = 24
          TabOrder = 2
          Text = '||>trigger on'
        end
        object Memo1: TMemo
          Left = 1
          Top = 251
          Width = 423
          Height = 112
          Align = alBottom
          TabOrder = 4
        end
        object IPAddress2: TEdit
          Left = 104
          Top = 30
          Width = 149
          Height = 24
          TabOrder = 0
          Text = '172.16.8.104'
        end
        object IPPort2: TEdit
          Left = 104
          Top = 62
          Width = 149
          Height = 24
          TabOrder = 1
          Text = '23'
          OnMouseDown = IPPort1MouseDown
        end
      end
    end
    object tsOCR_Cognex_Setting: TTabSheet
      Caption = 'OCR_Cognex_Setting'
      ImageIndex = 2
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 397
        Height = 503
        Align = alLeft
        Caption = 'OCR Setup'
        TabOrder = 0
        object Label85: TLabel
          Left = 12
          Top = 32
          Width = 82
          Height = 16
          Caption = 'No IC Count'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label86: TLabel
          Left = 10
          Top = 70
          Width = 88
          Height = 16
          Caption = 'Retest Count'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 10
          Top = 140
          Width = 80
          Height = 16
          Caption = 'Word Type'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edOCRRetry: TEdit
          Left = 116
          Top = 66
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
          OnClick = edOCRRetryClick
        end
        object edOCRSkip: TEdit
          Left = 114
          Top = 28
          Width = 45
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '2'
          OnClick = edOCRRetryClick
        end
        object edOCRWordCount: TEdit
          Left = 116
          Top = 104
          Width = 45
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '2'
          OnClick = edOCRRetryClick
        end
        object rgOCRTriggerMode: TRadioGroup
          Left = 219
          Top = 168
          Width = 170
          Height = 74
          Caption = 'OCR Trigger Mode'
          ItemIndex = 0
          Items.Strings = (
            'Switch Trigger'
            'Command Trigger(SE8)')
          TabOrder = 16
        end
        object FileListBox1: TFileListBox
          Left = 200
          Top = 392
          Width = 145
          Height = 97
          ItemHeight = 16
          TabOrder = 15
          Visible = False
        end
        object DirectoryListBox1: TDirectoryListBox
          Left = 24
          Top = 392
          Width = 145
          Height = 97
          FileList = FileListBox1
          ItemHeight = 16
          TabOrder = 14
          Visible = False
        end
        object ckDisabledKeyin: TCheckBox
          Left = 16
          Top = 168
          Width = 193
          Height = 17
          Caption = 'Disabled Skip IC Keyin'
          TabOrder = 4
        end
        object cbCheckBarCodeMap: TCheckBox
          Left = 16
          Top = 192
          Width = 193
          Height = 17
          Caption = 'Enable Check BarCode Map'
          TabOrder = 5
        end
        object ckOCRCheckHasIC: TCheckBox
          Left = 16
          Top = 216
          Width = 193
          Height = 17
          Caption = 'Enabled OCR Check Has IC'
          TabOrder = 6
          Visible = False
        end
        object ckOCRMoveSRead: TCheckBox
          Left = 16
          Top = 240
          Width = 193
          Height = 17
          Caption = 'Enabled OCR Move S Read'
          TabOrder = 7
          Visible = False
        end
        object edOCRWordType: TEdit
          Left = 116
          Top = 136
          Width = 125
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = 'AANN**'
        end
        object ckStartposshift: TCheckBox
          Left = 16
          Top = 270
          Width = 113
          Height = 17
          Caption = 'Start pos shift'
          TabOrder = 9
        end
        object edStartposshift: TEdit
          Left = 132
          Top = 266
          Width = 45
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          Text = '0'
          OnClick = edStartposshiftClick
        end
        object ckOCRLightChange: TCheckBox
          Left = 15
          Top = 294
          Width = 137
          Height = 17
          Caption = 'Auto change light'
          TabOrder = 11
        end
        object ckOCRLightNoDown: TCheckBox
          Left = 15
          Top = 319
          Width = 209
          Height = 17
          Caption = 'Light cylinder no down'
          TabOrder = 12
        end
        object ckCompareOCRData: TCheckBox
          Left = 15
          Top = 341
          Width = 146
          Height = 17
          Caption = 'Compare OCR Data'
          TabOrder = 13
        end
        object GroupBox3: TGroupBox
          Left = 187
          Top = 289
          Width = 145
          Height = 70
          Caption = 'OCR And Bin Log'
          TabOrder = 10
          object ckOCRAndBinLog: TCheckBox
            Left = 7
            Top = 24
            Width = 122
            Height = 17
            Caption = 'OCR And Bin Log'
            TabOrder = 0
          end
          object ckOCRBinLogAddMark: TCheckBox
            Left = 7
            Top = 46
            Width = 108
            Height = 17
            Caption = 'Log Add Mark'
            TabOrder = 1
          end
        end
      end
      object Panel4: TPanel
        Left = 0
        Top = 503
        Width = 846
        Height = 89
        Align = alBottom
        Color = 12761254
        TabOrder = 2
        object spbSave: TSpeedButton
          Left = 178
          Top = 25
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
          Left = 431
          Top = 25
          Width = 227
          Height = 40
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
      object GroupBox2: TGroupBox
        Left = 397
        Top = 0
        Width = 449
        Height = 503
        Align = alRight
        Caption = 'Light Setup'
        TabOrder = 1
        object Label17: TLabel
          Left = 24
          Top = 33
          Width = 66
          Height = 16
          Caption = 'COM Port: '
        end
        object Label7: TLabel
          Left = 16
          Top = 117
          Width = 58
          Height = 16
          Caption = 'Blue Light'
        end
        object spbResetCom: TSpeedButton
          Left = 232
          Top = 28
          Width = 80
          Height = 25
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'Reset COM'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Margin = 2
          ParentFont = False
          OnClick = spbResetComClick
        end
        object SpeedButton1: TSpeedButton
          Left = 336
          Top = 28
          Width = 80
          Height = 25
          AllowAllUp = True
          GroupIndex = 2
          Caption = 'Stop COM'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          Margin = 2
          ParentFont = False
        end
        object Label5: TLabel
          Left = 16
          Top = 157
          Width = 55
          Height = 16
          Caption = 'Red Light'
        end
        object cbCommReader1: TComboBox
          Left = 96
          Top = 29
          Width = 117
          Height = 24
          ItemHeight = 16
          TabOrder = 0
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
        object edSetBlueLight: TEdit
          Left = 268
          Top = 112
          Width = 57
          Height = 24
          TabOrder = 1
          Text = '0'
          OnChange = edSetBlueLightChange
          OnClick = edSetBlueLightClick
        end
        object sbSetBlueLight: TScrollBar
          Left = 84
          Top = 116
          Width = 173
          Height = 17
          Max = 255
          PageSize = 0
          TabOrder = 3
          OnChange = sbSetBlueLightChange
        end
        object btSetBlueLight: TButton
          Left = 336
          Top = 112
          Width = 105
          Height = 25
          Caption = 'Set Blue Light'
          TabOrder = 2
          OnClick = btSetBlueLightClick
        end
        object edSetRedLight: TEdit
          Left = 268
          Top = 152
          Width = 57
          Height = 24
          TabOrder = 4
          Text = '0'
          OnChange = edSetRedLightChange
          OnClick = edSetBlueLightChange
        end
        object sbSetRedLight: TScrollBar
          Left = 84
          Top = 156
          Width = 173
          Height = 17
          Max = 255
          PageSize = 0
          TabOrder = 6
          OnChange = sbSetRedLightChange
        end
        object btSetRedLight: TButton
          Left = 336
          Top = 152
          Width = 105
          Height = 25
          Caption = 'Set Red Light'
          TabOrder = 5
          OnClick = btSetRedLightClick
        end
        object Memo3: TMemo
          Left = 2
          Top = 315
          Width = 445
          Height = 186
          Align = alBottom
          TabOrder = 7
        end
      end
    end
  end
  object ckWordCount: TCheckBox
    Left = 8
    Top = 136
    Width = 97
    Height = 17
    Caption = 'Word Count'#65306
    TabOrder = 1
  end
  object CommOcr: TComm
    CommName = 'COM11'
    BaudRate = 38400
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = False
    Inx_XonXoffFlow = False
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
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = CommOcrReceiveData
    Left = 40
    Top = 580
  end
  object TimerOcrOn: TTimer
    Enabled = False
    OnTimer = TimerOcrOnTimer
    Left = 68
    Top = 580
  end
  object TimerOcrChangeFile: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerOcrChangeFileTimer
    Left = 68
    Top = 544
  end
  object CommOcr_Tester: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = False
    Inx_XonXoffFlow = False
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
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = CommOcr_TesterReceiveData
    Left = 41
    Top = 544
  end
  object Timer1: TTimer
    Tag = 2
    Enabled = False
    Interval = 1
    OnTimer = Timer1Timer
    Left = 12
    Top = 580
  end
  object ClientSocket1: TClientSocket
    Active = False
    Address = '172.16.8.104'
    ClientType = ctNonBlocking
    Port = 3000
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    Left = 12
    Top = 544
  end
  object ClientSocket2: TClientSocket
    Active = False
    Address = '172.16.8.104'
    ClientType = ctNonBlocking
    Port = 23
    OnConnect = ClientSocket2Connect
    OnDisconnect = ClientSocket2Disconnect
    OnRead = ClientSocket2Read
    Left = 100
    Top = 580
  end
  object CommOcr_Light: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = False
    Inx_XonXoffFlow = False
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
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    Left = 100
    Top = 543
  end
end
