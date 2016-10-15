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

#include "UnitPiirival.h"
#include "VDeclare.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPiiriVal *FormPiiriVal;

void luepiirit(wchar_t *flname);
extern wchar_t *piirinimi[];
extern piiriluku;
extern INT piirifl[];

//---------------------------------------------------------------------------
__fastcall TFormPiiriVal::TFormPiiriVal(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormPiiriVal::FormResize(TObject *Sender)
{
	LBPiirit->Height = ClientHeight - LBPiirit->Top - 6;
	LBPiiriVal->Height = LBPiirit->Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormPiiriVal::FormShow(TObject *Sender)
{
	memset(piirifl, 0, piiriluku * sizeof(int));
	LBPiirit->Clear();
	LBPiiriVal->Clear();
	for (int r = 0; r < piiriluku; r++) {
		swprintf(piirilista[r], L"%3d  %-50s", r+1, piirinimi[r]);
		LBPiirit->Items->Add(piirilista[r]);
	}
}
//---------------------------------------------------------------------------
int TFormPiiriVal::haepiiri(wchar_t *snimi)
{
	for (int r = 0; r < piiriluku; r++)
		if (!wcscmp(snimi, piirilista[r]))
			return(r);
	return(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormPiiriVal::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBPiirit->Items->Count; rv++) {
		if (LBPiirit->Selected[rv]) {
			int piiri;
			if ((piiri = haepiiri(LBPiirit->Items->Strings[rv].c_str())) >= 0)
				piirifl[piiri] = 1;
			}
		}
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBPiiriVal->Items->Count; rv++) {
		if (LBPiiriVal->Selected[rv]) {
			int piiri;
			if ((piiri = haepiiri(LBPiiriVal->Items->Strings[rv].c_str())) >= 0)
				piirifl[piiri] = 0;
			}
		}
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::BtnLisKaikkiClick(TObject *Sender)
{
	for (int piiri = 0; piiri < piiriluku; piiri++)
		piirifl[piiri] = 1;
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int piiri = 0; piiri < piiriluku; piiri++)
		piirifl[piiri] = 0;
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void TFormPiiriVal::NaytaPiirivalinnat(void)
{
	LBPiirit->Clear();
	LBPiiriVal->Clear();
	for (int piiri = 0; piiri < piiriluku; piiri++) {
		if (piirifl[piiri])
			LBPiiriVal->Items->Add(UnicodeString(piirilista[piiri]));
		else
			LBPiirit->Items->Add(UnicodeString(piirilista[piiri]));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::Button1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::LBPiiriValDblClick(TObject *Sender)
{
	int piiri;

	piiri = haepiiri(LBPiiriVal->Items->Strings[LBPiiriVal->ItemIndex].c_str());
	if (piiri >= 0)
		piirifl[piiri] = 0;
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::LBPiiritDblClick(TObject *Sender)
{
	int piiri;

	piiri = haepiiri(LBPiirit->Items->Strings[LBPiirit->ItemIndex].c_str());
	if (piiri >= 0)
		piirifl[piiri] = 1;
	NaytaPiirivalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiiriVal::Button2Click(TObject *Sender)
{
	OpenDialog1->FileName = FormMain->CurrentDir+UnicodeString(L"piirit.lst");
	OpenDialog1->DefaultExt = L"lst";
	OpenDialog1->Filter = L"Teksstitiedostot|*.TXT;*.LST|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		luepiirit(OpenDialog1->FileName.c_str());
		memset(piirifl, 0, piiriluku * sizeof(int));
		LBPiirit->Clear();
		LBPiiriVal->Clear();
		for (int r = 0; r < piiriluku; r++) {
			swprintf(piirilista[r], L"%3d  %-50s", r+1, piirinimi[r]);
			LBPiirit->Items->Add(piirilista[r]);
			}
		}
}
//---------------------------------------------------------------------------

