object FormSakkoKierr: TFormSakkoKierr
  Left = 0
  Top = 0
  Caption = 'Sakkokierrosten seuranta'
  ClientHeight = 336
  ClientWidth = 560
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 7
    Width = 85
    Height = 13
    Caption = 'Kirjatut kierrokset'
  end
  object Label2: TLabel
    Left = 272
    Top = 7
    Width = 134
    Height = 13
    Caption = 'Suorittamattomat kierrokset'
  end
  object Label3: TLabel
    Left = 8
    Top = 290
    Width = 62
    Height = 13
    Caption = 'Kilpailijan nro'
  end
  object Label4: TLabel
    Left = 79
    Top = 290
    Width = 30
    Height = 13
    Caption = 'Osuus'
  end
  object EdtKirjaus: TEdit
    Left = 8
    Top = 307
    Width = 65
    Height = 21
    Alignment = taRightJustify
    TabOrder = 0
    OnKeyPress = EdtKirjausKeyPress
  end
  object BtnPeruuta: TButton
    Left = 121
    Top = 305
    Width = 129
    Height = 25
    Caption = 'Peruuta valittu kirjaus'
    TabOrder = 2
    OnClick = BtnPeruutaClick
  end
  object SGAvoimet: TStringGrid
    Left = 259
    Top = 25
    Width = 293
    Height = 304
    TabStop = False
    ColCount = 4
    DefaultRowHeight = 18
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 4
  end
  object LBKirjatut: TListBox
    Left = 8
    Top = 25
    Width = 242
    Height = 262
    ItemHeight = 13
    TabOrder = 3
  end
  object EdtOsuus: TEdit
    Left = 79
    Top = 307
    Width = 34
    Height = 21
    Alignment = taRightJustify
    TabOrder = 1
    OnKeyPress = EdtOsuusKeyPress
  end
  object MainMenu1: TMainMenu
    Left = 224
    Top = 24
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Poistakaikkimerkinnt1: TMenuItem
        Caption = '&Poista kaikki merkinn'#228't'
        OnClick = Poistakaikkimerkinnt1Click
      end
      object Vaihdatiedosto1: TMenuItem
        Caption = '&Vaihda tiedosto'
        Visible = False
        OnClick = Vaihdatiedosto1Click
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
  end
end
