object FormYhtveto: TFormYhtveto
  Left = 264
  Top = 275
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'Yhteenveto'
  ClientHeight = 643
  ClientWidth = 698
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnActivate = FormActivate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object YvGrid: TStringGrid
    Left = -1
    Top = -1
    Width = 699
    Height = 600
    DefaultRowHeight = 16
    FixedCols = 0
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
    OnClick = YvGridClick
  end
  object BtnSulje: TButton
    Left = 324
    Top = 608
    Width = 80
    Height = 27
    Caption = '&Sulje'
    TabOrder = 1
    OnClick = BtnSuljeClick
  end
  object BtnPaivita: TButton
    Left = 45
    Top = 608
    Width = 81
    Height = 27
    Caption = '&P'#228'ivit'#228
    TabOrder = 2
    OnClick = BtnPaivitaClick
  end
  object MainMenu1: TMainMenu
    Left = 48
    object Tiedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Kirjoitatiedostoon1: TMenuItem
        Caption = '&Kirjoita tiedostoon'
        OnClick = Kirjoitatiedostoon1Click
      end
      object Teerastivlianalyysi1: TMenuItem
        Caption = 'Tee rastiv'#228'liaika-analyysi'
        Visible = False
        OnClick = Teerastivlianalyysi1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    Filter = 'Tekstitiedosto|*.txt|Kaikki|*.*'
    Left = 100
  end
end
