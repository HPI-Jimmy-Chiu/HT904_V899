object fOmron: TfOmron
  Left = 608
  Top = 295
  Width = 1334
  Height = 795
  Caption = 'Omron Thermo Controller'
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
  object Label45: TLabel
    Left = 52
    Top = 24
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label46: TLabel
    Left = 52
    Top = 48
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label47: TLabel
    Left = 52
    Top = 72
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label48: TLabel
    Left = 52
    Top = 100
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Panel7: TPanel
    Left = 0
    Top = 0
    Width = 437
    Height = 471
    Align = alLeft
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object PageControl1: TPageControl
      Left = 0
      Top = 0
      Width = 437
      Height = 471
      ActivePage = tsRecord
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabIndex = 3
      TabOrder = 0
      TabWidth = 100
      object tsAuto: TTabSheet
        Caption = 'Auto'
        object Label26: TLabel
          Left = 408
          Top = 0
          Width = 11
          Height = 22
          Caption = '0'
        end
        object btGetModuler: TButton
          Left = 15
          Top = 96
          Width = 121
          Height = 25
          Caption = 'Get Moduler'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btGetModulerClick
        end
        object btRunStop: TButton
          Left = 15
          Top = 152
          Width = 121
          Height = 25
          Caption = 'Set RUN / STOP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = btRunStopClick
        end
        object rgSensorType: TRadioGroup
          Left = 16
          Top = 4
          Width = 145
          Height = 85
          Caption = 'Thermo Sensor'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            'PT Type'
            'K Type')
          ParentFont = False
          TabOrder = 0
        end
        object btSetThermoType: TButton
          Left = 15
          Top = 124
          Width = 121
          Height = 25
          Caption = 'Set Sensor Type'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btSetThermoTypeClick
        end
        object rgRunStop: TRadioGroup
          Left = 164
          Top = 4
          Width = 133
          Height = 85
          Caption = 'RUN / STOP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            'STOP'
            'RUN')
          ParentFont = False
          TabOrder = 1
        end
        object Button1: TButton
          Left = 16
          Top = 184
          Width = 121
          Height = 25
          Caption = 'Stop Control'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = Button1Click
        end
      end
      object tsManual: TTabSheet
        Caption = 'Manual'
        ImageIndex = 1
        object labSTX: TLabel
          Left = 8
          Top = 8
          Width = 48
          Height = 22
          Caption = '[STX]'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object gbBccArea: TGroupBox
          Left = 2
          Top = 33
          Width = 427
          Height = 393
          Caption = 'BCC'#35336#31639#31684#22285
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = #24494#36575#27491#40657#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label3: TLabel
            Left = 88
            Top = 48
            Width = 33
            Height = 22
            Caption = '000'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label4: TLabel
            Left = 16
            Top = 24
            Width = 65
            Height = 24
            AutoSize = False
            Caption = #21934#20803#32232#34399
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
          end
          object Label5: TLabel
            Left = 84
            Top = 24
            Width = 77
            Height = 24
            AutoSize = False
            Caption = #23376#22320#22336'SID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #24494#36575#27491#40657#39636
            Font.Style = []
            ParentFont = False
          end
          object labETX: TLabel
            Left = 8
            Top = 364
            Width = 48
            Height = 22
            Caption = '[ETX]'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object cbUnitAddress: TComboBox
            Left = 16
            Top = 44
            Width = 65
            Height = 30
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ItemHeight = 22
            ParentFont = False
            TabOrder = 0
            Text = '01'
            Items.Strings = (
              '01'
              '02'
              '03'
              '04'
              '05')
          end
          object gbFinsMiniCommand: TGroupBox
            Left = 12
            Top = 76
            Width = 405
            Height = 285
            Caption = 'FINS-mini'#21629#20196
            TabOrder = 1
            object Label8: TLabel
              Left = 12
              Top = 20
              Width = 65
              Height = 24
              AutoSize = False
              Caption = 'MRC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = #24494#36575#27491#40657#39636
              Font.Style = []
              ParentFont = False
            end
            object Label9: TLabel
              Left = 52
              Top = 20
              Width = 65
              Height = 24
              AutoSize = False
              Caption = 'SRC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = #24494#36575#27491#40657#39636
              Font.Style = []
              ParentFont = False
            end
            object cbMrcSrc: TComboBox
              Left = 12
              Top = 48
              Width = 305
              Height = 30
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -19
              Font.Name = 'Arial'
              Font.Style = []
              ItemHeight = 22
              ParentFont = False
              TabOrder = 0
              Text = '0101 '#35712#21462#35373#23450#20540
              OnChange = cbMrcSrcChange
              Items.Strings = (
                '0101 '#35712#21462#35373#23450#20540
                '0102 '#23531#20837#35373#23450#20540
                '0104 '#35712#21462#22810#20491#38750#36899#32396#35373#23450#20540
                '0113 '#23531#20837#22810#20491#38750#36899#32396#35373#23450#20540
                '0110 '#35079#21512'Reg'#35712#21462
                '0111 '#35079#21512#35712#21462'Reg'
                '0112 '#35079#21512#35712#21462'Reg'#30906#35469
                '0503 '#35712#21462#25511#21046#22120#22411#34399#33287#32233#34909#22823#23567
                '05A0 '#35712#21462#25511#21046#22120#22411#34399
                '0601 '#25511#21046#22120#29376#24907#35712#21462
                '0801 '#22238#38899#28204#35430
                '3005 '#25805#20316#21629#20196)
            end
            object pcMrcSrc: TPageControl
              Left = 8
              Top = 80
              Width = 389
              Height = 197
              ActivePage = tsReadVariable
              TabIndex = 0
              TabOrder = 1
              object tsReadVariable: TTabSheet
                Caption = '0101'
                object Label32: TLabel
                  Left = 8
                  Top = 32
                  Width = 121
                  Height = 24
                  AutoSize = False
                  Caption = #35722#37327#39006#22411' (2BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label33: TLabel
                  Left = 136
                  Top = 32
                  Width = 145
                  Height = 24
                  AutoSize = False
                  Caption = #35712#21462#36215#22987#20301#32622' (4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label34: TLabel
                  Left = 4
                  Top = 100
                  Width = 153
                  Height = 24
                  AutoSize = False
                  Caption = #20301#30340#20301#32622' ('#22266#23450#28858'00)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label35: TLabel
                  Left = 164
                  Top = 100
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #20803#25976#25976#37327'(4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label36: TLabel
                  Left = 0
                  Top = 8
                  Width = 181
                  Height = 24
                  AutoSize = False
                  Caption = #35712#21462#35722#37327#21312#22495
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label11: TLabel
                  Left = 60
                  Top = 132
                  Width = 22
                  Height = 22
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Edit7: TEdit
                  Left = 72
                  Top = 60
                  Width = 57
                  Height = 28
                  MaxLength = 1
                  TabOrder = 1
                  Text = '4'
                end
                object Edit8: TEdit
                  Left = 140
                  Top = 60
                  Width = 89
                  Height = 28
                  MaxLength = 4
                  TabOrder = 2
                  Text = '0000'
                end
                object Edit9: TEdit
                  Left = 164
                  Top = 132
                  Width = 89
                  Height = 28
                  MaxLength = 4
                  TabOrder = 3
                  Text = '0001'
                end
                object ComboBox5: TComboBox
                  Left = 4
                  Top = 60
                  Width = 65
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
              end
              object tbWriteVariable: TTabSheet
                Caption = '0102'
                ImageIndex = 1
                object Label28: TLabel
                  Left = 140
                  Top = 36
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #23531#20837#20301#22336' (4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label30: TLabel
                  Left = 164
                  Top = 100
                  Width = 161
                  Height = 24
                  AutoSize = False
                  Caption = #23531#20837#25976#25818'('#25976#37327'X4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label31: TLabel
                  Left = 0
                  Top = 8
                  Width = 181
                  Height = 24
                  AutoSize = False
                  Caption = #23531#20837#35722#37327#21312#22495
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label12: TLabel
                  Left = 8
                  Top = 36
                  Width = 121
                  Height = 24
                  AutoSize = False
                  Caption = #35722#37327#39006#22411' (2BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label38: TLabel
                  Left = 4
                  Top = 100
                  Width = 153
                  Height = 24
                  AutoSize = False
                  Caption = #20301#30340#20301#32622' ('#22266#23450#28858'00)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label40: TLabel
                  Left = 60
                  Top = 132
                  Width = 22
                  Height = 22
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Edit6: TEdit
                  Left = 72
                  Top = 64
                  Width = 57
                  Height = 28
                  MaxLength = 1
                  TabOrder = 1
                  Text = '0'
                end
                object Edit10: TEdit
                  Left = 140
                  Top = 64
                  Width = 89
                  Height = 28
                  MaxLength = 4
                  TabOrder = 2
                  Text = '0000'
                end
                object Edit11: TEdit
                  Left = 164
                  Top = 132
                  Width = 89
                  Height = 28
                  TabOrder = 3
                  Text = '270F'
                end
                object ComboBox3: TComboBox
                  Left = 4
                  Top = 64
                  Width = 65
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
              end
              object tsCompositeRead: TTabSheet
                Caption = '0104'
                ImageIndex = 2
                object Label22: TLabel
                  Left = 0
                  Top = 4
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #35722#37327#39006#22411' (2BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label23: TLabel
                  Left = 92
                  Top = 4
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #35712#21462#20301#22336' (4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label27: TLabel
                  Left = 148
                  Top = 32
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label1: TLabel
                  Left = 148
                  Top = 65
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label2: TLabel
                  Left = 148
                  Top = 97
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label24: TLabel
                  Left = 148
                  Top = 129
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Edit4: TEdit
                  Left = 77
                  Top = 27
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 2
                  Text = '0000'
                end
                object Edit12: TEdit
                  Left = 77
                  Top = 60
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 5
                  Text = '0100'
                end
                object Edit13: TEdit
                  Left = 77
                  Top = 92
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 8
                  Text = '0200'
                end
                object Edit14: TEdit
                  Left = 77
                  Top = 124
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 11
                  Text = '0300'
                end
                object ComboBox6: TComboBox
                  Left = 4
                  Top = 26
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit15: TEdit
                  Left = 45
                  Top = 27
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 1
                  Text = '0'
                end
                object ComboBox7: TComboBox
                  Left = 4
                  Top = 59
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 3
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit16: TEdit
                  Left = 45
                  Top = 60
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 4
                  Text = '0'
                end
                object ComboBox8: TComboBox
                  Left = 4
                  Top = 91
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 6
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit17: TEdit
                  Left = 45
                  Top = 92
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 7
                  Text = '0'
                end
                object ComboBox9: TComboBox
                  Left = 4
                  Top = 123
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 9
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit18: TEdit
                  Left = 45
                  Top = 124
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 10
                  Text = '0'
                end
              end
              object tsCompositeWrite: TTabSheet
                Caption = '0113'
                ImageIndex = 4
                object Label21: TLabel
                  Left = 184
                  Top = 4
                  Width = 129
                  Height = 24
                  AutoSize = False
                  Caption = #23531#20837#25976#25818'('#25976#37327'X4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label13: TLabel
                  Left = 0
                  Top = 4
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #35722#37327#39006#22411' (2BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label14: TLabel
                  Left = 92
                  Top = 4
                  Width = 93
                  Height = 24
                  AutoSize = False
                  Caption = #35712#21462#20301#22336' (4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label15: TLabel
                  Left = 148
                  Top = 32
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label16: TLabel
                  Left = 148
                  Top = 65
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label29: TLabel
                  Left = 148
                  Top = 97
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label37: TLabel
                  Left = 148
                  Top = 129
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Edit3: TEdit
                  Left = 77
                  Top = 27
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 2
                  Text = '0000'
                end
                object Edit19: TEdit
                  Left = 77
                  Top = 60
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 6
                  Text = '0100'
                end
                object Edit20: TEdit
                  Left = 77
                  Top = 92
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 10
                  Text = '0200'
                end
                object Edit21: TEdit
                  Left = 77
                  Top = 124
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 14
                  Text = '0300'
                end
                object ComboBox1: TComboBox
                  Left = 4
                  Top = 26
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = 'E'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit22: TEdit
                  Left = 45
                  Top = 27
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 1
                  Text = '0'
                end
                object ComboBox2: TComboBox
                  Left = 4
                  Top = 59
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 4
                  Text = 'E'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit23: TEdit
                  Left = 45
                  Top = 60
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 5
                  Text = '0'
                end
                object ComboBox10: TComboBox
                  Left = 4
                  Top = 91
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 8
                  Text = 'E'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit24: TEdit
                  Left = 45
                  Top = 92
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 9
                  Text = '0'
                end
                object ComboBox11: TComboBox
                  Left = 4
                  Top = 123
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 12
                  Text = 'E'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit25: TEdit
                  Left = 45
                  Top = 124
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 13
                  Text = '0'
                end
                object Edit26: TEdit
                  Left = 173
                  Top = 27
                  Width = 90
                  Height = 28
                  MaxLength = 8
                  TabOrder = 3
                  Text = '00000001'
                end
                object Edit27: TEdit
                  Left = 173
                  Top = 60
                  Width = 90
                  Height = 28
                  MaxLength = 8
                  TabOrder = 7
                  Text = '00000001'
                end
                object Edit28: TEdit
                  Left = 173
                  Top = 92
                  Width = 90
                  Height = 28
                  MaxLength = 8
                  TabOrder = 11
                  Text = '00000001'
                end
                object Edit29: TEdit
                  Left = 173
                  Top = 124
                  Width = 90
                  Height = 28
                  MaxLength = 8
                  TabOrder = 15
                  Text = '00000001'
                end
              end
              object tsCompositeReadReg: TTabSheet
                Caption = '0111'
                ImageIndex = 4
                object Label17: TLabel
                  Left = 0
                  Top = 4
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #35722#37327#39006#22411' (2BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label18: TLabel
                  Left = 92
                  Top = 4
                  Width = 125
                  Height = 24
                  AutoSize = False
                  Caption = #35712#21462#20301#22336' (4BIT)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -13
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label19: TLabel
                  Left = 148
                  Top = 32
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label20: TLabel
                  Left = 148
                  Top = 65
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label39: TLabel
                  Left = 148
                  Top = 97
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Label41: TLabel
                  Left = 148
                  Top = 129
                  Width = 18
                  Height = 18
                  Caption = '00'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'Arial'
                  Font.Style = []
                  ParentFont = False
                end
                object Edit5: TEdit
                  Left = 77
                  Top = 27
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 2
                  Text = '0000'
                end
                object Edit30: TEdit
                  Left = 77
                  Top = 60
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 5
                  Text = '0100'
                end
                object Edit31: TEdit
                  Left = 77
                  Top = 92
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 8
                  Text = '0200'
                end
                object Edit32: TEdit
                  Left = 77
                  Top = 124
                  Width = 70
                  Height = 28
                  MaxLength = 4
                  TabOrder = 11
                  Text = '0300'
                end
                object ComboBox12: TComboBox
                  Left = 4
                  Top = 26
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit33: TEdit
                  Left = 45
                  Top = 27
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 1
                  Text = '0'
                end
                object ComboBox13: TComboBox
                  Left = 4
                  Top = 59
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 3
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit34: TEdit
                  Left = 45
                  Top = 60
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 4
                  Text = '0'
                end
                object ComboBox14: TComboBox
                  Left = 4
                  Top = 91
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 6
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit35: TEdit
                  Left = 45
                  Top = 92
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 7
                  Text = '0'
                end
                object ComboBox15: TComboBox
                  Left = 4
                  Top = 123
                  Width = 40
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 9
                  Text = 'C'
                  OnChange = cbMrcSrcChange
                  Items.Strings = (
                    'C'
                    'D'
                    'E'
                    'F')
                end
                object Edit36: TEdit
                  Left = 45
                  Top = 124
                  Width = 30
                  Height = 28
                  MaxLength = 1
                  TabOrder = 10
                  Text = '0'
                end
              end
              object tsEchoTest: TTabSheet
                Caption = '0801'
                ImageIndex = 5
                object Label6: TLabel
                  Left = 8
                  Top = 4
                  Width = 161
                  Height = 24
                  AutoSize = False
                  Caption = #28204#35430#25976#25818' (0~323)'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Edit1: TEdit
                  Left = 8
                  Top = 32
                  Width = 149
                  Height = 28
                  MaxLength = 323
                  TabOrder = 0
                  OnClick = Edit1Click
                end
              end
              object tsOperationCommand: TTabSheet
                Caption = '3005'
                ImageIndex = 6
                object Label7: TLabel
                  Left = 8
                  Top = 4
                  Width = 85
                  Height = 24
                  AutoSize = False
                  Caption = #21629#20196#20195#30908
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object Label10: TLabel
                  Left = 8
                  Top = 72
                  Width = 85
                  Height = 24
                  AutoSize = False
                  Caption = #30456#38364#35338#24687
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = #24494#36575#27491#40657#39636
                  Font.Style = []
                  ParentFont = False
                end
                object ComboBox4: TComboBox
                  Left = 8
                  Top = 28
                  Width = 169
                  Height = 30
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'Arial'
                  Font.Style = []
                  ItemHeight = 22
                  ParentFont = False
                  TabOrder = 0
                  Text = '04 '#23531#20837#27169#24335
                  Items.Strings = (
                    '04 '#23531#20837#27169#24335
                    '06 '#36575#39636#24489#20301
                    '0A '#36939#34892' (RUN)'
                    '0B '#20572#27490' (STOP)'
                    '0C '#25163#21205
                    '0D '#33258#21205
                    '0E 40% AT'
                    '0F 100% AT'
                    '10 AT'#21462#28040
                    '1E '#32068'0'#35722#26356
                    '1F '#32068'1'#35722#26356
                    '20 '#32068'2'#35722#26356
                    '21 '#32068'3'#35722#26356
                    '22 '#26412#22320'SP'#35722#26356
                    '23 '#36960#31243'SP'#35722#26356
                    '2D '#24489#20301#37679#35492
                    '32 '#35686#22577'1'#20445#25345#35299#38500
                    '33 '#35686#22577'2'#20445#25345#35299#38500
                    '34 '#35686#22577'3'#20445#25345#35299#38500
                    '35 '#25152#26377#20445#25345#35299#38500
                    '37 '#20445#23384'RAM'#25976#25818
                    '38 '#21443#25976#21021#22987#21270
                    '39 '#20445#23384'RAM'#25976#25818
                    '3B '#30331#35352#21934#20803#37197#32622)
                end
                object Edit2: TEdit
                  Left = 8
                  Top = 96
                  Width = 129
                  Height = 28
                  MaxLength = 323
                  TabOrder = 1
                end
              end
            end
          end
          object btSend: TButton
            Left = 344
            Top = 364
            Width = 73
            Height = 21
            Caption = 'Send'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btSendClick
          end
        end
      end
      object tsSimple: TTabSheet
        Caption = 'Parameter'
        ImageIndex = 2
        object btSetSV: TButton
          Left = 15
          Top = 211
          Width = 121
          Height = 50
          Caption = 'Set SV'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btSetSVClick
        end
        object btResetCOM: TButton
          Left = 15
          Top = 277
          Width = 121
          Height = 50
          Caption = 'Reset COM'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btResetCOMClick
        end
        object Button3: TButton
          Left = 16
          Top = 344
          Width = 121
          Height = 50
          Caption = 'Stop Control'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = Button1Click
        end
        object gbAT: TGroupBox
          Left = 4
          Top = 4
          Width = 413
          Height = 177
          Caption = 'AT && RT'
          TabOrder = 0
          object btAT: TButton
            Left = 15
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT ON (For All)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btATClick
          end
          object btATOff: TButton
            Left = 283
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT OFF'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btATOffClick
          end
          object cb40PercentAT: TCheckBox
            Left = 16
            Top = 84
            Width = 129
            Height = 17
            Caption = '40% AT'
            Checked = True
            State = cbChecked
            TabOrder = 3
          end
          object btAT1by1: TButton
            Left = 151
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT ON (1 by 1)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btAT1by1Click
          end
          object btRT: TButton
            Left = 15
            Top = 116
            Width = 121
            Height = 50
            Caption = 'RT ON (For All)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = btRTClick
          end
          object btRT1by1: TButton
            Left = 151
            Top = 116
            Width = 121
            Height = 50
            Caption = 'RT ON (1 by 1)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            OnClick = btRT1by1Click
          end
          object btRTOff: TButton
            Left = 283
            Top = 116
            Width = 121
            Height = 50
            Caption = 'RT OFF'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            OnClick = btRTOffClick
          end
        end
        object grpStatus: TGroupBox
          Left = 152
          Top = 188
          Width = 269
          Height = 229
          Caption = 'Module Status'
          TabOrder = 1
          object ledEJ1N1: TMyLed
            Left = 24
            Top = 24
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N2: TMyLed
            Left = 24
            Top = 49
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N3: TMyLed
            Left = 24
            Top = 74
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N4: TMyLed
            Left = 24
            Top = 99
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N5: TMyLed
            Left = 24
            Top = 125
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N6: TMyLed
            Left = 24
            Top = 150
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N7: TMyLed
            Left = 24
            Top = 175
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ledEJ1N8: TMyLed
            Left = 24
            Top = 200
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object Label25: TLabel
            Left = 52
            Top = 24
            Width = 78
            Height = 22
            Caption = 'Module 1'
          end
          object Label42: TLabel
            Left = 52
            Top = 49
            Width = 78
            Height = 22
            Caption = 'Module 2'
          end
          object Label43: TLabel
            Left = 52
            Top = 74
            Width = 78
            Height = 22
            Caption = 'Module 3'
          end
          object Label44: TLabel
            Left = 52
            Top = 99
            Width = 78
            Height = 22
            Caption = 'Module 4'
          end
          object Label49: TLabel
            Left = 52
            Top = 125
            Width = 78
            Height = 22
            Caption = 'Module 5'
          end
          object Label50: TLabel
            Left = 52
            Top = 150
            Width = 78
            Height = 22
            Caption = 'Module 6'
          end
          object Label51: TLabel
            Left = 52
            Top = 175
            Width = 78
            Height = 22
            Caption = 'Module 7'
          end
          object Label52: TLabel
            Left = 52
            Top = 200
            Width = 78
            Height = 22
            Caption = 'Module 8'
          end
        end
      end
      object tsRecord: TTabSheet
        Caption = 'Record'
        ImageIndex = 3
        object Memo1: TMemo
          Left = 0
          Top = 0
          Width = 429
          Height = 393
          Align = alClient
          Color = 14670284
          ScrollBars = ssBoth
          TabOrder = 0
        end
        object Panel11: TPanel
          Left = 0
          Top = 393
          Width = 429
          Height = 41
          Align = alBottom
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 1
          object cbGetRecord: TCheckBox
            Left = 4
            Top = 12
            Width = 149
            Height = 17
            Caption = 'Get Record'
            TabOrder = 0
          end
          object btSaveData: TButton
            Left = 264
            Top = 12
            Width = 145
            Height = 25
            Caption = 'Save Data'
            TabOrder = 1
            OnClick = btSaveDataClick
          end
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 437
    Top = 0
    Width = 889
    Height = 471
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 0
      Top = 0
      Width = 889
      Height = 471
      Align = alClient
      Caption = 'Auto Run'
      Color = 12761254
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object ScrollBox1: TScrollBox
        Left = 2
        Top = 20
        Width = 885
        Height = 449
        Align = alClient
        BiDiMode = bdLeftToRight
        BorderStyle = bsNone
        Color = 12761254
        ParentBiDiMode = False
        ParentColor = False
        TabOrder = 0
        object GroupBox: TGroupBox
          Left = 10
          Top = 10
          Width = 206
          Height = 120
          Caption = 'myPalSample'
          DragKind = dkDock
          TabOrder = 0
          Visible = False
          object Panel2: TPanel
            Left = 72
            Top = 20
            Width = 129
            Height = 44
            BevelOuter = bvNone
            Caption = '##.##'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clYellow
            Font.Height = 48
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object Panel3: TPanel
            Left = 4
            Top = 20
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = #8451
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clYellow
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel4: TPanel
            Left = 4
            Top = 39
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'RUN'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object Panel5: TPanel
            Left = 4
            Top = 77
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'AT'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object Panel6: TPanel
            Left = 4
            Top = 58
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'Input Error'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object Panel8: TPanel
            Left = 72
            Top = 64
            Width = 129
            Height = 21
            BevelOuter = bvNone
            Caption = 'Event'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object Panel10: TPanel
            Left = 4
            Top = 96
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'SP'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4227327
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object edSV: TEdit
            Left = 72
            Top = 86
            Width = 129
            Height = 27
            AutoSize = False
            BorderStyle = bsNone
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = 24
            Font.Name = 'Arial'
            Font.Style = []
            MaxLength = 3
            ParentFont = False
            TabOrder = 7
            Text = '0'
          end
          object cbEnableAT: TCheckBox
            Left = 48
            Top = 76
            Width = 17
            Height = 17
            Color = clGray
            ParentColor = False
            TabOrder = 5
          end
        end
        object GroupBox1: TGroupBox
          Left = 10
          Top = 138
          Width = 206
          Height = 120
          Caption = 'myPalSample'
          DragKind = dkDock
          TabOrder = 1
          Visible = False
          object Image1: TImage
            Left = 4
            Top = 20
            Width = 197
            Height = 95
          end
          object Edit37: TEdit
            Left = 72
            Top = 86
            Width = 128
            Height = 27
            AutoSize = False
            BiDiMode = bdLeftToRight
            BorderStyle = bsNone
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = 24
            Font.Name = 'Arial'
            Font.Style = []
            MaxLength = 3
            ParentBiDiMode = False
            ParentFont = False
            TabOrder = 0
            Text = '0'
          end
        end
      end
    end
  end
  object Panel9: TPanel
    Left = 0
    Top = 471
    Width = 1326
    Height = 293
    Align = alBottom
    TabOrder = 2
    object meLogs: TMemo
      Left = 1
      Top = 1
      Width = 1324
      Height = 291
      Align = alClient
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object Timer2: TTimer
    Enabled = False
    OnTimer = Timer2Timer
    Left = 574
  end
  object CommOmron: TComm
    CommName = 'COM13'
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
    ByteSize = _7
    Parity = Even
    StopBits = _2
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = CommOmronReceiveData
    Left = 602
  end
end
