object VertAikaFrm: TVertAikaFrm
  Left = 485
  Top = 194
  Caption = 'Asetukset'
  ClientHeight = 340
  ClientWidth = 516
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  FormStyle = fsStayOnTop
  Menu = MainMenu1
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object OKBtn: TButton
    Left = 271
    Top = 306
    Width = 98
    Height = 31
    Caption = 'OK'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = OKBtnClick
  end
  object PeruutaBtn: TButton
    Left = 383
    Top = 306
    Width = 98
    Height = 31
    Caption = 'Peruuta'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = PeruutaBtnClick
  end
  object GroupBox1: TGroupBox
    Left = 30
    Top = 26
    Width = 432
    Height = 160
    Caption = ' Vertailuajat '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    object Label6: TLabel
      Left = 17
      Top = 23
      Width = 260
      Height = 17
      Caption = 'Jokaisen rastiv'#228'lin vertailuaika lasketaan'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 82
      Top = 48
      Width = 195
      Height = 17
      Caption = 'parhaan kilpailijan keskiarvona'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 22
      Top = 74
      Width = 345
      Height = 17
      Caption = 'Laskennassa otetaan kuitenkin huomioon korkeintaan'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label9: TLabel
      Left = 82
      Top = 100
      Width = 266
      Height = 17
      Caption = 'prosenttia  rastiv'#228'lin kyseisell'#228' osuudella '
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label10: TLabel
      Left = 20
      Top = 130
      Width = 391
      Height = 17
      Caption = 'juosseista  py'#246'ristettyn'#228' yl'#246'sp'#228'in l'#228'himp'#228#228'n kokonaislukuun.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object EdMaxLkm: TEdit
      Left = 22
      Top = 44
      Width = 48
      Height = 25
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object EdMaxPros: TEdit
      Left = 22
      Top = 96
      Width = 48
      Height = 25
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
  end
  object SeurLstVal: TCheckBox
    Left = 47
    Top = 207
    Width = 376
    Height = 18
    Caption = 'P'#228'ivit'#228' seurantagrafiikan lista automaattisesti'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object SeurGrVal: TCheckBox
    Left = 47
    Top = 228
    Width = 376
    Height = 19
    Caption = 'P'#228'ivit'#228' seurantagrafiikan piirros automaattisesti'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 4
  end
  object TlsLuetVal: TCheckBox
    Left = 47
    Top = 258
    Width = 441
    Height = 19
    Caption = 
      'P'#228'ivit'#228' pistekohtainen viimeisinten tulosluettelo automaattisest' +
      'i'
    Checked = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    State = cbChecked
    TabOrder = 5
  end
  object MainMenu1: TMainMenu
    Left = 216
    Top = 8
    object Nytt1: TMenuItem
      Caption = '&N'#228'ytt'#246
      object Luekorostustiedot1: TMenuItem
        Caption = 'Lue &korostustiedot'
        OnClick = Luekorostustiedot1Click
      end
    end
    object Muitaasetuksia1: TMenuItem
      Caption = 'Muita asetuksia'
      object Kuuluttajarajoitukset1: TMenuItem
        Caption = 'Kuuluttajarajoitukset'
        OnClick = Kuuluttajarajoitukset1Click
      end
      object Maksimikesto1: TMenuItem
        Caption = 'Osuuden maksimikesto analyysissa'
        OnClick = Maksimikesto1Click
      end
    end
  end
end
