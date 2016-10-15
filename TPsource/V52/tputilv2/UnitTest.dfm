object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 540
  ClientWidth = 704
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 8
    Top = 112
    Width = 688
    Height = 420
    Lines.Strings = (
      'Rivi 1'
      'Rivi 2')
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object RadioGroup1: TRadioGroup
    Left = 24
    Top = 8
    Width = 89
    Height = 89
    Caption = 'Type'
    ItemIndex = 0
    Items.Strings = (
      'Ansi'
      'UTF-8'
      'UTF16LE'
      'None')
    TabOrder = 1
  end
  object Button1: TButton
    Left = 224
    Top = 16
    Width = 75
    Height = 25
    Caption = 'Lue'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 224
    Top = 47
    Width = 75
    Height = 25
    Caption = 'Kirjoita'
    TabOrder = 3
    OnClick = Button2Click
  end
  object RadioGroup2: TRadioGroup
    Left = 128
    Top = 8
    Width = 81
    Height = 89
    Caption = 'Laajuus'
    ItemIndex = 0
    Items.Strings = (
      'Rivit'
      'Kirjaimet'
      'Tavut')
    TabOrder = 4
  end
  object OpenDialog1: TOpenDialog
    Left = 432
    Top = 24
  end
  object SaveDialog1: TSaveDialog
    Left = 480
    Top = 24
  end
end
