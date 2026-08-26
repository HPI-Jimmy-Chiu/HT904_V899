object fAutoAlignment: TfAutoAlignment
  Left = 985
  Top = 97
  Width = 866
  Height = 672
  Caption = 'AOA'
  Color = clBtnFace
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 850
    Height = 633
    Align = alClient
    Color = 13550775
    TabOrder = 0
    object GroupBox47: TGroupBox
      Left = 310
      Top = 56
      Width = 297
      Height = 515
      Caption = 'Auto Alignment'
      TabOrder = 2
      object Label819: TLabel
        Left = 16
        Top = 318
        Width = 82
        Height = 13
        Caption = 'Device thickness'
        Visible = False
      end
      object Label820: TLabel
        Left = 216
        Top = 318
        Width = 37
        Height = 13
        Caption = '0.01mm'
        Visible = False
      end
      object GroupBox46: TGroupBox
        Left = 2
        Top = 61
        Width = 293
        Height = 132
        Align = alTop
        Caption = 'Tray'
        TabOrder = 1
        object cbAutoAlignmentTray_AfterHome: TCheckBox
          Left = 24
          Top = 17
          Width = 105
          Height = 17
          Caption = 'After Home'
          Enabled = False
          TabOrder = 1
          OnClick = cbAutoAlignmentTray_AfterHomeClick
        end
        object cbAutoAlignmentTray_ChangeSetup: TCheckBox
          Left = 24
          Top = 41
          Width = 177
          Height = 17
          Caption = 'Change Setup'
          TabOrder = 3
        end
        object cbAutoAlignmentTray_InitialStart: TCheckBox
          Left = 24
          Top = 65
          Width = 177
          Height = 17
          Caption = 'Initial Start'
          TabOrder = 5
        end
        object cbAutoAlignmentTray_EveryTray: TCheckBox
          Left = 24
          Top = 89
          Width = 177
          Height = 17
          Caption = 'Every Tray'
          TabOrder = 7
        end
        object cbAutoAlignmentTray_OneCycle: TCheckBox
          Left = 24
          Top = 113
          Width = 177
          Height = 17
          Caption = 'After OneCycle'
          TabOrder = 9
        end
        object cbAutoAlignmentTray_AfterHome_Z: TCheckBox
          Left = 168
          Top = 16
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 0
          Visible = False
        end
        object cbAutoAlignmentTray_ChangeSetup_Z: TCheckBox
          Left = 168
          Top = 40
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 2
          Visible = False
        end
        object cbAutoAlignmentTray_InitialStart_Z: TCheckBox
          Left = 168
          Top = 64
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 4
          Visible = False
        end
        object cbAutoAlignmentTray_EveryTray_Z: TCheckBox
          Left = 168
          Top = 88
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 6
          Visible = False
        end
        object cbAutoAlignmentTray_OneCycle_Z: TCheckBox
          Left = 168
          Top = 112
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 8
          Visible = False
        end
      end
      object grpInputArea: TGroupBox
        Left = 2
        Top = 193
        Width = 293
        Height = 112
        Align = alTop
        Caption = 'Shuttle/Hotplate'
        TabOrder = 2
        object cbAutoAlignmentCK_ChangeSetup: TCheckBox
          Left = 24
          Top = 41
          Width = 177
          Height = 17
          Caption = 'Change Setup'
          TabOrder = 2
        end
        object cbAutoAlignmentCK_InitialStart: TCheckBox
          Left = 24
          Top = 65
          Width = 177
          Height = 17
          Caption = 'Initial Start'
          TabOrder = 4
        end
        object cbAutoAlignmentCK_AfterHome: TCheckBox
          Left = 24
          Top = 17
          Width = 97
          Height = 17
          Caption = 'After Home'
          TabOrder = 0
        end
        object cbAutoAlignmentCK_OneCycle: TCheckBox
          Left = 24
          Top = 89
          Width = 177
          Height = 17
          Caption = 'After OneCycle'
          TabOrder = 6
        end
        object cbAutoAlignmentCK_ChangeSetup_Z: TCheckBox
          Left = 168
          Top = 41
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 3
          Visible = False
        end
        object cbAutoAlignmentCK_InitialStart_Z: TCheckBox
          Left = 168
          Top = 65
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 5
          Visible = False
        end
        object cbAutoAlignmentCK_AfterHome_Z: TCheckBox
          Left = 168
          Top = 17
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 1
          Visible = False
        end
        object cbAutoAlignmentCK_OneCycle_Z: TCheckBox
          Left = 168
          Top = 89
          Width = 85
          Height = 17
          Caption = 'Include Z'
          TabOrder = 7
          Visible = False
        end
      end
      object edAutoAlignment_DeviceThick: TEdit
        Left = 137
        Top = 315
        Width = 65
        Height = 21
        TabOrder = 3
        Text = '0'
        Visible = False
        OnMouseDown = edAutoAlignment_DeviceThickMouseDown
      end
      object PageControl19: TPageControl
        Left = 2
        Top = 333
        Width = 293
        Height = 180
        ActivePage = TabSheet36
        Align = alBottom
        TabIndex = 1
        TabOrder = 4
        Visible = False
        object TabSheet33: TTabSheet
          Caption = 'Tray'
          object GroupBox53: TGroupBox
            Left = 0
            Top = 0
            Width = 285
            Height = 77
            Align = alClient
            Caption = 'InArm Z'
            TabOrder = 0
            object Label821: TLabel
              Left = 5
              Top = 24
              Width = 70
              Height = 13
              Caption = 'Realase Offset'
            end
            object Label822: TLabel
              Left = 202
              Top = 24
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object Label823: TLabel
              Left = 5
              Top = 49
              Width = 69
              Height = 13
              Caption = 'Pick Up Offset'
            end
            object Label824: TLabel
              Left = 202
              Top = 49
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object edAutoAlignmentTray_InArmZRealaseOffset: TEdit
              Left = 126
              Top = 21
              Width = 65
              Height = 21
              TabOrder = 0
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
            object edAutoAlignmentTray_InArmZPickUpOffset: TEdit
              Left = 126
              Top = 46
              Width = 65
              Height = 21
              TabOrder = 1
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
          end
          object GroupBox54: TGroupBox
            Left = 0
            Top = 77
            Width = 285
            Height = 75
            Align = alBottom
            Caption = 'OutArm Z'
            TabOrder = 1
            object Label825: TLabel
              Left = 5
              Top = 24
              Width = 70
              Height = 13
              Caption = 'Realase Offset'
            end
            object Label826: TLabel
              Left = 202
              Top = 24
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object Label827: TLabel
              Left = 5
              Top = 49
              Width = 69
              Height = 13
              Caption = 'Pick Up Offset'
            end
            object Label828: TLabel
              Left = 202
              Top = 49
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object edAutoAlignmentTray_OutArmZRealaseOffset: TEdit
              Left = 126
              Top = 21
              Width = 65
              Height = 21
              TabOrder = 0
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
            object edAutoAlignmentTray_OutArmZPickUpOffset: TEdit
              Left = 126
              Top = 46
              Width = 65
              Height = 21
              TabOrder = 1
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
          end
        end
        object TabSheet36: TTabSheet
          Caption = 'Kit'
          ImageIndex = 1
          object GroupBox57: TGroupBox
            Left = 0
            Top = 0
            Width = 285
            Height = 77
            Align = alClient
            Caption = 'InArm Z'
            TabOrder = 0
            object Label829: TLabel
              Left = 5
              Top = 24
              Width = 70
              Height = 13
              Caption = 'Realase Offset'
            end
            object Label830: TLabel
              Left = 202
              Top = 24
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object Label831: TLabel
              Left = 5
              Top = 49
              Width = 69
              Height = 13
              Caption = 'Pick Up Offset'
            end
            object Label832: TLabel
              Left = 202
              Top = 49
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object edAutoAlignmentCK_InArmZRealaseOffset: TEdit
              Left = 126
              Top = 21
              Width = 65
              Height = 21
              TabOrder = 0
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
            object edAutoAlignmentCK_InArmZPickUpOffset: TEdit
              Left = 126
              Top = 46
              Width = 65
              Height = 21
              TabOrder = 1
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
          end
          object GroupBox58: TGroupBox
            Left = 0
            Top = 77
            Width = 285
            Height = 75
            Align = alBottom
            Caption = 'OutArm Z'
            TabOrder = 1
            object Label833: TLabel
              Left = 5
              Top = 24
              Width = 70
              Height = 13
              Caption = 'Realase Offset'
            end
            object Label834: TLabel
              Left = 202
              Top = 24
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object Label835: TLabel
              Left = 5
              Top = 49
              Width = 69
              Height = 13
              Caption = 'Pick Up Offset'
            end
            object Label836: TLabel
              Left = 202
              Top = 49
              Width = 37
              Height = 13
              Caption = '0.01mm'
            end
            object edAutoAlignmentCK_OutArmZRealaseOffset: TEdit
              Left = 126
              Top = 21
              Width = 65
              Height = 21
              TabOrder = 0
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
            object edAutoAlignmentCK_OutArmZPickUpOffset: TEdit
              Left = 126
              Top = 46
              Width = 65
              Height = 21
              TabOrder = 1
              Text = '0'
              OnClick = edAutoAlignmentTray_InArmZRealaseOffsetClick
            end
          end
        end
      end
      object pnlUseFixTray: TPanel
        Left = 2
        Top = 15
        Width = 293
        Height = 46
        Align = alTop
        BevelOuter = bvNone
        Color = 13550775
        TabOrder = 0
        object cbAOA_UseFix1: TCheckBox
          Left = 32
          Top = 3
          Width = 60
          Height = 17
          Caption = 'Use Fix1'
          TabOrder = 0
        end
        object cbAOA_UseFix3: TCheckBox
          Left = 192
          Top = 3
          Width = 60
          Height = 17
          Caption = 'Use Fix3'
          TabOrder = 2
        end
        object cbAOA_UseFix2: TCheckBox
          Left = 112
          Top = 3
          Width = 60
          Height = 17
          Caption = 'Use Fix2'
          TabOrder = 1
        end
        object cbAOA_UseFix4: TCheckBox
          Left = 32
          Top = 23
          Width = 60
          Height = 17
          Caption = 'Use Fix4'
          TabOrder = 3
        end
        object cbAOA_UseFix5: TCheckBox
          Left = 112
          Top = 23
          Width = 60
          Height = 17
          Caption = 'Use Fix5'
          TabOrder = 4
        end
        object cbAOA_UseFix6: TCheckBox
          Left = 192
          Top = 23
          Width = 60
          Height = 17
          Caption = 'Use Fix6'
          TabOrder = 5
        end
      end
    end
    object cbEnabledAutoAlignment: TCheckBox
      Left = 13
      Top = 23
      Width = 284
      Height = 26
      Caption = 'Enabled Auto Alignment'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object PageControl18: TPageControl
      Left = 615
      Top = 56
      Width = 227
      Height = 515
      ActivePage = TabSheet34
      TabIndex = 0
      TabOrder = 3
      object TabSheet34: TTabSheet
        Caption = 'Tray'
        object Label805: TLabel
          Left = 5
          Top = 368
          Width = 209
          Height = 16
          Caption = 'X:min value 2 Y:min value 2'
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = #26032#32048#26126#39636
          Font.Style = [fsBold]
          ParentFont = False
        end
        object myAlignmentTray: TTMyTray
          Left = 16
          Top = 24
          Width = 185
          Height = 313
          XItem = 16
          YItem = 35
        end
        object GroupBox49: TGroupBox
          Left = 24
          Top = 384
          Width = 185
          Height = 81
          TabOrder = 1
          object Label810: TLabel
            Left = 8
            Top = 49
            Width = 34
            Height = 13
            Caption = 'Y Point'
          end
          object Label807: TLabel
            Left = 8
            Top = 17
            Width = 34
            Height = 13
            Caption = 'X Point'
          end
          object edtAlignmentPointX: TEdit
            Left = 72
            Top = 16
            Width = 33
            Height = 21
            TabOrder = 0
            Text = '2'
            OnMouseDown = edtAlignmentPointXMouseDown
          end
          object edtAlignmentPointY: TEdit
            Left = 72
            Top = 48
            Width = 33
            Height = 21
            TabOrder = 2
            Text = '4'
            OnMouseDown = edtAlignmentPointXMouseDown
          end
          object btnAutoAlignmentTrayPointCal: TButton
            Left = 112
            Top = 40
            Width = 57
            Height = 25
            Caption = 'Set'
            TabOrder = 1
            OnClick = btnAutoAlignmentTrayPointCalClick
          end
        end
      end
      object TabSheet35: TTabSheet
        Caption = 'HotPlate'
        ImageIndex = 1
        object Label806: TLabel
          Left = 5
          Top = 368
          Width = 209
          Height = 16
          Caption = 'X:min value 2 Y:min value 2'
          Font.Charset = CHINESEBIG5_CHARSET
          Font.Color = clRed
          Font.Height = -16
          Font.Name = #26032#32048#26126#39636
          Font.Style = [fsBold]
          ParentFont = False
        end
        object myAlignmentPlate: TTMyTray
          Left = 16
          Top = 24
          Width = 185
          Height = 313
          XItem = 16
          YItem = 35
        end
        object GroupBox50: TGroupBox
          Left = 24
          Top = 384
          Width = 185
          Height = 81
          TabOrder = 1
          object Label817: TLabel
            Left = 8
            Top = 49
            Width = 34
            Height = 13
            Caption = 'Y Point'
          end
          object Label818: TLabel
            Left = 8
            Top = 17
            Width = 34
            Height = 13
            Caption = 'X Point'
          end
          object edtAlignmentPlatePointX: TEdit
            Left = 72
            Top = 16
            Width = 33
            Height = 21
            TabOrder = 0
            Text = '2'
            OnMouseDown = edtAlignmentPointXMouseDown
          end
          object edtAlignmentPlatePointY: TEdit
            Left = 72
            Top = 48
            Width = 33
            Height = 21
            TabOrder = 2
            Text = '4'
            OnMouseDown = edtAlignmentPointXMouseDown
          end
          object btnAutoAlignmentPlatePointCal: TButton
            Left = 112
            Top = 40
            Width = 57
            Height = 25
            Caption = 'Set'
            TabOrder = 1
            OnClick = btnAutoAlignmentPlatePointCalClick
          end
        end
      end
    end
    object GroupBox3: TGroupBox
      Left = 6
      Top = 56
      Width = 297
      Height = 515
      Caption = 'TCP/IP Setting'
      TabOrder = 1
      object Label395: TLabel
        Left = 13
        Top = 148
        Width = 109
        Height = 13
        Caption = 'Decode time out (ms) : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        Visible = False
      end
      object GroupBox1: TGroupBox
        Left = 8
        Top = 18
        Width = 140
        Height = 89
        Caption = 'InArm Address Port'
        TabOrder = 0
        object Label1: TLabel
          Left = 6
          Top = 28
          Width = 32
          Height = 13
          Caption = 'Adress'
        end
        object Label2: TLabel
          Left = 13
          Top = 60
          Width = 19
          Height = 13
          Caption = 'Port'
        end
        object edtInArmAddress: TEdit
          Left = 41
          Top = 24
          Width = 90
          Height = 21
          TabOrder = 0
          Text = '192.168.1.1'
        end
        object edtInArmPort: TEdit
          Left = 41
          Top = 56
          Width = 90
          Height = 21
          TabOrder = 1
          Text = '6001'
          OnMouseDown = edtInArmPortMouseDown
        end
      end
      object GroupBox2: TGroupBox
        Left = 149
        Top = 18
        Width = 140
        Height = 89
        Caption = 'OutArm Address Port'
        TabOrder = 1
        object Label3: TLabel
          Left = 8
          Top = 28
          Width = 32
          Height = 13
          Caption = 'Adress'
        end
        object Label4: TLabel
          Left = 14
          Top = 60
          Width = 19
          Height = 13
          Caption = 'Port'
        end
        object edtOutArmAddress: TEdit
          Left = 42
          Top = 24
          Width = 90
          Height = 21
          TabOrder = 0
          Text = '192.168.1.1'
        end
        object edtOutArmPort: TEdit
          Left = 42
          Top = 56
          Width = 90
          Height = 21
          TabOrder = 1
          Text = '6001'
          OnMouseDown = edtInArmPortMouseDown
        end
      end
      object btnConnect: TButton
        Left = 8
        Top = 110
        Width = 81
        Height = 25
        Caption = 'Connect'
        TabOrder = 2
        OnClick = btnConnectClick
      end
      object btnDisconnect: TButton
        Left = 93
        Top = 110
        Width = 81
        Height = 25
        Caption = 'Disconnect'
        TabOrder = 3
        OnClick = btnDisconnectClick
      end
      object GroupBox4: TGroupBox
        Left = 8
        Top = 169
        Width = 281
        Height = 340
        Caption = 'Command Test'
        TabOrder = 5
        object Label5: TLabel
          Left = 9
          Top = 72
          Width = 27
          Height = 13
          Caption = 'InArm'
        end
        object Label6: TLabel
          Left = 5
          Top = 210
          Width = 35
          Height = 13
          Caption = 'OutArm'
        end
        object Label7: TLabel
          Left = 9
          Top = 32
          Width = 56
          Height = 13
          Caption = 'File Name : '
        end
        object cbInArmCommand: TComboBox
          Left = 48
          Top = 68
          Width = 145
          Height = 21
          ItemHeight = 13
          TabOrder = 1
          Text = 'Load File'
          Items.Strings = (
            'Load File'
            'Teach CCD Center'
            'Teach CCD Left'
            'Teach CCD Right'
            'Teach CCD Up'
            'Teach CCD Down'
            'Teach CCD Golden Pattern'
            'Teach CCD Tray Pattern'
            'Teach CCD HotPlate Pattern'
            'Teach CCD Shuttle Pattern'
            'Wait Teach Reply'
            'Auto Get Golden Postion'
            'Auto Get Tray Postion'
            'Auto Get Hotplate Postion'
            'Auto Get Shuttle Postion'
            'Teach CCD AutoClean Pattern'
            'Auto Get AutoClean Postion')
        end
        object cbOutArmCommand: TComboBox
          Left = 48
          Top = 206
          Width = 145
          Height = 21
          ItemHeight = 13
          TabOrder = 4
          Text = 'Load File'
          Items.Strings = (
            'Load File'
            'Teach CCD Center'
            'Teach CCD Left'
            'Teach CCD Right'
            'Teach CCD Up'
            'Teach CCD Down'
            'Teach CCD Golden Pattern'
            'Teach CCD Tray Pattern'
            'Teach CCD HotPlate Pattern'
            'Teach CCD Shuttle Pattern'
            'Wait Teach Reply'
            'Auto Get Golden Postion'
            'Auto Get Tray Postion'
            'Auto Get Hotplate Postion'
            'Auto Get Shuttle Postion'
            'Teach CCD AutoClean Pattern'
            'Auto Get AutoClean Postion')
        end
        object Memo1: TMemo
          Left = 1
          Top = 91
          Width = 277
          Height = 106
          Color = 14670284
          TabOrder = 3
        end
        object Memo2: TMemo
          Left = 2
          Top = 232
          Width = 277
          Height = 106
          Align = alBottom
          Color = 14670284
          TabOrder = 6
        end
        object Button1: TButton
          Left = 200
          Top = 68
          Width = 60
          Height = 21
          Caption = 'Send'
          TabOrder = 2
          OnClick = Button1Click
        end
        object Button2: TButton
          Left = 200
          Top = 206
          Width = 60
          Height = 21
          Caption = 'Send'
          TabOrder = 5
          OnClick = Button2Click
        end
        object edtFileName: TEdit
          Left = 72
          Top = 29
          Width = 121
          Height = 21
          TabOrder = 0
          Text = 'edtFileName'
          OnChange = edtFileNameChange
        end
      end
      object edAOADecodetimeout: TEdit
        Left = 129
        Top = 144
        Width = 81
        Height = 21
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Text = '3000'
        Visible = False
        OnMouseDown = edAutoAlignment_DeviceThickMouseDown
      end
    end
    object Panel2: TPanel
      Left = 1
      Top = 588
      Width = 848
      Height = 44
      Align = alBottom
      BevelOuter = bvNone
      Color = 13550775
      TabOrder = 4
      object spbSave: TSpeedButton
        Left = 110
        Top = 3
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
        Left = 500
        Top = 2
        Width = 227
        Height = 40
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
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 733
    Top = 8
  end
  object ClientSocket2: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket2Connect
    OnDisconnect = ClientSocket2Disconnect
    OnRead = ClientSocket2Read
    OnError = ClientSocket2Error
    Left = 816
    Top = 8
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket1Connect
    OnDisconnect = ClientSocket1Disconnect
    OnRead = ClientSocket1Read
    OnError = ClientSocket1Error
    Left = 776
    Top = 8
  end
end
