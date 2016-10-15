object FormHenkHaku: TFormHenkHaku
  Left = 0
  Top = 0
  Caption = 'Henkil'#246'haku'
  ClientHeight = 411
  ClientWidth = 661
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 89
    Width = 91
    Height = 13
    Caption = 'Haettavat osuudet'
  end
  object Label2: TLabel
    Left = 16
    Top = 150
    Width = 51
    Height = 13
    Caption = 'Nimen alku'
  end
  object RGJarj: TRadioGroup
    Left = 14
    Top = 14
    Width = 137
    Height = 65
    Caption = ' Hakuj'#228'rjestys '
    ItemIndex = 1
    Items.Strings = (
      'Etunimi Sukunimi'
      'Sukunimi Etunimi')
    TabOrder = 0
    OnClick = RGJarjClick
  end
  object CBOsuudet: TComboBox
    Left = 14
    Top = 110
    Width = 129
    Height = 21
    TabOrder = 1
    Text = 'Kaikki osuudet'
    OnChange = CBOsuudetChange
    Items.Strings = (
      'Kaikki osuudet'
      '1. osuus'
      '2. osuus')
  end
  object Edit1: TEdit
    Left = 14
    Top = 169
    Width = 127
    Height = 21
    TabOrder = 2
    OnChange = Edit1Change
  end
  object SG1: TStringGrid
    Left = 172
    Top = 13
    Width = 481
    Height = 388
    ColCount = 3
    DefaultRowHeight = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect, goFixedColClick]
    TabOrder = 3
    OnDblClick = SG1DblClick
  end
  object Button1: TButton
    Left = 16
    Top = 224
    Width = 125
    Height = 50
    Caption = 'N'#228'yt'#228' valittu joukkue  tai osuus'
    TabOrder = 4
    WordWrap = True
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 16
    Top = 280
    Width = 125
    Height = 33
    Caption = 'Poistu valitsematta'
    TabOrder = 5
    OnClick = Button2Click
  end
end
