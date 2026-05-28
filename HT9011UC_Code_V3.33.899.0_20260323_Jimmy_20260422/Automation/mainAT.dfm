object fMainAT: TfMainAT
  Left = 202
  Top = 120
  Width = 864
  Height = 631
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
    Width = 209
    Height = 237
    Caption = 'System Status'
    TabOrder = 0
    object Label1: TLabel
      Left = 5
      Top = 80
      Width = 79
      Height = 13
      Caption = 'Machine Model :'
    end
    object Label2: TLabel
      Left = 5
      Top = 105
      Width = 61
      Height = 13
      Caption = 'Machine ID :'
    end
    object Label3: TLabel
      Left = 5
      Top = 130
      Width = 41
      Height = 13
      Caption = 'Version :'
    end
    object Label4: TLabel
      Left = 5
      Top = 155
      Width = 93
      Height = 13
      Caption = 'Connection Status :'
    end
    object Label5: TLabel
      Left = 5
      Top = 180
      Width = 76
      Height = 13
      Caption = 'Handler Status :'
    end
    object lbSystemStatus: TLabel
      Left = 20
      Top = 28
      Width = 173
      Height = 29
      AutoSize = False
      Caption = 'HALT'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label30: TLabel
      Left = 5
      Top = 204
      Width = 83
      Height = 13
      Caption = 'Set Up File Name'
    end
    object cbMachineModel: TComboBox
      Left = 104
      Top = 76
      Width = 90
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      Text = 'HT-9045W'
      Items.Strings = (
        'HT-9046'
        'HT-9045W'
        'HT-7048'
        'HT-7045'
        'HT-7040'
        'HT-9040'
        'HT-9080'
        'HT-8080')
    end
    object edMachineID: TEdit
      Left = 104
      Top = 101
      Width = 90
      Height = 21
      TabOrder = 1
      Text = 'HT-100'
    end
    object enVersion: TEdit
      Left = 104
      Top = 126
      Width = 90
      Height = 21
      TabOrder = 2
      Text = 'V2.06'
    end
    object cbConnectStatus: TComboBox
      Left = 104
      Top = 151
      Width = 90
      Height = 21
      ItemHeight = 13
      TabOrder = 3
      Text = 'Ready'
      Items.Strings = (
        'Ready'
        'Not Ready')
    end
    object cbHandlerStatus: TComboBox
      Left = 104
      Top = 176
      Width = 90
      Height = 21
      ItemHeight = 13
      TabOrder = 4
      Text = 'HALT'
      Items.Strings = (
        'HALT'
        'PAUSE'
        'RUNNING')
    end
    object cbSetUpFileName: TComboBox
      Left = 104
      Top = 200
      Width = 90
      Height = 21
      ItemHeight = 13
      TabOrder = 5
      Text = '3x3 14x35'
    end
  end
  object GroupBox2: TGroupBox
    Left = 36
    Top = 496
    Width = 777
    Height = 93
    Caption = 'Actual Temperature'
    TabOrder = 1
    object APHeadLabel1: TAPHeadLabel
      Left = 28
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Plate 1'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel2: TAPHeadLabel
      Left = 101
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Plate 2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel3: TAPHeadLabel
      Left = 176
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Shuttle 1'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel4: TAPHeadLabel
      Left = 251
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Shuttle 2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel5: TAPHeadLabel
      Left = 324
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Head 1/2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel6: TAPHeadLabel
      Left = 397
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Head 3/4'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel7: TAPHeadLabel
      Left = 470
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Head 5/6'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel8: TAPHeadLabel
      Left = 543
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Head 7/8'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel9: TAPHeadLabel
      Left = 616
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Dut'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object APHeadLabel10: TAPHeadLabel
      Left = 689
      Top = 28
      Width = 76
      Height = 27
      Alignment = taCenter
      AutoBounds = False
      BoundLines = [blLeft, blTop, blRight, blBottom]
      BoundColor = 5645915
      Gradient.StartColor = 16512185
      Gradient.Style = gsNone
      SubCaption.Ellipsis = False
      SubCaption.Style = []
      Caption = 'Chamber'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edTemp1: TEdit
      Left = 28
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = '25.0'
    end
    object edTemp2: TEdit
      Left = 101
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = '25.0'
    end
    object edTemp3: TEdit
      Left = 176
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = '25.0'
    end
    object edTemp4: TEdit
      Left = 251
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Text = '25.0'
    end
    object edTemp5: TEdit
      Left = 324
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      Text = '---'
    end
    object edTemp6: TEdit
      Left = 397
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = '---'
    end
    object edTemp7: TEdit
      Left = 470
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      Text = '---'
    end
    object edTemp8: TEdit
      Left = 543
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      Text = '---'
    end
    object edTemp9: TEdit
      Left = 616
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      Text = '25.0'
    end
    object edTemp10: TEdit
      Left = 689
      Top = 54
      Width = 76
      Height = 28
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      Text = '25.0'
    end
  end
  object GroupBox3: TGroupBox
    Left = 12
    Top = 252
    Width = 209
    Height = 117
    Caption = 'Temperature'
    TabOrder = 2
    object Label29: TLabel
      Left = 16
      Top = 60
      Width = 48
      Height = 13
      Caption = 'SoakTime'
    end
    object Label28: TLabel
      Left = 16
      Top = 32
      Width = 60
      Height = 13
      Caption = 'Temperature'
    end
    object edtSoakTime: TEdit
      Left = 80
      Top = 58
      Width = 37
      Height = 21
      TabOrder = 0
      Text = '25'
    end
    object edtTemperature: TEdit
      Left = 80
      Top = 28
      Width = 37
      Height = 21
      TabOrder = 1
      Text = '25'
    end
    object cTempmode: TCheckBox
      Left = 16
      Top = 88
      Width = 153
      Height = 17
      Caption = 'Temp On / Off'
      TabOrder = 2
    end
  end
  object GroupBox4: TGroupBox
    Left = 12
    Top = 372
    Width = 209
    Height = 85
    Caption = 'Tester'
    TabOrder = 3
    object cConnectionTester: TCheckBox
      Left = 12
      Top = 20
      Width = 137
      Height = 17
      Caption = 'Connect with Tester'
      TabOrder = 0
    end
    object rgTesterMode: TRadioGroup
      Left = 8
      Top = 44
      Width = 169
      Height = 33
      Caption = 'Test Mode'
      Columns = 3
      ItemIndex = 0
      Items.Strings = (
        '0'
        '1'
        '2')
      TabOrder = 1
    end
  end
  object gbCategory: TGroupBox
    Left = 380
    Top = 12
    Width = 150
    Height = 425
    Caption = 'Category'
    TabOrder = 4
  end
  object gbMapping: TGroupBox
    Left = 532
    Top = 12
    Width = 150
    Height = 425
    Caption = 'Site Mapping'
    TabOrder = 5
  end
  object gbDutOn: TGroupBox
    Left = 684
    Top = 12
    Width = 150
    Height = 425
    Caption = 'Dut On / Off'
    TabOrder = 6
  end
  object gbTrayForm: TGroupBox
    Left = 224
    Top = 12
    Width = 150
    Height = 220
    Caption = 'Tray Form'
    TabOrder = 7
    object Label6: TLabel
      Left = 12
      Top = 28
      Width = 30
      Height = 13
      Caption = 'X-Item'
    end
    object Label7: TLabel
      Left = 12
      Top = 61
      Width = 30
      Height = 13
      Caption = 'Y-Item'
    end
    object Label8: TLabel
      Left = 12
      Top = 93
      Width = 32
      Height = 13
      Caption = 'X-Start'
    end
    object Label9: TLabel
      Left = 12
      Top = 126
      Width = 32
      Height = 13
      Caption = 'Y-Start'
    end
    object Label10: TLabel
      Left = 12
      Top = 158
      Width = 34
      Height = 13
      Caption = 'X-Pitch'
    end
    object Label11: TLabel
      Left = 12
      Top = 191
      Width = 34
      Height = 13
      Caption = 'Y-Pitch'
    end
    object Edit1: TEdit
      Left = 70
      Top = 24
      Width = 70
      Height = 21
      TabOrder = 0
      Text = '14'
    end
    object Edit2: TEdit
      Left = 70
      Top = 56
      Width = 70
      Height = 21
      TabOrder = 1
      Text = '35'
    end
    object Edit3: TEdit
      Left = 70
      Top = 89
      Width = 70
      Height = 21
      TabOrder = 2
      Text = '8.15'
    end
    object Edit4: TEdit
      Left = 70
      Top = 121
      Width = 70
      Height = 21
      TabOrder = 3
      Text = '7.9'
    end
    object Edit5: TEdit
      Left = 70
      Top = 154
      Width = 70
      Height = 21
      TabOrder = 4
      Text = '9.2'
    end
    object Edit6: TEdit
      Left = 70
      Top = 186
      Width = 70
      Height = 21
      TabOrder = 5
      Text = '8.8'
    end
  end
  object gbPlateForm: TGroupBox
    Left = 224
    Top = 237
    Width = 150
    Height = 220
    Caption = 'Plate Form'
    TabOrder = 8
    object Label12: TLabel
      Left = 12
      Top = 28
      Width = 30
      Height = 13
      Caption = 'X-Item'
    end
    object Label13: TLabel
      Left = 12
      Top = 61
      Width = 30
      Height = 13
      Caption = 'Y-Item'
    end
    object Label14: TLabel
      Left = 12
      Top = 93
      Width = 32
      Height = 13
      Caption = 'X-Start'
    end
    object Label15: TLabel
      Left = 12
      Top = 126
      Width = 32
      Height = 13
      Caption = 'Y-Start'
    end
    object Label16: TLabel
      Left = 12
      Top = 158
      Width = 34
      Height = 13
      Caption = 'X-Pitch'
    end
    object Label17: TLabel
      Left = 12
      Top = 191
      Width = 34
      Height = 13
      Caption = 'Y-Pitch'
    end
    object Edit7: TEdit
      Left = 70
      Top = 24
      Width = 70
      Height = 21
      TabOrder = 0
      Text = '6'
    end
    object Edit8: TEdit
      Left = 70
      Top = 56
      Width = 70
      Height = 21
      TabOrder = 1
      Text = '11'
    end
    object Edit9: TEdit
      Left = 70
      Top = 89
      Width = 70
      Height = 21
      TabOrder = 2
      Text = '13.35'
    end
    object Edit10: TEdit
      Left = 70
      Top = 121
      Width = 70
      Height = 21
      TabOrder = 3
      Text = '20.05'
    end
    object Edit11: TEdit
      Left = 70
      Top = 154
      Width = 70
      Height = 21
      TabOrder = 4
      Text = '26.6'
    end
    object Edit12: TEdit
      Left = 70
      Top = 186
      Width = 70
      Height = 21
      TabOrder = 5
      Text = '30'
    end
  end
end
