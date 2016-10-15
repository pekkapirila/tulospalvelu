object FormMerkKoodeja: TFormMerkKoodeja
  Left = 0
  Top = 0
  Caption = 'Tarkastuskoodien kirjaus'
  ClientHeight = 441
  ClientWidth = 612
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 14
    Top = 11
    Width = 250
    Height = 15
    Caption = 'Valitse kilpailijat sy'#246'tt'#228'm'#228'll'#228' numero ja osuus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object SG1: TStringGrid
    Left = 13
    Top = 29
    Width = 433
    Height = 404
    DefaultRowHeight = 20
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 0
    OnKeyDown = SG1KeyDown
  end
  object RGTark: TRadioGroup
    Left = 461
    Top = 25
    Width = 127
    Height = 170
    Caption = ' Meritt'#228'v'#228' koodi'
    Items.Strings = (
      'Ei l'#228'htenyt'
      'Avoin'
      'Keskeytt'#228'nyt'
      'Hyl'#228'tty'
      'Tarkastettu'
      'Poissa'
      'Hylk'#228'ysesitys'
      'Merkitse GPS'
      'Poista GPS')
    TabOrder = 1
  end
  object Button1: TButton
    Left = 461
    Top = 301
    Width = 127
    Height = 49
    Caption = 'Toteuta merkinn'#228't'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 461
    Top = 356
    Width = 127
    Height = 33
    Caption = 'Tyhjenn'#228' taulukko'
    TabOrder = 3
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 461
    Top = 397
    Width = 127
    Height = 33
    Caption = 'Sulje kaavake'
    TabOrder = 4
    TabStop = False
    OnClick = Button3Click
  end
  object CBJoukkEil: TCheckBox
    Left = 464
    Top = 202
    Width = 124
    Height = 47
    Caption = 'Laajenna 1. osuuden "Ei l'#228'htenyt" kaikille osuuksille'
    Checked = True
    State = cbChecked
    TabOrder = 5
    WordWrap = True
  end
  object CBLoppuEil: TCheckBox
    Left = 464
    Top = 248
    Width = 140
    Height = 47
    Caption = 
      'Merkitse hylk'#228'yksen ja keskeytt'#228'misen j'#228'lkeiset osuudet ei-l'#228'hte' +
      'neiksi '
    TabOrder = 6
    WordWrap = True
  end
end
