object FormNaytaTulos: TFormNaytaTulos
  Left = 0
  Top = 0
  Caption = 'Leimantarkastuksen tulos'
  ClientHeight = 300
  ClientWidth = 450
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 7
    Width = 414
    Height = 23
    Caption = 'Kilpailijan nimi                                             '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LblTulos: TLabel
    Left = 26
    Top = 35
    Width = 74
    Height = 23
    Caption = '00:00:00'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LblSija: TLabel
    Left = 126
    Top = 35
    Width = 73
    Height = 23
    Caption = '100./200'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LblEro: TLabel
    Left = 215
    Top = 36
    Width = 88
    Height = 23
    Caption = '+00:00:00'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object Panel1: TPanel
    Left = 24
    Top = 64
    Width = 185
    Height = 217
    Caption = 'OK'
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -27
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentBackground = False
    ParentFont = False
    TabOrder = 0
  end
  object Memo1: TMemo
    Left = 240
    Top = 64
    Width = 185
    Height = 217
    Alignment = taCenter
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    Lines.Strings = (
      ''
      'Leimoissa puutteita'
      'Siirry itkumuurille'
      ''
      'Problems in punches'
      'Go to Wailing Wall'
      ''
      'St'#228'mplingar felaktiga'
      'G'#229' till klagomuren')
    ParentFont = False
    ParentShowHint = False
    ReadOnly = True
    ShowHint = False
    TabOrder = 1
  end
  object MainMenu1: TMainMenu
    Left = 231
    Top = 162
    object Valinnat1: TMenuItem
      Caption = 'Valinnat'
      object Teksti1: TMenuItem
        Caption = 'Salli tekstin muokkaus'
        OnClick = Teksti1Click
      end
      object Nytnkesto1: TMenuItem
        Caption = 'N'#228'yt'#246'n kesto'
        OnClick = Nytnkesto1Click
      end
      object Nyttulosjasija1: TMenuItem
        Caption = 'N'#228'yt'#228' tulos ja sija'
        Checked = True
        OnClick = Nyttulosjasija1Click
      end
      object Esiluennanlhtraja1: TMenuItem
        Caption = 'Esiluennan l'#228'ht'#246'raja'
        Visible = False
        OnClick = Esiluennanlhtraja1Click
      end
    end
  end
end
