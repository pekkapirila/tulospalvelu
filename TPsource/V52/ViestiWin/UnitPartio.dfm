object FormPartio: TFormPartio
  Left = 0
  Top = 0
  Caption = 'Partiosuoritus'
  ClientHeight = 659
  ClientWidth = 326
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 9
    Top = 61
    Width = 28
    Height = 13
    Caption = 'Rastit'
    Visible = False
  end
  object Label2: TLabel
    Left = 99
    Top = 61
    Width = 47
    Height = 13
    Caption = 'Aikasakko'
    Visible = False
  end
  object Label3: TLabel
    Left = 205
    Top = 62
    Width = 20
    Height = 13
    Caption = 'Yht.'
    ParentShowHint = False
    ShowHint = True
    Visible = False
  end
  object DG1: TDrawGrid
    Left = 7
    Top = 158
    Width = 313
    Height = 495
    ColCount = 6
    DefaultRowHeight = 18
    RowCount = 52
    TabOrder = 0
    OnDrawCell = DG1DrawCell
  end
  object EdtKilpno: TEdit
    Left = 5
    Top = 7
    Width = 57
    Height = 21
    Alignment = taRightJustify
    TabOrder = 1
  end
  object EdtJoukkue: TEdit
    Left = 70
    Top = 7
    Width = 250
    Height = 21
    TabOrder = 2
  end
  object SGNimet: TStringGrid
    Left = 7
    Top = 81
    Width = 277
    Height = 74
    ColCount = 3
    DefaultRowHeight = 18
    RowCount = 3
    FixedRows = 0
    TabOrder = 3
  end
  object EdtSarja: TEdit
    Left = 7
    Top = 35
    Width = 74
    Height = 21
    TabOrder = 4
  end
  object EdtSija: TEdit
    Left = 93
    Top = 35
    Width = 52
    Height = 21
    Alignment = taRightJustify
    TabOrder = 5
  end
  object EdtTulos: TEdit
    Left = 160
    Top = 35
    Width = 72
    Height = 21
    Alignment = taRightJustify
    TabOrder = 6
  end
  object EdtTark: TEdit
    Left = 253
    Top = 34
    Width = 65
    Height = 21
    TabOrder = 7
  end
  object EdtRastiPisteet: TEdit
    Left = 49
    Top = 58
    Width = 44
    Height = 21
    Hint = 
      'Keltainen v'#228'ri kertoo, ett'#228' tallennettu tieto poikkeaa n'#228'ytetyst' +
      #228
    TabStop = False
    Alignment = taRightJustify
    ReadOnly = True
    TabOrder = 8
    Visible = False
  end
  object BtnTallenna: TButton
    Left = 288
    Top = 61
    Width = 32
    Height = 74
    Hint = 'Painike tallentaa n'#228'ytetyt tiedot'
    Caption = 'Tal len na'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 9
    Visible = False
    WordWrap = True
    OnClick = BtnTallennaClick
  end
  object EdtSakko: TEdit
    Left = 154
    Top = 58
    Width = 44
    Height = 21
    Hint = 
      'Keltainen v'#228'ri kertoo, ett'#228' tallennettu tieto poikkeaa n'#228'ytetyst' +
      #228
    Alignment = taRightJustify
    ParentShowHint = False
    ShowHint = True
    TabOrder = 10
    Visible = False
    OnChange = EdtSakkoChange
  end
  object EdtPisteet: TEdit
    Left = 228
    Top = 58
    Width = 44
    Height = 21
    Hint = 
      'Keltainen v'#228'ri kertoo, ett'#228' tallennettu tieto poikkeaa n'#228'ytetyst' +
      #228
    TabStop = False
    Alignment = taRightJustify
    ParentShowHint = False
    ReadOnly = True
    ShowHint = True
    TabOrder = 11
    Visible = False
  end
end
