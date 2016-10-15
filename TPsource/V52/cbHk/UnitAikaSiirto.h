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

#ifndef UnitAikaSiirtoH
#define UnitAikaSiirtoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "HkDeclare.h"
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TFormAikaSiirto : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label2;
	TListBox *LBSarjat;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TListBox *LBSarjaVal;
	TLabel *Label6;
	TRadioGroup *RGKohde;
	TComboBox *CBVaiheet;
	TLabel *Label1;
	TButton *Button1;
	TButton *Button2;
	TEdit *EdtSiirto;
	TLabel *Label3;
	TRadioGroup *RGTulos;
	TComboBox *CBKohdeVaihe;
	TLabel *Label4;
	TBitBtn *BitBtn1;
	TLabel *LblSelostus;
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RGKohdeClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall CBVaiheetClick(TObject *Sender);
private:	// User declarations
	void NaytaSarjavalinnat(void);
	bool SarjaValinnat[MAXSARJALUKU];
	void Selostus(void);
public:		// User declarations
	__fastcall TFormAikaSiirto(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAikaSiirto *FormAikaSiirto;
//---------------------------------------------------------------------------
#endif
