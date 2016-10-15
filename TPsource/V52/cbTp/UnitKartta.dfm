object FormKartta: TFormKartta
  Left = 0
  Top = 0
  Caption = 'Kartta'
  ClientHeight = 656
  ClientWidth = 768
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object ScrHor: TScrollBar
    Left = 0
    Top = 637
    Width = 745
    Height = 17
    PageSize = 0
    Position = 50
    TabOrder = 0
    OnKeyDown = ScrVertKeyDown
    OnScroll = ScrHorScroll
  end
  object ScrVert: TScrollBar
    Left = 748
    Top = 0
    Width = 17
    Height = 637
    Kind = sbVertical
    PageSize = 0
    Position = 50
    TabOrder = 1
    OnKeyDown = ScrVertKeyDown
    OnScroll = ScrVertScroll
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 747
    Height = 637
    TabOrder = 2
    OnMouseMove = Panel1MouseMove
    object Image1: TImage
      Left = -1
      Top = 0
      Width = 748
      Height = 637
      OnMouseDown = Image1MouseDown
      OnMouseMove = Image1MouseMove
      OnMouseUp = Image1MouseUp
    end
  end
  object MainMenu1: TMainMenu
    Left = 432
    object iedosto1: TMenuItem
      Caption = '&Tiedosto'
      object Lataakartta1: TMenuItem
        Caption = 'Lataa kartta'
        OnClick = Lataakartta1Click
      end
      object Suljekaavake1: TMenuItem
        Caption = '&Sulje kaavake'
        OnClick = Suljekaavake1Click
      end
    end
    object Koko1: TMenuItem
      Caption = '&Asetukset'
      object Pienet: TMenuItem
        Caption = '&Pienet symbolit'
        OnClick = PienetClick
      end
      object Keskikok: TMenuItem
        Caption = 'Keskikok. symbolit'
        Checked = True
        OnClick = KeskikokClick
      end
      object Suuretsymbolit1: TMenuItem
        Caption = 'Suuret symbolit'
        OnClick = Suuretsymbolit1Click
      end
      object Tummatvirheet1: TMenuItem
        Caption = '&Tummat virheet'
        OnClick = Tummatvirheet1Click
      end
    end
    object Nyt1: TMenuItem
      Caption = '&N'#228'yt'#228
      object Rastit1: TMenuItem
        Caption = 'Ra&stit'
        OnClick = Rastit1Click
      end
      object Rata1: TMenuItem
        Caption = 'Ra&ta'
        OnClick = Rata1Click
      end
    end
    object Kartankohdistus1: TMenuItem
      Caption = '&Kartan kohdistus'
      object Arvaakohdistus1: TMenuItem
        Caption = '&Arvaa kohdistus'
        Visible = False
      end
      object Kohdistakahdenrastinavulla1: TMenuItem
        Caption = 'Kohdista rastien avulla'
        OnClick = Kohdistakahdenrastinavulla1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      OnClick = Help1Click
    end
  end
  object OpenPictureDialog1: TOpenPictureDialog
    Left = 472
  end
end
