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

#include "UnitAikakorjaus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAikakorjaus *FormAikakorjaus;
//---------------------------------------------------------------------------
__fastcall TFormAikakorjaus::TFormAikakorjaus(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::BtnSuljeClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::BtnKopioi1Click(TObject *Sender)
{
	EdtSiirto2->Text = EdtSiirto1->Text;
	EdtAika1->Text = L"00:00:00,000";
	EdtAika2->Text = L"23:59:59,999";
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::FormShow(TObject *Sender)
{

	CBJono->Clear();
	if (maxjono) {
		for (int r = 0; r <= maxjono; r++) {
			CBJono->Items->Add(L"Jono "+UnicodeString(r+1));
			}
		}
#ifndef MAXOSUUSLUKU
	else if (lajatfl) {
		CBJono->Items->Add(L"Aika");
		CBJono->Items->Add(L"Lähtö");
		}
#endif
	else {
		CBJono->Items->Add(L"Jono 1");
		CBJono->Enabled = false;
		}
	CBJono->ItemIndex = Jono;
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::EdtAika1Exit(TObject *Sender)
{
	wchar_t st[40];

	EdtAika1->Text = aikatowstr_cols_n(st, wstrtoaika_vap(EdtAika1->Text.c_str(), t0), t0, L',', 12);
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::EdtAika2Exit(TObject *Sender)
{
	wchar_t st[40];

	EdtAika2->Text = aikatowstr_cols_n(st, wstrtoaika_vap(EdtAika2->Text.c_str(), t0), t0, L',', 12);
}
//---------------------------------------------------------------------------
int TFormAikakorjaus::siirto(int t)
{
	__int64 s;

	s = __int64(T2-t)*S1 + __int64(t-T1)*S2;
	s = s/(T2-T1);
	return(INT32(s));
}
//---------------------------------------------------------------------------
void __fastcall TFormAikakorjaus::BtnToteutaClick(TObject *Sender)
{
	int r1, r2, uptr;
	aikatp *vpt, upt;

	T1 = wstrtoaika_vap(EdtAika1->Text.c_str(), t0);
	T2 = wstrtoaika_vap(EdtAika2->Text.c_str(), t0);
	Jono = CBJono->ItemIndex;
	if (T1 == T2)
		return;
	S1 = _wtoi(EdtSiirto1->Text.c_str());
	S2 = _wtoi(EdtSiirto2->Text.c_str());
	r1 = _wtoi(EdtEns->Text.c_str()) - 1;
	if (r1 < 0)
		r1 = 0;
	r2 = _wtoi(EdtViim->Text.c_str());
	if (r2 <= r1)
		return;
	if (r1 > 0 && r1 < aikajono[Jono]->aktrows)
		r1 = aikajono[Jono]->aktrow[r1]-1;
	if (r2 > aikajono[Jono]->aktrows)
		r2 = aikajono[Jono]->rwtime;
	else
		r2 = aikajono[Jono]->aktrow[r2]-1;
	vpt = new aikatp[r2-r1];
	for (int r = r1; r < r2; r++) {
		aikajono[Jono]->getTime(vpt+r-r1, r);
		}
	for (int r = 0; r < r2-r1; r++) {
		upt = vpt[r];
		upt.t = NORMKELLO_A(vpt[r].t + AIKAJAK*siirto(purajak(vpt[r].t)));
		upt.date = tm_copydate(upt.t, vpt[r].t, vpt[r].date);
#ifdef MAXOSUUSLUKU
		tall_rivi(Jono, &upt, vpt+r, &uptr, 0, 0, 0);
		if (upt.status == 0 && upt.kno && upt.piste >= -1) {
			kilptietue kilp;
			int d;
			EnterCriticalSection(&tall_CriticalSection);
			if ((d = getpos(upt.kno)) > 0) {
				kilp.getrec(d);
				if (upt.osuus < Sarjat[kilp.sarja].osuusluku && upt.piste <= Sarjat[kilp.sarja].valuku[upt.osuus] &&
					abs((int) NORMKELLO(kilp.Maali(upt.osuus, upt.piste)-purajak(vpt[r].t))) < SEK) {
					kilp.setMaali(upt.osuus, upt.piste, purajak(upt.t));
					tallenna(&kilp, d, 0, 0, 0, 0);
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
#else
		tall_rivi(Jono, &upt, vpt+r, &uptr, 0, 0, 0, false);
		if (upt.status == 0 && upt.kno && upt.piste >= -1) {
			kilptietue kilp;
			int d;
			EnterCriticalSection(&tall_CriticalSection);
			if ((d = getpos(upt.kno)) > 0) {
				kilp.GETREC(d);
				if (upt.piste <= Sarjat[kilp.Sarja()].valuku[k_pv]) {
					kilp.set_tulos(upt.piste, purajak(upt.t), true);
					kilp.tallenna(d, 0, 0, 0, 0);
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
#endif
		}
	delete[] vpt;
}
//---------------------------------------------------------------------------
