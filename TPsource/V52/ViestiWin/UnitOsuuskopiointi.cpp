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

#include "UnitOsuuskopiointi.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOsuuskopiointi *FormOsuuskopiointi;
//---------------------------------------------------------------------------
__fastcall TFormOsuuskopiointi::TFormOsuuskopiointi(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormOsuuskopiointi::FormShow(TObject *Sender)
{
	LBSarjat->Clear();
	LblTila->Visible = false;
	for (int srj = 0; srj < sarjaluku; srj++) {
		LBSarjat->Items->Add(Sarjat[srj].sarjanimi);
		}
	CBOsuudet->Clear();
	for (int i = 0;i < kilpparam.n_os_akt; i++) {
	   CBOsuudet->Items->Add(UnicodeString(i+1)+L". osuus");
		}
	CBOsuudet->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormOsuuskopiointi::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
}
//---------------------------------------------------------------------------
void __fastcall TFormOsuuskopiointi::Button1Click(TObject *Sender)
{
	kilptietue kilp;
	int Os, d, kohdeos[MAXOSUUSLUKU];
	wchar_t kohdestr[200], *p;
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;

	LblTila->Visible = true;
	LblTila->Caption = L"Toiminto käynnissä";
	LblTila->Refresh();
	Sleep(100);
	EnterCriticalSection(&tall_CriticalSection);

	Os = CBOsuudet->ItemIndex;
	memset(kohdeos, 0, sizeof(kohdeos));
	wcsncpy(kohdestr, EdtKohdeos->Text.c_str(), sizeof(kohdestr)/2-1);
	p = wcstok(kohdestr, L" ,;/\t");
	while (p) {
		int k = _wtoi(p);
		if (k > 0 && k <= kilpparam.osuusluku)
			kohdeos[k-1] = 1;
		p = wcstok(NULL, L" ,;/\t");
		}
	for (d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus != 0)
			continue;
		if (RGSarjat->ItemIndex == 1 && !LBSarjat->Selected[kilp.sarja])
			continue;
		if (Os >= Sarjat[kilp.sarja].osuusluku)
			continue;
		for (int k_os = 0; k_os < Sarjat[kilp.sarja].osuusluku; k_os++) {
			if (kohdeos[k_os] == 0)
				continue;
			if (CBNimi->Checked) {
				memcpy(kilp.ostiet[k_os].nimi, kilp.ostiet[Os].nimi, sizeof(kilp.ostiet[Os].nimi));
				}
			if (CBTunnKdi->Checked) {
				kilp.ostiet[k_os].badge[0] = kilp.ostiet[Os].badge[0];
				kilp.ostiet[k_os].badge[1] = kilp.ostiet[Os].badge[1];
				kilp.ostiet[k_os].laina[0] = kilp.ostiet[Os].laina[0];
				kilp.ostiet[k_os].laina[1] = kilp.ostiet[Os].laina[1];
				}
			if (CBSeura->Checked) {
				memcpy(kilp.ostiet[k_os].osseura, kilp.ostiet[Os].osseura, sizeof(kilp.ostiet[Os].osseura));
				}
			if (CBId->Checked) {
				kilp.ostiet[k_os].lisno = kilp.ostiet[Os].lisno;
				}
			if (CBArvo->Checked) {
				memcpy(kilp.ostiet[k_os].arvo, kilp.ostiet[Os].arvo, sizeof(kilp.ostiet[Os].arvo));
				}
			if (CBHajonta->Checked) {
				memcpy(kilp.ostiet[k_os].hajonta, kilp.ostiet[Os].hajonta, sizeof(kilp.ostiet[Os].hajonta));
				}
			}
		tallenna(&kilp, d, 0, 0, 0, NULL);
		}
	kirjheti = kirjheti0;
	LblTila->Caption = L"Toiminto valmis";
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TFormOsuuskopiointi::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuuskopiointi::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Parisprintti");
}
//---------------------------------------------------------------------------

