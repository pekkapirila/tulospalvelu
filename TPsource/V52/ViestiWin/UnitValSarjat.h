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

#ifndef UnitValSarjatH
#define UnitValSarjatH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "VDeclare.h"

//---------------------------------------------------------------------------
class TFormValSarjat : public TForm
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
	TButton *Button2;
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	void NaytaSarjavalinnat(void);
public:		// User declarations
	__fastcall TFormValSarjat(TComponent* Owner);
	bool SarjaValinnat[MAXSARJALUKU+MAXYHD];
	int ValitutSarjat[MAXSARJALUKU+MAXYHD+1];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormValSarjat *FormValSarjat;
//---------------------------------------------------------------------------
#endif
