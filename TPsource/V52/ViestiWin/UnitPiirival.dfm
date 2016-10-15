object FormPiiriVal: TFormPiiriVal
  Left = 0
  Top = 0
  Caption = 'Piirivalinta'
  ClientHeight = 525
  ClientWidth = 588
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label6: TLabel
    Left = 9
    Top = 54
    Width = 93
    Height = 14
    Caption = 'Ei-tulostettavat piirit'
  end
  object Label7: TLabel
    Left = 341
    Top = 38
    Width = 84
    Height = 14
    Caption = 'Tulostettavat piirit'
  end
  object Label1: TLabel
    Left = 341
    Top = 54
    Width = 211
    Height = 14
    Caption = 'Kaikki valitut piirit tulostetaan samaan listaan'
  end
  object Label2: TLabel
    Left = 268
    Top = 388
    Width = 57
    Height = 45
    Caption = #196'l'#228' sulje ennen tulostusta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    WordWrap = True
  end
  object Label3: TLabel
    Left = 144
    Top = 46
    Width = 145
    Height = 14
    Caption = 'Mukaan koko sarjojen parhaita'
  end
  object LBPiirit: TListBox
    Left = 8
    Top = 72
    Width = 239
    Height = 446
    ItemHeight = 14
    MultiSelect = True
    TabOrder = 0
    OnDblClick = LBPiiritDblClick
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
  object LBPiiriVal: TListBox
    Left = 341
    Top = 72
    Width = 239
    Height = 446
    ItemHeight = 14
    MultiSelect = True
    TabOrder = 5
    OnDblClick = LBPiiriValDblClick
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
  object RGPiirisijat: TRadioGroup
    Left = 117
    Top = -1
    Width = 208
    Height = 41
    Caption = ' Tulostettavat sijoitukset '
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'Kokonaissijat'
      'Piirisijat')
    TabOrder = 7
  end
  object EdtParhaat: TEdit
    Left = 296
    Top = 44
    Width = 29
    Height = 22
    Alignment = taRightJustify
    TabOrder = 8
    Text = '0'
  end
  object Button2: TButton
    Left = 21
    Top = 8
    Width = 90
    Height = 33
    Caption = 'Lue PIIRIT.LST'
    TabOrder = 9
    OnClick = Button2Click
  end
  object OpenDialog1: TOpenDialog
    Left = 440
    Top = 8
  end
end
