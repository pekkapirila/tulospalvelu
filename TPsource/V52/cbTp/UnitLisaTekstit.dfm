object FormLisaTekstit: TFormLisaTekstit
  Left = 0
  Top = 0
  Caption = 'FormLisaTekstit'
  ClientHeight = 336
  ClientWidth = 527
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 8
    Width = 120
    Height = 13
    Caption = 'Kirjoita kirjoitintulosteisiin'
  end
  object CheckBox1: TCheckBox
    Left = 32
    Top = 24
    Width = 153
    Height = 17
    Caption = 'Koko luettelon alkuteksti'
    TabOrder = 0
    OnClick = CheckBox1Click
  end
  object CheckBox2: TCheckBox
    Left = 32
    Top = 40
    Width = 185
    Height = 17
    Caption = 'Jokaisen muun sivun alkuteksti'
    TabOrder = 1
    OnClick = CheckBox2Click
  end
  object CheckBox3: TCheckBox
    Left = 32
    Top = 56
    Width = 137
    Height = 17
    Caption = 'Sivun alalaidan teksti'
    TabOrder = 2
    OnClick = CheckBox3Click
  end
  object CheckBox4: TCheckBox
    Left = 32
    Top = 73
    Width = 152
    Height = 17
    Caption = 'Koko luettelon lopputeksti'
    TabOrder = 3
    OnClick = CheckBox4Click
  end
  object MainMenu1: TMainMenu
    Left = 344
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Luetekstit1: TMenuItem
        Caption = '&Lue tekstit'
        OnClick = Luetekstit1Click
      end
      object allennatekstit1: TMenuItem
        Caption = '&Tallenna tekstit'
        OnClick = allennatekstit1Click
      end
      object Poistu1: TMenuItem
        Caption = '&Poistu'
        OnClick = Poistu1Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 384
  end
  object SaveDialog1: TSaveDialog
    Left = 432
  end
end
