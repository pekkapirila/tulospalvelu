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

#include "UnitJjVaihdot.h"
#include "VDeclare.h"
#include "UnitJoukkuetiedot.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormJjVaihdot *FormJjVaihdot;
//---------------------------------------------------------------------------
__fastcall TFormJjVaihdot::TFormJjVaihdot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::BtnPeruutaClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::BtnToteutaClick(TObject *Sender)
{
	ModalResult = 101;
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::BtnEtuSukuClick(TObject *Sender)
{
	ModalResult = 102;
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::BtnJaaNimiClick(TObject *Sender)
{
	ModalResult = 103;
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::FormShow(TObject *Sender)
{
	SG1->ColWidths[0] = 140 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Nimi";
	SG1->Cells[1][0] = L"Uusi osuus";
}
//---------------------------------------------------------------------------
void __fastcall TFormJjVaihdot::FormResize(TObject *Sender)
{
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormJjVaihdot::BtnIsoalkuClick(TObject *Sender)
{
	ModalResult = 104;
}
//---------------------------------------------------------------------------

