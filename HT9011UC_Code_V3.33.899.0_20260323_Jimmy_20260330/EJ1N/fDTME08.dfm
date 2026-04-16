object frmDTME08: TfrmDTME08
  Left = 1189
  Top = 300
  Width = 1296
  Height = 795
  Caption = 'DTME08'
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
  object Label45: TLabel
    Left = 52
    Top = 24
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label46: TLabel
    Left = 52
    Top = 48
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label47: TLabel
    Left = 52
    Top = 72
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Label48: TLabel
    Left = 52
    Top = 100
    Width = 44
    Height = 13
    Caption = 'Module 1'
  end
  object Panel7: TPanel
    Left = 0
    Top = 0
    Width = 425
    Height = 471
    Align = alLeft
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object PageControl1: TPageControl
      Left = 0
      Top = 0
      Width = 425
      Height = 471
      ActivePage = tsSimple
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabIndex = 2
      TabOrder = 0
      TabWidth = 100
      object tsAuto: TTabSheet
        Caption = 'Auto'
        object btGetModuler: TButton
          Left = 15
          Top = 96
          Width = 121
          Height = 25
          Caption = 'Get Moduler'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object btRunStop: TButton
          Left = 15
          Top = 155
          Width = 121
          Height = 25
          Caption = 'Set RUN / STOP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          Visible = False
        end
        object rgSensorType: TRadioGroup
          Left = 16
          Top = 4
          Width = 145
          Height = 85
          Caption = 'Thermo Sensor'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            'PT Type'
            'K Type')
          ParentFont = False
          TabOrder = 0
        end
        object btSetThermoType: TButton
          Left = 15
          Top = 125
          Width = 121
          Height = 25
          Caption = 'Set Sensor Type'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btSetThermoTypeClick
        end
        object rgRunStop: TRadioGroup
          Left = 164
          Top = 4
          Width = 133
          Height = 85
          Caption = 'RUN / STOP'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ItemIndex = 0
          Items.Strings = (
            'STOP'
            'RUN')
          ParentFont = False
          TabOrder = 1
        end
        object Button1: TButton
          Left = 16
          Top = 184
          Width = 121
          Height = 25
          Caption = 'Stop Control'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Visible = False
        end
      end
      object tsManual: TTabSheet
        Caption = 'Manual'
        ImageIndex = 1
        object GroupBox3: TGroupBox
          Left = 8
          Top = 8
          Width = 409
          Height = 65
          Caption = 'Connection Settings'
          Color = 12761254
          ParentColor = False
          TabOrder = 0
          object Label1: TLabel
            Left = 8
            Top = 27
            Width = 19
            Height = 22
            Caption = 'IP'
          end
          object Label2: TLabel
            Left = 152
            Top = 27
            Width = 35
            Height = 22
            Caption = 'Port'
          end
          object edAddress: TEdit
            Left = 35
            Top = 24
            Width = 111
            Height = 30
            TabOrder = 0
            Text = '127.0.0.1'
          end
          object edPort: TEdit
            Left = 194
            Top = 24
            Width = 55
            Height = 30
            TabOrder = 1
            Text = '502'
            OnClick = edPortClick
          end
          object btnSetSave: TButton
            Left = 256
            Top = 16
            Width = 57
            Height = 41
            Caption = 'Save'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
            OnClick = btnSetSaveClick
          end
          object btnReload: TButton
            Left = 320
            Top = 16
            Width = 81
            Height = 41
            Caption = 'Reload'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -19
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 3
            OnClick = btnReloadClick
          end
        end
        object GroupBox1: TGroupBox
          Left = 8
          Top = 73
          Width = 161
          Height = 344
          Caption = 'Tools'
          Color = 12761254
          ParentColor = False
          TabOrder = 1
          object btnGetPV: TPanel
            Left = 8
            Top = 64
            Width = 140
            Height = 33
            Caption = 'Get PV'
            Color = clSkyBlue
            TabOrder = 0
            OnClick = btnGetPVClick
          end
          object btnSetAT: TPanel
            Left = 8
            Top = 104
            Width = 140
            Height = 33
            Caption = 'Set AT'
            Color = clSkyBlue
            TabOrder = 1
            OnClick = btnSetATClick
          end
          object btnGetSV: TPanel
            Left = 8
            Top = 144
            Width = 140
            Height = 33
            Caption = 'Get SV'
            Color = clSkyBlue
            TabOrder = 2
            OnClick = btnGetSVClick
          end
          object btnSetSV: TPanel
            Left = 8
            Top = 184
            Width = 140
            Height = 33
            Caption = 'Set SV'
            Color = clSkyBlue
            TabOrder = 3
            OnClick = btnSetSVClick
          end
          object btnGetStatus: TPanel
            Left = 8
            Top = 224
            Width = 140
            Height = 33
            Caption = 'Get Status'
            Color = clSkyBlue
            TabOrder = 4
            OnClick = btnGetStatusClick
          end
          object cbCh: TComboBox
            Left = 8
            Top = 24
            Width = 145
            Height = 30
            ItemHeight = 0
            TabOrder = 5
          end
          object btnSetSensor: TPanel
            Left = 8
            Top = 264
            Width = 140
            Height = 33
            Caption = 'Set Sensor'
            Color = clSkyBlue
            TabOrder = 6
            OnClick = btnSetSensorClick
          end
        end
      end
      object tsSimple: TTabSheet
        Caption = 'Parameter'
        ImageIndex = 2
        object btSetSV: TButton
          Left = 15
          Top = 211
          Width = 121
          Height = 50
          Caption = 'Set SV'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          OnClick = btSetSVClick
        end
        object gbAT: TGroupBox
          Left = 4
          Top = 4
          Width = 413
          Height = 177
          Caption = 'AT && RT'
          TabOrder = 0
          object btAT: TButton
            Left = 15
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT ON (For All)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            OnClick = btATClick
          end
          object btATOff: TButton
            Left = 283
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT OFF'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            OnClick = btATOffClick
          end
          object btAT1by1: TButton
            Left = 151
            Top = 28
            Width = 121
            Height = 50
            Caption = 'AT ON (1 by 1)'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = btAT1by1Click
          end
        end
        object grpStatus: TGroupBox
          Left = 152
          Top = 188
          Width = 265
          Height = 229
          Caption = 'Module Status'
          TabOrder = 1
          object ledEJ1N1: TMyLed
            Left = 24
            Top = 24
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object Label25: TLabel
            Left = 52
            Top = 24
            Width = 78
            Height = 22
            Caption = 'Module 1'
          end
          object cbSimulation: TCheckBox
            Left = 144
            Top = 24
            Width = 113
            Height = 17
            Caption = 'Simulation'
            TabOrder = 0
            Visible = False
          end
          object chkHeater: TCheckBox
            Left = 160
            Top = 204
            Width = 97
            Height = 17
            Caption = 'Heater On'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            OnClick = chkHeaterClick
          end
        end
        object btnSetSVFollowCh1: TButton
          Left = 15
          Top = 275
          Width = 138
          Height = 50
          Caption = 'Set SV follow ch1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btnSetSVFollowCh1Click
        end
        object btnCtrl: TButton
          Left = 16
          Top = 344
          Width = 121
          Height = 50
          Caption = 'Stop Control'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = btnCtrlClick
        end
      end
      object tsRecord: TTabSheet
        Caption = 'Record'
        ImageIndex = 3
        object Memo1: TMemo
          Left = 0
          Top = 0
          Width = 417
          Height = 385
          Align = alClient
          Color = 14670284
          ScrollBars = ssBoth
          TabOrder = 0
        end
        object Panel11: TPanel
          Left = 0
          Top = 385
          Width = 417
          Height = 41
          Align = alBottom
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 1
          object cbGetRecord: TCheckBox
            Left = 4
            Top = 12
            Width = 149
            Height = 17
            Caption = 'Get Record'
            TabOrder = 0
          end
          object btSaveData: TButton
            Left = 264
            Top = 12
            Width = 145
            Height = 25
            Caption = 'Save Data'
            TabOrder = 1
          end
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 425
    Top = 0
    Width = 863
    Height = 471
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object GroupBox2: TGroupBox
      Left = 0
      Top = 0
      Width = 863
      Height = 471
      Align = alClient
      Caption = 'Auto Run'
      Color = 12761254
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object ScrollBox1: TScrollBox
        Left = 2
        Top = 20
        Width = 859
        Height = 449
        Align = alClient
        BiDiMode = bdLeftToRight
        BorderStyle = bsNone
        Color = 12761254
        ParentBiDiMode = False
        ParentColor = False
        TabOrder = 0
        object GroupBox: TGroupBox
          Left = 10
          Top = 10
          Width = 206
          Height = 120
          Caption = 'myPalSample'
          DragKind = dkDock
          TabOrder = 0
          Visible = False
          object Panel2: TPanel
            Left = 72
            Top = 20
            Width = 129
            Height = 44
            BevelOuter = bvNone
            Caption = '##.##'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clYellow
            Font.Height = 48
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object Panel3: TPanel
            Left = 4
            Top = 20
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = #8451
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clYellow
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object Panel4: TPanel
            Left = 4
            Top = 39
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'RUN'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object Panel5: TPanel
            Left = 4
            Top = 77
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'AT'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object Panel6: TPanel
            Left = 4
            Top = 58
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'Input Error'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object Panel8: TPanel
            Left = 72
            Top = 64
            Width = 129
            Height = 21
            BevelOuter = bvNone
            Caption = 'Event'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object Panel10: TPanel
            Left = 4
            Top = 96
            Width = 69
            Height = 19
            BevelOuter = bvNone
            Caption = 'SP'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = 4227327
            Font.Height = -13
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object edSV: TEdit
            Left = 72
            Top = 86
            Width = 129
            Height = 27
            AutoSize = False
            BorderStyle = bsNone
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = 24
            Font.Name = 'Arial'
            Font.Style = []
            MaxLength = 3
            ParentFont = False
            TabOrder = 7
            Text = '0'
            OnClick = edSVClick
          end
          object cbEnableAT: TCheckBox
            Left = 48
            Top = 76
            Width = 17
            Height = 17
            Color = clGray
            ParentColor = False
            TabOrder = 5
          end
        end
      end
    end
  end
  object Panel9: TPanel
    Left = 0
    Top = 471
    Width = 1288
    Height = 293
    Align = alBottom
    TabOrder = 2
    object meLogs: TMemo
      Left = 1
      Top = 1
      Width = 1286
      Height = 291
      Align = alClient
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ScrollBars = ssBoth
      TabOrder = 0
    end
  end
  object TimerUpdate: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerUpdateTimer
    Left = 992
    Top = 8
  end
end
