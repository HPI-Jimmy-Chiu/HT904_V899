object fFTPClient: TfFTPClient
  Left = 260
  Top = 272
  BorderStyle = bsSingle
  Caption = 'FTP'
  ClientHeight = 512
  ClientWidth = 971
  Color = clSilver
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PrintScale = poPrintToFit
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 673
    Height = 493
    ActivePage = TabSheet2
    Align = alLeft
    TabIndex = 1
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = 'Network Setting'
      object Label1: TLabel
        Left = 544
        Top = 188
        Width = 5
        Height = 25
        Caption = '.'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clCaptionText
        Font.Height = -20
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object GroupBox1: TGroupBox
        Left = 0
        Top = 8
        Width = 353
        Height = 321
        TabOrder = 0
        object Panel1: TPanel
          Left = 4
          Top = 8
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Machine Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 0
          Visible = False
        end
        object Panel2: TPanel
          Left = 4
          Top = 52
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Machine IP Address'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 2
          Visible = False
        end
        object Panel3: TPanel
          Left = 4
          Top = 96
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Sub Net Mask'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 4
          Visible = False
        end
        object Panel4: TPanel
          Left = 4
          Top = 140
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Host Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 6
        end
        object Panel5: TPanel
          Left = 4
          Top = 184
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Host IP Address'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 8
          Visible = False
        end
        object Panel6: TPanel
          Left = 4
          Top = 228
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Gateway Address'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 10
          Visible = False
        end
        object Panel7: TPanel
          Left = 4
          Top = 272
          Width = 173
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Ethernet Address'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 12
          Visible = False
        end
        object edtMachineName: TEdit
          Left = 184
          Top = 12
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Visible = False
        end
        object edtHostName: TEdit
          Left = 184
          Top = 144
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 7
          Text = '127.0.0.1'
        end
        object edtMachineIPAddress0: TEdit
          Left = 184
          Top = 56
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          Visible = False
        end
        object edtSubNetMask0: TEdit
          Left = 184
          Top = 100
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 5
          Visible = False
        end
        object edtHostIPAddress0: TEdit
          Left = 184
          Top = 188
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 9
          Visible = False
        end
        object edtGatewayAddress0: TEdit
          Left = 184
          Top = 232
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 11
          Visible = False
        end
        object edtEthernetAddress: TEdit
          Left = 184
          Top = 276
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 13
          Visible = False
        end
      end
      object GroupBox2: TGroupBox
        Left = 360
        Top = 8
        Width = 293
        Height = 321
        TabOrder = 1
        object Panel8: TPanel
          Left = 4
          Top = 8
          Width = 281
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'F T P'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 0
        end
        object Panel9: TPanel
          Left = 4
          Top = 52
          Width = 117
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'User Name'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 1
        end
        object Panel10: TPanel
          Left = 4
          Top = 96
          Width = 117
          Height = 41
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Password'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 3
        end
        object Panel11: TPanel
          Left = 4
          Top = 136
          Width = 117
          Height = 53
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Download'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 5
        end
        object Panel12: TPanel
          Left = 4
          Top = 188
          Width = 117
          Height = 53
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Upload'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 7
        end
        object edtUserName: TEdit
          Left = 124
          Top = 56
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Text = 'jay'
        end
        object edtPassword: TEdit
          Left = 124
          Top = 100
          Width = 160
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          PasswordChar = '*'
          TabOrder = 4
          Text = '0201'
        end
        object edtDownload: TEdit
          Left = 124
          Top = 144
          Width = 160
          Height = 41
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 6
          Text = '/'
        end
        object edtUpload: TEdit
          Left = 124
          Top = 196
          Width = 160
          Height = 41
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 8
          Text = '/'
        end
      end
      object ListBox1: TListBox
        Left = 164
        Top = 344
        Width = 121
        Height = 37
        ItemHeight = 13
        TabOrder = 2
        Visible = False
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Server'
      ImageIndex = 1
      object GroupBox3: TGroupBox
        Left = 0
        Top = 4
        Width = 409
        Height = 381
        TabOrder = 0
        object Panel13: TPanel
          Left = 4
          Top = 8
          Width = 133
          Height = 33
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Setup File'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 0
        end
        object edtServerWaferName: TEdit
          Left = 144
          Top = 8
          Width = 257
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = edtServerWaferNameChange
          OnKeyDown = edtServerWaferNameKeyDown
          OnKeyPress = edtServerWaferNameKeyPress
          OnMouseDown = edtServerWaferNameMouseDown
        end
        object lstServerFile: TListBox
          Left = 4
          Top = 48
          Width = 397
          Height = 325
          ItemHeight = 13
          ParentColor = True
          TabOrder = 2
          OnDblClick = lstServerFileDblClick
        end
      end
      object GroupBox4: TGroupBox
        Left = 412
        Top = 4
        Width = 250
        Height = 381
        TabOrder = 1
        object labN06_DownloadPath: TLabel
          Left = 8
          Top = 115
          Width = 76
          Height = 13
          Caption = 'Download Path:'
        end
        object labN06_UploadPath: TLabel
          Left = 8
          Top = 176
          Width = 62
          Height = 13
          Caption = 'Upload Path:'
        end
        object plSLoad: TPanel
          Left = 28
          Top = 40
          Width = 185
          Height = 41
          Caption = 'Download to Handler'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = plSLoadClick
        end
        object FTP_DownPath: TEdit
          Left = 8
          Top = 136
          Width = 235
          Height = 21
          Enabled = False
          TabOrder = 1
        end
        object FTP_UpLdPath: TEdit
          Left = 8
          Top = 196
          Width = 235
          Height = 21
          Enabled = False
          TabOrder = 2
        end
        object Panel15: TPanel
          Left = 24
          Top = 248
          Width = 185
          Height = 41
          Caption = 'Copy File Name'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 3
          OnClick = Panel15Click
        end
        object Panel16: TPanel
          Left = 24
          Top = 296
          Width = 185
          Height = 41
          Caption = 'Clean File Name'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 4
          OnClick = Panel16Click
        end
      end
      object ProgressBar1: TProgressBar
        Left = 424
        Top = 360
        Width = 199
        Height = 17
        Min = 0
        Max = 100
        Smooth = True
        TabOrder = 2
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'HD'
      ImageIndex = 2
      object GroupBox5: TGroupBox
        Left = 0
        Top = 4
        Width = 409
        Height = 381
        TabOrder = 0
        object Panel14: TPanel
          Left = 4
          Top = 8
          Width = 133
          Height = 33
          BevelOuter = bvLowered
          BorderStyle = bsSingle
          Caption = 'Setup File'
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentColor = True
          ParentFont = False
          TabOrder = 0
        end
        object edtHDWaferName: TEdit
          Left = 144
          Top = 8
          Width = 257
          Height = 33
          AutoSize = False
          Color = clWhite
          Font.Charset = ANSI_CHARSET
          Font.Color = clWindowText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnChange = edtHDWaferNameChange
          OnKeyDown = edtHDWaferNameKeyDown
          OnKeyPress = edtHDWaferNameKeyPress
          OnMouseDown = edtHDWaferNameMouseDown
        end
        object lstHDFile: TListBox
          Left = 4
          Top = 48
          Width = 397
          Height = 325
          ItemHeight = 13
          ParentColor = True
          TabOrder = 2
          OnDblClick = lstHDFileDblClick
        end
      end
      object GroupBox6: TGroupBox
        Left = 412
        Top = 4
        Width = 250
        Height = 381
        TabOrder = 1
        object labN06_DownloadPath1: TLabel
          Left = 4
          Top = 175
          Width = 76
          Height = 13
          Caption = 'Download Path:'
        end
        object labN06_UploadPath1: TLabel
          Left = 4
          Top = 236
          Width = 62
          Height = 13
          Caption = 'Upload Path:'
        end
        object plLoad: TPanel
          Left = 28
          Top = 20
          Width = 200
          Height = 41
          Caption = 'Load from HD'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = plLoadClick
        end
        object plUnload: TPanel
          Left = 28
          Top = 72
          Width = 200
          Height = 41
          Caption = 'Upload to Server'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          OnClick = plUnloadClick
        end
        object FileListBox1: TFileListBox
          Left = 2
          Top = 300
          Width = 246
          Height = 79
          Align = alBottom
          ItemHeight = 13
          TabOrder = 5
          Visible = False
        end
        object FTP_DownPath1: TEdit
          Left = 4
          Top = 196
          Width = 235
          Height = 21
          Enabled = False
          TabOrder = 3
        end
        object FTP_UpLdPath1: TEdit
          Left = 4
          Top = 256
          Width = 235
          Height = 21
          Enabled = False
          TabOrder = 4
        end
        object plUnloadALL: TPanel
          Left = 28
          Top = 124
          Width = 200
          Height = 41
          Caption = 'Upload All to Server'
          Color = clSilver
          Font.Charset = ANSI_CHARSET
          Font.Color = clCaptionText
          Font.Height = -20
          Font.Name = #26032#32048#26126#39636
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
          OnClick = plUnloadALLClick
        end
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Taster'
      ImageIndex = 3
      object GroupBox7: TGroupBox
        Left = 12
        Top = 12
        Width = 621
        Height = 97
        Caption = 'Handler'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label2: TLabel
          Left = 72
          Top = 24
          Width = 121
          Height = 20
          AutoSize = False
          Caption = 'Type'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object Label3: TLabel
          Left = 236
          Top = 24
          Width = 125
          Height = 20
          AutoSize = False
          Caption = 'ID'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'MS Sans Serif'
          Font.Style = []
          ParentFont = False
        end
        object edHandlerType: TEdit
          Left = 72
          Top = 48
          Width = 121
          Height = 24
          Enabled = False
          TabOrder = 0
          Text = 'HT9045W'
        end
        object edHandlerID: TEdit
          Left = 236
          Top = 48
          Width = 121
          Height = 24
          Enabled = False
          TabOrder = 1
          Text = 'HT9045-001'
        end
      end
      object GroupBox8: TGroupBox
        Left = 12
        Top = 116
        Width = 621
        Height = 225
        Caption = 'Tester'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        object rgInputMethod: TRadioGroup
          Left = 20
          Top = 20
          Width = 253
          Height = 69
          Caption = 'Input method'
          ItemIndex = 0
          Items.Strings = (
            'By List'
            'Manually')
          TabOrder = 1
          OnClick = rgInputMethodClick
        end
        object grpTesterMap: TGroupBox
          Left = 280
          Top = 16
          Width = 333
          Height = 129
          Caption = 'Taster Map'
          TabOrder = 0
          object Label4: TLabel
            Left = 36
            Top = 24
            Width = 121
            Height = 20
            AutoSize = False
            Caption = 'Type'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label5: TLabel
            Left = 36
            Top = 57
            Width = 125
            Height = 20
            AutoSize = False
            Caption = 'ID'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object Label7: TLabel
            Left = 36
            Top = 91
            Width = 121
            Height = 20
            AutoSize = False
            Caption = 'IP'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'MS Sans Serif'
            Font.Style = []
            ParentFont = False
          end
          object cbTesterType: TComboBox
            Left = 164
            Top = 20
            Width = 145
            Height = 24
            ItemHeight = 0
            TabOrder = 0
            OnChange = cbTesterTypeChange
          end
          object cbTesterID: TComboBox
            Left = 164
            Top = 54
            Width = 145
            Height = 24
            ItemHeight = 0
            TabOrder = 1
            OnChange = cbTesterIDChange
          end
          object cbTasterIp: TComboBox
            Left = 164
            Top = 87
            Width = 145
            Height = 24
            Enabled = False
            ItemHeight = 0
            TabOrder = 2
          end
        end
        object grpTesterName: TGroupBox
          Left = 280
          Top = 152
          Width = 333
          Height = 61
          Caption = 'Taster Name'
          TabOrder = 2
          object edTesterName: TEdit
            Left = 12
            Top = 21
            Width = 309
            Height = 24
            TabOrder = 0
          end
        end
      end
      object btSafeTasterName: TButton
        Left = 84
        Top = 348
        Width = 177
        Height = 57
        Caption = 'Save'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = btSafeTasterNameClick
      end
      object Button3: TButton
        Left = 376
        Top = 348
        Width = 177
        Height = 57
        Caption = 'Exit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = Button3Click
      end
    end
  end
  object sbFTPStatus: TStatusBar
    Left = 0
    Top = 493
    Width = 971
    Height = 19
    Color = clSilver
    Panels = <
      item
        Width = 200
      end
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object memoFTP: TMemo
    Left = 673
    Top = 0
    Width = 298
    Height = 493
    Align = alClient
    Lines.Strings = (
      '')
    TabOrder = 1
    OnDblClick = memoFTPDblClick
  end
  object NMFTP1: TNMFTP
    Port = 21
    ReportLevel = 0
    OnStatus = NMFTP1Status
    OnConnectionFailed = NMFTP1ConnectionFailed
    OnError = NMFTP1Error
    OnTransactionStart = NMFTP1TransactionStart
    OnTransactionStop = NMFTP1TransactionStop
    OnAuthenticationFailed = NMFTP1AuthenticationFailed
    OnFailure = NMFTP1Failure
    OnSuccess = NMFTP1Success
    OnListItem = NMFTP1ListItem
    OnUnSupportedFunction = NMFTP1UnSupportedFunction
    Vendor = 2411
    ParseList = False
    ProxyPort = 0
    Passive = False
    FirewallType = FTUser
    FWAuthenticate = False
    Left = 632
    Top = 328
  end
end
