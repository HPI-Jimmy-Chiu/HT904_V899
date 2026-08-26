object fCCDPin1: TfCCDPin1
  Left = 16
  Top = 1
  Width = 899
  Height = 698
  Caption = 'fCCDPin1'
  Color = clBtnFace
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
  object SpeedButton1: TSpeedButton
    Left = 6
    Top = 530
    Width = 137
    Height = 33
    Caption = 'Live'
    OnClick = SpeedButton1Click
  end
  object SpeedButton2: TSpeedButton
    Left = 6
    Top = 570
    Width = 137
    Height = 33
    Caption = 'Grab'
    OnClick = SpeedButton2Click
  end
  object SpeedButton3: TSpeedButton
    Left = 6
    Top = 610
    Width = 137
    Height = 33
    Caption = 'Stop'
    OnClick = SpeedButton3Click
  end
  object SpeedButton8: TSpeedButton
    Left = 144
    Top = 530
    Width = 137
    Height = 33
    Caption = 'Shuttle Go'
  end
  object Label3: TLabel
    Left = 536
    Top = 536
    Width = 32
    Height = 13
    Caption = 'Label3'
  end
  object Label1: TLabel
    Left = 696
    Top = 16
    Width = 44
    Height = 13
    Caption = 'Max Limit'
  end
  object PageControl1: TPageControl
    Tag = 1
    Left = 3
    Top = 1
    Width = 655
    Height = 521
    ActivePage = TabSheet1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Run Image'
      object LiveImage: TEBW8Image
        Left = 3
        Top = 7
        Width = 640
        Height = 480
        TabStop = True
        TabOrder = 0
        ControlData = {
          03000500254200009C31000000000000000352E30B918FCE119DE300AA004BB8
          510100000090013C67020005417269616C}
      end
    end
    object TabSheet2: TTabSheet
      Tag = 1
      Caption = 'Source Image'
      ImageIndex = 1
      object SourceImage: TEBW8Image
        Left = 3
        Top = 7
        Width = 640
        Height = 480
        TabStop = True
        TabOrder = 0
        ControlData = {
          03000500254200009C31000000000000000352E30B918FCE119DE300AA004BB8
          510100000090013C67020005417269616C}
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'TabSheet3'
      ImageIndex = 2
      object EBW8Image1: TEBW8Image
        Left = 3
        Top = 7
        Width = 640
        Height = 480
        TabStop = True
        TabOrder = 0
        ControlData = {
          03000500254200009C31000000000000000352E30B918FCE119DE300AA004BB8
          510100000090013C67020005417269616C}
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'TabSheet4'
      ImageIndex = 3
      object EBW8Image2: TEBW8Image
        Left = 3
        Top = 7
        Width = 640
        Height = 480
        TabStop = True
        TabOrder = 0
        ControlData = {
          03000500254200009C31000000000000000352E30B918FCE119DE300AA004BB8
          510100000090013C67020005417269616C}
      end
    end
  end
  object ROI1: TEBW8ROI
    Left = 288
    Top = 528
    Width = 32
    Height = 32
    ControlData = {03000500560A00002B05000000000000}
  end
  object EasyMain: TEasyMain
    Left = 374
    Top = 529
    Width = 32
    Height = 32
    ControlData = {03000500560A00002B05000000000000}
  end
  object Button1: TButton
    Left = 144
    Top = 569
    Width = 137
    Height = 33
    Caption = 'Stand'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 288
    Top = 570
    Width = 97
    Height = 29
    Caption = 'Caculate'
    TabOrder = 4
    OnClick = Button2Click
  end
  object ROI2: TEBW8ROI
    Left = 328
    Top = 528
    Width = 32
    Height = 32
    ControlData = {03000500560A00002B05000000000000}
  end
  object Memo1: TMemo
    Left = 688
    Top = 64
    Width = 202
    Height = 577
    Lines.Strings = (
      'Memo1')
    TabOrder = 6
  end
  object CCD: TECameraPicolo
    Left = 400
    Top = 568
    Width = 32
    Height = 32
    OnSurfaceAvailable = CCDSurfaceAvailable
    ControlData = {
      000300003D040000ED0300000800060000002300300000000300000000000300
      0200000003000000000003000000010003000000000003000000010003000000
      01000300000000000300800200000300E0010000030000000100030000000100
      0300000000000300000000000B0000000B0001000B0001000300020000000300
      0000000003000100000003000000000003000000000003000000000003000100
      00000300000000000300000000000300FFFFFFFF}
  end
  object ScrollBar1: TScrollBar
    Left = 664
    Top = 24
    Width = 16
    Height = 497
    Kind = sbVertical
    Max = 255
    PageSize = 0
    TabOrder = 8
  end
  object Edit3: TEdit
    Left = 696
    Top = 32
    Width = 121
    Height = 21
    TabOrder = 9
    Text = '50'
  end
end
