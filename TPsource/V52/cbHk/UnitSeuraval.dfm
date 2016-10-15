object FormSeuraVal: TFormSeuraVal
  Left = 0
  Top = 0
  Caption = 'Seuravalinta'
  ClientHeight = 525
  ClientWidth = 588
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label6: TLabel
    Left = 9
    Top = 20
    Width = 105
    Height = 14
    Caption = 'Ei-tulostettavat seurat'
  end
  object Label7: TLabel
    Left = 341
    Top = 9
    Width = 96
    Height = 14
    Caption = 'Tulostettavat seurat'
  end
  object Label1: TLabel
    Left = 341
    Top = 23
    Width = 223
    Height = 14
    Caption = 'Kaikki valitut seurat tulostetaan samaan listaan'
  end
  object Label2: TLabel
    Left = 268
    Top = 388
    Width = 47
    Height = 42
    Caption = #196'l'#228' sulje ennen tulostusta'
    WordWrap = True
  end
  object Label3: TLabel
    Left = 136
    Top = 13
    Width = 145
    Height = 14
    Caption = 'Mukaan koko sarjojen parhaita'
  end
  object LBSeurat: TListBox
    Left = 8
    Top = 40
    Width = 239
    Height = 476
    ItemHeight = 14
    MultiSelect = True
    TabOrder = 0
    OnDblClick = LBSeuratDblClick
  end
  object BtnLisKaikki: TButton
    Left = 270
    Top = 123
    Width = 49
    Height = 49
    Caption = 'Lis'#228#228' kaikki'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    WordWrap = True
    OnClick = BtnLisKaikkiClick
  end
  object BtnLisValitut: TButton
    Left = 270
    Top = 188
    Width = 49
    Height = 49
    Caption = 'Lis'#228#228' valitut'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    WordWrap = True
    OnClick = BtnLisValitutClick
  end
  object BtnPoisValitut: TButton
    Left = 270
    Top = 253
    Width = 49
    Height = 49
    Caption = 'Poista valitut'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    WordWrap = True
    OnClick = BtnPoisValitutClick
  end
  object BtnPoisKaikki: TButton
    Left = 270
    Top = 316
    Width = 49
    Height = 49
    Caption = 'Poista kaikki'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    WordWrap = True
    OnClick = BtnPoisKaikkiClick
  end
  object LBSeuraVal: TListBox
    Left = 341
    Top = 40
    Width = 239
    Height = 476
    ItemHeight = 14
    MultiSelect = True
    TabOrder = 5
    OnDblClick = LBSeuraValDblClick
  end
  object Button1: TButton
    Left = 260
    Top = 440
    Width = 70
    Height = 41
    Caption = 'Sulje'
    TabOrder = 6
    OnClick = Button1Click
  end
  object EdtParhaat: TEdit
    Left = 288
    Top = 11
    Width = 29
    Height = 22
    Alignment = taRightJustify
    TabOrder = 7
    Text = '0'
  end
  object CBMaa: TCheckBox
    Left = 258
    Top = 44
    Width = 71
    Height = 29
    Caption = 'Ota maa huomioon'
    TabOrder = 8
    WordWrap = True
    OnClick = CBMaaClick
  end
end
