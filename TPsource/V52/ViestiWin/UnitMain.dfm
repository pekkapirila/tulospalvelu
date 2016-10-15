object FormMain: TFormMain
  Left = 291
  Top = 376
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'ViestiWin'
  ClientHeight = 455
  ClientWidth = 458
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 24
    Top = 61
    Width = 82
    Height = 16
    Caption = 'Avatut ikkunat'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 8
    Width = 96
    Height = 14
    Caption = 'Avatun kilpailun nimi'
  end
  object Label3: TLabel
    Left = 331
    Top = 425
    Width = 112
    Height = 14
    Caption = 'Pekka Piril'#228', 2011, 2015'
  end
  object LbVaihe: TLabel
    Left = 256
    Top = 56
    Width = 127
    Height = 16
    Caption = 'Vaihe: Esivalmistelu'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object Label4: TLabel
    Left = 8
    Top = 348
    Width = 49
    Height = 16
    Caption = 'Tietueita'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 116
    Top = 348
    Width = 48
    Height = 16
    Caption = 'Tuloksia'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 8
    Top = 391
    Width = 57
    Height = 16
    Caption = 'M'#228#228'ritelty'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 131
    Top = 391
    Width = 36
    Height = 16
    Caption = 'Avattu'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 8
    Top = 370
    Width = 60
    Height = 16
    Caption = 'Yhteyksi'#228
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LblLahet: TLabel
    Left = 8
    Top = 418
    Width = 122
    Height = 15
    Caption = 'L'#228'hetett'#228'vi'#228' sanomia'
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object BtnTulPalv: TButton
    Left = 253
    Top = 255
    Width = 194
    Height = 88
    Caption = 'Avaa kilpailu tulospalveluun'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    WordWrap = True
    OnClick = BtnTulPalvClick
  end
  object ListBox1: TListBox
    Left = 8
    Top = 80
    Width = 225
    Height = 257
    Hint = 'Klikkaa rivi'#228' n'#228'ytt'#228#228'ksesi ko. ikkuna'
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    OnClick = ListBox1Click
  end
  object Button2: TButton
    Left = 253
    Top = 80
    Width = 194
    Height = 87
    Caption = 'Kilpailun luominen ja perusominaisuudet'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    WordWrap = True
    OnClick = Button2Click
  end
  object BtnIlmTila: TButton
    Left = 253
    Top = 167
    Width = 194
    Height = 88
    Caption = 'Avaa kilpailu esivalmisteluun'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    WordWrap = True
    OnClick = BtnIlmTilaClick
  end
  object EditKilpailu: TEdit
    Left = 8
    Top = 28
    Width = 401
    Height = 22
    TabStop = False
    ReadOnly = True
    TabOrder = 4
    Text = 'Ei avattua kilpailua'
  end
  object EdtTietueita: TEdit
    Left = 71
    Top = 345
    Width = 41
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 5
    Text = '0'
  end
  object EdtTuloksia: TEdit
    Left = 172
    Top = 345
    Width = 41
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    PopupMenu = PopupMenu1
    ReadOnly = True
    TabOrder = 6
    Text = '0'
    TextHint = 'Klikkaa oikealla n'#228'pp'#228'imell'#228' vaihtaaksesi sis'#228'lt'#246
  end
  object EdtYhtLkm: TEdit
    Left = 71
    Top = 388
    Width = 41
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 7
    Text = '0'
  end
  object EdtYhtAvattu: TEdit
    Left = 172
    Top = 388
    Width = 41
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = '0'
  end
  object Memo1: TMemo
    Left = 225
    Top = 348
    Width = 222
    Height = 70
    Hint = 'Klikkaa aluetta avataksesi erillinen sanomaikkuna'
    TabStop = False
    ParentShowHint = False
    ReadOnly = True
    ScrollBars = ssBoth
    ShowHint = True
    TabOrder = 9
    OnDblClick = Memo1DblClick
  end
  object EdtLahet: TEdit
    Left = 151
    Top = 416
    Width = 62
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 10
    Text = '0'
  end
  object BitBtn1: TBitBtn
    Left = 416
    Top = 2
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
    TabOrder = 11
    OnClick = BitBtn1Click
  end
  object MainMenu1: TMainMenu
    Left = 20
    object Toiminnot1: TMenuItem
      Caption = '&Tiedostot'
      object Avausmritykset1: TMenuItem
        Caption = 'Avaus&m'#228#228'ritykset'
        OnClick = Avausmritykset1Click
      end
      object Tallennaikkunat1: TMenuItem
        Caption = '&Tallenna ikkunat'
        OnClick = Tallennaikkunat1Click
      end
      object Haeikkunat1: TMenuItem
        Caption = '&Hae ikkunat'
        OnClick = Haeikkunat1Click
      end
      object Kirjoitasiirtotiedostoon1: TMenuItem
        Caption = '&Kirjoita siirtotiedostoon'
        OnClick = Kirjoitasiirtotiedostoon1Click
      end
      object MySQL1: TMenuItem
        Caption = 'MySQL'
        OnClick = MySQL1Click
      end
      object Vertailuajanmrittely1: TMenuItem
        Caption = '&Asetukset'
        OnClick = Vertailuajanmrittely1Click
      end
      object iedonsiirtoasetukset1: TMenuItem
        Caption = 'T&iedonsiirtoasetukset'
        object Asetuksetjauusinta1: TMenuItem
          Caption = '&Asetukset ja uusinta'
          OnClick = Asetuksetjauusinta1Click
        end
        object iedonsiirtotiedostontarkastelu1: TMenuItem
          Caption = '&Tiedonsiirtotiedoston tarkastelu'
          OnClick = iedonsiirtotiedostontarkastelu1Click
        end
        object Lhettiedosto1: TMenuItem
          Caption = '&L'#228'het'#228' tiedosto'
          OnClick = Lhettiedosto1Click
        end
      end
      object Pidkaavakepllimmisen1: TMenuItem
        Caption = '&Pid'#228' kaavake p'#228#228'llimm'#228'isen'#228
        OnClick = Pidkaavakepllimmisen1Click
      end
      object Sanomaikkuna1: TMenuItem
        Caption = 'Sanomaikkuna'
        OnClick = Memo1DblClick
      end
      object Lopeta1: TMenuItem
        Caption = '&Lopeta'
        OnClick = Lopeta1Click
      end
    end
    object Valmistelu1: TMenuItem
      Caption = '&Valmistelu'
      object Sarjatiedot1: TMenuItem
        Caption = '&Sarjatiedot'
        OnClick = Sarjatiedot1Click
      end
      object Numerointi1: TMenuItem
        Caption = '&Numerointi ja l'#228'ht'#246'ajat'
        Enabled = False
        OnClick = Numerointi1Click
      end
      object Nollaatietoja1: TMenuItem
        Caption = 'N&ollaa tietoja'
        Enabled = False
        OnClick = Nollaatietoja1Click
      end
      object Ratatiedot1: TMenuItem
        Caption = '&Ratatiedot'
        OnClick = Ratatiedot1Click
      end
    end
    object Osanottajat1: TMenuItem
      Caption = '&Osanottajat'
      Enabled = False
      object Joukkueet1: TMenuItem
        Caption = '&Joukkueet'
        OnClick = Joukkueet1Click
      end
      object Yksijoukkue1: TMenuItem
        Caption = '&Yksi joukkue'
        OnClick = Yksijoukkue1Click
      end
      object Haetiedostosta1: TMenuItem
        Caption = '&Hae tiedostosta'
        OnClick = Haetiedostosta1Click
      end
      object Merkitsetarkastuskoodeja1: TMenuItem
        Caption = '&Tarkastus- ja seurantakoodit'
        OnClick = Merkitsetarkastuskoodeja1Click
      end
      object Siirrlistietoja1: TMenuItem
        Caption = '&Siirr'#228' lis'#228'tietoja'
        OnClick = Siirrlistietoja1Click
      end
      object Kopioiosuustietoja1: TMenuItem
        Caption = '&Kopioi osuustietoja'
        OnClick = Kopioiosuustietoja1Click
      end
    end
    object Seuranta1: TMenuItem
      Caption = '&Seuranta'
      Enabled = False
      object Tilannepisteess1: TMenuItem
        Caption = '&Tilanne pisteess'#228
        Enabled = False
        object Uusi5: TMenuItem
          Caption = '&Uusi'
          OnClick = Tilannepisteess1Click
        end
        object N1ikkuna4: TMenuItem
          Caption = '&1. ikkuna'
          OnClick = N1ikkuna4Click
        end
        object N2ikkuna4: TMenuItem
          Caption = '&2. ikkuna'
          OnClick = N2ikkuna4Click
        end
        object N3ikkuna3: TMenuItem
          Caption = '&3. ikkuna'
          OnClick = N3ikkuna3Click
        end
        object N4ikkuna4: TMenuItem
          Caption = '&4. ikkuna'
          OnClick = N4ikkuna4Click
        end
        object N5ikkuna4: TMenuItem
          Caption = '&5. ikkuna'
          OnClick = N5ikkuna4Click
        end
        object N6ikkuna4: TMenuItem
          Caption = '&6. ikkuna'
          OnClick = N6ikkuna4Click
        end
      end
      object Viimeisettapahtumat1: TMenuItem
        Caption = '&Viimeiset tapahtumat'
        Enabled = False
        object Uusi6: TMenuItem
          Caption = '&Uusi'
          OnClick = ViimTapahtumat1Click
        end
        object N11: TMenuItem
          Caption = '&1. ikkuna'
          OnClick = N11Click
        end
        object N2ikkuna6: TMenuItem
          Caption = '&2. ikkuna'
          OnClick = N2ikkuna6Click
        end
        object N3ikkuna5: TMenuItem
          Caption = '&3. ikkuna'
          OnClick = N3ikkuna5Click
        end
        object N4ikkuna6: TMenuItem
          Caption = '&4. ikkuna'
          OnClick = N4ikkuna6Click
        end
      end
      object Joukkueentiedot1: TMenuItem
        Caption = '&Joukkueen tiedot'
        object Uusi1: TMenuItem
          Caption = '&Uusi ikkuna'
          OnClick = Joukkueentiedot1Click
        end
        object N1ikkuna1: TMenuItem
          Caption = '&1. ikkuna'
          OnClick = N1ikkuna1Click
        end
        object N2ikkuna1: TMenuItem
          Caption = '&2. ikkuna'
          OnClick = N2ikkuna1Click
        end
        object N3ikkna1: TMenuItem
          Caption = '&3. ikkuna'
          OnClick = N3ikkna1Click
        end
        object N4ikkuna1: TMenuItem
          Caption = '&4. ikkuna'
          OnClick = N4ikkuna1Click
        end
        object N5ikkuna1: TMenuItem
          Caption = '&5. ikkuna'
          OnClick = N5ikkuna1Click
        end
        object N6ikkuna1: TMenuItem
          Caption = '&6. ikkuna'
          OnClick = N6ikkuna1Click
        end
      end
      object Osuudentiedot1: TMenuItem
        Caption = '&Osuuden tiedot'
        object Uusi4: TMenuItem
          Caption = '&Uusi'
          OnClick = Osuudentiedot1Click
        end
        object N1ikkuna5: TMenuItem
          Caption = '&1. ikkuna'
          OnClick = N1ikkuna5Click
        end
        object N2ikkuna5: TMenuItem
          Caption = '&2. ikkuna'
          OnClick = N2ikkuna5Click
        end
        object N3ikkuna4: TMenuItem
          Caption = '&3. ikkuna'
          OnClick = N3ikkuna4Click
        end
        object N4ikkuna5: TMenuItem
          Caption = '&4. ikkuna'
          OnClick = N4ikkuna5Click
        end
        object N5ikkuna5: TMenuItem
          Caption = '&5. ikkuna'
          OnClick = N5ikkuna5Click
        end
        object N6ikkuna5: TMenuItem
          Caption = '&6. ikkuna'
          OnClick = N6ikkuna5Click
        end
      end
      object Kilpailuneteneminen1: TMenuItem
        Caption = '&Kilpailun eteneminen'
        object Uusi3: TMenuItem
          Caption = '&Uusi'
          OnClick = Kilpailuneteneminen1Click
        end
        object N1ikkuna3: TMenuItem
          Caption = '&1. ikkuna'
          OnClick = N1ikkuna3Click
        end
        object N2ikkuna3: TMenuItem
          Caption = '&2. ikkuna'
          OnClick = N2ikkuna3Click
        end
        object N3ikkuna2: TMenuItem
          Caption = '&3. ikkuna'
          OnClick = N3ikkuna2Click
        end
        object N4ikkuna3: TMenuItem
          Caption = '&4. ikkuna'
          OnClick = N4ikkuna3Click
        end
        object N5ikkuna3: TMenuItem
          Caption = '&5. ikkuna'
          OnClick = N5ikkuna3Click
        end
        object N6ikkuna3: TMenuItem
          Caption = '&6. ikkuna'
          OnClick = N6ikkuna3Click
        end
      end
      object Status1: TMenuItem
        Caption = '&Status'
        OnClick = Status1Click
      end
      object analyysit1: TMenuItem
        Caption = '&Analyysit'
        Enabled = False
        object Emitanalyysit2: TMenuItem
          Caption = '&Emitanalyysit'
          object Uusi7: TMenuItem
            Caption = '&Uusi'
            OnClick = Emitanalyysit1Click
          end
          object N1ikkuna6: TMenuItem
            Caption = '&1. ikkuna'
            OnClick = N1ikkuna2Click
          end
          object N2ikkuna7: TMenuItem
            Caption = '&2. ikkuna'
            OnClick = N2ikkuna2Click
          end
          object N3ikkuna6: TMenuItem
            Caption = '&3. ikkuna'
            OnClick = N3ikkuna1Click
          end
          object N4ikkuna7: TMenuItem
            Caption = '&4. ikkuna'
            OnClick = N4ikkuna2Click
          end
          object N5ikkuna6: TMenuItem
            Caption = '&5. ikkuna'
            OnClick = N5ikkuna2Click
          end
          object N6ikkuna6: TMenuItem
            Caption = '&6. ikkuna'
            OnClick = N6ikkuna2Click
          end
        end
        object Yhteenveto2: TMenuItem
          Caption = '&Yhteenveto'
          OnClick = Yhteenveto2Click
        end
      end
      object GAZtaulujenohjaus1: TMenuItem
        Caption = '&Osuusn'#228'ytt'#246'taulujen ohjaus'
        Visible = False
        OnClick = N1GAZ1Click
      end
    end
    object Tulospalvelu1: TMenuItem
      Caption = 'Tulos&palvelu'
      Enabled = False
      object Ajanotto1: TMenuItem
        Caption = '&Ajanotto'
        object Ajanotto11: TMenuItem
          Caption = 'Ajanotto - &1'
          OnClick = Ajanotto11Click
        end
        object Ajanotto21: TMenuItem
          Caption = 'Ajanotto - &2'
          OnClick = Ajanotto21Click
        end
        object Ajanotto31: TMenuItem
          Caption = 'Ajanotto - &3'
          OnClick = Ajanotto31Click
        end
        object Ajanotto41: TMenuItem
          Caption = 'Ajanotto - &4'
          OnClick = Ajanotto41Click
        end
      end
      object Emitluenta1: TMenuItem
        Caption = '&Emit-luenta'
        OnClick = Emitluenta1Click
      end
      object SisLuenta: TMenuItem
        Caption = 'Sis'#228#228'n&luenta (emit)'
        OnClick = SisLuentaClick
      end
      object Vliaikatulosteet1: TMenuItem
        Caption = '&V'#228'liaikatulosteet'
        OnClick = Vliaikatulosteet1Click
      end
      object Ammunnansakot1: TMenuItem
        Caption = 'Ammunnan &sakot'
        Enabled = False
        Visible = False
        OnClick = Ammunnansakot1Click
      end
      object Sakkoseuranta1: TMenuItem
        Caption = 'Sakk&oseuranta'
        Enabled = False
        Visible = False
        OnClick = Sakkoseuranta1Click
      end
      object Yhteislhdt1: TMenuItem
        Caption = '&Yhteisl'#228'hd'#246't'
        OnClick = Yhteislhdt1Click
      end
      object Lhestyjienseuranta1: TMenuItem
        Caption = 'L'#228'hestyjien seuranta'
        object Lhestyjt11: TMenuItem
          Caption = 'L'#228'hestyj'#228't 1'
          OnClick = LhestyjienseurantaClick
        end
        object Lhestyjt21: TMenuItem
          Caption = 'L'#228'hestyj'#228't 2'
          OnClick = LhestyjienseurantaClick
        end
        object Lhestyjt31: TMenuItem
          Caption = 'L'#228'hestyj'#228't 3'
          OnClick = LhestyjienseurantaClick
        end
        object Lhestyjt41: TMenuItem
          Caption = 'L'#228'hestyj'#228't 4'
          OnClick = LhestyjienseurantaClick
        end
      end
    end
    object tulosteet1: TMenuItem
      Caption = 'T&ulosteet'
      object Laadituloste1: TMenuItem
        Caption = '&Laadi tuloste'
        OnClick = Laadituloste1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object Help2: TMenuItem
        Caption = 'Help'
        OnClick = Help2Click
      end
      object iedot1: TMenuItem
        Caption = 'Tiedot'
        OnClick = iedot1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 80
    Top = 24
  end
  object SaveDialog1: TSaveDialog
    Left = 120
    Top = 24
  end
  object OpenDialog2: TOpenDialog
    Left = 152
    Top = 16
  end
  object PopupMenu1: TPopupMenu
    Left = 240
    Top = 304
    object Nyttulokset1: TMenuItem
      Caption = 'N'#228'yt'#228' &tulokset'
      Checked = True
      OnClick = Nyttulokset1Click
    end
    object Nytavoimet1: TMenuItem
      Caption = 'N'#228'yt'#228' &avoimet'
      OnClick = Nytavoimet1Click
    end
  end
end
