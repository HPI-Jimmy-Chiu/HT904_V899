object fLotInfo: TfLotInfo
  Left = 224
  Top = 107
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biHelp]
  BorderStyle = bsToolWindow
  Caption = 'Lot Info'
  ClientHeight = 561
  ClientWidth = 658
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
  object SpeedButton1: TSpeedButton
    Left = 23
    Top = 171
    Width = 323
    Height = 44
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -29
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
  end
  object pgLotinfo: TPageControl
    Left = 0
    Top = 0
    Width = 658
    Height = 561
    ActivePage = tsLotID
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    OnChange = pgLotinfoChange
    object tsDeviceInfo: TTabSheet
      Caption = 'Device Info'
      ImageIndex = 1
      object Panel29: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'FTP'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel30: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label153: TLabel
          Left = 0
          Top = 16
          Width = 65
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Device Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label5: TLabel
          Left = 0
          Top = 40
          Width = 65
          Height = 13
          Alignment = taRightJustify
          AutoSize = False
          Caption = 'Temperature'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object labLevelMode: TLabel
          Left = 28
          Top = 146
          Width = 36
          Height = 15
          AutoSize = False
          Caption = 'User'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object lbShowDevName: TLabel
          Left = 248
          Top = 16
          Width = 105
          Height = 13
          AutoSize = False
          Caption = 'Show Device Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object edDeviceName: TEdit
          Left = 70
          Top = 12
          Width = 171
          Height = 21
          Color = 14670284
          TabOrder = 1
          OnKeyDown = edDeviceNameKeyDown
          OnKeyPress = edDeviceNameKeyPress
          OnKeyUp = edDeviceNameKeyUp
          OnMouseDown = edDeviceNameMouseDown
        end
        object btDownload: TButton
          Left = 48
          Top = 72
          Width = 169
          Height = 25
          Caption = 'Download from Server'
          TabOrder = 3
          OnClick = btDownloadClick
        end
        object btUpload: TButton
          Left = 48
          Top = 104
          Width = 169
          Height = 25
          Caption = 'Update to Server'
          TabOrder = 4
          OnClick = btUploadClick
        end
        object edTemp: TEdit
          Left = 70
          Top = 36
          Width = 171
          Height = 21
          Color = 14670284
          TabOrder = 2
          OnKeyDown = edTempKeyDown
          OnKeyPress = edTempKeyPress
          OnKeyUp = edTempKeyUp
        end
        object cbbDeviceName: TComboBox
          Left = 6
          Top = 276
          Width = 191
          Height = 23
          Color = 14670284
          DropDownCount = 20
          Font.Charset = ANSI_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ItemHeight = 0
          ParentFont = False
          TabOrder = 7
          Visible = False
          OnChange = cbbDeviceNameChange
        end
        object FileListBox1: TFileListBox
          Left = 8
          Top = 172
          Width = 145
          Height = 97
          Color = 14670284
          ItemHeight = 13
          Mask = '*.zip'
          TabOrder = 6
          Visible = False
        end
        object FileListBox2: TFileListBox
          Left = 379
          Top = 0
          Width = 271
          Height = 508
          Align = alRight
          Color = 14670284
          ItemHeight = 13
          Mask = '*.Data'
          TabOrder = 0
          Visible = False
        end
        object coLevelMode: TComboBox
          Left = 70
          Top = 143
          Width = 145
          Height = 21
          Color = 14670284
          ItemHeight = 13
          TabOrder = 5
          Text = 'Normal'
          OnChange = coLevelModeChange
          Items.Strings = (
            'Normal'
            'Engineer')
        end
      end
    end
    object tsLotID: TTabSheet
      Caption = 'Lot'
      ImageIndex = 2
      object Panel27: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Lot Info'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel8: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object palSecsGem: TPanel
          Left = 0
          Top = 0
          Width = 650
          Height = 369
          Align = alTop
          BevelInner = bvLowered
          Color = 12761254
          TabOrder = 0
          OnMouseDown = palSecsGemMouseDown
          object labLotID: TLabel
            Left = 15
            Top = 12
            Width = 38
            Height = 13
            Caption = 'Lot ID : '
            OnMouseDown = labLotIDMouseDown
          end
          object sbSECSLotStart: TSpeedButton
            Left = 27
            Top = 254
            Width = 138
            Height = 25
            GroupIndex = 1
            Caption = 'Lot Start'
            OnClick = sbSECSLotStartClick
          end
          object sbSECSLotEnd: TSpeedButton
            Left = 27
            Top = 290
            Width = 138
            Height = 25
            GroupIndex = 1
            Caption = 'Lot End'
            OnClick = sbSECSLotEndClick
          end
          object spSECSLotCheck: TSpeedButton
            Left = 196
            Top = 6
            Width = 65
            Height = 25
            Caption = 'Lot Check'
            OnClick = spSECSLotCheckClick
          end
          object lblPage: TLabel
            Left = 195
            Top = 7
            Width = 6
            Height = 24
            Caption = '-'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object btnSaveData: TSpeedButton
            Left = 176
            Top = 255
            Width = 81
            Height = 25
            Caption = 'Save Data'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            Visible = False
            OnClick = btnSaveDataClick
          end
          object labCusLotID: TLabel
            Left = 15
            Top = 115
            Width = 52
            Height = 13
            Caption = 'Cus. lot ID:'
            Visible = False
          end
          object labCusDevGrp: TLabel
            Left = 15
            Top = 140
            Width = 74
            Height = 13
            Caption = 'Cus. device grp'
            Visible = False
          end
          object labDeviceName: TLabel
            Left = 15
            Top = 164
            Width = 34
            Height = 13
            Caption = 'Device'
            Visible = False
          end
          object labConfigL04: TLabel
            Left = 11
            Top = 204
            Width = 147
            Height = 13
            Caption = '[L04] Temperature range(2..10)'
            Visible = False
          end
          object sbUploadPAT: TSpeedButton
            Left = 176
            Top = 286
            Width = 81
            Height = 25
            GroupIndex = 1
            Down = True
            Caption = 'Upload'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            Visible = False
            OnClick = sbUploadPATClick
          end
          object btnFTPDownLoadbyDeviceID: TSpeedButton
            Left = 196
            Top = 160
            Width = 65
            Height = 25
            Caption = 'DownLoad'
            Visible = False
            OnClick = btnFTPDownLoadbyDeviceIDClick
          end
          object labCusStep: TLabel
            Left = 16
            Top = 188
            Width = 44
            Height = 13
            Caption = 'Cus. step'
            Visible = False
          end
          object edtSysLotID: TEdit
            Left = 93
            Top = 8
            Width = 100
            Height = 21
            Hint = 'Can not use ([\/:*?"<>|])'
            Color = 14670284
            TabOrder = 1
            OnKeyDown = edtSysLotIDKeyDown
            OnKeyPress = edtSysLotIDKeyPress
            OnKeyUp = edtSysLotIDKeyUp
            OnMouseUp = edtSysLotIDMouseUp
          end
          object Panel28: TPanel
            Left = 12
            Top = 32
            Width = 185
            Height = 80
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 3
            object lblOPID: TLabel
              Left = 3
              Top = 8
              Width = 64
              Height = 13
              Caption = 'Operator ID : '
            end
            object lbLotRunMode: TLabel
              Left = 3
              Top = 60
              Width = 59
              Height = 13
              Caption = 'Run Mode : '
            end
            object lbProcess: TLabel
              Left = 3
              Top = 34
              Width = 47
              Height = 13
              Caption = 'Process : '
              Visible = False
            end
            object lblTestTimes: TLabel
              Left = 183
              Top = 28
              Width = 6
              Height = 24
              Caption = '-'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -19
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object edtSysOperatorID: TEdit
              Left = 81
              Top = 4
              Width = 100
              Height = 21
              Color = 14670284
              TabOrder = 0
              OnKeyDown = edtSysOperatorIDKeyDown
              OnKeyPress = edtSysOperatorIDKeyPress
              OnKeyUp = edtSysOperatorIDKeyUp
              OnMouseUp = edtSysOperatorIDMouseUp
            end
            object cbRunMode: TComboBox
              Left = 81
              Top = 57
              Width = 100
              Height = 21
              Color = 14670284
              ItemHeight = 13
              TabOrder = 3
              OnDropDown = cbRunModeDropDown
              OnKeyDown = cbRunModeKeyDown
              OnKeyPress = cbRunModeKeyPress
              OnKeyUp = cbRunModeKeyUp
              Items.Strings = (
                'Normal'
                'RT'
                'EQC')
            end
            object cbProcess: TComboBox
              Left = 81
              Top = 30
              Width = 100
              Height = 21
              Color = 14670284
              ItemHeight = 13
              TabOrder = 1
              Text = 'FT1'
              Visible = False
              Items.Strings = (
                'FT1'
                'FT2'
                'FT3'
                'FT4'
                'FT5'
                'FT6'
                'FT7'
                'FT8'
                'FT9'
                'FT10'
                'setup'
                'Repair')
            end
            object cbTestTimes: TComboBox
              Left = 191
              Top = 30
              Width = 58
              Height = 21
              Color = clYellow
              ItemHeight = 13
              TabOrder = 2
              Text = 'RP15'
              Visible = False
              OnKeyPress = cbRunModeKeyPress
              Items.Strings = (
                'RP0'
                'RP1'
                'RP2'
                'RP3'
                'RP4'
                'RP5'
                'RP6'
                'RP7'
                'RP8'
                'RP9'
                'RP10'
                'RP11'
                'RP12'
                'RP13'
                'RP14'
                'RP15')
            end
          end
          object edPage: TEdit
            Left = 201
            Top = 8
            Width = 32
            Height = 21
            Color = 14670284
            TabOrder = 2
            OnKeyPress = edPageKeyPress
            OnMouseDown = edPageMouseDown
          end
          object pgcLotInfo: TPageControl
            Left = 307
            Top = 2
            Width = 341
            Height = 365
            ActivePage = tsChipAdv
            Align = alRight
            TabIndex = 5
            TabOrder = 0
            object tsMurata: TTabSheet
              Caption = 'Murata'
              object edtLine: TLabeledEdit
                Left = 71
                Top = 4
                Width = 121
                Height = 21
                Color = 14670284
                EditLabel.Width = 20
                EditLabel.Height = 13
                EditLabel.Caption = 'Line'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                Enabled = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 0
              end
              object edtProcessName: TLabeledEdit
                Left = 71
                Top = 32
                Width = 121
                Height = 21
                Color = 14670284
                EditLabel.Width = 69
                EditLabel.Height = 13
                EditLabel.Caption = 'Process Name'
                Enabled = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 1
              end
              object edtProduct: TLabeledEdit
                Left = 71
                Top = 60
                Width = 121
                Height = 21
                Color = 14670284
                EditLabel.Width = 37
                EditLabel.Height = 13
                EditLabel.Caption = 'Product'
                Enabled = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 2
              end
              object pnlXMLOnLine: TPanel
                Left = 11
                Top = 90
                Width = 193
                Height = 33
                BevelInner = bvLowered
                Caption = 'Server On Line'
                Color = clLime
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = pnlXMLOnLineClick
              end
            end
            object tsSigurd_CX: TTabSheet
              Caption = 'Sigurd'
              ImageIndex = 1
              object labCustomerLotId: TLabel
                Left = 2
                Top = 6
                Width = 85
                Height = 13
                Caption = 'Customer Lot ID : '
              end
              object labStation: TLabel
                Left = 45
                Top = 32
                Width = 42
                Height = 13
                Caption = 'Station : '
              end
              object labStationNum: TLabel
                Left = 3
                Top = 58
                Width = 82
                Height = 13
                Caption = 'Station Number : '
              end
              object edCustomerLotId: TEdit
                Left = 89
                Top = 2
                Width = 132
                Height = 21
                Color = 14670284
                TabOrder = 0
              end
              object coStation: TComboBox
                Left = 89
                Top = 28
                Width = 52
                Height = 21
                Color = 14670284
                ItemHeight = 13
                TabOrder = 1
                Text = 'FT'
                OnKeyPress = cbRunModeKeyPress
                Items.Strings = (
                  'FT1'
                  'FT2'
                  'RT1'
                  'RT2'
                  'RT3'
                  'EQC')
              end
              object edStationNum: TEdit
                Left = 88
                Top = 54
                Width = 52
                Height = 21
                Color = 14670284
                TabOrder = 2
                OnMouseDown = edStationNumMouseDown
              end
            end
            object tsSPIL_SZ: TTabSheet
              Caption = 'SPIL'
              ImageIndex = 2
              object labJobSeq: TLabel
                Left = 9
                Top = 10
                Width = 70
                Height = 13
                Caption = 'Job sequence:'
              end
              object edtJobSeq: TEdit
                Left = 84
                Top = 6
                Width = 100
                Height = 21
                Color = 14670284
                TabOrder = 0
              end
            end
            object tsOEE: TTabSheet
              Caption = 'OEE'
              ImageIndex = 3
              object lbl_PIOEEMO: TLabel
                Left = 4
                Top = 94
                Width = 33
                Height = 20
                AutoSize = False
                Caption = 'MO'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clPurple
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
              end
              object sb_RunExecutFile: TSpeedButton
                Left = 1
                Top = 120
                Width = 124
                Height = 31
                Caption = 'Run Execut'
                Font.Charset = ANSI_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                Margin = 22
                ParentFont = False
                Spacing = -1
                Visible = False
                OnClick = sb_RunExecutFileClick
              end
              object pal_PIOEELotStatus: TPanel
                Left = 0
                Top = 0
                Width = 333
                Height = 87
                Align = alTop
                BevelOuter = bvNone
                Color = clYellow
                TabOrder = 0
                object lbl_PIOEELotStatus: TLabel
                  Left = 0
                  Top = 0
                  Width = 109
                  Height = 24
                  Align = alTop
                  Caption = 'Lot Status : '
                  Color = clYellow
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clPurple
                  Font.Height = -19
                  Font.Name = 'MS Sans Serif'
                  Font.Style = [fsBold]
                  ParentColor = False
                  ParentFont = False
                end
                object lb_PIOEELotStatus: TLabel
                  Left = 0
                  Top = 24
                  Width = 333
                  Height = 63
                  Align = alClient
                  Alignment = taRightJustify
                  AutoSize = False
                  Caption = 'None'
                  Color = clYellow
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clBlue
                  Font.Height = -19
                  Font.Name = 'MS Sans Serif'
                  Font.Style = [fsBold]
                  ParentColor = False
                  ParentFont = False
                  WordWrap = True
                end
              end
              object ed_PIOEEMO: TEdit
                Left = 36
                Top = 88
                Width = 201
                Height = 28
                Color = 14670284
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBlue
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                ParentFont = False
                ReadOnly = True
                TabOrder = 1
              end
            end
            object ts2DSort: TTabSheet
              Caption = '2DSort'
              ImageIndex = 4
              object grpManualSet2D: TGroupBox
                Left = 0
                Top = 0
                Width = 333
                Height = 149
                Align = alTop
                Caption = 'Manual Select 2D List'
                TabOrder = 0
                object Label53: TLabel
                  Left = 5
                  Top = 73
                  Width = 38
                  Height = 13
                  Caption = 'Lot ID : '
                  OnMouseDown = labLotIDMouseDown
                end
                object Label54: TLabel
                  Left = 5
                  Top = 97
                  Width = 43
                  Height = 13
                  Caption = 'Bin File : '
                  OnMouseDown = labLotIDMouseDown
                end
                object rgSort2DID: TRadioGroup
                  Left = 2
                  Top = 15
                  Width = 329
                  Height = 52
                  Align = alTop
                  Caption = 'Bin File Load'
                  Columns = 2
                  ItemIndex = 0
                  Items.Strings = (
                    'Input Barcode'
                    'Select File')
                  TabOrder = 0
                end
                object edSort2DIDLotID: TEdit
                  Left = 48
                  Top = 70
                  Width = 200
                  Height = 21
                  Hint = 'Can not use ([\/:*?"<>|])'
                  Color = 14670284
                  TabOrder = 1
                end
                object edSort2DIDBinFile: TEdit
                  Left = 48
                  Top = 94
                  Width = 200
                  Height = 21
                  Hint = 'Can not use ([\/:*?"<>|])'
                  Color = 14670284
                  TabOrder = 2
                end
                object btnLoadFile: TButton
                  Left = 160
                  Top = 119
                  Width = 76
                  Height = 21
                  Caption = 'File'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'MS Sans Serif'
                  Font.Style = []
                  ParentFont = False
                  TabOrder = 3
                  OnClick = btnLoadFileClick
                end
              end
              object grp2DLotInfo: TGroupBox
                Left = 0
                Top = 149
                Width = 333
                Height = 188
                Align = alTop
                Caption = '2D Lot Info'
                TabOrder = 1
                object mmo2DLotInfo: TMemo
                  Left = 2
                  Top = 15
                  Width = 329
                  Height = 171
                  Align = alClient
                  Color = 14670284
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -11
                  Font.Name = 'Courier New'
                  Font.Style = []
                  Lines.Strings = (
                    'Lot ID:'
                    'Stage:'
                    'OPERATOR:'
                    'CUST_CODE:'
                    'CUST_LOT_ID:'
                    'FAMILY:'
                    'CURR_DEVICE:'
                    'LOT_TYPE:'
                    'MACHINE_NO:'
                    'SUBCON_PROD:')
                  ParentFont = False
                  ScrollBars = ssVertical
                  TabOrder = 0
                end
              end
            end
            object tsChipAdv: TTabSheet
              Caption = 'ChipAdv'
              ImageIndex = 5
              object lbledtStarTime: TLabeledEdit
                Left = 79
                Top = 0
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 51
                EditLabel.Height = 13
                EditLabel.Caption = 'Start Time '
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 0
              end
              object lbledtEndTime: TLabeledEdit
                Left = 79
                Top = 23
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 48
                EditLabel.Height = 13
                EditLabel.Caption = 'End Time '
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 1
              end
              object lbledtTesterOsVer: TLabeledEdit
                Left = 79
                Top = 45
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 67
                EditLabel.Height = 13
                EditLabel.Caption = 'Tester OS Ver'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 2
              end
              object lbledtCustomer: TLabeledEdit
                Left = 79
                Top = 91
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 44
                EditLabel.Height = 13
                EditLabel.Caption = 'Customer'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 4
              end
              object lbledtTestProg: TLabeledEdit
                Left = 79
                Top = 114
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 63
                EditLabel.Height = 13
                EditLabel.Caption = 'Test Program'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 5
              end
              object lbledtDeviceName: TLabeledEdit
                Left = 79
                Top = 136
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 65
                EditLabel.Height = 13
                EditLabel.Caption = 'Device Name'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 6
              end
              object lbledtTesterID: TLabeledEdit
                Left = 79
                Top = 68
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 44
                EditLabel.Height = 13
                EditLabel.Caption = 'Tester ID'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 3
              end
              object lbledtSubLotNo: TLabeledEdit
                Left = 79
                Top = 159
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 56
                EditLabel.Height = 13
                EditLabel.Caption = 'Sub Lot NO'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 7
              end
              object lbledtTestCode: TLabeledEdit
                Left = 79
                Top = 204
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 49
                EditLabel.Height = 13
                EditLabel.Caption = 'Test Code'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 9
              end
              object lbledtMachineID: TLabeledEdit
                Left = 79
                Top = 295
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 55
                EditLabel.Height = 13
                EditLabel.Caption = 'Machine ID'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                Enabled = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                ReadOnly = True
                TabOrder = 13
              end
              object lbledtTestBinNo: TLabeledEdit
                Left = 79
                Top = 227
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 56
                EditLabel.Height = 13
                EditLabel.Caption = 'Test Bin No'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 10
              end
              object lbledtModeCode: TLabeledEdit
                Left = 79
                Top = 182
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 55
                EditLabel.Height = 13
                EditLabel.Caption = 'Mode Code'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 8
              end
              object edtLotEventLogName: TLabeledEdit
                Left = 79
                Top = 318
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 98
                EditLabel.Height = 13
                EditLabel.Caption = 'Lot Event Log Name'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                Enabled = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                ReadOnly = True
                TabOrder = 14
                Visible = False
              end
              object edtStage: TLabeledEdit
                Left = 79
                Top = 250
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 28
                EditLabel.Height = 13
                EditLabel.Caption = 'Stage'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 11
              end
              object edtStep: TLabeledEdit
                Left = 79
                Top = 273
                Width = 186
                Height = 21
                Color = 14670284
                EditLabel.Width = 22
                EditLabel.Height = 13
                EditLabel.Caption = 'Step'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 12
              end
            end
            object tsVTest: TTabSheet
              Caption = 'VTest'
              ImageIndex = 6
              object grpOEEState: TGroupBox
                Left = 0
                Top = 76
                Width = 265
                Height = 261
                Caption = 'OEE State'
                TabOrder = 0
                Visible = False
                object labOEEState: TLabel
                  Left = 18
                  Top = 15
                  Width = 107
                  Height = 24
                  Caption = 'State : Stop'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -19
                  Font.Name = 'MS Sans Serif'
                  Font.Style = [fsBold]
                  ParentFont = False
                end
                object sgOEEState: TStringGrid
                  Left = 13
                  Top = 42
                  Width = 133
                  Height = 213
                  ColCount = 2
                  DefaultRowHeight = 20
                  RowCount = 10
                  FixedRows = 0
                  TabOrder = 1
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
                    20)
                end
                object rgOEEState: TRadioGroup
                  Left = 155
                  Top = 37
                  Width = 86
                  Height = 218
                  Caption = 'State'
                  Items.Strings = (
                    'Production'
                    'Idle'
                    'Down'
                    'Setup'
                    'ENG'
                    'PM')
                  TabOrder = 0
                  OnClick = rgOEEStateClick
                end
              end
            end
            object tsPATSetUp: TTabSheet
              Caption = 'Set Up'
              ImageIndex = 7
              object sbLotReset: TSpeedButton
                Left = 11
                Top = 256
                Width = 62
                Height = 25
                GroupIndex = 1
                Caption = 'Lot Reset'
                OnClick = sbLotResetClick
              end
              object leRunCardNumber: TLabeledEdit
                Left = 91
                Top = 4
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 85
                EditLabel.Height = 13
                EditLabel.Caption = 'Run Card Number'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 0
                OnMouseDown = leRunCardNumberMouseDown
              end
              object cbPATMode: TComboBox
                Left = 9
                Top = 28
                Width = 88
                Height = 21
                Color = 14670284
                ItemHeight = 13
                TabOrder = 1
                Text = 'Normal Test'
                OnChange = cbPATModeChange
                Items.Strings = (
                  'Normal Test'
                  'Pre-Test'
                  'Re-Test'
                  'GD'
                  'EQC')
              end
              object btnPATInstall: TButton
                Left = 244
                Top = 29
                Width = 76
                Height = 21
                Caption = 'Install'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -11
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = btnPATInstallClick
              end
              object lnPartNumber: TLabeledEdit
                Left = 91
                Top = 76
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 62
                EditLabel.Height = 13
                EditLabel.Caption = 'Part Number:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 5
              end
              object lnTestProgram: TLabeledEdit
                Left = 91
                Top = 100
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 66
                EditLabel.Height = 13
                EditLabel.Caption = 'Test Program:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 6
              end
              object lnOpName: TLabeledEdit
                Left = 91
                Top = 124
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 51
                EditLabel.Height = 13
                EditLabel.Caption = 'Op. Name:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 7
              end
              object lnEntityNo: TLabeledEdit
                Left = 91
                Top = 148
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 46
                EditLabel.Height = 13
                EditLabel.Caption = 'Entity No:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 8
              end
              object lnLoadBoard: TLabeledEdit
                Left = 91
                Top = 172
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 58
                EditLabel.Height = 13
                EditLabel.Caption = 'Load Board:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 9
              end
              object lnStartTime: TLabeledEdit
                Left = 91
                Top = 196
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 51
                EditLabel.Height = 13
                EditLabel.Caption = 'Start Time:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 10
              end
              object lnEndTime: TLabeledEdit
                Left = 91
                Top = 220
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 48
                EditLabel.Height = 13
                EditLabel.Caption = 'End Time:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 11
              end
              object btnRealTime: TButton
                Left = 88
                Top = 256
                Width = 75
                Height = 25
                Caption = 'RealTime'
                TabOrder = 12
                Visible = False
                OnClick = btnRealTimeClick
              end
              object btnpatHourly: TButton
                Left = 168
                Top = 256
                Width = 75
                Height = 25
                Caption = 'Hourly'
                TabOrder = 13
                Visible = False
                OnClick = btnpatHourlyClick
              end
              object btnpatEndLot: TButton
                Left = 248
                Top = 256
                Width = 75
                Height = 25
                Caption = 'EndLot'
                TabOrder = 14
                Visible = False
                OnClick = btnpatEndLotClick
              end
              object cbPATSubMode: TComboBox
                Left = 105
                Top = 28
                Width = 88
                Height = 21
                Color = 14670284
                ItemHeight = 13
                TabOrder = 2
                Text = 'EQC'
                Items.Strings = (
                  'EQC')
              end
              object lnLotNumber: TLabeledEdit
                Left = 91
                Top = 52
                Width = 242
                Height = 21
                Color = 14670284
                EditLabel.Width = 58
                EditLabel.Height = 13
                EditLabel.Caption = 'Lot Number:'
                EditLabel.Font.Charset = DEFAULT_CHARSET
                EditLabel.Font.Color = clMenuText
                EditLabel.Font.Height = -11
                EditLabel.Font.Name = 'MS Sans Serif'
                EditLabel.Font.Style = []
                EditLabel.ParentFont = False
                LabelPosition = lpLeft
                LabelSpacing = 3
                TabOrder = 4
              end
            end
          end
          object edtCusLotID: TEdit
            Left = 93
            Top = 112
            Width = 100
            Height = 21
            Color = 14670284
            TabOrder = 4
            Visible = False
            OnKeyPress = edtSysOperatorIDKeyPress
            OnKeyUp = edtSysOperatorIDKeyUp
            OnMouseUp = edtSysOperatorIDMouseUp
          end
          object edtCusDevGrp: TEdit
            Left = 93
            Top = 136
            Width = 100
            Height = 21
            Color = 14670284
            TabOrder = 5
            Visible = False
            OnKeyPress = edtSysOperatorIDKeyPress
            OnKeyUp = edtSysOperatorIDKeyUp
            OnMouseUp = edtSysOperatorIDMouseUp
          end
          object edtDevice: TEdit
            Left = 93
            Top = 162
            Width = 100
            Height = 21
            Color = 14670284
            TabOrder = 6
            Visible = False
            OnKeyPress = edtSysOperatorIDKeyPress
            OnKeyUp = edtSysOperatorIDKeyUp
            OnMouseUp = edtSysOperatorIDMouseUp
          end
          object edtCusStep: TEdit
            Left = 93
            Top = 186
            Width = 100
            Height = 21
            Color = 14670284
            TabOrder = 7
            Visible = False
            OnKeyPress = edtSysOperatorIDKeyPress
            OnKeyUp = edtSysOperatorIDKeyUp
            OnMouseUp = edtSysOperatorIDMouseUp
          end
        end
        object palQAMode: TPanel
          Left = 0
          Top = 369
          Width = 650
          Height = 37
          Align = alTop
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 1
          Visible = False
          object labQACount: TLabel
            Left = 4
            Top = 12
            Width = 182
            Height = 20
            Caption = 'QA Mode Device Counts :'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object btnQAmodeSave: TButton
            Left = 292
            Top = 6
            Width = 93
            Height = 26
            Caption = 'Save'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btnQAmodeSaveClick
          end
          object edQAMode: TEdit
            Left = 196
            Top = 4
            Width = 93
            Height = 28
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '50'
            OnMouseDown = edQAModeMouseDown
          end
        end
        object grpRFID: TGroupBox
          Left = 0
          Top = 406
          Width = 650
          Height = 101
          Align = alTop
          Caption = 'RFID'
          TabOrder = 2
          object pnlLoader: TPanel
            Left = 12
            Top = 20
            Width = 197
            Height = 33
            TabOrder = 0
          end
          object btnGetLoader: TButton
            Left = 216
            Top = 20
            Width = 109
            Height = 33
            Caption = 'Get Loader RFID'
            TabOrder = 1
            OnClick = btnGetLoaderClick
          end
          object mmRFID: TMemo
            Left = 2
            Top = 58
            Width = 646
            Height = 41
            Align = alBottom
            ScrollBars = ssVertical
            TabOrder = 2
          end
        end
      end
    end
    object tsFTP: TTabSheet
      Caption = 'FTP'
      ImageIndex = 3
      object Panel25: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object lbFTPStatus: TLabel
          Left = 160
          Top = 165
          Width = 137
          Height = 20
          AutoSize = False
          Caption = 'Status'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object btnFtpServer: TButton
          Left = 3
          Top = 21
          Width = 143
          Height = 21
          Caption = 'Server'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          OnClick = btnFtpServerClick
        end
        object btnFtpHD: TButton
          Tag = 1
          Left = 4
          Top = 106
          Width = 143
          Height = 21
          Caption = 'HD'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          OnClick = btnFtpServerClick
        end
        object btnFtpTester: TButton
          Tag = 2
          Left = 4
          Top = 134
          Width = 143
          Height = 22
          Caption = 'Tester Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
          OnClick = btnFtpServerClick
        end
        object btSaveSetupFile: TButton
          Left = 4
          Top = 49
          Width = 143
          Height = 21
          Caption = 'Save to handler'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          Visible = False
          OnClick = btSaveSetupFileClick
        end
        object btnDataFTPSaveToData: TButton
          Tag = 3
          Left = 4
          Top = 77
          Width = 143
          Height = 21
          Caption = 'DataFTP Save  to  Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          OnClick = btnFtpServerClick
        end
        object btnFTPTryConnect: TButton
          Tag = 4
          Left = 4
          Top = 161
          Width = 143
          Height = 22
          Caption = 'Connection test'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
          Visible = False
        end
        object BtnPause: TBtnPanel
          Left = 215
          Top = 19
          Width = 127
          Height = 41
          BevelInner = bvRaised
          Caption = 'PAUSE'
          Color = 8404992
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = BtnPauseClick
          OnMouseDown = BtnPauseMouseDown
          OnMouseUp = BtnPauseMouseUp
          TrueColor = clBlue
          FalseColor = 8404992
          TrueFontColor = clYellow
          FalseFontColor = clWhite
          Style = tsButtons
        end
      end
      object Panel26: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'FTP'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsRTCFullViewImg: TTabSheet
      Caption = 'RTC Full View Image'
      ImageIndex = 4
      object Panel31: TPanel
        Left = 0
        Top = 0
        Width = 626
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'RTC'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel32: TPanel
        Left = 0
        Top = 25
        Width = 626
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label6: TLabel
          Left = 48
          Top = 266
          Width = 162
          Height = 16
          Caption = 'Index Arm Exchange Status'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 48
          Top = 240
          Width = 132
          Height = 16
          Caption = 'RTC Inspection Status'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 48
          Top = 292
          Width = 149
          Height = 16
          Caption = 'RTC Temperature Status'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object ALed1: TALed
          Left = 20
          Top = 236
          Width = 22
          Height = 22
          TrueColor = clRed
          LEDStyle = LEDSqLarge
        end
        object ALed2: TALed
          Left = 20
          Top = 262
          Width = 22
          Height = 22
          TrueColor = clGreen
          LEDStyle = LEDSqLarge
        end
        object ALed3: TALed
          Left = 20
          Top = 288
          Width = 22
          Height = 22
          TrueColor = clRed
          LEDStyle = LEDSqLarge
        end
        object imgRTCFullView2: TImage
          Left = 16
          Top = 112
          Width = 374
          Height = 100
          Stretch = True
        end
        object imgRTCFullView1: TImage
          Left = 16
          Top = 8
          Width = 374
          Height = 100
          Stretch = True
        end
        object imgRTCFullView3: TImage
          Left = 219
          Top = 8
          Width = 374
          Height = 100
          Stretch = True
        end
        object imgRTCFullView4: TImage
          Left = 219
          Top = 112
          Width = 374
          Height = 100
          Stretch = True
        end
        object btStartCount: TButton
          Left = 280
          Top = 272
          Width = 75
          Height = 25
          Caption = 'Start Count'
          TabOrder = 1
          OnClick = btStartCountClick
        end
        object cbRTCASTD: TCheckBox
          Left = 23
          Top = 216
          Width = 155
          Height = 17
          Caption = 'Auto STD IC PARA TUNE'
          TabOrder = 0
          Visible = False
          OnClick = cbRTCASTDClick
        end
      end
    end
    object tsATC: TTabSheet
      Caption = 'ATC'
      ImageIndex = 5
      object pan_ATCInterface: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Panel1: TPanel
          Left = 0
          Top = 0
          Width = 650
          Height = 25
          Align = alTop
          BevelInner = bvLowered
          Caption = 'Working Temperature'
          Color = 15313978
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object palATCWorkingTemp: TPanel
          Left = 0
          Top = 25
          Width = 650
          Height = 25
          Align = alTop
          BevelInner = bvLowered
          Caption = '0.0 '#39'C'
          Color = 2468626
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlATC_Flag: TPanel
          Left = 0
          Top = 50
          Width = 650
          Height = 75
          Align = alTop
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 2
          object aldATCPower: TALed
            Left = 3
            Top = 2
            Width = 22
            Height = 22
            Hint = 'ATC Power'
            LEDStyle = LEDSqLarge
          end
          object aldATCChillerStatus: TALed
            Left = 3
            Top = 26
            Width = 22
            Height = 22
            Hint = 'ATCChillerStatus'
            LEDStyle = LEDSqLarge
          end
          object Label9: TLabel
            Left = 28
            Top = 4
            Width = 80
            Height = 18
            Caption = 'ATC Power'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object lblChiller: TLabel
            Left = 28
            Top = 28
            Width = 104
            Height = 18
            Caption = 'Chiller Running'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object lblATC_Now_RecipeFile: TLabel
            Left = 180
            Top = 51
            Width = 155
            Height = 20
            Caption = 'ATC_Now_RecipeFile'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            Visible = False
          end
          object aldATC7Status: TALed
            Left = 3
            Top = 50
            Width = 22
            Height = 22
            Hint = 'ATCChillerStatus'
            LEDStyle = LEDSqLarge
          end
          object lblATC70: TLabel
            Left = 28
            Top = 52
            Width = 153
            Height = 18
            AutoSize = False
            Caption = 'ATC 7.0 Running'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object pl_ATC_Online: TPanel
            Left = 240
            Top = 4
            Width = 105
            Height = 33
            BevelInner = bvLowered
            Caption = 'ATC On Line'
            Color = clLime
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = pl_ATC_OnlineClick
          end
          object btnClearTemperature: TButton
            Left = 140
            Top = 4
            Width = 97
            Height = 33
            Caption = 'Clear temperature'
            TabOrder = 0
            Visible = False
            OnClick = btnClearTemperatureClick
          end
          object pan_ATCChillerSV: TPanel
            Left = 352
            Top = 4
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = 'Chiller SV'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pl_ATCChillerSV: TPanel
            Left = 352
            Top = 27
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object pan_DewPoint: TPanel
            Left = 436
            Top = 4
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = 'DewPoint '
            Color = clHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object pl_DewPoint: TPanel
            Left = 436
            Top = 27
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = '123.34'
            Color = clAqua
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 5
          end
        end
        object Pan_ATC_Use_8Head: TPanel
          Left = 272
          Top = 124
          Width = 273
          Height = 88
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 4
          Visible = False
          object pan_ATCTempHead05: TPanel
            Left = 0
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH5'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object pan_ATCTempHead06: TPanel
            Left = 68
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH6'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object pan_ATCTempHead07: TPanel
            Left = 136
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH7'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pan_ATCTempHead08: TPanel
            Left = 204
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH8'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object pl_ATCTempHead05: TPanel
            Left = 0
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object pl_ATCTempHead06: TPanel
            Left = 68
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object pl_ATCTempHead07: TPanel
            Left = 136
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object pl_ATCTempHead08: TPanel
            Left = 204
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object pl_ATCRefHead05: TPanel
            Left = 0
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object pl_ATCRefHead07: TPanel
            Left = 136
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object pl_ATCRefHead08: TPanel
            Left = 204
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object pl_ATCRefHead06: TPanel
            Left = 68
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
        end
        object Pan_ATC_Use_4Head: TPanel
          Left = 0
          Top = 124
          Width = 273
          Height = 88
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 3
          object pan_ATCTempHead01: TPanel
            Left = 0
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH1'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object pl_ATCTempHead01: TPanel
            Left = 0
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object pan_ATCTempHead02: TPanel
            Left = 68
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH2'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object pl_ATCTempHead02: TPanel
            Left = 68
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object pan_ATCTempHead03: TPanel
            Left = 136
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH3'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pl_ATCTempHead03: TPanel
            Left = 136
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object pan_ATCTempHead04: TPanel
            Left = 204
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH4'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object pl_ATCTempHead04: TPanel
            Left = 204
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object pl_ATCRefHead01: TPanel
            Left = 0
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object pl_ATCRefHead02: TPanel
            Left = 68
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object pl_ATCRefHead03: TPanel
            Left = 136
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object pl_ATCRefHead04: TPanel
            Left = 204
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
        end
        object Pan_ATC_Use_32Head: TPanel
          Left = 0
          Top = 212
          Width = 546
          Height = 265
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 5
          object pan_ATCTempHead09: TPanel
            Left = 0
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH9'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object pl_ATCRefHead09: TPanel
            Left = 0
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 16
          end
          object pan_ATCTempHead10: TPanel
            Left = 68
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH10'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object pl_ATCRefHead10: TPanel
            Left = 68
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 17
          end
          object pan_ATCTempHead11: TPanel
            Left = 136
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH11'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pl_ATCRefHead11: TPanel
            Left = 136
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 18
          end
          object pan_ATCTempHead12: TPanel
            Left = 204
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH12'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object pl_ATCRefHead12: TPanel
            Left = 204
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 19
          end
          object pl_ATCTempHead09: TPanel
            Left = 0
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object pl_ATCTempHead10: TPanel
            Left = 68
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object pl_ATCTempHead11: TPanel
            Left = 136
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object pl_ATCTempHead12: TPanel
            Left = 204
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object pan_ATCTempHead13: TPanel
            Left = 272
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH13'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object pan_ATCTempHead14: TPanel
            Left = 340
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH14'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object pan_ATCTempHead15: TPanel
            Left = 408
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH15'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object pan_ATCTempHead16: TPanel
            Left = 476
            Top = 0
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH16'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object pl_ATCRefHead13: TPanel
            Left = 272
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 20
          end
          object pl_ATCRefHead14: TPanel
            Left = 340
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 21
          end
          object pl_ATCRefHead15: TPanel
            Left = 408
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 22
          end
          object pl_ATCRefHead16: TPanel
            Left = 476
            Top = 46
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 23
          end
          object pl_ATCTempHead13: TPanel
            Left = 272
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object pl_ATCTempHead15: TPanel
            Left = 408
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object pl_ATCTempHead16: TPanel
            Left = 476
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
          end
          object pl_ATCTempHead14: TPanel
            Left = 340
            Top = 23
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object pan_ATCTempHead17: TPanel
            Left = 0
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH17'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 24
          end
          object pl_ATCRefHead17: TPanel
            Left = 0
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 40
          end
          object pan_ATCTempHead18: TPanel
            Left = 68
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH18'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 25
          end
          object pl_ATCRefHead18: TPanel
            Left = 68
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 41
          end
          object pan_ATCTempHead19: TPanel
            Left = 136
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH19'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 26
          end
          object pl_ATCRefHead19: TPanel
            Left = 136
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 42
          end
          object pan_ATCTempHead20: TPanel
            Left = 204
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH20'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 27
          end
          object pl_ATCRefHead20: TPanel
            Left = 204
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 43
          end
          object pl_ATCTempHead17: TPanel
            Left = 0
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 32
          end
          object pl_ATCTempHead18: TPanel
            Left = 68
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 33
          end
          object pl_ATCTempHead19: TPanel
            Left = 136
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 34
          end
          object pl_ATCTempHead20: TPanel
            Left = 204
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 35
          end
          object pan_ATCTempHead21: TPanel
            Left = 272
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH21'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 28
          end
          object pan_ATCTempHead22: TPanel
            Left = 340
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH22'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 29
          end
          object pan_ATCTempHead23: TPanel
            Left = 408
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH23'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 30
          end
          object pan_ATCTempHead24: TPanel
            Left = 476
            Top = 87
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH24'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 31
          end
          object pl_ATCRefHead21: TPanel
            Left = 272
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 44
          end
          object pl_ATCRefHead22: TPanel
            Left = 340
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 45
          end
          object pl_ATCRefHead23: TPanel
            Left = 408
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 46
          end
          object pl_ATCRefHead24: TPanel
            Left = 476
            Top = 133
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 47
          end
          object pl_ATCTempHead21: TPanel
            Left = 272
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 36
          end
          object pl_ATCTempHead23: TPanel
            Left = 408
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 38
          end
          object pl_ATCTempHead24: TPanel
            Left = 476
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 39
          end
          object pl_ATCTempHead22: TPanel
            Left = 340
            Top = 110
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 37
          end
          object pan_ATCTempHead25: TPanel
            Left = 0
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH25'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 48
          end
          object pl_ATCRefHead25: TPanel
            Left = 0
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 64
          end
          object pan_ATCTempHead26: TPanel
            Left = 68
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH26'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 49
          end
          object pl_ATCRefHead26: TPanel
            Left = 68
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 65
          end
          object pan_ATCTempHead27: TPanel
            Left = 136
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH27'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 50
          end
          object pl_ATCRefHead27: TPanel
            Left = 136
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 66
          end
          object pan_ATCTempHead28: TPanel
            Left = 204
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH28'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 51
          end
          object pl_ATCRefHead28: TPanel
            Left = 204
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 67
          end
          object pl_ATCTempHead25: TPanel
            Left = 0
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 56
          end
          object pl_ATCTempHead26: TPanel
            Left = 68
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 57
          end
          object pl_ATCTempHead27: TPanel
            Left = 136
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 58
          end
          object pl_ATCTempHead28: TPanel
            Left = 204
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 59
          end
          object pan_ATCTempHead29: TPanel
            Left = 272
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH29'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 52
          end
          object pan_ATCTempHead30: TPanel
            Left = 340
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH30'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 53
          end
          object pan_ATCTempHead31: TPanel
            Left = 408
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH31'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 54
          end
          object pan_ATCTempHead32: TPanel
            Left = 476
            Top = 175
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = 'CH32'
            Color = 15313978
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 55
          end
          object pl_ATCRefHead29: TPanel
            Left = 272
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 68
          end
          object pl_ATCRefHead30: TPanel
            Left = 340
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 69
          end
          object pl_ATCRefHead31: TPanel
            Left = 408
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 70
          end
          object pl_ATCRefHead32: TPanel
            Left = 476
            Top = 221
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 71
          end
          object pl_ATCTempHead29: TPanel
            Left = 272
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 60
          end
          object pl_ATCTempHead31: TPanel
            Left = 408
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 62
          end
          object pl_ATCTempHead32: TPanel
            Left = 476
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 63
          end
          object pl_ATCTempHead30: TPanel
            Left = 340
            Top = 198
            Width = 70
            Height = 25
            BevelInner = bvLowered
            Caption = '0.0 '#39'C'
            Color = 2468626
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 61
          end
        end
      end
      object palATC: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'ATC 2.0 Monitor'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object ts_OCRInterface: TTabSheet
      Caption = 'OCR'
      ImageIndex = 6
      object Panel23: TPanel
        Left = 0
        Top = 25
        Width = 539
        Height = 516
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object lbl1: TLabel
          Left = 4
          Top = 140
          Width = 92
          Height = 13
          Caption = 'Handler with Tester'
        end
        object lbl2: TLabel
          Left = 4
          Top = 160
          Width = 97
          Height = 13
          Caption = 'LOTID from Tester : '
        end
        object lblTester_LotID: TLabel
          Left = 101
          Top = 160
          Width = 72
          Height = 13
          Caption = 'lblTester_LotID'
        end
        object Label11: TLabel
          Left = 4
          Top = 172
          Width = 90
          Height = 13
          Caption = 'LOTID from OCR : '
        end
        object lblOCR_LotID: TLabel
          Left = 101
          Top = 172
          Width = 65
          Height = 13
          Caption = 'lblOCR_LotID'
        end
        object Image1: TImage
          Left = 0
          Top = 395
          Width = 539
          Height = 121
          Align = alBottom
          Stretch = True
        end
        object mmoHandlerWithTester: TMemo
          Left = 0
          Top = 0
          Width = 539
          Height = 133
          Align = alTop
          Color = 14670284
          TabOrder = 0
        end
        object palHandlerwithTester: TPanel
          Left = 105
          Top = 138
          Width = 161
          Height = 17
          Caption = 'Connection'
          Color = clLime
          TabOrder = 1
        end
      end
      object Panel24: TPanel
        Left = 0
        Top = 0
        Width = 539
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'OCR'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
      end
    end
    object ts_SocketInterface: TTabSheet
      Caption = 'Socket'
      ImageIndex = 7
      object Panel21: TPanel
        Left = 0
        Top = 0
        Width = 539
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Socket'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel22: TPanel
        Left = 0
        Top = 25
        Width = 539
        Height = 516
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label12: TLabel
          Left = 4
          Top = 14
          Width = 57
          Height = 13
          Caption = 'Handler ID :'
          Visible = False
        end
        object OnLine: TSpeedButton
          Left = 376
          Top = 8
          Width = 45
          Height = 45
          Enabled = False
          Glyph.Data = {
            360C0000424D360C000000000000360000002800000020000000200000000100
            180000000000000C0000130B0000130B00000000000000000000FFFFFF000000
            0000000000000000000000000000000000000000000000000000000000000000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000000000000000000000000000
            0000000000000000000000000000000000000000FFFFFFFFFFFF808080808080
            8080808080808080808080808080808080808080808080808080808080808080
            80000000FFFFFFFFFFFFFFFFFF80808080808080808080808080808080808080
            8080808080808080808080808080808080808080000000FFFFFF808080FFFFFF
            C0C0C000FF0000FF00C0C0C0C0C0C0000000000000000000000000C0C0C08080
            80000000FFFFFFFFFFFFFFFFFF808080FFFFFFC0C0C000FF0000FF00C0C0C0C0
            C0C0000000000000000000000000C0C0C0808080000000FFFFFF808080FFFFFF
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0808080808080808080808080C0C0C08080
            80000000FFFFFFFFFFFFFFFFFF808080FFFFFFC0C0C0C0C0C0C0C0C0C0C0C0C0
            C0C0808080808080808080808080C0C0C0808080000000FFFFFF808080FFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8080
            80000000FFFFFFFFFFFFFFFFFF808080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080000000FFFFFFFFFFFF808080
            000000000000000000000000000000000000000000000000000000000000C0C0
            C0000000FFFFFFFFFFFFFFFFFFFFFFFF80808000000000000000000000000000
            0000000000000000000000000000000000C0C0C0000000FFFFFFFFFFFF808080
            8080808080808080808080808080808080808080808080808080808080800000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF80808080808080808080808080808080
            8080808080808080808080808080808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFFC0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C08080800000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFC0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFF808080000000BF0000BF0000BF0000BF0000BF0000C0C0C08080800000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF808080000000BF0000BF
            0000BF0000BF0000BF0000C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFF808080000000FF0000FF0000FF0000FF0000BF0000C0C0C08080800000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF808080000000FF0000FF
            0000FF0000FF0000BF0000C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFF808080000000FF0000FF0000FF0000FF0000BF0000C0C0C08080800000
            00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF808080000000FF0000FF
            0000FF0000FF0000BF0000C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFF808080000000FFFF00FF0000FF0000FF0000BF0000C0C0C08080800000
            00000000FFFFFFFFFFFFFFFFFF000000808080FFFFFF808080000000FFFF00FF
            0000FF0000FF0000BF0000C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFF808080000000000000000000000000000000000000C0C0C08080800000
            00FFFFFF000000000000000000FFFFFF808080FFFFFF80808000000000000000
            0000000000000000000000C0C0C0808080000000FFFFFFFFFFFFFFFFFF808080
            FFFFFFC0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C08080800000
            00FFFFFFFFFFFF000000FFFFFFFFFFFF808080FFFFFFC0C0C0C0C0C0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0808080000000FFFFFFFFFFFFFFFFFFFFFFFF
            808080808080808080808080808080808080808080808080808080808080FFFF
            FFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFF80808080808080808080808080
            8080808080808080808080808080808080FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFF000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000
            00000000000000000000000000000000000000000000000000000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8080808080808080808080
            80808080808080808080808080808080808080808080808080808080000000FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFC0C0C000FF
            0000FF00C0C0C0C0C0C0000000000000000000000000C0C0C0808080000000FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFC0C0C0C0C0
            C0C0C0C0C0C0C0C0C0C0808080808080808080808080C0C0C0808080000000FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080000000FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8080800000000000
            00000000000000000000000000000000000000000000000000C0C0C0000000FF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8080808080808080
            80808080808080808080808080808080808080808080808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFC0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF8080
            80000000BFBF00BFBF00BFBF00BFBF00808080C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF8080
            80000000BFBF00BFBF00BFBF00BFBF00808080C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF8080
            80000000BFBF00BFBF00BFBF00BFBF00808080C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF8080
            80000000FFFF00BFBF00BFBF00BFBF00808080C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFF8080
            80000000000000000000000000000000808080C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF808080FFFFFFC0C0
            C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0808080000000FFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8080808080
            80808080808080808080808080808080808080808080808080FFFFFFFFFFFFFF
            FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        end
        object edtHandlerID: TEdit
          Left = 76
          Top = 9
          Width = 201
          Height = 21
          Color = 14670284
          TabOrder = 0
          Text = 'edtHandlerID'
          Visible = False
        end
        object Memo1: TMemo
          Left = 0
          Top = 263
          Width = 539
          Height = 253
          Align = alBottom
          Color = 14670284
          TabOrder = 1
        end
      end
    end
    object tsSelection: TTabSheet
      Caption = 'Selection'
      ImageIndex = 8
      object Panel13: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Selection'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel14: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label44: TLabel
          Left = 6
          Top = 9
          Width = 375
          Height = 24
          AutoSize = False
          Caption = 'The data of checked item will be overwritten.'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object groupbDownloadItem: TGroupBox
          Left = 8
          Top = 31
          Width = 337
          Height = 302
          Caption = 'Download'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object chkTempOffset: TCheckBox
            Left = 11
            Top = 28
            Width = 115
            Height = 17
            Caption = 'Temperature Offset'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object chkContactHigh: TCheckBox
            Left = 11
            Top = 56
            Width = 97
            Height = 17
            Caption = 'Contact Height'
            TabOrder = 2
          end
          object chkContactForce: TCheckBox
            Left = 11
            Top = 83
            Width = 97
            Height = 17
            Caption = 'Contact Force'
            TabOrder = 4
          end
          object chkContactMode: TCheckBox
            Left = 11
            Top = 111
            Width = 97
            Height = 17
            Caption = 'Contact Mode'
            TabOrder = 6
          end
          object chkHotPlate: TCheckBox
            Left = 156
            Top = 28
            Width = 97
            Height = 17
            Caption = 'HotPlate'
            TabOrder = 1
          end
          object chkLoadUnload: TCheckBox
            Left = 156
            Top = 56
            Width = 97
            Height = 17
            Caption = 'Load Unload'
            TabOrder = 3
          end
          object chkSpeedSetting: TCheckBox
            Left = 156
            Top = 83
            Width = 97
            Height = 17
            Caption = 'Speed Setting'
            TabOrder = 5
          end
          object chkShuttleMode: TCheckBox
            Left = 156
            Top = 111
            Width = 97
            Height = 17
            Caption = 'Shuttle Mode'
            TabOrder = 7
          end
          object btnSave: TButton
            Left = 11
            Top = 272
            Width = 75
            Height = 25
            Caption = 'Save'
            TabOrder = 17
            OnClick = btnSaveClick
          end
          object chkTestMode: TCheckBox
            Left = 156
            Top = 139
            Width = 97
            Height = 17
            Caption = 'Test Mode'
            TabOrder = 9
          end
          object chkBinasgn: TCheckBox
            Left = 156
            Top = 166
            Width = 97
            Height = 17
            Caption = 'Binasgn'
            TabOrder = 10
          end
          object chkBinasgnOff: TCheckBox
            Left = 156
            Top = 194
            Width = 97
            Height = 17
            Caption = 'BinasgnOff'
            TabOrder = 13
          end
          object checkbAutoClean: TCheckBox
            Left = 11
            Top = 139
            Width = 97
            Height = 17
            Caption = 'Auto Clean'
            TabOrder = 8
          end
          object chkIndexHeatingMode: TCheckBox
            Left = 156
            Top = 222
            Width = 145
            Height = 17
            Caption = 'Index Heating Mode'
            TabOrder = 15
          end
          object chkART: TCheckBox
            Left = 11
            Top = 167
            Width = 97
            Height = 17
            Caption = 'Auto Retest'
            TabOrder = 11
          end
          object chkART_RTCount: TCheckBox
            Left = 11
            Top = 194
            Width = 130
            Height = 17
            Caption = 'Auto Retest RT Count'
            TabOrder = 12
          end
          object cbBottom2DOffset: TCheckBox
            Left = 156
            Top = 249
            Width = 145
            Height = 17
            Caption = 'Bottom 2DID offset'
            TabOrder = 16
          end
          object chkCleanCount: TCheckBox
            Left = 11
            Top = 222
            Width = 97
            Height = 17
            Caption = 'Cleaning count'
            TabOrder = 14
          end
          object chkPositionOffset: TCheckBox
            Left = 11
            Top = 249
            Width = 97
            Height = 17
            Caption = 'Position Offset'
            TabOrder = 18
          end
        end
        object grpMesCheck: TGroupBox
          Left = 8
          Top = 335
          Width = 337
          Height = 59
          Caption = 'Mes Check'
          TabOrder = 2
          Visible = False
          object chkStopYield: TCheckBox
            Left = 11
            Top = 24
            Width = 97
            Height = 17
            Caption = 'Stop Yield'
            TabOrder = 0
          end
          object chkConsecutiveFailure: TCheckBox
            Left = 155
            Top = 24
            Width = 97
            Height = 17
            Caption = 'Consecutive Failure'
            TabOrder = 1
          end
        end
        object chkAutoCleanContactHeight: TCheckBox
          Left = 164
          Top = 308
          Width = 147
          Height = 19
          Caption = 'Auto Clean Contact Height'
          TabOrder = 1
        end
        object rgHomeStopPos: TRadioGroup
          Left = 8
          Top = 412
          Width = 337
          Height = 57
          Caption = 'In out arm home stop position'
          Columns = 2
          Items.Strings = (
            'In arm to shuttle'
            'Out arm to shuttle')
          TabOrder = 3
        end
      end
    end
    object tsBarCode: TTabSheet
      Caption = 'BarCode'
      ImageIndex = 7
      object Panel16: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Barcode'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel17: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object sgBarcode: TStringGrid
          Left = 0
          Top = 0
          Width = 650
          Height = 189
          Align = alTop
          Color = 14670284
          ColCount = 6
          FixedColor = 12761254
          RowCount = 7
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = #27161#26999#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object btClearBarcodeCount: TButton
          Left = 4
          Top = 232
          Width = 137
          Height = 25
          Caption = 'Clear Count'
          TabOrder = 2
          OnClick = btClearBarcodeCountClick
        end
        object btClearBarcodeList: TButton
          Left = 152
          Top = 232
          Width = 127
          Height = 25
          Caption = 'Clear List'
          TabOrder = 3
          OnClick = btClearBarcodeListClick
        end
        object btChangeFile: TButton
          Left = 290
          Top = 232
          Width = 127
          Height = 25
          Caption = 'Change File'
          TabOrder = 4
          OnClick = btChangeFileClick
        end
        object grpBarcodeDisplayLotInfo: TGroupBox
          Left = 0
          Top = 410
          Width = 650
          Height = 98
          Align = alBottom
          TabOrder = 6
          Visible = False
          object myInShuttleLotInfo: TTMyTray
            Left = 3
            Top = 12
            Width = 62
            Height = 80
            XItem = 1
            YItem = 3
            DirectWidth = 0
          end
          object mtBarcodeInShLotInfo: TTMyTray
            Left = 65
            Top = 12
            Width = 360
            Height = 80
            YItem = 3
            DirectWidth = 0
          end
        end
        object lbCheckCodeByLot: TPanel
          Left = 0
          Top = 189
          Width = 650
          Height = 41
          Align = alTop
          BevelInner = bvLowered
          Caption = 'Check duplicate code by lot'
          Color = 14670284
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
        object edtLotVerify: TLabeledEdit
          Left = 104
          Top = 276
          Width = 233
          Height = 21
          EditLabel.Width = 58
          EditLabel.Height = 13
          EditLabel.Caption = 'Lot Verify ID'
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 5
          OnKeyPress = edtLotVerifyKeyPress
          OnMouseDown = edtLotVerifyMouseDown
        end
      end
    end
    object ts_AutoCleanMonitor: TTabSheet
      Caption = 'Auto Clean Monitor'
      ImageIndex = 10
      object palAutoCleanMonitor: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 533
        Align = alClient
        Color = 12761254
        TabOrder = 0
        object GroupBox1: TGroupBox
          Left = 1
          Top = 26
          Width = 648
          Height = 113
          Align = alTop
          Caption = 'Auto Clean Yield'
          TabOrder = 1
          object Label13: TLabel
            Left = 16
            Top = 32
            Width = 89
            Height = 13
            AutoSize = False
            Caption = 'Now Low Yield'
          end
          object Label14: TLabel
            Left = 136
            Top = 35
            Width = 17
            Height = 13
            AutoSize = False
            Caption = '%'
          end
          object Label15: TLabel
            Left = 16
            Top = 80
            Width = 121
            Height = 13
            AutoSize = False
            Caption = 'Now Site Yield Different'
          end
          object Label16: TLabel
            Left = 176
            Top = 83
            Width = 17
            Height = 13
            AutoSize = False
            Caption = '%'
          end
          object Label17: TLabel
            Left = 160
            Top = 32
            Width = 88
            Height = 13
            Caption = 'User set : '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = #32048#26126#39636
            Font.Style = [fsBold]
            ParentFont = False
          end
          object Label18: TLabel
            Left = 200
            Top = 80
            Width = 88
            Height = 13
            Caption = 'User set : '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = #32048#26126#39636
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edtAutoCleanLowYield: TEdit
            Left = 96
            Top = 28
            Width = 33
            Height = 21
            Enabled = False
            ReadOnly = True
            TabOrder = 0
            Text = '100'
          end
          object edtAutoCleanSiteYieldDiff: TEdit
            Left = 136
            Top = 76
            Width = 33
            Height = 21
            Enabled = False
            TabOrder = 1
            Text = '100'
          end
        end
        object GroupBox2: TGroupBox
          Left = 1
          Top = 139
          Width = 648
          Height = 73
          Align = alTop
          TabOrder = 2
          object Label19: TLabel
            Left = 24
            Top = 24
            Width = 89
            Height = 13
            AutoSize = False
            Caption = 'Contact Count'
          end
          object Label20: TLabel
            Left = 232
            Top = 24
            Width = 41
            Height = 13
            AutoSize = False
            Caption = 'Count'
          end
          object Label21: TLabel
            Left = 25
            Top = 47
            Width = 88
            Height = 13
            Caption = 'User set : '
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlue
            Font.Height = -13
            Font.Name = #32048#26126#39636
            Font.Style = [fsBold]
            ParentFont = False
          end
          object edtAutoCleanContactCount: TEdit
            Left = 120
            Top = 20
            Width = 89
            Height = 21
            Enabled = False
            TabOrder = 0
            Text = '0'
          end
        end
        object Panel18: TPanel
          Left = 1
          Top = 1
          Width = 648
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Auto Clean Monitor'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
      end
    end
    object ts_AutoRetestMonitor: TTabSheet
      Caption = 'AutoRetestMonitor'
      ImageIndex = 11
      object palAutoRetestMonitor: TPanel
        Left = 0
        Top = 0
        Width = 626
        Height = 533
        Align = alClient
        Color = 12761254
        TabOrder = 0
        object ART_Panel: TPanel
          Left = 4
          Top = 5
          Width = 428
          Height = 372
          Color = 12761254
          TabOrder = 1
          object LART_Limit: TLabel
            Left = 4
            Top = 9
            Width = 91
            Height = 16
            Caption = 'ART max limit : '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_Count: TLabel
            Left = 4
            Top = 42
            Width = 129
            Height = 16
            Caption = 'Currently ART Count : '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_Passlimit: TLabel
            Left = 4
            Top = 67
            Width = 112
            Height = 16
            Caption = 'ART Pass % limit : '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_PassYield: TLabel
            Left = 4
            Top = 108
            Width = 107
            Height = 16
            Caption = 'Currently Pass %: '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_Openlimit: TLabel
            Left = 4
            Top = 131
            Width = 104
            Height = 16
            Caption = 'ART O/S % limit : '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_OpenYield: TLabel
            Left = 4
            Top = 168
            Width = 99
            Height = 16
            Caption = 'Currently O/S %: '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_RecoverPasslimit: TLabel
            Left = 4
            Top = 192
            Width = 167
            Height = 16
            Caption = 'ART Recover Pass % limit : '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object LART_RecoverPassYield: TLabel
            Left = 4
            Top = 229
            Width = 92
            Height = 16
            Caption = 'RecovPass %: '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label33: TLabel
            Left = 115
            Top = 105
            Width = 5
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clLime
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label48: TLabel
            Left = 203
            Top = 103
            Width = 11
            Height = 24
            Caption = '>'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object eART_PasslimitL: TLabel
            Left = 229
            Top = 103
            Width = 10
            Height = 24
            Caption = '0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object eART_OpenlimitL: TLabel
            Left = 227
            Top = 158
            Width = 10
            Height = 24
            Caption = '0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label49: TLabel
            Left = 203
            Top = 159
            Width = 11
            Height = 24
            Caption = '<'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label50: TLabel
            Left = 202
            Top = 219
            Width = 11
            Height = 24
            Caption = '<'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object eART_RecoverPasslimitL: TLabel
            Left = 227
            Top = 218
            Width = 10
            Height = 24
            Caption = '0'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label34: TLabel
            Left = 4
            Top = 253
            Width = 111
            Height = 13
            Caption = 'Auto1 output amount :  '
          end
          object Label35: TLabel
            Left = 4
            Top = 280
            Width = 108
            Height = 13
            Caption = 'Auto2 output amount : '
          end
          object Label51: TLabel
            Left = 4
            Top = 307
            Width = 108
            Height = 13
            Caption = 'Auto3 output amount:  '
          end
          object eART_Limit: TEdit
            Left = 131
            Top = 3
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 0
            Text = '0'
          end
          object eART_Count: TEdit
            Left = 131
            Top = 36
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 1
            Text = '0'
          end
          object eART_Passlimit: TEdit
            Left = 31
            Top = 85
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 2
            Text = '0'
          end
          object eART_PassYield: TEdit
            Left = 131
            Top = 106
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 3
            Text = '0'
          end
          object eART_Openlimit: TEdit
            Left = 35
            Top = 145
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 4
            Text = '0'
          end
          object eART_OpenYield: TEdit
            Left = 128
            Top = 161
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 5
            Text = '0'
          end
          object eART_RecoverPasslimit: TEdit
            Left = 35
            Top = 206
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 6
            Text = '0'
          end
          object eART_RecoverPassYield: TEdit
            Left = 128
            Top = 222
            Width = 65
            Height = 21
            Color = 14670284
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 7
            Text = '0'
          end
          object edAuto1Cnt_KS: TEdit
            Left = 128
            Top = 249
            Width = 65
            Height = 21
            Color = 14670284
            ReadOnly = True
            TabOrder = 8
            Text = '0'
          end
          object edAuto2Cnt_KS: TEdit
            Left = 128
            Top = 276
            Width = 65
            Height = 21
            Color = 14670284
            ReadOnly = True
            TabOrder = 9
            Text = '0'
          end
          object edAuto3Cnt_KS: TEdit
            Left = 128
            Top = 303
            Width = 65
            Height = 21
            Color = 14670284
            ReadOnly = True
            TabOrder = 10
            Text = '0'
          end
        end
        object GroupBox3: TGroupBox
          Left = 264
          Top = 3
          Width = 441
          Height = 374
          Caption = 'ART currently information'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label23: TLabel
            Left = 12
            Top = 16
            Width = 176
            Height = 16
            Caption = 'Loader should  input amount : '
          end
          object Label24: TLabel
            Left = 12
            Top = 69
            Width = 132
            Height = 16
            Caption = 'Auto1 output amount :  '
          end
          object Label25: TLabel
            Left = 12
            Top = 96
            Width = 129
            Height = 16
            Caption = 'Auto2 output amount : '
          end
          object Label26: TLabel
            Left = 12
            Top = 123
            Width = 129
            Height = 16
            Caption = 'Auto3 output amount:  '
          end
          object Label22: TLabel
            Left = 12
            Top = 43
            Width = 182
            Height = 16
            Caption = 'Loader already  input amount : '
          end
          object Label27: TLabel
            Left = 12
            Top = 176
            Width = 129
            Height = 16
            Caption = 'Currently ART Count : '
          end
          object Label28: TLabel
            Left = 12
            Top = 149
            Width = 91
            Height = 16
            Caption = 'ART max limit : '
          end
          object Label30: TLabel
            Left = 12
            Top = 234
            Width = 115
            Height = 16
            Caption = 'ART Pass  % limit : '
          end
          object Label46: TLabel
            Left = 284
            Top = 149
            Width = 80
            Height = 16
            Caption = 'Least Times :'
          end
          object edInputAmount: TEdit
            Left = 216
            Top = 12
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 0
            Text = '0'
          end
          object edLoadingCnt: TEdit
            Left = 216
            Top = 39
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 1
            Text = '0'
          end
          object edAuto1Cnt: TEdit
            Left = 216
            Top = 65
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 2
            Text = '0'
          end
          object edAuto2Cnt: TEdit
            Left = 216
            Top = 92
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 3
            Text = '0'
          end
          object edAuto3Cnt: TEdit
            Left = 216
            Top = 119
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 4
            Text = '0'
          end
          object edARTMaxLimit: TEdit
            Left = 216
            Top = 145
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 5
            Text = '0'
          end
          object edCurrARTCnt: TEdit
            Left = 216
            Top = 172
            Width = 65
            Height = 24
            ReadOnly = True
            TabOrder = 7
            Text = '0'
          end
          object edARTPassLimit: TEdit
            Left = 216
            Top = 230
            Width = 65
            Height = 24
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 9
            Text = '0'
          end
          object Edit16: TEdit
            Left = 368
            Top = 145
            Width = 65
            Height = 24
            ImeMode = imClose
            ReadOnly = True
            TabOrder = 6
            Text = '0'
          end
          object palCurrFailRate: TPanel
            Left = 8
            Top = 202
            Width = 285
            Height = 29
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 8
            object Label47: TLabel
              Left = 4
              Top = 2
              Width = 101
              Height = 16
              Caption = 'Currently Fail  %: '
            end
            object edCurrARTRate: TEdit
              Left = 208
              Top = -2
              Width = 65
              Height = 24
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
          end
          object palFTRateLmt: TPanel
            Left = 8
            Top = 256
            Width = 361
            Height = 29
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 10
            object Label29: TLabel
              Left = 4
              Top = 7
              Width = 95
              Height = 16
              Caption = 'FT  Fail %  limit: '
            end
            object Edit10: TEdit
              Left = 136
              Top = 3
              Width = 65
              Height = 24
              Color = 14670284
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object Edit11: TEdit
              Left = 208
              Top = 3
              Width = 65
              Height = 24
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object Edit12: TEdit
              Left = 282
              Top = 3
              Width = 65
              Height = 24
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 2
              Text = '0'
            end
          end
          object palRTRateLmt: TPanel
            Left = 8
            Top = 288
            Width = 353
            Height = 33
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 11
            object Label45: TLabel
              Left = 4
              Top = 8
              Width = 97
              Height = 16
              Caption = 'RT  Fail %  limit: '
            end
            object Edit13: TEdit
              Left = 136
              Top = 4
              Width = 65
              Height = 24
              Color = 14670284
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object Edit14: TEdit
              Left = 208
              Top = 4
              Width = 65
              Height = 24
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object Edit15: TEdit
              Left = 282
              Top = 4
              Width = 65
              Height = 24
              ImeMode = imClose
              ReadOnly = True
              TabOrder = 2
              Text = '0'
            end
          end
          object Panel6: TPanel
            Left = 3
            Top = 324
            Width = 430
            Height = 165
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 12
            object sgATRCount: TStringGrid
              Left = 0
              Top = 0
              Width = 216
              Height = 165
              Align = alLeft
              Color = 14670284
              ColCount = 3
              DefaultColWidth = 70
              DefaultRowHeight = 25
              FixedColor = 12761254
              RowCount = 4
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              ColWidths = (
                70
                70
                70)
            end
          end
        end
      end
    end
    object tsOCRBarCode: TTabSheet
      Caption = 'OCRBarCode'
      ImageIndex = 13
      object Memo2: TMemo
        Left = 0
        Top = 25
        Width = 193
        Height = 508
        Align = alLeft
        Color = 14670284
        TabOrder = 1
      end
      object Panel9: TPanel
        Left = 389
        Top = 25
        Width = 261
        Height = 508
        Align = alRight
        Color = 12761254
        TabOrder = 2
        object spOCRSaveLog: TSpeedButton
          Left = 3
          Top = 171
          Width = 94
          Height = 25
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Save Log'
          OnClick = spOCRSaveLogClick
        end
        object Label40: TLabel
          Left = 4
          Top = 68
          Width = 104
          Height = 16
          Caption = 'OCR Setup File'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object spOCRChangeFile: TSpeedButton
          Left = 4
          Top = 111
          Width = 102
          Height = 25
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Change OCR File'
          OnClick = spOCRChangeFileClick
        end
        object Label41: TLabel
          Left = 4
          Top = 4
          Width = 48
          Height = 16
          Caption = 'Label41'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label42: TLabel
          Left = 4
          Top = 144
          Width = 60
          Height = 20
          Caption = 'Lot ID'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -17
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object spOCRLogIn: TSpeedButton
          Left = 144
          Top = 3
          Width = 62
          Height = 25
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Log   In'
          OnClick = spOCRLogInClick
        end
        object Label36: TLabel
          Left = 36
          Top = 215
          Width = 81
          Height = 25
          Caption = 'Key In '#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label37: TLabel
          Left = 36
          Top = 270
          Width = 64
          Height = 25
          Caption = 'Total'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label38: TLabel
          Left = 36
          Top = 188
          Width = 64
          Height = 25
          Caption = 'Pass'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label39: TLabel
          Left = 36
          Top = 243
          Width = 70
          Height = 25
          Caption = 'No IC'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -21
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object Label43: TLabel
          Left = 4
          Top = 28
          Width = 94
          Height = 16
          Caption = 'OCR Use File'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbOCRNowFile: TLabel
          Left = 3
          Top = 49
          Width = 37
          Height = 16
          Caption = 'Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbOCRUseFile: TLabel
          Left = 3
          Top = 89
          Width = 37
          Height = 16
          Caption = 'Name'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object spOCRCleanList: TSpeedButton
          Left = 131
          Top = 111
          Width = 94
          Height = 25
          AllowAllUp = True
          GroupIndex = 1
          Caption = 'Clean List'
          OnClick = spOCRCleanListClick
        end
        object edOCRLotID: TEdit
          Left = 68
          Top = 144
          Width = 149
          Height = 21
          Color = 14670284
          TabOrder = 1
        end
        object sgOCR: TStringGrid
          Left = 1
          Top = 363
          Width = 259
          Height = 144
          Align = alBottom
          Color = 14670284
          ColCount = 3
          DefaultColWidth = 70
          DefaultRowHeight = 25
          FixedColor = 12761254
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = #27161#26999#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object Memo3: TMemo
          Left = 152
          Top = 48
          Width = 57
          Height = 65
          Color = 14670284
          Lines.Strings = (
            'Memo3')
          TabOrder = 0
          Visible = False
        end
      end
      object Panel19: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'OCR Barcode'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsESDMonitor: TTabSheet
      Caption = 'ESD Monitor'
      ImageIndex = 14
      object pnlESDMonitor: TPanel
        Left = 0
        Top = 25
        Width = 539
        Height = 516
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object lbESDReportData: TLabel
          Left = 12
          Top = 108
          Width = 104
          Height = 16
          Caption = 'ESD Report Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbESDDecayReportData: TLabel
          Left = 224
          Top = 108
          Width = 147
          Height = 16
          Caption = 'ESD Decay Report Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object pnl2: TPanel
          Left = 65
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnl3: TPanel
          Left = 119
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnl4: TPanel
          Left = 173
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pl_ESDProx1_1: TPanel
          Left = 65
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pl_ESDProx2_1: TPanel
          Left = 119
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pl_ESDProx1_2: TPanel
          Left = 65
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 15
        end
        object pl_ESDProx2_2: TPanel
          Left = 119
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 16
        end
        object pl_ESDProx1_3: TPanel
          Left = 65
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 22
        end
        object pl_ESDProx2_3: TPanel
          Left = 119
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 23
        end
        object pl_ESDProx3_1: TPanel
          Left = 173
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object pl_ESDProx3_2: TPanel
          Left = 173
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 17
        end
        object pl_ESDProx3_3: TPanel
          Left = 173
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 24
        end
        object pnl14: TPanel
          Left = 11
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 21
        end
        object pnl15: TPanel
          Left = 11
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 14
        end
        object pnl16: TPanel
          Left = 11
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pl_PositiveDecay3_3: TPanel
          Left = 173
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 52
        end
        object pl_PositiveDecay3_2: TPanel
          Left = 173
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 45
        end
        object pl_PositiveDecay3_1: TPanel
          Left = 173
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 38
        end
        object pl4: TPanel
          Left = 11
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 49
        end
        object pl5: TPanel
          Left = 11
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 42
        end
        object pl6: TPanel
          Left = 11
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 35
        end
        object pl7: TPanel
          Left = 65
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 29
        end
        object pl_PositiveDecay1_1: TPanel
          Left = 65
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 36
        end
        object pl_PositiveDecay2_1: TPanel
          Left = 119
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 37
        end
        object pl_PositiveDecay2_2: TPanel
          Left = 119
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 44
        end
        object pl_PositiveDecay1_2: TPanel
          Left = 65
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 43
        end
        object pl12: TPanel
          Left = 119
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 30
        end
        object pl13: TPanel
          Left = 173
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 31
        end
        object pl_PositiveDecay1_3: TPanel
          Left = 65
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 50
        end
        object pl_PositiveDecay2_3: TPanel
          Left = 119
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 51
        end
        object pl_NegativeDecay3_3: TPanel
          Left = 173
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 80
        end
        object pl_NegativeDecay3_2: TPanel
          Left = 173
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 73
        end
        object pl_NegativeDecay3_1: TPanel
          Left = 173
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 66
        end
        object pl19: TPanel
          Left = 11
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 77
        end
        object pl20: TPanel
          Left = 11
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 70
        end
        object pl21: TPanel
          Left = 11
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Prox 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 63
        end
        object pl22: TPanel
          Left = 65
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 1'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 57
        end
        object pl_NegativeDecay1_1: TPanel
          Left = 65
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 64
        end
        object pl_NegativeDecay2_1: TPanel
          Left = 119
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 65
        end
        object pl_NegativeDecay2_2: TPanel
          Left = 119
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 72
        end
        object pl_NegativeDecay1_2: TPanel
          Left = 65
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 71
        end
        object pl27: TPanel
          Left = 119
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 2'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 58
        end
        object pl28: TPanel
          Left = 173
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 3'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 59
        end
        object pl_NegativeDecay1_3: TPanel
          Left = 65
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 78
        end
        object pl_NegativeDecay2_3: TPanel
          Left = 119
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 79
        end
        object pl31: TPanel
          Left = 11
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'VOLTS'
          Color = clGray
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pl32: TPanel
          Left = 11
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = ' DECAY(+)'
          Color = clGray
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 28
        end
        object pl34: TPanel
          Left = 11
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = ' DECAY(-)'
          Color = clGray
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 56
        end
        object pnl5: TPanel
          Left = 226
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 4'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pl_ESDProx4_1: TPanel
          Left = 226
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object pl_ESDProx4_2: TPanel
          Left = 226
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 18
        end
        object pl_ESDProx4_3: TPanel
          Left = 226
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 25
        end
        object pnl9: TPanel
          Left = 280
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 5'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pl_ESDProx5_1: TPanel
          Left = 280
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
        object pl_ESDProx5_2: TPanel
          Left = 280
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 19
        end
        object pl_ESDProx5_3: TPanel
          Left = 280
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 26
        end
        object pnl13: TPanel
          Left = 334
          Top = 8
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 6'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pl_ESDProx6_1: TPanel
          Left = 334
          Top = 31
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
        object pl_ESDProx6_2: TPanel
          Left = 334
          Top = 54
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 20
        end
        object pl_ESDProx6_3: TPanel
          Left = 334
          Top = 77
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 27
        end
        object pnl20: TPanel
          Left = 226
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 4'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 32
        end
        object pl_PositiveDecay4_1: TPanel
          Left = 226
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 39
        end
        object pl_PositiveDecay4_2: TPanel
          Left = 226
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 46
        end
        object pl_PositiveDecay4_3: TPanel
          Left = 226
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 53
        end
        object pnl24: TPanel
          Left = 280
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 5'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 33
        end
        object pl_PositiveDecay5_1: TPanel
          Left = 280
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 40
        end
        object pl_PositiveDecay5_2: TPanel
          Left = 280
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 47
        end
        object pl_PositiveDecay5_3: TPanel
          Left = 280
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 54
        end
        object pnl28: TPanel
          Left = 334
          Top = 128
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 6'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 34
        end
        object pl_PositiveDecay6_1: TPanel
          Left = 334
          Top = 151
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 41
        end
        object pl_PositiveDecay6_2: TPanel
          Left = 334
          Top = 174
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 48
        end
        object pl_PositiveDecay6_3: TPanel
          Left = 334
          Top = 197
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 55
        end
        object pnl32: TPanel
          Left = 226
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 4'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 60
        end
        object pl_NegativeDecay4_1: TPanel
          Left = 226
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 67
        end
        object pl_NegativeDecay4_2: TPanel
          Left = 226
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 74
        end
        object pl_NegativeDecay4_3: TPanel
          Left = 226
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 15313978
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 81
        end
        object pnl36: TPanel
          Left = 280
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 5'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 61
        end
        object pl_NegativeDecay5_1: TPanel
          Left = 280
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 68
        end
        object pl_NegativeDecay5_2: TPanel
          Left = 280
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 75
        end
        object pl_NegativeDecay5_3: TPanel
          Left = 280
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = 2468626
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 82
        end
        object pnl40: TPanel
          Left = 334
          Top = 224
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = 'Station 6'
          Color = 10801371
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 62
        end
        object pl_NegativeDecay6_1: TPanel
          Left = 334
          Top = 247
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 69
        end
        object pl_NegativeDecay6_2: TPanel
          Left = 334
          Top = 270
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 76
        end
        object pl_NegativeDecay6_3: TPanel
          Left = 334
          Top = 293
          Width = 55
          Height = 25
          BevelInner = bvLowered
          Caption = '0.0'
          Color = clAqua
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 83
        end
      end
      object pnl1: TPanel
        Left = 0
        Top = 0
        Width = 539
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'ESD Monitor'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsASEMARMS: TTabSheet
      Caption = 'ARMS'
      ImageIndex = 15
      object pnARMSTitle: TPanel
        Left = 0
        Top = 0
        Width = 539
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'RUN SERVER'
        Color = 4180834
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object Panel20: TPanel
        Left = 0
        Top = 25
        Width = 539
        Height = 516
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object sbARMSShow: TSpeedButton
          Left = 8
          Top = 12
          Width = 105
          Height = 33
          Caption = 'ARMS Setup'
          OnClick = sbARMSShowClick
        end
      end
    end
    object tsASECLEventLog: TTabSheet
      Caption = 'Event Log'
      ImageIndex = 16
      object Panel10: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 508
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object ScrollBox1: TScrollBox
          Left = 0
          Top = 177
          Width = 650
          Height = 290
          Align = alClient
          Color = 12761254
          ParentColor = False
          TabOrder = 1
          object lbSocketIDRowA: TLabel
            Left = 8
            Top = 31
            Width = 110
            Height = 46
            AutoSize = False
            Caption = 'Socket A'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDRowB: TLabel
            Left = 8
            Top = 90
            Width = 110
            Height = 51
            AutoSize = False
            Caption = 'Socket B'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDRowC: TLabel
            Left = 8
            Top = 148
            Width = 110
            Height = 53
            AutoSize = False
            Caption = 'Socket C'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDRowD: TLabel
            Left = 8
            Top = 207
            Width = 110
            Height = 54
            AutoSize = False
            Caption = 'Socket D'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColA: TLabel
            Left = 124
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'a'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColB: TLabel
            Left = 189
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'b'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColC: TLabel
            Left = 254
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'c'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColD: TLabel
            Left = 319
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'd'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColE: TLabel
            Left = 385
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'e'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColF: TLabel
            Left = 450
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'f'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColG: TLabel
            Left = 515
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'g'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lbSocketIDColH: TLabel
            Left = 580
            Top = 3
            Width = 57
            Height = 24
            Alignment = taCenter
            AutoSize = False
            Caption = 'h'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object edtSocketAa: TEdit
            Left = 123
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
            Text = 'ASE001999'
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAb: TEdit
            Left = 188
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAc: TEdit
            Left = 253
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAd: TEdit
            Left = 318
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAe: TEdit
            Left = 383
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAf: TEdit
            Left = 448
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAg: TEdit
            Left = 513
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketAh: TEdit
            Left = 578
            Top = 58
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBa: TEdit
            Left = 123
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 24
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBb: TEdit
            Left = 188
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 25
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBc: TEdit
            Left = 253
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 26
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBd: TEdit
            Left = 318
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 27
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBe: TEdit
            Left = 383
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 28
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBf: TEdit
            Left = 448
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 29
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBg: TEdit
            Left = 513
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 30
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketBh: TEdit
            Left = 578
            Top = 118
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 31
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCa: TEdit
            Left = 123
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 40
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCb: TEdit
            Left = 188
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 41
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCc: TEdit
            Left = 253
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 42
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCd: TEdit
            Left = 318
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 43
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCe: TEdit
            Left = 383
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 44
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCf: TEdit
            Left = 448
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 45
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCg: TEdit
            Left = 513
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 46
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketCh: TEdit
            Left = 578
            Top = 178
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 47
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDa: TEdit
            Left = 123
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 56
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDb: TEdit
            Left = 188
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 57
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDc: TEdit
            Left = 253
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 58
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDd: TEdit
            Left = 318
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 59
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDe: TEdit
            Left = 383
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 60
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDf: TEdit
            Left = 448
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 61
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDg: TEdit
            Left = 513
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 62
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtSocketDh: TEdit
            Left = 578
            Top = 238
            Width = 60
            Height = 20
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 63
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object palAa: TPanel
            Left = 123
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object palAb: TPanel
            Left = 188
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object palAc: TPanel
            Left = 253
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object palAd: TPanel
            Left = 318
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object palAg: TPanel
            Left = 513
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object palAh: TPanel
            Left = 578
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object palAf: TPanel
            Left = 448
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object palAe: TPanel
            Left = 383
            Top = 31
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object palBa: TPanel
            Left = 123
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 16
          end
          object palBb: TPanel
            Left = 188
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 17
          end
          object palBc: TPanel
            Left = 253
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 18
          end
          object palBd: TPanel
            Left = 318
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 19
          end
          object palBg: TPanel
            Left = 513
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 22
          end
          object palBh: TPanel
            Left = 578
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 23
          end
          object palBf: TPanel
            Left = 448
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 21
          end
          object palBe: TPanel
            Left = 383
            Top = 90
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 20
          end
          object palCa: TPanel
            Left = 123
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 32
          end
          object palCb: TPanel
            Left = 188
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 33
          end
          object palCc: TPanel
            Left = 253
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 34
          end
          object palCd: TPanel
            Left = 318
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 35
          end
          object palCg: TPanel
            Left = 513
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 38
          end
          object palCh: TPanel
            Left = 578
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 39
          end
          object palCf: TPanel
            Left = 448
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 37
          end
          object palCe: TPanel
            Left = 383
            Top = 148
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 36
          end
          object palDa: TPanel
            Left = 123
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 48
          end
          object palDb: TPanel
            Left = 188
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 49
          end
          object palDc: TPanel
            Left = 253
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 50
          end
          object palDd: TPanel
            Left = 318
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 51
          end
          object palDg: TPanel
            Left = 513
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 54
          end
          object palDh: TPanel
            Left = 578
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 55
          end
          object palDf: TPanel
            Left = 448
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 53
          end
          object palDe: TPanel
            Left = 383
            Top = 207
            Width = 60
            Height = 25
            Color = clSilver
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -9
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 52
          end
        end
        object pnlLotStart_ASECL: TPanel
          Left = 0
          Top = 467
          Width = 650
          Height = 41
          Align = alBottom
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 2
          object btnASECL_LotStart: TSpeedButton
            Left = 6
            Top = 4
            Width = 120
            Height = 36
            AllowAllUp = True
            GroupIndex = 1
            Caption = 'Lot Start'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            Glyph.Data = {
              360C0000424D360C000000000000360000002800000020000000200000000100
              180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
              FFFFFFFEFEFEFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFD
              FDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFEFEFEFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
              F8F8F8E8E8E8DADADAD9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9
              D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D9D8D8D8D9DAD9E8E8E8F8F8F8FEFEFEFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8
              DADAD9F4F4F4F7F7F7F7F7F7F8F8F8F9F9F8F9F9F9FAFAFAFAFAFAFAFAFAFBFB
              FBFBFBFBFCFCFCFCFCFCFDFDFDF8FBF8E9F3E9E6F2E6F2F7F2DADBDAF8F8F8FF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3F3F3
              F3F3F3F3F3F2F5F5F5F7F7F7F8F8F8F8F8F8F8F8F8F9F9F9FAFAFAFAFAFAFBFB
              FBFBFBFBFBFBFBFBFBFBFBFCFBE6F1E664AF64429A42B9DCB9E6F1E6F3F3F3FF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F4F4F4F2F2F13CB5FD00A7FF00B1FF00BAFF00C3FF00CBFF00D3FF00DAFF00DD
              FF00DAFF00D3FF00CBFF02C2FD0AB7DD0B8031056A0C0D6A107FBD81EFF0EFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F4F4F4F1F1F100A0FF00A4FF00ADFF00B5FF00BDFF00C4FF00CBFF00CFFF00D1
              FF00CFFF00CBFF00C4FF00BDFD10B8DC1B8C4A249B4927AB4A0B6A1476AD7BFE
              FFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F4F4F3F0F0F000A0FF00A0FF00A7FF00AEFF00B5FF00BBFF00C0FF00C3FF00C4
              FF00C3FF00C0FF00BBFF00B5FD07AFDA127A3C2192472DC05A23B24410801F47
              9E54E4F2E6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F3F3F2F0F0EF00A0FF00A0FF00A0FF00A6FF00ACFF00B1FF00B5FF00B7FF00B8
              FF00B7FF00B5FF00B1FF00ACFD04A6DB096025187D3125B64A23B2441AA8320F
              941F269B3AB3DDBDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F2F2F1EFEFEE00A0FF00A0FF00A0FF00A0FF00A1FF00A6FF00A9FF00ABFF00AC
              FF00ABFF00A9FF00A6FF00A1FD05A1DC09561E116A1F1BAB331AA93418A62D12
              9D1F089410189D2E7ACC91F6FCF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F1F1F1EEEEED00A0FF00A0FF00A0FF00A0FF00A0FF00A0FF00A0FF00A0FF00A0
              FF00A0FF00A0FF00A0FF00A0FD05A1DC09541D12681D1CA52C18A328139E200E
              991807930D018A0212962252C879E9F9EFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F1F1F0EDEDEC00A0FF00A0FF03A1FF09A3FF0DA5FF10A6FF14A7FF17A9FF17A9
              FF14A7FF10A6FF0DA5FF09A3FD08A3DE08571E23802C3AB34539B2433BB14236
              AD3C2AA42F239B261CA3336FD991F3FBF6FFFFFFFFFFFFFFFFFFFFFFFFF1F1F1
              F1F1F0ECECEB33B3FF3EB7FF3FB7FF40B8FF40B8FF3FB7FF3FB7FF40B8FF40B8
              FF3FB7FF3FB7FF3FB7FF3FB7FD39B5E00B63202C8D3349BB564ABB5648BA5447
              B95247B7513ABC588ED1A3D9DAD9E8E8E8F8F8F8FEFEFEFFFFFFFFFFFFF1F1F1
              EFEFEEEBEBEA50BEFF52BFFF51BEFF52BFFF52BFFF52BFFF52BFFF51BEFF52BF
              FF51BEFF51BEFF52BFFF51BFFE4ABDE10F7126389F445CCA6F5CCA6E5EC9704D
              BD5D4BC469D4F1DCFDFDFDFDFDFDFBFBFBDCDCDCF8F8F8FFFFFFFFFFFFF1F1F1
              EEEEEEEAEAE961C4FF63C5FF64C5FF63C5FF64C5FF64C5FF64C5FF64C5FF63C5
              FF63C5FF63C5FF63C5FF63C5FE59C3E315822E4DB25E74DB8F71D5883FB35467
              C27CF2F9F4FCFCFCFCFCFCFCFCFCFCFCFCFAFAFAF3F3F3FFFFFFFFFFFFF1F1F1
              EEEEEDE9E8E774CBFF76CCFF76CCFF76CCFF76CCFF76CCFF76CCFF76CCFF76CC
              FF76CCFF76CCFF76CCFF75CCFE6BCAE51F983A6DC68173D38D2CA541669B70B3
              B4B3C7C8C7BABCBBADAFAEB6B7B7FBFBFBFCFCFCF1F1F1FFFFFFFFFFFFF1F1F1
              EDECEBE8E7E66B9DBA85ABC285ABC285ABC285ABC285ABC285ABC285ABC285AB
              C285ABC285ABC285ABC284ACC184B4BB48B15D25A2342DAE43A2DDB2868787AC
              ACACBFC1C0B4B5B5A8AAA99B9E9DFAFAFAFBFBFBF1F1F1FFFFFFFFFFFFF1F1F1
              EBEBEBE6E6E5E7E7E6E8E7E6E8E8E7E9E9E8EAE9E8EAEAE9EBEBEAEBEBEAECEC
              EBECECEBECECECEDEDECECECEBE5EDE7B9E3C184D093C6E6D0E5F0E8808181A2
              A4A3B4B6B5ABADACA0A2A1949796F9F9F9FAFAFAF1F1F1FFFFFFFFFFFFF1F1F1
              EBEAE9E5E4E3E6E5E4E6E6E5E7E6E5E8E7E6E8E8E7E9E9E8E9E9E8EAEAE9EAEA
              E9EBEBEAEBEBEAECECEBECECEBEBECEAE7ECE8E3ECE5E8ECE9F0F1F17D7D7D99
              9B9AA8AAA99FA1A09598978B8E8CF8F8F8FAFAFAF1F1F1FFFFFFFFFFFFF4F4F4
              E9E8E7E4E3E2E4E4E3E5E4E3E6E5E4E6E6E5E7E6E5E7E7E6E8E8E7E9E8E7E9E9
              E8EAE9E8EAEAE9EAEAE9EFEFEEEFEFEEEAEAE9EAEBE9ECECEBEEEEED7B7D7C8F
              9190999C9B929493898C8B808381F7F7F7F9F9F9F1F1F1FFFFFFFFFFFFFBFBFB
              E5E5E5E9E8E7EAE9E8EBEAE9EBEBEAEBEBEAECEBEBECECEBEDECEBEDEDECEEED
              ECEEEEEDEEEEEDF2F2F1646665686868F2F2F2EFEFEEEDEDECB2B3B37B7D7C87
              89888A8D8C8487857C7F7E747775F6F6F6F8F8F8F1F1F1FFFFFFFFFFFFFFFFFF
              FBFBFBF6F6F6F4F4F4F3F3F3F3F3F3F3F3F3F3F3F3E4E4E4A0A09F9D9D9C4548
              464B4D4C5456555D5F5E6769686C6E6C6C6D6C6D6F6E717473787A797F828182
              8483808382777A786E7170666A68F5F5F4F7F7F7F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0F0F0D0D0CFCDCDCC676A
              69737474797B797E8180848685878989878A89878A898B8D8B8E908F90939291
              94939092908C8E8D878A887A7C7AF4F4F3F7F7F6F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1E6E6E5E2E2E18587
              86868888858887888A898C8F8D8F91908F929190929192949392949491949391
              93929092908C8E8D8A8C8B8A8C8BF2F2F2F6F6F5F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1EEEEEEEAEAE99497
              9696999896989796989796999896989796999896999896999896999896999896
              9998969998969998969897949796F1F1F1F4F4F4F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1EEEDECE9E8E7A1A3
              A2A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3
              A5A4A3A5A4A3A5A4A3A5A4A1A3A2F0F0F0F4F4F4F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1EDECEBE8E7E69394
              94A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3A5A4A3
              A5A4A3A5A4A3A5A4A3A5A4949594EFEFEEF3F3F2F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1ECEBEBE6E6E5E7E7
              E6E8E7E6E8E8E7E9E9E8EAE9E8EAEAE9EBEBEAEBEBEAECECEBECECEBECECECED
              EDECEDEDECEDEDECEEEEEDEEEEEDEEEEEDF2F2F1F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F1F1EBEBEAE5E4E3E6E5
              E4E6E6E5E7E6E5E8E7E6E8E8E7E9E8E7E9E9E8EAEAE9EAEAE9EBEBEAEBEBEAEC
              ECEBECECEBECECEBECECEBEDEDECEDEDECF1F1F1F1F1F1FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4F4F4E9E8E7E4E3E2E4E4
              E3E5E4E3E6E5E4E6E6E5E7E6E5E8E7E6E8E8E7E8E8E7E9E9E8EAE9E8EAEAE9EA
              EAE9EFEFEEEFEFEEEBEBEAECECEBECECEBEFEFEEF4F4F4FFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBE5E5E5E9E8E7EAE9
              E8EBEAE9EBEBEAEBEBEAECEBEBECECEBEDECEBEDEDECEEEDECEEEEEDEEEEEDF2
              F1F1EEEEEEEEEEEEF2F2F2EFEFEEEEEEEDE7E7E7FBFBFBFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF6F6F6F4F4
              F4F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F4F4F4F5
              F5F5FEFEFEFEFEFEF5F5F5F4F4F4F6F6F6FBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
            ParentFont = False
            OnClick = btnASECL_LotStartClick
          end
          object btnASECL_LotEnd: TSpeedButton
            Left = 164
            Top = 4
            Width = 120
            Height = 36
            AllowAllUp = True
            GroupIndex = 1
            Caption = 'Lot End'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            Glyph.Data = {
              360C0000424D360C000000000000360000002800000020000000200000000100
              180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFAFAFAEFEFEFE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
              E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9EAEAEAF2F2F2FBFBFBFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFEFEFEFCECECEBDBDBDBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
              BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCC0C0C0D4D4D4F2F2F2FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFE9E9E9AF9E90AB9787AA9584AB9583AB9583AA9584A99584A99585A99585A9
              9585A99585A99585A99585A99585A99585AA9787B3A69CEAEAEAFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFE9E9E9AB9787FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAA9787E9E9E9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFE9E9E9AA9584FFFFFF27D2FF32D3FF32D3FF26D2FFFFFFFFBEA07FBEA383BE
              A384BEA384BEA384BEA384BEA383BCA080FFFFFFA89584E9E9E9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2EAEAEAE9E9E9E9E9E9E9E9E9E9E9
              E9D8D8D8AB9583FFFFFF27C2FF36C5FF36C6FF26C2FFFFFFFBFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA89484E9E9E9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFF2F2F2D4D4D4C0C0C0BCBCBCBCBCBCBCBCBCBCBC
              BCB7B7B7AB9583FFFFFF20B4FF31B9FF31B9FF20B5FFFFFEF8BFA180BEA385BF
              A485BFA485BFA485BFA485BEA384BCA081FFFFFFA89584E9E9E9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFEAEAEAB3A69CAA9787A89584A89483A89483A895
              84A99585A99280FFFFFF13A4FF24A9FF24AAFF14A4FFFFFBF4FBFAFCF9FCFFFA
              FCFFFAFCFFFAFCFFFAFCFFF9FCFFF7F9FDFFFFFFA89584E9E9E9FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9AA9787FFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFA58E7CFFFFFF008FFF0A94FF0A94FF0090FFFFF7F3C0A282BFA486BF
              A587BFA587BFA587BFA587BFA486BDA182FFFFFFA89584E9E9E9FAFAFAEFEFEF
              E9E9E9E9E9E9E9E9E9E9E9E9D8D8D8A89584FFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFA38D7CFFFFFFFBF4F3FFF8F6FFF8F6FCF7F7F3F4FAF0F5FEF0F6FFF0
              F6FFF0F6FFF0F6FFF0F6FFEFF5FFEEF3FAFFFFFFA89584E9E9E9EFEFEFCECECE
              BDBDBDBCBCBCBCBCBCBCBCBCB7B7B7A89484FFFFFFBB9F7FBDA182BDA283BEA3
              83C1A687A38F7FFFFFFFEAAE59ECB361EDB463ECB463EBB364EAB365EAB465EA
              B465EAB465EAB465EAB465EAB363E8AE5AFFFFFFA89585E9E9E9E9E9E9AF9E90
              AA9787A89586A89586A99686A99687A69281FFFFFFFDFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFA38F7FFFFFFFD39A46D5A051D6A154D6A154D6A154D6A154D6A154D6
              A154D6A154D6A154D6A154D5A051D39A46FFFFFFA89586E9E9E9E9E9E9AA9787
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA39081FFFFFFBCA081BEA385BEA386BFA4
              86C2A788A39080FFFFFFBE7E25C0832FC18531C18531C18531C18531C18531C1
              8531C18531C18531C18531C0832FBE7E25FFFFFFA89586EAEAEAE9E9E9A89586
              FFFFFFC98B34CE9440D39A47D99F47A39186FFFFFFF6FBFFF8FDFFF8FEFFF9FE
              FFFCFFFFA39081FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAA9787F2F2F2E9E9E9A89586
              FFFFFFCA903DD09B4DD59F53DBA351A4948AFFFFFFEBB15CECB565EDB565EDB3
              5FF0F6FEBF9F77A4958BA4958BA4958BA4958BA4958BA4948BA39388A39185A3
              9388A4948BA4948AA39287A39182A79383AA9787C3B6ACFBFBFBE9E9E9A89586
              FFFFFFCA923ED19C51D5A155DAA352A4958CFFFFFFDCA351DEA95CDEA95DDDA5
              53F3FDFFE0A652E2AB5BE3AD5CE3AD5CE3AD5CE3AD5CE3AB5BE2A853F7FFFFE2
              A853E3AB5BE3AB5AE1A751FFFFFFA99687E9E9E9FFFFFFFFFFFFE9E9E9A89586
              FFFFFFCA913DD09C51D5A156D9A250A5958CFFFFFFCE943FD19B4CD19B4DCF96
              42F1FBFFD09642D29C4CD29D4FD29D4FD29D4FD29D4FD29C4CD09642F2FBFFD0
              9642D29C4CD29B4CCF9540FFFFFFA99686E9E9E9FFFFFFFFFFFFE9E9E9A89686
              FFFFFFC98E3CCF9A4FD5A157D9A453A5958BFFFFFFBE7F24C1842FC2852FC080
              27F1F6FFC08027C2852FC28631C28631C28631C28631C2852FC08027F1F6FFC0
              8027C2852FC1842FBE7F24FFFFFFA89586E9E9E9FFFFFFFFFFFFE9E9E9A89686
              FFFFFFC78D3BCE994DD39F54D8A354A5948AFFFFFFF4F0F7F9F3FEFAF4FEF9F2
              FBF9F0F7F9F2FBFAF4FEFAF4FFFAF4FFFAF4FFFAF4FFFAF4FEF9F2FBF9F0F7F9
              F2FBFAF4FEF9F3FEF4F0F7FFFFFFA99585E9E9E9FFFFFFFFFFFFE9E9E9A89686
              FFFFFFC68C38CD974BD29D52D7A051A6958BFFFFFF42B9944BBD9A4DBE9B4DBD
              9A4DBD9A4DBD9A4DBE9B4DBE9B4DBE9B4DBE9B4DBE9B4DBE9B4DBD9A4DBD9A4D
              BD9A4DBE9B4BBD9A42B994FFFFFFAA9585E9E9E9FFFFFFFFFFFFE9E9E9A89686
              FFFFFFC58B37CB9649D09B4FD59E4EA6958CFFFFFF34A46D3DA8723FA8743FA8
              743FA8743FA8743FA8743FA8743FA8743FA8743FA8743FA8743FA8743FA8743F
              A8743FA8743DA87234A46DFFFFFFAA9585EAEAEAFFFFFFFFFFFFE9E9E9A89686
              FFFFFFC48A35CA9447CD994DD49C4DA7978FFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFAB9787F2F2F2FFFFFFFFFFFFE9E9E9A89686
              FFFFFFC28834C89345CC974AD09B4CB89A79A7978FA6958BA7958AA7948AA694
              8AA6948AA69489A69389A69389A69389A69389A69389A69389A69389A79489A7
              9389A69286A69081A89382AA9687C3B6ACFBFBFBFFFFFFFFFFFFE9E9E9A99686
              FFFFFFC08532C79143CA9549CD984CD19B4DD39E4FD7A152D9A357DAA75ADBA9
              5DDEAA60DDAC63DEAD65DEAE66DEAE67DEAE68DDAE69DCAE69DBAD69DAAC67D7
              A964D3A258FFFFFFA99687E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9A99686
              FFFFFFBF8330C58F41C89347CA954ACD994ECF9C51D19F55D4A158D6A45CD7A6
              60D8A861D9A963D9AA65DAAA68D9AB69D9AB6AD9AB6BD8AB6BD7AB6CD6AA6BD3
              A766CF9E57FFFFFFA99686E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9A99686
              FFFFFFBD812DC38D3FC69145C89347CB964BCD994FCE9C52D19F56D2A159D3A2
              5CD5A45ED5A661D6A763D6A864D6A865D6A967D5A968D5A968D4A968D3A868D1
              A463CC9C57FFFFFFA89586E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9A99686
              FFFFFFBA7F2AC18B3DC48F42C69145C89347CA964BCC994FCE9B52D09E55D1A0
              58D2A15BD3A35DD3A45FD3A561D4A562D3A663D3A664D3A665D2A666D2A666CF
              A263CA9A53FFFFFFA89586E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9A99686
              FFFFFFBB7F2ABE893BC28D40C48F42C69044C89348CA964BCC984ECD9A52CE9C
              55CF9E57D0A059D1A15CD1A25DD1A25FD1A360D1A361D1A362D0A362CFA363CD
              A05FC99952FFFFFFA89586E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFE9E9E9A99686
              FFFFFFBA7E28BE8637BE8839C18A3CC38C3EC48E41C69145C89348C9954BCB97
              4ECC9950CD9B53CD9C55CD9D57CE9D58CE9E59CE9E5ACD9F5BCD9E5CCC9E5DCB
              9D5BC6954EFFFFFFA89586E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFEAEAEAA99686
              FFFFFFB7781DBA7E28BB7F2ABA7F2ABB802CBE822EC08532C18835C38A38C48C
              3CC58F3FC69041C89143C89246C79347C79449C7944AC8954BC8954CC6954DC6
              944CC48F44FFFFFFA89586EAEAEAFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2AA9787
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFAA9787F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFBFBFBC3B6AC
              AA9787A99686A99686A99686A99686A99686A99686A99686A99686A89686A896
              86A89686A89586A89586A89586A89586A89586A89586A89586A89586A89586A8
              9586A99586AA9787C3B6ACFBFBFBFFFFFFFFFFFFFFFFFFFFFFFF}
            ParentFont = False
            OnClick = btnASECL_LotEndClick
          end
        end
        object pnlLotInfo_ASECL: TPanel
          Left = 0
          Top = 0
          Width = 650
          Height = 177
          Align = alTop
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 0
          object lb_Main_LotName: TLabel
            Left = 10
            Top = 8
            Width = 110
            Height = 24
            AutoSize = False
            Caption = 'Lot Name'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lb_Main_EventLogOPID: TLabel
            Left = 10
            Top = 40
            Width = 110
            Height = 24
            AutoSize = False
            Caption = 'Operator ID'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lb_Main_EventLogTesterID: TLabel
            Left = 10
            Top = 72
            Width = 110
            Height = 24
            AutoSize = False
            Caption = 'Tester ID'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object lb_Main_EventLogLoginMode: TLabel
            Left = 10
            Top = 104
            Width = 110
            Height = 24
            AutoSize = False
            Caption = 'Login Mode'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object sb_Main_EvenLevelLogin: TSpeedButton
            Left = 292
            Top = 96
            Width = 120
            Height = 36
            AllowAllUp = True
            GroupIndex = 1
            Caption = 'Login'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            Glyph.Data = {
              360C0000424D360C000000000000360000002800000020000000200000000100
              180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2
              EAEAEAE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
              E9E9E9E9E9E9E9E9E9E9EAEAEAF2F2F2FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2D4D4D4
              C0C0C0BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
              BCBCBCBCBCBCBCBCBCBCC0C0C0D4D4D4F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEAEA818181
              6566666666666666666666666666666666666666666565656666666666666666
              66666666656666666666656666808080DADADAE9E9E9E9E9E9E9E9E9EAEAEAF2
              F2F2FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9E9E9656565
              5D5B5B5856565F5E5E5956565957575956565C5B5B6363635C5B5B5956565957
              575956566362625856565D5B5B656565B7B7B7BCBCBCBCBCBCBCBCBCC0C0C0D4
              D4D4F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9E9E9646465
              706E6C6360606564646462626462626462626464646263636464646462626462
              62646262656464636060706E6D65656565666666666665666666666665666681
              8181EAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFECECEC636363
              817E7E6A68686C6A6A6D6B6B6867676364646262625F5F5E6262626464646867
              676D6B6B6C6A6A6A6868817E7F6364645856565956566362625856565D5B5B65
              6565E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F6636363
              8B8A897876757573726E6D6C6363636060608A8A889C9C9A8787866060606363
              636E6D6C7573727876758B8A89636363646161646262656464636060706E6C64
              6465E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDBEBEBE
              6C6D6B9B9A988785847E7C7B61616159595ADFDFDE91918FD7D7D55A5A5B6161
              617E7C7B8784839A99976B6B6A6363636867676D6A6B6C6A6A6A6868817E7E63
              6363ECECECFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFD
              B5B5B56C6A6A7575766C6B6B5E5D5D838383FFFFFF8C8B88FCFBF7807F7E5E5F
              5E6B6A6A7272726665657272726161616363636E6D6C7573727876758B8A8963
              6363F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FEFEFEDCDCDC6969696262625B5A59E4E1DFFFFFFF918B86FFFBF5D4D1CC5C5B
              5A5C5C5C6F6F6F868685D9D9D75B5B5B6161617E7C7B8785849B9A986C6C6BBE
              BEBEFDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBF2F2F2
              EAEAEAE9E9E9E1E1E17B7978938D89C5E7F66BAACA5694B53E7C9E88A7B58681
              7D7A7877F6F5F38F8E8CFDFCF980807E5F5F5F6C6B6B7574756B696AAAAAAAE7
              E7E7E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9EAEAEAF2F2F2FBFBFBF2F2F2D4D4D4
              C0C0C0BCBCBCBCBCBCBBBBBB7D8A9C5385AB79B9D86AA6C65B96B62A587B3346
              5DE8E2DEFFFFFF928C87FFFBF5D4D1CC5D5B5B615E606C6669AFAEAEBCBCBCBC
              BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCC0C0C0D4D4D4F2F2F2EAEAEA89B79E
              6DB2906DB3906FB89275BF9328587E8BCEEE84C5E579B9DA72B0D166A3C72C53
              81C9E8F46CABCB5694B63E7C9E87A5B487807D616F676BAD8D6DB8936DB6926D
              B2906CB18F6CB08E6CAF8E6CAF8E6CAF8E6DB08F89B69EEAEAEAE9E9E96CAF8D
              C9F3DAC4F2D7C8F7D9AAD7C53C6DA098DCFC89C9E97EBEDE77B6D770AFD03D6E
              995788AC7BBADA6AA7C65B95B629557A6D9BA3C9FCDBC5F5D8C4F3D7C3F2D6C3
              F1D6C3F1D6C4F1D6C4F2D7C4F2D7C3F1D6C9F3DA6CAF8DE9E9E9E9E9E969AC8A
              C8F2DA64A8856AAF883F747D30699AA0E5FF90D2F185C5E57EBDDD78B8D81E4B
              785589B187C9E87AB9DB72B0D166A1C624527E6FB68B68AD8865AB8765AB8765
              AB8766AB8868AC8A66A98766AA8864A784C8F2DA69AC8AE9E9E9E9E9E968AA87
              C9F4DE5B9F7966A87E3A71773C7AA9ADF4FF9DE2FF91D4F386C7E882C4E21A46
              72548DB68FD0EF7FC0E077B6D770AFD03C6B9BA1D0C0BAECCFB6E8CDB6E7CDB6
              E7CDB6E8CEBBECD25B9E795FA17C5B9E78C9F4DE68AA87E9E9E9E9E9E965A985
              CCF3DF53986E5A9D6D1747701C57891F538119467340729A82C4E377B2D20E32
              6225578699DCFB87C7E77EBDDD78B8D81C497A6694A0B5EACBAFE3C8AFE2C8AF
              E3C8AFE3C8B3E6CC60A07A50936A52976DCCF3DF65A985E9E9E9E9E9E963A783
              CDF2E062A680C2F6D506326F2C74A24785AF5A8CB4567CA430527C2547721A48
              7A0D396DA6EDFF93D5F486C7E882C4E3194575588C95B4EAC9ADE2C6AEE2C8AF
              E3C9AFE2C8ADE2C7B0E4CAB5E8CF60A480CDF2E063A783E9E9E9E9E9E961A580
              CFF2E15DA27ABCF1D006316D2971A04387B1528CB35A8AB34D7CA63764911846
              79123F721E4C7841739A82C4E377B2D20D32641D4E77B4EBCAADE2C871B29065
              A98772B290ACE0C8AADFC6ADE2C95AA079CFF2E161A580E9E9E9E9E9E95FA47E
              D2F2E4599E76B4EDCC0D38701855883883B04487B14985AF3F77A22D61901443
              761543755D90B8577DA530527C25477219477B083570B2ECC9ABE3C85B9D785F
              A07C5B9D78A8E1C7A5DEC3A8E1C6589C75D2F2E45FA47EE9E9E9E9E9E95EA17C
              D3F4E7569B72ACE6C86EA3A4052F6B1D598C307AA83379A72B6D9A1A4F811440
              72306D9B548FB65A8BB44D7CA6376491164579093671AEEAC7A6E0C55FA07950
              92695FA079A5DEC4A2DCC1A5DFC4559A72D3F4E75EA17CE9E9E9E9E9E95CA079
              D7F5EA52986EA5E0C4A8E3C375ADA915427504306B08326D09346F09366E1750
              833A85B24589B24985AF3F76A22C6191114076103E74A9E5C3A0DBC0A2DCC2A3
              DDC3A2DCC29FDABF9FDABFA2DDC352976ED7F5EA5CA079E9E9E9E9E9E95A9E77
              DCF6EC50946BA2DDC4A0DCC1A2DEC1A5E2C2A7E4C2A8E5C3ABE8C46DA4A30731
              6D1E5A8D317BA93479A82C6D9B184D820C387256908DA2DEC19BD8BD9BD7BD9B
              D7BD9BD7BD9CD7BE9ED9C0A1DDC450946BDCF6EC5A9E77E9E9E9E9E9E9589C75
              E0F7EF61A58069AC8971B2909CD9BF9AD7BD9AD7BC9AD7BC9CD9BDA2E0C072AB
              A71240720935710E38740D37751240724480719EDCBE9BD8BD98D5BC98D5BB98
              D5BB98D5BC9BD7BE71B18F69AC8961A580E0F7EF589C75E9E9E9E9E9E9559B72
              E3F9F2599E785FA27D5EA07B9BD9C199D7BE98D7BD98D7BD99D7BE9AD9BF9FDE
              C195D6B46DAF825B9F6E5B9E6E6CAE8392D3B49CDBC199D7BE98D7BD98D7BD98
              D7BD99D7BE9BD9C15EA07B5FA27D599E78E3F9F2559B72E9E9E9EAEAEA549870
              E7F9F4478D614D92674C9066478C60468B5E468B5E468B5E468B5E468B5E478C
              5F498E614B90634D91654D91654B9063488D61478B5F468B5E468B5E468B5E46
              8B5E468B5E478C604C90664D9267478D61E7F9F4549870EAEAEAF2F2F253986E
              E8F8F4E3F5F0E4F5F0E4F5F0E3F5EFE3F5EFE3F5EFE3F5EFE3F5EFE3F5EFE3F5
              EFE4F5F0E4F5F0E4F6F0E4F6F0E4F5F0E4F5F0E3F5EFE3F5EFE3F5EFE3F5EFE3
              F5EFE3F5EFE3F5EFE4F5F0E4F5F0E3F5F0E8F8F453986EF2F2F2FBFBFB86B297
              7EC0A19AD9BF96D6BB95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5
              BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95D5BA95
              D5BA95D5BA95D5BA95D5BA96D6BB9AD9BF7EC0A186B297FBFBFBFFFFFFFCFCFC
              86B29793CAACBDEED4BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBEC
              D2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BBECD2BB
              ECD2BBECD2BBECD2BBECD2BDEED493CAAC86B297FCFCFCFFFFFFFFFFFFFFFFFF
              FBFBFB87B3975092694F91684F91684F91684F91684F91684F91684F91684F91
              684F91684F91684F91684F91684F91684F91684F91684F91684F91684F91684F
              91684F91684F91684F916850926987B397FBFBFBFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
              FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
            ParentFont = False
            OnClick = sb_Main_EvenLevelLoginClick
          end
          object lb_Main_TesterLoadBoard: TLabel
            Left = 10
            Top = 136
            Width = 110
            Height = 24
            AutoSize = False
            Caption = 'Load Board'
            Color = 8421440
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentColor = False
            ParentFont = False
            Layout = tlCenter
          end
          object edtASECL_LotID: TEdit
            Left = 128
            Top = 8
            Width = 217
            Height = 24
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #26032#32048#26126#39636
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtASECL_OPID: TEdit
            Left = 128
            Top = 40
            Width = 121
            Height = 24
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #26032#32048#26126#39636
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object edtASECL_TesterID: TEdit
            Left = 128
            Top = 72
            Width = 121
            Height = 24
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #26032#32048#26126#39636
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
          object cbbASECL_LoginMode: TComboBox
            Left = 128
            Top = 104
            Width = 161
            Height = 28
            Color = 14670284
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemHeight = 20
            ParentFont = False
            TabOrder = 3
            Text = 'OPERATOR'
            Items.Strings = (
              'OPERATOR'
              'ENG_VENDOR'
              'ENG_MI'
              'PM'
              'AUDIT'
              'ADMIN')
          end
          object edtASECL_LoadBoard: TEdit
            Left = 128
            Top = 136
            Width = 121
            Height = 24
            Color = 14670284
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = #26032#32048#26126#39636
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = ' '
            OnClick = edtASECL_LotIDClick
          end
        end
      end
      object Panel12: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Socket ID'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
    end
    object tsChamberBoost: TTabSheet
      Caption = 'Chamber Boost'
      ImageIndex = 17
      object btnStopChamberBoost: TSpeedButton
        Left = 47
        Top = 207
        Width = 323
        Height = 44
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Stop Boost'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -29
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF6F6F6EEEE
          EEEAEAEAE9E9E9E9E9E9E9E9E9E9E9E9EAEAEAEEEEEEF6F6F6FCFCFCFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF9F9F9EFEFEFE4E4E4D6D6D6C7C7
          C7BEBEBEBCBCBCBCBCBCBCBCBCBCBCBCBEBEBEC7C7C7D6D6D6E4E4E4EFEFEFF9
          F9F9FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFCFCFCF2F2F2DFDFDFCACACAB5B6BB7A80B94653
          BD2D3CC02D3CC02D3DC02E3DC02E3DC02E3EC14753BD7A81B9B5B6BBCACACADF
          DFDFF2F2F2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFAFAFAE9E9E9D1D1D1AAACBC4551BD2E3EC3394FD33D56
          DE405DE93D5BEB3958EA3654E83351E9304DE52D46DB2C42D02E3EC24653BDAB
          ACBCD1D1D1E9E9E9FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFAFAFAE6E6E6C7C7C7656EBB2D3DC3445CDE5370F0627BF5738A
          FC8396FF8599FF8498FF8395FF8295FF7D8FFF677DFA4C64F12E4BE72A43D82E
          3FC26770BCC7C7C7E6E6E6FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFAFAFAE6E6E6C5C5C53947BD3B4DCE5672EF7089F9889DFF7E90FF7083
          FF6779FF6275FE6375FE6376FE6376FE6879FF6E81FF7689FF7A8CFF546BF426
          43E32B3FCC3D4ABEC5C5C5E6E6E6FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FCFCFCE9E9E9C7C7C73A48BE4B62DD6582F5859CFF8196FF6577FD6073FD6174
          FD6174FD6275FD6275FD6275FD6275FD6275FD6275FD6274FD6577FE7789FF6E
          82FE304CE82941D53D4BBEC7C7C7E9E9E9FCFCFCFFFFFFFFFFFFFFFFFFFEFEFE
          F2F2F2D1D1D13947BE4F65DE6E8AF78EA4FF7286FE5C6FFC5D70FC5F71FC5F72
          FC5F72FC5F72FC5F72FC5F72FC5F72FC5F72FC5F72FC5F72FC5F72FC5E71FC6B
          7EFF798CFF3551EA2941D53E4BBFD1D1D1F2F2F2FEFEFEFFFFFFFFFFFFF9F9F9
          E0E0E06972BE3F52D0728DF791A7FF6D81FC5A6DFB5C6FFB5D70FB5D70FB5D70
          FB5D70FB5D70FB5D70FB5D70FB5D70FB5D70FB5D70FB5D70FB5D70FB5D70FB5C
          6FFB697BFD7A8CFF304CE82B3FCB6B74BFE0E0E0F9F9F9FFFFFFFFFFFFEFEFEF
          B8BAC92D3DC2718CF58FA7FF7085FD576BFA596DFA5A6EFA5A6EFA5A6EFA5A6E
          FA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA5A6EFA59
          6EFA596DFA697CFE6F82FE2643E32E3FC2B8BAC9EFEFEFFFFFFFFCFCFCE4E4E4
          4552BF5C74E588A1FD869CFF5467F9576BF9586CF9586CF9586CF9586CF9586C
          F9586CF9586CF9586CF9586CF9586CF9586CF9586CF9586CF9586CF9586CF958
          6CF9586CF9566AF97587FF546CF52A43D84854C0E4E4E4FCFCFCF6F6F6CFD0D5
          2D3CC2819DFC98AFFF576CF85368F7556AF7556AF7556AF7556AF7556AF7556A
          F7556AF7556AF7556AF7556AF7556AF7556AF7556AF7556AF7556AF7556AF755
          6AF7556AF75469F7586DF87B8DFF2E4BE82E3EC2CFD0D5F6F6F6EEEEEE858BC4
          5166D98CA6FE8299FE4E63F55267F65368F65368F65368F65368F65368F65368
          F65368F65368F65368F65368F65368F65368F65368F65368F65368F65368F653
          68F65368F65368F65166F67185FF4C65F12D42D0868DC5EEEEEEEAEAEA4753C0
          6B82EA96AEFF697EF94D63F54F66F54F66F54E64F54D64F54D63F54D63F54D63
          F54D63F54D63F54D63F54D63F54D63F54D63F54D63F54D63F54D63F54D64F54E
          64F54F66F55066F54E64F56175FA687EFA2D47DB4B57C1EAEAEAE9E9E92837BF
          87A2FB9DB6FF5166F44C62F44D64F44A61F4435BF33D56F33B54F33B53F33B53
          F33B53F33B53F33B53F33B53F33B53F33B53F33B53F33B53F33B54F33D56F343
          5BF34A61F44D64F44D63F45166F57F91FF304DE62E3DC0E9E9E9E9E9E92736BE
          90ADFFA0B8FF4359F2495FF3495FF34057F2C8CEFBFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC8
          CEFB4057F2495FF34A60F3475DF28597FF3351E92E3DC0E9E9E9E9E9E92735BE
          93AFFFA1BAFF4057F1475EF2455DF23750F1FFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFF3750F1455DF2475EF2445BF18698FF3654E92E3DC0E9E9E9E9E9E92635BE
          97B2FFA2BBFF3E54EE455BF0435BF0354EEEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFF354EEE435BF0455CF04158EF879AFF3A58EA2D3DC0E9E9E9EAEAEA2635BE
          9BB6FFA3BCFF3A52ED4159EF4159EF3851EEC5CCFAFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC5
          CCFA3851EE4159EF425AEF3E56EE899BFF3E5CEB2D3CC0EAEAEAEEEEEE2736BF
          97B2FFA4BEFF455CEF3E56EE4058EE3D56EE364FED304AEC2E48EC2D48EC2D48
          EC2D48EC2D48EC2D48EC2D48EC2D48EC2D48EC2D48EC2D48EC2E48EC304AEC36
          4FED3D56EE4058EE3F57EE465CEF8699FF415DE92D3CC0EEEEEEF6F6F64955C3
          7A91EDA3BEFF5F78F43952EC3D56ED3D57ED3C55ED3B55ED3B54ED3B54ED3B54
          ED3B54ED3B54ED3B54ED3B54ED3B54ED3B54ED3B54ED3B54ED3B54ED3B55ED3C
          55ED3D57ED3D57ED3B54EC586EF5768CFD3D56DE4E5BC4F6F6F6FCFCFC959CD6
          5D72DDA3BEFF829BFB344DEA3A53EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55
          EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55EC3C55EC3C
          55EC3C55EC3B54EC3750EB7287FC647CF63A4FD4969DD6FCFCFCFFFFFFE7E8ED
          2D3CC3A7C2FFA9C2FF3C56EB354FEA3852EB3953EB3953EB3953EB3953EB3953
          EB3953EB3953EB3953EB3953EB3953EB3953EB3953EB3953EB3953EB3953EB39
          53EB3953EB3650EA3C55EB8CA0FF5470F02E3EC3E7E8EDFFFFFFFFFFFFF9F9F9
          4A56C6788EEAA6C1FF8AA3FC2D47E7344EE83751E93751E93751E93751E93751
          E93751E93751E93751E93751E93751E93751E93751E93751E93751E93751E937
          51E9354FE8304AE7798FFD728BF9455DDF4E5AC7F9F9F9FFFFFFFFFFFFFEFEFE
          DBDDEC2D3CC2A2BCFFA8C3FF607AF22C46E6314BE8344EE8344EE8344EE8344E
          E8344EE8344EE8344EE8344EE8344EE8344EE8344EE8344EE8344EE8344EE832
          4CE82E48E65870F2889FFF5773EF2D3DC2DBDDECFEFEFEFFFFFFFFFFFFFFFFFF
          FCFCFC7B84D24D60D3A9C4FFAAC4FF5771EF2943E52F49E6314BE7324CE7324C
          E7324CE7324CE7324CE7324CE7324CE7324CE7324CE7324CE7324BE7304AE62B
          45E55169F092A8FF6683F53B4DCE7D86D2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFAFAFA3B49C37186E7A8C3FFAAC4FF5D77F1243FE32A45E52D48E62E49
          E62F4AE62F4AE62F4AE62F4AE62F4AE62F4AE62E49E62D48E62B46E52641E458
          71F195ABFF6F8AF74C62DD3F4DC4FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFAFAFA3C49C47186E7A9C4FFA8C3FF89A2FB304CE5233FE32742
          E32A45E42B46E52B46E52B46E52B46E52A45E42843E32540E3304CE67D95FC92
          AAFF738EF84F65DF404DC5FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFAFAFA3C4BC44E60D3A2BCFFA7C2FFAAC3FF7D97F9516C
          ED2F4BE51D3AE11E3BE11E3BE11E3AE12E4BE54F6AEE7790F99CB2FF8AA2FE71
          8DF53F52D0404DC4FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFCFCFC7F87D62D3CC2788EEAA7C2FFA4BFFFA5BF
          FFA7BFFFA6BFFFA5BEFFA4BDFFA3BAFFA0B8FF99B1FF8DA7FF829DFD5D74E52D
          3DC28089D6FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEE1E3F34C5AC82D3CC35D72DD7A91
          EE97B2FF9BB7FF98B3FF94AFFF91ADFF88A3FB6B83EA5166D92D3CC24F5BC8E1
          E3F3FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF3F4F99FA6E04E5B
          C92736BF2635BE2635BE2735BE2736BE2837BF505CC9A0A6E0F3F4F9FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        ParentFont = False
        OnClick = btnStopChamberBoostClick
      end
      object btnStartChamberBoost: TSpeedButton
        Left = 47
        Top = 147
        Width = 323
        Height = 44
        AllowAllUp = True
        GroupIndex = 1
        Caption = 'Start Boost'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -29
        Font.Name = 'Arial'
        Font.Style = []
        Glyph.Data = {
          360C0000424D360C000000000000360000002800000020000000200000000100
          180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFAFAFAEFEFEFE9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
          E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
          E9E9EFEFEFFAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFEFEFEFCECECEBDBDBDBCBCBCBCBCBCBCBCBCBCBCBCBCBC
          BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBD
          BDBDCECECEEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E94276A5386EA2376DA0376CA0376CA0376CA0376C
          A0376CA0376CA0376CA0376CA0376CA0376CA0376CA0376CA0376CA0376DA038
          6EA24276A5E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9386EA268C0E866BCE465BBE465BBE465BBE465BB
          E466BCE466BCE566BCE566BCE566BCE465BBE465BBE465BBE465BBE466BCE468
          C0E8386EA2E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA063B9E360B4DF60B3DE60B3DE60B3DE60B4
          DF61B7E162B8E262B9E262B8E261B7E160B4DF60B3DE60B3DE60B3DE60B4DF63
          B9E3376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA062B9E45DB2DE5DB1DD5DB1DD5EB3DE5FB6
          E1468BB9336C99346E99336C99468BB95FB6E15EB3DE5DB1DD5DB1DD5DB2DE62
          B9E4376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA063B9E45AB0DE5AAFDD5AAFDD5BB2E0458A
          B95F88BC8787F95956F68787F95884B5458AB95BB2E05AAFDD5AAFDD5AB0DE63
          B9E4376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA066B9E458AFDD58AEDC58AFDC5AB3E0346C
          998686F80000DE0000DD0000DE9092FB336B985AB3E058AFDC58AEDC58AFDD66
          B9E4376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA067BBE554ACDD55ACDC55ADDD57B2E1356E
          995552F64746F11B1AE54746F15552F54577A757B2E055ADDD55ACDC54ACDD67
          BBE5376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376DA06BBBE652ABDD53ABDC53ACDD55B0E0346C
          998484F82B28ECA3A4FF2B28EC8384F8346C9855B0E053ACDD53ABDC52ABDD6B
          BBE6376DA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376CA06DBCE64EA9DB50A9DB50A9DB50ACDE4089
          B76088BC8789F80000DF878AF85884B54089B850ACDD50A9DB50A9DB4EA9DB6D
          BCE6376CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9376CA070BDE64CA8DB4DA8DB4DA8DB4BA8DC4CAC
          DF408AB83B79990000E63B7999408BB94CACDF4BA8DC4DA8DB4DA8DB4CA8DB70
          BDE6376CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA072BFE849A5DB4AA5DB49A6DC92CCEE92CE
          EF4DAFE23E799C0000E93E799C4DAFE292CEEF92CCEE49A6DC4AA5DB49A5DB72
          BFE8366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA076BFE845A4DA45A4DA45A6DD194C7A1A4D
          7C4AAEE23E789C0000E93E789C4AAEE21A4D7C194C7A45A6DD45A4DA45A4DA76
          BFE8366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA078C0E942A2DC8FCAED8ECCF08ECEF290CF
          F248ABE23F789C0000E93F789C48ABE290CFF28ECEF28ECCF08FCAED42A2DC78
          C0E9366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA07BC2EB40A3DD1E4E7B1D507E1B507F1C4F
          7D45ABE23F799C0000E93F799C45ABE21C4F7D1B507F1D507E1E4E7B40A3DD7B
          C2EB366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA07FC3EB3EA0DB41A3DE40A5E08CCDF18CCD
          F142A8E13F799C0000E93F799C42A8E18CCDF18CCDF140A5E041A3DE3EA0DB7F
          C3EB366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9366CA083C6EC399DD93B9FDB3AA1DE1B4E7C1C4F
          7C3FA7E13F789C0000E93F789C3FA7E11C4F7C1B4E7C3AA1DE3B9FDB399DD983
          C6EC366CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9356CA086C6EC359BD988C6EC86C8EF86C9F188CA
          F13BA3DF39739B0000E839739B3BA3DF88CAF186C9F186C8EF88C6EC359BD986
          C6EC356CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9356CA08AC9ED359CDB204F7B1F517F1D517F1D50
          7D37A1E030699AFFFFFF30699A37A1E01D507D1D517F1F517F204F7B359CDB8A
          C9ED356CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9356CA08ECAEE3298D9369CDD359EDF84C7F284C7
          F1329DDF2A6399FFFFFF2A6399329DDF84C7F184C7F2359EDF369CDD3298D98E
          CAEE356CA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9356BA091CCF02E96D73198D8309ADB1D507C1E50
          7D309CDE2A6399FFFFFF2A6399309CDE1E507D1D507C309ADB3198D82E96D791
          CCF0356BA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9356BA096CEF02994D780C1EB7FC2EE7FC4F081C4
          F02C99DD2A6399FFFFFF2A63992C99DD81C4F07FC4F07FC2EE80C1EB2994D796
          CEF0356BA0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9346B9F9BD1F12893D921517C21537F1E527F1F51
          7E2A98DE2A6399FFFFFF2A63992A98DE1F517E1E527F21537F21517C2893D99B
          D1F1346B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9346B9FA0D2F22691D72C96DA2B97DD7EC3EF7EC3
          EF2796DC2A6399FFFFFF2A63992796DC7EC3EF7EC3EF2B97DD2C96DA2691D7A0
          D2F2346B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9346B9FA3D4F3218DD52690D72592DA1F517D2051
          7D2494DD2B6399FFFFFF2B63992494DD20517D1F517D2592DA2690D7218DD5A3
          D4F3346B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9336B9FA8D6F31D8CD679BDEA78BEED77C0EF7AC0
          EF2293DC2C6499FFFFFF2C64992293DC7AC0EF77C0EF78BEED79BDEA1D8CD6A8
          D6F3336B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9336B9FADDAF51B8CD724527C24547F24547F2554
          7E2192DB31669AFFFFFF31669A2192DB25547E24547F24547F24527C1B8CD7AD
          DAF5336B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFE9E9E9336B9FB2DBF61989D5218FD92391DB2391DB2390
          DA2190D92D77B334689A2D77B32190D92390DA2391DB2391DB218FD91989D5B2
          DBF6336B9FE9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFEAEAEA326B9FB7DEF71185D41988D51A89D51A89D51A89
          D51A89D6188AD7178AD8188AD71A89D61A89D51A89D51A89D51988D51185D4B7
          DEF7326B9FEAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFF2F2F2346DA1BDE5FB79BDEC7DBFEC7EBFEC7EBFEC7EBF
          EC7EBFEC7EBFEC7EBFED7EBFEC7EBFEC7EBFEC7EBFEC7EBFEC7DBFEC79BDECBD
          E5FB346DA1F2F2F2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFBFBFB7298BC356DA1356CA0366DA0376DA0376DA0376D
          A0376DA0376DA0376DA0376DA0376DA0376DA0376DA0376DA0366DA0356CA035
          6DA17298BCFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
        ParentFont = False
        OnClick = btnStartChamberBoostClick
      end
      object Panel33: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 25
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Chamber Boost Function'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object pnlChamberBoost: TPanel
        Left = 0
        Top = 25
        Width = 650
        Height = 100
        Align = alTop
        BevelInner = bvLowered
        Caption = '00:00'
        Color = 2468626
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -64
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
    end
    object ATC_WinWay: TTabSheet
      Caption = 'ATC WinWay'
      ImageIndex = 18
      object pnlWinwayCH1: TPanel
        Left = 7
        Top = 24
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = 'CH 1'
        Color = 10801371
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object pnlWinwayCH2: TPanel
        Left = 77
        Top = 24
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = 'CH 2'
        Color = 10801371
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
      end
      object pnlWinwayCH3: TPanel
        Left = 147
        Top = 24
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = 'CH 3'
        Color = 10801371
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object pnlWinwayCH4: TPanel
        Left = 217
        Top = 24
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = 'CH 4'
        Color = 10801371
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object pnlWinwaySTCH1: TPanel
        Left = 7
        Top = 120
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        Visible = False
      end
      object pnlWinwaySTCH2: TPanel
        Left = 77
        Top = 120
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        Visible = False
      end
      object pnlWinwaySTCH3: TPanel
        Left = 147
        Top = 120
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        Visible = False
      end
      object pnlWinwaySTCH4: TPanel
        Left = 217
        Top = 120
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        Visible = False
      end
      object pnlWinwayPVCH1: TPanel
        Left = 7
        Top = 49
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
      end
      object pnlWinwayPVCH2: TPanel
        Left = 77
        Top = 49
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
      end
      object pnlWinwayPVCH3: TPanel
        Left = 147
        Top = 49
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
      end
      object pnlWinwayPVCH4: TPanel
        Left = 217
        Top = 49
        Width = 70
        Height = 25
        BevelInner = bvLowered
        Caption = '0.0 '#39'C'
        Color = 2468626
        Font.Charset = ANSI_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
      end
    end
    object tsRFMD: TTabSheet
      Caption = 'RFMD'
      ImageIndex = 18
      object btnESCFunction: TButton
        Left = 26
        Top = 13
        Width = 246
        Height = 41
        Caption = 'Empty Socket Check'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = btnESCFunctionClick
      end
      object btnCancelTestPause: TButton
        Left = 27
        Top = 64
        Width = 246
        Height = 41
        Caption = 'Cancel Tester Pause'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnClick = btnCancelTestPauseClick
      end
    end
    object ts_FTPAutomation: TTabSheet
      Caption = 'FTP Automation'
      ImageIndex = 19
      object sbTest: TSpeedButton
        Left = 0
        Top = 304
        Width = 33
        Height = 22
        Caption = 'T1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        OnClick = sbTestClick
      end
      object sbRecipeUpload: TSpeedButton
        Left = 63
        Top = 304
        Width = 100
        Height = 22
        Caption = 'Upload'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = sbRecipeUploadClick
      end
      object sbRecipeDownload: TSpeedButton
        Left = 168
        Top = 304
        Width = 100
        Height = 22
        Caption = 'Download'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = sbRecipeDownloadClick
      end
      object sbFTPAutomationSave: TSpeedButton
        Left = 296
        Top = 304
        Width = 71
        Height = 22
        Caption = 'Save'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
        OnClick = sbFTPAutomationSaveClick
      end
      object gbFTPAutomation_Download: TGroupBox
        Left = 0
        Top = 0
        Width = 417
        Height = 129
        Caption = 'Download'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Visible = False
        object lblN06_Host01: TLabel
          Left = 68
          Top = 15
          Width = 67
          Height = 16
          Caption = 'Server IP'#65306
        end
        object lblN06_UserName01: TLabel
          Left = 78
          Top = 43
          Width = 57
          Height = 16
          Caption = 'User ID'#65306
        end
        object lblN06_Password01: TLabel
          Left = 63
          Top = 72
          Width = 72
          Height = 16
          Caption = 'Password'#65306
        end
        object labN06_DownloadPath: TLabel
          Left = 53
          Top = 100
          Width = 82
          Height = 16
          Caption = 'Server Path'#65306
        end
        object edFTPAutomation_Down_ServerPath: TEdit
          Left = 142
          Top = 96
          Width = 264
          Height = 24
          TabOrder = 3
        end
        object edFTPAutomation_Down_Password: TEdit
          Left = 142
          Top = 68
          Width = 264
          Height = 24
          TabOrder = 2
        end
        object edFTPAutomation_Down_UserID: TEdit
          Left = 142
          Top = 39
          Width = 264
          Height = 24
          TabOrder = 1
        end
        object edFTPAutomation_Down_ServerIP: TEdit
          Left = 142
          Top = 11
          Width = 264
          Height = 24
          TabOrder = 0
        end
      end
      object gbFTPAutomation_Upload: TGroupBox
        Left = 0
        Top = 130
        Width = 417
        Height = 159
        Caption = 'Upload'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Visible = False
        object lblN06_Host02: TLabel
          Left = 68
          Top = 15
          Width = 67
          Height = 16
          Caption = 'Server IP'#65306
        end
        object lblN06_UserName02: TLabel
          Left = 78
          Top = 43
          Width = 57
          Height = 16
          Caption = 'User ID'#65306
        end
        object lblN06_Password02: TLabel
          Left = 63
          Top = 72
          Width = 72
          Height = 16
          Caption = 'Password'#65306
        end
        object lblN06_UploadPath: TLabel
          Left = 53
          Top = 100
          Width = 82
          Height = 16
          Caption = 'Server Path'#65306
        end
        object lblN06_UploadErrPath: TLabel
          Left = 27
          Top = 128
          Width = 107
          Height = 16
          Caption = 'Error Code Path'#65306
        end
        object edFTPAutomation_Up_ServerPath: TEdit
          Left = 142
          Top = 96
          Width = 264
          Height = 24
          TabOrder = 3
        end
        object edFTPAutomation_Up_Password: TEdit
          Left = 142
          Top = 68
          Width = 264
          Height = 24
          TabOrder = 2
        end
        object edFTPAutomation_Up_UserID: TEdit
          Left = 142
          Top = 39
          Width = 264
          Height = 24
          TabOrder = 1
        end
        object edFTPAutomation_Up_ServerIP: TEdit
          Left = 142
          Top = 11
          Width = 264
          Height = 24
          TabOrder = 0
        end
        object edFTPAutomation_Up_ErrorCodePath: TEdit
          Left = 142
          Top = 124
          Width = 264
          Height = 24
          TabOrder = 4
        end
      end
    end
    object tsYieldMonitior: TTabSheet
      Caption = 'Yield Monitor'
      ImageIndex = 20
      object pgcYieldMonitor: TPageControl
        Left = 0
        Top = 0
        Width = 650
        Height = 533
        ActivePage = tsTPW
        Align = alClient
        Style = tsFlatButtons
        TabIndex = 1
        TabOrder = 0
        object tsSigurd: TTabSheet
          Caption = 'Sigurd'
          ParentShowHint = False
          ShowHint = True
          object pnlYieldMonitor: TPanel
            Left = 0
            Top = 0
            Width = 642
            Height = 25
            Align = alTop
            BevelOuter = bvNone
            Caption = 'Yield Monitor'
            Color = 9534289
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -19
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object gbRMS: TGroupBox
            Left = 0
            Top = 25
            Width = 642
            Height = 56
            Align = alTop
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object lblMonitor_Contact: TLabel
              Left = 192
              Top = 24
              Width = 128
              Height = 16
              Caption = ' Contact mode '#27169#24335' '#65306
            end
            object cbMonitor_FTPRMS: TCheckBox
              Left = 8
              Top = 24
              Width = 169
              Height = 17
              Caption = 'FTP'#26159#21542#36899#32218#38283#21855'RMS'
              Enabled = False
              TabOrder = 1
            end
            object cbContactMode: TComboBox
              Left = 317
              Top = 20
              Width = 180
              Height = 24
              Style = csDropDownList
              Enabled = False
              ItemHeight = 16
              ItemIndex = 0
              TabOrder = 0
              Text = 'Direct Contact'
              Items.Strings = (
                'Direct Contact'
                'Drop Contact'
                'Direct & Slow Contact'
                'TMOVE Contact'
                'TMOVE Drop Contact'
                'Direct & Soft EP Contact'
                'Drop & Soft EP Contact'
                'Shift Contact for 8Site 1x4'
                'Drop & Slow Contact'
                'TMOVE Slow Contact'
                'TMOVE Drop & Slow Contact')
            end
          end
          object gbCmpBySite: TGroupBox
            Left = 0
            Top = 81
            Width = 642
            Height = 80
            Align = alTop
            Caption = 'By Site Compare Yield% (1min)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object lblSiteYieldCmp_Cur: TLabel
              Left = 136
              Top = 56
              Width = 81
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object lblSiteYieldCmpIg_Cur: TLabel
              Left = 384
              Top = 56
              Width = 81
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object cbSiteYieldCmp_FT: TCheckBox
              Left = 9
              Top = 24
              Width = 288
              Height = 14
              Caption = 'Enable               %  after contact count'
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
            end
            object edSiteYieldCmp_FT: TEdit
              Left = 88
              Top = 21
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object edSiteYieldCmpIg_FT: TEdit
              Left = 301
              Top = 21
              Width = 80
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '100'
            end
            object edSiteYieldCmpIg_Cur: TEdit
              Left = 301
              Top = 48
              Width = 80
              Height = 24
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 5
              Text = '100'
              Visible = False
            end
            object edSiteYieldCmp_Cur: TEdit
              Left = 88
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 4
              Text = '0'
              Visible = False
            end
            object edSiteYieldCmpOnOff_Cur: TEdit
              Left = 8
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 3
              Text = 'OFF'
              Visible = False
            end
          end
          object gbLowYieldByTotal: TGroupBox
            Left = 0
            Top = 161
            Width = 642
            Height = 80
            Align = alTop
            Caption = 'Low Yields%(By Total)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object lblLowYieldByTotal_Cur: TLabel
              Left = 136
              Top = 56
              Width = 81
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object lblLowYieldByTotalIg_Cur: TLabel
              Left = 360
              Top = 56
              Width = 73
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object cbLowYieldByTotal_FT: TCheckBox
              Left = 9
              Top = 24
              Width = 272
              Height = 14
              Caption = 'Enable               %  Ignore IC count  '
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
            end
            object edLowYieldByTotalIg_FT: TEdit
              Left = 277
              Top = 21
              Width = 80
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '100'
            end
            object edLowYieldByTotal_FT: TEdit
              Left = 88
              Top = 21
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
            object edLowYieldByTotalIg_Cur: TEdit
              Left = 277
              Top = 48
              Width = 80
              Height = 24
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 5
              Text = '100'
              Visible = False
            end
            object edLowYieldByTotal_Cur: TEdit
              Left = 88
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 4
              Text = '0'
              Visible = False
            end
            object edLowYieldByTotalOnOff_Cur: TEdit
              Left = 8
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 3
              Text = 'OFF'
              Visible = False
            end
          end
          object gbContsFailBySocket: TGroupBox
            Left = 0
            Top = 241
            Width = 642
            Height = 80
            Align = alTop
            Caption = 'Consecutive Failure Alarm ( Socket )'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object labContsFailBySocket_FT: TLabel
              Left = 145
              Top = 24
              Width = 231
              Height = 16
              Caption = 'Failure Count                                               IC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblContsFailSocketAlarmCT_Cur: TLabel
              Left = 352
              Top = 56
              Width = 81
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object rbContsFailBySocket_FTOn: TRadioButton
              Left = 8
              Top = 24
              Width = 57
              Height = 17
              Caption = 'ON'
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object rbContsFailBySocket_FTOff: TRadioButton
              Left = 80
              Top = 24
              Width = 57
              Height = 17
              Caption = 'Off'
              Checked = True
              Color = 12761254
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentColor = False
              ParentFont = False
              TabOrder = 2
              TabStop = True
            end
            object edContsFailSocketAlarmCT_FT: TEdit
              Left = 232
              Top = 21
              Width = 117
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '100'
            end
            object edContsFailSocketAlarmCT_Cur: TEdit
              Left = 232
              Top = 48
              Width = 117
              Height = 24
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 4
              Text = '100'
              Visible = False
            end
            object edtContsFailBySocket_Cur: TEdit
              Left = 8
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 3
              Text = 'OFF'
              Visible = False
            end
          end
          object gbContsFailByHead: TGroupBox
            Left = 0
            Top = 321
            Width = 642
            Height = 80
            Align = alTop
            Caption = 'Consecutive Failure Alarm ( Head)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            object labContsFailByHead_FT: TLabel
              Left = 145
              Top = 23
              Width = 231
              Height = 16
              Caption = 'Failure Count                                               IC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblContsFailHeadAlarmCT_Cur: TLabel
              Left = 352
              Top = 56
              Width = 81
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object rbContsFailByHead_FTOn: TRadioButton
              Left = 8
              Top = 24
              Width = 49
              Height = 17
              Caption = 'ON'
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object rbContsFailByHead_FTOff: TRadioButton
              Left = 80
              Top = 24
              Width = 57
              Height = 17
              Caption = 'Off'
              Checked = True
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              TabStop = True
            end
            object edContsFailHeadAlarmCT_FT: TEdit
              Left = 232
              Top = 21
              Width = 117
              Height = 24
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '100'
            end
            object edContsFailHeadAlarmCT_Cur: TEdit
              Left = 232
              Top = 48
              Width = 117
              Height = 28
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 4
              Text = '100'
              Visible = False
            end
            object edtContsFailByHead_Cur: TEdit
              Left = 8
              Top = 48
              Width = 44
              Height = 24
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 3
              Text = 'OFF'
              Visible = False
            end
          end
          object grpOSBin: TGroupBox
            Left = 0
            Top = 401
            Width = 642
            Height = 88
            Align = alTop
            Caption = 'O/S Bin Info'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            object lblOSBin: TLabel
              Left = 9
              Top = 24
              Width = 256
              Height = 16
              AutoSize = False
              Caption = 'O/S Bin                                  %                    '#38982
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblOSBin_Cur: TLabel
              Left = 232
              Top = 56
              Width = 73
              Height = 16
              AutoSize = False
              Caption = #30446#21069#35373#23450
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              Visible = False
            end
            object edOSBin: TEdit
              Left = 64
              Top = 21
              Width = 33
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = 'Null'
            end
            object edOSBinPreset: TEdit
              Left = 120
              Top = 21
              Width = 33
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '100'
            end
            object edOSBinCnt: TEdit
              Left = 184
              Top = 21
              Width = 41
              Height = 24
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 2
              Text = '1200'
            end
            object edOSBinCnt_Cur: TEdit
              Left = 184
              Top = 48
              Width = 41
              Height = 24
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 5
              Text = '100'
              Visible = False
            end
            object edOSBinPreset_Cur: TEdit
              Left = 120
              Top = 48
              Width = 33
              Height = 24
              BorderStyle = bsNone
              Color = clRed
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 4
              Text = '100'
              Visible = False
            end
            object btnVerifyMode: TBtnPanelLane
              Left = 317
              Top = 29
              Width = 152
              Height = 36
              BevelInner = bvRaised
              Caption = 'Verify Mode Off'
              Color = 8404992
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWhite
              Font.Height = -13
              Font.Name = 'Arial'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 3
              TrueColor = 16744448
              FalseColor = 8404992
              FalseFontColor = clWhite
              Alias = 'SwShuttleFan'
              Style = tsButtons
            end
          end
          object gbManualCheckList: TGroupBox
            Left = 0
            Top = 489
            Width = 642
            Height = 56
            Align = alTop
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            object btnManualCheckList: TButton
              Left = 16
              Top = 23
              Width = 97
              Height = 25
              Caption = 'Manual Check'
              TabOrder = 0
              OnClick = btnManualCheckListClick
            end
            object btnManualStandard: TButton
              Left = 120
              Top = 23
              Width = 97
              Height = 25
              Caption = 'Standard'
              TabOrder = 1
              OnClick = btnManualStandardClick
            end
          end
        end
        object tsTPW: TTabSheet
          Caption = 'TPW'
          ImageIndex = 1
          object gbLowYield: TGroupBox
            Left = 0
            Top = 0
            Width = 642
            Height = 57
            Align = alTop
            Caption = 'Low Yields% (1min)'
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object labLowYieldICCount: TLabel
              Left = 231
              Top = 9
              Width = 7
              Height = 16
              Caption = '0'
            end
            object cbLowYield: TCheckBox
              Left = 9
              Top = 27
              Width = 312
              Height = 14
              Caption = 'Enable                 % after test count'
              TabOrder = 2
            end
            object edLowYieldIg: TEdit
              Left = 229
              Top = 24
              Width = 80
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object edLowYield: TEdit
              Left = 72
              Top = 24
              Width = 44
              Height = 21
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '0'
            end
          end
          object GroupBox4: TGroupBox
            Left = 0
            Top = 113
            Width = 642
            Height = 56
            Align = alTop
            Caption = 'Consecutive Failure Alarm ( Head)'
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object lblContsFailByHead: TLabel
              Left = 145
              Top = 24
              Width = 231
              Height = 16
              Caption = 'Failure Count                                               IC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object rbContsFailByHead_On: TRadioButton
              Left = 8
              Top = 24
              Width = 49
              Height = 17
              Caption = 'ON'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object rbContsFailByHead_Off: TRadioButton
              Left = 80
              Top = 24
              Width = 57
              Height = 17
              Caption = 'Off'
              Checked = True
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              TabStop = True
            end
            object edContsFailHeadAlarmCT: TEdit
              Left = 232
              Top = 21
              Width = 117
              Height = 21
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '100'
            end
          end
          object GroupBox5: TGroupBox
            Left = 0
            Top = 57
            Width = 642
            Height = 56
            Align = alTop
            Caption = 'Consecutive Failure Alarm ( Socket )'
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            object lblContsFailBySocket: TLabel
              Left = 145
              Top = 24
              Width = 231
              Height = 16
              Caption = 'Failure Count                                               IC'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object rbContsFailBySocket_On: TRadioButton
              Left = 8
              Top = 24
              Width = 57
              Height = 17
              Caption = 'ON'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object rbContsFailBySocket_Off: TRadioButton
              Left = 80
              Top = 24
              Width = 57
              Height = 17
              Caption = 'Off'
              Checked = True
              Color = 12761254
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentColor = False
              ParentFont = False
              TabOrder = 2
              TabStop = True
            end
            object edContsFailSocketAlarmCT: TEdit
              Left = 232
              Top = 21
              Width = 117
              Height = 21
              BorderStyle = bsNone
              Enabled = False
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 0
              Text = '100'
            end
          end
          object gb_HeadToHeadYieldEnable: TGroupBox
            Left = 0
            Top = 283
            Width = 642
            Height = 57
            Align = alTop
            Caption = 'Head To Head Yield % '
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            object cb_HeadToHeadYieldEnable: TCheckBox
              Left = 9
              Top = 20
              Width = 384
              Height = 30
              Caption = 'Enable              %  Ignore IC count                       ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object ed_HeadToHeadYield: TEdit
              Left = 84
              Top = 24
              Width = 44
              Height = 21
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object ed_HeadToHeadYieldCount: TEdit
              Left = 273
              Top = 24
              Width = 80
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 2
              Text = '100'
            end
          end
          object gbAllSiteFail: TGroupBox
            Left = 0
            Top = 169
            Width = 642
            Height = 57
            Align = alTop
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            object lblAllSiteFail: TLabel
              Left = 273
              Top = 24
              Width = 164
              Height = 20
              Caption = '*Count limitation: 1-100'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object cbAllSiteFail: TCheckBox
              Left = 9
              Top = 24
              Width = 264
              Height = 19
              Caption = 'All Site Fail                              count'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object edAllSiteFailCount: TEdit
              Left = 118
              Top = 24
              Width = 97
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '100'
            end
          end
          object gb_SiteToSiteYieldEnable: TGroupBox
            Left = 0
            Top = 226
            Width = 642
            Height = 57
            Align = alTop
            Caption = 'Site To Site Yield % '
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            object cb_SiteToSiteYieldEnable: TCheckBox
              Left = 9
              Top = 20
              Width = 384
              Height = 30
              Caption = 'Enable              %  Ignore IC count                       ea'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object ed_SiteToSiteYield: TEdit
              Left = 84
              Top = 24
              Width = 44
              Height = 21
              BiDiMode = bdLeftToRight
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
              ReadOnly = True
              TabOrder = 1
              Text = '0'
            end
            object ed_SiteToSiteYieldCount: TEdit
              Left = 273
              Top = 24
              Width = 80
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              ReadOnly = True
              TabOrder = 2
              Text = '100'
            end
          end
          object grp_SmartAutoClean: TGroupBox
            Left = 0
            Top = 340
            Width = 642
            Height = 112
            Align = alTop
            Caption = 'Smart Auto Clean'
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            object lbl_SmartAutoCleanCount: TLabel
              Left = 228
              Top = 9
              Width = 7
              Height = 16
              Caption = '0'
            end
            object lblAdaptiveIntervalCount: TLabel
              Left = 7
              Top = 57
              Width = 181
              Height = 16
              Caption = 'Adaptive Interval : 1000 / Count'
            end
            object lblAdaptiveLowYield: TLabel
              Left = 8
              Top = 81
              Width = 153
              Height = 16
              Caption = 'Adaptive Low Yield : 0.0%'
            end
            object lbl_SmartAutoCleanCount_CTF: TLabel
              Left = 308
              Top = 9
              Width = 7
              Height = 16
              Caption = '0'
            end
            object chk_SmartAutoClean: TCheckBox
              Left = 9
              Top = 25
              Width = 344
              Height = 18
              Caption = 'Enable ,Execut clean count             CTF'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
            end
            object edt_SmartAutoClean: TEdit
              Left = 225
              Top = 24
              Width = 40
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              Text = '2'
            end
            object ed_SmartAutoCleanCTF: TEdit
              Left = 305
              Top = 24
              Width = 40
              Height = 21
              BorderStyle = bsNone
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '2'
            end
          end
          object btnManualI49: TButton
            Left = 8
            Top = 472
            Width = 97
            Height = 25
            Caption = 'Manual [I49]'
            TabOrder = 7
            OnClick = btnManualI49Click
          end
        end
      end
    end
    object tsTesterLog: TTabSheet
      Caption = 'Tester Log'
      ImageIndex = 21
      object Panel35: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 49
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        DesignSize = (
          650
          49)
        object labTCPIPStatus: TLabel
          Left = 10
          Top = 8
          Width = 121
          Height = 33
          Alignment = taCenter
          Anchors = [akLeft, akBottom]
          AutoSize = False
          BiDiMode = bdLeftToRight
          Caption = 'OFF-LINE'
          Color = clRed
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentColor = False
          ParentFont = False
        end
        object labTCPIPSimulate: TLabel
          Left = 136
          Top = 8
          Width = 121
          Height = 33
          Alignment = taCenter
          AutoSize = False
          Caption = 'Simulate'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -24
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object btTesterTCPShow: TButton
          Left = 280
          Top = 11
          Width = 123
          Height = 25
          Caption = 'Tester TCP Show'
          TabOrder = 0
          OnClick = btTesterTCPShowClick
        end
      end
      object mmTesterLog: TMemo
        Left = 0
        Top = 49
        Width = 650
        Height = 272
        Align = alTop
        Color = 14670284
        Lines.Strings = (
          '')
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object rgUnloader: TRadioGroup
        Left = 0
        Top = 462
        Width = 650
        Height = 71
        Align = alBottom
        Columns = 4
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ItemIndex = 1
        Items.Strings = (
          'All'
          'Auto 1'
          'Auto 2'
          'Auto 3')
        ParentFont = False
        TabOrder = 2
        OnClick = rgUnloaderClick
      end
    end
    object ts_ATC6_1: TTabSheet
      Caption = 'ATC 6.1'
      ImageIndex = 23
      object pnl_ATC61: TPanel
        Left = 0
        Top = 0
        Width = 650
        Height = 533
        Align = alClient
        Color = 12761254
        TabOrder = 0
        object lblDryAirSwSts: TLabel
          Left = 12
          Top = 145
          Width = 190
          Height = 20
          AutoSize = False
          Caption = 'DryAir EnergySaving : OFF'
          Color = 12761254
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = #26032#32048#26126#39636
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object lblTesterDryAirSwSts: TLabel
          Left = 220
          Top = 145
          Width = 190
          Height = 20
          AutoSize = False
          Caption = 'Tester Purge Control :  OFF'
          Color = 12761254
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clBlack
          Font.Height = -15
          Font.Name = #26032#32048#26126#39636
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object Panel34: TPanel
          Left = 1
          Top = 1
          Width = 648
          Height = 25
          Align = alTop
          BevelOuter = bvNone
          Caption = 'ATC 6.1 Monitor'
          Color = 9534289
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object grp_DewPoint_1032: TGroupBox
          Left = 8
          Top = 28
          Width = 261
          Height = 109
          Caption = 'DewPoint'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label52: TLabel
            Left = 233
            Top = 24
            Width = 16
            Height = 20
            Caption = #176'C'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label57: TLabel
            Left = 233
            Top = 50
            Width = 16
            Height = 20
            Caption = #176'C'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label58: TLabel
            Left = 233
            Top = 78
            Width = 16
            Height = 20
            Caption = #176'C'
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object hlpal1: TPanel
            Left = 10
            Top = 48
            Width = 135
            Height = 25
            Alignment = taLeftJustify
            BevelInner = bvLowered
            Caption = 'DewPoint  Index'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object pnl_DewPoint_InArm: TPanel
            Left = 146
            Top = 20
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = '123.34'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 1
          end
          object hlpal2: TPanel
            Left = 10
            Top = 76
            Width = 135
            Height = 25
            Alignment = taLeftJustify
            BevelInner = bvLowered
            Caption = 'DewPoint  OutArm'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object pnl_DewPoint_OutArm: TPanel
            Left = 146
            Top = 76
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = '123.34'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 5
          end
          object hlpal3: TPanel
            Left = 10
            Top = 20
            Width = 135
            Height = 25
            Alignment = taLeftJustify
            BevelInner = bvLowered
            Caption = 'DewPoint  InArm'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object pnl_DewPoint_Index: TPanel
            Left = 146
            Top = 48
            Width = 80
            Height = 25
            BevelInner = bvLowered
            Caption = '123.34'
            Color = 12761254
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            ParentShowHint = False
            ShowHint = True
            TabOrder = 3
          end
        end
        object palATC61Status: TPanel
          Left = 276
          Top = 38
          Width = 105
          Height = 33
          BevelInner = bvLowered
          Caption = 'ATC OnLine'
          Color = clLime
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = pl_ATC_OnlineClick
        end
        object pgcATC61: TPageControl
          Left = 1
          Top = 179
          Width = 360
          Height = 357
          ActivePage = ts1
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Style = tsButtons
          TabIndex = 0
          TabOrder = 3
          object ts1: TTabSheet
            Caption = 'Air Stream Status'
            ImageIndex = 2
            object grp_AirStreamStatus: TGroupBox
              Left = 0
              Top = 0
              Width = 352
              Height = 281
              Align = alTop
              Caption = 'Status'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              object lblAirStreamStatus: TLabel
                Left = 10
                Top = 50
                Width = 47
                Height = 20
                Caption = 'Status'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label60: TLabel
                Left = 10
                Top = 83
                Width = 120
                Height = 20
                Caption = 'Set Temperature'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl5: TLabel
                Left = 10
                Top = 116
                Width = 92
                Height = 20
                Caption = 'Alarm Status'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl6: TLabel
                Left = 10
                Top = 149
                Width = 76
                Height = 20
                Caption = 'Air Defrost'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl7: TLabel
                Left = 316
                Top = 84
                Width = 16
                Height = 20
                Caption = #176'C'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl12: TLabel
                Left = 10
                Top = 182
                Width = 77
                Height = 20
                Caption = 'Temp Ch 1'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl13: TLabel
                Left = 10
                Top = 215
                Width = 77
                Height = 20
                Caption = 'Temp Ch 2'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl14: TLabel
                Left = 316
                Top = 188
                Width = 16
                Height = 20
                Caption = #176'C'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl15: TLabel
                Left = 316
                Top = 222
                Width = 16
                Height = 20
                Caption = #176'C'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object lbl16: TLabel
                Left = 10
                Top = 248
                Width = 77
                Height = 20
                Caption = 'Air Volume'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label61: TLabel
                Left = 165
                Top = 18
                Width = 39
                Height = 20
                Caption = 'Index'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label62: TLabel
                Left = 251
                Top = 18
                Width = 50
                Height = 20
                Caption = 'Socket'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object Label65: TLabel
                Left = 316
                Top = 149
                Width = 28
                Height = 20
                Caption = 'Sec'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
              end
              object palAirDefrostSec: TPanel
                Left = 243
                Top = 146
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 6
              end
              object palAirMachineAlarmStatus: TPanel
                Left = 243
                Top = 112
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 4
              end
              object palAirMachineSetTemperature: TPanel
                Left = 243
                Top = 78
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 2
              end
              object palAirMachineStatus: TPanel
                Left = 243
                Top = 44
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 0
              end
              object pal_Air_Machine_Temp_Ch1: TPanel
                Left = 243
                Top = 180
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 8
              end
              object pal_Air_Machine_Temp_Ch2: TPanel
                Left = 243
                Top = 214
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 10
              end
              object pal_AirStream_AirVolume: TPanel
                Left = 243
                Top = 248
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 12
              end
              object palAirMachineStatus_Index: TPanel
                Left = 149
                Top = 45
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 1
              end
              object palAirMachineSetTemperature_Index: TPanel
                Left = 149
                Top = 79
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 3
              end
              object palAirMachineAlarmStatus_Index: TPanel
                Left = 149
                Top = 113
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 5
              end
              object palAirDefrostSec_Index: TPanel
                Left = 149
                Top = 147
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 7
              end
              object pnl_AirMachineTemp_Index_Ch_1: TPanel
                Left = 149
                Top = 181
                Width = 70
                Height = 26
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 9
              end
              object pnl_AirMachineTemp_Index_Ch_2: TPanel
                Left = 149
                Top = 215
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 11
              end
              object pal_AirStream_AirVolume_Index: TPanel
                Left = 149
                Top = 249
                Width = 70
                Height = 26
                Hint = 'Click here to switch count and percent'
                BevelInner = bvLowered
                Caption = '0'
                Color = clWhite
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Times New Roman'
                Font.Style = [fsBold]
                ParentFont = False
                ParentShowHint = False
                ShowHint = True
                TabOrder = 13
              end
            end
            object btnAirStreamOnOff: TButton
              Tag = 5
              Left = 9
              Top = 290
              Width = 130
              Height = 30
              Hint = 'Manual On /Off'
              Caption = 'Air Stream Stop'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 1
              OnClick = btnAirStreamOnOffClick
            end
            object chk_ManualControl_AirStream: TCheckBox
              Left = 160
              Top = 296
              Width = 121
              Height = 17
              Caption = 'Manual Control'
              TabOrder = 2
              Visible = False
            end
          end
          object ts_RefrigerantStatus: TTabSheet
            Caption = 'Refrigerant Status'
            ImageIndex = 3
            object PageControl2: TPageControl
              Left = 0
              Top = 0
              Width = 352
              Height = 323
              ActivePage = ts_RefrigerantStatus_Page_1
              Align = alClient
              Style = tsFlatButtons
              TabIndex = 0
              TabOrder = 0
              object ts_RefrigerantStatus_Page_1: TTabSheet
                Caption = 'Page 1'
                object labRefrigerantMachineLowLimit: TLabel
                  Left = 1
                  Top = 269
                  Width = 232
                  Height = 20
                  Caption = 'Comp#2 Hp Over Low  Limit :'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'MS Sans Serif'
                  Font.Style = [fsBold]
                  ParentFont = False
                end
                object labRefrigerantMachineHighLimit: TLabel
                  Left = 1
                  Top = 247
                  Width = 232
                  Height = 20
                  Caption = 'Comp#2 Hp Over High Limit :'
                  Font.Charset = DEFAULT_CHARSET
                  Font.Color = clWindowText
                  Font.Height = -16
                  Font.Name = 'MS Sans Serif'
                  Font.Style = [fsBold]
                  ParentFont = False
                end
                object pnlRefrigerantMachine2: TPanel
                  Left = 160
                  Top = 4
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 0
                  object gbRefrigerantMachine2: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 02'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue2: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue2: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_2: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_2: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox10: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_2: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_2: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox11: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_2: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_2: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine3: TPanel
                  Left = 4
                  Top = 120
                  Width = 148
                  Height = 112
                  Color = clGray
                  TabOrder = 1
                  object gbRefrigerantMachine3: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 03'
                    Color = 12761254
                    DockSite = True
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue3: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue3: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_3: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_3: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox12: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_3: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_3: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox13: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_3: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_3: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine1: TPanel
                  Left = 4
                  Top = 4
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 2
                  object gbRefrigerantMachine1: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 01'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue1: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue1: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_1: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_1: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGreen
                      TabOrder = 1
                    end
                    object GroupBox8: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_1: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_1: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox9: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_1: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_1: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine4: TPanel
                  Left = 160
                  Top = 120
                  Width = 148
                  Height = 112
                  Color = clGray
                  TabOrder = 3
                  object gbRefrigerantMachine4: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 04'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue4: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue4: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_4: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_4: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox15: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_4: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_4: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox17: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_4: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_4: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
              end
              object ts_RefrigerantStatus_Page_2: TTabSheet
                Caption = 'Page 2'
                ImageIndex = 1
                object pnlRefrigerantMachine5: TPanel
                  Left = 4
                  Top = 4
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 0
                  object gbRefrigerantMachine5: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 05'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue5: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue5: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_5: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_5: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox14: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_5: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_5: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox16: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_5: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_5: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine6: TPanel
                  Left = 160
                  Top = 4
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 1
                  object gbRefrigerantMachine6: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 06'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue6: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue6: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_6: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_6: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox18: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_6: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_6: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox19: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_6: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_6: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine7: TPanel
                  Left = 4
                  Top = 120
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 2
                  object gbRefrigerantMachine7: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 07'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue7: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue7: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_7: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_7: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox20: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_7: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_7: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox21: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_7: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_7: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
                object pnlRefrigerantMachine8: TPanel
                  Left = 160
                  Top = 120
                  Width = 148
                  Height = 112
                  Color = clGreen
                  TabOrder = 3
                  object gbRefrigerantMachine8: TGroupBox
                    Left = 6
                    Top = 6
                    Width = 136
                    Height = 100
                    Caption = 'No . 08'
                    Color = 12761254
                    Font.Charset = DEFAULT_CHARSET
                    Font.Color = clWindowText
                    Font.Height = -11
                    Font.Name = 'MS Sans Serif'
                    Font.Style = [fsBold]
                    ParentColor = False
                    ParentFont = False
                    TabOrder = 0
                    object LabRefrigerantValue8: TLabel
                      Left = 8
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object LabRefrigerantAdjustValue8: TLabel
                      Left = 104
                      Top = 15
                      Width = 25
                      Height = 16
                      Caption = '120'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -13
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                    end
                    object pnlRefCopm1Status_8: TPanel
                      Left = 5
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 0
                    end
                    object pnlRefCopm2Status_8: TPanel
                      Left = 71
                      Top = 30
                      Width = 60
                      Height = 10
                      Color = clGray
                      TabOrder = 1
                    end
                    object GroupBox22: TGroupBox
                      Left = 4
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #1'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 2
                      object labRefCopm1HpValue_8: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm1LpValue_8: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                    object GroupBox23: TGroupBox
                      Left = 70
                      Top = 40
                      Width = 62
                      Height = 55
                      Caption = 'Comp #2'
                      Font.Charset = DEFAULT_CHARSET
                      Font.Color = clWindowText
                      Font.Height = -11
                      Font.Name = 'MS Sans Serif'
                      Font.Style = [fsBold]
                      ParentFont = False
                      TabOrder = 3
                      object labRefCopm2HpValue_8: TLabel
                        Left = 10
                        Top = 15
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clMaroon
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                      object labRefCopm2LpValue_8: TLabel
                        Left = 10
                        Top = 35
                        Width = 37
                        Height = 16
                        Caption = '120.1'
                        Font.Charset = DEFAULT_CHARSET
                        Font.Color = clGray
                        Font.Height = -13
                        Font.Name = 'MS Sans Serif'
                        Font.Style = [fsBold]
                        ParentFont = False
                      end
                    end
                  end
                end
              end
            end
          end
        end
      end
    end
    object tsBundle: TTabSheet
      Caption = 'Bundle'
      ImageIndex = 24
      object ALed5: TALed
        Left = 6
        Top = 81
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object ALed6: TALed
        Left = 6
        Top = 89
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object ALed7: TALed
        Left = 6
        Top = 105
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object Panel36: TPanel
        Left = 1
        Top = 0
        Width = 392
        Height = 241
        BevelInner = bvLowered
        BevelOuter = bvSpace
        ParentColor = True
        TabOrder = 0
        object labNowAuto1TrayID: TLabel
          Left = 120
          Top = 28
          Width = 3
          Height = 13
        end
        object Label67: TLabel
          Left = 26
          Top = 81
          Width = 100
          Height = 13
          Caption = 'Auto 1 Now Tray ID :'
        end
        object Label55: TLabel
          Left = 26
          Top = 95
          Width = 100
          Height = 13
          Caption = 'Auto 2 Now Tray ID :'
        end
        object labNowAuto2TrayID: TLabel
          Left = 123
          Top = 74
          Width = 3
          Height = 13
        end
        object Label70: TLabel
          Left = 26
          Top = 110
          Width = 100
          Height = 13
          Caption = 'Auto 3 Now Tray ID :'
        end
        object labNowAuto3TrayID: TLabel
          Left = 120
          Top = 57
          Width = 3
          Height = 13
        end
        object Label72: TLabel
          Left = 27
          Top = 127
          Width = 72
          Height = 13
          Caption = 'Auto 1 Tray ID:'
        end
        object labAuto1TrayID: TLabel
          Left = 100
          Top = 127
          Width = 3
          Height = 13
        end
        object Label74: TLabel
          Left = 27
          Top = 142
          Width = 72
          Height = 13
          Caption = 'Auto 2 Tray ID:'
        end
        object labAuto2TrayID: TLabel
          Left = 100
          Top = 142
          Width = 3
          Height = 13
        end
        object Label76: TLabel
          Left = 27
          Top = 156
          Width = 72
          Height = 13
          Caption = 'Auto 3 Tray ID:'
        end
        object labAuto3TrayID: TLabel
          Left = 100
          Top = 156
          Width = 3
          Height = 13
        end
        object lbl3: TLabel
          Left = 218
          Top = 83
          Width = 101
          Height = 13
          Caption = 'Auto 1 Tray Count    :'
        end
        object labAuto1TrayCount: TLabel
          Left = 337
          Top = 27
          Width = 3
          Height = 13
        end
        object Label56: TLabel
          Left = 218
          Top = 98
          Width = 101
          Height = 13
          Caption = 'Auto 2 Tray Count    :'
        end
        object labAuto2TrayCount: TLabel
          Left = 337
          Top = 42
          Width = 3
          Height = 13
        end
        object Label59: TLabel
          Left = 218
          Top = 113
          Width = 101
          Height = 13
          Caption = 'Auto 3 Tray Count    :'
        end
        object labAuto3TrayCount: TLabel
          Left = 337
          Top = 57
          Width = 3
          Height = 13
        end
        object lbl10: TLabel
          Left = 144
          Top = 172
          Width = 92
          Height = 13
          Caption = 'Empty Tray Enable:'
        end
        object lbl11: TLabel
          Left = 270
          Top = 172
          Width = 87
          Height = 13
          Caption = 'Color Tray Enable:'
        end
        object labLoaderEnable: TLabel
          Left = 125
          Top = 173
          Width = 3
          Height = 13
        end
        object labEmptyEnable: TLabel
          Left = 239
          Top = 173
          Width = 3
          Height = 13
        end
        object lablbl9: TLabel
          Left = 26
          Top = 172
          Width = 96
          Height = 13
          Caption = 'Loader Tray Enable:'
        end
        object lab46: TLabel
          Left = 26
          Top = 188
          Width = 100
          Height = 13
          Caption = 'Lot Tray Count         :'
        end
        object labLotTrayCount: TLabel
          Left = 129
          Top = 189
          Width = 3
          Height = 13
        end
        object labLotFinishTrayCount: TLabel
          Left = 250
          Top = 187
          Width = 3
          Height = 13
        end
        object lab47: TLabel
          Left = 144
          Top = 187
          Width = 103
          Height = 13
          Caption = 'Finish Tray  Count     :'
        end
        object labLoader: TLabel
          Left = 8
          Top = 33
          Width = 100
          Height = 13
          Caption = 'Loader last bundle ID:'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object labLoaderBundleID: TLabel
          Left = 128
          Top = 33
          Width = 100
          Height = 13
          Caption = 'Loader last bundle ID:'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lbl4: TLabel
          Left = 8
          Top = 49
          Width = 96
          Height = 13
          Caption = 'Loadercar bundle ID:'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblLoaderCarBundleID: TLabel
          Left = 120
          Top = 49
          Width = 100
          Height = 13
          Caption = 'Loader last bundle ID:'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Calibri'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object lblAutoCount: TLabel
          Left = 322
          Top = 83
          Width = 6
          Height = 13
          Caption = '0'
        end
        object lblAutoCount2: TLabel
          Left = 322
          Top = 98
          Width = 6
          Height = 13
          Caption = '0'
        end
        object lblAutoCount3: TLabel
          Left = 322
          Top = 113
          Width = 6
          Height = 13
          Caption = '0'
        end
        object aLedAuto1: TALed
          Left = 2
          Top = 81
          Width = 22
          Height = 14
          TrueColor = clRed
          LEDStyle = LEDHorizontal
        end
        object aLedAuto2: TALed
          Left = 2
          Top = 95
          Width = 22
          Height = 14
          TrueColor = clRed
          LEDStyle = LEDHorizontal
        end
        object aLedAuto3: TALed
          Left = 2
          Top = 109
          Width = 22
          Height = 14
          TrueColor = clRed
          LEDStyle = LEDHorizontal
        end
        object ALedLoader: TALed
          Left = 2
          Top = 172
          Width = 22
          Height = 14
          TrueColor = clRed
          LEDStyle = LEDHorizontal
        end
        object Panel37: TPanel
          Left = 2
          Top = 2
          Width = 388
          Height = 23
          Align = alTop
          BevelOuter = bvNone
          Caption = 'Lot Info'
          Color = 7913976
          Font.Charset = ANSI_CHARSET
          Font.Color = clRed
          Font.Height = -19
          Font.Name = 'Courier New'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object palClearLotInfo: TPanel
          Left = 275
          Top = 210
          Width = 113
          Height = 25
          Caption = 'Clear Lot Info'
          Color = clSkyBlue
          TabOrder = 1
          Visible = False
        end
        object palRemoveTray: TPanel
          Left = 3
          Top = 211
          Width = 134
          Height = 25
          BiDiMode = bdLeftToRight
          Caption = 'Manual Remove Tray'
          Color = clLime
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentBiDiMode = False
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
      end
    end
    object tsSetupFileCheck: TTabSheet
      Caption = 'Setup File Check'
      ImageIndex = 23
      object Label1: TLabel
        Left = 16
        Top = 16
        Width = 100
        Height = 25
        Caption = 'Device ID : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object edDeviceID: TEdit
        Left = 117
        Top = 16
        Width = 140
        Height = 21
        Hint = 'Can not use ([\/:*?"<>|])'
        Color = 14670284
        TabOrder = 0
        OnKeyDown = edtSysLotIDKeyDown
        OnKeyPress = edtSysLotIDKeyPress
        OnKeyUp = edtSysLotIDKeyUp
      end
    end
    object tsAMR: TTabSheet
      Caption = 'AMR'
      ImageIndex = 24
      object lblAMRLoaderTotal: TLabel
        Left = 300
        Top = 26
        Width = 30
        Height = 13
        Caption = 'Total'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblAMRCnt: TLabel
        Left = 244
        Top = 26
        Width = 34
        Height = 13
        Caption = 'Count'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblAMRSet: TLabel
        Left = 188
        Top = 26
        Width = 41
        Height = 13
        Caption = 'Setting'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object aldLoaderLast: TALed
        Left = 151
        Top = 180
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object aldWaitTrayFeed: TALed
        Left = 151
        Top = 152
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object aldWaitRXAuto3: TALed
        Left = 151
        Top = 125
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object aldWaitRXAuto2: TALed
        Left = 151
        Top = 98
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object aldWaitRXAuto1: TALed
        Left = 151
        Top = 72
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object aldWaitTXLoader: TALed
        Left = 151
        Top = 46
        Width = 22
        Height = 14
        LEDStyle = LEDHorizontal
      end
      object cbA60_1: TCheckBox
        Left = 13
        Top = 4
        Width = 100
        Height = 29
        Caption = 'Enable AMR'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
      end
      object pnlWaitTXTotalLoader: TPanel
        Left = 290
        Top = 43
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
        TabOrder = 1
      end
      object pnlWaitTXSetLoader: TPanel
        Left = 184
        Top = 43
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
        TabOrder = 2
      end
      object pnlWaitTXCntLoader: TPanel
        Left = 234
        Top = 43
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
        TabOrder = 3
      end
      object pnlWaitRXCntAuto1: TPanel
        Left = 234
        Top = 68
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
        TabOrder = 4
      end
      object pnlWaitRXSetAuto1: TPanel
        Left = 184
        Top = 69
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
        TabOrder = 5
      end
      object pnlWaitRXSetAuto2: TPanel
        Left = 184
        Top = 95
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
      object pnlWaitRXCntAuto2: TPanel
        Left = 234
        Top = 94
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
      object pnlWaitRXCntAuto3: TPanel
        Left = 234
        Top = 121
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
        TabOrder = 8
      end
      object pnlWaitRXSetAuto3: TPanel
        Left = 184
        Top = 122
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
        TabOrder = 9
      end
      object pnlWaitTXLoader: TPanel
        Left = 4
        Top = 43
        Width = 141
        Height = 20
        Caption = 'Waitting for TX Loader'
        Color = 5812224
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 10
      end
      object pnlWaitRXAuto1: TPanel
        Left = 4
        Top = 70
        Width = 141
        Height = 20
        Caption = 'Waitting for RX Auto1'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 11
      end
      object pnlWaitRXAuto2: TPanel
        Left = 4
        Top = 96
        Width = 141
        Height = 20
        Caption = 'Waitting for RX Auto2'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 12
      end
      object pnlWaitRXAuto3: TPanel
        Left = 4
        Top = 123
        Width = 141
        Height = 20
        Caption = 'Waitting for RX Auto3'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 13
      end
      object pnlWaitTrayFeed: TPanel
        Left = 4
        Top = 150
        Width = 141
        Height = 20
        Caption = 'Waitting for Tray Feed'
        Color = clSkyBlue
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
      end
      object pnlLoaderLast: TPanel
        Left = 4
        Top = 179
        Width = 141
        Height = 20
        Caption = 'Loader Last'
        Color = 5812224
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 15
      end
      object Panel2: TPanel
        Left = 8
        Top = 208
        Width = 257
        Height = 321
        Caption = 'Panel35'
        TabOrder = 16
        object StrGrdCategory: TStringGrid
          Left = 1
          Top = 1
          Width = 255
          Height = 319
          TabStop = False
          Align = alClient
          ColCount = 3
          DefaultColWidth = 76
          DefaultRowHeight = 16
          RowCount = 16
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Times New Roman'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          RowHeights = (
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16
            16)
        end
      end
    end
    object tsOtherTool: TTabSheet
      Caption = 'Other Tool'
      ImageIndex = 25
      object GroupBox6: TGroupBox
        Left = 0
        Top = 0
        Width = 417
        Height = 73
        Caption = 'First Tray Check On Unloader'
        TabOrder = 0
        object cbFirstTrayCheckOnUnloader: TCheckBox
          Left = 8
          Top = 24
          Width = 217
          Height = 17
          Caption = 'First Tray Check On Unloader'
          TabOrder = 0
          OnClick = cbFirstTrayCheckOnUnloaderClick
          OnMouseDown = cbFirstTrayCheckOnUnloaderMouseDown
        end
        object cb1stCheck_Auto1: TCheckBox
          Left = 28
          Top = 48
          Width = 117
          Height = 17
          Caption = 'Auto1 Position'
          Enabled = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 1
        end
        object cb1stCheck_Auto2: TCheckBox
          Left = 152
          Top = 48
          Width = 117
          Height = 17
          Caption = 'Auto2 Position'
          Enabled = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 2
        end
        object cb1stCheck_Auto3: TCheckBox
          Left = 276
          Top = 48
          Width = 117
          Height = 17
          Caption = 'Auto3 Position'
          Enabled = False
          ParentShowHint = False
          ShowHint = True
          TabOrder = 3
        end
      end
      object GroupBox7: TGroupBox
        Left = 0
        Top = 80
        Width = 417
        Height = 49
        Caption = 'Loader count and Alarm'
        TabOrder = 1
        object Label2: TLabel
          Left = 8
          Top = 24
          Width = 57
          Height = 13
          Caption = 'Now counts'
        end
        object Label3: TLabel
          Left = 208
          Top = 24
          Width = 61
          Height = 13
          Caption = 'Alarm counts'
        end
        object edLoaderCountNow: TEdit
          Left = 72
          Top = 16
          Width = 121
          Height = 21
          Enabled = False
          TabOrder = 0
          Text = '0'
        end
        object edLoaderCountAlarm: TEdit
          Left = 272
          Top = 16
          Width = 121
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
      end
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 100
    OnTimer = Timer1Timer
    Left = 592
    Top = 24
  end
  object Timer2: TTimer
    OnTimer = Timer2Timer
    Left = 464
    Top = 532
  end
  object Timer3: TTimer
    Enabled = False
    OnTimer = Timer3Timer
    Left = 564
    Top = 24
  end
  object NetATCTime: TTimer
    Enabled = False
    Interval = 200
    OnTimer = NetATCTimeTimer
    Left = 436
    Top = 532
  end
  object TimerERMS: TTimer
    Enabled = False
    Interval = 200
    OnTimer = TimerERMSTimer
    Left = 408
    Top = 532
  end
  object LotKeyInTime: TTimer
    Enabled = False
    Interval = 50
    OnTimer = LotKeyInTimeTimer
    Left = 380
    Top = 532
  end
  object tmrChamberBoost: TTimer
    Enabled = False
    Interval = 50
    OnTimer = tmrChamberBoostTimer
    Left = 352
    Top = 532
  end
  object RFID_Reader: TComm
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
    ReadIntervalTimeout = 700
    ReadTotalTimeoutMultiplier = 10
    ReadTotalTimeoutConstant = 1000
    WriteTotalTimeoutMultiplier = 10
    WriteTotalTimeoutConstant = 1000
    OnReceiveData = RFID_ReaderReceiveData
    Left = 296
    Top = 532
  end
  object OpenDialog1: TOpenDialog
    Filter = '*.csv|*.csv'
    InitialDir = 'D:\'
    Title = 'Open DIO File'
    Left = 323
    Top = 532
  end
end
