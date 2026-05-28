object fRecipeDefault: TfRecipeDefault
  Left = 815
  Top = 177
  Width = 552
  Height = 464
  BorderIcons = [biSystemMenu]
  Caption = 'fRecipeDefault'
  Color = clInactiveCaption
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pagecTimer: TPageControl
    Left = 0
    Top = 0
    Width = 536
    Height = 426
    ActivePage = tsTrayArm
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MultiLine = True
    ParentFont = False
    Style = tsButtons
    TabIndex = 4
    TabOrder = 0
    TabWidth = 90
    object tsIndex: TTabSheet
      Caption = 'Index'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      object pnlReceipeDefaultIndex: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbIndexunit_Speed: TLabel
          Left = 11
          Top = 13
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Index_X Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbIndexunit_VaccumCheckTime: TLabel
          Tag = 4
          Left = 11
          Top = 37
          Width = 221
          Height = 20
          AutoSize = False
          Caption = 'Index_Vacuum Check Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbIndexunit_CounterAirOnTime: TLabel
          Tag = 5
          Left = 11
          Top = 61
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Index_Counter Air ON Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbIndexunit_DestoryAgainTime: TLabel
          Tag = 6
          Left = 11
          Top = 86
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Index_Destroy Again Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbIndexunit_DestoryAgainCount: TLabel
          Tag = 7
          Left = 11
          Top = 110
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Index_Destroy Again Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlIndexunit_DestoryAgainCount: TPanel
          Tag = 7
          Left = 240
          Top = 112
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlIndexunit_DestoryAgainTime: TPanel
          Tag = 6
          Left = 240
          Top = 86
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlIndexunit_CounterAirOnTime: TPanel
          Tag = 5
          Left = 240
          Top = 62
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlIndexunit_VaccumCheckTime: TPanel
          Tag = 4
          Left = 240
          Top = 37
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlIndexunit_Speed: TPanel
          Left = 240
          Top = 14
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
      end
    end
    object tsInputArm: TTabSheet
      Caption = 'Input Arm'
      ImageIndex = 1
      object pnlReceipeDefaultInputArm: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbInput_XYSpeed: TLabel
          Tag = 11
          Left = 10
          Top = 14
          Width = 420
          Height = 20
          Hint = 'Input_XYSpeed'
          AutoSize = False
          Caption = 'Input_XYSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ZSpeed: TLabel
          Tag = 12
          Left = 10
          Top = 38
          Width = 420
          Height = 20
          Hint = 'Input_ZSpeed'
          AutoSize = False
          Caption = 'Input_ZSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_PitchSpeed: TLabel
          Tag = 13
          Left = 10
          Top = 62
          Width = 420
          Height = 20
          Hint = 'Input_PitchSpeed'
          AutoSize = False
          Caption = 'Input_PitchSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_XYAcc: TLabel
          Tag = 14
          Left = 10
          Top = 86
          Width = 420
          Height = 20
          Hint = 'Input_XYAcc'
          AutoSize = False
          Caption = 'Input_XYAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ZAcc: TLabel
          Tag = 15
          Left = 10
          Top = 110
          Width = 420
          Height = 20
          Hint = 'Input_ZAcc'
          AutoSize = False
          Caption = 'Input_ZAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_PitchAcc: TLabel
          Tag = 16
          Left = 10
          Top = 134
          Width = 420
          Height = 20
          Hint = 'Input_PitchAcc'
          AutoSize = False
          Caption = 'Input_PitchAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_VacumCheckTime: TLabel
          Tag = 17
          Left = 10
          Top = 158
          Width = 420
          Height = 20
          Hint = 'Input_Vacuum Check Time'
          AutoSize = False
          Caption = 'Input_Vacuum Check Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ArmAirOnTime: TLabel
          Tag = 18
          Left = 10
          Top = 182
          Width = 420
          Height = 20
          Hint = 'Input_ArmAirOnTime'
          AutoSize = False
          Caption = 'Input_ArmAirOnTime'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ArmShuttleWaitTime: TLabel
          Tag = 19
          Left = 10
          Top = 207
          Width = 420
          Height = 20
          Hint = 'Input_ArmShuttle Wait Time'
          AutoSize = False
          Caption = 'Input_ArmShuttle Wait Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ArmDestroyAgainTime: TLabel
          Tag = 20
          Left = 10
          Top = 230
          Width = 420
          Height = 20
          Hint = 'Input_ArmDestroy Again Time'
          AutoSize = False
          Caption = 'Input_ArmDestroy Again Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbInput_ArmDestroyAgainCount: TLabel
          Tag = 21
          Left = 10
          Top = 255
          Width = 420
          Height = 20
          Hint = 'Input_ArmDestroy Again Count'
          AutoSize = False
          Caption = 'Input_ArmDestroy Again Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlInput_ArmDestroyAgainCount: TPanel
          Tag = 21
          Left = 440
          Top = 254
          Width = 60
          Height = 20
          Hint = 'Input_ArmDestroy Again Count'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlInput_ArmDestroyAgainTime: TPanel
          Tag = 20
          Left = 440
          Top = 230
          Width = 60
          Height = 20
          Hint = 'Input_ArmDestroy Again Time'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlInput_ArmShuttleWaitTime: TPanel
          Tag = 19
          Left = 440
          Top = 206
          Width = 60
          Height = 20
          Hint = 'Input_ArmShuttleWaitTime'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlInput_ArmAirOnTime: TPanel
          Tag = 18
          Left = 440
          Top = 182
          Width = 60
          Height = 20
          Hint = 'Input_ArmAirOnTime'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlInput_VacumCheckTime: TPanel
          Tag = 17
          Left = 440
          Top = 158
          Width = 60
          Height = 20
          Hint = 'Input_Vacuum Check Time'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlInput_PitchAcc: TPanel
          Tag = 16
          Left = 440
          Top = 134
          Width = 60
          Height = 20
          Hint = 'Input_PitchAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlInput_ZAcc: TPanel
          Tag = 15
          Left = 440
          Top = 110
          Width = 60
          Height = 20
          Hint = 'Input_ZAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlInput_XYAcc: TPanel
          Tag = 14
          Left = 440
          Top = 86
          Width = 60
          Height = 20
          Hint = 'Input_XYAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pnlInput_PitchSpeed: TPanel
          Tag = 13
          Left = 440
          Top = 62
          Width = 60
          Height = 20
          Hint = 'Input_PitchSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pnlInput_ZSpeed: TPanel
          Tag = 12
          Left = 440
          Top = 38
          Width = 60
          Height = 20
          Hint = 'Input_ZSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pnlInput_XYSpeed: TPanel
          Tag = 11
          Left = 440
          Top = 14
          Width = 60
          Height = 20
          Hint = 'Input_XYSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
      end
    end
    object tsOutputArm: TTabSheet
      Caption = 'Output Arm'
      ImageIndex = 2
      object pnlReceipeDefaultOutputArm: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbOutput_ZSpeed: TLabel
          Tag = 28
          Left = 10
          Top = 38
          Width = 380
          Height = 20
          Hint = 'Output_ZSpeed'
          AutoSize = False
          Caption = 'Output_ZSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_ZAcc: TLabel
          Tag = 30
          Left = 10
          Top = 110
          Width = 380
          Height = 20
          Hint = 'Output_ZAcc'
          AutoSize = False
          Caption = 'Output_ZAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_PitchAcc: TLabel
          Tag = 31
          Left = 10
          Top = 134
          Width = 380
          Height = 20
          Hint = 'Output_PitchAcc'
          AutoSize = False
          Caption = 'Output_PitchAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_InArmSpeed: TLabel
          Tag = 32
          Left = 10
          Top = 158
          Width = 380
          Height = 20
          Hint = 'Output_Vacum Check Time'
          AutoSize = False
          Caption = 'Output_Vacum Check Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_ArmAirOnTime: TLabel
          Tag = 33
          Left = 10
          Top = 182
          Width = 380
          Height = 20
          Hint = 'Output_Arm Air On Time'
          AutoSize = False
          Caption = 'Output_Arm Air On Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_ArmDestroyAgainTime: TLabel
          Tag = 34
          Left = 10
          Top = 206
          Width = 380
          Height = 20
          Hint = 'Output_Arm Destroy Again Time'
          AutoSize = False
          Caption = 'Output_Arm Destroy Again Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_ArmDestroyAgainCount: TLabel
          Tag = 35
          Left = 10
          Top = 230
          Width = 380
          Height = 20
          Hint = 'Output_Arm Destroy Again Count'
          AutoSize = False
          Caption = 'Output_Arm Destroy Again Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_OutXYAcc: TLabel
          Tag = 30
          Left = 10
          Top = 86
          Width = 380
          Height = 20
          Hint = 'Output_OutXYAcc'
          AutoSize = False
          Caption = 'Output_OutXYAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_XYSpeed: TLabel
          Tag = 28
          Left = 10
          Top = 14
          Width = 380
          Height = 20
          Hint = 'Output_XYSpeed'
          AutoSize = False
          Caption = 'Output_XYSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbOutput_PitchSpeed: TLabel
          Tag = 29
          Left = 10
          Top = 62
          Width = 380
          Height = 20
          Hint = 'Output_PitchSpeed'
          AutoSize = False
          Caption = 'Output_PitchSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlOutput_ZSpeed: TPanel
          Tag = 28
          Left = 400
          Top = 38
          Width = 60
          Height = 20
          Hint = 'Output_ZSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlOutput_PitchSpeed: TPanel
          Tag = 29
          Left = 400
          Top = 62
          Width = 60
          Height = 20
          Hint = 'Output_PitchSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlOutput_ZAcc: TPanel
          Tag = 30
          Left = 400
          Top = 110
          Width = 60
          Height = 20
          Hint = 'Output_ZAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlOutput_PitchAcc: TPanel
          Tag = 31
          Left = 400
          Top = 134
          Width = 60
          Height = 20
          Hint = 'Output_PitchAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlOutput_VacumCheckTime: TPanel
          Tag = 32
          Left = 400
          Top = 158
          Width = 60
          Height = 20
          Hint = 'Output_Vacum Check Time'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlOutput_ArmAirOnTime: TPanel
          Tag = 33
          Left = 400
          Top = 182
          Width = 60
          Height = 20
          Hint = 'Output_Arm Air On Time'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlOutput_ArmDestroyAgainTime: TPanel
          Tag = 34
          Left = 400
          Top = 206
          Width = 60
          Height = 20
          Hint = 'Output_Arm Destroy Again Time'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlOutput_ArmDestroyAgainCount: TPanel
          Tag = 35
          Left = 400
          Top = 229
          Width = 60
          Height = 20
          Hint = 'Output_Arm Destroy Again Count'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pnlOutput_OutXYAcc: TPanel
          Tag = 30
          Left = 400
          Top = 86
          Width = 60
          Height = 20
          Hint = 'Output_OutXYAcc'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pnlOutput_XYSpeed: TPanel
          Tag = 28
          Left = 400
          Top = 14
          Width = 60
          Height = 20
          Hint = 'Output_XYSpeed'
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
      end
    end
    object tsShuttle: TTabSheet
      Caption = 'Shuttle'
      ImageIndex = 3
      object pnlReceipeDefaultShuttle: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbShuttle_Shuttle1Speed: TLabel
          Tag = 28
          Left = 10
          Top = 14
          Width = 380
          Height = 20
          Hint = 'Shuttle_Shuttle1 Speed'
          AutoSize = False
          Caption = 'Shuttle_Shuttle1 Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbShuttle_Shuttle2Speed: TLabel
          Tag = 29
          Left = 10
          Top = 38
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'Shuttle_Shuttle2 Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbShuttle_Shuttle1Accel: TLabel
          Tag = 30
          Left = 10
          Top = 62
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'Shuttle_Shuttle1 Accel'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbShuttle_Shuttle2Accel: TLabel
          Tag = 31
          Left = 10
          Top = 86
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'Shuttle_Shuttle2 Accel'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlShuttle_Shuttle2Accel: TPanel
          Tag = 31
          Left = 400
          Top = 86
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlShuttle_Shuttle1Accel: TPanel
          Tag = 30
          Left = 400
          Top = 62
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlShuttle_Shuttle2Speed: TPanel
          Tag = 29
          Left = 400
          Top = 38
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlShuttle_Shuttle1Speed: TPanel
          Tag = 28
          Left = 400
          Top = 14
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
      end
    end
    object tsTrayArm: TTabSheet
      Caption = 'Tray Arm'
      ImageIndex = 4
      object pnlReceipeDefaultTrayArm: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbTrayArm_XSpeed: TLabel
          Tag = 28
          Left = 10
          Top = 14
          Width = 380
          Height = 20
          Hint = 'TrayArm_XSpeed'
          AutoSize = False
          Caption = 'TrayArm_XSpeed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbTrayArm_XAcc: TLabel
          Tag = 29
          Left = 10
          Top = 38
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'TrayArm_XAcc'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbTrayArm_RetryCount: TLabel
          Tag = 30
          Left = 10
          Top = 62
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'TrayArm_Retry Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbTrayArm_VacuumCheckTime: TLabel
          Tag = 31
          Left = 10
          Top = 86
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'TrayArm_Vacuum Check Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbTrayArm_CounterAirONTime: TLabel
          Tag = 31
          Left = 10
          Top = 112
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'TrayArm_Counter Air ON Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbTrayArm_HandDownTime: TLabel
          Tag = 31
          Left = 10
          Top = 137
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'TrayArm_Hand Down Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlTrayArm_VacuumCheckTime: TPanel
          Tag = 31
          Left = 400
          Top = 85
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlTrayArm_RetryCount: TPanel
          Tag = 30
          Left = 400
          Top = 62
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlTrayArm_XAcc: TPanel
          Tag = 29
          Left = 400
          Top = 38
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlTrayArm_XSpeed: TPanel
          Tag = 28
          Left = 400
          Top = 14
          Width = 60
          Height = 22
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlTrayArm_CounterAirONTime: TPanel
          Tag = 31
          Left = 400
          Top = 112
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlTrayArm_HandDownTime: TPanel
          Tag = 31
          Left = 400
          Top = 138
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
      end
    end
    object tsAutoClean: TTabSheet
      Caption = 'Auto Clean'
      ImageIndex = 5
      object pnlReceipeDefaultAutoClean: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lbAutoClean_AlarmCount: TLabel
          Tag = 28
          Left = 10
          Top = 14
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Alarm Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_OutArmSpeed: TLabel
          Tag = 29
          Left = 10
          Top = 38
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_OutArm Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_ShuttleSpeed: TLabel
          Tag = 30
          Left = 10
          Top = 62
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Shuttle Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_RotationStageSpeed: TLabel
          Tag = 31
          Left = 10
          Top = 86
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Rotation Stage Speed'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_ContactTime: TLabel
          Tag = 31
          Left = 10
          Top = 112
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Contact Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lbAutoClean_ContactCount: TLabel
          Tag = 31
          Left = 10
          Top = 137
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Contact Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAutoCleaning_ModeIntervalContactCount: TLabel
          Tag = 31
          Left = 10
          Top = 161
          Width = 380
          Height = 20
          AutoSize = False
          Caption = 'AutoClean_Cleaning Mode Interval Contact Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAutoCleaning_ModeSocketAlarmCount: TLabel
          Tag = 31
          Left = 10
          Top = 185
          Width = 380
          Height = 20
          Hint = 'AutoClean_Cleaning Mode Socket Alarm Count'
          AutoSize = False
          Caption = 'AutoClean_Cleaning Mode Socket Alarm Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlAutoClean_RotationStageSpeed: TPanel
          Tag = 31
          Left = 400
          Top = 85
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlAutoClean_ShuttleSpeed: TPanel
          Tag = 30
          Left = 400
          Top = 62
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlAutoClean_OutArmSpeed: TPanel
          Tag = 29
          Left = 400
          Top = 38
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlAutoClean_AlarmCount: TPanel
          Tag = 28
          Left = 400
          Top = 14
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlAutoClean_ContactTime: TPanel
          Tag = 31
          Left = 400
          Top = 112
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlAutoClean_ContactCount: TPanel
          Tag = 31
          Left = 400
          Top = 138
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlAutoCleaning_ModeIntervalContactCount: TPanel
          Tag = 31
          Left = 400
          Top = 162
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlAutoCleaning_ModeSocketAlarmCount: TPanel
          Tag = 31
          Left = 400
          Top = 186
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -16
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
      end
    end
    object tsYield: TTabSheet
      Caption = 'Yield'
      ImageIndex = 6
      object pnlReceipeDefaultYield: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lblLowYieldEnable_FT: TLabel
          Left = 11
          Top = 13
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Normal Low Yield Enable'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblLowYield_FT: TLabel
          Tag = 4
          Left = 11
          Top = 37
          Width = 374
          Height = 20
          AutoSize = False
          Caption = 'Yield_Normal Low Yield'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblLowYieldIg: TLabel
          Tag = 5
          Left = 11
          Top = 61
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Normal Low Yield Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblContsFailSocketAlarmCT_FT: TLabel
          Tag = 6
          Left = 11
          Top = 86
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Normal Consecutive Failure Alarm By Socket'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblContsFailHeadAlarmCT_FT: TLabel
          Tag = 7
          Left = 11
          Top = 110
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Normal Consecutive Failure Alarm By Head'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4IntervalYieldYield: TLabel
          Tag = 7
          Left = 11
          Top = 230
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Interval Total Yield Different'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4IntervalYieldIntervalCount: TLabel
          Tag = 7
          Left = 11
          Top = 254
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Interval Total Yield Different Interval Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4IntervalYieldContinueCount: TLabel
          Tag = 7
          Left = 11
          Top = 278
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Interval Total Yield Different Continue Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblSiteToSiteYield: TLabel
          Tag = 7
          Left = 11
          Top = 302
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Site To Site Yield'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblHeadToHeadYield: TLabel
          Tag = 7
          Left = 11
          Top = 326
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Head To Head Yield'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4ContinueCount: TLabel
          Tag = 7
          Left = 11
          Top = 182
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Site To Site Continue Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4IntervalCount: TLabel
          Tag = 7
          Left = 11
          Top = 158
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Site To Site Interval Count'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4ContinueType: TLabel
          Tag = 7
          Left = 11
          Top = 134
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Site To Site Continue Type Select'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblAlarm4EnableIntervalYield: TLabel
          Tag = 7
          Left = 11
          Top = 206
          Width = 373
          Height = 20
          AutoSize = False
          Caption = 'Yield_Alarm4 Interval Total Yield Different Select'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlContsFailHeadAlarmCT_FT: TPanel
          Tag = 7
          Left = 400
          Top = 112
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlContsFailSocketAlarmCT_FT: TPanel
          Tag = 6
          Left = 400
          Top = 86
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlLowYieldIg: TPanel
          Tag = 5
          Left = 400
          Top = 62
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
        object pnlLowYield_FT: TPanel
          Tag = 4
          Left = 400
          Top = 37
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 3
        end
        object pnlLowYieldEnable_FT: TPanel
          Left = 400
          Top = 14
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 4
        end
        object pnlAlarm4IntervalYieldYield: TPanel
          Tag = 7
          Left = 400
          Top = 232
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 5
        end
        object pnlAlarm4IntervalYieldIntervalCount: TPanel
          Tag = 7
          Left = 400
          Top = 256
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 6
        end
        object pnlAlarm4IntervalYieldContinueCount: TPanel
          Tag = 7
          Left = 400
          Top = 280
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 7
        end
        object pnlSiteToSiteYield: TPanel
          Tag = 7
          Left = 400
          Top = 304
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 8
        end
        object pnlHeadToHeadYield: TPanel
          Tag = 7
          Left = 400
          Top = 328
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 9
        end
        object pnlAlarm4ContinueType: TPanel
          Tag = 7
          Left = 400
          Top = 136
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 10
        end
        object pnlAlarm4IntervalCount: TPanel
          Tag = 7
          Left = 400
          Top = 160
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 11
        end
        object pnlAlarm4ContinueCount: TPanel
          Tag = 7
          Left = 400
          Top = 184
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 12
        end
        object pnlAlarm4EnableIntervalYield: TPanel
          Tag = 7
          Left = 400
          Top = 208
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 13
        end
      end
    end
    object tsTester: TTabSheet
      Caption = 'Tester'
      ImageIndex = 7
      object pnlReceipeDefaultTester: TPanel
        Left = 0
        Top = 0
        Width = 528
        Height = 357
        Align = alClient
        BevelInner = bvLowered
        Color = clInactiveCaption
        TabOrder = 0
        object lblStartDelayTime: TLabel
          Tag = 7
          Left = 11
          Top = 62
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Tester_Start Delay Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblMaxTestTime: TLabel
          Tag = 7
          Left = 11
          Top = 38
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Tester_Wait Max Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object lblInitialMaxTest: TLabel
          Tag = 7
          Left = 11
          Top = 14
          Width = 220
          Height = 20
          AutoSize = False
          Caption = 'Tester_Initial Max Time'
          Color = 9791537
          Font.Charset = ANSI_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          ParentColor = False
          ParentFont = False
        end
        object pnlInitialMaxTest: TPanel
          Tag = 7
          Left = 240
          Top = 16
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
        object pnlMaxTestTime: TPanel
          Tag = 7
          Left = 240
          Top = 40
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
        object pnlStartDelayTime: TPanel
          Tag = 7
          Left = 240
          Top = 64
          Width = 60
          Height = 20
          BevelInner = bvLowered
          Color = clGreen
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
        end
      end
    end
  end
end
