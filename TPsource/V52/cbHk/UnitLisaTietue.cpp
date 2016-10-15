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

#include "UnitLisaTietue.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLisaTietue *FormLisaTietue;
//---------------------------------------------------------------------------
__fastcall TFormLisaTietue::TFormLisaTietue(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTietue::RGPohjustusClick(TObject *Sender)
{
	switch (RGPohjustus->ItemIndex) {
		case 0:
			EdAiempi->Enabled = true;
			EdAiempiKno->Enabled = true;
			CBRata->Enabled = false;
			CBRata->ItemIndex = -1;
			CBRata->Text = L"";
			break;
		case 1:
			EdAiempi->Enabled = false;
			EdAiempiKno->Enabled = false;
			EdAiempi->Text = L"0";
			EdAiempiKno->Text = L"0";
			CBRata->Enabled = true;
			CBRata->ItemIndex = 0;
			break;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTietue::Button1Click(TObject *Sender)
{
	emittp em;
	int kno, bdg, ip, ubdg, t;
	ratatp *rt;
	extern ekditp *kuvio;
	extern int nkuvio;
	wchar_t tstr[20] = L"";

	ubdg = _wtoi(EdUusiEmit->Text.c_str());
	em.nollaa();
	switch (RGPohjustus->ItemIndex) {
		case 0:
			bdg = _wtoi(EdAiempi->Text.c_str());
			kno = _wtoi(EdAiempiKno->Text.c_str());
			if ((bdg > 0 && kno > 0) || (bdg <= 0 && kno <= 0)) {
				Application->MessageBoxW(L"Anna joko aiempi koodi tai aiempi kilpailija (ei molempia)", L"Virhe", MB_OK);
				return;
				}
			if (bdg > 0)
				kno = bdg2kno(bdg);
			ip = getEmitJarr(kno, NULL);
			if (ip < 0) {
				Application->MessageBoxW(L"Aiempaa tietuetta ei löydy", L"Virhe", MB_OK);
				return;
				}
			em.get(ip);
			if (ubdg) {
				em.badge = ubdg;
				em.badge0 = ubdg;
				}
			else {
				if (em.badge != em.badge0)
					em.badge = em.badge0;
				else {
					Application->MessageBoxW(L"Anna uusi emit-koodi", L"Virhe", MB_OK);
					return;
					}
				}
			if (getEmitJarr(bdg2kno(em.badge), NULL) >= 0) {
				Application->MessageBoxW(L"Uutta emit-koodia vastaava tietue jo olemassa. Lisäys peruttu.", L"Este", MB_OK);
				return;
				}
			em.kilpno = 0;
			em.maali = TMAALI0/KSEK;
			break;
		case 1:
			if (ubdg <= 0) {
				Application->MessageBoxW(L"Anna uusi emit-koodi", L"Virhe", MB_OK);
				return;
				}
			rt = rata+CBRata->ItemIndex;
			for (int i = 0; i < rt->rastiluku; i++) {
				UCHAR code = 0;
				for (int j = 0; j < nkuvio; j++) {
					if (kuvio[j].rasti == rt->rastikoodi[i]) {
						code = kuvio[j].koodi;
						break;
						}
					}
				em.ctrltime[i+1] = i+1;
				em.ctrlcode[i+1] = code;
				}
			em.ctrltime[rt->rastiluku+1] = 3600;
			em.ctrlcode[rt->rastiluku+1] = 250;
			em.time = t_time_l(biostime(0,0), t0);
			em.badge = ubdg;
			em.badge0 = ubdg;
			break;
		 }
	if (EdLuenta->Text.Length() > 4) {
		wcsncpy(tstr, EdLuenta->Text.c_str(), 14);
		if ((t = wstrtoaika_vap(tstr, t0)) != TMAALI0)
			em.time = t/KSEK;
		}
	em.kirjaus = TMAALI0/KSEK;
	addem(&em, 0);
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTietue::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTietue::FormShow(TObject *Sender)
{
	ratatp *rt;

	CBRata->Items->Clear();
	for (rt = rata; rt < rata+nrata; rt++)
		CBRata->Items->Add(rt->tunnus);
	CBRata->ItemIndex = 0;
}
//---------------------------------------------------------------------------
