object FormLahestyjat: TFormLahestyjat
  Left = 0
  Top = 0
  Caption = 'L'#228'hestyjien seuranta'
  ClientHeight = 336
  ClientWidth = 527
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
    Left = 5
    Top = 8
    Width = 289
    Height = 16
    Caption = 'N'#228'yt'#228' valitun sarjan l'#228'hestyj'#228't valittuun pisteeseen'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 5
    Top = 31
    Width = 31
    Height = 16
    Caption = 'Sarja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 127
    Top = 31
    Width = 35
    Height = 16
    Caption = 'Osuus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 228
    Top = 31
    Width = 27
    Height = 16
    Caption = 'Piste'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object DG1: TDrawGrid
    Left = 0
    Top = 64
    Width = 529
    Height = 273
    DefaultRowHeight = 20
    FixedCols = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    ParentFont = False
    TabOrder = 0
    OnDrawCell = DG1DrawCell
  end
  object CBSarja: TComboBox
    Left = 42
    Top = 29
    Width = 79
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'CBSarja'
    OnClick = CBSarjaClick
  end
  object CBOsuus: TComboBox
    Left = 169
    Top = 29
    Width = 53
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 2
    Text = '1. os.'
    OnChange = CBOsuusChange
    Items.Strings = (
      '1. os.'
      '2. os.'
      '3. os.')
  end
  object CBPiste: TComboBox
    Left = 262
    Top = 29
    Width = 52
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Tahoma'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 3
    Text = 'Maali'
    OnClick = CBPisteClick
    Items.Strings = (
      'Maali'
      '1. va'
      '2. va'
      '3. va')
  end
  object RGFont: TRadioGroup
    Left = 319
    Top = 6
    Width = 90
    Height = 51
    Caption = ' Merkkikoko '
    Columns = 2
    Items.Strings = (
      'Pieni'
      'Keski'
      'Suuri'
      'Max')
    TabOrder = 4
    OnClick = RGFontClick
  end
end
