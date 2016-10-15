object FormJakelu: TFormJakelu
  Left = 0
  Top = 0
  Caption = 'Tiedostojakelun m'#228#228'rittely'
  ClientHeight = 381
  ClientWidth = 794
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 17
    Top = 61
    Width = 82
    Height = 13
    Caption = 'Palvelimen osoite'
  end
  object Label2: TLabel
    Left = 266
    Top = 130
    Width = 117
    Height = 13
    Caption = 'Palvelimen tiedostopolku'
  end
  object Label4: TLabel
    Left = 17
    Top = 126
    Width = 131
    Height = 13
    Caption = 'K'#228'ytt'#228'j'#228'tunnus palvelimella'
  end
  object Label6: TLabel
    Left = 266
    Top = 87
    Width = 173
    Height = 13
    Caption = ' L'#228'hetett'#228'v'#228' tiedosto tai hakemisto '
  end
  object Label7: TLabel
    Left = 17
    Top = 105
    Width = 79
    Height = 13
    Caption = 'Palvelimen portti'
  end
  object LblYhteys: TLabel
    Left = 168
    Top = 32
    Width = 65
    Height = 16
    Caption = 'Yhteys: Ei'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGray
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object EdServerAddr: TEdit
    Left = 17
    Top = 77
    Width = 193
    Height = 21
    TabOrder = 0
    OnChange = EdServerAddrChange
  end
  object EdServerPath: TEdit
    Left = 266
    Top = 148
    Width = 263
    Height = 21
    TabOrder = 9
    OnExit = EdServerPathExit
  end
  object EdUser: TEdit
    Left = 17
    Top = 141
    Width = 193
    Height = 21
    TabOrder = 1
    OnChange = EdUserChange
  end
  object RGAuth: TRadioGroup
    Left = 17
    Top = 169
    Width = 168
    Height = 61
    Caption = ' Autentikointimenettely '
    ItemIndex = 0
    Items.Strings = (
      'Salasana'
      'Julkisen avaimen tiedosto')
    TabOrder = 3
    OnClick = RGAuthClick
  end
  object RGTiedot: TRadioGroup
    Left = 266
    Top = 21
    Width = 168
    Height = 61
    Caption = ' L'#228'hetett'#228'v'#228' aineisto '
    ItemIndex = 0
    Items.Strings = (
      'Yksi tiedosto'
      'Hakemiston sis'#228'lt'#246)
    TabOrder = 6
    OnClick = RGTiedotClick
  end
  object EdSource: TEdit
    Left = 266
    Top = 102
    Width = 263
    Height = 21
    TabOrder = 8
    OnExit = EdSourceExit
  end
  object BtnSelFile: TButton
    Left = 479
    Top = 72
    Width = 50
    Height = 26
    Caption = 'Valitse'
    TabOrder = 7
    OnClick = BtnSelFileClick
  end
  object BtnOpen: TButton
    Left = 266
    Top = 237
    Width = 183
    Height = 25
    Caption = 'Avaa yhteys'
    TabOrder = 10
    OnClick = BtnOpenClick
  end
  object RGKeepOpen: TRadioGroup
    Left = 17
    Top = 304
    Width = 168
    Height = 61
    Caption = ' Yhteyden pysyvyys '
    Enabled = False
    ItemIndex = 0
    Items.Strings = (
      'Avaa aina siirron ajaksi'
      'Pid'#228' pysyv'#228'sti avoinna')
    TabOrder = 5
    OnClick = RGKeepOpenClick
  end
  object BtnClose: TButton
    Left = 266
    Top = 268
    Width = 183
    Height = 25
    Caption = 'Sulje yhteys'
    TabOrder = 11
    OnClick = BtnCloseClick
  end
  object BtnSendOnce: TButton
    Left = 266
    Top = 299
    Width = 183
    Height = 25
    Caption = 'Siirr'#228' tiedostot kerran'
    TabOrder = 12
    OnClick = BtnSendOnceClick
  end
  object EdServerPort: TMaskEdit
    Left = 176
    Top = 102
    Width = 29
    Height = 21
    TabStop = False
    Alignment = taRightJustify
    EditMask = '99999;1; '
    MaxLength = 5
    TabOrder = 15
    Text = '21   '
    OnChange = EdServerPortChange
  end
  object GBKey: TGroupBox
    Left = 17
    Top = 235
    Width = 193
    Height = 56
    Caption = ' Avaintiedosto '
    TabOrder = 4
    Visible = False
    object CBKeyFile: TComboBox
      Left = 17
      Top = 24
      Width = 160
      Height = 21
      TabOrder = 0
      OnDropDown = CBKeyFileDropDown
    end
  end
  object BtnSelaa: TButton
    Left = 471
    Top = 237
    Width = 58
    Height = 41
    Caption = 'Selaa palvelinta'
    Enabled = False
    TabOrder = 13
    WordWrap = True
    OnClick = BtnSelaaClick
  end
  object Panel1: TPanel
    Left = 540
    Top = 10
    Width = 241
    Height = 363
    TabOrder = 14
    Visible = False
    DesignSize = (
      241
      363)
    object Panel2: TPanel
      Left = 0
      Top = 34
      Width = 17
      Height = 19
      BevelOuter = bvNone
      Caption = ' >'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object lbRootDir: TEdit
      Left = 17
      Top = 35
      Width = 224
      Height = 19
      Anchors = [akLeft, akTop, akRight]
      Ctl3D = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentCtl3D = False
      ParentFont = False
      TabOrder = 1
      Text = '.'
    end
    object FileView: TTreeView
      Left = 0
      Top = 55
      Width = 241
      Height = 308
      Align = alCustom
      BorderWidth = 1
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      HideSelection = False
      Images = ImageList1
      Indent = 19
      ParentFont = False
      ReadOnly = True
      RowSelect = True
      ShowButtons = False
      ShowLines = False
      ShowRoot = False
      SortType = stText
      TabOrder = 2
      OnDblClick = FileViewDblClick
      OnKeyDown = FileViewKeyDown
    end
    object BtnOtaPolku: TButton
      Left = 6
      Top = 1
      Width = 54
      Height = 33
      Caption = 'Ota polku k'#228'ytt'#246#246'n'
      TabOrder = 3
      WordWrap = True
      OnClick = BtnOtaPolkuClick
    end
    object BtnLuoKansio: TButton
      Left = 63
      Top = 1
      Width = 78
      Height = 33
      Caption = 'Luo alla m'#228#228'ri- telty kansio'
      TabOrder = 4
      WordWrap = True
      OnClick = BtnLuoKansioClick
    end
    object BtnPoistaTiedosto: TButton
      Left = 143
      Top = 1
      Width = 53
      Height = 33
      Caption = 'Poista tiedosto'
      TabOrder = 5
      WordWrap = True
      OnClick = BtnPoistaTiedostoClick
    end
    object BtnPaivita: TButton
      Left = 198
      Top = 2
      Width = 41
      Height = 33
      Caption = 'P'#228'ivit'#228
      TabOrder = 6
      WordWrap = True
      OnClick = BtnPaivitaClick
    end
  end
  object CBPoista: TCheckBox
    Left = 272
    Top = 175
    Width = 201
    Height = 17
    Caption = 'Poista tiedosto l'#228'hett'#228'misen j'#228'lkeen'
    TabOrder = 16
    OnClick = CBPoistaClick
  end
  object BitBtn1: TBitBtn
    Left = 498
    Top = 8
    Width = 31
    Height = 31
    Margins.Left = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Glyph.Data = {
      FE0A0000424DFE0A00000000000036000000280000001E0000001E0000000100
      180000000000C80A000000000000000000000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCBB5A9C3A89EB297
      8DA689849B857F968D839391869C9A90ABA8A0BFB9B4FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFB3A393A48C78A88B76BC9C89D8B7A7F1D2C3
      FFE5D5FBEAD7EADCC9CFC1AFAEA191988D7F9C9288B7ACA4FFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFB5A69D9F8E7BB9A489DFC8A8FFEDCAFFFFDFFFFFE3FFFFE3FFFFE4FF
      FDDFFFFFE7FFFFE9FFFFE9FDE9D0D4C3AEAFA18F978C7EFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCB8AD
      B19786FBDDC0FFEFCBFFFFD7FFFFD5FFECC0EFD7ADE9D4AEF6DCB7FFDFB9FFDD
      B6FFE4BEFFF8D4FFFFE3FFFFE0F6E1C5D3C3ACB2A793C4BBADFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFD9C8BFCAAE9DF4CFB3FF
      F7D1FFE9BBFFD29FF1B884D8A370CE9D6DD6A97DE8B98DF2BC8DFCC493FECA9B
      FACC9DF8CFA2FFDEB3FFF8D3FFFFE4DAC9AEC7BAA4D7CEC0FFFFFFFFFFFFFFFF
      FFFFFFFF0000FFFFFFFFFFFFFDFFFFD4CAC3BCA798F5D2B8FFFFDFFFC695EFAC
      75DD9559DB9357E19C63DF9F69CF9564C78C5BDC9A65DF9A61DC9C65E2A76FEB
      B782F7CA97FFD9A9FFE0B5FFFFE3D9C7AACBBDAAF7EEE1FFFFFBFFFEFFFFFDFF
      0000FFFFFFFFFFFFF6F1EEAB9B8FFFF5DEFFF9D7F3B587D48A50DF8F4EE4914D
      E59451D28749DA975EF9BC8AF3B481EDA468DE9152EBA161DF9A5BCD8E51DEA6
      6BEDBC84F1C693F8D5A9FFFFE3F1DCC1B4A694FFFEF5FFFBFAFFFBFF0000FFFF
      FFFCF6F7CCBAB3DCBEADFFFBDFF2B48BD0854DDB8543E28841E38A40E6944CE0
      9956FFC185FFFFCCFFFFD2FFD092F3A05CEC9B57E99A57DC934FE4A05FE5A669
      E4AD74F9CA97FDD7ADFFF6D7D1BCA6EFE1D5FFFCF9FFFFFF0000FFFFFFF5DAD6
      D1AB9FFFF1DCFFD3B0C67746C36A2DDF8138E48738E28A3DE09448D39853ECBF
      82FFFECCFFFFD4FFEAACF9A35DE7914BEE9852EB9650E89851DD924ED89354EF
      B17BDCAA7CFFF5D2FFEACFCBB9A8FFFBF3FFFFFF0000FFFFFFC7AAA1FFDAC9FF
      FEE1CA8056BD642BDB7834EC8536ED8734E48535DB883DC68645DDAE76FFF3C6
      FFFFD0FFCE8FEA924CE68C45F2964FF2944BEF934AE28A43DD8947DF965AE1A0
      6DFAC69EFFFFE6C6AD99F2E4D8FFFFFF0000F9F9EDA79B89FFFFE8FAC6A19C55
      23BF6525EA8137E3701FF5802BFC8D3DF49048CB793FBE7C4CCC9874C48D66E4
      9A60D37C39E68B46F4954CEE8B41F48E45F08A42E88743D98043F0A36BD29367
      FFF8D6F3D5BCC5B5A4FFFFFF0000DEDECCCDBFA8FFFAD9B67F54B2672FC66521
      E57628EE7420F57A24EC7929E17A35C7743DE19C74FFE5CAFFFAD9DC925CD07A
      38E68B46F18F47EB8239F3863CF3843BEF833CDB7737EB945BC78251F4C098FF
      FFE7A89782FCF6E90000D0BAA8FFEAD0FFE0BCAA6435C76F33D36922DF6918EA
      6F17EF7620E47827D57A35BC7942D6A57DFFF6DBFFFFE6FFBF8ADE8A49DE8340
      EB8641EC7E36F27F36F27A32F07C35DD722FD7793ED38655CF976EFFFFE6C1AF
      98E2DDCE0000C1A08CFFF7DBFFD1ADB96B3CB05419D36A27D16317D36716D873
      24D77E34D18747BE8450D8AC83FFF6D7FFFFE6FFEFC4D99764C4773FD77D42E4
      7E3DEC7B38EB722CF07731E16E2BCB6628DF8B57CC8C63FFEDCEF1D9C1CAC0AF
      0000B39278FFFFE2FFC199A65627C26A34E3874CDB8245EFA061DA9458ECAF77
      DEA873D09C6DD09B70FFDCB3FFFEDCFFFFEAFFECD4CC9A7CB6734CD78250E680
      45EA7937EA712CE66F2AD6692BCA6C37C67A50FFF9DAF0CBB7E8D0C40000BEA0
      85FFFEDEFFC8A2B86A3FEE9A6AFFAD7AFFB783E6A775E2AE7FBE9165B3895ECC
      9C72D6A176EEAE85FFE8C7FFFCECFFFEF3F8D4C2E5B193C77E52D1763FE87E3E
      E97633EB7633D26527C66833CB7D52FFFBDEF2CAB7DBC0B60000BA9F8AFFF2D7
      F7BE9EB7704AF5A77CFFB486FBB78AE7AD83D8AA81CAA37DDBB592C19774C797
      73CB916ED1A185F6DFD0FFFBEEFFFFEDFFF0D0D69267CD7741E58343DE7532E1
      7633D26D2FC16B37BE794EFFFCE0E9C8B5D5C0B80000C6AE9CFFF3DBFFD3B7BA
      7B59F5B089FFBE97E7A882DBA783DDB291FEDABCFFF1D4D6B195BE9477BE8C6E
      B3876FBFAA9BE2D2C5FFFFECFFFFE3FFC89CD48651DC8144D2712FD16F2FD77B
      40BF703DB5774FFFFCE1D3B7A6E7D8CF0000D4C1B4EFCFBCFFF4DDAA7456F9BC
      9CFFC7A7E7B190D1A286FAD5BBFFF9E2FFF5E0FFE9D2C9A48EB78B74C9A49091
      8171C2B3A3FFF3DBFFFFE2FFFBD1D7915CCE7C42D37C3ECE763ACF7E45B26F3E
      C7926DFFFFE8B09C8BF8EFE60000ECDBD2B99F8FFFFFEBA5765BE6B196EDB99B
      EDBDA1DDB39CECC9B5FFF8E7FFFFEEFFFFEEDEBDAAC8A08DB796839C897ACFBF
      AEFFF7DFFFFFE2FFFFD7D39160C87D45DF8E55D7884FBB723EAD7044F4C6A4FF
      F7DEA49484FFFFFF0000FFFFFFBCA294FFF0DDEAC0A9C7987DE4B59AF8CAB2E7
      C0AAD1B09DFADDCEFFFFF1FFF9EAFFE1D1F2CFBBC5A592D8C1B1FFE9D7FFFDE4
      FFFFE3FFD4ACD39567CE8853DA925CD58F5AB17141BC885FFFF2D2E7CCB7C6B6
      A9FFFFFF0000FFFFFFC7B2A3E8CAB7FFFFEA9C735ADFB39BF3C9B2E0B8A5E3C2
      AFDCBBACFFE7D8FFFFF0FFFFEEFFF0D9FFFEE7FFF8E4FFFFEEFFF8E0FFFFE0DA
      A580D99C70D59363CB8857CA8A5AB2784ED6A682FFFFE7B89F8BF2E4D8FFFFFF
      0000FFFFFFFFFFFFB89F8BFFF7DEFFE3C7A37C60F4CCB3F0C9B3E4BEACD8B4A2
      E0BCAAFCD9C5FFF8E0FFFFE6FFFEE4FFFBE5FFF5E2FFDFC7ECBA9CD39B78D093
      6BD8986FDE9E75B97C54AF7A55FFFFE4FFDCC4B89F8FFFFFF7FFFFFF0000FFFF
      FFFFFFFFD7C2ADCCB197FFFFE3DDBB9EB08B71F7D0BAF2CAB7E9C1AEE2BAA7E0
      BAA2E4BDA1E5BF9FE5BE9EECC2ABEABBA5D4A28BBE8A6CCA9171E1A581D79975
      B67A56B57F5CFFF1D1FFE1C8C7A794EFD9CDFFFFFFFFFFFF0000FFFFFFFFFFFF
      FFFFFFBCA88FEED6BAFEE1C6E2C0A8B08973E7BDAAFFDFCCF9CFB8E2B79CE2B9
      99E9C29CE7BF9CD7A88DD7A38CE3AD94F5BFA1F1BA9BD59C7CB98362AE7958FF
      DFC0FFFCE0C8A793D5BCACFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF
      FFFFA39278F1D9C1FFF5DEE8C3AFC09683B38875DFB39CFFDFC2FFDEBDF2CBA4
      E9C099FBC9ADF6C0A7EFBA9FDBA68BB58163A67456D7A889FFECCFFFF8DFCFAF
      98D0B6A5FFFEF1FFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFBDAE9BE1C9B7FFFFF0FFE9DAE2B6A5B98C77B18469C19774D2AC82DDB58BD6
      A486CE997EB47F64A27054C19276FCD2B5FFF4D8FFF2D8C8AC94CEB9A4FFF5E5
      FFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      CEBBACB29587FAD4C8FFFFF0FFFCEAFFDEC3F0C7A6D7B489C29C72C19070DFAA
      8FFFCDB1FFEBCFFFFFE2FFF7DBE6C7AEAD957DDFCCB7FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFCAA59DB1877BEFC3B2FFFCE3FFFBDBFFF4CCFFFFD7FFFFE1FFFFE3FFFFE5
      FFE9CCD7B296B29479D3BBA3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFF0000F1F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD
      F7FFEFE6EFC4B5CAA089C19B7BC6A27CC7A47CCEA180BD8F70BA8E71D9B395FF
      E2C8FFFFE7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      0000}
    TabOrder = 17
    TabStop = False
    OnClick = BitBtn1Click
  end
  object CBAutoJakelu: TCheckBox
    Left = 272
    Top = 199
    Width = 177
    Height = 17
    Caption = 'Automaattinen siirto k'#228'yt'#246'ss'#228
    Enabled = False
    TabOrder = 18
  end
  object PanelPW: TPanel
    Left = 17
    Top = 241
    Width = 185
    Height = 41
    TabOrder = 2
    object LblPW: TLabel
      Left = 10
      Top = 0
      Width = 49
      Height = 13
      Caption = ' Salasana '
      Transparent = False
    end
    object EdPW: TEdit
      Left = 10
      Top = 16
      Width = 136
      Height = 21
      PasswordChar = '*'
      TabOrder = 0
      OnChange = EdPWChange
    end
  end
  object GBProtokolla: TGroupBox
    Left = 17
    Top = 18
    Width = 127
    Height = 37
    Caption = ' Tiedonsiirtoprotokolla '
    TabOrder = 19
    object rbFTP: TRadioButton
      Left = 10
      Top = 14
      Width = 49
      Height = 17
      Caption = 'FTP'
      TabOrder = 0
      OnClick = rbFTPClick
    end
    object rbSFTP: TRadioButton
      Left = 71
      Top = 14
      Width = 49
      Height = 17
      Caption = 'SFTP'
      TabOrder = 1
      OnClick = rbSFTPClick
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 192
    Top = 296
  end
  object IdFTP1: TIdFTP
    OnDisconnected = IdFTP1Disconnected
    OnConnected = IdFTP1Connected
    IPVersion = Id_IPv4
    ConnectTimeout = 0
    NATKeepAlive.UseKeepAlive = False
    NATKeepAlive.IdleTimeMS = 0
    NATKeepAlive.IntervalMS = 0
    ProxySettings.ProxyType = fpcmNone
    ProxySettings.Port = 0
    Left = 440
    Top = 8
  end
  object ImageList1: TImageList
    Left = 440
    Top = 56
    Bitmap = {
      494C010103000400800010001000FFFFFFFFFF10FFFFFFFFFFFFFFFF424D3600
      0000000000003600000028000000400000001000000001002000000000000010
      0000000000000000000000000000000000000000000000000000C2A6A400C2A6
      A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6
      A400C2A6A400C2A6A40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEFC
      FB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFC
      FB00FEFCFB00C2A6A40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000042424200424242003E3E3E003E3E3E003A3A3A003A3A
      3A00363636003636360033333300000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEFC
      FB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFCFB00FEFC
      FB00FEFCFB00C2A6A40000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000004747470075757500717171006D6D6D006D6D6D006D6D6D006A6A
      6A006A6A6A006A6A6A0035353500000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEFA
      F500FEFCFB00FEFAF500FEFAF500FEFCFB00FEFAF500FEFAF500FEFCFB00FEFA
      F500FEFAF500C2A6A400000000000000000077848F006E7C8500626F7900535F
      6800444E5600333C4300252C3200191F240010151A0010151A0010151A001015
      1A0010151A0010151A0010151A00000000000000000000000000000000000000
      0000000000004A4A4A00818181007C7C7C007474740074747400787878007878
      78007B7B7B007B7B7B0038383800000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEFA
      F500FEFAF500FEFAF500FEFAF500FEFAF500FEFAF500FEFAF500FEFAF500FEFA
      F500FEFAF500C2A6A400000000000000000079879200A2DFF00072CDEA004BB8
      E20039AEDB002FA9D90028A1D2002398C700228EBC002083AE001B81AB00177E
      A9001D799F0021749600191F2400000000000000000000000000000000000000
      0000000000004D4D4D008C8C8C00878787007D7D7D007D7D7D00848484008787
      87008A8A8A008A8A8A003B3B3B00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEF7
      F000FEF7F000FEF7F000FEF7F000FEF7F000FEF7F000FEF7F000FEF7F000FEF7
      F000FEF7F000C2A6A40000000000000000007C8A9500A9ECF60095E5FC0082DE
      FB0075D5FA006CD0F7005DC5F40052BEF00044B4EC0038ABE9002EA4E300229A
      DF001890D7001D799F0022292F00000000000000000000000000000000000000
      00000000000050505000949494008F8F8F00828282007F7F7F00464646004646
      460042424200424242003E3E3E00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEF7
      F000FEF7F000FEF7F000FEF7F000FEF3E900FEF7F000FEF7F000FEF3E900FEF7
      F000FEF7F000C2A6A40000000000000000007F8E9800B6EEF8009BE8FB0095E5
      FC0082DEFB0075D5FA006ACAF4005DC5F4004FBCF00044B4EC0038ABE9002EA4
      E3001F96DC00177EA9002F363D00000000000000000000000000000000000000
      000000000000535353009D9D9D00989898008787870084848400494949000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEF3
      E900FEF3E900FEF3E900FEF3E900FEF3E900FEF3E900FEF3E900FEF3E900FEF3
      E900FEF3E900C2A6A400000000000000000083959F00B6EEF800ABEDFB009BE8
      FB008BE2FC0082DEFB0075D5FA006ACAF4005DC5F4004FBCF00040B3ED0038AB
      E900269EE2001282B4003D454D00000000000000000000000000000000000000
      00000000000056565600A7A7A700A2A2A200929292008D8D8D004C4C4C000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFF0
      E200FFF0E200FEF3E900FFEEDE00FEF3E900FFEEDE00FEF3E900FFEEDE00FEF3
      E900FFEEDE00C2A6A400000000000000000083959F00BEF0F900ABF0F700A5EC
      FB009BE8FB008BE2FC007EDCFC0075D5FA0065CCF7005DC5F4004FBCF00040B3
      ED002EA4E3000E8ABF004B555E000000000000000000000000005F5F5F005F5F
      5F005B5B5B0058585800AFAFAF00ABABAB009B9B9B00949494004E4E4E004E4E
      4E004A4A4A004A4A4A0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FEF3
      E900FFEEDE00FFF0E200FEF3E900FFEEDE00FFF0E200DDCFC200DDCFC200DDCF
      C200DDCFC200C2A6A4000000000000000000869AA300BEF0F900B6EEF800B6EE
      F800A5ECFB0095E5FC008BE2FC007EDCFC0075D5FA0065CCF7005DC5F4004FBC
      F00037A9E7000E8FC70059656D0000000000000000000000000061616100B4B4
      B400B1B1B100B1B1B100B6B6B600B3B3B300A6A6A6009D9D9D00979797009797
      9700A4A4A4004B4B4B0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFEE
      DE00FFEEDE00FFEEDE00FFEEDE00FFEEDE00FFEEDE00C3B4A800C3B4A800C3B4
      A800C3B4A800C2A6A40000000000000000008A9DA700BEF0F900B6EEF800B6EE
      F800ABF0F700A5ECFB0095E5FC008BE2FC007EDCFC0071D3FA0065CCF70057C3
      F3004FBCF0001B99CF0065727C00000000000000000000000000000000006262
      6200BEBEBE00C1C1C100C1C1C100BCBCBC00B6B6B600B0B0B000A8A8A800A8A8
      A800505050000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFEE
      DE00FFEAD700FFEEDE00FFEAD700FFEAD700FFEEDE00B0A29600B0A29600B0A2
      9600B0A29600C2A6A40000000000000000008FA4AC00BEF0F900BEF0F900BEF0
      F900BEF0F900B4F2F800ABEDFB00A5ECFB0095E5FC008ADFFC007EDCFC0075D5
      FA006ACAF4005DC5F40065727C00000000000000000000000000000000000000
      000063636300C7C7C700C7C7C700C7C7C700C2C2C200C2C2C200BBBBBB005555
      5500000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFEA
      D700FFEAD700FFEAD700FFEAD700FFEAD700C9B9AC00FEFAF500FEF7F000E6DA
      D900C2A6A4000000000000000000000000008FA4AC008FA4AC008FA4AC008FA4
      AC008FA4AC008CA0A9008A9DA7008A9DA700869AA300869AA3008395A0008395
      A0008395A0008395A00000000000000000000000000000000000000000000000
      00000000000064646400CCCCCC00CCCCCC00CCCCCC00C8C8C8005A5A5A000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFEA
      D700FFE6D000FFEAD700FFE6D000FFEAD700C5B5A900FEFAF500DDCFC200C2A6
      A400000000000000000000000000000000008FA4AC00B2E4EC00B6EEF800B6EE
      F800A9ECF60092E1F3008FA4AC00000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000065656500D1D1D100D1D1D1005F5F5F00000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400FFE6
      D000FFE6D000FFE6D000FFE6D000FFE6D000C9B9AC00DDCFC200C2A6A4000000
      000000000000000000000000000000000000000000008FA4AC008FA4AC008FA4
      AC008FA4AC008FA4AC0000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000666666006666660000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000C2A6A400C2A6
      A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6A400C2A6A400000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      0000000000000000000000000000000000000000000000000000000000000000
      000000000000000000000000000000000000424D3E000000000000003E000000
      2800000040000000100000000100010000000000800000000000000000000000
      000000000000000000000000FFFFFF00C003FFFFFFFF0000C003FFFFFC010000
      C003FFFFF8010000C0030001F8010000C0030001F8010000C0030001F8010000
      C0030001F81F0000C0030001F81F0000C0030001C0030000C0030001C0030000
      C0030001E0070000C0030001F00F0000C0070003F81F0000C00F01FFFC3F0000
      C01F83FFFE7F0000C03FFFFFFFFF000000000000000000000000000000000000
      000000000000}
  end
  object MainMenu1: TMainMenu
    Left = 488
    Top = 304
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object allennamritykset1: TMenuItem
        Caption = '&Tallenna m'#228#228'ritykset'
        OnClick = allennamritykset1Click
      end
      object Luemritykset1: TMenuItem
        Caption = '&Lue m'#228#228'ritykset'
        OnClick = Luemritykset1Click
      end
      object Luouusiavaintiedostopari1: TMenuItem
        Caption = 'Luo uusi avaintiedostopari'
        Visible = False
        OnClick = Luouusiavaintiedostopari1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 192
    Top = 336
  end
  object ScSFTPClient: TScSFTPClient
    SSHClient = ScSSHClient
    Left = 304
    Top = 336
  end
  object ScSSHClient: TScSSHClient
    KeyStorage = ScFileStorage
    AfterConnect = ScSSHClientAfterConnect
    BeforeConnect = ScSSHClientBeforeConnect
    AfterDisconnect = ScSSHClientAfterDisconnect
    OnServerKeyValidate = ScSSHClientServerKeyValidate
    Left = 352
    Top = 336
  end
  object ScFileStorage: TScFileStorage
    Left = 400
    Top = 336
  end
end