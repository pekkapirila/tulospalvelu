object SeurantaForm: TSeurantaForm
  Left = 408
  Top = 259
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  Caption = 'Seurantagrafiikka'
  ClientHeight = 713
  ClientWidth = 916
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 13
    Top = 698
    Width = 54
    Height = 14
    Caption = 'Listan koko'
  end
  object Label5: TLabel
    Left = 116
    Top = 22
    Width = 30
    Height = 17
    Caption = 'Viiva'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object JoukkList: TListBox
    Left = 9
    Top = 267
    Width = 164
    Height = 419
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Arial'
    Font.Style = []
    ItemHeight = 15
    MultiSelect = True
    ParentFont = False
    TabOrder = 0
    OnClick = JoukkListClick
  end
  object EdMaxLkm: TEdit
    Left = 103
    Top = 694
    Width = 57
    Height = 22
    TabOrder = 1
    Text = '1000'
  end
  object PaivitaButton: TButton
    Left = 9
    Top = 224
    Width = 160
    Height = 27
    Caption = 'P'#228'ivit'#228' lista'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = PaivitaButtonClick
  end
  object PiirraBtn: TButton
    Left = 9
    Top = 181
    Width = 160
    Height = 27
    Caption = 'P'#228'ivit'#228' kuva'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = PiirraBtnClick
  end
  object OsVal: TComboBox
    Left = 9
    Top = 43
    Width = 87
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 1
    ParentFont = False
    TabOrder = 4
    Text = '1. osuus'
    OnChange = OsValChange
    Items.Strings = (
      'Numerot'
      '1. osuus'
      '2. osuus'
      '3. osuus'
      '4. osuus'
      '5. osuus'
      '6. osuus'
      '7. osuus')
  end
  object GroupBox1: TGroupBox
    Left = 9
    Top = 78
    Width = 156
    Height = 91
    Caption = ' N'#228'ytett'#228'v'#228't osuudet '
    TabOrder = 5
    object Label3: TLabel
      Left = 17
      Top = 26
      Width = 23
      Height = 17
      Caption = 'Alin'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 17
      Top = 60
      Width = 23
      Height = 17
      Caption = 'Ylin'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object AlinOs: TComboBox
      Left = 56
      Top = 22
      Width = 87
      Height = 25
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 0
      Text = '1. osuus'
      OnChange = AlinOsChange
      Items.Strings = (
        '1. osuus'
        '2. osuus'
        '3. osuus'
        '4. osuus'
        '5. osuus'
        '6. osuus'
        '7. osuus')
    end
    object YlinOs: TComboBox
      Left = 56
      Top = 56
      Width = 87
      Height = 25
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 1
      Text = '1. osuus'
      OnChange = YlinOsChange
      Items.Strings = (
        '1. osuus'
        '2. osuus'
        '3. osuus'
        '4. osuus'
        '5. osuus'
        '6. osuus'
        '7. osuus')
    end
  end
  object ViivaVal: TComboBox
    Left = 116
    Top = 43
    Width = 49
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 6
    Text = '1'
    OnChange = ViivaValChange
    Items.Strings = (
      '1'
      '2'
      '3')
  end
  object SarjaVal: TComboBox
    Left = 9
    Top = 9
    Width = 95
    Height = 25
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 7
    Text = 'Sarja'
    OnChange = SarjaValChange
    Items.Strings = (
      'Sarja')
  end
end
