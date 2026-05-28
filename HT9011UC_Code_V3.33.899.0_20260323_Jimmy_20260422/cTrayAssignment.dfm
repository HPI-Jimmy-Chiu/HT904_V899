object fTrayAssignment: TfTrayAssignment
  Left = 67
  Top = 148
  Width = 1490
  Height = 847
  Caption = 'Tray Assignment'
  Color = 12761254
  DragKind = dkDock
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clNavy
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object palTrayAssign: TPanel
    Left = 0
    Top = 0
    Width = 1474
    Height = 429
    Align = alTop
    BevelInner = bvLowered
    Color = 12761254
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object gbAuto3: TGroupBox
      Left = 704
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      object imgAuto3: TImage
        Tag = 3
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto3Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto3: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object gbAuto2: TGroupBox
      Left = 565
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      object imgAuto2: TImage
        Tag = 2
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto2Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto2: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object gbAuto1: TGroupBox
      Left = 426
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      object imgAuto1: TImage
        Tag = 1
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto1Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto1: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object gbColorTray: TGroupBox
      Left = 286
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Color Tray'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      object cbColor: TComboBox
        Left = 10
        Top = 92
        Width = 109
        Height = 24
        Style = csDropDownList
        ItemHeight = 16
        TabOrder = 0
        OnChange = cbEmptyChange
        Items.Strings = (
          'Type1'
          'Type2'
          'Type3')
      end
    end
    object gbEmptyTray: TGroupBox
      Left = 147
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Empty Tray'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      object cbEmpty: TComboBox
        Left = 10
        Top = 92
        Width = 109
        Height = 24
        Style = csDropDownList
        ItemHeight = 16
        TabOrder = 0
        OnChange = cbEmptyChange
        Items.Strings = (
          'Type1'
          'Type2'
          'Type3')
      end
    end
    object GroupBox1: TGroupBox
      Left = 8
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Loader'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      object imgLoader: TImage
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object cbLoader: TComboBox
        Left = 10
        Top = 92
        Width = 109
        Height = 24
        Style = csDropDownList
        ItemHeight = 16
        TabOrder = 0
        OnChange = cbLoaderChange
        OnDropDown = cbLoaderDropDown
        Items.Strings = (
          'Type1'
          'Type2'
          'Type3')
      end
    end
    object pgRunMode: TPageControl
      Left = 2
      Top = 153
      Width = 1470
      Height = 120
      ActivePage = tsReTestGroup
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabIndex = 1
      TabOrder = 2
      TabWidth = 100
      object tsNormalTestGroup: TTabSheet
        Caption = 'Normal Test'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        object pnlFTTrayFrom: TPanel
          Left = 360
          Top = 0
          Width = 1102
          Height = 89
          Align = alClient
          BevelInner = bvLowered
          Color = 12761254
          TabOrder = 1
          object Shape11: TShape
            Left = 152
            Top = -16
            Width = 329
            Height = 1
          end
          object Label63: TLabel
            Left = 6
            Top = 36
            Width = 31
            Height = 16
            Caption = 'From'
          end
          object RGAuto1: TRadioGroup
            Left = 60
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 0
            OnClick = cbEmptyChange
          end
          object RGAuto2: TRadioGroup
            Left = 199
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 1
            OnClick = RGAuto2Click
          end
          object RGAuto3: TRadioGroup
            Left = 338
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 2
            OnClick = cbEmptyChange
          end
          object RGAuto4: TRadioGroup
            Left = 476
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 3
            OnClick = cbEmptyChange
          end
          object RGAuto5: TRadioGroup
            Left = 615
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 4
            OnClick = RGAuto2Click
          end
          object RGAuto6: TRadioGroup
            Left = 754
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 1
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 5
            OnClick = cbEmptyChange
          end
        end
        object pnlLoad_FT: TPanel
          Left = 0
          Top = 0
          Width = 360
          Height = 89
          Align = alLeft
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 0
          object Label64: TLabel
            Left = 2
            Top = 38
            Width = 17
            Height = 16
            Caption = 'To'
          end
          object lblHint: TLabel
            Left = 128
            Top = 16
            Width = 102
            Height = 16
            Caption = 'Loader to By P24'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object RGLoader: TRadioGroup
            Left = 26
            Top = 6
            Width = 97
            Height = 67
            Caption = 'FT'
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 0
            OnClick = RGLoaderClick
          end
        end
      end
      object tsReTestGroup: TTabSheet
        Caption = 'Retest'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ImageIndex = 1
        ParentFont = False
        object pnlRTTrayFrom: TPanel
          Left = 360
          Top = 0
          Width = 1102
          Height = 89
          Align = alClient
          BevelInner = bvLowered
          Color = 12761254
          TabOrder = 1
          object Shape1: TShape
            Left = 152
            Top = -16
            Width = 329
            Height = 1
          end
          object Label1: TLabel
            Left = 6
            Top = 36
            Width = 31
            Height = 16
            Caption = 'From'
          end
          object rgAuto1_RT: TRadioGroup
            Left = 60
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 0
            OnClick = cbEmptyChange
          end
          object rgAuto2_RT: TRadioGroup
            Left = 199
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 1
            OnClick = cbEmptyChange
          end
          object rgAuto3_RT: TRadioGroup
            Left = 338
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 2
            OnClick = cbEmptyChange
          end
          object rgAuto4_RT: TRadioGroup
            Left = 476
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 3
            OnClick = cbEmptyChange
          end
          object rgAuto5_RT: TRadioGroup
            Left = 615
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 4
            OnClick = cbEmptyChange
          end
          object rgAuto6_RT: TRadioGroup
            Left = 754
            Top = 2
            Width = 129
            Height = 67
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 5
            OnClick = cbEmptyChange
          end
        end
        object pnlLoad_RT: TPanel
          Left = 0
          Top = 0
          Width = 360
          Height = 89
          Align = alLeft
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 0
          object Label2: TLabel
            Left = 2
            Top = 38
            Width = 17
            Height = 16
            Caption = 'To'
          end
          object lblHint2: TLabel
            Left = 200
            Top = 32
            Width = 74
            Height = 16
            Caption = 'Lock By P24'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object rgLoad_RT: TRadioGroup
            Left = 26
            Top = 6
            Width = 97
            Height = 67
            Caption = 'RT'
            ItemIndex = 0
            Items.Strings = (
              '[1] Empty'
              '[2] Color')
            TabOrder = 0
            OnClick = rgLoad_RTClick
          end
        end
      end
      object tsNormalTestGraph: TTabSheet
        Caption = 'Normal Test'
        ImageIndex = 2
        object ImgNormalTest: TImage
          Left = 0
          Top = 0
          Width = 1235
          Height = 89
          Align = alClient
        end
        object sbNormalTest: TScrollBar
          Left = 1235
          Top = 0
          Width = 17
          Height = 89
          Align = alRight
          Kind = sbVertical
          Max = 15
          PageSize = 0
          TabOrder = 0
          OnChange = sbNormalTestChange
        end
      end
      object tsReTestGraph: TTabSheet
        Caption = 'Re-Test'
        ImageIndex = 3
        object ImgReTest: TImage
          Left = 0
          Top = 0
          Width = 1235
          Height = 89
          Align = alClient
        end
        object sbNormalTest_RT: TScrollBar
          Left = 1235
          Top = 0
          Width = 17
          Height = 89
          Align = alRight
          Kind = sbVertical
          Max = 15
          PageSize = 0
          TabOrder = 0
          OnChange = sbNormalTest_RTChange
        end
      end
    end
    object pnlTop: TPanel
      Left = 2
      Top = 2
      Width = 1470
      Height = 151
      Align = alTop
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 0
      object rgLoaderType: TRadioGroup
        Left = 8
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Loader Type'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemIndex = 1
        Items.Strings = (
          'Same'
          'Different')
        ParentFont = False
        TabOrder = 0
        OnClick = rgLoaderTypeClick
      end
      object grpFix1: TGroupBox
        Left = 424
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object imgFix1: TImage
          Tag = 7
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix1: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix1: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix1: TCheckBox
          Left = 12
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
      object grpFix2: TGroupBox
        Left = 563
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        object imgFix2: TImage
          Tag = 8
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix2: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix2: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix2: TCheckBox
          Tag = 1
          Left = 8
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
      object grpFix3: TGroupBox
        Left = 702
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        object imgFix3: TImage
          Tag = 9
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix3: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix3: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix3: TCheckBox
          Tag = 2
          Left = 8
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
      object rgFixTrayMode: TRadioGroup
        Left = 147
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix Tray Mode'
        ItemIndex = 0
        Items.Strings = (
          '[1]  Full Bin'
          '[2] Up Down')
        TabOrder = 1
        OnClick = rgFixTrayModeClick
      end
      object gbRotateKit: TGroupBox
        Left = 278
        Top = 2
        Width = 139
        Height = 145
        Caption = 'Device Rotation'
        TabOrder = 2
        Visible = False
        object Label4: TLabel
          Left = 8
          Top = 35
          Width = 43
          Height = 16
          Caption = 'Loader'
        end
        object Label5: TLabel
          Left = 8
          Top = 91
          Width = 56
          Height = 16
          Caption = 'Unloader'
        end
        object cbInputRotateKIT: TComboBox
          Left = 66
          Top = 31
          Width = 70
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 0
          Items.Strings = (
            '0   Deg'
            '90  Deg'
            '-90 Deg')
        end
        object cbOutputRotateKIT: TComboBox
          Left = 66
          Top = 87
          Width = 70
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          TabOrder = 1
          Items.Strings = (
            '0   Deg'
            '90  Deg'
            '-90 Deg')
        end
      end
      object grpFix4: TGroupBox
        Left = 840
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 4'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        object imgFix4: TImage
          Tag = 10
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix4: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix4: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix4: TCheckBox
          Left = 12
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
      object grpFix5: TGroupBox
        Left = 979
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 5'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        object imgFix5: TImage
          Tag = 11
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix5: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix5: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix5: TCheckBox
          Tag = 1
          Left = 8
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
      object grpFix6: TGroupBox
        Left = 1118
        Top = 2
        Width = 129
        Height = 145
        Caption = 'Fix 6'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        object imgFix6: TImage
          Tag = 12
          Left = 34
          Top = 22
          Width = 61
          Height = 61
          AutoSize = True
          OnClick = imgLoaderClick
        end
        object labFix6: TLabel
          Left = 2
          Top = 127
          Width = 125
          Height = 16
          Align = alBottom
          Alignment = taCenter
          Caption = 'Has IC on tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object cbFix6: TComboBox
          Left = 10
          Top = 92
          Width = 109
          Height = 24
          Style = csDropDownList
          ItemHeight = 16
          ItemIndex = 0
          TabOrder = 0
          Text = 'Type1'
          Items.Strings = (
            'Type1'
            'Type2'
            'Type3')
        end
        object ckUseFix6: TCheckBox
          Tag = 2
          Left = 8
          Top = 120
          Width = 109
          Height = 17
          Caption = 'UpDown Tray'
          TabOrder = 1
        end
      end
    end
    object gbAuto6: TGroupBox
      Left = 1120
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 6'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 11
      object imgAuto6: TImage
        Tag = 6
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto6Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto6: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object gbAuto5: TGroupBox
      Left = 981
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 10
      object imgAuto5: TImage
        Tag = 5
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto5Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto5: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object gbAuto4: TGroupBox
      Left = 842
      Top = 277
      Width = 129
      Height = 146
      Caption = 'Auto 4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      object imgAuto4: TImage
        Tag = 4
        Left = 34
        Top = 22
        Width = 61
        Height = 61
        AutoSize = True
        OnClick = imgLoaderClick
      end
      object edAuto4Type: TEdit
        Left = 9
        Top = 92
        Width = 114
        Height = 24
        Enabled = False
        ReadOnly = True
        TabOrder = 0
      end
      object chkTrayEndRecvAuto4: TCheckBox
        Left = 12
        Top = 120
        Width = 109
        Height = 17
        Caption = 'Tray End Recv'
        TabOrder = 1
      end
    end
    object palAMR: TPanel
      Left = 1262
      Top = 10
      Width = 210
      Height = 415
      Color = 12761254
      TabOrder = 1
      object Label47: TLabel
        Left = 12
        Top = 130
        Width = 102
        Height = 16
        Caption = 'Input Tray Count :'
      end
      object Label49: TLabel
        Left = 12
        Top = 158
        Width = 95
        Height = 16
        Caption = 'Full Tray Count :'
      end
      object cbSpecTrayCnt: TCheckBox
        Left = 8
        Top = 97
        Width = 195
        Height = 17
        Caption = 'Manually specify the quantity.'
        TabOrder = 0
      end
      object edAMRInputTrayCount: TEdit
        Left = 119
        Top = 126
        Width = 49
        Height = 24
        TabOrder = 1
        Text = '0'
      end
      object edAMRFullTrayCount: TEdit
        Left = 119
        Top = 154
        Width = 49
        Height = 24
        TabOrder = 2
        Text = '0'
      end
      object grpVTestNoTBin: TGroupBox
        Left = 1
        Top = 184
        Width = 209
        Height = 137
        Caption = 'VTest'
        TabOrder = 3
        Visible = False
        object Label48: TLabel
          Left = 7
          Top = 51
          Width = 24
          Height = 16
          Caption = 'Fix1'
        end
        object Label50: TLabel
          Left = 7
          Top = 77
          Width = 24
          Height = 16
          Caption = 'Fix2'
        end
        object Label51: TLabel
          Left = 7
          Top = 103
          Width = 24
          Height = 16
          Caption = 'Fix3'
        end
        object edNoRTBinFix1: TEdit
          Left = 39
          Top = 48
          Width = 138
          Height = 24
          TabOrder = 1
          Text = 'FIX1@NORTBIN'
        end
        object edNoRTBinFix2: TEdit
          Left = 39
          Top = 74
          Width = 138
          Height = 24
          TabOrder = 2
          Text = 'FIX2@NORTBIN'
        end
        object edNoRTBinFix3: TEdit
          Left = 39
          Top = 100
          Width = 138
          Height = 24
          TabOrder = 3
          Text = 'FIX3@NORTBIN'
        end
        object cbVTestNoRTBin: TCheckBox
          Left = 8
          Top = 25
          Width = 195
          Height = 17
          Caption = 'No ReTest Bins (Fix Tray)'
          TabOrder = 0
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 746
    Width = 1474
    Height = 62
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 2
    object sbtExit: TSpeedButton
      Left = 504
      Top = 14
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
    object spbSave: TSpeedButton
      Left = 54
      Top = 15
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
    object rbTemp: TRadioButton
      Left = -77
      Top = 4
      Width = 113
      Height = 17
      Caption = 'rbTemp'
      TabOrder = 0
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 429
    Width = 1474
    Height = 317
    Align = alClient
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object Panel5: TPanel
      Left = 0
      Top = 0
      Width = 421
      Height = 317
      Align = alLeft
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 0
      object grpCheckLoaderDirection: TGroupBox
        Left = 0
        Top = 0
        Width = 421
        Height = 49
        Align = alTop
        Caption = 'Check loader tray direction'
        TabOrder = 0
        object chkCheckLoadDirection: TCheckBox
          Left = 20
          Top = 21
          Width = 353
          Height = 17
          Caption = 'Enable to check loader tray direction'
          TabOrder = 0
        end
      end
      object gbColorTrayMode: TGroupBox
        Left = 0
        Top = 49
        Width = 421
        Height = 49
        Align = alTop
        Caption = 'Use Color Tray Mode'
        TabOrder = 1
        object chkColorTray: TCheckBox
          Left = 20
          Top = 21
          Width = 353
          Height = 17
          Caption = 'Color Sensor Detect'
          TabOrder = 0
        end
      end
      object gbUnloadMode: TGroupBox
        Left = 0
        Top = 98
        Width = 421
        Height = 88
        Align = alTop
        Caption = 'Unloading Tray Mode'
        TabOrder = 2
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
      object rgLoaderTrayMode: TRadioGroup
        Left = 0
        Top = 186
        Width = 421
        Height = 90
        Align = alTop
        Caption = 'Loader Tray Mode'
        ItemIndex = 0
        Items.Strings = (
          'None'
          'When Loader had skip device, need remove loader tray manually.'
          'When Loader had skip device, need open door to check tray.')
        TabOrder = 3
      end
      object gbTrayArmMode: TGroupBox
        Left = 0
        Top = 276
        Width = 421
        Height = 40
        Align = alTop
        Caption = 'Tray Arm Motion'
        TabOrder = 4
        object cbMoveAfterTrayGoOut: TCheckBox
          Left = 20
          Top = 18
          Width = 353
          Height = 17
          Caption = 'Tray arm move to AUTO after tray goes outside.'
          TabOrder = 0
        end
      end
    end
    object grpICSortFunc: TGroupBox
      Left = 421
      Top = 0
      Width = 1053
      Height = 317
      Align = alClient
      Caption = 'Device Sort Funcrion'
      TabOrder = 1
      Visible = False
      object grpICSort3: TGroupBox
        Left = 282
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 3'
        TabOrder = 2
        object Label7: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label20: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label21: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label22: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label23: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label24: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label25: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort3: TCheckBox
          Left = 8
          Top = 24
          Width = 121
          Height = 17
          Caption = 'Enable Auto 3'
          TabOrder = 0
        end
        object edtICSort3_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort3_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort3_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort3_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort3_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort3_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
      object grpICSort2: TGroupBox
        Left = 142
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 2'
        TabOrder = 1
        object Label6: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label14: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label15: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label16: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label17: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label18: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label19: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort2: TCheckBox
          Left = 8
          Top = 24
          Width = 117
          Height = 17
          Caption = 'Enable Auto 2'
          TabOrder = 0
        end
        object edtICSort2_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort2_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort2_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort2_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort2_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort2_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
      object grpICSort1: TGroupBox
        Left = 2
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 1'
        TabOrder = 0
        object Label3: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label8: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label9: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label10: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label11: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label12: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label13: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort1: TCheckBox
          Left = 8
          Top = 24
          Width = 121
          Height = 17
          Caption = 'Enable Auto 1'
          TabOrder = 0
        end
        object edtICSort1_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort1_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort1_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort1_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort1_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort1_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
      object grpICSort4: TGroupBox
        Left = 422
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 4'
        TabOrder = 3
        object Label26: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label27: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label28: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label29: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label30: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label31: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label32: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort4: TCheckBox
          Left = 8
          Top = 24
          Width = 121
          Height = 17
          Caption = 'Enable Auto 4'
          TabOrder = 0
        end
        object edtICSort4_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort4_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort4_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort4_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort4_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort4_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
      object grpICSort6: TGroupBox
        Left = 702
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 6'
        TabOrder = 5
        object Label33: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label34: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label35: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label36: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label37: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label38: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label39: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort6: TCheckBox
          Left = 8
          Top = 24
          Width = 121
          Height = 17
          Caption = 'Enable Auto 6'
          TabOrder = 0
        end
        object edtICSort6_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort6_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort6_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort6_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort6_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort6_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
      object grpICSort5: TGroupBox
        Left = 562
        Top = 18
        Width = 140
        Height = 297
        Align = alLeft
        Caption = 'Auto 5'
        TabOrder = 4
        object Label40: TLabel
          Left = 8
          Top = 44
          Width = 121
          Height = 16
          Caption = 'No. of Tray       Count'
        end
        object Label41: TLabel
          Left = 36
          Top = 68
          Width = 7
          Height = 16
          Caption = '1'
        end
        object Label42: TLabel
          Left = 36
          Top = 98
          Width = 7
          Height = 16
          Caption = '2'
        end
        object Label43: TLabel
          Left = 36
          Top = 127
          Width = 7
          Height = 16
          Caption = '3'
        end
        object Label44: TLabel
          Left = 36
          Top = 156
          Width = 7
          Height = 16
          Caption = '4'
        end
        object Label45: TLabel
          Left = 36
          Top = 186
          Width = 7
          Height = 16
          Caption = '5'
        end
        object Label46: TLabel
          Left = 36
          Top = 217
          Width = 7
          Height = 16
          Caption = '6'
        end
        object chkICSort5: TCheckBox
          Left = 8
          Top = 24
          Width = 121
          Height = 17
          Caption = 'Enable Auto 5'
          TabOrder = 0
        end
        object edtICSort5_1: TEdit
          Left = 84
          Top = 64
          Width = 49
          Height = 24
          TabOrder = 1
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort5_2: TEdit
          Left = 84
          Top = 94
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort5_3: TEdit
          Left = 84
          Top = 123
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort5_4: TEdit
          Left = 84
          Top = 152
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort5_5: TEdit
          Left = 84
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
        object edtICSort5_6: TEdit
          Left = 84
          Top = 213
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '0'
          OnMouseDown = edtICSort1_1MouseDown
        end
      end
    end
  end
end
