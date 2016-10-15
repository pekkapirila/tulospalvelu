object FormSakkoKierr: TFormSakkoKierr
  Left = 0
  Top = 0
  Caption = 'Sakkokierrosten seuranta'
  ClientHeight = 336
  ClientWidth = 522
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
    Top = 13
    Width = 85
    Height = 13
    Caption = 'Kirjatut kierrokset'
  end
  object Label2: TLabel
    Left = 272
    Top = 13
    Width = 134
    Height = 13
    Caption = 'Suorittamattomat kierrokset'
  end
  object Label3: TLabel
    Left = 8
    Top = 290
    Width = 82
    Height = 13
    Caption = 'Kilpailijan numero'
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
    TabOrder = 1
    OnClick = BtnPeruutaClick
  end
  object SGAvoimet: TStringGrid
    Left = 259
    Top = 32
    Width = 251
    Height = 296
    TabStop = False
    ColCount = 3
    DefaultRowHeight = 18
    FixedCols = 0
    FixedRows = 0
    TabOrder = 2
  end
  object LBKirjatut: TListBox
    Left = 8
    Top = 32
    Width = 242
    Height = 252
    ItemHeight = 13
    TabOrder = 3
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
