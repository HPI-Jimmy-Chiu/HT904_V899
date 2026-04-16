object fQAMode: TfQAMode
  Left = 761
  Top = 257
  Width = 502
  Height = 500
  Caption = 'QA Mode'
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
  object btnApply: TButton
    Left = 12
    Top = 415
    Width = 200
    Height = 40
    Caption = 'Save'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = btnApplyClick
  end
  object btnOk: TButton
    Left = 257
    Top = 415
    Width = 200
    Height = 40
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = btnOkClick
  end
  object pgcQAMode: TPageControl
    Left = 0
    Top = 0
    Width = 494
    Height = 405
    ActivePage = tsQAMode
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    TabWidth = 100
    object tsQAMode: TTabSheet
      Caption = 'QA Mode'
      object gbQAMode: TGroupBox
        Left = 0
        Top = 0
        Width = 486
        Height = 374
        Align = alClient
        Caption = 'QA Mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'Arial Black'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label34: TLabel
          Left = 12
          Top = 32
          Width = 111
          Height = 20
          Caption = 'Device Counts :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label35: TLabel
          Left = 76
          Top = 57
          Width = 182
          Height = 20
          Caption = '*Count limitation: 5-10000'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labQAModeBin: TLabel
          Left = 12
          Top = 216
          Width = 214
          Height = 20
          Caption = 'Setted bin for untest devices : '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edQAMode: TEdit
          Left = 136
          Top = 28
          Width = 121
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '50'
          OnMouseDown = edQAModeMouseDown
        end
        object rgQARunMode: TRadioGroup
          Left = 8
          Top = 104
          Width = 461
          Height = 105
          Caption = 'Running Mode after QA Mode Finish'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            'Clean out every devices without test automatically.'
            'One cycle and alarm, then normal production.'
            'Clean out every devices as setted bin automatically.'
            'Not counting Device.Need to end manually.')
          ParentFont = False
          TabOrder = 2
        end
        object cbQAModeBin: TComboBox
          Left = 232
          Top = 212
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 3
          Text = '1'
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
        object gbLoaderDirection: TGroupBox
          Left = 372
          Top = 15
          Width = 94
          Height = 87
          Caption = 'Loader'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Image1: TImage
            Left = 18
            Top = 20
            Width = 61
            Height = 61
            AutoSize = True
            OnClick = Image1Click
          end
        end
        object cbTrayEndDoQAModeAgain: TCheckBox
          Left = 12
          Top = 256
          Width = 349
          Height = 17
          Caption = 'Tray end need to do QA mode again'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object cbTrayEndCloseYield100Site: TCheckBox
          Left = 12
          Top = 304
          Width = 325
          Height = 17
          Caption = 'Tray end need to close the yield 100% site'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object cbQAD22Double: TCheckBox
          Left = 12
          Top = 280
          Width = 253
          Height = 17
          Caption = '[D22] Double Contact Function'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object cbbQAD22Double: TComboBox
          Left = 259
          Top = 274
          Width = 115
          Height = 28
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 5
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
      end
    end
    object tsQASampling: TTabSheet
      Caption = 'QA Sampling'
      ImageIndex = 1
      object pnlQASample: TPanel
        Left = 0
        Top = 0
        Width = 486
        Height = 374
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object lblQASampleBin: TLabel
          Left = 20
          Top = 48
          Width = 93
          Height = 20
          Caption = 'Sampling Bin'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblQASampleTray: TLabel
          Left = 20
          Top = 84
          Width = 100
          Height = 20
          Caption = 'Sampling Tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 20
          Top = 120
          Width = 113
          Height = 20
          Caption = 'Sampling Count'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbTempOffset: TCheckBox
          Left = 8
          Top = 12
          Width = 425
          Height = 25
          Caption = 'Enable QA sampling function'
          Checked = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -19
          Font.Name = 'Arial Black'
          Font.Style = [fsBold]
          ParentFont = False
          State = cbChecked
          TabOrder = 0
        end
        object cbbQASampleBin: TComboBox
          Left = 144
          Top = 44
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ItemIndex = 1
          ParentFont = False
          TabOrder = 1
          Text = '1'
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
        object cbbQASampleTray: TComboBox
          Left = 144
          Top = 80
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ItemIndex = 3
          ParentFont = False
          TabOrder = 2
          Text = 'Fix 1'
          Items.Strings = (
            'Auto 1'
            'Auto 2'
            'Auto 3'
            'Fix 1'
            'Fix 2'
            'Fix 3')
        end
        object edtQASampleCnt: TEdit
          Left = 144
          Top = 116
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '100'
          OnMouseDown = edtQASampleCntMouseDown
        end
      end
    end
  end
end
