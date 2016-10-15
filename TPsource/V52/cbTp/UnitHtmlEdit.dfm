object FormHtmlEdit: TFormHtmlEdit
  Left = 0
  Top = 0
  Caption = 'FormHtmlEdit'
  ClientHeight = 467
  ClientWidth = 658
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 8
    Top = 43
    Width = 648
    Height = 419
    ActivePage = TabHtml
    TabOrder = 0
    object TabText: TTabSheet
      Caption = 'Muokkaus'
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object MemoText: TMemo
        Left = 4
        Top = 7
        Width = 628
        Height = 377
        ScrollBars = ssBoth
        TabOrder = 0
      end
    end
    object TabHtml: TTabSheet
      Caption = 'Katselu'
      ImageIndex = 1
      OnShow = TabHtmlShow
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object WebBrowser1: TWebBrowser
        Left = 3
        Top = 8
        Width = 629
        Height = 380
        TabOrder = 0
        ControlData = {
          4C00000002410000462700000000000000000000000000000000000000000000
          000000004C000000000000000000000001000000E0D057007335CF11AE690800
          2B2E126208000000000000004C0000000114020000000000C000000000000046
          8000000000000000000000000000000000000000000000000000000000000000
          00000000000000000100000000000000000000000000000000000000}
      end
    end
  end
  object RGLuonne: TRadioGroup
    Left = 8
    Top = 3
    Width = 290
    Height = 33
    Caption = ' Tekstin luonne'
    Columns = 2
    ItemIndex = 1
    Items.Strings = (
      'Itsen'#228'inen tiedosto'
      'Osa laajempaa tiedostoa')
    TabOrder = 1
  end
  object Button1: TButton
    Left = 344
    Top = 8
    Width = 137
    Height = 25
    Caption = 'Tallenna ja ota k'#228'ytt'#246#246'n'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 504
    Top = 8
    Width = 121
    Height = 25
    Caption = 'Poistu tallentamatta'
    TabOrder = 3
    OnClick = Button2Click
  end
  object MainMenu1: TMainMenu
    Left = 272
    Top = 24
    object Tiedosto1: TMenuItem
      Caption = 'Tiedosto'
      object Luetiedosto1: TMenuItem
        Caption = '&Lue tiedosto'
        OnClick = Luetiedosto1Click
      end
      object Tallenna1: TMenuItem
        Caption = '&Tallenna'
        OnClick = Tallenna1Click
      end
      object allennanimell1: TMenuItem
        Caption = 'Tallenna &nimell'#228
        OnClick = allennanimell1Click
      end
      object Sulje1: TMenuItem
        Caption = '&Sulje'
      end
    end
    object Merkist1: TMenuItem
      Caption = '&Merkist'#246
      object UTF81: TMenuItem
        Caption = 'UTF-8'
        Checked = True
        OnClick = UTF81Click
      end
      object ISO885911: TMenuItem
        Caption = 'ISO-8859-1'
        OnClick = ISO885911Click
      end
    end
  end
  object OpenDialog1: TOpenDialog
    Left = 200
    Top = 16
  end
  object SaveDialog1: TSaveDialog
    Left = 144
    Top = 16
  end
end
