object FormStatus: TFormStatus
  Left = 0
  Top = 0
  Caption = 'Yleistilanne'
  ClientHeight = 514
  ClientWidth = 758
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object LkmGrid: TStringGrid
    Left = -1
    Top = -1
    Width = 758
    Height = 367
    ColCount = 13
    DefaultColWidth = 32
    DefaultRowHeight = 20
    RowCount = 9
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
  end
  object YhteysGrid: TStringGrid
    Left = 0
    Top = 379
    Width = 441
    Height = 129
    ColCount = 6
    DefaultColWidth = 40
    DefaultRowHeight = 18
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goTabs]
    TabOrder = 1
    ColWidths = (
      40
      193
      40
      28
      48
      50)
  end
  object GBAutotulostus: TGroupBox
    Left = 455
    Top = 378
    Width = 152
    Height = 129
    Caption = ' Autotulostus '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Visible = False
    object Label1: TLabel
      Left = 16
      Top = 26
      Width = 45
      Height = 15
      Caption = 'Laajuus'
    end
    object Label2: TLabel
      Left = 16
      Top = 49
      Width = 39
      Height = 15
      Caption = 'Kynnys'
    end
    object Label3: TLabel
      Left = 16
      Top = 72
      Width = 42
      Height = 15
      Caption = 'Korkein'
    end
    object Label4: TLabel
      Left = 16
      Top = 95
      Width = 57
      Height = 15
      Caption = 'Aika-askel'
    end
    object EdtLaajuus: TEdit
      Left = 88
      Top = 23
      Width = 45
      Height = 23
      Alignment = taRightJustify
      ReadOnly = True
      TabOrder = 0
    end
    object EdtKynnys: TEdit
      Left = 88
      Top = 46
      Width = 45
      Height = 23
      TabStop = False
      Alignment = taRightJustify
      ReadOnly = True
      TabOrder = 1
      Text = '0'
    end
    object EdtKorkein: TEdit
      Left = 88
      Top = 69
      Width = 45
      Height = 23
      TabStop = False
      Alignment = taRightJustify
      ReadOnly = True
      TabOrder = 2
      Text = '0'
    end
    object EdtAikaaskel: TEdit
      Left = 88
      Top = 92
      Width = 45
      Height = 23
      TabStop = False
      Alignment = taRightJustify
      ReadOnly = True
      TabOrder = 3
      Text = '0'
    end
  end
  object MainMenu1: TMainMenu
    Left = 672
    Top = 424
    object Nyt1: TMenuItem
      Caption = 'N'#228'yt'#228
      object Yhteislht1: TMenuItem
        Caption = '&Yhteisl'#228'ht'#246
        OnClick = Yhteislht1Click
      end
      object Hylkysesitykset1: TMenuItem
        Caption = '&Hylk'#228'ysesitykset'
        OnClick = Hylkysesitykset1Click
      end
      object Emittietueet1: TMenuItem
        Caption = '&Emittietueet'
        Visible = False
        OnClick = Emittietueet1Click
      end
      object Emitkoodeja1: TMenuItem
        Caption = 'Emit&koodeja'
        OnClick = Emitkoodeja1Click
      end
      object Yhteydeterikaavakkeella1: TMenuItem
        Caption = 'Yhteydet eri kaavakkeella'
        OnClick = Yhteydeterikaavakkeella1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      OnClick = Help1Click
    end
  end
end
