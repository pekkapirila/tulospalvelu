object FormMaalikello: TFormMaalikello
  Left = 0
  Top = 0
  Caption = 'Maalikello'
  ClientHeight = 395
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 529
    Height = 394
    ActivePage = TabYhteydet
    Style = tsFlatButtons
    TabOrder = 0
    object TabYhteydet: TTabSheet
      Caption = 'Laiteyhteydet'
      object GroupBox4: TGroupBox
        Left = 3
        Top = 3
        Width = 243
        Height = 355
        Caption = ' Ajanottolaite - Laitenumero 1 '
        TabOrder = 0
        TabStop = True
        object LblYhteys1: TLabel
          Left = 58
          Top = 296
          Width = 100
          Height = 16
          Caption = 'Yhteys suljettu'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object Label13: TLabel
          Left = -149
          Top = 545
          Width = 6
          Height = 13
          Caption = '1'
        end
        object BtnLaite1Avaa: TButton
          Left = 25
          Top = 323
          Width = 75
          Height = 25
          Caption = 'Avaa yhteys'
          TabOrder = 1
          OnClick = BtnLaite1AvaaClick
        end
        object BtnLaite1Sulje: TButton
          Left = 126
          Top = 323
          Width = 75
          Height = 25
          Caption = 'Sulje yhteys'
          TabOrder = 2
          OnClick = BtnLaite1SuljeClick
        end
        object Panel1: TPanel
          Left = 4
          Top = 15
          Width = 235
          Height = 270
          Caption = 'Panel1'
          ShowCaption = False
          TabOrder = 0
          object Label22: TLabel
            Left = 11
            Top = 39
            Width = 23
            Height = 13
            Caption = 'Laite'
          end
          object Label23: TLabel
            Left = 11
            Top = 64
            Width = 33
            Height = 13
            Caption = 'Yhteys'
          end
          object Label24: TLabel
            Left = 11
            Top = 88
            Width = 67
            Height = 13
            Caption = 'Portin numero'
          end
          object LbLaiteIP1: TLabel
            Left = 11
            Top = 113
            Width = 83
            Height = 13
            Hint = 'LbLaite1IP'
            Caption = 'Laitteen ip-osoite'
            Visible = False
          end
          object LbLaite1Baud: TLabel
            Left = 11
            Top = 136
            Width = 24
            Height = 13
            Caption = 'Baud'
          end
          object Label57: TLabel
            Left = 11
            Top = 160
            Width = 83
            Height = 13
            Caption = 'Kellon nollausaika'
          end
          object Label1: TLabel
            Left = 78
            Top = 177
            Width = 110
            Height = 13
            Caption = 'Kellon kanavien pisteet'
          end
          object Label11: TLabel
            Left = 8
            Top = 191
            Width = 21
            Height = 13
            Caption = 'Com'
          end
          object Label12: TLabel
            Left = 42
            Top = 192
            Width = 19
            Height = 13
            Caption = 'N'#228'p'
          end
          object Label2: TLabel
            Left = 83
            Top = 191
            Width = 6
            Height = 13
            Caption = '0'
          end
          object Label3: TLabel
            Left = 100
            Top = 191
            Width = 6
            Height = 13
            Caption = '1'
          end
          object Label4: TLabel
            Left = 117
            Top = 191
            Width = 6
            Height = 13
            Caption = '2'
          end
          object Label5: TLabel
            Left = 134
            Top = 191
            Width = 6
            Height = 13
            Caption = '3'
          end
          object Label6: TLabel
            Left = 151
            Top = 191
            Width = 6
            Height = 13
            Caption = '4'
          end
          object Label7: TLabel
            Left = 168
            Top = 191
            Width = 6
            Height = 13
            Caption = '5'
          end
          object Label8: TLabel
            Left = 185
            Top = 191
            Width = 6
            Height = 13
            Caption = '6'
          end
          object Label9: TLabel
            Left = 202
            Top = 191
            Width = 6
            Height = 13
            Caption = '7'
          end
          object Label10: TLabel
            Left = 219
            Top = 191
            Width = 6
            Height = 13
            Caption = '8'
          end
          object Label16: TLabel
            Left = 77
            Top = 228
            Width = 143
            Height = 13
            Caption = 'Kellon kanavien ajanottojonot'
          end
          object Label26: TLabel
            Left = 11
            Top = 12
            Width = 59
            Height = 13
            Caption = 'Laitenumero'
          end
          object CBLaite1: TComboBox
            Left = 87
            Top = 36
            Width = 119
            Height = 21
            TabOrder = 0
            Text = 'Ei laitetta'
            OnChange = CBLaite1Change
            Items.Strings = (
              'Ei laitetta'
              'Timy'
              'Comet'
              'Alge S4'
              'Alge S3'
              'Emit RTR1'
              'Emit RTR2')
          end
          object CBLaiteYht1: TComboBox
            Left = 87
            Top = 61
            Width = 119
            Height = 21
            Enabled = False
            TabOrder = 1
            Text = 'Sarjaportti'
            OnChange = CBLaiteYht1Change
            Items.Strings = (
              'Sarjaportti'
              'TCP (laite palvelin)'
              'TCP (laite asiakas)'
              'UDP')
          end
          object EdLaitePort1: TEdit
            Left = 169
            Top = 85
            Width = 37
            Height = 21
            Alignment = taRightJustify
            Enabled = False
            TabOrder = 2
            Text = '0'
          end
          object EdLaiteIP1: TEdit
            Left = 114
            Top = 110
            Width = 92
            Height = 21
            Alignment = taRightJustify
            TabOrder = 3
            Text = '000.000.000.000'
            Visible = False
          end
          object CBLaiteBaud1: TComboBox
            Left = 87
            Top = 133
            Width = 119
            Height = 21
            ItemIndex = 0
            TabOrder = 4
            Text = 'Laitteen oletus'
            Items.Strings = (
              'Laitteen oletus'
              '1200'
              '2400'
              '4800'
              '9600'
              '19200'
              '38400'
              '57600'
              '115200')
          end
          object EdAikaEro1: TEdit
            Left = 143
            Top = 157
            Width = 63
            Height = 21
            Alignment = taRightJustify
            Enabled = False
            TabOrder = 5
          end
          object ECC1: TEdit
            Left = 11
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 6
            Text = 'M'
            OnExit = ECExit
          end
          object ECX1: TEdit
            Left = 28
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 7
            Text = 'M'
            OnExit = ECExit
          end
          object ECK1: TEdit
            Left = 45
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 8
            Text = 'M'
            OnExit = ECExit
          end
          object EC01: TEdit
            Left = 78
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 9
            Text = 'L'
            OnExit = ECExit
          end
          object EC11: TEdit
            Left = 95
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 10
            Text = 'M'
            OnExit = ECExit
          end
          object EC21: TEdit
            Left = 112
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 11
            Text = '1'
            OnExit = ECExit
          end
          object EC31: TEdit
            Left = 129
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 12
            Text = '2'
            OnExit = ECExit
          end
          object EC41: TEdit
            Left = 146
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 13
            Text = '3'
            OnExit = ECExit
          end
          object EC51: TEdit
            Left = 163
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 14
            Text = '4'
            OnExit = ECExit
          end
          object EC61: TEdit
            Left = 180
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 15
            Text = '5'
            OnExit = ECExit
          end
          object EC71: TEdit
            Left = 197
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 16
            Text = '6'
            OnExit = ECExit
          end
          object EC81: TEdit
            Left = 214
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 17
            Text = '7'
            OnExit = ECExit
          end
          object EJC1: TEdit
            Left = 11
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 18
            Text = 'M'
            OnExit = EJExit
          end
          object EJX1: TEdit
            Left = 28
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 19
            Text = 'M'
            OnExit = EJExit
          end
          object EJK1: TEdit
            Left = 45
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 20
            Text = 'M'
            OnExit = EJExit
          end
          object EJ01: TEdit
            Left = 78
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 21
            Text = 'L'
            OnExit = EJExit
          end
          object EJ11: TEdit
            Left = 95
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 22
            Text = 'M'
            OnExit = EJExit
          end
          object EJ21: TEdit
            Left = 112
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 23
            Text = '1'
            OnExit = EJExit
          end
          object EJ31: TEdit
            Left = 129
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 24
            Text = '2'
            OnExit = EJExit
          end
          object EJ41: TEdit
            Left = 146
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 25
            Text = '3'
            OnExit = EJExit
          end
          object EJ51: TEdit
            Left = 163
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 26
            Text = '4'
            OnExit = EJExit
          end
          object EJ61: TEdit
            Left = 180
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 27
            Text = '5'
            OnExit = EJExit
          end
          object EJ71: TEdit
            Left = 197
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 28
            Text = '6'
            OnExit = EJExit
          end
          object EJ81: TEdit
            Left = 214
            Top = 243
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 29
            Text = '7'
            OnExit = EJExit
          end
          object Edit1: TEdit
            Left = 167
            Top = 9
            Width = 37
            Height = 21
            TabStop = False
            Alignment = taRightJustify
            ReadOnly = True
            TabOrder = 30
            Text = '1'
          end
        end
      end
      object GBLaite2: TGroupBox
        Left = 268
        Top = 3
        Width = 238
        Height = 355
        Caption = ' Toinen ajanottolaite  '
        TabOrder = 1
        TabStop = True
        object LblYhteys2: TLabel
          Left = 73
          Top = 296
          Width = 100
          Height = 16
          Caption = 'Yhteys suljettu'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
        end
        object BtnLaite2Avaa: TButton
          Left = 29
          Top = 323
          Width = 75
          Height = 25
          Caption = 'Avaa yhteys'
          TabOrder = 1
          OnClick = BtnLaite2AvaaClick
        end
        object BtnLaite2Sulje: TButton
          Left = 132
          Top = 323
          Width = 75
          Height = 25
          Caption = 'Sulje yhteys'
          TabOrder = 2
          OnClick = BtnLaite2SuljeClick
        end
        object Panel2: TPanel
          Left = 4
          Top = 16
          Width = 231
          Height = 269
          Caption = 'Panel2'
          ShowCaption = False
          TabOrder = 0
          object Label32: TLabel
            Left = 10
            Top = 11
            Width = 59
            Height = 13
            Caption = 'Laitenumero'
          end
          object Label27: TLabel
            Left = 10
            Top = 36
            Width = 23
            Height = 13
            Caption = 'Laite'
          end
          object Label28: TLabel
            Left = 10
            Top = 61
            Width = 33
            Height = 13
            Caption = 'Yhteys'
          end
          object Label29: TLabel
            Left = 10
            Top = 85
            Width = 67
            Height = 13
            Caption = 'Portin numero'
          end
          object LbLaiteIP2: TLabel
            Left = 10
            Top = 110
            Width = 83
            Height = 13
            Caption = 'Laitteen ip-osoite'
            Visible = False
          end
          object Label55: TLabel
            Left = 10
            Top = 159
            Width = 83
            Height = 13
            Caption = 'Kellon nollausaika'
          end
          object Label59: TLabel
            Left = 76
            Top = 177
            Width = 110
            Height = 13
            Caption = 'Kellon kanavien pisteet'
          end
          object Label14: TLabel
            Left = 4
            Top = 191
            Width = 21
            Height = 13
            Caption = 'Com'
          end
          object Label15: TLabel
            Left = 38
            Top = 192
            Width = 19
            Height = 13
            Caption = 'N'#228'p'
          end
          object Label58: TLabel
            Left = 77
            Top = 191
            Width = 6
            Height = 13
            Caption = '0'
          end
          object Label56: TLabel
            Left = 94
            Top = 191
            Width = 6
            Height = 13
            Caption = '1'
          end
          object Label54: TLabel
            Left = 111
            Top = 191
            Width = 6
            Height = 13
            Caption = '2'
          end
          object Label53: TLabel
            Left = 128
            Top = 191
            Width = 6
            Height = 13
            Caption = '3'
          end
          object Label52: TLabel
            Left = 145
            Top = 191
            Width = 6
            Height = 13
            Caption = '4'
          end
          object Label51: TLabel
            Left = 162
            Top = 191
            Width = 6
            Height = 13
            Caption = '5'
          end
          object Label50: TLabel
            Left = 179
            Top = 191
            Width = 6
            Height = 13
            Caption = '6'
          end
          object Label49: TLabel
            Left = 196
            Top = 191
            Width = 6
            Height = 13
            Caption = '7'
          end
          object Label48: TLabel
            Left = 213
            Top = 191
            Width = 6
            Height = 13
            Caption = '8'
          end
          object Label17: TLabel
            Left = 73
            Top = 227
            Width = 143
            Height = 13
            Caption = 'Kellon kanavien ajanottojonot'
          end
          object EdLaite2No: TEdit
            Left = 159
            Top = 8
            Width = 37
            Height = 21
            Alignment = taRightJustify
            TabOrder = 0
            Text = '0'
            OnExit = EdLaite2NoExit
          end
          object CBLaite2: TComboBox
            Left = 77
            Top = 33
            Width = 119
            Height = 21
            Enabled = False
            TabOrder = 1
            Text = 'Ei laitetta'
            OnChange = CBLaite2Change
            Items.Strings = (
              'Ei laitetta'
              'Timy')
          end
          object CBLaiteYht2: TComboBox
            Left = 77
            Top = 58
            Width = 119
            Height = 21
            Enabled = False
            ItemIndex = 0
            TabOrder = 2
            Text = 'Sarjaportti'
            OnChange = CBLaiteYht2Change
            Items.Strings = (
              'Sarjaportti'
              'TCP (laite palvelin)'
              'TCP (laite asiakas)'
              'UDP')
          end
          object EdLaitePort2: TEdit
            Left = 159
            Top = 82
            Width = 37
            Height = 21
            Alignment = taRightJustify
            Enabled = False
            TabOrder = 3
            Text = '0'
          end
          object EdLaiteIP2: TEdit
            Left = 104
            Top = 107
            Width = 92
            Height = 21
            Alignment = taRightJustify
            TabOrder = 4
            Text = '000.000.000.000'
            Visible = False
          end
          object EdAikaEro2: TEdit
            Left = 133
            Top = 156
            Width = 63
            Height = 21
            Alignment = taRightJustify
            Enabled = False
            TabOrder = 5
          end
          object ECC2: TEdit
            Left = 7
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 6
            Text = 'M'
            OnExit = ECExit
          end
          object ECX2: TEdit
            Left = 24
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 7
            Text = 'M'
            OnExit = ECExit
          end
          object ECK2: TEdit
            Left = 41
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 8
            Text = 'M'
            OnExit = ECExit
          end
          object EC02: TEdit
            Left = 72
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 9
            Text = 'L'
            OnExit = ECExit
          end
          object EC12: TEdit
            Left = 89
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 10
            Text = 'M'
            OnExit = ECExit
          end
          object EC22: TEdit
            Left = 106
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 11
            Text = '1'
            OnExit = ECExit
          end
          object EC32: TEdit
            Left = 123
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 12
            Text = '2'
            OnExit = ECExit
          end
          object EC42: TEdit
            Left = 140
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 13
            Text = '3'
            OnExit = ECExit
          end
          object EC52: TEdit
            Left = 157
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 14
            Text = '4'
            OnExit = ECExit
          end
          object EC62: TEdit
            Left = 174
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 15
            Text = '5'
            OnExit = ECExit
          end
          object EC72: TEdit
            Left = 191
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 16
            Text = '6'
            OnExit = ECExit
          end
          object EC82: TEdit
            Left = 208
            Top = 206
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 17
            Text = '7'
            OnExit = ECExit
          end
          object EJC2: TEdit
            Left = 7
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 18
            Text = 'M'
            OnExit = EJExit
          end
          object EJX2: TEdit
            Left = 24
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 19
            Text = 'M'
            OnExit = EJExit
          end
          object EJK2: TEdit
            Left = 41
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 20
            Text = 'M'
            OnExit = EJExit
          end
          object EJ02: TEdit
            Left = 74
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 21
            Text = 'L'
            OnExit = EJExit
          end
          object EJ12: TEdit
            Left = 91
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 22
            Text = 'M'
            OnExit = EJExit
          end
          object EJ22: TEdit
            Left = 108
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 23
            Text = '1'
            OnExit = EJExit
          end
          object EJ32: TEdit
            Left = 125
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 24
            Text = '2'
            OnExit = EJExit
          end
          object EJ42: TEdit
            Left = 142
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 25
            Text = '3'
            OnExit = EJExit
          end
          object EJ52: TEdit
            Left = 159
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 26
            Text = '4'
            OnExit = EJExit
          end
          object EJ62: TEdit
            Left = 176
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 27
            Text = '5'
            OnExit = EJExit
          end
          object EJ72: TEdit
            Left = 193
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 28
            Text = '6'
            OnExit = EJExit
          end
          object EJ82: TEdit
            Left = 209
            Top = 242
            Width = 16
            Height = 21
            Alignment = taCenter
            TabOrder = 29
            Text = '7'
            OnExit = EJExit
          end
        end
      end
    end
    object TabOhjaus: TTabSheet
      Caption = 'Kellon ohjaus'
      ImageIndex = 1
      object Label18: TLabel
        Left = 26
        Top = 9
        Width = 467
        Height = 15
        Caption = 
          'Maalikellon ohjaus on mahdollista vain laitteelle 1, joka on jok' +
          'o Alge S4 tai Emit RTR2'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
      end
      object GBS4: TGroupBox
        Left = 3
        Top = 30
        Width = 246
        Height = 267
        Caption = ' Alge S4 '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Arial'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        object Label19: TLabel
          Left = 129
          Top = 25
          Width = 23
          Height = 15
          Caption = 'Rivit'
        end
        object Label20: TLabel
          Left = 171
          Top = 48
          Width = 4
          Height = 15
          Caption = '-'
        end
        object Label21: TLabel
          Left = 14
          Top = 133
          Width = 161
          Height = 15
          Caption = 'Valokennoviive (yksikk'#246' 0,1 s)'
        end
        object RGUusinta: TRadioGroup
          Left = 7
          Top = 19
          Width = 117
          Height = 60
          Caption = ' Uudelleenl'#228'hetys'
          Items.Strings = (
            'Kaikki ajat'
            'Valitut rivit')
          TabOrder = 0
          OnClick = RGUusintaClick
        end
        object EdAlku: TEdit
          Left = 129
          Top = 45
          Width = 37
          Height = 23
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object EdLoppu: TEdit
          Left = 181
          Top = 45
          Width = 37
          Height = 23
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 2
          Text = '0'
        end
        object BtnS4Uudelleen: TButton
          Left = 46
          Top = 85
          Width = 142
          Height = 25
          Caption = 'L'#228'het'#228' uudelleen'
          TabOrder = 3
          OnClick = BtnS4UudelleenClick
        end
        object EdViive: TEdit
          Left = 188
          Top = 130
          Width = 37
          Height = 23
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 4
          Text = '0'
        end
        object BtnViive: TButton
          Left = 46
          Top = 160
          Width = 142
          Height = 25
          Caption = 'Aseta valokennoviive'
          TabOrder = 5
          OnClick = BtnViiveClick
        end
        object BtnTyhjenna: TButton
          Left = 46
          Top = 211
          Width = 142
          Height = 25
          Caption = 'Tyhjenn'#228' kellon muisti '
          TabOrder = 6
          OnClick = BtnTyhjennaClick
        end
      end
      object GBRTR2: TGroupBox
        Left = 264
        Top = 30
        Width = 229
        Height = 267
        Caption = 'Emit RTR2 '
        TabOrder = 1
        object Label25: TLabel
          Left = 141
          Top = 33
          Width = 52
          Height = 13
          Caption = 'Lukum'#228#228'r'#228
        end
        object RGRTR2Uusinta: TRadioGroup
          Left = 15
          Top = 27
          Width = 117
          Height = 60
          Caption = ' Uudelleenl'#228'hetys'
          Items.Strings = (
            'Kaikki ajat'
            'Valittu lukum'#228#228'r'#228)
          TabOrder = 0
          OnClick = RGRTR2UusintaClick
        end
        object EdLukum: TEdit
          Left = 141
          Top = 53
          Width = 37
          Height = 21
          Alignment = taRightJustify
          Enabled = False
          TabOrder = 1
          Text = '0'
        end
        object Button1: TButton
          Left = 54
          Top = 98
          Width = 142
          Height = 25
          Caption = 'L'#228'het'#228' uudelleen'
          TabOrder = 2
          OnClick = Button1Click
        end
      end
    end
  end
  object BitBtn1: TBitBtn
    Left = 490
    Top = 3
    Width = 31
    Height = 31
    Margins.Left = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Glyph.Data = {
      FE0A0000424DFE0A00000000000036000000280000001E0000001E0000000100
      180000000000C80A000000000000000000000000000000000000FFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCBB5A9C3A89EB297
      8DA689849B857F968D839391869C9A90ABA8A0BFB9B4FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFB3A393A48C78A88B76BC9C89D8B7A7F1D2C3
      FFE5D5FBEAD7EADCC9CFC1AFAEA191988D7F9C9288B7ACA4FFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFFB5A69D9F8E7BB9A489DFC8A8FFEDCAFFFFDFFFFFE3FFFFE3FFFFE4FF
      FDDFFFFFE7FFFFE9FFFFE9FDE9D0D4C3AEAFA18F978C7EFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFCCB8AD
      B19786FBDDC0FFEFCBFFFFD7FFFFD5FFECC0EFD7ADE9D4AEF6DCB7FFDFB9FFDD
      B6FFE4BEFFF8D4FFFFE3FFFFE0F6E1C5D3C3ACB2A793C4BBADFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFD9C8BFCAAE9DF4CFB3FF
      F7D1FFE9BBFFD29FF1B884D8A370CE9D6DD6A97DE8B98DF2BC8DFCC493FECA9B
      FACC9DF8CFA2FFDEB3FFF8D3FFFFE4DAC9AEC7BAA4D7CEC0FFFFFFFFFFFFFFFF
      FFFFFFFF0000FFFFFFFFFFFFFDFFFFD4CAC3BCA798F5D2B8FFFFDFFFC695EFAC
      75DD9559DB9357E19C63DF9F69CF9564C78C5BDC9A65DF9A61DC9C65E2A76FEB
      B782F7CA97FFD9A9FFE0B5FFFFE3D9C7AACBBDAAF7EEE1FFFFFBFFFEFFFFFDFF
      0000FFFFFFFFFFFFF6F1EEAB9B8FFFF5DEFFF9D7F3B587D48A50DF8F4EE4914D
      E59451D28749DA975EF9BC8AF3B481EDA468DE9152EBA161DF9A5BCD8E51DEA6
      6BEDBC84F1C693F8D5A9FFFFE3F1DCC1B4A694FFFEF5FFFBFAFFFBFF0000FFFF
      FFFCF6F7CCBAB3DCBEADFFFBDFF2B48BD0854DDB8543E28841E38A40E6944CE0
      9956FFC185FFFFCCFFFFD2FFD092F3A05CEC9B57E99A57DC934FE4A05FE5A669
      E4AD74F9CA97FDD7ADFFF6D7D1BCA6EFE1D5FFFCF9FFFFFF0000FFFFFFF5DAD6
      D1AB9FFFF1DCFFD3B0C67746C36A2DDF8138E48738E28A3DE09448D39853ECBF
      82FFFECCFFFFD4FFEAACF9A35DE7914BEE9852EB9650E89851DD924ED89354EF
      B17BDCAA7CFFF5D2FFEACFCBB9A8FFFBF3FFFFFF0000FFFFFFC7AAA1FFDAC9FF
      FEE1CA8056BD642BDB7834EC8536ED8734E48535DB883DC68645DDAE76FFF3C6
      FFFFD0FFCE8FEA924CE68C45F2964FF2944BEF934AE28A43DD8947DF965AE1A0
      6DFAC69EFFFFE6C6AD99F2E4D8FFFFFF0000F9F9EDA79B89FFFFE8FAC6A19C55
      23BF6525EA8137E3701FF5802BFC8D3DF49048CB793FBE7C4CCC9874C48D66E4
      9A60D37C39E68B46F4954CEE8B41F48E45F08A42E88743D98043F0A36BD29367
      FFF8D6F3D5BCC5B5A4FFFFFF0000DEDECCCDBFA8FFFAD9B67F54B2672FC66521
      E57628EE7420F57A24EC7929E17A35C7743DE19C74FFE5CAFFFAD9DC925CD07A
      38E68B46F18F47EB8239F3863CF3843BEF833CDB7737EB945BC78251F4C098FF
      FFE7A89782FCF6E90000D0BAA8FFEAD0FFE0BCAA6435C76F33D36922DF6918EA
      6F17EF7620E47827D57A35BC7942D6A57DFFF6DBFFFFE6FFBF8ADE8A49DE8340
      EB8641EC7E36F27F36F27A32F07C35DD722FD7793ED38655CF976EFFFFE6C1AF
      98E2DDCE0000C1A08CFFF7DBFFD1ADB96B3CB05419D36A27D16317D36716D873
      24D77E34D18747BE8450D8AC83FFF6D7FFFFE6FFEFC4D99764C4773FD77D42E4
      7E3DEC7B38EB722CF07731E16E2BCB6628DF8B57CC8C63FFEDCEF1D9C1CAC0AF
      0000B39278FFFFE2FFC199A65627C26A34E3874CDB8245EFA061DA9458ECAF77
      DEA873D09C6DD09B70FFDCB3FFFEDCFFFFEAFFECD4CC9A7CB6734CD78250E680
      45EA7937EA712CE66F2AD6692BCA6C37C67A50FFF9DAF0CBB7E8D0C40000BEA0
      85FFFEDEFFC8A2B86A3FEE9A6AFFAD7AFFB783E6A775E2AE7FBE9165B3895ECC
      9C72D6A176EEAE85FFE8C7FFFCECFFFEF3F8D4C2E5B193C77E52D1763FE87E3E
      E97633EB7633D26527C66833CB7D52FFFBDEF2CAB7DBC0B60000BA9F8AFFF2D7
      F7BE9EB7704AF5A77CFFB486FBB78AE7AD83D8AA81CAA37DDBB592C19774C797
      73CB916ED1A185F6DFD0FFFBEEFFFFEDFFF0D0D69267CD7741E58343DE7532E1
      7633D26D2FC16B37BE794EFFFCE0E9C8B5D5C0B80000C6AE9CFFF3DBFFD3B7BA
      7B59F5B089FFBE97E7A882DBA783DDB291FEDABCFFF1D4D6B195BE9477BE8C6E
      B3876FBFAA9BE2D2C5FFFFECFFFFE3FFC89CD48651DC8144D2712FD16F2FD77B
      40BF703DB5774FFFFCE1D3B7A6E7D8CF0000D4C1B4EFCFBCFFF4DDAA7456F9BC
      9CFFC7A7E7B190D1A286FAD5BBFFF9E2FFF5E0FFE9D2C9A48EB78B74C9A49091
      8171C2B3A3FFF3DBFFFFE2FFFBD1D7915CCE7C42D37C3ECE763ACF7E45B26F3E
      C7926DFFFFE8B09C8BF8EFE60000ECDBD2B99F8FFFFFEBA5765BE6B196EDB99B
      EDBDA1DDB39CECC9B5FFF8E7FFFFEEFFFFEEDEBDAAC8A08DB796839C897ACFBF
      AEFFF7DFFFFFE2FFFFD7D39160C87D45DF8E55D7884FBB723EAD7044F4C6A4FF
      F7DEA49484FFFFFF0000FFFFFFBCA294FFF0DDEAC0A9C7987DE4B59AF8CAB2E7
      C0AAD1B09DFADDCEFFFFF1FFF9EAFFE1D1F2CFBBC5A592D8C1B1FFE9D7FFFDE4
      FFFFE3FFD4ACD39567CE8853DA925CD58F5AB17141BC885FFFF2D2E7CCB7C6B6
      A9FFFFFF0000FFFFFFC7B2A3E8CAB7FFFFEA9C735ADFB39BF3C9B2E0B8A5E3C2
      AFDCBBACFFE7D8FFFFF0FFFFEEFFF0D9FFFEE7FFF8E4FFFFEEFFF8E0FFFFE0DA
      A580D99C70D59363CB8857CA8A5AB2784ED6A682FFFFE7B89F8BF2E4D8FFFFFF
      0000FFFFFFFFFFFFB89F8BFFF7DEFFE3C7A37C60F4CCB3F0C9B3E4BEACD8B4A2
      E0BCAAFCD9C5FFF8E0FFFFE6FFFEE4FFFBE5FFF5E2FFDFC7ECBA9CD39B78D093
      6BD8986FDE9E75B97C54AF7A55FFFFE4FFDCC4B89F8FFFFFF7FFFFFF0000FFFF
      FFFFFFFFD7C2ADCCB197FFFFE3DDBB9EB08B71F7D0BAF2CAB7E9C1AEE2BAA7E0
      BAA2E4BDA1E5BF9FE5BE9EECC2ABEABBA5D4A28BBE8A6CCA9171E1A581D79975
      B67A56B57F5CFFF1D1FFE1C8C7A794EFD9CDFFFFFFFFFFFF0000FFFFFFFFFFFF
      FFFFFFBCA88FEED6BAFEE1C6E2C0A8B08973E7BDAAFFDFCCF9CFB8E2B79CE2B9
      99E9C29CE7BF9CD7A88DD7A38CE3AD94F5BFA1F1BA9BD59C7CB98362AE7958FF
      DFC0FFFCE0C8A793D5BCACFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFF
      FFFFA39278F1D9C1FFF5DEE8C3AFC09683B38875DFB39CFFDFC2FFDEBDF2CBA4
      E9C099FBC9ADF6C0A7EFBA9FDBA68BB58163A67456D7A889FFECCFFFF8DFCFAF
      98D0B6A5FFFEF1FFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFBDAE9BE1C9B7FFFFF0FFE9DAE2B6A5B98C77B18469C19774D2AC82DDB58BD6
      A486CE997EB47F64A27054C19276FCD2B5FFF4D8FFF2D8C8AC94CEB9A4FFF5E5
      FFFFFFFFFFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      CEBBACB29587FAD4C8FFFFF0FFFCEAFFDEC3F0C7A6D7B489C29C72C19070DFAA
      8FFFCDB1FFEBCFFFFFE2FFF7DBE6C7AEAD957DDFCCB7FFFFFFFFFFFFFFFFFFFF
      FFFFFFFFFFFFFFFF0000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFCAA59DB1877BEFC3B2FFFCE3FFFBDBFFF4CCFFFFD7FFFFE1FFFFE3FFFFE5
      FFE9CCD7B296B29479D3BBA3FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      FFFFFFFF0000F1F8FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFD
      F7FFEFE6EFC4B5CAA089C19B7BC6A27CC7A47CCEA180BD8F70BA8E71D9B395FF
      E2C8FFFFE7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
      0000}
    TabOrder = 1
    TabStop = False
    OnClick = BitBtn1Click
  end
end
