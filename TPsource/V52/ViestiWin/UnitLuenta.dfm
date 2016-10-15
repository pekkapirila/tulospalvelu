object FormLuenta: TFormLuenta
  Left = 0
  Top = 0
  Caption = 'Sis'#228#228'nluenta'
  ClientHeight = 411
  ClientWidth = 741
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 12
    Top = 43
    Width = 58
    Height = 16
    Caption = 'Viivakoodi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 12
    Top = 76
    Width = 66
    Height = 16
    Caption = 'Joukkuenro'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 159
    Top = 76
    Width = 38
    Height = 16
    Caption = 'Osuus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 12
    Top = 108
    Width = 48
    Height = 16
    Caption = 'Joukkue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 12
    Top = 11
    Width = 71
    Height = 16
    Caption = 'Luettu koodi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 259
    Top = 43
    Width = 55
    Height = 16
    Caption = 'Vahvistus'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 350
    Top = 39
    Width = 72
    Height = 14
    Caption = 'Enter: vahvista'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 350
    Top = 52
    Width = 61
    Height = 14
    Caption = 'Esc: peruuta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 471
    Top = 248
    Width = 98
    Height = 16
    Caption = 'Tallennetut tiedot'
  end
  object EdtViivakoodi: TEdit
    Left = 90
    Top = 40
    Width = 156
    Height = 24
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 0
    OnEnter = EdtViivakoodiEnter
    OnExit = EdtViivakoodiExit
    OnKeyPress = EdtViivakoodiKeyPress
  end
  object EdtKilpno: TEdit
    Left = 90
    Top = 73
    Width = 59
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
    Text = '0'
    OnEnter = EdtKilpnoEnter
    OnExit = EdtKilpnoExit
    OnKeyPress = EdtKilpnoKeyPress
  end
  object EdtOsuus: TEdit
    Left = 203
    Top = 73
    Width = 43
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
    Text = '0'
    OnEnter = EdtOsuusEnter
    OnExit = EdtOsuusExit
    OnKeyPress = EdtOsuusKeyPress
  end
  object RGTunnistus: TRadioGroup
    Left = 439
    Top = 37
    Width = 129
    Height = 60
    Caption = ' Kilpailijan valinta '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    Items.Strings = (
      'Viivakoodi'
      'Numero ja osuus')
    ParentFont = False
    TabOrder = 3
    OnClick = RGTunnistusClick
  end
  object EdtJoukkue: TEdit
    Left = 90
    Top = 105
    Width = 223
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 4
  end
  object BtnTallenna: TButton
    Left = 471
    Top = 131
    Width = 75
    Height = 47
    Caption = 'Tallenna'
    TabOrder = 5
    TabStop = False
    OnClick = BtnTallennaClick
  end
  object BtnPeruuta: TButton
    Left = 471
    Top = 206
    Width = 75
    Height = 35
    Caption = 'Peruuta'
    TabOrder = 6
    TabStop = False
    OnClick = BtnPeruutaClick
  end
  object DG1: TDrawGrid
    Left = 12
    Top = 144
    Width = 445
    Height = 258
    TabStop = False
    DefaultRowHeight = 22
    FixedCols = 0
    TabOrder = 7
    OnDrawCell = DG1DrawCell
  end
  object EdtBadge: TEdit
    Left = 90
    Top = 8
    Width = 67
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = '0'
  end
  object MemoMsg: TMemo
    Left = 574
    Top = 24
    Width = 166
    Height = 223
    TabStop = False
    Color = clFuchsia
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    Lines.Strings = (
      'MemoMsg')
    ParentFont = False
    ReadOnly = True
    TabOrder = 9
    Visible = False
  end
  object EdtMsg: TEdit
    Left = 165
    Top = 8
    Width = 403
    Height = 27
    TabStop = False
    BevelEdges = []
    BevelInner = bvNone
    BevelOuter = bvNone
    Color = clBtnFace
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Arial'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    TabOrder = 10
  end
  object EdtVahvistus: TEdit
    Left = 320
    Top = 40
    Width = 21
    Height = 24
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
    OnEnter = EdtVahvistusEnter
    OnExit = EdtVahvistusExit
    OnKeyPress = EdtVahvistusKeyPress
  end
  object RGkoodi: TRadioGroup
    Left = 325
    Top = 74
    Width = 101
    Height = 60
    Caption = ' Luettava koodi '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    Items.Strings = (
      '1. koodi'
      '2. koodi')
    ParentFont = False
    TabOrder = 12
    OnClick = RGTunnistusClick
  end
  object SG1: TStringGrid
    Left = 471
    Top = 268
    Width = 260
    Height = 132
    TabStop = False
    ColCount = 3
    DefaultColWidth = 70
    DefaultRowHeight = 22
    FixedCols = 0
    TabOrder = 13
  end
  object CBEhdOsuus: TCheckBox
    Left = 448
    Top = 103
    Width = 97
    Height = 17
    Caption = 'Ehdota osuus'
    TabOrder = 14
    Visible = False
  end
  object ProgressBar1: TProgressBar
    Left = 574
    Top = 8
    Width = 157
    Height = 10
    TabOrder = 15
  end
  object MainMenu1: TMainMenu
    Left = 528
    Top = 24
    object Valinnat1: TMenuItem
      Caption = '&Valinnat'
      object Huomautaohitetusta: TMenuItem
        Caption = 'Huomauta &ohitetusta osuudesta'
        Checked = True
        OnClick = HuomautaohitetustaClick
      end
      object Huomautasuljetusta: TMenuItem
        Caption = 'Huomauta &suljetusta osuudesta'
        Checked = True
        OnClick = HuomautasuljetustaClick
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      OnClick = Help1Click
    end
    object Laitteet1: TMenuItem
      Caption = 'Laitteet'
      object Emitlaitteidenyhteydetjaohjaus1: TMenuItem
        Caption = 'Emit-laitteiden yhteydet ja ohjaus'
        OnClick = Emitlaitteidenyhteydetjaohjaus1Click
      end
    end
  end
end
