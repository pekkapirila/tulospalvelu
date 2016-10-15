object FormVaiheMaar: TFormVaiheMaar
  Left = 0
  Top = 0
  Caption = 'Vaihekohtaiset m'#228#228'ritykset'
  ClientHeight = 336
  ClientWidth = 755
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SG1: TStringGrid
    Left = 6
    Top = 16
    Width = 742
    Height = 265
    ColCount = 6
    DefaultRowHeight = 20
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goTabs]
    TabOrder = 0
    OnExit = SG1Exit
    OnSelectCell = SG1SelectCell
  end
  object Button1: TButton
    Left = 208
    Top = 297
    Width = 145
    Height = 25
    Caption = 'Hyv'#228'ksy ja poistu'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 388
    Top = 297
    Width = 145
    Height = 25
    Caption = 'Peruuta muutokset'
    TabOrder = 2
    OnClick = Button2Click
  end
end
