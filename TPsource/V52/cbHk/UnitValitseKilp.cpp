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
#include <System.DateUtils.hpp>

#include "WinHk.h"
#include "UnitValitseKilp.h"
#include "UnitKisaLuett.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormValitseKilp *FormValitseKilp;
static kisaLuettTp kisaLuett[100];
static int kilpLkm;

//---------------------------------------------------------------------------
__fastcall TFormValitseKilp::TFormValitseKilp(TComponent* Owner)
	: TForm(Owner)
{
	if (FormKisaLuett == NULL) {
		FormKisaLuett = new TFormKisaLuett(this);
	}
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormValitseKilp::BtnValittuClick(TObject *Sender)
{
	int ix;

	ix = CBKilpailu->ItemIndex - 1;
	if (ix < 0) {
		Application->MessageBoxW(L"Kilpailua ei valittu", L"Ei valittu", MB_OK);
		return;
		}
	if (kisaLuett[ix].polku[wcslen(kisaLuett[ix].polku)-1] == L'\\')
		kisaLuett[ix].polku[wcslen(kisaLuett[ix].polku)-1] = 0;
	Path = UnicodeString(kisaLuett[ix].polku);
	Title = kisaLuett[ix].nimi;
	wcsncpy(kilpailu, Title.c_str(), sizeof(kilpailu)/2-14);
	wcscat(kilpailu, L" - ");
	stdDateStr(kilpailu+wcslen(kilpailu), kisaLuett[ix].date);
	pvparam[0].Date = kisaLuett[ix].date;
	if (!ilmTila) {
		if (wcsstr(kisaLuett[ix].cfgfile, L"\\") == NULL)
			cfgFName = Path+L"\\"+kisaLuett[ix].cfgfile;
		else
			cfgFName = UnicodeString(kisaLuett[ix].cfgfile);
		}
	else
		cfgFName = Path+UnicodeString(L"\\ilmoitt.cfg");
	ModalResult = mrYes;
}
//---------------------------------------------------------------------------
void __fastcall TFormValitseKilp::BtnMuuClick(TObject *Sender)
{
	ModalResult = mrNo;
}
//---------------------------------------------------------------------------
void __fastcall TFormValitseKilp::BtnPeruutaClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TFormValitseKilp::FormDestroy(TObject *Sender)
{
	delete FormKisaLuett;
	FormKisaLuett = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormValitseKilp::FormShow(TObject *Sender)
{
	int idate;

	CBKilpailu->Clear();
	CBKilpailu->Items->Add(L"Ei valittu");
	ValitseKilp();
}
//---------------------------------------------------------------------------
void __fastcall TFormValitseKilp::ValitseKilp(void)
{
	wchar_t st[60];
	int idate = -1, td;

	td = floor(Date());
	kilpLkm = sizeof(kisaLuett)/sizeof(kisaLuett[0]);
	FormKisaLuett->lueKilpLuett((FormMain->InitDir+UnicodeString(L"\\")+fnameRacelist).c_str(), kisaLuett, &kilpLkm);
	if (kilpLkm > 0) {
		for (int k = 0; k < kilpLkm; k++) {
			memset(st, 0, sizeof(st));
			stdDateStr(st, kisaLuett[k].date);
			wcscpy(st+wcslen(st), L": ");
			wcsncpy(st+wcslen(st), kisaLuett[k].nimi, sizeof(st)/2-wcslen(st)-1);
			CBKilpailu->Items->Add(UnicodeString(st));
			if (idate == -1 && kisaLuett[k].date >= td)
				idate = k;
			}
		if (idate == -1)
			idate = kilpLkm-1;
		CBKilpailu->ItemIndex = idate+1;
		}
	if (Avaa && idate > -1 && td == kisaLuett[idate].date) {
		BtnValittuClick(this);
    }
}
//---------------------------------------------------------------------------

