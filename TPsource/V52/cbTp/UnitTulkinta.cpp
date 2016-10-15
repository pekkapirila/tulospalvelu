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

#include "UnitTulkinta.h"
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTulkinta *FormTulkinta;

//---------------------------------------------------------------------------
__fastcall TFormTulkinta::TFormTulkinta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	SG1->ColWidths[1] = 120*Screen->PixelsPerInch / 96;
	SG1->ColWidths[2] = 200*Screen->PixelsPerInch / 96;
	SG1->ColWidths[3] = 200*Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Kenttä";
	SG1->Cells[1][0] = L"Sisältö";
	SG1->Cells[2][0] = L"Luettu";
	SG1->Cells[3][0] = L"Tulkittu";
}
//---------------------------------------------------------------------------

void __fastcall TFormTulkinta::NaytaTulkinta(rivitulkintatp *RiviTulk, int nRivi, int rNo, int rTot, const wchar_t *sanoma)
{
	if (RiviTulk == NULL)
		return;
	if (sanoma && sanoma[0]) {
		LblSanoma->Width = SG1->Width;
		LblSanoma->Caption = sanoma;
		LblSanoma->Visible = true;
		}
	else
		LblSanoma->Visible = false;
	EdtTietue->Text = UnicodeString(rNo) + L" / " + UnicodeString(rTot);
	SG1->RowCount = nRivi + 1;
	for (int r = 0; r < nRivi; r++) {
		SG1->Cells[0][r+1] = RiviTulk[r].no;
		SG1->Cells[1][r+1] = RiviTulk[r].nimi;
		SG1->Cells[2][r+1] = RiviTulk[r].teksti;
		SG1->Cells[3][r+1] = RiviTulk[r].arvo;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormTulkinta::BtnHyvaksyClick(TObject *Sender)
{
	Vastaus = 1;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormTulkinta::BtnLueloputClick(TObject *Sender)
{
	Vastaus = 2;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormTulkinta::BtnOhitaClick(TObject *Sender)
{
	Vastaus = 3;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormTulkinta::BtnKeskeytaClick(TObject *Sender)
{
	Vastaus = 99;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

