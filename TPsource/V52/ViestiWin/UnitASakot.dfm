object FormASakot: TFormASakot
  Left = 0
  Top = 0
  Caption = 'Ampumasakkojen sy'#246'tt'#246
  ClientHeight = 323
  ClientWidth = 508
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
  object Label1: TLabel
    Left = 16
    Top = 270
    Width = 28
    Height = 13
    Caption = 'Kilpno'
  end
  object Label2: TLabel
    Left = 65
    Top = 270
    Width = 30
    Height = 13
    Caption = 'Osuus'
  end
  object Label3: TLabel
    Left = 374
    Top = 270
    Width = 27
    Height = 13
    Caption = 'Sakot'
  end
  object EditKno: TEdit
    Left = 16
    Top = 248
    Width = 42
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnExit = EditKnoExit
    OnKeyPress = EditKnoKeyPress
  end
  object EditNimi: TEdit
    Left = 95
    Top = 248
    Width = 169
    Height = 23
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 6
    Text = 'Nimi'
  end
  object EditS1: TEdit
    Left = 374
    Top = 248
    Width = 25
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '-'
    OnExit = EditS1Exit
    OnKeyPress = EditS4KeyPress
  end
  object EditS2: TEdit
    Left = 405
    Top = 248
    Width = 25
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '-'
    OnExit = EditS2Exit
    OnKeyPress = EditS4KeyPress
  end
  object EditS3: TEdit
    Left = 435
    Top = 248
    Width = 25
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = '-'
    OnExit = EditS3Exit
    OnKeyPress = EditS4KeyPress
  end
  object EditS4: TEdit
    Left = 466
    Top = 248
    Width = 25
    Height = 23
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = '-'
    OnExit = EditS4Exit
    OnKeyPress = EditS4KeyPress
  end
  object EditAika: TEdit
    Left = 304
    Top = 248
    Width = 65
    Height = 23
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 7
    Text = 'Kello'
  end
  object EditPiste: TEdit
    Left = 268
    Top = 248
    Width = 33
    Height = 23
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = 'Piste'
  end
  object StringGrid1: TStringGrid
    Left = 16
    Top = 16
    Width = 475
    Height = 226
    TabStop = False
    DefaultColWidth = 50
    DefaultRowHeight = 20
    RowCount = 2
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
  end
  object Button1: TButton
    Left = 251
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Hyv'#228'ksy'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 16
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Seuraava'
    TabOrder = 10
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 104
    Top = 288
    Width = 75
    Height = 25
    Caption = 'Edellinen'
    TabOrder = 11
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 403
    Top = 288
    Width = 88
    Height = 25
    Caption = 'Sulje kaavake'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
    OnClick = Button4Click
  end
  object EditOsuus: TEdit
    Left = 65
    Top = 248
    Width = 25
    Height = 23
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    Text = '0'
    OnExit = EditOsuusExit
    OnKeyPress = EditOsuusKeyPress
  end
end
