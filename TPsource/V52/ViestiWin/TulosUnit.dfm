object TilanneForm: TTilanneForm
  Left = 435
  Top = 297
  Caption = 'Tilanne'
  ClientHeight = 654
  ClientWidth = 881
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object TulosGrid: TStringGrid
    Left = 0
    Top = 78
    Width = 880
    Height = 586
    DefaultRowHeight = 18
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving]
    ParentFont = False
    TabOrder = 0
    OnDblClick = TulosGridDblClick
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 880
    Height = 79
    TabOrder = 1
    object Label1: TLabel
      Left = 160
      Top = 10
      Width = 59
      Height = 18
      Caption = 'K'#228'rjest'#228
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object PaivBtn: TButton
      Left = 13
      Top = 9
      Width = 101
      Height = 24
      Caption = 'P'#228'ivit'#228
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = PaivBtnClick
    end
    object SarjaVal: TComboBox
      Left = 13
      Top = 39
      Width = 95
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
      Text = 'Sarja'
      OnChange = SarjaValChange
      Items.Strings = (
        'Sarja')
    end
    object OsVal: TComboBox
      Left = 118
      Top = 39
      Width = 80
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 2
      Text = '1. osuus'
      OnChange = OsValChange
      Items.Strings = (
        '1. osuus'
        '2. osuus'
        '3. osuus'
        '4. osuus'
        '5. osuus'
        '6. osuus'
        '7. osuus')
    end
    object EditAlusta: TEdit
      Left = 230
      Top = 8
      Width = 82
      Height = 27
      TabStop = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
      ReadOnly = True
      TabOrder = 3
      Text = '00.00.00'
    end
    object PisteVal: TComboBox
      Left = 207
      Top = 39
      Width = 106
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 1
      ParentFont = False
      TabOrder = 4
      Text = 'Vaihto/Maali'
      OnChange = PisteValChange
      Items.Strings = (
        'Osanottajat'
        'Vaihto/Maali'
        '1. v'#228'liaika'
        '2. v'#228'liaika'
        '3. v'#228'liaika'
        '4. v'#228'liaika')
    end
    object JarjVal: TRadioGroup
      Left = 326
      Top = 1
      Width = 110
      Height = 69
      Caption = ' J'#228'rjestys '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 0
      Items.Strings = (
        'Vaihto/maali'
        'Osuusaika')
      ParentFont = False
      TabOrder = 5
      OnClick = JarjValClick
    end
    object HylVal: TCheckBox
      Left = 447
      Top = 9
      Width = 138
      Height = 20
      Caption = 'Hyl'#228'tyt ja keskeytt.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = HylValClick
    end
    object EroVal: TCheckBox
      Left = 447
      Top = 29
      Width = 105
      Height = 21
      Caption = 'Ero k'#228'rkeen'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      OnClick = EroValClick
    end
    object Viimeisimmat: TCheckBox
      Left = 447
      Top = 51
      Width = 129
      Height = 20
      Caption = 'P'#228'ivit'#228' jatkuvasti'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 8
      OnClick = ViimeisimmatClick
    end
    object CBkaikki: TCheckBox
      Left = 585
      Top = 8
      Width = 105
      Height = 20
      Caption = 'T'#228'ysi osuus'
      Checked = True
      Enabled = False
      State = cbChecked
      TabOrder = 9
      Visible = False
    end
    object CB1puute: TCheckBox
      Left = 585
      Top = 29
      Width = 105
      Height = 21
      Caption = '1 puuttuu'
      Enabled = False
      TabOrder = 10
      Visible = False
    end
    object CB2puute: TCheckBox
      Left = 585
      Top = 50
      Width = 105
      Height = 20
      Caption = '2 puuttuu'
      Enabled = False
      TabOrder = 11
      Visible = False
    end
  end
  object MainMenu1: TMainMenu
    Left = 656
    Top = 40
    object oiminto1: TMenuItem
      Caption = '&Toiminto'
      object Kokoruutu1: TMenuItem
        Caption = 'Koko ruutu'
        OnClick = Kokoruutu1Click
      end
      object Skrollaa1: TMenuItem
        Caption = 'Skrollaa'
        OnClick = Skrollaa1Click
      end
      object Lopetaskrollaus1: TMenuItem
        Caption = 'Lopeta skrollaus'
        OnClick = Lopetaskrollaus1Click
      end
      object Pehmeskrollaus1: TMenuItem
        Caption = 'Skrollaustapa'
        object Liukuva1: TMenuItem
          Caption = 'Liukuva'
          OnClick = Liukuva1Click
        end
        object Rivittinaskeltava1: TMenuItem
          Caption = 'Rivitt'#228'in askeltava'
          OnClick = Rivittinaskeltava1Click
        end
        object Ruutukerrallaan1: TMenuItem
          Caption = 'Sivu kerrallaan'
          OnClick = Ruutukerrallaan1Click
        end
      end
      object aukosarjanptteeksi1: TMenuItem
        Caption = 'N'#228'yt'#246'n minimikesto'
        OnClick = aukosarjanptteeksi1Click
      end
      object Sarjanminimikesto1: TMenuItem
        Caption = 'Lis'#228'kesto rivi'#228' kohti'
        OnClick = Sarjanminimikesto1Click
      end
      object Skrollauksensarjalista1: TMenuItem
        Caption = 'Skrollauksen sarjalista'
        OnClick = Skrollauksensarjalista1Click
      end
      object N4: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object Nyt1: TMenuItem
      Caption = '&N'#228'yt'#228
      object Maatunnus1: TMenuItem
        Caption = '&Maatunnus'
        Checked = True
        OnClick = Maatunnus1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Vainjrjestyspiste1: TMenuItem
        Caption = '&Vain j'#228'rjestyspiste'
        Checked = True
        Visible = False
      end
      object Pisteet1: TMenuItem
        Caption = '&Pisteet (useita)'
        Visible = False
        object Kaikkiajat1: TMenuItem
          Caption = '&Kaikki ajat'
        end
        object Maali1: TMenuItem
          Caption = '&Maali'
        end
        object N1vliaika1: TMenuItem
          Caption = '&1. v'#228'liaika'
          Visible = False
        end
        object N2vliaika1: TMenuItem
          Caption = '&2. v'#228'liaika'
          Visible = False
        end
        object N3vliaika1: TMenuItem
          Caption = '&3. v'#228'liaika'
          Visible = False
        end
        object N4vliaika1: TMenuItem
          Caption = '&4. v'#228'liaika'
          Visible = False
        end
        object N5vliaika1: TMenuItem
          Caption = '&5. v'#228'liaika'
          Visible = False
        end
        object N6vliaika1: TMenuItem
          Caption = '&6. v'#228'liaika'
          Visible = False
        end
        object N7vliaika1: TMenuItem
          Caption = '&7. v'#228'liaika'
          Visible = False
        end
        object N8vliaka1: TMenuItem
          Caption = '&8. v'#228'liaika'
          Visible = False
        end
        object N9vliaika1: TMenuItem
          Caption = '&9. v'#228'liaika'
          Visible = False
        end
        object N10vliaikaA1: TMenuItem
          Caption = '10. v'#228'liaika (&A)'
          Visible = False
        end
        object N11vliaikaB1: TMenuItem
          Caption = '11. v'#228'liaika (&B)'
          Visible = False
        end
        object N12vliaikaC1: TMenuItem
          Caption = '12. v'#228'liaika (&C)'
          Visible = False
        end
        object N13vliaikaD1: TMenuItem
          Caption = '13. v'#228'liaika (&D)'
          Visible = False
        end
        object N14vliaikaE1: TMenuItem
          Caption = '14. v'#228'liaika (&E)'
          Visible = False
        end
        object N15vliaikaF1: TMenuItem
          Caption = '15. v'#228'liaika (&F)'
          Visible = False
        end
        object N16vliaikaG1: TMenuItem
          Caption = '16. v'#228'liaika (&G)'
          Visible = False
        end
        object N17vliaikaH1: TMenuItem
          Caption = '17. v'#228'liaika (&H)'
          Visible = False
        end
        object N18vliaikaI1: TMenuItem
          Caption = '18. v'#228'liaika (&I)'
          Visible = False
        end
        object N19vliaikaJ1: TMenuItem
          Caption = '19. v'#228'liaika (&J)'
          Visible = False
        end
        object N20vliaikaK1: TMenuItem
          Caption = '20. v'#228'liaika (&L)'
          Visible = False
        end
        object N21vliaikaL1: TMenuItem
          Caption = '21. v'#228'liaika (&N)'
          Visible = False
        end
        object N22vliaikaM1: TMenuItem
          Caption = '22. v'#228'liaika (&O)'
          Visible = False
        end
        object N23vliaikaN1: TMenuItem
          Caption = '23. v'#228'liaika (&P)'
          Visible = False
        end
        object N24vliaikaO1: TMenuItem
          Caption = '24. v'#228'liaika (&Q)'
          Visible = False
        end
        object N25vliaikaP1: TMenuItem
          Caption = '25. v'#228'liaika (&R)'
          Visible = False
        end
        object N26vliaikaQ1: TMenuItem
          Caption = '26. v'#228'liaika (&S)'
          Visible = False
        end
        object N27vliaikaR1: TMenuItem
          Caption = '27. v'#228'liaika (&T)'
          Visible = False
        end
        object N28vliaikaS1: TMenuItem
          Caption = '28. v'#228'liaika (&U)'
          Visible = False
        end
        object N29vliaikaT1: TMenuItem
          Caption = '29. v'#228'liaika (&V)'
          Visible = False
        end
        object N30vliaikaU1: TMenuItem
          Caption = '30. v'#228'liaika (&W)'
          Visible = False
        end
      end
      object N3: TMenuItem
        Caption = '-'
        Visible = False
      end
      object Erokrkeen1: TMenuItem
        Caption = 'N'#228'yt'#228' &ero k'#228'rkeen'
      end
      object Etunimiensin1: TMenuItem
        Caption = 'Etunimi ensin'
        Checked = True
        OnClick = Etunimiensin1Click
      end
    end
    object Kirjasin1: TMenuItem
      Caption = '&Kirjasin'
      object Suuri1: TMenuItem
        Caption = '&Suuri (12 pts)'
        OnClick = Suuri1Click
      end
      object Normaali1: TMenuItem
        Caption = '&Normaali (10 pts)'
        Checked = True
        OnClick = Normaali1Click
      end
      object Pieni1: TMenuItem
        Caption = '&Pieni (8 pts)'
        OnClick = Pieni1Click
      end
      object Muumerkkikoko1: TMenuItem
        Caption = '&Muu merkkikoko'
        OnClick = Muumerkkikoko1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Lihavoitu1: TMenuItem
        Caption = '&Lihavoitu'
        OnClick = Lihavoitu1Click
      end
      object N5: TMenuItem
        Caption = '-'
      end
      object allennasarakeleveydet1: TMenuItem
        Caption = '&Tallenna sarakeleveydet'
        OnClick = allennasarakeleveydet1Click
      end
    end
  end
end
