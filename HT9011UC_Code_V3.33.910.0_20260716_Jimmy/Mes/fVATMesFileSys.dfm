object fMesSystem: TfMesSystem
  Left = 80
  Top = 111
  Width = 710
  Height = 869
  Caption = 'Mes system'
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
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 694
    Height = 831
    ActivePage = tabsInformation2
    Align = alClient
    TabIndex = 1
    TabOrder = 0
    object tabsInformation: TTabSheet
      Caption = 'Lot Information'
      object LabeledEditLotNo: TLabeledEdit
        Left = 138
        Top = 7
        Width = 424
        Height = 21
        EditLabel.Width = 34
        EditLabel.Height = 13
        EditLabel.Caption = 'Lot NO'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 0
      end
      object buttonDownloadLotInfor: TButton
        Left = 574
        Top = 24
        Width = 105
        Height = 34
        Caption = 'Download'
        TabOrder = 1
        OnClick = buttonDownloadLotInforClick
      end
      object LabeledHandlerDevice: TLabeledEdit
        Left = 138
        Top = 87
        Width = 424
        Height = 21
        EditLabel.Width = 74
        EditLabel.Height = 13
        EditLabel.Caption = 'Handler Device'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 4
      end
      object LabeledTestTemp: TLabeledEdit
        Left = 138
        Top = 188
        Width = 424
        Height = 21
        EditLabel.Width = 80
        EditLabel.Height = 13
        EditLabel.Caption = 'Test temperature'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 8
      end
      object LabeledSoakTime: TLabeledEdit
        Left = 138
        Top = 213
        Width = 424
        Height = 21
        EditLabel.Width = 47
        EditLabel.Height = 13
        EditLabel.Caption = 'Soak time'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 9
      end
      object LabeledEditLotSize: TLabeledEdit
        Left = 138
        Top = 112
        Width = 424
        Height = 21
        EditLabel.Width = 36
        EditLabel.Height = 13
        EditLabel.Caption = 'Lot size'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 5
      end
      object LabeledEditOPID: TLabeledEdit
        Left = 138
        Top = 32
        Width = 424
        Height = 21
        EditLabel.Width = 28
        EditLabel.Height = 13
        EditLabel.Caption = 'Op ID'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 2
      end
      object LabeledEditContFail: TLabeledEdit
        Left = 138
        Top = 163
        Width = 424
        Height = 21
        EditLabel.Width = 72
        EditLabel.Height = 13
        EditLabel.Caption = 'Continuous Fail'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 7
      end
      object LabeledLeadCount: TLabeledEdit
        Left = 138
        Top = 588
        Width = 424
        Height = 21
        EditLabel.Width = 55
        EditLabel.Height = 13
        EditLabel.Caption = 'Lead Count'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 24
      end
      object LabeledAutoBin1: TLabeledEdit
        Left = 138
        Top = 387
        Width = 424
        Height = 21
        EditLabel.Width = 114
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceAutoBin1'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 16
      end
      object LabeledAutoBin2: TLabeledEdit
        Left = 138
        Top = 412
        Width = 424
        Height = 21
        EditLabel.Width = 114
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceAutoBin2'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 17
      end
      object LabeledAutoBin3: TLabeledEdit
        Left = 138
        Top = 437
        Width = 424
        Height = 21
        EditLabel.Width = 114
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceAutoBin3'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 18
      end
      object LabeledFixBin1: TLabeledEdit
        Left = 138
        Top = 463
        Width = 424
        Height = 21
        EditLabel.Width = 105
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceFixBin1'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 19
      end
      object LabeledFixBin2: TLabeledEdit
        Left = 138
        Top = 488
        Width = 424
        Height = 21
        EditLabel.Width = 105
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceFixBin2'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 20
      end
      object LabeledFixBin3: TLabeledEdit
        Left = 138
        Top = 513
        Width = 424
        Height = 21
        EditLabel.Width = 105
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceFixBin3'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 21
      end
      object LabeledSiteGap: TLabeledEdit
        Left = 138
        Top = 137
        Width = 424
        Height = 21
        EditLabel.Width = 39
        EditLabel.Height = 13
        EditLabel.Caption = 'Site gap'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 6
      end
      object LabeledForcePerPin: TLabeledEdit
        Left = 138
        Top = 614
        Width = 424
        Height = 21
        EditLabel.Width = 64
        EditLabel.Height = 13
        EditLabel.Caption = 'Force Per Pin'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 25
      end
      object lbledtHandlerDeviceLowYield: TLabeledEdit
        Left = 137
        Top = 640
        Width = 424
        Height = 21
        EditLabel.Width = 114
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceLowYield'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 26
      end
      object lbledtProcess: TLabeledEdit
        Left = 137
        Top = 666
        Width = 424
        Height = 21
        EditLabel.Width = 94
        EditLabel.Height = 13
        EditLabel.Caption = 'Process ( WipStep )'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 27
      end
      object lbledtLabeledFTAutoBin1: TLabeledEdit
        Left = 138
        Top = 237
        Width = 424
        Height = 21
        EditLabel.Width = 127
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceAutoBin1'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 10
      end
      object lbledtLabeledFTAutoBin2: TLabeledEdit
        Left = 138
        Top = 262
        Width = 424
        Height = 21
        EditLabel.Width = 127
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceAutoBin2'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 11
      end
      object lbledtLabeledFTAutoBin3: TLabeledEdit
        Left = 138
        Top = 287
        Width = 424
        Height = 21
        EditLabel.Width = 127
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceAutoBin3'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 12
      end
      object LabeledFTFixBin1: TLabeledEdit
        Left = 138
        Top = 312
        Width = 424
        Height = 21
        EditLabel.Width = 118
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceFixBin1'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 13
      end
      object LabeledFTFixBin2: TLabeledEdit
        Left = 138
        Top = 337
        Width = 424
        Height = 21
        EditLabel.Width = 118
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceFixBin2'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 14
      end
      object LabeledFTFixBin3: TLabeledEdit
        Left = 138
        Top = 362
        Width = 424
        Height = 21
        EditLabel.Width = 118
        EditLabel.Height = 13
        EditLabel.Caption = 'FTHandlerDeviceFixBin3'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 15
      end
      object lbledtCustCode: TLabeledEdit
        Left = 137
        Top = 692
        Width = 424
        Height = 21
        EditLabel.Width = 46
        EditLabel.Height = 13
        EditLabel.Caption = 'CustCode'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 28
      end
      object lbledtCustPart: TLabeledEdit
        Left = 137
        Top = 718
        Width = 424
        Height = 21
        EditLabel.Width = 40
        EditLabel.Height = 13
        EditLabel.Caption = 'CustPart'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 29
      end
      object lbledtInternalLot: TLabeledEdit
        Left = 137
        Top = 744
        Width = 424
        Height = 21
        EditLabel.Width = 102
        EditLabel.Height = 13
        EditLabel.Caption = 'InternalLot ( LotNum )'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 30
      end
      object lbledtCustLotNum: TLabeledEdit
        Left = 137
        Top = 770
        Width = 424
        Height = 21
        EditLabel.Width = 58
        EditLabel.Height = 13
        EditLabel.Caption = 'CustLotNum'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 31
      end
      object lbledtDevicePassBins: TLabeledEdit
        Left = 137
        Top = 538
        Width = 424
        Height = 21
        EditLabel.Width = 114
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDevicePassBins'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 22
      end
      object lbledtDeviceRetestBins: TLabeledEdit
        Left = 137
        Top = 563
        Width = 424
        Height = 21
        EditLabel.Width = 122
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceRetestBins'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 23
      end
      object lbledtHandlerID: TLabeledEdit
        Left = 138
        Top = 63
        Width = 424
        Height = 21
        EditLabel.Width = 48
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerID'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 3
      end
    end
    object tabsInformation2: TTabSheet
      Caption = 'Information2'
      ImageIndex = 3
      object lbledtHandlerDeviceATCcheck: TLabeledEdit
        Left = 181
        Top = 70
        Width = 424
        Height = 21
        EditLabel.Width = 122
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceATCcheck'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 0
      end
      object lbledtHandlerDeviceSiteCheck: TLabeledEdit
        Left = 181
        Top = 171
        Width = 424
        Height = 21
        EditLabel.Width = 120
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceSiteCheck'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 4
      end
      object lbledtHandlerDeviceSOTdelay: TLabeledEdit
        Left = 181
        Top = 95
        Width = 424
        Height = 21
        EditLabel.Width = 118
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceSOTdelay'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 1
      end
      object lbledtHandlerDeviceTestTempVar: TLabeledEdit
        Left = 181
        Top = 146
        Width = 424
        Height = 21
        EditLabel.Width = 135
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTestTempVar'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 3
      end
      object lbledtHandlerDeviceGPIBaddress: TLabeledEdit
        Left = 181
        Top = 120
        Width = 424
        Height = 21
        EditLabel.Width = 133
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceGPIBaddress'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 2
      end
      object lbledtHandlerDeviceOpenBins: TLabeledEdit
        Left = 181
        Top = 197
        Width = 424
        Height = 21
        EditLabel.Width = 117
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceOpenBins'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 5
      end
      object lbledtHandlerDeviceHotplateXDevision: TLabeledEdit
        Left = 181
        Top = 222
        Width = 424
        Height = 21
        EditLabel.Width = 159
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateXDevision'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 6
      end
      object lbledtHandlerDeviceHotplateYDevision: TLabeledEdit
        Left = 181
        Top = 248
        Width = 424
        Height = 21
        EditLabel.Width = 159
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateYDevision'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 7
      end
      object lbledtHandlerDeviceHotplateXStart: TLabeledEdit
        Left = 181
        Top = 274
        Width = 424
        Height = 21
        EditLabel.Width = 140
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateXStart'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 8
      end
      object lbledtHandlerDeviceHotplateYStart: TLabeledEdit
        Left = 181
        Top = 300
        Width = 424
        Height = 21
        EditLabel.Width = 140
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateYStart'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 9
      end
      object lbledtHandlerDeviceHotplateXPitch: TLabeledEdit
        Left = 181
        Top = 325
        Width = 424
        Height = 21
        EditLabel.Width = 142
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateXPitch'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 10
      end
      object lbledtHandlerDeviceHotplateYPitch: TLabeledEdit
        Left = 181
        Top = 351
        Width = 424
        Height = 21
        EditLabel.Width = 142
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceHotplateYPitch'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 11
      end
      object lbledtHandlerDeviceTrayXDevision: TLabeledEdit
        Left = 181
        Top = 377
        Width = 424
        Height = 21
        EditLabel.Width = 140
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayXDevision'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 12
      end
      object lbledtHandlerDeviceTrayYDevision: TLabeledEdit
        Left = 181
        Top = 403
        Width = 424
        Height = 21
        EditLabel.Width = 140
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayYDevision'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 13
      end
      object lbledtHandlerDeviceTrayXStart: TLabeledEdit
        Left = 181
        Top = 429
        Width = 424
        Height = 21
        EditLabel.Width = 121
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayXStart'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 14
      end
      object lbledtHandlerDeviceTrayYStart: TLabeledEdit
        Left = 181
        Top = 455
        Width = 424
        Height = 21
        EditLabel.Width = 121
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayYStart'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 15
      end
      object lbledtHandlerDeviceTrayXPitch: TLabeledEdit
        Left = 181
        Top = 480
        Width = 424
        Height = 21
        EditLabel.Width = 123
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayXPitch'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 16
      end
      object lbledtHandlerDeviceTrayYPitch: TLabeledEdit
        Left = 181
        Top = 506
        Width = 424
        Height = 21
        EditLabel.Width = 123
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceTrayYPitch'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 17
      end
      object lbledtHandlerDeviceSiteLayouts: TLabeledEdit
        Left = 181
        Top = 532
        Width = 424
        Height = 21
        EditLabel.Width = 126
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceSiteLayouts'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 18
      end
      object lbledtHandlerDeviceCleanPadLimit: TLabeledEdit
        Left = 181
        Top = 558
        Width = 424
        Height = 21
        EditLabel.Width = 138
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceCleanPadLimit'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 19
      end
      object lbledtHandlerDeviceInshuttleSensorCheck: TLabeledEdit
        Left = 181
        Top = 584
        Width = 424
        Height = 21
        EditLabel.Width = 175
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceInshuttleSensorCheck'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 20
      end
      object lbledtTestTimePcs: TLabeledEdit
        Left = 181
        Top = 610
        Width = 424
        Height = 21
        EditLabel.Width = 67
        EditLabel.Height = 13
        EditLabel.Caption = 'TestTime/Pcs'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 21
      end
      object lbledtC1: TLabeledEdit
        Left = 181
        Top = 636
        Width = 424
        Height = 21
        EditLabel.Width = 13
        EditLabel.Height = 13
        EditLabel.Caption = 'C1'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 22
      end
      object lbledtTestSites: TLabeledEdit
        Left = 181
        Top = 662
        Width = 424
        Height = 21
        EditLabel.Width = 44
        EditLabel.Height = 13
        EditLabel.Caption = 'TestSites'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 23
      end
      object lbledtIndexTime: TLabeledEdit
        Left = 181
        Top = 688
        Width = 424
        Height = 21
        EditLabel.Width = 49
        EditLabel.Height = 13
        EditLabel.Caption = 'IndexTime'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 24
      end
      object lbledtHandlerDeviceNotRepeatableArea: TLabeledEdit
        Left = 181
        Top = 714
        Width = 424
        Height = 21
        EditLabel.Width = 165
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceNotRepeatableArea'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 25
      end
      object lbledtHandlerDeviceICNum: TLabeledEdit
        Left = 181
        Top = 740
        Width = 424
        Height = 21
        EditLabel.Width = 103
        EditLabel.Height = 13
        EditLabel.Caption = 'HandlerDeviceICNum'
        ImeName = ''
        LabelPosition = lpLeft
        LabelSpacing = 3
        ReadOnly = True
        TabOrder = 26
      end
    end
    object tabsSet: TTabSheet
      Caption = 'Set'
      ImageIndex = 1
      object groupbMesFileTemp: TGroupBox
        Left = 3
        Top = 6
        Width = 638
        Height = 235
        Caption = 'GetLotInformationForHandler'
        TabOrder = 0
        object LabeledURL: TLabeledEdit
          Left = 106
          Top = 16
          Width = 424
          Height = 21
          EditLabel.Width = 22
          EditLabel.Height = 13
          EditLabel.Caption = 'URL'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 0
        end
        object LabeledACode: TLabeledEdit
          Left = 106
          Top = 50
          Width = 424
          Height = 21
          EditLabel.Width = 32
          EditLabel.Height = 13
          EditLabel.Caption = 'ACode'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 1
        end
        object LabeledAction: TLabeledEdit
          Left = 106
          Top = 86
          Width = 424
          Height = 21
          EditLabel.Width = 30
          EditLabel.Height = 13
          EditLabel.Caption = 'Action'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 2
        end
        object lbledtSummaryReportPath: TLabeledEdit
          Left = 106
          Top = 134
          Width = 424
          Height = 21
          EditLabel.Width = 97
          EditLabel.Height = 13
          EditLabel.Caption = 'Summary report path'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 3
        end
        object lbledtByTimeOEEpath: TLabeledEdit
          Left = 106
          Top = 166
          Width = 424
          Height = 21
          EditLabel.Width = 83
          EditLabel.Height = 13
          EditLabel.Caption = 'By time OEE path'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 4
        end
        object lbledtUPHReportPath: TLabeledEdit
          Left = 106
          Top = 197
          Width = 424
          Height = 21
          EditLabel.Width = 77
          EditLabel.Height = 13
          EditLabel.Caption = 'UPH report path'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 5
        end
      end
      object grpCreateManualEOCAP: TGroupBox
        Left = 3
        Top = 246
        Width = 638
        Height = 179
        Caption = 'CreateManualEOCAP'
        TabOrder = 1
        object labEocapStatus: TLabel
          Left = 104
          Top = 312
          Width = 69
          Height = 13
          Caption = 'eocapStatus : '
        end
        object lbledtCreateManualEOCAP_URL: TLabeledEdit
          Left = 104
          Top = 44
          Width = 424
          Height = 21
          EditLabel.Width = 22
          EditLabel.Height = 13
          EditLabel.Caption = 'URL'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 1
        end
        object btnSendJamCode: TButton
          Left = 544
          Top = 44
          Width = 75
          Height = 25
          Caption = 'Send'
          TabOrder = 2
          OnClick = btnSendJamCodeClick
        end
        object lstCreateManualEOCAP: TListBox
          Left = 104
          Top = 98
          Width = 423
          Height = 63
          ImeName = ''
          ItemHeight = 13
          Items.Strings = (
            '123')
          ScrollWidth = 1000000
          TabOrder = 4
        end
        object chkCreateManualEOCAP: TCheckBox
          Left = 104
          Top = 19
          Width = 169
          Height = 17
          Caption = 'Enabled CreateManualEOCAP'
          TabOrder = 0
          Visible = False
        end
        object lbledtCreateManualEOCAP_JamCode: TLabeledEdit
          Left = 104
          Top = 71
          Width = 424
          Height = 21
          EditLabel.Width = 47
          EditLabel.Height = 13
          EditLabel.Caption = 'Jam Code'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 3
          Text = 'JAM0303:Device drop error (Arm 1)'
        end
        object lbledtQueryEocapStatusURL: TLabeledEdit
          Left = 102
          Top = 204
          Width = 424
          Height = 21
          EditLabel.Width = 89
          EditLabel.Height = 13
          EditLabel.Caption = 'QueryEocapStatus'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 5
        end
        object btnQueryEocapStatus: TButton
          Left = 544
          Top = 204
          Width = 75
          Height = 25
          Caption = 'Send'
          TabOrder = 6
          OnClick = btnQueryEocapStatusClick
        end
        object lstQueryEocapStatus: TListBox
          Left = 104
          Top = 234
          Width = 423
          Height = 63
          ImeName = ''
          ItemHeight = 13
          Items.Strings = (
            '123')
          ScrollWidth = 1000000
          TabOrder = 7
        end
      end
      object buttonSave: TButton
        Left = 559
        Top = 696
        Width = 75
        Height = 25
        Caption = 'Save'
        TabOrder = 4
        OnClick = buttonSaveClick
      end
      object chkTrayHotplateCheck: TCheckBox
        Left = 8
        Top = 686
        Width = 138
        Height = 17
        Caption = 'Tray && Hotplate Check'
        TabOrder = 3
      end
      object grpGetRcsCheckingResult: TGroupBox
        Left = 3
        Top = 430
        Width = 638
        Height = 243
        Caption = 'GetRcsCheckingResult'
        TabOrder = 2
        object lbledtGetRcsCheckingResultUrl: TLabeledEdit
          Left = 106
          Top = 48
          Width = 424
          Height = 21
          EditLabel.Width = 22
          EditLabel.Height = 13
          EditLabel.Caption = 'URL'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 1
        end
        object lbledtGetRcsCheckingResultACode: TLabeledEdit
          Left = 106
          Top = 82
          Width = 424
          Height = 21
          EditLabel.Width = 32
          EditLabel.Height = 13
          EditLabel.Caption = 'ACode'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 2
        end
        object lbledtGetRcsCheckingResultAction: TLabeledEdit
          Left = 106
          Top = 111
          Width = 424
          Height = 21
          EditLabel.Width = 30
          EditLabel.Height = 13
          EditLabel.Caption = 'Action'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 3
        end
        object lbledtGetRcsCheckingResultItemName: TLabeledEdit
          Left = 106
          Top = 141
          Width = 424
          Height = 21
          EditLabel.Width = 48
          EditLabel.Height = 13
          EditLabel.Caption = 'ItemName'
          ImeName = ''
          LabelPosition = lpLeft
          LabelSpacing = 3
          TabOrder = 4
        end
        object lstGetRcsCheckingResult: TListBox
          Left = 106
          Top = 169
          Width = 423
          Height = 63
          ImeName = ''
          ItemHeight = 13
          Items.Strings = (
            '123')
          ScrollWidth = 1000000
          TabOrder = 5
        end
        object btnGetRcsCheckingResult: TButton
          Left = 544
          Top = 175
          Width = 75
          Height = 25
          Caption = 'Get'
          TabOrder = 6
          OnClick = btnGetRcsCheckingResultClick
        end
        object chkGetRcsCheckingResult: TCheckBox
          Left = 104
          Top = 21
          Width = 184
          Height = 17
          Caption = 'Enabled GetRcsCheckingResult'
          TabOrder = 0
        end
      end
    end
    object tabsLog: TTabSheet
      Caption = 'Log'
      ImageIndex = 2
      object listbMesFileLog: TListBox
        Left = 0
        Top = 1
        Width = 757
        Height = 470
        ImeName = ''
        ItemHeight = 13
        ScrollWidth = 1000000
        TabOrder = 0
      end
    end
  end
  object IdHTTPMESSystem: TIdHTTP
    Request.Accept = 'text/html, */*'
    Request.ContentLength = 0
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.ProxyPort = 0
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    Left = 780
    Top = 40
  end
  object IdHTTPqueryEocapStatus: TIdHTTP
    Request.Accept = 'text/html, */*'
    Request.ContentLength = 0
    Request.ContentRangeEnd = 0
    Request.ContentRangeStart = 0
    Request.ProxyPort = 0
    Request.UserAgent = 'Mozilla/3.0 (compatible; Indy Library)'
    Left = 780
    Top = 80
  end
end
