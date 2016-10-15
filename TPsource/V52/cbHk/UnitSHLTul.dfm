object FormSHLtul: TFormSHLtul
  Left = 0
  Top = 0
  Caption = 'Hiihtoliiton tulostem'#228#228'rittelyt'
  ClientHeight = 336
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object EdtKilp: TLabeledEdit
    Left = 16
    Top = 24
    Width = 121
    Height = 21
    EditLabel.Width = 66
    EditLabel.Height = 13
    EditLabel.Caption = 'Kilpailun koodi'
    TabOrder = 0
  end
  object Button1: TButton
    Left = 16
    Top = 56
    Width = 121
    Height = 49
    Caption = 'Luo vakionimet rankisarjoille'
    TabOrder = 1
    WordWrap = True
    OnClick = Button1Click
  end
  object RGtyylit: TRadioGroup
    Left = 16
    Top = 111
    Width = 125
    Height = 81
    Caption = ' Merkitse kaikille tyyli '
    ItemIndex = 0
    Items.Strings = (
      'J'#228't'#228' ennalleen'
      'Merkitse P'
      'Merkitse V')
    TabOrder = 2
    OnClick = RGtyylitClick
  end
  object SG1: TStringGrid
    Left = 149
    Top = 10
    Width = 370
    Height = 318
    ColCount = 4
    DefaultRowHeight = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goTabs]
    TabOrder = 3
  end
  object Button2: TButton
    Left = 16
    Top = 209
    Width = 116
    Height = 53
    Caption = 'Sulje kaavake ja kirjoita tiedosto'
    TabOrder = 4
    WordWrap = True
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 16
    Top = 271
    Width = 116
    Height = 53
    Caption = 'Sulje kaavake ja peruuta tulostus'
    TabOrder = 5
    WordWrap = True
    OnClick = Button3Click
  end
end
