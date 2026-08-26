object HandlerSystem: THandlerSystem
  Left = 370
  Top = 190
  Width = 1278
  Height = 977
  Caption = 'Handler System'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -16
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    1270
    946)
  PixelsPerInch = 96
  TextHeight = 20
  object Label30: TLabel
    Left = 36
    Top = 174
    Width = 166
    Height = 153
    Alignment = taRightJustify
    Anchors = [akLeft, akTop, akRight, akBottom]
    AutoSize = False
    Caption = 'NUDN1 qty'
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 1270
    Height = 946
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object pcSetting: TPageControl
      Left = 0
      Top = 0
      Width = 1270
      Height = 883
      ActivePage = TabSheet4
      Align = alClient
      MultiLine = True
      TabIndex = 3
      TabOrder = 0
      TabWidth = 200
      OnChange = pcSettingChange
      object TabSheet1: TTabSheet
        Caption = 'Handler'
        object pnlHandler3: TPanel
          Left = 480
          Top = 0
          Width = 240
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object GroupBox1: TGroupBox
            Left = 4
            Top = 4
            Width = 232
            Height = 293
            Align = alTop
            Caption = 'SafeDoor Enable'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Visible = False
            object SafeDoor1: TCheckBox
              Left = 8
              Top = 24
              Width = 113
              Height = 17
              Caption = 'Safe-Door 1'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object SafeDoor2: TCheckBox
              Left = 8
              Top = 48
              Width = 113
              Height = 17
              Caption = 'Safe-Door 2'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
            object SafeDoor3: TCheckBox
              Left = 8
              Top = 72
              Width = 113
              Height = 17
              Caption = 'Safe-Door 3'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
            end
            object SafeDoor4: TCheckBox
              Left = 8
              Top = 96
              Width = 113
              Height = 17
              Caption = 'Safe-Door 4'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 3
            end
            object SafeDoor5: TCheckBox
              Left = 8
              Top = 120
              Width = 113
              Height = 17
              Caption = 'Safe-Door 5'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 4
            end
            object SafeDoor6: TCheckBox
              Left = 8
              Top = 144
              Width = 113
              Height = 17
              Caption = 'Safe-Door 6'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 5
            end
            object SafeDoor7: TCheckBox
              Left = 8
              Top = 168
              Width = 113
              Height = 17
              Caption = 'Safe-Door 7'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 6
            end
            object SafeDoor8: TCheckBox
              Left = 8
              Top = 192
              Width = 113
              Height = 17
              Caption = 'Safe-Door 8'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 7
            end
            object BtnEnableAll: TButton
              Left = 8
              Top = 260
              Width = 113
              Height = 25
              Caption = 'Disable All'
              TabOrder = 10
              OnClick = BtnEnableAllClick
            end
            object SafeDoor9: TCheckBox
              Left = 8
              Top = 216
              Width = 113
              Height = 17
              Caption = 'Safe-Door 9'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 8
            end
            object SafeDoor10: TCheckBox
              Left = 8
              Top = 240
              Width = 117
              Height = 17
              Caption = 'Safe-Door 10'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 9
            end
          end
          object GroupBox2: TGroupBox
            Left = 4
            Top = 297
            Width = 232
            Height = 73
            Align = alTop
            Caption = 'Heater Door'
            TabOrder = 1
            Visible = False
            object HeaterDoor1: TCheckBox
              Left = 4
              Top = 24
              Width = 121
              Height = 17
              Caption = 'Safe-Door 1'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 0
            end
            object HeaterDoor2: TCheckBox
              Left = 4
              Top = 48
              Width = 121
              Height = 17
              Caption = 'Safe-Door 2'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
            end
          end
        end
        object pnlHandler1: TPanel
          Left = 0
          Top = 0
          Width = 240
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object GroupBox3: TGroupBox
            Left = 4
            Top = 4
            Width = 232
            Height = 273
            Align = alTop
            Caption = 'Handler Information'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clNavy
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object Label4: TLabel
              Left = 5
              Top = 24
              Width = 132
              Height = 20
              Alignment = taRightJustify
              AutoSize = False
              Caption = 'Customer Code'#65306
            end
            object Label1: TLabel
              Left = 5
              Top = 56
              Width = 132
              Height = 20
              Alignment = taRightJustify
              AutoSize = False
              Caption = 'Serial Number   '#65306
            end
            object Label10: TLabel
              Left = 4
              Top = 88
              Width = 63
              Height = 20
              Caption = 'Model'#65306' '
            end
            object edtCustomerCode: TEdit
              Left = 141
              Top = 22
              Width = 80
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 3
              ParentFont = False
              TabOrder = 0
              Text = '0'
              OnClick = edtCustomerCodeClick
            end
            object edtSeriaNo: TEdit
              Left = 141
              Top = 54
              Width = 80
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              MaxLength = 8
              ParentFont = False
              TabOrder = 1
              Text = '29818'
            end
            object cbHandlerModel: TComboBox
              Left = 76
              Top = 84
              Width = 145
              Height = 28
              ItemHeight = 20
              ItemIndex = 3
              TabOrder = 2
              Text = 'HT9046LS'
              Items.Strings = (
                'HT9045'
                'HT9046'
                'HT9045_12Site'
                'HT9046LS'
                'HT502'
                'HT1032'
                'HT7080')
            end
            object rgModel: TRadioGroup
              Left = 2
              Top = 120
              Width = 228
              Height = 151
              Align = alBottom
              Caption = 'Sub-model'
              ItemIndex = 0
              Items.Strings = (
                'Normal'
                'HT-9046LA'
                'HT-9016C'
                'HT-9046AU'
                'HT-9046CR')
              TabOrder = 3
            end
          end
          object rgMotionCard: TRadioGroup
            Left = 4
            Top = 277
            Width = 232
            Height = 80
            Align = alTop
            Caption = 'Motion Card'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'SYN-TEK Motion-Net'
              'CONTECT SMC-8DF-C01')
            ParentFont = False
            TabOrder = 1
          end
          object rgIOCard: TRadioGroup
            Left = 4
            Top = 357
            Width = 232
            Height = 120
            Align = alTop
            Caption = 'I/O Card'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              'SYN_TEK PCI-L122'
              'PISO MN200'
              'MN200 with New IO Table'
              'PISO P64 C64')
            ParentFont = False
            TabOrder = 2
          end
          object rgTTLCard: TRadioGroup
            Left = 4
            Top = 477
            Width = 232
            Height = 120
            Align = alTop
            Caption = 'TTL Card Interface'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'ISA'
              'PCI 1735U'
              'RS-232(1Board)'
              'RS-232(2Board)')
            ParentFont = False
            TabOrder = 3
            OnClick = rgTTLCardClick
          end
          object rgUseSucker_9046_DB: TRadioGroup
            Left = 4
            Top = 597
            Width = 232
            Height = 60
            Align = alTop
            Caption = 'HT9045 Use Sucker_9046.DB'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object rgUseSensor_9046_DB: TRadioGroup
            Left = 4
            Top = 657
            Width = 232
            Height = 60
            Align = alTop
            Caption = 'HT9045 Use Sensor_9046.DB'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgTTLUseAddress: TRadioGroup
            Left = 152
            Top = 482
            Width = 81
            Height = 51
            Caption = 'Address'
            Items.Strings = (
              'No'
              'Yes')
            TabOrder = 4
          end
        end
        object pnlHandler2: TPanel
          Left = 240
          Top = 0
          Width = 240
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object rgSafeDoorLock: TRadioGroup
            Left = 4
            Top = 4
            Width = 232
            Height = 60
            Align = alTop
            Caption = 'Safe Door Lock'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            TabOrder = 0
          end
          object rgMNetSpeed: TRadioGroup
            Left = 4
            Top = 64
            Width = 232
            Height = 80
            Align = alTop
            Caption = 'MotionNet Speed'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              '2.5M'
              '5M'
              '10M'
              '20M')
            ParentFont = False
            TabOrder = 1
          end
          object rgIndexMotionCard: TRadioGroup
            Left = 4
            Top = 144
            Width = 232
            Height = 80
            Align = alTop
            Caption = 'Index Motion Card'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Galil'
              'CSMC')
            ParentFont = False
            TabOrder = 2
          end
          object rg_ControlPanelMode: TRadioGroup
            Left = 4
            Top = 224
            Width = 232
            Height = 80
            Align = alTop
            Caption = 'Control Panel Mode'
            Color = clBtnFace
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clFuchsia
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'IO control'
              'Communication')
            ParentColor = False
            ParentFont = False
            TabOrder = 3
          end
          object rgVacuUnitType: TRadioGroup
            Left = 4
            Top = 304
            Width = 232
            Height = 80
            Align = alTop
            Caption = 'Vacuum Unit '
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'IO'
              'ECAT-VC8_ODM1')
            ParentFont = False
            TabOrder = 4
          end
          object rgSafePlcIO: TRadioGroup
            Left = 4
            Top = 384
            Width = 240
            Height = 91
            Align = alTop
            Caption = 'Safe PLC IO'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'Install_Schneider'
              'Install_Reer')
            ParentFont = False
            TabOrder = 5
          end
        end
      end
      object TabSheet2: TTabSheet
        Caption = 'Loader / Unloader'
        ImageIndex = 1
        object pnlLoader1: TPanel
          Left = 0
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object rgTrayVibration: TRadioGroup
            Left = 4
            Top = 84
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Tray Vibration'
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Cylinder'
              'Motor')
            TabOrder = 1
          end
          object rgNumberPanelType: TRadioGroup
            Left = 4
            Top = 404
            Width = 217
            Height = 120
            Align = alTop
            Caption = 'Bin Display Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clFuchsia
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              'None'
              '1 Digital Type'
              '2 Digital Type'
              '2 Digital with Color'
              'TFT Type')
            ParentFont = False
            TabOrder = 5
          end
          object rgInstallFix3: TRadioGroup
            Left = 4
            Top = 344
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Fix 3 (for 9046LS)'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 4
          end
          object rgAutoAlignment: TRadioGroup
            Left = 4
            Top = 524
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Auto Alignment'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgLoaderVibration: TRadioGroup
            Left = 4
            Top = 164
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Loader vibration'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Uninstall'
              'Install')
            TabOrder = 2
          end
          object rgFix3FullPlace: TRadioGroup
            Left = 4
            Top = 224
            Width = 217
            Height = 120
            Align = alTop
            Caption = 'Fix 3 Full Place'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              'Un Install'
              'Short Shuttle'
              'Use Cylinder'
              'Use Cylinder for 46LA'
              'Use Stepper Motor')
            ParentFont = False
            TabOrder = 3
          end
          object rgTrayArmMode: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Tray Arm Mode'
            ItemIndex = 0
            Items.Strings = (
              'Above Conveyor'
              'Under Conveyor')
            TabOrder = 0
          end
          object rgCOLORSENSOR_MUN: TRadioGroup
            Left = 4
            Top = 584
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Color Sensor MU-N'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install in Loader')
            ParentFont = False
            TabOrder = 7
          end
          object rgLDCarRotAtm: TRadioGroup
            Left = 4
            Top = 644
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Loader Car Rotate Arm'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Uninstall'
              'Install')
            TabOrder = 8
          end
          object rgOCRYStepMot: TRadioGroup
            Left = 4
            Top = 704
            Width = 217
            Height = 64
            Align = alTop
            Caption = 'Loader Y Use Step Motor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 10
          end
          object rgLdUldCassetteMode: TRadioGroup
            Left = 4
            Top = 768
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'LD/Uld Cassette Mode'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Uninstall'
              'Install')
            TabOrder = 9
          end
        end
        object pnlLoader2: TPanel
          Left = 225
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object Auto2SelectCy: TRadioGroup
            Left = 4
            Top = 153
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Auto 2 Empty'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgLdUldUseStepMotor: TRadioGroup
            Left = 4
            Top = 213
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Loader Unload Step Motor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object MachineTrack: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 149
            Align = alTop
            Caption = 'Machine Track amount'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              '4 Track (E/C Manual)'
              '6 Track (E/C Automatic)'
              '7 Track (Empty Unloader)'
              '8 Track (5 Unloader)'
              '9 Track (6 Unloader)')
            ParentFont = False
            TabOrder = 0
          end
          object grpTrayZUseMot: TGroupBox
            Left = 4
            Top = 273
            Width = 217
            Height = 240
            Align = alTop
            Caption = 'Tray Z Use Motor'
            TabOrder = 3
            object chkLoader: TCheckBox
              Left = 12
              Top = 24
              Width = 125
              Height = 17
              Caption = 'Loader Track'
              TabOrder = 0
            end
            object chkEmpty: TCheckBox
              Left = 12
              Top = 47
              Width = 117
              Height = 17
              Caption = 'Empty Track'
              TabOrder = 1
            end
            object chkColor: TCheckBox
              Left = 12
              Top = 71
              Width = 129
              Height = 17
              Caption = 'Color Track'
              TabOrder = 2
            end
            object chkAuto1: TCheckBox
              Left = 12
              Top = 95
              Width = 165
              Height = 17
              Caption = 'Auto 1 Track'
              TabOrder = 3
            end
            object chkAuto2: TCheckBox
              Left = 12
              Top = 118
              Width = 165
              Height = 17
              Caption = 'Auto 2 Track'
              TabOrder = 4
            end
            object chkAuto3: TCheckBox
              Left = 12
              Top = 141
              Width = 165
              Height = 17
              Caption = 'Auto 3 Track'
              TabOrder = 5
            end
            object chkAuto6: TCheckBox
              Left = 12
              Top = 212
              Width = 165
              Height = 17
              Caption = 'Auto 6 Track'
              TabOrder = 8
            end
            object chkAuto5: TCheckBox
              Left = 12
              Top = 189
              Width = 165
              Height = 17
              Caption = 'Auto 5 Track'
              TabOrder = 7
            end
            object chkAuto4: TCheckBox
              Left = 12
              Top = 165
              Width = 165
              Height = 17
              Caption = 'Auto 4 Track'
              TabOrder = 6
            end
          end
          object gbUseCassetteMode: TGroupBox
            Left = 4
            Top = 513
            Width = 217
            Height = 240
            Align = alTop
            Caption = 'Tray Cassette Mode'
            TabOrder = 4
            object cbLoaderCassette: TCheckBox
              Left = 12
              Top = 24
              Width = 125
              Height = 17
              Caption = 'Loader Track'
              TabOrder = 0
            end
            object cbEmptyCassette: TCheckBox
              Left = 12
              Top = 47
              Width = 117
              Height = 17
              Caption = 'Empty Track'
              TabOrder = 1
            end
            object cbColorCassette: TCheckBox
              Left = 12
              Top = 71
              Width = 129
              Height = 17
              Caption = 'Color Track'
              TabOrder = 2
            end
            object cbAuto1Cassette: TCheckBox
              Left = 12
              Top = 95
              Width = 165
              Height = 17
              Caption = 'Auto 1 Track'
              TabOrder = 3
            end
            object cbAuto2Cassette: TCheckBox
              Left = 12
              Top = 118
              Width = 165
              Height = 17
              Caption = 'Auto 2 Track'
              TabOrder = 4
            end
            object cbAuto3Cassette: TCheckBox
              Left = 12
              Top = 141
              Width = 165
              Height = 17
              Caption = 'Auto 3 Track'
              TabOrder = 5
            end
            object cbAuto4Cassette: TCheckBox
              Left = 12
              Top = 165
              Width = 165
              Height = 17
              Caption = 'Auto 4 Track'
              TabOrder = 6
            end
            object cbAuto5Cassette: TCheckBox
              Left = 12
              Top = 189
              Width = 165
              Height = 17
              Caption = 'Auto 5 Track'
              TabOrder = 7
            end
            object cbAuto6Cassette: TCheckBox
              Left = 12
              Top = 212
              Width = 165
              Height = 17
              Caption = 'Auto 6 Track'
              TabOrder = 8
            end
          end
        end
        object pnlLoader3: TPanel
          Left = 450
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object rgCKDFCM: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'CKD FCM [Clean Air]'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            TabOrder = 0
          end
          object rgCatchTrayModel: TRadioGroup
            Left = 4
            Top = 264
            Width = 217
            Height = 120
            Align = alTop
            Caption = 'Catch Tray Model'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '4 Sensor'
              '2 Sensor'
              '2 Sensor(ART)'
              '4 Sensor(ART)'
              '2 Sensor(ART & No Cover)')
            ParentFont = False
            TabOrder = 4
          end
          object rgCatchTrayBlock: TRadioGroup
            Left = 4
            Top = 144
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use CatchTrayBlock '
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgTrayRobot: TRadioGroup
            Left = 4
            Top = 64
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Use Tray Robot'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install (For HT-9046LM)')
            ParentFont = False
            TabOrder = 1
          end
          object rgMultileEmpty: TRadioGroup
            Left = 4
            Top = 524
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Multile Empty'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object rgLoaderHinge: TRadioGroup
            Left = 4
            Top = 204
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Loader Hinge'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
          object rg2ndLoader: TRadioGroup
            Left = 4
            Top = 644
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use 2nd Loader'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgAuto3Magazine: TRadioGroup
            Left = 4
            Top = 584
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Auto 3 Magazine'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 7
          end
          object rgOHTSystem: TRadioGroup
            Left = 4
            Top = 464
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use OHT System'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 8
          end
          object rgMRSystem: TRadioGroup
            Left = 4
            Top = 384
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Use MR System'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install MR'
              'Install LM')
            ParentFont = False
            TabOrder = 9
          end
        end
        object pnlLoader4: TPanel
          Left = 675
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 3
          object rgAutoTrackCanGoRear: TRadioGroup
            Left = 4
            Top = 514
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Auto Can Go Rear'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object gbART: TGroupBox
            Left = 4
            Top = 4
            Width = 217
            Height = 510
            Align = alTop
            Caption = 'Auto Retest (ART)'
            TabOrder = 0
            object rgInstallAutoRestest: TRadioGroup
              Left = 2
              Top = 22
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'ART Function'
              Columns = 2
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              TabOrder = 0
            end
            object rgAuto1ART: TRadioGroup
              Left = 2
              Top = 82
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'AUTO 1 ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 1
            end
            object rgAuto2ART: TRadioGroup
              Left = 2
              Top = 142
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'AUTO 2 ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 2
            end
            object rgAuto3ART: TRadioGroup
              Left = 2
              Top = 202
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'AUTO 3 ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 3
            end
            object rgInstallARTtwocylinder: TRadioGroup
              Left = 2
              Top = 442
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'ART Z Two Cylinder'
              Columns = 2
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              TabOrder = 7
            end
            object rgAuto6ART: TRadioGroup
              Left = 2
              Top = 382
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'AUTO 6 ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 6
            end
            object rgAuto5ART: TRadioGroup
              Left = 2
              Top = 322
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'rgAuto5ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 5
            end
            object rgAuto4ART: TRadioGroup
              Left = 2
              Top = 262
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'AUTO 4 ART'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clPurple
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 4
            end
          end
          object grpTrayYUseMot: TGroupBox
            Left = 4
            Top = 574
            Width = 217
            Height = 152
            Align = alTop
            Caption = 'Tray Y Use Motor'
            TabOrder = 2
            object chkLoaderY: TCheckBox
              Left = 12
              Top = 24
              Width = 125
              Height = 17
              Caption = 'Loader Track'
              TabOrder = 0
            end
            object chkEmptyY: TCheckBox
              Left = 12
              Top = 45
              Width = 125
              Height = 17
              Caption = 'Empty Track'
              TabOrder = 1
            end
            object chkColorY: TCheckBox
              Left = 12
              Top = 66
              Width = 125
              Height = 17
              Caption = 'Color Track'
              TabOrder = 2
            end
            object chkAuto1Y: TCheckBox
              Left = 12
              Top = 87
              Width = 125
              Height = 17
              Caption = 'Auto1 Track'
              TabOrder = 3
            end
            object chkAuto2Y: TCheckBox
              Left = 12
              Top = 108
              Width = 125
              Height = 17
              Caption = 'Auto2 Track'
              TabOrder = 4
            end
            object chkAuto3Y: TCheckBox
              Left = 12
              Top = 129
              Width = 125
              Height = 17
              Caption = 'Auto3 Track'
              TabOrder = 5
            end
          end
        end
        object pnlLoader5: TPanel
          Left = 900
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 4
          object rgTrayMappingGrabImage: TRadioGroup
            Left = 4
            Top = 105
            Width = 217
            Height = 89
            Align = alTop
            Caption = 'Tray Mapping Grab Image'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Normal'
              'One Times'
              'Customer Plug-in')
            ParentFont = False
            TabOrder = 0
          end
          object rgLoaderKeyence: TRadioGroup
            Left = 4
            Top = 295
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Loader Tray ID KEYENCE'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgMultileEmptyTrayIDKeyence: TRadioGroup
            Left = 4
            Top = 355
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Multile Tray ID KEYENCE'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgEmptyKeyence: TRadioGroup
            Left = 4
            Top = 415
            Width = 217
            Height = 111
            Align = alTop
            Caption = 'Unloader Cover Tray ID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install(Auto 2)'
              'Install(Empty)'
              'Install(Color)')
            ParentFont = False
            TabOrder = 3
          end
          object rgRFIDSystem: TRadioGroup
            Left = 4
            Top = 586
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use RFID System'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'ASLENT'
              'BRILLIAN')
            ParentFont = False
            TabOrder = 4
          end
          object rgDoubleBeltMode: TRadioGroup
            Left = 4
            Top = 646
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Double Belt Mode'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object rgCoverTrayID: TRadioGroup
            Left = 4
            Top = 194
            Width = 217
            Height = 101
            Align = alTop
            Caption = 'Cover Tray ID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Keyence'
              'NFC')
            ParentFont = False
            TabOrder = 6
          end
          object rgRFIDReader: TRadioGroup
            Left = 4
            Top = 526
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Manual RFID Reader'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 7
          end
          object rgTrayMapping: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 101
            Align = alTop
            Caption = 'Use Tray Mapping'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install'
              'Device Remain Checking')
            ParentFont = False
            TabOrder = 8
          end
        end
        object pnlLoader6: TPanel
          Left = 1125
          Top = 0
          Width = 210
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 5
        end
      end
      object tsInOutArm: TTabSheet
        Caption = 'In && Out Arm'
        ImageIndex = 5
        object gbRotateKit: TGroupBox
          Left = 996
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          Caption = 'Rotate Kit Setting'
          TabOrder = 4
          object rgRotateKit: TRadioGroup
            Left = 2
            Top = 22
            Width = 221
            Height = 60
            Align = alTop
            Caption = 'Rotate Kit'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Uninstall'
              'Install')
            ParentFont = False
            TabOrder = 0
          end
          object rgRotateKitIn: TRadioGroup
            Left = 2
            Top = 222
            Width = 221
            Height = 60
            Align = alTop
            Caption = 'Input Position'
            Columns = 2
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Hot Plate 1'
              'Hot Plate 2')
            ParentFont = False
            TabOrder = 2
          end
          object rgRotateKitOut: TRadioGroup
            Left = 2
            Top = 282
            Width = 221
            Height = 80
            Align = alTop
            Caption = 'Output Position'
            Enabled = False
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              'Auto 1'
              'Auto 2'
              'Auto 3')
            ParentFont = False
            TabOrder = 3
          end
          object rgRotateKit_Type: TRadioGroup
            Left = 2
            Top = 82
            Width = 221
            Height = 140
            Align = alTop
            Caption = 'Rotate Kit Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Cylinder'
              '1 Motor (1 to 8)'
              '4 Motor (2x2)'
              '8 Motor (2x4)'
              '1 Motor 1 Dut (1x1)'
              '2 Motor 2 Dut (1x2)'
              'In/Out Arm 1 Motor')
            ParentFont = False
            TabOrder = 1
            OnClick = rgRotateKit_TypeClick
          end
        end
        object pnlInArm1: TPanel
          Left = 0
          Top = 0
          Width = 273
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object grpInOutArmYPitch: TGroupBox
            Left = 4
            Top = 204
            Width = 265
            Height = 585
            Align = alTop
            Caption = 'In && Out Arm XY-Pitch'
            TabOrder = 2
            object lblMinYPitch: TLabel
              Left = 8
              Top = 463
              Width = 79
              Height = 20
              Caption = 'Min Y Pitch'
            end
            object lblMaxYPitch: TLabel
              Left = 8
              Top = 496
              Width = 83
              Height = 20
              Caption = 'Max Y Pitch'
            end
            object lblXPitchMin: TLabel
              Left = 8
              Top = 396
              Width = 79
              Height = 20
              Caption = 'Min X Pitch'
            end
            object lblXPitchMax: TLabel
              Left = 8
              Top = 429
              Width = 83
              Height = 20
              Caption = 'Max X Pitch'
            end
            object Label35: TLabel
              Left = 8
              Top = 528
              Width = 96
              Height = 20
              Caption = 'Base X to HP'
            end
            object rgInOutArmYPitch: TRadioGroup
              Left = 2
              Top = 142
              Width = 261
              Height = 240
              Align = alTop
              Caption = 'Input Arm Y Pitch Module'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBackground
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 6
              Items.Strings = (
                'Fixed Y Pitch 60 mm'
                'Manual Y Pitch 60 && 63.5 mm'
                'Auto Y Pitch In Bc / Out Bb'
                'Manual Y Pitch 60 && 36mm'
                'Auto Y Pitch in Row A'
                'Auto Y Pitch 16 Picker @ Bd'
                'Auto Y Pitch @ Bb (HT7080)'
                'Auto Y Pitch 16 @ In Bd / Out Be'
                'Auto Y Pitch In Bb / Out Bc')
              ParentFont = False
              TabOrder = 1
            end
            object edtMinYPitch: TEdit
              Left = 112
              Top = 459
              Width = 121
              Height = 28
              TabOrder = 4
              Text = '2000'
              OnClick = edtMinYPitchClick
            end
            object edtMaxYPitch: TEdit
              Left = 112
              Top = 492
              Width = 121
              Height = 28
              TabOrder = 5
              Text = '6000'
              OnClick = edtMaxYPitchClick
            end
            object rgInOutArmXPitch: TRadioGroup
              Left = 2
              Top = 22
              Width = 261
              Height = 120
              Align = alTop
              Caption = 'X Pitch Modular'
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBackground
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 2
              Items.Strings = (
                '40 to 120 mm'
                '40 to 150 mm'
                'Manual Key In'
                '22 to 154 mm')
              ParentFont = False
              TabOrder = 0
            end
            object edtMinXPitch: TEdit
              Left = 112
              Top = 392
              Width = 121
              Height = 28
              TabOrder = 2
              Text = '6000'
              OnClick = edtMinXPitchClick
            end
            object edtMaxXPitch: TEdit
              Left = 112
              Top = 425
              Width = 121
              Height = 28
              TabOrder = 3
              Text = '12000'
              OnClick = edtMaxXPitchClick
            end
            object edtHPLimit: TEdit
              Left = 112
              Top = 524
              Width = 121
              Height = 28
              TabOrder = 6
              Text = '7000'
              OnClick = edtMaxYPitchClick
            end
          end
          object ArmZAtoZH: TRadioGroup
            Left = 4
            Top = 4
            Width = 265
            Height = 80
            Align = alTop
            Caption = 'In/Out Arm Z axis Control'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Cylinder'
              'Motor'
              'Motor + Cylinder')
            ParentFont = False
            TabOrder = 0
          end
          object rgPickerCount: TRadioGroup
            Left = 4
            Top = 84
            Width = 265
            Height = 120
            Align = alTop
            Caption = 'Picker Count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              '4 Picker'
              '8 Picker'
              '2 Picker (A, G)'
              '16 Picker'
              '1 Picker')
            ParentFont = False
            TabOrder = 1
          end
        end
        object pnlInArm2: TPanel
          Left = 273
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object rgLaserDistance: TRadioGroup
            Left = 4
            Top = 158
            Width = 217
            Height = 56
            Align = alTop
            Caption = 'Laser'#28204#36317
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install (Hot Plate + Shuttle)')
            ParentFont = False
            TabOrder = 2
          end
          object rgOCR: TRadioGroup
            Left = 4
            Top = 214
            Width = 217
            Height = 76
            Align = alTop
            Caption = 'OCR Function'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install in Handler'
              'Install in OCR IPC')
            ParentFont = False
            TabOrder = 3
          end
          object rgDeviceFlipper: TRadioGroup
            Left = 4
            Top = 586
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Device Flipper'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object rgPreciser: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 77
            Align = alTop
            Caption = 'Preciser'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Uninstall'
              'Install with Motor'
              'Install without Motor')
            ParentFont = False
            TabOrder = 0
          end
          object rgPreciserPos: TRadioGroup
            Left = 4
            Top = 81
            Width = 217
            Height = 77
            Align = alTop
            Caption = 'Preciser Install Area'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Normal'
              'Between Shuttle'
              'Hotplate')
            ParentFont = False
            TabOrder = 1
          end
          object rgDieClean: TRadioGroup
            Left = 4
            Top = 290
            Width = 217
            Height = 56
            Align = alTop
            Caption = 'Use Die Clean'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 8
          end
          object rgOutArmYPitch: TRadioGroup
            Left = 4
            Top = 346
            Width = 217
            Height = 240
            Align = alTop
            Caption = 'Output Arm Y Pitch Module'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 6
            Items.Strings = (
              'Fixed Y Pitch 60 mm'
              'Manual Y Pitch 60 && 63.5 mm'
              'Auto Y Pitch In Bc / Out Bb'
              'Manual Y Pitch 60 && 36mm'
              'Auto Y Pitch in Row A'
              'Auto Y Pitch 16 Picker @ Bd'
              'Auto Y Pitch @ Bb (HT7080)'
              'Auto Y Pitch 16 @ In Bd / Out Be'
              'Auto Y Pitch In Bb / Out Bc')
            ParentFont = False
            TabOrder = 4
          end
          object rgFixAICCD: TRadioGroup
            Left = 4
            Top = 646
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'USE Fix2 AI CCD'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgCCDAutoAlignmentMode: TRadioGroup
            Left = 4
            Top = 706
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'CCD Auto Alignment'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clFuchsia
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 7
          end
        end
        object pnlInArm3: TPanel
          Left = 498
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object rgHotPlatePos: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Hot Plate Pin Position'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Left'
              'Right (New)')
            ParentFont = False
            TabOrder = 0
          end
          object rgHotplateType: TRadioGroup
            Left = 4
            Top = 64
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Hotplate Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '1.5 X ( HT9045W )'
              '2.0 X ( HT9045WS )'
              '1.0 X (HT9046LA)')
            ParentFont = False
            TabOrder = 1
          end
          object rgHotPlateLimit: TRadioGroup
            Left = 4
            Top = 144
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Hot Plate Limitation'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Old'
              'New (For HT-9046LA)')
            ParentFont = False
            TabOrder = 2
          end
          object rgMagneticScale: TRadioGroup
            Left = 4
            Top = 224
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Magnetic Scale'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install (In / Out XY)')
            ParentFont = False
            TabOrder = 3
          end
          object rgInPlacement: TRadioGroup
            Left = 4
            Top = 304
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'InArmPlacement'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 4
          end
        end
        object pnlInArm4: TPanel
          Left = 723
          Top = 0
          Width = 273
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 3
          object grpOutSortArm: TGroupBox
            Left = 4
            Top = 4
            Width = 265
            Height = 189
            Align = alTop
            Caption = 'Out Sort Arm'
            TabOrder = 0
            object lblOutSortXPitchMin: TLabel
              Left = 20
              Top = 119
              Width = 79
              Height = 20
              Caption = 'Min X Pitch'
            end
            object lblOutSortXPitchMax: TLabel
              Left = 20
              Top = 152
              Width = 83
              Height = 20
              Caption = 'Max X Pitch'
            end
            object edtOutSortXPitchMin: TEdit
              Left = 124
              Top = 115
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '1500'
              OnClick = edtMinXPitchClick
            end
            object edtOutSortXPitchMax: TEdit
              Left = 124
              Top = 148
              Width = 121
              Height = 28
              TabOrder = 2
              Text = '4000'
              OnClick = edtMaxXPitchClick
            end
            object rgOutSortArm: TRadioGroup
              Left = 2
              Top = 22
              Width = 261
              Height = 60
              Align = alTop
              Caption = 'Picker Count'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 1
              Items.Strings = (
                'Uninstall'
                '2 Picker')
              ParentFont = False
              TabOrder = 0
            end
          end
        end
      end
      object TabSheet4: TTabSheet
        Caption = 'Index Items'
        ImageIndex = 3
        object pnlIndex4: TPanel
          Left = 675
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 3
          object rgUse4DUT: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Dut Heater Count'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              '1 EA'
              '4 EA'
              '2 EA')
            ParentFont = False
            TabOrder = 0
          end
          object rgOTDInstall: TRadioGroup
            Left = 4
            Top = 264
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'One Touch Docking'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Type 0 (2 Sensor)'
              'Install Software control'
              'Install Hardware control')
            ParentFont = False
            TabOrder = 4
          end
          object rgESDTemp: TRadioGroup
            Left = 4
            Top = 204
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Index ESD Temp'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
          object rgATCHeatGun: TRadioGroup
            Left = 4
            Top = 424
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'ATC Heat Gun'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgAirConditioner: TRadioGroup
            Left = 4
            Top = 344
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Air Conditioner'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install'
              'Install with RS232')
            ParentFont = False
            TabOrder = 5
          end
          object rgLBTemp: TRadioGroup
            Left = 4
            Top = 84
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'L/B Temp'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgLBTemp2: TRadioGroup
            Left = 4
            Top = 144
            Width = 217
            Height = 60
            Align = alTop
            Caption = '2 L/B Temp'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnlIndex3: TPanel
          Left = 450
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object rgIOChangeToque: TRadioGroup
            Left = 4
            Top = 384
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'I/O Change Toque'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgIndexMotorType: TRadioGroup
            Left = 4
            Top = 244
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Index Motor Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Panasonic A4'
              'Mitsubishi'
              'Panasonic A5')
            ParentFont = False
            TabOrder = 4
          end
          object rgSocketClamp: TRadioGroup
            Left = 4
            Top = 324
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'SLK and Socket Clamp'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object rgRealTimeCCD: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'RTC Temp.'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 0
          end
          object rgRealTimeCCDTempNum: TRadioGroup
            Left = 4
            Top = 64
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'RTC Temp No'
            Columns = 3
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              '0'
              '1'
              '2')
            ParentFont = False
            TabOrder = 1
          end
          object rgIndexSuckerType: TRadioGroup
            Left = 4
            Top = 184
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Index Vacuum Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Positive  (+)'
              'Negative (-)')
            ParentFont = False
            TabOrder = 3
          end
          object rgCleanAir: TRadioGroup
            Left = 4
            Top = 124
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Clean Air'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgFinePitch: TRadioGroup
            Left = 4
            Top = 444
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Fine Pitch'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 7
          end
          object rgIndexMotorAxis: TRadioGroup
            Left = 4
            Top = 504
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Index Arm Motor Axis'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Use 4 Axis'
              'Use 3 Axis')
            ParentFont = False
            TabOrder = 8
          end
        end
        object pnlIndex1: TPanel
          Left = 0
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object Label26: TLabel
            Left = -1
            Top = 685
            Width = 119
            Height = 20
            Caption = 'Ind. EP Count'#65306' '
          end
          object rgIndexPressType: TRadioGroup
            Left = 4
            Top = 321
            Width = 217
            Height = 185
            Align = alTop
            Caption = 'Index Pressure Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 6
            Items.Strings = (
              'Normal'
              '240KG'
              '120KG'
              '500KG'
              '260KG'
              '400KG'
              '360KG'
              '160KG'
              '640KG'
              '800KG')
            ParentFont = False
            TabOrder = 3
          end
          object gbEPReadSetting: TGroupBox
            Left = 4
            Top = 241
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'EP Setting for Feedback'
            TabOrder = 2
            object Label12: TLabel
              Left = 8
              Top = 23
              Width = 71
              Height = 20
              Caption = 'Max VDC:'
            end
            object Label16: TLabel
              Left = 8
              Top = 53
              Width = 75
              Height = 20
              Caption = 'Min VDC : '
            end
            object edMaxMpaFB: TEdit
              Left = 88
              Top = 19
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '5.013'
              OnClick = edMaxMpaFBClick
            end
            object edtMinMpaFB: TEdit
              Left = 88
              Top = 49
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '0.968'
              OnClick = edMaxMpaFBClick
            end
          end
          object rgDoubleEPControl: TRadioGroup
            Left = 4
            Top = 566
            Width = 217
            Height = 99
            Align = alTop
            Caption = 'Dual EP Control'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Normal'
              'Indivial EP'
              'Multi EP (Max Qual Site)')
            ParentFont = False
            TabOrder = 5
          end
          object ElectronPressure: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 157
            Align = alTop
            Caption = 'Electron Pressure'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              'Un Install'
              'Install (Analog)'
              'Install (Digital)'
              'Install (Digital and Encoding)'
              'PISO ET-7226'
              'Two EP')
            ParentFont = False
            TabOrder = 0
          end
          object rgWeightCali: TRadioGroup
            Left = 4
            Top = 506
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Weight Calibration'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'Old Method'
              'New Method')
            TabOrder = 4
          end
          object gbEPSetting: TGroupBox
            Left = 4
            Top = 161
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'EP Setting for Output'
            TabOrder = 1
            object Label11: TLabel
              Left = 8
              Top = 23
              Width = 72
              Height = 20
              Caption = 'Max KPA: '
            end
            object Label13: TLabel
              Left = 8
              Top = 53
              Width = 75
              Height = 20
              Caption = 'Min MPA : '
            end
            object edMaxKpa: TEdit
              Left = 88
              Top = 19
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '499'
              OnClick = edMaxKpaClick
            end
            object edMinMpa: TEdit
              Left = 88
              Top = 49
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '0.001'
              OnClick = edMinMpaClick
            end
          end
          object cbIndEPCnt: TComboBox
            Left = 122
            Top = 679
            Width = 91
            Height = 28
            ItemHeight = 20
            TabOrder = 6
            Text = '16'
            Items.Strings = (
              '16'
              '4'
              '8')
          end
        end
        object pnlIndex2: TPanel
          Left = 225
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object rgATC: TRadioGroup
            Left = 4
            Top = 349
            Width = 217
            Height = 204
            Align = alTop
            Caption = 'ATC Mode'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              'Un Install'
              'ATC 6.0'
              'ATC 3.0 (Air Cooling)'
              'Silicon Type'
              'HonPrec Type'
              'No Chamber'
              'NewATCSystem'
              'WinWay')
            ParentFont = False
            TabOrder = 2
          end
          object rgHeaterType: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 147
            Align = alTop
            Caption = 'Heater Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 3
            Items.Strings = (
              'TC401  '
              'Panasonic KT4H'
              'Omron E5DC'
              'No Heater'
              'DTK4848')
            ParentFont = False
            TabOrder = 0
            OnClick = rgHeaterTypeClick
          end
          object GroupBox4: TGroupBox
            Left = 4
            Top = 553
            Width = 217
            Height = 120
            Align = alTop
            Caption = 'ATC Setting'
            TabOrder = 3
            object lbATCSystemIP: TLabel
              Left = 8
              Top = 27
              Width = 27
              Height = 20
              Caption = 'IP : '
            end
            object Label15: TLabel
              Left = 8
              Top = 55
              Width = 41
              Height = 20
              Caption = 'Port : '
            end
            object Label14: TLabel
              Left = 6
              Top = 87
              Width = 80
              Height = 20
              Caption = 'Use Heat : '
            end
            object edATCSystemIP: TEdit
              Left = 88
              Top = 23
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '172.16.8.90'
            end
            object edATCSystemPort: TEdit
              Left = 88
              Top = 51
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '5001'
              OnClick = edATCSystemPortClick
            end
            object edATCSystemUseHeat: TEdit
              Left = 88
              Top = 83
              Width = 121
              Height = 28
              TabOrder = 2
              Text = '4'
              OnClick = edATCSystemUseHeatClick
            end
          end
          object rgHeater: TRadioGroup
            Left = 4
            Top = 151
            Width = 217
            Height = 198
            Align = alTop
            Caption = 'Index Heater Counts'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clGreen
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '4 Heaters'
              '16 Heaters'
              '16 Heaters with Omron EJ1N'
              '32 Heaters with Omron EJ1N'
              '32 Heaters with KT4H'
              '16 Heaters with DTME08'
              '32 Heaters with DTME08')
            ParentFont = False
            TabOrder = 1
          end
          object rgATCMixMode: TRadioGroup
            Left = 4
            Top = 673
            Width = 217
            Height = 64
            Align = alTop
            Caption = 'ATC Mix mode'
            Columns = 2
            ItemIndex = 0
            Items.Strings = (
              'None'
              'ATC3.3+6.0')
            TabOrder = 4
          end
        end
        object pnlIndex5: TPanel
          Left = 900
          Top = 0
          Width = 362
          Height = 823
          Align = alClient
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 4
          object rgHighTempLimit: TRadioGroup
            Left = 4
            Top = 4
            Width = 354
            Height = 105
            Align = alTop
            Caption = 'Handler Temperature Limitation'
            Columns = 3
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '130'
              '150'
              '155'
              '175'
              '200')
            ParentFont = False
            TabOrder = 0
          end
          object grpContactHeight: TGroupBox
            Left = 4
            Top = 429
            Width = 354
            Height = 80
            Align = alTop
            Caption = 'User define max contact height'
            TabOrder = 4
            object lblContactHeight: TLabel
              Left = 136
              Top = 48
              Width = 26
              Height = 20
              Caption = 'mm'
            end
            object chkUserDefMaxContactHeight: TCheckBox
              Left = 8
              Top = 24
              Width = 280
              Height = 17
              Caption = 'User define maximum contact height'
              TabOrder = 0
            end
            object edtUserDefMaxContactHeight: TEdit
              Left = 8
              Top = 45
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '-135.0'
              OnClick = edtUserDefMaxContactHeightClick
            end
          end
          object GroupBox8: TGroupBox
            Left = 4
            Top = 189
            Width = 354
            Height = 160
            Align = alTop
            Caption = 'Hot Gun Flow'
            TabOrder = 2
            object Label17: TLabel
              Left = 220
              Top = 95
              Width = 54
              Height = 20
              Caption = 'LineNo:'
            end
            object Label18: TLabel
              Left = 222
              Top = 127
              Width = 52
              Height = 20
              Caption = 'DevNo:'
            end
            object Label19: TLabel
              Left = 20
              Top = 95
              Width = 131
              Height = 20
              Caption = 'Gun 1 ChannelNo:'
            end
            object Label20: TLabel
              Left = 20
              Top = 127
              Width = 131
              Height = 20
              Caption = 'Gun 2 ChannelNo:'
            end
            object edHotGunFlow_LineNo: TEdit
              Left = 279
              Top = 91
              Width = 50
              Height = 28
              TabOrder = 2
              Text = '0'
              OnClick = edHotGunFlow_Gun1_ChannelNoClick
            end
            object edHotGunFlow_DevNo: TEdit
              Left = 279
              Top = 123
              Width = 50
              Height = 28
              TabOrder = 4
              Text = '0'
              OnClick = edHotGunFlow_DevNoClick
            end
            object edHotGunFlow_Gun1_ChannelNo: TEdit
              Left = 158
              Top = 91
              Width = 50
              Height = 28
              TabOrder = 1
              Text = '0'
              OnClick = edHotGunFlow_Gun1_ChannelNoClick
            end
            object edHotGunFlow_Gun2_ChannelNo: TEdit
              Left = 158
              Top = 123
              Width = 50
              Height = 28
              TabOrder = 3
              Text = '0'
              OnClick = edHotGunFlow_Gun1_ChannelNoClick
            end
            object rgHotGunFlow: TRadioGroup
              Left = 2
              Top = 22
              Width = 350
              Height = 60
              Align = alTop
              Caption = 'Hot Gun Flow'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 0
            end
          end
          object rgHeatGun: TRadioGroup
            Left = 4
            Top = 109
            Width = 354
            Height = 80
            Align = alTop
            Caption = 'Heat Gun'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install'
              'Install with CDA Function')
            ParentFont = False
            TabOrder = 1
          end
          object rgDewpointHW: TRadioGroup
            Left = 4
            Top = 509
            Width = 354
            Height = 80
            Align = alTop
            Caption = 'Dew Point Detect'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'CAEL-DPA-136-2SA(-60~+60)'
              'Tekhne TK-80 TR(-80~+20)')
            ParentFont = False
            TabOrder = 5
          end
          object grpIndexZSafePos: TGroupBox
            Left = 4
            Top = 349
            Width = 354
            Height = 80
            Align = alTop
            Caption = 'User Define Index Z SafePos'
            TabOrder = 3
            object lblIndexHeight: TLabel
              Left = 136
              Top = 48
              Width = 26
              Height = 20
              Caption = 'mm'
            end
            object chkUser_Define_IndexZ_SafePos: TCheckBox
              Left = 8
              Top = 24
              Width = 280
              Height = 17
              Caption = 'User Define Index Z SafePos Enable'
              TabOrder = 0
            end
            object edtUserDefineIndexZSafePos: TEdit
              Left = 8
              Top = 45
              Width = 121
              Height = 28
              TabOrder = 1
              Text = '200'
              OnClick = edtUserDefineIndexZSafePosClick
            end
          end
        end
      end
      object tsShuttle: TTabSheet
        Caption = 'Shuttle'
        ImageIndex = 7
        object pnlSht3: TPanel
          Left = 625
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object rgShuttleVibration: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Shuttle Vibration'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 0
          end
          object rgUseOutSht: TRadioGroup
            Left = 4
            Top = 64
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Use Individual Out Shuttle'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgNUECType: TRadioGroup
            Left = 4
            Top = 124
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'NU-EC Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clRed
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'NU-EC1'
              'NU-EC1A')
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnlSht2: TPanel
          Left = 400
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object rg2DBarcode: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 160
            Align = alTop
            Caption = '2DID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Out Shuttle'
              'In Shuttle (RS232)'
              'USE CCD'
              'In Shuttle (Ethernet)'
              'USE OCR')
            ParentFont = False
            TabOrder = 0
          end
          object rgShuttleFloating: TRadioGroup
            Left = 4
            Top = 364
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Shuttle Floating Detection'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Use CCD')
            ParentFont = False
            TabOrder = 4
          end
          object rgBottom2DID: TRadioGroup
            Left = 4
            Top = 304
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Bottom 2DID'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
          object grpCognexCCD: TGroupBox
            Left = 4
            Top = 224
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Cognex CCD  Setting'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            object lblCognexCCD: TLabel
              Left = 6
              Top = 33
              Width = 79
              Height = 20
              Caption = 'Use CCD : '
            end
            object edCognexSystemCCD: TEdit
              Left = 88
              Top = 29
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '2'
              OnClick = edCognexSystemCCDClick
            end
          end
          object rgCCDTemp: TRadioGroup
            Left = 4
            Top = 164
            Width = 217
            Height = 60
            Align = alTop
            Caption = '2DID Temp.'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgBottom2DID_CCD: TRadioGroup
            Left = 4
            Top = 424
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Bottom 2DID CCD'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '4 CCD'
              '8 CCD')
            ParentFont = False
            TabOrder = 5
          end
        end
        object pnlSht1: TPanel
          Left = 0
          Top = 0
          Width = 400
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object rgShuttleSensor: TRadioGroup
            Left = 4
            Top = 4
            Width = 392
            Height = 240
            Align = alTop
            Caption = 'Shuttle Sensor Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 4
            Items.Strings = (
              'Takex Sensor'
              'Line Sensor with Amplifier'
              'Line Sensor with CC-Link'
              'Line Sensor with CC-Link for 3 NU-CL1'
              'Line Sensor with CanBus'
              'Line Sensor with CanBus for 3 NU-DN1'
              'Line Sensor with EtherCAT'
              'Line Sensor with EtherCAT for 3 NU-EC1')
            ParentFont = False
            TabOrder = 0
          end
          object rgShuttleZType: TRadioGroup
            Left = 4
            Top = 244
            Width = 392
            Height = 60
            Align = alTop
            Caption = 'Shuttle Z Sensor Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clMaroon
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Pitch Min : 50mm'
              'Pitch Min : 23mm')
            ParentFont = False
            TabOrder = 1
          end
          object rgShuttleCrossSensor: TRadioGroup
            Left = 4
            Top = 304
            Width = 392
            Height = 60
            Align = alTop
            Caption = 'Shuttle Cross Sensor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgAutoShuttleSensor: TRadioGroup
            Left = 4
            Top = 364
            Width = 392
            Height = 60
            Align = alTop
            Caption = 'Auto Shuttle Sensor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
          object rgInShtLastSensor: TRadioGroup
            Left = 4
            Top = 424
            Width = 392
            Height = 60
            Align = alTop
            Caption = 'In shuttle last sensor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              '45-->7 / 46-->9'
              '45-->9 / 46-->9')
            ParentFont = False
            TabOrder = 4
          end
          object Panel4: TPanel
            Left = 4
            Top = 484
            Width = 392
            Height = 137
            Align = alTop
            BevelOuter = bvNone
            TabOrder = 5
            object cbEnableOutShtSensor: TCheckBox
              Left = 7
              Top = 8
              Width = 345
              Height = 17
              Caption = 'Enable out shuttle rear to front sensor'
              TabOrder = 0
            end
            object chkOutShtYSensorByLatch: TCheckBox
              Left = 7
              Top = 36
              Width = 285
              Height = 17
              Caption = 'Use out shuttle Y sensor by Latch'
              TabOrder = 1
            end
            object chk2x3modeUseAxisYSensor: TCheckBox
              Left = 7
              Top = 64
              Width = 285
              Height = 19
              Caption = '2x3 mode use axis Y sensor'
              TabOrder = 2
            end
            object chkBiasModeUseYSensor: TCheckBox
              Left = 7
              Top = 92
              Width = 398
              Height = 19
              Caption = 'Off-Center mode use out shuttle Y sensor'
              TabOrder = 3
            end
          end
          object rgInShtAutoLatch: TRadioGroup
            Left = 4
            Top = 621
            Width = 392
            Height = 80
            Align = alTop
            Caption = 'In Shuttle Auto Latch'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
        end
      end
      object TabSheet3: TTabSheet
        Caption = 'Other Items'
        ImageIndex = 2
        object grpOther: TGroupBox
          Left = 450
          Top = 0
          Width = 400
          Height = 823
          Align = alLeft
          Caption = 'Socket/ Rotate/ Color sensor with CC-link/Canbus'
          TabOrder = 2
          DesignSize = (
            400
            823)
          object Label22: TLabel
            Left = 18
            Top = 194
            Width = 202
            Height = 20
            Alignment = taRightJustify
            Anchors = [akLeft, akTop, akRight, akBottom]
            Caption = 'Socket sensor amplifier qty : '
          end
          object Label23: TLabel
            Left = 19
            Top = 226
            Width = 201
            Height = 20
            Alignment = taRightJustify
            Anchors = [akLeft, akTop, akRight, akBottom]
            Caption = 'Rotate sensor amplifier qty : '
          end
          object Label24: TLabel
            Left = 31
            Top = 258
            Width = 189
            Height = 20
            Alignment = taRightJustify
            Anchors = [akLeft, akTop, akRight, akBottom]
            Caption = 'Color sensor amplifier qty : '
          end
          object Label37: TLabel
            Left = 11
            Top = 354
            Width = 233
            Height = 20
            Alignment = taRightJustify
            Anchors = [akLeft, akTop, akRight, akBottom]
            Caption = '2nd Socket sensor amplifier qty : '
          end
          object lab3rdSocketSensor: TLabel
            Left = 15
            Top = 382
            Width = 229
            Height = 20
            Alignment = taRightJustify
            Anchors = [akLeft, akTop, akRight, akBottom]
            Caption = '3rd Socket sensor amplifier qty : '
          end
          object cbSocketSenAmpCnt: TComboBox
            Left = 232
            Top = 190
            Width = 145
            Height = 28
            ItemHeight = 20
            TabOrder = 2
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
              '15'
              '16')
          end
          object cbRotateSenAmpCnt: TComboBox
            Left = 232
            Top = 222
            Width = 145
            Height = 28
            ItemHeight = 20
            TabOrder = 3
            Text = '0'
            Items.Strings = (
              '0'
              '1'
              '2'
              '3'
              '4')
          end
          object cbColorSenAmpCnt: TComboBox
            Left = 232
            Top = 254
            Width = 145
            Height = 28
            ItemHeight = 20
            TabOrder = 4
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
              '8')
          end
          object rgSocketSen: TRadioGroup
            Left = 2
            Top = 22
            Width = 396
            Height = 60
            Align = alTop
            Caption = 'Socket sensror'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 0
          end
          object rgColorSensor: TRadioGroup
            Left = 2
            Top = 82
            Width = 396
            Height = 60
            Align = alTop
            Caption = 'Use Color Tray Sensor'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object grpCanBus: TGroupBox
            Left = 2
            Top = 515
            Width = 396
            Height = 306
            Align = alBottom
            Caption = 'CanBus Setting'
            TabOrder = 6
            DesignSize = (
              396
              306)
            object Label27: TLabel
              Left = 16
              Top = 98
              Width = 240
              Height = 96
              Alignment = taRightJustify
              Anchors = [akLeft, akTop, akRight, akBottom]
              AutoSize = False
              Caption = 'NUDN1 qty : '
            end
            object Label28: TLabel
              Left = 16
              Top = 127
              Width = 240
              Height = 48
              Alignment = taRightJustify
              Anchors = [akLeft, akTop, akRight, akBottom]
              AutoSize = False
              Caption = 'NUDN1 MACID11 amplifier qty : '
            end
            object Label29: TLabel
              Left = 16
              Top = 160
              Width = 240
              Height = 48
              Alignment = taRightJustify
              Anchors = [akLeft, akTop, akRight, akBottom]
              AutoSize = False
              Caption = 'NUDN1 MACID12 amplifier qty : '
            end
            object Label31: TLabel
              Left = 16
              Top = 191
              Width = 240
              Height = 48
              Alignment = taRightJustify
              Anchors = [akLeft, akTop, akRight, akBottom]
              AutoSize = False
              Caption = 'NUDN1 MACID13 amplifier qty : '
            end
            object Label36: TLabel
              Left = 16
              Top = 223
              Width = 240
              Height = 48
              Alignment = taRightJustify
              Anchors = [akLeft, akTop, akRight, akBottom]
              AutoSize = False
              Caption = 'NUDN1 MACID14 amplifier qty : '
            end
            object rgCanBusMethod: TRadioGroup
              Left = 2
              Top = 22
              Width = 392
              Height = 60
              Align = alTop
              Columns = 2
              ItemIndex = 0
              Items.Strings = (
                'Default (old method)'
                'Manual (new method)')
              TabOrder = 0
            end
            object coNudn1Macid11: TComboBox
              Left = 268
              Top = 126
              Width = 100
              Height = 28
              ItemHeight = 20
              ItemIndex = 0
              TabOrder = 2
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
                '15'
                '16')
            end
            object coNudn1Macid12: TComboBox
              Left = 268
              Top = 158
              Width = 100
              Height = 28
              ItemHeight = 20
              ItemIndex = 0
              TabOrder = 3
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
                '15'
                '16')
            end
            object coNudn1Macid13: TComboBox
              Left = 268
              Top = 190
              Width = 100
              Height = 28
              ItemHeight = 20
              ItemIndex = 0
              TabOrder = 4
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
                '15'
                '16')
            end
            object coCanBusNudn1: TComboBox
              Left = 268
              Top = 94
              Width = 100
              Height = 28
              ItemHeight = 20
              TabOrder = 1
              Text = '0'
              Items.Strings = (
                '0'
                '1'
                '2'
                '3'
                '4')
            end
            object coNudn1Macid14: TComboBox
              Left = 268
              Top = 222
              Width = 100
              Height = 28
              ItemHeight = 20
              ItemIndex = 0
              TabOrder = 5
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
                '15'
                '16')
            end
          end
          object cbSocketSenAmpCnt2nd: TComboBox
            Left = 248
            Top = 350
            Width = 145
            Height = 28
            ItemHeight = 20
            TabOrder = 5
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
              '15'
              '16')
          end
          object cbSocketSenAmpCnt3rd: TComboBox
            Left = 247
            Top = 381
            Width = 145
            Height = 28
            ItemHeight = 20
            TabOrder = 7
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
              '15'
              '16')
          end
        end
        object pnlOther1: TPanel
          Left = 0
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object rgSafeDoor: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 120
            Align = alTop
            Caption = 'Safe Door Amount'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              '7 Safe-Doors'
              '8 Safe-Doors'
              '9 Safe-Doors'
              '10 Safe-Doors'
              '7 Safe-Doors for 1032')
            ParentFont = False
            TabOrder = 0
          end
          object rgAOI: TRadioGroup
            Left = 4
            Top = 461
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'AOI'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
          object GroupBox7: TGroupBox
            Left = 4
            Top = 124
            Width = 217
            Height = 80
            Align = alTop
            Caption = '24V Monitor Pulse Count'
            TabOrder = 1
            object ed24VMonitorPulseCount: TEdit
              Left = 48
              Top = 32
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '20'
              OnClick = ed24VMonitorPulseCountClick
            end
          end
          object gbVibrationCommuncation: TGroupBox
            Left = 4
            Top = 204
            Width = 217
            Height = 137
            Align = alTop
            Caption = 'Vibration Communcation'
            TabOrder = 2
            object labVibrateComm: TLabel
              Left = 8
              Top = 104
              Width = 98
              Height = 20
              Caption = 'Vibration card'
            end
            object rgVibrationCommuncation: TRadioGroup
              Left = 2
              Top = 22
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'Vibration Communcation'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clOlive
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un-Install'
                'Install')
              ParentFont = False
              TabOrder = 0
            end
            object cbVibrationCardQty: TComboBox
              Left = 136
              Top = 101
              Width = 49
              Height = 28
              ItemHeight = 20
              TabOrder = 1
              Text = '2'
              Items.Strings = (
                '0'
                '1'
                '2')
            end
          end
          object rgBaseHeaterCount: TRadioGroup
            Left = 4
            Top = 341
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Base Heater Count'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              '6 EA')
            ParentFont = False
            TabOrder = 3
          end
          object rgShuttleFloodgate: TRadioGroup
            Left = 4
            Top = 401
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Shuttle Floodgate'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBackground
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 4
          end
          object rgTopScanner_AOI: TRadioGroup
            Left = 4
            Top = 521
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Top Scanner AOI'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 6
          end
          object rgScanner_AOI: TRadioGroup
            Left = 0
            Top = 588
            Width = 220
            Height = 109
            Caption = 'Scanner AOI'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Bottom AOI'
              'TopBottom AOI')
            ParentFont = False
            TabOrder = 7
          end
        end
        object Panel1: TPanel
          Left = 225
          Top = 0
          Width = 225
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object rgMagBinDispType: TRadioGroup
            Left = 4
            Top = 4
            Width = 217
            Height = 80
            Align = alTop
            Caption = 'Magazine Bin Disp type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clTeal
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 2
            Items.Strings = (
              'Uninstall'
              'HT-A18'
              'HT-BT007+HTBT008'
              'TFT')
            ParentFont = False
            TabOrder = 0
          end
          object grpTriTemperature: TGroupBox
            Left = 4
            Top = 204
            Width = 217
            Height = 425
            Align = alTop
            Caption = 'Tri Temperature '
            TabOrder = 3
            object lbl7: TLabel
              Left = 10
              Top = 151
              Width = 86
              Height = 20
              Caption = 'Max Degree'
            end
            object lbl8: TLabel
              Left = 10
              Top = 184
              Width = 82
              Height = 20
              Caption = 'Min Degree'
            end
            object lbl9: TLabel
              Left = 10
              Top = 217
              Width = 98
              Height = 20
              Caption = 'Total Channel'
            end
            object Label34: TLabel
              Left = 10
              Top = 249
              Width = 125
              Height = 20
              Caption = 'Total Compressor'
            end
            object rgTriTempMachine: TRadioGroup
              Left = 2
              Top = 22
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'Tri Temperature mode'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBackground
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install')
              ParentFont = False
              TabOrder = 0
            end
            object rgAirStreamSelect: TRadioGroup
              Left = 2
              Top = 82
              Width = 213
              Height = 60
              Align = alTop
              Caption = 'Air Stream'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 1
              Items.Strings = (
                'Uninstall'
                'Install')
              ParentFont = False
              TabOrder = 1
            end
            object edtTriTemperature_MaxDegree: TEdit
              Left = 140
              Top = 148
              Width = 50
              Height = 28
              TabOrder = 2
              Text = '175'
              OnClick = edtTriTemperature_MaxDegreeClick
            end
            object edtTriTemperature_MinDegree: TEdit
              Left = 140
              Top = 180
              Width = 50
              Height = 28
              TabOrder = 3
              Text = '-55'
              OnClick = edtTriTemperature_MinDegreeClick
            end
            object edtTriTempTotalCh: TEdit
              Left = 140
              Top = 212
              Width = 50
              Height = 28
              TabOrder = 4
              Text = '40'
              OnClick = edtTriTempTotalChClick
            end
            object grp_SetHeaterTemp: TGroupBox
              Left = 16
              Top = 276
              Width = 175
              Height = 137
              Caption = 'Set Heater Temp'
              TabOrder = 6
              object Label25: TLabel
                Left = 6
                Top = 33
                Width = 88
                Height = 20
                Caption = 'Max Out Sht'
              end
              object Label32: TLabel
                Left = 6
                Top = 69
                Width = 72
                Height = 20
                Caption = 'Max Index'
              end
              object Label33: TLabel
                Left = 6
                Top = 105
                Width = 70
                Height = 20
                Caption = 'Max Base'
              end
              object edtOutShtMaxTemp: TEdit
                Left = 121
                Top = 33
                Width = 50
                Height = 28
                TabOrder = 0
                Text = '60'
                OnClick = edtOutShtMaxTempClick
              end
              object edtIndexMaxTemp: TEdit
                Tag = 1
                Left = 121
                Top = 67
                Width = 50
                Height = 28
                TabOrder = 1
                Text = '60'
                OnClick = edtIndexMaxTempClick
              end
              object edtBaseMaxTemp: TEdit
                Tag = 2
                Left = 121
                Top = 101
                Width = 50
                Height = 28
                TabOrder = 2
                Text = '60'
                OnClick = edtOutShtMaxTempClick
              end
            end
            object edt_Total_Compressor: TEdit
              Left = 140
              Top = 244
              Width = 50
              Height = 28
              TabOrder = 5
              Text = '3'
              OnChange = edt_Total_CompressorChange
            end
          end
          object rgAutoFormSize: TRadioGroup
            Left = 4
            Top = 84
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Auto Form Size'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'Uninstall'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rg_IndexDoorHeater: TRadioGroup
            Left = 4
            Top = 144
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'Index Door Heater'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Uninstall'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgE84Sensor: TRadioGroup
            Left = 4
            Top = 629
            Width = 217
            Height = 60
            Align = alTop
            Caption = 'E84  Sensor '
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clOlive
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 4
          end
        end
      end
      object TabSheet5: TTabSheet
        Caption = 'Com Port'
        ImageIndex = 4
        object Label2: TLabel
          Left = 12
          Top = 12
          Width = 93
          Height = 20
          Caption = 'Index Torque'
        end
        object Label3: TLabel
          Left = 12
          Top = 85
          Width = 163
          Height = 20
          Caption = 'Temperature Controller'
        end
        object Label5: TLabel
          Left = 12
          Top = 123
          Width = 225
          Height = 20
          Caption = 'Temperature Controller (Omron)'
        end
        object Label6: TLabel
          Left = 12
          Top = 198
          Width = 45
          Height = 20
          Caption = 'Tester'
        end
        object Label7: TLabel
          Left = 12
          Top = 235
          Width = 78
          Height = 20
          Caption = 'Bin Display'
        end
        object Label8: TLabel
          Left = 12
          Top = 273
          Width = 109
          Height = 20
          Caption = 'Real Time CCD'
        end
        object Label9: TLabel
          Left = 12
          Top = 160
          Width = 176
          Height = 20
          Caption = 'Dynamic Temperature IC'
        end
        object lbl1: TLabel
          Left = 12
          Top = 348
          Width = 44
          Height = 20
          Caption = 'ATC 2'
        end
        object lbl2: TLabel
          Left = 12
          Top = 385
          Width = 44
          Height = 20
          Caption = 'ATC 3'
        end
        object lbl3: TLabel
          Left = 12
          Top = 423
          Width = 44
          Height = 20
          Caption = 'ATC 4'
        end
        object lbl4: TLabel
          Left = 12
          Top = 310
          Width = 44
          Height = 20
          Caption = 'ATC 1'
        end
        object labOCR: TLabel
          Left = 12
          Top = 460
          Width = 35
          Height = 20
          Caption = 'OCR'
        end
        object labOCRwithTester: TLabel
          Left = 12
          Top = 497
          Width = 108
          Height = 20
          Caption = 'OCRwithTester'
        end
        object labAirConditioner: TLabel
          Left = 12
          Top = 533
          Width = 104
          Height = 20
          Caption = 'Air Conditioner'
        end
        object labEMAwarePort1: TLabel
          Left = 508
          Top = 37
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '3M EM Aware Port 1'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEMAwarePort2: TLabel
          Left = 508
          Top = 76
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '3M EM Aware Port 2'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEMAwarePort3: TLabel
          Left = 508
          Top = 114
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '3M EM Aware Port 3'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labEMAwarePort4: TLabel
          Left = 508
          Top = 153
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '3M EM Aware Port 4'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labSimcoION_Novx3360: TLabel
          Left = 508
          Top = 191
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'Simco ION Novx 3360'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Visible = False
        end
        object labLaser1: TLabel
          Left = 508
          Top = 230
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'COM Laser 1'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLaser2: TLabel
          Left = 508
          Top = 268
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'COM Laser 2'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labLaserInArm: TLabel
          Left = 508
          Top = 307
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'COM Laser In Arm'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lab2DReader1: TLabel
          Left = 508
          Top = 345
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '2D Reader Port 1'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lab2DReader2: TLabel
          Left = 508
          Top = 384
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '2D Reader Port 2'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lab2DReader3: TLabel
          Left = 508
          Top = 422
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '2D Reader Port 3'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lab2DReader4: TLabel
          Left = 508
          Top = 461
          Width = 180
          Height = 27
          AutoSize = False
          Caption = '2D Reader Port 4'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblGroundMan: TLabel
          Left = 508
          Top = 499
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'Ground Man'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labKASUGA_Fan: TLabel
          Left = 12
          Top = 574
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'KASUGA_Fan'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblTrayStepMotor: TLabel
          Left = 12
          Top = 610
          Width = 245
          Height = 27
          AutoSize = False
          Caption = 'Step motor of tray && PadInterface'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labTTL1RS232: TLabel
          Left = 508
          Top = 534
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'TTL1 RS232'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object labTTL2RS232: TLabel
          Left = 508
          Top = 574
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'TTL2 RS232'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lblRFIDReader: TLabel
          Left = 508
          Top = 614
          Width = 180
          Height = 27
          AutoSize = False
          Caption = 'RFID Reader'#65306
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbl5: TLabel
          Left = 12
          Top = 651
          Width = 87
          Height = 20
          Caption = 'Bin Display2'
        end
        object cbComIndex: TComboBox
          Left = 260
          Top = 8
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 0
          Text = 'COM11'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComTemp: TComboBox
          Left = 260
          Top = 81
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 5
          Text = 'COM12'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComTempOmron: TComboBox
          Left = 260
          Top = 119
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 7
          Text = 'COM13'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComTester: TComboBox
          Left = 260
          Top = 194
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 11
          Text = 'COM2'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComBinDisp: TComboBox
          Left = 260
          Top = 231
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 13
          Text = 'COM14'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComRTC: TComboBox
          Left = 260
          Top = 269
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 15
          Text = 'COM1'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbComDyTemp: TComboBox
          Left = 260
          Top = 156
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 9
          Text = 'COM2'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbbATC2: TComboBox
          Left = 260
          Top = 344
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 18
          Text = 'COM16'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbbATC3: TComboBox
          Left = 260
          Top = 381
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 20
          Text = 'COM17'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbbATC4: TComboBox
          Left = 260
          Top = 419
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 22
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbbATC1: TComboBox
          Left = 260
          Top = 306
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 16
          Text = 'COM15'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object btnSetATCCom: TButton
          Left = 844
          Top = 536
          Width = 129
          Height = 25
          Caption = 'Set Default'
          TabOrder = 30
          OnClick = btnSetATCComClick
        end
        object cbOCR: TComboBox
          Left = 260
          Top = 456
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 24
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbOCRwithTester: TComboBox
          Left = 260
          Top = 493
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 26
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbAirCon: TComboBox
          Left = 260
          Top = 529
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 28
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
        object cbEMAwarePort1: TComboBox
          Left = 687
          Top = 36
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 2
          Text = 'COM15'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbEMAwarePort2: TComboBox
          Left = 687
          Top = 75
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 4
          Text = 'COM16'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbEMAwarePort3: TComboBox
          Left = 687
          Top = 113
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 6
          Text = 'COM17'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbEMAwarePort4: TComboBox
          Left = 687
          Top = 152
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 8
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbESDUse4COM: TCheckBox
          Left = 508
          Top = 8
          Width = 345
          Height = 17
          Caption = 'Use 4 COM port for 3M EM Aware'
          Checked = True
          State = cbChecked
          TabOrder = 1
        end
        object cbNovx3360: TComboBox
          Left = 687
          Top = 190
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 10
          Text = 'COM18'
          Visible = False
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbComLaser1: TComboBox
          Left = 687
          Top = 229
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 12
          Text = 'COM16'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbComLaser2: TComboBox
          Left = 687
          Top = 267
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 14
          Text = 'COM17'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbComLaserInArm: TComboBox
          Left = 687
          Top = 306
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 17
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cb2DReader1: TComboBox
          Left = 687
          Top = 344
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 19
          Text = 'COM15'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cb2DReader2: TComboBox
          Left = 687
          Top = 383
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 21
          Text = 'COM16'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cb2DReader3: TComboBox
          Left = 687
          Top = 421
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 23
          Text = 'COM17'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cb2DReader4: TComboBox
          Left = 687
          Top = 460
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 25
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbbGroundMan: TComboBox
          Left = 687
          Top = 498
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 27
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbKASUGA_Fan: TComboBox
          Left = 260
          Top = 574
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 32
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbbTrayStepMotor: TComboBox
          Left = 260
          Top = 609
          Width = 145
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 33
          Text = 'COM18'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object chkUseHPComCard: TCheckBox
          Left = 32
          Top = 40
          Width = 261
          Height = 17
          Caption = 'Use HonPrec COM board'
          Checked = True
          State = cbChecked
          TabOrder = 3
        end
        object cbComTTLRS232: TComboBox
          Left = 687
          Top = 533
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ParentFont = False
          TabOrder = 29
          Text = 'COM3'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbComTTLRS232_2: TComboBox
          Left = 687
          Top = 573
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ItemIndex = 7
          ParentFont = False
          TabOrder = 31
          Text = 'COM8'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbbRFIDReader: TComboBox
          Left = 687
          Top = 613
          Width = 150
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ItemHeight = 20
          ItemIndex = 7
          ParentFont = False
          TabOrder = 34
          Text = 'COM8'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19'
            'COM20')
        end
        object cbbComBinDisp2: TComboBox
          Left = 260
          Top = 647
          Width = 145
          Height = 28
          ItemHeight = 20
          TabOrder = 35
          Text = 'COM14'
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
            'COM15'
            'COM16'
            'COM17'
            'COM18'
            'COM19')
        end
      end
      object tsCustomerCode: TTabSheet
        Caption = 'Customer Code'
        ImageIndex = 6
        object rgCustomerList: TRadioGroup
          Left = 0
          Top = 0
          Width = 1262
          Height = 823
          Align = alClient
          Caption = 'Customer Code'
          Columns = 3
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Courier New'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            '000 HonPrec              '#40251#21185
            '731 Carsem_Thai          Carsem '#27888#22283
            '730 IFXTH                IFXTH'#27888#22283
            '740 Ramos                Ramos Technology'
            '741 TechL_Vitenam        TechL-Vitenam'
            '742 TechL_Korea          TechL-Korea'
            '751 Mellanox_Israel      Mellanox '#20197#33394#21015
            '743 SFA_Semicon          SFA_Semicon'
            '763 CC_UTAC_Indonesia    '#32879#28204#31185#25216' '#21360#23612
            '764 Elmos Germany        Elmos'#24503#22283
            '765 Allegro Philippines  Allegro'#33778#24459#36051
            '766 Morningcore          '#23480#33455' '
            '767 Paceis               '#33322#33455#28304' '
            '768 Sigmastar            '#37555#21147' '
            '769 BRAVETEK             '#21338#30332#38651#23376' '
            '770 DJI_SZ               '#22823#30086#21109#26032' '#28145#22323
            '771 JSCC_OS              '#38263#38651#24494#38651#23376
            '772 TYTAN                '#25104#37117#24907#22374
            '773 CAPCON               '#21271#20140#33775#23553
            '780 NXP_TJ               '#24681#26234#28006' '#22825#27941
            '781 XINYUN               '#26477#24030#33455#20113
            '783 SCX                  '#28145#28204#33455
            '787 TAIJI_SEMI           '#22826#26997#21322#23566#39636'('#34311#24030')'
            '790 Forehope_NINGBO      '#29996#30717' '#23527#27874
            '791 SJ_Semiconductor     '#30427#21512#26230#24494
            '792 SJ_Semiconductor_OS  '#30427#21512#26230#24494'Open Short'
            '793 30JAVEE              '#25104#37117#19977#38646#22025#24494#38651#23376
            '794 CAMBRICON            '#19978#28023#23506#27494#32000
            '795 VATE                 '#31435#34907
            '796 ASIAOPTICAL          '#20126#27954#20809#23416
            '797 HXYSEMI              '#27993#27743#33322#28304#33455
            '800 Broadcom US          '#21338#36890' '#32654#22283
            '801 Tessolve_US          Tessolve-US'
            '802 Infineon             '#33521#39131#20940
            '803 TERADYNE_US          '#27888#29790#36948' '#32654#22283
            '804 TeraProbe            TeraProbe '#26085#26412
            '805 TATA                 '#22612#22612'-'#21360#24230
            '806 STM                  '#24847#27861#21322#23566#39636' '#39340#29246#20182
            '809 IMEC                 '#24859#32654#31185
            '810 AVAGO_Korea          '#23433#33775#39640' '#38867#22283
            '812 PTI                  '#21147#25104'3C'#24288
            '813 SFA_Semicon          SFA Semicon'
            '820 TSMC_TAINAN          '#21488#31309#38651' '#21488#21335' '
            '825 TSMC_HSINCHU         '#21488#31309#38651' '#26032#31481
            '828 ChenYuanXiang_CHINA  '#26216#20803#32724' '#35199#23433
            '829 EUROFINS             '#27472#38520
            '830 HTKJXA_CHINA         '#35199#23433' '#22825#27700#33775#22825
            '831 RF360                RF360'
            '832 LBSH                 '#19978#28023#31103#27604
            '833 Realtek              '#29790#26161#21322#23566#39636
            '834 RIGGER_MICRO         '#37165#24030#37555#20625#24494
            '835 XINITECH             '#21271#20140#33455#21147
            '836 GIGA_FORCE_Zhejiang  '#23395#35920' '#27993#27743' '
            '837 GIGA_FORCE_Shanghai  '#23395#35920' '#19978#28023
            '838 CSAMQ                '#38263#27801#23433#29287#27849
            '839 GONGJIN_SHANGHAI     '#19978#28023#20849#36914
            '840 SANDISK_CHINA        '#19978#28023#26032#24093
            '841 GONGJIN_SUZHOU       '#34311#24030#20849#36914
            '842 Mathilda             '#39340#33293#31185#25216
            '843 DoosanTesna          DOOSAN TESNA'
            '844 Renesas Malaysia     Renesas '#39340#20358#35199#20126
            '845 STK                  STK '#26085#26412
            '846 Renesas              Renesas '#26085#26412
            '847 THINE                Thine '#26085#26412
            '848 SINOICTECH           '#19978#28023#33775#23994
            '849 ChipOn               '#19978#28023#33455#26106
            '850 ChipMos_TAINAN       '#21335#33538' '#21488#21335
            '851 ChipMos_ZHUBEI       '#21335#33538' '#31481#21271
            '852 ITS                  '#21109#37327#31185#25216
            '853 NEXPERIA             '#23433#19990' '#24291#26481
            '854 Atec_Semiconductor   '#19978#28023#26107#33406#21322#23566#39636
            '855 JSSI_Semiconductor   '#27743#34311#33455#24503#21322#23566#39636
            '856 Microchip_FR         Microchip '#27861#22283
            '857 Indie_US             Indie '#32654#22283'.'
            '858 CETC                 '#20013#22283#38651#23376'-'#31532'58'#30740#31350#25152' '
            '859 XDXCT                '#35937#24093#20808'              '
            '860 MAXIM_TAILAND        MAXIM '#27888#22283
            '861 Microchip_Thailand   Microchip '#27888#22283
            '862 Microchip_Philippines Microchip '#33778#24459#36051
            '863 Microchip_China      Microchip '#20013#22283
            '864 Microchip_US         Microchip '#32654#22283
            '865 HANA Micron'
            '866 iTest_Inc            iTest,Inc. '#32654#22283
            '867 EMemory              EMemory '#21147#26106#38651#23376
            '868 CYUEAN               CYUEAN '#30830#23433#31185#25216
            '869 PANTHER              PANTHER '#40251#35895#31185#25216
            '870 ARDENTEC             '#27427#35438
            '871 FULCAP               '#35438#23481
            '872 AOSL                 '#33836#26377#21322#23566#39636
            '873 Nuvoton_Israel       '#26032#21776' '#20197#33394#21015
            '874 GT                   '#23536#37030#31185#25216
            '875 Novatek              '#32879#35424#31185#25216
            '876 Sunplus              '#20940#38525#31185#25216
            '877 Amlogic              '#26230#26216#21322#23566#39636
            '878 Higon                '#28023#20809' '#25104#37117
            '879 Kingston             '#37329#22763#38931
            '880 Spreadtrum           '#23637#35338' '#19978#28023
            '881 Amazon               Amazon'
            '882 Murata               Murata'
            '883 Goertek              '#27468#29246#24494#38651#23376
            '884 FMSH                 '#19978#28023#24489#26086#24494#38651#23376' '
            '885 HDSC                 '#33775#22823#21322#23566#39636
            '886 SANECHIPS            '#20013#33288#24494#38651#23376' '#28145#22323
            '887 Habana Labs          Habana Labs'
            '889 CENTER               '#20013#33455#22283#38555
            '890 UMC                  '#32879#38651
            '891 ATEC                 '#33406#31185
            '892 Winstek              '#21488#26143#31185
            '893 Sanan                '#19977#23433
            '894 UTAC_TW              '#32879#28204#31185#25216' '#21488#28771
            '895 BARUN'
            '897 SILTERRA_CHINKIANG   '#30717#20339' '#37806#27743
            '898 AMD_SUZHOU           AMD  '#34311#24030
            '899 YTEC                 '#20037#20803
            '900 SPIL                 '#30717#21697' '#20013#23665
            '901 ISE_USA              ISE'
            '902 HYGEIA_SUZHOU        '#28023#20140' '#34311#24030
            '903 JSI_HAOXING          '#38263#38651' '#32057#33288
            '904 JINGJIAWEI_CHANGSHA  '#38263#27801#26223#22025#24494
            '905 ISE_SH               '#19978#28023#26376#33455#21322#23566#39636#31185#25216
            '906 INTEL_IL             INTEL '#20197#33394#21015
            '907 IBM_CANADA           IBM '#21152#25343#22823
            '908 PGC                  '#24040#26377#31185
            '909 ITESTSEMI            '#23433#28204#21322#23566#39636'    '
            '910 SPIL SHINCHU         '#30717#21697' '#26032#31481
            '911 SPIL TAICHUNG_LOGIC  '#30717#21697' '#21488#20013
            '912 SPIL CHINA_SUZHOU    '#30717#21697' '#34311#24030
            '913 HUAWEI               '#33775#28858
            '914 ANST                 '#34311#24030#23433#30427
            '915 V-Test               '#28961#37675' '#20553#28204#21322#23566#39636
            '916 TFME CHINA           '#36890#23500#24494#38651
            '918 BOJIAN               '#34311#24030' '#21338#21133
            '919 V-Test Shanghai      '#19978#28023' '#20553#28204#21322#23566#39636
            '920 KYEC CHEN            '#20140#20803' '#35352#25014#39636
            '921 KYEC LEE             '#20140#20803' '#37007#36655
            '922 KYEC JCTHIU          '#20140#20803
            '923 DL_TEK               '#26481#29747'.'
            '924 KYEC XILINX          '#20140#20803' Xilinx'
            '925 KYEC STM             '#20140#20803' STM '
            '926 Advantest_GE         Advantest '#24503#22283
            '927 LATTICESEMI          '#33802#36842#24605#21322#23566#39636
            '928 ITRI                 '#24037#26989#25216#34899#30740#31350#38498' '
            '929 ASE KaohSiung_K12    '#26085#26376#20809' '#39640#38596'K12'
            '930 ASE SG               '#26085#26376#20809' '#26032#21152#22369
            '931 ASE JP               '#26085#26376#20809' '#26085#26412
            '932 ASE Korea            '#26085#26376#20809' '#38867#22283
            '933 ASE CL               '#26085#26376#20809' '#20013#22754
            '934 ASE SH               '#26085#26376#20809' '#19978#28023
            '935 ASE N                '#26085#26376#20809' NXP'
            '937 ASE Malaysia'#12288#12288#12288#12288' '#26085#26376#20809' '#39340#20358#35199#20126
            '938 ASE KaohSiung_K3'#12288'   '#26085#26376#20809' '#39640#38596'K3'
            '939 ASE KaohSiung_K11    '#26085#26376#20809' '#39640#38596'K11'
            '940 UTAC                 Open/Short'
            '941 SIGURD HUKOU         '#30717#26684' '#28246#21475
            '942 RFMD BEIJING         RFMD '#21271#20140
            '943 JSCC                 SC '#27743#38512
            '944 JSCS                 SC '#26032#21152#22369
            '945 SIGURD ChungXing     '#30717#26684' '#20013#33288
            '946 SIGURD PeiXing       '#30717#26684' '#21271#33288
            '947 JSCK                 SC '#38867#22283
            '948 RFMD USA             RFMD'#32654#22283#12288#12288
            '949 ASE KS               '#26085#26376#20809' '#23825#23665
            '950 APTOS                '#32676#35920
            '951 WINBOND              '#33775#37030
            '952 G-Link               G Link'
            '953 AMKOR                '#33406#20811#29246
            '954 GIGA                 GIGA'
            '955 LINGSEN              '#33777#29983
            '956 Greatek              '#36229#35920
            '957 PTI                  '#21147#25104
            '958 THAILIN              '#27888#26519
            '959 JCET                 '#27743#34311#38263#38651
            '960 OSE                  '#33775#27888
            '961 MTI                  '#32858#25104
            '962 NUVOTON              '#26032#21776
            '964 Eutrend              '#32895#21220'  '
            '965 TICP                 '#39640#38596#20856#31684'.  '
            '966 THEIL                '#21516#27427#38651#23376
            '967 TERAPOWER            '#26230#20806#25104
            '968 RICHTEK              '#31435#37665
            '969 TSI                  '#35488#36960#31185#25216
            '970 GIGAS'#12288#12288#12288#12288#12288#12288#12288#12288#20840#26234
            '971 AMKOR Korea          '#23433#38752#38867#22283'.'
            '972 AMKOR China          '#23433#38752#19978#28023'.'
            '973 AMKOR Japan          '#23433#38752#26085#26412'.'
            '974 AMKOR Philippines    '#23433#38752#33778#24459#36051
            '975 UTAC_TH              '#32879#28204#31185#25216' '#27888#22283
            '976 ANALOG DEVICES PH    '#20126#24503#35582#21322#23566#39636#33778#24459#36051
            '977 ONSEMI_CA            On Semi CANADA'
            '978 INTEL_US             INTEL US'
            '979 INTEL_M              INTEL Malaysia'
            '980 CARSEM M             Carsem Malaysia'
            '981 UNISEM M             Unisem Malaysia'
            '982 AMD M                AMD Malaysia'
            '983 INARI M              INARI Malaysia '
            '984 OnSemi M             On Semi Malaysia '
            '985 MAXIM '
            '986 MARVELL '
            '987 ATMEL '
            '988 USI                  '#29872#40251#31185#25216
            '989 DYNACARD             '#26032#26481#20126#24494#38651#23376
            '990 CYPRESS '
            '991 GERADTECH            '#26234#29790#36948
            '992 GM-Test '
            '993 I-Tech '
            '994 WinPac '
            '995 Silicon_Labs SG      Silicon_Labs'#26032#21152#22369
            '996 Silicon_Labs SZ      Silicon_Labs'#28145#22323
            '997 Altera_USA           Altera '#32654#22283
            '998 ETRENDTECH           '#36920#26124#31185#25216
            '999 Qualcomm             '#39640#36890)
          ParentFont = False
          TabOrder = 0
          OnClick = rgCustomerListClick
        end
      end
      object tsIONFan: TTabSheet
        Caption = 'ION Fan'
        ImageIndex = 8
        object pnlESD1: TPanel
          Left = 0
          Top = 0
          Width = 300
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
          object rgIonFanType: TRadioGroup
            Left = 4
            Top = 4
            Width = 292
            Height = 120
            Align = alTop
            Caption = 'ION Fan Alarm Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 1
            Items.Strings = (
              'By Detection Board (5 I/O for all)'
              'By Alarm (1 I/O for one)'
              'None'
              'By Power and Alarm (2 I/O for one)')
            ParentFont = False
            TabOrder = 0
          end
          object rgUsePulseType: TRadioGroup
            Left = 4
            Top = 204
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use ION Pulse Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object grpIONPulseCount: TGroupBox
            Left = 4
            Top = 124
            Width = 292
            Height = 80
            Align = alTop
            Caption = 'ION Pulse Count'
            TabOrder = 2
            object edIONPulseCount: TEdit
              Left = 40
              Top = 32
              Width = 121
              Height = 28
              TabOrder = 0
              Text = '700'
              OnClick = edIONPulseCountClick
            end
          end
          object rgChamberUsePulseType: TRadioGroup
            Left = 4
            Top = 264
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use Chamber Pulse Type'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
        end
        object pnlESD2: TPanel
          Left = 300
          Top = 0
          Width = 300
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 1
          object grpGroundMan: TGroupBox
            Left = 4
            Top = 4
            Width = 292
            Height = 180
            Align = alTop
            Caption = 'Ground Man'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            object Label21: TLabel
              Left = 10
              Top = 150
              Width = 95
              Height = 20
              Caption = 'Alarm Ohm'#65306
            end
            object rgGroundMan: TRadioGroup
              Left = 2
              Top = 22
              Width = 288
              Height = 60
              Align = alTop
              Caption = 'Use Ground Man'
              Columns = 2
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                'Un Install'
                'Install with COM')
              ParentFont = False
              TabOrder = 0
            end
            object rgGroundMan_ScanPoint: TRadioGroup
              Left = 2
              Top = 82
              Width = 288
              Height = 60
              Align = alTop
              Caption = 'Scan Point'
              Columns = 3
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBlue
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ItemIndex = 0
              Items.Strings = (
                '8'
                '22'
                '28')
              ParentFont = False
              TabOrder = 1
            end
            object edGroundMan_AlarmOhm: TEdit
              Left = 109
              Top = 146
              Width = 57
              Height = 28
              TabOrder = 2
              Text = '3'
              OnClick = edGroundMan_AlarmOhmClick
            end
          end
        end
        object pnlCloseLoop: TPanel
          Left = 600
          Top = 0
          Width = 300
          Height = 823
          Align = alLeft
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 2
          object rg3M_EM_AWARE_Monitor: TRadioGroup
            Left = 4
            Top = 4
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use 3M EM Aware'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 0
          end
          object rgNovx3360: TRadioGroup
            Left = 4
            Top = 64
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use Novx 3360'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 1
          end
          object rgKasuga: TRadioGroup
            Left = 4
            Top = 124
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use Kasuga'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 2
          end
          object rgKasuga_Fan: TRadioGroup
            Left = 4
            Top = 184
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Use Kasuga Fan'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 3
          end
          object rgHTIonBar: TRadioGroup
            Left = 4
            Top = 244
            Width = 292
            Height = 133
            Align = alTop
            Caption = 'Use HT IonBar'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clPurple
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Loader Only'
              'UnloaderOnly (Ion 4/5/8)'
              'Full Area')
            ParentFont = False
            TabOrder = 4
          end
          object rgAutoCleanIonFan: TRadioGroup
            Left = 4
            Top = 377
            Width = 292
            Height = 60
            Align = alTop
            Caption = 'Auto Clean Ion Fan'
            Columns = 2
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlue
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ItemIndex = 0
            Items.Strings = (
              'Un Install'
              'Install')
            ParentFont = False
            TabOrder = 5
          end
        end
      end
      object tsSearchFunction: TTabSheet
        Caption = 'Search Function'
        ImageIndex = 9
        object scrlbxSearchFunc: TScrollBox
          Left = 0
          Top = 0
          Width = 500
          Height = 823
          Align = alLeft
          TabOrder = 0
        end
        object edtSearchFunction: TLabeledEdit
          Left = 516
          Top = 33
          Width = 185
          Height = 28
          EditLabel.Width = 117
          EditLabel.Height = 20
          EditLabel.Caption = 'Search Function'
          LabelPosition = lpAbove
          LabelSpacing = 3
          TabOrder = 1
          OnChange = edtSearchFunctionChange
        end
      end
      object tsHeater: TTabSheet
        Caption = 'Heater'
        ImageIndex = 10
        object grpHeater: TGroupBox
          Left = 0
          Top = 0
          Width = 1262
          Height = 823
          Align = alClient
          TabOrder = 0
        end
      end
    end
    object Panel3: TPanel
      Left = 0
      Top = 883
      Width = 1270
      Height = 63
      Align = alBottom
      TabOrder = 1
      object SaveBtn: TBitBtn
        Left = 390
        Top = 22
        Width = 89
        Height = 33
        Caption = 'Save'
        TabOrder = 1
        OnClick = SaveBtnClick
        Glyph.Data = {
          AA040000424DAA04000000000000360000002800000014000000130000000100
          1800000000007404000074120000741200000000000000000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033990033990033
          990033990033990033990033990033990033990033990033990033990033C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0990033C0C0C0FFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF990033C0C0C0669966990033C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0990033C0C0C0FFFFFFFFFFFFFFFF
          FFFFFFFF990033669966FFFFFF990033C0C0C0669966990033C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0990033C0C0C0990033C0C0C0FFFFFFFFFFFFFFFFFFFFFFFF
          990033669966FFFFFF990033C0C0C0669966990033C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0990033C0C0C0990033C0C0C0FFFFFFFFFFFFFFFFFFFFFFFF99003399
          0033FFFFFF990033C0C0C0669966990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          990033C0C0C09900339900339900339900339900339900339900339900339900
          33990033C0C0C0669966990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0
          C0C0669966669966669966669966669966669966669966669966669966669966
          C0C0C0669966990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C06699
          66990033990033990033990033990033990033990033990033990033C0C0C066
          9966990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0669966990033
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C06699669900
          33C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0669966990033C0C0C099
          6633996633996633996633996633C0C0C0990033C0C0C0669966990033C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0669966990033C0C0C09966339966
          33996633996633996633C0C0C0990033C0C0C0993333990033C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0990033C0C0C0669966990033C0C0C0996633996633996633
          996633996633C0C0C0990033C0C0C0669966990033C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C099003399003399003399003399003399003399003399003399
          0033990033990033990033990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
      end
      object ExitBtn: TBitBtn
        Left = 488
        Top = 22
        Width = 89
        Height = 33
        Caption = 'Close'
        TabOrder = 2
        OnClick = ExitBtnClick
        OnMouseDown = ExitBtnMouseDown
        Glyph.Data = {
          DE010000424DDE01000000000000760000002800000024000000120000000100
          0400000000006801000000000000000000001000000000000000000000000000
          80000080000000808000800000008000800080800000C0C0C000808080000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00388888888877
          F7F787F8888888888333333F00004444400888FFF444448888888888F333FF8F
          000033334D5007FFF4333388888888883338888F0000333345D50FFFF4333333
          338F888F3338F33F000033334D5D0FFFF43333333388788F3338F33F00003333
          45D50FEFE4333333338F878F3338F33F000033334D5D0FFFF43333333388788F
          3338F33F0000333345D50FEFE4333333338F878F3338F33F000033334D5D0FFF
          F43333333388788F3338F33F0000333345D50FEFE4333333338F878F3338F33F
          000033334D5D0EFEF43333333388788F3338F33F0000333345D50FEFE4333333
          338F878F3338F33F000033334D5D0EFEF43333333388788F3338F33F00003333
          4444444444333333338F8F8FFFF8F33F00003333333333333333333333888888
          8888333F00003333330000003333333333333FFFFFF3333F00003333330AAAA0
          333333333333888888F3333F00003333330000003333333333338FFFF8F3333F
          0000}
        NumGlyphs = 2
      end
      object LoadBtn: TBitBtn
        Left = 296
        Top = 22
        Width = 89
        Height = 33
        Caption = 'Load'
        TabOrder = 0
        OnClick = LoadBtnClick
        Glyph.Data = {
          AA040000424DAA04000000000000360000002800000014000000130000000100
          1800000000007404000074120000741200000000000000000000C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C09900339900
          33990033990033990033990033990033990033990033990033990033C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033990033C0C0C000CCFF
          00CCFF00CCFF00CCFF00CCFF00CCFF00CCFF00CCFFC0C0C0993333C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF990033C0C0C000CCFF00
          CCFF00CCFF00CCFF00CCFF00CCFF00CCFF00CCFFC0C0C0993333C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF00FFFF990033C0C0C000CCFF00CC
          FF00CCFF00CCFF00CCFF00CCFF00CCFF00CCFFC0C0C0993333C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0990033FFFFFF00FFFF00FFFF990033C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033C0C0C0C0C0C0C0C0
          C0C0C0C0990033FFFFFF00FFFF00FFFF00FFFF99003399003399003399003399
          0033990033990033990033990033990033990033C0C0C0C0C0C0C0C0C0C0C0C0
          990033FFFFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FF
          FF00FFFFC0C0C0990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FF
          FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF00FFFF
          C0C0C0990033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF00FF
          FF00FFFF00FFFF00FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC0C0C099
          0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFF00FFFF00FFFF
          00FFFFFFFFFF990033990033990033990033990033990033990033990033C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033FFFFFFFFFFFFFFFFFF99
          0033C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0990033990033990033C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000000000000000C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000000000C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0000000C0
          C0C0C0C0C0C0C0C0000000C0C0C0000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C00000000000
          00000000C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0C0
          C0C0C0C0C0C0C0C0C0C0C0C0C0C0}
      end
      object edtSearchCode: TLabeledEdit
        Left = 788
        Top = 27
        Width = 185
        Height = 28
        EditLabel.Width = 166
        EditLabel.Height = 20
        EditLabel.Caption = 'Search Customer Code'
        LabelPosition = lpAbove
        LabelSpacing = 3
        TabOrder = 3
        Visible = False
        OnChange = edtSearchCodeChange
      end
    end
  end
end
