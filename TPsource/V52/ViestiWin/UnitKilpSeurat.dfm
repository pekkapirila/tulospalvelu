object FormKilpSeurat: TFormKilpSeurat
  Left = 0
  Top = 0
  Caption = 'Osanottajien seuratiedot'
  ClientHeight = 487
  ClientWidth = 750
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 3
    Height = 14
  end
  object SG1: TStringGrid
    Left = 4
    Top = 8
    Width = 741
    Height = 474
    DefaultRowHeight = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving]
    TabOrder = 0
    OnMouseWheelDown = SG1MouseWheelDown
    OnMouseWheelUp = SG1MouseWheelUp
  end
  object MainMenu1: TMainMenu
    Left = 456
    Top = 65528
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Kirjoitaseurat: TMenuItem
        Caption = 'Kirjoita tiedostoon s&eurat'
        OnClick = KirjoitaseuratClick
      end
      object Sisosotluku: TMenuItem
        Caption = 'Sis'#228'llyt'#228' tiedostoon &osanottajaluku'
        OnClick = SisosotlukuClick
      end
      object Kirjoitakaikkitaulukontiedot1: TMenuItem
        Caption = 'Kirjoita kaikki &taulukon tiedot'
        OnClick = Kirjoitakaikkitaulukontiedot1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Left = 512
    Top = 8
  end
end
