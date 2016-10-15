object TapForm: TTapForm
  Left = 519
  Top = 80
  Caption = 'Tapahtumat'
  ClientHeight = 414
  ClientWidth = 716
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 133
    Top = 9
    Width = 108
    Height = 16
    Caption = 'N'#228'ytett'#228'v'#228't pisteet'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object TapGrid: TDrawGrid
    Left = 0
    Top = 39
    Width = 716
    Height = 389
    ColCount = 11
    DefaultRowHeight = 20
    DefaultDrawing = False
    FixedCols = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving]
    ParentFont = False
    TabOrder = 0
    OnDblClick = TapGridDblClick
    OnDrawCell = TapGridDrawCell
    ColWidths = (
      39
      23
      126
      129
      33
      62
      29
      31
      55
      53
      26)
  end
  object Button1: TButton
    Left = 32
    Top = 6
    Width = 81
    Height = 22
    Caption = 'P'#228'ivit'#228
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button1Click
  end
  object PisteVal: TComboBox
    Left = 254
    Top = 6
    Width = 109
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 2
    Text = 'Kaikki'
    OnChange = PisteValChange
    Items.Strings = (
      'Kaikki'
      'Maali'
      'V'#228'liaika 1'
      'V'#228'liaika 2'
      'V'#228'liaika 3'
      'V'#228'liaika 4')
  end
  object CBTarkSan: TCheckBox
    Left = 405
    Top = 9
    Width = 165
    Height = 18
    Caption = 'N'#228'yt'#228' tarkistussanomat'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = CBTarkSanClick
  end
  object MainMenu1: TMainMenu
    Left = 318
    Top = 65528
    object Valinnat2: TMenuItem
      Caption = '&Valinnat'
      object Etunimiensin1: TMenuItem
        Caption = '&Etunimi ensin'
        Checked = True
        OnClick = Etunimiensin1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object Valinnat1: TMenuItem
      Caption = '&Kirjasin'
      object Pieni1: TMenuItem
        Caption = '&Pieni (8 pts)'
        OnClick = Pieni1Click
      end
      object Keski1: TMenuItem
        Caption = '&Keski (10 pts)'
        Checked = True
        OnClick = Keski1Click
      end
      object Suuri1: TMenuItem
        Caption = '&Suuri (12 pts)'
        OnClick = Suuri1Click
      end
      object Muumerkkikoko1: TMenuItem
        Caption = '&Muu merkkikoko'
        OnClick = Muumerkkikoko1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Lihavoitu1: TMenuItem
        Caption = '&Lihavoitu'
        OnClick = Lihavoitu1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object allennasarakeleveydet1: TMenuItem
        Caption = '&Tallenna sarakeleveydet'
        OnClick = allennasarakeleveydet1Click
      end
    end
  end
end
