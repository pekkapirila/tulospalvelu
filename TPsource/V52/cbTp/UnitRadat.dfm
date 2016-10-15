object FormRadat: TFormRadat
  Left = 0
  Top = 0
  Caption = 'Ratatiedot'
  ClientHeight = 589
  ClientWidth = 661
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object PageControl1: TPageControl
    Left = 7
    Top = 9
    Width = 561
    Height = 574
    ActivePage = TabSheetRadat
    TabOrder = 0
    OnChange = PageControl1Change
    object TabSheetRadat: TTabSheet
      Caption = 'Radat'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label1: TLabel
        Left = 9
        Top = 3
        Width = 65
        Height = 14
        Caption = 'Radan valinta'
      end
      object Label2: TLabel
        Left = 9
        Top = 70
        Width = 86
        Height = 28
        Caption = 'Radan (uusi) nimi tai lis'#228'tt'#228'v'#228' rata'
        WordWrap = True
      end
      object Label6: TLabel
        Left = 244
        Top = 4
        Width = 217
        Height = 14
        Caption = 'Rastikoodit ja matkat (my'#246's maaliviivan koodi)'
      end
      object Label3: TLabel
        Left = 8
        Top = 48
        Width = 203
        Height = 14
        Caption = 'Radan lis'#228'ys aloitetaan antamalla uusi nimi'
      end
      object Lähtö: TLabel
        Left = 480
        Top = 314
        Width = 27
        Height = 14
        Caption = 'L'#228'ht'#246
      end
      object Label28: TLabel
        Left = 480
        Top = 362
        Width = 24
        Height = 14
        Caption = 'Maali'
      end
      object Label32: TLabel
        Left = 481
        Top = 407
        Width = 51
        Height = 14
        Caption = 'Ratapituus'
      end
      object Label33: TLabel
        Left = 482
        Top = 455
        Width = 31
        Height = 14
        Caption = 'Nousu'
      end
      object LblVaroitus: TLabel
        Left = 8
        Top = 400
        Width = 173
        Height = 15
        Caption = 'Varoitus: Radan nimi liian pitk'#228
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clRed
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
        Visible = False
      end
      object CBRadat: TComboBox
        Left = 9
        Top = 20
        Width = 126
        Height = 22
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = CBRadatChange
      end
      object EdtRNimi: TEdit
        Left = 3
        Top = 104
        Width = 84
        Height = 22
        TabOrder = 1
        OnChange = EdtRNimiChange
        OnExit = EdtRNimiExit
        OnKeyPress = KeyToUpper
      end
      object SGRata: TStringGrid
        Left = 244
        Top = 25
        Width = 229
        Height = 504
        ColCount = 3
        DefaultColWidth = 60
        DefaultRowHeight = 20
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
        TabOrder = 2
        OnClick = SGRataClick
        OnMouseWheelDown = SGRataMouseWheelDown
        OnMouseWheelUp = SGRataMouseWheelUp
        OnSetEditText = SGRataSetEditText
      end
      object BtnTallRata: TButton
        Left = 12
        Top = 420
        Width = 188
        Height = 45
        Caption = 'Tallenna k'#228'sitelty rata muistiin'
        TabOrder = 3
        OnClick = BtnTallRataClick
      end
      object BtnRataTyhjenna: TButton
        Left = 488
        Top = 102
        Width = 53
        Height = 50
        Caption = 'Tyhjenn'#228' taulukko'
        TabOrder = 4
        WordWrap = True
        OnClick = BtnRataTyhjennaClick
      end
      object BtnRataLisaaRasti: TButton
        Left = 488
        Top = 184
        Width = 53
        Height = 50
        Caption = 'Lis'#228#228' rasti'
        TabOrder = 5
        WordWrap = True
        OnClick = BtnRataLisaaRastiClick
      end
      object BtnRataPoistaRasti: TButton
        Left = 489
        Top = 240
        Width = 53
        Height = 50
        Caption = 'Poista rasti'
        TabOrder = 6
        WordWrap = True
        OnClick = BtnRataPoistaRastiClick
      end
      object RGUNimi: TRadioGroup
        Left = 106
        Top = 70
        Width = 118
        Height = 57
        Caption = ' Uusi rata / nimi'
        Items.Strings = (
          'Vaihda radan nimi'
          'Lis'#228#228' rata')
        TabOrder = 7
        Visible = False
        OnClick = RGUNimiClick
      end
      object BtnPoistaRata: TButton
        Left = 12
        Top = 475
        Width = 188
        Height = 45
        Caption = 'Poista rata'
        TabOrder = 8
        OnClick = BtnPoistaRataClick
      end
      object GroupBox2: TGroupBox
        Left = 5
        Top = 135
        Width = 233
        Height = 98
        Caption = ' Nollausennakko l'#228'hd'#246'ss'#228' '
        TabOrder = 9
        object Label4: TLabel
          Left = 9
          Top = 62
          Width = 139
          Height = 14
          Caption = 'Nollausennakko l'#228'hd'#246'ss'#228' (s)'
          Visible = False
        end
        object CBLahto: TComboBox
          Left = 9
          Top = 25
          Width = 211
          Height = 22
          ItemIndex = 0
          TabOrder = 0
          Text = 'Nollausennakko vaihtelee'
          OnChange = CBLahtoChange
          Items.Strings = (
            'Nollausennakko vaihtelee'
            'Nollaus l'#228'ht'#246'hetkell'#228
            'Kiinte'#228' nollausennakko')
        end
        object EdtEnnakko: TEdit
          Left = 176
          Top = 54
          Width = 44
          Height = 22
          Alignment = taRightJustify
          TabOrder = 1
          Text = '0'
          Visible = False
        end
      end
      object GroupBox3: TGroupBox
        Left = 3
        Top = 239
        Width = 235
        Height = 153
        Caption = ' Maaliviivan leimasin ja ajanotto '
        TabOrder = 10
        object LblMatkaMaaliin: TLabel
          Left = 9
          Top = 115
          Width = 141
          Height = 14
          Caption = 'Matka viim. rastilta maaliin (m)'
          Visible = False
        end
        object CBMaali: TComboBox
          Left = 7
          Top = 21
          Width = 223
          Height = 22
          ItemIndex = 0
          TabOrder = 0
          Text = 'Maaliviivalla leimasin (tavallinen tai online)'
          OnChange = CBMaaliChange
          Items.Strings = (
            'Maaliviivalla leimasin (tavallinen tai online)'
            'Maaliviivalla ei leimasinta')
        end
        object CBAutoTulos: TCheckBox
          Left = 7
          Top = 49
          Width = 211
          Height = 45
          Caption = 
            'Leimantarkastus laskee automaattisesti tulokset, ellei jo otettu' +
            'na. (Poista ruksi vain, kun poikeuksellisia syit'#228')'
          Checked = True
          State = cbChecked
          TabOrder = 1
          Visible = False
          WordWrap = True
        end
        object EdtMatkaMaaliin: TEdit
          Left = 187
          Top = 112
          Width = 33
          Height = 22
          Alignment = taRightJustify
          TabOrder = 2
          Text = '0'
          Visible = False
        end
      end
      object BitBtn3: TBitBtn
        Left = 510
        Top = 16
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
        TabStop = False
        OnClick = BitBtn3Click
      end
      object EdtLahto: TEdit
        Left = 481
        Top = 333
        Width = 49
        Height = 22
        TabOrder = 12
        OnKeyPress = KeyToUpper
      end
      object EdtMaali: TEdit
        Left = 481
        Top = 380
        Width = 49
        Height = 22
        TabOrder = 13
        OnKeyPress = KeyToUpper
      end
      object EdtRatapit: TEdit
        Left = 482
        Top = 425
        Width = 49
        Height = 22
        TabOrder = 14
        OnKeyPress = KeyToUpper
      end
      object EdtNousu: TEdit
        Left = 483
        Top = 473
        Width = 49
        Height = 22
        TabOrder = 15
        OnKeyPress = KeyToUpper
      end
    end
    object TabSheetLeimat: TTabSheet
      Caption = 'Rastileimasimet'
      ImageIndex = 1
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label29: TLabel
        Left = 462
        Top = 228
        Width = 86
        Height = 70
        Caption = 
          'Varoitus: Lopeta kent'#228'n muokkaus painamalla Enter tai vaihtamall' +
          'a ruutua'
        WordWrap = True
      end
      object SGLeimat: TStringGrid
        Left = 14
        Top = 16
        Width = 442
        Height = 513
        ColCount = 7
        DefaultColWidth = 48
        DefaultRowHeight = 20
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goEditing, goTabs, goAlwaysShowEditor]
        TabOrder = 0
        OnClick = SGLeimatClick
        OnExit = SGLeimatExit
        OnMouseWheelDown = SGLeimatMouseWheelDown
        OnMouseWheelUp = SGLeimatMouseWheelUp
        OnSetEditText = SGLeimatSetEditText
      end
      object BtnTallKoodit: TButton
        Left = 476
        Top = 169
        Width = 57
        Height = 53
        Caption = 'Tallenna koodit muistiin'
        TabOrder = 1
        WordWrap = True
        OnClick = BtnTallKooditClick
      end
      object BtnTyhjKoodit: TButton
        Left = 476
        Top = 367
        Width = 57
        Height = 50
        Caption = 'Tyhjenn'#228' taulukko'
        TabOrder = 2
        WordWrap = True
        OnClick = BtnTyhjKooditClick
      end
      object BtnLeimatLisaaRasti: TButton
        Left = 476
        Top = 47
        Width = 57
        Height = 52
        Caption = 'Lis'#228#228' rasti'
        TabOrder = 3
        WordWrap = True
        OnClick = BtnLeimatLisaaRastiClick
      end
      object BtnLeimatPoistaRasti: TButton
        Left = 476
        Top = 108
        Width = 57
        Height = 52
        Caption = 'Poista rasti'
        TabOrder = 4
        WordWrap = True
        OnClick = BtnLeimatPoistaRastiClick
      end
      object BtnPeruutaKoodi: TButton
        Left = 476
        Top = 309
        Width = 57
        Height = 50
        Caption = 'Peruuta muu- tokset'
        TabOrder = 5
        WordWrap = True
        OnClick = BtnPeruutaKoodiClick
      end
      object BtnOletuskoodit: TButton
        Left = 476
        Top = 425
        Width = 57
        Height = 50
        Caption = 'Laadi oletus- taulukko'
        TabOrder = 6
        WordWrap = True
        OnClick = BtnOletuskooditClick
      end
      object BitBtn4: TBitBtn
        Left = 502
        Top = 10
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
        TabOrder = 7
        TabStop = False
        OnClick = BitBtn4Click
      end
    end
    object TabSheetXML: TTabSheet
      Caption = 'Siirtotiedostot'
      ImageIndex = 2
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label8: TLabel
        Left = 14
        Top = 16
        Width = 127
        Height = 14
        Caption = 'Luettavan tiedoston tyyppi'
      end
      object Label5: TLabel
        Left = 272
        Top = 56
        Width = 163
        Height = 14
        Caption = 'Aiemmin luetut tai m'#228#228'ritellyt tiedot'
      end
      object Label13: TLabel
        Left = 272
        Top = 79
        Width = 30
        Height = 14
        Caption = 'Ratoja'
      end
      object Label14: TLabel
        Left = 272
        Top = 108
        Width = 36
        Height = 14
        Caption = 'Rasteja'
      end
      object Label30: TLabel
        Left = 264
        Top = 240
        Width = 240
        Height = 56
        Caption = 
          'Aiempia tietoja k'#228'sitell'#228#228'n siin'#228' muodossa kuin ne on luettu XML' +
          '-tiedostosta. Tehtyjen muutosten tallentuminen edellytt'#228#228' kirjoi' +
          'ttamista tiedostoon ja lukemista uudelleen t'#228'st'#228' tiedostosta.'
        WordWrap = True
      end
      object CBTiedLaji: TComboBox
        Left = 14
        Top = 35
        Width = 161
        Height = 22
        ItemIndex = 0
        TabOrder = 0
        Text = 'XML (IOF-muoto V2.0.3)'
        OnChange = CBTiedLajiChange
        Items.Strings = (
          'XML (IOF-muoto V2.0.3)'
          'XML (IOF-muoto V3.0)'
          'OCAD 8'
          'OCAD 9'
          'Control Point'
          'Vanha RADAT.LST'
          'Vanha LEIMAT.LST')
      end
      object GBXML: TGroupBox
        Left = 13
        Top = 74
        Width = 217
        Height = 197
        Caption = ' XML-tiedoston m'#228#228'rityksi'#228' '
        TabOrder = 1
        object Label31: TLabel
          Left = 12
          Top = 132
          Width = 148
          Height = 28
          Caption = 'Yll'#228' olevat valinnat koskevat XML-standardin versiota 2.0.3.'
          WordWrap = True
        end
        object RGXMLrata: TRadioGroup
          Left = 12
          Top = 18
          Width = 185
          Height = 92
          Caption = ' Radan nimi  tiedoston  mukaisesta '
          ItemIndex = 0
          Items.Strings = (
            'Ratavariaation nimest'#228
            'Radan nimest'#228
            'Nimest'#228' + variaatiosta'
            'Sarjan lyhyest'#228' nimest'#228)
          TabOrder = 0
        end
        object CBVariaatio: TCheckBox
          Left = 12
          Top = 114
          Width = 177
          Height = 17
          Caption = 'Liit'#228' ratavariaatio radan nimeen'
          TabOrder = 1
          Visible = False
        end
        object CBXMLLeimat: TCheckBox
          Left = 12
          Top = 168
          Width = 153
          Height = 17
          Caption = 'Lue my'#246's leimasinkoodit'
          Checked = True
          State = cbChecked
          TabOrder = 2
        end
      end
      object GBOcad8: TGroupBox
        Left = 11
        Top = 78
        Width = 211
        Height = 110
        Caption = ' OCAD 8 m'#228#228'rityksi'#228' '
        TabOrder = 2
        Visible = False
        object RGOcadRata: TRadioGroup
          Left = 9
          Top = 15
          Width = 185
          Height = 51
          Caption = ' Radan nimi  tiedoston  mukaisesta '
          Items.Strings = (
            'Sarjan nimest'#228
            'Ratatunnuksesta')
          TabOrder = 0
        end
        object CBpilkkuerotin: TCheckBox
          Left = 16
          Top = 80
          Width = 169
          Height = 17
          Caption = 'My'#246's pilkku on kentt'#228'erotin'
          TabOrder = 1
        end
      end
      object GBcp: TGroupBox
        Left = 17
        Top = 74
        Width = 213
        Height = 145
        Caption = ' T'#228'sment'#228'vi'#228' m'#228#228'rityksi'#228' '
        TabOrder = 3
        Visible = False
        object Label9: TLabel
          Left = 16
          Top = 82
          Width = 128
          Height = 14
          Caption = 'Ratanimikent'#228'n j'#228'rjestysno'
        end
        object Label10: TLabel
          Left = 16
          Top = 101
          Width = 105
          Height = 28
          Caption = 'Ens. rastikoodikent'#228'n j'#228'rjestysno'
          WordWrap = True
        end
        object RGErotin: TRadioGroup
          Left = 9
          Top = 16
          Width = 185
          Height = 57
          Caption = ' Kentti'#228' erottaa '
          Items.Strings = (
            'Pilkku, puolipiste tai tabulaattori'
            'V'#228'lily'#246'nnit')
          TabOrder = 0
        end
        object EdtRataSij: TEdit
          Left = 157
          Top = 79
          Width = 28
          Height = 22
          Alignment = taRightJustify
          TabOrder = 1
          Text = '0'
        end
        object EdtKoodiSij: TEdit
          Left = 157
          Top = 106
          Width = 28
          Height = 22
          Alignment = taRightJustify
          TabOrder = 2
          Text = '0'
        end
      end
      object BtnImport: TButton
        Left = 13
        Top = 291
        Width = 98
        Height = 46
        Caption = 'Lue tiedosto'
        TabOrder = 4
        OnClick = BtnImportClick
      end
      object RGlisaa: TRadioGroup
        Left = 264
        Top = 147
        Width = 246
        Height = 80
        Caption = 'Toiminta, kun tietoja jo k'#228'sitelt'#228'vin'#228
        Enabled = False
        ItemIndex = 1
        Items.Strings = (
          'Lis'#228#228' tiedot k'#228'sitelt'#228'vin'#228' oleviin'
          'Poista aiemmat tiedot'
          'Lis'#228#228' korvaten aiemmat samannimiset radat')
        TabOrder = 5
      end
      object EdtRataLkm: TEdit
        Left = 336
        Top = 76
        Width = 48
        Height = 22
        Alignment = taRightJustify
        TabOrder = 6
        Text = '0'
      end
      object EdtRastiLkm: TEdit
        Left = 336
        Top = 105
        Width = 48
        Height = 22
        Alignment = taRightJustify
        TabOrder = 7
        Text = '0'
      end
      object BitBtn2: TBitBtn
        Left = 264
        Top = 16
        Width = 203
        Height = 37
        Margins.Left = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Caption = 'T'#228't'#228' kaavaketta koskevia ohjeita'
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
        WordWrap = True
        OnClick = BitBtn2Click
      end
    end
    object TabSheetSarjat: TTabSheet
      Caption = 'Sarjat'
      ImageIndex = 5
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label20: TLabel
        Left = 24
        Top = 11
        Width = 174
        Height = 16
        Caption = 'Sarjoille kohdennetut radat'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        ParentFont = False
      end
      object Label21: TLabel
        Left = 366
        Top = 56
        Width = 169
        Height = 42
        Caption = 
          'Ohjelman oletuksena on sarjanimen mukainen radan nimi. T'#228't'#228' tiet' +
          'oa ei tallenneta.'
        WordWrap = True
      end
      object Label22: TLabel
        Left = 366
        Top = 107
        Width = 169
        Height = 42
        Caption = 
          'T'#228'h'#228'n taulukkoon tehdyt muutokset tallentuvat automaattisesti ti' +
          'etoja tallennettaessa'
        WordWrap = True
      end
      object SGSarjat: TStringGrid
        Left = 24
        Top = 48
        Width = 320
        Height = 494
        ColCount = 2
        DefaultColWidth = 140
        DefaultRowHeight = 20
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
        TabOrder = 0
        OnKeyPress = SGSarjatKeyPress
        OnMouseWheelDown = SGSarjatMouseWheelDown
        OnMouseWheelUp = SGSarjatMouseWheelUp
        OnSetEditText = SGSarjatSetEditText
      end
      object BtnPoistaKohdennus: TButton
        Left = 376
        Top = 224
        Width = 145
        Height = 41
        Caption = 'Poista valittu kohdennus'
        TabOrder = 1
        OnClick = BtnPoistaKohdennusClick
      end
      object BtnTyhjKohdennukset: TButton
        Left = 376
        Top = 291
        Width = 145
        Height = 41
        Caption = 'Tyhjenn'#228' taulukko'
        TabOrder = 2
        OnClick = BtnTyhjKohdennuksetClick
      end
    end
    object TabSheet1: TTabSheet
      Caption = 'Kirjoitintulosteet'
      ImageIndex = 3
      OnShow = TabSheet1Show
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label15: TLabel
        Left = 11
        Top = 4
        Width = 34
        Height = 14
        Caption = 'Kirjoitin'
      end
      object Label16: TLabel
        Left = 11
        Top = 52
        Width = 35
        Height = 14
        Caption = 'Tuloste'
      end
      object Label17: TLabel
        Left = 118
        Top = 96
        Width = 99
        Height = 14
        Caption = 'Ei-tulostettavat radat'
      end
      object Label18: TLabel
        Left = 312
        Top = 96
        Width = 90
        Height = 14
        Caption = 'Tulostettavat radat'
      end
      object KirjoitinVal: TComboBox
        Left = 11
        Top = 21
        Width = 398
        Height = 22
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        Text = 'Valitse kirjoitin'
      end
      object CBTuloste: TComboBox
        Left = 11
        Top = 69
        Width = 398
        Height = 22
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        Text = 'Valitse tuloste'
        Items.Strings = (
          'Piikkikuviot'
          'Ratatiedot'
          'Leimasinkoodit rastitunnusj'#228'rjestyksess'#228
          'Leimasinkoodit koodij'#228'rjestyksess'#228)
      end
      object LBRadat: TListBox
        Left = 118
        Top = 115
        Width = 113
        Height = 406
        ItemHeight = 14
        MultiSelect = True
        TabOrder = 2
        OnDblClick = LBRadatDblClick
      end
      object BtnPoisKaikki: TButton
        Left = 245
        Top = 344
        Width = 53
        Height = 49
        Caption = 'Poista kaikki'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        WordWrap = True
        OnClick = BtnPoisKaikkiClick
      end
      object BtnPoisValitut: TButton
        Left = 245
        Top = 279
        Width = 53
        Height = 49
        Caption = 'Poista valitut'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        WordWrap = True
        OnClick = BtnPoisValitutClick
      end
      object BtnLisValitut: TButton
        Left = 245
        Top = 214
        Width = 53
        Height = 49
        Caption = 'Lis'#228#228' valitut'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        WordWrap = True
        OnClick = BtnLisValitutClick
      end
      object BtnLisKaikki: TButton
        Left = 245
        Top = 149
        Width = 53
        Height = 49
        Caption = 'Lis'#228#228' kaikki'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        WordWrap = True
        OnClick = BtnLisKaikkiClick
      end
      object LBRataVal: TListBox
        Left = 312
        Top = 115
        Width = 113
        Height = 406
        ItemHeight = 14
        MultiSelect = True
        TabOrder = 7
        OnDblClick = LBRataValDblClick
      end
      object BtnTulosta: TButton
        Left = 16
        Top = 328
        Width = 75
        Height = 63
        Caption = 'Tulosta'
        TabOrder = 8
        OnClick = BtnTulostaClick
      end
      object BitBtn5: TBitBtn
        Left = 505
        Top = 10
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
        TabOrder = 9
        TabStop = False
        OnClick = BitBtn5Click
      end
    end
    object TabSheetOngelmat: TTabSheet
      Caption = 'Tarkastuksen tulos'
      ImageIndex = 4
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object Label7: TLabel
        Left = 16
        Top = 7
        Width = 307
        Height = 14
        Caption = 
          'Havaitut ongelmat (tarkastus koskee muistiin tallennettuja tieto' +
          'ja)'
      end
      object Memo1: TMemo
        Left = 16
        Top = 32
        Width = 521
        Height = 489
        ScrollBars = ssBoth
        TabOrder = 0
      end
      object BitBtn6: TBitBtn
        Left = 513
        Top = 0
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
        TabOrder = 1
        TabStop = False
        OnClick = BitBtn6Click
      end
    end
    object TabSheetKoord: TTabSheet
      Caption = 'Rastikoordinaatit'
      ImageIndex = 6
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object GroupBox1: TGroupBox
        Left = 24
        Top = 25
        Width = 397
        Height = 72
        Caption = ' Kartan koordinaatit  '
        TabOrder = 0
        object Label23: TLabel
          Left = 264
          Top = 18
          Width = 51
          Height = 14
          Caption = 'Mittakaava'
        end
        object Label24: TLabel
          Left = 14
          Top = 18
          Width = 31
          Height = 14
          Caption = 'Vasen'
        end
        object Label25: TLabel
          Left = 134
          Top = 18
          Width = 16
          Height = 14
          Caption = 'Yl'#228
        end
        object Label26: TLabel
          Left = 134
          Top = 46
          Width = 16
          Height = 14
          Caption = 'Ala'
        end
        object Label27: TLabel
          Left = 14
          Top = 46
          Width = 27
          Height = 14
          Caption = 'Oikea'
        end
        object EdtScale: TEdit
          Left = 330
          Top = 15
          Width = 49
          Height = 22
          Alignment = taRightJustify
          TabOrder = 0
          Text = '10000'
        end
        object EdtMapLeft: TEdit
          Left = 64
          Top = 15
          Width = 52
          Height = 22
          Alignment = taRightJustify
          TabOrder = 1
          Text = '0,0'
        end
        object EdtMapTop: TEdit
          Left = 174
          Top = 15
          Width = 52
          Height = 22
          Alignment = taRightJustify
          TabOrder = 2
          Text = '0,0'
        end
        object EdtMapBottom: TEdit
          Left = 174
          Top = 43
          Width = 52
          Height = 22
          Alignment = taRightJustify
          TabOrder = 3
          Text = '0,0'
        end
        object EdtMapRight: TEdit
          Left = 64
          Top = 43
          Width = 52
          Height = 22
          Alignment = taRightJustify
          TabOrder = 4
          Text = '0,0'
        end
      end
      object SGKoord: TStringGrid
        Left = 24
        Top = 112
        Width = 273
        Height = 426
        ColCount = 4
        DefaultColWidth = 60
        DefaultRowHeight = 18
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs]
        TabOrder = 1
        OnGetEditText = SGKoordGetEditText
        OnSetEditText = SGKoordSetEditText
      end
      object CBKirjKoord: TCheckBox
        Left = 336
        Top = 136
        Width = 177
        Height = 17
        Caption = 'Kirjoita koordinaatit ratatietoihin'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
      object BtnVainKoord: TButton
        Left = 328
        Top = 183
        Width = 193
        Height = 41
        Caption = 'Kirjoita erillinen koordinaattitiedosto'
        TabOrder = 3
        OnClick = BtnVainKoordClick
      end
      object BtnLueKoordCsv: TButton
        Left = 328
        Top = 247
        Width = 193
        Height = 41
        Caption = 'Lue koordinaait csv-tiedostosta'
        TabOrder = 4
        OnClick = BtnLueKoordCsvClick
      end
      object BtnTyhjKoord: TButton
        Left = 328
        Top = 369
        Width = 193
        Height = 40
        Caption = 'Tyhjenn'#228' koordinaattitiedot'
        TabOrder = 5
        OnClick = BtnTyhjKoordClick
      end
      object BtnPoistaKoord: TButton
        Left = 328
        Top = 428
        Width = 193
        Height = 41
        Caption = 'Poista rivi'
        TabOrder = 6
        OnClick = BtnPoistaKoordClick
      end
      object BtnKoordKayttoon: TButton
        Left = 328
        Top = 489
        Width = 193
        Height = 41
        Caption = 'Ota tiedot k'#228'ytt'#246#246'n'
        TabOrder = 7
        OnClick = BtnKoordKayttoonClick
      end
      object BtnKirjKoordCsv: TButton
        Left = 328
        Top = 305
        Width = 193
        Height = 41
        Caption = 'Kirjoita koordinaait csv-tiedostoon'
        TabOrder = 8
        OnClick = BtnKirjKoordCsvClick
      end
    end
  end
  object BtnSulje: TButton
    Left = 586
    Top = 509
    Width = 67
    Height = 60
    Caption = 'Sulje kaavake'
    TabOrder = 1
    WordWrap = True
    OnClick = BtnSuljeClick
  end
  object BtnOtaKayttoon: TButton
    Left = 586
    Top = 409
    Width = 67
    Height = 74
    Caption = 'Ota muutetut tiedot k'#228'ytt'#246#246'n'
    TabOrder = 2
    Visible = False
    WordWrap = True
    OnClick = BtnOtaKayttoonClick
  end
  object BtnTarkista: TButton
    Left = 586
    Top = 59
    Width = 67
    Height = 49
    Caption = 'Tarkasta m'#228#228'ritykset'
    TabOrder = 3
    WordWrap = True
    OnClick = BtnTarkistaClick
  end
  object BitBtn1: TBitBtn
    Left = 620
    Top = 10
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
    TabOrder = 4
    TabStop = False
    Visible = False
    WordWrap = True
    OnClick = BitBtn1Click
  end
  object GBTall: TGroupBox
    Left = 574
    Top = 199
    Width = 86
    Height = 194
    Ctl3D = True
    ParentCtl3D = False
    TabOrder = 5
    object Label11: TLabel
      Left = 10
      Top = 12
      Width = 62
      Height = 56
      Caption = 'Ohjelman xml-tiedosto- muodon toiminnot'
      WordWrap = True
    end
    object BtnLueXml: TButton
      Left = 11
      Top = 78
      Width = 66
      Height = 48
      Caption = 'Lue radat ja rastit (xml)'
      TabOrder = 0
      WordWrap = True
      OnClick = BtnLueXmlClick
    end
    object BtnKirjXml: TButton
      Left = 11
      Top = 131
      Width = 66
      Height = 55
      Caption = 'Kirjoita radat ja rastit (xml)'
      TabOrder = 1
      WordWrap = True
      OnClick = BtnKirjXmlClick
    end
  end
  object CBNaytaTall: TCheckBox
    Left = 576
    Top = 114
    Width = 90
    Height = 73
    Caption = 'N'#228'yt'#228' tiedoston lukemis- ja tallennus- painikkeet'
    TabOrder = 6
    Visible = False
    WordWrap = True
    OnClick = CBNaytaTallClick
  end
  object OpenDialog1: TOpenDialog
    Left = 456
    Top = 16
  end
  object SaveDialog1: TSaveDialog
    Left = 512
    Top = 24
  end
  object MainMenu1: TMainMenu
    Left = 544
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Lueradatjarastitxml1: TMenuItem
        Caption = '&Lue radat ja rastit (xml)'
        OnClick = Lueradatjarastitxml1Click
      end
      object allennaradatjarastit1: TMenuItem
        Caption = '&Tallenna radat ja rastit '
        OnClick = allennaradatjarastit1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object LuevanhaRADATLST1: TMenuItem
        Caption = 'Lue vanha RADAT.LST'
        OnClick = LuevanhaRADATLST1Click
      end
      object KirjoitavanhaRADATLST1: TMenuItem
        Caption = 'Kirjoita vanha RADAT.LST'
        OnClick = KirjoitavanhaRADATLST1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object LuevanhaLEIMATLST1: TMenuItem
        Caption = 'Lue vanha LEIMAT.LST'
        OnClick = LuevanhaLEIMATLST1Click
      end
      object KirjoitavanhaLEIMATLST1: TMenuItem
        Caption = 'Kirjoita vanha LEIMAT.LST'
        OnClick = KirjoitavanhaLEIMATLST1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = 'Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object Ratatoimintoja1: TMenuItem
      Caption = 'Ratatoimintoja'
      object Kopioiradannollausennakkokaikkiinratoihin1: TMenuItem
        Caption = 'Kopioi radan nollausennakko kaikkiin ratoihin'
        OnClick = Kopioiradannollausennakkokaikkiinratoihin1Click
      end
      object Kopioiradanmaalivalinnatkaikkiinratoihin1: TMenuItem
        Caption = 'Kopioi radan maalivalinnat kaikkiin ratoihin'
        OnClick = Kopioiradanmaalivalinnatkaikkiinratoihin1Click
      end
      object Liskaikkiinratoihinmaalinrastikoodi1: TMenuItem
        Caption = 'Lis'#228#228' kaikkiin ratoihin maalin rastikoodi (numeroarvo)'
        OnClick = Liskaikkiinratoihinmaalinrastikoodi1Click
      end
      object MerkLahtokoodi: TMenuItem
        Caption = 'Merkitse kaikille radoille l'#228'hd'#246'n koodi'
        OnClick = MerkLahtokoodiClick
      end
      object MerkMaalikoodi: TMenuItem
        Caption = 'Merkitse kaikille radoille maalin koodi'
        OnClick = MerkMaalikoodiClick
      end
      object Laskerastivlienmatkatkoordinaateista1: TMenuItem
        Caption = 'Laske rastiv'#228'lien matkat koordinaateista'
        OnClick = Laskerastivlienmatkatkoordinaateista1Click
      end
      object Merkitsekaikkienratojenpituudetmaalinmukaan1: TMenuItem
        Caption = 'Merkitse kaikkien ratojen pituudet maalin mukaan'
        OnClick = Merkitsekaikkienratojenpituudetmaalinmukaan1Click
      end
    end
    object Kartta1: TMenuItem
      Caption = '&Kartta'
      object Avaakartta1: TMenuItem
        Caption = '&Avaa kartta'
        OnClick = Avaakartta1Click
      end
      object Nytrata1: TMenuItem
        Caption = 'N'#228'yt'#228' &rata'
        OnClick = Nytrata1Click
      end
      object Nytrastit1: TMenuItem
        Caption = '&N'#228'yt'#228' rastit'
        OnClick = Nytrastit1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      OnClick = Help1Click
    end
  end
  object OpenDialog2: TOpenDialog
    Left = 528
    Top = 80
  end
end
