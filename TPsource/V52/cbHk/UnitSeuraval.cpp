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

#include "UnitSeuraval.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSeuraVal *FormSeuraVal;
//---------------------------------------------------------------------------
__fastcall TFormSeuraVal::TFormSeuraVal(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuraVal::FormResize(TObject *Sender)
{
	LBSeurat->Height = ClientHeight - LBSeurat->Top - 6;
	LBSeuraVal->Height = LBSeurat->Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuraVal::FormShow(TObject *Sender)
{
	seuralista = new wchar_t[nilmt+10][LSEURA+5];
	SeuraValinnat = new int[nilmt+10];

	memset(SeuraValinnat, 0, (nilmt+10) * sizeof(int));
	HaeSeuraLista(sralaji, CBMaa->Checked, k_pv+1, seuralista, &nsra, nilmt+10);
	LBSeurat->Clear();
	LBSeuraVal->Clear();
	for (int r = 0; r < nsra; r++) {
		LBSeurat->Items->Add(seuralista[r]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuraVal::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete[] seuralista;
	delete[] SeuraValinnat;
	seuralista = NULL;
	SeuraValinnat = NULL;
	nsra = 0;
}
//---------------------------------------------------------------------------
int TFormSeuraVal::haeseura(wchar_t *snimi)
{
	for (int r = 0; r < nsra; r++)
		if (!wcscmp(snimi, seuralista[r]))
			return(r);
	return(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuraVal::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSeurat->Items->Count; rv++) {
		if (LBSeurat->Selected[rv]) {
			int sra;
			if ((sra = haeseura(LBSeurat->Items->Strings[rv].c_str())) >= 0)
				SeuraValinnat[sra] = true;
			}
		}
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSeuraVal->Items->Count; rv++) {
		if (LBSeuraVal->Selected[rv]) {
			int sra;
			if ((sra = haeseura(LBSeuraVal->Items->Strings[rv].c_str())) >= 0)
				SeuraValinnat[sra] = false;
			}
		}
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::BtnLisKaikkiClick(TObject *Sender)
{
	for (int sra = 0; sra < nsra; sra++)
		SeuraValinnat[sra] = true;
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int sra = 0; sra < nsra; sra++)
		SeuraValinnat[sra] = false;
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void TFormSeuraVal::NaytaSeuravalinnat(void)
{
	LBSeurat->Clear();
	LBSeuraVal->Clear();
	for (int sra = 0; sra < nsra; sra++) {
		if (SeuraValinnat[sra])
			LBSeuraVal->Items->Add(UnicodeString(seuralista[sra]));
		else
			LBSeurat->Items->Add(UnicodeString(seuralista[sra]));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::Button1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::LBSeuraValDblClick(TObject *Sender)
{
	int sra;

	sra = haeseura(LBSeuraVal->Items->Strings[LBSeuraVal->ItemIndex].c_str());
	if (sra >= 0)
		SeuraValinnat[sra] = false;
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::LBSeuratDblClick(TObject *Sender)
{
	int sra;

	sra = haeseura(LBSeurat->Items->Strings[LBSeurat->ItemIndex].c_str());
	if (sra >= 0)
		SeuraValinnat[sra] = true;
	NaytaSeuravalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuraVal::CBMaaClick(TObject *Sender)
{
	FormShow(Sender);
}
//---------------------------------------------------------------------------

