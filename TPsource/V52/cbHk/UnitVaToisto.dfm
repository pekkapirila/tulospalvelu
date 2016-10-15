object FormVaToisto: TFormVaToisto
  Left = 0
  Top = 0
  Caption = 'V'#228'liajat - toistuvat'
  ClientHeight = 290
  ClientWidth = 414
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 19
    Width = 25
    Height = 13
    Caption = 'Sarja'
  end
  object Label2: TLabel
    Left = 32
    Top = 49
    Width = 142
    Height = 13
    Caption = 'V'#228'liaikapisteiden v'#228'li metrein'#228
  end
  object Label3: TLabel
    Left = 32
    Top = 76
    Width = 185
    Height = 13
    Caption = 'Ensimm'#228'inen annettava piste metrein'#228
  end
  object Label4: TLabel
    Left = 32
    Top = 103
    Width = 179
    Height = 13
    Caption = 'Ensimm'#228'isen pisteen j'#228'rjestysnumero'
  end
  object Label5: TLabel
    Left = 32
    Top = 130
    Width = 157
    Height = 13
    Caption = 'Annettavien pisteiden lukum'#228#228'r'#228
  end
  object Label6: TLabel
    Left = 32
    Top = 157
    Width = 140
    Height = 13
    Caption = 'J'#228'rjestysnumeroiden askellus'
  end
  object Label7: TLabel
    Left = 32
    Top = 188
    Width = 107
    Height = 13
    Caption = 'Pisteiden julkisuusaste'
  end
  object Label8: TLabel
    Left = 32
    Top = 216
    Width = 63
    Height = 13
    Caption = 'Merkint'#228'tapa'
  end
  object EdtSarja: TEdit
    Left = 72
    Top = 16
    Width = 121
    Height = 21
    ReadOnly = True
    TabOrder = 0
    Text = 'EdtSarja'
  end
  object EdtVali: TEdit
    Left = 231
    Top = 46
    Width = 40
    Height = 21
    Alignment = taRightJustify
    TabOrder = 1
    Text = '0'
  end
  object EdtMtk1: TEdit
    Left = 231
    Top = 73
    Width = 40
    Height = 21
    Alignment = taRightJustify
    TabOrder = 2
    Text = '0'
  end
  object EdtEnspiste: TEdit
    Left = 231
    Top = 100
    Width = 40
    Height = 21
    Alignment = taRightJustify
    TabOrder = 3
    Text = '1'
  end
  object EdtLkm: TEdit
    Left = 231
    Top = 127
    Width = 40
    Height = 21
    Alignment = taRightJustify
    TabOrder = 4
    Text = '0'
  end
  object EdtAskel: TEdit
    Left = 231
    Top = 154
    Width = 40
    Height = 21
    Alignment = taRightJustify
    TabOrder = 5
    Text = '1'
  end
  object CBJulkisuus: TComboBox
    Left = 186
    Top = 184
    Width = 85
    Height = 21
    ItemIndex = 4
    TabOrder = 6
    Text = 'Julkinen'
    Items.Strings = (
      'Piilota'
      'Taso 1'
      'Taso 2'
      'Taso 3'
      'Julkinen')
  end
  object Button1: TButton
    Left = 304
    Top = 202
    Width = 75
    Height = 25
    Caption = 'Merkitse'
    TabOrder = 7
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 304
    Top = 253
    Width = 75
    Height = 25
    Caption = 'Peruuta'
    TabOrder = 8
    OnClick = Button2Click
  end
  object CBtapa: TComboBox
    Left = 186
    Top = 212
    Width = 85
    Height = 21
    ItemIndex = 1
    TabOrder = 9
    Text = 'km, 1 desim.'
    Items.Strings = (
      'km, ei desim.'
      'km, 1 desim.'
      'km, 2 desim.'
      'metrein'#228)
  end
  object RGdes: TRadioGroup
    Left = 32
    Top = 240
    Width = 239
    Height = 38
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'Desim.-pilkku'
      'Desim.-piste')
    TabOrder = 10
  end
end
