object FTestIF: TFTestIF
  Left = 615
  Top = 123
  BorderStyle = bsDialog
  Caption = 'FTestIF'
  ClientHeight = 800
  ClientWidth = 907
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object grpIFType: TGroupBox
    Left = 0
    Top = 0
    Width = 357
    Height = 748
    Align = alLeft
    Caption = 'Tester Interface'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object rgInterfaceType: TRadioGroup
      Left = 2
      Top = 18
      Width = 353
      Height = 57
      Align = alTop
      Caption = 'Interface for Connection'
      Columns = 3
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemIndex = 1
      Items.Strings = (
        'DIO'
        'GP-IB'
        'RS232'
        'TCP/IP')
      ParentFont = False
      TabOrder = 0
      OnClick = rgInterfaceTypeClick
    end
    object PageControl2: TPageControl
      Left = 2
      Top = 75
      Width = 353
      Height = 671
      ActivePage = tsGpib
      Align = alClient
      TabIndex = 1
      TabOrder = 1
      object tsDio: TTabSheet
        Caption = 'DIO'
        object grpTTL: TGroupBox
          Left = 0
          Top = 0
          Width = 345
          Height = 453
          Align = alTop
          Caption = 'Setting'
          TabOrder = 0
          object grpTTLSetting: TGroupBox
            Left = 2
            Top = 18
            Width = 341
            Height = 60
            Align = alTop
            Caption = 'Type'
            TabOrder = 0
            object cbDIOType: TComboBox
              Left = 22
              Top = 24
              Width = 252
              Height = 24
              Hint = 'Select commuction method'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImeName = ''
              ItemHeight = 0
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
              OnChange = cbDIOTypeChange
            end
          end
          object lstTTL: TListBox
            Left = 2
            Top = 78
            Width = 341
            Height = 311
            Align = alTop
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'Arial'
            Font.Style = []
            ItemHeight = 17
            ParentFont = False
            TabOrder = 1
          end
          object AntiSignalCBox: TCheckBox
            Left = 3
            Top = 392
            Width = 90
            Height = 20
            Caption = 'Anti-Signal'
            TabOrder = 2
            OnClick = AntiSignalCBoxClick
          end
          object cbASEJPMode: TCheckBox
            Left = 3
            Top = 416
            Width = 158
            Height = 20
            Caption = 'ASE Special Mode'
            TabOrder = 3
            OnClick = AntiSignalCBoxClick
          end
        end
      end
      object tsGpib: TTabSheet
        Caption = 'GPIB'
        ImageIndex = 1
        object labcAutoOnecycleHomStart: TLabel
          Left = 294
          Top = 432
          Width = 21
          Height = 16
          Caption = 'min'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object grpGPIB: TGroupBox
          Left = 0
          Top = 0
          Width = 345
          Height = 420
          Align = alTop
          Caption = 'Setting'
          TabOrder = 0
          object grpGPIBType: TGroupBox
            Left = 2
            Top = 18
            Width = 341
            Height = 110
            Align = alTop
            Caption = 'Type'
            TabOrder = 0
            object lblGPIBAddr: TLabel
              Left = 119
              Top = 74
              Width = 51
              Height = 16
              Caption = 'Address'
            end
            object cbGPIBType: TComboBox
              Left = 22
              Top = 24
              Width = 252
              Height = 24
              Hint = 'Select commuction method'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImeMode = imClose
              ItemHeight = 16
              ItemIndex = 1
              ParentFont = False
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
              Text = '255 Bin'
              Items.Strings = (
                'Advan type1'
                '255 Bin'
                '16 Bin'
                '32 Bin'
                'SPEA Type'
                '16 Bin GS'
                '32 Bin GS'
                'Advan T6577'
                'Qrovo Protocol'
                'Delta_Castle')
            end
            object edGPIBAddress: TEdit
              Left = 178
              Top = 72
              Width = 100
              Height = 24
              ImeName = ''
              TabOrder = 1
              OnClick = edGPIBAddressClick
            end
          end
          object cbSpiroxTesterLotEnd: TCheckBox
            Left = 5
            Top = 316
            Width = 268
            Height = 21
            Caption = 'Spirox Lot End and Full lot end command.'
            ParentShowHint = False
            ShowHint = True
            TabOrder = 2
          end
          object rg2DID_Format: TRadioGroup
            Left = 2
            Top = 128
            Width = 341
            Height = 182
            Align = alTop
            Caption = '2DID format'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Standard'
              'AMD format'
              'Intel format')
            ParentFont = False
            TabOrder = 1
          end
        end
        object cbAutoOnecycleHomStart: TCheckBox
          Left = 8
          Top = 431
          Width = 209
          Height = 17
          Caption = 'Auto home and start every'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object edtAutoOnecycleHomStartTime: TEdit
          Left = 187
          Top = 427
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          Visible = False
          OnClick = edtAutoOnecycleHomStartTimeClick
        end
      end
      object tsRs232: TTabSheet
        Caption = 'RS232'
        ImageIndex = 2
        object pgcRS232: TPageControl
          Left = 0
          Top = 0
          Width = 345
          Height = 640
          ActivePage = tsRS232Setting
          Align = alClient
          TabIndex = 0
          TabOrder = 0
          TabWidth = 100
          object tsRS232Setting: TTabSheet
            Caption = 'Setting'
            object grpRS232: TGroupBox
              Left = 0
              Top = 0
              Width = 337
              Height = 369
              Align = alTop
              Caption = 'Setting'
              TabOrder = 0
              object grpRS232Type: TGroupBox
                Left = 2
                Top = 18
                Width = 333
                Height = 73
                Align = alTop
                Caption = 'Type'
                TabOrder = 0
                object cbRs232Type: TComboBox
                  Left = 19
                  Top = 22
                  Width = 251
                  Height = 24
                  Hint = 'Select commuction method'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ItemHeight = 16
                  ItemIndex = 0
                  ParentFont = False
                  ParentShowHint = False
                  ShowHint = True
                  TabOrder = 0
                  Text = 'Standard'
                  OnChange = cbRs232TypeChange
                  Items.Strings = (
                    'Standard')
                end
                object cb_NeedSendVSOT: TCheckBox
                  Left = 200
                  Top = 48
                  Width = 73
                  Height = 17
                  Caption = 'SPRD'
                  TabOrder = 1
                  Visible = False
                end
                object cbForEgistec: TCheckBox
                  Left = 20
                  Top = 52
                  Width = 105
                  Height = 17
                  Caption = 'For EgisTec'
                  TabOrder = 2
                end
              end
              object pnlRS232: TPanel
                Left = 2
                Top = 91
                Width = 333
                Height = 280
                Align = alTop
                BevelOuter = bvNone
                Color = 12761254
                TabOrder = 1
                object rgParity: TRadioGroup
                  Left = 11
                  Top = 137
                  Width = 318
                  Height = 57
                  Caption = 'Parity'
                  Columns = 3
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ItemIndex = 0
                  Items.Strings = (
                    'Even'
                    'Odd'
                    'None')
                  ParentFont = False
                  TabOrder = 3
                end
                object rgStopBit: TRadioGroup
                  Left = 11
                  Top = 204
                  Width = 318
                  Height = 57
                  Caption = 'Stop Bit'
                  Columns = 2
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ItemIndex = 0
                  Items.Strings = (
                    '1 Bit'
                    '2Bits')
                  ParentFont = False
                  TabOrder = 4
                end
                object rgBitLength: TRadioGroup
                  Left = 11
                  Top = 73
                  Width = 318
                  Height = 57
                  Caption = 'Bit Length'
                  Columns = 2
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ItemIndex = 0
                  Items.Strings = (
                    '7 Bits'
                    '8 Bits')
                  ParentFont = False
                  TabOrder = 2
                end
                object rgBaudRate: TRadioGroup
                  Left = 11
                  Top = 11
                  Width = 279
                  Height = 57
                  Caption = 'Baud Rate'
                  Columns = 3
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ItemIndex = 0
                  Items.Strings = (
                    '19200'
                    '9600'
                    '4800')
                  ParentFont = False
                  TabOrder = 0
                  Visible = False
                end
                object gbBaudRate: TGroupBox
                  Left = 11
                  Top = 12
                  Width = 318
                  Height = 61
                  Caption = 'Baud Rate'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clNavy
                  Font.Height = -15
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 1
                  object cbbBaudRate: TComboBox
                    Left = 19
                    Top = 22
                    Width = 251
                    Height = 24
                    Hint = 'Select commuction method'
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clNavy
                    Font.Height = -15
                    Font.Name = 'MS Sans Serif'
                    Font.Style = []
                    ItemHeight = 16
                    ItemIndex = 2
                    ParentFont = False
                    ParentShowHint = False
                    ShowHint = True
                    TabOrder = 0
                    Text = '9600'
                    OnChange = cbRs232TypeChange
                    Items.Strings = (
                      '4800'
                      '7200'
                      '9600'
                      '14400'
                      '19200'
                      '115200')
                  end
                end
              end
            end
          end
          object tsSLTSetting: TTabSheet
            Caption = 'SLT Setting'
            ImageIndex = 1
            object labProgramerrorcode: TLabel
              Left = 0
              Top = 40
              Width = 146
              Height = 16
              Caption = 'Check Program ErrCode'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbRunCheckProgramUse: TCheckBox
              Left = 8
              Top = 15
              Width = 145
              Height = 17
              Caption = 'Run Check Program'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object edtRunCheckProgramName: TEdit
              Left = 152
              Top = 10
              Width = 169
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImeName = ''
              MaxLength = 100
              ParentFont = False
              TabOrder = 0
              Text = '0'
            end
            object edtRunCheckProgramNameErrCode: TEdit
              Left = 152
              Top = 38
              Width = 169
              Height = 24
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImeName = ''
              MaxLength = 100
              ParentFont = False
              TabOrder = 2
              Text = 'PROGRAMERROR'
            end
            object GroupBox5: TGroupBox
              Left = 270
              Top = 88
              Width = 49
              Height = 401
              Caption = 'Alarm'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
              object Label43: TLabel
                Left = 11
                Top = 305
                Width = 25
                Height = 16
                Caption = 'Use'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object cbMaxBIOSWaitTimeAlm: TCheckBox
                Left = 15
                Top = 211
                Width = 17
                Height = 17
                TabOrder = 0
              end
              object cbMinTestTimeAlm: TCheckBox
                Left = 15
                Top = 269
                Width = 17
                Height = 17
                TabOrder = 1
              end
              object cbMaxTestTimeAlm: TCheckBox
                Left = 15
                Top = 320
                Width = 17
                Height = 17
                TabOrder = 2
              end
            end
            object GroupBox4: TGroupBox
              Left = 151
              Top = 88
              Width = 121
              Height = 401
              Caption = 'ErrCode'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
              object edMinTestTimeErrCode: TEdit
                Left = 7
                Top = 270
                Width = 106
                Height = 21
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -12
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 1
                Text = 'MINTESTERROR'
              end
              object edBIOSErrCode: TEdit
                Left = 7
                Top = 213
                Width = 106
                Height = 21
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -12
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 0
                Text = 'MAXBIOSERROR'
              end
              object edMaxTestTimeErrCode: TEdit
                Left = 6
                Top = 315
                Width = 106
                Height = 21
                Enabled = False
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -12
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 2
                Text = 'MAXTESTERROR'
              end
            end
            object GroupBox6: TGroupBox
              Left = 6
              Top = 88
              Width = 147
              Height = 401
              Caption = 'Time Setting'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
              object Label31: TLabel
                Left = 112
                Top = 100
                Width = 27
                Height = 16
                Caption = 'Sec.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label32: TLabel
                Left = 6
                Top = 76
                Width = 110
                Height = 16
                Caption = 'Send NEXT Delay'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label33: TLabel
                Left = 6
                Top = 188
                Width = 123
                Height = 16
                Caption = 'Max BIOS Wait Time'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label4: TLabel
                Left = 112
                Top = 212
                Width = 27
                Height = 16
                Caption = 'Sec.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label6: TLabel
                Left = 6
                Top = 244
                Width = 85
                Height = 16
                Caption = 'Min Test Time'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label13: TLabel
                Left = 112
                Top = 268
                Width = 27
                Height = 16
                Caption = 'Sec.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label14: TLabel
                Left = 6
                Top = 20
                Width = 112
                Height = 16
                Caption = 'PowerSwitchDelay'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label17: TLabel
                Left = 112
                Top = 44
                Width = 27
                Height = 16
                Caption = 'Sec.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label30: TLabel
                Left = 6
                Top = 132
                Width = 112
                Height = 16
                Caption = 'Test OK Wait Time'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object Label18: TLabel
                Left = 112
                Top = 156
                Width = 27
                Height = 16
                Caption = 'Sec.'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                Visible = False
              end
              object Label21: TLabel
                Left = 6
                Top = 295
                Width = 136
                Height = 18
                AutoSize = False
                Caption = 'Max Test Time'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label22: TLabel
                Left = 113
                Top = 321
                Width = 24
                Height = 16
                Caption = 'Sec'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object edTestOKWaitTime: TEdit
                Left = 9
                Top = 152
                Width = 94
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 2
                Text = '0.00'
                Visible = False
                OnClick = edPowerSwitchDelayClick
              end
              object edSendNEXTDelay: TEdit
                Left = 9
                Top = 96
                Width = 94
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 1
                Text = '0.00'
                OnClick = edPowerSwitchDelayClick
              end
              object edMaxBIOSWaitTime: TEdit
                Left = 9
                Top = 208
                Width = 94
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 3
                Text = '0.00'
                OnClick = edMaxBIOSWaitTimeClick
              end
              object edMinTestTime: TEdit
                Left = 9
                Top = 264
                Width = 94
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 4
                Text = '0.00'
                OnClick = edMinTestTimeClick
              end
              object edPowerSwitchDelay: TEdit
                Left = 9
                Top = 40
                Width = 94
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentFont = False
                TabOrder = 0
                Text = '0.00'
                OnClick = edPowerSwitchDelayClick
              end
              object edSLTMaxTestTime: TEdit
                Left = 9
                Top = 313
                Width = 94
                Height = 24
                BiDiMode = bdLeftToRight
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clNavy
                Font.Height = -15
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ImeName = ''
                ParentBiDiMode = False
                ParentFont = False
                TabOrder = 5
                Text = '0.00'
                OnClick = edMaxBIOSWaitTimeClick
              end
            end
          end
        end
      end
      object tsTCPIP: TTabSheet
        Caption = 'TCP/IP'
        ImageIndex = 4
        object Label23: TLabel
          Left = 5
          Top = 58
          Width = 51
          Height = 16
          Caption = 'Address'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 5
          Top = 98
          Width = 24
          Height = 16
          Caption = 'Port'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object btTesterTCPShow: TButton
          Left = 184
          Top = 96
          Width = 115
          Height = 25
          Caption = 'Tester TCP '
          TabOrder = 2
          OnClick = btTesterTCPShowClick
        end
        object edTCPIP_Address: TEdit
          Left = 83
          Top = 54
          Width = 130
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '192.168.1.50'
        end
        object edTCPIP_Port: TEdit
          Left = 83
          Top = 94
          Width = 60
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '6000'
          OnClick = edTCPIP_PortClick
        end
      end
    end
  end
  object pnlBtm: TPanel
    Left = 0
    Top = 748
    Width = 907
    Height = 52
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 2
    object spbSave: TSpeedButton
      Left = 62
      Top = 9
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
      Left = 384
      Top = 8
      Width = 237
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
    object rbTemp: TRadioButton
      Left = -77
      Top = 28
      Width = 113
      Height = 17
      Caption = 'rbTemp'
      TabOrder = 0
    end
  end
  object pnlTimeSetting: TPanel
    Left = 357
    Top = 0
    Width = 550
    Height = 748
    Align = alClient
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object grpTestingStopTime: TGroupBox
      Left = 0
      Top = 219
      Width = 550
      Height = 76
      Align = alTop
      Caption = 'Testing Stop Time'
      Font.Charset = CHINESEBIG5_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      object lblInitialWaitTIme: TLabel
        Left = 12
        Top = 22
        Width = 140
        Height = 18
        AutoSize = False
        Caption = 'Initial Wait Time'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblInitialWaitTImeSec: TLabel
        Left = 265
        Top = 23
        Width = 22
        Height = 16
        Caption = 'sec'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblTestWaitTime: TLabel
        Left = 12
        Top = 49
        Width = 136
        Height = 18
        AutoSize = False
        Caption = 'Testing Wait Time'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object lblTestWaitTimeSec: TLabel
        Left = 265
        Top = 50
        Width = 22
        Height = 16
        Caption = 'sec'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edtTestingWaitTime: TEdit
        Left = 153
        Top = 46
        Width = 94
        Height = 24
        BiDiMode = bdLeftToRight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = ''
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 1
        Text = '0.00'
        OnKeyPress = edMaxTestTimeKeyPress
        OnMouseDown = edMaxTestTimeMouseDown
      end
      object edtInitWaitTime: TEdit
        Left = 153
        Top = 19
        Width = 94
        Height = 24
        BiDiMode = bdLeftToRight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = ''
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 0
        Text = '0.00'
        OnKeyPress = edMaxTestTimeKeyPress
        OnMouseDown = edMaxTestTimeMouseDown
      end
    end
    object gbRs232BinCount: TGroupBox
      Left = 0
      Top = 295
      Width = 550
      Height = 46
      Align = alTop
      Caption = 'RS232 Max Bin Count'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      object labRs232BinCount: TLabel
        Left = 132
        Top = 21
        Width = 93
        Height = 16
        Caption = 'Count : 15 ~ 255'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edMaxBinCount: TEdit
        Left = 12
        Top = 17
        Width = 117
        Height = 24
        TabOrder = 0
        Text = '32'
        OnMouseDown = edMaxBinCountMouseDown
      end
    end
    object gbInitialDelay: TGroupBox
      Left = 0
      Top = 341
      Width = 550
      Height = 407
      Align = alClient
      Caption = 'Use initial start delay in socket'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object pnl1stDevice: TPanel
        Left = 2
        Top = 54
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object cbEveryFirstDeviceUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 169
          Height = 17
          Caption = 'Every first devices'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_1: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_1_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlTitle: TPanel
        Left = 2
        Top = 18
        Width = 546
        Height = 36
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object lblDelayTime: TLabel
          Left = 320
          Top = 1
          Width = 137
          Height = 19
          AutoSize = False
          Caption = 'Initial delay time (Sec)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblTriggerTime: TLabel
          Left = 192
          Top = 1
          Width = 113
          Height = 19
          AutoSize = False
          Caption = 'Trigger time (Sec)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblFT: TLabel
          Left = 361
          Top = 20
          Width = 17
          Height = 16
          Caption = 'FT'
        end
        object lblRT: TLabel
          Left = 469
          Top = 20
          Width = 19
          Height = 16
          Caption = 'RT'
        end
      end
      object pnlPause: TPanel
        Left = 2
        Top = 204
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 6
        object cbWhenPressStopOverUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 197
          Height = 17
          Caption = 'When Press Stop Over '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object edtInitialDelay_6: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edOverSec: TEdit
          Left = 224
          Top = 3
          Width = 80
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_6_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlLowTestTime: TPanel
        Left = 2
        Top = 174
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 5
        object cbWhenHappenTestedTimeBelowUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 221
          Height = 17
          Caption = 'When happen tested time below'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object edtInitialDelay_4: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edBelowSec: TEdit
          Left = 224
          Top = 3
          Width = 80
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_4_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlHeatDoor: TPanel
        Left = 2
        Top = 144
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 4
        object cbAfterOpenHeatDoorUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 169
          Height = 17
          Caption = 'After Open Heat Door '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_5: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_5_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlAutoClean: TPanel
        Left = 2
        Top = 114
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 3
        object edtInitialDelay_3: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object cbAfterAutoCleanFunctionUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 177
          Height = 17
          Caption = 'After Auto Clean Function '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_3_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlAlarmMsg: TPanel
        Left = 2
        Top = 84
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 2
        object cbAfterShowAlarmMessageUseInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 177
          Height = 17
          Caption = 'After ShowAlarm Message'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_2: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_2_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlOTD: TPanel
        Left = 2
        Top = 264
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 8
        object cbOTDUnlockDelay: TCheckBox
          Left = 8
          Top = 8
          Width = 137
          Height = 15
          Caption = 'OTD unlock'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_9: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_9_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlEOTTime: TPanel
        Left = 2
        Top = 294
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 9
        object cbTestFinishToNextTestOver: TCheckBox
          Left = 8
          Top = 8
          Width = 137
          Height = 15
          Caption = 'EOT monitor time '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object iTestFinishToNextTestOver: TEdit
          Left = 224
          Top = 3
          Width = 80
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_8: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_8_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object pnlNoFullsite: TPanel
        Left = 2
        Top = 234
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 7
        object cbWhenNoFullSiteUseInitialDelay: TCheckBox
          Left = 8
          Top = 6
          Width = 129
          Height = 17
          Caption = 'When No Full Site'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object edtInitialDelay_7: TEdit
          Left = 320
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_7_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object Panel39: TPanel
        Left = 2
        Top = 375
        Width = 546
        Height = 30
        Align = alBottom
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 12761254
        TabOrder = 13
        object cbUseOtherArmToTestAfterInitialDelay: TCheckBox
          Left = 8
          Top = 7
          Width = 350
          Height = 17
          Caption = 'After initial start delay, use the other arm to test first'
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object pnlSOTTime: TPanel
        Left = 2
        Top = 324
        Width = 546
        Height = 30
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 10
        object cbTestStartToNextTestStart: TCheckBox
          Left = 8
          Top = 9
          Width = 137
          Height = 15
          Caption = 'SOT monitor time '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object iTeststartToNextTestStart: TEdit
          Left = 224
          Top = 4
          Width = 80
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_10: TEdit
          Left = 320
          Top = 4
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
        object edtInitialDelay_10_RT: TEdit
          Left = 428
          Top = 3
          Width = 100
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edtInitialDelay_1Click
        end
      end
      object cbUseSocketHeating: TCheckBox
        Left = 10
        Top = 359
        Width = 169
        Height = 17
        Caption = 'Use Socket Heating'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
      end
      object edt_UseSocketHeating: TEdit
        Left = 224
        Top = 355
        Width = 80
        Height = 24
        BiDiMode = bdLeftToRight
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImeName = ''
        ParentBiDiMode = False
        ParentFont = False
        TabOrder = 11
        Text = '0.00'
        OnClick = edtInitialDelay_1Click
      end
    end
    object PageControl1: TPageControl
      Left = 0
      Top = 0
      Width = 550
      Height = 219
      ActivePage = rgTime1
      Align = alTop
      TabIndex = 0
      TabOrder = 0
      object rgTime1: TTabSheet
        Caption = 'Time1'
        object palTime: TPanel
          Left = 0
          Top = 0
          Width = 542
          Height = 191
          Align = alClient
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 0
          object Label50: TLabel
            Left = 9
            Top = 8
            Width = 140
            Height = 18
            AutoSize = False
            Caption = 'Initial Maximum Test'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label51: TLabel
            Left = 9
            Top = 60
            Width = 119
            Height = 18
            AutoSize = False
            Caption = 'Dummy Test'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label53: TLabel
            Left = 278
            Top = 9
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label58: TLabel
            Left = 278
            Top = 61
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label19: TLabel
            Left = 9
            Top = 34
            Width = 136
            Height = 18
            AutoSize = False
            Caption = 'Maximum Test'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label20: TLabel
            Left = 278
            Top = 35
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label49: TLabel
            Left = 9
            Top = 111
            Width = 99
            Height = 16
            Caption = 'Initial Start Delay'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label1: TLabel
            Left = 278
            Top = 111
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label2: TLabel
            Left = 9
            Top = 139
            Width = 136
            Height = 16
            Caption = 'Initial Start Delay Count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label52: TLabel
            Left = 278
            Top = 139
            Width = 32
            Height = 16
            Caption = 'times'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label8: TLabel
            Left = 11
            Top = 167
            Width = 112
            Height = 16
            Caption = 'After Tested Delay'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label12: TLabel
            Left = 278
            Top = 167
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edMaxTestTime: TEdit
            Left = 166
            Top = 31
            Width = 94
            Height = 24
            BiDiMode = bdLeftToRight
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 1
            Text = '0.00'
            OnClick = edInitialMaxTestClick
          end
          object edInitialMaxTest: TEdit
            Left = 166
            Top = 5
            Width = 94
            Height = 24
            BiDiMode = bdLeftToRight
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 0
            Text = '0.00'
            OnClick = edInitialMaxTestClick
          end
          object edDummyTestTime: TEdit
            Left = 166
            Top = 57
            Width = 94
            Height = 24
            BiDiMode = bdLeftToRight
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 2
            Text = '0.00'
            OnKeyPress = edMaxTestTimeKeyPress
            OnMouseDown = edMaxTestTimeMouseDown
          end
          object edtInitStartDelay: TEdit
            Left = 166
            Top = 107
            Width = 94
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentFont = False
            TabOrder = 4
            Text = '0'
            OnClick = edtInitStartDelayClick
            OnKeyPress = edMaxTestTimeKeyPress
          end
          object edtInitStartDelayCT: TEdit
            Left = 166
            Top = 135
            Width = 94
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentFont = False
            TabOrder = 5
            Text = '0'
            OnClick = edtInitialDecCount1Click
          end
          object edtAfterTestedDelay: TEdit
            Left = 166
            Top = 163
            Width = 94
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentFont = False
            TabOrder = 6
            Text = '0'
            OnKeyPress = edMaxTestTimeKeyPress
            OnMouseDown = edMaxTestTimeMouseDown
          end
          object palStartDelay: TPanel
            Left = 4
            Top = 80
            Width = 322
            Height = 26
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 3
            object Label54: TLabel
              Left = 274
              Top = 7
              Width = 25
              Height = 16
              Caption = 'sec.'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label45: TLabel
              Left = 5
              Top = 6
              Width = 105
              Height = 19
              AutoSize = False
              Caption = 'Start Delay'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object edStartDelayTime: TEdit
              Left = 162
              Top = 3
              Width = 94
              Height = 24
              BiDiMode = bdLeftToRight
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ImeName = ''
              ParentBiDiMode = False
              ParentFont = False
              TabOrder = 0
              Text = '0.00'
              OnKeyPress = edMaxTestTimeKeyPress
              OnMouseDown = edMaxTestTimeMouseDown
            end
          end
        end
      end
      object rgTime2: TTabSheet
        Caption = 'Time2'
        ImageIndex = 1
        object Label34: TLabel
          Left = 127
          Top = 35
          Width = 20
          Height = 19
          AutoSize = False
          Caption = '+'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label35: TLabel
          Left = 224
          Top = 37
          Width = 95
          Height = 16
          Caption = 'sec(each count)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 3
          Top = 37
          Width = 109
          Height = 16
          Caption = 'Initial Start Delay 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 380
          Top = 38
          Width = 28
          Height = 13
          Caption = 'Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 3
          Top = 66
          Width = 109
          Height = 16
          Caption = 'Initial Start Delay 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 127
          Top = 64
          Width = 20
          Height = 19
          AutoSize = False
          Caption = '+'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 224
          Top = 66
          Width = 95
          Height = 16
          Caption = 'sec(each count)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label15: TLabel
          Left = 380
          Top = 67
          Width = 28
          Height = 13
          Caption = 'Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 3
          Top = 95
          Width = 109
          Height = 16
          Caption = 'Initial Start Delay 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label28: TLabel
          Left = 127
          Top = 93
          Width = 20
          Height = 19
          AutoSize = False
          Caption = '+'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label29: TLabel
          Left = 224
          Top = 95
          Width = 95
          Height = 16
          Caption = 'sec(each count)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label36: TLabel
          Left = 380
          Top = 96
          Width = 28
          Height = 13
          Caption = 'Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label37: TLabel
          Left = 3
          Top = 124
          Width = 109
          Height = 16
          Caption = 'Initial Start Delay 4'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label38: TLabel
          Left = 127
          Top = 122
          Width = 20
          Height = 19
          AutoSize = False
          Caption = '+'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label39: TLabel
          Left = 224
          Top = 124
          Width = 95
          Height = 16
          Caption = 'sec(each count)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label40: TLabel
          Left = 380
          Top = 125
          Width = 28
          Height = 13
          Caption = 'Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label41: TLabel
          Left = 4
          Top = 8
          Width = 95
          Height = 20
          Caption = 'Use Function'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edtInitialDec1: TEdit
          Left = 148
          Top = 33
          Width = 70
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitialDecCount1: TEdit
          Left = 327
          Top = 32
          Width = 50
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
        object iStartDelayCount: TComboBox
          Left = 122
          Top = 4
          Width = 67
          Height = 24
          Hint = 'Select commuction method'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ItemHeight = 16
          ParentFont = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          Text = '0'
          Items.Strings = (
            '0'
            '1'
            '2'
            '3'
            '4')
        end
        object edtInitialDec2: TEdit
          Left = 148
          Top = 62
          Width = 70
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 3
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitialDecCount2: TEdit
          Left = 327
          Top = 62
          Width = 50
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 4
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
        object edtInitialDec3: TEdit
          Left = 148
          Top = 91
          Width = 70
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 5
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitialDecCount3: TEdit
          Left = 327
          Top = 91
          Width = 50
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 6
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
        object edtInitialDec4: TEdit
          Left = 148
          Top = 120
          Width = 70
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 7
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitialDecCount4: TEdit
          Left = 327
          Top = 120
          Width = 50
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 8
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
      end
      object rgPurgeAir: TTabSheet
        Caption = 'PurgeAir'
        ImageIndex = 2
        object Label7: TLabel
          Left = 224
          Top = 5
          Width = 34
          Height = 16
          Caption = 'Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbPurgeAir: TCheckBox
          Left = 8
          Top = 7
          Width = 137
          Height = 17
          Caption = 'After Contract'
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object edPurgeAir: TEdit
          Left = 148
          Top = 1
          Width = 70
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0'
          OnClick = edPurgeAirClick
        end
      end
      object tsRT: TTabSheet
        Caption = 'RT'
        ImageIndex = 3
        object labRT1: TLabel
          Left = 9
          Top = 8
          Width = 140
          Height = 18
          AutoSize = False
          Caption = 'Initial Maximum Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT3: TLabel
          Left = 9
          Top = 60
          Width = 119
          Height = 18
          AutoSize = False
          Caption = 'Dummy Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT7: TLabel
          Left = 278
          Top = 9
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT9: TLabel
          Left = 278
          Top = 61
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT2: TLabel
          Left = 9
          Top = 34
          Width = 136
          Height = 18
          AutoSize = False
          Caption = 'Maximum Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT8: TLabel
          Left = 278
          Top = 35
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT5: TLabel
          Left = 9
          Top = 111
          Width = 99
          Height = 16
          Caption = 'Initial Start Delay'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT11: TLabel
          Left = 278
          Top = 111
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT6: TLabel
          Left = 9
          Top = 139
          Width = 136
          Height = 16
          Caption = 'Initial Start Delay Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labRT12: TLabel
          Left = 278
          Top = 139
          Width = 32
          Height = 16
          Caption = 'times'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edMaxTestTime_RT: TEdit
          Left = 166
          Top = 31
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edInitialMaxTestClick
        end
        object edInitialMaxTest_RT: TEdit
          Left = 166
          Top = 5
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edInitialMaxTestClick
        end
        object edDummyTestTime_RT: TEdit
          Left = 166
          Top = 57
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitStartDelay_RT: TEdit
          Left = 166
          Top = 107
          Width = 94
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentFont = False
          TabOrder = 4
          Text = '0'
          OnClick = edtInitStartDelayClick
          OnKeyPress = edMaxTestTimeKeyPress
        end
        object edtInitStartDelayCT_RT: TEdit
          Left = 166
          Top = 135
          Width = 94
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentFont = False
          TabOrder = 5
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
        object Panel1: TPanel
          Left = 4
          Top = 80
          Width = 322
          Height = 26
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 3
          object labRT10: TLabel
            Left = 274
            Top = 7
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object labRT4: TLabel
            Left = 5
            Top = 6
            Width = 105
            Height = 19
            AutoSize = False
            Caption = 'Start Delay'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edStartDelayTime_RT: TEdit
            Left = 162
            Top = 3
            Width = 94
            Height = 24
            BiDiMode = bdLeftToRight
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 0
            Text = '0.00'
            OnKeyPress = edMaxTestTimeKeyPress
            OnMouseDown = edMaxTestTimeMouseDown
          end
        end
      end
      object tsEQC: TTabSheet
        Caption = 'EQC'
        ImageIndex = 4
        object labEQC1: TLabel
          Left = 9
          Top = 8
          Width = 140
          Height = 18
          AutoSize = False
          Caption = 'Initial Maximum Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC3: TLabel
          Left = 9
          Top = 60
          Width = 119
          Height = 18
          AutoSize = False
          Caption = 'Dummy Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC7: TLabel
          Left = 278
          Top = 9
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC9: TLabel
          Left = 278
          Top = 61
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC2: TLabel
          Left = 9
          Top = 34
          Width = 136
          Height = 18
          AutoSize = False
          Caption = 'Maximum Test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC8: TLabel
          Left = 278
          Top = 35
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC5: TLabel
          Left = 9
          Top = 111
          Width = 99
          Height = 16
          Caption = 'Initial Start Delay'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC11: TLabel
          Left = 278
          Top = 111
          Width = 25
          Height = 16
          Caption = 'sec.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC6: TLabel
          Left = 9
          Top = 139
          Width = 136
          Height = 16
          Caption = 'Initial Start Delay Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEQC12: TLabel
          Left = 278
          Top = 139
          Width = 32
          Height = 16
          Caption = 'times'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edMaxTestTime_EQC: TEdit
          Left = 166
          Top = 31
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 1
          Text = '0.00'
          OnClick = edInitialMaxTestClick
        end
        object edInitialMaxTest_EQC: TEdit
          Left = 166
          Top = 5
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 0
          Text = '0.00'
          OnClick = edInitialMaxTestClick
        end
        object edDummyTestTime_EQC: TEdit
          Left = 166
          Top = 57
          Width = 94
          Height = 24
          BiDiMode = bdLeftToRight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Text = '0.00'
          OnKeyPress = edMaxTestTimeKeyPress
          OnMouseDown = edMaxTestTimeMouseDown
        end
        object edtInitStartDelay_EQC: TEdit
          Left = 166
          Top = 107
          Width = 94
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentFont = False
          TabOrder = 4
          Text = '0'
          OnClick = edtInitStartDelayClick
          OnKeyPress = edMaxTestTimeKeyPress
        end
        object edtInitStartDelayCT_EQC: TEdit
          Left = 166
          Top = 135
          Width = 94
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = ''
          ParentFont = False
          TabOrder = 5
          Text = '0'
          OnClick = edtInitialDecCount1Click
        end
        object Panel2: TPanel
          Left = 4
          Top = 80
          Width = 322
          Height = 26
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 3
          object labEQC10: TLabel
            Left = 274
            Top = 7
            Width = 25
            Height = 16
            Caption = 'sec.'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object labEQC4: TLabel
            Left = 5
            Top = 6
            Width = 105
            Height = 19
            AutoSize = False
            Caption = 'Start Delay'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object edStartDelayTime_EQC: TEdit
            Left = 162
            Top = 3
            Width = 94
            Height = 24
            BiDiMode = bdLeftToRight
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -15
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ImeName = ''
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 0
            Text = '0.00'
            OnKeyPress = edMaxTestTimeKeyPress
            OnMouseDown = edMaxTestTimeMouseDown
          end
        end
      end
    end
  end
end
