object FormAnnaTeksti: TFormAnnaTeksti
  Left = 0
  Top = 0
  Caption = 'Sy'#246't'#228' pyydetty teksti'
  ClientHeight = 116
  ClientWidth = 420
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 14
  object LblPrompt: TLabel
    Left = 19
    Top = 8
    Width = 304
    Height = 16
    Caption = 
      '................................................................' +
      '............'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object EdTeksti: TEdit
    Left = 8
    Top = 53
    Width = 361
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnExit = EdTekstiExit
    OnKeyDown = EdTekstiKeyDown
    OnKeyPress = EdTekstiKeyPress
  end
  object Button1: TButton
    Left = 192
    Top = 83
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 296
    Top = 83
    Width = 75
    Height = 25
    Caption = 'Peruuta'
    TabOrder = 2
    OnClick = Button2Click
  end
end
