object FormOsuus: TFormOsuus
  Left = 497
  Top = 153
  Caption = 'Osuustiedot'
  ClientHeight = 679
  ClientWidth = 693
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
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 13
    Top = 21
    Width = 48
    Height = 16
    Caption = '&Joukkue'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 289
    Top = 77
    Width = 32
    Height = 16
    Caption = 'L'#228'ht'#246
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 289
    Top = 49
    Width = 31
    Height = 16
    Caption = 'Maali'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 13
    Top = 49
    Width = 31
    Height = 16
    Caption = 'Tulos'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 190
    Top = 49
    Width = 22
    Height = 16
    Caption = 'Sija'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 13
    Top = 77
    Width = 52
    Height = 16
    Caption = 'Osuustls'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 155
    Top = 77
    Width = 58
    Height = 16
    Caption = 'Osuussija'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label11: TLabel
    Left = 526
    Top = 77
    Width = 62
    Height = 16
    Caption = 'Emit-koodi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 418
    Top = 77
    Width = 52
    Height = 16
    Caption = 'L'#228'ht'#246'sija'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label10: TLabel
    Left = 551
    Top = 21
    Width = 27
    Height = 16
    Caption = 'Rata'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label12: TLabel
    Left = 228
    Top = 0
    Width = 30
    Height = 16
    Caption = 'Sarja'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object EdtJoukkue: TEdit
    Left = 73
    Top = 17
    Width = 40
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = '0'
    OnExit = EdtJoukkueChanged
    OnKeyDown = EdtJoukkueKeyDown
  end
  object JoukkNimi: TEdit
    Left = 323
    Top = 17
    Width = 178
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 9
  end
  object OsNimi: TEdit
    Left = 418
    Top = 45
    Width = 242
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 10
  end
  object Maa: TEdit
    Left = 508
    Top = 17
    Width = 40
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 11
  end
  object TLahto: TEdit
    Left = 332
    Top = 73
    Width = 79
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 12
  end
  object TMaali: TEdit
    Left = 332
    Top = 45
    Width = 79
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 13
  end
  object Tulos: TEdit
    Left = 74
    Top = 45
    Width = 76
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 14
  end
  object Sija: TEdit
    Left = 224
    Top = 45
    Width = 36
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 15
  end
  object OsTls: TEdit
    Left = 74
    Top = 73
    Width = 76
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 16
  end
  object OsSija: TEdit
    Left = 224
    Top = 73
    Width = 36
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 17
  end
  object Ekoodi: TEdit
    Left = 594
    Top = 73
    Width = 66
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 18
  end
  object EmitGrid: TStringGrid
    Left = 0
    Top = 131
    Width = 692
    Height = 548
    TabStop = False
    ColCount = 8
    DefaultColWidth = 55
    DefaultRowHeight = 18
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    ParentFont = False
    TabOrder = 19
  end
  object LahtoSija: TEdit
    Left = 487
    Top = 73
    Width = 35
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 20
  end
  object OsKarkiBtn: TButton
    Left = 339
    Top = 100
    Width = 96
    Height = 26
    Caption = 'Osuuden &nopein'
    TabOrder = 6
    OnClick = OsKarkiBtnClick
  end
  object OsSeur: TButton
    Left = 438
    Top = 100
    Width = 109
    Height = 26
    Caption = 'Osuuden &seuraava'
    TabOrder = 7
    OnClick = OsSeurClick
  end
  object OsEd: TButton
    Left = 550
    Top = 100
    Width = 104
    Height = 26
    Caption = 'Osuuden &edellinen'
    TabOrder = 8
    OnClick = OsEdClick
  end
  object VaihdKarki: TButton
    Left = 16
    Top = 100
    Width = 87
    Height = 26
    Caption = '&Vaihdon k'#228'rki'
    TabOrder = 3
    OnClick = VaihdKarkiBtn
  end
  object Button1: TButton
    Left = 106
    Top = 100
    Width = 105
    Height = 26
    Caption = 'Vaihdon se&uraava'
    TabOrder = 4
    OnClick = VaihdSeurBtn
  end
  object Button2: TButton
    Left = 214
    Top = 100
    Width = 100
    Height = 26
    Caption = 'Vaihdon e&dellinen'
    TabOrder = 5
    OnClick = VaihdEdBtn
  end
  object EdHajonta: TEdit
    Left = 582
    Top = 17
    Width = 78
    Height = 24
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 21
  end
  object ValSarja: TComboBox
    Left = 228
    Top = 17
    Width = 88
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 2
    Text = 'Sarja'
    OnChange = ValSarjaChange
    Items.Strings = (
      'Sarja')
  end
  object ValOsuus: TComboBox
    Left = 123
    Top = 17
    Width = 95
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 1
    Text = 'Sarja'
    OnChange = ValOsuusChange
    Items.Strings = (
      'Sarja')
  end
  object MainMenu1: TMainMenu
    Left = 488
    Top = 4
    object Haku1: TMenuItem
      Caption = '&Haku'
      object Haenimell1: TMenuItem
        Caption = 'Hae &nimell'#228
        OnClick = Haenimell1Click
      end
    end
    object Rastivliaikaanalyysi1: TMenuItem
      Caption = 'Rastiv'#228'liaika-analyysi'
      Visible = False
      object eeanalyysi1: TMenuItem
        Caption = 'Tee analyysi'
        OnClick = eeanalyysi1Click
      end
    end
  end
end
