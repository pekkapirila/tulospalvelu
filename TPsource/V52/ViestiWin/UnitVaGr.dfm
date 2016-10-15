object FormVaGr: TFormVaGr
  Left = 203
  Top = 229
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'V'#228'liaikagrafiikka'
  ClientHeight = 713
  ClientWidth = 928
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 13
    Top = 698
    Width = 54
    Height = 14
    Caption = 'Listan koko'
  end
  object Label5: TLabel
    Left = 116
    Top = 22
    Width = 30
    Height = 17
    Caption = 'Viiva'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object JoukkList: TListBox
    Left = 9
    Top = 336
    Width = 164
    Height = 350
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ItemHeight = 15
    MultiSelect = True
    ParentFont = False
    TabOrder = 0
    OnClick = PiirraBtnClick
  end
  object PiirraBtn: TButton
    Left = 9
    Top = 181
    Width = 160
    Height = 27
    Caption = 'P'#228'ivit'#228' kuva'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = PiirraBtnClick
  end
  object OsVal: TComboBox
    Left = 9
    Top = 43
    Width = 83
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 1
    ParentFont = False
    TabOrder = 2
    Text = '1. osuus'
    OnChange = OsValChange
    Items.Strings = (
      'Numerot'
      '1. osuus'
      '2. osuus'
      '3. osuus'
      '4. osuus'
      '5. osuus'
      '6. osuus'
      '7. osuus')
  end
  object JarjVal: TRadioGroup
    Left = 9
    Top = 220
    Width = 160
    Height = 70
    Caption = ' J'#228'rjestys '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 1
    Items.Strings = (
      'Vaihtoj'#228'rjestys'
      'Osuusj'#228'rjestys')
    ParentFont = False
    TabOrder = 3
    OnClick = PaivitaClick
  end
  object PaivitaButton: TButton
    Left = 9
    Top = 297
    Width = 160
    Height = 27
    Caption = 'P'#228'ivit'#228' lista'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = PaivitaClick
  end
  object EdMaxLkm: TEdit
    Left = 103
    Top = 694
    Width = 57
    Height = 22
    TabOrder = 5
    Text = '1000'
  end
  object TietoVal: TRadioGroup
    Left = 4
    Top = 78
    Width = 165
    Height = 91
    Caption = ' Tiedot '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    Items.Strings = (
      'Suhteelliset ajat'
      'Korjaamattomat ajat'
      'Aikaerot maastossa')
    ParentFont = False
    TabOrder = 6
    OnClick = PiirraBtnClick
  end
  object ViivaVal: TComboBox
    Left = 116
    Top = 43
    Width = 49
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 7
    Text = '1'
    OnChange = ViivaValChange
    Items.Strings = (
      '1'
      '2'
      '3')
  end
  object SarjaVal: TComboBox
    Left = 9
    Top = 9
    Width = 95
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 8
    OnChange = SarjaValChange
    Items.Strings = (
      '')
  end
end
