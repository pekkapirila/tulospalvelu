object FormMsgs: TFormMsgs
  Left = 0
  Top = 0
  Caption = 'Sanomat'
  ClientHeight = 513
  ClientWidth = 547
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 15
  object Memo1: TMemo
    Left = 8
    Top = 32
    Width = 529
    Height = 473
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object Button1: TButton
    Left = 386
    Top = 5
    Width = 151
    Height = 25
    Caption = 'Sulje ikkuna'
    TabOrder = 1
    OnClick = Button1Click
  end
end
