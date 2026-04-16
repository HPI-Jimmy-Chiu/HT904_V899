object fObserver: TfObserver
  Left = 207
  Top = 143
  BorderStyle = bsToolWindow
  Caption = 'Observer'
  ClientHeight = 775
  ClientWidth = 932
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
  object labDeviceName: TPanel
    Left = 0
    Top = 0
    Width = 944
    Height = 29
    BevelInner = bvLowered
    Font.Charset = ANSI_CHARSET
    Font.Color = clWhite
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object pgcObserv: TPageControl
    Left = 0
    Top = 0
    Width = 932
    Height = 738
    ActivePage = tsCounter
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MultiLine = True
    ParentFont = False
    TabIndex = 0
    TabOrder = 0
    TabWidth = 119
    OnChange = pgcObservChange
    object tsCounter: TTabSheet
      Caption = 'Counter'
      object Label1: TLabel
        Left = 14
        Top = 668
        Width = 76
        Height = 13
        Caption = 'Clear Data Time'
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 289
        Width = 924
        Height = 161
        Align = alTop
        Caption = 'Contact Count (Kinds)'
        Color = 12761254
        ParentColor = False
        TabOrder = 1
        object Label16: TLabel
          Left = 700
          Top = 30
          Width = 24
          Height = 13
          Caption = 'Total'
        end
        object StringGrid2: TStringGrid
          Left = 8
          Top = 61
          Width = 901
          Height = 95
          Color = 14670284
          ColCount = 17
          DefaultColWidth = 47
          DefaultRowHeight = 20
          FixedColor = 9534289
          FixedCols = 0
          RowCount = 4
          FixedRows = 0
          TabOrder = 3
          OnDrawCell = StringGrid2DrawCell
          OnMouseDown = StringGrid2MouseDown
        end
        object rgContactCountKinds: TRadioGroup
          Left = 8
          Top = 16
          Width = 300
          Height = 36
          Caption = 'Raw No.'
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            'Row-A'
            'Row-B'
            'Row-C'
            'Row-D')
          TabOrder = 0
          OnClick = rgContactCountKindsClick
        end
        object rgContactCountKindsForm: TRadioGroup
          Left = 322
          Top = 16
          Width = 200
          Height = 36
          Caption = 'Display Form'
          Columns = 3
          ItemIndex = 0
          Items.Strings = (
            'Total'
            'Kinds'
            'Kinds(%)')
          TabOrder = 1
          OnClick = rgContactCountKindsFormClick
        end
        object APHeadLabel13: TPanel
          Left = 735
          Top = 26
          Width = 160
          Height = 20
          BevelInner = bvLowered
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
      end
      object GroupBox5: TGroupBox
        Left = 0
        Top = 450
        Width = 924
        Height = 161
        Align = alTop
        Caption = 'Contact Count (History)'
        Color = 12761254
        ParentColor = False
        TabOrder = 2
        object Label18: TLabel
          Left = 700
          Top = 33
          Width = 24
          Height = 13
          Caption = 'Total'
        end
        object Image1: TImage
          Left = 628
          Top = 24
          Width = 57
          Height = 29
          OnDblClick = Image1DblClick
        end
        object StringGrid3: TStringGrid
          Left = 8
          Top = 61
          Width = 901
          Height = 95
          Color = 14670284
          ColCount = 9
          DefaultColWidth = 85
          DefaultRowHeight = 20
          FixedColor = 9534289
          FixedCols = 0
          RowCount = 4
          FixedRows = 0
          TabOrder = 3
          OnDrawCell = StringGrid3DrawCell
          OnMouseDown = StringGrid3MouseDown
        end
        object rgContactCountHistory: TRadioGroup
          Left = 8
          Top = 20
          Width = 300
          Height = 36
          Caption = 'Raw No.'
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            'Row-A'
            'Row-B'
            'Row-C'
            'Row-D')
          TabOrder = 0
          OnClick = rgContactCountHistoryClick
        end
        object rgContactCountHistoryForm: TRadioGroup
          Left = 334
          Top = 20
          Width = 200
          Height = 36
          Caption = 'Display Form'
          Columns = 3
          ItemIndex = 0
          Items.Strings = (
            'Total'
            'Kinds'
            'Kinds(%)')
          TabOrder = 1
          OnClick = rgContactCountHistoryFormClick
        end
        object APHeadLabel14: TPanel
          Left = 735
          Top = 29
          Width = 160
          Height = 20
          BevelInner = bvLowered
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
      end
      object APHeadLabel18: TPanel
        Left = 99
        Top = 665
        Width = 160
        Height = 20
        BevelInner = bvLowered
        Color = 9534289
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
      end
      object pnlCounterTop: TPanel
        Left = 0
        Top = 0
        Width = 924
        Height = 289
        Align = alTop
        BevelOuter = bvNone
        BevelWidth = 2
        Color = 12761254
        TabOrder = 0
        object GroupBox1: TGroupBox
          Left = 0
          Top = 0
          Width = 299
          Height = 289
          Align = alLeft
          Caption = 'Operating Information'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          TabOrder = 0
          object Label2: TLabel
            Left = 22
            Top = 22
            Width = 75
            Height = 13
            Caption = 'Power ON Time'
          end
          object Label3: TLabel
            Left = 31
            Top = 53
            Width = 66
            Height = 13
            Caption = 'Running Time'
          end
          object Label4: TLabel
            Left = 34
            Top = 84
            Width = 63
            Height = 13
            Caption = 'Product Time'
          end
          object Label7: TLabel
            Left = 28
            Top = 116
            Width = 69
            Height = 13
            Caption = 'Loading Count'
          end
          object Label8: TLabel
            Left = 67
            Top = 147
            Width = 31
            Height = 13
            Caption = 'MUBA'
          end
          object Label9: TLabel
            Left = 67
            Top = 178
            Width = 30
            Height = 13
            Caption = 'MTBA'
          end
          object labDayJamRate: TLabel
            Left = 51
            Top = 239
            Width = 45
            Height = 13
            Caption = 'Jam Rate'
            Visible = False
          end
          object lblMTBF: TLabel
            Left = 67
            Top = 213
            Width = 29
            Height = 13
            Caption = 'MTBF'
          end
          object labPowerOnTime: TPanel
            Left = 111
            Top = 18
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object labRunningTime: TPanel
            Left = 111
            Top = 50
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object labProductTime: TPanel
            Left = 111
            Top = 81
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object labLoadingCount: TPanel
            Left = 111
            Top = 113
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object labMUBA: TPanel
            Left = 111
            Top = 144
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object labMTBA: TPanel
            Left = 111
            Top = 176
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object labMTBF: TPanel
            Left = 111
            Top = 208
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object pnlDayJamRate: TPanel
            Left = 111
            Top = 236
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Visible = False
          end
          object btnClearTime: TButton
            Left = 160
            Top = 260
            Width = 111
            Height = 25
            Caption = 'Clear Time Data'
            TabOrder = 8
            OnClick = btnClearTimeClick
          end
        end
        object GroupBox2: TGroupBox
          Left = 299
          Top = 0
          Width = 625
          Height = 289
          Align = alClient
          Caption = 'Version'
          Color = 12761254
          ParentColor = False
          TabOrder = 1
          object Label35: TLabel
            Left = 12
            Top = 16
            Width = 29
            Height = 13
            Caption = 'Model'
          end
          object Label38: TLabel
            Left = 12
            Top = 43
            Width = 46
            Height = 13
            Caption = 'Serial No.'
          end
          object Label53: TLabel
            Left = 380
            Top = 64
            Width = 65
            Height = 13
            Caption = 'Release Date'
          end
          object Label40: TLabel
            Left = 380
            Top = 16
            Width = 35
            Height = 13
            Caption = 'Factory'
          end
          object Label6: TLabel
            Left = 12
            Top = 70
            Width = 55
            Height = 13
            Caption = 'Machine ID'
          end
          object Label11: TLabel
            Left = 380
            Top = 40
            Width = 35
            Height = 13
            Caption = 'Version'
          end
          object Label10: TLabel
            Left = 12
            Top = 98
            Width = 63
            Height = 13
            Caption = 'GPIB Version'
          end
          object Label12: TLabel
            Left = 12
            Top = 125
            Width = 60
            Height = 13
            Caption = 'ESD Version'
          end
          object Label13: TLabel
            Left = 12
            Top = 152
            Width = 59
            Height = 13
            Caption = 'ATC Version'
          end
          object labModel: TPanel
            Left = 79
            Top = 12
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'HT-9045W'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object labSerialNo: TPanel
            Left = 79
            Top = 39
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = '28678'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object labReleaseDate: TPanel
            Left = 451
            Top = 60
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object labFactory: TPanel
            Left = 451
            Top = 12
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'Hon. Prec.'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object labMachineID: TPanel
            Left = 79
            Top = 66
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = '28678'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object labVersion: TPanel
            Left = 451
            Top = 36
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'Hisi_HT9045_V02.00.K006'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object Memo1: TMemo
            Left = 2
            Top = 176
            Width = 621
            Height = 111
            Align = alBottom
            Color = 14670284
            ScrollBars = ssVertical
            TabOrder = 11
          end
          object grpATCSerialNumber: TGroupBox
            Left = 248
            Top = 80
            Width = 361
            Height = 89
            Caption = 'ATC 3.1 SN/FN'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Visible = False
            object lblATCInPC1: TLabel
              Left = 6
              Top = 32
              Width = 36
              Height = 13
              Caption = 'ATC 1 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblATCInPC2: TLabel
              Left = 6
              Top = 46
              Width = 36
              Height = 13
              Caption = 'ATC 2 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblATCInPC4: TLabel
              Left = 6
              Top = 74
              Width = 36
              Height = 13
              Caption = 'ATC 4 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lblATCInPC3: TLabel
              Left = 6
              Top = 60
              Width = 36
              Height = 13
              Caption = 'ATC 3 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbSerialNumber04: TLabel
              Left = 50
              Top = 74
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbSerialNumber03: TLabel
              Left = 50
              Top = 60
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbSerialNumber02: TLabel
              Left = 50
              Top = 46
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbSerialNumber01: TLabel
              Left = 50
              Top = 32
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label14: TLabel
              Left = 166
              Top = 32
              Width = 36
              Height = 13
              Caption = 'ATC 1 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbFirmwareNumber01: TLabel
              Left = 210
              Top = 32
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label17: TLabel
              Left = 166
              Top = 46
              Width = 36
              Height = 13
              Caption = 'ATC 2 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbFirmwareNumber02: TLabel
              Left = 210
              Top = 46
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label20: TLabel
              Left = 166
              Top = 60
              Width = 36
              Height = 13
              Caption = 'ATC 3 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbFirmwareNumber03: TLabel
              Left = 210
              Top = 60
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label22: TLabel
              Left = 166
              Top = 74
              Width = 36
              Height = 13
              Caption = 'ATC 4 :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clNavy
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object lbFirmwareNumber04: TLabel
              Left = 210
              Top = 74
              Width = 20
              Height = 13
              Caption = 'N/A'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -11
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label30: TLabel
              Left = 6
              Top = 16
              Width = 86
              Height = 16
              Caption = 'Serial Number'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object Label31: TLabel
              Left = 166
              Top = 16
              Width = 106
              Height = 16
              Caption = 'Firmware Number'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clRed
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
          end
          object pnlGPIBVersion: TPanel
            Left = 79
            Top = 94
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'NA'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object pnlESDVersion: TPanel
            Left = 79
            Top = 121
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'NA'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object pnlATCVersion: TPanel
            Left = 79
            Top = 148
            Width = 160
            Height = 20
            BevelInner = bvLowered
            Caption = 'NA'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object pnlTTLRS232Version: TPanel
            Left = 241
            Top = 94
            Width = 130
            Height = 20
            BevelInner = bvLowered
            Caption = 'NA'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
        end
      end
    end
    object tsTestCate: TTabSheet
      Caption = 'Tester Category'
      ImageIndex = 1
      object ScrollBox1: TScrollBox
        Left = 0
        Top = 0
        Width = 924
        Height = 692
        Align = alClient
        BorderStyle = bsNone
        Color = 12761254
        ParentColor = False
        TabOrder = 0
        object mtRowName: TTMyTray
          Left = 0
          Top = 100
          Width = 91
          Height = 66
          XItem = 1
          YItem = 1
          DirectStyle = csNull
        end
        object mtCategorySum: TTMyTray
          Left = 0
          Top = 160
          Width = 91
          Height = 154
          Color = 9534289
          XItem = 1
          YItem = 5
          DirectStyle = csNull
        end
        object myCategoryName: TTMyTray
          Left = 0
          Top = 310
          Width = 91
          Height = 1145
          Color = 9534289
          XItem = 1
          YItem = 60
          DirectStyle = csNull
        end
        object mtDutName: TTMyTray
          Left = 88
          Top = 100
          Width = 726
          Height = 33
          Color = 9534289
          XItem = 8
          YItem = 1
          DirectStyle = csNull
        end
        object mtArmName: TTMyTray
          Left = 88
          Top = 130
          Width = 726
          Height = 33
          Color = 9534289
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtHeadTotal: TTMyTray
          Left = 88
          Top = 160
          Width = 726
          Height = 33
          Color = clWhite
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtSockTotal: TTMyTray
          Left = 88
          Top = 190
          Width = 726
          Height = 33
          Color = clWhite
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtPassHead: TTMyTray
          Left = 88
          Top = 220
          Width = 726
          Height = 33
          Color = clWhite
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtPassSocket: TTMyTray
          Left = 88
          Top = 250
          Width = 726
          Height = 33
          Color = clWhite
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtIfError: TTMyTray
          Left = 88
          Top = 280
          Width = 726
          Height = 33
          Color = clWhite
          XItem = 16
          YItem = 1
          DirectStyle = csNull
        end
        object mtCategoryNo: TTMyTray
          Left = 88
          Top = 310
          Width = 726
          Height = 1145
          Color = clWhite
          XItem = 16
          YItem = 60
          DirectStyle = csNull
        end
        object GroupBox8: TGroupBox
          Tag = 3
          Left = 164
          Top = 4
          Width = 233
          Height = 60
          Caption = 'Display Form'
          TabOrder = 1
          object Label28: TLabel
            Left = 15
            Top = 17
            Width = 26
            Height = 13
            Caption = 'Head'
          end
          object Label29: TLabel
            Left = 15
            Top = 37
            Width = 34
            Height = 13
            Caption = 'Socket'
          end
          object rbHeadNumber: TRadioButton
            Left = 72
            Top = 15
            Width = 89
            Height = 17
            Caption = 'Real Number'
            Checked = True
            TabOrder = 0
            TabStop = True
            OnClick = rgRowNoClick
          end
          object rbSocketNumber: TRadioButton
            Left = 71
            Top = 35
            Width = 90
            Height = 17
            Caption = 'Real Number'
            ParentShowHint = False
            ShowHint = False
            TabOrder = 2
            TabStop = True
            OnClick = rgRowNoClick
          end
          object rbHeadPercent: TRadioButton
            Left = 177
            Top = 15
            Width = 40
            Height = 17
            Caption = '%'
            TabOrder = 1
            OnClick = rgRowNoClick
          end
          object rbSocketPercent: TRadioButton
            Left = 177
            Top = 35
            Width = 40
            Height = 17
            Caption = '%'
            TabOrder = 3
            OnClick = rgRowNoClick
          end
        end
        object rgRowNo: TRadioGroup
          Left = 8
          Top = 4
          Width = 145
          Height = 60
          Caption = 'Row No.'
          Columns = 2
          ItemIndex = 0
          Items.Strings = (
            'Row-A'
            'Row-B')
          TabOrder = 0
          OnClick = rgRowNoClick
        end
        object mtNo: TTMyTray
          Left = 0
          Top = 70
          Width = 91
          Height = 33
          XItem = 1
          YItem = 1
          DirectStyle = csNull
        end
        object mtChName: TTMyTray
          Left = 88
          Top = 70
          Width = 726
          Height = 33
          Color = 9534289
          XItem = 8
          YItem = 1
          DirectStyle = csNull
        end
        object mtTotalName: TTMyTray
          Left = 809
          Top = 69
          Width = 92
          Height = 93
          XItem = 1
          YItem = 1
          DirectStyle = csNull
        end
        object mtCategoryTotal: TTMyTray
          Left = 809
          Top = 310
          Width = 92
          Height = 1145
          Color = clWhite
          XItem = 1
          YItem = 60
          DirectStyle = csNull
        end
        object mtTotal: TTMyTray
          Left = 809
          Top = 158
          Width = 92
          Height = 156
          Color = clWhite
          XItem = 1
          YItem = 5
          DirectStyle = csNull
        end
      end
    end
    object tsScanner: TTabSheet
      Caption = 'Scanner Category'
      ImageIndex = 2
      object GroupBox7: TGroupBox
        Left = 156
        Top = 8
        Width = 233
        Height = 57
        Caption = 'Display Form'
        TabOrder = 1
        object Label23: TLabel
          Left = 15
          Top = 17
          Width = 26
          Height = 13
          Caption = 'Head'
        end
        object Label24: TLabel
          Left = 15
          Top = 37
          Width = 34
          Height = 13
          Caption = 'Socket'
        end
        object RadioButton9: TRadioButton
          Left = 71
          Top = 15
          Width = 90
          Height = 17
          Caption = 'Real Number'
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton10: TRadioButton
          Left = 71
          Top = 35
          Width = 90
          Height = 17
          Caption = 'Real Number'
          TabOrder = 2
        end
        object RadioButton11: TRadioButton
          Left = 177
          Top = 15
          Width = 40
          Height = 17
          Caption = '%'
          TabOrder = 1
        end
        object RadioButton12: TRadioButton
          Left = 177
          Top = 35
          Width = 40
          Height = 17
          Caption = '%'
          TabOrder = 3
        end
      end
      object GroupBox9: TGroupBox
        Left = 8
        Top = 8
        Width = 137
        Height = 57
        Caption = 'Raw No.'
        TabOrder = 0
        object RadioButton17: TRadioButton
          Left = 7
          Top = 15
          Width = 55
          Height = 17
          Caption = 'Row-A'
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object RadioButton18: TRadioButton
          Left = 7
          Top = 34
          Width = 55
          Height = 17
          Caption = 'Row-B'
          TabOrder = 2
        end
        object RadioButton19: TRadioButton
          Left = 71
          Top = 15
          Width = 55
          Height = 17
          Caption = 'Row-C'
          TabOrder = 1
        end
        object RadioButton20: TRadioButton
          Left = 71
          Top = 34
          Width = 55
          Height = 17
          Caption = 'Row-D'
          TabOrder = 3
        end
      end
      object StringGrid5: TStringGrid
        Left = 0
        Top = 139
        Width = 924
        Height = 553
        Align = alBottom
        Color = 14670284
        ColCount = 10
        DefaultColWidth = 80
        DefaultRowHeight = 20
        FixedColor = 9534289
        FixedCols = 0
        RowCount = 9
        FixedRows = 0
        TabOrder = 2
        OnDrawCell = StringGrid5DrawCell
      end
    end
    object tsMDBQuery: TTabSheet
      Caption = 'System Message'
      ImageIndex = 3
      object pgcMessage: TPageControl
        Left = 0
        Top = 0
        Width = 924
        Height = 692
        ActivePage = tsSGJamCount
        Align = alClient
        TabIndex = 3
        TabOrder = 0
        TabWidth = 100
        OnChange = pgcMessageChange
        object tsMDB: TTabSheet
          Caption = 'MDB'
          object Label25: TLabel
            Left = 336
            Top = 610
            Width = 60
            Height = 13
            Caption = 'Display Data'
          end
          object Label33: TLabel
            Left = 24
            Top = 614
            Width = 48
            Height = 13
            Caption = 'Start Date'
          end
          object Label57: TLabel
            Left = 24
            Top = 636
            Width = 48
            Height = 13
            Caption = 'Start Time'
          end
          object Label58: TLabel
            Left = 184
            Top = 614
            Width = 45
            Height = 13
            Caption = 'End Date'
          end
          object Label59: TLabel
            Left = 184
            Top = 636
            Width = 45
            Height = 13
            Caption = 'End Time'
          end
          object Label26: TLabel
            Left = 516
            Top = 638
            Width = 55
            Height = 13
            Caption = 'Total Count'
          end
          object Label5: TLabel
            Left = 515
            Top = 615
            Width = 60
            Height = 13
            Caption = 'Total Loader'
          end
          object lbltTotalLoader: TPanel
            Left = 576
            Top = 611
            Width = 105
            Height = 20
            BevelInner = bvLowered
            Caption = '0'
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
            OnMouseDown = lbltTotalLoaderMouseDown
          end
          object cbDisplayData: TComboBox
            Left = 336
            Top = 624
            Width = 169
            Height = 21
            ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
            ItemHeight = 13
            TabOrder = 9
            Text = 'Alarm History'
            OnChange = DateTimePicker1CloseUp
            Items.Strings = (
              'Event Log'
              'Process Record'
              'Message Record'
              'Motion Record'
              'Production Record'
              'Alarm History'
              'Alarm History (JAM Only)'
              'Alarm History (MES Only)'
              'Alarm History (WAR Only)'
              'Alarm Statistics (Jam Only)'
              'Alarm Statistics by Date (Jam Only)'
              'Jam Chart (Summary)'
              'Jam Chart (In Arm)'
              'Jam Chart (Out Arm)'
              'Jam Chart (Index Unit)'
              'Jam Chart (In Shuttle)'
              'Jam Chart (Out Shuttle)'
              'Alarm Code List'
              'Production Summary Report')
          end
          object DateTimePicker1: TDateTimePicker
            Left = 74
            Top = 610
            Width = 100
            Height = 21
            CalAlignment = dtaLeft
            Date = 40162.5441927662
            Time = 40162.5441927662
            DateFormat = dfShort
            DateMode = dmComboBox
            Kind = dtkDate
            ParseInput = False
            TabOrder = 6
            OnCloseUp = DateTimePicker1CloseUp
          end
          object DateTimePicker2: TDateTimePicker
            Left = 74
            Top = 632
            Width = 100
            Height = 21
            CalAlignment = dtaLeft
            Date = 40162.5441927662
            Time = 40162.5441927662
            DateFormat = dfShort
            DateMode = dmComboBox
            Kind = dtkTime
            ParseInput = False
            TabOrder = 10
          end
          object DateTimePicker3: TDateTimePicker
            Left = 232
            Top = 610
            Width = 100
            Height = 21
            CalAlignment = dtaLeft
            Date = 40162.5441927662
            Time = 40162.5441927662
            DateFormat = dfShort
            DateMode = dmComboBox
            Kind = dtkDate
            ParseInput = False
            TabOrder = 7
            OnCloseUp = DateTimePicker1CloseUp
          end
          object DateTimePicker4: TDateTimePicker
            Left = 232
            Top = 632
            Width = 100
            Height = 21
            CalAlignment = dtaLeft
            Date = 40162.5441927662
            Time = 40162.5441927662
            DateFormat = dfShort
            DateMode = dmComboBox
            Kind = dtkTime
            ParseInput = False
            TabOrder = 11
          end
          object btnQueryEventLog: TButton
            Left = 788
            Top = 608
            Width = 56
            Height = 45
            Caption = 'Query'
            TabOrder = 4
            OnClick = BtnQueryClick
          end
          object Button7: TButton
            Left = 848
            Top = 608
            Width = 60
            Height = 45
            Caption = 'Save'
            TabOrder = 5
            OnClick = BtnSaveClick
          end
          object btAutoSave: TButton
            Left = 688
            Top = 608
            Width = 85
            Height = 25
            Caption = 'AutoSaveTest'
            TabOrder = 3
            Visible = False
            OnClick = btAutoSaveClick
          end
          object CheckBox1: TCheckBox
            Left = 684
            Top = 636
            Width = 97
            Height = 17
            Caption = 'CheckBox1'
            TabOrder = 13
            Visible = False
          end
          object pnlTotalCount: TPanel
            Left = 575
            Top = 634
            Width = 106
            Height = 20
            BevelInner = bvLowered
            Color = 9534289
            Font.Charset = ANSI_CHARSET
            Font.Color = clWhite
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object strngrdMDBQuery: TStringGrid
            Left = 0
            Top = 0
            Width = 916
            Height = 601
            Align = alTop
            Color = 14670284
            ColCount = 9
            DefaultColWidth = 100
            DefaultRowHeight = 16
            FixedColor = 9534289
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing]
            TabOrder = 0
          end
          object Chart2: TChart
            Left = 46
            Top = 104
            Width = 850
            Height = 349
            BackWall.Brush.Color = clWhite
            BackWall.Color = 14670284
            MarginBottom = 3
            MarginTop = 3
            Title.Font.Charset = DEFAULT_CHARSET
            Title.Font.Color = clBlue
            Title.Font.Height = -16
            Title.Font.Name = 'Arial'
            Title.Font.Style = []
            Title.Text.Strings = (
              'Jam Chart')
            BackColor = 14670284
            Chart3DPercent = 5
            Legend.ColorWidth = 15
            Legend.TopPos = 18
            Color = 12761254
            TabOrder = 2
            object BarSeries2: TBarSeries
              Marks.ArrowLength = 20
              Marks.Style = smsValue
              Marks.Visible = True
              SeriesColor = 8404992
              Title = 'Jam'
              XValues.DateTime = False
              XValues.Name = 'X'
              XValues.Multiplier = 1
              XValues.Order = loAscending
              YValues.DateTime = False
              YValues.Name = 'Bar'
              YValues.Multiplier = 1
              YValues.Order = loNone
            end
          end
          object strngrdTemp: TStringGrid
            Left = 312
            Top = 88
            Width = 320
            Height = 120
            TabOrder = 1
            Visible = False
          end
        end
        object tsEventLogTxt: TTabSheet
          Caption = 'Text'
          ImageIndex = 1
          object strngrdEventLog: TStringGrid
            Left = 0
            Top = 117
            Width = 916
            Height = 547
            Align = alClient
            Color = 14670284
            ColCount = 9
            DefaultColWidth = 120
            DefaultRowHeight = 16
            FixedColor = 9534289
            FixedCols = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing]
            TabOrder = 1
          end
          object pnlEventLogTxt: TPanel
            Left = 0
            Top = 0
            Width = 916
            Height = 117
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object lblYear: TLabel
              Left = 4
              Top = 4
              Width = 22
              Height = 13
              Caption = 'Year'
            end
            object Label15: TLabel
              Left = 4
              Top = 32
              Width = 30
              Height = 13
              Caption = 'Month'
            end
            object Label19: TLabel
              Left = 4
              Top = 60
              Width = 22
              Height = 13
              Caption = 'Filter'
            end
            object cbbEventLogYear: TComboBox
              Left = 44
              Top = 0
              Width = 145
              Height = 21
              Color = 14670284
              ItemHeight = 13
              ItemIndex = 0
              TabOrder = 0
              Text = '2018'
              Items.Strings = (
                '2018')
            end
            object cbbMonth: TComboBox
              Left = 44
              Top = 28
              Width = 145
              Height = 21
              Color = 14670284
              ItemHeight = 13
              TabOrder = 2
              Text = '1'
              OnChange = cbbMonthChange
              Items.Strings = (
                '01'
                '02'
                '03'
                '04'
                '05'
                '06'
                '07'
                '08'
                '09'
                '10'
                '11'
                '12')
            end
            object lstEventLog: TListBox
              Left = 196
              Top = 0
              Width = 720
              Height = 117
              Align = alRight
              Color = 14670284
              ItemHeight = 13
              Items.Strings = (
                '3333'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1')
              TabOrder = 1
              OnClick = lstEventLogClick
            end
            object btnQueryEventLogTxt: TButton
              Left = 116
              Top = 80
              Width = 75
              Height = 33
              Caption = 'Query'
              TabOrder = 5
              OnClick = btnQueryEventLogTxtClick
            end
            object cbbFilter: TComboBox
              Left = 44
              Top = 56
              Width = 145
              Height = 21
              Color = 14670284
              ItemHeight = 13
              TabOrder = 3
              Text = 'All Data'
              Items.Strings = (
                'All Data'
                '0 Event'
                '1 Input Arm'
                '2 Output Arm'
                '3 Index Unit'
                '4 Input Shuttle'
                '5 Output Shuttle'
                '6 Empty Tray Arm'
                '7 Tester I/F'
                '8 Scanner'
                '9 Tray Loader'
                '10 Empty Tray'
                '11 Tray Unloader 1'
                '12 Tray Unloader 2'
                '13 Tray Unloader 3'
                '14 Color Tray'
                '15 Temp. Controller'
                '16 System'
                '17 Fix Tray 1'
                '18 Fix Tray 2'
                '19 Fix Tray 3'
                '20 ESD System'
                'Process'
                'Motion'
                'Message'
                'ChangeLog'
                'TimeData'
                'FTP'
                'Exception'
                'JAM only'
                'WAR only'
                'MES only')
            end
            object btnBackupLogYear: TButton
              Left = 16
              Top = 80
              Width = 75
              Height = 33
              Caption = 'Backup Log'
              TabOrder = 4
              OnClick = btnBackupLogYearClick
            end
          end
        end
        object tsMsgTimeData: TTabSheet
          Caption = 'Time Data'
          ImageIndex = 2
          object pnlTimeDataTop: TPanel
            Left = 0
            Top = 0
            Width = 916
            Height = 141
            Align = alTop
            BevelInner = bvLowered
            Color = 12761254
            TabOrder = 0
            object lstTimeData: TListBox
              Left = 194
              Top = 2
              Width = 720
              Height = 137
              Align = alRight
              Color = 14670284
              ItemHeight = 13
              Items.Strings = (
                '3333'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1'
                '1')
              TabOrder = 0
              OnClick = lstTimeDataClick
            end
            object btnTimeData: TButton
              Left = 136
              Top = 100
              Width = 75
              Height = 33
              Caption = 'Query'
              TabOrder = 1
              OnClick = btnQueryEventLogTxtClick
            end
          end
          object strngrdTimeData: TStringGrid
            Left = 0
            Top = 141
            Width = 916
            Height = 523
            Align = alClient
            Color = 14670284
            ColCount = 13
            DefaultColWidth = 68
            DefaultRowHeight = 16
            FixedColor = 9534289
            FixedCols = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing]
            TabOrder = 1
          end
        end
        object tsSGJamCount: TTabSheet
          Caption = 'SG_JamCount'
          ImageIndex = 3
          object Panel2: TPanel
            Left = 0
            Top = 0
            Width = 916
            Height = 105
            Align = alTop
            BevelOuter = bvNone
            Color = 12761254
            TabOrder = 0
            object labLoaderCount: TLabel
              Left = 124
              Top = 72
              Width = 114
              Height = 20
              BiDiMode = bdRightToLeft
              Caption = 'labLoaderCount'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentBiDiMode = False
              ParentFont = False
            end
            object Label21: TLabel
              Left = 8
              Top = 72
              Width = 105
              Height = 20
              Caption = 'Loader Count :'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
            end
            object btnSG_QueryNow: TButton
              Left = 824
              Top = 16
              Width = 97
              Height = 33
              Caption = 'Query Now'
              TabOrder = 0
              OnClick = btnSG_QueryNowClick
            end
            object btnSG_QueryYesterday: TButton
              Left = 826
              Top = 64
              Width = 95
              Height = 33
              Caption = 'Query Yesterday'
              TabOrder = 1
              OnClick = btnSG_QueryYesterdayClick
            end
          end
          object strngrdJamLog: TStringGrid
            Left = 0
            Top = 105
            Width = 916
            Height = 559
            Align = alClient
            Color = 14670284
            ColCount = 9
            DefaultColWidth = 120
            DefaultRowHeight = 16
            FixedColor = 9534289
            FixedCols = 0
            Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing]
            TabOrder = 1
          end
        end
      end
    end
    object tsYield: TTabSheet
      Caption = 'Yield'
      ImageIndex = 8
      object ChartYield: TChart
        Left = 0
        Top = 0
        Width = 924
        Height = 249
        AllowZoom = False
        BackWall.Brush.Color = clWhite
        BackWall.Color = 14670284
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clBlue
        Title.Font.Height = -16
        Title.Font.Name = 'Arial'
        Title.Font.Style = []
        Title.Text.Strings = (
          'Yield Chart')
        Title.Visible = False
        BackColor = 14670284
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 10
        LeftAxis.Maximum = 105
        LeftAxis.Minimum = -5
        LeftAxis.Title.Caption = 'Yield (%)'
        LeftAxis.Title.Font.Charset = DEFAULT_CHARSET
        LeftAxis.Title.Font.Color = clBlack
        LeftAxis.Title.Font.Height = -13
        LeftAxis.Title.Font.Name = 'Arial'
        LeftAxis.Title.Font.Style = []
        Legend.Alignment = laBottom
        Legend.Color = clGreen
        Legend.LegendStyle = lsLastValues
        Legend.ShadowColor = clTeal
        Legend.VertMargin = 3
        Legend.Visible = False
        View3D = False
        Align = alTop
        Color = 14670284
        TabOrder = 0
        object SpeedButton1: TSpeedButton
          Left = 1
          Top = 224
          Width = 80
          Height = 25
          Caption = 'Clear'
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          OnClick = SpeedButton1Click
        end
        object edYieldMax: TEdit
          Left = 4
          Top = 4
          Width = 41
          Height = 21
          TabOrder = 0
          Text = '100'
          OnClick = edYieldMaxClick
        end
        object edYieldMin: TEdit
          Left = 4
          Top = 200
          Width = 41
          Height = 21
          TabOrder = 1
          Text = '0'
          OnClick = edYieldMinClick
        end
        object SeriesAa: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Aa'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = True
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAb: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGreen
          Title = 'Site Ab'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAc: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clYellow
          Title = 'Site Ac'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAd: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlue
          Title = 'Site Ad'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAe: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = 33023
          Title = 'Site Ae'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAf: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clBlack
          Title = 'Site Af'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAg: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = 12615808
          Title = 'Site Ag'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesAh: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Ah'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBa: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Ba'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBb: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bb'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBc: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bc'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBd: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bd'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBe: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Be'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBf: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bf'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBg: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bg'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesBh: TLineSeries
          ColorEachPoint = True
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clWhite
          Title = 'Site Bh'
          LinePen.Width = 2
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCa: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clGray
          Title = 'Site Ca'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCb: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clFuchsia
          Title = 'Site Cb'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCc: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clTeal
          Title = 'Site Cc'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCd: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clNavy
          Title = 'Site Cd'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCe: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clMaroon
          Title = 'Site Ce'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCf: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clLime
          Title = 'Site Cf'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCg: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clOlive
          Title = 'Site Cg'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesCh: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clPurple
          Title = 'Site Ch'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDa: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clSilver
          Title = 'Site Da'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDb: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clAqua
          Title = 'Site Db'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDc: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Dc'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDd: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Dd'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDe: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site De'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDf: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Df'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDg: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Dg'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
        object SeriesDh: TLineSeries
          Marks.ArrowLength = 8
          Marks.Visible = False
          SeriesColor = clRed
          Title = 'Site Dh'
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          XValues.DateTime = False
          XValues.Name = 'X'
          XValues.Multiplier = 1
          XValues.Order = loAscending
          YValues.DateTime = False
          YValues.Name = 'Y'
          YValues.Multiplier = 1
          YValues.Order = loNone
        end
      end
      object pnlYield: TPanel
        Left = 0
        Top = 249
        Width = 924
        Height = 443
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object mtRowA: TTMyTray
          Left = 24
          Top = 10
          Width = 420
          Height = 180
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clYellow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          XItem = 11
          YItem = 9
          OnMouseUp = mtRowAMouseUp
        end
        object mtRowC: TTMyTray
          Tag = 2
          Left = 488
          Top = 10
          Width = 420
          Height = 180
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clYellow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          XItem = 11
          YItem = 9
          OnMouseUp = mtRowAMouseUp
        end
        object mtRowB: TTMyTray
          Tag = 1
          Left = 24
          Top = 198
          Width = 420
          Height = 180
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clYellow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          XItem = 11
          YItem = 9
          OnMouseUp = mtRowAMouseUp
        end
        object mtRowD: TTMyTray
          Tag = 3
          Left = 487
          Top = 198
          Width = 420
          Height = 180
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clYellow
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          XItem = 11
          YItem = 9
          OnMouseUp = mtRowAMouseUp
        end
      end
    end
    object tsTemperature: TTabSheet
      Caption = 'Temperature'
      ImageIndex = 12
      object TempChart: TChart
        Left = 0
        Top = 0
        Width = 924
        Height = 692
        BackWall.Brush.Color = clWhite
        BackWall.Color = 14670284
        Title.Font.Charset = DEFAULT_CHARSET
        Title.Font.Color = clBlue
        Title.Font.Height = -16
        Title.Font.Name = 'Arial'
        Title.Font.Style = []
        Title.Text.Strings = (
          'Temperature Chart ( 60 min)')
        BackColor = 14670284
        LeftAxis.Automatic = False
        LeftAxis.AutomaticMaximum = False
        LeftAxis.AutomaticMinimum = False
        LeftAxis.ExactDateTime = False
        LeftAxis.Increment = 5
        LeftAxis.Maximum = 135
        LeftAxis.Minimum = 25
        LeftAxis.Title.Caption = 'Temp.'
        LeftAxis.Title.Font.Charset = DEFAULT_CHARSET
        LeftAxis.Title.Font.Color = clBlack
        LeftAxis.Title.Font.Height = -13
        LeftAxis.Title.Font.Name = 'Arial'
        LeftAxis.Title.Font.Style = []
        LeftAxis.TitleSize = 1
        Legend.Alignment = laBottom
        Legend.Color = clGreen
        Legend.LegendStyle = lsSeries
        Legend.ShadowColor = clTeal
        Legend.TextStyle = ltsPlain
        Legend.VertMargin = 3
        View3D = False
        Align = alClient
        Color = 14670284
        TabOrder = 0
      end
      object cbbTempChart: TComboBox
        Left = 4
        Top = 584
        Width = 145
        Height = 21
        ItemHeight = 13
        TabOrder = 1
        Text = 'ALL'
        OnChange = cbbTempChartChange
        Items.Strings = (
          'ALL'
          'Hot Plate 1'
          'Hot Plate 2'
          'Shuttle 1 '
          'Shuttle 2'
          'Head 1'
          'Head 2'
          'Head 3'
          'Head 4'
          'Socket'
          'Chamber'
          'CCD'
          'Aa1'
          'Ab1'
          'Ac1'
          'Ad1'
          'Ba1'
          'Bb1'
          'Bc1'
          'Bd1'
          'Aa2'
          'Ab2'
          'Ac2'
          'Ad2'
          'Ba2'
          'Bb2'
          'Bc2'
          'Bd2'
          'Heater Gun1'
          'Heater Gun2'
          'DUT1'
          'DUT2'
          'DUT3'
          'DUT4'
          'Ae1'
          'Af1 '
          'Ag1'
          'Ah1'
          'Be1'
          'Bf1'
          'Bg1'
          'Bh1'
          'Ae2'
          'Af2'
          'Ag2'
          'Ah2'
          'Be2'
          'Bf2'
          'Bg2'
          'Bh2'
          '2D'
          'L/B'
          'ESD'
          'CCD_2'
          'HotAir1'
          'HotAir2')
      end
    end
    object tsTestInfo: TTabSheet
      Caption = 'Test Information'
      ImageIndex = 9
      object pgcTestInfo: TPageControl
        Left = 0
        Top = 0
        Width = 924
        Height = 692
        ActivePage = tsTimeData
        Align = alClient
        TabIndex = 1
        TabOrder = 0
        object tsTestTime: TTabSheet
          Caption = 'Test Time'
          object TimeInfoGrid: TStringGrid
            Left = 0
            Top = 0
            Width = 741
            Height = 611
            Color = 14670284
            ColCount = 6
            DefaultColWidth = 120
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
          object TimeInfoGrid_InArm: TStringGrid
            Left = 744
            Top = 0
            Width = 177
            Height = 611
            Color = 14670284
            ColCount = 2
            DefaultColWidth = 80
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 1
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
        end
        object tsTimeData: TTabSheet
          Caption = 'TimeData'
          ImageIndex = 5
          object sgTimeData: TStringGrid
            Left = 0
            Top = 0
            Width = 916
            Height = 664
            Align = alClient
            Color = 14670284
            ColCount = 10
            FixedColor = 9534289
            RowCount = 22
            TabOrder = 0
          end
        end
        object tsLoadInfo: TTabSheet
          Caption = 'LoadInfo'
          ImageIndex = 6
          object Memo2: TMemo
            Left = 0
            Top = 0
            Width = 916
            Height = 189
            Align = alTop
            Color = 14670284
            ScrollBars = ssBoth
            TabOrder = 0
          end
          object btOpenLoadLog: TButton
            Left = 8
            Top = 572
            Width = 165
            Height = 25
            Caption = 'Open loader log'
            TabOrder = 3
            OnClick = btOpenLoadLogClick
          end
          object Memo3: TMemo
            Left = 0
            Top = 189
            Width = 916
            Height = 157
            Align = alTop
            Color = 14670284
            ScrollBars = ssBoth
            TabOrder = 1
          end
          object Memo4: TMemo
            Left = 0
            Top = 346
            Width = 916
            Height = 157
            Align = alTop
            Color = 14670284
            ScrollBars = ssBoth
            TabOrder = 2
          end
        end
        object ts2: TTabSheet
          Caption = 'TestReceive'
          ImageIndex = 7
          object sg_ListTimeReceiveInfoGrid: TStringGrid
            Left = 0
            Top = 2
            Width = 371
            Height = 611
            Color = 14670284
            ColCount = 3
            DefaultColWidth = 120
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
        end
        object tsTimeInfo: TTabSheet
          Caption = 'Time Info'
          ImageIndex = 8
          object strngrdTestTime: TStringGrid
            Left = 0
            Top = 0
            Width = 948
            Height = 657
            Align = alClient
            Color = 14670284
            ColCount = 6
            DefaultColWidth = 120
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
        end
        object tsIndexAirOn1: TTabSheet
          Caption = 'IndexAirOn1'
          ImageIndex = 9
          object strngrdIndeAirOn1: TStringGrid
            Left = 0
            Top = 0
            Width = 948
            Height = 657
            Align = alClient
            Color = 14670284
            ColCount = 4
            DefaultColWidth = 120
            FixedColor = 9534289
            RowCount = 105
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
        end
        object tsIndexAirOn2: TTabSheet
          Caption = 'IndexAirOn2'
          ImageIndex = 10
          object strngrdIndeAirOn2: TStringGrid
            Left = 0
            Top = 0
            Width = 948
            Height = 657
            Align = alClient
            Color = 14670284
            ColCount = 4
            DefaultColWidth = 120
            FixedColor = 9534289
            RowCount = 105
            TabOrder = 0
            RowHeights = (
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24
              24)
          end
        end
      end
    end
    object tsDataRecord: TTabSheet
      Caption = 'Record'
      ImageIndex = 2
      object pgcPrecautions: TPageControl
        Left = 0
        Top = 0
        Width = 924
        Height = 692
        ActivePage = tsPrecautionLog
        Align = alClient
        TabIndex = 2
        TabOrder = 0
        object tsPrecautionsRecord: TTabSheet
          Caption = 'Precautions Record'
          object pnPrecautionsRecord: TPanel
            Left = 0
            Top = -22
            Width = 1009
            Height = 545
            BevelInner = bvLowered
            Color = 13487501
            Font.Charset = ANSI_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'Courier New'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
            object pnMaintenanceRecord: TPanel
              Left = 4
              Top = 24
              Width = 990
              Height = 520
              BevelInner = bvLowered
              Color = 14072713
              TabOrder = 0
              object sbHandlerPrecautionRecordSet: TSpeedButton
                Left = 812
                Top = 75
                Width = 100
                Height = 30
                Caption = 'Add'
                Glyph.Data = {
                  66090000424D660900000000000036000000280000001C0000001C0000000100
                  18000000000030090000232E0000232E00000000000000000000FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F7
                  F3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E8173757892949686888A75777A707275
                  7E80828D8E919B9C9EB5B6B7CECFD0DBDBDCF0F0F0F6F6F7FFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFF56585B747679DEDFDFDEDFDFDDDEDEDDDEDEDDDEDEDDDEDEDDDEDED0D1D2
                  B6B8B990919383858877797B78797C8285878D8E90969799BEBFC0FFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF484A4E8A8B8EDFE0E0DF
                  E0E0DFE0E0DFE0E0DFE0E0DFE0E0DFE0E0DEDFDFDDDEDEC9CACBBEBFC0C6C7C7
                  DDDEDEDDDEDEDDDEDEDDDEDEB6B7B8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFF3A3D41A0A2A3E1E2E2E1E2E2E0E1E1E0E1E1E0E1E1E0
                  E1E1E0E1E1DFE0E0DFE0E0D3D4D5868889B7B9BAD2D3D3DDDEDEDFE0E0DFE0E0
                  9FA1A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2C2E
                  33B6B8B9E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E1E2E2E1E2E2DD
                  DEDEBECACF3E72AAAFB1B2D0D1D1DFE0E0E0E1E1A0A2A3FFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1E2126CECFD0E4E5E5B7B8B97B7C
                  7E898A8C969799A5A6A7B4B5B6C1C2C2CECFD0D9DADBBDBFC04895C89D9EA0C1
                  C2C3DBDCDCE2E3E3A6A8A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFF9F9F9191C21E3E3E3E6E6E6E6E6E6DCDDDDCDCECEBDBEBFAFB0B2A1A2
                  A4929395838487737477656669B28C56803B2187888AD2D3D4E2E3E3B2B3B4FF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEBEB2B2E32E7E8E8
                  E7E8E8E7E8E8E7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E6E6E6E6E6E6E6E6E6E0E0
                  E0F07F29BA5E1E97989AC4C6C7DFE0E0C2C3C4FFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFDDDDDE424448E9E9E9E9E9E998989A858688939496
                  A1A2A4B0B1B2C0C1C2CECFD0DCDDDEE7E8E8E3E4E4F57023C16820833D22BBBB
                  BCDBDBDBD8D9DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF
                  D0D1585B5EEAEBEBEAEBEBE7E8E8D7D8D8C8C8C9B8B8BAA8A9AA9A9B9D8C8C8E
                  7C7D7F707174717174717274CC823B8E4F16A9ABACC1C3C3EFEFEFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1C2C46F7174EBECECEBECECEB
                  ECECEBECECEBECECEBECECEBECECEBECECEAEBEBEAEBEBEAEBEBE9EAEACFD0D1
                  F06F23BE641F8E46279D9E9FF1F1F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFB3B4B586888AECECECE1E1E1747577747578838486939496A2
                  A3A4B0B1B2C0C1C1CFD0D1DFE0E0EBECECE2E3E3BFC0C1C26B217E3A21858789
                  EAEBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA5A6A89D9F
                  A1EDEEEEEDEEEEEDEEEEE9E9EADADADBCBCBCCBCBDBEACADAE9C9D9F8E8E907F
                  80827273756F7072757678F37827B65C1E6F6F70DEDEDFFDFDFDFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF97999BB6B6B8EFEFEFEDEDEDE7E7E8EEEF
                  EFEEEFEFEEEFEFEEEFEFEEEFEFEDEEEEEDEEEEEDEEEEEDEEEEEDEEEEDDDEDEEE
                  7126BC6420843E21C6C7C8F6F6F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFF898A8DCDCFD0F0F1F1CBCCCD7A7B7E737477757679838486939395A3A3
                  A5B2B3B4C1C1C2D0D0D1E0E0E1EEEEEEEEEFEFDCDEDED58D3DAF5C1AB8B9BBEB
                  ECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7B7D80E6E6E6F2F2F2
                  F2F2F2F2F2F2F1F2F2EFF0F0E0E1E1D1D2D2C1C2C3B0B1B3A2A3A49394968283
                  867577799E9EA0E5E5E6F47224BE641F8D4525DFDFE0FBFBFBFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFF737578F3F4F4F3F4F4DADBDBD5D6D6E4E4E4F2F2F2
                  F3F3F3F3F3F3F3F3F3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F2F2F0F1F1E1E2
                  E3C46E21803B20BBBCBDE2E3E3FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF77
                  797BF5F5F5F5F5F5B8B8BA9293958182847475787475777F80828E8F919D9EA0
                  ADAEB0BEBFC0CDCECFDDDEDEF2F2F2F3F3F3ECEDEDB9B8B86C6C6C9E9C9DE1E2
                  E2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E81F7F7F7F7F7F7F7F7F7F7
                  F7F7F6F6F6F6F6F6F6F6F6ECECECDCDCDCCBCCCCBABBBCABABAD9C9C9E8B8C8E
                  CECFD0F4F5F5EFF0F0B8B8B8A2A2A2949494F0F0F1FEFEFEFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFF85878AF8F8F8F8F8F8BABBBCF8F8F8F8F8F8F8F8F8F8F8F8F8
                  F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F6F6F6F6F6F6DEDEDFFFFFFF
                  8D87EB4239DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF939497FAFA
                  FAD7D7D8767879E4E4E5F9F9F99E9FA1686A6DF9F9F9F9F9F97D7E82DDDDDEF8
                  F8F8F8F8F8E1E2E2F8F8F8F8F8F8D2D2D3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0A1A3FBFBFBF1F1F1C1C2C3FBFBFBFBFB
                  FBD0D0D1BABBBCFBFBFBFBFBFB747678C7C7C8FAFAFAB9BABB616365F5F5F5F9
                  F9F9CACACBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFF2F2F2CECFCFBBBCBDD2D3D4898A8D8A8C8EBBBCBDC3C4C5BEBFC0E9EA
                  EABDBEBFFBFBFBFBFBFBECECECC1C2C3FBFBFBFBFBFBC7C8C9FFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4E4E4
                  CFD0D0FFFFFFFFFFFFC8C8C9DEDEDFFFFFFFFEFEFEE4E4E5C4C6C7AAAAADAEAF
                  B1C9CACB92939697999ABFBFC1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFF9F9F9CDCECEFFFFFFFFFFFFD7D7D8D1D1D2FFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFF}
                OnClick = sbHandlerPrecautionRecordSetClick
              end
              object sbHandlerPrecautionRecordClear: TSpeedButton
                Left = 812
                Top = 374
                Width = 100
                Height = 34
                Caption = 'Clear'
                Glyph.Data = {
                  4A090000424D4A090000000000001A0000000C0000001C001C0001001800FFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFC8C8CAAEAFB1A4A5A797999B888A914444722E2E742B2A
                  7836356C505167696A6D7475787D7E8185868985868985868985868985868984
                  8689848689848689848689848689848689848689DBDBDCFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFB9B9D72A2A8E2426A02426A72426A92526A3262695575998DCE4
                  E8E0EAEBDFEAEBDFEAEBDFEAEBDEEAEBDEEAEBDDEAEBDCEAEBDBEAEBDAE9EBD9
                  E9EBD8E9EB8F9699D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFDCDCEC25279A282CB3
                  3236B92326B22326B1272AB23638B624269F5F63A5F2FEFFF2FEFFF1FEFFF1FE
                  FFF0FEFFEFFEFFEEFEFFEDFEFFEBFDFFEAFDFFEAFDFFE9FDFF929B9DD6D6D7FF
                  FFFFFFFFFFFFFFFFFFFFFF3B3CA22126B5AEB0E6F7F7FD5B5ECA3135BBD4D5F1
                  F6F6FC272BB525269CC9D4E7F1FEFFF0FEFFEFFEFFEEFEFFEDFEFFECFDFFEBFD
                  FFEAFDFFE9FDFFE8FDFFE7FDFF929B9DD6D6D7FFFFFFFFFFFFFFFFFFF8F8FC24
                  27AB2026C06267D2FBFBFEF7F7FDDBDCF4FFFFFFACADE52126B82326AF8084B3
                  CDD7D8CCD7D8CCD7D8D9E7E9A8B3B57C81847C81847C81847C81849AA5A7E5FD
                  FF919B9DD6D6D7FFFFFFFFFFFFFFFFFFD3D3EC2226B72025C42025C3777ADAFF
                  FFFFFFFFFFC2C4EE2228C02026BE2126BA6C72B6EDFEFFECFDFFEBFDFFEAFDFF
                  A1AAADBECCCFE8FDFFE6FDFFC7DADC90999CE5FDFF919B9DD6D6D7FFFFFFFFFF
                  FFFFFFFFE9E9F62226BA1F25C92E34CCD3D5F4FFFFFFFFFFFFF7F7FD595CD320
                  25C22126BC7076B4CBD6D8CAD6D8CAD6D8D8E9EAA0AAADC3D4D6E6FDFFE5FDFF
                  D0E4E690999CE4FDFF909B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFF2629BA1E25
                  CDAAACECFFFFFFB3B5EE7276DEFCFCFFF2F3FC242AC92125BCA5B2DDEAFDFFE9
                  FDFFE8FDFFE7FDFF9FAAADC2D4D6E5FDFFE5FDFFCFE4E68F999CE2FDFF8F9B9D
                  D6D6D7FFFFFFFFFFFFFFFFFFFFFFFF9193DB2026CA8F94EAC1C3F39094E98F93
                  E8ACAFEDB9BBF02E34CD2B2FB3CCD9DFCFDEE0CFDEE0CEDEE0D8EDEF9EAAAD96
                  A1A3A4B1B4A3B1B49AA7A98E999CE0FDFF8F9B9DD6D6D7FFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFF4C4ECA5055D96F74E57F84E88488E8757AE45F64DD2A2FC0B8C7
                  E7E1F5F7E0F5F7DFF5F7DFF5F7E2F9FBCADEE0BBCDCFBACDCFB9CDCFB8CDCFC1
                  D8DBDEFDFF8E9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9596DF
                  272BC62126CD2026CE2328C7444AC8D6EAF9E6FDFFE5FDFFE5FDFFE5FDFFE5FD
                  FFE4FDFFE3FDFFE2FDFFE1FDFFDFFDFFDEFDFFDEFDFFDDFDFF8E9B9DD6D6D7FF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC5D4D7E8FDFFE7FDFF
                  E6FDFFE5FDFFE5FDFFE5FDFFE5FDFFE4FDFFE3FDFFE2FDFFE1FDFFE0FDFFDFFD
                  FFDEFDFFDDFDFFDCFDFFDBFDFF8D9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFF999B9DC3D4D7DCF1F3D7EBEDD7EBEDD7EBEDD7EBEDD7EBED
                  D6EBEDD5EBEDD4EBEDDBF6F8DFFDFFDEFDFFDEFDFFDCFDFFDBFDFFDAFDFFD9FC
                  FF8D9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2
                  D4D7D9EDEFD4E7E8D4E7E8D3E7E8D2E7E8D1E7E8D0E7E8D0E7E8CFE7E8D7F3F5
                  CAE4E6C0DADCC0DADCBFDADCBDD9DCC2DFE2D9FCFF8D9A9DD6D6D7FFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2D4D7E5FDFFE4FDFFE3FDFFE2
                  FDFFE1FDFFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFF9AABAD8D9B9D94A4A793A4A7
                  909FA28B999CD8FCFF8C9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFF999B9DC1D4D7CFE3E5C5D6D8C4D6D8C4D6D8C3D6D8C2D6D8C2D6D8C2
                  D6D8C1D6D8CEEAEC99ABADB8D3D6D9FCFFD9FCFFC5E4E78A999CD6FCFF8B9A9D
                  D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DBFD4D7E1FD
                  FFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFF99AAADB8
                  D3D6D9FCFFD7FCFFC4E4E78A999CD4FCFF8A9A9DD6D6D7FFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFF989B9DBED4D7CCE3E5C2D6D8C2D6D8C2D6D8C1D6
                  D8C1D6D8C0D6D8BFD6D8BFD6D8CDE9EC99AAADB8D3D6D7FCFFD5FCFFC2E4E789
                  999CD2FCFF899A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  989B9DBCD4D7DEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFFD9FCFFD9FCFFD9FC
                  FFD8FCFF98AAAD78828578828578828577828588999CD0FCFF899A9DD6D6D7FF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DBBD4D7DCFDFFDBFDFF
                  DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD3FAFDD3FBFEADE7
                  EBC6F6F9CFFBFECEF9FCCFFCFF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFF989B9DBAD4D7DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFF
                  D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFCEFAFDB3EBEFBEEFF2A6E3E7C4F6FACDFC
                  FF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8
                  D4D7D9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFF
                  D1FCFFB1EAEED4FBFEDDFCFFE2FDFFC2EDF0A4E2E5849EA1D6D6D7FFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8D4D7D9FCFFD8FCFFD7FCFFD6
                  FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0FCFFD0FCFFA8E4E8DDFCFFE2FDFF
                  E3FDFFE8FDFFEDFDFFB4D5D7D5D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFF989B9DB7D4D7D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0
                  FCFFD0FCFFCFFCFFCCFBFEBAEBEEE2FDFFE3FDFFE8FDFFC9E2E4B0BCBEF7F8F8
                  FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF979A9DACC7CAD2F8
                  FBD1F8FBD0F8FBCFF8FBCEF8FBCDF8FBCDF8FBCCF8FBCBF8FBCAF8FBB2EAEEDE
                  FBFDDCF9FCB4D0D2C0C5C7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFA7A9AB7176797176797076797076797076797076
                  7970767970767970767970767970767985A6A99EB9BBDCDDDEFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
                OnClick = sbHandlerPrecautionRecordClearClick
              end
              object sbHandlerPrecautionFormShow: TSpeedButton
                Left = 814
                Top = 410
                Width = 131
                Height = 33
                Caption = 'Show'
                OnClick = sbHandlerPrecautionFormShowClick
              end
              object sbPRFinishDate: TSpeedButton
                Left = 415
                Top = 481
                Width = 200
                Height = 30
                Caption = #32080#26696#26085#26399#65306
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = #27161#26999#39636
                Font.Style = [fsBold]
                ParentFont = False
                OnClick = sbPRFinishDateClick
              end
              object sbPRStartDate: TSpeedButton
                Left = 8
                Top = 448
                Width = 200
                Height = 30
                Caption = #38283#22987#26085#26399' :'
                Font.Charset = ANSI_CHARSET
                Font.Color = clBlack
                Font.Height = -16
                Font.Name = #27161#26999#39636
                Font.Style = [fsBold]
                ParentFont = False
                OnClick = sbPRStartDateClick
              end
              object sbPrecautionSave: TSpeedButton
                Left = 814
                Top = 445
                Width = 131
                Height = 35
                Caption = 'Save '
                Glyph.Data = {
                  66100000424D6610000000000000360000002800000025000000250000000100
                  18000000000030100000232E0000232E00000000000000000000FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFBDBEBF9B9C9F9EA0A19EA0A19E
                  A0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A1
                  9EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0A19EA0
                  A19EA0A19EA0A19EA0A19EA0A19A9C9EE6E6E7FFFFFFFFFFFF00FFFFFFFFFFFF
                  EEEEEF60636636393D36393D36393D36393D36393D2B2E32405A5E4461664461
                  6644616644616644616644616644616644616644616644616644616644616644
                  61664461664461664461664460652B333834373B36393D36393D36393D414347
                  9D9EA0FFFFFFFFFFFF00FFFFFFFFFFFFC8C9CA595B5E3C3F433C3F433C3F433C
                  3F4334373B88A0A8CDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FC
                  CDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCCDF2FCB4D5
                  DE2B2E323C3F433C3F433C3F433C3F43A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C8585B5D424548424548424548424548323538E7E7E8FFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF272A2E424548424548424548424548
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C85C5F61484B4E484B4E484B4E48
                  4B4E35383CEAEAEAC9C9CAC3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4
                  C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4EBEB
                  EC292C30484B4E484B4E484B4E484B4EA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C86063664E51544E51544E51544E5154393C40EAEAEAF7F7F7F5F5F6F5F5
                  F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5F5F6F5
                  F5F6F5F5F6F5F5F6F5F5F6F5F5F6FDFDFD2B2E334E51544E51544E5154505356
                  A9AAACFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C865676954565954565954565954
                  56593D3F43EAEAEAD2D2D3CDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCE
                  CDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCECDCDCEEDEE
                  EE2E303554565954565954565957585BABACAEFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C8696B6E5A5C5F5A5C5F5A5C5F5A5C5F414347EAEAEACACACBC3C4C4C3C4
                  C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3
                  C4C4C3C4C4C3C4C4C3C4C4C3C4C4EAEBEB3033375A5C5F5A5C5F5A5C5F5D5F62
                  A9AAACFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C86F717462646762646762646762
                  646746484CEAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FF33363A626467626467626467636568A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C8747677686A6C686A6C686A6C686A6C4A4C4FEAEAEACACACBC3C4C4C3C4
                  C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3
                  C4C4C3C4C4C3C4C4C3C4C4C3C4C4EAEBEB35383C686A6C686A6C686A6C737577
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C8787A7C6E70726E70726E70726E
                  70724D5053EAEAEAD4D5D5CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0
                  CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0CFCFD0EDEE
                  EE373A3E6E70726E70726E7072898B8DA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C87C7E7F747577747577747577747577515356EAEAEAF5F5F5F3F3F3F3F3
                  F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
                  F3F3F3F3F3F3F3F3F3F3F3F3F3F3FCFCFC3A3C407475777475777475779C9D9F
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C88182847A7B7D7A7B7D7A7B7D7A
                  7B7D55575AEAEAEACACBCBC3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4
                  C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4C3C4C4EAEA
                  EA3C3E427A7B7D7A7B7D818284B1B2B3A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C8858689808183808183808183808183595A5EEAEAEACECFCFC8C9C9C8C9
                  C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8C9C9C8
                  C9C9C8C9C9C8C9C9C8C9C9C8C9C9E7E8E83E41448081838081839D9E9FAEAFB0
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C88A8B8C86878886878886878886
                  87885D5E61EAEAEAF6F7F7F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5
                  F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5F5FCFC
                  FC4143468687888B8C8DB3B4B5ABACADA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C88E8F918C8D8E8C8D8E8C8D8E8C8D8E646568D7D8D9CECECFCACACBCACA
                  CBCACACBCACACBCACACBCACACBCACACBCACACBCACACBCACACBCACACBCACACBCA
                  CACBCACACBCACACBCACACBCACACBEAEAEA46484B8C8D8EACACADB9BABBAFB0B0
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  95968A8B8D55676CA9D4D8AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0
                  AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCE0AFDCDF718D
                  91717274A5A6A7BEBFBFB8B9B9AFB0B1A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C89495969495969495969495969495969495968D8E90797A7C797A7C797A
                  7C797A7C797A7C797A7C797A7C797A7C797A7C797A7C797A7C797A7C797A7C79
                  7A7C797A7C797A7C797A7C797A7C8688899C9D9EC2C2C3BEBFBFB8B9B9AFB0B1
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  959694959694959691939488898B88898B88898B88898B88898B88898B88898B
                  88898B88898B88898B88898B88898B88898B88898B88898B88898B919293A2A3
                  A3BBBCBDBEBFBFB8B9B9B2B3B3AAAAABA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C89495969495969495969495969495969495967071741C1F243033375C5E
                  615E60635E60635E60635E60635E60635E60635E60635E60635E60635E60634F
                  52551C1F241C1F241C1F241E21268E8F91BFC0C1B8B9B9B2B3B3ACADAEA9AAAB
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  959694959657595C222529818384A5A6A6A5A6A7A5A6A7A5A6A7A5A6A69E9F9F
                  9E9F9F9E9F9F9E9F9F9E9F9F9E9F9F9E9F9F22252922252925282C7072749294
                  95C1C2C2B8B9B9B2B3B3ACADAEA9AAABA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C89495969495969495969495969495969495965B5C60282A2F929395686A
                  6D282A2F282A2F282A2F909294A5A6A7A5A6A7A5A6A7A5A6A7A5A6A7A5A6A7A5
                  A6A7282A2F2D2F3478797C88898C8A8C8EBBBCBDB2B3B3ACADAEACADAEA4A5A5
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  95969495965E60632D303498999B64676A2D30342D30342D30348F8F92ABACAD
                  ABACADABACADABACADABACADABACADABACAD36393C7E80828B8D8F7F81848284
                  86B6B6B7ACADAEACADAEA6A7A89FA0A0A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C894959694959694959694959694959694959661636633353AA0A1A16A6C
                  6F33353A33353A33353A949597B3B4B4B3B4B4B3B4B4B3B4B4B3B4B4B3B4B4BF
                  C0C09A9B9D9A9B9D8E8F9283848786878AB6B6B7ACADAEACADAEA6A7A89FA0A0
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  9596949596656769393B3FA6A8A8707274393B3F393B3F393B3F999B9CBABBBB
                  BABBBBBABBBBBABBBBBBBCBCCFD0D0E0E1E19D9EA092939587888A7B7C7F7F81
                  83B0B1B2ACADAEA6A7A8A0A1A2999A9BA8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C8949596949596949596949596949596949596696A6C3F4144ACAEAF7577
                  793F41443F41443F41449EA0A2C0C1C2C0C1C2C0C1C2C6C7C8DDDEDEE7E7E8E3
                  E4E4A0A1A29596988A8C8D7F8082838486B0B1B2ACADAEA6A7A8A0A1A2999A9B
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFC6C7C894959694959694959694959694
                  95969495966C6D7045464AB4B5B67B7C7F45464A45464A45464AA5A6A7C8C9C9
                  CACBCBDBDCDCE9E9E9EAEAEAE7E7E7E4E5E598999B8E8F9183848679797C7D7E
                  81B0B1B2A6A7A8A0A1A29A9B9C949596A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  C6C7C89495969495969495969495969495969495967071744B4C50BBBCBD8181
                  844B4C504B4C504B4C50AAABACDBDCDCECECECEDEDEDEDEDEDEAEBEBE8E8E8E5
                  E5E592929587888A7D7E81737477818285AAABACA0A1A29A9B9C949596949596
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFD3D3D486878994959694959694959694
                  9596949596737477505155C1C2C3858688505155525357838487E2E2E3F4F4F4
                  F1F2F2EFF0F0EFF0F0EDEDEDEBEBEBE8E9E99595988A8B8E81818477787B8586
                  88AAABACA0A1A29A9B9C949596949596A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  FFFFFFB9BABC80818495969794959694959694959676777A56575BC9CACAC7C8
                  C9CBCCCCE9E9E9EDEDEDF4F4F4F4F4F4F2F3F3F2F3F3F0F1F1EFEFEFEDEDEDEB
                  EBEB8E8F928586897C7D807C7D80818184A5A6A79A9B9C949596949596949596
                  A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFD4D4D57E8083B2B3B3B1
                  B2B2B1B2B2A9AAABB8B9BAF7F7F8FFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFDFDFD
                  FDFDFDFDFDFDFDFDFDFCFCFCFCFCFCFBFBFBB2B3B3ACADAEA7A8A9A7A7A8ACAD
                  AEBEBFBFB6B7B7B1B2B2B1B2B2B1B2B2A8A9ABFFFFFFFFFFFF00FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFE8E8E96E70734F53564A4D51919395A4A6A7A2A4A5A0A1
                  A29E9FA09C9D9E9A9C9D999B9C999B9C999B9C999B9C999B9C999B9C999B9C99
                  9B9C999B9C999B9C999B9C999B9C97989B4D4F53494B4F484A4E484A4E525558
                  9D9EA0FFFFFFFFFFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F7C1
                  C2C4C1C2C4F6F6F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFC7C8C9BEBFC0C0C0C2C1C2C4C4C5C6F5F5F5FFFFFFFFFFFF00FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFF00}
                OnClick = sbPrecautionSaveClick
              end
              object cobNoteContentsSet: TSpeedButton
                Left = 812
                Top = 35
                Width = 100
                Height = 30
                Caption = 'Add'
                Glyph.Data = {
                  66090000424D660900000000000036000000280000001C0000001C0000000100
                  18000000000030090000232E0000232E00000000000000000000FFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F7
                  F3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E8173757892949686888A75777A707275
                  7E80828D8E919B9C9EB5B6B7CECFD0DBDBDCF0F0F0F6F6F7FFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFF56585B747679DEDFDFDEDFDFDDDEDEDDDEDEDDDEDEDDDEDEDDDEDED0D1D2
                  B6B8B990919383858877797B78797C8285878D8E90969799BEBFC0FFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF484A4E8A8B8EDFE0E0DF
                  E0E0DFE0E0DFE0E0DFE0E0DFE0E0DFE0E0DEDFDFDDDEDEC9CACBBEBFC0C6C7C7
                  DDDEDEDDDEDEDDDEDEDDDEDEB6B7B8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFF3A3D41A0A2A3E1E2E2E1E2E2E0E1E1E0E1E1E0E1E1E0
                  E1E1E0E1E1DFE0E0DFE0E0D3D4D5868889B7B9BAD2D3D3DDDEDEDFE0E0DFE0E0
                  9FA1A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2C2E
                  33B6B8B9E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E1E2E2E1E2E2DD
                  DEDEBECACF3E72AAAFB1B2D0D1D1DFE0E0E0E1E1A0A2A3FFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1E2126CECFD0E4E5E5B7B8B97B7C
                  7E898A8C969799A5A6A7B4B5B6C1C2C2CECFD0D9DADBBDBFC04895C89D9EA0C1
                  C2C3DBDCDCE2E3E3A6A8A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFF9F9F9191C21E3E3E3E6E6E6E6E6E6DCDDDDCDCECEBDBEBFAFB0B2A1A2
                  A4929395838487737477656669B28C56803B2187888AD2D3D4E2E3E3B2B3B4FF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEBEB2B2E32E7E8E8
                  E7E8E8E7E8E8E7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E6E6E6E6E6E6E6E6E6E0E0
                  E0F07F29BA5E1E97989AC4C6C7DFE0E0C2C3C4FFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFDDDDDE424448E9E9E9E9E9E998989A858688939496
                  A1A2A4B0B1B2C0C1C2CECFD0DCDDDEE7E8E8E3E4E4F57023C16820833D22BBBB
                  BCDBDBDBD8D9DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF
                  D0D1585B5EEAEBEBEAEBEBE7E8E8D7D8D8C8C8C9B8B8BAA8A9AA9A9B9D8C8C8E
                  7C7D7F707174717174717274CC823B8E4F16A9ABACC1C3C3EFEFEFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1C2C46F7174EBECECEBECECEB
                  ECECEBECECEBECECEBECECEBECECEBECECEAEBEBEAEBEBEAEBEBE9EAEACFD0D1
                  F06F23BE641F8E46279D9E9FF1F1F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFB3B4B586888AECECECE1E1E1747577747578838486939496A2
                  A3A4B0B1B2C0C1C1CFD0D1DFE0E0EBECECE2E3E3BFC0C1C26B217E3A21858789
                  EAEBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA5A6A89D9F
                  A1EDEEEEEDEEEEEDEEEEE9E9EADADADBCBCBCCBCBDBEACADAE9C9D9F8E8E907F
                  80827273756F7072757678F37827B65C1E6F6F70DEDEDFFDFDFDFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF97999BB6B6B8EFEFEFEDEDEDE7E7E8EEEF
                  EFEEEFEFEEEFEFEEEFEFEEEFEFEDEEEEEDEEEEEDEEEEEDEEEEEDEEEEDDDEDEEE
                  7126BC6420843E21C6C7C8F6F6F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFF898A8DCDCFD0F0F1F1CBCCCD7A7B7E737477757679838486939395A3A3
                  A5B2B3B4C1C1C2D0D0D1E0E0E1EEEEEEEEEFEFDCDEDED58D3DAF5C1AB8B9BBEB
                  ECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7B7D80E6E6E6F2F2F2
                  F2F2F2F2F2F2F1F2F2EFF0F0E0E1E1D1D2D2C1C2C3B0B1B3A2A3A49394968283
                  867577799E9EA0E5E5E6F47224BE641F8D4525DFDFE0FBFBFBFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFF737578F3F4F4F3F4F4DADBDBD5D6D6E4E4E4F2F2F2
                  F3F3F3F3F3F3F3F3F3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F2F2F0F1F1E1E2
                  E3C46E21803B20BBBCBDE2E3E3FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF77
                  797BF5F5F5F5F5F5B8B8BA9293958182847475787475777F80828E8F919D9EA0
                  ADAEB0BEBFC0CDCECFDDDEDEF2F2F2F3F3F3ECEDEDB9B8B86C6C6C9E9C9DE1E2
                  E2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E81F7F7F7F7F7F7F7F7F7F7
                  F7F7F6F6F6F6F6F6F6F6F6ECECECDCDCDCCBCCCCBABBBCABABAD9C9C9E8B8C8E
                  CECFD0F4F5F5EFF0F0B8B8B8A2A2A2949494F0F0F1FEFEFEFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFF85878AF8F8F8F8F8F8BABBBCF8F8F8F8F8F8F8F8F8F8F8F8F8
                  F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F6F6F6F6F6F6DEDEDFFFFFFF
                  8D87EB4239DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF939497FAFA
                  FAD7D7D8767879E4E4E5F9F9F99E9FA1686A6DF9F9F9F9F9F97D7E82DDDDDEF8
                  F8F8F8F8F8E1E2E2F8F8F8F8F8F8D2D2D3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0A1A3FBFBFBF1F1F1C1C2C3FBFBFBFBFB
                  FBD0D0D1BABBBCFBFBFBFBFBFB747678C7C7C8FAFAFAB9BABB616365F5F5F5F9
                  F9F9CACACBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFF2F2F2CECFCFBBBCBDD2D3D4898A8D8A8C8EBBBCBDC3C4C5BEBFC0E9EA
                  EABDBEBFFBFBFBFBFBFBECECECC1C2C3FBFBFBFBFBFBC7C8C9FFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4E4E4
                  CFD0D0FFFFFFFFFFFFC8C8C9DEDEDFFFFFFFFEFEFEE4E4E5C4C6C7AAAAADAEAF
                  B1C9CACB92939697999ABFBFC1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFF9F9F9CDCECEFFFFFFFFFFFFD7D7D8D1D1D2FFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                  FFFFFFFFFFFFFFFFFFFF}
                OnClick = cobNoteContentsSetClick
              end
              object sbScreenkeyboard: TSpeedButton
                Left = 808
                Top = 8
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
                OnClick = sbScreenkeyboardClick
              end
              object cobHandlerPrecautionRecord: TComboBox
                Left = 10
                Top = 74
                Width = 799
                Height = 30
                Font.Charset = ANSI_CHARSET
                Font.Color = clNavy
                Font.Height = -19
                Font.Name = 'Courier New'
                Font.Style = [fsBold]
                ItemHeight = 0
                ParentFont = False
                TabOrder = 6
              end
              object MemoHandlerPrecautionRecord: TMemo
                Left = 10
                Top = 110
                Width = 799
                Height = 297
                Color = 14670284
                Font.Charset = ANSI_CHARSET
                Font.Color = clNavy
                Font.Height = -16
                Font.Name = 'Courier New'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 7
              end
              object pnPrecautionRecordDocumentNoTitle: TPanel
                Left = 5
                Top = 6
                Width = 196
                Height = 26
                BevelOuter = bvNone
                Caption = 'DOCUMENT NO.'#65306
                Color = 6055843
                Font.Charset = ANSI_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 0
              end
              object edPrecautionRecordDocumentNo: TEdit
                Left = 208
                Top = 6
                Width = 249
                Height = 26
                TabOrder = 1
              end
              object Panel91: TPanel
                Left = 464
                Top = 6
                Width = 340
                Height = 26
                BevelOuter = bvNone
                Caption = '('#33509#28961#35215#31684#20381#24490#65292#21063#20197'"N/A"'#34920#31034')'
                Color = 6055843
                Font.Charset = ANSI_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 2
              end
              object Panel92: TPanel
                Left = 5
                Top = 38
                Width = 196
                Height = 26
                BevelOuter = bvNone
                Caption = #27880#24847#20107#38917#20839#23481#65306
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 3
              end
              object edNoteContents: TEdit
                Left = 208
                Top = 38
                Width = 300
                Height = 26
                TabOrder = 4
              end
              object Panel93: TPanel
                Left = 10
                Top = 417
                Width = 196
                Height = 26
                BevelOuter = bvNone
                Caption = #37096#38272#20027#31649#26680#20934#65306
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 12
              end
              object edApprovedManager: TEdit
                Left = 208
                Top = 416
                Width = 200
                Height = 26
                TabOrder = 10
              end
              object Panel94: TPanel
                Left = 414
                Top = 417
                Width = 196
                Height = 26
                BevelOuter = bvNone
                Caption = #35069#34920#32773#65306
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 13
              end
              object edWatchmakers: TEdit
                Left = 612
                Top = 416
                Width = 200
                Height = 26
                TabOrder = 11
              end
              object pnPrecautionStartTime: TPanel
                Left = 211
                Top = 450
                Width = 200
                Height = 26
                BevelOuter = bvNone
                Color = 13761532
                Font.Charset = ANSI_CHARSET
                Font.Color = clPurple
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 16
              end
              object Panel96: TPanel
                Left = 10
                Top = 485
                Width = 127
                Height = 26
                BevelOuter = bvNone
                Caption = #32080#26696#26041#24335#65306
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 22
              end
              object Panel5: TPanel
                Left = 414
                Top = 449
                Width = 196
                Height = 26
                BevelOuter = bvNone
                Caption = #32080#26696#32773#65306
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -19
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 15
              end
              object edFinishName: TEdit
                Left = 612
                Top = 448
                Width = 200
                Height = 26
                TabOrder = 14
              end
              object cobPRFinishType: TComboBox
                Left = 144
                Top = 483
                Width = 113
                Height = 26
                ItemHeight = 18
                TabOrder = 21
                Items.Strings = (
                  'By MO'
                  'By Day')
              end
              object pnPrecautionEndTime: TPanel
                Left = 617
                Top = 482
                Width = 195
                Height = 26
                BevelOuter = bvNone
                Color = 13761532
                Font.Charset = ANSI_CHARSET
                Font.Color = clPurple
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 19
              end
              object cobNoteContents: TComboBox
                Left = 509
                Top = 38
                Width = 300
                Height = 26
                ItemHeight = 0
                TabOrder = 5
              end
              object Panel7: TPanel
                Left = 330
                Top = 482
                Width = 80
                Height = 26
                BevelOuter = bvNone
                Caption = 'day later'
                Color = 6055843
                Font.Charset = CHINESEBIG5_CHARSET
                Font.Color = clWhite
                Font.Height = -16
                Font.Name = #27161#26999#39636
                Font.Style = []
                ParentFont = False
                TabOrder = 18
              end
              object edPromptDay: TEdit
                Left = 272
                Top = 482
                Width = 55
                Height = 26
                TabOrder = 17
              end
              object DateTimePickerEnd: TDateTimePicker
                Left = 816
                Top = 336
                Width = 73
                Height = 26
                CalAlignment = dtaLeft
                Date = 42340.6477528009
                Time = 42340.6477528009
                DateFormat = dfShort
                DateMode = dmComboBox
                Kind = dtkDate
                ParseInput = False
                TabOrder = 9
                Visible = False
              end
              object DateTimePickerStart: TDateTimePicker
                Left = 816
                Top = 304
                Width = 73
                Height = 26
                CalAlignment = dtaLeft
                Date = 42340
                Time = 42340
                DateFormat = dfShort
                DateMode = dmComboBox
                Kind = dtkDate
                ParseInput = False
                TabOrder = 8
                Visible = False
              end
              object pnPRSpecificationNO: TPanel
                Left = 814
                Top = 482
                Width = 127
                Height = 26
                BevelOuter = bvNone
                Color = clLime
                Font.Charset = ANSI_CHARSET
                Font.Color = clPurple
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 20
              end
            end
          end
        end
        object tsHanderMajorMaintenance: TTabSheet
          Caption = 'Major Maintenance'
          ImageIndex = 1
          object pnHanderMajrMaintenance: TPanel
            Left = 0
            Top = 0
            Width = 993
            Height = 521
            BevelInner = bvLowered
            Color = 14072713
            TabOrder = 0
            object sbMajorMaintenanceDate: TSpeedButton
              Left = 109
              Top = 10
              Width = 100
              Height = 30
              Caption = #26085#26399':'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = [fsBold]
              ParentFont = False
              OnClick = sbMajorMaintenanceDateClick
            end
            object sbMajorMaintenanceStartTime: TSpeedButton
              Left = 214
              Top = 10
              Width = 100
              Height = 30
              Caption = #38283#22987#26178#38291':'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -15
              Font.Name = #27161#26999#39636
              Font.Style = [fsBold]
              ParentFont = False
              OnClick = sbMajorMaintenanceStartTimeClick
            end
            object sbMajorMaintenanceEndTime: TSpeedButton
              Left = 735
              Top = 453
              Width = 100
              Height = 30
              Caption = #32080#26463#26178#38291':'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -15
              Font.Name = #27161#26999#39636
              Font.Style = [fsBold]
              ParentFont = False
              OnClick = sbMajorMaintenanceEndTimeClick
            end
            object sbUndesirablePhenomenonClear: TSpeedButton
              Left = 10
              Top = 453
              Width = 100
              Height = 30
              Caption = 'Clear'
              Glyph.Data = {
                4A090000424D4A090000000000001A0000000C0000001C001C0001001800FFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFC8C8CAAEAFB1A4A5A797999B888A914444722E2E742B2A
                7836356C505167696A6D7475787D7E8185868985868985868985868985868984
                8689848689848689848689848689848689848689DBDBDCFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFB9B9D72A2A8E2426A02426A72426A92526A3262695575998DCE4
                E8E0EAEBDFEAEBDFEAEBDFEAEBDEEAEBDEEAEBDDEAEBDCEAEBDBEAEBDAE9EBD9
                E9EBD8E9EB8F9699D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFDCDCEC25279A282CB3
                3236B92326B22326B1272AB23638B624269F5F63A5F2FEFFF2FEFFF1FEFFF1FE
                FFF0FEFFEFFEFFEEFEFFEDFEFFEBFDFFEAFDFFEAFDFFE9FDFF929B9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFF3B3CA22126B5AEB0E6F7F7FD5B5ECA3135BBD4D5F1
                F6F6FC272BB525269CC9D4E7F1FEFFF0FEFFEFFEFFEEFEFFEDFEFFECFDFFEBFD
                FFEAFDFFE9FDFFE8FDFFE7FDFF929B9DD6D6D7FFFFFFFFFFFFFFFFFFF8F8FC24
                27AB2026C06267D2FBFBFEF7F7FDDBDCF4FFFFFFACADE52126B82326AF8084B3
                CDD7D8CCD7D8CCD7D8D9E7E9A8B3B57C81847C81847C81847C81849AA5A7E5FD
                FF919B9DD6D6D7FFFFFFFFFFFFFFFFFFD3D3EC2226B72025C42025C3777ADAFF
                FFFFFFFFFFC2C4EE2228C02026BE2126BA6C72B6EDFEFFECFDFFEBFDFFEAFDFF
                A1AAADBECCCFE8FDFFE6FDFFC7DADC90999CE5FDFF919B9DD6D6D7FFFFFFFFFF
                FFFFFFFFE9E9F62226BA1F25C92E34CCD3D5F4FFFFFFFFFFFFF7F7FD595CD320
                25C22126BC7076B4CBD6D8CAD6D8CAD6D8D8E9EAA0AAADC3D4D6E6FDFFE5FDFF
                D0E4E690999CE4FDFF909B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFF2629BA1E25
                CDAAACECFFFFFFB3B5EE7276DEFCFCFFF2F3FC242AC92125BCA5B2DDEAFDFFE9
                FDFFE8FDFFE7FDFF9FAAADC2D4D6E5FDFFE5FDFFCFE4E68F999CE2FDFF8F9B9D
                D6D6D7FFFFFFFFFFFFFFFFFFFFFFFF9193DB2026CA8F94EAC1C3F39094E98F93
                E8ACAFEDB9BBF02E34CD2B2FB3CCD9DFCFDEE0CFDEE0CEDEE0D8EDEF9EAAAD96
                A1A3A4B1B4A3B1B49AA7A98E999CE0FDFF8F9B9DD6D6D7FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFF4C4ECA5055D96F74E57F84E88488E8757AE45F64DD2A2FC0B8C7
                E7E1F5F7E0F5F7DFF5F7DFF5F7E2F9FBCADEE0BBCDCFBACDCFB9CDCFB8CDCFC1
                D8DBDEFDFF8E9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9596DF
                272BC62126CD2026CE2328C7444AC8D6EAF9E6FDFFE5FDFFE5FDFFE5FDFFE5FD
                FFE4FDFFE3FDFFE2FDFFE1FDFFDFFDFFDEFDFFDEFDFFDDFDFF8E9B9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC5D4D7E8FDFFE7FDFF
                E6FDFFE5FDFFE5FDFFE5FDFFE5FDFFE4FDFFE3FDFFE2FDFFE1FDFFE0FDFFDFFD
                FFDEFDFFDDFDFFDCFDFFDBFDFF8D9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFF999B9DC3D4D7DCF1F3D7EBEDD7EBEDD7EBEDD7EBEDD7EBED
                D6EBEDD5EBEDD4EBEDDBF6F8DFFDFFDEFDFFDEFDFFDCFDFFDBFDFFDAFDFFD9FC
                FF8D9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2
                D4D7D9EDEFD4E7E8D4E7E8D3E7E8D2E7E8D1E7E8D0E7E8D0E7E8CFE7E8D7F3F5
                CAE4E6C0DADCC0DADCBFDADCBDD9DCC2DFE2D9FCFF8D9A9DD6D6D7FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2D4D7E5FDFFE4FDFFE3FDFFE2
                FDFFE1FDFFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFF9AABAD8D9B9D94A4A793A4A7
                909FA28B999CD8FCFF8C9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFF999B9DC1D4D7CFE3E5C5D6D8C4D6D8C4D6D8C3D6D8C2D6D8C2D6D8C2
                D6D8C1D6D8CEEAEC99ABADB8D3D6D9FCFFD9FCFFC5E4E78A999CD6FCFF8B9A9D
                D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DBFD4D7E1FD
                FFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFF99AAADB8
                D3D6D9FCFFD7FCFFC4E4E78A999CD4FCFF8A9A9DD6D6D7FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFF989B9DBED4D7CCE3E5C2D6D8C2D6D8C2D6D8C1D6
                D8C1D6D8C0D6D8BFD6D8BFD6D8CDE9EC99AAADB8D3D6D7FCFFD5FCFFC2E4E789
                999CD2FCFF899A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                989B9DBCD4D7DEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFFD9FCFFD9FCFFD9FC
                FFD8FCFF98AAAD78828578828578828577828588999CD0FCFF899A9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DBBD4D7DCFDFFDBFDFF
                DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD3FAFDD3FBFEADE7
                EBC6F6F9CFFBFECEF9FCCFFCFF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFF989B9DBAD4D7DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFF
                D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFCEFAFDB3EBEFBEEFF2A6E3E7C4F6FACDFC
                FF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8
                D4D7D9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFF
                D1FCFFB1EAEED4FBFEDDFCFFE2FDFFC2EDF0A4E2E5849EA1D6D6D7FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8D4D7D9FCFFD8FCFFD7FCFFD6
                FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0FCFFD0FCFFA8E4E8DDFCFFE2FDFF
                E3FDFFE8FDFFEDFDFFB4D5D7D5D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFF989B9DB7D4D7D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0
                FCFFD0FCFFCFFCFFCCFBFEBAEBEEE2FDFFE3FDFFE8FDFFC9E2E4B0BCBEF7F8F8
                FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF979A9DACC7CAD2F8
                FBD1F8FBD0F8FBCFF8FBCEF8FBCDF8FBCDF8FBCCF8FBCBF8FBCAF8FBB2EAEEDE
                FBFDDCF9FCB4D0D2C0C5C7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFA7A9AB7176797176797076797076797076797076
                7970767970767970767970767970767985A6A99EB9BBDCDDDEFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
              OnClick = sbUndesirablePhenomenonClearClick
            end
            object sbCountermeasureClear: TSpeedButton
              Left = 420
              Top = 453
              Width = 100
              Height = 30
              Caption = 'Clear'
              Glyph.Data = {
                4A090000424D4A090000000000001A0000000C0000001C001C0001001800FFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFC8C8CAAEAFB1A4A5A797999B888A914444722E2E742B2A
                7836356C505167696A6D7475787D7E8185868985868985868985868985868984
                8689848689848689848689848689848689848689DBDBDCFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFB9B9D72A2A8E2426A02426A72426A92526A3262695575998DCE4
                E8E0EAEBDFEAEBDFEAEBDFEAEBDEEAEBDEEAEBDDEAEBDCEAEBDBEAEBDAE9EBD9
                E9EBD8E9EB8F9699D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFDCDCEC25279A282CB3
                3236B92326B22326B1272AB23638B624269F5F63A5F2FEFFF2FEFFF1FEFFF1FE
                FFF0FEFFEFFEFFEEFEFFEDFEFFEBFDFFEAFDFFEAFDFFE9FDFF929B9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFF3B3CA22126B5AEB0E6F7F7FD5B5ECA3135BBD4D5F1
                F6F6FC272BB525269CC9D4E7F1FEFFF0FEFFEFFEFFEEFEFFEDFEFFECFDFFEBFD
                FFEAFDFFE9FDFFE8FDFFE7FDFF929B9DD6D6D7FFFFFFFFFFFFFFFFFFF8F8FC24
                27AB2026C06267D2FBFBFEF7F7FDDBDCF4FFFFFFACADE52126B82326AF8084B3
                CDD7D8CCD7D8CCD7D8D9E7E9A8B3B57C81847C81847C81847C81849AA5A7E5FD
                FF919B9DD6D6D7FFFFFFFFFFFFFFFFFFD3D3EC2226B72025C42025C3777ADAFF
                FFFFFFFFFFC2C4EE2228C02026BE2126BA6C72B6EDFEFFECFDFFEBFDFFEAFDFF
                A1AAADBECCCFE8FDFFE6FDFFC7DADC90999CE5FDFF919B9DD6D6D7FFFFFFFFFF
                FFFFFFFFE9E9F62226BA1F25C92E34CCD3D5F4FFFFFFFFFFFFF7F7FD595CD320
                25C22126BC7076B4CBD6D8CAD6D8CAD6D8D8E9EAA0AAADC3D4D6E6FDFFE5FDFF
                D0E4E690999CE4FDFF909B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFF2629BA1E25
                CDAAACECFFFFFFB3B5EE7276DEFCFCFFF2F3FC242AC92125BCA5B2DDEAFDFFE9
                FDFFE8FDFFE7FDFF9FAAADC2D4D6E5FDFFE5FDFFCFE4E68F999CE2FDFF8F9B9D
                D6D6D7FFFFFFFFFFFFFFFFFFFFFFFF9193DB2026CA8F94EAC1C3F39094E98F93
                E8ACAFEDB9BBF02E34CD2B2FB3CCD9DFCFDEE0CFDEE0CEDEE0D8EDEF9EAAAD96
                A1A3A4B1B4A3B1B49AA7A98E999CE0FDFF8F9B9DD6D6D7FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFF4C4ECA5055D96F74E57F84E88488E8757AE45F64DD2A2FC0B8C7
                E7E1F5F7E0F5F7DFF5F7DFF5F7E2F9FBCADEE0BBCDCFBACDCFB9CDCFB8CDCFC1
                D8DBDEFDFF8E9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9596DF
                272BC62126CD2026CE2328C7444AC8D6EAF9E6FDFFE5FDFFE5FDFFE5FDFFE5FD
                FFE4FDFFE3FDFFE2FDFFE1FDFFDFFDFFDEFDFFDEFDFFDDFDFF8E9B9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC5D4D7E8FDFFE7FDFF
                E6FDFFE5FDFFE5FDFFE5FDFFE5FDFFE4FDFFE3FDFFE2FDFFE1FDFFE0FDFFDFFD
                FFDEFDFFDDFDFFDCFDFFDBFDFF8D9B9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFF999B9DC3D4D7DCF1F3D7EBEDD7EBEDD7EBEDD7EBEDD7EBED
                D6EBEDD5EBEDD4EBEDDBF6F8DFFDFFDEFDFFDEFDFFDCFDFFDBFDFFDAFDFFD9FC
                FF8D9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2
                D4D7D9EDEFD4E7E8D4E7E8D3E7E8D2E7E8D1E7E8D0E7E8D0E7E8CFE7E8D7F3F5
                CAE4E6C0DADCC0DADCBFDADCBDD9DCC2DFE2D9FCFF8D9A9DD6D6D7FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DC2D4D7E5FDFFE4FDFFE3FDFFE2
                FDFFE1FDFFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFF9AABAD8D9B9D94A4A793A4A7
                909FA28B999CD8FCFF8C9A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFF999B9DC1D4D7CFE3E5C5D6D8C4D6D8C4D6D8C3D6D8C2D6D8C2D6D8C2
                D6D8C1D6D8CEEAEC99ABADB8D3D6D9FCFFD9FCFFC5E4E78A999CD6FCFF8B9A9D
                D6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF999B9DBFD4D7E1FD
                FFE0FDFFDFFDFFDEFDFFDEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFF99AAADB8
                D3D6D9FCFFD7FCFFC4E4E78A999CD4FCFF8A9A9DD6D6D7FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFF989B9DBED4D7CCE3E5C2D6D8C2D6D8C2D6D8C1D6
                D8C1D6D8C0D6D8BFD6D8BFD6D8CDE9EC99AAADB8D3D6D7FCFFD5FCFFC2E4E789
                999CD2FCFF899A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                989B9DBCD4D7DEFDFFDDFDFFDCFDFFDBFDFFDAFDFFD9FCFFD9FCFFD9FCFFD9FC
                FFD8FCFF98AAAD78828578828578828577828588999CD0FCFF899A9DD6D6D7FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DBBD4D7DCFDFFDBFDFF
                DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD3FAFDD3FBFEADE7
                EBC6F6F9CFFBFECEF9FCCFFCFF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFF989B9DBAD4D7DAFDFFD9FCFFD9FCFFD9FCFFD9FCFFD8FCFF
                D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFCEFAFDB3EBEFBEEFF2A6E3E7C4F6FACDFC
                FF889A9DD6D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8
                D4D7D9FCFFD9FCFFD9FCFFD8FCFFD7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFF
                D1FCFFB1EAEED4FBFEDDFCFFE2FDFFC2EDF0A4E2E5849EA1D6D6D7FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF989B9DB8D4D7D9FCFFD8FCFFD7FCFFD6
                FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0FCFFD0FCFFA8E4E8DDFCFFE2FDFF
                E3FDFFE8FDFFEDFDFFB4D5D7D5D6D7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFF989B9DB7D4D7D7FCFFD6FCFFD5FCFFD4FCFFD3FCFFD2FCFFD1FCFFD0
                FCFFD0FCFFCFFCFFCCFBFEBAEBEEE2FDFFE3FDFFE8FDFFC9E2E4B0BCBEF7F8F8
                FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF979A9DACC7CAD2F8
                FBD1F8FBD0F8FBCFF8FBCEF8FBCDF8FBCDF8FBCCF8FBCBF8FBCAF8FBB2EAEEDE
                FBFDDCF9FCB4D0D2C0C5C7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFA7A9AB7176797176797076797076797076797076
                7970767970767970767970767970767985A6A99EB9BBDCDDDEFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
              OnClick = sbCountermeasureClearClick
            end
            object sbMajorMaintenanceSave: TSpeedButton
              Left = 838
              Top = 453
              Width = 100
              Height = 30
              Caption = 'Save '
              Glyph.Data = {
                66090000424D660900000000000036000000280000001C0000001C0000000100
                18000000000030090000232E0000232E00000000000000000000FFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FBFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFF7E7EC4CDCDEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFEFEFED0D1D2C9C9CBC9C9CBC9C9CBC9C9CBC9C9CBC9C9CB
                C9C9CBC9C9CBC9C9CBC9C9CBC9C9CBC9C9CBC9C9CB7C7DB32527A2A5A6C6C9C9
                CBC9C9CBC9C9CBC9C9CBC9C9CBD0D1D2FEFEFEFFFFFFFFFFFFFFFFFF8B8C8F50
                5357484A4E484A4E47494D3B474B4D6B704D6B704D6B704D6B704D6B704D6B70
                4D6B704D6B703F527E2527A12526A43544993A4F9439496C43454C484A4E484A
                4E5A5D60CFD0D1FFFFFFFFFFFFFFFFFF7C7E8140434740434740434734373CE1
                E2E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCCCE72628A226279F
                2326B21F25C61D25D61D25D71D25D02C328F3F42484B4E52C8C9CAFFFFFFFFFF
                FFFFFFFF7E8083484B4E484B4E484B4E373A3DF8F8F8E3E3E3E3E3E3E3E3E3E3
                E3E3E3E3E3E3E3E3C7C7D82729A326279F2326B12025C41D25D61D25D71D25D9
                1D25DA1D25D8242AAE4F525CC8C9CAFFFFFFFFFFFFFFFFFF8183865053565053
                565053563C3F43F8F8F8E2E2E3E2E2E3E2E2E3E2E2E3E2E2E3E0E0E23233A826
                279F2326B12025C41D25D61D25D71D25D91D25DA1D25DA1D25DA1D25D92A2FA7
                C8C9CAFFFFFFFFFFFFFFFFFF848688585A5D585A5D585A5D414347F7F8F8DCDC
                DDDCDCDDDCDCDDDCDCDDDCDCDDDCDCDDB1B2D4262ABD2025C51D25D61D25D729
                30D64349D6343BCD1E25CE1D25DA1D25DA1E25D18C8DB9FFFFFFFFFFFFFFFFFF
                87888B60626560626560626546494CF5F5F5D5D6D6D5D6D6D5D6D6D5D6D6D5D6
                D6D5D6D6D5D6D6CACBD5383FD71D25D71D25D6DADCF6FFFFFFFFFFFF33375221
                28BF1D25DA1D25DA3C40ABFFFFFFFFFFFFFFFFFF898B8D686A6C686A6C686A6C
                4C4E51F5F5F5D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D3
                D37A7EE41E26D2DBDCF3FFFFFFFFFFFF3C3E426062702227BC1D25DA262BB3FF
                FFFFFFFFFFFFFFFF8C8E90707274707274707274515356F5F5F5D2D2D3D2D2D3
                D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D3D3FEFEFEC3C5EDE7E8F6FFFF
                FFFFFFFF404246707274494C841E25D32328B4FFFFFFFFFFFFFFFFFF8F909378
                797B78797B78797B56585BF5F5F5D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3D2D2D3
                D2D2D3D2D2D3D2D3D3FEFEFEFFFFFFFFFFFFFFFFFFFFFFFF43454978797B7879
                7C2429B12B2FA9FFFFFFFFFFFFFFFFFF9293958081838081838081835B5D60F6
                F6F6D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5D4D4D5
                D4D4D5D4D4D5D4D4D5E4E4E447494D8081839393953C3F993E419DFFFFFFFFFF
                FFFFFFFF95969888898A88898A88898A606265F7F7F7DDDDDDDDDDDDDDDDDDDD
                DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDEBEBEB
                4B4D5088898AB1B2B36E6F92636599FFFFFFFFFFFFFFFFFF98989B9091929091
                92909192737476A3CCD0C2F0F2C2F0F2C2F0F2C2F0F2C2F0F2C2F0F2C2F0F2C2
                F0F2C2F0F2C2F0F2C2F0F2C2F0F2C2F0F2B9E7EB5C5E60A3A4A5B8B9BA91929E
                A7A8B5FFFFFFFFFFFFFFFFFF999A9C9495969495969495969495966D6F715F61
                635F61635F61635F61635F61635F61635F61635F61635F61635F61635F61635F
                61635F616366686B98999ABABABBB3B4B4A7A8AAC8C9CAFFFFFFFFFFFFFFFFFF
                999A9C9495969495969495969495968C8E8F7375787274767274767274767274
                76727476727476727476727476727476727476727476727476959798BBBCBDBB
                BBBCB3B4B4A7A8AAC8C9CAFFFFFFFFFFFFFFFFFF999A9C949596949596949596
                949596494C5037393E87888A8C8D8E8C8D8E8C8D8E8C8D8E8C8D8E8C8D8E8C8D
                8E898A8C43464A1E212624272C737579BFBFC0B3B4B4ABACADA2A3A4C8C9CAFF
                FFFFFFFFFFFFFFFF999A9C94959694959694959694959647494D5C5D61828386
                56585C56585C8A8C8DA2A3A4A2A3A4A2A3A4A2A3A4A2A3A46E6F722D2F347C7D
                8088898CBFC0C1B3B4B4ABACADA2A3A4C8C9CAFFFFFFFFFFFFFFFFFF999A9C94
                95969495969495969495964D4F526466697072742D30342D30347B7C7EABACAD
                ABACADABACADABACADABACAD7F818383848788898C7D7F81B8B9B9ABACADA4A5
                A5A1A2A4C8C9CAFFFFFFFFFFFFFFFFFF999A9C94959694959694959694959653
                54586C6E7077797C35373B35373B828385B4B5B6B4B5B6B4B5B6B4B5B6C2C3C4
                BBBCBD8C8D907E7F82737477B1B2B2A4A5A5A4A5A59C9D9FC8C9CAFFFFFFFFFF
                FFFFFFFF999A9C949596949596949596949596585A5D7476787E80823C3E423C
                3E42898A8CBDBEBFBDBEBFBFC0C1D6D7D7E3E4E4C4C4C6909194828486787A7C
                B1B2B2A4A5A5A4A5A59C9D9FC8C9CAFFFFFFFFFFFFFFFFFF999A9C9495969495
                969495969495965E60627D7F80878889444649444649919293C7C8C8D4D5D5E7
                E8E8E7E8E8E4E4E4C0C1C287898B7A7B7D717374AAABABA4A5A59C9D9E969799
                C8C9CAFFFFFFFFFFFFFFFFFF98999B9495969495969495969495966465688586
                898E8F914C4D514C4D51A9AAABE7E8E8EFEFEFEBECECE8E9E9E5E5E5BEBFBF7F
                80837373766A6B6EAAABAB9C9D9E949596969799C8C9CAFFFFFFFFFFFFFFFFFF
                BCBDBF8E90929495969495969495966A6B6E8D8E91B3B4B6A7A7AAD0D0D1EBEC
                ECF2F2F2F2F2F2EFF0F0EDEDEDEAEAEAC3C4C584858878797C707174AAABAB9C
                9D9E949596969799C8C9CAFFFFFFFFFFFFFFFFFFFFFFFFD0D1D2919294A2A3A4
                A2A3A4AFB0B1DADBDBF9F9F9F9F9F9F8F8F8F6F6F6F6F6F6F4F4F4F3F3F3F1F1
                F1F0F0F0D3D3D4A6A7A8A0A1A2A4A5A6AFB0B1A2A3A4A2A3A4A3A4A5C8C9CAFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFE3E4E47F8083737678BABABCC1C2C4C1C2C4
                C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2C4C1C2
                C4C1C3C47B7C7F747578747578797B7EE0E1E2FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFF}
              OnClick = sbMajorMaintenanceSaveClick
            end
            object sbUndesirablePhenomenon: TSpeedButton
              Tag = 1
              Left = 325
              Top = 119
              Width = 80
              Height = 30
              Caption = 'Add'
              Glyph.Data = {
                66090000424D660900000000000036000000280000001C0000001C0000000100
                18000000000030090000232E0000232E00000000000000000000FFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F7
                F3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E8173757892949686888A75777A707275
                7E80828D8E919B9C9EB5B6B7CECFD0DBDBDCF0F0F0F6F6F7FFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFF56585B747679DEDFDFDEDFDFDDDEDEDDDEDEDDDEDEDDDEDEDDDEDED0D1D2
                B6B8B990919383858877797B78797C8285878D8E90969799BEBFC0FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF484A4E8A8B8EDFE0E0DF
                E0E0DFE0E0DFE0E0DFE0E0DFE0E0DFE0E0DEDFDFDDDEDEC9CACBBEBFC0C6C7C7
                DDDEDEDDDEDEDDDEDEDDDEDEB6B7B8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFF3A3D41A0A2A3E1E2E2E1E2E2E0E1E1E0E1E1E0E1E1E0
                E1E1E0E1E1DFE0E0DFE0E0D3D4D5868889B7B9BAD2D3D3DDDEDEDFE0E0DFE0E0
                9FA1A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2C2E
                33B6B8B9E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E1E2E2E1E2E2DD
                DEDEBECACF3E72AAAFB1B2D0D1D1DFE0E0E0E1E1A0A2A3FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1E2126CECFD0E4E5E5B7B8B97B7C
                7E898A8C969799A5A6A7B4B5B6C1C2C2CECFD0D9DADBBDBFC04895C89D9EA0C1
                C2C3DBDCDCE2E3E3A6A8A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFF9F9F9191C21E3E3E3E6E6E6E6E6E6DCDDDDCDCECEBDBEBFAFB0B2A1A2
                A4929395838487737477656669B28C56803B2187888AD2D3D4E2E3E3B2B3B4FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEBEB2B2E32E7E8E8
                E7E8E8E7E8E8E7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E6E6E6E6E6E6E6E6E6E0E0
                E0F07F29BA5E1E97989AC4C6C7DFE0E0C2C3C4FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFDDDDDE424448E9E9E9E9E9E998989A858688939496
                A1A2A4B0B1B2C0C1C2CECFD0DCDDDEE7E8E8E3E4E4F57023C16820833D22BBBB
                BCDBDBDBD8D9DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF
                D0D1585B5EEAEBEBEAEBEBE7E8E8D7D8D8C8C8C9B8B8BAA8A9AA9A9B9D8C8C8E
                7C7D7F707174717174717274CC823B8E4F16A9ABACC1C3C3EFEFEFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1C2C46F7174EBECECEBECECEB
                ECECEBECECEBECECEBECECEBECECEBECECEAEBEBEAEBEBEAEBEBE9EAEACFD0D1
                F06F23BE641F8E46279D9E9FF1F1F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFB3B4B586888AECECECE1E1E1747577747578838486939496A2
                A3A4B0B1B2C0C1C1CFD0D1DFE0E0EBECECE2E3E3BFC0C1C26B217E3A21858789
                EAEBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA5A6A89D9F
                A1EDEEEEEDEEEEEDEEEEE9E9EADADADBCBCBCCBCBDBEACADAE9C9D9F8E8E907F
                80827273756F7072757678F37827B65C1E6F6F70DEDEDFFDFDFDFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF97999BB6B6B8EFEFEFEDEDEDE7E7E8EEEF
                EFEEEFEFEEEFEFEEEFEFEEEFEFEDEEEEEDEEEEEDEEEEEDEEEEEDEEEEDDDEDEEE
                7126BC6420843E21C6C7C8F6F6F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFF898A8DCDCFD0F0F1F1CBCCCD7A7B7E737477757679838486939395A3A3
                A5B2B3B4C1C1C2D0D0D1E0E0E1EEEEEEEEEFEFDCDEDED58D3DAF5C1AB8B9BBEB
                ECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7B7D80E6E6E6F2F2F2
                F2F2F2F2F2F2F1F2F2EFF0F0E0E1E1D1D2D2C1C2C3B0B1B3A2A3A49394968283
                867577799E9EA0E5E5E6F47224BE641F8D4525DFDFE0FBFBFBFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFF737578F3F4F4F3F4F4DADBDBD5D6D6E4E4E4F2F2F2
                F3F3F3F3F3F3F3F3F3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F2F2F0F1F1E1E2
                E3C46E21803B20BBBCBDE2E3E3FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF77
                797BF5F5F5F5F5F5B8B8BA9293958182847475787475777F80828E8F919D9EA0
                ADAEB0BEBFC0CDCECFDDDEDEF2F2F2F3F3F3ECEDEDB9B8B86C6C6C9E9C9DE1E2
                E2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E81F7F7F7F7F7F7F7F7F7F7
                F7F7F6F6F6F6F6F6F6F6F6ECECECDCDCDCCBCCCCBABBBCABABAD9C9C9E8B8C8E
                CECFD0F4F5F5EFF0F0B8B8B8A2A2A2949494F0F0F1FEFEFEFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFF85878AF8F8F8F8F8F8BABBBCF8F8F8F8F8F8F8F8F8F8F8F8F8
                F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F6F6F6F6F6F6DEDEDFFFFFFF
                8D87EB4239DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF939497FAFA
                FAD7D7D8767879E4E4E5F9F9F99E9FA1686A6DF9F9F9F9F9F97D7E82DDDDDEF8
                F8F8F8F8F8E1E2E2F8F8F8F8F8F8D2D2D3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0A1A3FBFBFBF1F1F1C1C2C3FBFBFBFBFB
                FBD0D0D1BABBBCFBFBFBFBFBFB747678C7C7C8FAFAFAB9BABB616365F5F5F5F9
                F9F9CACACBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFF2F2F2CECFCFBBBCBDD2D3D4898A8D8A8C8EBBBCBDC3C4C5BEBFC0E9EA
                EABDBEBFFBFBFBFBFBFBECECECC1C2C3FBFBFBFBFBFBC7C8C9FFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4E4E4
                CFD0D0FFFFFFFFFFFFC8C8C9DEDEDFFFFFFFFEFEFEE4E4E5C4C6C7AAAAADAEAF
                B1C9CACB92939697999ABFBFC1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFF9F9F9CDCECEFFFFFFFFFFFFD7D7D8D1D1D2FFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFF}
              OnClick = sbUndesirablePhenomenonClick
            end
            object sbCountermeasure: TSpeedButton
              Tag = 2
              Left = 853
              Top = 119
              Width = 80
              Height = 30
              Caption = 'Add'
              Glyph.Data = {
                66090000424D660900000000000036000000280000001C0000001C0000000100
                18000000000030090000232E0000232E00000000000000000000FFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6F7
                F3F3F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E8173757892949686888A75777A707275
                7E80828D8E919B9C9EB5B6B7CECFD0DBDBDCF0F0F0F6F6F7FFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFF56585B747679DEDFDFDEDFDFDDDEDEDDDEDEDDDEDEDDDEDEDDDEDED0D1D2
                B6B8B990919383858877797B78797C8285878D8E90969799BEBFC0FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF484A4E8A8B8EDFE0E0DF
                E0E0DFE0E0DFE0E0DFE0E0DFE0E0DFE0E0DEDFDFDDDEDEC9CACBBEBFC0C6C7C7
                DDDEDEDDDEDEDDDEDEDDDEDEB6B7B8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFF3A3D41A0A2A3E1E2E2E1E2E2E0E1E1E0E1E1E0E1E1E0
                E1E1E0E1E1DFE0E0DFE0E0D3D4D5868889B7B9BAD2D3D3DDDEDEDFE0E0DFE0E0
                9FA1A3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2C2E
                33B6B8B9E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E2E3E3E1E2E2E1E2E2DD
                DEDEBECACF3E72AAAFB1B2D0D1D1DFE0E0E0E1E1A0A2A3FFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1E2126CECFD0E4E5E5B7B8B97B7C
                7E898A8C969799A5A6A7B4B5B6C1C2C2CECFD0D9DADBBDBFC04895C89D9EA0C1
                C2C3DBDCDCE2E3E3A6A8A9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFF9F9F9191C21E3E3E3E6E6E6E6E6E6DCDDDDCDCECEBDBEBFAFB0B2A1A2
                A4929395838487737477656669B28C56803B2187888AD2D3D4E2E3E3B2B3B4FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEAEBEB2B2E32E7E8E8
                E7E8E8E7E8E8E7E7E7E7E7E7E7E7E7E7E7E7E7E7E7E6E6E6E6E6E6E6E6E6E0E0
                E0F07F29BA5E1E97989AC4C6C7DFE0E0C2C3C4FFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFDDDDDE424448E9E9E9E9E9E998989A858688939496
                A1A2A4B0B1B2C0C1C2CECFD0DCDDDEE7E8E8E3E4E4F57023C16820833D22BBBB
                BCDBDBDBD8D9DAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF
                D0D1585B5EEAEBEBEAEBEBE7E8E8D7D8D8C8C8C9B8B8BAA8A9AA9A9B9D8C8C8E
                7C7D7F707174717174717274CC823B8E4F16A9ABACC1C3C3EFEFEFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC1C2C46F7174EBECECEBECECEB
                ECECEBECECEBECECEBECECEBECECEBECECEAEBEBEAEBEBEAEBEBE9EAEACFD0D1
                F06F23BE641F8E46279D9E9FF1F1F1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFB3B4B586888AECECECE1E1E1747577747578838486939496A2
                A3A4B0B1B2C0C1C1CFD0D1DFE0E0EBECECE2E3E3BFC0C1C26B217E3A21858789
                EAEBEBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA5A6A89D9F
                A1EDEEEEEDEEEEEDEEEEE9E9EADADADBCBCBCCBCBDBEACADAE9C9D9F8E8E907F
                80827273756F7072757678F37827B65C1E6F6F70DEDEDFFDFDFDFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF97999BB6B6B8EFEFEFEDEDEDE7E7E8EEEF
                EFEEEFEFEEEFEFEEEFEFEEEFEFEDEEEEEDEEEEEDEEEEEDEEEEEDEEEEDDDEDEEE
                7126BC6420843E21C6C7C8F6F6F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFF898A8DCDCFD0F0F1F1CBCCCD7A7B7E737477757679838486939395A3A3
                A5B2B3B4C1C1C2D0D0D1E0E0E1EEEEEEEEEFEFDCDEDED58D3DAF5C1AB8B9BBEB
                ECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7B7D80E6E6E6F2F2F2
                F2F2F2F2F2F2F1F2F2EFF0F0E0E1E1D1D2D2C1C2C3B0B1B3A2A3A49394968283
                867577799E9EA0E5E5E6F47224BE641F8D4525DFDFE0FBFBFBFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFF737578F3F4F4F3F4F4DADBDBD5D6D6E4E4E4F2F2F2
                F3F3F3F3F3F3F3F3F3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F2F2F0F1F1E1E2
                E3C46E21803B20BBBCBDE2E3E3FDFDFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF77
                797BF5F5F5F5F5F5B8B8BA9293958182847475787475777F80828E8F919D9EA0
                ADAEB0BEBFC0CDCECFDDDEDEF2F2F2F3F3F3ECEDEDB9B8B86C6C6C9E9C9DE1E2
                E2FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7C7E81F7F7F7F7F7F7F7F7F7F7
                F7F7F6F6F6F6F6F6F6F6F6ECECECDCDCDCCBCCCCBABBBCABABAD9C9C9E8B8C8E
                CECFD0F4F5F5EFF0F0B8B8B8A2A2A2949494F0F0F1FEFEFEFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFF85878AF8F8F8F8F8F8BABBBCF8F8F8F8F8F8F8F8F8F8F8F8F8
                F8F8F8F8F8F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F6F6F6F6F6F6DEDEDFFFFFFF
                8D87EB4239DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF939497FAFA
                FAD7D7D8767879E4E4E5F9F9F99E9FA1686A6DF9F9F9F9F9F97D7E82DDDDDEF8
                F8F8F8F8F8E1E2E2F8F8F8F8F8F8D2D2D3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA0A1A3FBFBFBF1F1F1C1C2C3FBFBFBFBFB
                FBD0D0D1BABBBCFBFBFBFBFBFB747678C7C7C8FAFAFAB9BABB616365F5F5F5F9
                F9F9CACACBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFF2F2F2CECFCFBBBCBDD2D3D4898A8D8A8C8EBBBCBDC3C4C5BEBFC0E9EA
                EABDBEBFFBFBFBFBFBFBECECECC1C2C3FBFBFBFBFBFBC7C8C9FFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE4E4E4
                CFD0D0FFFFFFFFFFFFC8C8C9DEDEDFFFFFFFFEFEFEE4E4E5C4C6C7AAAAADAEAF
                B1C9CACB92939697999ABFBFC1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFF9F9F9CDCECEFFFFFFFFFFFFD7D7D8D1D1D2FFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFF}
              OnClick = sbCountermeasureClick
            end
            object sbMajorMaintenanceSearch: TSpeedButton
              Left = 618
              Top = 486
              Width = 95
              Height = 30
              Caption = 'Search'
              Glyph.Data = {
                4A090000424D4A090000000000001A0000000C0000001C001C0001001800FFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8E9E9E9E0E1E2E0E1E2E5E5
                E6F4F4F4F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FDFDFDFFFFFFF7F7F7D9DADAB9BABB3A3C40ABADAECECFD1E7E7E8F9F9F9FEFE
                FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFBDEDEDF
                84868935383C606265505256C2C3C4DADBDBE5E5E6EEEEEFF6F6F7FCFCFCFEFE
                FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBD0D1D22E313536383C3F4145
                6A6C6FACADAFB3B4B5C0C0C2CDCECFE6E6E7F0F0F1F6F6F7F7F7F8F7F7F8F6F6
                F7F7F7F8F7F7F8FAFAFAFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFB1B2B442444846474B3D3F4386878AB4B5B6B6B7B9
                BABBBCC7C8C9CDCECFD3D3D4D3D3D4D1D2D3CECFD0D1D2D3D3D3D4DDDDDEE3E4
                E4F2F2F2F6F6F7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFF9E9FA14345494244483A3C409E9FA1BDBEBFB4B5B6A8A9ABA6A7A9A6A7A9
                A6A7A9A3A4A6A1A2A4A3A4A6A6A7A9B1B2B4BBBCBDD4D4D5DFDFE0EAEAEAFAFA
                FAFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF87888A44454940
                42463C3E42A4A5A7A9AAAC9697999293969192959092948E90928D8F918E9092
                9092949697999A9B9EADAEB0BABBBCCACACBE4E4E5F0F0F1F9F9F9FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC6F707346474B3D3F433F404597989C8E
                90928D8F918D8F918D8F918D8F918D8F918D8F918D8F918D8F918D8F91939497
                9D9EA0ADAEB0CDCECFDEDEDFEDEDEEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFDCDCDE5D5E614345493B3D415253578E90928E90928E90928E90928E
                90928E90928E90928E90928E90928E90928F9193929396A2A4A6ADAEB0C9C9CB
                D9DADAFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2D2D3D45354
                58404246383A3E77787B8E90928E90928D8F918284867577797B7D80888A8C8E
                90928E90928E90929293969E9FA1A9AAACCACACBDADBDBFAFAFAFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFAFAFADEDEE067686B3D3F4377797C8D8E
                9050525634353943454956585B5A5C5F52545758595E7B7D809092949C9D9FA9
                AAACB9BABCDADBDBE9E9E9FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFF2F2F2CBCCCD25282D30323765676996979898999A9798
                999697989596979091926F7072717274C4C5C6D0D1D2DDDDDEF2F2F2F9F9F9FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
                9FA0A23E404498999A9FA0A0A19D94B29B70BB995BBE9A57B99D6BA8A399999A
                9B929394808184E9EAEAF4F4F4FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCACBCC383A3E999A9B9D9E9EC1A778
                DFB76EE2BB73E0B667DEB05BD8A444CE9326C4A161A6A6A5979899ABABADFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFF626468949597A2A3A4CCB17EE6C17FE8C587E5BF79E5BF79E5BF79
                E4BD76E5C07DDCAD57CA9F50A9A8A7969798F4F4F4FFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFE05D5F62D7D8D8C0
                AD8AE5C17CE9C584E8C17BE5BB6FE5BB6FE5BB6FE4B96AE5BF79E2B970DCB05F
                C8AD7EA2A3A4AFB0B1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFAAABADB9B9BAC6C7C8DCBA7CEDD19DECCE96ECCB8EED
                C986E6BA66E6BA66E2B45EE4B96AE0B462DDAD58D7A651E7E7E7EEEEEEFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8E90
                92CCCCCDBEB7A9E5C180EDD3A3F0D7A8F0D4A1F3D9A7F2D094EEC780E9C176E1
                B25BDDAB4FD9A443D49D38E2E3E3F3F3F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA1A2A4CACBCCBAAC92EACC96ECCE
                99F4E2BEF2DAAEF5DBADF7DCAAF1D094E8C075E5BB6CE1B461D9A443D49D38E1
                E2E2EAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFC3C4C5BFC0C1B9B1A1EED6ABECD09DF6E6C7F6E4C3F7E3BFF5DB
                ADF3D9A9EECF95E4BA6BE0B360D8A341D39B36E0E1E1E1E1E2FFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEFEFA2A4A5
                B9BAB9F0DDBAF0DAB2F6E7CBFAEFDAFBEFDAF8EACFF7E4C3EFD4A3E6C07AE3BC
                73DBAB54D8A54BDCDDDDD8D8D9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA7A9ABC2C3C4DAC7A5F4E5CAF7EBD4
                F7E9D0FAF1E0F9EDD9F8EAD2F8E8CEEDD09BE5C180DEB365DCBB80DCDDDDE1E1
                E1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFECEDEDC2C3C3C0BDB7F1E2C7F8EFDDF6E8CEF8ECD5F9EFDCF8EDD9
                F7EAD2F3E0BEECD09FE1BC7AD3CDBEC7C8C9F5F6F6FFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0D1D2CC
                CDCDC7C2B6F4E8D1F7ECD8F7EDD9F6E8CEF7ECD7F8EEDAF3E1C2EDD7B0D6CDBB
                C1C2C2DCDDDDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D8CFD0D1CAC9C7DACAAEF1
                E3C9F4E7CFF4E6CDEEDCBBE2CFABD3D1CDC5C6C7DBDBDCFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFEEEFEFD1D2D3D1D2D3D0D1D2CFD0D1CECFCFCDCECECC
                CDCDCFD0D0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFF1F2F2E5E5E5DEDFDFDDDEDEE4E4E5ECECEDFDFDFDFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
              OnClick = sbMajorMaintenanceSearchClick
            end
            object sbScreenkeyboard2: TSpeedButton
              Left = 827
              Top = 11
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
              OnClick = sbScreenkeyboardClick
            end
            object pnMajorMaintenanceDate: TPanel
              Left = 110
              Top = 50
              Width = 100
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
            end
            object pnMajorMaintenanceClassType: TPanel
              Left = 5
              Top = 10
              Width = 100
              Height = 26
              BevelOuter = bvNone
              Caption = #29677#21029':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object cobMajorMaintenanceClassType: TComboBox
              Left = 5
              Top = 50
              Width = 100
              Height = 21
              ItemHeight = 13
              TabOrder = 4
              Items.Strings = (
                'A'
                'B'
                'C'
                'D'
                'E')
            end
            object pnMajorMaintenanceStartTime: TPanel
              Left = 214
              Top = 50
              Width = 100
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
            end
            object pnMajorMaintenanceEndTime: TPanel
              Left = 735
              Top = 486
              Width = 100
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 18
            end
            object pnMajorMaintenanceCheckNo: TPanel
              Left = 317
              Top = 10
              Width = 200
              Height = 26
              BevelOuter = bvNone
              Caption = #30906#35469#25209#34399':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object pnMajorMaintenancePersonnel: TPanel
              Left = 520
              Top = 10
              Width = 150
              Height = 26
              BevelOuter = bvNone
              Caption = #32173#20462#32773':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 2
            end
            object pnMajorMaintenanceCheckPersonnel: TPanel
              Left = 673
              Top = 10
              Width = 150
              Height = 26
              BevelOuter = bvNone
              Caption = #30906#35469#32773':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 3
            end
            object edMajorMaintenanceCheckNo: TEdit
              Left = 317
              Top = 50
              Width = 200
              Height = 21
              TabOrder = 7
            end
            object edMajorMaintenancePersonnel: TEdit
              Left = 519
              Top = 50
              Width = 150
              Height = 21
              TabOrder = 8
            end
            object edMajorMaintenanceCheckPersonnel: TEdit
              Left = 671
              Top = 50
              Width = 150
              Height = 21
              TabOrder = 9
            end
            object pnUndesirablePhenomenon: TPanel
              Left = 5
              Top = 90
              Width = 400
              Height = 26
              BevelOuter = bvNone
              Caption = #19981#33391#29694#35937':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 11
            end
            object pnCountermeasure: TPanel
              Left = 414
              Top = 90
              Width = 525
              Height = 26
              BevelOuter = bvNone
              Caption = #34389#29702#23565#31574':'
              Color = 6055843
              Font.Charset = CHINESEBIG5_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 12
            end
            object MemoUndesirablePhenomenon: TMemo
              Left = 5
              Top = 152
              Width = 399
              Height = 297
              Color = 14670284
              TabOrder = 15
            end
            object MemoCountermeasure: TMemo
              Left = 415
              Top = 152
              Width = 525
              Height = 297
              Color = 14670284
              TabOrder = 16
            end
            object pnMMSpecificationNO: TPanel
              Left = 839
              Top = 487
              Width = 97
              Height = 26
              BevelOuter = bvNone
              Color = clLime
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 20
            end
            object cobUndesirablePhenomenon: TComboBox
              Left = 5
              Top = 120
              Width = 313
              Height = 21
              ItemHeight = 0
              TabOrder = 13
            end
            object cobCountermeasure: TComboBox
              Left = 415
              Top = 120
              Width = 429
              Height = 21
              ItemHeight = 0
              TabOrder = 14
            end
            object Panel10: TPanel
              Left = 10
              Top = 486
              Width = 120
              Height = 30
              BevelOuter = bvNone
              Caption = #25628#23563#27284#26696#65306
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 17
            end
            object cobMajorMaintenanceSearch: TComboBox
              Left = 132
              Top = 487
              Width = 477
              Height = 27
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'Times New Roman'
              Font.Style = [fsBold]
              ItemHeight = 0
              ParentFont = False
              TabOrder = 19
            end
            object FileListBoxMajorMaintenance: TFileListBox
              Tag = 4
              Left = 899
              Top = 51
              Width = 41
              Height = 33
              ItemHeight = 13
              Mask = '*.csv'
              TabOrder = 10
              Visible = False
            end
          end
        end
        object tsPrecautionLog: TTabSheet
          Caption = 'Precaution Log'
          ImageIndex = 2
          object pnPrecautionLog: TPanel
            Left = 2
            Top = 3
            Width = 990
            Height = 520
            BevelInner = bvLowered
            Color = 14072713
            TabOrder = 0
            object sbSearchPrecautionLog: TSpeedButton
              Left = 650
              Top = 10
              Width = 125
              Height = 30
              Caption = 'Search'
              Glyph.Data = {
                4A090000424D4A090000000000001A0000000C0000001C001C0001001800FFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8E9E9E9E0E1E2E0E1E2E5E5
                E6F4F4F4F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FDFDFDFFFFFFF7F7F7D9DADAB9BABB3A3C40ABADAECECFD1E7E7E8F9F9F9FEFE
                FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFBDEDEDF
                84868935383C606265505256C2C3C4DADBDBE5E5E6EEEEEFF6F6F7FCFCFCFEFE
                FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBD0D1D22E313536383C3F4145
                6A6C6FACADAFB3B4B5C0C0C2CDCECFE6E6E7F0F0F1F6F6F7F7F7F8F7F7F8F6F6
                F7F7F7F8F7F7F8FAFAFAFCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFB1B2B442444846474B3D3F4386878AB4B5B6B6B7B9
                BABBBCC7C8C9CDCECFD3D3D4D3D3D4D1D2D3CECFD0D1D2D3D3D3D4DDDDDEE3E4
                E4F2F2F2F6F6F7FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFF9E9FA14345494244483A3C409E9FA1BDBEBFB4B5B6A8A9ABA6A7A9A6A7A9
                A6A7A9A3A4A6A1A2A4A3A4A6A6A7A9B1B2B4BBBCBDD4D4D5DFDFE0EAEAEAFAFA
                FAFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF87888A44454940
                42463C3E42A4A5A7A9AAAC9697999293969192959092948E90928D8F918E9092
                9092949697999A9B9EADAEB0BABBBCCACACBE4E4E5F0F0F1F9F9F9FFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFC6F707346474B3D3F433F404597989C8E
                90928D8F918D8F918D8F918D8F918D8F918D8F918D8F918D8F918D8F91939497
                9D9EA0ADAEB0CDCECFDEDEDFEDEDEEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFDCDCDE5D5E614345493B3D415253578E90928E90928E90928E90928E
                90928E90928E90928E90928E90928E90928F9193929396A2A4A6ADAEB0C9C9CB
                D9DADAFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2F2F2D2D3D45354
                58404246383A3E77787B8E90928E90928D8F918284867577797B7D80888A8C8E
                90928E90928E90929293969E9FA1A9AAACCACACBDADBDBFAFAFAFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFAFAFADEDEE067686B3D3F4377797C8D8E
                9050525634353943454956585B5A5C5F52545758595E7B7D809092949C9D9FA9
                AAACB9BABCDADBDBE9E9E9FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFF2F2F2CBCCCD25282D30323765676996979898999A9798
                999697989596979091926F7072717274C4C5C6D0D1D2DDDDDEF2F2F2F9F9F9FF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFE
                9FA0A23E404498999A9FA0A0A19D94B29B70BB995BBE9A57B99D6BA8A399999A
                9B929394808184E9EAEAF4F4F4FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCACBCC383A3E999A9B9D9E9EC1A778
                DFB76EE2BB73E0B667DEB05BD8A444CE9326C4A161A6A6A5979899ABABADFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFF626468949597A2A3A4CCB17EE6C17FE8C587E5BF79E5BF79E5BF79
                E4BD76E5C07DDCAD57CA9F50A9A8A7969798F4F4F4FFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFE05D5F62D7D8D8C0
                AD8AE5C17CE9C584E8C17BE5BB6FE5BB6FE5BB6FE4B96AE5BF79E2B970DCB05F
                C8AD7EA2A3A4AFB0B1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFAAABADB9B9BAC6C7C8DCBA7CEDD19DECCE96ECCB8EED
                C986E6BA66E6BA66E2B45EE4B96AE0B462DDAD58D7A651E7E7E7EEEEEEFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8E90
                92CCCCCDBEB7A9E5C180EDD3A3F0D7A8F0D4A1F3D9A7F2D094EEC780E9C176E1
                B25BDDAB4FD9A443D49D38E2E3E3F3F3F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA1A2A4CACBCCBAAC92EACC96ECCE
                99F4E2BEF2DAAEF5DBADF7DCAAF1D094E8C075E5BB6CE1B461D9A443D49D38E1
                E2E2EAEAEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFC3C4C5BFC0C1B9B1A1EED6ABECD09DF6E6C7F6E4C3F7E3BFF5DB
                ADF3D9A9EECF95E4BA6BE0B360D8A341D39B36E0E1E1E1E1E2FFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEEEFEFA2A4A5
                B9BAB9F0DDBAF0DAB2F6E7CBFAEFDAFBEFDAF8EACFF7E4C3EFD4A3E6C07AE3BC
                73DBAB54D8A54BDCDDDDD8D8D9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA7A9ABC2C3C4DAC7A5F4E5CAF7EBD4
                F7E9D0FAF1E0F9EDD9F8EAD2F8E8CEEDD09BE5C180DEB365DCBB80DCDDDDE1E1
                E1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFECEDEDC2C3C3C0BDB7F1E2C7F8EFDDF6E8CEF8ECD5F9EFDCF8EDD9
                F7EAD2F3E0BEECD09FE1BC7AD3CDBEC7C8C9F5F6F6FFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD0D1D2CC
                CDCDC7C2B6F4E8D1F7ECD8F7EDD9F6E8CEF7ECD7F8EEDAF3E1C2EDD7B0D6CDBB
                C1C2C2DCDDDDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD7D7D8CFD0D1CAC9C7DACAAEF1
                E3C9F4E7CFF4E6CDEEDCBBE2CFABD3D1CDC5C6C7DBDBDCFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFEEEFEFD1D2D3D1D2D3D0D1D2CFD0D1CECFCFCDCECECC
                CDCDCFD0D0E9E9E9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
                FFFFFFFFF1F2F2E5E5E5DEDFDFDDDEDEE4E4E5ECECEDFDFDFDFFFFFFFFFFFFFF
                FFFFFFFFFFFFFFFFFFFFFFFFFFFF}
              OnClick = sbSearchPrecautionLogClick
            end
            object Bevel1: TBevel
              Left = 0
              Top = 50
              Width = 990
              Height = 3
              Shape = bsFrame
              Style = bsRaised
            end
            object MemoNoteLog: TMemo
              Left = 10
              Top = 182
              Width = 719
              Height = 203
              Color = 14670284
              Font.Charset = ANSI_CHARSET
              Font.Color = clNavy
              Font.Height = -15
              Font.Name = 'Courier New'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 9
            end
            object pnDOCUMENTNO: TPanel
              Left = 10
              Top = 60
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = 'DOCUMENT NO.:'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 4
            end
            object pnNoteLog: TPanel
              Left = 10
              Top = 148
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #27880#24847#20107#38917#20839#23481':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 8
            end
            object pnApprovedManager: TPanel
              Left = 10
              Top = 396
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #37096#38272#20027#31649#26680#20934':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 10
            end
            object pnWatchmakers: TPanel
              Left = 420
              Top = 398
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #35069#34920#32773':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 12
            end
            object pnPrecautionLogPromptDay: TPanel
              Left = 352
              Top = 478
              Width = 57
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 21
            end
            object pnFinishType: TPanel
              Left = 10
              Top = 478
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #32080#26696#26041#24335':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 18
            end
            object pnFinishName: TPanel
              Left = 420
              Top = 438
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #32080#26696#32773':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 16
            end
            object pnPrecautionLogFinishName: TPanel
              Left = 550
              Top = 438
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 17
            end
            object pnPrecautionLogDocumentNo: TPanel
              Left = 140
              Top = 60
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
            end
            object pnPrecautionLogNoteContents: TPanel
              Left = 140
              Top = 100
              Width = 533
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 7
            end
            object pnStartTime: TPanel
              Left = 10
              Top = 438
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #38283#22987#26085#26399':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 14
            end
            object pnEndTime: TPanel
              Left = 420
              Top = 478
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #32080#26696#26085#26399':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 22
            end
            object pnPrecautionLogStartTime: TPanel
              Left = 140
              Top = 438
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 15
            end
            object pnPrecautionLogApprovedManager: TPanel
              Left = 140
              Top = 396
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 11
            end
            object pnPrecautionLogWatchmakers: TPanel
              Left = 550
              Top = 398
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 13
            end
            object pnPrecautionLogFinishType: TPanel
              Left = 140
              Top = 478
              Width = 77
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 19
            end
            object pnPromptDay: TPanel
              Left = 222
              Top = 478
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #38928#35336#32080#26696#22825#25976':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 20
            end
            object pnPrecautionLogEndTime: TPanel
              Left = 550
              Top = 478
              Width = 180
              Height = 26
              BevelOuter = bvNone
              Color = 13761532
              Font.Charset = ANSI_CHARSET
              Font.Color = clPurple
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 23
            end
            object Panel8: TPanel
              Left = 10
              Top = 10
              Width = 120
              Height = 30
              BevelOuter = bvNone
              Caption = #25628#23563#27284#26696#65306
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object cobSearchPrecautionLog: TComboBox
              Left = 140
              Top = 10
              Width = 500
              Height = 27
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'Times New Roman'
              Font.Style = [fsBold]
              ItemHeight = 0
              ParentFont = False
              TabOrder = 2
            end
            object FileListBoxPrecautionLog: TFileListBox
              Tag = 4
              Left = 896
              Top = 8
              Width = 41
              Height = 33
              ItemHeight = 13
              Mask = '*.csv'
              TabOrder = 0
              Visible = False
            end
            object pnNoteContents: TPanel
              Left = 10
              Top = 100
              Width = 120
              Height = 26
              BevelOuter = bvNone
              Caption = #27880#24847#20107#38917':'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -16
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 6
            end
            object Panel9: TPanel
              Left = 333
              Top = 59
              Width = 340
              Height = 26
              BevelOuter = bvNone
              Caption = '('#33509#28961#35215#31684#20381#24490#65292#21063#20197'"N/A"'#34920#31034')'
              Color = 6055843
              Font.Charset = ANSI_CHARSET
              Font.Color = clWhite
              Font.Height = -19
              Font.Name = #27161#26999#39636
              Font.Style = []
              ParentFont = False
              TabOrder = 3
            end
          end
        end
      end
    end
    object tsOEE_ProductionInfor: TTabSheet
      Caption = 'Production Information'
      ImageIndex = 7
    end
    object tsLotInfo: TTabSheet
      Caption = 'Lot information'
      ImageIndex = 9
      object labMultiLotCnt: TLabel
        Left = 14
        Top = 457
        Width = 68
        Height = 13
        Caption = 'Muli Lot Count'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label27: TLabel
        Left = 12
        Top = 497
        Width = 50
        Height = 13
        Caption = 'Bundle ID:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label32: TLabel
        Left = 12
        Top = 516
        Width = 47
        Height = 13
        Caption = 'Bundle in:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object Label34: TLabel
        Left = 12
        Top = 536
        Width = 54
        Height = 13
        Caption = 'Bundle out:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labBundleID: TLabel
        Left = 79
        Top = 449
        Width = 3
        Height = 13
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labBundlIn: TLabel
        Left = 79
        Top = 468
        Width = 3
        Height = 13
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labBundOut: TLabel
        Left = 76
        Top = 488
        Width = 3
        Height = 13
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object gbLotinfo: TGroupBox
        Left = 2
        Top = 2
        Width = 405
        Height = 447
        Caption = 'Lot information'
        Color = 12761254
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        object labLotInfo1: TLabel
          Left = 12
          Top = 16
          Width = 61
          Height = 13
          Caption = 'CUSTOMER'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo2: TLabel
          Left = 12
          Top = 43
          Width = 78
          Height = 13
          Caption = 'INNER_LOT_ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo3: TLabel
          Left = 12
          Top = 70
          Width = 73
          Height = 13
          Caption = 'CUST_LOT_ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo4: TLabel
          Left = 12
          Top = 98
          Width = 151
          Height = 13
          Caption = 'CUSTOMER_DEVICE_GROUP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo5: TLabel
          Left = 12
          Top = 125
          Width = 76
          Height = 13
          Caption = 'DEVICE_NAME'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo6: TLabel
          Left = 12
          Top = 152
          Width = 36
          Height = 13
          Caption = 'STAGE'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo7: TLabel
          Left = 12
          Top = 179
          Width = 28
          Height = 13
          Caption = 'STEP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo8: TLabel
          Left = 12
          Top = 206
          Width = 83
          Height = 13
          Caption = 'REPORTCOUNT'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo9: TLabel
          Left = 12
          Top = 234
          Width = 92
          Height = 13
          Caption = 'PROGRAM_NAME'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo10: TLabel
          Left = 12
          Top = 261
          Width = 74
          Height = 13
          Caption = 'TEST_BIN_NO'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo11: TLabel
          Left = 12
          Top = 288
          Width = 60
          Height = 13
          Caption = 'TESTER_ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo12: TLabel
          Left = 12
          Top = 315
          Width = 69
          Height = 13
          Caption = 'HANDLER_ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo13: TLabel
          Left = 12
          Top = 342
          Width = 82
          Height = 13
          Caption = 'TEMPERATURE'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo14: TLabel
          Left = 12
          Top = 370
          Width = 59
          Height = 13
          Caption = 'CURR_QTY'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLotInfo15: TLabel
          Left = 12
          Top = 397
          Width = 77
          Height = 13
          Caption = 'OPERATOR_ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labBinSet: TLabel
          Left = 12
          Top = 421
          Width = 71
          Height = 13
          Caption = 'BIN_SETTING'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object palCustomer: TPanel
          Left = 171
          Top = 12
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object palInnLotID: TPanel
          Left = 171
          Top = 39
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object palCustLotID: TPanel
          Left = 171
          Top = 67
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object palCustDevGup: TPanel
          Left = 171
          Top = 94
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object palDevName: TPanel
          Left = 171
          Top = 121
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object palStage: TPanel
          Left = 171
          Top = 149
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object palStep: TPanel
          Left = 171
          Top = 176
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object palReportCnt: TPanel
          Left = 171
          Top = 203
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object palProgramName: TPanel
          Left = 171
          Top = 231
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object palTestBin: TPanel
          Left = 171
          Top = 258
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object palTestID: TPanel
          Left = 171
          Top = 286
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object palHandlerID: TPanel
          Left = 171
          Top = 313
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object palTemperature: TPanel
          Left = 171
          Top = 340
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
        object palCurrQty: TPanel
          Left = 171
          Top = 368
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
        object palOPID: TPanel
          Left = 171
          Top = 395
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 14
        end
        object palBinSetting: TPanel
          Left = 171
          Top = 419
          Width = 226
          Height = 20
          Alignment = taLeftJustify
          BevelInner = bvLowered
          Caption = 'NA'
          Color = 9534289
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 15
        end
      end
      object grpLotInfo: TGroupBox
        Left = 411
        Top = 2
        Width = 545
        Height = 683
        Caption = 'Lot Summary'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object memoLotSummary: TMyMemo
          Left = 8
          Top = 16
          Width = 533
          Height = 665
          Color = 14670284
          ScrollBars = ssBoth
          TabOrder = 0
          Path = 'D:\HandlerLog'
        end
      end
      object palMultiLotCnt: TPanel
        Left = 172
        Top = 455
        Width = 226
        Height = 20
        Alignment = taLeftJustify
        BevelInner = bvLowered
        Caption = 'NA'
        Color = 9534289
        Font.Charset = ANSI_CHARSET
        Font.Color = clWhite
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
      end
      object btnLot2: TButton
        Tag = 1
        Left = 82
        Top = 553
        Width = 75
        Height = 25
        Caption = 'LOT 2'
        TabOrder = 4
        OnClick = btnLot1Click
      end
      object btnLot3: TButton
        Tag = 2
        Left = 160
        Top = 553
        Width = 75
        Height = 25
        Caption = 'LOT 3'
        TabOrder = 5
        OnClick = btnLot1Click
      end
      object btnLot4: TButton
        Tag = 3
        Left = 238
        Top = 553
        Width = 75
        Height = 25
        Caption = 'LOT 4'
        TabOrder = 6
        OnClick = btnLot1Click
      end
      object btnLot5: TButton
        Tag = 4
        Left = 316
        Top = 553
        Width = 75
        Height = 25
        Caption = 'LOT 5'
        TabOrder = 7
        OnClick = btnLot1Click
      end
      object btnLot1: TButton
        Left = 5
        Top = 553
        Width = 75
        Height = 25
        Caption = 'LOT 1'
        TabOrder = 3
        OnClick = btnLot1Click
      end
    end
  end
  object btExit: TPanel
    Left = 0
    Top = 738
    Width = 932
    Height = 37
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = BtnExitClick
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 817
    Top = 651
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'xls'
    Filter = 'Excel (*.xls)|*.xls'
    InitialDir = 'D:\'
    Left = 874
    Top = 651
  end
  object SavePictureDialog1: TSavePictureDialog
    DefaultExt = 'bmp'
    FileName = 'JamChart'
    Filter = 'Bitmaps (*.bmp)|*.bmp'
    InitialDir = 'D:\'
    Left = 846
    Top = 651
  end
  object OpenDialog1: TOpenDialog
    Left = 788
    Top = 651
  end
end
