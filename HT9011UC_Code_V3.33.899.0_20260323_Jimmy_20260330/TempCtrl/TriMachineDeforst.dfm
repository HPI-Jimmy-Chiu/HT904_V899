object frmDefrost: TfrmDefrost
  Left = 2038
  Top = 213
  BorderStyle = bsToolWindow
  Caption = 'Defrost'
  ClientHeight = 549
  ClientWidth = 871
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poDefault
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 21
    Top = 408
    Width = 530
    Height = 260
    Caption = 'Temperature Status'
    TabOrder = 0
    Visible = False
    object pcTriTempControl: TPageControl
      Left = 8
      Top = 16
      Width = 513
      Height = 235
      ActivePage = tsTriTemp_20ChShow
      TabIndex = 1
      TabOrder = 0
      object tsTriTemp_12ChShow: TTabSheet
        Caption = 'Temperature_1026'
        ImageIndex = 1
        object pnHotPlate1: TPanel
          Left = 255
          Top = 145
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 0
          object lbHotPlate1: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'HotPlate 1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHotPlate1TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempHotPlate1: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHotPlate11: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHotPlate2: TPanel
          Left = 380
          Top = 145
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 1
          object lbHotPlate2: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'HotPlate 2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHotPlate2TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempHotPlate2: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHotPlate12: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnShuttle1: TPanel
          Left = 5
          Top = 145
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 2
          object lbShuttle1: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Shuttle 1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnShuttle1TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetShuttle1: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetShuttle1: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnShuttle2: TPanel
          Left = 130
          Top = 145
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 3
          object lbShuttle2: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Shuttle 2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnShuttle2TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetShuttle2: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetShuttle2: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1: TPanel
          Left = 5
          Top = 5
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 4
          object lbHead1: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead1: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead1: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2: TPanel
          Left = 130
          Top = 5
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 5
          object lbHead2: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2TempShow: TPanel
            Left = 5
            Top = 27
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead2: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead2: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead5: TPanel
          Left = 255
          Top = 5
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 6
          object lbHead5: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 5'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead5TempShow: TPanel
            Left = 5
            Top = 27
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead5: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead5: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead6: TPanel
          Left = 380
          Top = 5
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 7
          object lbHead6: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 6'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead6TempShow: TPanel
            Left = 5
            Top = 27
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead6: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead6: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead3: TPanel
          Left = 5
          Top = 75
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 8
          object lbHead3: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 3'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead3TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead3: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead3: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead4: TPanel
          Left = 130
          Top = 75
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 9
          object lbHead4: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 4'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead4TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead4: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead4: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead7: TPanel
          Left = 255
          Top = 75
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 10
          object lbHead7: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 7'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead7TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead7: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead7: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead8: TPanel
          Left = 380
          Top = 75
          Width = 120
          Height = 55
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 11
          object lbHead8: TLabel
            Left = 5
            Top = 5
            Width = 75
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 8'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead8TempShow: TPanel
            Left = 5
            Top = 30
            Width = 75
            Height = 20
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead8: TPanel
            Left = 85
            Top = 5
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object pnTempOffsetHead8: TPanel
            Left = 85
            Top = 30
            Width = 30
            Height = 20
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -16
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
      end
      object tsTriTemp_20ChShow: TTabSheet
        Caption = 'Temperature_1028'
        ImageIndex = 2
        object pnHead1_1: TPanel
          Left = 5
          Top = 4
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 0
          object lbHead1_1: TLabel
            Left = 5
            Top = 5
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_1TempShow: TPanel
            Left = 5
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead1_1: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel26: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_3: TPanel
          Left = 135
          Top = 4
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 1
          object lbHead1_3: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_3'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_3TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead1_3: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel36: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_5: TPanel
          Left = 260
          Top = 4
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 2
          object lbHead1_5: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_5'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_5TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel45: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel46: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_7: TPanel
          Left = 385
          Top = 4
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 3
          object lbHead1_7: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_7'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_7TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel49: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel50: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_8: TPanel
          Left = 385
          Top = 44
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 4
          object lbHead1_8: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_8'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_8TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel51: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel52: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_6: TPanel
          Left = 260
          Top = 44
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 5
          object lbHead1_6: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_6'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_6TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel47: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel48: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_4: TPanel
          Left = 135
          Top = 44
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 6
          object lbHead1_4: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_4'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_4TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead1_4: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel38: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead1_2: TPanel
          Left = 5
          Top = 44
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 7
          object lbHead1_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 1_2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead1_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object pnTempSetHead1_2: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel28: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_1: TPanel
          Left = 5
          Top = 84
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 8
          object lbHead2_1: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_1TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel29: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel30: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_3: TPanel
          Left = 135
          Top = 84
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 9
          object lbHead2_3: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_3'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_3TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel39: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel40: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_5: TPanel
          Left = 260
          Top = 84
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 10
          object lbHead2_5: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_5'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_5TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel59: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel60: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_7: TPanel
          Left = 385
          Top = 84
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 11
          object lbHead2_7: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_7'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_7TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel53: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel54: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_8: TPanel
          Left = 385
          Top = 124
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 12
          object lbHead2_8: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_8'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_8TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel55: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel56: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_6: TPanel
          Left = 260
          Top = 124
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 13
          object lbHead2_6: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_6'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_6TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel61: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel62: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_4: TPanel
          Left = 135
          Top = 124
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 14
          object lbHead2_4: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_4'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_4TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel41: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel42: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHead2_2: TPanel
          Left = 5
          Top = 124
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 15
          object lbHead2_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Head 2_2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHead2_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel31: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel32: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnShuttle1_2: TPanel
          Left = 5
          Top = 164
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 16
          object lbShuttle1_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Shuttle 1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnShuttle1_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel33: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel34: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnShuttle2_2: TPanel
          Left = 135
          Top = 164
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 17
          object lbShuttle2_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'Shuttle 2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnShuttle2_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel43: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel44: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHotPlate1_2: TPanel
          Left = 260
          Top = 164
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 18
          object lbHotPlate1_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'HotPlate 1'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHotPlate1_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel63: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel64: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
        object pnHotPlate2_2: TPanel
          Left = 385
          Top = 165
          Width = 110
          Height = 40
          BevelInner = bvLowered
          Color = 13487501
          TabOrder = 19
          object lbHotPlate2_2: TLabel
            Left = 3
            Top = 4
            Width = 71
            Height = 15
            Alignment = taCenter
            AutoSize = False
            Caption = 'HotPlate 2'
            Color = clGreen
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWhite
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentColor = False
            ParentFont = False
          end
          object pnHotPlate2_2TempShow: TPanel
            Left = 3
            Top = 21
            Width = 71
            Height = 15
            BevelOuter = bvNone
            Caption = '000.00'
            Color = 1881083
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 0
          end
          object Panel57: TPanel
            Left = 80
            Top = 5
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clRed
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 1
          end
          object Panel58: TPanel
            Left = 80
            Top = 21
            Width = 25
            Height = 15
            BevelOuter = bvNone
            Caption = '125'
            Color = clGray
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clBlack
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = [fsBold]
            ParentFont = False
            TabOrder = 2
          end
        end
      end
    end
  end
  object GroupBox2: TGroupBox
    Left = 530
    Top = 120
    Width = 335
    Height = 260
    Caption = 'Defrost Status'
    Color = 12761254
    ParentColor = False
    TabOrder = 1
    object GroupBox3: TGroupBox
      Left = 5
      Top = 15
      Width = 322
      Height = 58
      Caption = 'Defrost Setting'
      Color = 12761254
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentColor = False
      ParentFont = False
      TabOrder = 0
      object Label33: TLabel
        Left = 5
        Top = 15
        Width = 173
        Height = 20
        Caption = 'Defrost Temperature:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label34: TLabel
        Left = 5
        Top = 35
        Width = 109
        Height = 20
        Caption = 'Defrost Time:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblSetTemp: TLabel
        Left = 180
        Top = 15
        Width = 31
        Height = 20
        Caption = '125'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object lblSetTime: TLabel
        Left = 122
        Top = 35
        Width = 31
        Height = 20
        Caption = '100'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label37: TLabel
        Left = 1000
        Top = 54
        Width = 162
        Height = 16
        Caption = 'Defrosting Mechanism :'
      end
      object Label38: TLabel
        Left = 220
        Top = 15
        Width = 28
        Height = 20
        Caption = #8451'  '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label39: TLabel
        Left = 162
        Top = 35
        Width = 29
        Height = 20
        Caption = 'min'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object ComboBox1: TComboBox
        Left = 1000
        Top = 54
        Width = 120
        Height = 24
        Color = clMenu
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -15
        Font.Name = 'MS Sans Serif'
        Font.Style = [fsBold]
        ImeName = 'Chinese (Traditional) - New Phonetic'
        ItemHeight = 16
        ParentFont = False
        TabOrder = 0
        Text = 'IndexArm1'
        Items.Strings = (
          'IndexArm1'
          'IndexArm2'
          'Shuttle1'
          'Shuttle2'
          'HotPlate1'
          'HotPlate2')
      end
      object Button1: TButton
        Left = 1000
        Top = 12
        Width = 80
        Height = 25
        Caption = 'Start '
        TabOrder = 1
      end
      object Button2: TButton
        Left = 1000
        Top = 50
        Width = 80
        Height = 25
        Caption = 'End'
        TabOrder = 2
      end
    end
    object GroupBox4: TGroupBox
      Left = 5
      Top = 72
      Width = 324
      Height = 180
      Caption = 'Defrost Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      object sgTriMachineAutoDefrost: TStringGrid
        Left = 8
        Top = 16
        Width = 305
        Height = 153
        ColCount = 2
        DefaultColWidth = 150
        DefaultRowHeight = 20
        RowCount = 7
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Times New Roman'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
    end
  end
  object pnlChangeMsg_Chi: TPanel
    Left = 5
    Top = 8
    Width = 840
    Height = 49
    Caption = 
      #38500#38684#20013'...........'#38500#38684#20013'...........'#38500#38684#20013'...........'#38500#38684#20013'...........'#38500#38684#20013'.....' +
      '......'#38500#38684#20013'...........'#38500#38684#20013'...........'#38500#38684#20013'...........'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #24494#36575#27491#40657#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object pnlChangeMsg_Eng: TPanel
    Left = 5
    Top = 61
    Width = 840
    Height = 49
    Caption = 
      'Defrosting...........Defrosting...........Defrosting...........D' +
      'efrosting...........Defrosting...........Defrosting...........'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = #24494#36575#27491#40657#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 3
  end
  object gbAutoDefrostProduction: TGroupBox
    Left = 5
    Top = 120
    Width = 521
    Height = 260
    Color = 12761254
    ParentColor = False
    TabOrder = 4
    object lblMessage1: TLabel
      Left = 8
      Top = 17
      Width = 463
      Height = 34
      Caption = #27231#21488#23559#26044'                '#24460#33258#21205#36914#34892#38500#38684#21151#33021
      Font.Charset = ANSI_CHARSET
      Font.Color = clBlack
      Font.Height = -27
      Font.Name = #24494#36575#27491#40657#39636
      Font.Style = []
      ParentFont = False
    end
    object m_labTmr: TLabel
      Left = 126
      Top = 17
      Width = 75
      Height = 36
      Caption = '05:00'
      Font.Charset = ANSI_CHARSET
      Font.Color = clRed
      Font.Height = -29
      Font.Name = #24494#36575#27491#40657#39636
      Font.Style = []
      ParentFont = False
    end
    object btnForceDefrost: TBtnPanel
      Left = 10
      Top = 120
      Width = 180
      Height = 41
      BevelInner = bvRaised
      Caption = 'Force Defrost'
      Color = 8404992
      Font.Charset = CHINESEBIG5_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnForceDefrostClick
      TrueColor = 12936728
      FalseColor = 8404992
      TrueFontColor = 8453888
      FalseFontColor = clWhite
    end
    object btnCloseFormWaitDefrost: TBtnPanel
      Left = 303
      Top = 120
      Width = 180
      Height = 41
      BevelInner = bvRaised
      Caption = 'Wait Defrost(Clean Out)'
      Color = 8404992
      Font.Charset = CHINESEBIG5_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = #26032#32048#26126#39636
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      OnClick = btnCloseFormWaitDefrostClick
      TrueColor = clBlue
      FalseColor = 8404992
      TrueFontColor = clYellow
      FalseFontColor = clWhite
    end
  end
  object btnClose: TBtnPanel
    Left = 789
    Top = 400
    Width = 127
    Height = 41
    BevelInner = bvRaised
    Caption = 'Close '
    Color = 8404992
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Visible = False
    OnClick = btnCloseClick
    TrueColor = clBlue
    FalseColor = 8404992
    TrueFontColor = clYellow
    FalseFontColor = clWhite
  end
  object btnDefrostTermination: TBtnPanel
    Left = 189
    Top = 294
    Width = 127
    Height = 41
    BevelInner = bvRaised
    Caption = 'DefrostTermination'
    Color = clMaroon
    Font.Charset = CHINESEBIG5_CHARSET
    Font.Color = clWhite
    Font.Height = -16
    Font.Name = #26032#32048#26126#39636
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = btnDefrostTerminationClick
    TrueColor = clBlue
    FalseColor = clMaroon
    TrueFontColor = clYellow
    FalseFontColor = clWhite
  end
  object tmrGetTemperatureStatus: TTimer
    Enabled = False
    OnTimer = tmrGetTemperatureStatusTimer
    Left = 8
    Top = 160
  end
  object tmrClose: TTimer
    Enabled = False
    OnTimer = tmrCloseTimer
    Top = 224
  end
  object tmrMessageFlash: TTimer
    Enabled = False
    OnTimer = tmrMessageFlashTimer
    Top = 192
  end
end
