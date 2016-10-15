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
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNollaus *FormNollaus;
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
	LblTila->Visible = false;
	for (int srj = 0; srj < sarjaluku; srj++) {
		LBSarjat->Items->Add(Sarjat[srj].sarjanimi);
//		LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
	CBOsuudet->Clear();
	CBOsuudet->Items->Add(L"Kaikki");
	for (int i = 0;i < kilpparam.n_os_akt; i++) {
	   CBOsuudet->Items->Add(UnicodeString(i+1)+L". osuus");
		}
	CBOsuudet->ItemIndex = 0;
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
	int epv, vpv, d;
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;

	LblTila->Visible = true;
	LblTila->Caption = L"Toiminto käynnissä";
	LblTila->Refresh();
	Sleep(100);
	EnterCriticalSection(&tall_CriticalSection);

	for (d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus != 0)
			continue;
		if (RGSarjat->ItemIndex == 1 && !LBSarjat->Selected[kilp.sarja])
			continue;
		if (CBOsuudet->ItemIndex > 0) {
			epv = CBOsuudet->ItemIndex-1;
			vpv = epv;
			}
		else {
			epv = 0;
			vpv = Sarjat[kilp.sarja].osuusluku - 1;
			}
		for (int Os = epv; Os <= vpv; Os++) {
			if (vaihdaetusuku) {
				wchar_t etu[80], suku[80];
				kilp.SukuNimi(etu, 78, Os);
				kilp.EtuNimi(suku, 78, Os);
				kilp.setNimi(etu, suku, Os);
				continue;
				}
			if (CBTulokset->Checked) {
				kilp.setMaali(Os, 0, TMAALI0);
				kilp.setSija(Os, 0, 0);
				kilp.ostiet[Os].ylahto = TMAALI0;
				kilp.ostiet[Os].ossija = 0;
				if (!((kilp.Tark(Os, 0) == 'P' && CBPoissa->Checked) ||
					(kilp.Tark(Os, 0) == 'E' && CBEil->Checked))) {
					kilp.SetTark(Os, '-');
					}
				memset(kilp.txt, 0, sizeof(kilp.txt));
				kilp.ostiet[Os].kone = 0;
				kilp.ostiet[Os].sakko = 0;
				kilp.ostiet[Os].seuranta = 0;
				memset(kilp.ostiet[Os].selitys, 0, sizeof(kilp.ostiet[Os].selitys));
				memset(kilp.ostiet[Os].ampsakot, '-', sizeof(kilp.ostiet[Os].ampsakot)-1);
				kilp.ostiet[Os].ampsakot[sizeof(kilp.ostiet[Os].ampsakot)-1] = 0;
				}
			if (CBPisteet->Checked)
				memset(kilp.pisteet, 0, sizeof(kilp.pisteet));
			if (CBFlags->Checked)
				memset(kilp.flag, 0, sizeof(kilp.flag));
			if (CBVa->Checked) {
				for (int i = 1; i <= kilpparam.valuku; i++) {
					kilp.setMaali(Os, i, TMAALI0);
					kilp.setSija(Os, i, 0);
					}
				}
			if (CBEmit->Checked) {
				kilp.ostiet[Os].badge[0] = 0;
				kilp.ostiet[Os].laina[0] = 0;
				kilp.ostiet[Os].badge[1] = 0;
				kilp.ostiet[Os].laina[1] = 0;
				}
			if (CBRadat->Checked) {
				memset(kilp.ostiet[Os].hajonta, 0, sizeof(kilp.ostiet[Os].hajonta));
				}
			}
		tallenna(&kilp, d, 0, 0, 0, NULL);
		}
	kirjheti = kirjheti0;
	LblTila->Caption = L"Toiminto valmis";
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
static void KerroKaikkiAjat(int kerroin, int jakaja)
{
/*
	kilptietue kilp;

	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d <= datf2.numrec; d++) {
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
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormNollaus::Kerrokaikkiajatkymmenell1Click(TObject *Sender)
{
//	KerroKaikkiAjat(10, 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::Jaakaikkiajatkymmenell1Click(TObject *Sender)
{
//	KerroKaikkiAjat(1, 10);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::PoistamiljoonatClick(TObject *Sender)
{
	kilptietue kilp;
	int epv, vpv, d;
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;

	LblTila->Visible = true;
	LblTila->Caption = L"Toiminto käynnissä";
	LblTila->Refresh();
	Sleep(100);
	EnterCriticalSection(&tall_CriticalSection);

	for (d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus != 0)
			continue;
		if (RGSarjat->ItemIndex == 1 && !LBSarjat->Selected[kilp.sarja])
			continue;
		if (CBOsuudet->ItemIndex > 0) {
			epv = CBOsuudet->ItemIndex-1;
			vpv = epv;
			}
		else {
			epv = 0;
			vpv = Sarjat[kilp.sarja].osuusluku - 1;
			}
		for (int Os = epv; Os <= vpv; Os++) {
			while (kilp.ostiet[Os].badge[0] > BADGEASKEL)
				kilp.ostiet[Os].badge[0] -= BADGEASKEL;
			}
		tallenna(&kilp, d, 0, 0, 0, NULL);
		}
	kirjheti = kirjheti0;
	LblTila->Caption = L"Toiminto valmis";
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormNollaus::VaihdaetusukuClick(TObject *Sender)
{
	vaihdaetusuku = true;
	Button1Click(Sender);
	vaihdaetusuku = false;
}
//---------------------------------------------------------------------------

