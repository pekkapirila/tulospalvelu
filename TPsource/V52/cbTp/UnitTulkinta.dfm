object FormTulkinta: TFormTulkinta
  Left = 0
  Top = 0
  Caption = 'Luetun tietueen tulkinta'
  ClientHeight = 598
  ClientWidth = 570
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 8
    Top = 75
    Width = 132
    Height = 15
    Caption = 'Tietueesta tulkitut kent'#228't'
  end
  object LblSanoma: TLabel
    Left = 8
    Top = 8
    Width = 53
    Height = 16
    Caption = 'Sanoma'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMaroon
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    Visible = False
  end
  object SG1: TStringGrid
    Left = 8
    Top = 94
    Width = 557
    Height = 499
    ColCount = 4
    DefaultColWidth = 40
    DefaultRowHeight = 18
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object EdtTietue: TLabeledEdit
    Left = 8
    Top = 45
    Width = 97
    Height = 23
    TabStop = False
    Alignment = taRightJustify
    EditLabel.Width = 94
    EditLabel.Height = 15
    EditLabel.Caption = 'Tietueen numero'
    ReadOnly = True
    TabOrder = 1
  end
  object BtnHyvaksy: TButton
    Left = 128
    Top = 30
    Width = 89
    Height = 39
    Caption = 'Hyv'#228'ksy ja lue seuraava'
    TabOrder = 2
    WordWrap = True
    OnClick = BtnHyvaksyClick
  end
  object BtnLueloput: TButton
    Left = 230
    Top = 30
    Width = 89
    Height = 38
    Caption = 'Hyv'#228'ksy ja lue loput'
    TabOrder = 3
    WordWrap = True
    OnClick = BtnLueloputClick
  end
  object BtnOhita: TButton
    Left = 332
    Top = 30
    Width = 89
    Height = 39
    Caption = 'Ohita tietue'
    TabOrder = 4
    OnClick = BtnOhitaClick
  end
  object BtnKeskeyta: TButton
    Left = 433
    Top = 30
    Width = 89
    Height = 38
    Caption = 'Keskeyt'#228' lukeminen'
    TabOrder = 5
    WordWrap = True
    OnClick = BtnKeskeytaClick
  end
end
