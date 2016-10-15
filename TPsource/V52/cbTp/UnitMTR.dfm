object FormMTR: TFormMTR
  Left = 0
  Top = 0
  Caption = 'MTR-ohjaus'
  ClientHeight = 437
  ClientWidth = 570
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
    Left = 16
    Top = 8
    Width = 71
    Height = 14
    Caption = 'Yhteysnumero'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 116
    Top = 8
    Width = 35
    Height = 14
    Caption = 'Laite-id'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 185
    Top = 8
    Width = 62
    Height = 14
    Caption = 'Laitteen kello'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 324
    Top = 8
    Width = 78
    Height = 14
    Caption = 'Poikkeama (sek)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 407
    Top = 9
    Width = 33
    Height = 14
    Caption = 'Paristo'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 8
    Top = 56
    Width = 160
    Height = 13
    Caption = 'Laitteen muistissa olevat istunnot'
  end
  object CBAsema: TComboBox
    Left = 17
    Top = 28
    Width = 54
    Height = 22
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ItemIndex = 0
    ParentFont = False
    TabOrder = 0
    Text = '1'
    Items.Strings = (
      '1')
  end
  object EdtId: TEdit
    Left = 116
    Top = 28
    Width = 57
    Height = 22
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
    Text = '0'
  end
  object EdtKello: TEdit
    Left = 185
    Top = 28
    Width = 119
    Height = 22
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
    Text = '0'
  end
  object EdtEro: TEdit
    Left = 324
    Top = 28
    Width = 62
    Height = 22
    TabStop = False
    Alignment = taRightJustify
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 3
    Text = '0'
  end
  object EdtParisto: TEdit
    Left = 404
    Top = 28
    Width = 62
    Height = 22
    TabStop = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 4
    Text = '0'
  end
  object Button2: TButton
    Left = 486
    Top = 131
    Width = 75
    Height = 46
    Caption = 'Aseta kello tietokoneen aikaan'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    WordWrap = True
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 486
    Top = 185
    Width = 75
    Height = 38
    Caption = 'Sulje istunto (MTR3)'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    WordWrap = True
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 486
    Top = 240
    Width = 75
    Height = 49
    Caption = 'Tyhjenn'#228' laitteen muisti'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    WordWrap = True
    OnClick = Button4Click
  end
  object SG1: TStringGrid
    Left = 8
    Top = 73
    Width = 467
    Height = 209
    DefaultRowHeight = 18
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 292
    Width = 474
    Height = 138
    Caption = ' Sanomien lukemispyynt'#246' '
    TabOrder = 9
    object Label7: TLabel
      Left = 272
      Top = 10
      Width = 35
      Height = 13
      Caption = 'Istunto'
    end
    object Label8: TLabel
      Left = 272
      Top = 47
      Width = 52
      Height = 13
      Caption = 'Lukum'#228#228'r'#228
    end
    object Label9: TLabel
      Left = 376
      Top = 10
      Width = 21
      Height = 13
      Caption = 'Ens.'
    end
    object Label10: TLabel
      Left = 422
      Top = 10
      Width = 22
      Height = 13
      Caption = 'Viim.'
    end
    object Label12: TLabel
      Left = 333
      Top = 115
      Width = 54
      Height = 13
      Caption = 'Tallennettu'
      Visible = False
    end
    object Button1: TButton
      Left = 376
      Top = 52
      Width = 75
      Height = 25
      Caption = 'Lue sanomia'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = Button1Click
    end
    object RGToisto: TRadioGroup
      Left = 11
      Top = 21
      Width = 110
      Height = 67
      Caption = ' Toisto '
      ItemIndex = 0
      Items.Strings = (
        'Yksi sanoma'
        'Jatkuva luenta')
      TabOrder = 1
      OnClick = RGToistoClick
    end
    object RGRajaus: TRadioGroup
      Left = 144
      Top = 20
      Width = 113
      Height = 70
      Caption = 'Rajaus'
      ItemIndex = 0
      Items.Strings = (
        'Istunto'
        'Viimeiset'
        'Sanomanumerot')
      TabOrder = 2
      OnClick = RGRajausClick
    end
    object CBIstunto: TComboBox
      Left = 272
      Top = 24
      Width = 81
      Height = 21
      ItemIndex = 0
      TabOrder = 3
      Text = 'Viimeisin'
      Items.Strings = (
        'Viimeisin'
        'Viim. -1'
        'Viim. -2'
        'Viim. -3'
        'Viim. -4'
        'Viim. -5'
        'Viim. -6'
        'Viim. -7')
    end
    object EdtLkm: TEdit
      Left = 272
      Top = 64
      Width = 42
      Height = 21
      Alignment = taRightJustify
      TabOrder = 4
      Text = '0'
    end
    object EdtEns: TEdit
      Left = 374
      Top = 25
      Width = 42
      Height = 21
      Alignment = taRightJustify
      TabOrder = 5
      Text = '0'
    end
    object EdtViim: TEdit
      Left = 422
      Top = 24
      Width = 42
      Height = 21
      Alignment = taRightJustify
      TabOrder = 6
      Text = '0'
    end
    object BtnSeur: TButton
      Left = 376
      Top = 83
      Width = 75
      Height = 25
      Caption = 'Seuraava'
      TabOrder = 7
      OnClick = BtnSeurClick
    end
    object RGkohde: TRadioGroup
      Left = 11
      Top = 92
      Width = 303
      Height = 40
      Caption = ' Lukemisen kohde '
      Columns = 2
      Items.Strings = (
        'Heti k'#228'sitelt'#228'v'#228'ksi'
        'Erilliseen tiedostoon')
      TabOrder = 8
    end
    object EdtTallennettu: TEdit
      Left = 394
      Top = 112
      Width = 57
      Height = 21
      Alignment = taRightJustify
      TabOrder = 9
      Text = '0'
      Visible = False
    end
  end
  object BtnSulje: TButton
    Left = 502
    Top = 370
    Width = 60
    Height = 46
    Caption = 'Sulje kaavake'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Arial'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    WordWrap = True
    OnClick = BtnSuljeClick
  end
  object SaveDialog1: TSaveDialog
    Left = 496
    Top = 56
  end
end
