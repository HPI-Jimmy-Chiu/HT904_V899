object Main: TMain
  Left = 173
  Top = 93
  Width = 870
  Height = 640
  Caption = 'Hon Prec Automation Simulator'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 12
    Top = 12
    Width = 313
    Height = 205
    Caption = 'System Status'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 80
      Width = 79
      Height = 13
      Caption = 'Machine Model :'
    end
    object Label2: TLabel
      Left = 16
      Top = 104
      Width = 61
      Height = 13
      Caption = 'Machine ID :'
    end
    object Label3: TLabel
      Left = 16
      Top = 128
      Width = 41
      Height = 13
      Caption = 'Version :'
    end
    object Label4: TLabel
      Left = 16
      Top = 152
      Width = 93
      Height = 13
      Caption = 'Connection Status :'
    end
    object Label5: TLabel
      Left = 16
      Top = 180
      Width = 76
      Height = 13
      Caption = 'Handler Status :'
    end
    object lbSystemStatus: TLabel
      Left = 20
      Top = 28
      Width = 265
      Height = 29
      AutoSize = False
      Caption = 'lbSystemStatus'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object cbMachineModel: TComboBox
      Left = 120
      Top = 76
      Width = 169
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Text = 'HT-9045W'
      Items.Strings = (
        'HT-9046'
        'HT-9045W'
        'HT-7048'
        'HT-7045 / HT-7040'
        'HT-9040 / HT-9080 / HT-8080')
    end
    object edMachineID: TEdit
      Left = 120
      Top = 100
      Width = 121
      Height = 21
      TabOrder = 1
      Text = 'HT-100'
    end
    object enVersion: TEdit
      Left = 120
      Top = 124
      Width = 121
      Height = 21
      TabOrder = 2
      Text = 'V2.06'
    end
    object cbConnectStatus: TComboBox
      Left = 120
      Top = 148
      Width = 169
      Height = 21
      ItemHeight = 13
      TabOrder = 3
      Text = 'Ready'
      Items.Strings = (
        'Ready'
        'Not Ready')
    end
    object cbHandlerStatus: TComboBox
      Left = 120
      Top = 176
      Width = 169
      Height = 21
      ItemHeight = 13
      TabOrder = 4
      Text = 'HALT'
      Items.Strings = (
        'HALT'
        'PAUSE'
        'RUNNING'
        '')
    end
  end
  object rgACK: TRadioGroup
    Left = 12
    Top = 224
    Width = 313
    Height = 45
    Caption = 'ACK#'
    Columns = 3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemIndex = 0
    Items.Strings = (
      'Succeed'
      'No Data'
      'Otherwise')
    ParentFont = False
    TabOrder = 1
  end
end
