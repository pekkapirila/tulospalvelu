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

#include "UnitNollaus.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormNollaus *FormNollaus;
//---------------------------------------------------------------------------
__fastcall TFormNollaus::TFormNollaus(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::FormShow(TObject *Sender)
{
	LBSarjat->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
	CBVaiheet->Clear();
	CBVaiheet->Items->Add(L"Kaikki");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBVaiheet->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	CBVaiheet->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::Button1Click(TObject *Sender)
{
	kilptietue kilp;
	int epv, vpv;

	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		if (RGSarjat->ItemIndex == 1 && !LBSarjat->Selected[kilp.Sarja()])
			continue;
		if (CBVaiheet->ItemIndex > 0) {
			epv = CBVaiheet->ItemIndex-1;
			vpv = epv;
			}
		else {
			epv = Sarjat[kilp.Sarja()].enspv;
			vpv = Sarjat[kilp.Sarja()].viimpv;
			}
		for (int Pv = epv; Pv <= vpv; Pv++) {
			if (CBLahto->Checked) {
				kilp.tall_lajat_pv(TMAALI0, Pv);
				}
			if (CBTulokset->Checked) {
				kilp.tall_tulos_pv(0, Pv);
				kilp.pv[Pv].va[0].vatulos = kilp.pv[Pv].tlahto;
				kilp.pv[Pv].va[1].vasija = 0;
				if (!((kilp.tark(Pv) == L'P' && CBPoissa->Checked) ||
					(kilp.tark(Pv) == L'E' && CBEil->Checked) ||
					(kilp.tark(Pv) == L'V' && CBVak->Checked))) {
					kilp.set_tark(L"-EV"[RGMerkinta->ItemIndex], Pv);
					}
				memset(kilp.pv[Pv].txt, 0, sizeof(kilp.pv[Pv].txt));
				kilp.pv[Pv].uusi = 0;
				kilp.pv[Pv].vac = 0;
				kilp.pv[Pv].flags[0] = 0;
				kilp.pv[Pv].flags[1] = 0;
				memset(kilp.pv[Pv].selitys, 0, sizeof(kilp.pv[Pv].selitys));
				wmemset(kilp.pv[Pv].asakot, L'-', sizeof(kilp.pv[Pv].asakot)/2-1);
				kilp.pv[Pv].asakot[sizeof(kilp.pv[Pv].asakot)/2-1] = 0;
				kilp.pv[Pv].tasoitus = 0;
				}
			if (CBVa->Checked) {
				for (int i = 0; i < kilpparam.valuku; i++) {
					kilp.pv[Pv].va[i+2].vatulos = 0;
					kilp.pv[Pv].va[i+2].vasija = 0;
					}
				}
			if (CBEmit->Checked) {
				kilp.pv[Pv].badge[0] = 0;
				kilp.pv[Pv].badge[1] = 0;
				kilp.pv[Pv].laina[0] = 0;
				kilp.pv[Pv].laina[1] = 0;
				}
			if (CBRadat->Checked) {
				memset(kilp.pv[Pv].rata, 0, sizeof(kilp.pv[Pv].rata));
				}
			}
		kilp.tallenna(d, 0, 0, 0, NULL);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toimenpide tehty", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
static void KerroKaikkiAjat(int kerroin, int jakaja)
{
	kilptietue kilp;

	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		for (int Pv = 0; Pv < kilpparam.n_pv_akt; Pv++) {
			kilp.pv[Pv].tlahto = kilp.pv[Pv].tlahto * kerroin / jakaja;
			for (int i = 0; i < kilpparam.valuku+2; i++) {
				kilp.pv[Pv].va[i].vatulos = kilp.pv[Pv].va[i].vatulos * kerroin / jakaja;
				}
			}
		kilp.tallenna(d, 0, 0, 0, NULL);
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::Kerrokaikkiajatkymmenell1Click(TObject *Sender)
{
	KerroKaikkiAjat(10, 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::Jaakaikkiajatkymmenell1Click(TObject *Sender)
{
	KerroKaikkiAjat(1, 10);
}
//---------------------------------------------------------------------------

static void Nimimuoto(int laji)
{
	kilptietue kilp;

	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		switch (laji) {
			case 0:
				locasewname(kilp.sukunimi);
				break;
			case 1:
				upcasewstr(kilp.sukunimi);
				break;
			case 2:
				locasewname(kilp.etunimi);
				break;
			}
		kilp.tallenna(d, 0, 0, 0, NULL);
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::SUKUNIMIisoillakirjaimilla1Click(TObject *Sender)
{
	Nimimuoto(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::Sukunimiisollaalkukirjaimella1Click(TObject *Sender)

{
	Nimimuoto(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::Etunimiisollaalkukirjaimella1Click(TObject *Sender)

{
	Nimimuoto(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Tulosten nollaus");
}
//---------------------------------------------------------------------------

