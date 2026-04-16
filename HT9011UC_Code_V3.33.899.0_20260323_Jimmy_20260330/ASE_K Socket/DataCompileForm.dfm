object DataCompile: TDataCompile
  Left = 326
  Top = 165
  Width = 1309
  Height = 561
  Caption = 'DataCompile'
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1301
    Height = 530
    Align = alClient
    Color = 12761254
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 106
      Height = 29
      Caption = 'Set Temp'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 8
      Top = 80
      Width = 113
      Height = 29
      Caption = 'Soak time '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 10
      Top = 405
      Width = 79
      Height = 29
      Caption = 'binning'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 10
      Top = 282
      Width = 97
      Height = 29
      Caption = 'site map '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label5: TLabel
      Left = 7
      Top = 140
      Width = 99
      Height = 29
      Caption = 'pin force '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label6: TLabel
      Left = 8
      Top = 192
      Width = 97
      Height = 29
      Caption = 'pin count'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 161
      Top = 239
      Width = 157
      Height = 29
      Caption = 'A    ====>     G'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 161
      Top = 366
      Width = 821
      Height = 29
      Caption = 
        'Bin 1  ====>     Bin15     Key in    Auto1-3 Fix 1-3 =   1 -6  0' +
        ' no use   Ex  2,1,1,0,'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 264
      Top = 461
      Width = 6
      Height = 29
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object edtWorkTemp: TEdit
      Left = 161
      Top = 24
      Width = 121
      Height = 37
      BiDiMode = bdLeftToRight
      Color = clYellow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 0
      Text = '30'
      OnClick = edtWorkTempClick
    end
    object edtSoakTime: TEdit
      Left = 161
      Top = 78
      Width = 121
      Height = 37
      BiDiMode = bdLeftToRight
      Color = clYellow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 1
      Text = '10'
      OnClick = edtSoakTimeClick
    end
    object edtForcePerPin: TEdit
      Left = 161
      Top = 135
      Width = 121
      Height = 37
      BiDiMode = bdLeftToRight
      Color = clYellow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 2
      Text = '10'
      OnClick = edtForcePerPinClick
    end
    object edtPinCount: TEdit
      Left = 161
      Top = 189
      Width = 121
      Height = 37
      BiDiMode = bdLeftToRight
      Color = clYellow
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentBiDiMode = False
      ParentFont = False
      TabOrder = 3
      Text = '10'
      OnClick = edtPinCountClick
    end
    object edtSiteMap: TEdit
      Left = 161
      Top = 288
      Width = 1065
      Height = 37
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
    object edtBinMap: TEdit
      Left = 161
      Top = 415
      Width = 1065
      Height = 37
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object btnCompare: TButton
      Left = 1056
      Top = 464
      Width = 145
      Height = 37
      Caption = 'Compare'
      TabOrder = 6
      OnClick = btnCompareClick
    end
  end
end
