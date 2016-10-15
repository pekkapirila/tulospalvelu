object FormMySQL: TFormMySQL
  Left = 0
  Top = 0
  Caption = 'MySQL yhteys'
  ClientHeight = 222
  ClientWidth = 407
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Arial'
  Font.Style = []
  OldCreateOrder = False
  PopupMode = pmAuto
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 14
  object Label1: TLabel
    Left = 262
    Top = 118
    Width = 40
    Height = 14
    Caption = 'Tietueita'
  end
  object Label2: TLabel
    Left = 262
    Top = 131
    Width = 42
    Height = 14
    Caption = 'osanotot'
  end
  object Label3: TLabel
    Left = 311
    Top = 131
    Width = 42
    Height = 14
    Caption = 'osottopv'
  end
  object Label4: TLabel
    Left = 360
    Top = 131
    Width = 23
    Height = 14
    Caption = 'tulos'
  end
  object Palvelin: TLabel
    Left = 21
    Top = 11
    Width = 36
    Height = 14
    Caption = 'Palvelin'
  end
  object Label5: TLabel
    Left = 21
    Top = 81
    Width = 45
    Height = 14
    Caption = 'Salasana'
  end
  object Label6: TLabel
    Left = 21
    Top = 58
    Width = 39
    Height = 14
    Caption = 'K'#228'ytt'#228'j'#228
  end
  object Label7: TLabel
    Left = 21
    Top = 35
    Width = 49
    Height = 14
    Caption = 'Tietokanta'
  end
  object Label8: TLabel
    Left = 231
    Top = 58
    Width = 128
    Height = 14
    Caption = 'Jatkuvan tiedonsiirron jono'
  end
  object Button1: TButton
    Left = 19
    Top = 113
    Width = 113
    Height = 25
    Caption = 'Avaa yhteys'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 19
    Top = 147
    Width = 113
    Height = 25
    Caption = 'Testaa yhteys'
    TabOrder = 1
    OnClick = Button2Click
  end
  object CBAvattu: TCheckBox
    Left = 150
    Top = 153
    Width = 97
    Height = 17
    Caption = 'Yhteys avattu'
    Enabled = False
    TabOrder = 2
  end
  object Edit1: TEdit
    Left = 262
    Top = 149
    Width = 41
    Height = 22
    Alignment = taRightJustify
    TabOrder = 3
  end
  object Edit2: TEdit
    Left = 311
    Top = 149
    Width = 41
    Height = 22
    Alignment = taRightJustify
    TabOrder = 4
  end
  object Edit3: TEdit
    Left = 360
    Top = 149
    Width = 41
    Height = 22
    Alignment = taRightJustify
    TabOrder = 5
  end
  object Button3: TButton
    Left = 262
    Top = 9
    Width = 113
    Height = 25
    Caption = 'Lataa parametrit'
    TabOrder = 6
    Visible = False
    OnClick = Button3Click
  end
  object Memo1: TMemo
    Left = 332
    Top = 40
    Width = 105
    Height = 31
    TabOrder = 7
    Visible = False
  end
  object EdtServer: TEdit
    Left = 103
    Top = 8
    Width = 121
    Height = 22
    TabOrder = 8
    Text = 'localhost'
  end
  object EdtPW: TEdit
    Left = 103
    Top = 78
    Width = 121
    Height = 22
    TabOrder = 9
    Text = 'hkkisawin'
  end
  object EdtUser: TEdit
    Left = 103
    Top = 55
    Width = 121
    Height = 22
    TabOrder = 10
    Text = 'kilp'
  end
  object EdtDB: TEdit
    Left = 103
    Top = 32
    Width = 121
    Height = 22
    TabOrder = 11
    Text = 'kilp'
  end
  object Button4: TButton
    Left = 19
    Top = 181
    Width = 113
    Height = 25
    Caption = 'Sulje yhteys'
    TabOrder = 12
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 288
    Top = 181
    Width = 113
    Height = 25
    Caption = 'Sulje kaavake'
    TabOrder = 13
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 354
    Top = 6
    Width = 75
    Height = 25
    Caption = 'Button6'
    TabOrder = 14
    Visible = False
    OnClick = Button6Click
  end
  object CBjatkuva: TCheckBox
    Left = 150
    Top = 185
    Width = 115
    Height = 17
    Caption = 'Jatkuva tiedonsiirto'
    Enabled = False
    TabOrder = 15
    OnClick = CBjatkuvaClick
  end
  object EdtJono: TEdit
    Left = 366
    Top = 55
    Width = 35
    Height = 22
    Alignment = taRightJustify
    ReadOnly = True
    TabOrder = 16
    Text = '0'
  end
  object CBVarmista: TCheckBox
    Left = 235
    Top = 80
    Width = 164
    Height = 17
    Caption = 'Varmista jatkuva tiedonsiirto'
    Checked = True
    State = cbChecked
    TabOrder = 17
  end
  object SQLConnection1: TSQLConnection
    ConnectionName = 'MYSQLCONNECTION'
    DriverName = 'MySQL'
    LoginPrompt = False
    Params.Strings = (
      'DriverName=MySQL'
      'HostName=ServerName'
      'Database=DBNAME'
      'User_Name=user'
      'Password=password'
      'ServerCharSet='
      'BlobSize=-1'
      'ErrorResourceFile='
      'LocaleCode=0000'
      'Compressed=False'
      'Encrypted=False'
      'ConnectTimeout=60')
    Left = 176
    Top = 8
  end
  object SQLDataSet1: TSQLDataSet
    MaxBlobSize = -1
    ParamCheck = False
    Params = <>
    SQLConnection = SQLConnection1
    Left = 176
    Top = 56
  end
  object OpenDialog1: TOpenDialog
    Left = 176
    Top = 104
  end
end
