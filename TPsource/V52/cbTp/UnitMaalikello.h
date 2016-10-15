// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//---------------------------------------------------------------------------

#ifndef UnitMaalikelloH
#define UnitMaalikelloH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#ifdef MAXOSUUSLUKU
#include <VDeclare.h>
#else
#include <HkDeclare.h>
#endif
//---------------------------------------------------------------------------
class TFormMaalikello : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabYhteydet;
	TTabSheet *TabOhjaus;
	TGroupBox *GroupBox4;
	TGroupBox *GBLaite2;
	TLabel *LblYhteys1;
	TLabel *LblYhteys2;
	TButton *BtnLaite1Avaa;
	TButton *BtnLaite1Sulje;
	TButton *BtnLaite2Avaa;
	TButton *BtnLaite2Sulje;
	TLabel *Label13;
	TLabel *Label18;
	TGroupBox *GBS4;
	TRadioGroup *RGUusinta;
	TEdit *EdAlku;
	TEdit *EdLoppu;
	TLabel *Label19;
	TLabel *Label20;
	TButton *BtnS4Uudelleen;
	TGroupBox *GBRTR2;
	TEdit *EdViive;
	TLabel *Label21;
	TButton *BtnViive;
	TButton *BtnTyhjenna;
	TRadioGroup *RGRTR2Uusinta;
	TLabel *Label25;
	TEdit *EdLukum;
	TButton *Button1;
	TPanel *Panel1;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *LbLaiteIP1;
	TLabel *LbLaite1Baud;
	TLabel *Label57;
	TLabel *Label1;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label16;
	TComboBox *CBLaite1;
	TComboBox *CBLaiteYht1;
	TEdit *EdLaitePort1;
	TEdit *EdLaiteIP1;
	TComboBox *CBLaiteBaud1;
	TEdit *EdAikaEro1;
	TEdit *ECC1;
	TEdit *ECX1;
	TEdit *ECK1;
	TEdit *EC01;
	TEdit *EC11;
	TEdit *EC21;
	TEdit *EC31;
	TEdit *EC41;
	TEdit *EC51;
	TEdit *EC61;
	TEdit *EC71;
	TEdit *EC81;
	TEdit *EJC1;
	TEdit *EJX1;
	TEdit *EJK1;
	TEdit *EJ01;
	TEdit *EJ11;
	TEdit *EJ21;
	TEdit *EJ31;
	TEdit *EJ41;
	TEdit *EJ51;
	TEdit *EJ61;
	TEdit *EJ71;
	TEdit *EJ81;
	TPanel *Panel2;
	TLabel *Label32;
	TLabel *Label27;
	TLabel *Label28;
	TLabel *Label29;
	TLabel *LbLaiteIP2;
	TLabel *Label55;
	TLabel *Label59;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label58;
	TLabel *Label56;
	TLabel *Label54;
	TLabel *Label53;
	TLabel *Label52;
	TLabel *Label51;
	TLabel *Label50;
	TLabel *Label49;
	TLabel *Label48;
	TLabel *Label17;
	TEdit *EdLaite2No;
	TComboBox *CBLaite2;
	TComboBox *CBLaiteYht2;
	TEdit *EdLaitePort2;
	TEdit *EdLaiteIP2;
	TEdit *EdAikaEro2;
	TEdit *ECC2;
	TEdit *ECX2;
	TEdit *ECK2;
	TEdit *EC02;
	TEdit *EC12;
	TEdit *EC22;
	TEdit *EC32;
	TEdit *EC42;
	TEdit *EC52;
	TEdit *EC62;
	TEdit *EC72;
	TEdit *EC82;
	TEdit *EJC2;
	TEdit *EJX2;
	TEdit *EJK2;
	TEdit *EJ02;
	TEdit *EJ12;
	TEdit *EJ22;
	TEdit *EJ32;
	TEdit *EJ42;
	TEdit *EJ52;
	TEdit *EJ62;
	TEdit *EJ72;
	TEdit *EJ82;
	TLabel *Label26;
	TEdit *Edit1;
	TBitBtn *BitBtn1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CBLaite1Change(TObject *Sender);
	void __fastcall CBLaiteYht1Change(TObject *Sender);
	void __fastcall CBLaite2Change(TObject *Sender);
	void __fastcall CBLaiteYht2Change(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnLaite1SuljeClick(TObject *Sender);
	void __fastcall BtnLaite2SuljeClick(TObject *Sender);
	void __fastcall BtnLaite1AvaaClick(TObject *Sender);
	void __fastcall BtnLaite2AvaaClick(TObject *Sender);
	void __fastcall RGUusintaClick(TObject *Sender);
	void __fastcall BtnS4UudelleenClick(TObject *Sender);
	void __fastcall BtnViiveClick(TObject *Sender);
	void __fastcall BtnTyhjennaClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall RGRTR2UusintaClick(TObject *Sender);
	void __fastcall EJExit(TObject *Sender);
	void __fastcall ECExit(TObject *Sender);
	void __fastcall EdLaite2NoExit(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	void __fastcall LaiteAvaa(int laite);
	TComboBox *CBLaite[2];
	TComboBox *CBLaiteYht[2];
	TEdit *EdLaitePort[2];
	TEdit *EdLaiteIP[2];
	TEdit *EdAikaEro[2];
	TLabel *LblYhteys[2];
	TEdit *EC[2][12];
	TEdit *EJ[2][12];
	TPanel *Panel[2];
	TButton *BtnLaiteAvaa[2];
public:		// User declarations
	__fastcall TFormMaalikello(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMaalikello *FormMaalikello;
//---------------------------------------------------------------------------
#endif
