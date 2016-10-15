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

#include "UnitUusinta.h"
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormUusinta *FormUusinta;

extern void emitva_uusi_sarja(int cn, int srj, int osuus);
void uusintaTCPvalitut(int srj, wchar_t cp, int alku, int loppu, int cn);

//---------------------------------------------------------------------------
__fastcall TFormUusinta::TFormUusinta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Button5->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::Button1Click(TObject *Sender)
{
	int ta, ty, cna, cny;

   if (CBYhteydet->ItemIndex < 0) {
		Application->MessageBoxW(L"Valitse lähetettävät sanomat", L"Valitse", MB_OK);
		return;
		}
   if (RGTapa->ItemIndex == 2 || RGTapa->ItemIndex == 3) {
		if (CBYhteydet->ItemIndex <= 0) {
			Application->MessageBoxW(L"Valitse yksi yhteys", L"Valitse", MB_OK);
			return;
			}
		if (RGTapa->ItemIndex == 2) {
			emitva_uusi_sarja(CBYhteydet->ItemIndex - 1, CBSarjat->ItemIndex-1, CBOsuudet->ItemIndex-1);
			return;
			}
		else {
			int alku = TMAALI0, loppu = TMAALI0;
			if (EdtAlku->Text.Trim().Length() > 0)
				alku = wstrtoaika_vap(EdtAlku->Text.Trim().c_str(), t0);
			if (EdtLoppu->Text.Trim().Length() > 0)
				loppu = wstrtoaika_vap(EdtLoppu->Text.Trim().c_str(), t0);
			uusintaTCPvalitut(CBSarjat->ItemIndex-1, L"TVMHL"[RGRajaus->ItemIndex],
				alku, loppu, CBYhteydet->ItemIndex - 1);
			}
		}
   if (CBYhteydet->ItemIndex == 0) {
		cna = 0;
		cny = maxyhteys-1;
		}
   else {
		cna = CBYhteydet->ItemIndex - 1;
		cny = cna;
		}
   if (RGTapa->ItemIndex == 0) {
		ta = -24 * 36000L;
		ty = 24 * 36000L;
		}
   else {
		ty = t_time_l(biostime(0, 0), t0);
		ta = ty - _wtoi(EdtRaja->Text.c_str()) * 600L;
		if (ta < -12*36000L)
		ta += 24*36000L;
		}
   merk_uusinta(cna, cny, KSEK*ta, KSEK*ty, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::FormShow(TObject *Sender)
{
	CBYhteydet->Items->Clear();
	CBYhteydet->Items->Add(L"Kaikki yhteydet");
	for	(int i = 0; i < maxyhteys; i++) {
		CBYhteydet->Items->Add(L"Yhteys "+UnicodeString(i+1));
	}
	CBYhteydet->ItemIndex = 0;
	if (sulkusalasana[0])
		Panel1->Visible = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::Button4Click(TObject *Sender)
{
	wchar_t str[80] = L"", ch;
	char tunnus[4] = "";

	inputstr_prompt(str, 20, L"Anna sulkusalasana", &ch, this);
	if (ch == ESC)
		return;
	if (wcscmpU(str, sulkusalasana)) {
		Application->MessageBoxW(L"Väärä salasana", L"Virhe", MB_OK);
		return;
		}
	if (EdtSulku->Text.Length() > 0 && EdtSulku->Text != L"Kaikki")
		wcstooem((char *)tunnus,EdtSulku->Text.c_str(), 2);
	lahetasulku(tunnus, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::Button3Click(TObject *Sender)
{
	int yno, maxno;
	if (CBYhteydet->ItemIndex == 0) {
		yno = 0;
		maxno = maxyhteys-1;
		}
	else {
		yno = CBYhteydet->ItemIndex - 1;
		maxno = yno;
		}
	for (;yno <= maxno; yno++) {
		switch (RGTila->ItemIndex) {
			case 0:
				lahcom[yno] = 1;
				lahcomserver[yno] = 1;
				vastcom[yno] = 1;
				keyclose[yno] = 0;
				lah_alku[yno] = 2;
				break;
			case 1:
				if (keyclose[yno]) {
					lahcomserver[yno] = 0;
					keyclose[yno] = 0;
					}
				vastcom[yno] = 1;
				break;
			case 2:
				if (keyclose[yno]) {
					vastcom[yno] = 0;
					keyclose[yno] = 0;
					}
				lahcom[yno] = 1;
				lahcomserver[yno] = 1;
				lah_alku[yno] = 2;
				break;
			case 3:
				keyclose[yno] = 1;
				break;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::CBYhteydetChange(TObject *Sender)
{
	if (CBYhteydet->ItemIndex == 0)
		RGTila->ItemIndex = -1;
	else {
		int yht = CBYhteydet->ItemIndex - 1;
		if (!comopen[yht])
			RGTila->ItemIndex = -1;
		else {
			if (lahcom[yht] && lahcomserver[yht]) {
				if (vastcom[yht])
					RGTila->ItemIndex = 0;
				else
					RGTila->ItemIndex = 2;
				}
			else {
				if (vastcom[yht])
					RGTila->ItemIndex = 1;
				else
					RGTila->ItemIndex = 3;
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormUusinta::RGTapaClick(TObject *Sender)
{
	RGRajaus->Visible = false;
	GBAikavali->Visible = false;
	switch (RGTapa->ItemIndex) {
		case 0:
			Label2->Visible = false;
			Label4->Visible = false;
			EdtRaja->Visible = false;
			CBSarjat->Visible = false;
			CBOsuudet->Visible = false;
			break;
		case 1:
			Label2->Caption = L"Rajaava aika minuutteina";
			Label2->Visible = true;
			Label4->Visible = false;
			EdtRaja->Visible = true;
			CBSarjat->Visible = false;
			CBOsuudet->Visible = false;
			break;
		case 2:
		case 3:
			Label2->Caption = L"Lähetettävät sarjat";
			Label2->Visible = true;
			EdtRaja->Visible = false;
			CBSarjat->Clear();
			CBSarjat->Items->Add(L"Kaikki sarjat");
			for (int srj = 0; srj < sarjaluku; srj++)
				CBSarjat->Items->Add(Sarjat[srj].sarjanimi);
			CBSarjat->ItemIndex = 0;
			CBSarjat->Visible = true;
#ifdef MAXOSUUSLUKU
			Label4->Visible = true;
			CBOsuudet->Clear();
			CBOsuudet->Items->Add(L"Kaikki osuudet");
			for (int os = 1; os < kilpparam.ntosuus; os++)
				CBOsuudet->Items->Add(UnicodeString(os)+L". osuus");
			CBOsuudet->Visible = true;
#endif
			if (RGTapa->ItemIndex == 2)
				break;
			RGRajaus->Visible = true;
			GBAikavali->Visible = true;
			break;
		}
	CBOsuudet->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void kaynnistatesti(int kielto);

void __fastcall TFormUusinta::Button5Click(TObject *Sender)
{
	kaynnistatesti(0);
}
//---------------------------------------------------------------------------

