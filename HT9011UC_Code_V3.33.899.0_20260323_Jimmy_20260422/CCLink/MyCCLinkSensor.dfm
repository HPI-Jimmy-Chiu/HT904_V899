object fCCLink: TfCCLink
  Left = 151
  Top = 11
  Width = 1077
  Height = 707
  Caption = 'Shuttle Sensor Utility'
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
    Width = 1061
    Height = 585
    ActivePage = tsSocketBase2
    Align = alClient
    TabIndex = 2
    TabOrder = 0
    TabWidth = 150
    object TabSheet1: TTabSheet
      Caption = 'Sensor Status'
      object palSensorBg1: TPanel
        Left = 0
        Top = 0
        Width = 1053
        Height = 557
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 0
        object palSensorBgL: TPanel
          Left = 0
          Top = 0
          Width = 809
          Height = 557
          Align = alLeft
          BevelOuter = bvNone
          Color = 12761254
          TabOrder = 0
          object palInSht1: TPanel
            Left = 0
            Top = 277
            Width = 809
            Height = 280
            Align = alClient
            BevelInner = bvRaised
            BevelOuter = bvLowered
            Color = 12761254
            TabOrder = 1
            object labInSht1: TLabel
              Left = 2
              Top = 2
              Width = 805
              Height = 40
              Align = alTop
              Alignment = taCenter
              Caption = 'In Shuttle 1'
              Font.Charset = ANSI_CHARSET
              Font.Color = clGreen
              Font.Height = -32
              Font.Name = 'Trebuchet MS'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label2: TLabel
              Left = 4
              Top = 196
              Width = 96
              Height = 24
              Caption = 'Input Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label10: TLabel
              Left = 4
              Top = 164
              Width = 113
              Height = 24
              Caption = 'Setting Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label14: TLabel
              Left = 4
              Top = 135
              Width = 115
              Height = 24
              Caption = 'Current Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label19: TLabel
              Left = 4
              Top = 105
              Width = 115
              Height = 24
              Caption = 'Sensor Status'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label20: TLabel
              Left = 4
              Top = 76
              Width = 90
              Height = 24
              Caption = 'Sensor No.'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object MyLedLane17: TALed
              Left = 660
              Top = 52
              Width = 22
              Height = 22
              LEDStyle = LEDSqLarge
            end
            object edIn1_9: TEdit
              Left = 646
              Top = 196
              Width = 50
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnMouseDown = edIn2_1MouseDown
            end
            object btIn1_9: TButton
              Tag = 16
              Left = 644
              Top = 224
              Width = 57
              Height = 25
              Caption = 'SET'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'OpenSymbol'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 6
              OnMouseDown = btIn2_8MouseDown
            end
            object mtIn3: TTMyTray
              Left = 642
              Top = 72
              Width = 60
              Height = 120
              Color = clWhite
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              XItem = 1
              YItem = 4
            end
            object palInSht1_1: TPanel
              Tag = 3
              Left = 124
              Top = 44
              Width = 517
              Height = 213
              BevelInner = bvLowered
              Color = 9534289
              ParentShowHint = False
              ShowHint = True
              TabOrder = 0
              object ledInSht1_1: TALed
                Left = 31
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_2: TALed
                Left = 93
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_3: TALed
                Left = 154
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_4: TALed
                Left = 216
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_5: TALed
                Left = 278
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_6: TALed
                Left = 340
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_7: TALed
                Left = 401
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht1_8: TALed
                Left = 463
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object btIn1_1: TButton
                Tag = 15
                Left = 12
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 8
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_2: TButton
                Tag = 14
                Left = 75
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 9
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_3: TButton
                Tag = 13
                Left = 137
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 10
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_4: TButton
                Tag = 12
                Left = 200
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 11
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_5: TButton
                Tag = 11
                Left = 262
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 12
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_8: TButton
                Tag = 8
                Left = 450
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 16
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_7: TButton
                Tag = 9
                Left = 387
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 14
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn1_6: TButton
                Tag = 10
                Left = 325
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 13
                OnMouseDown = btIn2_8MouseDown
              end
              object edIn1_8: TEdit
                Left = 452
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 7
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_7: TEdit
                Left = 390
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 6
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_6: TEdit
                Left = 327
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 5
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_5: TEdit
                Left = 265
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_4: TEdit
                Left = 203
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_3: TEdit
                Left = 141
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_2: TEdit
                Left = 78
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn1_1: TEdit
                Left = 16
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object mtIn1: TTMyTray
                Left = 9
                Top = 28
                Width = 500
                Height = 120
                Color = clWhite
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                XItem = 8
                YItem = 4
              end
            end
            object btSetIn1: TButton
              Tag = 1
              Left = 8
              Top = 224
              Width = 105
              Height = 25
              Caption = 'Set In Shuttle 1'
              TabOrder = 4
              OnClick = btSetIn1Click
            end
            object btSave: TButton
              Left = 728
              Top = 168
              Width = 75
              Height = 25
              Caption = 'Save'
              TabOrder = 1
              OnClick = btSaveClick
            end
            object btRead: TButton
              Left = 727
              Top = 208
              Width = 75
              Height = 25
              Caption = 'Read'
              TabOrder = 3
              OnClick = btReadClick
            end
          end
          object palInSht2: TPanel
            Left = 0
            Top = 0
            Width = 809
            Height = 277
            Align = alTop
            BevelInner = bvRaised
            BevelOuter = bvLowered
            Color = 12761254
            TabOrder = 0
            object labInSht2: TLabel
              Left = 2
              Top = 2
              Width = 805
              Height = 40
              Align = alTop
              Alignment = taCenter
              Caption = 'In Shuttle 2'
              Font.Charset = ANSI_CHARSET
              Font.Color = clGreen
              Font.Height = -32
              Font.Name = 'Trebuchet MS'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object Label5: TLabel
              Left = 4
              Top = 164
              Width = 113
              Height = 24
              Caption = 'Setting Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label11: TLabel
              Left = 4
              Top = 135
              Width = 115
              Height = 24
              Caption = 'Current Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label15: TLabel
              Left = 4
              Top = 105
              Width = 115
              Height = 24
              Caption = 'Sensor Status'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label3: TLabel
              Left = 4
              Top = 76
              Width = 90
              Height = 24
              Caption = 'Sensor No.'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object Label18: TLabel
              Left = 4
              Top = 196
              Width = 96
              Height = 24
              Caption = 'Input Value'
              Font.Charset = ANSI_CHARSET
              Font.Color = clBlack
              Font.Height = -19
              Font.Name = 'Trebuchet MS'
              Font.Style = []
              ParentFont = False
            end
            object MyLedLane18: TALed
              Left = 660
              Top = 52
              Width = 22
              Height = 22
              LEDStyle = LEDSqLarge
            end
            object btIn2_9: TButton
              Tag = 17
              Left = 644
              Top = 224
              Width = 57
              Height = 25
              Caption = 'SET'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'OpenSymbol'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 5
              OnMouseDown = btIn2_8MouseDown
            end
            object edIn2_9: TEdit
              Left = 646
              Top = 196
              Width = 50
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 2
              Text = '0'
              OnMouseDown = edIn2_1MouseDown
            end
            object mtIn4: TTMyTray
              Left = 642
              Top = 72
              Width = 60
              Height = 120
              Color = clWhite
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              XItem = 1
              YItem = 4
            end
            object palInSht2_1: TPanel
              Tag = 3
              Left = 124
              Top = 44
              Width = 517
              Height = 213
              BevelInner = bvLowered
              Color = 9534289
              ParentShowHint = False
              ShowHint = True
              TabOrder = 1
              object ledInSht2_1: TALed
                Left = 32
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_2: TALed
                Left = 94
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_3: TALed
                Left = 155
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_4: TALed
                Left = 217
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_5: TALed
                Left = 279
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_6: TALed
                Left = 341
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_7: TALed
                Left = 402
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object ledInSht2_8: TALed
                Left = 464
                Top = 8
                Width = 22
                Height = 22
                LEDStyle = LEDSqLarge
              end
              object btIn2_1: TButton
                Tag = 7
                Left = 12
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 8
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_2: TButton
                Tag = 6
                Left = 75
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 9
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_3: TButton
                Tag = 5
                Left = 137
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 10
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_4: TButton
                Tag = 4
                Left = 200
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 11
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_5: TButton
                Tag = 3
                Left = 262
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 12
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_8: TButton
                Left = 450
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 16
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_7: TButton
                Tag = 1
                Left = 387
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 14
                OnMouseDown = btIn2_8MouseDown
              end
              object btIn2_6: TButton
                Tag = 2
                Left = 325
                Top = 180
                Width = 57
                Height = 25
                Caption = 'SET'
                Font.Charset = ANSI_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'OpenSymbol'
                Font.Style = [fsBold]
                ParentFont = False
                TabOrder = 13
                OnMouseDown = btIn2_8MouseDown
              end
              object edIn2_8: TEdit
                Left = 452
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 7
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_7: TEdit
                Left = 390
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 6
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_6: TEdit
                Left = 327
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 5
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_5: TEdit
                Left = 265
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 4
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_4: TEdit
                Left = 203
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 3
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_3: TEdit
                Left = 141
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 2
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_2: TEdit
                Left = 78
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 1
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object edIn2_1: TEdit
                Left = 16
                Top = 152
                Width = 50
                Height = 24
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -13
                Font.Name = 'MS Sans Serif'
                Font.Style = []
                ParentFont = False
                TabOrder = 0
                Text = '0'
                OnMouseDown = edIn2_1MouseDown
              end
              object mtIn2: TTMyTray
                Left = 9
                Top = 28
                Width = 500
                Height = 120
                Color = clWhite
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clWindowText
                Font.Height = -16
                Font.Name = 'MS Sans Serif'
                Font.Style = [fsBold]
                XItem = 8
                YItem = 4
              end
            end
            object btSetIn2: TButton
              Left = 8
              Top = 224
              Width = 105
              Height = 25
              Caption = 'Set In Shuttle 2'
              TabOrder = 3
              OnClick = btSetIn2Click
            end
            object btShuttlePositionMove: TButton
              Left = 4
              Top = 4
              Width = 133
              Height = 37
              Caption = 'Shuttle Position Move'
              TabOrder = 0
              OnClick = btShuttlePositionMoveClick
            end
          end
        end
        object palSensorBgR: TPanel
          Left = 809
          Top = 0
          Width = 244
          Height = 557
          Align = alClient
          Color = 12761254
          TabOrder = 1
          object palOutSht2: TPanel
            Left = 1
            Top = 1
            Width = 242
            Height = 276
            Align = alTop
            BevelOuter = bvLowered
            Color = 12761254
            TabOrder = 0
            object labOutSht2: TLabel
              Left = 1
              Top = 1
              Width = 240
              Height = 40
              Align = alTop
              Alignment = taCenter
              Caption = 'Out Shuttle 2'
              Font.Charset = ANSI_CHARSET
              Font.Color = clGreen
              Font.Height = -32
              Font.Name = 'Trebuchet MS'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object MyLedLane19: TALed
              Left = 100
              Top = 52
              Width = 22
              Height = 22
              LEDStyle = LEDSqLarge
            end
            object mtOut2: TTMyTray
              Left = 82
              Top = 72
              Width = 60
              Height = 120
              Color = clWhite
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              XItem = 1
              YItem = 4
            end
            object edOut2: TEdit
              Left = 86
              Top = 196
              Width = 50
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnMouseDown = edIn2_1MouseDown
            end
            object btOut2: TButton
              Tag = 19
              Left = 84
              Top = 224
              Width = 57
              Height = 25
              Caption = 'SET'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'OpenSymbol'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
              OnMouseDown = btIn2_8MouseDown
            end
          end
          object palOutSht1: TPanel
            Left = 1
            Top = 277
            Width = 242
            Height = 279
            Align = alClient
            BevelInner = bvRaised
            BevelOuter = bvLowered
            Color = 12761254
            TabOrder = 1
            object labOutSht1: TLabel
              Left = 2
              Top = 2
              Width = 238
              Height = 40
              Align = alTop
              Alignment = taCenter
              Caption = 'Out Shuttle 1'
              Font.Charset = ANSI_CHARSET
              Font.Color = clGreen
              Font.Height = -32
              Font.Name = 'Trebuchet MS'
              Font.Style = [fsBold]
              ParentFont = False
            end
            object MyLedLane20: TALed
              Left = 100
              Top = 52
              Width = 22
              Height = 22
              LEDStyle = LEDSqLarge
            end
            object mtOut1: TTMyTray
              Left = 82
              Top = 72
              Width = 60
              Height = 120
              Color = clWhite
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'MS Sans Serif'
              Font.Style = [fsBold]
              XItem = 1
              YItem = 4
            end
            object edOut1: TEdit
              Left = 86
              Top = 196
              Width = 50
              Height = 24
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clWindowText
              Font.Height = -13
              Font.Name = 'MS Sans Serif'
              Font.Style = []
              ParentFont = False
              TabOrder = 1
              Text = '0'
              OnMouseDown = edIn2_1MouseDown
            end
            object btOut1: TButton
              Tag = 18
              Left = 84
              Top = 224
              Width = 57
              Height = 25
              Caption = 'SET'
              Font.Charset = ANSI_CHARSET
              Font.Color = clWindowText
              Font.Height = -16
              Font.Name = 'OpenSymbol'
              Font.Style = [fsBold]
              ParentFont = False
              TabOrder = 2
              OnMouseDown = btIn2_8MouseDown
            end
          end
        end
      end
    end
    object tsSocketAndRotate: TTabSheet
      Caption = 'Socker Sensor && Rotate'
      ImageIndex = 2
      object palSocketSensor: TPanel
        Left = 0
        Top = 0
        Width = 649
        Height = 281
        Align = alCustom
        Alignment = taLeftJustify
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 12761254
        TabOrder = 0
        object labSocketSensor: TLabel
          Left = 2
          Top = 2
          Width = 645
          Height = 40
          Align = alTop
          Alignment = taCenter
          Caption = 'Socket Base Sensor'
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -32
          Font.Name = 'Trebuchet MS'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label12: TLabel
          Left = 4
          Top = 164
          Width = 113
          Height = 24
          Caption = 'Setting Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label13: TLabel
          Left = 4
          Top = 135
          Width = 115
          Height = 24
          Caption = 'Current Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label16: TLabel
          Left = 4
          Top = 105
          Width = 115
          Height = 24
          Caption = 'Sensor Status'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label17: TLabel
          Left = 4
          Top = 76
          Width = 90
          Height = 24
          Caption = 'Sensor No.'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label21: TLabel
          Left = 4
          Top = 196
          Width = 96
          Height = 24
          Caption = 'Input Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object palSocketSensor1: TPanel
          Tag = 3
          Left = 124
          Top = 44
          Width = 517
          Height = 213
          BevelInner = bvLowered
          Color = 9534289
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          object SocketLED1: TALed
            Left = 32
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED2: TALed
            Left = 94
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED3: TALed
            Left = 155
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED4: TALed
            Left = 217
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED5: TALed
            Left = 279
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED6: TALed
            Left = 341
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED7: TALed
            Left = 402
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED8: TALed
            Left = 464
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object btIn3_1: TButton
            Left = 15
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_2: TButton
            Tag = 1
            Left = 77
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_3: TButton
            Tag = 2
            Left = 138
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_4: TButton
            Tag = 3
            Left = 200
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_5: TButton
            Tag = 4
            Left = 262
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_8: TButton
            Tag = 7
            Left = 448
            Top = 181
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 16
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_7: TButton
            Tag = 6
            Left = 386
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 14
            OnMouseDown = btIn3_1MouseDown
          end
          object btIn3_6: TButton
            Tag = 5
            Left = 325
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            OnMouseDown = btIn3_1MouseDown
          end
          object edIn3_8: TEdit
            Left = 448
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_7: TEdit
            Left = 386
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_6: TEdit
            Left = 325
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_5: TEdit
            Left = 263
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_4: TEdit
            Left = 201
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_3: TEdit
            Left = 138
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_2: TEdit
            Left = 77
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn3_1: TEdit
            Left = 15
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object mtInSocketBase: TTMyTray
            Left = 9
            Top = 28
            Width = 500
            Height = 120
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            XItem = 8
            YItem = 4
          end
        end
        object SetSocket: TButton
          Left = 8
          Top = 224
          Width = 105
          Height = 25
          Caption = 'Set Socket Base'
          TabOrder = 1
          OnClick = SetSocketClick
        end
      end
      object palRotate: TPanel
        Left = 650
        Top = 0
        Width = 403
        Height = 557
        Align = alRight
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 12761254
        TabOrder = 1
        Visible = False
        object Label22: TLabel
          Left = 4
          Top = 76
          Width = 90
          Height = 24
          Caption = 'Sensor No.'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label23: TLabel
          Left = 4
          Top = 106
          Width = 115
          Height = 24
          Caption = 'Sensor Status'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 4
          Top = 136
          Width = 115
          Height = 24
          Caption = 'Current Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label25: TLabel
          Left = 4
          Top = 166
          Width = 113
          Height = 24
          Caption = 'Setting Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label26: TLabel
          Left = 4
          Top = 196
          Width = 96
          Height = 24
          Caption = 'Input Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object labRotateSensor: TLabel
          Left = 2
          Top = 2
          Width = 399
          Height = 40
          Align = alTop
          Alignment = taCenter
          Caption = 'Rotate Sensor'
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -32
          Font.Name = 'Trebuchet MS'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object SetRotate: TButton
          Left = 4
          Top = 224
          Width = 105
          Height = 25
          Caption = 'Set Rotate'
          TabOrder = 1
          OnClick = SetRotateClick
        end
        object palRotate1: TPanel
          Left = 124
          Top = 44
          Width = 273
          Height = 213
          BevelInner = bvLowered
          Color = 9534289
          TabOrder = 0
          object RotateLED1: TALed
            Left = 32
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object RotateLED2: TALed
            Left = 94
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object RotateLED3: TALed
            Left = 155
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object RotateLED4: TALed
            Left = 217
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object edIn3_9: TEdit
            Left = 16
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_9: TButton
            Left = 15
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 5
            OnMouseDown = btIn3_9MouseDown
          end
          object mtInOutRotateSen: TTMyTray
            Left = 9
            Top = 28
            Width = 247
            Height = 121
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            XItem = 4
            YItem = 4
          end
          object edIn3_10: TEdit
            Left = 78
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_10: TButton
            Tag = 1
            Left = 77
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 6
            OnMouseDown = btIn3_9MouseDown
          end
          object edIn3_11: TEdit
            Left = 139
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_11: TButton
            Tag = 2
            Left = 138
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 7
            OnMouseDown = btIn3_9MouseDown
          end
          object edIn3_12: TEdit
            Left = 201
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_12: TButton
            Tag = 3
            Left = 200
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnMouseDown = btIn3_9MouseDown
          end
        end
      end
      object palColorSensor: TPanel
        Left = 0
        Top = 282
        Width = 649
        Height = 268
        Align = alCustom
        Color = 12761254
        TabOrder = 2
        object labColorSensor: TLabel
          Left = 1
          Top = 1
          Width = 647
          Height = 40
          Align = alTop
          Alignment = taCenter
          Caption = 'Color Sensor'
          Color = 12761254
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -32
          Font.Name = 'Trebuchet MS'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object Label4: TLabel
          Left = 4
          Top = 164
          Width = 113
          Height = 24
          Caption = 'Setting Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label6: TLabel
          Left = 4
          Top = 135
          Width = 115
          Height = 24
          Caption = 'Current Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label7: TLabel
          Left = 4
          Top = 105
          Width = 115
          Height = 24
          Caption = 'Sensor Status'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label8: TLabel
          Left = 4
          Top = 76
          Width = 90
          Height = 24
          Caption = 'Sensor No.'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label9: TLabel
          Left = 4
          Top = 196
          Width = 96
          Height = 24
          Caption = 'Input Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object palColorSensor1: TPanel
          Tag = 3
          Left = 124
          Top = 44
          Width = 517
          Height = 213
          BevelInner = bvLowered
          Color = 9534289
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          object ColorLED1: TALed
            Left = 32
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED2: TALed
            Left = 94
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED3: TALed
            Left = 155
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED4: TALed
            Left = 217
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED5: TALed
            Left = 279
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED6: TALed
            Left = 343
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED8: TALed
            Left = 471
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object ColorLED7: TALed
            Left = 407
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object edIn3_13: TEdit
            Left = 16
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_13: TButton
            Left = 15
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            OnMouseDown = btIn3_13MouseDown
          end
          object mtColorSen: TTMyTray
            Left = 9
            Top = 28
            Width = 504
            Height = 121
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            XItem = 8
            YItem = 4
          end
          object edIn3_14: TEdit
            Left = 78
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_14: TButton
            Tag = 1
            Left = 77
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            OnMouseDown = btIn3_13MouseDown
          end
          object edIn3_15: TEdit
            Left = 139
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_15: TButton
            Tag = 2
            Left = 138
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 14
            OnMouseDown = btIn3_13MouseDown
          end
          object edIn3_16: TEdit
            Left = 201
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_16: TButton
            Tag = 3
            Left = 200
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 15
            OnMouseDown = btIn3_13MouseDown
          end
          object edIn3_17: TEdit
            Left = 263
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_17: TButton
            Tag = 4
            Left = 262
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 16
            OnMouseDown = btIn3_13MouseDown
          end
          object edIn3_18: TEdit
            Left = 326
            Top = 153
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_18: TButton
            Tag = 5
            Left = 325
            Top = 180
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            OnMouseDown = btIn3_13MouseDown
          end
          object edIn3_20: TEdit
            Left = 452
            Top = 153
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_20: TButton
            Tag = 7
            Left = 451
            Top = 179
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            OnMouseDown = btIn3_13MouseDown
          end
          object edtIn3_19: TEdit
            Left = 389
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn3_19: TButton
            Tag = 6
            Left = 388
            Top = 179
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnMouseDown = btIn3_13MouseDown
          end
        end
        object SetColor: TButton
          Tag = 2
          Left = 8
          Top = 224
          Width = 105
          Height = 25
          Caption = 'Set Color'
          TabOrder = 1
          OnClick = SetColorClick
        end
      end
    end
    object tsSocketBase2: TTabSheet
      Caption = 'Socket sensor 9-24'
      ImageIndex = 4
      object Panel3: TPanel
        Left = 0
        Top = 0
        Width = 649
        Height = 281
        Align = alCustom
        Alignment = taLeftJustify
        BevelInner = bvRaised
        BevelOuter = bvLowered
        Color = 12761254
        TabOrder = 0
        object Label1: TLabel
          Left = 2
          Top = 2
          Width = 645
          Height = 40
          Align = alTop
          Alignment = taCenter
          Caption = 'Socket Base Sensor 9-16'
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -32
          Font.Name = 'Trebuchet MS'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label27: TLabel
          Left = 4
          Top = 164
          Width = 113
          Height = 24
          Caption = 'Setting Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label28: TLabel
          Left = 4
          Top = 135
          Width = 115
          Height = 24
          Caption = 'Current Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label29: TLabel
          Left = 4
          Top = 105
          Width = 115
          Height = 24
          Caption = 'Sensor Status'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label30: TLabel
          Left = 4
          Top = 76
          Width = 90
          Height = 24
          Caption = 'Sensor No.'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label31: TLabel
          Left = 4
          Top = 196
          Width = 96
          Height = 24
          Caption = 'Input Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Panel4: TPanel
          Tag = 3
          Left = 124
          Top = 44
          Width = 517
          Height = 213
          BevelInner = bvLowered
          Color = 9534289
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          object SocketLED9: TALed
            Left = 32
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED10: TALed
            Left = 94
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED11: TALed
            Left = 155
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED12: TALed
            Left = 217
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED13: TALed
            Left = 279
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED14: TALed
            Left = 341
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED15: TALed
            Left = 402
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED16: TALed
            Left = 464
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object btIn4_1: TButton
            Left = 15
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_2: TButton
            Tag = 1
            Left = 77
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_3: TButton
            Tag = 2
            Left = 138
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_4: TButton
            Tag = 3
            Left = 200
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 11
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_5: TButton
            Tag = 4
            Left = 262
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_8: TButton
            Tag = 7
            Left = 448
            Top = 181
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 16
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_7: TButton
            Tag = 6
            Left = 386
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 14
            OnMouseDown = btIn4_1MouseDown
          end
          object btIn4_6: TButton
            Tag = 5
            Left = 325
            Top = 180
            Width = 57
            Height = 25
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_8: TEdit
            Left = 448
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_7: TEdit
            Left = 386
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_6: TEdit
            Left = 325
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_5: TEdit
            Left = 263
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_4: TEdit
            Left = 201
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_3: TEdit
            Left = 138
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_2: TEdit
            Left = 77
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object edIn4_1: TEdit
            Left = 15
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object mtInSocketBase9_16: TTMyTray
            Left = 9
            Top = 28
            Width = 500
            Height = 120
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            XItem = 8
            YItem = 4
          end
        end
        object setSocket9_24: TButton
          Left = 8
          Top = 224
          Width = 105
          Height = 25
          Caption = 'Set Socket Base'
          TabOrder = 1
        end
      end
      object Panel5: TPanel
        Left = 0
        Top = 282
        Width = 649
        Height = 268
        Align = alCustom
        Color = 12761254
        TabOrder = 1
        object Label32: TLabel
          Left = 1
          Top = 1
          Width = 647
          Height = 40
          Align = alTop
          Alignment = taCenter
          Caption = 'Socket Base Sensor 17-24'
          Color = 12761254
          Font.Charset = ANSI_CHARSET
          Font.Color = clGreen
          Font.Height = -32
          Font.Name = 'Trebuchet MS'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
        end
        object Label33: TLabel
          Left = 4
          Top = 164
          Width = 113
          Height = 24
          Caption = 'Setting Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label34: TLabel
          Left = 4
          Top = 135
          Width = 115
          Height = 24
          Caption = 'Current Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label35: TLabel
          Left = 4
          Top = 105
          Width = 115
          Height = 24
          Caption = 'Sensor Status'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label36: TLabel
          Left = 4
          Top = 76
          Width = 90
          Height = 24
          Caption = 'Sensor No.'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Label37: TLabel
          Left = 4
          Top = 196
          Width = 96
          Height = 24
          Caption = 'Input Value'
          Font.Charset = ANSI_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Trebuchet MS'
          Font.Style = []
          ParentFont = False
        end
        object Panel6: TPanel
          Tag = 3
          Left = 124
          Top = 44
          Width = 517
          Height = 213
          BevelInner = bvLowered
          Color = 9534289
          ParentShowHint = False
          ShowHint = True
          TabOrder = 0
          object SocketLED17: TALed
            Left = 32
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED18: TALed
            Left = 94
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED19: TALed
            Left = 155
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED20: TALed
            Left = 217
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED21: TALed
            Left = 279
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED22: TALed
            Left = 343
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED24: TALed
            Left = 471
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object SocketLED23: TALed
            Left = 407
            Top = 8
            Width = 22
            Height = 22
            LEDStyle = LEDSqLarge
          end
          object edIn4_9: TEdit
            Left = 16
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_9: TButton
            Tag = 8
            Left = 15
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 12
            OnMouseDown = btIn4_1MouseDown
          end
          object mtInSocketBase17_24: TTMyTray
            Left = 9
            Top = 28
            Width = 504
            Height = 121
            Color = clWhite
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            XItem = 8
            YItem = 4
          end
          object edIn4_10: TEdit
            Left = 78
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_10: TButton
            Tag = 9
            Left = 77
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 13
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_11: TEdit
            Left = 139
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_11: TButton
            Tag = 10
            Left = 138
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 14
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_12: TEdit
            Left = 201
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_12: TButton
            Tag = 11
            Left = 200
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 15
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_13: TEdit
            Left = 263
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_13: TButton
            Tag = 12
            Left = 262
            Top = 181
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 16
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_14: TEdit
            Left = 326
            Top = 153
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_14: TButton
            Tag = 13
            Left = 325
            Top = 180
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 10
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_16: TEdit
            Left = 452
            Top = 153
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_16: TButton
            Tag = 15
            Left = 451
            Top = 179
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 9
            OnMouseDown = btIn4_1MouseDown
          end
          object edIn4_15: TEdit
            Left = 389
            Top = 152
            Width = 55
            Height = 24
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
            Text = '0'
            OnMouseDown = edIn2_1MouseDown
          end
          object btIn4_15: TButton
            Tag = 14
            Left = 388
            Top = 179
            Width = 57
            Height = 26
            Caption = 'SET'
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -16
            Font.Name = 'OpenSymbol'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 8
            OnMouseDown = btIn4_1MouseDown
          end
        end
        object Button1: TButton
          Left = 8
          Top = 224
          Width = 105
          Height = 25
          Caption = 'Set Socket Base'
          TabOrder = 1
        end
      end
    end
    object tsCCLinkSetting: TTabSheet
      Caption = 'Settings'
      ImageIndex = 1
      object spbSave: TSpeedButton
        Left = 10
        Top = 240
        Width = 175
        Height = 49
        Caption = 'Save'
        Glyph.Data = {
          FA0E0000424DFA0E000000000000360000002800000024000000230000000100
          180000000000C40E000001000000010000000000000000000000EFEFEFEFEFEF
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
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFECECEAECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFECECEAECECEAE5E5E5DDDDDDE5E5E5ECECEAECECEAEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE5E5E5E3E3E1DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD
          DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD7D7D7CCCCCCD7D7D70083
          3CDDDDDDDDDDDDECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFECECEADDDDDDD7D7D7CCCCCC
          CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC5C5
          C5BDBDBDCCCCCC00833C00833CD7D7D7D7D7D7DDDDDDDDDDDDDDDDDDDDDDDDE3
          E3E1E5E5E5EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFECECEA
          952E7C93297A93297A93297A8E29758E29758E297585216D85216D85216D8521
          6D85216D85216D7C1C64741B5F9A5E8D00833C00BF5900833CD7D7D7DDDDDDDD
          DDDDE3E3E1E3E3E1E3E3E1E3E3E1ECECEAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFE5E5E5DDDDDD9D3083ECECEAECECEAECECEAECECEAECECEAECEC
          EAECECEAECECEAECECEAECECEAECECEAE3E3E1D7D7D7DDDDDD00833C00BF5900
          BF5900833C00833C00833C00833C00833C00833C00833C00833CE5E5E5EFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE3E3E1D7D7D7A33888ECECEAECEC
          EAECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAE3E3E1E3
          E3E100833C00BF5900C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A
          00C45A00833CE5E5E5EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF952E7C952E
          7C8E2975A33888ECECEAECECEAECECEAECECEAECECEAECECEAECECEAECECEAEC
          ECEAECECEAECECEAECECEA00833C00C45A00C45A00C45A00C45A00C45A00C45A
          00C45A00C45A00C45A00C45A00C45A00833CE5E5E5EFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEF9D3083ECECEAE3E3E1A33888ECECEAECECEAECECEAECECEAEC
          ECEAECECEAECECEAECECEAECECEAECECEAF3F2F200833C79E6AA00C45A00C45A
          00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C45A00833CECEC
          EAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA33888ECECEAE5E5E5A93D8EEF
          EFEFEFEFEFEFEFEFC5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBD
          CCCCCC008B4079E6AA00C45A00C45A00C45A00C45A00C45A00C45A00C45A00C4
          5A00C45A00C45A00833CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA3
          3888ECECEAE5E5E5AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2
          F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2008B4079E6AA00C45A00C45A79E6
          AA79E6AA79E6AA79E6AA79E6AA79E6AA79E6AA00833CF7F7F7EFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFA33888ECECEAE5E5E5A93D8EEFEFEFEFEFEFEFEFEF
          C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDB5B5B5B5B5B5B5B5B5F3F2
          F2008B4079E6AA00C45A00833C00833C00833C00833C00833C00833C00833C00
          833CFBFBFAEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA93D8EEFEFEFECECEA
          AE4193F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2
          F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2008B4079E6AA00833CECECEAF7F7F7FB
          FBFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFAE4193F3F2F2ECECEAB8489CF3F2F2F3F2F2F3F2F2C5C5C5C5C5C5C5C5
          C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBDF3F2F2F3F2F2F3F2F200
          833C00833CEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFA93D8EEFEFEFECECEABC4C9FF3F2F2F3F2
          F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3
          F2F2F3F2F2F3F2F2F3F2F2B66EA400833CF3F2F2EFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFAE4193F3F2
          F2ECECEAC452A7F7F7F7F7F7F7F7F7F7C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5C5
          C5C5BDBDBDBDBDBDBDBDBDBDBDBDF7F7F7F7F7F7F7F7F79D3083E5E5E5F3F2F2
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFB8489CF3F2F2ECECEAC956ABF7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7
          F7F7F7A33888DDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFBC4C9FF3F2F2EFEFEFCD58B0FB
          FBFAFBFBFAFBFBFAD461B6CD58B0C452A7B8489CAE4193AE4193A338889D3083
          93297A8C2373FBFBFAFBFBFAFBFBFAA93D8EDDDDDDEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFC4
          52A7F7F7F7F3F2F2D461B6FBFBFAFBFBFAFBFBFADD6CBEEB89D0EB89D0E982CE
          E87DCDE87DCDE87DCDE87DCDE87DCD9D3083FBFBFAFBFBFAFBFBFAAE4193DDDD
          DDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFC956ABF7F7F7F3F2F2DC65BDFBFBFAFBFBFAFBFBFA
          E56CC6ED8AD3ED8AD3E982CEE982CEE982CEE87DCDE87DCDE87DCD9D3083FBFB
          FAFBFBFAFBFBFAB8489CDDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFCD58B0FBFBFAF3F2F2
          E068C1FFFFFFFFFFFFFFFFFFE56CC6E068C1D461B6CD58B0C452A7C452A7B848
          9CAE4193A6398C9D3083FFFFFFFFFFFFFFFFFFBD4DA1DDDDDDEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFD461B6FBFBFAF3F2F2E068C1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC4
          52A7DDDDDDEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFDC65BDFBFBFAF7F7F7E56CC6FFFFFFFFFF
          FFFFFFFFC5C5C5C5C5C5C5C5C5C5C5C5BDBDBDBDBDBDBDBDBDBDBDBDBDBDBDBD
          BDBDFFFFFFFFFFFFFFFFFFC956ABE3E3E1EFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE068C1FFFF
          FFFBFBFAE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E5E5E5EFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFE068C1FFFFFFFFFFFFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFD15BB2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFE56CC6E5
          6CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E068C1E068C1
          E068C1DC65BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE5
          6CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15BB2E5E5E5EFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFE56CC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
          FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD15B
          B2EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE56CC6E56CC6E56CC6
          E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E56CC6E068C1E068C1E068C1DC65
          BDDC65BDDC65BDDC65BDD461B6EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF
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
          EFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEF}
        OnClick = spbSaveClick
      end
      object rgInputMethod: TRadioGroup
        Left = 12
        Top = 12
        Width = 173
        Height = 121
        Caption = 'Input Method'
        ItemIndex = 0
        Items.Strings = (
          'Manual Input'
          'By Percentage'
          'By Default Value')
        TabOrder = 1
      end
      object gbPercentage: TGroupBox
        Left = 12
        Top = 144
        Width = 173
        Height = 85
        Caption = 'Percentage'
        TabOrder = 2
        object labPercentage: TLabel
          Left = 108
          Top = 40
          Width = 29
          Height = 13
          AutoSize = False
          Caption = '%'
        end
        object edPercentage: TEdit
          Left = 24
          Top = 36
          Width = 81
          Height = 21
          TabOrder = 0
          Text = '90'
          OnMouseDown = edPercentageMouseDown
        end
      end
      object gbDefaultValue: TGroupBox
        Left = 204
        Top = 144
        Width = 173
        Height = 85
        Caption = 'Default Value'
        TabOrder = 3
        object edDefaultValue: TEdit
          Left = 24
          Top = 36
          Width = 81
          Height = 21
          TabOrder = 0
          Text = '1480'
          OnMouseDown = edDefaultValueMouseDown
        end
      end
      object Panel2: TPanel
        Left = 656
        Top = 0
        Width = 309
        Height = 561
        Color = 12761254
        TabOrder = 0
        object Panel1: TPanel
          Left = 1
          Top = 1
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'NU-DN1'
          Color = 11444360
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object palAmplifier0: TPanel
          Left = 1
          Top = 34
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Socket1'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
        object palAmplifier1: TPanel
          Left = 1
          Top = 67
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Socket2'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
        end
        object palAmplifier2: TPanel
          Left = 1
          Top = 100
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Socket3'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
        end
        object palAmplifier3: TPanel
          Left = 1
          Top = 133
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Socket4'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
        end
        object palAmplifier4: TPanel
          Left = 1
          Top = 166
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Rotate1'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
        end
        object palAmplifier5: TPanel
          Left = 1
          Top = 199
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Rotate2'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
        end
        object palAmplifier6: TPanel
          Left = 1
          Top = 232
          Width = 307
          Height = 29
          Align = alTop
          Caption = 'Rotate3'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 7
        end
        object palAmplifier7: TPanel
          Left = 1
          Top = 261
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Rotate4'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
        end
        object palAmplifier8: TPanel
          Left = 1
          Top = 294
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color1'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 9
        end
        object palAmplifier9: TPanel
          Left = 1
          Top = 327
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color2'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 10
        end
        object palAmplifier10: TPanel
          Left = 1
          Top = 360
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color3'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 11
        end
        object palAmplifier11: TPanel
          Left = 1
          Top = 393
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color4'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 12
        end
        object palAmplifier12: TPanel
          Left = 1
          Top = 426
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color5'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 13
        end
        object palAmplifier13: TPanel
          Left = 1
          Top = 459
          Width = 307
          Height = 33
          Align = alTop
          Caption = 'Color6'
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 14
        end
        object palAmplifier14: TPanel
          Left = 1
          Top = 492
          Width = 307
          Height = 33
          Align = alTop
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 15
        end
        object palAmplifier15: TPanel
          Left = 1
          Top = 525
          Width = 307
          Height = 33
          Align = alTop
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 16
        end
      end
      object grpCanBusSeach: TGroupBox
        Left = 200
        Top = 240
        Width = 449
        Height = 313
        Caption = 'Can Bus Hardware Check'
        TabOrder = 4
        object btnCanBusSeach: TButton
          Left = 8
          Top = 16
          Width = 89
          Height = 33
          Caption = 'Check'
          TabOrder = 0
          OnClick = btnCanBusSeachClick
        end
        object mmoCanBusLog: TMemo
          Left = 8
          Top = 56
          Width = 425
          Height = 249
          Color = 14670284
          TabOrder = 2
        end
        object btnReadCanBusSetting: TButton
          Left = 104
          Top = 16
          Width = 89
          Height = 33
          Caption = 'Read Setting'
          TabOrder = 1
          OnClick = btnReadCanBusSettingClick
        end
      end
    end
    object tsErrMess: TTabSheet
      Caption = 'Error Message'
      ImageIndex = 3
      object mmoErrMess: TMemo
        Left = 0
        Top = 0
        Width = 1053
        Height = 557
        Align = alClient
        Color = 14670284
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Courier New'
        Font.Style = []
        Lines.Strings = (
          'Wr0000/Wr02 Error Message'
          
            '  0 : Successfully completed. The command has been processed suc' +
            'cessfully.'
          '  1 : Command number error. The command number is out of range.'
          
            '  2 : Data category error. The data category value is out of ran' +
            'ge.'
          
            '  3 : ID number out of range. A nonexisting ID number is specifi' +
            'ed.'
          '  4 : Data number out of range. The data number is out of range.'
          
            '  5 : Write inhibit. An attempt was made to write in a read-only' +
            ' data number.'
          
            '  6 : Write data out of range. The value of write data is out of' +
            ' range.'
          
            '  7 : Sensor-to-sensor communication error. An error occurred in' +
            ' the communication between sensors. Example: Noise, improper con' +
            'tact,etc.'
          
            '  8 : Motion command area access error.  An attempt was made to ' +
            'read/write to a motion command area.'
          
            '  9 : Motion command address error.  The address is out of range' +
            '. (The address is not usedwith this sensor.'
          
            ' 10 : Motion command execution error.  Failed to execute a motio' +
            'n command.'
          
            ' 11 : Read inhibit.  An attempt was made to read data of a write' +
            '-only data number.'
          ''
          ''
          'CCLink Status Error Message'
          ' 0x68 : Get Own Station Parameter Status Error!!'
          ' 0x69 : Get the loading status Error!!'
          ' 0x6A : Get Station number duplicate check Error!!'
          ' 0x71 : Get Number of Modules Error!!'
          ' 0x72 : Get Number of Stations Error!!'
          ' 0x90 : Get Cables Status Error!!'
          '')
        ParentFont = False
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
  end
  object palButton: TPanel
    Left = 0
    Top = 585
    Width = 1061
    Height = 83
    Align = alBottom
    BevelInner = bvRaised
    BevelOuter = bvLowered
    Color = 12761254
    TabOrder = 1
    object sbExit: TSpeedButton
      Tag = 2
      Left = 894
      Top = 12
      Width = 160
      Height = 41
      AllowAllUp = True
      GroupIndex = 1
      Caption = 'Exit'
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -16
      Font.Name = 'OpenSymbol'
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
      OnClick = sbExitClick
    end
    object sbReset: TSpeedButton
      Left = 720
      Top = 12
      Width = 160
      Height = 41
      Caption = 'Reset CC-Link'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'OpenSymbol'
      Font.Style = []
      Glyph.Data = {
        86080000424D86080000000000003600000028000000190000001C0000000100
        1800000000005008000001000000010000000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFDAA677E4D0A1FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBD7637FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCF7D2FDFA454D6995BE6CCA8FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        D28130E4AA5EC67D2BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC67A
        27F2E88AFFC27BD6A458DCB392FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFD38128E9BC73FFD482DFAF61C67934FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFC98036FFDF85F9D484FFCE79ECB162E3C095FFFFFFFF
        FFFFFFFFFFF4FFFFFFFFFFFFFFFFFFFFFFFFD17F38ECD2AEF6D499FFD27AFFCE
        7DD3A962CE822FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCA813DFFDF7BFBD27DFF
        D280FFD571D6A35AE2CFB4FFFFFFFFFFFFF9FFFFFFFFFFFFFFFFFFC38238E9D9
        C9FFE9C0FFD68FFFBE7BFAC777F9CE7DE6AD52C28339FFFFFFFFFFFFFFFFFFFF
        FFFFCE822FF0D27FFFD288FCCD7BFBD37AFDCD75D59E65FFFFFFFFFFFFFFFFFF
        FFFFFFFFCB8B44FFE5C6FFF5D4F9E2B5FFC98EFFC775F9C071FFCC71FFD47CE6
        B157CF8446FFFFFFFFFFFFFFFFFFEBC49DDC9649F7BE81FBD27DF5CC7DFFDB89
        E4A963E6D0ADFFFFFFFFFFFFFFCC8C45FFE7BFFDEFCBFAE8CBFFE5C3FDD495F4
        C16BFFC873FCC77EFECD7FFFDF8FE6B462CE8A3FFFFFFFFFFFFFF1FEF6F0D7C3
        E09850F2CF7FFFD685FFD188E4C17FDAB387FFFFFFFFFFFFFFF4DFB2FFF7C4FC
        EBB2FCF0CCFCE5C5FFD5A1F5C476FFC681FFD377F7D593FFDE99FFF19CE3BE6A
        DD8B3DFFFFFFF3FCFFFFFFFBECC898EBB76BFFD989F2D590F4D685DFA45EFFFF
        FFD2FFFFFFFFFFFFFFFFFFFFFFFFECC59EFFEBCBFFDA9FFFCD87FCCB7BF3AB5D
        E39045D99149DB9541D68B46DE9153FFFFFFFAFFFFFFFCFEF7E9CCDEA256FFDF
        8FFFDA91F3E397DB9650FFFFFFDEFFFFFFFFFFFFFFFFFFFFFFFFE3B683F8F3D4
        FFE5A2FCD587F9DA85DF9751FEFEF8FFF8FFF2FFF8F3FFFFFFFDF5FFFFFFF9FD
        F8FAFBFFFDFBFBDC9A4CFBE194FFE293FDE6A2D99E58FFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFE7B67EF7F1DAFFE9C1FFD391FBD98DE3A460F4E6D3FCFBFDFFFF
        F7FFF8FBFFFFF9FFFFFFFAFFF7FFFBFFF2E4D8E3A75BFCE599FCECA0FDDA9AE2
        B37BFFFFFFFBFFFFFFFFFFFFFFFFFFFFFFFFE8BF8EFCF0CEF4F3DFF7DD9AFFDE
        90F1C37CEEC8A5FFF9FFFFFFFBFFFEFFF5FDF3FFFFFFF8FDFEFFFCFAE0CDA2F6
        C47CFFE7A1F0E7A4FFDFA1E9BF9AFFFFFFF6FFFFFFFFFFFFFFFFFFFFFFFFFBDA
        C0EFCCAAFFFEEAF3E8ACFFDA9BFBD392E2AC70EEE1BBFCFEEBFAFEFFF9FBFBFF
        FFFFFFFFF6F5DDC1E1AB6CFBDD9CFFE7A9F9F2B9F9CB95EED9C3FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFE3B875FCECC7F2F0C8FFE896FBEFA1F3D691DB
        AD60F8D1ABF8E5DDFFFDF5FCEDDDFACAA8E2AE72FFDE8FF5E8AAFFEDBBFFF3BF
        E7B48CFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD9C3F3D29AFD
        F7E4FFF7D5FFE8A2F9E6A9FFE7A5F5C98AEEB468ECA757E1B56EF8CB88FEE59D
        F8EFAFFEF6BBFFF9CEE4C090FFDCBBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFEFD1AEFAE1B9FFFADFFFFCDBFAEEB8FEE4AEFFEEB4FFEEAF
        FAECAAFDEEAFFDEDB2FFEEB9FFF7C7FFFFD8E8CB8EF5D6A3FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4CEABECC98AFFFFD1
        FFF6F0FDFFDBF4F5CBF6F2C1FEF8C3FDF7C8FEF8D3FBF9E1F6F5E7ECC690E9D4
        B5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFF9DDC5E7B591F2DDB7FFF3D9FFFAE6FFFFF4FFFFF4FFFAE9FFFA
        DDFFE5BCE5C08EFFDFBCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCE1BFFACB97F0BA
        7EE8AB67ECAE68F1BA7DEDC89CEFDDC6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFF}
      Margin = 10
      ParentFont = False
      Spacing = 5
      Visible = False
      OnClick = sbResetClick
    end
    object lbErrMess: TLabel
      Left = 8
      Top = 4
      Width = 697
      Height = 73
      AutoSize = False
      Color = 11444360
      Font.Charset = ANSI_CHARSET
      Font.Color = clRed
      Font.Height = -16
      Font.Name = 'Trebuchet MS'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      Layout = tlCenter
    end
  end
  object Timer1: TTimer
    OnTimer = Timer1Timer
    Left = 900
    Top = 24
  end
  object tmrCanBusSearch: TTimer
    OnTimer = tmrCanBusSearchTimer
    Left = 936
    Top = 24
  end
end
