object ATC_InterfaceForm: TATC_InterfaceForm
  Left = 343
  Top = 121
  Width = 1097
  Height = 847
  Caption = '(OFF-Line)'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 20
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 689
    Height = 791
    Align = alLeft
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object A31_TempOffsetLabel: TLabel
      Left = 16
      Top = 392
      Width = 139
      Height = 20
      Caption = 'Temperature Offset'
    end
    object A31_SetTempLabel: TLabel
      Left = 16
      Top = 352
      Width = 120
      Height = 20
      Caption = 'Set Temperature'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object A31_RunStopsBtn: TSpeedButton
      Left = 192
      Top = 216
      Width = 177
      Height = 49
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'RUN'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = A31_RunStopsBtnClick
    end
    object Label1: TLabel
      Left = 16
      Top = 472
      Width = 225
      Height = 20
      AutoSize = False
      Caption = 'Set Single Temperature Offset'
    end
    object Label2: TLabel
      Left = 16
      Top = 432
      Width = 201
      Height = 20
      AutoSize = False
      Caption = 'Set Single Temperature'
    end
    object ATC_ConnectBtn: TSpeedButton
      Left = 8
      Top = 216
      Width = 177
      Height = 49
      AllowAllUp = True
      GroupIndex = 2
      Caption = 'Connect'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clGreen
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = ATC_ConnectBtnClick
    end
    object ATC_Now_RecipeFile: TLabel
      Left = 8
      Top = 280
      Width = 155
      Height = 20
      Caption = 'ATC_Now_RecipeFile'
    end
    object Label7: TLabel
      Left = 544
      Top = 240
      Width = 48
      Height = 20
      Alignment = taRightJustify
      Caption = 'Label7'
    end
    object sheRecipe: TShape
      Left = 12
      Top = 720
      Width = 30
      Height = 30
      Brush.Color = clScrollBar
      Shape = stCircle
    end
    object labRecipe: TLabel
      Left = 48
      Top = 724
      Width = 161
      Height = 29
      AutoSize = False
      Caption = 'Label1'
    end
    object A31_Temp_OFS: TEdit
      Left = 168
      Top = 388
      Width = 49
      Height = 28
      TabOrder = 15
      Text = '0'
      OnClick = A31_Temp_OFSClick
    end
    object A31_Set_Temp_OFS: TButton
      Left = 224
      Top = 384
      Width = 185
      Height = 33
      Caption = 'Set Temperature Offset'
      TabOrder = 13
      OnClick = A31_Set_Temp_OFSClick
    end
    object A31_Temperature: TEdit
      Left = 168
      Top = 348
      Width = 49
      Height = 28
      TabOrder = 11
      Text = '0'
      OnClick = A31_TemperatureClick
    end
    object A31_Set_Temp: TButton
      Left = 224
      Top = 344
      Width = 185
      Height = 33
      Caption = 'Set Temperature'
      TabOrder = 9
      OnClick = A31_Set_TempClick
    end
    object ATC_Recipe_File: TButton
      Left = 280
      Top = 304
      Width = 129
      Height = 33
      Caption = 'Set Recipe File'
      TabOrder = 6
      OnClick = ATC_Recipe_FileClick
    end
    object ChannelGroupBox: TGroupBox
      Left = 8
      Top = 504
      Width = 673
      Height = 129
      Caption = 'Site Enabled'
      TabOrder = 24
      object ATC_SetChannelEnabled: TButton
        Left = 584
        Top = 24
        Width = 81
        Height = 89
        Caption = 'Set'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -24
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = ATC_SetChannelEnabledClick
      end
    end
    object A31_Read_Temp: TButton
      Left = 512
      Top = 272
      Width = 161
      Height = 33
      Caption = 'Read Temperature'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      OnClick = A31_Read_TempClick
    end
    object A31_Single_Temp_OFS_Site: TComboBox
      Left = 248
      Top = 468
      Width = 57
      Height = 28
      ItemHeight = 20
      TabOrder = 22
      Text = '0'
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
        '15')
    end
    object ATC_Set_Single_Temp_OFS: TButton
      Left = 368
      Top = 464
      Width = 89
      Height = 33
      Caption = 'Set'
      TabOrder = 21
      OnClick = ATC_Set_Single_Temp_OFSClick
    end
    object A31_Single_Temp_Site: TComboBox
      Left = 248
      Top = 428
      Width = 57
      Height = 28
      ItemHeight = 20
      TabOrder = 18
      Text = '0'
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
        '15')
    end
    object ATC_Set_Single_Temp: TButton
      Left = 368
      Top = 424
      Width = 89
      Height = 33
      Caption = 'Set'
      TabOrder = 17
      OnClick = ATC_Set_Single_TempClick
    end
    object Button2: TButton
      Left = 512
      Top = 312
      Width = 161
      Height = 33
      Caption = 'Close Message From'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      OnClick = Button2Click
    end
    object A31_Single_Temp: TEdit
      Left = 312
      Top = 428
      Width = 49
      Height = 28
      TabOrder = 19
      Text = '0'
      OnClick = A31_TemperatureClick
    end
    object A31_Single_Temp_OFS: TEdit
      Left = 312
      Top = 468
      Width = 49
      Height = 28
      TabOrder = 23
      Text = '0'
      OnClick = A31_Temp_OFSClick
    end
    object Temp_Panel: TPanel
      Left = 8
      Top = 8
      Width = 673
      Height = 201
      BevelOuter = bvNone
      Color = clRed
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      object Panel5: TPanel
        Left = 0
        Top = 72
        Width = 33
        Height = 25
        Caption = 'TJ'
        Color = clInfoBk
        TabOrder = 3
      end
      object Panel7: TPanel
        Left = 0
        Top = 176
        Width = 33
        Height = 25
        Caption = 'TJ'
        Color = clInfoBk
        TabOrder = 7
      end
      object Panel4: TPanel
        Left = 0
        Top = 48
        Width = 33
        Height = 25
        Caption = 'TC2'
        Color = clInfoBk
        TabOrder = 2
      end
      object Panel3: TPanel
        Left = 0
        Top = 24
        Width = 33
        Height = 25
        Caption = 'TC'
        Color = clInfoBk
        TabOrder = 1
      end
      object Panel9: TPanel
        Left = 0
        Top = 0
        Width = 33
        Height = 25
        Color = clInfoBk
        TabOrder = 0
      end
      object Panel6: TPanel
        Left = 0
        Top = 152
        Width = 33
        Height = 25
        Caption = 'TC2'
        Color = clInfoBk
        TabOrder = 6
      end
      object Panel2: TPanel
        Left = 0
        Top = 128
        Width = 33
        Height = 25
        Caption = 'TC'
        Color = clInfoBk
        TabOrder = 5
      end
      object Panel8: TPanel
        Left = 0
        Top = 104
        Width = 33
        Height = 25
        Color = clInfoBk
        TabOrder = 4
      end
    end
    object cBox_RecordComm: TCheckBox
      Left = 528
      Top = 216
      Width = 153
      Height = 17
      Caption = 'Record Command'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = cBox_RecordCommClick
    end
    object ATC_RecipeFile: TComboBox
      Left = 8
      Top = 306
      Width = 265
      Height = 28
      ItemHeight = 20
      TabOrder = 7
    end
    object btnGetNowRecipeFile: TButton
      Left = 512
      Top = 352
      Width = 161
      Height = 33
      Caption = 'Get Now Recipe File'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 12
      OnClick = btnGetNowRecipeFileClick
    end
    object btnGetRecipeFileList: TButton
      Left = 512
      Top = 392
      Width = 161
      Height = 33
      Caption = 'Get Recipe File List'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 16
      OnClick = btnGetRecipeFileListClick
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 632
      Width = 673
      Height = 81
      Caption = 'SW / FW Version'
      TabOrder = 25
      object lab_SW: TLabel
        Left = 8
        Top = 24
        Width = 56
        Height = 20
        Caption = 'lab_SW'
      end
      object labFW1: TLabel
        Left = 120
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW1'
      end
      object labFW7: TLabel
        Left = 120
        Top = 48
        Width = 55
        Height = 20
        Caption = 'labFW7'
      end
      object labFW2: TLabel
        Left = 208
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW2'
      end
      object labFW8: TLabel
        Left = 208
        Top = 48
        Width = 55
        Height = 20
        Caption = 'labFW8'
      end
      object labFW3: TLabel
        Left = 296
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW3'
      end
      object labFW9: TLabel
        Left = 296
        Top = 48
        Width = 55
        Height = 20
        Caption = 'labFW9'
      end
      object labFW4: TLabel
        Left = 384
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW4'
      end
      object labFW10: TLabel
        Left = 384
        Top = 48
        Width = 64
        Height = 20
        Caption = 'labFW10'
      end
      object labFW5: TLabel
        Left = 472
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW5'
      end
      object labFW11: TLabel
        Left = 472
        Top = 48
        Width = 64
        Height = 20
        Caption = 'labFW11'
      end
      object labFW6: TLabel
        Left = 560
        Top = 24
        Width = 55
        Height = 20
        Caption = 'labFW6'
      end
      object labFW12: TLabel
        Left = 560
        Top = 48
        Width = 64
        Height = 20
        Caption = 'labFW12'
      end
    end
    object A31_LotID: TEdit
      Left = 376
      Top = 248
      Width = 145
      Height = 28
      TabOrder = 4
      Text = 'A31_LotID'
    end
    object A31_LotStart: TButton
      Left = 528
      Top = 240
      Width = 73
      Height = 33
      Caption = 'Lot Start'
      TabOrder = 2
    end
    object A31_LotEnd: TButton
      Left = 608
      Top = 240
      Width = 73
      Height = 33
      Caption = 'Lot End'
      TabOrder = 3
    end
    object btnGetSelfTestResult: TButton
      Left = 520
      Top = 456
      Width = 161
      Height = 33
      Caption = 'Get Self-Test Result'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 20
      OnClick = btnGetSelfTestResultClick
    end
    object btnFFCEnable: TButton
      Left = 416
      Top = 344
      Width = 97
      Height = 33
      Caption = 'FFCEnable'
      TabOrder = 10
      OnClick = btnFFCEnableClick
    end
    object btnFFCTrigger: TButton
      Left = 412
      Top = 384
      Width = 105
      Height = 33
      Caption = 'FFCTrigger'
      TabOrder = 14
      OnClick = btnFFCTriggerClick
    end
    object probRecipe: TProgressBar
      Left = 8
      Top = 752
      Width = 281
      Height = 25
      Min = 0
      Max = 100
      TabOrder = 26
    end
  end
  object PageControl: TPageControl
    Left = 689
    Top = 0
    Width = 0
    Height = 791
    ActivePage = TabSheet31
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabIndex = 3
    TabOrder = 2
    object TabSheet20: TTabSheet
      Caption = 'ATC 2.0'
    end
    object TabSheet21: TTabSheet
      Caption = 'ATC 2.1'
      ImageIndex = 1
    end
    object TabSheet30: TTabSheet
      Caption = 'ATC 3.0'
      ImageIndex = 2
    end
    object TabSheet31: TTabSheet
      Caption = 'ATC 3.1'
      ImageIndex = 3
      object SecondSensorFunction: TSpeedButton
        Left = 4
        Top = 464
        Width = 161
        Height = 33
        AllowAllUp = True
        GroupIndex = 5
        Caption = '2nd Sensor Function'
        OnClick = SecondSensorFunctionClick
      end
      object SpeedButton1: TSpeedButton
        Left = 48
        Top = 504
        Width = 145
        Height = 33
        AllowAllUp = True
        GroupIndex = 5
        Caption = 'Run Self-Test'
        OnClick = SpeedButton1Click
      end
      object SpeedButton2: TSpeedButton
        Left = 48
        Top = 544
        Width = 145
        Height = 33
        AllowAllUp = True
        GroupIndex = 5
        Caption = 'Manual Self-Test'
        OnClick = SpeedButton2Click
      end
      object SpeedButton3: TSpeedButton
        Left = 46
        Top = 584
        Width = 145
        Height = 33
        AllowAllUp = True
        GroupIndex = 5
        Caption = 'Abnormal Site'
        OnClick = SpeedButton3Click
      end
      object SpeedButton4: TSpeedButton
        Left = 46
        Top = 624
        Width = 145
        Height = 33
        AllowAllUp = True
        GroupIndex = 5
        Caption = 'Get Controller SN'
        OnClick = SpeedButton4Click
      end
      object A31_Temp_Reading_Mode: TComboBox
        Left = 5
        Top = 212
        Width = 182
        Height = 28
        ItemHeight = 20
        TabOrder = 5
        Text = 'TEMP_FROM_TC'
        Visible = False
        Items.Strings = (
          'TEMP_FROM_TC'
          'TEMP_FROM_TJ'
          'TEMP_FROM_AD_TC'
          'TEMP_FROM_AD_TJ'
          'TEMP_FROM_INT_TC')
      end
      object A31_Set_Temp_Reading_Mode: TButton
        Left = 4
        Top = 248
        Width = 182
        Height = 33
        Caption = 'Set Temp. Read Mode'
        TabOrder = 6
        Visible = False
        OnClick = A31_Set_Temp_Reading_ModeClick
      end
      object A31_Handler_Arm: TButton
        Left = 4
        Top = 8
        Width = 161
        Height = 33
        Caption = 'Handler Arm'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnClick = A31_Handler_ArmClick
      end
      object A31_Test_Start: TButton
        Left = 4
        Top = 48
        Width = 161
        Height = 33
        Caption = 'Test Start'
        TabOrder = 1
        OnClick = A31_Test_StartClick
      end
      object A31_Initial_Set: TButton
        Left = 4
        Top = 88
        Width = 161
        Height = 33
        Caption = 'Initial ATC Set'
        TabOrder = 2
        Visible = False
        OnClick = A31_Initial_SetClick
      end
      object A31_Chiller_Status: TButton
        Left = 4
        Top = 129
        Width = 161
        Height = 33
        Caption = 'Chiller Status'
        TabOrder = 3
        Visible = False
        OnClick = A31_Chiller_StatusClick
      end
      object A31_GetAlarmMsg: TButton
        Left = 4
        Top = 168
        Width = 161
        Height = 33
        Caption = 'Get Alarm Msg'
        TabOrder = 4
        OnClick = A31_GetAlarmMsgClick
      end
      object Panel11: TPanel
        Left = 0
        Top = 288
        Width = 185
        Height = 169
        BevelInner = bvRaised
        BevelOuter = bvLowered
        TabOrder = 7
        object Label3: TLabel
          Left = 8
          Top = 16
          Width = 101
          Height = 20
          Caption = 'PID CH Count'
        end
        object Label4: TLabel
          Left = 8
          Top = 48
          Width = 10
          Height = 20
          Caption = 'P'
        end
        object Label5: TLabel
          Left = 8
          Top = 80
          Width = 5
          Height = 20
          Caption = 'I'
        end
        object Label6: TLabel
          Left = 8
          Top = 112
          Width = 12
          Height = 20
          Caption = 'D'
        end
        object cBox_PID: TComboBox
          Left = 112
          Top = 12
          Width = 57
          Height = 28
          ItemHeight = 20
          TabOrder = 0
          Text = '1'
          Items.Strings = (
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
            '16'
            '17'
            '18'
            '19'
            '20'
            '21'
            '22'
            '23'
            '24'
            '25'
            '26'
            '27'
            '28'
            '29'
            '30'
            '31'
            '32')
        end
        object ed_P: TEdit
          Left = 32
          Top = 48
          Width = 57
          Height = 28
          TabOrder = 1
          Text = '100'
          OnClick = ed_PClick
        end
        object ed_I: TEdit
          Left = 32
          Top = 80
          Width = 57
          Height = 28
          TabOrder = 3
          Text = '0.1'
          OnClick = ed_PClick
        end
        object ed_D: TEdit
          Left = 32
          Top = 112
          Width = 57
          Height = 28
          TabOrder = 4
          Text = '10'
          OnClick = ed_PClick
        end
        object PID_Set: TButton
          Left = 96
          Top = 48
          Width = 75
          Height = 33
          Caption = 'Set'
          TabOrder = 2
          OnClick = PID_SetClick
        end
      end
      object Edit1: TEdit
        Left = 8
        Top = 504
        Width = 33
        Height = 28
        ReadOnly = True
        TabOrder = 8
        Text = '0'
      end
      object Edit2: TEdit
        Left = 8
        Top = 544
        Width = 33
        Height = 28
        ReadOnly = True
        TabOrder = 9
        Text = '0'
      end
      object Edit3: TEdit
        Left = 8
        Top = 584
        Width = 33
        Height = 28
        TabOrder = 10
        Text = '0'
      end
    end
    object TabSheet50: TTabSheet
      Caption = 'ATC 5.0'
      ImageIndex = 4
      object GroupBox1: TGroupBox
        Left = 0
        Top = 8
        Width = 185
        Height = 105
        Caption = 'ATC Module'
        TabOrder = 0
        object AtcModuleBox: TComboBox
          Left = 16
          Top = 32
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 0
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4')
        end
        object RunModuleBtn: TButton
          Left = 16
          Top = 72
          Width = 75
          Height = 25
          Caption = 'Run'
          TabOrder = 1
        end
        object StopModuleBtn: TButton
          Left = 96
          Top = 72
          Width = 75
          Height = 25
          Caption = 'Stop'
          TabOrder = 2
        end
      end
    end
    object TabSheet51: TTabSheet
      Caption = 'ATC 5.1'
      ImageIndex = 7
    end
    object TabSheet60: TTabSheet
      Caption = 'ATC 6.0'
      ImageIndex = 5
    end
    object TabSheet70: TTabSheet
      Caption = 'ATC 7.0'
      ImageIndex = 6
      object EMG_UP_DOWN: TCheckBox
        Left = 8
        Top = 144
        Width = 137
        Height = 17
        Caption = 'EMG Up/Down'
        TabOrder = 0
        OnClick = EMG_UP_DOWNClick
      end
      object GroupBox3: TGroupBox
        Left = 8
        Top = 16
        Width = 145
        Height = 121
        Caption = 'Use TSD'
        TabOrder = 1
        object cBox_CH1: TCheckBox
          Left = 8
          Top = 24
          Width = 97
          Height = 17
          Caption = 'CH1'
          TabOrder = 0
        end
        object cBox_CH2: TCheckBox
          Left = 8
          Top = 48
          Width = 97
          Height = 17
          Caption = 'CH2'
          TabOrder = 1
        end
        object cBox_CH3: TCheckBox
          Left = 8
          Top = 72
          Width = 97
          Height = 17
          Caption = 'CH3'
          TabOrder = 2
        end
        object cBox_CH4: TCheckBox
          Left = 8
          Top = 96
          Width = 97
          Height = 17
          Caption = 'CH4'
          TabOrder = 3
        end
        object BtnSetTSD: TButton
          Left = 72
          Top = 24
          Width = 59
          Height = 89
          Caption = 'Set'
          TabOrder = 4
          OnClick = BtnSetTSDClick
        end
      end
    end
  end
  object Panel10: TPanel
    Left = 689
    Top = 0
    Width = 399
    Height = 791
    Align = alRight
    TabOrder = 1
    object A31_Memo: TMemo
      Left = 1
      Top = 1
      Width = 397
      Height = 624
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
    end
    object AlarmMsg: TMemo
      Left = 1
      Top = 625
      Width = 397
      Height = 165
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      Lines.Strings = (
        '1'
        '2'
        '3'
        '4')
      ParentFont = False
      TabOrder = 1
    end
  end
  object ClientSocket: TClientSocket
    Active = False
    Address = '172.16.8.101'
    ClientType = ctNonBlocking
    Port = 1234
    OnConnect = ClientSocketConnect
    OnDisconnect = ClientSocketDisconnect
    OnRead = ClientSocketRead
    OnError = ClientSocketError
    Left = 648
    Top = 16
  end
  object Timer: TTimer
    Interval = 10
    OnTimer = TimerTimer
    Left = 616
    Top = 15
  end
  object CommFlagTimer: TTimer
    OnTimer = CommFlagTimerTimer
    Left = 584
    Top = 16
  end
  object FileSocket: TClientSocket
    Active = False
    Address = '172.16.8.101'
    ClientType = ctNonBlocking
    Port = 5678
    OnConnect = FileSocketConnect
    OnDisconnect = FileSocketDisconnect
    OnRead = FileSocketRead
    OnError = FileSocketError
    Left = 648
    Top = 56
  end
end
