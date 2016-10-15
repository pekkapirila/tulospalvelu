object FormSuodatus: TFormSuodatus
  Left = 0
  Top = 0
  Caption = 'Joukkuesuodatus'
  ClientHeight = 342
  ClientWidth = 497
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 224
    Top = 152
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
    Visible = False
  end
  object Label4: TLabel
    Left = 224
    Top = 108
    Width = 69
    Height = 13
    Caption = 'Tarkastustieto'
  end
  object Label5: TLabel
    Left = 224
    Top = 195
    Width = 140
    Height = 13
    Caption = 'Osuuskohtaisen ehdon osuus'
  end
  object RGPeruste: TRadioGroup
    Left = 24
    Top = 21
    Width = 161
    Height = 169
    Caption = ' Suodatusperuste '
    ItemIndex = 0
    Items.Strings = (
      'Ei alla lueteltu tieto'
      'Seuranimen alku'
      'Maa'
      'Kilpailijanumero'
      'Osuusnimen alku'
      'Tunnistinkoodi (Emit)'
      'Lisenssinumero'
      'GPS')
    TabOrder = 0
  end
  object EdtTieto: TEdit
    Left = 224
    Top = 169
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
    ItemIndex = 0
    TabOrder = 2
    Text = 'Ei vaatimusta'
    OnChange = CBSarjaChange
    Items.Strings = (
      'Ei vaatimusta')
  end
  object CBSeura: TComboBox
    Left = 224
    Top = 83
    Width = 177
    Height = 21
    ItemIndex = 0
    TabOrder = 3
    Text = 'Ei vaatimusta'
    Visible = False
    OnChange = CBSeuraChange
    Items.Strings = (
      'Ei vaatimusta')
  end
  object BtnSulje: TButton
    Left = 350
    Top = 275
    Width = 67
    Height = 25
    Caption = 'Poistu'
    TabOrder = 4
    OnClick = BtnSuljeClick
  end
  object BtnKayta: TButton
    Left = 224
    Top = 242
    Width = 89
    Height = 25
    Caption = 'Sovella'
    CommandLinkHint = 'orm'
    TabOrder = 5
    OnClick = BtnKaytaClick
  end
  object Button1: TButton
    Left = 224
    Top = 275
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
    ItemIndex = 0
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
      'Poissa')
  end
  object CBOsuus: TComboBox
    Left = 224
    Top = 213
    Width = 121
    Height = 21
    ItemIndex = 0
    TabOrder = 8
    Text = '1. osuus'
    Items.Strings = (
      '1. osuus'
      '2. osuus')
  end
  object CBTyhja: TCheckBox
    Left = 360
    Top = 171
    Width = 97
    Height = 17
    Caption = 'Tyhj'#228' kentt'#228
    TabOrder = 9
  end
  object EdtValittu: TLabeledEdit
    Left = 72
    Top = 277
    Width = 41
    Height = 21
    TabStop = False
    Alignment = taRightJustify
    EditLabel.Width = 33
    EditLabel.Height = 13
    EditLabel.Caption = 'Valittu '
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 10
    Text = '0'
  end
end
