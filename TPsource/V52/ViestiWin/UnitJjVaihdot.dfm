object FormJjVaihdot: TFormJjVaihdot
  Left = 0
  Top = 0
  Caption = 'Juoksuj'#228'rjestysvaihdot'
  ClientHeight = 404
  ClientWidth = 458
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 281
    Top = 234
    Width = 158
    Height = 42
    Caption = 
      'Kun kohdeosuus on 0, ei nimi'#228' siirret'#228'. T'#228'ten voidaan taulukkoa ' +
      'k'#228'ytt'#228#228' my'#246's kopiointiin.'
    WordWrap = True
  end
  object SG1: TStringGrid
    Left = 8
    Top = 8
    Width = 249
    Height = 388
    ColCount = 2
    DefaultColWidth = 50
    DefaultRowHeight = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 0
  end
  object BtnEtuSuku: TButton
    Left = 281
    Top = 10
    Width = 137
    Height = 43
    Caption = 'Vaihda etu- ja sukunimet'
    TabOrder = 1
    WordWrap = True
    OnClick = BtnEtuSukuClick
  end
  object BtnToteuta: TButton
    Left = 281
    Top = 169
    Width = 137
    Height = 57
    Caption = 'Toteuta oheisen taulukon vaihdot tai kopioinnit'
    TabOrder = 2
    WordWrap = True
    OnClick = BtnToteutaClick
  end
  object CBEmitkoodit: TCheckBox
    Left = 281
    Top = 306
    Width = 137
    Height = 17
    Caption = 'Siirr'#228' samalla Emit-koodit'
    TabOrder = 3
  end
  object BtnJaaNimi: TButton
    Left = 281
    Top = 61
    Width = 137
    Height = 45
    Caption = 'Jaa nimet etu- ja sukunimiin'
    TabOrder = 4
    WordWrap = True
    OnClick = BtnJaaNimiClick
  end
  object BtnPeruuta: TButton
    Left = 281
    Top = 350
    Width = 137
    Height = 45
    Caption = 'Sulje kaavake tekem'#228'tt'#228' muutoksia'
    TabOrder = 5
    WordWrap = True
    OnClick = BtnPeruutaClick
  end
  object CBHajonnat: TCheckBox
    Left = 281
    Top = 327
    Width = 158
    Height = 17
    Caption = 'Siirr'#228' samalla ratatunnukset'
    TabOrder = 6
  end
  object CBLisenssi: TCheckBox
    Left = 281
    Top = 284
    Width = 169
    Height = 17
    Caption = 'Siirr'#228' samalla lisenssinumerot'
    Checked = True
    State = cbChecked
    TabOrder = 7
  end
  object BtnIsoalku: TButton
    Left = 281
    Top = 114
    Width = 137
    Height = 47
    Caption = 'Nimet isoin alkukirjaimin'
    TabOrder = 8
    WordWrap = True
    OnClick = BtnIsoalkuClick
  end
end
