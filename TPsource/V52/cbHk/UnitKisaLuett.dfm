object FormKisaLuett: TFormKisaLuett
  Left = 0
  Top = 0
  Caption = 'Kilpailuluettelo'
  ClientHeight = 402
  ClientWidth = 629
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SG1: TStringGrid
    Left = 8
    Top = 115
    Width = 613
    Height = 279
    DefaultRowHeight = 20
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goDrawFocusSelected, goColSizing, goEditing, goTabs]
    TabOrder = 0
    OnSelectCell = SG1SelectCell
  end
  object Button1: TButton
    Left = 454
    Top = 59
    Width = 54
    Height = 50
    Caption = 'Poista valittu rivi'
    TabOrder = 1
    Visible = False
    WordWrap = True
  end
  object Button2: TButton
    Left = 543
    Top = 59
    Width = 78
    Height = 50
    Caption = 'Tallenna tiedot ja poistu'
    TabOrder = 2
    WordWrap = True
    OnClick = Button2Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 2
    Width = 355
    Height = 107
    Caption = ' Kansioiden luonti ja tietojen kopiointi runkokansiosta '
    TabOrder = 3
    object Label1: TLabel
      Left = 8
      Top = 22
      Width = 60
      Height = 13
      Caption = 'Runkokansio'
    end
    object EdtRunko: TEdit
      Left = 76
      Top = 19
      Width = 162
      Height = 21
      TabOrder = 0
    end
    object BtnRunko: TButton
      Left = 257
      Top = 19
      Width = 75
      Height = 21
      Caption = 'Valitse kansio'
      TabOrder = 1
      OnClick = BtnRunkoClick
    end
    object CBLuo: TCheckBox
      Left = 13
      Top = 46
      Width = 136
      Height = 17
      Caption = 'Luo puuttuvat kansiot'
      TabOrder = 2
    end
    object CBSarjat: TCheckBox
      Left = 174
      Top = 46
      Width = 136
      Height = 17
      Caption = 'Kopioi sarjatiedot'
      TabOrder = 3
    end
    object CBData: TCheckBox
      Left = 174
      Top = 63
      Width = 136
      Height = 17
      Caption = 'Kopioi osanottajatiedot'
      TabOrder = 4
    end
    object CBKonfig: TCheckBox
      Left = 174
      Top = 81
      Width = 136
      Height = 17
      Caption = 'Kopioi konfiguraatio'
      TabOrder = 5
    end
    object BtnToteuta: TButton
      Left = 13
      Top = 72
      Width = 128
      Height = 25
      Caption = 'Toteuta valitut toimet'
      TabOrder = 6
      OnClick = BtnToteutaClick
    end
  end
  object Button3: TButton
    Left = 390
    Top = 59
    Width = 54
    Height = 50
    Caption = 'Lis'#228#228' t'#228'm'#228' kilpailu'
    TabOrder = 4
    Visible = False
    WordWrap = True
  end
  object MainMenu1: TMainMenu
    Left = 468
    Top = 15
    object Tiedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Luekilpailuluettelo1: TMenuItem
        Caption = '&Lue kilpailuluettelo'
        OnClick = Luekilpailuluettelo1Click
      end
      object allenna1: TMenuItem
        Caption = '&Tallenna ja poistu'
      end
      object Luecsv1: TMenuItem
        Caption = 'Lue &csv-tiedosto'
        Visible = False
      end
      object Kirjoitacsv1: TMenuItem
        Caption = '&Kirjoita csv-tiedosto'
        Visible = False
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 392
    Top = 8
  end
  object SaveDialog1: TSaveDialog
    Left = 504
    Top = 16
  end
end
