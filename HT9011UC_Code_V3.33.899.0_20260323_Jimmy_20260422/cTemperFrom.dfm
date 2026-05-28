object fTemperFrom: TfTemperFrom
  Left = 154
  Top = 197
  BorderIcons = []
  BorderStyle = bsNone
  Caption = 'fTemperFrom'
  ClientHeight = 421
  ClientWidth = 1192
  Color = clBtnFace
  DragKind = dkDock
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
  object palLed: TPanel
    Left = 1170
    Top = 0
    Width = 22
    Height = 421
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object Panel73: TPanel
      Left = 5
      Top = 48
      Width = 12
      Height = 15
      BevelOuter = bvNone
      Color = clYellow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnMouseDown = Panel73MouseDown
    end
    object Panel72: TPanel
      Left = 5
      Top = 28
      Width = 12
      Height = 15
      BevelOuter = bvNone
      Color = 2468626
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnMouseDown = Panel72MouseDown
    end
    object Panel71: TPanel
      Left = 5
      Top = 8
      Width = 12
      Height = 15
      BevelOuter = bvNone
      Color = 8388863
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlack
      Font.Height = -12
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnMouseDown = Panel71MouseDown
    end
  end
  object pnlLeft: TPanel
    Left = 0
    Top = 0
    Width = 1170
    Height = 421
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
    object gbFunctionWindow: TGroupBox
      Left = 0
      Top = 0
      Width = 1170
      Height = 53
      Align = alTop
      TabOrder = 0
      object palErrorRecovery: TPanel
        Left = 4
        Top = 6
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Error Recovery On'
        TabOrder = 0
      end
      object palDoubleDevice: TPanel
        Left = 150
        Top = 6
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Double Device On'
        TabOrder = 1
      end
      object palCGoodBin: TPanel
        Left = 296
        Top = 6
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Continue Good Bin On'
        TabOrder = 2
      end
      object palYieldMonitor: TPanel
        Left = 442
        Top = 6
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Yield Monitoring On'
        TabOrder = 3
      end
      object palConsAlarm: TPanel
        Left = 588
        Top = 6
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Consecutive Alarm On'
        TabOrder = 4
      end
      object palIndexStartErr: TPanel
        Left = 4
        Top = 27
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Index Start Error On'
        TabOrder = 5
      end
      object palOcrOn: TPanel
        Left = 150
        Top = 27
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 6
      end
      object pnlQAMode: TPanel
        Left = 296
        Top = 27
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        TabOrder = 7
      end
      object palTest1: TPanel
        Left = 442
        Top = 27
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Test 1'
        TabOrder = 8
      end
      object palTest2: TPanel
        Left = 588
        Top = 27
        Width = 147
        Height = 22
        BevelInner = bvLowered
        BevelOuter = bvNone
        Caption = 'Test 2'
        TabOrder = 9
      end
    end
    object palNewTempWindow: TPanel
      Left = 0
      Top = 122
      Width = 1170
      Height = 139
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 3
      object pnl16Heater: TPanel
        Left = 0
        Top = 0
        Width = 1170
        Height = 69
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 0
        object grpIndex: TGroupBox
          Left = 750
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Index'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
          object hlNameChamber_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Chamber'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameDut_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Socket'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempChamber_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempDut_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object gbShuttle: TGroupBox
          Left = 132
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Shuttle'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          object hlNameShuttle1_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Shut 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameShuttle2_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Shut 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempShuttle1_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempShuttle2_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object gbPlate: TGroupBox
          Left = 0
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Hot Plate'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object hlNamePlate1_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Plate 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNamePlate2_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Plate 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempPlate1_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempPlate2_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object gbArm1: TGroupBox
          Left = 264
          Top = 0
          Width = 243
          Height = 69
          Align = alLeft
          Caption = 'Arm1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          object hlNameAa1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Aa 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameAb1: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ab 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlNameAc1: TPanel
            Left = 120
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ac 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlNameAd1: TPanel
            Left = 179
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ad 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlNameBa1: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ba 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object hlNameBb1: TPanel
            Left = 61
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bb 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object hlNameBc1: TPanel
            Left = 120
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bc 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object hlNameBd1: TPanel
            Left = 179
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bd 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object hlTempAa1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlTempAb1: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlTempAc1: TPanel
            Left = 120
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object hlTempAd1: TPanel
            Left = 179
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlTempBa1: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object hlTempBb1: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object hlTempBc1: TPanel
            Left = 120
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object hlTempBd1: TPanel
            Left = 179
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
          end
        end
        object gbArm2: TGroupBox
          Left = 507
          Top = 0
          Width = 243
          Height = 69
          Align = alLeft
          Caption = 'Arm2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
          object hlNameAc2: TPanel
            Left = 120
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ac 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlNameAa2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Aa 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameAb2: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ab 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempAa2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlTempAb2: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlTempAc2: TPanel
            Left = 120
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object hlNameAd2: TPanel
            Left = 179
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ad 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlNameBa2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ba 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object hlNameBb2: TPanel
            Left = 61
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bb 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object hlNameBc2: TPanel
            Left = 120
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bc 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object hlNameBd2: TPanel
            Left = 179
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bd 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object hlTempAd2: TPanel
            Left = 179
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlTempBa2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object hlTempBb2: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object hlTempBc2: TPanel
            Left = 120
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object hlTempBd2: TPanel
            Left = 179
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
          end
        end
        object gbHeatGun: TGroupBox
          Left = 948
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Heat Gun'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 9
          object hlNameHeatGun1_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Gun 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameHeatGun2_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Gun 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempHeatGun1_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempHeatGun2_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object gbATCHeatGun: TGroupBox
          Left = 882
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'ATC air'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
          object hlNameATCHeatGun1_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Gun 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameATCHeatGun2_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Gun 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempATCHeatGun1_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempATCHeatGun2_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object grpOutSht: TGroupBox
          Left = 1014
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Out Sht'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 10
          object hlNameOutShuttle1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Out Sht 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameOutShuttle2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Out Sht 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempOutShuttle1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempOutShuttle2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object grp_Shuttle2: TGroupBox
          Left = 198
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Shuttle 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          Visible = False
          object hlNameShuttle_3: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Shut 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameShuttle_4: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Shut 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempShuttle_3: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempShuttle_4: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object grp_HopPlate2: TGroupBox
          Left = 66
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'HP 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          Visible = False
          object hlNamePlate_3: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Plate 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNamePlate_4: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Plate 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempPlate_3: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempPlate_4: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object grpDoor: TGroupBox
          Left = 1080
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'Door'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 11
          object hlNameDoor1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Door 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameDoor2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Door 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempDoor1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempDoor2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
        object grpCCD: TGroupBox
          Left = 816
          Top = 0
          Width = 66
          Height = 69
          Align = alLeft
          Caption = 'CCD'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 7
          object hlNameCCD_3: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'CCD'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempCCD_3: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlNameCCD_2_2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'CCD1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempCCD_2_2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
      end
      object pnl32Heater: TPanel
        Left = 0
        Top = 69
        Width = 1170
        Height = 70
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        object gbArm1_2: TGroupBox
          Left = 374
          Top = 0
          Width = 243
          Height = 70
          Align = alLeft
          Caption = 'Arm1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 3
          object hlNameAe1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ae 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameAf1: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Af 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlNameAg1: TPanel
            Left = 120
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ag 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlNameAh1: TPanel
            Left = 179
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ah 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlNameBe1: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Be 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object hlNameBf1: TPanel
            Left = 61
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bf 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object hlNameBg1: TPanel
            Left = 120
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bg 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object hlNameBh1: TPanel
            Left = 179
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bh 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object hlTempAe1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlTempAf1: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlTempAg1: TPanel
            Left = 120
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object hlTempAh1: TPanel
            Left = 179
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlTempBe1: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object hlTempBf1: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object hlTempBg1: TPanel
            Left = 120
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object hlTempBh1: TPanel
            Left = 179
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
          end
        end
        object gbArm2_2: TGroupBox
          Left = 617
          Top = 0
          Width = 243
          Height = 70
          Align = alLeft
          Caption = 'Arm2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 4
          object hlNameAe2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ae 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameAf2: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Af 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempAe2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlTempAf2: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlNameAg2: TPanel
            Left = 120
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ag 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlNameAh2: TPanel
            Left = 179
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Ah 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlNameBe2: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Be 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object hlNameBf2: TPanel
            Left = 61
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bf 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object hlNameBg2: TPanel
            Left = 120
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bg 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object hlNameBh2: TPanel
            Left = 179
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Bh 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
          object hlTempAh2: TPanel
            Left = 179
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlTempBe2: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 12
          end
          object hlTempBf2: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 13
          end
          object hlTempBg2: TPanel
            Left = 120
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 14
          end
          object hlTempBh2: TPanel
            Left = 179
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 15
          end
          object hlTempAg2: TPanel
            Left = 120
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
        end
        object grp2DID: TGroupBox
          Left = 860
          Top = 0
          Width = 66
          Height = 70
          Align = alLeft
          Caption = '2D'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 5
          object hlName2D_2: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '2D'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTemp2D_2: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object grpLB: TGroupBox
          Left = 926
          Top = 0
          Width = 66
          Height = 70
          Align = alLeft
          Caption = 'L/B'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 6
          object hlNameLB: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'L/B'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempLB: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object grpESD: TGroupBox
          Left = 992
          Top = 0
          Width = 66
          Height = 70
          Align = alLeft
          Caption = 'ESD Air'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 7
          object hlNameESD: TPanel
            Left = 0
            Top = 11
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'ESD'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempESD: TPanel
            Left = 0
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object grpChamber: TGroupBox
          Left = 0
          Top = 0
          Width = 66
          Height = 70
          Align = alLeft
          Caption = 'Chamber'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
          object hlNameChamber_3: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Chamber'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempChamber_3: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
        end
        object grpBase: TGroupBox
          Left = 190
          Top = 0
          Width = 184
          Height = 70
          Align = alLeft
          Caption = 'Base'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 2
          object hlNameBase1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempBase1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlNameBase4: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 4'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 8
          end
          object hlTempBase4: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 9
          end
          object hlNameBase2: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempBase2: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlNameBase5: TPanel
            Left = 61
            Top = 39
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 5'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
          object hlTempBase5: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 10
          end
          object hlNameBase3: TPanel
            Left = 120
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 3'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempBase3: TPanel
            Left = 120
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlNameBase6: TPanel
            Left = 120
            Top = 39
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'Base 6'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlTempBase6: TPanel
            Left = 120
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 11
          end
        end
        object gb_Index: TGroupBox
          Left = 66
          Top = 0
          Width = 124
          Height = 70
          Align = alLeft
          Caption = 'Index'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
          object hlNameDut_A1: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'DUT 1'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlTempDut_A1: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlNameDut_A2: TPanel
            Left = 61
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'DUT 2'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlNameDut_A4: TPanel
            Left = 61
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'DUT 4'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 5
          end
          object hlTempDut_A2: TPanel
            Left = 61
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
          object hlTempDut_A4: TPanel
            Left = 61
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 7
          end
          object hlNameDut_A3: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'DUT 3'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 4
          end
          object hlTempDut_A3: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 6
          end
        end
        object grpLB1: TGroupBox
          Left = 1058
          Top = 0
          Width = 66
          Height = 70
          Align = alLeft
          Caption = 'L/B'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 8
          object hlNameLBUp: TPanel
            Left = 2
            Top = 12
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'L/B Up'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 0
          end
          object hlNameLBDown: TPanel
            Left = 2
            Top = 40
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = 'L/B Down'
            Color = clBtnHighlight
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 2
          end
          object hlTempLBUp: TPanel
            Left = 2
            Top = 24
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 1
          end
          object hlTempLBDown: TPanel
            Left = 2
            Top = 52
            Width = 60
            Height = 13
            BevelOuter = bvNone
            Caption = '130.0'
            Color = 51712
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Arial'
            Font.Style = []
            ParentFont = False
            TabOrder = 3
          end
        end
      end
    end
    object Memo1: TMemo
      Left = 206
      Top = 308
      Width = 469
      Height = 53
      ScrollBars = ssBoth
      TabOrder = 4
      Visible = False
    end
    object Button1: TButton
      Left = 354
      Top = 324
      Width = 75
      Height = 25
      Caption = #38283#22987#35352#37636
      TabOrder = 5
      Visible = False
      OnClick = Button1Click
    end
    object gbOldTempWindow: TPanel
      Left = 0
      Top = 53
      Width = 1170
      Height = 69
      Align = alTop
      BevelInner = bvRaised
      BevelOuter = bvLowered
      TabOrder = 2
      object pnl4Plate1: TPanel
        Left = 2
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 0
        object hlNamePlate1: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Plate 1'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempPlate1: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Plate 1'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Plate2: TPanel
        Left = 59
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 1
        object hlNamePlate2: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Plate 2'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempPlate2: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Plate 2'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Sht1: TPanel
        Left = 116
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 2
        object hlNameShuttle1: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Shut 1'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempShuttle1: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Shuttle 1'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Sht2: TPanel
        Left = 173
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 3
        object hlNameShuttle2: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Shut 2'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempShuttle2: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Shuttle 2'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Head1: TPanel
        Left = 230
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 4
        object hlNameHead12: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 1/2'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHead12: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 1/2'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Head3: TPanel
        Left = 287
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 5
        object hlNameHead34: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 3/4'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHead34: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 3/4'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Head5: TPanel
        Left = 344
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 6
        object hlNameHead56: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 5/6'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHead56: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 5/6'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Head7: TPanel
        Left = 401
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 7
        object hlNameHead78: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 7/8'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHead78: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Head 7/8'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Dut: TPanel
        Left = 458
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 8
        object hlNameDut: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Dut'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempDut: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Dut'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4Chamber: TPanel
        Left = 515
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 9
        object hlNameChamber: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Chamber'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempChamber: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Chamber'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4CCD: TPanel
        Left = 572
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 10
        object hlNameCCD: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'CCD'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempCCD: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'CCD'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4HotGun1: TPanel
        Left = 629
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 11
        object hlNameHeatGun1: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Heat Gun 1'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHeatGun1: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'HeatGun1'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object pnl4HotGun2: TPanel
        Left = 686
        Top = 2
        Width = 57
        Height = 65
        Align = alLeft
        BevelOuter = bvNone
        TabOrder = 12
        object hlNameHeatGun2: TPanel
          Left = 0
          Top = 11
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'Heat Gun 2'
          Color = clBtnHighlight
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object hlTempHeatGun2: TPanel
          Left = 0
          Top = 35
          Width = 57
          Height = 25
          BevelOuter = bvNone
          Caption = 'HeatGun2'
          Color = 51712
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
    end
    object Button6: TButton
      Left = 82
      Top = 5
      Width = 81
      Height = 25
      Caption = 'AlarmSimulator'
      TabOrder = 1
      Visible = False
      OnClick = Button6Click
    end
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 680
    Top = 345
  end
end
