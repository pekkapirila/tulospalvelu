object FormNappain: TFormNappain
  Left = 0
  Top = 0
  Caption = 'Ajanotton'#228'pp'#228'in'
  ClientHeight = 243
  ClientWidth = 292
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 109
    Width = 240
    Height = 26
    Caption = 
      'Ajanotton'#228'pp'#228'in toimii vain, kun selailtava ajanottotaulu on akt' +
      'iivisena. ja muutokset sallittu.'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 24
    Top = 44
    Width = 58
    Height = 13
    Caption = 'Valittu koodi'
  end
  object Label3: TLabel
    Left = 24
    Top = 144
    Width = 112
    Height = 13
    Caption = 'Piste, jolle ajat otetaan'
  end
  object Edit1: TEdit
    Left = 24
    Top = 14
    Width = 225
    Height = 21
    TabOrder = 0
    Text = 'Paina valittavaa n'#228'pp'#228'int'#228' t'#228'ss'#228' kent'#228'ss'#228
    OnKeyDown = Edit1KeyDown
    OnKeyPress = Edit1KeyPress
  end
  object Button1: TButton
    Left = 24
    Top = 70
    Width = 225
    Height = 25
    Caption = 'Poista ajanotton'#228'pp'#228'in k'#228'yt'#246'st'#228
    TabOrder = 1
    OnClick = Button1Click
  end
  object Edit2: TEdit
    Left = 104
    Top = 41
    Width = 33
    Height = 21
    TabStop = False
    ReadOnly = True
    TabOrder = 2
  end
  object Edit3: TEdit
    Left = 154
    Top = 41
    Width = 95
    Height = 21
    TabStop = False
    ReadOnly = True
    TabOrder = 3
  end
  object Button2: TButton
    Left = 24
    Top = 203
    Width = 224
    Height = 25
    Caption = 'Sulje kaavake'
    TabOrder = 4
    OnClick = Button2Click
  end
  object CBPiste: TComboBox
    Left = 25
    Top = 161
    Width = 208
    Height = 21
    ItemIndex = 0
    TabOrder = 5
    Text = 'Automaattisesti m'#228#228'r'#228'ytyv'#228
    OnClick = CBPisteClick
    Items.Strings = (
      'Automaattisesti m'#228#228'r'#228'ytyv'#228
      'L'#228'ht'#246'aika'
      'Maalin aika'
      '1. v'#228'liaika')
  end
end
