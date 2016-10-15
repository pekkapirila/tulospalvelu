object FormSarjaVal: TFormSarjaVal
  Left = 0
  Top = 0
  Caption = 'Valitse sarja'
  ClientHeight = 249
  ClientWidth = 314
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 13
    Width = 25
    Height = 13
    Caption = 'Sarja'
  end
  object CBSarja: TComboBox
    Left = 16
    Top = 32
    Width = 145
    Height = 21
    TabOrder = 0
    Text = 'CBSarja'
  end
  object BtnPeruuta: TButton
    Left = 200
    Top = 144
    Width = 75
    Height = 52
    Caption = 'Peruuta'
    TabOrder = 2
    OnClick = BtnPeruutaClick
  end
  object BtnHyvaksy: TButton
    Left = 200
    Top = 56
    Width = 75
    Height = 51
    Caption = 'Hyv'#228'ksy'
    TabOrder = 1
    OnClick = BtnHyvaksyClick
  end
end
