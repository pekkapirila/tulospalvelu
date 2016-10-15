object FormValitseKilp: TFormValitseKilp
  Left = 0
  Top = 0
  Caption = 'Valitse kilpailu'
  ClientHeight = 137
  ClientWidth = 515
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object CBKilpailu: TComboBox
    Left = 8
    Top = 16
    Width = 309
    Height = 21
    ItemIndex = 0
    TabOrder = 3
    Text = 'Ei valittu'
    Items.Strings = (
      'Ei valittu')
  end
  object BtnValittu: TButton
    Left = 340
    Top = 14
    Width = 161
    Height = 25
    Caption = 'Siirry oheiseen kilpailuun'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnClick = BtnValittuClick
  end
  object BtnMuu: TButton
    Left = 340
    Top = 56
    Width = 161
    Height = 25
    Caption = 'Hae muu kilpailu'
    TabOrder = 1
    OnClick = BtnMuuClick
  end
  object BtnPeruuta: TButton
    Left = 340
    Top = 98
    Width = 161
    Height = 25
    Caption = 'Peruuta kilpailun avaaminen'
    TabOrder = 2
    OnClick = BtnPeruutaClick
  end
end
