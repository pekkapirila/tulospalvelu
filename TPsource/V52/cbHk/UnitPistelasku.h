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

#ifndef UnitPistelaskuH
#define UnitPistelaskuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormPistelasku : public TForm
{
__published:	// IDE-managed Components
	TListBox *LBSarjat;
	TButton *BtnPoisKaikki;
	TButton *BtnPoisValitut;
	TButton *BtnLisValitut;
	TButton *BtnLisKaikki;
	TListBox *LBSarjaVal;
	TLabel *Label6;
	TLabel *Label2;
	TBitBtn *BitBtn1;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TButton *BtnLaskeVaihe;
	TButton *BtnLaskeYhtPisteet;
	TEdit *EdVaiheLkm;
	TLabel *Label3;
	TEdit *EdA1;
	TEdit *EdL1;
	TLabel *Label4;
	TEdit *EdP1;
	TLabel *Label5;
	TEdit *EdD1;
	TLabel *Label7;
	TEdit *EdA2;
	TEdit *EdL2;
	TEdit *EdP2;
	TEdit *EdD2;
	TEdit *EdA3;
	TEdit *EdL3;
	TEdit *EdP3;
	TEdit *EdD3;
	TLabel *LblLkm;
	TLabel *Label1;
	TLabel *Label8;
	TLabel *Label9;
	TGroupBox *GroupBox3;
	TEdit *EdtOsOttoMin;
	TLabel *Label10;
	TEdit *EdtHyvMin;
	TLabel *Label12;
	TEdit *EdtVoittopist;
	TLabel *Label13;
	TComboBox *ComboBox1;
	TLabel *Label11;
	TLabel *Label14;
	TLabel *Label15;
	TEdit *EditA;
	TEdit *EditB;
	TCheckBox *CBEiNeg;
	TRadioGroup *RGPyor;
	TLabel *Label16;
	TEdit *EdA4;
	TEdit *EdL4;
	TEdit *EdP4;
	TEdit *EdD4;
	TLabel *Label18;
	TEdit *EdA5;
	TEdit *EdL5;
	TEdit *EdP5;
	TEdit *EdD5;
	TCheckBox *CBMolemmat;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *allennasnnttiedostoon1;
	TMenuItem *Luesnnttiedostosta1;
	TMenuItem *Help1;
	TCheckBox *CBVertAika;
	TEdit *EdtVertAika;
	TLabel *Label17;
	TButton *BtnLaskeKaikki;
	TMenuItem *Haeedellisenvaiheenkaavat1;
	TCheckBox *CBpisteetOn;
	TCheckBox *CByhteispisteetOn;
	TCheckBox *CByhtpkaikille;
	TCheckBox *CBYPisteTls;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TMenuItem *aulukkonytt1;
	TMenuItem *Kopioitmnvaiheenkaavattoisellevaiheelle1;
	TCheckBox *CBJoukkNimi;
	TMenuItem *Aiempivaihe1;
	TMenuItem *Laskeaiempivaihe1;
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall BtnLaskeVaiheClick(TObject *Sender);
	void __fastcall BtnLaskeYhtPisteetClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall allennasnnttiedostoon1Click(TObject *Sender);
	void __fastcall Luesnnttiedostosta1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall BtnLaskeKaikkiClick(TObject *Sender);
	void __fastcall Haeedellisenvaiheenkaavat1Click(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall CBVertAikaClick(TObject *Sender);
	void __fastcall aulukkonytt1Click(TObject *Sender);
	void __fastcall Kopioitmnvaiheenkaavattoisellevaiheelle1Click(TObject *Sender);
	void __fastcall Laskeaiempivaihe1Click(TObject *Sender);

private:	// User declarations
	void NaytaSarjavalinnat(void);
	void HaeKertoimet(pistekaavatp *kaava);
	void NaytaKertoimet(pistekaavatp *kaava);
	bool SarjaValinnat[MAXSARJALUKU+MAXYHD];
	pistekaavatp kv;
public:		// User declarations
	bool SarjaValittu(int Srj);
	__fastcall TFormPistelasku(TComponent* Owner);
//	int Pisteet(pistekaavatp *kva, int sj, int n, int tls, int karki);
//	void __fastcall LaskeYhtPisteet(pistekaavatp *kva, int srj);
//	void __fastcall LaskeVaiheSarja(pistekaavatp *kv, int srj);
//	void __fastcall BtnLaskeKaikkiClick(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPistelasku *FormPistelasku;
//---------------------------------------------------------------------------
#endif
