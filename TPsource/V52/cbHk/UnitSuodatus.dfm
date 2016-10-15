object FormSuodatus: TFormSuodatus
  Left = 0
  Top = 0
  Caption = 'Osanottajasuodatus'
  ClientHeight = 337
  ClientWidth = 438
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 224
    Top = 154
    Width = 45
    Height = 13
    Caption = 'Muu tieto'
  end
  object Label2: TLabel
    Left = 224
    Top = 21
    Width = 25
    Height = 13
    Caption = 'Sarja'
  end
  object Label3: TLabel
    Left = 224
    Top = 65
    Width = 28
    Height = 13
    Caption = 'Seura'
  end
  object Label4: TLabel
    Left = 224
    Top = 108
    Width = 69
    Height = 13
    Caption = 'Tarkastustieto'
  end
  object Label5: TLabel
    Left = 24
    Top = 268
    Width = 59
    Height = 13
    Caption = 'Ehdon vaihe'
  end
  object RGPeruste: TRadioGroup
    Left = 24
    Top = 12
    Width = 161
    Height = 250
    Caption = ' Suodatusperuste '
    ItemIndex = 0
    Items.Strings = (
      'Ei seuraavia'
      'Nimi'
      'Yhdistys'
      'Joukkue'
      'Maa'
      'Kilpailijanumero'
      'Rintanumero'
      'Tunnistinkoodi (Emit)'
      'Id-numero (lisenssi)'
      'Kv. id-numero'
      'Qual'
      'GPS'
      'Tulos (yl'#228'raja)')
    TabOrder = 0
  end
  object EdtTieto: TEdit
    Left = 224
    Top = 173
    Width = 121
    Height = 21
    TabOrder = 1
    OnChange = EdtTietoChange
    OnKeyPress = EdtTietoKeyPress
  end
  object CBSarja: TComboBox
    Left = 224
    Top = 38
    Width = 177
    Height = 21
    TabOrder = 2
    Text = 'Ei vaatimusta'
    OnChange = CBSarjaChange
  end
  object CBSeura: TComboBox
    Left = 224
    Top = 83
    Width = 177
    Height = 21
    TabOrder = 3
    Text = 'Ei vaatimusta'
    OnChange = CBSeuraChange
  end
  object BtnSulje: TButton
    Left = 350
    Top = 235
    Width = 67
    Height = 25
    Caption = 'Poistu'
    TabOrder = 4
    OnClick = BtnSuljeClick
  end
  object BtnKayta: TButton
    Left = 224
    Top = 202
    Width = 89
    Height = 25
    Caption = 'Sovella'
    TabOrder = 5
    OnClick = BtnKaytaClick
  end
  object Button1: TButton
    Left = 224
    Top = 235
    Width = 89
    Height = 25
    Caption = 'Poista ehdot'
    TabOrder = 6
    OnClick = Button1Click
  end
  object CBTark: TComboBox
    Left = 224
    Top = 126
    Width = 177
    Height = 21
    TabOrder = 7
    Text = 'Ei vaatimusta'
    OnChange = CBTarkChange
    Items.Strings = (
      'Ei vaatimusta'
      'Avoinna'
      'Tarkastettu'
      'Hyl'#228'tty'
      'Keskeytt'#228'nyt'
      'Hylk'#228'ysesitys'
      'Ei-l'#228'htenyt'
      'Vakantti'
      'Poissa'
      'Virhe'
      'Ei aikaa'
      'Havaittu')
  end
  object CBVaihe: TComboBox
    Left = 24
    Top = 285
    Width = 97
    Height = 21
    ItemIndex = 0
    TabOrder = 8
    Text = '1. vaihe'
    Items.Strings = (
      '1. vaihe')
  end
end
