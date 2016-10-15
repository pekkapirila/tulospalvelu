object FormEmithaku: TFormEmithaku
  Left = 0
  Top = 0
  Caption = 'Emit-koodin haku'
  ClientHeight = 294
  ClientWidth = 500
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
    Left = 30
    Top = 15
    Width = 73
    Height = 13
    Caption = 'Haettava koodi'
  end
  object Edit1: TEdit
    Left = 32
    Top = 32
    Width = 67
    Height = 21
    Alignment = taRightJustify
    TabOrder = 0
    Text = '0'
    OnExit = Edit1Exit
    OnKeyPress = Edit1KeyPress
  end
  object RG1: TRadioGroup
    Left = 310
    Top = 8
    Width = 163
    Height = 55
    Caption = ' Haettavat koodilajit '
    ItemIndex = 1
    Items.Strings = (
      'Tarkka koodi'
      'My'#246's vaihtuneet koodit')
    TabOrder = 4
  end
  object Button1: TButton
    Left = 114
    Top = 30
    Width = 50
    Height = 25
    Caption = 'Hae'
    TabOrder = 1
    OnClick = Button1Click
  end
  object SG1: TStringGrid
    Left = 16
    Top = 80
    Width = 473
    Height = 201
    ColCount = 4
    DefaultRowHeight = 20
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    TabOrder = 5
    OnDblClick = SG1DblClick
    OnKeyDown = SG1KeyDown
  end
  object BtnHyvaksy: TButton
    Left = 172
    Top = 30
    Width = 64
    Height = 25
    Caption = 'Hyv'#228'ksy'
    TabOrder = 2
    OnClick = BtnHyvaksyClick
  end
  object Button2: TButton
    Left = 245
    Top = 31
    Width = 59
    Height = 25
    Caption = 'Peruuta'
    TabOrder = 3
    OnClick = Button2Click
  end
end
