object FormTuloskaava: TFormTuloskaava
  Left = 0
  Top = 0
  Caption = 'Kaavan muokkaus'
  ClientHeight = 396
  ClientWidth = 638
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 20
    Top = 12
    Width = 29
    Height = 15
    Caption = 'Sarja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 20
    Top = 36
    Width = 30
    Height = 15
    Caption = 'Vaihe'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 20
    Top = 58
    Width = 28
    Height = 15
    Caption = 'Piste'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 20
    Top = 84
    Width = 596
    Height = 15
    Caption = 
      'Yll'#228' valitun pisteen tulos tai vaihekohtaiset pisteet lasketaan ' +
      'yhdest'#228' tai kahdesta muusta tuloksesta kaavalla'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 24
    Top = 107
    Width = 525
    Height = 12
    Caption = 
      'Tulos (tai pisteet) = kerroin x (yl'#228'tulos - alatulos - nollatulo' +
      's) / jakaja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Lucida Console'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 20
    Top = 144
    Width = 469
    Height = 15
    Caption = 
      'Laskentaa ei tehd'#228', ellei yl'#228'tulosta ole. Alatulos vaaditaan vai' +
      'n, jos vaatimus on valittu.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 20
    Top = 197
    Width = 504
    Height = 15
    Caption = 
      'Lis'#228'ksi voidaan tulokselle esitt'#228#228' ala- ja yl'#228'raja, joita sovell' +
      'etaan, kun tulos ei noudata rajaa.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 20
    Top = 263
    Width = 94
    Height = 15
    Caption = 'Yl'#228'tuloksen piste'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 20
    Top = 286
    Width = 94
    Height = 15
    Caption = 'Alatuloksen piste'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label10: TLabel
    Left = 20
    Top = 333
    Width = 92
    Height = 15
    Caption = 'Tuloksen alaraja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label11: TLabel
    Left = 21
    Top = 313
    Width = 40
    Height = 15
    Caption = 'Kerroin'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label12: TLabel
    Left = 191
    Top = 309
    Width = 36
    Height = 15
    Caption = 'Jakaja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label13: TLabel
    Left = 191
    Top = 333
    Width = 38
    Height = 15
    Caption = 'Yl'#228'raja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label14: TLabel
    Left = 262
    Top = 249
    Width = 3
    Height = 15
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label15: TLabel
    Left = 20
    Top = 215
    Width = 236
    Height = 15
    Caption = 'Aikoja k'#228'sitell'#228#228'n sekunnin tuhannesosina'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label16: TLabel
    Left = 191
    Top = 263
    Width = 200
    Height = 15
    Caption = 'Lopputuloksen piste on 0, l'#228'hd'#246'n -1.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label17: TLabel
    Left = 307
    Top = 333
    Width = 100
    Height = 15
    Caption = 'Yl'#228'raja=0: Ei rajaa'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label18: TLabel
    Left = 20
    Top = 357
    Width = 170
    Height = 15
    Caption = 'Nolla pistett'#228' antava tulos (ms)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label19: TLabel
    Left = 20
    Top = 162
    Width = 342
    Height = 15
    Caption = 'Jos alatuloksen piste on -1, k'#228'ytet'#228#228'n yl'#228'tulosta sellaisenaan. '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label20: TLabel
    Left = 20
    Top = 179
    Width = 440
    Height = 15
    Caption = 
      'Nolla pistett'#228' antavaa tulosta k'#228'ytet'#228#228'n vain laskettaessa vaihe' +
      'kohtaisia pisteit'#228'.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label21: TLabel
    Left = 20
    Top = 127
    Width = 562
    Height = 15
    Caption = 
      #39'Yl'#228'tulos'#39' on tulos tarkastellun jakson lopussa ja '#39'alatulos'#39' se' +
      'n alkupisteess'#228'. Alatulosta ei aina k'#228'ytet'#228'.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object EdtYla: TEdit
    Left = 128
    Top = 260
    Width = 49
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
  end
  object EdtAla: TEdit
    Left = 128
    Top = 283
    Width = 49
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '1'
  end
  object EdtAlaraja: TEdit
    Left = 128
    Top = 330
    Width = 49
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    Text = '0'
  end
  object EdtKerroin: TEdit
    Left = 128
    Top = 306
    Width = 49
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '1'
  end
  object EdtJakaja: TEdit
    Left = 240
    Top = 306
    Width = 59
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = '1'
  end
  object EdtYlaraja: TEdit
    Left = 240
    Top = 330
    Width = 59
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Text = '0'
  end
  object BtnTallenna: TButton
    Left = 430
    Top = 285
    Width = 194
    Height = 25
    Caption = 'Tallenna ja sulje kaavake'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = BtnTallennaClick
  end
  object BtnPoista: TButton
    Left = 430
    Top = 318
    Width = 194
    Height = 25
    Caption = 'Poista kaava k'#228'yt'#246'st'#228' ja sulje'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = BtnPoistaClick
  end
  object BtnKaikille: TButton
    Left = 304
    Top = 19
    Width = 232
    Height = 25
    Caption = 'Kopioi kaikille sarjoille ja tallenna'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
    OnClick = BtnKaikilleClick
  end
  object CBVaadiAla: TCheckBox
    Left = 191
    Top = 285
    Width = 97
    Height = 17
    Caption = 'Vaadi alatulos'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
  end
  object BtnPoistaKaikilta: TButton
    Left = 304
    Top = 49
    Width = 232
    Height = 25
    Caption = 'Poista kaava kaikilta sarjoilta ja tallenna'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    OnClick = BtnPoistaKaikiltaClick
  end
  object Button2: TButton
    Left = 430
    Top = 352
    Width = 194
    Height = 25
    Caption = 'Poistu tallentamatta muutoksia'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    OnClick = Button2Click
  end
  object EdtNollataso: TEdit
    Left = 240
    Top = 354
    Width = 59
    Height = 23
    Alignment = taRightJustify
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    Text = '0'
  end
  object CBSarja: TComboBox
    Left = 60
    Top = 9
    Width = 151
    Height = 23
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    Text = 'CBSarja'
    OnChange = CBSarjaChange
  end
  object CBVaihe: TComboBox
    Left = 60
    Top = 32
    Width = 151
    Height = 23
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
    Text = 'CBSarja'
    OnChange = CBVaiheChange
  end
  object CBLaskettava: TComboBox
    Left = 60
    Top = 55
    Width = 151
    Height = 23
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
    Text = 'CBSarja'
    OnChange = CBLaskettavaChange
  end
  object EdtKaavaOn: TEdit
    Left = 430
    Top = 244
    Width = 194
    Height = 23
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
  end
  object CBSakotPois: TCheckBox
    Left = 191
    Top = 241
    Width = 238
    Height = 17
    Caption = 'J'#228't'#228' sakot pois k'#228'ytett'#228'vist'#228' tuloksista'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 17
  end
end
