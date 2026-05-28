object fProductionInfo: TfProductionInfo
  Left = 486
  Top = 133
  Width = 1155
  Height = 773
  Caption = 'Handler Production Informaition'
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 1139
    Height = 734
    ActivePage = tsCalibrateSuckZHeight
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    Style = tsButtons
    TabIndex = 5
    TabOrder = 0
    TabWidth = 150
    OnChange = PageControl1Change
    object tsInformation: TTabSheet
      Caption = 'Information'
      object PageControl2: TPageControl
        Left = 0
        Top = 65
        Width = 1131
        Height = 566
        ActivePage = TabSheet5
        Align = alClient
        Style = tsButtons
        TabIndex = 4
        TabOrder = 1
        object TabSheet1: TTabSheet
          Caption = 'OEE List'
          object sg_OEEList: TStringGrid
            Left = 0
            Top = 0
            Width = 1123
            Height = 528
            Align = alClient
            ColCount = 36
            DefaultColWidth = 50
            DefaultRowHeight = 20
            FixedCols = 0
            RowCount = 20
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
            ParentFont = False
            TabOrder = 0
            ColWidths = (
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50
              50)
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'Summary Report'
          ImageIndex = 1
          object sg_SummaryReport: TStringGrid
            Left = 0
            Top = 0
            Width = 902
            Height = 352
            Align = alClient
            ColCount = 2
            DefaultColWidth = 200
            DefaultRowHeight = 20
            FixedCols = 0
            RowCount = 20
            FixedRows = 0
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
            ParentFont = False
            TabOrder = 0
            ColWidths = (
              200
              311)
            RowHeights = (
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20)
          end
        end
        object TabSheet3: TTabSheet
          Caption = 'Setup Condition'
          ImageIndex = 2
          object sg_SetupCondition: TStringGrid
            Left = 0
            Top = 0
            Width = 902
            Height = 352
            Align = alClient
            ColCount = 2
            DefaultColWidth = 350
            DefaultRowHeight = 20
            FixedCols = 0
            RowCount = 20
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
            ParentFont = False
            TabOrder = 0
            RowHeights = (
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20)
          end
        end
        object TabSheet4: TTabSheet
          Caption = 'History'
          ImageIndex = 3
          object sg_History: TStringGrid
            Left = 0
            Top = 0
            Width = 902
            Height = 352
            Align = alClient
            ColCount = 6
            DefaultColWidth = 150
            DefaultRowHeight = 20
            FixedCols = 0
            RowCount = 20
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
            ParentFont = False
            TabOrder = 0
            ColWidths = (
              150
              102
              91
              498
              66
              72)
          end
        end
        object TabSheet5: TTabSheet
          Caption = 'ESD Control Msg'
          ImageIndex = 4
          object sg_ESDControlMsg: TStringGrid
            Left = 0
            Top = 0
            Width = 1123
            Height = 528
            Align = alClient
            ColCount = 2
            DefaultColWidth = 200
            DefaultRowHeight = 20
            FixedCols = 0
            RowCount = 20
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
            ParentFont = False
            TabOrder = 0
            ColWidths = (
              226
              347)
            RowHeights = (
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20
              20)
          end
        end
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 1131
        Height = 65
        Align = alTop
        BevelOuter = bvLowered
        TabOrder = 0
        object pnlPMAlarmBeforeDays: TPanel
          Left = 16
          Top = 16
          Width = 169
          Height = 28
          BevelOuter = bvNone
          Caption = 'Choice MO'
          Color = 383181
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object cb_PIChoiseMO: TComboBox
          Left = 192
          Top = 16
          Width = 425
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 0
          ParentFont = False
          TabOrder = 1
          OnKeyUp = cb_PIChoiseMOKeyUp
        end
        object bt_PIRefreshMOList: TButton
          Left = 624
          Top = 16
          Width = 81
          Height = 28
          Caption = 'Refresh'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          OnClick = bt_PIRefreshMOListClick
        end
        object coCheckFTPFileList: TComboBox
          Left = 731
          Top = 13
          Width = 145
          Height = 28
          Style = csDropDownList
          ItemHeight = 0
          TabOrder = 3
          Visible = False
        end
      end
      object Panel2: TPanel
        Left = 0
        Top = 631
        Width = 1131
        Height = 65
        Align = alBottom
        BevelOuter = bvLowered
        TabOrder = 2
        object bt_PICloseForm: TButton
          Left = 760
          Top = 8
          Width = 121
          Height = 49
          Caption = 'Close'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          OnClick = bt_PICloseFormClick
        end
      end
    end
    object tsFilePath: TTabSheet
      Caption = 'FilePath'
      ImageIndex = 2
      object dlb_PIChoiseMO: TDirectoryListBox
        Left = 325
        Top = 13
        Width = 161
        Height = 120
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemHeight = 16
        ParentFont = False
        TabOrder = 0
        Visible = False
      end
    end
    object tsHaltStatusForm: TTabSheet
      Caption = 'Halt Status Form'
      ImageIndex = 3
      object Label1: TLabel
        Left = 49
        Top = 96
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Insert OP.# :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label2: TLabel
        Left = 49
        Top = 152
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Status :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lbStatus_HALT: TLabel
        Left = 240
        Top = 152
        Width = 249
        Height = 29
        AutoSize = False
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
      end
      object Label6: TLabel
        Left = 49
        Top = 208
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Select Status :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object sbScreenkeyboard_HALT: TSpeedButton
        Left = 500
        Top = 99
        Width = 120
        Height = 25
        Caption = 'Keyboard'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Glyph.Data = {
          D2030000424DD20300000000000036000000280000001C0000000B0000000100
          1800000000009C030000C40E0000C40E00000000000000000000D8E5E6D7E4E6
          D6E2E4D3E2E3D3DFE1D4E1E3D8E4E7D7E4E6D5E1E3D6E3E5D6E3E5D6E3E5D6E3
          E5D6E3E5D6E3E5D6E3E5D6E3E5D6E3E5D6E3E5D4E2E3D8E6E7D4E2E4D7E4E6DD
          EAECDCE9EBD6E3E6D7E4E6D5E2E4E0ECEDD9E3E3DEECECCFD9DBC4CED1E0EDEE
          DDE8EBDEEBEDDCE8EAE3F0F2E0EDEEE2EFF1E1EEF0E3F2F3E1EEF0E2F0F1E2EF
          F1E1EFF0E3F0F3DDEAECDAE9E9DEEAEBDCEAECDEEBEDDEEBEDDFEBEED9E4E6DA
          E8EAD8E4E5D4E2E3D1DDDED8E6E7D7E5E8C0CACBD5E2E5BDC5C8D6E1E4C1CCCE
          DBE8EBC1CBCDD4E0E5B5BCBED3E1E4C6D0D3C5D0D5CFD8D9BEC8CACDD9DBBBC3
          C6D9E6E8BEC9CBD5E2E5D3E0E2D2DEE1D5E3E3D3E0E2DDEBEBD9E6E8D4E1E1E0
          ECEFD0DCD3DBE7E5CFDACFDBE8E9D3DDD4D6E3E6D4DFD2D9E6ECCBD5C1D6E2E5
          D1DCCDD6E2E2CED8C3D9E6E9DAE7E8DBE7E8D9E8EADBE6E1DDEAEDD9E4E6DCE9
          EAD7E3E3DCE8EAD7E4E6DBE8E9D7E3E4D9E6E8D8E5E7D2DEE0C9D4D6CCD7D8DA
          E6E9C5D0D2D8E5E8CAD5D6D9E7E9C8D3D4DDEAEEC3CFCFD8E4E6CDD8DAD2DEE2
          D0DADDC1CDCFDAE7E7B4BCBEDEEBECDAE6E8D8E5E6DDEAEBDBE8EAD6E3E5DEEA
          ECD7E3E4DAE7E8D7E3E2D2DDD5CED9CFDCE8EACFDBCED7E4E6CFD9CBD9E7EBD1
          DBCAD5E1E5CDD7C3D2DEDEDBE6E4D1DCDAD5E0D8D3DEDBD6E2E0D8E3DFDBE7EA
          D8E4E3DDEAECD4E1E3D7E2E3CCD7D9D5E3E5E0EDEEC2CED0DCE8EAD1DDE0CDD7
          DBC2CCCFD3DEE1CAD3D6D8E6E8BFC8CADEEBEDC2CDD0DFECEFC2CACDDAE8ECD2
          DDDFD3DEE1CBD6D8D5E1E4D4E1E2CDD8DCD4E1E1D9E7EAD7E3E4DBE9EBCAD4D6
          C6D1D4D4E1E3E1EEEEBAC5C6DDE9EAD0DCCED7E2E1D0D9C9CDD7C9DEEAEBCBD4
          C5DBE7E9D2DDD1DAE7ECCDD7C6DBE8ECCDD6C5D7E3E4D9E4E0D3DEDCD9E6E0D2
          DBD1D7E5E4D5DFD7DEECF1CED8CDDEEBEFDAE5E7DFECEED7E3E5DBE8E9D7E5E7
          D9E6E9C2CDD1CAD4D7CAD5DBD9E5EBD5E2E4D6E4E9E0EDF0A3AAAFE1EEF1D3DF
          E3D9E6E8D6E3E7D6E2E4CBD8DCC7D2D5C6CFD2C1CCCFCFDCDDB6C0C4DDEAEBDC
          E9EDD6E2E5D2DFE1D4DFE1D5E2E5EBF7F7DEEBEFD9E5DAE0EBE9DEEBE4D7E2DA
          E4F0F1D4DFD6E2EEF0D1DBD1E9F7FCD2DCCDE2EFF4D6E0D7D9E7E4DCE7E0DEEB
          E9C5CBC2E2EBE3E2EFECDDE8E8E8F5F7D4E0D7E1EFF2D7E1DBD9E7E9D9E4E6E0
          EDEF7D7B7CDBEAEBD9E5EAD7E4E7D8E4E8D9E5E9D7E3E6D8E5E9D7E3E6D9E6EA
          D7E3E5D9E5EAD7E3E6D9E5E9D8E5E8D8E5E9D7E4E6AAADAFD3DFE2D8E5E8D8E5
          E7D7E3E6D9E5E9D7E3E6D8E5E9DBE9EBC0C8C9A5ABAD}
        ParentFont = False
        Spacing = 10
        OnClick = sbScreenkeyboard_HALTClick
      end
      object SpeedButton_Halt00: TSpeedButton
        Left = 240
        Top = 200
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt01: TSpeedButton
        Left = 392
        Top = 200
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt02: TSpeedButton
        Left = 544
        Top = 200
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt03: TSpeedButton
        Left = 696
        Top = 200
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt04: TSpeedButton
        Left = 240
        Top = 264
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt05: TSpeedButton
        Left = 392
        Top = 264
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt06: TSpeedButton
        Left = 544
        Top = 264
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt07: TSpeedButton
        Left = 696
        Top = 264
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt08: TSpeedButton
        Left = 240
        Top = 328
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt09: TSpeedButton
        Left = 392
        Top = 328
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt10: TSpeedButton
        Left = 544
        Top = 328
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt11: TSpeedButton
        Left = 696
        Top = 328
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt12: TSpeedButton
        Left = 240
        Top = 392
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt13: TSpeedButton
        Left = 392
        Top = 392
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt14: TSpeedButton
        Left = 544
        Top = 392
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object SpeedButton_Halt15: TSpeedButton
        Left = 696
        Top = 392
        Width = 137
        Height = 49
        Visible = False
        OnClick = SpeedButton_Halt00Click
      end
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 1131
        Height = 65
        Align = alTop
        BevelOuter = bvLowered
        TabOrder = 0
        object Panel4: TPanel
          Left = 1
          Top = 1
          Width = 1129
          Height = 63
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Insert "HALT Status"'
          Color = clYellow
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -32
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
      object edInsertOPID_HALT: TEdit
        Left = 240
        Top = 96
        Width = 249
        Height = 28
        TabOrder = 1
        OnClick = edInsertOPID_HALTClick
      end
      object Button2: TButton
        Left = 248
        Top = 464
        Width = 345
        Height = 57
        Caption = 'Exit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -32
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = Button2Click
      end
      object Panel9: TPanel
        Left = 120
        Top = 65
        Width = 561
        Height = 24
        BevelOuter = bvLowered
        TabOrder = 3
        object pn_ErrorMsg_HALTStatus: TPanel
          Left = 1
          Top = 1
          Width = 559
          Height = 22
          Align = alClient
          BevelOuter = bvNone
          Color = clRed
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
    end
    object tsPauseStatusForm: TTabSheet
      Caption = 'Pause Status Form'
      ImageIndex = 4
      object Label7: TLabel
        Left = 49
        Top = 96
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Insert OP.# :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label8: TLabel
        Left = 49
        Top = 152
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Status :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label9: TLabel
        Left = 49
        Top = 208
        Width = 168
        Height = 29
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Select Status :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object sbScreenkeyboard_Pause: TSpeedButton
        Left = 500
        Top = 99
        Width = 120
        Height = 25
        Caption = 'Keyboard'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Glyph.Data = {
          D2030000424DD20300000000000036000000280000001C0000000B0000000100
          1800000000009C030000C40E0000C40E00000000000000000000D8E5E6D7E4E6
          D6E2E4D3E2E3D3DFE1D4E1E3D8E4E7D7E4E6D5E1E3D6E3E5D6E3E5D6E3E5D6E3
          E5D6E3E5D6E3E5D6E3E5D6E3E5D6E3E5D6E3E5D4E2E3D8E6E7D4E2E4D7E4E6DD
          EAECDCE9EBD6E3E6D7E4E6D5E2E4E0ECEDD9E3E3DEECECCFD9DBC4CED1E0EDEE
          DDE8EBDEEBEDDCE8EAE3F0F2E0EDEEE2EFF1E1EEF0E3F2F3E1EEF0E2F0F1E2EF
          F1E1EFF0E3F0F3DDEAECDAE9E9DEEAEBDCEAECDEEBEDDEEBEDDFEBEED9E4E6DA
          E8EAD8E4E5D4E2E3D1DDDED8E6E7D7E5E8C0CACBD5E2E5BDC5C8D6E1E4C1CCCE
          DBE8EBC1CBCDD4E0E5B5BCBED3E1E4C6D0D3C5D0D5CFD8D9BEC8CACDD9DBBBC3
          C6D9E6E8BEC9CBD5E2E5D3E0E2D2DEE1D5E3E3D3E0E2DDEBEBD9E6E8D4E1E1E0
          ECEFD0DCD3DBE7E5CFDACFDBE8E9D3DDD4D6E3E6D4DFD2D9E6ECCBD5C1D6E2E5
          D1DCCDD6E2E2CED8C3D9E6E9DAE7E8DBE7E8D9E8EADBE6E1DDEAEDD9E4E6DCE9
          EAD7E3E3DCE8EAD7E4E6DBE8E9D7E3E4D9E6E8D8E5E7D2DEE0C9D4D6CCD7D8DA
          E6E9C5D0D2D8E5E8CAD5D6D9E7E9C8D3D4DDEAEEC3CFCFD8E4E6CDD8DAD2DEE2
          D0DADDC1CDCFDAE7E7B4BCBEDEEBECDAE6E8D8E5E6DDEAEBDBE8EAD6E3E5DEEA
          ECD7E3E4DAE7E8D7E3E2D2DDD5CED9CFDCE8EACFDBCED7E4E6CFD9CBD9E7EBD1
          DBCAD5E1E5CDD7C3D2DEDEDBE6E4D1DCDAD5E0D8D3DEDBD6E2E0D8E3DFDBE7EA
          D8E4E3DDEAECD4E1E3D7E2E3CCD7D9D5E3E5E0EDEEC2CED0DCE8EAD1DDE0CDD7
          DBC2CCCFD3DEE1CAD3D6D8E6E8BFC8CADEEBEDC2CDD0DFECEFC2CACDDAE8ECD2
          DDDFD3DEE1CBD6D8D5E1E4D4E1E2CDD8DCD4E1E1D9E7EAD7E3E4DBE9EBCAD4D6
          C6D1D4D4E1E3E1EEEEBAC5C6DDE9EAD0DCCED7E2E1D0D9C9CDD7C9DEEAEBCBD4
          C5DBE7E9D2DDD1DAE7ECCDD7C6DBE8ECCDD6C5D7E3E4D9E4E0D3DEDCD9E6E0D2
          DBD1D7E5E4D5DFD7DEECF1CED8CDDEEBEFDAE5E7DFECEED7E3E5DBE8E9D7E5E7
          D9E6E9C2CDD1CAD4D7CAD5DBD9E5EBD5E2E4D6E4E9E0EDF0A3AAAFE1EEF1D3DF
          E3D9E6E8D6E3E7D6E2E4CBD8DCC7D2D5C6CFD2C1CCCFCFDCDDB6C0C4DDEAEBDC
          E9EDD6E2E5D2DFE1D4DFE1D5E2E5EBF7F7DEEBEFD9E5DAE0EBE9DEEBE4D7E2DA
          E4F0F1D4DFD6E2EEF0D1DBD1E9F7FCD2DCCDE2EFF4D6E0D7D9E7E4DCE7E0DEEB
          E9C5CBC2E2EBE3E2EFECDDE8E8E8F5F7D4E0D7E1EFF2D7E1DBD9E7E9D9E4E6E0
          EDEF7D7B7CDBEAEBD9E5EAD7E4E7D8E4E8D9E5E9D7E3E6D8E5E9D7E3E6D9E6EA
          D7E3E5D9E5EAD7E3E6D9E5E9D8E5E8D8E5E9D7E4E6AAADAFD3DFE2D8E5E8D8E5
          E7D7E3E6D9E5E9D7E3E6D8E5E9DBE9EBC0C8C9A5ABAD}
        ParentFont = False
        Spacing = 10
        OnClick = sbScreenkeyboard_HALTClick
      end
      object lbStatus_Pause: TLabel
        Left = 240
        Top = 152
        Width = 249
        Height = 29
        AutoSize = False
        Color = clSilver
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentColor = False
        ParentFont = False
      end
      object SpeedButton_Pause00: TSpeedButton
        Left = 240
        Top = 200
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause01: TSpeedButton
        Left = 392
        Top = 200
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause02: TSpeedButton
        Left = 544
        Top = 200
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause03: TSpeedButton
        Left = 696
        Top = 200
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause04: TSpeedButton
        Left = 240
        Top = 264
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause05: TSpeedButton
        Left = 392
        Top = 264
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause06: TSpeedButton
        Left = 544
        Top = 264
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause07: TSpeedButton
        Left = 696
        Top = 264
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause08: TSpeedButton
        Left = 240
        Top = 328
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause09: TSpeedButton
        Left = 392
        Top = 328
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause10: TSpeedButton
        Left = 544
        Top = 328
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause11: TSpeedButton
        Left = 696
        Top = 328
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause12: TSpeedButton
        Left = 240
        Top = 392
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause13: TSpeedButton
        Left = 392
        Top = 392
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause14: TSpeedButton
        Left = 544
        Top = 392
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object SpeedButton_Pause15: TSpeedButton
        Left = 696
        Top = 392
        Width = 137
        Height = 49
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = SpeedButton_Pause00Click
      end
      object Panel5: TPanel
        Left = 0
        Top = 0
        Width = 1131
        Height = 65
        Align = alTop
        BevelOuter = bvLowered
        TabOrder = 0
        object Panel6: TPanel
          Left = 1
          Top = 1
          Width = 1129
          Height = 63
          Align = alClient
          BevelOuter = bvNone
          Caption = 'Insert "Pause Status"'
          Color = clYellow
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -32
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
      object edInsertOPID_Pause: TEdit
        Left = 240
        Top = 96
        Width = 249
        Height = 28
        TabOrder = 1
      end
      object Button1: TButton
        Left = 248
        Top = 464
        Width = 345
        Height = 57
        Caption = 'Exit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -32
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 2
        OnClick = Button2Click
      end
      object Panel7: TPanel
        Left = 120
        Top = 65
        Width = 561
        Height = 24
        BevelOuter = bvLowered
        TabOrder = 3
        object pn_ErrorMsg_PauseStatus: TPanel
          Left = 1
          Top = 1
          Width = 559
          Height = 22
          Align = alClient
          BevelOuter = bvNone
          Color = clRed
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
    end
    object tsCompareData: TTabSheet
      Caption = 'Compare Data'
      ImageIndex = 4
      object Panel8: TPanel
        Left = 0
        Top = 0
        Width = 1131
        Height = 696
        Align = alClient
        Color = 12761254
        TabOrder = 0
        object pnl1: TPanel
          Left = 1
          Top = 1
          Width = 1129
          Height = 512
          Align = alTop
          Color = 12761254
          TabOrder = 0
          object sgDatas: TStringGrid
            Left = 1
            Top = 1
            Width = 1127
            Height = 510
            Align = alClient
            Color = 14670284
            DefaultColWidth = 75
            FixedCols = 0
            TabOrder = 0
          end
        end
        object pnl2: TPanel
          Left = 1
          Top = 513
          Width = 1129
          Height = 182
          Align = alClient
          Color = 12761254
          TabOrder = 1
          object btnCancel: TSpeedButton
            Left = 300
            Top = 16
            Width = 160
            Height = 50
            Caption = 'Abort'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            Glyph.Data = {
              361B0000424D361B000000000000360000002800000030000000300000000100
              180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF878C
              BB252B875E5FA8F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB6767BB
              5555CB9898D2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFF8F8FC82634791C25770E13703E3E90F6F6FAFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFF6F6FA3F3F922B2BA16868E22B2BAA8F8FC8FFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810A0E
              7F4E59A01216740000613A3A88F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA3A3A89000065111282
              8186E40B0B8B0000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFF8F8FC80000810000815860BD636AF27F89D8070868000055383883
              F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFF6F6FA37378200005807086D7F89D9656CF35860BD0000810000818F8FC8FF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810000815860BD4E54
              F10000F51B1DF57E88D507085B00004B35357EF6F6FAFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA35357D00004B0708607E88D71B1DF5
              0000F54E54F15860BD0000810000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8E
              8EC7000081000081585FBD4D54F10000F50000F50000F51A1DF57E87D307084E
              00004233337BF5F5FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5FA3333
              7A0000420708527E88D41B1DF50000F50000F50000F54E54F15860BD00008100
              00818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFF9898CC1B257C0B0F7F555EBD4B53F10000F50000
              F50000F50000F50000F51A1CF47B86CE07084000003C33337BF5F5FAFFFFFFFF
              FFFFFFFFFFFFFFFFF5F5FA32327900003A0708447D87D01A1DF50000F50000F5
              0000F50000F50000F54E54F15860BD11118916168C9999CDFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF24288B21
              2D7B4F5EA25C67F20000F50000F50000F50000F50000F50000F50000F5191CF4
              7785CA06073500003832327AF5F5FAFFFFFFFFFFFFF5F5FA3232790000350607
              377A85CC1A1CF40000F50000F50000F50000F50000F50000F50000F5636BF463
              69B939399D2E2E97FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFF5E61AA212D7B161E837384DA181CF10000EF0000
              EF0000EF0000EF0000EF0000EF0000EE181CEE7382C406062B00003632327AF4
              F4F9F5F5FA31317800003206062D7683C6191CEE0000EE0000EF0000EF0000EF
              0000EF0000EF0000EF191CF17C89DB4949A55959AD6E6EB7FFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB46
              499F000081060785697ED9161BEC0000E90000E90000E90000E90000E90000E9
              0000E8171BE86E7FBE0606250000342D2D6F2E2E700000300606257181C0181C
              E80000E80000E90000E90000E90000E90000E90000E9191CEC7685D907078503
              03826A6AB6F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4646A40000810607855F7BD8141A
              E60000E30000E30000E30000E30000E30000E30000E1151AE1647BB906061F00
              002300002106061E687CBB161BE20000E10000E30000E30000E30000E30000E3
              0000E3171BE67082D80607850000814646A4F7F7FBFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFF7F7FB4646A40000810507855577D61219E00000DC0000DC0000DC0000DC
              0000DC0000DB0000D91319DA5A77B50506130506135C78B6141ADB0000DA0000
              DB0000DC0000DC0000DC0000DC0000DC151AE0657CD60607850000814646A4F7
              F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4545A50000830506
              874D74D61118DB0000D60000D60000D60000D60000D60000D50000D21219D450
              73B15274B21219D40000D30000D50000D60000D60000D60000D60000D61219DB
              5A79D60607870000834545A5F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFF7F7FB4444A60000850406884B72D51018D50000D00000D0
              0000D00000D00000D00000CE0000CB0D13CC0D13CD0000CB0000CE0000D00000
              D00000D00000D00000D01118D54E73D50506880000854444A6F7F7FBFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4444
              A700008704068A4770D41018D00000CB0000CB0000CB0000CA0000CA0000C700
              00C50000C50000C80000CA0000CB0000CB0000CB0000CB1018D04B71D404068A
              0000874444A7F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4343A800008904068C446FD40F18CA
              0000C40000C40000C40000C30000C20000C10000C10000C30000C40000C40000
              C40000C41018CA4770D404068C0000894343A8F6F6FBFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFF6F6FB4242A900008B04068E416DD30E17C50000BE0000BE0000BE0000BE00
              00BD0000BD0000BE0000BE0000BE0000BE0F18C5436FD304068E00008B4242A9
              F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AB00008D040690
              3E6CD20A12BF0000B80000B80000B80000B80000B80000B80000B80000B80B12
              BF416DD204069000008D4242ABF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFF6F6FB4242AC00008F030692537BD7191FBE0000B20000B20000B200
              00B20000B20000B20000B20000B20A12B93E6CD204069200008F4242ACF6F6FB
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AD000091030694416DD7
              3139CB3A3AC93938C72C2CC32121BE3434C34F4FCA6666CF5251C70606AD0000
              AB0D17B53B6AD20306940000914242ADF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6
              FB4343AF000093030696436ED7373ECB3030C53030C53030C53030C53030C530
              30C43030C43232C55858D28B8BE18C8BDD4040BE0C17AF396BD1030696000093
              4343AFF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFF6F6FB4444B1000095030698456FD83E45CD3737C7
              3737C73737C73737C73737C63737C43737C13737C13737C43737C63737C73B3B
              C77A79DC9C9BE48C90DC507AD60306980000954444B1F6F6FBFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4444B30000
              9703069A4770D9444ACF3E3EC93E3EC93E3EC93E3EC93E3EC93E3EC73E3EC260
              65CC6064CB3E3EC13E3EC63E3EC83E3EC93E3EC93E3EC95554D09B9DE97A96E3
              03069A0000974444B3F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFF7F7FC4545B500009904069C4971DA4C51D14646CB4646CB4646CB
              4646CB4646CB4646CA4646C66B6FD0708FD66D8DD4686DCD4646C54646CA4646
              CB4646CB4646CB4646CB4646CB4F54D26586DF04069C0000994545B5F7F7FCFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646B600009B04069E4A72
              DA5358D44E4ECF4E4ECF4E4ECF4E4ECF4E4ECF4E4ECE4E4ECC7377D67A96DA03
              06670306637693D56F73D24E4ECB4E4ECE4E4ECF4E4ECF4E4ECF4E4ECF4E4ECF
              6064D77D99E504069E00009B4646B6F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7
              F7FC4646B800009D0406A04D73DB595DD85656D35656D35656D35656D35656D3
              5656D35656D37A7DDE849DDF0306720000660000610306647E99DA7477DA5656
              D25656D35656D35656D35656D35656D35656D37276E08EA5EA0406A000009D46
              46B8F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747BA00009F0406A24F74DD6164DC5D5D
              D85D5DD85D5DD85D5DD85D5DD85D5DD85D5DD88285E48CA3E503068300007A36
              369735359200006B030671849DE07A7DE05D5DD85D5DD85D5DD85D5DD85D5DD8
              5D5DD85D5DD8888BE89AADEC0406A200009F4747BAF7F7FCFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6262C500
              00A10407A45076DD676AE26565DF6565DF6565DF6565DF6565DF6565DF6565DF
              898BE994A8EA03069600008A3B3BA6F4F4FAF4F4FA38389D00007903068689A1
              E67F81E76565DF6565DF6565DF6565DF6565DF6565DF6565DF9D9FF1A3B4EE04
              07A40000A16262C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFF1D1DAD0000A31F36BD707BEC6D6DE66D6DE66D6D
              E66D6DE66D6DE66D6DE66D6DE69091EF9AADED0306A300009B3F3FB1F5F5FBFF
              FFFFFFFFFFF5F5FB3C3CA700008F03069C8FA4EA8486ED6D6DE66D6DE66D6DE6
              6D6DE66D6DE66D6DE66D6DE6CBCDFB1B33BA0000A31D1DADFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9999DB00
              00A50000A54360D07B80EF7474ED7474ED7474ED7474ED7474ED9596F4A0B1EF
              0406A80000A44343BAF6F6FCFFFFFFFFFFFFFFFFFFFFFFFFF5F5FB4141B40000
              A00406A794A8ED8989F27474ED7474ED7474ED7474ED7474EDAAADF7697ED900
              00A50000A59999DBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FD80000A70000A7536CD5878BF47B7B
              F47B7BF47B7BF49B9BF9A4B5F10406AA0000A74444BEF6F6FCFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFF6F6FC4343BC0000A70406AA97A9EE8E8FF77B7BF4
              7B7BF47B7BF4ADB0F96B81DB0000A70000A78F8FD8FFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFF8F8FD90000A90000A95E76D99295F88383FC9F9FFDAAB8F20406AC0000A9
              4545C0F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7
              FC4545C00000A90406AC9AABEF9292FC8383FCB0B2FB6D83DC0000A90000A98F
              8FD9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDA0000AB0000AB687E
              DCB4B5FDADBAF30406AE0000AB4646C2F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646C20000AB0406AE9CAEF0
              BBBCFD7084DE0000AB0000AB8F8FDAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFF8F8FDB0000AC0101AC8EA3E51215B10000AC4646C3F7F7FC
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFF7F7FC4646C30000AC0B0EB0AABEEF0808AC0000AC8F8FDBFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDB0D0D
              ACA7A7DF2C2CB24747C5F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747C52020AF
              C2C2EC1B1BAE8F8FDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFF9797DC5B5BCA6464CCF7F7FDFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFF7F7FD6363CB6D6DD19797DDFFFFFFFFFFFFFFFFFFFF
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
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
            ParentFont = False
            OnClick = btnCancelClick
          end
          object btnSummit: TSpeedButton
            Left = 512
            Top = 16
            Width = 160
            Height = 50
            Caption = 'Enter'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = []
            Glyph.Data = {
              361B0000424D361B000000000000360000002800000030000000300000000100
              180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFAF7F7F7FCFCFCFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3
              CAD3CEB2C6BBD9DDDBF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFEFEFEF0F0F0B8CAC051B57F2BB56973B792D1D7D4F7F7F7FF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEECECECABC5B742B374
              07BB5807BF5B12B85E65B489C8D2CDF6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFCFCFCE8E8E8A1C1AF39B16E09B95741CE8276DEA63ACB7B11B75A5AB381C0
              CEC6F3F3F3FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE4E5E498BDA82EAF650AB85547CF84
              9EEEC4B6F7D6ABEECA45CC800EB5564EB078B8CABFF0F0F0FEFEFEFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBDDE0
              DE8AB89D25AC5D09B55249CF858EECBA86F2B786F3B9ACF9D3B7F1D354CD880C
              B35142AD6EABC3B5ECECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFF9F9F9D8DCDA7DB4931FAA570AB25048CE847DEAAE6FEDA5
              6FEFA780F2B592F7C4B4FBDABBF2D65BCE8C0DB05039AA67A1BFADE8E8E8FCFC
              FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8D1D7D373B08B17A7
              4F0BB04E44CE816AE7A157E89259EA946BEEA37BF2B18BF5BE99F9CBB3FCDBB9
              F3D65DCD8C0EAE4E2EA65E98BAA6E3E4E3FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFF5F5F5C7D0CA65AB8012A44A0DAF4D40CE7F55E4913DE27C42E48153E88F
              63EC9D72EFAA80F2B58CF5C095F8C8A5FAD1ADF2CF60CE8E0EAC4C25A45589B4
              99DDDFDEFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF2F2F2BFCCC358A7750DA1440DAC4B38CD
              793BDF7D24DC682BDE6E3BE27B4BE6885AE99567EDA173F0AB7EF2B485F4BA88
              F5BC91F5C19BF0C35FCE8D10AA4A1FA14E7CAF8FD7DBD8F9F9F9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF0F0F0B6
              C6BC4EA36C099D3D0DAB4A30CC7328DB6E0BD75313D85923DC6733E07441E480
              4EE78C5AEA9665EC9F6EEEA674F0AB77F1AE76F0AD7AF0AE88EDB55BCE8B11A8
              49179D4672AC86D0D6D2F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFDFDFDECECECAABFB1419D600699390CAA4826CA6D1DD66303D1
              4801D3490BD75219DA5E27DE6A35E07641E4814CE68A56E8915EEB9862EC9C65
              EC9F64EC9E60EB9B60EB9A70EAA455CE8714A74911993F64A67AC7D0CAF5F5F5
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE7E7E7A1BAA9379A5604
              96340AA7451EC96614D15902CC4300CE4300D14704D54C0ED7551BDB6128DE6B
              34E0753EE37D46E5844DE78A52E88E53E88F53E88F50E78D4AE68847E58557E4
              914BCE8114A5470D963A58A16FBFCBC3F2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FCFCFCE3E4E395B49E2F954E02933007A54215C65F0DCB4F01C73E00C94000CC
              4300CF4500D24705D54D0FD7561BDA5F25DD692EDF7036E1773CE37C40E37F42
              E48141E4803FE37E3AE27A33E0742DDF6F3DE07D3FCD7915A4470992334E9C66
              B6C5BBF0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFADDDFDE88AD92259144018F2C04A5400B
              C45706C64600C23A00C43C00C73E00CA4100CD4300CF4500D24704D54C0CD753
              16D95B1EDB6225DD682ADE6D2EDF702FE0712FDF712DDF6F28DE6C23DC661BDA
              5F13D95922DB6832CC7014A345078F2E41975AAABEAFECECECFDFDFDFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9D7DBD8
              7DA8881D8C3A018C2800A33C03C04E03C03E00BD3500C03800C23A00C53D00C8
              3F00CB4100CD4300CF4500D14703D44B0AD65111D85717DA5C1CDA601FDB631F
              DC631EDB621BDA6017DA5D12D8580BD75305D54D02D34914D55A27CA6812A142
              058B2A37914FA1B8A7E7E7E7FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFDCE1DD78A88318873300882400A13900BB4600BA3700
              B83100BB3400BE3600C03800C33A03C63E08CA4510CE4D19D25520D55C26D862
              2BDA6730DC6B34DD6E36DD7136DE7134DE6F2EDD6B26DB651AD95D0ED65306D4
              4C01D24700D04500CE430DD0501EC8610FA0400388252F8D4595B29CE3E4E3FC
              FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAECBB4228C39
              008521009F3500B63E00B43000B32D00B62F00B83103BC360DC1401CC74E2ECE
              5E3CD36A45D77249D9774CDB7A4BDC7B39DA6E2ED86744DD784EDF7F4DE07F4D
              E07F4CE07F4BDF7D47DE7A40DD7434DA6B24D65E13D15106CC4500C93F07CB48
              15C6580CA03E02832125873A88AA8FDDDFDDFAFAFAFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFA2C6A9168A30009A3000B03700AE2B00AE2900B12B04
              B53116BD412FC65845CE6B52D37757D67C58D87E57D97F58DA8055DB7F3BD66C
              10CF4D06CE4721D45C4BDD7B57DF8457E08457DF8457DF8457DF8458DE8457DE
              8355DC804CD97939D46A21CE560AC64205C5410CC250079F3A02811F1D81317C
              A383D5D8D5F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA3C9AC169E3E
              00AA2F00A82500A92402AD2814B53A38C35A54CD7360D27E63D48162D68262D7
              8262D88462DA865EDA843CD36B0ECA4927D05C5ADA831FD05820D25950DC7D62
              DF8B62DF8A62DF8A62DF8A62DE8962DE8962DD8863DC8862DB875BD98145D26E
              1CC64D04C03C05BE48029E37007E1B177C28709C77CFD4D0F8F8F8FFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFABD6B618A83900A32000A4200BAB2C30BA4E5BCA746C
              D1846ED3876DD4876DD6886DD7896DD88A6ED98C67D9883CD0670DC54431CE60
              B0EBC2EDFAF19DE8B426CF5A1ECE5554DA7E6DE0916DDF916DDF916DDF906DDE
              906DDD8F6DDD8F6DDC8E6EDB8D6EDA8C5CD47E29C55304BB3700BA3F009D3300
              7B18117820629469C7CDC8F5F5F5FFFFFFFFFFFFFFFFFFFFFFFFD6ECDA51BA63
              05A1200CA62946BE5E72CE8579D28C78D48D78D58E78D68F78D79078D89179DA
              936DD88A3BCB630DC13F3ECE67BBEDCAFCFEFDFFFFFFF8FDF9A9EABD32CF611C
              CB5157D97E77E09778E09878DF9778DF9678DE9678DE9578DD9578DC9478DB93
              78DA936AD58734C45908B73500B538009A2E0079150D7318588E5DBEC7BFF2F2
              F2FEFEFEFFFFFFFFFFFFFDFEFDD0ECD455BC660FA52A45BC5B7BD18C83D59482
              D59482D69582D89682D99783DA9973D78D38C75E0CBC3B4BCE6EC7EFD2FDFEFD
              FFFFFFFFFFFFFFFFFFFAFDFBB8ECC73BCE6619C74C57D67C80E09B83E09E82DF
              9D82DF9C82DF9C82DE9B82DD9A82DC9A82DB9983DB9977D68E42C5620DB43500
              AF3100972A007512096D124D8751B5C0B6F0F0F0FEFEFEFFFFFFFFFFFFFFFFFF
              DCF0DF63C27316A82F43BA5980D2918ED89D8CD89D8DDA9E8DDBA077D58E33C1
              560DB7385ACE78D6F2DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFEC8
              F0D348CF6E18C34954D37787E0A08DE1A58CE0A38DE0A38DDFA38DDFA28DDEA1
              8DDDA18DDCA08EDCA086D89955C76F15B13700AA2B00952600720E066A0B3F7E
              41AAB8AAEBEBEBFDFDFDFFFFFFFFFFFFFFFFFFE2F3E572C88019A93240BA5685
              D49499DCA798DCA77AD38E30BD5012B43869CF82DDF4E3FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFED1F1DA57D07817C04553CF748EE0
              A499E2AC98E2AB98E1AB98E1AA98E0A998DFA998DFA898DEA898DDA794DAA365
              C9791DAF3A01A42500912000700B046506377837A3B5A3ECECECFFFFFFFFFFFF
              FFFFFFFFFFFFE9F6EB81CD8D1AA8333BB75288D5977CD28D2AB74815B13678D1
              8CE6F6EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFDBF4E165D18017BC424ECB6E95E0A8A4E4B4A3E3B2A3E3B2A3E2B2
              A3E1B1A3E1B0A3E0B0A3DFAFA3DFAFA0DDAC73CB8321AC39009E1E008D1A006E
              090262023B7D3BBFCDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F9F290D29A22
              AA3A28B0411EAE3A1CAF3989D599EEF9F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE6F7EA75D48C1AB8
              4047C66699DFAAAFE5BBADE5BAADE4B9ADE4B9ADE3B8ADE2B8ADE2B8ADE2B7AE
              E1B7ACE0B580CE8D28AA3B029919008B16006E08166D16A3BEA3FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFF4FAF59FD8A82FB0452AB04199D9A5F2FAF4FFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFEBF8EE85D7981FB64143C15F9ADEAAB9E8C3B9E7C3
              B8E6C2B9E6C2B9E6C1B9E5C0B8E5C0B8E4C0B9E4C0B8E3BF8FD19835AB440395
              16008812167A1DA2BEA2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8
              FCF9CCEBD1C8EACEF7FCF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1FA
              F395DAA322B3403BBC569DDCAAC5EACCC4EACBC4E9CBC4E9CAC4E9CAC4E8CAC4
              E8C9C4E7C9C4E7C9C4E6C8A1D7A742AD4D049112168F23A5C9A9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8FCF9A4DDAF2EB44832B54B9BDAA6
              CDECD2CFEDD4CEECD3CEECD3CEECD3CEEBD2CEEBD2CEEAD2D0EBD3CCE9CF7DC5
              830D91172B9B32BEDDC0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFF9FDFAB2E1BB38B44E2DAF4395D6A0D5EFD9DAF1DED9EFDDDAF0DCDA
              EFDCD9EFDCDAEFDDD3ECD68DCB92269C2E24982B99CE9CF2F8F2FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFEFCC1E6C740B351
              28A93A90D29ADFF2E2E6F4E8E5F4E6E5F4E6E6F4E7DBF0DD86C88B1F97272B9B
              31A8D5AAF7FBF7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFEFEFECDE9D151B65E21A33289CE91E3F3E5F1F9F2F2
              F9F2DFF1E07EC48318941F39A13EB6DCB8FCFDFCFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              D6EDD95FB968209E2D80C787E8F5E9E2F2E374BF7916921D46A64AC3E2C4FCFD
              FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0F0E26BBB711F992968BA6E60
              B56513901A52AC55CFE7D0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFE8F3E97CC0801A9220108D1663B365DAECDBFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEDF6EDA2D2A497
              CC99E2F0E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
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
            ParentFont = False
            OnClick = btnSummitClick
          end
        end
      end
    end
    object tsCalibrateSuckZHeight: TTabSheet
      Caption = 'Calibrate Suck Z Height'
      ImageIndex = 5
      object pcInOutArmSuckZ: TPageControl
        Left = 0
        Top = 0
        Width = 1131
        Height = 616
        ActivePage = tsInArmSuckZ
        Align = alClient
        TabIndex = 0
        TabOrder = 0
        object tsInArmSuckZ: TTabSheet
          Caption = 'In Arm Suck Z'
          object Panel12: TPanel
            Left = 0
            Top = 0
            Width = 1123
            Height = 41
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object cbInarmSuckZAutoEnable: TCheckBox
              Left = 15
              Top = 15
              Width = 153
              Height = 17
              Caption = 'Enable'
              TabOrder = 0
            end
          end
          object Panel13: TPanel
            Left = 0
            Top = 41
            Width = 1123
            Height = 128
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object GroupBox12: TGroupBox
              Left = 0
              Top = 0
              Width = 300
              Height = 128
              Align = alLeft
              Caption = 'InArm Z'
              TabOrder = 0
              object Label90: TLabel
                Left = 32
                Top = 16
                Width = 11
                Height = 20
                Caption = 'A'
              end
              object Label91: TLabel
                Left = 32
                Top = 63
                Width = 11
                Height = 20
                Caption = 'B'
              end
              object Label92: TLabel
                Left = 104
                Top = 16
                Width = 11
                Height = 20
                Caption = 'C'
              end
              object Label93: TLabel
                Left = 104
                Top = 63
                Width = 12
                Height = 20
                Caption = 'D'
              end
              object Label94: TLabel
                Left = 175
                Top = 16
                Width = 11
                Height = 20
                Caption = 'E'
              end
              object Label95: TLabel
                Left = 175
                Top = 63
                Width = 10
                Height = 20
                Caption = 'F'
              end
              object Label96: TLabel
                Left = 247
                Top = 16
                Width = 13
                Height = 20
                Caption = 'G'
              end
              object Label97: TLabel
                Left = 247
                Top = 63
                Width = 12
                Height = 20
                Caption = 'H'
              end
              object setEditZ1A: TEdit
                Left = 11
                Top = 32
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
              object setEditZ1B: TEdit
                Left = 11
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 4
              end
              object setEditZ1C: TEdit
                Left = 84
                Top = 32
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 1
              end
              object setEditZ1D: TEdit
                Left = 84
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 5
              end
              object setEditZ1E: TEdit
                Left = 156
                Top = 32
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 2
              end
              object setEditZ1F: TEdit
                Left = 156
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 6
              end
              object setEditZ1G: TEdit
                Left = 229
                Top = 32
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 3
              end
              object setEditZ1H: TEdit
                Left = 229
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 7
              end
            end
            object gbTestAreaInarm: TGroupBox
              Left = 300
              Top = 0
              Width = 116
              Height = 128
              Align = alLeft
              Caption = 'Test Area'
              TabOrder = 1
              object Label64: TLabel
                Left = 8
                Top = 30
                Width = 11
                Height = 20
                Caption = 'X'
              end
              object Label65: TLabel
                Left = 8
                Top = 62
                Width = 11
                Height = 20
                Caption = 'Y'
              end
              object edInarmTestAreaX: TEdit
                Left = 29
                Top = 26
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
              object edInarmTestAreaY: TEdit
                Left = 29
                Top = 58
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 1
              end
            end
            object gbInarmSearchStartZ: TGroupBox
              Left = 416
              Top = 0
              Width = 116
              Height = 128
              Align = alLeft
              Caption = 'Search Start Z'
              TabOrder = 2
              object Label5: TLabel
                Left = 8
                Top = 30
                Width = 10
                Height = 20
                Caption = 'Z'
              end
              object edInarmSearchStartZ: TEdit
                Left = 29
                Top = 26
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
            end
          end
        end
        object tsOutArmSuckZ: TTabSheet
          Caption = 'Out Arm Suck Z'
          ImageIndex = 1
          object Panel10: TPanel
            Left = 0
            Top = 41
            Width = 1123
            Height = 128
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object GroupBox13: TGroupBox
              Left = 0
              Top = 0
              Width = 300
              Height = 128
              Align = alLeft
              Caption = 'OutArm Z'
              TabOrder = 0
              object Label19: TLabel
                Left = 32
                Top = 16
                Width = 11
                Height = 20
                Caption = 'A'
              end
              object Label23: TLabel
                Left = 32
                Top = 63
                Width = 11
                Height = 20
                Caption = 'B'
              end
              object Label27: TLabel
                Left = 104
                Top = 16
                Width = 11
                Height = 20
                Caption = 'C'
              end
              object Label28: TLabel
                Left = 104
                Top = 63
                Width = 12
                Height = 20
                Caption = 'D'
              end
              object Label30: TLabel
                Left = 175
                Top = 16
                Width = 11
                Height = 20
                Caption = 'E'
              end
              object Label31: TLabel
                Left = 175
                Top = 63
                Width = 10
                Height = 20
                Caption = 'F'
              end
              object Label42: TLabel
                Left = 247
                Top = 16
                Width = 13
                Height = 20
                Caption = 'G'
              end
              object Label43: TLabel
                Left = 247
                Top = 63
                Width = 12
                Height = 20
                Caption = 'H'
              end
              object setEditZ2G: TEdit
                Left = 229
                Top = 33
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 3
              end
              object setEditZ2E: TEdit
                Left = 156
                Top = 33
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 2
              end
              object setEditZ2C: TEdit
                Left = 84
                Top = 33
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 1
              end
              object setEditZ2A: TEdit
                Left = 11
                Top = 33
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
              object setEditZ2H: TEdit
                Left = 229
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 7
              end
              object setEditZ2F: TEdit
                Left = 156
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 6
              end
              object setEditZ2D: TEdit
                Left = 84
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 5
              end
              object setEditZ2B: TEdit
                Left = 11
                Top = 80
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 4
              end
            end
            object gbTestAreaOutarm: TGroupBox
              Left = 300
              Top = 0
              Width = 116
              Height = 128
              Align = alLeft
              Caption = 'Test Area'
              TabOrder = 1
              object Label3: TLabel
                Left = 8
                Top = 30
                Width = 11
                Height = 20
                Caption = 'X'
              end
              object Label4: TLabel
                Left = 8
                Top = 62
                Width = 11
                Height = 20
                Caption = 'Y'
              end
              object edOutarmTestAreaX: TEdit
                Left = 29
                Top = 26
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
              object edOutarmTestAreaY: TEdit
                Left = 29
                Top = 58
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 1
              end
            end
            object gbOutarmSearchStartZ: TGroupBox
              Left = 416
              Top = 0
              Width = 116
              Height = 128
              Align = alLeft
              Caption = 'Search Start Z'
              TabOrder = 2
              object Label10: TLabel
                Left = 8
                Top = 30
                Width = 10
                Height = 20
                Caption = 'Z'
              end
              object edOutarmSearchStartZ: TEdit
                Left = 29
                Top = 26
                Width = 60
                Height = 28
                ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
                TabOrder = 0
              end
            end
          end
          object Panel11: TPanel
            Left = 0
            Top = 0
            Width = 1123
            Height = 41
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 1
            object cbOutarmSuckZAutoEnable: TCheckBox
              Left = 15
              Top = 15
              Width = 153
              Height = 17
              Caption = 'Enable'
              TabOrder = 0
            end
          end
        end
      end
      object plBottomArmAutoSuckZ: TPanel
        Left = 0
        Top = 616
        Width = 1131
        Height = 80
        Align = alBottom
        Color = 12761254
        TabOrder = 1
        object SpeedButton1: TSpeedButton
          Left = 348
          Top = 16
          Width = 160
          Height = 50
          Caption = 'Abort'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            361B0000424D361B000000000000360000002800000030000000300000000100
            180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF878C
            BB252B875E5FA8F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB6767BB
            5555CB9898D2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFF8F8FC82634791C25770E13703E3E90F6F6FAFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFF6F6FA3F3F922B2BA16868E22B2BAA8F8FC8FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810A0E
            7F4E59A01216740000613A3A88F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA3A3A89000065111282
            8186E40B0B8B0000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFF8F8FC80000810000815860BD636AF27F89D8070868000055383883
            F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFF6F6FA37378200005807086D7F89D9656CF35860BD0000810000818F8FC8FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810000815860BD4E54
            F10000F51B1DF57E88D507085B00004B35357EF6F6FAFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA35357D00004B0708607E88D71B1DF5
            0000F54E54F15860BD0000810000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8E
            8EC7000081000081585FBD4D54F10000F50000F50000F51A1DF57E87D307084E
            00004233337BF5F5FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5FA3333
            7A0000420708527E88D41B1DF50000F50000F50000F54E54F15860BD00008100
            00818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF9898CC1B257C0B0F7F555EBD4B53F10000F50000
            F50000F50000F50000F51A1CF47B86CE07084000003C33337BF5F5FAFFFFFFFF
            FFFFFFFFFFFFFFFFF5F5FA32327900003A0708447D87D01A1DF50000F50000F5
            0000F50000F50000F54E54F15860BD11118916168C9999CDFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF24288B21
            2D7B4F5EA25C67F20000F50000F50000F50000F50000F50000F50000F5191CF4
            7785CA06073500003832327AF5F5FAFFFFFFFFFFFFF5F5FA3232790000350607
            377A85CC1A1CF40000F50000F50000F50000F50000F50000F50000F5636BF463
            69B939399D2E2E97FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF5E61AA212D7B161E837384DA181CF10000EF0000
            EF0000EF0000EF0000EF0000EF0000EE181CEE7382C406062B00003632327AF4
            F4F9F5F5FA31317800003206062D7683C6191CEE0000EE0000EF0000EF0000EF
            0000EF0000EF0000EF191CF17C89DB4949A55959AD6E6EB7FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB46
            499F000081060785697ED9161BEC0000E90000E90000E90000E90000E90000E9
            0000E8171BE86E7FBE0606250000342D2D6F2E2E700000300606257181C0181C
            E80000E80000E90000E90000E90000E90000E90000E9191CEC7685D907078503
            03826A6AB6F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4646A40000810607855F7BD8141A
            E60000E30000E30000E30000E30000E30000E30000E1151AE1647BB906061F00
            002300002106061E687CBB161BE20000E10000E30000E30000E30000E30000E3
            0000E3171BE67082D80607850000814646A4F7F7FBFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFF7F7FB4646A40000810507855577D61219E00000DC0000DC0000DC0000DC
            0000DC0000DB0000D91319DA5A77B50506130506135C78B6141ADB0000DA0000
            DB0000DC0000DC0000DC0000DC0000DC151AE0657CD60607850000814646A4F7
            F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4545A50000830506
            874D74D61118DB0000D60000D60000D60000D60000D60000D50000D21219D450
            73B15274B21219D40000D30000D50000D60000D60000D60000D60000D61219DB
            5A79D60607870000834545A5F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFF7F7FB4444A60000850406884B72D51018D50000D00000D0
            0000D00000D00000D00000CE0000CB0D13CC0D13CD0000CB0000CE0000D00000
            D00000D00000D00000D01118D54E73D50506880000854444A6F7F7FBFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4444
            A700008704068A4770D41018D00000CB0000CB0000CB0000CA0000CA0000C700
            00C50000C50000C80000CA0000CB0000CB0000CB0000CB1018D04B71D404068A
            0000874444A7F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4343A800008904068C446FD40F18CA
            0000C40000C40000C40000C30000C20000C10000C10000C30000C40000C40000
            C40000C41018CA4770D404068C0000894343A8F6F6FBFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFF6F6FB4242A900008B04068E416DD30E17C50000BE0000BE0000BE0000BE00
            00BD0000BD0000BE0000BE0000BE0000BE0F18C5436FD304068E00008B4242A9
            F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AB00008D040690
            3E6CD20A12BF0000B80000B80000B80000B80000B80000B80000B80000B80B12
            BF416DD204069000008D4242ABF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFF6F6FB4242AC00008F030692537BD7191FBE0000B20000B20000B200
            00B20000B20000B20000B20000B20A12B93E6CD204069200008F4242ACF6F6FB
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AD000091030694416DD7
            3139CB3A3AC93938C72C2CC32121BE3434C34F4FCA6666CF5251C70606AD0000
            AB0D17B53B6AD20306940000914242ADF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6
            FB4343AF000093030696436ED7373ECB3030C53030C53030C53030C53030C530
            30C43030C43232C55858D28B8BE18C8BDD4040BE0C17AF396BD1030696000093
            4343AFF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFF6F6FB4444B1000095030698456FD83E45CD3737C7
            3737C73737C73737C73737C63737C43737C13737C13737C43737C63737C73B3B
            C77A79DC9C9BE48C90DC507AD60306980000954444B1F6F6FBFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4444B30000
            9703069A4770D9444ACF3E3EC93E3EC93E3EC93E3EC93E3EC93E3EC73E3EC260
            65CC6064CB3E3EC13E3EC63E3EC83E3EC93E3EC93E3EC95554D09B9DE97A96E3
            03069A0000974444B3F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFF7F7FC4545B500009904069C4971DA4C51D14646CB4646CB4646CB
            4646CB4646CB4646CA4646C66B6FD0708FD66D8DD4686DCD4646C54646CA4646
            CB4646CB4646CB4646CB4646CB4F54D26586DF04069C0000994545B5F7F7FCFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646B600009B04069E4A72
            DA5358D44E4ECF4E4ECF4E4ECF4E4ECF4E4ECF4E4ECE4E4ECC7377D67A96DA03
            06670306637693D56F73D24E4ECB4E4ECE4E4ECF4E4ECF4E4ECF4E4ECF4E4ECF
            6064D77D99E504069E00009B4646B6F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7
            F7FC4646B800009D0406A04D73DB595DD85656D35656D35656D35656D35656D3
            5656D35656D37A7DDE849DDF0306720000660000610306647E99DA7477DA5656
            D25656D35656D35656D35656D35656D35656D37276E08EA5EA0406A000009D46
            46B8F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747BA00009F0406A24F74DD6164DC5D5D
            D85D5DD85D5DD85D5DD85D5DD85D5DD85D5DD88285E48CA3E503068300007A36
            369735359200006B030671849DE07A7DE05D5DD85D5DD85D5DD85D5DD85D5DD8
            5D5DD85D5DD8888BE89AADEC0406A200009F4747BAF7F7FCFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6262C500
            00A10407A45076DD676AE26565DF6565DF6565DF6565DF6565DF6565DF6565DF
            898BE994A8EA03069600008A3B3BA6F4F4FAF4F4FA38389D00007903068689A1
            E67F81E76565DF6565DF6565DF6565DF6565DF6565DF6565DF9D9FF1A3B4EE04
            07A40000A16262C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFF1D1DAD0000A31F36BD707BEC6D6DE66D6DE66D6D
            E66D6DE66D6DE66D6DE66D6DE69091EF9AADED0306A300009B3F3FB1F5F5FBFF
            FFFFFFFFFFF5F5FB3C3CA700008F03069C8FA4EA8486ED6D6DE66D6DE66D6DE6
            6D6DE66D6DE66D6DE66D6DE6CBCDFB1B33BA0000A31D1DADFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9999DB00
            00A50000A54360D07B80EF7474ED7474ED7474ED7474ED7474ED9596F4A0B1EF
            0406A80000A44343BAF6F6FCFFFFFFFFFFFFFFFFFFFFFFFFF5F5FB4141B40000
            A00406A794A8ED8989F27474ED7474ED7474ED7474ED7474EDAAADF7697ED900
            00A50000A59999DBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FD80000A70000A7536CD5878BF47B7B
            F47B7BF47B7BF49B9BF9A4B5F10406AA0000A74444BEF6F6FCFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFF6F6FC4343BC0000A70406AA97A9EE8E8FF77B7BF4
            7B7BF47B7BF4ADB0F96B81DB0000A70000A78F8FD8FFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFF8F8FD90000A90000A95E76D99295F88383FC9F9FFDAAB8F20406AC0000A9
            4545C0F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7
            FC4545C00000A90406AC9AABEF9292FC8383FCB0B2FB6D83DC0000A90000A98F
            8FD9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDA0000AB0000AB687E
            DCB4B5FDADBAF30406AE0000AB4646C2F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646C20000AB0406AE9CAEF0
            BBBCFD7084DE0000AB0000AB8F8FDAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFF8F8FDB0000AC0101AC8EA3E51215B10000AC4646C3F7F7FC
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFF7F7FC4646C30000AC0B0EB0AABEEF0808AC0000AC8F8FDBFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDB0D0D
            ACA7A7DF2C2CB24747C5F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747C52020AF
            C2C2EC1B1BAE8F8FDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFF9797DC5B5BCA6464CCF7F7FDFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFF7F7FD6363CB6D6DD19797DDFFFFFFFFFFFFFFFFFFFF
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
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
          ParentFont = False
          OnClick = btnCancelClick
        end
        object btnAutoCalSuckZSave: TSpeedButton
          Left = 560
          Top = 16
          Width = 160
          Height = 50
          Caption = 'Save'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Glyph.Data = {
            361B0000424D361B000000000000360000002800000030000000300000000100
            180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFAF7F7F7FCFCFCFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3
            CAD3CEB2C6BBD9DDDBF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFEFEFEF0F0F0B8CAC051B57F2BB56973B792D1D7D4F7F7F7FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEECECECABC5B742B374
            07BB5807BF5B12B85E65B489C8D2CDF6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFCFCFCE8E8E8A1C1AF39B16E09B95741CE8276DEA63ACB7B11B75A5AB381C0
            CEC6F3F3F3FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE4E5E498BDA82EAF650AB85547CF84
            9EEEC4B6F7D6ABEECA45CC800EB5564EB078B8CABFF0F0F0FEFEFEFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBDDE0
            DE8AB89D25AC5D09B55249CF858EECBA86F2B786F3B9ACF9D3B7F1D354CD880C
            B35142AD6EABC3B5ECECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFF9F9F9D8DCDA7DB4931FAA570AB25048CE847DEAAE6FEDA5
            6FEFA780F2B592F7C4B4FBDABBF2D65BCE8C0DB05039AA67A1BFADE8E8E8FCFC
            FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8D1D7D373B08B17A7
            4F0BB04E44CE816AE7A157E89259EA946BEEA37BF2B18BF5BE99F9CBB3FCDBB9
            F3D65DCD8C0EAE4E2EA65E98BAA6E3E4E3FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFF5F5F5C7D0CA65AB8012A44A0DAF4D40CE7F55E4913DE27C42E48153E88F
            63EC9D72EFAA80F2B58CF5C095F8C8A5FAD1ADF2CF60CE8E0EAC4C25A45589B4
            99DDDFDEFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF2F2F2BFCCC358A7750DA1440DAC4B38CD
            793BDF7D24DC682BDE6E3BE27B4BE6885AE99567EDA173F0AB7EF2B485F4BA88
            F5BC91F5C19BF0C35FCE8D10AA4A1FA14E7CAF8FD7DBD8F9F9F9FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF0F0F0B6
            C6BC4EA36C099D3D0DAB4A30CC7328DB6E0BD75313D85923DC6733E07441E480
            4EE78C5AEA9665EC9F6EEEA674F0AB77F1AE76F0AD7AF0AE88EDB55BCE8B11A8
            49179D4672AC86D0D6D2F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFDFDFDECECECAABFB1419D600699390CAA4826CA6D1DD66303D1
            4801D3490BD75219DA5E27DE6A35E07641E4814CE68A56E8915EEB9862EC9C65
            EC9F64EC9E60EB9B60EB9A70EAA455CE8714A74911993F64A67AC7D0CAF5F5F5
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE7E7E7A1BAA9379A5604
            96340AA7451EC96614D15902CC4300CE4300D14704D54C0ED7551BDB6128DE6B
            34E0753EE37D46E5844DE78A52E88E53E88F53E88F50E78D4AE68847E58557E4
            914BCE8114A5470D963A58A16FBFCBC3F2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FCFCFCE3E4E395B49E2F954E02933007A54215C65F0DCB4F01C73E00C94000CC
            4300CF4500D24705D54D0FD7561BDA5F25DD692EDF7036E1773CE37C40E37F42
            E48141E4803FE37E3AE27A33E0742DDF6F3DE07D3FCD7915A4470992334E9C66
            B6C5BBF0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFADDDFDE88AD92259144018F2C04A5400B
            C45706C64600C23A00C43C00C73E00CA4100CD4300CF4500D24704D54C0CD753
            16D95B1EDB6225DD682ADE6D2EDF702FE0712FDF712DDF6F28DE6C23DC661BDA
            5F13D95922DB6832CC7014A345078F2E41975AAABEAFECECECFDFDFDFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9D7DBD8
            7DA8881D8C3A018C2800A33C03C04E03C03E00BD3500C03800C23A00C53D00C8
            3F00CB4100CD4300CF4500D14703D44B0AD65111D85717DA5C1CDA601FDB631F
            DC631EDB621BDA6017DA5D12D8580BD75305D54D02D34914D55A27CA6812A142
            058B2A37914FA1B8A7E7E7E7FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFDCE1DD78A88318873300882400A13900BB4600BA3700
            B83100BB3400BE3600C03800C33A03C63E08CA4510CE4D19D25520D55C26D862
            2BDA6730DC6B34DD6E36DD7136DE7134DE6F2EDD6B26DB651AD95D0ED65306D4
            4C01D24700D04500CE430DD0501EC8610FA0400388252F8D4595B29CE3E4E3FC
            FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAECBB4228C39
            008521009F3500B63E00B43000B32D00B62F00B83103BC360DC1401CC74E2ECE
            5E3CD36A45D77249D9774CDB7A4BDC7B39DA6E2ED86744DD784EDF7F4DE07F4D
            E07F4CE07F4BDF7D47DE7A40DD7434DA6B24D65E13D15106CC4500C93F07CB48
            15C6580CA03E02832125873A88AA8FDDDFDDFAFAFAFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFA2C6A9168A30009A3000B03700AE2B00AE2900B12B04
            B53116BD412FC65845CE6B52D37757D67C58D87E57D97F58DA8055DB7F3BD66C
            10CF4D06CE4721D45C4BDD7B57DF8457E08457DF8457DF8457DF8458DE8457DE
            8355DC804CD97939D46A21CE560AC64205C5410CC250079F3A02811F1D81317C
            A383D5D8D5F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA3C9AC169E3E
            00AA2F00A82500A92402AD2814B53A38C35A54CD7360D27E63D48162D68262D7
            8262D88462DA865EDA843CD36B0ECA4927D05C5ADA831FD05820D25950DC7D62
            DF8B62DF8A62DF8A62DF8A62DE8962DE8962DD8863DC8862DB875BD98145D26E
            1CC64D04C03C05BE48029E37007E1B177C28709C77CFD4D0F8F8F8FFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFABD6B618A83900A32000A4200BAB2C30BA4E5BCA746C
            D1846ED3876DD4876DD6886DD7896DD88A6ED98C67D9883CD0670DC54431CE60
            B0EBC2EDFAF19DE8B426CF5A1ECE5554DA7E6DE0916DDF916DDF916DDF906DDE
            906DDD8F6DDD8F6DDC8E6EDB8D6EDA8C5CD47E29C55304BB3700BA3F009D3300
            7B18117820629469C7CDC8F5F5F5FFFFFFFFFFFFFFFFFFFFFFFFD6ECDA51BA63
            05A1200CA62946BE5E72CE8579D28C78D48D78D58E78D68F78D79078D89179DA
            936DD88A3BCB630DC13F3ECE67BBEDCAFCFEFDFFFFFFF8FDF9A9EABD32CF611C
            CB5157D97E77E09778E09878DF9778DF9678DE9678DE9578DD9578DC9478DB93
            78DA936AD58734C45908B73500B538009A2E0079150D7318588E5DBEC7BFF2F2
            F2FEFEFEFFFFFFFFFFFFFDFEFDD0ECD455BC660FA52A45BC5B7BD18C83D59482
            D59482D69582D89682D99783DA9973D78D38C75E0CBC3B4BCE6EC7EFD2FDFEFD
            FFFFFFFFFFFFFFFFFFFAFDFBB8ECC73BCE6619C74C57D67C80E09B83E09E82DF
            9D82DF9C82DF9C82DE9B82DD9A82DC9A82DB9983DB9977D68E42C5620DB43500
            AF3100972A007512096D124D8751B5C0B6F0F0F0FEFEFEFFFFFFFFFFFFFFFFFF
            DCF0DF63C27316A82F43BA5980D2918ED89D8CD89D8DDA9E8DDBA077D58E33C1
            560DB7385ACE78D6F2DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFEC8
            F0D348CF6E18C34954D37787E0A08DE1A58CE0A38DE0A38DDFA38DDFA28DDEA1
            8DDDA18DDCA08EDCA086D89955C76F15B13700AA2B00952600720E066A0B3F7E
            41AAB8AAEBEBEBFDFDFDFFFFFFFFFFFFFFFFFFE2F3E572C88019A93240BA5685
            D49499DCA798DCA77AD38E30BD5012B43869CF82DDF4E3FFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFED1F1DA57D07817C04553CF748EE0
            A499E2AC98E2AB98E1AB98E1AA98E0A998DFA998DFA898DEA898DDA794DAA365
            C9791DAF3A01A42500912000700B046506377837A3B5A3ECECECFFFFFFFFFFFF
            FFFFFFFFFFFFE9F6EB81CD8D1AA8333BB75288D5977CD28D2AB74815B13678D1
            8CE6F6EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFDBF4E165D18017BC424ECB6E95E0A8A4E4B4A3E3B2A3E3B2A3E2B2
            A3E1B1A3E1B0A3E0B0A3DFAFA3DFAFA0DDAC73CB8321AC39009E1E008D1A006E
            090262023B7D3BBFCDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F9F290D29A22
            AA3A28B0411EAE3A1CAF3989D599EEF9F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE6F7EA75D48C1AB8
            4047C66699DFAAAFE5BBADE5BAADE4B9ADE4B9ADE3B8ADE2B8ADE2B8ADE2B7AE
            E1B7ACE0B580CE8D28AA3B029919008B16006E08166D16A3BEA3FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFF4FAF59FD8A82FB0452AB04199D9A5F2FAF4FFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFEBF8EE85D7981FB64143C15F9ADEAAB9E8C3B9E7C3
            B8E6C2B9E6C2B9E6C1B9E5C0B8E5C0B8E4C0B9E4C0B8E3BF8FD19835AB440395
            16008812167A1DA2BEA2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8
            FCF9CCEBD1C8EACEF7FCF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1FA
            F395DAA322B3403BBC569DDCAAC5EACCC4EACBC4E9CBC4E9CAC4E9CAC4E8CAC4
            E8C9C4E7C9C4E7C9C4E6C8A1D7A742AD4D049112168F23A5C9A9FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8FCF9A4DDAF2EB44832B54B9BDAA6
            CDECD2CFEDD4CEECD3CEECD3CEECD3CEEBD2CEEBD2CEEAD2D0EBD3CCE9CF7DC5
            830D91172B9B32BEDDC0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFF9FDFAB2E1BB38B44E2DAF4395D6A0D5EFD9DAF1DED9EFDDDAF0DCDA
            EFDCD9EFDCDAEFDDD3ECD68DCB92269C2E24982B99CE9CF2F8F2FFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFEFCC1E6C740B351
            28A93A90D29ADFF2E2E6F4E8E5F4E6E5F4E6E6F4E7DBF0DD86C88B1F97272B9B
            31A8D5AAF7FBF7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFEFEFECDE9D151B65E21A33289CE91E3F3E5F1F9F2F2
            F9F2DFF1E07EC48318941F39A13EB6DCB8FCFDFCFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            D6EDD95FB968209E2D80C787E8F5E9E2F2E374BF7916921D46A64AC3E2C4FCFD
            FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0F0E26BBB711F992968BA6E60
            B56513901A52AC55CFE7D0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFE8F3E97CC0801A9220108D1663B365DAECDBFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEDF6EDA2D2A497
            CC99E2F0E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
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
          ParentFont = False
          OnClick = btnAutoCalSuckZSaveClick
        end
      end
    end
  end
  object tm_PI: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tm_PITimer
    Left = 804
    Top = 4
  end
  object tm_IPSCControl: TTimer
    Enabled = False
    Interval = 500
    OnTimer = tm_IPSCControlTimer
    Left = 836
    Top = 4
  end
end
