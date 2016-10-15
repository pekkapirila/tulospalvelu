object FormUusinta: TFormUusinta
  Left = 0
  Top = 0
  Caption = 'Tiedonsiirron uusinta'
  ClientHeight = 373
  ClientWidth = 631
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 20
    Top = 8
    Width = 97
    Height = 15
    Caption = 'K'#228'sitelt'#228'v'#228' yhteys'
  end
  object Label2: TLabel
    Left = 20
    Top = 172
    Width = 139
    Height = 15
    Caption = 'Rajaava aika minuutteina'
    Visible = False
  end
  object Label4: TLabel
    Left = 166
    Top = 172
    Width = 114
    Height = 15
    Caption = 'L'#228'hetett'#228'v'#228't osuudet'
    Visible = False
  end
  object CBYhteydet: TComboBox
    Left = 20
    Top = 29
    Width = 145
    Height = 23
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 0
    Text = 'Kaikki yhteydet'
    OnChange = CBYhteydetChange
    Items.Strings = (
      'Kaikki yhteydet')
  end
  object RGTapa: TRadioGroup
    Left = 14
    Top = 58
    Width = 280
    Height = 101
    Caption = ' Uusintal'#228'hetystapa '
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    Items.Strings = (
      'Kaikki sanomat'
      'Viimeisimm'#228't l'#228'hetyshetken mukaan'
      'Emit-v'#228'liajat'
      'XML-muotoiset sanomat ulkoiseen palveluun')
    ParentFont = False
    TabOrder = 1
    OnClick = RGTapaClick
  end
  object EdtRaja: TEdit
    Left = 20
    Top = 195
    Width = 49
    Height = 23
    Alignment = taRightJustify
    TabOrder = 2
    Text = '0'
    Visible = False
  end
  object Button1: TButton
    Left = 20
    Top = 244
    Width = 171
    Height = 25
    Caption = 'K'#228'ynnist'#228' uudelleenl'#228'hetys'
    TabOrder = 3
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 540
    Top = 236
    Width = 75
    Height = 25
    Caption = 'Sulje'
    TabOrder = 4
    OnClick = Button2Click
  end
  object RGTila: TRadioGroup
    Left = 305
    Top = 21
    Width = 177
    Height = 97
    Caption = ' Yhteyden tila '
    Items.Strings = (
      'Vastaanotto ja l'#228'hetys'
      'Vain vastaanotto'
      'Vain l'#228'hetys'
      'Yhteys katkaistu')
    TabOrder = 5
  end
  object Button3: TButton
    Left = 305
    Top = 129
    Width = 177
    Height = 25
    Caption = 'Vaihda yhteyden tila'
    TabOrder = 6
    OnClick = Button3Click
  end
  object Panel1: TPanel
    Left = 499
    Top = 26
    Width = 119
    Height = 193
    TabOrder = 7
    Visible = False
    object Label3: TLabel
      Left = 12
      Top = 10
      Width = 88
      Height = 60
      Caption = 'Anna koneen tunnus, jos k'#228'sky ei kaikille koneille'
      WordWrap = True
    end
    object Button4: TButton
      Left = 12
      Top = 122
      Width = 97
      Height = 61
      Caption = 'L'#228'het'#228' ohjelman sulkuk'#228'sky'
      TabOrder = 0
      WordWrap = True
      OnClick = Button4Click
    end
    object EdtSulku: TEdit
      Left = 12
      Top = 83
      Width = 67
      Height = 23
      TabOrder = 1
      Text = 'Kaikki'
    end
  end
  object CBSarjat: TComboBox
    Left = 21
    Top = 201
    Width = 124
    Height = 23
    TabOrder = 8
    Text = 'CBSarjat'
    Visible = False
  end
  object CBOsuudet: TComboBox
    Left = 162
    Top = 199
    Width = 108
    Height = 23
    TabOrder = 9
    Visible = False
    Items.Strings = (
      'Kaikki osuudet'
      '1. osuus')
  end
  object Button5: TButton
    Left = 352
    Top = 175
    Width = 113
    Height = 48
    Caption = 'L'#228'het'#228' testin k'#228'ynnistyspyynt'#246
    TabOrder = 10
    Visible = False
    WordWrap = True
    OnClick = Button5Click
  end
  object RGRajaus: TRadioGroup
    Left = 208
    Top = 232
    Width = 145
    Height = 121
    Caption = ' Rajaus '
    Items.Strings = (
      'Tuloksen saaneet'
      'V'#228'liajat'
      'Molemmat'
      'Hyl+kesk+avoimet'
      'Osanottajat')
    TabOrder = 11
    Visible = False
  end
  object GBAikavali: TGroupBox
    Left = 361
    Top = 232
    Width = 157
    Height = 121
    Caption = ' Aikav'#228'li '
    TabOrder = 12
    Visible = False
    object Label5: TLabel
      Left = 5
      Top = 24
      Width = 125
      Height = 15
      Caption = 'Alkuhetki (tyhj'#228': alusta)'
    end
    object Label6: TLabel
      Left = 2
      Top = 69
      Width = 148
      Height = 15
      Caption = 'Loppuhetki (tyhj'#228': loppuun)'
    end
    object EdtAlku: TEdit
      Left = 14
      Top = 43
      Width = 107
      Height = 23
      TabOrder = 0
    end
    object EdtLoppu: TEdit
      Left = 14
      Top = 88
      Width = 107
      Height = 23
      TabOrder = 1
    end
  end
end
