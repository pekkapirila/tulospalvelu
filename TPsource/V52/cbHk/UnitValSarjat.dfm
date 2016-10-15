object FormValSarjat: TFormValSarjat
  Left = 0
  Top = 0
  Caption = 'Valitse sarjat'
  ClientHeight = 603
  ClientWidth = 340
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
  object Label2: TLabel
    Left = 17
    Top = 8
    Width = 110
    Height = 15
    Caption = 'Ei-k'#228'sitelt'#228'v'#228't sarjat'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 200
    Top = 8
    Width = 97
    Height = 15
    Caption = 'K'#228'sitelt'#228'v'#228't sarjat'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object LBSarjat: TListBox
    Left = 17
    Top = 27
    Width = 121
    Height = 558
    Hint = 'Kaksoisklikkaa sarjaa tai k'#228'yt'#228' painikkeita'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ItemHeight = 14
    MultiSelect = True
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnDblClick = LBSarjatDblClick
  end
  object BtnLisKaikki: TButton
    Left = 145
    Top = 128
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
    Left = 145
    Top = 193
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
    Left = 145
    Top = 258
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
    Left = 145
    Top = 324
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
  object LBSarjaVal: TListBox
    Left = 200
    Top = 27
    Width = 121
    Height = 558
    Hint = 'Kaksoisklikkaa sarjaa tai k'#228'yt'#228' painikkeita'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ItemHeight = 14
    MultiSelect = True
    ParentFont = False
    ParentShowHint = False
    ShowHint = True
    TabOrder = 5
    OnDblClick = LBSarjaValDblClick
  end
  object Button2: TButton
    Left = 144
    Top = 528
    Width = 49
    Height = 57
    Caption = 'Sulje ikkuna'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    WordWrap = True
    OnClick = Button2Click
  end
end
