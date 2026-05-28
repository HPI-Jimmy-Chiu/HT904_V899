object fQwertyKey: TfQwertyKey
  Left = 654
  Top = 239
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Qwerty Keyboard'
  ClientHeight = 469
  ClientWidth = 1005
  Color = 12761254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SpeedButton63: TSpeedButton
    Left = 20
    Top = 0
    Width = 50
    Height = 50
    Caption = '1'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton64: TSpeedButton
    Left = 74
    Top = 0
    Width = 50
    Height = 50
    Caption = '2'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton65: TSpeedButton
    Left = 128
    Top = 0
    Width = 50
    Height = 50
    Caption = '3'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object SpeedButton66: TSpeedButton
    Left = 182
    Top = 0
    Width = 50
    Height = 50
    Caption = '4'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object palContent: TPanel
    Left = 0
    Top = 0
    Width = 1005
    Height = 49
    Align = alTop
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 0
    object edQwertyContent: TEdit
      Left = 4
      Top = 4
      Width = 988
      Height = 40
      AutoSize = False
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnKeyDown = edQwertyContentKeyDown
      OnKeyPress = edQwertyContentKeyPress
    end
  end
  object palNumKey: TPanel
    Left = 725
    Top = 189
    Width = 280
    Height = 280
    Align = alClient
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 3
    object speedbNo17: TSpeedButton
      Left = 4
      Top = 0
      Width = 50
      Height = 50
      Caption = '7'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo18: TSpeedButton
      Left = 58
      Top = 0
      Width = 50
      Height = 50
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo19: TSpeedButton
      Left = 112
      Top = 0
      Width = 50
      Height = 50
      Caption = '9'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo14: TSpeedButton
      Left = 4
      Top = 56
      Width = 50
      Height = 50
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo15: TSpeedButton
      Left = 58
      Top = 56
      Width = 50
      Height = 50
      Caption = '5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo16: TSpeedButton
      Left = 112
      Top = 56
      Width = 50
      Height = 50
      Caption = '6'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo11: TSpeedButton
      Left = 4
      Top = 112
      Width = 50
      Height = 50
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo12: TSpeedButton
      Left = 58
      Top = 112
      Width = 50
      Height = 50
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo13: TSpeedButton
      Left = 112
      Top = 112
      Width = 50
      Height = 50
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo10: TSpeedButton
      Left = 4
      Top = 168
      Width = 50
      Height = 50
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object spbDP: TSpeedButton
      Left = 58
      Top = 168
      Width = 50
      Height = 50
      Caption = '.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = spbDPClick
    end
    object spbMinus: TSpeedButton
      Left = 112
      Top = 168
      Width = 50
      Height = 50
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = spbMinusClick
    end
    object spbCancel2: TSpeedButton
      Left = 112
      Top = 224
      Width = 50
      Height = 50
      Caption = 'Abort'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = spbCancelClick
    end
    object spbSummit2: TSpeedButton
      Left = 166
      Top = 224
      Width = 104
      Height = 50
      Caption = 'OK'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbSummitClick
    end
    object spbClear2: TSpeedButton
      Left = 58
      Top = 224
      Width = 50
      Height = 50
      Caption = 'Del'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = spbClearClick
    end
    object spbBackSpace2: TSpeedButton
      Left = 4
      Top = 224
      Width = 50
      Height = 50
      Caption = 'BS'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      OnClick = spbBackSpaceClick
    end
    object spbPercent: TSpeedButton
      Left = 166
      Top = 168
      Width = 104
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        360C0000424D360C000000000000360000002800000020000000200000000100
        180000000000000C0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF2F2F3C0BDBAA28772A27F64A17F64A28065A799
        8ECECFCFE3E3E3F9F9F9FFFFFFEBEBEBD7D7D7D1D2D3B0A9A4A2795BB4662EC5
        651FC6641CBC6525A86F45A59486C8C8C8D4D5D6E1E1E1F9F9F9FFFFFFFFFFFF
        FFFFFFFFFFFFFEFEFEFCFCFDF5EEE9C77538F37928F98031F98031FA8030E36B
        19AC907ACACCCDCCCCCCD4D4D4C6C7C7C8C9CAAA886FD46518FB7F2FFF9C5AFF
        AC74FFAE78FFA467FF8B40E96D1AB27140BCB8B5C6C8C9CCCCCCFDFDFDF0F0F0
        E5E5E5DEDEDEDADADADDDEDFC5AA96EE711DFFC6A2FAE0D1F9DECFFDE1D1FF9E
        5FCF651ACDC9C6D6D6D7D2D3D3D8D9DAB99275EB6C16FFA569FEDDC9EDE3E1E4
        DDDFE2DCDFE9E1E1F8E4D9FFC095FC7E2DC26E31D2CFCCE5E7E8E6E6E6DFDFDF
        DFDFDFE0E0E0E1E1E1E4E6E7CDB9A9E56713FFD0B3DAD9E1C5B6B9D5D1D8FDD7
        BFEF701DC79C7CE4E7E9E3E5E7D2C3B8DB610EFFA870F2EBEACFC6CAC3B0B1C1
        ACACC0ABACC2AEAEC8BABCE3E3E8FFCDAEF17727D29366FFFFFFF6F6F6F0F0F0
        EFEFEFEEEEEEEEEEEEEFF0F0ECEAE9CF7537F98E4AF0E4E1C8B7B8C6B3B3EAE4
        E5FDA167D16419EEE4DDF8F7F7CF8A59F58034F6E3D9CDC1C5C5B1B0CBBBBBDC
        D5D9DFDADED1C5C7C6B2B2C6B2B3E7E5E9FEAD7AD56113F3E1D5FFFFFFFEFEFE
        FDFDFDFDFDFDFDFDFDFDFDFDFEFEFFE6C6AFDD600DFCC39FDBD6DBC6B3B2D3C9
        CDF9D6C1EB7325D89970FBF6F3D26013FBB080E0DCE0C7B4B4CAB9BAEBE0DDFB
        BA90FBAB78F6DCCED2C6C9C8B4B4D0C6CAF8D5C1E67123DCA782FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFBF9CB6E2EF38842F0E1DBCEBFC1CAB7
        B8E6E1E3F8A36DD26419D7A17FDE6717F7C9ADD5CCD1CAB7B7D4CBD0F7CCB1E2
        6614D45806F6A36DE3DDE0CAB8B8CCBCBDEDDED9F38944CF8048FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE5C0A8D96112F7B88FDEDADECBB9
        B9D4CACDF3D5C3E87427B94B03E26F22F2D2C1D1C6C9CCBABADBD5DAF6B58CD0
        5B0CBB4F08F89250EADEDBCDBDBECCBBBCE6DDDEFE9E63CD6D2CFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCB743AEC7F36EEDBD3D2C5
        C8CDBDBDE3DEE2F3A46FCF5909E17126EED5C7D1C5C8CEBDBDDDD8DCFCB284D2
        6923C7733DF88C49EDDCD6CFC1C3CEBEBEE3DBDDFDA56ECD6621FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFBF5F2F7EDE6F8EFE9E5C1ABD25C0FF2AD80E0DC
        E0CFBFBED5CBCEEED3C4E67327E57428ECD2C4D3C7CAD0BFBFDDD8DCFFB386D2
        6A25C87541F68A46ECDAD5D1C3C5D0C0C0E2DBDDFCA46DCC6520FFFFFFFFFFFF
        FFFFFFFEFCFBE2B99FCD7C48C9631FCB5C14CB5D15C96422BE4E06E4742AECD4
        C9D6CBCED2C2C2E2DDE1ECA170E86E1CEBCCBBD6CCCFD2C3C3DCD6DBFDB890D1
        5D13B84F0EF9904FE9DBD8D3C5C6D2C3C4E4DBDCFB9C62CA6928FFFFFFFFFFFF
        F6EAE2C9753FD35E0FE47B34EA9861EBA475EBA374EA955CE0752CE0630EE99F
        6EE2DDE1D4C5C5D7CDD0E9D1C6F98236F3C4A9D8D1D5D5C5C5D9D2D6F7C7ABE3
        6818CB5204FCA26AE2DBDDD5C6C6D5C8C9E9DAD6F68D4BCB7742FFFFFFFAF2EE
        C5692FDD6B1DEAAA81E7D7D1E2DCDFDFDADEDFDADEE2DCDFE8D4CBE79D6DE26D
        1CE7CABADAD1D4D6C7C7E0DBE0FAAD7FFAA979DEDADFD6C8C8D7CBCCE7D8D5FA
        A979FB9B5FF1CBB7DBD3D6D7C8C8D8D0D4F2D0BFEB762CD59B78FFFFFFD39570
        D66213E8B28FE2DFE6D9CFD1D8CACAD8CACBD8CACAD8CACADAD0D3E3E0E5E49C
        6DE59057E2DCE0D9CBCBD9CFD1E9D2C8FE9656E9D4CDD9D0D2D9CBCADACFD0E1
        DADCE3DBDCDDD5D7D9CBCBD8CBCBDFDCE1FCB082D05C11EED7CAF5E8E0C45712
        E38D51E3DCE0DBCECFDBCECDDED5D7E2D9DBE1D9DCDDD3D5DBCDCDDBD0D2E3D8
        D6F99555EDC7B4DCD4D8DACDCDDEDADEF6AF85F9A36EE2DBDEDBD3D6DACECEDA
        CDCDDACDCEDACDCDDBD0D1DDDAE1F4C9B1F07D33CA7D4FFFFFFFDCAD92CD590B
        E3B699DFD9DEDDD0D0DED6D9E3C4B3DF8445DE8A4FE2CBC2DED4D6DDD0D0DED9
        DEF4B38EFB9F66E1D9DCDDD1D1DDD3D5E6D3CDFE904BFAA672EBD1C7E1D9DBDE
        D9DDDED8DCE0D9DCE5D7D4F6C0A0F4853DC25B1CF4E5DDFFFFFFCA7F54D3671C
        E1C9BEDFD7D9DFD3D4E1DBDFDC905BC04700CE5405EFB18CDFDADFDFD3D3DED7
        DAE9C6B6FF8D42EBC4B0DED8DCDFD3D3DEDADFF2B896F47B2BEB7E38F7A16AF7
        AF82F7B185F8A977F59251DF6B21C2652DEFDACEFFFFFFFFFFFFC26632D36E27
        E0D0CCE1D8D9E1D7D7E1D7D8EA9054B84B0CC35310F7AA79E0D9DCE1D6D6E0D8
        D9E5CDC4FD9049F9A069E0D6D8E1D6D7E0D7D9E4D4D0F49151B94802C25D23CA
        5E1BCA5C17C75F1EC6713FDAAA91FBF5F2FFFFFFFFFFFFFFFFFFBF5D24E48647
        E1D4D3E2D9DAE2D9DAE2D5D4F5995DC26836C5652CF5A36EE1D8DAE3D9D9E2DA
        DBE5D1CBED8845E87528EDC2ACE0DADEE3D9D9DFDADFF1BD9FDD6B21D5A188F9
        F1EDF6EBE5FAF3F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBF5D25F2975C
        E2D5D4E4DCDDE4DCDDE2D5D4F2995EC16533C46229F4A471E0D8DAE5DCDCE4DD
        DEE4D1CBEB8847BE4B05F7A067E0D4D4E5DCDDE4DCDDE1D3D1F79C60C05A1FFB
        F6F3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC06636EE9052
        E2D1CEE6DFE0E7DFDFE0D6D7F4A36CB54406BD4D0BF4AD7EE0D9DCE7DEDFE5DE
        E0E5CDC4EA8643A83805DF7229EDC0A7E1DCE0E7DFDFE1DCE0EDC0A6E0732AD3
        9B80FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC77E5BE78441
        E5CCC1E6E0E3E9E2E1E0DADDF1B38CD15F15D8681EEDBA9BE1DCE0E9E1E1E5DF
        E2E8C7B6E17933D0947AC2612AF5A166DFD0CFE8E1E2E8E1E2DFD2D2F6A56DBF
        5518F7ECE7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDCAF9CD66C27
        ECC1A7E3DEE2EBE4E4E7E1E2DEC7BFEFB085EEB189DECAC6E9E2E3ECE4E4E1DB
        DFF0BB99CD601BF1DFD7DAAC98DA712AEDBFA3E1DCE0ECE5E5E3DEE2E9C3AEE2
        7C36CD8E72FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7EDE9BD591F
        F5AE78DCCFD0ECE6E7EDE6E6E8E1E3E0DADDE1DADDE9E2E3EDE7E7EAE5E7DDCD
        CAF3A569BD5E2BFEFDFCFCF8F6BC5823F3A469DECDCAEBE6E7EDE7E8DDD2D4F4
        B07DBF5617F3E3DCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD39B84
        D87531F1C3A1DAD1D4EAE4E7F0EAEAF1EBEBF1EBEBEFEAEAE9E3E6DACFD1F4C0
        97CF6825DCB09EFFFFFFFFFFFFDBAD9BD46F2AECC1A4DFD9DFEDE8EAE3DFE5E3
        C6B7E28644D1977EFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDF9F8
        BD6943DE8240F2C39AE0CBC2DED1D2DFD5D7DFD5D7DDD1D1E1CBC0F4C094D777
        35C47A5AFFFEFEFFFFFFFFFFFFFDFAF9B95A2AF1AA6FE6CDBEE2CCC1E2CCC2EC
        CFB9E28D4CCE9178FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FAF5F2C67E60C46123E39659EBB07EEBB78CEBB78BEBAF7CE19255C15C21CA89
        6FFDFBFAFFFFFFFFFFFFFFFFFFFFFFFFE4C3B6BC581EDC894ADD8A4CDD8B4DD9
        8343BA5B29F2E2DBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFE5C4B8C27455B7531FB74D13B64E13B75422C47A5DE8CDC2FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE9CFC4CF957FCD907ACD907AD2
        9A86F3E3DDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      OnClick = spbPercentClick
    end
    object spbAdd1: TSpeedButton
      Left = 166
      Top = 0
      Width = 50
      Height = 50
      Caption = '+1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
    object spbMinus1: TSpeedButton
      Left = 220
      Top = 0
      Width = 50
      Height = 50
      Caption = '-1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
    object spbAdd10: TSpeedButton
      Left = 166
      Top = 56
      Width = 50
      Height = 50
      Caption = '+10'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
    object spbMinus10: TSpeedButton
      Left = 220
      Top = 56
      Width = 50
      Height = 50
      Caption = '-10'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
    object spbAdd100: TSpeedButton
      Left = 166
      Top = 112
      Width = 50
      Height = 50
      Caption = '+100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
    object spbMinus100: TSpeedButton
      Left = 220
      Top = 112
      Width = 50
      Height = 50
      Caption = '-100'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBlue
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      OnClick = spbAdd1Click
    end
  end
  object palQwertyKey: TPanel
    Left = 0
    Top = 189
    Width = 725
    Height = 280
    Align = alLeft
    BevelOuter = bvNone
    Color = 12761254
    TabOrder = 2
    Visible = False
    object speedbSyb3: TSpeedButton
      Left = 634
      Top = 112
      Width = 50
      Height = 50
      Caption = '~'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo01: TSpeedButton
      Left = 4
      Top = 0
      Width = 50
      Height = 50
      Caption = '1'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo02: TSpeedButton
      Left = 58
      Top = 0
      Width = 50
      Height = 50
      Caption = '2'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo03: TSpeedButton
      Left = 112
      Top = 0
      Width = 50
      Height = 50
      Caption = '3'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo04: TSpeedButton
      Left = 166
      Top = 0
      Width = 50
      Height = 50
      Caption = '4'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo05: TSpeedButton
      Left = 220
      Top = 0
      Width = 50
      Height = 50
      Caption = '5'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo06: TSpeedButton
      Left = 274
      Top = 0
      Width = 50
      Height = 50
      Caption = '6'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo07: TSpeedButton
      Left = 328
      Top = 0
      Width = 50
      Height = 50
      Caption = '7'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo08: TSpeedButton
      Left = 382
      Top = 0
      Width = 50
      Height = 50
      Caption = '8'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo09: TSpeedButton
      Left = 436
      Top = 0
      Width = 50
      Height = 50
      Caption = '9'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbNo00: TSpeedButton
      Left = 490
      Top = 0
      Width = 50
      Height = 50
      Caption = '0'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb_: TSpeedButton
      Left = 544
      Top = 0
      Width = 50
      Height = 50
      Caption = '-'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyPl: TSpeedButton
      Left = 598
      Top = 0
      Width = 50
      Height = 50
      Caption = '='
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrQ: TSpeedButton
      Left = 22
      Top = 56
      Width = 50
      Height = 50
      Caption = 'q'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrW: TSpeedButton
      Left = 76
      Top = 56
      Width = 50
      Height = 50
      Caption = 'w'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrE: TSpeedButton
      Left = 130
      Top = 56
      Width = 50
      Height = 50
      Caption = 'e'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrR: TSpeedButton
      Left = 184
      Top = 56
      Width = 50
      Height = 50
      Caption = 'r'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrT: TSpeedButton
      Left = 238
      Top = 56
      Width = 50
      Height = 50
      Caption = 't'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrY: TSpeedButton
      Left = 292
      Top = 56
      Width = 50
      Height = 50
      Caption = 'y'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrU: TSpeedButton
      Left = 346
      Top = 56
      Width = 50
      Height = 50
      Caption = 'u'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrI: TSpeedButton
      Left = 400
      Top = 56
      Width = 50
      Height = 50
      Caption = 'i'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrO: TSpeedButton
      Left = 454
      Top = 56
      Width = 50
      Height = 50
      Caption = 'o'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrP: TSpeedButton
      Left = 508
      Top = 56
      Width = 50
      Height = 50
      Caption = 'p'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSybL: TSpeedButton
      Left = 562
      Top = 56
      Width = 50
      Height = 50
      Caption = '['
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSybR: TSpeedButton
      Left = 616
      Top = 56
      Width = 50
      Height = 50
      Caption = ']'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyOr: TSpeedButton
      Left = 670
      Top = 56
      Width = 50
      Height = 50
      Caption = '\'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrA: TSpeedButton
      Left = 40
      Top = 112
      Width = 50
      Height = 50
      Caption = 'a'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrS: TSpeedButton
      Left = 94
      Top = 112
      Width = 50
      Height = 50
      Caption = 's'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrD: TSpeedButton
      Left = 148
      Top = 112
      Width = 50
      Height = 50
      Caption = 'd'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrF: TSpeedButton
      Left = 202
      Top = 112
      Width = 50
      Height = 50
      Caption = 'f'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrG: TSpeedButton
      Left = 256
      Top = 112
      Width = 50
      Height = 50
      Caption = 'g'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrH: TSpeedButton
      Left = 310
      Top = 112
      Width = 50
      Height = 50
      Caption = 'h'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrJ: TSpeedButton
      Left = 364
      Top = 112
      Width = 50
      Height = 50
      Caption = 'j'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrK: TSpeedButton
      Left = 418
      Top = 112
      Width = 50
      Height = 50
      Caption = 'k'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrL: TSpeedButton
      Left = 472
      Top = 112
      Width = 50
      Height = 50
      Caption = 'l'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb1: TSpeedButton
      Left = 526
      Top = 112
      Width = 50
      Height = 50
      Caption = ';'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb2: TSpeedButton
      Left = 580
      Top = 112
      Width = 50
      Height = 50
      Caption = #39
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrZ: TSpeedButton
      Left = 58
      Top = 168
      Width = 50
      Height = 50
      Caption = 'z'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrX: TSpeedButton
      Left = 112
      Top = 168
      Width = 50
      Height = 50
      Caption = 'x'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrC: TSpeedButton
      Left = 166
      Top = 168
      Width = 50
      Height = 50
      Caption = 'c'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrV: TSpeedButton
      Left = 220
      Top = 168
      Width = 50
      Height = 50
      Caption = 'v'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrB: TSpeedButton
      Left = 274
      Top = 168
      Width = 50
      Height = 50
      Caption = 'b'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrN: TSpeedButton
      Left = 328
      Top = 168
      Width = 50
      Height = 50
      Caption = 'n'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbChrM: TSpeedButton
      Left = 382
      Top = 168
      Width = 50
      Height = 50
      Caption = 'm'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb4: TSpeedButton
      Left = 436
      Top = 168
      Width = 50
      Height = 50
      Caption = ','
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb5: TSpeedButton
      Left = 490
      Top = 168
      Width = 50
      Height = 50
      Caption = '.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSyb6: TSpeedButton
      Left = 544
      Top = 168
      Width = 50
      Height = 50
      Caption = '/'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object speedbSpac: TSpeedButton
      Left = 4
      Top = 224
      Width = 388
      Height = 50
      Caption = ' '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        361B0000424D361B000000000000360000002800000030000000300000000100
        180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFDFDFDFAFAFAF9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9
        F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9
        F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9
        F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9FAFAFAFEFE
        FEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF4F4F4D4D4D3BABABAB5B5B5B5B5B5B5
        B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5
        B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B6B5
        B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5B5
        B5B5B5B5B5B5B5B5B5B5B5BDBDBDDADADAF8F8F8FFFFFFFFFFFFFFFFFFF7F7F7
        C4C4C39A99989A9A999F9E9EA1A09FA3A2A1A4A4A3A6A5A5A7A7A7A9A8A8A9A9
        A9ABAAAAACABABADACACAEADADAFAEAEAFAFAFAFAFAFAFAFAFAFAFAFB0AFAFB0
        AFAFB0AFAFAFAFAFAFAFAFAFAEAEAEADADACACACABAAAAAAA9A9A9A8A8A7A6A6
        A5A4A4A4A3A3A2A1A0A09F9E9E9D9D9C9A9A999998989795959393918F8E9898
        97D0D0D0FBFBFBFFFFFFFFFFFFE4E4E4ABAAA9B4B4B2C8C6C5CFCDCCD1D0CFD3
        D2D1D6D4D3D8D6D6DAD9D8DCDADADEDCDCDFDEDDE0DFDFE2E1E0E2E1E1E4E3E2
        E4E3E3E5E4E3E5E4E4E6E5E4E6E5E5E6E5E5E6E5E5E6E5E5E5E4E4E4E3E2E3E2
        E1E1E0E0E0DEDEDEDCDCDCDADADAD9D8D7D6D5D5D3D3D2D1D0D0CECDCDCBCACA
        C8C7C7C5C4C4C2C1C1BFBDB7B4B3A09E9DAFADADF0F0F0FFFFFFFFFFFFDCDCDC
        B4B2B1BCBBB9CFCFCEE8E8E8E8E8E8E8E8E7E8E8E7E8E8E7E8E8E8E8E8E8E8E8
        E8E8E8E8E8E8E8E8E7E7E8E8E8E8E8E8E8E8E8E8E8E7E8E8E7E8E8E7E8E8E8E8
        E8E8E8E8E8E8E8E8E8E8E8E8E8E8E8E8E8E8E7E7E7E7E7E7E7E7E7E7E6E7E6E6
        E7E6E6E6E5E5E6E5E5E5E5E4E5E4E4E5E3E3E4E3E3E4E3E2E2E1E1C4C3C3AAA7
        A6ABAAA9E7E7E7FFFFFFFFFFFFDDDDDCB6B5B3B9B8B7D3D3D3F1F1F2F1F1F2F1
        F1F2F1F1F2F1F1F2F1F1F1F1F1F1F1F1F1F1F1F2F1F1F2F1F2F2F1F1F2F1F1F2
        F1F1F2F1F1F2F1F1F2F1F1F2F1F1F1F1F1F1F1F1F1F1F1F2F1F1F2F1F1F2F1F1
        F2F1F2F2F1F2F2F1F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F3F3F1F1F1C8C7C7A8A6A5ADABAAE6E6E6FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDD
        B6B5B4BAB8B7D3D3D3F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2
        F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F2F1F1F1C8C8C8A8A6
        A5ADABAAE7E7E7FFFFFFFFFFFFDDDDDDB6B5B4BAB8B7D4D3D3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3F3
        F3F3F3F3F3F3F3F3F2F2F2C8C8C8A8A6A5ADABAAE7E7E7FFFFFFFFFFFFE1E1E1
        B9B8B6B9B8B6CFCFCEEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEB
        EBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEB
        EBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBECEC
        ECECECECECECECECECECECECECECECECECECECECECECECECEBEBEBC4C4C4A8A6
        A5B0AEADE9E9E9FFFFFFFFFFFFECECECC0C0BEB8B7B5B6B5B5BFBFBEC0C0BFC1
        C1C0C2C2C1C2C2C2C3C3C3C4C4C3C4C4C4C4C4C4C5C5C5C6C6C5C6C6C6C6C6C6
        C6C6C6C6C6C6C7C6C6C7C6C6C7C7C7C7C7C7C7C7C7C7C6C6C6C6C6C6C6C6C6C6
        C6C6C5C5C5C4C4C4C4C4C4C3C3C3C3C3C2C2C2C1C1C1C1C0C0C0BFBFBFBEBEBE
        BDBDBDBCBCBCBBBBBAB9B9AEACACA8A6A5BCBBBBF3F3F3FFFFFFFFFFFFFAFAFA
        DAD9D9C1C0BFBDBCBBC0BEBDC3C2C1C6C4C3C8C7C6CAC9C8CDCCCBCFCECDD1D0
        CFD1D1D1D2D2D2D4D4D3D6D5D5D7D6D6D8D6D6D8D7D7D9D8D8D9D8D8D9D9D9D9
        D9D9D9D9D9D9D8D8D8D8D7D7D6D6D6D5D5D4D3D3D2D1D1D1D0D0CFCECDCCCBCB
        CAC9C8C7C6C5C4C3C2C1C0BFBEBDBCBBB9B8B7B6B5B4B2B2B1AFAEAFADACB7B6
        B5DEDEDEFDFDFDFFFFFFFFFFFFFFFFFFFAFAFAE9E9E8E2E1E0E2E2E2E4E4E3E5
        E5E4E6E6E5E7E6E6E8E7E7E9E8E8E9E9E9EAEAE9EAEAEAEBEBEBEBEBEBECEBEB
        ECECECECECECEDECECEDECECEDEDEDEDEDEDEDEDEDEDEDEDECECECECECECEBEB
        EBEBEBEBEAEAEAE9E9E9E9E8E8E8E7E7E7E6E6E6E5E5E5E4E4E4E3E3E2E2E1E1
        E1E0E0DFDFDFDEDDDDDCDCDDDCDCEAEAEAFDFDFDFFFFFFFFFFFFFFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
    end
    object spbChangeCase: TSpeedButton
      Left = 4
      Top = 168
      Width = 49
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        360C0000424D360C000000000000360000002800000020000000200000000100
        180000000000000C0000C40E0000C40E00000000000000000000FAFAFAEFEFEF
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9
        E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9E9EFEFEFFAFAFAEFEFEFCECECE
        BDBDBDBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
        BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBC
        BCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBCBDBDBDCECECEEFEFEFE9E9E9A6A6A6
        A2A2A29F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F
        9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F
        9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9FA2A2A2A6A6A6E9E9E9E9E9E9A2A2A2
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA2A2A2E9E9E9E9E9E99F9F9F
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFFDFDFDFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFCFDFEFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFDFEFFFCFCFCFCFCFCFDFEFFFEFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFFCFCFCFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFBFCFCFDFFFFD396
        52C77922C97D29C87A24D39753FDFFFFFBFCFCFBFBFCFDFFFFDDB280CB8232CA
        7E2CCA802ECA7E2CC77922D39753F7EEE5FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFFBFBFBFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFAFBFDFFFEFF
        FFDDB180CB8130D49A57FFFFFFFCFDFFFAFAFAFAFAFAFBFDFFFFFFFFD9A46BCD
        873ACE893DCC8536DAA872FEFFFFFCFEFFFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFFAFAFAF9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9FAFAFBFD
        FFFAFDFFCD8637DAAA74FDFFFFFAFCFFFAFBFCFAFBFCFBFEFFF8F5F4CB8231CF
        8B3FCF8B3FCB8130F8F5F3FBFEFFFAFBFBFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF9F9F9F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8F8FA
        FAFBFFFFE0B88BC97C27FBFFFFFDFFFFFCFFFFFCFFFFFDFFFFE5C5A2CD8738D0
        8C41CE893BDAA973FBFFFFF9FAFBF9F9F9FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF8F8F6F7F7F6F7F7F6F7F7F6F7F7F6F7F7F6F7F7F6F7F7F6F7F7F6F7F7
        F7F9FBFCF5F2EFCA7E2ACD8534CD8431CC8230CC8330CD8534CE883AD39146D5
        9347CE8635EFE2D3F9FCFEF7F8F7F8F8F6FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF6F7F5F6F6F5F6F6F5F6F6F5F6F6F5F6F6F5F6F6F5F6F6F5F6F6F5F6F6
        F5F6F8F8F9FFFFD9A871CE8738CE8738CD8533CE8635CF893CD7974CD99B51D8
        984BCF8F48FAFFFFF7F8F9F6F6F5F6F7F5FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF5F4F4F5F4F4F5F4F4F5F4F4F5F4F4F5F4F4F5F4F4F5F4F4F5F4F4F5F4
        F4F5F4F5F7F9FCEDE0D2CA7F2BEBD6C2FBFFFFFCFFFFD29552DEA358DEA358D5
        9142E7CFB6F7FAFEF5F5F6F5F4F4F5F4F4FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF4F3F3F4F3F3F4F3F3F4F3F3F4F3F3F4F3F3F4F3F3F5F3F4F7F4F5F8F5
        F6F8F5F6F9F7F9FBFEFFD28E44D18E45F9FFFFF0E9E3D28D3DE0A95EE0A85CCE
        8A3DF4F6F8F5F6F8F4F3F3F4F3F3F4F3F3FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F3F2F2F4F2F3F9F4F6FFF9FDFFFB
        FFFFFBFFFFFCFFFFFFFFF8D5BFD17F2CEFE5DBDFBA92D79749DEA258D69648DD
        B68AF5F9FEF3F3F4F3F2F2F3F2F2F3F2F2FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF2F1F1F2F1F1F2F1F1F2F1F1F2F1F1F2F1F1F4F2F2FFF6F900782A007F
        36007F37007E3700803A008642DF8C42DBA56ED29754D99C50D99B50CE8533EF
        ECEAF3F4F7F2F1F2F2F1F1F2F1F1F2F1F1FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFF1F0EFF1F0EFF1F1F1F1F2F1F1F2F1F1F1F1F3F1F1FBF5F7FFFEFF0081
        3A00DBA300D8A00BEAAC008441F5BE99D08536D08D41D7974CD49144D49E61F4
        F8FEF2F2F2F1F0EFF1F0EFF1F0EFF1F0EFFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFEFEFEEEFF1F1F1F5F8F2F8FEF2F8FEF1F6FBF3F5F8FDF8FDFFFEFF007F
        3800D19E00CE9B2BE9B800803AFFF3F7CF8332D08D41D29044CD8636E6D4BFF1
        F4F6EFEFEFEFEFEEEFEFEEEFEFEEEFEFEEFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFEEEEEEF0F4F7D9AA76CA7E2ACB7F2BD59D5FF5EFEEEC934F007F3700CB
        9F00C79C4BEAC648EDCA009B5CFFFEFFDEB080CD8639CD873AD08F48F1F8FFEF
        F0F1EEEEEDEEEEEDEEEEEDEEEEEDEEEEEDFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFEDEEEEEFF5FBCC7F2BCE883ACE8738D28A3EDC802B00843E00C6A100C2
        9B67EED6007C34007F37007F36FFFAFFECDACECC8331CC8434E0C4A5EFF3F6ED
        EEEEEDEDECEDEDECEDEDECEDEDECEDEDEBFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFECEDEDEEF4F9D08632D28D40D59955F4FEFFFAE5E000823C81F6ED81F3
        E7007A2EFFF8FFFFF9FF00792BFDF4F9F2F6FCD19552CB7E29EDEFF0EDEFF0EC
        ECEBECECEBECECEBECECEBECECEBEBECEAFFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFEBEAEBECEFF3E0BB8ED69442D69341ECECEBF6F8FFEE93400084410080
        39FFF5FEF7EFF2F4EEF0F7EFF2F1EDEFEDEFF2E1C5A8D7A570EDF0F5EBEBECEB
        EAEAEBEAEAEBEAEAEBEAEAEBEAEAEAE9E9FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE9E8E9EAEBEDECF0F7E3C5A0DB9B46DFAB68ECE8E4E4A051ECA356EB9E
        4BF3F1F9ECEBECEBE9EAECEAEAEBE9EAEAEBEBEBEDF0E9E5E2EAEBEDEAE9EAEA
        E9E9EAE9E9EAE9E9EAE9E9EAE9E9E9E8E8FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE8E7E8E9EAEBEAEDF3EBF2FBEAE9E8E2BB8ADD9B47DFA154E1A358E09E
        4CEBF0F7E9E9EAE9E8E8E9E8E8E9E8E8E9E8E8E9E8E9E9E9EAE9E8E9E9E8E8E9
        E8E8E9E8E8E9E8E8E9E8E8E9E8E8E8E7E7FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE7E7E6E8EBEEDFBE96DCA96BE1C7A7EAF0F9E8E8E8D89848D99B4FD795
        43EAEFF5E8E8E8E8E7E6E8E7E6E8E7E6E8E7E6E8E7E6E8E7E6E8E7E6E8E7E6E8
        E7E6E8E7E6E8E7E6E8E7E6E8E7E6E7E6E5FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE6E6E5E8ECF0D28730D38D3BD28933E9F0F9EAF3FDD38E3DD49245D38B
        38E9EEF4E7E7E7E7E6E5E7E6E5E7E6E5E7E6E5E7E6E5E7E6E5E7E6E5E7E6E5E7
        E6E5E7E6E5E7E6E5E7E6E5E7E6E5E6E5E4FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE5E4E4E7EAEDD9B083CD832FD6A772EAF4FFDEC9B1CE8635CE8636D49C
        5EE8EBEFE6E6E6E6E5E4E6E5E4E6E5E4E6E5E4E6E5E4E6E5E4E6E5E4E6E5E4E6
        E5E4E6E5E4E6E5E4E6E5E4E6E5E4E5E4E2FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE4E3E2E5E6E6E7EBF0DBBC9ACF8B40CB7C24CC7D27CC7F2AD6A876E5E4
        E6E6E7E8E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5E4E3E5
        E4E3E5E4E3E5E4E3E5E4E3E5E4E3E4E2E1FFFFFF9F9F9FE9E9E9E9E9E99F9F9F
        FFFFFFE2E1E0E3E3E2E4E4E5E5E7EAE5E9EEE6EAF0E6EBF1E6EBF0E5E8ECE4E5
        E6E4E3E3E4E3E2E4E3E2E4E3E2E4E3E2E4E3E2E4E3E2E4E3E2E4E3E2E4E3E2E4
        E3E2E4E3E2E4E3E2E4E3E2E3E2E1E2E1E0FFFFFF9F9F9FE9E9E9EAEAEAA0A0A0
        FFFFFFE1DFDEE1E0DFE1E0DFE1E1E0E1E1E1E2E2E2E2E2E2E1E2E1E1E1E1E1E0
        E0E1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1
        E0DFE1E0DFE1E0DFE1E0DFE1E0DFE1DFDEFFFFFFA0A0A0EAEAEAF2F2F2A2A2A2
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA2A2A2F2F2F2FBFBFBBDBDBD
        A2A2A2A0A0A09F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F
        9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F9F
        9F9F9F9F9F9F9F9F9F9F9F9F9F9FA0A0A0A2A2A2BDBDBDFBFBFB}
      ParentFont = False
      OnClick = spbChangeCaseClick
    end
    object spbBackSpace: TSpeedButton
      Left = 652
      Top = 0
      Width = 68
      Height = 50
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        361B0000424D361B000000000000360000002800000030000000300000000100
        180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEC
        EAE1DCD4E6E2DDFCFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF8F7F6B8AFA88A776598825494805CD3CBC6FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7E4E28F80764E351F775E
        31AE964D775F35A89A90FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFC
        FBFBC9C1BD6C58495C401D96743BBF9455C49F566147239C8E84FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF0EEEDA599915A432C765929B28A4FC18E5FBB85
        5FC09359593E19998C83FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEDCD6D3806E6159
        402196753DC1945EC48F66C18B66C18B66C4975E593E19998C83FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFF7F5F4BCB2AB644C396E5128B08A51C99768C7926CC7916DC7926DC792
        6DC89B64573D1A988B83FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7E3E098877B5F44298B6A3BBF9360CA
        966DCA9570CB9772CC9873CD9874CD9873CB9F6A553C1B988B83FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFCFCD1C8C3
        79614F6A4C28A47E4DC69668CA956FCC9773CF9B76D19D79D39E7BD39F7BD29E
        7ACEA270533B1C978B83FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFF3F0EEB0A0956B4E34815F36B68A5BC6926AC8936FCC9873D1
        9C78D4A07CD7A380D8A582D9A582D8A581D1A67651391D978B83FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0D9D4947B69704D2C987146
        BD8C60C28D66C6916CCC9772D09C78D5A17DD9A682DCA986DEAB88DEAB88DEAB
        87D5A97C4E371D968B83FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAF9
        F8C8B9B08060477E5730A87B50BA875EBE8762C38D68C8936FCE9975D39F7BD8
        A581DDAA87E1AF8BE4B28FE5B28FE3B18DD7AC814B351E8B8078ECEAE9EDEBE9
        EDEBE9EDEBE9EEEBE9EEEBE9EEEBEAEEEBE9EEEBE9EEEBE9EEEBEAF1EFEDFAF9
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFEDE7E4AA92827851328F653DAC7C52B47E57B9825C
        BF8863C48F6AC99570D09B77D5A17EDAA784E0AD8AE5B390E9B795EAB896E7B6
        93DAAF854A361F352213422E1E46301F49311F4D332050352153372157392258
        39215638215437205C402A836B55BAACA0FAFAF9FFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFCD8CCC494735C7C52
        2E966942A5724CAA744FB27B55B9825DBF8964C48F6BCA9571D19C78D6A27FDB
        A985E2AF8CE7B593EDBB99EEBD9AEAB895E1B38C9F7F5C90704F906E4D8F6D4A
        8F6B478E6A458D68428D66408B643E88613A845E37805A3487633C9F84608B73
        5DF2F0EEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFF7F4F2C0AB9C845C3E8458339466409A6743A06B47A9734EB27B55B9825C
        BF8963C48F6ACA9570D09C77D6A27EDBA884E1AE8BE6B491EBB997ECBA98E8B6
        94E4B28EE3B18BDEAC85D9A67FD3A078CD9971C7936BC28D64BC875EB48057AB
        7850A370499B684298684089653F644831EFECEAFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE7DED8A887717F512F8659348B5B378F5C
        3997633FA06B46A8734EB17A54B8815BBE8862C38E68C9946FCE9A75D4A07CD9
        A682DEAB88E3B08DE6B391E6B491E4B28FE0AE8ADBA885D6A27FD19C78CB9672
        C6906BC08A65BA845EB47D57AC7550A36D499A6542915D3A8F5E3A78512F5738
        22EEEBE9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFBFAD3C0B397
        6D4F7F4F2A7F512E7E4E2C84512F8D593795613E9E6945A7714CAF7853B6805A
        BC8660C18C66C6926DCC9873D19D79D6A27FDAA783DEAB87E0AD8AE0AE8ADFAC
        89DCA885D8A481D39F7BCF9A76C99470C48F6ABE8863B9825DB37C56AA744FA2
        6C48996441905C3A8E5D3977502E5A3A22EFECEAFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFF3EDE9BE9F8B8B5A377A4A2671432271402078472682502E8B59
        3694613E9D6945A6714DAE7954B6805BBC8661C18C67C5926DCB9773CF9C79D3
        A17ED7A582DAA885DCA987DCAA87DBA986D8A583D4A17ED09B77CB9571C6916B
        C18B65BC8660B7805AB07954A7724D9F6A4697623F8E5B388B5B37754E2D5B3B
        23EFECEAFFFFFFFFFFFFFFFFFFFFFFFFFEFEFDE3D3C9AA7F61824D276F3F1E65
        38196C3E1F76492A8053338B5E3E986B4BA17555AA7E5EB28767BA8F6EC19776
        C79D7CCBA281CFA787D3AB8DD7AF91D9B294DCB597DDB799DEB89ADFB89BDEB6
        99DBB395D8AE8FD3A686CE9F7EC99A78C49371BC8964B37C57AB744FA46E499C
        674394603D8B5836895735744B2B5D3B23EFECEAFFFFFFFFFFFFFFFFFFF9F6F4
        D2B7A59F6A45885430784E2E724A2D7A5336855F428F694C9873559F795BA47D
        5EA98161AE8666B48C6BBA916FBF9674C59A78C89E7CCBA17FCEA483D1A786D3
        AA89D5AC8BD7AE8DD8AF8DD8AF8ED7AE8DD7AE8DD7B090D7B293D5B093D2AC8F
        CDA688C9A182C3997BBB9072AF81629C6A48915D3A8855338554317148295F3C
        24EFECEAFFFFFFFFFFFFFFFFFFDAC2B29C5E3392613D8A66498866498B694C8B
        6A4D8F6D509571549A76589F7B5CA58061AA8465AF8969B58E6DBA9271BF9675
        C49B79C89F7DCBA280CEA583D0A786D2AA88D3AB8AD4AD8CD5AD8CD5AD8CD5AD
        8CD4AB8AD2AA88D0A785CDA482CAA17FC89F7DC79F7FC5A081C09A7CBC977AB9
        967CB18E749E765A8A5D3D72492A623D24F0ECEAFFFFFFFFFFFFFFFFFFDBC3B2
        A05F3495623D916E51907054906F53906F539372559876599D7B5DA27F61A783
        65AC8869B08C6DB59070B99574BE9878C29C7BC6A07ECAA381CCA684CEA786D0
        AA88D1AB89D2AC8BD3AD8BD3AD8BD2AC8AD2AB89D0A988CFA786CCA583CAA281
        C69F7DC29B79BE9775B99271B58F6FB28D6FAD896BAD8C71B69B848B6A51623C
        22F0ECEAFFFFFFFFFFFFFFFFFFFAF6F4D6B9A5A96E4698613996705196775B96
        775A97775B9B7B5EA07F61A48365A98769AD8B6CB28F70B69373B99676BD9A7A
        C29D7DC5A080C8A482CBA684CDA886CFAA88D0AB89D1AC8AD2AC8BD2AC8BD1AC
        8AD0AB89CFA988CEA886CBA584C9A381C5A07FC19C7BBD9878B99474B59070B0
        8C6CAB8769A68364AC8D728D6C54643C22F0EDEAFFFFFFFFFFFFFFFFFFFFFFFF
        FEFEFDE7D5C9B985629E5F339C704E9C7D609C7E629F8063A38366A78769AB8B
        6DAF8F70B39173B79576BA9979BE9B7CC19E7EC4A181C7A483CAA685CCA887CE
        AA88CFAB89D0AC8AD0AC8BD0AC8BD0AC8ACFAB89CEAA88CCA787C9A585C7A382
        C4A080C19D7DBD9A7AB99677B59273B18F70AC8B6CA88668AC8E728B6A4F663D
        23F1EDEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5EDE8CDA58AA8683CA1
        6D46A38163A4866AA6886BAA8B6EAE8F71B19274B59577B8987ABB9B7CBE9E7E
        C1A081C4A283C6A585C8A787CBA888CCAA89CDAB8ACEAB8BCFAC8BCFAC8BCEAB
        8BCDAA8ACCAA89CAA888C8A686C6A484C3A181C09F7FBD9C7CBA9879B69576B2
        9273AE8E70AA8A6CAD8F748A674C683E23F1EDEAFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFCFAF8DEC3B2B37951A3673CA88261AB8F72AD9173B093
        76B39678B7997AB99B7DBC9E7FBFA081C2A283C4A485C6A687C8A889CAA98ACB
        AA8ACCAB8BCDAC8CCDAD8DCDAC8CCCAC8BCCAB8BCBAA8AC9A989C8A787C6A585
        C3A383C0A081BE9E7FBA9B7CB79879B49576B09274AC8E70AE91758965496A40
        23F1EDEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEE
        E0D7C1906FA66538AB7D59B29477B4997CB69A7CB99D7EBB9F81BEA183C0A385
        C2A586C4A788C6A889C8AA8BC9AB8CCAAC8CCBAD8DCCAD8ECCAE8ECCAD8ECCAD
        8DCBAC8DCAAB8BC8AA8AC7A889C5A787C3A486C1A283BEA081BC9E7FB99B7CB5
        987AB29577AF9274AF94778964466D4123F1EDEBFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F4F1D4B199AD6D42AB734BB695
        77BBA084BCA183BEA385C0A587C2A688C4A88AC6AA8BC7AB8CC8AC8ECAAD8ECB
        AE8FCBAE90CCAF90CCAF8FCCAF8FCCAE8FCCAF90D0B598D0B597CEB396CDB295
        CBB094C9AF92C7AC90C4AB8EC2A88CBFA68ABCA388BAA186B9A1879371547347
        2AF2EDEAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFDFCFBE5D0C1B9815CA9693EB78E6DC1A689C1A78AC2A88AC5AA8C
        C6AC8DC7AC8EC9AE8FCAAF90CBAF91CCB092CCB192CCB192CCB192CCB192CDB1
        92C6AB8D9277608D725B90735B92745B94765C97765C99775C9C785C9E795C9E
        795B9C775A9A7659A88A71B79F8D9E7E69F4F1EFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2E8E1C99D
        7FAA673BB4825BC5A98CC7AF91C7AF90C9B091CAB092CBB193CCB294CDB394CD
        B395CEB495CEB495CEB495CEB495CEB495C2A7894E2D174D2C165A38225F3B23
        653D246A40257044267646267B49277C49277B49277947268B6246BEA899DACD
        C5FCFBFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFBF8F6DBBEAAB2744BAE7247C3A181CCB598
        CCB597CDB597CEB697CFB698CFB799D0B799D0B899D0B899D0B799D0B899D0B8
        99C5AB8E58351C9F8B7EF0EDEAF0EDEBF1EDEBF1EDEBF2EEEBF2EEEBF3EEEBF3
        EEEBF3EEEBF3EEEBF4F0EDF8F6F5FDFDFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFEFEEAD9CEBF8B68AA693CBB906DCFB79BD1BB9DD1BA9CD2BB9CD2BB9DD3
        BC9DD3BC9DD3BC9ED3BC9DD3BC9DD3BC9DC8B0925F391EAB9789FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6EFEAD0AA90A9683D
        AE7B55CCB094D7C2A6D6C0A2D6C0A2D6C0A2D7C0A2D7C0A2D6C1A2D6C0A1D7C0
        A2CCB597673D20AF988AFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFDFBFAE2CABBAF7750A0673DC29F80DAC4A8DCC8AADB
        C6A7DCC6A8DBC6A7DBC6A7DBC5A7DBC5A7D1BA9C6F4222B39B8AFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        EEE2DABF93769B5E34B38866D7C0A4E3CFB1E2CDAEE1CDADE1CDADE1CCADE1CC
        ADD7C0A2784725B79D8BFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F3EFD1B39F9F643CA36E49CE
        B296E7D5B7E9D6B7E7D5B5E7D4B4E7D4B5DEC7A8814C27BC9F8CFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFEFDFCE4D3C7AF7D5C995C34BF9879E6D2B6F0DEBFEFDBBCEFDC
        BCE5CFAF8A512AC0A28DFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF2E8E2C4
        9E859C5F35AF7D59DDC3A7F5E4C7F6E4C4EBD6B793572EC5A48EFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBF8F6D9C0AEA96F47A66A3FCFAB8DF2E3
        CFE8D7C4A9734FCBAA94FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFEFEECDED5BE8C6BAE7046D8BDAAE7D8CEC69D81DFC8B9FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F1EDDCBFACE1C8
        B8EFE3DAECDDD3FAF6F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFAF6F4FCF9F7FFFFFFFFFFFFFFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      OnClick = spbBackSpaceClick
    end
    object spbSummit: TSpeedButton
      Left = 560
      Top = 224
      Width = 160
      Height = 50
      Caption = 'Enter'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        361B0000424D361B000000000000360000002800000030000000300000000100
        180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFAF7F7F7FCFCFCFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF3F3F3
        CAD3CEB2C6BBD9DDDBF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFEFEFEF0F0F0B8CAC051B57F2BB56973B792D1D7D4F7F7F7FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEECECECABC5B742B374
        07BB5807BF5B12B85E65B489C8D2CDF6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFCFCFCE8E8E8A1C1AF39B16E09B95741CE8276DEA63ACB7B11B75A5AB381C0
        CEC6F3F3F3FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE4E5E498BDA82EAF650AB85547CF84
        9EEEC4B6F7D6ABEECA45CC800EB5564EB078B8CABFF0F0F0FEFEFEFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBDDE0
        DE8AB89D25AC5D09B55249CF858EECBA86F2B786F3B9ACF9D3B7F1D354CD880C
        B35142AD6EABC3B5ECECECFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF9F9F9D8DCDA7DB4931FAA570AB25048CE847DEAAE6FEDA5
        6FEFA780F2B592F7C4B4FBDABBF2D65BCE8C0DB05039AA67A1BFADE8E8E8FCFC
        FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8F8D1D7D373B08B17A7
        4F0BB04E44CE816AE7A157E89259EA946BEEA37BF2B18BF5BE99F9CBB3FCDBB9
        F3D65DCD8C0EAE4E2EA65E98BAA6E3E4E3FCFCFCFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFF5F5F5C7D0CA65AB8012A44A0DAF4D40CE7F55E4913DE27C42E48153E88F
        63EC9D72EFAA80F2B58CF5C095F8C8A5FAD1ADF2CF60CE8E0EAC4C25A45589B4
        99DDDFDEFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF2F2F2BFCCC358A7750DA1440DAC4B38CD
        793BDF7D24DC682BDE6E3BE27B4BE6885AE99567EDA173F0AB7EF2B485F4BA88
        F5BC91F5C19BF0C35FCE8D10AA4A1FA14E7CAF8FD7DBD8F9F9F9FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF0F0F0B6
        C6BC4EA36C099D3D0DAB4A30CC7328DB6E0BD75313D85923DC6733E07441E480
        4EE78C5AEA9665EC9F6EEEA674F0AB77F1AE76F0AD7AF0AE88EDB55BCE8B11A8
        49179D4672AC86D0D6D2F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFDFDFDECECECAABFB1419D600699390CAA4826CA6D1DD66303D1
        4801D3490BD75219DA5E27DE6A35E07641E4814CE68A56E8915EEB9862EC9C65
        EC9F64EC9E60EB9B60EB9A70EAA455CE8714A74911993F64A67AC7D0CAF5F5F5
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFCFCE7E7E7A1BAA9379A5604
        96340AA7451EC96614D15902CC4300CE4300D14704D54C0ED7551BDB6128DE6B
        34E0753EE37D46E5844DE78A52E88E53E88F53E88F50E78D4AE68847E58557E4
        914BCE8114A5470D963A58A16FBFCBC3F2F2F2FEFEFEFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FCFCFCE3E4E395B49E2F954E02933007A54215C65F0DCB4F01C73E00C94000CC
        4300CF4500D24705D54D0FD7561BDA5F25DD692EDF7036E1773CE37C40E37F42
        E48141E4803FE37E3AE27A33E0742DDF6F3DE07D3FCD7915A4470992334E9C66
        B6C5BBF0F0F0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFADDDFDE88AD92259144018F2C04A5400B
        C45706C64600C23A00C43C00C73E00CA4100CD4300CF4500D24704D54C0CD753
        16D95B1EDB6225DD682ADE6D2EDF702FE0712FDF712DDF6F28DE6C23DC661BDA
        5F13D95922DB6832CC7014A345078F2E41975AAABEAFECECECFDFDFDFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9D7DBD8
        7DA8881D8C3A018C2800A33C03C04E03C03E00BD3500C03800C23A00C53D00C8
        3F00CB4100CD4300CF4500D14703D44B0AD65111D85717DA5C1CDA601FDB631F
        DC631EDB621BDA6017DA5D12D8580BD75305D54D02D34914D55A27CA6812A142
        058B2A37914FA1B8A7E7E7E7FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFDCE1DD78A88318873300882400A13900BB4600BA3700
        B83100BB3400BE3600C03800C33A03C63E08CA4510CE4D19D25520D55C26D862
        2BDA6730DC6B34DD6E36DD7136DE7134DE6F2EDD6B26DB651AD95D0ED65306D4
        4C01D24700D04500CE430DD0501EC8610FA0400388252F8D4595B29CE3E4E3FC
        FCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAECBB4228C39
        008521009F3500B63E00B43000B32D00B62F00B83103BC360DC1401CC74E2ECE
        5E3CD36A45D77249D9774CDB7A4BDC7B39DA6E2ED86744DD784EDF7F4DE07F4D
        E07F4CE07F4BDF7D47DE7A40DD7434DA6B24D65E13D15106CC4500C93F07CB48
        15C6580CA03E02832125873A88AA8FDDDFDDFAFAFAFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFA2C6A9168A30009A3000B03700AE2B00AE2900B12B04
        B53116BD412FC65845CE6B52D37757D67C58D87E57D97F58DA8055DB7F3BD66C
        10CF4D06CE4721D45C4BDD7B57DF8457E08457DF8457DF8457DF8458DE8457DE
        8355DC804CD97939D46A21CE560AC64205C5410CC250079F3A02811F1D81317C
        A383D5D8D5F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFA3C9AC169E3E
        00AA2F00A82500A92402AD2814B53A38C35A54CD7360D27E63D48162D68262D7
        8262D88462DA865EDA843CD36B0ECA4927D05C5ADA831FD05820D25950DC7D62
        DF8B62DF8A62DF8A62DF8A62DE8962DE8962DD8863DC8862DB875BD98145D26E
        1CC64D04C03C05BE48029E37007E1B177C28709C77CFD4D0F8F8F8FFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFABD6B618A83900A32000A4200BAB2C30BA4E5BCA746C
        D1846ED3876DD4876DD6886DD7896DD88A6ED98C67D9883CD0670DC54431CE60
        B0EBC2EDFAF19DE8B426CF5A1ECE5554DA7E6DE0916DDF916DDF916DDF906DDE
        906DDD8F6DDD8F6DDC8E6EDB8D6EDA8C5CD47E29C55304BB3700BA3F009D3300
        7B18117820629469C7CDC8F5F5F5FFFFFFFFFFFFFFFFFFFFFFFFD6ECDA51BA63
        05A1200CA62946BE5E72CE8579D28C78D48D78D58E78D68F78D79078D89179DA
        936DD88A3BCB630DC13F3ECE67BBEDCAFCFEFDFFFFFFF8FDF9A9EABD32CF611C
        CB5157D97E77E09778E09878DF9778DF9678DE9678DE9578DD9578DC9478DB93
        78DA936AD58734C45908B73500B538009A2E0079150D7318588E5DBEC7BFF2F2
        F2FEFEFEFFFFFFFFFFFFFDFEFDD0ECD455BC660FA52A45BC5B7BD18C83D59482
        D59482D69582D89682D99783DA9973D78D38C75E0CBC3B4BCE6EC7EFD2FDFEFD
        FFFFFFFFFFFFFFFFFFFAFDFBB8ECC73BCE6619C74C57D67C80E09B83E09E82DF
        9D82DF9C82DF9C82DE9B82DD9A82DC9A82DB9983DB9977D68E42C5620DB43500
        AF3100972A007512096D124D8751B5C0B6F0F0F0FEFEFEFFFFFFFFFFFFFFFFFF
        DCF0DF63C27316A82F43BA5980D2918ED89D8CD89D8DDA9E8DDBA077D58E33C1
        560DB7385ACE78D6F2DEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFEC8
        F0D348CF6E18C34954D37787E0A08DE1A58CE0A38DE0A38DDFA38DDFA28DDEA1
        8DDDA18DDCA08EDCA086D89955C76F15B13700AA2B00952600720E066A0B3F7E
        41AAB8AAEBEBEBFDFDFDFFFFFFFFFFFFFFFFFFE2F3E572C88019A93240BA5685
        D49499DCA798DCA77AD38E30BD5012B43869CF82DDF4E3FFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFED1F1DA57D07817C04553CF748EE0
        A499E2AC98E2AB98E1AB98E1AA98E0A998DFA998DFA898DEA898DDA794DAA365
        C9791DAF3A01A42500912000700B046506377837A3B5A3ECECECFFFFFFFFFFFF
        FFFFFFFFFFFFE9F6EB81CD8D1AA8333BB75288D5977CD28D2AB74815B13678D1
        8CE6F6EAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFDBF4E165D18017BC424ECB6E95E0A8A4E4B4A3E3B2A3E3B2A3E2B2
        A3E1B1A3E1B0A3E0B0A3DFAFA3DFAFA0DDAC73CB8321AC39009E1E008D1A006E
        090262023B7D3BBFCDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1F9F290D29A22
        AA3A28B0411EAE3A1CAF3989D599EEF9F0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE6F7EA75D48C1AB8
        4047C66699DFAAAFE5BBADE5BAADE4B9ADE4B9ADE3B8ADE2B8ADE2B8ADE2B7AE
        E1B7ACE0B580CE8D28AA3B029919008B16006E08166D16A3BEA3FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF4FAF59FD8A82FB0452AB04199D9A5F2FAF4FFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFEBF8EE85D7981FB64143C15F9ADEAAB9E8C3B9E7C3
        B8E6C2B9E6C2B9E6C1B9E5C0B8E5C0B8E4C0B9E4C0B8E3BF8FD19835AB440395
        16008812167A1DA2BEA2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8
        FCF9CCEBD1C8EACEF7FCF8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1FA
        F395DAA322B3403BBC569DDCAAC5EACCC4EACBC4E9CBC4E9CAC4E9CAC4E8CAC4
        E8C9C4E7C9C4E7C9C4E6C8A1D7A742AD4D049112168F23A5C9A9FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8FCF9A4DDAF2EB44832B54B9BDAA6
        CDECD2CFEDD4CEECD3CEECD3CEECD3CEEBD2CEEBD2CEEAD2D0EBD3CCE9CF7DC5
        830D91172B9B32BEDDC0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFF9FDFAB2E1BB38B44E2DAF4395D6A0D5EFD9DAF1DED9EFDDDAF0DCDA
        EFDCD9EFDCDAEFDDD3ECD68DCB92269C2E24982B99CE9CF2F8F2FFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFEFCC1E6C740B351
        28A93A90D29ADFF2E2E6F4E8E5F4E6E5F4E6E6F4E7DBF0DD86C88B1F97272B9B
        31A8D5AAF7FBF7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFEFEFECDE9D151B65E21A33289CE91E3F3E5F1F9F2F2
        F9F2DFF1E07EC48318941F39A13EB6DCB8FCFDFCFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        D6EDD95FB968209E2D80C787E8F5E9E2F2E374BF7916921D46A64AC3E2C4FCFD
        FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE0F0E26BBB711F992968BA6E60
        B56513901A52AC55CFE7D0FEFEFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFE8F3E97CC0801A9220108D1663B365DAECDBFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEDF6EDA2D2A497
        CC99E2F0E2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      OnClick = spbSummitClick
    end
    object spbClear: TSpeedButton
      Left = 600
      Top = 168
      Width = 120
      Height = 50
      Caption = 'Delete'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        361B0000424D361B000000000000360000002800000030000000300000000100
        180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFAEAEAEAE0E0E0DFDFDFDF
        DFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDF
        DFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFDFE0E0E0EAEAEAFAFAFAFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        F5F5F5C1C1C17B7B7B4B4B4B4040404040404040404040404040404040404040
        4040404040404040404040404040404040404040404040404040404040404040
        4040404040444444747474D1D1D1FCFCFCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFEFEFEF4F4F4B4B4B43838384B4B4B30303016161616
        1616161616161616161616161616161616161616161616161616161616161616
        1616161616161616161616161616161616161616162424246060608D8D8DF6F6
        F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEFEFEFA8A8A8
        1F1F1F0D0D0D5456555B5E5D6063626669686B6F6D717473767A787C7F7E8084
        838488878589878286847D8180787C7A7376756D716F686B6A6265645D605E57
        5A58525453585A5A323232656565F4F4F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFEFEFEEBEBEB9A9A9A1C1C1C1010104A4C4B474A484B4E4D51545356
        59585C5F5E616563666A696B6F6D6F73717176747276747074726D716F686C6B
        6468665F6261585B5A5256544D504E464948414342494B4A1414145D5D5DF4F4
        F4FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDEAEAEA8F8F8F141414121212
        4748473C3E3D4144434D4F4E5457565457565659585A5E5D5F62616266646569
        67676A69676B69666A686467666366656467665F62615557564E504F484B4942
        45443D3F3E4547461313135E5E5EF5F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCFC
        FCE2E2E28080800909091818183D3E3E3032313739384345447375749C9D9C5D
        5F5E545655525654555957585B5A5A5D5C5B5F5D5B5F5D5B5E5D5D605F656867
        9698978F9190565857494C4B4244433C3F3E3739383F41401111115F5F5FF5F5
        F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFCFCFCDDDDDD7272720909091B1B1B303130252726
        2C2E2D383A3A696A6ADADADAE4E4E4A6A7A75D5F5E5053524C4F4E4E51504F52
        515053525154535558576265649A9B9BE1E1E1E1E1E18788884B4D4C3D3F3E36
        3837313332393A3A101010606060F5F5F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFAFAFADBDBDB6262
        620606061C1C1C2324241A1B1A2021202D2E2E606161D7D8D8E7E7E7E7E7E7E6
        E6E6A5A6A55D5F5E4B4D4C4447464547464648474C4F4E5E605F969797E4E4E4
        E7E7E7E7E7E7E0E0E07F81803E3F3F3032312A2C2B3133320F0F0F616161F5F5
        F5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFF9F9F9D0D0D05353530101011B1B1B1414140D0E0E1414141F2020
        5A5B5ADADADAE8E8E8E8E8E8E8E8E8E8E8E8E8E8E8A9AAAA5D5E5E4346453B3D
        3C4345445B5C5B9A9B9BE5E5E5E8E8E8E8E8E8E8E8E8E8E8E8E3E3E37B7B7B31
        3232232524292A290D0D0D626262F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7F7C8C8C84646460404041919
        190909090202020607060E0F0E2A2A2AD4D4D4EAEAEAE9E9E9EAEAEAEAEAEAE9
        E9E9EAEAEAE8E8E8ABABAB595B5A4546465557569A9A9AE4E5E5E9E9E9EAEAEA
        EAEAEAE9E9E9EAEAEAEAEAEAE1E2E14B4C4C1E1F1F2021210C0C0C636363F6F6
        F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF6
        F6F6C5C5C5363636040404181818050505000000000000010101050505151515
        6E6E6EE5E5E5EBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEAEAEAA8A9A96566
        65969797E8E8E8EBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBEBE9E9E991919125
        26261314141717170B0B0B636363F6F6F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFEFEFEF2F2F2B4B4B42929290505051515150303030000
        000000000000000000000000000404041717176C6C6CE4E4E4ECECECECECECEC
        ECECECECECECECECECECECECECECC7C7C7E9E9E9ECECECECECECECECECECECEC
        ECECECECECECE9E9E98D8D8D2626261010100707070D0D0D090909646464F6F6
        F6FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEF1F1F1A9A9A922
        2222050505111111020202000000000000000000000000000000000000000000
        0606061C1C1C707070E7E7E7EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDED
        EDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEDEBEBEB8E8F8F2728280C0C0C02
        0202000000080808080808656565F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFEFEFEF0F0F0A4A4A41616160909091E1E1E1515151818181C1C1C2121
        212525252828282B2B2B2D2D2D2D2D2D2D2D2D313131444444979797F0F0F0F2
        F2F2F1F1F1EFEFEFEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
        ECECEC9797972828280B0B0B010101000000000000070707070707666666F7F7
        F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFBFBFBB1B1B10F0F0F0E0E0E3B
        3B3B4040403E3E3E3D3D3D3A3A3A373737353535343434333333323232303030
        303030303030383838515151A1A1A1F9F9F9FEFEFEFEFEFEFEFEFEFDFDFDFBFB
        FBF9F9F9F7F7F7F5F5F5F3F3F3EFEFEF9797972E2E2E0B0B0B01010100000000
        0000000000050505050505676767F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFAFAFA7A7A7A0101013737374141414040403F3F3F3E3E3E3D3D3D3B3B
        3B3A3A3A3939393838383737373636363535353434343333333B3B3B575757A6
        A6A6FCFCFCFDFDFDFDFDFDFEFEFEFEFEFEFFFFFFFFFFFFFFFFFFFFFFFFC5C5C5
        7D7D7D5D5D5D4343432424240D0D0D010101000000040404040404686868F7F7
        F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDC5C5C51414140C0C0C3C
        3C3C4545454444444343434242424141414040403F3F3F3E3E3E3C3C3C3C3C3C
        3A3A3A3939393838383F3F3F5A5A5AA7A7A7FAFAFAFCFCFCFCFCFCFDFDFDFDFD
        FDFEFEFEFEFEFEFEFEFEFFFFFFC0C0C05959593434343434344949495959595C
        5C5C5252522E2E2E030303686868F8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFCFCFCC7C7C72020200909093A3A3A4A4A4A4949494747474747
        474545454444444343434242424141414040403F3F3F444444595959A7A7A7F8
        F8F8FBFBFBFBFBFBFBFBFBFCFCFCFCFCFCFDFDFDFDFDFDFDFDFDFEFEFEFEFEFE
        BEBEBE5555553636362C2C2C2A2A2A2929292C2C2C6A6A6A0303036A6A6AF8F8
        F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDCFCFCF34
        34340B0B0B3A3A3A4E4E4E4D4D4D4C4C4C4B4B4B4A4A4A494949484848464646
        464646494949595959A4A4A4F5F5F5FAFAFAFAFAFAFAFAFAFBFBFBFBFBFBFBFB
        FBFCFCFCFCFCFCFCFCFCFDFDFDFDFDFDFCFCFCBBBBBB5151513838383030302E
        2E2E2D2D2D5858580606066D6D6DF8F8F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFDFDFDDBDBDB4343430E0E0E3B3B3B5353535151
        515050504F4F4F4E4E4E4D4D4D4C4C4C4E4E4E5A5A5A9D9D9DF6F6F6F9F9F9FA
        FAFAFAFAFAFAFAFAFBFBFBFBFBFBFBFBFBFBFBFBFCFCFCFCFCFCFCFCFCFCFCFC
        FDFDFDFDFDFDB5B5B54F4F4F3B3B3B3434343333335858580B0B0B707070F9F9
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFE
        FEFEECECEC585858121212383938585858565656555555535353535353545454
        5C5C5CA0A0A0F6F6F6F9F9F9F9F9F9FAFAFAFAFAFAFAFAFAFAFAFAFAFAFAD6D6
        D6F7F7F7FBFBFBFCFCFCFCFCFCFCFCFCFCFCFCFDFDFDFDFDFDB6B6B64F4F4F3D
        3D3D3838385858580F1010737473F9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEEEEEE6E6F6F1718183A3A
        3A5B5B5B5A5A5A5959595959595E5E5E9E9E9EF4F4F4FAFAFAFAFAFAFAFAFAFA
        FAFAFAFAFAFAFAFAFAFAFABABABA797979A9A9A9F8F8F8FCFCFCFCFCFCFCFCFC
        FCFCFCFDFDFDFDFDFDFBFBFBB3B3B34C4C4C3F3F3F595959141414777777F9F9
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF3F3F3858585191A193939396262625E5E5E5E5E5E6B6B6B
        ECECECFAFAFAFAFAFAFAFAFAFAFAFAFBFBFBFBFBFBFAFAFABCBCBC6E6E6E5F5F
        5F686868AAAAAAF8F8F8FCFCFCFCFCFCFCFCFCFDFDFDFDFDFDFDFDFDFBFBFB6D
        6D6D4646465959591819187A7A7AF9F9F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F99A9A
        9A252626353736676767636363646464868686ECECECFCFCFCFCFCFCFCFCFCFC
        FCFCFAFAFAB7B7B76F6F6F5D5D5D5757575A5A5A686868A4A4A4F7F7F7FDFDFD
        FDFDFDFDFDFDFDFDFDF6F6F69090904F4F4F4A4A4A5A5A5A1C1D1D7C7D7DF9F9
        F9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFF9F9F9ACACAC2E2F2F3738376A6A6A686868
        6969698C8C8CEEEEEEFDFDFDFDFDFDFBFBFBB7B7B77171716363635C5C5C5B5B
        5B5A5A5A5D5D5D696969A2A2A2F9F9F9FDFDFDFEFEFEF6F6F694949457575750
        50504E4E4E5C5C5C2122227F8080FAFAFAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFBFBFBC0C1C03435343637377070706C6C6C6D6D6D8D8D8DF3F3F3FDFDFDBB
        BBBB7272726868686363636161616060605F5F5F5E5E5E616161686868A3A3A3
        FAFAFAFBFBFB9595955D5D5D5757575555555353535D5D5D252626838383FAFA
        FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDFDFDD0D0D0474848333434
        7373737272727171718F8F8FB1B1B17474746D6D6D6A6A6A6868686767676666
        666565656464646363636363636868689B9B9B9797976262625D5D5D5B5B5B5A
        5A5A5959595F5F5F2A2B2B868786FBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFDFDFDDADADA56575736383772737379797975757575757572
        72727070706F6F6F6E6E6E6C6C6C6B6B6B6A6A6A696969686868676767676767
        6868686767676464646262626060605F5F5F5E5E5E6262623638378A8A8AFBFB
        FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEE7E7E7
        616262393B3A7F7F7F9696969595959494949393939292929090908F8F8F8E8E
        8E8D8D8D8C8C8C8B8B8B8A8A8A88888887878786868685858583838382828281
        81818080808080808182819C9C9CFBFBFBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEEFEFEF8384849597969C9C9C44464537
        3938373938373938373938373938373938373938373938373938373938373938
        373938373938373938373938373938373938393B3B686969AFAFAFC8C8C8FDFD
        FDFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFF4F4F4CECFCFB9B9B99B9C9B9798989798989798989798989798989798
        9897989897989897989897989897989897989897989897989897989897989897
        9898979898A1A1A1C7C8C8F7F7F7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFEFEFEFEFEFEFEFEFE
        FEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFE
        FEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFEFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      Margin = 0
      ParentFont = False
      OnClick = spbClearClick
    end
    object spbCancel: TSpeedButton
      Left = 396
      Top = 224
      Width = 160
      Height = 50
      Caption = 'Abort'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Arial'
      Font.Style = []
      Glyph.Data = {
        361B0000424D361B000000000000360000002800000030000000300000000100
        180000000000001B0000C40E0000C40E00000000000000000000FFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF878C
        BB252B875E5FA8F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB6767BB
        5555CB9898D2FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFF8F8FC82634791C25770E13703E3E90F6F6FAFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFF6F6FA3F3F922B2BA16868E22B2BAA8F8FC8FFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810A0E
        7F4E59A01216740000613A3A88F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA3A3A89000065111282
        8186E40B0B8B0000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFF8F8FC80000810000815860BD636AF27F89D8070868000055383883
        F6F6FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFF6F6FA37378200005807086D7F89D9656CF35860BD0000810000818F8FC8FF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FC80000810000815860BD4E54
        F10000F51B1DF57E88D507085B00004B35357EF6F6FAFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FA35357D00004B0708607E88D71B1DF5
        0000F54E54F15860BD0000810000818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8E
        8EC7000081000081585FBD4D54F10000F50000F50000F51A1DF57E87D307084E
        00004233337BF5F5FAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5F5FA3333
        7A0000420708527E88D41B1DF50000F50000F50000F54E54F15860BD00008100
        00818F8FC8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFF9898CC1B257C0B0F7F555EBD4B53F10000F50000
        F50000F50000F50000F51A1CF47B86CE07084000003C33337BF5F5FAFFFFFFFF
        FFFFFFFFFFFFFFFFF5F5FA32327900003A0708447D87D01A1DF50000F50000F5
        0000F50000F50000F54E54F15860BD11118916168C9999CDFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF24288B21
        2D7B4F5EA25C67F20000F50000F50000F50000F50000F50000F50000F5191CF4
        7785CA06073500003832327AF5F5FAFFFFFFFFFFFFF5F5FA3232790000350607
        377A85CC1A1CF40000F50000F50000F50000F50000F50000F50000F5636BF463
        69B939399D2E2E97FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFF5E61AA212D7B161E837384DA181CF10000EF0000
        EF0000EF0000EF0000EF0000EF0000EE181CEE7382C406062B00003632327AF4
        F4F9F5F5FA31317800003206062D7683C6191CEE0000EE0000EF0000EF0000EF
        0000EF0000EF0000EF191CF17C89DB4949A55959AD6E6EB7FFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB46
        499F000081060785697ED9161BEC0000E90000E90000E90000E90000E90000E9
        0000E8171BE86E7FBE0606250000342D2D6F2E2E700000300606257181C0181C
        E80000E80000E90000E90000E90000E90000E90000E9191CEC7685D907078503
        03826A6AB6F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4646A40000810607855F7BD8141A
        E60000E30000E30000E30000E30000E30000E30000E1151AE1647BB906061F00
        002300002106061E687CBB161BE20000E10000E30000E30000E30000E30000E3
        0000E3171BE67082D80607850000814646A4F7F7FBFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFF7F7FB4646A40000810507855577D61219E00000DC0000DC0000DC0000DC
        0000DC0000DB0000D91319DA5A77B50506130506135C78B6141ADB0000DA0000
        DB0000DC0000DC0000DC0000DC0000DC151AE0657CD60607850000814646A4F7
        F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FB4545A50000830506
        874D74D61118DB0000D60000D60000D60000D60000D60000D50000D21219D450
        73B15274B21219D40000D30000D50000D60000D60000D60000D60000D61219DB
        5A79D60607870000834545A5F7F7FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFF7F7FB4444A60000850406884B72D51018D50000D00000D0
        0000D00000D00000D00000CE0000CB0D13CC0D13CD0000CB0000CE0000D00000
        D00000D00000D00000D01118D54E73D50506880000854444A6F7F7FBFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4444
        A700008704068A4770D41018D00000CB0000CB0000CB0000CA0000CA0000C700
        00C50000C50000C80000CA0000CB0000CB0000CB0000CB1018D04B71D404068A
        0000874444A7F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4343A800008904068C446FD40F18CA
        0000C40000C40000C40000C30000C20000C10000C10000C30000C40000C40000
        C40000C41018CA4770D404068C0000894343A8F6F6FBFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFF6F6FB4242A900008B04068E416DD30E17C50000BE0000BE0000BE0000BE00
        00BD0000BD0000BE0000BE0000BE0000BE0F18C5436FD304068E00008B4242A9
        F6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AB00008D040690
        3E6CD20A12BF0000B80000B80000B80000B80000B80000B80000B80000B80B12
        BF416DD204069000008D4242ABF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFF6F6FB4242AC00008F030692537BD7191FBE0000B20000B20000B200
        00B20000B20000B20000B20000B20A12B93E6CD204069200008F4242ACF6F6FB
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6FB4242AD000091030694416DD7
        3139CB3A3AC93938C72C2CC32121BE3434C34F4FCA6666CF5251C70606AD0000
        AB0D17B53B6AD20306940000914242ADF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6F6
        FB4343AF000093030696436ED7373ECB3030C53030C53030C53030C53030C530
        30C43030C43232C55858D28B8BE18C8BDD4040BE0C17AF396BD1030696000093
        4343AFF6F6FBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFF6F6FB4444B1000095030698456FD83E45CD3737C7
        3737C73737C73737C73737C63737C43737C13737C13737C43737C63737C73B3B
        C77A79DC9C9BE48C90DC507AD60306980000954444B1F6F6FBFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4444B30000
        9703069A4770D9444ACF3E3EC93E3EC93E3EC93E3EC93E3EC93E3EC73E3EC260
        65CC6064CB3E3EC13E3EC63E3EC83E3EC93E3EC93E3EC95554D09B9DE97A96E3
        03069A0000974444B3F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFF7F7FC4545B500009904069C4971DA4C51D14646CB4646CB4646CB
        4646CB4646CB4646CA4646C66B6FD0708FD66D8DD4686DCD4646C54646CA4646
        CB4646CB4646CB4646CB4646CB4F54D26586DF04069C0000994545B5F7F7FCFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646B600009B04069E4A72
        DA5358D44E4ECF4E4ECF4E4ECF4E4ECF4E4ECF4E4ECE4E4ECC7377D67A96DA03
        06670306637693D56F73D24E4ECB4E4ECE4E4ECF4E4ECF4E4ECF4E4ECF4E4ECF
        6064D77D99E504069E00009B4646B6F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7
        F7FC4646B800009D0406A04D73DB595DD85656D35656D35656D35656D35656D3
        5656D35656D37A7DDE849DDF0306720000660000610306647E99DA7477DA5656
        D25656D35656D35656D35656D35656D35656D37276E08EA5EA0406A000009D46
        46B8F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747BA00009F0406A24F74DD6164DC5D5D
        D85D5DD85D5DD85D5DD85D5DD85D5DD85D5DD88285E48CA3E503068300007A36
        369735359200006B030671849DE07A7DE05D5DD85D5DD85D5DD85D5DD85D5DD8
        5D5DD85D5DD8888BE89AADEC0406A200009F4747BAF7F7FCFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF6262C500
        00A10407A45076DD676AE26565DF6565DF6565DF6565DF6565DF6565DF6565DF
        898BE994A8EA03069600008A3B3BA6F4F4FAF4F4FA38389D00007903068689A1
        E67F81E76565DF6565DF6565DF6565DF6565DF6565DF6565DF9D9FF1A3B4EE04
        07A40000A16262C5FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFF1D1DAD0000A31F36BD707BEC6D6DE66D6DE66D6D
        E66D6DE66D6DE66D6DE66D6DE69091EF9AADED0306A300009B3F3FB1F5F5FBFF
        FFFFFFFFFFF5F5FB3C3CA700008F03069C8FA4EA8486ED6D6DE66D6DE66D6DE6
        6D6DE66D6DE66D6DE66D6DE6CBCDFB1B33BA0000A31D1DADFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF9999DB00
        00A50000A54360D07B80EF7474ED7474ED7474ED7474ED7474ED9596F4A0B1EF
        0406A80000A44343BAF6F6FCFFFFFFFFFFFFFFFFFFFFFFFFF5F5FB4141B40000
        A00406A794A8ED8989F27474ED7474ED7474ED7474ED7474EDAAADF7697ED900
        00A50000A59999DBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FD80000A70000A7536CD5878BF47B7B
        F47B7BF47B7BF49B9BF9A4B5F10406AA0000A74444BEF6F6FCFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFF6F6FC4343BC0000A70406AA97A9EE8E8FF77B7BF4
        7B7BF47B7BF4ADB0F96B81DB0000A70000A78F8FD8FFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFF8F8FD90000A90000A95E76D99295F88383FC9F9FFDAAB8F20406AC0000A9
        4545C0F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7
        FC4545C00000A90406AC9AABEF9292FC8383FCB0B2FB6D83DC0000A90000A98F
        8FD9FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDA0000AB0000AB687E
        DCB4B5FDADBAF30406AE0000AB4646C2F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4646C20000AB0406AE9CAEF0
        BBBCFD7084DE0000AB0000AB8F8FDAFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFF8F8FDB0000AC0101AC8EA3E51215B10000AC4646C3F7F7FC
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFF7F7FC4646C30000AC0B0EB0AABEEF0808AC0000AC8F8FDBFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF8F8FDB0D0D
        ACA7A7DF2C2CB24747C5F7F7FCFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF7F7FC4747C52020AF
        C2C2EC1B1BAE8F8FDBFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFF9797DC5B5BCA6464CCF7F7FDFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFF7F7FD6363CB6D6DD19797DDFFFFFFFFFFFFFFFFFFFF
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
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF}
      ParentFont = False
      OnClick = spbCancelClick
    end
  end
  object palValueLimit: TPanel
    Left = 0
    Top = 49
    Width = 1005
    Height = 140
    Align = alTop
    BevelOuter = bvNone
    Color = 12761254
    Enabled = False
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 15
      Width = 118
      Height = 19
      Caption = 'Current Value : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object labMaxValue: TLabel
      Left = 8
      Top = 59
      Width = 87
      Height = 19
      Caption = 'Maximum : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object labMinValue: TLabel
      Left = 8
      Top = 103
      Width = 79
      Height = 19
      Caption = 'Minimun : '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object edCurrValue: TEdit
      Left = 132
      Top = 4
      Width = 120
      Height = 40
      AutoSize = False
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 0
      Text = '10.0'
      OnKeyDown = edQwertyContentKeyDown
      OnKeyPress = edQwertyContentKeyPress
    end
    object edMaxValue: TEdit
      Left = 132
      Top = 48
      Width = 120
      Height = 40
      AutoSize = False
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 1
      Text = '10.0'
      OnKeyDown = edQwertyContentKeyDown
      OnKeyPress = edQwertyContentKeyPress
    end
    object edMinValue: TEdit
      Left = 132
      Top = 92
      Width = 120
      Height = 40
      AutoSize = False
      Color = 14670284
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 2
      Text = '10.0'
      OnKeyDown = edQwertyContentKeyDown
      OnKeyPress = edQwertyContentKeyPress
    end
  end
end
