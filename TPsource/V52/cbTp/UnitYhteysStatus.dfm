object FormYhteysStatus: TFormYhteysStatus
  Left = 0
  Top = 0
  Caption = 'Yhteydet'
  ClientHeight = 336
  ClientWidth = 441
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object YhteysGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 441
    Height = 336
    ColCount = 6
    DefaultColWidth = 40
    DefaultRowHeight = 18
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goTabs]
    TabOrder = 0
    ColWidths = (
      40
      193
      40
      28
      48
      50)
  end
end
