object FormVaTuloste: TFormVaTuloste
  Left = 0
  Top = 0
  Caption = 'V'#228'liaikatuloste'
  ClientHeight = 379
  ClientWidth = 623
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 507
    Top = 102
    Width = 99
    Height = 90
    Caption = 'Selaa painikkeilla tai n'#228'pp'#228'imill'#228' yl'#246's / alas Enter tulostaa'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object Label2: TLabel
    Left = 12
    Top = 40
    Width = 80
    Height = 18
    Caption = 'Joukkuenro'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 178
    Top = 40
    Width = 44
    Height = 18
    Caption = 'Osuus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 12
    Top = 72
    Width = 58
    Height = 18
    Caption = 'Joukkue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 12
    Top = 10
    Width = 85
    Height = 18
    Caption = 'Luettu koodi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 281
    Top = 40
    Width = 66
    Height = 18
    Caption = 'Vahvistus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 386
    Top = 34
    Width = 84
    Height = 16
    Caption = 'Enter: vahvista'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 386
    Top = 49
    Width = 74
    Height = 16
    Caption = 'Esc: peruuta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object DG1: TDrawGrid
    Left = 0
    Top = 107
    Width = 494
    Height = 272
    TabStop = False
    DefaultColWidth = 60
    FixedCols = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goEditing, goRowSelect]
    ParentFont = False
    TabOrder = 3
    OnDrawCell = DG1DrawCell
    OnKeyDown = DG1KeyDown
    OnKeyPress = DG1KeyPress
  end
  object Button1: TButton
    Left = 521
    Top = 223
    Width = 76
    Height = 37
    Caption = 'Yl'#246's'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 521
    Top = 271
    Width = 76
    Height = 37
    Caption = 'Alas'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 521
    Top = 319
    Width = 76
    Height = 37
    Caption = 'Tulosta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = Button3Click
  end
  object EdtKilpno: TEdit
    Left = 111
    Top = 37
    Width = 59
    Height = 26
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnKeyPress = EdtKilpnoKeyPress
  end
  object EdtOsuus: TEdit
    Left = 237
    Top = 37
    Width = 37
    Height = 26
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '0'
    OnKeyPress = EdtOsuusKeyPress
  end
  object EdtJoukkue: TEdit
    Left = 111
    Top = 69
    Width = 240
    Height = 26
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 7
  end
  object EdtBadge: TEdit
    Left = 111
    Top = 7
    Width = 81
    Height = 26
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = '0'
  end
  object EdtVahvistus: TEdit
    Left = 356
    Top = 37
    Width = 21
    Height = 26
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnEnter = EdtVahvistusEnter
    OnExit = EdtVahvistusExit
    OnKeyPress = EdtVahvistusKeyPress
  end
  object MainMenu1: TMainMenu
    Left = 328
    Top = 65528
    object Valinnat1: TMenuItem
      Caption = '&Valinnat'
      object AinoaSuoraan: TMenuItem
        Caption = 'Tulosta &ainoa k'#228'ytt'#246' ilman vahvistusta'
        OnClick = AinoaSuoraanClick
      end
      object AinaViimeinen: TMenuItem
        Caption = 'Tulosta aina &viimeinen k'#228'ytt'#246' ilman vahvistusta'
        OnClick = AinaViimeinenClick
      end
      object Kirjoittimenvalinta1: TMenuItem
        Caption = '&Kirjoittimen valinta'
        OnClick = Kirjoittimenvalinta1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
  end
end
