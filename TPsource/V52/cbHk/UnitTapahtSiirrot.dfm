object FormTapahtSiirrot: TFormTapahtSiirrot
  Left = 0
  Top = 0
  Caption = 'Tapahtumasarjan siirrot'
  ClientHeight = 259
  ClientWidth = 496
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 15
  object GroupBox1: TGroupBox
    Left = 8
    Top = 7
    Width = 473
    Height = 242
    Caption = ' Kilpailunumeroiden siirto '
    TabOrder = 0
    object Label1: TLabel
      Left = 21
      Top = 24
      Width = 185
      Height = 15
      Caption = 'Vaihe, jonka osanottajia siirret'#228#228'n'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 21
      Top = 55
      Width = 153
      Height = 15
      Caption = 'Numeroalue, jolta siirret'#228#228'n'
    end
    object Label3: TLabel
      Left = 237
      Top = 55
      Width = 8
      Height = 15
      Caption = '--'
    end
    object Label4: TLabel
      Left = 21
      Top = 88
      Width = 153
      Height = 15
      Caption = 'Numeroalue, jolle siirret'#228#228'n'
    end
    object Label5: TLabel
      Left = 237
      Top = 88
      Width = 8
      Height = 15
      Caption = '--'
    end
    object Label6: TLabel
      Left = 19
      Top = 199
      Width = 185
      Height = 15
      Caption = 'Vakanttien siirrossa k'#228'ytetty lis'#228'ys'
    end
    object CBVaihe: TComboBox
      Left = 224
      Top = 21
      Width = 83
      Height = 23
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ItemIndex = 0
      ParentFont = False
      TabOrder = 0
      Text = '1. vaihe'
      Items.Strings = (
        '1. vaihe'
        '2. vaihe'
        '3. vaihe'
        '4. vaihe'
        '5. vaihe')
    end
    object EdtAlaraja: TEdit
      Left = 191
      Top = 52
      Width = 37
      Height = 23
      Alignment = taRightJustify
      TabOrder = 1
      Text = '0'
    end
    object EdtYlaraja: TEdit
      Left = 252
      Top = 52
      Width = 37
      Height = 23
      Alignment = taRightJustify
      TabOrder = 2
      Text = '0'
    end
    object EdtAlku: TEdit
      Left = 191
      Top = 85
      Width = 37
      Height = 23
      Alignment = taRightJustify
      TabOrder = 3
      Text = '0'
    end
    object CBNumKaikki: TCheckBox
      Left = 19
      Top = 122
      Width = 266
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Anna uudet numerot my'#246's muille vaiheille'
      Checked = True
      State = cbChecked
      TabOrder = 4
    end
    object CBTulSarjat: TCheckBox
      Left = 19
      Top = 144
      Width = 266
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Merkitse sama sarja tuleville vaiheille'
      Checked = True
      State = cbChecked
      TabOrder = 5
    end
    object Button1: TButton
      Left = 328
      Top = 132
      Width = 113
      Height = 41
      Caption = 'Toteuta siirto'
      TabOrder = 6
      OnClick = Button1Click
    end
    object CBPalautaVakantit: TCheckBox
      Left = 19
      Top = 169
      Width = 266
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Palauta samalla syrj'#228#228'n siirretyt vakantit'
      Checked = True
      State = cbChecked
      TabOrder = 7
    end
    object EdtVakBibLisa: TEdit
      Left = 248
      Top = 196
      Width = 37
      Height = 23
      Alignment = taRightJustify
      TabOrder = 8
      Text = '0'
      OnChange = EdtVakBibLisaChange
    end
    object Button4: TButton
      Left = 376
      Top = 207
      Width = 88
      Height = 25
      Caption = 'Sulje kaavake'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 9
      OnClick = Button4Click
    end
  end
end
