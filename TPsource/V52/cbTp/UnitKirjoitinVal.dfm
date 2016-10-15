object FormKirjVal: TFormKirjVal
  Left = 0
  Top = 0
  Caption = 'Valitse kirjoitin'
  ClientHeight = 102
  ClientWidth = 420
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object KirjoitinVal: TComboBox
    Left = 11
    Top = 21
    Width = 398
    Height = 22
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'Valitse kirjoitin'
  end
  object Button1: TButton
    Left = 323
    Top = 58
    Width = 89
    Height = 25
    Caption = 'Sulje'
    TabOrder = 1
    OnClick = Button1Click
  end
end
