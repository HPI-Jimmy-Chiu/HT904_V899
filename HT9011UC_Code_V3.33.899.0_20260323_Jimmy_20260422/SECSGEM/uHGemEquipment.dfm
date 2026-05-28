object HGem: THGem
  Left = 653
  Top = 263
  Width = 627
  Height = 595
  Caption = 'HGem'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = [fsBold]
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object FileListBox1: TFileListBox
    Left = 88
    Top = 192
    Width = 57
    Height = 41
    ItemHeight = 16
    Mask = 'spool\\*.dat'
    TabOrder = 1
  end
  object FileListBox2: TFileListBox
    Left = 96
    Top = 240
    Width = 57
    Height = 33
    ItemHeight = 16
    Mask = 'd:\*.*'
    TabOrder = 3
  end
  object CheckListBox1: TCheckListBox
    Left = 152
    Top = 192
    Width = 49
    Height = 41
    ItemHeight = 16
    TabOrder = 2
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 611
    Height = 557
    ActivePage = TabSheet1
    Align = alClient
    TabIndex = 0
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Main'
      object palSheet0: TPanel
        Left = 0
        Top = 0
        Width = 603
        Height = 526
        Align = alClient
        TabOrder = 0
        object Label8: TLabel
          Left = 16
          Top = 16
          Width = 106
          Height = 16
          Caption = 'Communication'
        end
        object ComboBox1: TComboBox
          Left = 176
          Top = 12
          Width = 145
          Height = 24
          ItemHeight = 16
          TabOrder = 0
          Text = 'HSMS'
          Items.Strings = (
            'HSMS'
            'SECS-1')
        end
        object OnLineOrOffLine: TRadioGroup
          Left = 8
          Top = 109
          Width = 169
          Height = 38
          Caption = 'DefaultControlState<EC:8>'
          Columns = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ItemIndex = 0
          Items.Strings = (
            'On Line'
            'Off Line')
          ParentFont = False
          TabOrder = 1
        end
        object RemoteOrLocal: TRadioGroup
          Left = 8
          Top = 176
          Width = 169
          Height = 46
          Caption = 'DefaultControlState<EC:8>'
          Columns = 2
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ItemIndex = 0
          Items.Strings = (
            'Remote'
            'Local')
          ParentFont = False
          TabOrder = 2
        end
        object DirectoryListBox1: TDirectoryListBox
          Left = 8
          Top = 232
          Width = 377
          Height = 149
          ItemHeight = 16
          TabOrder = 3
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'HSMS'
      ImageIndex = 1
      object palSheet1: TPanel
        Left = 0
        Top = 0
        Width = 603
        Height = 526
        Align = alClient
        TabOrder = 0
        object lblAddress: TLabel
          Left = 13
          Top = 92
          Width = 59
          Height = 16
          Caption = 'Address'
        end
        object lblPort: TLabel
          Left = 13
          Top = 124
          Width = 29
          Height = 16
          Caption = 'Port'
        end
        object lblT3TimeOut: TLabel
          Left = 13
          Top = 186
          Width = 176
          Height = 16
          Caption = 'T3                                 sec'
        end
        object lblT5TimeOut: TLabel
          Left = 13
          Top = 218
          Width = 176
          Height = 16
          Caption = 'T5                                 sec'
        end
        object lblT6TimeOut: TLabel
          Left = 13
          Top = 250
          Width = 176
          Height = 16
          Caption = 'T6                                 sec'
        end
        object lblT7TimeOut: TLabel
          Left = 13
          Top = 282
          Width = 176
          Height = 16
          Caption = 'T7                                 sec'
        end
        object lblT8TimeOut: TLabel
          Left = 13
          Top = 314
          Width = 176
          Height = 16
          Caption = 'T8                                 sec'
        end
        object lblDeviceID: TLabel
          Left = 12
          Top = 155
          Width = 69
          Height = 16
          Caption = 'Device ID'
        end
        object rgRole: TRadioGroup
          Left = 8
          Top = 14
          Width = 361
          Height = 65
          Hint = 'Need restart program after modify'
          Caption = '    Role     '
          Columns = 2
          Items.Strings = (
            '[1] Passive'
            '[2] Active')
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
        end
        object edtIP: TEdit
          Left = 112
          Top = 88
          Width = 153
          Height = 24
          TabOrder = 1
          Text = '128.128.128.128'
        end
        object edtPort: TEdit
          Left = 112
          Top = 120
          Width = 49
          Height = 24
          TabOrder = 2
          Text = '5001'
          OnClick = edtPortClick
        end
        object edtT3TimeOut: TEdit
          Left = 112
          Top = 182
          Width = 49
          Height = 24
          TabOrder = 4
          Text = '30'
          OnClick = edtT3TimeOutClick
        end
        object edtT5TimeOut: TEdit
          Left = 112
          Top = 214
          Width = 49
          Height = 24
          TabOrder = 5
          Text = '30'
        end
        object edtT6TimeOut: TEdit
          Left = 112
          Top = 246
          Width = 49
          Height = 24
          TabOrder = 6
          Text = '30'
        end
        object edtT7TimeOut: TEdit
          Left = 112
          Top = 278
          Width = 49
          Height = 24
          TabOrder = 7
          Text = '30'
        end
        object edtT8TimeOut: TEdit
          Left = 112
          Top = 310
          Width = 49
          Height = 24
          TabOrder = 8
          Text = '30'
        end
        object edDeviceID: TEdit
          Left = 111
          Top = 151
          Width = 49
          Height = 24
          TabOrder = 3
          Text = '0'
          OnClick = edtPortClick
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Normal'
      ImageIndex = 2
      object palSheet2: TPanel
        Left = 0
        Top = 0
        Width = 603
        Height = 525
        Align = alClient
        TabOrder = 0
        object GemCheckBoxAcceptHostOnlineRequest: TCheckBox
          Left = 8
          Top = 15
          Width = 201
          Height = 17
          Caption = 'Accept OnLine Req'
          TabOrder = 0
        end
        object GemCheckBoxShowBinary: TCheckBox
          Left = 8
          Top = 47
          Width = 201
          Height = 17
          Caption = 'Show Binary Data'
          TabOrder = 1
        end
        object GemCheckBoxShowHeadInformation: TCheckBox
          Left = 8
          Top = 79
          Width = 185
          Height = 17
          Caption = 'Show Head Info.'
          TabOrder = 2
        end
        object GemCheckBoxUseExtendedAlarm: TCheckBox
          Left = 8
          Top = 111
          Width = 289
          Height = 17
          Caption = 'Use Extended Alarm '
          TabOrder = 3
        end
        object chkAnnotatedEventReport: TCheckBox
          Left = 8
          Top = 176
          Width = 569
          Height = 17
          Caption = 'Enable Annotated Event Report'
          TabOrder = 4
        end
        object chkMoreMessageAbortProcess: TCheckBox
          Left = 8
          Top = 208
          Width = 569
          Height = 17
          Caption = 'Send Illegore data when has more message '
          TabOrder = 5
        end
        object ckAddDefaultReport: TCheckBox
          Left = 8
          Top = 240
          Width = 393
          Height = 17
          Caption = 'Add Default Report ( change this item need restart )'
          TabOrder = 6
        end
        object cbECChaneEventReport: TCheckBox
          Left = 8
          Top = 144
          Width = 289
          Height = 17
          Caption = 'EC change Event Report'
          TabOrder = 7
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Debug'
      ImageIndex = 3
      object palSheet3: TPanel
        Left = 0
        Top = 0
        Width = 611
        Height = 533
        Align = alClient
        TabOrder = 0
        object SFCodeResponseList: TListBox
          Left = 8
          Top = 23
          Width = 193
          Height = 89
          ItemHeight = 16
          TabOrder = 0
        end
        object strGrdAlarmOld: TStringGrid
          Left = 8
          Top = 120
          Width = 505
          Height = 273
          RowCount = 1
          FixedRows = 0
          TabOrder = 1
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Event'
      ImageIndex = 4
      object palSheet4: TPanel
        Left = 0
        Top = 0
        Width = 603
        Height = 525
        Align = alClient
        TabOrder = 0
        object strGrdCEID: TStringGrid
          Left = 1
          Top = 1
          Width = 601
          Height = 217
          Align = alTop
          ColCount = 258
          RowCount = 1025
          TabOrder = 0
        end
        object stdGridReportID: TStringGrid
          Left = 1
          Top = 218
          Width = 601
          Height = 217
          Align = alTop
          ColCount = 1026
          RowCount = 257
          TabOrder = 1
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'Alarm'
      ImageIndex = 5
      object palSheet5: TPanel
        Left = 0
        Top = 0
        Width = 611
        Height = 533
        Align = alClient
        TabOrder = 0
        object strGrdAlarm: TStringGrid
          Left = 1
          Top = 1
          Width = 609
          Height = 531
          Align = alClient
          ColCount = 12
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
          TabOrder = 0
        end
        object btnExport: TButton
          Left = 512
          Top = 472
          Width = 75
          Height = 25
          Caption = 'Export'
          TabOrder = 1
          OnClick = btnExportClick
        end
      end
    end
    object tsSecsECData: TTabSheet
      Caption = 'EC Data'
      ImageIndex = 7
      object sgSECSECData: TStringGrid
        Left = 0
        Top = 0
        Width = 603
        Height = 525
        Align = alClient
        ColCount = 4
        RowCount = 1
        FixedRows = 0
        TabOrder = 0
      end
    end
    object tsSetup: TTabSheet
      Caption = 'Setup'
      ImageIndex = 6
      object pSetup: TPanel
        Left = 0
        Top = 0
        Width = 611
        Height = 533
        Align = alClient
        TabOrder = 0
        object Label9: TLabel
          Left = 16
          Top = 16
          Width = 67
          Height = 16
          Caption = 'Run Time'
        end
        object Label10: TLabel
          Left = 16
          Top = 48
          Width = 134
          Height = 16
          Caption = 'Average Test Time'
        end
        object Label11: TLabel
          Left = 16
          Top = 80
          Width = 105
          Height = 16
          Caption = 'Yield ( 0.0~1.0)'
        end
        object edtRunTime: TEdit
          Left = 164
          Top = 13
          Width = 76
          Height = 24
          TabOrder = 0
        end
        object edtAverageTestTime: TEdit
          Left = 164
          Top = 45
          Width = 76
          Height = 24
          TabOrder = 1
        end
        object edtOEE_Yield: TEdit
          Left = 164
          Top = 77
          Width = 76
          Height = 24
          TabOrder = 2
        end
      end
    end
    object tsPPBody: TTabSheet
      Caption = 'PPBody'
      ImageIndex = 8
      object memoPPBody: TMemo
        Left = 0
        Top = 197
        Width = 603
        Height = 328
        Align = alClient
        ScrollBars = ssVertical
        TabOrder = 1
      end
      object FileListBox3: TFileListBox
        Left = 0
        Top = 0
        Width = 603
        Height = 197
        Align = alTop
        ItemHeight = 13
        Mask = '*.data'
        TabOrder = 0
      end
    end
  end
  object clientGem: TClientSocket
    Active = False
    Address = '192.168.1.3'
    ClientType = ctBlocking
    Port = 5100
    OnConnecting = clientGemConnecting
    OnConnect = clientGemConnect
    OnDisconnect = clientGemDisconnect
    OnRead = clientGemRead
    OnError = clientGemError
    Left = 544
    Top = 104
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 300
    OnTimer = Timer1Timer
    Left = 544
    Top = 40
  end
  object srvGem: TServerSocket
    Active = False
    Port = 6000
    ServerType = stNonBlocking
    ThreadCacheSize = 1000
    OnClientConnect = srvGemClientConnect
    OnClientDisconnect = srvGemClientDisconnect
    OnClientRead = clientGemRead
    OnClientError = srvGemClientError
    Left = 544
    Top = 72
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'xls'
    Filter = 'Excel (*.xls)|*.xls'
    InitialDir = 'D:\'
    Left = 544
    Top = 136
  end
end
