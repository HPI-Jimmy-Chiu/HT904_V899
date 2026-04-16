object fTesterTCP: TfTesterTCP
  Left = 403
  Top = 66
  Width = 856
  Height = 881
  Caption = 'Tester TCP/IP'
  Color = 8421440
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object pgcTcpIp: TPageControl
    Left = 0
    Top = 0
    Width = 848
    Height = 850
    ActivePage = tsTestResult2
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabHeight = 30
    TabIndex = 3
    TabOrder = 0
    TabWidth = 120
    object tsMain: TTabSheet
      Caption = 'Main'
      object mmTCPIPCommLog: TMemo
        Left = 0
        Top = 507
        Width = 840
        Height = 303
        Align = alBottom
        Color = 14670284
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 32
      end
      object gbSite: TGroupBox
        Left = 0
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site01'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object labOcr01: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite01: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn01: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite01: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox1: TGroupBox
        Left = 104
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site02'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object labOcr02: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite02: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn02: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite02: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox2: TGroupBox
        Left = 208
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site03'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object labOcr03: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite03: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn03: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite03: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox3: TGroupBox
        Left = 520
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site06'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        object labOcr06: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite06: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn06: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite06: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox4: TGroupBox
        Left = 416
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site05'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        object labOcr05: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite05: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn05: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite05: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox5: TGroupBox
        Left = 312
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site04'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object labOcr04: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite04: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn04: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite04: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox6: TGroupBox
        Left = 728
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site08'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        object labOcr08: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite08: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn08: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite08: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox7: TGroupBox
        Left = 624
        Top = 0
        Width = 100
        Height = 120
        Caption = 'Site07'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        object labOcr07: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite07: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn07: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite07: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox8: TGroupBox
        Left = 0
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site09'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        object labOcr09: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite09: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn09: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite09: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox9: TGroupBox
        Left = 104
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site10'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        object labOcr10: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite10: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn10: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite10: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox10: TGroupBox
        Left = 208
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site11'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        object labOcr11: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite11: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn11: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite11: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox11: TGroupBox
        Left = 520
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site14'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 13
        object labOcr14: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite14: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn14: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite14: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox12: TGroupBox
        Left = 416
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site13'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
        object labOcr13: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite13: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn13: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite13: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox13: TGroupBox
        Left = 312
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site12'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        object labOcr12: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite12: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn12: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite12: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox14: TGroupBox
        Left = 728
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site16'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 15
        object labOcr16: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite16: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn16: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite16: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox15: TGroupBox
        Left = 624
        Top = 124
        Width = 100
        Height = 120
        Caption = 'Site15'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 14
        object labOcr15: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite15: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn15: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite15: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox16: TGroupBox
        Left = 0
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site17'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
        object labOcr17: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite17: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn17: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite17: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox17: TGroupBox
        Left = 104
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site18'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 17
        object labOcr18: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite18: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn18: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite18: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox18: TGroupBox
        Left = 208
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site19'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 18
        object labOcr19: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite19: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn19: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite19: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox19: TGroupBox
        Left = 520
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site22'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
        object labOcr22: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite22: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn22: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite22: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox20: TGroupBox
        Left = 416
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site21'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
        object labOcr21: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite21: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn21: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite21: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox21: TGroupBox
        Left = 312
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site20'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 19
        object labOcr20: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite20: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn20: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite20: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox22: TGroupBox
        Left = 728
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site24'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 23
        object labOcr24: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite24: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn24: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite24: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox23: TGroupBox
        Left = 624
        Top = 248
        Width = 100
        Height = 120
        Caption = 'Site23'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 22
        object labOcr23: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite23: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn23: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite23: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox24: TGroupBox
        Left = 0
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site25'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 24
        object labOcr25: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite25: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn25: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite25: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox25: TGroupBox
        Left = 104
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site26'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 25
        object labOcr26: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite26: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn26: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite26: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox26: TGroupBox
        Left = 208
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site27'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 26
        object labOcr27: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite27: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn27: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite27: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox27: TGroupBox
        Left = 520
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site30'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 29
        object labOcr30: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite30: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn30: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite30: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox28: TGroupBox
        Left = 416
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site29'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 28
        object labOcr29: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite29: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn29: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite29: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox29: TGroupBox
        Left = 312
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site28'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 27
        object labOcr28: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite28: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn28: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite28: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox30: TGroupBox
        Left = 728
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site32'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 31
        object labOcr32: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite32: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn32: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite32: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object GroupBox31: TGroupBox
        Left = 624
        Top = 372
        Width = 100
        Height = 120
        Caption = 'Site31'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindow
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 30
        object labOcr31: TLabel
          Left = 2
          Top = 104
          Width = 96
          Height = 14
          Align = alBottom
          Alignment = taCenter
          Caption = 'OCR Text'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object cbSite31: TComboBox
          Left = 35
          Top = 68
          Width = 55
          Height = 28
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ImeMode = imClose
          ImeName = #20013#25991' ('#32321#39636') - '#26032#27880#38899
          ItemHeight = 20
          ParentFont = False
          TabOrder = 1
          Text = '1'
          Items.Strings = (
            '1'
            '2'
            '3'
            '4'
            '5'
            '6'
            '1..5'
            '1..10'
            '1..16')
        end
        object cbSiteOn31: TCheckBox
          Left = 15
          Top = 72
          Width = 14
          Height = 17
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
        object plSite31: TPanel
          Left = 12
          Top = 18
          Width = 77
          Height = 49
          BevelInner = bvLowered
          Caption = '01'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -35
          Font.Name = 'Impact'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
    end
    object tsSetting: TTabSheet
      Caption = 'Setting'
      ImageIndex = 1
      object grpTrayFunc_OS: TGroupBox
        Left = 0
        Top = 120
        Width = 840
        Height = 89
        Align = alTop
        Caption = 'OutArm Place Type'
        Color = 8421440
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenu
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 1
        object lblTrayFunc_OS: TLabel
          Left = 15
          Top = 30
          Width = 405
          Height = 18
          Caption = 'If Func.Checked, OutArm will perform  Variable Pitch Mode'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object chkAuto1_OS: TCheckBox
          Left = 12
          Top = 60
          Width = 77
          Height = 17
          Caption = 'Auto1'
          TabOrder = 0
        end
        object chkAuto2_OS: TCheckBox
          Left = 112
          Top = 60
          Width = 77
          Height = 17
          Caption = 'Auto2'
          TabOrder = 1
        end
        object chkAuto3_OS: TCheckBox
          Left = 212
          Top = 60
          Width = 77
          Height = 17
          Caption = 'Auto3'
          TabOrder = 2
        end
        object btnSave: TButton
          Left = 425
          Top = 52
          Width = 90
          Height = 29
          Caption = 'Save'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = btnSaveClick
        end
      end
      object gbTCPIP: TGroupBox
        Left = 0
        Top = 0
        Width = 840
        Height = 120
        Align = alTop
        Caption = 'TCP/IP'
        Color = 8421440
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clMenu
        Font.Height = -19
        Font.Name = 'Arial'
        Font.Style = []
        ParentColor = False
        ParentFont = False
        TabOrder = 0
        object Label23: TLabel
          Left = 9
          Top = 22
          Width = 59
          Height = 18
          Caption = 'Address'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 143
          Top = 22
          Width = 29
          Height = 18
          Caption = 'Port'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindow
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
        end
        object edTCPIP_Address: TEdit
          Left = 7
          Top = 46
          Width = 130
          Height = 26
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 0
          Text = '192.168.1.50'
        end
        object edTCPIP_Port: TEdit
          Left = 142
          Top = 46
          Width = 60
          Height = 26
          Enabled = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          ReadOnly = True
          TabOrder = 1
          Text = '6000'
        end
        object btTCPIP_Connect: TBitBtn
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
          TabOrder = 3
          OnClick = btTCPIP_ConnectClick
        end
        object btTCPIP_Disconnect: TBitBtn
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
          TabOrder = 4
          OnClick = btTCPIP_DisconnectClick
        end
        object btTCPIP_Trigger: TButton
          Left = 297
          Top = 76
          Width = 90
          Height = 29
          Caption = 'Send CMD'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          OnClick = btTCPIP_TriggerClick
        end
        object edTCPIP_CMD: TEdit
          Left = 215
          Top = 46
          Width = 171
          Height = 26
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
      end
    end
    object tsTestResult: TTabSheet
      Caption = 'Test Result'
      ImageIndex = 2
      object pnlLeft: TPanel
        Left = 0
        Top = 0
        Width = 833
        Height = 810
        Align = alLeft
        BevelOuter = bvNone
        Color = 8421440
        TabOrder = 0
        object grpTempData: TGroupBox
          Left = 0
          Top = 0
          Width = 833
          Height = 229
          Align = alTop
          Caption = 'Temp Data'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object SourceMemo: TMemo
            Left = 2
            Top = 22
            Width = 829
            Height = 205
            Align = alClient
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            ScrollBars = ssBoth
            TabOrder = 0
          end
        end
        object grpSummary: TGroupBox
          Left = 0
          Top = 229
          Width = 833
          Height = 581
          Align = alClient
          Caption = 'Summary'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object redtSummary: TRichEdit
            Left = 2
            Top = 22
            Width = 829
            Height = 557
            Align = alClient
            Color = clWhite
            Font.Charset = CHINESEBIG5_CHARSET
            Font.Color = clBlack
            Font.Height = -11
            Font.Name = 'Courier New'
            Font.Style = []
            ParentFont = False
            ScrollBars = ssBoth
            TabOrder = 0
          end
        end
      end
    end
    object tsTestResult2: TTabSheet
      Caption = 'Test Result2'
      ImageIndex = 3
      object scrlbxSummary: TScrollBox
        Left = 0
        Top = 0
        Width = 840
        Height = 739
        Align = alClient
        TabOrder = 0
      end
      object rgUnloader: TRadioGroup
        Left = 0
        Top = 739
        Width = 840
        Height = 71
        Align = alBottom
        Columns = 4
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ItemIndex = 0
        Items.Strings = (
          'All'
          'Auto 1'
          'Auto 2'
          'Auto 3')
        ParentFont = False
        TabOrder = 1
        OnClick = rgUnloaderClick
      end
    end
  end
  object TimerTCPIPConnect: TTimer
    Enabled = False
    OnTimer = TimerTCPIPConnectTimer
    Left = 672
  end
  object ClientSocket_TCPIP: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket_TCPIPConnect
    OnDisconnect = ClientSocket_TCPIPDisconnect
    OnRead = ClientSocket_TCPIPRead
    OnError = ClientSocket_TCPIPError
    Left = 704
    Top = 4
  end
  object TimerProcessTCPData: TTimer
    Enabled = False
    Interval = 1
    OnTimer = TimerProcessTCPDataTimer
    Left = 643
    Top = 1
  end
end
