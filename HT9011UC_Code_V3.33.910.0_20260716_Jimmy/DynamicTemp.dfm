object fDynamicTemp: TfDynamicTemp
  Left = 206
  Top = 86
  Width = 1000
  Height = 650
  Caption = 'Dynamic Temperature'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object btExit: TPanel
    Left = 0
    Top = 571
    Width = 984
    Height = 41
    Align = alBottom
    Caption = 'Exit'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = btExitClick
  end
  object PanelMain7: TPanel
    Left = 0
    Top = 0
    Width = 984
    Height = 571
    Align = alClient
    BevelInner = bvLowered
    Color = 12761254
    TabOrder = 1
    object Chart1: TChart
      Left = 2
      Top = 134
      Width = 980
      Height = 435
      AllowZoom = False
      BackWall.Brush.Color = clWhite
      BackWall.Color = 12761254
      Title.Font.Charset = DEFAULT_CHARSET
      Title.Font.Color = clBlue
      Title.Font.Height = -16
      Title.Font.Name = 'Arial'
      Title.Font.Style = []
      Title.Text.Strings = (
        'Yield Chart')
      Title.Visible = False
      BackColor = 12761254
      LeftAxis.Automatic = False
      LeftAxis.AutomaticMaximum = False
      LeftAxis.AutomaticMinimum = False
      LeftAxis.ExactDateTime = False
      LeftAxis.Increment = 0.1
      LeftAxis.LabelStyle = talValue
      LeftAxis.Maximum = 110
      LeftAxis.Minimum = 90
      LeftAxis.StartPosition = 3
      LeftAxis.EndPosition = 97
      LeftAxis.Title.Caption = 'Deg.'
      LeftAxis.Title.Font.Charset = DEFAULT_CHARSET
      LeftAxis.Title.Font.Color = clBlack
      LeftAxis.Title.Font.Height = -13
      LeftAxis.Title.Font.Name = 'Arial'
      LeftAxis.Title.Font.Style = []
      LeftAxis.TitleSize = 20
      Legend.Alignment = laBottom
      Legend.Color = clGreen
      Legend.ColorWidth = 15
      Legend.LegendStyle = lsLastValues
      Legend.ShadowColor = clTeal
      Legend.VertMargin = 3
      Legend.Visible = False
      View3D = False
      Align = alClient
      Color = 12761254
      TabOrder = 0
      object edMax: TEdit
        Left = 4
        Top = 4
        Width = 37
        Height = 21
        TabOrder = 0
        Text = '110'
        OnMouseDown = edMaxMouseDown
      end
      object edMin: TEdit
        Left = 4
        Top = 352
        Width = 37
        Height = 21
        TabOrder = 1
        Text = '90'
        OnMouseDown = edMinMouseDown
      end
      object Series1: TLineSeries
        ColorEachPoint = True
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clRed
        Title = 'Site 1'
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = True
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series2: TLineSeries
        ColorEachPoint = True
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clGreen
        Title = 'Site 2'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series3: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clYellow
        Title = 'Site 3'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series4: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clBlue
        Title = 'Site 4'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series5: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = 33023
        Title = 'Site 5'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series6: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clBlack
        Title = 'Site 6'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series7: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = 12615808
        Title = 'Site 7'
        LinePen.Width = 2
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series8: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clWhite
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series9: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clGray
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series10: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clFuchsia
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series11: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clTeal
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series12: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clNavy
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series13: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clMaroon
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series14: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clLime
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series15: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clOlive
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series16: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clPurple
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series17: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clYellow
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
      object Series18: TLineSeries
        Marks.ArrowLength = 8
        Marks.Visible = False
        SeriesColor = clYellow
        Pointer.InflateMargins = True
        Pointer.Style = psRectangle
        Pointer.Visible = False
        XValues.DateTime = False
        XValues.Name = 'X'
        XValues.Multiplier = 1
        XValues.Order = loAscending
        YValues.DateTime = False
        YValues.Name = 'Y'
        YValues.Multiplier = 1
        YValues.Order = loNone
      end
    end
    object Panel1: TPanel
      Left = 2
      Top = 2
      Width = 980
      Height = 100
      Align = alTop
      BevelOuter = bvNone
      Color = 12761254
      TabOrder = 1
      object Label4: TLabel
        Left = 402
        Top = -6
        Width = 4
        Height = 19
        Font.Charset = ANSI_CHARSET
        Font.Color = clMaroon
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = [fsBold]
        ParentFont = False
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
      end
      object gbUpperDbl: TLabel
        Left = 744
        Top = 8
        Width = 57
        Height = 13
        Caption = 'gbUpperDbl'
        OnClick = gbUpperDblClick
      end
      object gbLowerDbl: TLabel
        Left = 744
        Top = 32
        Width = 57
        Height = 13
        Caption = 'gbLowerDbl'
        OnClick = gbLowerDblClick
      end
      object gbA1: TGroupBox
        Left = 4
        Top = 0
        Width = 80
        Height = 50
        Caption = 'A1'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object CheckBox1: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Color = 12761254
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentColor = False
          ParentFont = False
          TabOrder = 0
        end
        object edtRealTime1: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 1
        end
      end
      object gbB2: TGroupBox
        Tag = 16
        Left = 4
        Top = 50
        Width = 80
        Height = 50
        Caption = 'B2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 1
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime2: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox2: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbG15: TGroupBox
        Tag = 14
        Left = 564
        Top = 0
        Width = 80
        Height = 50
        Caption = 'G15'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4716787
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 9
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime15: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox15: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 27
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4716787
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbG13: TGroupBox
        Tag = 12
        Left = 484
        Top = 0
        Width = 80
        Height = 50
        Caption = 'G13'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 10
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime13: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox13: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbE11: TGroupBox
        Tag = 10
        Left = 404
        Top = 0
        Width = 80
        Height = 50
        Caption = 'E11'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 11
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime11: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox11: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbE9: TGroupBox
        Tag = 8
        Left = 324
        Top = 0
        Width = 80
        Height = 50
        Caption = 'E9'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 12
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime9: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox9: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clRed
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbC7: TGroupBox
        Tag = 6
        Left = 244
        Top = 0
        Width = 80
        Height = 50
        Caption = 'C7'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4716787
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 13
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime7: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox7: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4716787
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbC5: TGroupBox
        Tag = 4
        Left = 164
        Top = 0
        Width = 80
        Height = 50
        Caption = 'C5'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 14
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime5: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox5: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWhite
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbA3: TGroupBox
        Tag = 2
        Left = 84
        Top = 0
        Width = 80
        Height = 50
        Caption = 'A3'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clGreen
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 15
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object CheckBox3: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clGreen
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 0
        end
        object edtRealTime3: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 1
        end
      end
      object gbF10: TGroupBox
        Tag = 9
        Left = 324
        Top = 50
        Width = 80
        Height = 50
        Caption = 'F10'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime10: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox10: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlue
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbH16: TGroupBox
        Tag = 15
        Left = 564
        Top = 50
        Width = 80
        Height = 50
        Caption = 'H16'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clAqua
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime16: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox16: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4194432
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbH14: TGroupBox
        Tag = 13
        Left = 484
        Top = 50
        Width = 80
        Height = 50
        Caption = 'H14'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 12615808
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime14: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox14: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 12615808
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbF12: TGroupBox
        Tag = 11
        Left = 404
        Top = 50
        Width = 80
        Height = 50
        Caption = 'F12'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime12: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox12: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          TabOrder = 1
        end
      end
      object gbD8: TGroupBox
        Tag = 7
        Left = 244
        Top = 50
        Width = 80
        Height = 50
        Caption = 'D8'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 4194432
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 6
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime8: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox8: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4194432
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbD6: TGroupBox
        Tag = 5
        Left = 164
        Top = 50
        Width = 80
        Height = 50
        Caption = 'D6'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = 12615808
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 7
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime6: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox6: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 12615808
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbB4: TGroupBox
        Tag = 3
        Left = 84
        Top = 50
        Width = 80
        Height = 50
        Caption = 'B4'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 8
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edtRealTime4: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ReadOnly = True
          ShowHint = False
          TabOrder = 0
        end
        object CheckBox4: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 12615808
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = [fsBold]
          ParentFont = False
          TabOrder = 1
        end
      end
      object sbTempICClear: TPanel
        Left = 928
        Top = 50
        Width = 60
        Height = 50
        Caption = 'Clear'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 16
        OnClick = sbTempICClearClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
      object btShow: TPanel
        Left = 928
        Top = 0
        Width = 60
        Height = 50
        Caption = 'Show'
        TabOrder = 17
        OnClick = btShowClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
      object btSelectAll: TPanel
        Left = 868
        Top = 0
        Width = 60
        Height = 50
        Caption = 'Selsect All'
        TabOrder = 18
        OnClick = btSelectAllClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
      object btDefaultPos: TPanel
        Left = 868
        Top = 50
        Width = 60
        Height = 50
        Caption = 'Default Pos'
        TabOrder = 19
        OnClick = btDefaultPosClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
      object gbUpper: TGroupBox
        Tag = 14
        Left = 644
        Top = 0
        Width = 80
        Height = 50
        Caption = 'Upper'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clLime
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 20
        OnDblClick = gbUpperDblClick
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edUpper: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          Text = '101'
          OnMouseDown = edUpperMouseDown
        end
        object CheckBox17: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4716787
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object gbLower: TGroupBox
        Tag = 15
        Left = 644
        Top = 50
        Width = 80
        Height = 50
        Caption = 'Lower'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clLime
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 21
        OnDblClick = gbLowerDblClick
        OnMouseDown = gbA1MouseDown
        OnMouseMove = gbA1MouseMove
        OnMouseUp = gbA1MouseUp
        object edLower: TEdit
          Left = 20
          Top = 20
          Width = 56
          Height = 27
          BiDiMode = bdRightToLeftNoAlign
          Color = 14670284
          Font.Charset = ANSI_CHARSET
          Font.Color = clMaroon
          Font.Height = -16
          Font.Name = 'Times New Roman'
          Font.Style = [fsBold]
          ImeName = ' '
          ParentBiDiMode = False
          ParentFont = False
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          Text = '99'
          OnMouseDown = edLowerMouseDown
        end
        object CheckBox18: TCheckBox
          Left = 4
          Top = 20
          Width = 17
          Height = 17
          Font.Charset = DEFAULT_CHARSET
          Font.Color = 4194432
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
        end
      end
      object Button1: TButton
        Left = 768
        Top = 56
        Width = 75
        Height = 25
        Caption = 'Start'
        TabOrder = 22
        OnClick = Button1Click
      end
    end
    object Panel2: TPanel
      Left = 2
      Top = 102
      Width = 980
      Height = 32
      Align = alTop
      BevelOuter = bvNone
      Caption = 'Panel2'
      Color = 12761254
      TabOrder = 2
      object ListBox1: TListBox
        Left = 0
        Top = 0
        Width = 860
        Height = 32
        Align = alClient
        Color = 14670284
        ImeName = ''
        ItemHeight = 13
        TabOrder = 0
      end
      object btTempICSave: TPanel
        Left = 920
        Top = 0
        Width = 60
        Height = 32
        Align = alRight
        Caption = 'Save'
        TabOrder = 1
        OnClick = btTempICSaveClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
      object btTempICLoad: TPanel
        Left = 860
        Top = 0
        Width = 60
        Height = 32
        Align = alRight
        Caption = 'Load'
        TabOrder = 2
        OnClick = btTempICLoadClick
        OnMouseDown = btSelectAllMouseDown
        OnMouseUp = btSelectAllMouseUp
      end
    end
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'txt'
    Filter = '*.txt|*.txt'
    Title = 'Save Temp IC List'
    Left = 676
    Top = 104
  end
  object OpenDialog2: TOpenDialog
    DefaultExt = 'txt'
    Filter = '*.txt|*.txt'
    Left = 704
    Top = 104
  end
  object Timer1: TTimer
    Interval = 100
    OnTimer = Timer1Timer
    Left = 746
    Top = 58
  end
end
