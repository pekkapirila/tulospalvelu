object FormTietoTulk: TFormTietoTulk
  Left = 0
  Top = 0
  Caption = 'Tiedoston tulkinta'
  ClientHeight = 436
  ClientWidth = 631
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 633
    Height = 175
    TabOrder = 0
  end
  object SG1: TStringGrid
    Left = 0
    Top = 233
    Width = 633
    Height = 203
    ColCount = 9
    DefaultRowHeight = 18
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 174
    Width = 633
    Height = 60
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 2
    object LblData: TLabel
      Left = 467
      Top = 35
      Width = 120
      Height = 13
      Caption = 'Siirret'#228'v'#228'n tiedon sarake'
    end
    object Label3: TLabel
      Left = 8
      Top = 11
      Width = 60
      Height = 13
      Caption = 'Kentt'#228'erotin'
    end
    object Label1: TLabel
      Left = 212
      Top = 11
      Width = 50
      Height = 13
      Caption = 'Sarakkeita'
    end
    object LblTunn: TLabel
      Left = 467
      Top = 11
      Width = 106
      Height = 13
      Caption = 'Tunnistetiedon sarake'
    end
    object CBErotin: TComboBox
      Left = 8
      Top = 25
      Width = 190
      Height = 21
      ItemIndex = 2
      TabOrder = 0
      Text = 'Puolipiste'
      Items.Strings = (
        'V'#228'lily'#246'ntej'#228' tai tabulaattori'
        'Tabulaattori'
        'Puolipiste'
        'Pilkku')
    end
    object EdtCols: TEdit
      Left = 212
      Top = 25
      Width = 47
      Height = 21
      Alignment = taRightJustify
      TabOrder = 1
      Text = '8'
    end
    object CBOtsRivi: TCheckBox
      Left = 275
      Top = 7
      Width = 115
      Height = 17
      Caption = 'Tiedoissa otsikkorivi'
      TabOrder = 2
    end
    object Button1: TButton
      Left = 396
      Top = 18
      Width = 64
      Height = 25
      Caption = 'Tulkitse'
      TabOrder = 3
      OnClick = Button1Click
    end
    object EdtData: TEdit
      Left = 594
      Top = 32
      Width = 34
      Height = 21
      Alignment = taRightJustify
      TabOrder = 4
      Text = '2'
      OnExit = EdtDataExit
    end
    object EdtTunn: TEdit
      Left = 594
      Top = 8
      Width = 34
      Height = 21
      Alignment = taRightJustify
      TabOrder = 5
      Text = '1'
      OnExit = EdtTunnExit
    end
    object CBLainMerk: TCheckBox
      Left = 275
      Top = 27
      Width = 102
      Height = 29
      Caption = 'Lainausmerkit rajaavat kentti'#228
      TabOrder = 6
      WordWrap = True
    end
  end
  object MainMenu1: TMainMenu
    Left = 480
    Top = 24
    object iedosto1: TMenuItem
      Caption = 'Tiedosto'
      object Luetiedottiedostosta1: TMenuItem
        Caption = 'Lue tiedot tiedostosta'
        OnClick = Luetiedottiedostosta1Click
      end
      object yhjenntekstialue1: TMenuItem
        Caption = 'Tyhjenn'#228' tekstialue'
        OnClick = yhjenntekstialue1Click
      end
      object Liittiedotleikepydlt1: TMenuItem
        Caption = 'Liit'#228' tiedot leikep'#246'yd'#228'lt'#228
        Enabled = False
        Visible = False
        OnClick = Liittiedotleikepydlt1Click
      end
      object Sulje1: TMenuItem
        Caption = 'Sulje'
        OnClick = Sulje1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 400
    Top = 40
  end
end
