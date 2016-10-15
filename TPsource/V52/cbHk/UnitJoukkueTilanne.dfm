object FormJoukkueTilanne: TFormJoukkueTilanne
  Left = 0
  Top = 0
  Caption = 'Joukkuetilanne'
  ClientHeight = 513
  ClientWidth = 547
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Arial'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 15
  object Label1: TLabel
    Left = 16
    Top = 9
    Width = 29
    Height = 15
    Caption = 'Sarja'
  end
  object Memo1: TMemo
    Left = 8
    Top = 32
    Width = 529
    Height = 473
    ScrollBars = ssBoth
    TabOrder = 0
  end
  object Button1: TButton
    Left = 386
    Top = 5
    Width = 151
    Height = 25
    Caption = 'Sulje ikkuna'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 214
    Top = 5
    Width = 131
    Height = 25
    Caption = 'P'#228'ivit'#228' tilanne'
    TabOrder = 2
    OnClick = Button2Click
  end
  object CBSarja: TComboBox
    Left = 64
    Top = 6
    Width = 132
    Height = 23
    ItemIndex = 0
    TabOrder = 3
    Text = 'Koko kilpailu'
    Items.Strings = (
      'Koko kilpailu')
  end
  object MainMenu1: TMainMenu
    Left = 304
    Top = 192
    object oiminnot1: TMenuItem
      Caption = '&Toiminnot'
      object Pivit1: TMenuItem
        Caption = '&P'#228'ivit'#228
        OnClick = Pivit1Click
      end
      object Sulje1: TMenuItem
        Caption = '&Sulje'
        OnClick = Sulje1Click
      end
    end
    object Valinnat1: TMenuItem
      Caption = '&Valinnat'
      object ulostenluonne1: TMenuItem
        Caption = 'Tulosten luonne'
        object Yhteisaika1: TMenuItem
          Caption = 'Yhteisaika'
          OnClick = Yhteisaika1Click
        end
        object Kilpailijoidenpisteist1: TMenuItem
          Caption = 'Kilpailijoiden pisteist'#228
          OnClick = Kilpailijoidenpisteist1Click
        end
        object Sijalukusumma1: TMenuItem
          Caption = 'Sijalukusumma'
          OnClick = Sijalukusumma1Click
        end
        object Uudetpisteetsijoituksista1: TMenuItem
          Caption = 'Uudet pisteet sijoituksista'
          Checked = True
          OnClick = Uudetpisteetsijoituksista1Click
        end
      end
      object Joukkueentunnistavatieto1: TMenuItem
        Caption = 'Joukkueen tunnistava tieto'
        object Seura1: TMenuItem
          Caption = '&Seura'
          OnClick = Seura1Click
        end
        object Seuralyhenne1: TMenuItem
          Caption = 'Seura&lyhenne'
          OnClick = Seuralyhenne1Click
        end
        object Yhdistys1: TMenuItem
          Caption = '&Yhdistys'
          OnClick = Yhdistys1Click
        end
        object Joukkuekentt1: TMenuItem
          Caption = '&Joukkuekentt'#228
          Checked = True
          OnClick = Joukkuekentt1Click
        end
        object Piiri1: TMenuItem
          Caption = '&Piiri'
          OnClick = Piiri1Click
        end
      end
      object Joukkueenkoko1: TMenuItem
        Caption = 'Joukkueen koko/Sijoitukset'
        OnClick = Joukkueenkoko1Click
      end
      object Voittajallelispiste1: TMenuItem
        Caption = 'Voittajalle lis'#228'piste'
        OnClick = Voittajallelispiste1Click
      end
      object Sarjankokorajoittaavoittajanpisteit1: TMenuItem
        Caption = 'Sarjan koko rajoittaa voittajan pisteit'#228
        Checked = True
        OnClick = Sarjankokorajoittaavoittajanpisteit1Click
      end
      object Joukkuetietovaaditaan1: TMenuItem
        Caption = 'Joukkuetieto vaaditaan'
        Visible = False
        OnClick = Joukkuetietovaaditaan1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      OnClick = Help1Click
    end
  end
end
