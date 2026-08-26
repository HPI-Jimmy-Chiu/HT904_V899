object fTrayMapping: TfTrayMapping
  Left = 121
  Top = 196
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = 'Tray Mapping'
  ClientHeight = 744
  ClientWidth = 975
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShortCut = FormShortCut
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 975
    Height = 700
    ActivePage = tsLoaderRFID
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 6
    TabOrder = 0
    TabWidth = 150
    OnChange = PageControl1Change
    object tsView: TTabSheet
      Caption = 'View'
      object Label1: TLabel
        Left = 640
        Top = 80
        Width = 59
        Height = 20
        Caption = 'Tray ID :'
      end
      object labTrayID: TLabel
        Left = 704
        Top = 80
        Width = 17
        Height = 20
        Caption = 'ID'
      end
      object labTrayIDStatus: TLabel
        Left = 640
        Top = 48
        Width = 102
        Height = 20
        Caption = 'Tray ID Status'
      end
      object labTrayMapStatus: TLabel
        Left = 640
        Top = 16
        Width = 116
        Height = 20
        Caption = 'Tray Map Status'
      end
      object Label4: TLabel
        Left = 641
        Top = 141
        Width = 59
        Height = 20
        Caption = 'Tray ID :'
      end
      object labTrayID2: TLabel
        Left = 705
        Top = 141
        Width = 17
        Height = 20
        Caption = 'ID'
      end
      object labTrayID2Status: TLabel
        Left = 640
        Top = 112
        Width = 102
        Height = 20
        Caption = 'Tray ID Status'
      end
      object GroupBox1: TGroupBox
        Left = 560
        Top = 176
        Width = 329
        Height = 257
        Caption = 'Simulate'
        TabOrder = 1
        object Edit1: TEdit
          Left = 16
          Top = 24
          Width = 281
          Height = 28
          TabOrder = 0
          Text = 'GV,4,1,4_10,0111011010111011010111011010111011011111'
        end
        object Edit2: TEdit
          Left = 16
          Top = 64
          Width = 281
          Height = 28
          TabOrder = 1
          Text = 'GV,3,0,1,UB1415680JLO'
        end
        object Button1: TButton
          Left = 112
          Top = 128
          Width = 75
          Height = 25
          Caption = 'Tray Map '
          TabOrder = 3
          OnClick = Button1Click
        end
        object Button2: TButton
          Left = 208
          Top = 128
          Width = 75
          Height = 25
          Caption = 'Tray ID'
          TabOrder = 4
          OnClick = Button2Click
        end
        object cbBarCodeSimulate: TCheckBox
          Left = 16
          Top = 160
          Width = 177
          Height = 17
          Caption = 'Simulate'
          TabOrder = 5
        end
        object Edit3: TEdit
          Left = 16
          Top = 184
          Width = 305
          Height = 28
          TabOrder = 7
          Text = 'D:\HT9045_Log\TrayMapComm\LXXXXX.XX_EENNNNNNNCUL.txt'
        end
        object Button4: TButton
          Left = 16
          Top = 224
          Width = 75
          Height = 25
          Caption = 'Down'
          TabOrder = 8
          OnClick = Button4Click
        end
        object CheckBox1: TCheckBox
          Left = 208
          Top = 160
          Width = 97
          Height = 17
          Caption = 'Error'
          TabOrder = 6
        end
        object Edit4: TEdit
          Left = 16
          Top = 96
          Width = 281
          Height = 28
          TabOrder = 2
          Text = 'GV,3,1,1,UB1415680JLO'
        end
      end
      object MemoTrayMap: TMemo
        Left = 0
        Top = 524
        Width = 967
        Height = 141
        Align = alBottom
        Color = 14670284
        TabOrder = 2
      end
      object PageControl2: TPageControl
        Left = 0
        Top = 0
        Width = 553
        Height = 524
        ActivePage = tsAutoTrayView3
        Align = alLeft
        TabIndex = 4
        TabOrder = 0
        object tsTrayView: TTabSheet
          Caption = 'Tray View'
          object sgTraymap: TStringGrid
            Left = 0
            Top = 0
            Width = 609
            Height = 489
            Align = alLeft
            Color = 14670284
            ColCount = 8
            DefaultColWidth = 37
            DefaultRowHeight = 20
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
          end
        end
        object tsSuckView: TTabSheet
          Caption = 'Loader View'
          ImageIndex = 1
          object sgSuckmap: TStringGrid
            Left = 0
            Top = 0
            Width = 609
            Height = 489
            Align = alLeft
            Color = 14670284
            ColCount = 8
            DefaultColWidth = 37
            DefaultRowHeight = 20
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
          end
        end
        object tsAutoTrayView1: TTabSheet
          Caption = 'Auto1 View'
          ImageIndex = 2
          object sgTrayDeviceCnt1: TStringGrid
            Left = 0
            Top = 0
            Width = 609
            Height = 489
            Align = alLeft
            Color = 14670284
            ColCount = 8
            DefaultColWidth = 37
            DefaultRowHeight = 20
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
          end
        end
        object tsAutoTrayView2: TTabSheet
          Caption = 'Auto2 View'
          ImageIndex = 3
          object sgTrayDeviceCnt2: TStringGrid
            Left = 0
            Top = 0
            Width = 609
            Height = 489
            Align = alLeft
            Color = 14670284
            ColCount = 8
            DefaultColWidth = 37
            DefaultRowHeight = 20
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
          end
        end
        object tsAutoTrayView3: TTabSheet
          Caption = 'Auto3 View'
          ImageIndex = 4
          object sgTrayDeviceCnt3: TStringGrid
            Left = 0
            Top = 0
            Width = 609
            Height = 489
            Align = alLeft
            Color = 14670284
            ColCount = 8
            DefaultColWidth = 37
            DefaultRowHeight = 20
            FixedColor = 9534289
            RowCount = 15
            TabOrder = 0
          end
        end
      end
    end
    object tsTraySetting: TTabSheet
      Caption = 'Tray ID/Map Setting'
      ImageIndex = 1
      object labSFCGetResultTimeOut: TLabel
        Left = 9
        Top = 193
        Width = 174
        Height = 20
        Caption = 'Get result time out (ms): '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labSFCStartDelay: TLabel
        Left = 9
        Top = 127
        Width = 153
        Height = 20
        Caption = 'Start delay time (ms): '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labSFCAutoRetry: TLabel
        Left = 9
        Top = 226
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
      object labSFCExposureTimeOut: TLabel
        Left = 9
        Top = 159
        Width = 171
        Height = 20
        Caption = 'Exposure time out (ms) :'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object cbEnableTrayMap: TCheckBox
        Left = 12
        Top = 14
        Width = 273
        Height = 19
        Caption = 'Enable Tray Map function'
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
      object cbEnableTrayID: TCheckBox
        Left = 12
        Top = 46
        Width = 273
        Height = 19
        Caption = 'Enable Tray ID function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 2
      end
      object edTrayGetResultTimeOut: TEdit
        Left = 237
        Top = 188
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        Text = '10'
        OnClick = edTrayStartDelayClick
      end
      object edTrayStartDelay: TEdit
        Left = 237
        Top = 124
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        Text = '10'
        OnClick = edTrayStartDelayClick
      end
      object edTrayAutoRetry: TEdit
        Left = 237
        Top = 220
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        Text = '1'
        OnClick = edTrayAutoRetryClick
      end
      object edTrayExposureTimeOut: TEdit
        Left = 237
        Top = 156
        Width = 81
        Height = 28
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        Text = '10'
        OnClick = edTrayStartDelayClick
      end
      object DirectoryListBox1: TDirectoryListBox
        Left = 440
        Top = 320
        Width = 241
        Height = 241
        Color = 14670284
        FileList = FileListBox1
        ItemHeight = 16
        TabOrder = 14
        Visible = False
      end
      object FileListBox1: TFileListBox
        Left = 688
        Top = 320
        Width = 257
        Height = 241
        Color = 14670284
        ItemHeight = 16
        TabOrder = 15
        Visible = False
      end
      object cbEnableDeviceRemain: TCheckBox
        Left = 364
        Top = 14
        Width = 317
        Height = 19
        Caption = 'Enable Device Remain function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 1
      end
      object cbEnableTrayID2: TCheckBox
        Left = 364
        Top = 46
        Width = 273
        Height = 19
        Caption = 'Enable Tray ID 2 function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 3
      end
      object cbEnableTrayDeviceCnt: TCheckBox
        Left = 12
        Top = 78
        Width = 381
        Height = 19
        Caption = 'Enable Tray Device Count function'
        Checked = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        State = cbChecked
        TabOrder = 4
      end
      object palTrayIDGroup: TPanel
        Left = 0
        Top = 348
        Width = 329
        Height = 189
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 16
        object labCodeMinLen: TLabel
          Left = 9
          Top = 10
          Width = 158
          Height = 20
          Caption = 'Tray code MIN length :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labCodeMaxLen: TLabel
          Left = 9
          Top = 43
          Width = 164
          Height = 20
          Caption = 'Tray code MAX length :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labTrayIDShift: TLabel
          Left = 9
          Top = 74
          Width = 142
          Height = 20
          Caption = 'Tray ID Catch Shift :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labTrayID2Shift: TLabel
          Left = 11
          Top = 104
          Width = 142
          Height = 20
          Caption = 'Tray ID Catch Shift :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edtTraycodeMinLength: TEdit
          Left = 237
          Top = 5
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '5'
          OnClick = edTrayAutoRetryClick
        end
        object edtTraycodeMaxLength: TEdit
          Left = 237
          Top = 37
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '5'
          OnClick = edTrayAutoRetryClick
        end
        object edtTrayIDShift: TEdit
          Left = 237
          Top = 69
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '5'
          OnClick = edtTrayMapShiftClick
        end
        object edtTrayID2Shift: TEdit
          Left = 237
          Top = 100
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Text = '5'
          OnClick = edtTrayMapShiftClick
        end
        object cbEnableOCRTrayID: TCheckBox
          Left = 9
          Top = 160
          Width = 281
          Height = 17
          Caption = 'Enable OCR Tray ID'
          TabOrder = 5
        end
        object cbEnableTrayIDFTP: TCheckBox
          Left = 9
          Top = 136
          Width = 281
          Height = 17
          Caption = 'Enable Tray ID Download Tray Map'
          TabOrder = 4
        end
      end
      object palTrayMapFunGroup: TPanel
        Left = 0
        Top = 533
        Width = 441
        Height = 49
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 17
        object cbDisableMapSuck: TCheckBox
          Left = 9
          Top = 0
          Width = 449
          Height = 17
          Caption = 'Set every tray as "Full Tray" ( In-Arm check every pocket )'
          TabOrder = 0
        end
        object cbEnableSuckMapCheck: TCheckBox
          Left = 9
          Top = 24
          Width = 473
          Height = 17
          Caption = 'Auto skip pocket ( If map data show No IC in pocket )'
          TabOrder = 1
        end
      end
      object palTrayMapGroup: TPanel
        Left = 0
        Top = 248
        Width = 337
        Height = 73
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 12
        object labTrayMapCatch: TLabel
          Left = 9
          Top = 11
          Width = 166
          Height = 20
          Caption = 'Tray Map Catch Count :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labTrayMapStartPos: TLabel
          Left = 9
          Top = 42
          Width = 158
          Height = 20
          Caption = 'Tray Map Catch Start :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edtTrayMapCatch: TEdit
          Left = 237
          Top = 5
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '5'
          OnClick = edtTrayMapCatchClick
        end
        object edtTrayMapStart: TEdit
          Left = 237
          Top = 37
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '5'
          OnClick = edtTrayMapShiftClick
        end
      end
      object palDeviceCountGroup: TPanel
        Left = 0
        Top = 316
        Width = 337
        Height = 35
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 13
        object lblTrayDeciveCntStartPos: TLabel
          Left = 9
          Top = 7
          Width = 222
          Height = 20
          Caption = 'Tray Decive Count Catch Start :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edtTrayDeciveCntStart: TEdit
          Left = 237
          Top = 3
          Width = 81
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Text = '5'
          OnClick = edtTrayDeciveCntStartClick
        end
      end
      object cbEnableTrayID3: TCheckBox
        Left = 364
        Top = 78
        Width = 273
        Height = 19
        Caption = 'Tray ID 3 (Manual) Function'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 5
        Visible = False
      end
      object cbEnableTrayMapLaser: TCheckBox
        Left = 364
        Top = 110
        Width = 273
        Height = 19
        Caption = 'Enable Tray Map Laser'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
        Visible = False
      end
      object cbEnableDeviceRemainLaser: TCheckBox
        Left = 668
        Top = 110
        Width = 301
        Height = 19
        Caption = 'Enable Device Remain Laser'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        Visible = False
      end
      object cbCheckTrayIDByLot: TCheckBox
        Left = 9
        Top = 582
        Width = 240
        Height = 20
        Caption = 'Check duplicate Tray ID by lot'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
      end
    end
    object tsTrayCCD: TTabSheet
      Caption = 'CCD Setting'
      ImageIndex = 2
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 373
        Height = 665
        Align = alLeft
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object GroupBox2: TGroupBox
          Left = 0
          Top = 0
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label5: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label6: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTray_ID_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_ID_Connect: TBitBtn
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object BtTray_ID_Disconnect: TBitBtn
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btTray_ID_Trigger: TButton
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTray_ID_CMD: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox3: TGroupBox
          Left = 0
          Top = 110
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray Map'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label7: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label8: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_Map_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTray_Map_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_Map_Connect: TBitBtn
            Tag = 1
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object BtTray_Map_Disconnect: TBitBtn
            Tag = 1
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btTray_Map_Trigger: TButton
            Tag = 1
            Left = 208
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTray_Map_CMD: TEdit
            Left = 207
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox4: TGroupBox
          Left = 0
          Top = 220
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray ID 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object Label2: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label3: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID2_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTray_ID2_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_ID2_Connect: TBitBtn
            Tag = 3
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object BtTray_ID2_Disconnect: TBitBtn
            Tag = 3
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btTray_ID2_Trigger: TButton
            Tag = 3
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTray_ID2_CMD: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox6: TGroupBox
          Left = 0
          Top = 330
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray Device Count1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label11: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label12: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTrayDeviceCnt1_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTrayDeviceCnt1_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object btnTrayDeviceCnt1_Connect: TBitBtn
            Tag = 4
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object btnTrayDeviceCnt1_Disconnect: TBitBtn
            Tag = 4
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btnTrayDeviceCnt1_Trigger: TButton
            Tag = 4
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTrayDeviceCnt1_CMD: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox7: TGroupBox
          Left = 0
          Top = 440
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray Device Count2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          object Label13: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label14: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTrayDeviceCnt2_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTrayDeviceCnt2_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object btnTrayDeviceCnt2_Connect: TBitBtn
            Tag = 5
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object btnTrayDeviceCnt2_Disconnect: TBitBtn
            Tag = 5
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btnTrayDeviceCnt2_Trigger: TButton
            Tag = 5
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTrayDeviceCnt2_CMD: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox5: TGroupBox
          Left = 0
          Top = 550
          Width = 373
          Height = 110
          Align = alTop
          Caption = 'Tray Device Count3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          object Label9: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label10: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTrayDeviceCnt3_Address: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '127.000.000.100'
          end
          object edTrayDeviceCnt3_Port: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '1000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object btnTrayDeviceCnt3_Connect: TBitBtn
            Tag = 6
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectClick
          end
          object btnTrayDeviceCnt3_Disconnect: TBitBtn
            Tag = 6
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectClick
          end
          object btnTrayDeviceCnt3_Trigger: TButton
            Tag = 6
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerClick
          end
          object edTrayDeviceCnt3_CMD: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
      end
      object memoTrayCCDCommLog: TMemo
        Left = 373
        Top = 0
        Width = 594
        Height = 665
        Align = alClient
        Color = 14670284
        ScrollBars = ssBoth
        TabOrder = 1
      end
    end
    object TabSheet1: TTabSheet
      Caption = ' Keyence Setting'
      ImageIndex = 3
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 373
        Height = 665
        Align = alLeft
        Color = 12761254
        TabOrder = 0
        object GroupBox8: TGroupBox
          Left = 1
          Top = 111
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Tray ID (Empty)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label18: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label19: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID2_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.202'
          end
          object edTray_ID2_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '9004'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_ID2_ConnectKY: TBitBtn
            Tag = 1
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BtTray_ID2_DisconnectKY: TBitBtn
            Tag = 1
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object btTray_ID2_TriggerKY: TButton
            Tag = 1
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edTray_ID2_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox9: TGroupBox
          Left = 1
          Top = 1
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Tray ID (Loader)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label20: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label21: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.201'
          end
          object edTray_ID_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '9004'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_ID_ConnectKY: TBitBtn
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BtTray_ID_DisconnectKY: TBitBtn
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object btTray_ID_TriggerKY: TButton
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edTray_ID_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox10: TGroupBox
          Left = 1
          Top = 331
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Tray ID (Auto 3)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label22: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label23: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID4_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.202'
          end
          object edTray_ID4_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '9004'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BtTray_ID3_ConnectKY: TBitBtn
            Tag = 3
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BtTray_ID3_DisconnectKY: TBitBtn
            Tag = 3
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object btTray_ID4_TriggerKY: TButton
            Tag = 3
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edTray_ID4_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox11: TGroupBox
          Left = 1
          Top = 221
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Tray ID (Multile Empty)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object Label24: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label25: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edTray_ID3_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.203'
          end
          object edTray_ID3_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '9004'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn1: TBitBtn
            Tag = 2
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn2: TBitBtn
            Tag = 2
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object btTray_ID3_TriggerKY: TButton
            Tag = 2
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edTray_ID3_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
      end
      object memoKeyenceCommLog: TMemo
        Left = 373
        Top = 0
        Width = 594
        Height = 665
        Align = alClient
        Color = 14670284
        ScrollBars = ssBoth
        TabOrder = 1
      end
    end
    object tsLoaderDeviceCheck: TTabSheet
      Caption = 'Laser(Loader)'
      ImageIndex = 4
      object GroupBox12: TGroupBox
        Left = 8
        Top = 8
        Width = 233
        Height = 649
        Caption = 'Setting'
        TabOrder = 0
        object Label15: TLabel
          Left = 9
          Top = 70
          Width = 59
          Height = 18
          Caption = 'Address'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 143
          Top = 70
          Width = 29
          Height = 18
          Caption = 'Port'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label17: TLabel
          Left = 81
          Top = 622
          Width = 70
          Height = 20
          Caption = 'LDCTask:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          OnMouseDown = Label18MouseDown
        end
        object Label26: TLabel
          Left = 9
          Top = 520
          Width = 110
          Height = 20
          Caption = 'Loop(Max. 100)'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label27: TLabel
          Left = 9
          Top = 30
          Width = 44
          Height = 18
          Caption = 'Mode:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label28: TLabel
          Left = 9
          Top = 554
          Width = 110
          Height = 20
          Caption = 'Wait AOI Times'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label29: TLabel
          Left = 9
          Top = 586
          Width = 191
          Height = 20
          Caption = 'Load CW Speed               %'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbTaskLDC: TLabel
          Left = 161
          Top = 621
          Width = 48
          Height = 20
          Caption = '            '
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edLoaderDC_Address: TEdit
          Left = 7
          Top = 94
          Width = 130
          Height = 26
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = '127.000.000.001'
        end
        object edLoaderDC_Port: TEdit
          Left = 142
          Top = 94
          Width = 60
          Height = 26
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = '5102'
        end
        object BtLoaderDC_Connect: TBitBtn
          Left = 6
          Top = 128
          Width = 120
          Height = 30
          Caption = 'Connect'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          OnClick = BtLoaderDC_ConnectClick
        end
        object edLoopTimes: TEdit
          Left = 128
          Top = 520
          Width = 49
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Text = '1'
          OnKeyPress = edLoopTimesKeyPress
        end
        object cbLaserScanMode: TComboBox
          Left = 64
          Top = 24
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 0
          OnChange = cbLaserScanModeChange
        end
        object edWaitAOITimes: TEdit
          Left = 128
          Top = 551
          Width = 49
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          Text = '1'
          OnKeyPress = edLoopTimesKeyPress
        end
        object edLoaderCWSpeed: TEdit
          Left = 128
          Top = 582
          Width = 49
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          Text = '33'
          OnKeyPress = edLoopTimesKeyPress
        end
        object Panel4: TPanel
          Left = 0
          Top = 160
          Width = 233
          Height = 337
          Color = 12761254
          TabOrder = 4
          object Panel5: TPanel
            Left = 1
            Top = 124
            Width = 231
            Height = 41
            Align = alTop
            Color = 12761254
            TabOrder = 3
            object cbAutoLearning: TCheckBox
              Left = 8
              Top = 10
              Width = 185
              Height = 23
              Caption = 'Auto Learning'
              TabOrder = 0
            end
          end
          object Panel6: TPanel
            Left = 1
            Top = 295
            Width = 231
            Height = 41
            Align = alBottom
            Color = 12761254
            TabOrder = 5
            object btnSimulateRemainIC: TButton
              Left = 23
              Top = 8
              Width = 186
              Height = 29
              Caption = 'Run'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'Arial'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
              OnClick = btnSimulateRemainICClick
            end
          end
          object Panel8: TPanel
            Left = 1
            Top = 254
            Width = 231
            Height = 41
            Align = alBottom
            Color = 12761254
            TabOrder = 4
          end
          object Panel7: TPanel
            Left = 1
            Top = 1
            Width = 231
            Height = 41
            Align = alTop
            Color = 12761254
            TabOrder = 0
            object cbEnableRetryCount: TCheckBox
              Left = 8
              Top = 10
              Width = 113
              Height = 23
              Caption = 'Retry Count'
              TabOrder = 1
            end
            object edRemainIC_Laser_Retry: TEdit
              Left = 120
              Top = 8
              Width = 65
              Height = 28
              TabOrder = 0
            end
          end
          object Panel9: TPanel
            Left = 1
            Top = 42
            Width = 231
            Height = 41
            Align = alTop
            Color = 12761254
            TabOrder = 1
            object cbAutoSkip: TCheckBox
              Left = 8
              Top = 10
              Width = 185
              Height = 23
              Caption = 'Auto Skip'
              TabOrder = 0
            end
          end
          object Panel10: TPanel
            Left = 1
            Top = 83
            Width = 231
            Height = 41
            Align = alTop
            Color = 12761254
            TabOrder = 2
            object cbRunAtInitialStart: TCheckBox
              Left = 8
              Top = 10
              Width = 185
              Height = 23
              Caption = 'Run When Initial Start'
              TabOrder = 0
            end
          end
        end
      end
      object mLoaderDCCommLog: TMemo
        Left = 248
        Top = 504
        Width = 705
        Height = 153
        Color = 14670284
        ScrollBars = ssVertical
        TabOrder = 2
      end
      object plTrayMap: TPanel
        Left = 248
        Top = 16
        Width = 705
        Height = 481
        Color = 12761254
        TabOrder = 1
        object PageControl3: TPageControl
          Left = 8
          Top = 8
          Width = 689
          Height = 473
          ActivePage = tsCommand
          TabIndex = 0
          TabOrder = 0
          object tsCommand: TTabSheet
            Caption = 'Command'
            object Panel11: TPanel
              Left = 0
              Top = 0
              Width = 217
              Height = 433
              Color = 12761254
              TabOrder = 0
              object edLoaderDC_CMD: TEdit
                Left = 7
                Top = 4
                Width = 98
                Height = 26
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
              end
              object btLoaderDC_Trigger: TButton
                Left = 111
                Top = 4
                Width = 90
                Height = 29
                Caption = 'Send CMD'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnClick = btLoaderDC_TriggerClick
              end
              object btnLoadFile: TButton
                Left = 111
                Top = 36
                Width = 90
                Height = 29
                Caption = 'LoadFile'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                OnClick = btnLoadFileClick
              end
              object btnClearBuffer: TButton
                Left = 7
                Top = 36
                Width = 90
                Height = 29
                Caption = 'ClearBuffer'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                OnClick = btnClearBufferClick
              end
              object btnGrab: TButton
                Left = 7
                Top = 68
                Width = 90
                Height = 29
                Caption = 'Grab'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
                OnClick = btnGrabClick
              end
              object btnGetValue: TButton
                Left = 111
                Top = 68
                Width = 90
                Height = 29
                Caption = 'GetValue'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 5
                OnClick = btnGetValueClick
              end
              object btnSetLot: TButton
                Left = 7
                Top = 102
                Width = 90
                Height = 29
                Caption = 'SetLot'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 6
                OnClick = btnSetLotClick
              end
              object btnCreateFile: TButton
                Left = 111
                Top = 102
                Width = 90
                Height = 29
                Caption = 'CreateFile'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 7
                OnClick = btnCreateFileClick
              end
              object btnGoldenImage: TButton
                Left = 7
                Top = 134
                Width = 194
                Height = 29
                Caption = 'Set GoldenImg'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 8
                OnClick = btnGoldenImageClick
              end
              object cbPassAOI: TCheckBox
                Left = 8
                Top = 170
                Width = 97
                Height = 17
                Caption = 'PassAOI'
                TabOrder = 9
              end
              object cbPassActive: TCheckBox
                Left = 104
                Top = 170
                Width = 97
                Height = 17
                Caption = 'PassActive'
                TabOrder = 10
              end
              object cbSimulationAOICommand: TCheckBox
                Left = 8
                Top = 202
                Width = 209
                Height = 17
                Caption = 'Simulation AOI Command'
                TabOrder = 11
                OnClick = cbSimulationAOICommandClick
              end
            end
            object GroupBox13: TGroupBox
              Left = 224
              Top = 0
              Width = 321
              Height = 209
              Caption = 'Active'
              TabOrder = 1
              object Label30: TLabel
                Left = 8
                Top = 24
                Width = 50
                Height = 20
                Caption = 'Loader'
              end
              object btnMoveIn: TButton
                Left = 159
                Top = 22
                Width = 90
                Height = 29
                Caption = 'Move In'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                OnClick = btnMoveInClick
              end
              object btnMoveOut: TButton
                Left = 63
                Top = 22
                Width = 90
                Height = 29
                Caption = 'Move Out'
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'Arial'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                OnClick = btnMoveOutClick
              end
            end
          end
          object TabSheet4: TTabSheet
            Caption = 'Function'
            ImageIndex = 4
          end
          object TabSheet2: TTabSheet
            Caption = 'Tray Map Result'
            ImageIndex = 1
            object lbTrayMapName: TLabel
              Left = 8
              Top = 8
              Width = 65
              Height = 20
              Caption = 'Tray Map'
            end
            object mtTrayMap: TTMyTray
              Left = 80
              Top = 5
              Width = 401
              Height = 428
              XItem = 1
              YItem = 1
            end
          end
          object TabSheet3: TTabSheet
            Caption = 'Check Device Result'
            ImageIndex = 2
            object Label31: TLabel
              Left = 8
              Top = 24
              Width = 50
              Height = 20
              Caption = 'Result:'
            end
            object lbResult: TLabel
              Left = 72
              Top = 8
              Width = 104
              Height = 48
              Caption = 'None'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -40
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              ParentFont = False
            end
          end
          object TabSheet5: TTabSheet
            Caption = 'Yield'
            ImageIndex = 3
            object btnAddRandom: TButton
              Left = 544
              Top = 8
              Width = 123
              Height = 33
              Caption = 'AddRandom'
              TabOrder = 1
              Visible = False
              OnClick = btnAddRandomClick
            end
            object btnRefreshYield: TButton
              Left = 544
              Top = 40
              Width = 123
              Height = 57
              Caption = 'Query'
              TabOrder = 2
              OnClick = btnRefreshYieldClick
            end
            object sgTrayDeviceYield: TStringGrid
              Left = 0
              Top = 0
              Width = 529
              Height = 438
              Align = alLeft
              Color = 14670284
              ColCount = 4
              DefaultColWidth = 37
              DefaultRowHeight = 20
              FixedColor = 9534289
              RowCount = 15
              Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
              TabOrder = 0
              ColWidths = (
                37
                139
                186
                243)
            end
          end
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'CoverTrayID'
      ImageIndex = 5
      object Panel12: TPanel
        Left = 0
        Top = 0
        Width = 373
        Height = 665
        Align = alLeft
        Caption = 'Panel12'
        Color = 12761254
        TabOrder = 0
        object GroupBox17: TGroupBox
          Left = 1
          Top = 441
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'AUTO3 Car'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          object Label38: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label39: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edCoverTray_ID5_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.155'
          end
          object edCoverTray_ID5_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '5000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn9: TBitBtn
            Tag = 8
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn10: TBitBtn
            Tag = 8
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object Button7: TButton
            Tag = 8
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edCoverTray_ID_AUTO3Car_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox16: TGroupBox
          Left = 1
          Top = 221
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'AUTO1 Car'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          object Label36: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label37: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edCoverTray_ID3_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.153'
          end
          object edCoverTray_ID3_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '5000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn7: TBitBtn
            Tag = 6
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn8: TBitBtn
            Tag = 6
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object Button6: TButton
            Tag = 6
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edCoverTray_ID_AUTO1Car_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox14: TGroupBox
          Left = 1
          Top = 1
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Loader Car'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label32: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label33: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edCoverTray_ID_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.151'
          end
          object edCoverTray_ID_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '5000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn3: TBitBtn
            Tag = 4
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn4: TBitBtn
            Tag = 4
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object Button3: TButton
            Tag = 4
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edCoverTray_ID_LoaderCar_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox15: TGroupBox
          Left = 1
          Top = 111
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'Loader tray'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label34: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label35: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edCoverTray_ID2_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.152'
          end
          object edCoverTray_ID2_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '5000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn5: TBitBtn
            Tag = 5
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn6: TBitBtn
            Tag = 5
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object Button5: TButton
            Tag = 5
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edCoverTray_ID_Loader_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object GroupBox18: TGroupBox
          Left = 1
          Top = 331
          Width = 371
          Height = 110
          Align = alTop
          Caption = 'AUTO2 Car'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          object Label40: TLabel
            Left = 9
            Top = 22
            Width = 59
            Height = 18
            Caption = 'Address'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object Label41: TLabel
            Left = 143
            Top = 22
            Width = 29
            Height = 18
            Caption = 'Port'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
          end
          object edCoverTray_ID4_AddressKY: TEdit
            Left = 7
            Top = 46
            Width = 130
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '172.16.8.154'
          end
          object edCoverTray_ID4_PortKY: TEdit
            Left = 142
            Top = 46
            Width = 60
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '5000'
            OnMouseDown = edTray_ID_PortMouseDown
          end
          object BitBtn11: TBitBtn
            Tag = 7
            Left = 6
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Connect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 4
            OnClick = BtTray_ID_ConnectKYClick
          end
          object BitBtn12: TBitBtn
            Tag = 7
            Left = 130
            Top = 76
            Width = 120
            Height = 30
            Caption = 'Disconnect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4210816
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnClick = BtTray_ID_DisconnectKYClick
          end
          object Button8: TButton
            Tag = 7
            Left = 212
            Top = 16
            Width = 90
            Height = 29
            Caption = 'Send CMD'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btTray_ID_TriggerKYClick
          end
          object edCoverTray_ID_AUTO2Car_CMDKY: TEdit
            Left = 211
            Top = 46
            Width = 90
            Height = 26
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
      end
      object MemoKeyenceLogCover: TMemo
        Left = 373
        Top = 0
        Width = 594
        Height = 665
        Align = alClient
        Color = 14670284
        ScrollBars = ssBoth
        TabOrder = 1
      end
    end
    object tsLoaderRFID: TTabSheet
      Caption = 'RFID(Loader)'
      ImageIndex = 6
      object grpRFIDCommSetting: TGroupBox
        Left = 0
        Top = 0
        Width = 967
        Height = 305
        Align = alTop
        Caption = 'Com Port Setting'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object btnLdRFIDConn: TButton
          Left = 11
          Top = 50
          Width = 82
          Height = 25
          Caption = 'Open'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = btnLdRFIDConnClick
        end
        object btnLdRFIDDisConn: TButton
          Left = 11
          Top = 82
          Width = 82
          Height = 25
          Caption = 'Close'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = btnLdRFIDDisConnClick
        end
        object cbbLdRFIDComPort: TComboBox
          Left = 11
          Top = 22
          Width = 197
          Height = 24
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 16
          ParentFont = False
          TabOrder = 2
          Text = 'COM19'
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
            'COM51'
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20'
            'COM21'
            'COM22'
            'COM23'
            'COM24')
        end
        object pnlLdRFIDState: TPanel
          Left = 99
          Top = 48
          Width = 82
          Height = 24
          BevelOuter = bvNone
          BiDiMode = bdLeftToRight
          Caption = 'Disconnect'
          Color = 12761254
          Ctl3D = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentBiDiMode = False
          ParentCtl3D = False
          ParentFont = False
          TabOrder = 3
        end
        object grpNFC_CH1: TGroupBox
          Left = 216
          Top = 24
          Width = 273
          Height = 273
          Caption = 'CH1'
          TabOrder = 4
          object lblNFC_CH1_UID: TLabel
            Left = 136
            Top = 64
            Width = 126
            Height = 20
            Caption = 'lblNFC_CH1_UID'
          end
          object lblNFC_CH1_CID: TLabel
            Left = 136
            Top = 96
            Width = 125
            Height = 20
            Caption = 'lblNFC_CH1_CID'
          end
          object btnLdRFIDClear1: TButton
            Left = 7
            Top = 24
            Width = 110
            Height = 23
            Caption = 'Clear'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btnLdRFIDClear1Click
          end
          object btnReadMemData1: TButton
            Left = 7
            Top = 162
            Width = 110
            Height = 25
            Caption = 'Read Mem Data'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btnReadMemData1Click
          end
          object btnReadUID1: TButton
            Left = 7
            Top = 58
            Width = 110
            Height = 25
            Caption = 'Read UID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btnReadUID1Click
          end
          object btnReadID1: TButton
            Left = 7
            Top = 198
            Width = 110
            Height = 25
            Caption = 'Read ID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = btnReadID1Click
          end
          object btnLdRFIDReadBlock1: TButton
            Left = 7
            Top = 94
            Width = 110
            Height = 25
            Caption = 'Read Block'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = btnLdRFIDReadBlock1Click
          end
        end
        object grpNFC_CH2: TGroupBox
          Left = 492
          Top = 24
          Width = 273
          Height = 273
          Caption = 'CH2'
          TabOrder = 5
          object lblNFC_CH2_UID: TLabel
            Left = 136
            Top = 64
            Width = 126
            Height = 20
            Caption = 'lblNFC_CH2_UID'
          end
          object lblNFC_CH2_CID: TLabel
            Left = 136
            Top = 96
            Width = 125
            Height = 20
            Caption = 'lblNFC_CH2_CID'
          end
          object btnLdRFIDClear2: TButton
            Left = 7
            Top = 22
            Width = 110
            Height = 25
            Caption = 'Clear'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btnLdRFIDClear2Click
          end
          object btnReadMemData2: TButton
            Left = 7
            Top = 162
            Width = 110
            Height = 25
            Caption = 'Read Mem Data'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btnReadMemData2Click
          end
          object btnReadUID2: TButton
            Left = 7
            Top = 58
            Width = 110
            Height = 25
            Caption = 'Read UID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btnReadUID2Click
          end
          object btnReadID2: TButton
            Left = 7
            Top = 198
            Width = 110
            Height = 25
            Caption = 'Read ID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            OnClick = btnReadID2Click
          end
          object btnLdRFIDReadBlock2: TButton
            Left = 7
            Top = 94
            Width = 110
            Height = 25
            Caption = 'Read Block'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            OnClick = btnLdRFIDReadBlock2Click
          end
        end
      end
      object memoLdRFID: TMemo
        Left = 0
        Top = 432
        Width = 967
        Height = 233
        Align = alBottom
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        Lines.Strings = (
          'MemoStrLog')
        ParentFont = False
        TabOrder = 1
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 700
    Width = 975
    Height = 44
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object spbSave: TSpeedButton
      Left = 206
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
      Left = 556
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
  object ClientSocket_Tray_ID: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 776
    Top = 68
  end
  object ClientSocket_Tray_Map: TClientSocket
    Tag = 1
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 804
    Top = 68
  end
  object TimerDownCCDTrayConnect: TTimer
    Enabled = False
    OnTimer = TimerDownCCDTrayConnectTimer
    Left = 936
    Top = 36
  end
  object TimerCCDTrayInitial: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerCCDTrayInitialTimer
    Left = 848
    Top = 36
  end
  object TimerProcessTrayData: TTimer
    Enabled = False
    Interval = 1
    OnTimer = TimerProcessTrayDataTimer
    Left = 820
    Top = 36
  end
  object ClientSocket_Tray_ID2: TClientSocket
    Tag = 3
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 832
    Top = 68
  end
  object ClientSocket_Tray_DeviceCnt3: TClientSocket
    Tag = 6
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 944
    Top = 68
  end
  object ClientSocket_Tray_DeviceCnt2: TClientSocket
    Tag = 5
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 912
    Top = 68
  end
  object ClientSocket_Tray_DeviceCnt1: TClientSocket
    Tag = 4
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Tray_IDConnect
    OnDisconnect = ClientSocket_Tray_IDDisconnect
    OnRead = ClientSocket_Tray_IDRead
    OnError = ClientSocket_Tray_IDError
    Left = 880
    Top = 68
  end
  object ClientSocket_Keyence1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 733
    Top = 264
  end
  object ClientSocket_Keyence2: TClientSocket
    Tag = 1
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 765
    Top = 264
  end
  object ClientSocket_Keyence3: TClientSocket
    Tag = 2
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 805
    Top = 264
  end
  object ClientSocket_Keyence4: TClientSocket
    Tag = 3
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 845
    Top = 264
  end
  object ClientSocket_CoverTray_ID_LoaderCar: TClientSocket
    Tag = 4
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 728
    Top = 300
  end
  object ClientSocket_CoverTray_ID_Loader: TClientSocket
    Tag = 5
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 768
    Top = 300
  end
  object ClientSocket_CoverTray_ID_AUTO1Car: TClientSocket
    Tag = 6
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 800
    Top = 300
  end
  object ClientSocket_CoverTray_ID_AUTO2Car: TClientSocket
    Tag = 7
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 832
    Top = 300
  end
  object ClientSocket_CoverTray_ID_AUTO3Car: TClientSocket
    Tag = 8
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_Keyence1Connect
    OnDisconnect = ClientSocket_Keyence1Disconnect
    OnRead = ClientSocket_Keyence1Read
    OnError = ClientSocket_Keyence1Error
    Left = 864
    Top = 300
  end
  object commRFID: TComm
    CommName = 'COM3'
    BaudRate = 115200
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = False
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
    OnReceiveData = commRFIDReceiveData
    Left = 880
    Top = 36
  end
  object tmrNFC: TTimer
    Enabled = False
    Interval = 100
    OnTimer = tmrNFCTimer
    Left = 792
    Top = 36
  end
end
