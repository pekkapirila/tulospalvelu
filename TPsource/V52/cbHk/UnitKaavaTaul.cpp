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

#include "UnitKaavaTaul.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKaavaTaul *FormKaavaTaul;
//---------------------------------------------------------------------------
__fastcall TFormKaavaTaul::TFormKaavaTaul(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::NaytaTaul(void)
{
	wchar_t *lajit[4] = {L" ", L"Sij", L"Tul", L"Mol"};
	wchar_t *eion[2] = {L"Ei", L"On"}, ln[50];
	int sijOn = 1, tulOn = 1, eSrj, vSrj, e_pv, v_pv, r;

	sijOn = CBSij->Checked ? 1 : 0;
	tulOn = CBTls->Checked ? 1 : 0;
	SG1->ColCount = 16*sijOn+7*tulOn+10;
	SG1->Cells[0][0] = L"Sarja";
	SG1->Cells[1][0] = L"Vaihe";
	SG1->Cells[2][0] = L"Jatk";
	SG1->Cells[3][0] = L"Laji";
	if (sijOn) {
		SG1->Cells[4][0] = L"A1";
		SG1->Cells[5][0] = L"L1";
		SG1->Cells[6][0] = L"E1";
		SG1->Cells[7][0] = L"D1";
		SG1->Cells[8][0] = L"A2";
		SG1->Cells[9][0] = L"L2";
		SG1->Cells[10][0] = L"E2";
		SG1->Cells[11][0] = L"D2";
		SG1->Cells[12][0] = L"A3";
		SG1->Cells[13][0] = L"L3";
		SG1->Cells[14][0] = L"E3";
		SG1->Cells[15][0] = L"D3";
		SG1->Cells[16][0] = L"A4";
		SG1->Cells[17][0] = L"L4";
		SG1->Cells[18][0] = L"E4";
		SG1->Cells[19][0] = L"L4";
		}
	if (tulOn) {
		SG1->Cells[16*sijOn+4][0] = L"TulLaji";
		SG1->Cells[16*sijOn+5][0] = L"VertAika";
		SG1->Cells[16*sijOn+6][0] = L"V-Pist";
		SG1->Cells[16*sijOn+7][0] = L"Kerr";
		SG1->Cells[16*sijOn+8][0] = L"Jak";
		SG1->Cells[16*sijOn+9][0] = L"Pyör";
		SG1->Cells[16*sijOn+10][0] = L"EiNeg";
		}
	SG1->Cells[16*sijOn+7*tulOn+4][0] = L"Y-Jatk";
	SG1->Cells[16*sijOn+7*tulOn+5][0] = L"V-lkm";
	SG1->Cells[16*sijOn+7*tulOn+6][0] = L"OoMin";
	SG1->Cells[16*sijOn+7*tulOn+7][0] = L"HyvMin";
	SG1->Cells[16*sijOn+7*tulOn+8][0] = L"Kaik";
	SG1->Cells[16*sijOn+7*tulOn+9][0] = L"YpTulos";
	SG1->ColWidths[0] = 60 * Screen->PixelsPerInch / 96;
	if (sijOn) {
		for (int c = 4; c <= 19; c++)
			SG1->ColWidths[c] = 30 * Screen->PixelsPerInch / 96;
		}
	if (tulOn) {
		for (int c = 16*sijOn+4; c <= 16*sijOn+10; c++)
			SG1->ColWidths[c] = 50 * Screen->PixelsPerInch / 96;
		}
	for (int c = 16*sijOn+7*tulOn+4; c <= 16*sijOn+7*tulOn+9; c++)
		SG1->ColWidths[c] = 50 * Screen->PixelsPerInch / 96;
	if (CBVaihe->ItemIndex == 0) {
		e_pv = 0;
		v_pv = kilpparam.n_pv_akt-1;
		}
	else {
		e_pv = CBVaihe->ItemIndex-1;
		v_pv = e_pv;
		}
	if (CBSarja->ItemIndex == 0) {
		eSrj = 0;
		vSrj = sarjaluku-1;
		}
	else {
		eSrj = CBSarja->ItemIndex-1;
		vSrj = eSrj;
		}
	r = 0;
	SG1->RowCount = 2;
	for (int srj = eSrj; srj <= vSrj; srj++) {
		for (int ipv = e_pv; ipv <= v_pv; ipv++) {
			if (Sarjat[srj].luonne[ipv] < 0)
				continue;
			r++;
			if (SG1->RowCount < r+1)
				SG1->RowCount = r+1;
			SG1->Cells[0][r] = Sarjat[srj].sarjanimi;
			SG1->Cells[1][r] = ipv+1;
			SG1->Cells[2][r] = eion[Sarjat[srj].pistekaava[ipv].vaiheon];
			SG1->Cells[3][r] = lajit[Sarjat[srj].pistekaava[ipv].laji];
			if (sijOn) {
				SG1->Cells[4][r] = Sarjat[srj].pistekaava[ipv].a[0];
				SG1->Cells[5][r] = Sarjat[srj].pistekaava[ipv].y[0];
				SG1->Cells[6][r] = Sarjat[srj].pistekaava[ipv].p[0];
				SG1->Cells[7][r] = Sarjat[srj].pistekaava[ipv].d[0];
				SG1->Cells[8][r] = Sarjat[srj].pistekaava[ipv].a[1];
				SG1->Cells[9][r] = Sarjat[srj].pistekaava[ipv].y[1];
				SG1->Cells[10][r] = Sarjat[srj].pistekaava[ipv].p[1];
				SG1->Cells[11][r] = Sarjat[srj].pistekaava[ipv].d[1];
				SG1->Cells[12][r] = Sarjat[srj].pistekaava[ipv].a[2];
				SG1->Cells[13][r] = Sarjat[srj].pistekaava[ipv].y[2];
				SG1->Cells[14][r] = Sarjat[srj].pistekaava[ipv].p[2];
				SG1->Cells[15][r] = Sarjat[srj].pistekaava[ipv].d[2];
				SG1->Cells[16][r] = Sarjat[srj].pistekaava[ipv].a[3];
				SG1->Cells[17][r] = Sarjat[srj].pistekaava[ipv].y[3];
				SG1->Cells[18][r] = Sarjat[srj].pistekaava[ipv].p[3];
				SG1->Cells[19][r] = Sarjat[srj].pistekaava[ipv].d[3];
				}
			if (tulOn) {
				SG1->Cells[16*sijOn+4][r] = Sarjat[srj].pistekaava[ipv].plaji;
				if (Sarjat[srj].pistekaava[ipv].vaika == 0)
					SG1->Cells[16*sijOn+5][r] = L"Kärki";
				else {
					aikatowstr_cols_n(ln, Sarjat[srj].pistekaava[ipv].vaika, 0, 0, 8);
					SG1->Cells[16*sijOn+5][r] = ln;
					}
				SG1->Cells[16*sijOn+6][r] = Sarjat[srj].pistekaava[ipv].vpist;
				SG1->Cells[16*sijOn+7][r] = Sarjat[srj].pistekaava[ipv].kerr;
				SG1->Cells[16*sijOn+8][r] = Sarjat[srj].pistekaava[ipv].jak;
				SG1->Cells[16*sijOn+9][r] = eion[Sarjat[srj].pistekaava[ipv].optiot & 1];
				SG1->Cells[16*sijOn+10][r] = eion[(Sarjat[srj].pistekaava[ipv].optiot & 4)/4];
				}
			SG1->Cells[16*sijOn+7*tulOn+4][r] = eion[Sarjat[srj].pistekaava[ipv].yhtpiston & 1];
			SG1->Cells[16*sijOn+7*tulOn+5][r] = Sarjat[srj].pistekaava[ipv].yhtpistvaihelkm;
			SG1->Cells[16*sijOn+7*tulOn+6][r] = Sarjat[srj].pistekaava[ipv].yhtpistosottomin;
			SG1->Cells[16*sijOn+7*tulOn+7][r] = Sarjat[srj].pistekaava[ipv].yhtpisthyvmin;
			SG1->Cells[16*sijOn+7*tulOn+8][r] = eion[(Sarjat[srj].pistekaava[ipv].yhtpiston & 2) / 2];
			SG1->Cells[16*sijOn+7*tulOn+9][r] = eion[(Sarjat[srj].pistekaava[ipv].yhtpiston & 4) / 4];
			}
		}
}

//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::FormShow(TObject *Sender)
{
	CBSarja->Clear();
	CBSarja->Items->Add(L"Kaikki");
	for (int srj = 0; srj < sarjaluku; srj++)
		CBSarja->Items->Add(Sarjat[srj].sarjanimi);
	CBSarja->ItemIndex = 0;
	CBVaihe->Clear();
	CBVaihe->Items->Add(L"Kaikki");
	for (int ipv = 1; ipv <= kilpparam.n_pv_akt; ipv++)
		CBVaihe->Items->Add(UnicodeString(ipv)+L". vaihe");
	CBVaihe->ItemIndex = k_pv+1;
	NaytaTaul();
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::CBSarjaChange(TObject *Sender)
{
	NaytaTaul();
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::CBVaiheChange(TObject *Sender)
{
	NaytaTaul();
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::CBSijClick(TObject *Sender)
{
	NaytaTaul();
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::CBTlsClick(TObject *Sender)
{
	NaytaTaul();
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth;
	SG1->Height = ClientHeight - SG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormKaavaTaul::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Pistelaskenta");
}
//---------------------------------------------------------------------------

