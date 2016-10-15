object FormCfg: TFormCfg
  Left = 0
  Top = 0
  Caption = 'Konfiguraation m'#228#228'rittely'
  ClientHeight = 541
  ClientWidth = 784
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 528
    Top = 12
    Width = 90
    Height = 13
    Caption = 'Konfiguraatioteksti'
  end
  object Memo1: TMemo
    Left = 528
    Top = 32
    Width = 256
    Height = 509
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 10
    Width = 522
    Height = 530
    ActivePage = TabSheet1
    ParentShowHint = False
    ShowHint = False
    Style = tsFlatButtons
    TabOrder = 1
    object TabSheet1: TTabSheet
      Caption = 'Perustiedot'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label2: TLabel
        Left = 25
        Top = 141
        Width = 81
        Height = 13
        Caption = 'Kilpailun vaihe'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label9: TLabel
        Left = 16
        Top = 188
        Width = 211
        Height = 13
        Caption = 'Avattavien kaavakkeiden m'#228#228'rittelytiedosto'
      end
      object Label10: TLabel
        Left = 164
        Top = 95
        Width = 53
        Height = 13
        Caption = '(2 merkki'#228')'
      end
      object EdKone: TLabeledEdit
        Left = 121
        Top = 92
        Width = 33
        Height = 21
        EditLabel.Width = 115
        EditLabel.Height = 13
        EditLabel.Caption = 'Koneen tunnuskoodi'
        EditLabel.Font.Charset = DEFAULT_CHARSET
        EditLabel.Font.Color = clWindowText
        EditLabel.Font.Height = -11
        EditLabel.Font.Name = 'Tahoma'
        EditLabel.Font.Style = [fsBold]
        EditLabel.Font.Quality = fqClearTypeNatural
        EditLabel.ParentFont = False
        LabelPosition = lpLeft
        TabOrder = 0
      end
      object EdIkkOts: TLabeledEdit
        Left = 112
        Top = 115
        Width = 129
        Height = 21
        EditLabel.Width = 74
        EditLabel.Height = 13
        EditLabel.Caption = 'Ikkunan otsikko'
        LabelPosition = lpLeft
        TabOrder = 1
      end
      object CBVaihe: TComboBox
        Left = 112
        Top = 138
        Width = 129
        Height = 21
        ItemIndex = 0
        TabOrder = 2
        Text = 'Ei valittu tai ainoa'
        Items.Strings = (
          'Ei valittu tai ainoa'
          'Vaihetiedostosta'
          '1. vaihe'
          '2. vaihe'
          '3. vaihe'
          '4. vaihe'
          '5. vaihe')
      end
      object GBKunto: TGroupBox
        Left = 278
        Top = 94
        Width = 233
        Height = 345
        Caption = ' Kuntosuunnistuksen ym. valintoja '
        TabOrder = 6
        object Label3: TLabel
          Left = 23
          Top = 25
          Width = 112
          Height = 13
          Caption = 'Kuntosuunnistuksen laji'
        end
        object Label4: TLabel
          Left = 16
          Top = 112
          Width = 122
          Height = 13
          Caption = 'Henkil'#246'tietokantatiedosto'
        end
        object Label5: TLabel
          Left = 16
          Top = 212
          Width = 64
          Height = 13
          Caption = 'Seuraluettelo'
        end
        object Label6: TLabel
          Left = 15
          Top = 260
          Width = 148
          Height = 13
          Caption = 'Uusien numeroiden alueen alku'
        end
        object Label7: TLabel
          Left = 15
          Top = 282
          Width = 155
          Height = 13
          Caption = 'Uuden tiedoston vakanttien alku'
        end
        object Label8: TLabel
          Left = 15
          Top = 304
          Width = 155
          Height = 13
          Caption = 'Uuden tiedoston vakanttien lkm.'
        end
        object CBKunto: TComboBox
          Left = 144
          Top = 22
          Width = 33
          Height = 21
          ItemIndex = 0
          TabOrder = 0
          Text = 'Ei'
          OnClick = CBKuntoClick
          Items.Strings = (
            'Ei'
            '1'
            '2'
            '3')
        end
        object EdHenkTietokanta: TEdit
          Left = 16
          Top = 129
          Width = 198
          Height = 21
          Enabled = False
          TabOrder = 3
          Text = 'EdHenkTietokanta'
        end
        object RGKunto: TRadioGroup
          Left = 7
          Top = 49
          Width = 205
          Height = 39
          Caption = ' Koneen teht'#228'v'#228' '
          Columns = 3
          Enabled = False
          ItemIndex = 2
          Items.Strings = (
            'Maali'
            'L'#228'ht'#246
            'Muu')
          TabOrder = 1
        end
        object EdSeuraLuett: TEdit
          Left = 16
          Top = 229
          Width = 198
          Height = 21
          Enabled = False
          TabOrder = 6
          Text = 'Edit1'
        end
        object RGSeurahaku: TRadioGroup
          Left = 15
          Top = 171
          Width = 205
          Height = 39
          Caption = ' Seuran haku luettelosta ja n'#228'ytt'#246'   '
          Columns = 3
          ItemIndex = 1
          Items.Strings = (
            'Hae tieto'
            'Ei hakua'
            'Piilota')
          TabOrder = 5
          OnClick = RGSeurahakuClick
        end
        object EdNumAlku: TEdit
          Left = 179
          Top = 257
          Width = 38
          Height = 21
          Alignment = taRightJustify
          TabOrder = 7
          Text = '5000'
        end
        object CBVahvistaAina: TCheckBox
          Left = 16
          Top = 153
          Width = 192
          Height = 17
          Caption = 'Vahvista aina haun l'#246'yt'#228'm'#228' henkil'#246
          Checked = True
          Enabled = False
          State = cbChecked
          TabOrder = 4
        end
        object EdVakAlku: TEdit
          Left = 179
          Top = 279
          Width = 38
          Height = 21
          Alignment = taRightJustify
          TabOrder = 8
          Text = '9980'
        end
        object EdVakLkm: TEdit
          Left = 179
          Top = 301
          Width = 38
          Height = 21
          Alignment = taRightJustify
          TabOrder = 9
          Text = '20'
        end
        object CBHenkTietokanta: TCheckBox
          Left = 16
          Top = 93
          Width = 146
          Height = 17
          Caption = 'K'#228'yt'#228' henkil'#246'tietokantaa'
          TabOrder = 2
          OnClick = CBHenkTietokantaClick
        end
      end
      object CBIkkunat: TCheckBox
        Left = 16
        Top = 165
        Width = 193
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Avaa kaavakkeita k'#228'ynnistett'#228'ess'#228' '
        TabOrder = 3
        OnClick = CBIkkunatClick
      end
      object EdIkkunat: TEdit
        Left = 16
        Top = 205
        Width = 225
        Height = 21
        Enabled = False
        TabOrder = 4
        Text = 'ikkunat.xml'
      end
      object RGNaytaVal: TRadioGroup
        Left = 12
        Top = 6
        Width = 488
        Height = 61
        Caption = ' N'#228'yt'#228' konfiguraatioeditorissa '
        Columns = 2
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ItemIndex = 0
        Items.Strings = (
          'Suppeat tiedot -  suunnistus'
          'Suppeat tiedot - hiihto, ym.'
          'Kaikki vaihtehdot')
        ParentFont = False
        TabOrder = 7
        OnClick = RGNaytaValClick
      end
      object Panel2: TPanel
        Left = 12
        Top = 247
        Width = 246
        Height = 161
        ShowCaption = False
        TabOrder = 5
        TabStop = True
        Visible = False
        object Label11: TLabel
          Left = 11
          Top = 22
          Width = 86
          Height = 13
          Caption = #196#228'nimerkin k'#228'ytt'#246
        end
        object CBAani: TComboBox
          Left = 104
          Top = 19
          Width = 123
          Height = 21
          ItemIndex = 0
          TabOrder = 0
          Text = 'Ei '#228#228'nimerkkej'#228
          Items.Strings = (
            'Ei '#228#228'nimerkkej'#228
            'Vain virhetilanteet'
            'Kaikki '#228#228'nimerkit')
        end
        object EdSammSalasana: TLabeledEdit
          Left = 162
          Top = 46
          Width = 65
          Height = 21
          EditLabel.Width = 135
          EditLabel.Height = 13
          EditLabel.Caption = 'Et'#228'sammuttamisen salasana'
          LabelPosition = lpLeft
          TabOrder = 1
        end
        object EdLisaa: TLabeledEdit
          Left = 190
          Top = 73
          Width = 37
          Height = 21
          Alignment = taRightJustify
          EditLabel.Width = 188
          EditLabel.Height = 13
          EditLabel.Caption = 'Yhdess'#228' istunnossa lis'#228'tt'#228'vien maxlkm.'
          LabelPosition = lpLeft
          TabOrder = 2
          Text = '1000'
        end
        object CBJuoksuIlm: TCheckBox
          Left = 8
          Top = 110
          Width = 201
          Height = 36
          Alignment = taLeftJustify
          Caption = 'Ilmoittautumiskaavake juoksutapahtumalle sopivana'
          TabOrder = 3
          WordWrap = True
        end
      end
      object BitBtn3: TBitBtn
        Left = 479
        Top = 69
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
        TabOrder = 8
        TabStop = False
        OnClick = Help1Click
      end
    end
    object TabSheet2: TTabSheet
      Caption = 'Tiedonsiirto'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label14: TLabel
        Left = 8
        Top = 6
        Width = 242
        Height = 13
        Caption = 'Varmistustiedoston (COMFILEn.DAT) s'#228'ilytt'#228'minen'
      end
      object Label18: TLabel
        Left = 8
        Top = 119
        Width = 52
        Height = 13
        Caption = 'Yhteydet'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object SG1: TStringGrid
        Left = -2
        Top = 136
        Width = 517
        Height = 367
        Hint = 
          'Ruudun valitseminen ja hiiren oikea n'#228'pp'#228'in tarjoaa usein vaihto' +
          'ehtoja'
        ColCount = 8
        DefaultRowHeight = 20
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentShowHint = False
        ShowHint = True
        TabOrder = 2
        OnMouseDown = SG1MouseDown
        ColWidths = (
          64
          64
          64
          64
          64
          64
          64
          64)
      end
      object CBComfile: TComboBox
        Left = 270
        Top = 3
        Width = 163
        Height = 21
        ItemIndex = 0
        TabOrder = 0
        Text = 'Kysy k'#228'ynnistett'#228'ess'#228
        Items.Strings = (
          'Kysy k'#228'ynnistett'#228'ess'#228
          'S'#228'ilyt'#228' kysym'#228'tt'#228)
      end
      object Panel3: TPanel
        Left = 6
        Top = 25
        Width = 506
        Height = 94
        ShowCaption = False
        TabOrder = 1
        TabStop = True
        object Label17: TLabel
          Left = 3
          Top = 5
          Width = 225
          Height = 13
          Caption = 'Yhteyksien maksimim'#228#228'r'#228' varmistustiedostossa'
        end
        object Label19: TLabel
          Left = 318
          Top = 5
          Width = 179
          Height = 13
          Caption = '0: M'#228#228'riteltyjen yhteyksien mukainen'
        end
        object Label15: TLabel
          Left = 3
          Top = 30
          Width = 162
          Height = 13
          Caption = 'UDP-porttinumeroiden pohja-arvo'
        end
        object Label16: TLabel
          Left = 3
          Top = 55
          Width = 210
          Height = 13
          Caption = 'Saapuvien sanomien v'#228'litt'#228'minen eteenp'#228'in'
        end
        object EdMaxYhteys: TEdit
          Left = 265
          Top = 2
          Width = 42
          Height = 21
          Alignment = taRightJustify
          TabOrder = 0
          Text = '0'
        end
        object EdPortBase: TEdit
          Left = 265
          Top = 27
          Width = 42
          Height = 21
          Alignment = taRightJustify
          TabOrder = 1
          Text = '15900'
        end
        object CBValita: TComboBox
          Left = 265
          Top = 52
          Width = 163
          Height = 21
          ItemIndex = 0
          TabOrder = 2
          Text = 'Ei rajoituksia'
          Items.Strings = (
            'Ei rajoituksia'
            'Ei v'#228'litet'#228' mit'#228#228'n'
            'Valittuja yhteyksi'#228' ei v'#228'litet'#228)
        end
        object CBLahTiedostot: TCheckBox
          Left = 3
          Top = 75
          Width = 251
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Salli tiedostojen l'#228'hett'#228'minen kaikkiin yhteyksiin'
          TabOrder = 3
        end
        object CBLahKaikkiPolku: TCheckBox
          Left = 265
          Top = 75
          Width = 234
          Height = 17
          Alignment = taLeftJustify
          Caption = 'Tiedostojen l'#228'hett'#228'minen kaikista kansioista'
          TabOrder = 4
        end
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'Ajanotto'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object LbMaxjono: TLabel
        Left = 157
        Top = 67
        Width = 100
        Height = 13
        Caption = 'Ajanottojonojen lkm.'
      end
      object LbAjat: TLabel
        Left = 6
        Top = 205
        Width = 159
        Height = 13
        Caption = 'Aikojen kohteet ja/tai jonot'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LbYhtPiste: TLabel
        Left = 215
        Top = 118
        Width = 42
        Height = 26
        Caption = 'Yhteinen piste'
        WordWrap = True
      end
      object GBComAjat: TGroupBox
        Left = 312
        Top = 427
        Width = 191
        Height = 62
        Caption = ' Ajanotto sarjaporttipainikkeella '
        TabOrder = 11
        TabStop = True
        object Label20: TLabel
          Left = 38
          Top = 37
          Width = 92
          Height = 13
          Caption = 'Sarjaportin numero'
        end
        object CBComAjat: TCheckBox
          Left = 15
          Top = 16
          Width = 97
          Height = 17
          Caption = 'K'#228'yt'#246'ss'#228
          TabOrder = 0
          OnClick = CBComAjatClick
        end
        object EdComAjat: TEdit
          Left = 142
          Top = 34
          Width = 27
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
      end
      object EdMaxjono: TEdit
        Left = 261
        Top = 64
        Width = 27
        Height = 21
        Alignment = taRightJustify
        TabOrder = 2
        Text = '0'
      end
      object GroupBox4: TGroupBox
        Left = 312
        Top = 10
        Width = 191
        Height = 162
        Caption = ' Ajanottolaite - Laitenumero 1 '
        TabOrder = 8
        TabStop = True
        object Label22: TLabel
          Left = 14
          Top = 20
          Width = 23
          Height = 13
          Caption = 'Laite'
        end
        object Label23: TLabel
          Left = 14
          Top = 45
          Width = 33
          Height = 13
          Caption = 'Yhteys'
        end
        object Label24: TLabel
          Left = 14
          Top = 69
          Width = 67
          Height = 13
          Caption = 'Portin numero'
        end
        object LbLaiteIP1: TLabel
          Left = 5
          Top = 94
          Width = 83
          Height = 13
          Hint = 'LbLaite1IP'
          Caption = 'Laitteen ip-osoite'
          Visible = False
        end
        object LbLaite1Baud: TLabel
          Left = 13
          Top = 117
          Width = 24
          Height = 13
          Caption = 'Baud'
        end
        object Label57: TLabel
          Left = 14
          Top = 141
          Width = 83
          Height = 13
          Caption = 'Kellon nollausaika'
        end
        object CBLaite1: TComboBox
          Left = 56
          Top = 17
          Width = 119
          Height = 21
          TabOrder = 0
          Text = 'Ei laitetta'
          OnChange = CBLaite1Change
          Items.Strings = (
            'Ei laitetta'
            'Timy'
            'ASC l'#228'ht'#246'kello'
            'Comet'
            'Alge S4'
            'Alge S3'
            'Emit RTR1'
            'Emit RTR2'
            'Emit ECB1'
            'Emit ETS1'
            '')
        end
        object CBLaiteYht1: TComboBox
          Left = 56
          Top = 42
          Width = 119
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = 'Sarjaportti'
          OnClick = CBLaiteYht1Click
          Items.Strings = (
            'Sarjaportti'
            'TCP (laite palvelin)'
            'TCP (laite asiakas)'
            'UDP')
        end
        object EdLaitePort1: TEdit
          Left = 138
          Top = 66
          Width = 37
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 2
          Text = '0'
        end
        object EdLaiteIP1: TEdit
          Left = 92
          Top = 91
          Width = 92
          Height = 21
          Alignment = taRightJustify
          TabOrder = 3
          Text = '000.000.000.000'
          Visible = False
        end
        object EdAikaEro1: TEdit
          Left = 112
          Top = 138
          Width = 63
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 4
        end
        object CBLaiteBaud1: TComboBox
          Left = 56
          Top = 114
          Width = 119
          Height = 21
          ItemIndex = 0
          TabOrder = 5
          Text = 'Laitteen oletus'
          Items.Strings = (
            'Laitteen oletus'
            '1200'
            '2400'
            '4800'
            '9600'
            '19200'
            '38400'
            '57600'
            '115200')
        end
      end
      object GBLaite2: TGroupBox
        Left = 312
        Top = 174
        Width = 191
        Height = 169
        Caption = ' Toinen ajanottolaite  '
        TabOrder = 9
        TabStop = True
        object Label27: TLabel
          Left = 14
          Top = 47
          Width = 23
          Height = 13
          Caption = 'Laite'
        end
        object Label28: TLabel
          Left = 14
          Top = 72
          Width = 33
          Height = 13
          Caption = 'Yhteys'
        end
        object Label29: TLabel
          Left = 14
          Top = 96
          Width = 67
          Height = 13
          Caption = 'Portin numero'
        end
        object LbLaiteIP2: TLabel
          Left = 5
          Top = 121
          Width = 83
          Height = 13
          Caption = 'Laitteen ip-osoite'
          Visible = False
        end
        object Label32: TLabel
          Left = 14
          Top = 22
          Width = 59
          Height = 13
          Caption = 'Laitenumero'
        end
        object Label55: TLabel
          Left = 14
          Top = 146
          Width = 83
          Height = 13
          Caption = 'Kellon nollausaika'
        end
        object CBLaite2: TComboBox
          Left = 56
          Top = 44
          Width = 119
          Height = 21
          Enabled = False
          TabOrder = 1
          Text = 'Ei laitetta'
          OnChange = CBLaite2Change
          Items.Strings = (
            'Ei laitetta'
            'Timy'
            'ASC')
        end
        object CBLaiteYht2: TComboBox
          Left = 56
          Top = 69
          Width = 119
          Height = 21
          Enabled = False
          ItemIndex = 0
          TabOrder = 2
          Text = 'Sarjaportti'
          OnChange = CBLaiteYht2Change
          Items.Strings = (
            'Sarjaportti'
            'TCP (laite palvelin)'
            'TCP (laite asiakas)'
            'UDP')
        end
        object EdLaitePort2: TEdit
          Left = 138
          Top = 93
          Width = 37
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 3
          Text = '0'
        end
        object EdLaiteIP2: TEdit
          Left = 92
          Top = 118
          Width = 92
          Height = 21
          Alignment = taRightJustify
          TabOrder = 4
          Text = '000.000.000.000'
          Visible = False
        end
        object EdLaite2No: TEdit
          Left = 138
          Top = 19
          Width = 37
          Height = 21
          Alignment = taRightJustify
          TabOrder = 0
          Text = '0'
          OnChange = EdLaite2NoChange
        end
        object EdAikaEro2: TEdit
          Left = 112
          Top = 143
          Width = 63
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 5
        end
      end
      object RGJonot: TRadioGroup
        Left = 3
        Top = 7
        Width = 148
        Height = 82
        Caption = ' Aikajonojen k'#228'ytt'#246' '
        Items.Strings = (
          'Aikoja ei k'#228'sitell'#228
          'Yksi jono'
          'L'#228'ht'#246'ajat ja tulokset'
          'Valittu lukum'#228#228'r'#228)
        TabOrder = 0
        OnClick = RGJonotClick
      end
      object SGAjat: TStringGrid
        Left = 6
        Top = 224
        Width = 285
        Height = 206
        Hint = 
          'Merkitse sarakkeeseen '#39'Kohde'#39' kirjain M, L tai A (piste avoin) t' +
          'ai v'#228'liajan j'#228'rjestysluku'
        ColCount = 3
        DefaultRowHeight = 20
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentShowHint = False
        ShowHint = True
        TabOrder = 6
        ColWidths = (
          64
          64
          64)
      end
      object RGPiste: TRadioGroup
        Left = 3
        Top = 93
        Width = 200
        Height = 105
        Caption = ' Ajan kohdepiste '
        Items.Strings = (
          'M'#228#228'r'#228't'#228#228'n my'#246'hemmin'
          'Kaikille sama (pl. mahd. l'#228'ht'#246'portti)'
          'Alla olevan taulukon mukaan'
          'Vuorossa oleva aika'
          'K'#228'yt'#228' tiedostoa lahdepisteet(1).lst')
        TabOrder = 3
        OnClick = RGPisteClick
      end
      object CBSailAjat: TCheckBox
        Left = 173
        Top = 10
        Width = 97
        Height = 37
        Caption = 'S'#228'ilyt'#228' aiemmat ajat kysym'#228'tt'#228
        Checked = True
        State = cbChecked
        TabOrder = 1
        WordWrap = True
      end
      object GBYhtAjat: TGroupBox
        Left = 312
        Top = 347
        Width = 191
        Height = 74
        Caption = ' Ajat tiedonsiirron kautta '
        TabOrder = 10
        TabStop = True
        object Label58: TLabel
          Left = 13
          Top = 48
          Width = 83
          Height = 13
          Caption = 'Aikojen nollahetki'
        end
        object CBYhteysAjat: TCheckBox
          Left = 24
          Top = 11
          Width = 134
          Height = 34
          Caption = 'Vastaanota aikoja tiedonsiirtoyhteydest'#228
          TabOrder = 0
          WordWrap = True
          OnClick = CBYhteysAjatClick
        end
        object EdYhtAikaEro: TEdit
          Left = 112
          Top = 45
          Width = 63
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
        end
      end
      object EdYhtPiste: TEdit
        Left = 268
        Top = 121
        Width = 20
        Height = 21
        Alignment = taRightJustify
        TabOrder = 4
        Text = 'M'
      end
      object GBVuoro: TGroupBox
        Left = 208
        Top = 146
        Width = 98
        Height = 68
        Caption = ' Vuoron k'#228'ytt'#246' '
        Enabled = False
        TabOrder = 5
        object Label62: TLabel
          Left = 11
          Top = 10
          Width = 55
          Height = 26
          Caption = 'Sama piste (sek)'
          WordWrap = True
        end
        object Label68: TLabel
          Left = 5
          Top = 45
          Width = 17
          Height = 13
          Caption = 'Alin'
        end
        object Label69: TLabel
          Left = 54
          Top = 45
          Width = 16
          Height = 13
          Caption = 'Ylin'
        end
        object EdSamapiste: TEdit
          Left = 71
          Top = 14
          Width = 22
          Height = 21
          Alignment = taRightJustify
          TabOrder = 0
          Text = '0'
        end
        object EdYlinpiste: TEdit
          Left = 76
          Top = 41
          Width = 19
          Height = 21
          Alignment = taRightJustify
          TabOrder = 1
          Text = '0'
        end
        object EdAlinpiste: TEdit
          Left = 29
          Top = 41
          Width = 19
          Height = 21
          Alignment = taRightJustify
          TabOrder = 2
          Text = '0'
        end
      end
      object Panel4: TPanel
        Left = -1
        Top = 435
        Width = 306
        Height = 62
        Caption = 'Panel4'
        ShowCaption = False
        TabOrder = 7
        TabStop = True
        object CBAutoLahto: TCheckBox
          Left = 6
          Top = 3
          Width = 300
          Height = 17
          Caption = 'Liit'#228' l'#228'ht'#246'ajat automaattisesti vuorossa olevaan kilpailijaan'
          TabOrder = 0
          OnClick = CBAutoLahtoClick
        end
        object CBVainVA: TCheckBox
          Left = 6
          Top = 22
          Width = 251
          Height = 17
          Caption = 'Kaikki ajat v'#228'liaikoja (est'#228' kirjaus loppuajaksi) '
          TabOrder = 1
        end
        object CBKelloNo: TCheckBox
          Left = 6
          Top = 40
          Width = 171
          Height = 17
          Caption = 'Kello l'#228'hett'#228#228' kilpailijanumeroita'
          TabOrder = 2
        end
      end
      object BitBtn2: TBitBtn
        Left = 276
        Top = 13
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
        TabOrder = 12
        TabStop = False
        OnClick = BitBtn2Click
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'Emit-toiminnot'
      ImageIndex = 3
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label34: TLabel
        Left = 3
        Top = 344
        Width = 72
        Height = 13
        Caption = 'Emit-laitteet'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object LbLainakortit: TLabel
        Left = 198
        Top = 290
        Width = 89
        Height = 13
        Caption = 'Lainakorttitiedosto'
      end
      object SGLeimat: TStringGrid
        Left = -1
        Top = 368
        Width = 517
        Height = 135
        Hint = 
          'Ruudun valitseminen ja hiiren oikea n'#228'pp'#228'in tarjoaa usein vaihto' +
          'ehtoja'
        ColCount = 7
        DefaultRowHeight = 20
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing]
        ParentShowHint = False
        ShowHint = True
        TabOrder = 13
        OnMouseDown = SGLeimatMouseDown
      end
      object CBEmit: TCheckBox
        Left = 5
        Top = 3
        Width = 190
        Height = 17
        Caption = 'Emit-tietojen k'#228'sittely k'#228'yt'#246'ss'#228
        TabOrder = 0
      end
      object CBEsiluenta: TCheckBox
        Left = 5
        Top = 20
        Width = 300
        Height = 17
        Caption = 'Toimintona korttien esiluenta l'#228'htij'#246'iden rekister'#246'imiseksi'
        TabOrder = 1
        OnClick = CBEsiluentaClick
      end
      object CBEsiluentaKoodi: TCheckBox
        Left = 29
        Top = 40
        Width = 236
        Height = 17
        Caption = 'Kirjaa esiluennan yhteydess'#228' Emit-koodi'
        TabOrder = 2
      end
      object RGAutorap: TRadioGroup
        Left = 3
        Top = 63
        Width = 258
        Height = 90
        Caption = ' Raportin kirjoittaminen kortin lukevalla koneella '
        ItemIndex = 0
        Items.Strings = (
          'Ei kirjoiteta'
          'Tarkastusraportti jokaisesta kortista'
          'Tarkastusraportti hylk'#228'ysesityksist'#228
          'Rastiv'#228'liaikaraportti erillisen'#228' toimintona')
        TabOrder = 3
      end
      object RGComAutorap: TRadioGroup
        Left = 3
        Top = 159
        Width = 258
        Height = 74
        Caption = ' Raportin kirjoittaminen tiedonsiirron perusteella '
        ItemIndex = 0
        Items.Strings = (
          'Ei kirjoiteta'
          'Tarkastusraportti jokaisesta kortista'
          'Tarkastusraportti hylk'#228'ysesityksist'#228)
        TabOrder = 4
      end
      object RGLainat: TRadioGroup
        Left = 3
        Top = 236
        Width = 185
        Height = 93
        Caption = ' Lainakortit '
        Items.Strings = (
          'Ei huomauteta'
          'Huomauta'
          'K'#228'yt'#228' lainakorttitiedostoa'
          'K'#228'yt'#228' vain tiedoston tietoja')
        TabOrder = 5
        OnClick = RGLainatClick
      end
      object EdLainakortit: TEdit
        Left = 197
        Top = 306
        Width = 148
        Height = 21
        Enabled = False
        TabOrder = 6
      end
      object CBEmitAnal: TCheckBox
        Left = 298
        Top = 337
        Width = 177
        Height = 17
        Caption = 'Emittietojen analyysi k'#228'yt'#246'ss'#228
        TabOrder = 12
      end
      object Panel5: TPanel
        Left = 275
        Top = 250
        Width = 241
        Height = 42
        ShowCaption = False
        TabOrder = 11
        TabStop = True
        object Label60: TLabel
          Left = 70
          Top = 22
          Width = 107
          Height = 13
          Caption = 'Tarkastuksen aikajono'
        end
        object CBEmitAjat: TCheckBox
          Left = 5
          Top = 1
          Width = 234
          Height = 17
          Caption = 'Leimantarkastuksen antamat ajat  tiedostoon'
          TabOrder = 0
        end
        object EdEmitAjat: TEdit
          Left = 184
          Top = 19
          Width = 38
          Height = 21
          Alignment = taRightJustify
          TabOrder = 1
          Text = '1'
        end
      end
      object Panel6: TPanel
        Left = 275
        Top = 159
        Width = 240
        Height = 89
        Hint = 
          'T'#228'ss'#228' m'#228#228'ritell'#228#228'n Emit- ja emiTag tunnistimiin perustuva ajanot' +
          'to'
        ParentShowHint = False
        ShowCaption = False
        ShowHint = True
        TabOrder = 10
        TabStop = True
        object Label37: TLabel
          Left = 21
          Top = 3
          Width = 203
          Height = 13
          Caption = 'Kaikkien (ainoan) lukijoiden ajanottok'#228'ytt'#246
        end
        object Label38: TLabel
          Left = 49
          Top = 45
          Width = 99
          Height = 13
          Caption = 'Valittu piste tai koodi'
        end
        object Label59: TLabel
          Left = 28
          Top = 66
          Width = 120
          Height = 13
          Caption = 'Jono johon aika kirjataan'
        end
        object CBAikaLukija: TComboBox
          Left = 21
          Top = 18
          Width = 217
          Height = 21
          ItemIndex = 0
          TabOrder = 0
          Text = 'Ei k'#228'ytet'#228' ajanottoon'
          OnClick = CBAikaLukijaClick
          Items.Strings = (
            'Ei k'#228'ytet'#228' ajanottoon'
            'K'#228'ytet'#228#228'n saaden laitekoodi SJboxista'
            'K'#228'ytet'#228#228'n kiinte'#228'n pisteen ajanottoon'
            'K'#228'ytet'#228#228'n vuoron mukaiselle pisteelle'
            'K'#228'ytet'#228#228'n antaen l'#228'hdekoodi')
        end
        object EdAikaLukija: TEdit
          Left = 154
          Top = 42
          Width = 45
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object EdEmitJono: TEdit
          Left = 156
          Top = 63
          Width = 38
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 2
          Text = '1'
        end
      end
      object Panel7: TPanel
        Left = 296
        Top = 36
        Width = 219
        Height = 78
        ShowCaption = False
        TabOrder = 8
        object Label36: TLabel
          Left = 7
          Top = 58
          Width = 156
          Height = 13
          Caption = 'Alkunumero uusille Emit-koodeille'
        end
        object CBMaaliRasti: TCheckBox
          Left = 2
          Top = 1
          Width = 195
          Height = 17
          Caption = 'Viimeisen leiman aika on rastiv'#228'liaika'
          TabOrder = 0
        end
        object CBVaadiAika: TCheckBox
          Left = 2
          Top = 17
          Width = 213
          Height = 17
          Caption = 'Huomauta, kun aikaa ei voida m'#228#228'r'#228't'#228
          TabOrder = 1
        end
        object CBKaksiTunn: TCheckBox
          Left = 2
          Top = 33
          Width = 213
          Height = 17
          Caption = 'Kilpailijalla kaksi tunnistinkoodia'
          TabOrder = 2
        end
        object EdEmitAlku: TEdit
          Left = 169
          Top = 53
          Width = 44
          Height = 21
          TabOrder = 3
        end
      end
      object Panel8: TPanel
        Left = 296
        Top = 0
        Width = 219
        Height = 36
        ShowCaption = False
        TabOrder = 7
        TabStop = True
        object CBAutoSarja: TCheckBox
          Left = 2
          Top = 3
          Width = 183
          Height = 17
          Caption = 'Vaihda sarja vastaamaan leimoja'
          TabOrder = 0
        end
        object CBAutoRata: TCheckBox
          Left = 2
          Top = 20
          Width = 183
          Height = 17
          Caption = 'Vaihda rata vastaamaan leimoja'
          TabOrder = 1
        end
      end
      object Panel10: TPanel
        Left = 296
        Top = 114
        Width = 217
        Height = 45
        ShowCaption = False
        TabOrder = 9
        TabStop = True
        object Label35: TLabel
          Left = 3
          Top = 3
          Width = 169
          Height = 13
          Caption = 'Kilpailukartan tiedosto (itkumuurille)'
        end
        object EdKartta: TEdit
          Left = 3
          Top = 19
          Width = 210
          Height = 21
          TabOrder = 0
        end
      end
      object BitBtn1: TBitBtn
        Left = 468
        Top = 306
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
        TabOrder = 14
        TabStop = False
        OnClick = BitBtn1Click
      end
    end
    object TabSheet7: TTabSheet
      Caption = 'Ampumahiihto'
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox9: TGroupBox
        Left = 3
        Top = 14
        Width = 191
        Height = 147
        Caption = ' Kurvisen laite  '
        TabOrder = 0
        object Label63: TLabel
          Left = 14
          Top = 47
          Width = 23
          Height = 13
          Caption = 'Laite'
        end
        object Label64: TLabel
          Left = 14
          Top = 72
          Width = 33
          Height = 13
          Caption = 'Yhteys'
        end
        object Label65: TLabel
          Left = 14
          Top = 96
          Width = 67
          Height = 13
          Caption = 'Portin numero'
        end
        object LbSakkoLaiteIP: TLabel
          Left = 5
          Top = 121
          Width = 83
          Height = 13
          Caption = 'Laitteen ip-osoite'
          Visible = False
        end
        object Label67: TLabel
          Left = 14
          Top = 22
          Width = 59
          Height = 13
          Caption = 'Laitenumero'
        end
        object CBSakkoLaite: TComboBox
          Left = 56
          Top = 44
          Width = 119
          Height = 21
          Enabled = False
          ItemIndex = 0
          TabOrder = 1
          Text = 'Ei laitetta'
          OnChange = CBSakkoLaiteChange
          Items.Strings = (
            'Ei laitetta'
            'Versio 0 (vanhin)'
            'Versio 1'
            'Versio 2 (uusin)')
        end
        object CBSakkoLaiteYht: TComboBox
          Left = 56
          Top = 69
          Width = 119
          Height = 21
          Enabled = False
          ItemIndex = 0
          TabOrder = 2
          Text = 'Sarjaportti'
          OnChange = CBSakkoLaiteYhtChange
          Items.Strings = (
            'Sarjaportti'
            'TCP (laite palvelin)'
            'TCP (laite asiakas)'
            'UDP')
        end
        object EdSakkoLaiteIP: TEdit
          Left = 92
          Top = 118
          Width = 92
          Height = 21
          Alignment = taRightJustify
          TabOrder = 3
          Text = '000.000.000.000'
          Visible = False
        end
        object EdSakkoLaiteNo: TEdit
          Left = 138
          Top = 19
          Width = 37
          Height = 21
          Alignment = taRightJustify
          TabOrder = 0
          Text = '0'
          OnChange = EdSakkoLaiteNoChange
        end
        object EdSakkoLaitePort: TEdit
          Left = 137
          Top = 93
          Width = 38
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 4
          Text = '0'
        end
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'Tulostus'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label39: TLabel
        Left = 14
        Top = 95
        Width = 124
        Height = 13
        Caption = 'Valittavan kirjoittimen nimi'
      end
      object Label41: TLabel
        Left = 14
        Top = 143
        Width = 261
        Height = 13
        Caption = 'Muotoilu luetaan alla olevasta tiedostosta, jos annettu'
      end
      object Label53: TLabel
        Left = 308
        Top = 226
        Width = 80
        Height = 26
        Caption = 'Desimaalierotin XML-tiedostoissa'
        Visible = False
        WordWrap = True
      end
      object RGKirjoitin: TRadioGroup
        Left = 14
        Top = 8
        Width = 219
        Height = 81
        Caption = ' Kirjoittimen valinta '
        ItemIndex = 1
        Items.Strings = (
          'Kirjoitinta ei haeta k'#228'ynnistett'#228'ess'#228
          'Oletuskirjoitin otetaan k'#228'ytt'#246#246'n'
          'Valitaan nimetty kirjoitin')
        TabOrder = 0
        OnClick = RGKirjoitinClick
      end
      object EdKirjoitin: TEdit
        Left = 16
        Top = 114
        Width = 217
        Height = 21
        Enabled = False
        TabOrder = 1
      end
      object EdMuotoilu: TEdit
        Left = 16
        Top = 160
        Width = 269
        Height = 21
        TabOrder = 2
      end
      object GBAutoTul: TGroupBox
        Left = 16
        Top = 190
        Width = 269
        Height = 134
        Caption = ' Automaattinen kirjoitintulostus '
        TabOrder = 3
        TabStop = True
        object Label42: TLabel
          Left = 14
          Top = 19
          Width = 94
          Height = 13
          Caption = 'Tulostuksen laajuus'
        end
        object Label43: TLabel
          Left = 202
          Top = 18
          Width = 22
          Height = 13
          Caption = 'Lkm.'
        end
        object Label44: TLabel
          Left = 9
          Top = 46
          Width = 98
          Height = 13
          Caption = 'Laskurin kynnysarvo'
        end
        object Label45: TLabel
          Left = 148
          Top = 46
          Width = 75
          Height = 13
          Caption = 'Aika-askel (sek)'
        end
        object Label46: TLabel
          Left = 9
          Top = 70
          Width = 96
          Height = 13
          Caption = 'Tulostettavat maalit'
        end
        object Label47: TLabel
          Left = 9
          Top = 94
          Width = 89
          Height = 13
          Caption = 'Monivaiheinen kisa'
        end
        object CBAutoLaajuus: TComboBox
          Left = 114
          Top = 16
          Width = 81
          Height = 21
          ItemIndex = 0
          TabOrder = 0
          Text = 'Ei k'#228'yt'#246'ss'#228
          OnClick = CBAutoLaajuusClick
          Items.Strings = (
            'Ei k'#228'yt'#246'ss'#228
            'Lopettaneet'
            'Ilmoittautuneet'
            'Parhaat')
        end
        object EdAutoParhaat: TEdit
          Left = 227
          Top = 15
          Width = 26
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
          Text = '10'
        end
        object EdAutoKynnys: TEdit
          Left = 112
          Top = 43
          Width = 30
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 2
          Text = '10'
        end
        object EdAutoAskel: TEdit
          Left = 227
          Top = 43
          Width = 26
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 3
          Text = '60'
        end
        object EdAutoMaalit: TEdit
          Left = 112
          Top = 67
          Width = 65
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 4
          Text = '123456789'
        end
        object CBAutoMonivaihe: TComboBox
          Left = 102
          Top = 91
          Width = 156
          Height = 21
          Enabled = False
          ItemIndex = 0
          TabOrder = 5
          Text = 'Yksivaiheinen kilpailu'
          Items.Strings = (
            'Yksivaiheinen kilpailu'
            'Vaihe ja yhteistulos, jrj.yht.'
            'Vaihe ja yhteistulos, jrj.vaihe'
            'Vain vaihe'
            'Vain yhteis'
            'My'#246's 1. vaihe 2. vaiheessa')
        end
        object CBVaadiTark: TCheckBox
          Left = 10
          Top = 115
          Width = 239
          Height = 17
          Caption = 'Sarja valmis vasta, kun tarkastukset kirjattu'
          TabOrder = 6
        end
      end
      object CBNollaKarki: TCheckBox
        Left = 304
        Top = 192
        Width = 169
        Height = 31
        Caption = 'Takaa-ajon voittajan tulos on 0 (ampumahiihdon k'#228'yt'#228'nt'#246')'
        TabOrder = 7
        WordWrap = True
      end
      object CBTakAjoVA: TCheckBox
        Left = 304
        Top = 162
        Width = 169
        Height = 31
        Caption = 'Takaa-ajon v'#228'liaikoihin sis'#228'llytet'#228#228'n 1. vaiheen aika'
        TabOrder = 6
        WordWrap = True
      end
      object GBAutoTied: TGroupBox
        Left = 16
        Top = 327
        Width = 491
        Height = 172
        Caption = ' Automaattinen tiedostotulostus '
        TabOrder = 4
        TabStop = True
        object Label48: TLabel
          Left = 363
          Top = 20
          Width = 83
          Height = 13
          Caption = 'Tulostusv'#228'li (sek)'
        end
        object Label49: TLabel
          Left = 14
          Top = 49
          Width = 68
          Height = 13
          Caption = 'Tiedoston nimi'
        end
        object Label50: TLabel
          Left = 14
          Top = 89
          Width = 71
          Height = 13
          Caption = 'Html-sivun nimi'
        end
        object Label51: TLabel
          Left = 240
          Top = 89
          Width = 86
          Height = 13
          Caption = 'Html-sivun otsikko'
        end
        object Label52: TLabel
          Left = 240
          Top = 130
          Width = 126
          Height = 13
          Caption = 'Jakelun m'#228#228'rittelytiedosto'
        end
        object Label54: TLabel
          Left = 14
          Top = 130
          Width = 52
          Height = 13
          Caption = 'Jakelutapa'
        end
        object Panel1: TPanel
          Left = 10
          Top = 16
          Width = 345
          Height = 27
          TabOrder = 0
          object RBYksiHtml: TRadioButton
            Left = 53
            Top = 5
            Width = 102
            Height = 17
            Caption = 'HTML, 1 tiedosto'
            TabOrder = 0
          end
          object RBXml: TRadioButton
            Left = 299
            Top = 5
            Width = 41
            Height = 17
            Caption = 'XML'
            TabOrder = 1
          end
          object RBSarjatHtml: TRadioButton
            Left = 167
            Top = 5
            Width = 128
            Height = 17
            Caption = 'HTML, sarjat erikseen'
            TabOrder = 2
          end
          object RBEiAutotied: TRadioButton
            Left = 9
            Top = 5
            Width = 34
            Height = 17
            Caption = 'Ei'
            TabOrder = 3
          end
        end
        object EdAutoTiedVali: TEdit
          Left = 453
          Top = 17
          Width = 26
          Height = 21
          Alignment = taRightJustify
          TabOrder = 1
          Text = '60'
        end
        object EdAutoTiedosto: TEdit
          Left = 14
          Top = 64
          Width = 209
          Height = 21
          TabOrder = 2
        end
        object EdHtmlTitle: TEdit
          Left = 14
          Top = 104
          Width = 210
          Height = 21
          TabOrder = 3
        end
        object EdHtmlOts: TEdit
          Left = 240
          Top = 104
          Width = 217
          Height = 21
          TabOrder = 4
        end
        object EdAutoJakelu: TEdit
          Left = 240
          Top = 145
          Width = 217
          Height = 21
          TabOrder = 6
        end
        object CBAutoJakelu: TComboBox
          Left = 14
          Top = 145
          Width = 161
          Height = 21
          ItemIndex = 0
          TabOrder = 5
          Text = 'Ei jakelua'
          OnClick = CBAutoJakeluClick
          Items.Strings = (
            'Ei jakelua'
            'Jakelu suoraan ohjelmasta'
            'Ulkoinen komentotiedosto')
        end
        object CBAutoEtuSuku: TCheckBox
          Left = 240
          Top = 73
          Width = 197
          Height = 17
          Caption = 'Etunimi ensin html-tulosteessa'
          TabOrder = 7
          Visible = False
        end
        object CBAfMolemmat: TCheckBox
          Left = 240
          Top = 58
          Width = 197
          Height = 17
          Caption = 'Vaihe- ja yhteistulos monivaiheisessa'
          TabOrder = 8
        end
      end
      object CBDesErotin: TComboBox
        Left = 402
        Top = 229
        Width = 68
        Height = 21
        ItemIndex = 0
        TabOrder = 8
        Text = 'Pilkku'
        Visible = False
        Items.Strings = (
          'Pilkku'
          'Piste')
      end
      object Panel9: TPanel
        Left = 290
        Top = 6
        Width = 225
        Height = 133
        ShowCaption = False
        TabOrder = 5
        TabStop = True
        object Label40: TLabel
          Left = 5
          Top = 89
          Width = 147
          Height = 13
          Caption = 'Lokin kohdetiedosto tai kirjoitin'
        end
        object RGLoki: TRadioGroup
          Left = 5
          Top = 2
          Width = 216
          Height = 81
          Caption = ' Lokin kirjoittaminen '
          ItemIndex = 0
          Items.Strings = (
            'Lokia ei kirjoiteta'
            'Loki oletustiedostoon'
            'Loki nimettyyn kohteeseen')
          TabOrder = 0
          OnClick = RGLokiClick
        end
        object EdLoki: TEdit
          Left = 5
          Top = 108
          Width = 216
          Height = 21
          Enabled = False
          TabOrder = 1
        end
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'HkMaali'
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label12: TLabel
        Left = 15
        Top = 46
        Width = 123
        Height = 13
        Caption = 'K'#228'ytett'#228'vien v'#228'rien koodi'
      end
      object Label13: TLabel
        Left = 16
        Top = 71
        Width = 167
        Height = 13
        Caption = 'K'#228'ynnistys ohittaen aloitusvalinnat'
      end
      object CB25rivi: TCheckBox
        Left = 15
        Top = 6
        Width = 152
        Height = 17
        Alignment = taLeftJustify
        Caption = '25-rivinen konsoli-ikkuna'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
      end
      object CBKirkas: TCheckBox
        Left = 15
        Top = 23
        Width = 152
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Kirkas intensiteetti'
        TabOrder = 1
      end
      object CBUudenTulos: TCheckBox
        Left = 15
        Top = 92
        Width = 290
        Height = 17
        Alignment = taLeftJustify
        Caption = 'Salli tuloksen antaminen uutta kilpailijaa lis'#228'tt'#228'ess'#228
        TabOrder = 4
      end
      object CBBoot: TComboBox
        Left = 208
        Top = 68
        Width = 97
        Height = 21
        ItemIndex = 0
        TabOrder = 3
        Text = 'Ei k'#228'yt'#246'ss'#228
        Items.Strings = (
          'Ei k'#228'yt'#246'ss'#228
          'P'#228#228'valikkoon'
          'Ajanottoon 1'
          'Ajanottoon 2'
          'Ajanottoon 3'
          'Ajanottoon 4')
      end
      object EdVarit: TEdit
        Left = 153
        Top = 43
        Width = 121
        Height = 21
        TabOrder = 2
      end
      object GroupBox2: TGroupBox
        Left = 16
        Top = 146
        Width = 281
        Height = 89
        Caption = ' Ajanoton lis'#228'parametrit ohjelmassa HkMaali '
        TabOrder = 5
        object Label31: TLabel
          Left = 16
          Top = 27
          Width = 116
          Height = 13
          Caption = 'Ajaotton'#228'pp'#228'imen koodi'
        end
        object CBEnnakoi: TCheckBox
          Left = 16
          Top = 61
          Width = 233
          Height = 17
          Caption = 'Salli numeroiden sy'#246'tt'#246' ennen ajan saamista'
          TabOrder = 0
        end
        object EdNappain: TEdit
          Left = 152
          Top = 24
          Width = 76
          Height = 21
          TabOrder = 1
          Text = '0,0'
        end
      end
    end
  end
  object Button1: TButton
    Left = 698
    Top = 3
    Width = 81
    Height = 25
    Caption = 'P'#228'ivit'#228' sis'#228'lt'#246
    TabOrder = 2
    OnClick = Pivitvalinnatikkunaan1Click
  end
  object Button2: TButton
    Left = 632
    Top = 4
    Width = 64
    Height = 25
    Caption = 'Tulkitse'
    Enabled = False
    TabOrder = 3
    Visible = False
    OnClick = Button2Click
  end
  object MainMenu1: TMainMenu
    Left = 676
    Top = 56
    object iedosto1: TMenuItem
      Caption = 'Tiedosto'
      object Luetiedosto1: TMenuItem
        Caption = 'Lue tiedosto'
        OnClick = Luetiedosto1Click
      end
      object Kirjoitatiedosto1: TMenuItem
        Caption = 'Kirjoita tiedosto'
        OnClick = Kirjoitatiedosto1Click
      end
      object Suljekaavake1: TMenuItem
        Caption = 'Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object oiminnot1: TMenuItem
      Caption = 'Toiminnot'
      object ulkitseikkunansislt1: TMenuItem
        Caption = 'Tulkitse ikkunan sis'#228'lt'#246
        OnClick = Button2Click
      end
      object Pivitvalinnatikkunaan1: TMenuItem
        Caption = 'P'#228'ivit'#228' valinnat ikkunaan'
        OnClick = Pivitvalinnatikkunaan1Click
      end
      object Poistatulkitutrivit1: TMenuItem
        Caption = 'Poista tulkitut rivit'
        OnClick = Poistatulkitutrivit1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      OnClick = Help1Click
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 552
    Top = 56
  end
  object SaveDialog1: TSaveDialog
    Left = 600
    Top = 56
  end
  object PopupMenuYht: TPopupMenu
    Left = 564
    Top = 199
    object UDP1: TMenuItem
      Caption = 'UDP-yhteys'
      OnClick = UDP1Click
    end
    object TCP1: TMenuItem
      Caption = 'TCP-asiakas'
      OnClick = UDP1Click
    end
    object TCPS1: TMenuItem
      Caption = 'TCP-palvelin'
      OnClick = UDP1Click
    end
    object BRO1: TMenuItem
      Caption = 'Broadcast'
      OnClick = UDP1Click
    end
    object COM1: TMenuItem
      Caption = 'Sarjaportti'
      OnClick = UDP1Click
    end
  end
  object PopupMenuSan: TPopupMenu
    Left = 700
    Top = 199
    object Perus1: TMenuItem
      Caption = 'Perussanomat (tyhjenn'#228')'
      OnClick = SanValClick
    end
    object Ajat1: TMenuItem
      Caption = 'Ajanottotietueet'
      OnClick = SanValClick
    end
    object Leimat1: TMenuItem
      Caption = 'Leimantarkastustietueet'
      OnClick = SanValClick
    end
    object Vainajat1: TMenuItem
      Caption = 'Vain ajanotto'
      OnClick = SanValClick
    end
    object Vainleimat1: TMenuItem
      Caption = 'Vain leimat'
      OnClick = SanValClick
    end
    object LeimatIn1: TMenuItem
      Caption = 'Leimat vastaanotto'
      OnClick = SanValClick
    end
    object LeimatOut1: TMenuItem
      Caption = 'Leimat l'#228'hetys'
      OnClick = SanValClick
    end
    object Rastiva1: TMenuItem
      Caption = 'Rastiv'#228'liajat'
      OnClick = SanValClick
    end
    object ECode1: TMenuItem
      Caption = 'V'#228'lit'#228' aikasanoma (ECode)'
      OnClick = SanValClick
    end
    object XMLtietueet1: TMenuItem
      Caption = '&XML-tietueet'
      OnClick = SanValClick
    end
    object Tied1: TMenuItem
      Caption = 'Tiedostoja'
    end
    object Loki1: TMenuItem
      Caption = 'Loki'
      OnClick = SanValClick
    end
  end
  object PopupMenuVal: TPopupMenu
    Left = 628
    Top = 199
    object Valita1: TMenuItem
      Caption = 'V'#228'lit'#228' sanomat'
      OnClick = Valita1Click
    end
    object ValitaEi1: TMenuItem
      Caption = #196'l'#228' v'#228'lit'#228' saapuvia sanomia'
      OnClick = Valita1Click
    end
  end
  object PopupMenuIO: TPopupMenu
    Left = 628
    Top = 239
    object InOut1: TMenuItem
      Caption = 'Sek'#228' vastaanotto ett'#228' l'#228'hetys'
      OnClick = InOut1Click
    end
    object In1: TMenuItem
      Caption = 'Vain vastaanotto (In)'
      OnClick = InOut1Click
    end
    object Out1: TMenuItem
      Caption = 'Vain l'#228'hetys (Out)'
      OnClick = InOut1Click
    end
  end
  object PopupMenuMalli: TPopupMenu
    Left = 564
    Top = 247
    object MalliUDPvast: TMenuItem
      Caption = 'Lis'#228#228' t'#228'h'#228'n mallirivi UDP yhteyden vastaanotto'
      OnClick = MalliClick
    end
    object MalliUDPavaaja: TMenuItem
      Caption = 'Mallirivi UDP yhteyden avaaja'
    end
    object MalliEmit: TMenuItem
      Caption = 'Mallirivi UDP yhteyden avaaja, my'#246's Emit-tietueet'
    end
    object MalliUDPvainIn: TMenuItem
      Caption = 'Mallirivi. Vain sanomien vastaanaanotto'
    end
    object MalliEmitVA: TMenuItem
      Caption = 'Mallirivi: Emitv'#228'liajat onlineen'
    end
  end
  object PopupMenuLaite: TPopupMenu
    Left = 576
    Top = 312
    object Lukija1: TMenuItem
      Caption = 'Lukijaleimasin'
      OnClick = LaiteClick
    end
    object emiTag1: TMenuItem
      Caption = 'emiTag-laite (my'#246's MTR5)'
      OnClick = LaiteClick
    end
    object LaiteMTR1: TMenuItem
      Caption = 'MTR (ei MTR5)'
      OnClick = LaiteClick
    end
  end
  object PopupMenuLaiteYht: TPopupMenu
    Left = 672
    Top = 312
    object LaiteSarjaportti: TMenuItem
      Caption = 'Sarjaportti'
      OnClick = LaiteYhtClick
    end
    object LaiteNopeaUSB: TMenuItem
      Caption = 'Sarjaportti - nopea USB'
      OnClick = LaiteYhtClick
    end
    object LaiteTCPasiakas: TMenuItem
      Caption = 'TCP-asiakas'
      OnClick = LaiteYhtClick
    end
    object LaiteTCPpalvelin: TMenuItem
      Caption = 'TCP-palvelin'
      OnClick = LaiteYhtClick
    end
    object LaiteUDP: TMenuItem
      Caption = 'UDP'
      OnClick = LaiteYhtClick
    end
  end
  object PopupMenuLaiteAjat: TPopupMenu
    Left = 576
    Top = 376
    object Eiaikoja1: TMenuItem
      Caption = 'Ei aikoja'
      OnClick = LaiteAjatClick
    end
    object Maali1: TMenuItem
      Caption = 'Maali'
      OnClick = LaiteAjatClick
    end
    object Valiaika1: TMenuItem
      Caption = 'V'#228'liaika 1'
      OnClick = LaiteAjatClick
    end
    object Valiaika2: TMenuItem
      Caption = 'V'#228'liaika 2'
      OnClick = LaiteAjatClick
    end
    object SjBox1: TMenuItem
      Caption = 'SJBoxin koodista'
      OnClick = LaiteAjatClick
    end
    object LaiteVuorot: TMenuItem
      Caption = 'Vuoron mukaan'
      OnClick = LaiteAjatClick
    end
    object Lahdekoodi1: TMenuItem
      Caption = 'L'#228'hdekoodi xx'
      OnClick = LaiteAjatClick
    end
  end
end
