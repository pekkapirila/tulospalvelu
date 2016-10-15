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

#include <vcl.h>
#pragma hdrstop

#include "UnitSarjaVal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSarjaVal *FormSarjaVal;
//---------------------------------------------------------------------------
__fastcall TFormSarjaVal::TFormSarjaVal(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjaVal::FormShow(TObject *Sender)
{
	CBSarja->Clear();
	for	(int srj = 0; srj < sarjaluku; srj++) {
#ifndef MAXOSUUSLUKU
		if (ToimintaTila < 2 ||
			Sarjat[srj].luonne[k_pv] >= 0)
//			(Sarjat[srj].enspv <= k_pv  && Sarjat[srj].viimpv >= k_pv))
#endif
		CBSarja->Items->Add(Sarjat[srj].sarjanimi);
		}
	CBSarja->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjaVal::BtnHyvaksyClick(TObject *Sender)
{
#ifdef MAXOSUUSLUKU
	Sarja = CBSarja->ItemIndex;
#else
	Sarja = haesarja_w(CBSarja->Text.c_str(), false);
#endif
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjaVal::BtnPeruutaClick(TObject *Sender)
{
	Sarja = -1;
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
