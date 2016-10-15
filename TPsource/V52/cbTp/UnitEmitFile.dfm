object FormEmitFile: TFormEmitFile
  Left = 0
  Top = 0
  Caption = 'Emit-tiedot tiedostosta'
  ClientHeight = 709
  ClientWidth = 933
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 18
    Top = 96
    Width = 49
    Height = 39
    Caption = 'Ensimm. siirrett'#228'v'#228' rivi'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 18
    Top = 170
    Width = 49
    Height = 39
    Caption = 'Viimeinen siirrett'#228'v'#228' rivi'
    WordWrap = True
  end
  object Button1: TButton
    Left = 6
    Top = 40
    Width = 71
    Height = 41
    Caption = 'Avaa tiedosto'
    TabOrder = 0
    WordWrap = True
    OnClick = Button1Click
  end
  object SG1: TStringGrid
    Left = 85
    Top = 0
    Width = 848
    Height = 709
    Align = alRight
    ColCount = 107
    DefaultColWidth = 40
    DefaultRowHeight = 20
    FixedCols = 4
    TabOrder = 1
  end
  object Edit1: TEdit
    Left = 10
    Top = 141
    Width = 69
    Height = 21
    Alignment = taRightJustify
    TabOrder = 2
    Text = '0'
  end
  object Edit2: TEdit
    Left = 10
    Top = 215
    Width = 69
    Height = 21
    Alignment = taRightJustify
    TabOrder = 3
    Text = '9999'
  end
  object Button2: TButton
    Left = 12
    Top = 362
    Width = 67
    Height = 42
    Caption = 'Toteuta siirto'
    TabOrder = 4
    WordWrap = True
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 12
    Top = 465
    Width = 64
    Height = 25
    Caption = 'Sulje '
    TabOrder = 5
    OnClick = Button3Click
  end
  object RGEmit: TRadioGroup
    Left = 8
    Top = 249
    Width = 71
    Height = 88
    Caption = ' Emit-koodi '
    ItemIndex = 0
    Items.Strings = (
      'K'#228'yt'#228' OrigBdg'
      'K'#228'yt'#228' Badge')
    TabOrder = 6
    WordWrap = True
  end
  object OpenDialog1: TOpenDialog
    Left = 16
    Top = 8
  end
end
