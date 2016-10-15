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

#include "UnitHenkHaku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormHenkHaku *FormHenkHaku;
//---------------------------------------------------------------------------
__fastcall TFormHenkHaku::TFormHenkHaku(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::FormShow(TObject *Sender)
{
	SG1->ColCount = 5;
	SG1->RowCount = 2;
	SG1->ColWidths[0] = 45;
	SG1->ColWidths[1] = 60;
	SG1->ColWidths[2] = 140;
	SG1->ColWidths[3] = 40;
	SG1->ColWidths[4] = 160;
	SG1->Cells[0][0] = L"Kilpno";
	SG1->Cells[1][0] = L"Sarja";
	SG1->Cells[2][0] = L"Joukkue";
	SG1->Cells[3][0] = L"Osuus";
	SG1->Cells[4][0] = L"Nimi";
	CBOsuudet->Clear();
	CBOsuudet->Items->Add(L"Kaikki osuudet");
	for (int os = 0; os < kilpparam.osuusluku; os++)
		CBOsuudet->Items->Add(UnicodeString(os+1) + L". osuus");
	CBOsuudet->ItemIndex = 0;
	NaytaNimet();
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::NaytaNimet(void)
{
	int n = 0, ntot = 0, osuus, etusuku;
	nimettp *nimet;

	osuus = CBOsuudet->ItemIndex-1;
	etusuku = 2*RGJarj->ItemIndex-1;
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (osuus < 0)
			ntot += nilm[srj]*Sarjat[srj].osuusluku;
		else if (osuus < Sarjat[srj].osuusluku)
			ntot += nilm[srj];
		}
	nimet = new nimettp[ntot];
	for (int d = 1; d < datf2.numrec; d++) {
		kilptietue kilp;

		kilp.getrec(d);
		if (kilp.kilpstatus || kilp.Tark(0) == 'P')
			continue;
		if (osuus >= Sarjat[kilp.sarja].osuusluku)
			continue;
		for (int os = (osuus < 0 ? 0 : osuus); os < (osuus < 0 ? Sarjat[kilp.sarja].osuusluku : osuus + 1); os++) {
			kilp.Nimi(nimet[n].nimi, sizeof(nimet[n].nimi)/2-1, os, etusuku);
			if (wcslen(nimet[n].nimi) < 2)
				continue;
			wcstoansi(nimet[n].animi, nimet[n].nimi, sizeof(nimet[n].animi)-1);
			upcasestr(nimet[n].animi);
			kilp.Joukkue(nimet[n].joukkue, sizeof(nimet[n].joukkue)/2-1);
			nimet[n].sarja = kilp.sarja;
			nimet[n].kno = kilp.kilpno;
			nimet[n].os = os;
			n++;
			}
		}
	SG1->RowCount = n+1;
	if (n) {
		quicksort((char *)nimet, n, sizeof(nimet[0].animi), sizeof(nimet[0]));
		for (int r = 1; r <= n; r++) {
			SG1->Cells[0][r] = nimet[r-1].kno;
			SG1->Cells[1][r] = Sarjat[nimet[r-1].sarja].sarjanimi;
			SG1->Cells[2][r] = nimet[r-1].joukkue;
			SG1->Cells[3][r] = nimet[r-1].os+1;
			SG1->Cells[4][r] = nimet[r-1].nimi;
			}
		}
	delete[] nimet;
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::CBOsuudetChange(TObject *Sender)
{
	NaytaNimet();
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::RGJarjClick(TObject *Sender)
{
	NaytaNimet();
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - SG1->Left - 4 * Screen->PixelsPerInch / 96;
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::Button1Click(TObject *Sender)
{
	if (SG1->Row > 0) {
		KilpNo = _wtoi(SG1->Cells[0][SG1->Row].c_str());
		Osuus = _wtoi(SG1->Cells[3][SG1->Row].c_str());
		}
	else {
		KilpNo = 0;
		Osuus = 0;
		}
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::Edit1Change(TObject *Sender)
{
	int r = 0;
	for (r = 1; r < SG1->RowCount-1; r++)
		if (wcscmp(SG1->Cells[4][r].UpperCase().c_str(), Edit1->Text.UpperCase().c_str()) > 0)
			break;
	SG1->Row = r;
}
//---------------------------------------------------------------------------
void __fastcall TFormHenkHaku::SG1DblClick(TObject *Sender)
{
	Button1Click(Sender);
}
//---------------------------------------------------------------------------
