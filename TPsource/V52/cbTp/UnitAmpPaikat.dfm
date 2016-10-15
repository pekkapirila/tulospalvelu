object FormAmpPaikat: TFormAmpPaikat
  Left = 0
  Top = 0
  Caption = 'Ampumapaikat'
  ClientHeight = 495
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object DG1: TDrawGrid
    Left = 0
    Top = -1
    Width = 529
    Height = 496
    DefaultRowHeight = 20
    TabOrder = 0
    OnDrawCell = DG1DrawCell
  end
end
