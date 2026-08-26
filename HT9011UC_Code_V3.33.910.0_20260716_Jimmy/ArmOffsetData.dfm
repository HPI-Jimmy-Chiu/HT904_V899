object fArmOffsetData: TfArmOffsetData
  Left = 1071
  Top = 236
  BorderIcons = []
  BorderStyle = bsToolWindow
  Caption = 'fArmOffsetData'
  ClientHeight = 504
  ClientWidth = 736
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
  object pgcArmOffset: TPageControl
    Left = 0
    Top = 0
    Width = 736
    Height = 464
    ActivePage = tsOutArm
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object tsInArm: TTabSheet
      Caption = 'In Arm'
      object pnlInArm: TPanel
        Left = 0
        Top = 40
        Width = 728
        Height = 257
        Align = alTop
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label1: TLabel
          Left = 84
          Top = 10
          Width = 33
          Height = 13
          Caption = 'Loader'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label2: TLabel
          Left = 16
          Top = 30
          Width = 36
          Height = 13
          Caption = 'Hand X'
        end
        object Label3: TLabel
          Left = 16
          Top = 55
          Width = 36
          Height = 13
          Caption = 'Hand Y'
        end
        object Label4: TLabel
          Left = 16
          Top = 105
          Width = 36
          Height = 13
          Caption = 'Pick up'
        end
        object Label5: TLabel
          Left = 16
          Top = 80
          Width = 39
          Height = 13
          Caption = 'Release'
        end
        object Label6: TLabel
          Left = 16
          Top = 130
          Width = 43
          Height = 13
          Caption = 'X Pitch 1'
        end
        object Label9: TLabel
          Left = 201
          Top = 10
          Width = 50
          Height = 13
          Caption = 'Hot Plate1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label10: TLabel
          Left = 323
          Top = 10
          Width = 50
          Height = 13
          Caption = 'Hot Plate2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label11: TLabel
          Left = 450
          Top = 10
          Width = 42
          Height = 13
          Caption = 'Shuttle 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label12: TLabel
          Left = 572
          Top = 10
          Width = 42
          Height = 13
          Caption = 'Shuttle 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label28: TLabel
          Left = 16
          Top = 155
          Width = 43
          Height = 13
          Caption = 'X Pitch 2'
        end
        object Label29: TLabel
          Left = 16
          Top = 180
          Width = 43
          Height = 13
          Caption = 'X Pitch 3'
        end
        object Label30: TLabel
          Left = 16
          Top = 205
          Width = 43
          Height = 13
          Caption = 'X Pitch 4'
        end
        object Label31: TLabel
          Left = 16
          Top = 230
          Width = 34
          Height = 13
          Caption = 'Y Pitch'
        end
      end
      object palOffsetParts: TPanel
        Left = 0
        Top = 0
        Width = 728
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Input Arm'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'Arial Black'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label27: TLabel
          Left = 674
          Top = 0
          Width = 54
          Height = 40
          Align = alRight
          Caption = 'Unit : mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clFuchsia
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Layout = tlBottom
        end
      end
    end
    object tsOutArm: TTabSheet
      Caption = 'OutArm'
      ImageIndex = 1
      object pnlOutArm: TPanel
        Left = 0
        Top = 40
        Width = 728
        Height = 396
        Align = alClient
        BevelOuter = bvNone
        Color = 12761254
        TabOrder = 1
        object Label13: TLabel
          Left = 80
          Top = 10
          Width = 42
          Height = 13
          Caption = 'Shuttle 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label19: TLabel
          Left = 159
          Top = 10
          Width = 42
          Height = 13
          Caption = 'Shuttle 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label20: TLabel
          Left = 238
          Top = 10
          Width = 31
          Height = 13
          Caption = 'Auto 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label21: TLabel
          Left = 317
          Top = 10
          Width = 31
          Height = 13
          Caption = 'Auto 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label22: TLabel
          Left = 395
          Top = 10
          Width = 31
          Height = 13
          Caption = 'Auto 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label23: TLabel
          Left = 474
          Top = 10
          Width = 22
          Height = 13
          Caption = 'Fix 1'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label24: TLabel
          Left = 553
          Top = 10
          Width = 22
          Height = 13
          Caption = 'Fix 2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label25: TLabel
          Left = 632
          Top = 10
          Width = 22
          Height = 13
          Caption = 'Fix 3'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -11
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label14: TLabel
          Left = 16
          Top = 30
          Width = 36
          Height = 13
          Caption = 'Hand X'
        end
        object Label15: TLabel
          Left = 16
          Top = 55
          Width = 36
          Height = 13
          Caption = 'Hand Y'
        end
        object Label16: TLabel
          Left = 16
          Top = 105
          Width = 36
          Height = 13
          Caption = 'Pick up'
        end
        object Label17: TLabel
          Left = 16
          Top = 80
          Width = 39
          Height = 13
          Caption = 'Release'
        end
        object Label18: TLabel
          Left = 16
          Top = 130
          Width = 43
          Height = 13
          Caption = 'X Pitch 1'
        end
        object Label32: TLabel
          Left = 16
          Top = 155
          Width = 43
          Height = 13
          Caption = 'X Pitch 2'
        end
        object Label33: TLabel
          Left = 16
          Top = 180
          Width = 43
          Height = 13
          Caption = 'X Pitch 3'
        end
        object Label34: TLabel
          Left = 16
          Top = 205
          Width = 43
          Height = 13
          Caption = 'X Pitch 4'
        end
        object Label35: TLabel
          Left = 16
          Top = 230
          Width = 34
          Height = 13
          Caption = 'Y Pitch'
        end
      end
      object Panel1: TPanel
        Left = 0
        Top = 0
        Width = 728
        Height = 40
        Align = alTop
        BevelOuter = bvNone
        Caption = 'Output Arm'
        Color = 9534289
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWhite
        Font.Height = -24
        Font.Name = 'Arial Black'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        object Label8: TLabel
          Left = 674
          Top = 0
          Width = 54
          Height = 40
          Align = alRight
          Caption = 'Unit : mm'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clFuchsia
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
          Layout = tlBottom
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 464
    Width = 736
    Height = 40
    Align = alBottom
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 1
    object btnSave: TButton
      Left = 116
      Top = 5
      Width = 150
      Height = 30
      Caption = 'Save'
      TabOrder = 0
      OnClick = btnSaveClick
    end
    object btnExit: TButton
      Left = 509
      Top = 5
      Width = 150
      Height = 30
      Caption = 'Exit'
      TabOrder = 1
      OnClick = btnOutArmExitClick
    end
  end
end
