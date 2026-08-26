object fContactForce: TfContactForce
  Left = 329
  Top = 114
  Width = 870
  Height = 747
  Caption = 'Contact Force Setting'
  Color = 12761254
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 862
    Height = 647
    ActivePage = tsDieForceDynamicKit
    Align = alClient
    TabIndex = 5
    TabOrder = 0
    TabWidth = 150
    object tsDynamicKit: TTabSheet
      Caption = 'Dynamic Kit'
      ImageIndex = 3
      object scrlbxDynamicKit: TScrollBox
        Left = 0
        Top = 0
        Width = 846
        Height = 612
        Align = alClient
        TabOrder = 0
        object gbLoadRate: TGroupBox
          Left = 0
          Top = 0
          Width = 842
          Height = 100
          Align = alTop
          Caption = 'Load rate of xx mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Visible = False
          object lblDiameter: TLabel
            Left = 6
            Top = 31
            Width = 56
            Height = 20
            Caption = '60mm : '
          end
          object lblHotOffset: TLabel
            Left = 464
            Top = 18
            Width = 203
            Height = 20
            Caption = '60mm offset by heater mode'
          end
          object lblDiameter_NS: TLabel
            Left = 6
            Top = 63
            Width = 101
            Height = 20
            Caption = '60mm for NS: '
          end
          object lblContactOffset: TLabel
            Left = 464
            Top = 45
            Width = 146
            Height = 20
            Caption = '60mm contact offset'
          end
          object lblContactOffset_NS: TLabel
            Left = 464
            Top = 73
            Width = 186
            Height = 20
            Caption = '60mm contact offset__NS'
          end
          object trckbrDiameter: TTrackBar
            Left = 108
            Top = 26
            Width = 275
            Height = 35
            Max = 150
            Min = 80
            Orientation = trHorizontal
            Frequency = 1
            Position = 80
            SelEnd = 0
            SelStart = 0
            TabOrder = 1
            TickMarks = tmBottomRight
            TickStyle = tsAuto
          end
          object edtHotOffset: TEdit
            Left = 677
            Top = 14
            Width = 80
            Height = 28
            TabOrder = 0
            OnClick = edtHotOffsetClick
          end
          object trckbrDiameter_NS: TTrackBar
            Left = 108
            Top = 58
            Width = 275
            Height = 35
            Max = 150
            Min = 80
            Orientation = trHorizontal
            Frequency = 1
            Position = 80
            SelEnd = 0
            SelStart = 0
            TabOrder = 4
            TickMarks = tmBottomRight
            TickStyle = tsAuto
          end
          object edtLoadRate: TEdit
            Left = 380
            Top = 26
            Width = 65
            Height = 28
            TabOrder = 2
          end
          object edtLoadRate_NS: TEdit
            Left = 380
            Top = 58
            Width = 65
            Height = 28
            TabOrder = 5
          end
          object edtContactOffset: TEdit
            Left = 677
            Top = 39
            Width = 80
            Height = 28
            TabOrder = 3
            OnClick = edtContactOffsetClick
          end
          object edtContactOffset_NS: TEdit
            Left = 677
            Top = 68
            Width = 80
            Height = 28
            ImeMode = imClose
            TabOrder = 6
            OnClick = edtContactOffset_NSClick
          end
        end
        object grpAddNewKit: TGroupBox
          Left = 0
          Top = 440
          Width = 842
          Height = 168
          Align = alBottom
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object edtCurrentType: TEdit
            Left = 32
            Top = 68
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 0
            Text = '30,40,60,56'
            Visible = False
          end
          object edtVisible: TEdit
            Left = 32
            Top = 104
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 1
            Text = '1,1,1,0'
            Visible = False
          end
        end
      end
    end
    object tsD25: TTabSheet
      Caption = '[D25] EP Load Rate '
      object labD25_1: TLabel
        Left = 16
        Top = 31
        Width = 45
        Height = 16
        Caption = '60mm : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD25_3: TLabel
        Left = 16
        Top = 171
        Width = 45
        Height = 16
        Caption = '30mm : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD25_2: TLabel
        Left = 16
        Top = 103
        Width = 45
        Height = 16
        Caption = '40mm : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD25_4: TLabel
        Left = 136
        Top = 64
        Width = 170
        Height = 16
        Caption = '60mm Offset by heater mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD25_5: TLabel
        Left = 136
        Top = 136
        Width = 170
        Height = 16
        Caption = '40mm Offset by heater mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD25_6: TLabel
        Left = 136
        Top = 208
        Width = 170
        Height = 16
        Caption = '30mm Offset by heater mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD60: TLabel
        Left = 16
        Top = 239
        Width = 45
        Height = 16
        Caption = '56mm : '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object labD60_1: TLabel
        Left = 136
        Top = 276
        Width = 170
        Height = 16
        Caption = '56mm Offset by heater mode'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object tbD25_Index60mm: TTrackBar
        Left = 96
        Top = 26
        Width = 275
        Height = 35
        Max = 150
        Min = 80
        Orientation = trHorizontal
        Frequency = 1
        Position = 80
        SelEnd = 0
        SelStart = 0
        TabOrder = 0
        TickMarks = tmBottomRight
        TickStyle = tsAuto
      end
      object tbD25_Index30mm: TTrackBar
        Left = 96
        Top = 166
        Width = 275
        Height = 35
        Max = 150
        Min = 80
        Orientation = trHorizontal
        Frequency = 1
        Position = 80
        SelEnd = 0
        SelStart = 0
        TabOrder = 4
        TickMarks = tmBottomRight
        TickStyle = tsAuto
      end
      object tbD25_Index40mm: TTrackBar
        Left = 96
        Top = 98
        Width = 275
        Height = 35
        Max = 150
        Min = 80
        Orientation = trHorizontal
        Frequency = 1
        Position = 80
        SelEnd = 0
        SelStart = 0
        TabOrder = 2
        TickMarks = tmBottomRight
        TickStyle = tsAuto
      end
      object edD25_60mm: TEdit
        Left = 322
        Top = 60
        Width = 49
        Height = 21
        TabOrder = 1
        OnMouseDown = edD25_60mmMouseDown
      end
      object edD25_40mm: TEdit
        Left = 322
        Top = 132
        Width = 49
        Height = 21
        TabOrder = 3
        OnMouseDown = edD25_60mmMouseDown
      end
      object edD25_30mm: TEdit
        Left = 322
        Top = 204
        Width = 49
        Height = 21
        TabOrder = 5
        OnMouseDown = edD25_60mmMouseDown
      end
      object tbD60_Index56mm: TTrackBar
        Left = 96
        Top = 234
        Width = 275
        Height = 35
        Max = 150
        Min = 80
        Orientation = trHorizontal
        Frequency = 1
        Position = 80
        SelEnd = 0
        SelStart = 0
        TabOrder = 6
        TickMarks = tmBottomRight
        TickStyle = tsAuto
      end
      object edD60_56mm: TEdit
        Left = 322
        Top = 272
        Width = 49
        Height = 21
        TabOrder = 7
        OnMouseDown = edD25_60mmMouseDown
      end
    end
    object tsNewMethod: TTabSheet
      Caption = 'New Method'
      ImageIndex = 1
      object GroupBox1: TGroupBox
        Left = 0
        Top = 0
        Width = 854
        Height = 117
        Align = alTop
        Caption = '30MM'
        TabOrder = 0
        object lab30mm_10kg: TLabel
          Left = 24
          Top = 28
          Width = 27
          Height = 13
          Caption = '16KG'
        end
        object lab30mm_60kg: TLabel
          Left = 24
          Top = 76
          Width = 27
          Height = 13
          Caption = '64KG'
        end
        object lab30mm10kgNum: TLabel
          Left = 736
          Top = 24
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object lab30mm60kgNum: TLabel
          Left = 736
          Top = 72
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object tb30mm_10kg: TTrackBar
          Left = 60
          Top = 20
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
        object tb30mm_60kg: TTrackBar
          Left = 60
          Top = 68
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 1
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
      end
      object GroupBox2: TGroupBox
        Left = 0
        Top = 117
        Width = 854
        Height = 117
        Align = alTop
        Caption = '40MM'
        TabOrder = 1
        object lab40mm_10kg: TLabel
          Left = 24
          Top = 28
          Width = 27
          Height = 13
          Caption = '16KG'
        end
        object lab40mm_60kg: TLabel
          Left = 24
          Top = 76
          Width = 27
          Height = 13
          Caption = '64KG'
        end
        object lab40mm60kgNum: TLabel
          Left = 736
          Top = 72
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object lab40mm10kgNum: TLabel
          Left = 736
          Top = 24
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object tb40mm_10kg: TTrackBar
          Left = 60
          Top = 20
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
        object tb40mm_60kg: TTrackBar
          Left = 60
          Top = 68
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 1
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
      end
      object GroupBox3: TGroupBox
        Left = 0
        Top = 234
        Width = 854
        Height = 117
        Align = alTop
        Caption = '60MM'
        TabOrder = 2
        object lab60mm_10kg: TLabel
          Left = 24
          Top = 28
          Width = 27
          Height = 13
          Caption = '16KG'
        end
        object lab60mm_60kg: TLabel
          Left = 24
          Top = 76
          Width = 27
          Height = 13
          Caption = '64KG'
        end
        object lab60mm60kgNum: TLabel
          Left = 736
          Top = 72
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object lab60mm10kgNum: TLabel
          Left = 736
          Top = 24
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object tb60mm_10kg: TTrackBar
          Left = 60
          Top = 20
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
        object tb60mm_60kg: TTrackBar
          Left = 60
          Top = 68
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 1
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
      end
      object GroupBox4: TGroupBox
        Left = 0
        Top = 351
        Width = 854
        Height = 117
        Align = alTop
        Caption = '56MM'
        TabOrder = 3
        object lab56mm_10kg: TLabel
          Left = 24
          Top = 28
          Width = 27
          Height = 13
          Caption = '16KG'
        end
        object lab56mm_60kg: TLabel
          Left = 24
          Top = 76
          Width = 27
          Height = 13
          Caption = '64KG'
        end
        object lab56mm60kgNum: TLabel
          Left = 736
          Top = 72
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object lab56mm10kgNum: TLabel
          Left = 736
          Top = 24
          Width = 41
          Height = 13
          AutoSize = False
          Caption = '0'
        end
        object tb56mm_10kg: TTrackBar
          Left = 60
          Top = 20
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 0
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
        object tb56mm_60kg: TTrackBar
          Left = 60
          Top = 68
          Width = 677
          Height = 45
          Max = 4095
          Orientation = trHorizontal
          Frequency = 1
          Position = 0
          SelEnd = 0
          SelStart = 0
          TabOrder = 1
          TickMarks = tmBottomRight
          TickStyle = tsAuto
          OnChange = tb30mm_10kgChange
        end
      end
    end
    object tsEPCalibration: TTabSheet
      Caption = 'EP Calibratiion'
      ImageIndex = 4
      object GroupBox5: TGroupBox
        Left = 132
        Top = 4
        Width = 540
        Height = 140
        Caption = 'EP Setting'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object gbEPSetting: TGroupBox
          Left = 40
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Output'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label11: TLabel
            Left = 8
            Top = 24
            Width = 72
            Height = 20
            Caption = 'Max KPA: '
          end
          object Label13: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min MPA : '
          end
          object edMaxKpa: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '499'
            OnClick = edMaxKpaClick
          end
          object edMinMpa: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            TabOrder = 1
            Text = '0.001'
            OnClick = edMinMpaClick
          end
        end
        object gbEPReadSetting: TGroupBox
          Left = 270
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Feedback'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label12: TLabel
            Left = 8
            Top = 24
            Width = 71
            Height = 20
            Caption = 'Max VDC:'
          end
          object Label1: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min VDC : '
          end
          object edMaxMpaFB: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '5.013'
            OnClick = edMaxMpaFBClick
          end
          object edtMinMpaFB: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            TabOrder = 1
            Text = '0.968'
            OnClick = edMaxMpaFBClick
          end
        end
      end
      object grpEPForDieForce: TGroupBox
        Left = 132
        Top = 152
        Width = 540
        Height = 140
        Caption = 'EP Setting For Die Force'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object GroupBox6: TGroupBox
          Left = 40
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Output'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label2: TLabel
            Left = 8
            Top = 24
            Width = 72
            Height = 20
            Caption = 'Max KPA: '
          end
          object Label3: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min MPA : '
          end
          object edMaxKpaDual: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '499'
            OnClick = edMaxKpaClick
          end
          object edMinMpaDual: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            ImeMode = imClose
            TabOrder = 1
            Text = '0.001'
            OnClick = edMinMpaClick
          end
        end
        object GroupBox7: TGroupBox
          Left = 270
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Feedback'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label4: TLabel
            Left = 8
            Top = 24
            Width = 71
            Height = 20
            Caption = 'Max VDC:'
          end
          object Label5: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min VDC : '
          end
          object edMaxMpaFBDual: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '5.013'
            OnClick = edMaxMpaFBClick
          end
          object edtMinMpaFBDual: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            TabOrder = 1
            Text = '0.968'
            OnClick = edMaxMpaFBClick
          end
        end
      end
      object grpEPForArm2: TGroupBox
        Left = 128
        Top = 300
        Width = 540
        Height = 140
        Caption = 'EP Setting For Arm2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -17
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        object gbEPSetting_1032: TGroupBox
          Left = 40
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Output'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          object Label6: TLabel
            Left = 8
            Top = 24
            Width = 72
            Height = 20
            Caption = 'Max KPA: '
          end
          object Label7: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min MPA : '
          end
          object edMaxKpa_1032: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '499'
            OnClick = edMaxKpaClick
          end
          object edMinMpa_1032: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            TabOrder = 1
            Text = '0.001'
            OnClick = edMinMpaClick
          end
        end
        object gbEPReadSetting_1032: TGroupBox
          Left = 270
          Top = 30
          Width = 225
          Height = 91
          Caption = 'For Feedback'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object Label8: TLabel
            Left = 8
            Top = 24
            Width = 71
            Height = 20
            Caption = 'Max VDC:'
          end
          object Label9: TLabel
            Left = 8
            Top = 56
            Width = 75
            Height = 20
            Caption = 'Min VDC : '
          end
          object edMaxMpaFB_1032: TEdit
            Left = 88
            Top = 20
            Width = 121
            Height = 28
            TabOrder = 0
            Text = '5.013'
            OnClick = edMaxMpaFBClick
          end
          object edtMinMpaFB_1032: TEdit
            Left = 88
            Top = 52
            Width = 121
            Height = 28
            TabOrder = 1
            Text = '0.968'
            OnClick = edMaxMpaFBClick
          end
        end
      end
      object GroupBox8: TGroupBox
        Left = 127
        Top = 450
        Width = 546
        Height = 91
        Caption = 'Calculator'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        object Label10: TLabel
          Left = 31
          Top = 20
          Width = 88
          Height = 20
          Caption = '4 mA(0MPa)'
        end
        object Label14: TLabel
          Left = 181
          Top = 20
          Width = 141
          Height = 20
          Caption = '12.889 mA(0.5MPa)'
        end
        object Label15: TLabel
          Left = 419
          Top = 20
          Width = 114
          Height = 20
          Caption = '20 mA (0.9MPa)'
        end
        object Label16: TLabel
          Left = 128
          Top = 46
          Width = 11
          Height = 20
          Caption = 'V'
        end
        object Label17: TLabel
          Left = 280
          Top = 46
          Width = 11
          Height = 20
          Caption = 'V'
        end
        object Label18: TLabel
          Left = 520
          Top = 46
          Width = 11
          Height = 20
          Caption = 'V'
        end
        object edtMinVol: TEdit
          Left = 32
          Top = 46
          Width = 94
          Height = 28
          TabOrder = 1
        end
        object edtMaxVol: TEdit
          Left = 420
          Top = 46
          Width = 94
          Height = 28
          Enabled = False
          TabOrder = 3
        end
        object edtMidVol: TEdit
          Left = 176
          Top = 46
          Width = 94
          Height = 28
          TabOrder = 2
        end
        object Button1: TButton
          Left = 327
          Top = 42
          Width = 75
          Height = 25
          Caption = 'Convert'
          TabOrder = 0
          OnClick = Button1Click
        end
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'EP load rate one by one'
      ImageIndex = 4
      object scrlbxDynamicKitInd: TScrollBox
        Left = 0
        Top = 0
        Width = 846
        Height = 612
        Align = alClient
        TabOrder = 0
        object gbLoadRateInd: TGroupBox
          Left = 0
          Top = 0
          Width = 842
          Height = 61
          Align = alTop
          Caption = 'Load rate of xx mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Visible = False
          object lblDiameterInd: TLabel
            Left = 6
            Top = 26
            Width = 56
            Height = 20
            Caption = '60mm : '
          end
          object lblContactOffsetInd: TLabel
            Left = 507
            Top = 26
            Width = 146
            Height = 20
            Caption = '60mm contact offset'
          end
          object trckbrDiameterInd: TTrackBar
            Left = 108
            Top = 24
            Width = 275
            Height = 35
            Max = 150
            Min = 80
            Orientation = trHorizontal
            Frequency = 1
            Position = 80
            SelEnd = 0
            SelStart = 0
            TabOrder = 1
            TickMarks = tmBottomRight
            TickStyle = tsAuto
          end
          object edtLoadRateInd: TEdit
            Left = 380
            Top = 24
            Width = 65
            Height = 28
            TabOrder = 2
            OnClick = edtLoadRateIndClick
          end
          object edtContactOffsetInd: TEdit
            Left = 676
            Top = 23
            Width = 80
            Height = 28
            TabOrder = 0
            OnClick = edtContactOffsetClick
          end
        end
        object grpAddNewKitInd: TGroupBox
          Left = 0
          Top = 440
          Width = 842
          Height = 168
          Align = alBottom
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object edtCurrentTypeInd: TEdit
            Left = 32
            Top = 68
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 0
            Text = '30,40,60,56'
            Visible = False
          end
          object edtVisibleInd: TEdit
            Left = 32
            Top = 104
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 1
            Text = '1,1,1,0'
            Visible = False
          end
        end
      end
    end
    object tsDieForceDynamicKit: TTabSheet
      Caption = 'Die Force Dynamic Kit'
      ImageIndex = 4
      object scrlbxDieForceDynamicKit: TScrollBox
        Left = 0
        Top = 0
        Width = 854
        Height = 619
        Align = alClient
        TabOrder = 0
        object gbDieForceLoadRate: TGroupBox
          Left = 0
          Top = 0
          Width = 850
          Height = 65
          Align = alTop
          Caption = 'Load rate of xx mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Visible = False
          object lblDieForceDiameter: TLabel
            Left = 6
            Top = 31
            Width = 56
            Height = 20
            Caption = '60mm : '
          end
          object lblDieForceContactOffset: TLabel
            Left = 464
            Top = 29
            Width = 146
            Height = 20
            Caption = '60mm contact offset'
          end
          object trckbrDieForceDiameter: TTrackBar
            Left = 108
            Top = 26
            Width = 275
            Height = 35
            Max = 150
            Min = 80
            Orientation = trHorizontal
            Frequency = 1
            Position = 80
            SelEnd = 0
            SelStart = 0
            TabOrder = 1
            TickMarks = tmBottomRight
            TickStyle = tsAuto
          end
          object edtDieForceLoadRate: TEdit
            Left = 380
            Top = 26
            Width = 65
            Height = 28
            TabOrder = 2
          end
          object edtDieForceContactOffset: TEdit
            Left = 677
            Top = 23
            Width = 80
            Height = 28
            TabOrder = 0
            OnClick = edtContactOffsetClick
          end
        end
        object grpDieForceAddNewKit: TGroupBox
          Left = 0
          Top = 447
          Width = 850
          Height = 168
          Align = alBottom
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object edtDieForceCurrentType: TEdit
            Left = 32
            Top = 68
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 0
            Text = '30,40,60,56'
            Visible = False
          end
          object edtDieForceVisible: TEdit
            Left = 32
            Top = 104
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 1
            Text = '1,1,1,0'
            Visible = False
          end
        end
      end
    end
    object tsContactHighOffset: TTabSheet
      Caption = 'ContactHighOffset'
      ImageIndex = 5
      object grpATCTempOffset: TGroupBox
        Left = 0
        Top = 0
        Width = 854
        Height = 145
        Align = alTop
        Caption = 'ContactHighOffset'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clNavy
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object lbArm1: TLabel
          Left = 6
          Top = 51
          Width = 46
          Height = 16
          Caption = 'ARM 1 :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lb4: TLabel
          Left = 52
          Top = 21
          Width = 740
          Height = 24
          Caption = 
            ' 120    180    240    300    360    420    480    520    560    ' +
            '600    640   680    720     760     800'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -19
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object lbArm2: TLabel
          Left = 6
          Top = 91
          Width = 46
          Height = 16
          Caption = 'ARM 2 :'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edtArm1Offset_01: TEdit
          Left = 55
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 0
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_02: TEdit
          Left = 105
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 1
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_03: TEdit
          Left = 155
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 2
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_04: TEdit
          Left = 205
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 3
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_05: TEdit
          Left = 255
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 4
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_06: TEdit
          Left = 305
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 5
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_07: TEdit
          Left = 355
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 6
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_01: TEdit
          Left = 55
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 15
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_02: TEdit
          Left = 105
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 16
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_03: TEdit
          Left = 155
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 17
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_04: TEdit
          Left = 205
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 18
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_05: TEdit
          Left = 255
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 19
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_06: TEdit
          Left = 305
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 20
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_07: TEdit
          Left = 355
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 21
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_08: TEdit
          Left = 405
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 7
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_09: TEdit
          Left = 455
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 8
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_10: TEdit
          Left = 505
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 9
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_11: TEdit
          Left = 555
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 10
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_08: TEdit
          Left = 405
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 22
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_09: TEdit
          Left = 455
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 23
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_10: TEdit
          Left = 505
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 24
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_11: TEdit
          Left = 555
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 25
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_12: TEdit
          Left = 605
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 11
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_13: TEdit
          Left = 655
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 12
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_14: TEdit
          Left = 705
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 13
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_12: TEdit
          Left = 605
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 26
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_13: TEdit
          Left = 655
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 27
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_14: TEdit
          Left = 705
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 28
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm1Offset_15: TEdit
          Left = 755
          Top = 51
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 14
          Text = '0'
          OnClick = edtContactOffsetClick
        end
        object edtArm2Offset_15: TEdit
          Left = 755
          Top = 91
          Width = 40
          Height = 24
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clNavy
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          MaxLength = 20
          ParentFont = False
          TabOrder = 29
          Text = '0'
          OnClick = edtContactOffsetClick
        end
      end
    end
    object tsDieForceOneByOneKit: TTabSheet
      Caption = 'tsDieForceOneByOneKit'
      ImageIndex = 7
      object scrlbxDieForceOneByOneDynamicKit: TScrollBox
        Left = 0
        Top = 0
        Width = 846
        Height = 611
        Align = alClient
        TabOrder = 0
        object gbDieForceOneByOneLoadRate: TGroupBox
          Left = 0
          Top = 0
          Width = 842
          Height = 61
          Align = alTop
          Caption = 'Load rate of xx mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          Visible = False
          object lblDieForceOneByOneDiameter: TLabel
            Left = 6
            Top = 26
            Width = 56
            Height = 20
            Caption = '60mm : '
          end
          object lblDieForceOneByOneContactOffset: TLabel
            Left = 515
            Top = 26
            Width = 146
            Height = 20
            Caption = '60mm contact offset'
          end
          object trckbrDieForceOneByOneDiameter: TTrackBar
            Left = 108
            Top = 24
            Width = 275
            Height = 35
            Max = 150
            Min = 80
            Orientation = trHorizontal
            Frequency = 1
            Position = 80
            SelEnd = 0
            SelStart = 0
            TabOrder = 0
            TickMarks = tmBottomRight
            TickStyle = tsAuto
          end
          object edtDieForceOneByOneLoadRate: TEdit
            Left = 380
            Top = 24
            Width = 65
            Height = 28
            TabOrder = 1
            OnClick = edtLoadRateIndClick
          end
          object edtDieForceOneByOneContactOffset: TEdit
            Left = 676
            Top = 23
            Width = 80
            Height = 28
            TabOrder = 2
            OnClick = edtContactOffsetClick
          end
        end
        object grpDieForceOneByOneAddNewKit: TGroupBox
          Left = 0
          Top = 439
          Width = 842
          Height = 168
          Align = alBottom
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          object edtDieForceOneByOneCurrentType: TEdit
            Left = 32
            Top = 68
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 0
            Text = '30,40,60,56'
            Visible = False
          end
          object edtDieForceOneByOneVisible: TEdit
            Left = 32
            Top = 104
            Width = 169
            Height = 28
            Enabled = False
            TabOrder = 1
            Text = '1,1,1,0'
            Visible = False
          end
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 647
    Width = 862
    Height = 69
    Align = alBottom
    Color = 12761254
    TabOrder = 1
    object btSave: TButton
      Left = 44
      Top = 8
      Width = 317
      Height = 53
      Caption = 'Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btSaveClick
    end
    object btExit: TButton
      Left = 516
      Top = 8
      Width = 317
      Height = 53
      Caption = 'Exit'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btExitClick
    end
  end
end
