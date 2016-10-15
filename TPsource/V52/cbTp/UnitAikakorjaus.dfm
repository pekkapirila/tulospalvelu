object FormAikakorjaus: TFormAikakorjaus
  Left = 0
  Top = 0
  Caption = 'Ajanoton aikojen korjaus'
  ClientHeight = 262
  ClientWidth = 446
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
  object Label9: TLabel
    Left = 8
    Top = 11
    Width = 23
    Height = 13
    Caption = 'Jono'
  end
  object Label1: TLabel
    Left = 8
    Top = 39
    Width = 78
    Height = 13
    Caption = 'K'#228'sitelt'#228'v'#228't rivit'
  end
  object Label2: TLabel
    Left = 156
    Top = 39
    Width = 8
    Height = 13
    Caption = '--'
  end
  object Label3: TLabel
    Left = 8
    Top = 110
    Width = 291
    Height = 13
    Caption = 'Korjaus tehd'#228#228'n lineaarisesti seuraavien aikojen perusteella.'
  end
  object Label4: TLabel
    Left = 8
    Top = 63
    Width = 396
    Height = 13
    Caption = 
      'Tee korjaukset aina ajoille, jotka ovat saman vuorokauden sis'#228'll' +
      #228' ennen korjausta.'
  end
  object Label5: TLabel
    Left = 8
    Top = 152
    Width = 58
    Height = 13
    Caption = 'Ajankohta 1'
  end
  object Label6: TLabel
    Left = 166
    Top = 152
    Width = 83
    Height = 13
    Caption = 'Siirto millisekuntia'
  end
  object Label7: TLabel
    Left = 8
    Top = 180
    Width = 58
    Height = 13
    Caption = 'Ajankohta 2'
  end
  object Label8: TLabel
    Left = 166
    Top = 180
    Width = 83
    Height = 13
    Caption = 'Siirto millisekuntia'
  end
  object Label10: TLabel
    Left = 8
    Top = 127
    Width = 263
    Height = 13
    Caption = 'Ajankohdat eiv'#228't vaikuta k'#228'sitelt'#228'vien rivien valintaan.'
  end
  object Label11: TLabel
    Left = 8
    Top = 81
    Width = 308
    Height = 13
    Caption = '(Korjaa ensin ajat, jotka pysyv'#228't saman vuorokauden puolella.) '
  end
  object CBJono: TComboBox
    Left = 74
    Top = 7
    Width = 58
    Height = 21
    ItemIndex = 0
    TabOrder = 0
    Text = '1'
    Items.Strings = (
      '1')
  end
  object EdtEns: TEdit
    Left = 108
    Top = 36
    Width = 42
    Height = 21
    Alignment = taRightJustify
    TabOrder = 1
    Text = '1'
  end
  object EdtViim: TEdit
    Left = 169
    Top = 36
    Width = 42
    Height = 21
    Alignment = taRightJustify
    TabOrder = 2
    Text = '99999'
  end
  object EdtAika1: TEdit
    Left = 81
    Top = 149
    Width = 69
    Height = 21
    TabOrder = 3
    Text = '00:00:00'
    OnExit = EdtAika1Exit
  end
  object EdtSiirto1: TEdit
    Left = 255
    Top = 149
    Width = 74
    Height = 21
    Alignment = taRightJustify
    TabOrder = 4
    Text = '0'
  end
  object EdtAika2: TEdit
    Left = 81
    Top = 177
    Width = 69
    Height = 21
    TabOrder = 5
    Text = '00:00:00'
    OnExit = EdtAika2Exit
  end
  object EdtSiirto2: TEdit
    Left = 255
    Top = 177
    Width = 74
    Height = 21
    Alignment = taRightJustify
    TabOrder = 6
    Text = '0'
  end
  object BtnToteuta: TButton
    Left = 8
    Top = 220
    Width = 113
    Height = 25
    Caption = 'Touteuta siirto'
    TabOrder = 7
    OnClick = BtnToteutaClick
  end
  object BtnSulje: TButton
    Left = 240
    Top = 220
    Width = 75
    Height = 25
    Caption = 'Sulje'
    TabOrder = 8
    OnClick = BtnSuljeClick
  end
  object BtnKopioi1: TButton
    Left = 344
    Top = 147
    Width = 81
    Height = 51
    Caption = 'K'#228'yt'#228' ajankohdan 1 siirtoa kaikille'
    TabOrder = 9
    WordWrap = True
    OnClick = BtnKopioi1Click
  end
end
