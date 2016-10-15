object FormLisaTietue: TFormLisaTietue
  Left = 0
  Top = 0
  Caption = 'Lis'#228#228' Emit-tietue'
  ClientHeight = 242
  ClientWidth = 380
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 56
    Top = 173
    Width = 23
    Height = 13
    Caption = 'Rata'
  end
  object RGPohjustus: TRadioGroup
    Left = 16
    Top = 32
    Width = 137
    Height = 57
    Caption = ' Tietueen pohjustustapa '
    ItemIndex = 0
    Items.Strings = (
      'Aiempi tietue'
      'Radan rastikoodit')
    TabOrder = 0
    OnClick = RGPohjustusClick
  end
  object EdAiempi: TLabeledEdit
    Left = 97
    Top = 109
    Width = 56
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 132
    EditLabel.Height = 13
    EditLabel.Caption = 'Aimman tietueen Emit-koodi'
    TabOrder = 1
    Text = '0'
  end
  object EdAiempiKno: TLabeledEdit
    Left = 97
    Top = 149
    Width = 56
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 119
    EditLabel.Height = 13
    EditLabel.Caption = 'Aimman tietueen kilpailija'
    TabOrder = 2
    Text = '0'
  end
  object EdLuenta: TLabeledEdit
    Left = 257
    Top = 109
    Width = 56
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 136
    EditLabel.Height = 13
    EditLabel.Caption = 'Luentahetki (jos muutetaan)'
    TabOrder = 3
    Text = '  :  :  '
  end
  object CBRata: TComboBox
    Left = 56
    Top = 189
    Width = 97
    Height = 21
    Enabled = False
    TabOrder = 4
  end
  object EdUusiEmit: TLabeledEdit
    Left = 257
    Top = 68
    Width = 56
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 126
    EditLabel.Height = 13
    EditLabel.Caption = 'Uuden tietueen Emit-koodi'
    TabOrder = 5
    Text = '0'
  end
  object Button1: TButton
    Left = 208
    Top = 147
    Width = 105
    Height = 25
    Caption = 'Lis'#228#228' tietue'
    TabOrder = 6
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 208
    Top = 185
    Width = 105
    Height = 25
    Caption = 'Peruuta ja poistu'
    TabOrder = 7
    OnClick = Button2Click
  end
end
