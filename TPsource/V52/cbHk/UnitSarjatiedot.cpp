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

#include "UnitSarjatiedot.h"
#include "UnitVaPisteet.h"
#include "UnitSarja.h"
#include "UnitTuloskaava.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSarjatiedot *FormSarjatiedot;
extern TFormSarja *FormSarja;
extern TFormVaPisteet *FormVaPisteet;
static int mukana = 0;

ColTp GridCols0[] = {
	{L"Nro", 30, 0, 1, 0, true},
	{L"Sarja", 55, 0, 1, 0, true},
	{L"AlkuNo", 55, 0, 1, 0, true},
	{L"Sakot/tulos", 60, 0, 1, 0, true},
	{L"EnsVaihe", 55, 0, 1, 0, false},
	{L"ViimVaihe", 55, 0, 1, 0, false},
	{L"Luonne", 40, 0, N_PV, 0, false},
	{L"BibAlku", 55, 0, N_PV, 0, false},
	{L"Lähtöväli", 55, 0, N_PV, 0, true},
	{L"Ens. lähtö", 60, 0, N_PV, 0, true},
	{L"Ens. j.ilm", 60, 0, N_PV, 0, false},
	{L"Lähtö", 40, 0, N_PV, 0, true},
	{L"Jono", 40, 0, N_PV, 0, false},
	{L"Maali", 40, 0, N_PV, 0, true},
	{L"Pari", 40, 0, N_PV, 0, false},
	{L"Amp.paikat", 60, 0, N_PV, 0, false},
	{L"Sakkoyks.", 60, 0, N_PV, 0, false},
	{L"Pisteitä", 60, 0, N_PV, 0, false},
	{L"Maksu", 60, 0, 1, 0, true},
	{L"Maksu-1", 60, 0, 1, 0, false},
	{L"Maksu-ale", 60, 0, 1, 0, false},
	{L"Matka", 60, 0, N_PV, 0, true},
	{L"Tapa", 40, 0, N_PV, 0, false},
	{L"Pitkä nimi", 180, 0, 1, 0, true},
	{L"Lukum", 40, 0, 1, 0, false},
	{L"Läsnä", 40, 0, N_PV, 0, false},
	{L"Viim.lähtö", 60, 0, N_PV, 0, false},
	{L"ViimNo", 40, 0, N_PV, 0, false}
};

ColTp *GridCols1;

ColTp GridColsArv[] = {
	{L"Nro", 30, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, true},
	{L"AlkuNo", 60, 0, 1, 0, true},
	{L"Sakot/tulos", 60, 0, 1, 0, false},
	{L"EnsPv", 60, 0, 1, 0, false},
	{L"ViimPv", 60, 0, 1, 0, false},
	{L"Luonne", 40, 0, N_PV, 0, false},
	{L"BibAlku", 55, 0, 1, 0, false},
	{L"Lähtöväli", 60, 0, 1, 0, true},
	{L"Ens. lähtö", 60, 0, 1, 0, true},
	{L"Ens. j.ilm", 60, 0, 1, 0, false},
	{L"Lähtö", 40, 0, 1, 0, true},
	{L"Jono", 40, 0, 1, 0, true},
	{L"Maali", 40, 0, 1, 0, false},
	{L"Pari", 40, 0, 1, 0, false},
	{L"Amp.paikat", 60, 0, 1, 0, false},
	{L"Amp.sakot", 60, 0, 1, 0, false},
	{L"Pisteitä", 60, 0, 1, 0, false},
	{L"Maksu", 60, 0, 1, 0, false},
	{L"Maksu-1", 60, 0, 1, 0, false},
	{L"Maksu-ale", 60, 0, 1, 0, false},
	{L"Matka", 60, 0, 1, 0, false},
	{L"Tapa", 40, 0, 1, 0, false},
	{L"Pitkä nimi", 180, 0, 1, 0, false},
	{L"Lukum", 40, 0, 1, 0, true},
	{L"Läsnä", 40, 0, 1, 0, true},
	{L"Viim.lähtö", 60, 0, 1, 0, true},
	{L"ViimNo", 40, 0, 1, 0, true}
};

ColTp GridColsPerus[] = {
	{L"Nro", 30, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, true},
	{L"AlkuNo", 60, 0, 1, 0, true},
	{L"Sakot/tulos", 60, 0, 1, 0, false},
	{L"EnsPv", 60, 0, 1, 0, false},
	{L"ViimPv", 60, 0, 1, 0, false},
	{L"Luonne", 40, 0, N_PV, 0, false},
	{L"BibAlku", 55, 0, 1, 0, false},
	{L"Lähtöväli", 60, 0, 1, 0, false},
	{L"Ens. lähtö", 60, 0, 1, 0, false},
	{L"Ens. j.ilm", 60, 0, 1, 0, false},
	{L"Lähtö", 40, 0, 1, 0, true},
	{L"Jono", 40, 0, 1, 0, false},
	{L"Maali", 40, 0, 1, 0, false},
	{L"Pari", 40, 0, 1, 0, false},
	{L"Amp.paikat", 60, 0, 1, 0, false},
	{L"Amp.sakot", 60, 0, 1, 0, false},
	{L"Pisteitä", 60, 0, 1, 0, false},
	{L"Maksu", 60, 0, 1, 0, false},
	{L"Maksu-1", 60, 0, 1, 0, false},
	{L"Maksu-ale", 60, 0, 1, 0, false},
	{L"Matka", 60, 0, 1, 0, false},
	{L"Tapa", 40, 0, 1, 0, false},
	{L"Pitkä nimi", 180, 0, 1, 0, false},
	{L"Lukum", 40, 0, 1, 0, true},
	{L"Läsnä", 40, 0, 1, 0, false},
	{L"Viim.lähtö", 60, 0, 1, 0, false},
	{L"ViimNo", 40, 0, 1, 0, true}
};

static int GridCols1Count;
static int GridColsArvCount;
static int GridColsPerusCount;
static int epv;

typedef struct {
	char   key[8];
	int    srj;
} srjjrjtp;

static srjjrjtp srjjrj[MAXSARJALUKU];
static int jrjsrj[MAXSARJALUKU];
static wchar_t *sakStr[] = {
	L" ",
	L"Lasten srj",
	L"Rastireitti",
	L"Tasoitukset",
	L"Sakot",
	L"Ei tuloksia"
};
static wchar_t sakChar[] = L" LRTSE";
static void teesarjajarj(int jrj);

//---------------------------------------------------------------------------

__fastcall TFormSarjatiedot::TFormSarjatiedot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	GridCols1 = new ColTp[sizeof(GridCols0)/sizeof(ColTp)];
	memcpy(GridCols1, GridCols0, sizeof(GridCols0));

	switch (kilpparam.kilplaji) {
		case L'S':
		case L'B':
//			GridCols1[9].Visible = true;
			GridCols1[13].Visible = false;
			GridCols1[14].Visible = true;
			break;
		case L'H':
//			GridCols1[9].Visible = false;
			GridCols1[13].Visible = false;
			GridCols1[14].Visible = false;
			break;
		case L'A':
//			GridCols1[9].Visible = false;
			GridCols1[13].Visible = true;
			GridCols1[14].Visible = true;
			break;
		}

	Alkunumero1->Checked = GridCols1[2].Visible;
	Sakollisuus1->Checked = GridCols1[3].Visible;
	N1vaihe1->Checked = GridCols1[4].Visible;
	Viimvaihe1->Checked = GridCols1[5].Visible;
	Bibalku1->Checked = GridCols1[6].Visible;
	Lhtvli1->Checked = GridCols1[7].Visible;
	Jlkiilmlht1->Checked = GridCols1[9].Visible;
	Lhtpaikka1->Checked = GridCols1[10].Visible;
	Maali1->Checked = GridCols1[12].Visible;
	Ryhmkoko1->Checked = GridCols1[13].Visible;
	Ampumapaikat1->Checked = GridCols1[14].Visible;
	Sakkoyksikk1->Checked = GridCols1[15].Visible;
	Maksut1->Checked = GridCols1[17].Visible;
	Matka1->Checked = GridCols1[20].Visible;
	Pitknimi1->Checked = GridCols1[22].Visible;
	if (DeveloperMode)
		Pakotasarjanimetisoillakirjaimilla1->Visible = true;
	pakotaIsot = Pakotasarjanimetisoillakirjaimilla1->Checked;
	aktrow = -1;
	aktcol = -1;
	teesarjajarj(0);
}
//---------------------------------------------------------------------------

static void teesarjajarj(int jrj)
{
	mukana = 0;
	switch (jrj) {
		case 0:
			for (int i = 0; i < sarjaluku; i++) {
				if (FormSarjatiedot->CBTiedot->ItemIndex < 2) {
					jrjsrj[i] = i;
					mukana++;
					}
				else {
					if (epv >= Sarjat[i].enspv && epv <= Sarjat[i].viimpv) {
						jrjsrj[mukana] = i;
						mukana++;
						}
					}
				srjjrj[i].srj = i;
				}
			break;
		case 1:
			for (int i = 0; i < sarjaluku; i++) {
				memset(srjjrj[i].key, 0, sizeof(srjjrj[i].key));
				if (FormSarjatiedot->CBTiedot->ItemIndex < 2) {
					*(int *)(srjjrj[i].key+4) = Sarjat[i].sarjaalku;
					}
				else {
					*(int *)(srjjrj[i].key+4) = Sarjat[i].bibalku[epv];
					}
				if (FormSarjatiedot->CBTiedot->ItemIndex >= 2 &&
					(epv < Sarjat[i].enspv || epv > Sarjat[i].viimpv))
					*(int *)(srjjrj[i].key) = 9999;
				else {
					mukana++;
					if (FormSarjatiedot->Otalhthuomioon1->Checked)
						*(int *)(srjjrj[i].key) = Sarjat[i].lno[epv];
					}
				lswab((long *)srjjrj[i].key);
				lswab((long *)(srjjrj[i].key+4));
				srjjrj[i].srj = i;
				}
			quicksort((char *)srjjrj, mukana, sizeof(srjjrj[0].key), sizeof(srjjrj[0]));
			for (int i = 0; i < mukana; i++) {
				jrjsrj[srjjrj[i].srj] = i;
				}
			break;
		case 2:
			for (int i = 0; i < sarjaluku; i++) {
				memset(srjjrj[i].key, 0, sizeof(srjjrj[i].key));
				if (epv >= Sarjat[i].enspv && epv <= Sarjat[i].viimpv) {
					*(int *)(srjjrj[i].key) = 65536*Sarjat[i].lno[epv]+Sarjat[i].portti[epv];
					mukana++;
					}
				else
					*(int *)(srjjrj[i].key) = 65536*9999;
				*(int *)(srjjrj[i].key+4) = Sarjat[i].enslahto[epv]+t0*TUNTI;
				lswab((long *)srjjrj[i].key);
				lswab((long *)(srjjrj[i].key+4));
				srjjrj[i].srj = i;
				}
			quicksort((char *)srjjrj, sarjaluku, sizeof(srjjrj[0].key), sizeof(srjjrj[0]));
			for (int i = 0; i < mukana; i++) {
				jrjsrj[srjjrj[i].srj] = i;
				}
			break;
		}
	if (mukana == 0)
		mukana = sarjaluku;
 }

//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::NaytaSarjat(void)
{
	wchar_t St[20];
	ColTp *GridCols;
	int nSrj = 0;

	LblMuokkaus->Enabled = SG1->Options.Contains(goEditing);
	LblMuokkaus->Visible = LblMuokkaus->Enabled;

	SG1->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		if (SG1->FixedCols > GridCols1Count)
			SG1->FixedCols = GridCols1Count;
		SG1->ColCount = GridCols1Count;
		epv = 0;
		}
	else if (CBTiedot->ItemIndex == 1) {
		GridCols = GridColsPerus;
		if (SG1->FixedCols > GridColsPerusCount)
			SG1->FixedCols = GridColsPerusCount;
		SG1->ColCount = GridColsPerusCount;
		epv = 0;
		}
	else {
		GridCols = GridColsArv;
		if (SG1->FixedCols > GridColsArvCount)
			SG1->FixedCols = GridColsArvCount;
		SG1->ColCount = GridColsArvCount;
		epv = CBTiedot->ItemIndex-2;
		}
	teesarjajarj(CBJarj->ItemIndex);

	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridCols[ic].Visible) {
			int n = 1;
			if (GridCols[ic].Count == N_PV)
				n = kilpparam.n_pv_akt;
			for (int i = 0; i < n; i++) {
				SG1->Cells[GridCols[ic].Order+i][0] = UnicodeString(GridCols[ic].Caption);
				SG1->ColWidths[GridCols[ic].Order+i] = GridCols[ic].Width * Screen->PixelsPerInch / 96;
				}
			}
		}

	SG1->RowCount = 2;
	for (int c = 0; c < SG1->ColCount; c++)
		SG1->Cells[c][1] = L"";
	for (int jr = 1; jr <= mukana; jr++) {
		int srj, ir;

		srj = srjjrj[jr-1].srj;
		if (CBLVal->ItemIndex > 0 && Sarjat[srj].lno[epv] != CBLVal->ItemIndex)
			continue;
		nSrj++;
		ir = nSrj;
		if (SG1->RowCount < nSrj+1)
			SG1->RowCount = nSrj+1;
		for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
			if (GridCols[ic].Visible) {
				int n = 1, col;
				if (GridCols[ic].Count == N_PV)
					n = kilpparam.n_pv_akt;
				col = GridCols[ic].Order;
				ColIx[col] = ic;
				ColPv[col] = 0;
				switch (ic) {
					case 0:
						SG1->Cells[col][ir] = UnicodeString(srj+1);
						break;
					case 1:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].sarjanimi);
						break;
					case 2:
						if (CBTiedot->ItemIndex < 2) {
							SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].sarjaalku);
							}
						else {
							SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].bibalku[epv]);
							}
						break;
					case 3:
						SG1->Cells[col][ir] = sakStr[Sarjat[srj].rrtas];
						break;
					case 4:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].enspv+1);
						break;
					case 5:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].viimpv+1);
						break;
					case 6:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].luonne[epv+i]);
							}
						break;
					case 7:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].bibalku[epv+i]);
							}
						break;
					case 8:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].lahtovali[epv+i]);
							}
						break;
					case 9:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							AIKATOWSTRS(St, Sarjat[srj].enslahto[epv+i], t0);
							St[8] = 0;
							SG1->Cells[col+i][ir] = UnicodeString(St);
							}
						break;
					case 10:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							AIKATOWSTRS(St, Sarjat[srj].ensjlahto[epv+i], t0);
							St[8] = 0;
							SG1->Cells[col+i][ir] = UnicodeString(St);
							}
						break;
					case 11:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].lno[epv+i]);
							}
						break;
					case 12:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].portti[epv+i]);
							}
						break;
					case 13:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].mno[epv+i]);
							}
						break;
					case 14:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].pari[epv+i]);
							}
						break;
					case 15:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].lsak[epv+i]);
							}
						break;
					case 16:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].tsak[epv+i]);
							}
						break;
					case 17:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].npist[epv+i]);
							}
						break;
					case 18:
						swprintf(St, L"%d,%02d", Sarjat[srj].maksu/100, Sarjat[srj].maksu%100);
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 19:
						swprintf(St, L"%d,%02d", Sarjat[srj].maksu1/100, Sarjat[srj].maksu1%100);
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 20:
						swprintf(St, L"%d,%02d", Sarjat[srj].maksual/100, Sarjat[srj].maksual%100);
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 21:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].matka[epv+i]);
							}
						break;
					case 22:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].tapa[epv+i]);
							}
						break;
					case 23:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].psarjanimi);
						break;
					case 24:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(nilm[srj]);
							}
						break;
					case 25:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = nilm_pv[epv+i][srj] - nvak[epv+i][srj];
							}
						break;
					case 26:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							AIKATOWSTRS(St, Sarjat[srj].enslahto[epv+i]+
								SEK*Sarjat[srj].lahtovali[epv+i]*((nilm_pv[epv+i][srj]-nvak[epv+i][srj]+
								Sarjat[srj].pari[epv+i]-1)/Sarjat[srj].pari[epv+i]-1), t0);
							St[8] = 0;
							SG1->Cells[col+i][ir] = UnicodeString(St);
							}
						break;
					case 27:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							if (CBTiedot->ItemIndex < 2)
								SG1->Cells[col+i][ir] =
									UnicodeString(Sarjat[srj].sarjaalku+nilm[srj]-1);
							else
								SG1->Cells[col+i][ir] =
									Sarjat[srj].bibalku[epv+i]+nilm_pv[epv+i][srj]-nvak[epv+i][srj]-1;
							}
						break;
					}
				}
			}
		}

}
//---------------------------------------------------------------------------
int __fastcall TFormSarjatiedot::paivitaMuutos(int col, int row)
{
	int i, k, srj, ipv, epv;
	int tl;
	wchar_t line[20], kh;
	sarjatietue Srj;
	ColTp *GridCols;

	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		epv = 0;
		}
	else if (CBTiedot->ItemIndex == 1) {
		GridCols = GridColsPerus;
		epv = 0;
		}
	else {
		GridCols = GridColsArv;
		epv = CBTiedot->ItemIndex-2;
		}
	k = row;
	srj = _wtoi(SG1->Cells[0][k].c_str()) - 1;
	ipv = 0;
	for (i = col-1; i > 0 && ColIx[i] == ColIx[col]; i--)
		ipv++;
	switch (ColIx[col]) {
		case 1:
			if (pakotaIsot)
				SG1->Cells[col][k] = UnicodeString(upcasewstr(SG1->Cells[col][k].SubString(1, LSARJA).c_str()));
			else
				SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, LSARJA);
			break;
		case 0:
		case 4:
		case 5:
		case 6:
		case 8:
		case 11:
		case 12:
		case 13:
		case 15:
		case 16:
		case 17:
			SG1->Cells[col][k] = UnicodeString(_wtoi(SG1->Cells[col][k].c_str()));
			break;
		case 14:
			tl = _wtoi(SG1->Cells[col][k].c_str());
			if (tl < 1)
				tl = 1;
			SG1->Cells[col][k] = tl;
			break;
		case 2:					// sarjaalku
			tl = _wtoi(SG1->Cells[col][k].c_str());
			SG1->Cells[col][k] = UnicodeString(tl);
			if (GridCols[27].Visible) {
				if (CBTiedot->ItemIndex < 2)
					SG1->Cells[GridCols[27].Order+ipv][k] =
						UnicodeString(tl+nilm[srj]-1);
				else
					SG1->Cells[GridCols[27].Order+ipv][k] =
						tl+nilm_pv[epv+ipv][srj]-nvak[epv+ipv][srj]-1;
				}
			break;
		case 3:
			tl = wcswcind(towupper(SG1->Cells[col][k].c_str()[0]), sakChar);
			if (tl < 0)
				tl = 0;
			SG1->Cells[col][k] = sakStr[tl];
			break;
		case 9:
			wcsncpy(line,SG1->Cells[col][k].c_str(), 10);
			line[10] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else
				tl = wstrtoaika_vap(SG1->Cells[col][k].c_str(), t0);
			SG1->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
			break;
		case 10:
			wcsncpy(line,SG1->Cells[col][k].c_str(), 10);
			line[10] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else
				tl = wstrtoaika_vap(SG1->Cells[col][k].c_str(), t0);
			SG1->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
			break;
		case 18:
		case 19:
		case 20:
			tl = wcstomaksu(SG1->Cells[col][k].c_str());
			swprintf(line, L"%d,%02d", tl/100, tl%100);
			SG1->Cells[col][k] = UnicodeString(line);
			break;
		case 21:
			SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, sizeof(Sarjat[0].matka[0])/2-1);
			break;
		case 22:
			SG1->Cells[col][k] = SG1->Cells[col][k].UpperCase();
			for (int m = 0; m < SG1->Cells[col][k].Length(); m++)
				if (wcswcind(SG1->Cells[col][k][m+1], L"VP") < 0) {
					UnicodeString St = SG1->Cells[col][k];
					St.Delete(m+1, 1);
					SG1->Cells[col][k] = St;
					}
			SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, 2);
			break;
		case 23:
			SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, LPSARJA);
			break;
		}
	if ((ColIx[col] == 8 || ColIx[col] == 9 || ColIx[col] == 14) &&
		GridCols[8].Visible && GridCols[9].Visible && GridCols[26].Visible) {
		int lv, pari;
		tl = wstrtoaika_vap(SG1->Cells[GridCols[9].Order+ipv][k].c_str(), t0);
		lv = _wtoi(SG1->Cells[GridCols[8].Order+ipv][k].c_str());
		if (GridCols[14].Visible)
			pari = _wtoi(SG1->Cells[GridCols[14].Order+ipv][k].c_str());
		else
			pari = Sarjat[srj].pari[epv+ipv];
		tl = tl + SEK * lv * ((nilm_pv[epv+ipv][srj]-nvak[epv+ipv][srj]+pari-1)/pari-1);
		SG1->Cells[GridCols[25].Order+ipv][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
		}
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::TallennaSarjat(void)
{
	wchar_t St[20];
	ColTp *GridCols;
	static int myosbibalku = -1;

	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		}
	else if (CBTiedot->ItemIndex == 1) {
		GridCols = GridColsPerus;
		}
	else {
		GridCols = GridColsArv;
		}
//	SG1->RowCount = sarjaluku+1;
	for (int ir = 1; ir < SG1->RowCount; ir++) {
		int srj;

//		srj = srjjrj[ir-1].srj;
		srj = _wtoi(SG1->Cells[0][ir].c_str()) - 1;
		for (int ic = 1; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
			if (GridCols[ic].Visible) {
				int n = 1, col;
				if (GridCols[ic].Count == N_PV)
					n = kilpparam.n_pv_akt;
				col = GridCols[ic].Order;
				switch (ic) {
					case 1:
						wcsncpy(Sarjat[srj].sarjanimi, SG1->Cells[col][ir].c_str(), LSARJA);
						break;
					case 2:
						if (!kilpparam.bibvaihtelee || CBTiedot->ItemIndex < 2) {
							int sa = SG1->Cells[col][ir].ToInt();
							if (Sarjat[srj].sarjaalku != sa && kilpparam.bibvaihtelee && myosbibalku < 0) {
								if (Application->MessageBoxW(L"Tallennetaanko kaikki alkunumeromuutokset myös vaiheisiin?", L"Valitse", MB_YESNO) == IDYES)
									myosbibalku = 1;
								else
									myosbibalku = 0;
								}
							Sarjat[srj].sarjaalku = sa;
							if (!kilpparam.bibvaihtelee || myosbibalku > 0) {
								for (int i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++)
									Sarjat[srj].bibalku[i_pv] = Sarjat[srj].sarjaalku;
								}
							}
						else {
							Sarjat[srj].bibalku[epv] = SG1->Cells[col][ir].ToInt();
							if (!kilpparam.bibvaihtelee)
								Sarjat[srj].sarjaalku = Sarjat[srj].bibalku[epv];
							}
						if (!kilpparam.bibvaihtelee) {
							for (int i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++)
								Sarjat[srj].bibalku[i_pv] = Sarjat[srj].sarjaalku;
							}
						break;
					case 3:
						Sarjat[srj].rrtas = wcswcind(towupper(SG1->Cells[col][ir].c_str()[0]), sakChar);
						if (Sarjat[srj].rrtas < 0)
							Sarjat[srj].rrtas = 0;
						break;
					case 4:
						Sarjat[srj].enspv = SG1->Cells[col][ir].ToInt() - 1;
						if (Sarjat[srj].enspv < 0)
							Sarjat[srj].enspv = 0;
						if (Sarjat[srj].enspv >= kilpparam.n_pv_akt)
							Sarjat[srj].enspv = kilpparam.n_pv_akt-1;
						break;
					case 5:
						Sarjat[srj].viimpv = SG1->Cells[col][ir].ToInt() - 1;
						if (Sarjat[srj].viimpv >= kilpparam.n_pv_akt)
							Sarjat[srj].viimpv = kilpparam.n_pv_akt-1;
						break;
					case 6:
						for (int i = 0; i < n; i++)
							Sarjat[srj].luonne[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 7:
						for (int i = 0; i < n; i++)
							Sarjat[srj].bibalku[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 8:
						for (int i = 0; i < n; i++)
							Sarjat[srj].lahtovali[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 9:
						for (int i = 0; i < n; i++)
							Sarjat[srj].enslahto[epv+i] = wstrtoaika_vap(SG1->Cells[col+i][ir].c_str(), t0);
						break;
					case 10:
						for (int i = 0; i < n; i++)
							Sarjat[srj].ensjlahto[epv+i] = wstrtoaika_vap(SG1->Cells[col+i][ir].c_str(), t0);
						break;
					case 11:
						for (int i = 0; i < n; i++)
							Sarjat[srj].lno[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 12:
						for (int i = 0; i < n; i++)
							Sarjat[srj].portti[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 13:
						for (int i = 0; i < n; i++)
							Sarjat[srj].mno[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 14:
						for (int i = 0; i < n; i++) {
							Sarjat[srj].pari[epv+i] = SG1->Cells[col+i][ir].ToInt();
							if (Sarjat[srj].pari[epv+i] < 1)
								Sarjat[srj].pari[epv+i] = 1;
							}
						break;
					case 15:
						for (int i = 0; i < n; i++)
							Sarjat[srj].lsak[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 16:
						for (int i = 0; i < n; i++)
							Sarjat[srj].tsak[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 17:
						for (int i = 0; i < n; i++)
							Sarjat[srj].npist[epv+i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 18:
						Sarjat[srj].maksu = wcstomaksu(SG1->Cells[col][ir].c_str());
						break;
					case 19:
						Sarjat[srj].maksu1 = wcstomaksu(SG1->Cells[col][ir].c_str());
						break;
					case 20:
						Sarjat[srj].maksual = wcstomaksu(SG1->Cells[col][ir].c_str());
						break;
					case 21:
						for (int i = 0; i < n; i++)
							wcsncpy(Sarjat[srj].matka[epv+i], SG1->Cells[col+i][ir].c_str(), sizeof(Sarjat[0].matka[0])/2-1);
						break;
					case 22:
						for (int i = 0; i < n; i++)
							wcsncpy(Sarjat[srj].tapa[epv+i], SG1->Cells[col+i][ir].c_str(), sizeof(Sarjat[0].tapa[0])/2-1);
						break;
					case 23:
						wcsncpy(Sarjat[srj].psarjanimi, SG1->Cells[col][ir].c_str(), sizeof(Sarjat[0].psarjanimi)/2-1);
						break;
					}
				}
			}
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
//	kirjsarjat();

}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - SG1->Left;
	SG1->Height = ClientHeight - SG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::BtnPeruutaClick(TObject *Sender)
{
	CBTiedot->Enabled = true;
	CBJarj->Enabled = true;
	if (CBTiedot->ItemIndex > 0)
		CBLVal->Enabled = true;
	SG1->Options >> (goEditing);
	NaytaSarjat();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::BtnMuokkausClick(TObject *Sender)
{
	CBTiedot->Enabled = false;
	CBJarj->Enabled = false;
	CBLVal->Enabled = false;
	SG1->Options << (goEditing);
	LblMuokkaus->Enabled = true;
	LblMuokkaus->Visible = LblMuokkaus->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::BtnTallennaClick(TObject *Sender)
{
	TallennaSarjat();
	BtnPeruutaClick(Sender);
	if (ToimintaTila == 2 && comfl && maxyhteys && memnullcmp((char *)lah_tiedostot, sizeof(lah_tiedostot))) {
		if (Application->MessageBoxW(L"Lähetetäänkö tiedosto verkon muille tietokoneille?",
			L"Lähetys", MB_YESNO) == IDYES) {
			if (lah_tiedosto(sarjaxmlfname, 0, 2))
				Application->MessageBoxW(L"Tiedoston lähettäminen ei onnistunut.", L"Ei lähetetty", MB_OK);
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::FormClose(TObject *Sender, TCloseAction &Action)

{
	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			TallennaSarjat();
			}
		SG1->Options >> (goEditing);
		LblMuokkaus->Enabled = false;
		LblMuokkaus->Visible = LblMuokkaus->Enabled;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button5Click(TObject *Sender)
{
   if (!FormVaPisteet)
		FormVaPisteet = new TFormVaPisteet(FormSarjatiedot);
	FormVaPisteet->Show();
	if (FormVaPisteet->WindowState == wsMinimized)
		FormVaPisteet->WindowState = wsNormal;
	FormVaPisteet->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1DblClick(TObject *Sender)
{
	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			TallennaSarjat();
			}
		}
	aktrow = SG1->Row;
	aktcol = SG1->Col;
	if (aktrow > 0) {
		if (!FormSarja)
			FormSarja = new TFormSarja(this);
		FormSarja->lataaSarja(SG1->Cells[0][aktrow].ToInt()-1);
		FormSarja->pakotaIsot = pakotaIsot;
		FormSarja->Show();
		if (FormSarja->WindowState == wsMinimized)
			FormSarja->WindowState = wsNormal;
		FormSarja->BringToFront();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (SG1->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktcol = ACol;
	aktrow = ARow;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1Exit(TObject *Sender)
{
	if (SG1->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktcol = -1;
	aktrow = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::CBTiedotChange(TObject *Sender)
{
	if (CBTiedot->ItemIndex > kilpparam.n_pv_akt+1) {
		CBTiedot->ItemIndex = kilpparam.n_pv_akt+1;
		}
	Automaattisetmuutokset1->Enabled = (CBTiedot->ItemIndex > 0);
	Siiraikoja1->Enabled = (CBTiedot->ItemIndex > 1);
	Mrsarjojenlhtajat1->Enabled = (CBTiedot->ItemIndex > 1);
	Vapaataaikaasarjojenvliss1->Enabled = (CBTiedot->ItemIndex > 1);
	Hiihtokilpailunperusmalli1->Enabled = (CBTiedot->ItemIndex > 1);
	Kopioivaiheenalkunumerotperustietoihin1->Visible  = (CBTiedot->ItemIndex > 1);
	Kopioiperustietojenalkunumerotvaiheelle1->Visible  = (CBTiedot->ItemIndex > 1);
	CBLVal->Enabled = (CBTiedot->ItemIndex > 0);
	LblLVal->Enabled = CBLVal->Enabled;
	if (!CBLVal->Enabled)
		CBLVal->ItemIndex = 0;
//	SG1->Options >> (goEditing);
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::CBJarjChange(TObject *Sender)
{
//	SG1->Options >> (goEditing);
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sarjat");
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int rr, cc;

	aktrow = SG1->Row;
	aktcol = SG1->Col;
	rr = aktrow;
	cc = aktcol;
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Options.Contains(goEditing)) {
			if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
				MB_YESNO) == IDYES) {
				TallennaSarjat();
				}
			BtnPeruutaClick(Sender);
			}
		else {
			BtnMuokkausClick(Sender);
			}
		aktrow = rr;
		aktcol = cc;
		SG1->Row = aktrow;
		SG1->Col = aktcol;
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Col < SG1->ColCount-1)
			SG1->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Col > 0)
			SG1->Col--;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1FixedCellClick(TObject *Sender, int ACol, int ARow)

{
	if (ARow > 0) {
		aktrow = ARow;
		aktcol = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Vakanttinumeroitasarjojenvliss1Click(TObject *Sender)
{
	wchar_t ch;

	inputint_prompt(&VapNum, L"Anna vapaiden numeroiden luku", &ch, this);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Vapaataaikaasarjojenvliss1Click(TObject *Sender)
{
	wchar_t ch;

	inputint_prompt(&VapAika, L"Anna vapaa aika sekunteina (sarjan viim. lähtijästä seur. sarjan alkuun)", &ch, this);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Mrsarjojenalkunumerot1Click(TObject *Sender)
{
	int i_pv;

	if (CBJarj->ItemIndex != 1) {
		Application->MessageBoxW(L"Valitse järjestykseksi alkunumero",
			L"Edellytys", MB_OK);
		return;
		}
	if ((i_pv = CBTiedot->ItemIndex-2) >= 0 && kilpparam.n_pv_akt > 1 && kilpparam.bibvaihtelee) {
		for (int i = 2; i < SG1->RowCount; i++) {
			int srj1, srj2;
			srj1 = _wtoi(SG1->Cells[0][i].c_str())-1;
			srj2 = _wtoi(SG1->Cells[0][i-1].c_str())-1;
			if (Otalhthuomioon1->Checked &&
				Sarjat[srj1].lno[i_pv] != Sarjat[srj2].lno[i_pv])
				continue;
			Sarjat[srj1].bibalku[i_pv] =
				Sarjat[srj2].bibalku[i_pv] + nilm_pv[i_pv][srj2] - nvak[i_pv][srj2] + VapNum;
			}
		}
	else {
		for (int i = 2; i < SG1->RowCount; i++) {
			int srj1, srj2;
			srj1 = _wtoi(SG1->Cells[0][i].c_str())-1;
			srj2 = _wtoi(SG1->Cells[0][i-1].c_str())-1;
			if (Otalhthuomioon1->Checked &&
				Sarjat[srj1].lno[0] != Sarjat[srj2].lno[0])
				continue;
			Sarjat[srj1].sarjaalku =
				Sarjat[srj2].sarjaalku + nilm[srj2] + VapNum;
			for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++)
				Sarjat[srj1].bibalku[i_pv] = Sarjat[srj1].sarjaalku;
			}
		}
	NaytaSarjat();
	Application->MessageBoxW(L"Toimenpide suoritettu. Uusia tietoja ei ole tallennettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Mrsarjojenlhtajat1Click(TObject *Sender)
{
	int i_pv;

	if ((i_pv = CBTiedot->ItemIndex-2) < 0 || CBJarj->ItemIndex != 2) {
		Application->MessageBoxW(L"Valitse näytettäväksi yksi vaihe ja järjestykseksi Lähtö/Lähtöaika",
			L"Edellytys", MB_OK);
		return;
		}

	for (int i = 2; i < SG1->RowCount; i++) {
		int srj1, srj2;
		srj1 = _wtoi(SG1->Cells[0][i].c_str())-1;
		srj2 = _wtoi(SG1->Cells[0][i-1].c_str())-1;
		if (Sarjat[srj1].lno[i_pv] == Sarjat[srj2].lno[i_pv] &&
			Sarjat[srj1].portti[i_pv] == Sarjat[srj2].portti[i_pv]) {
			Sarjat[srj1].enslahto[i_pv] =
				Sarjat[srj2].enslahto[i_pv]
					+ SEK*Sarjat[srj2].lahtovali[i_pv]
						*((nilm_pv[i_pv][srj2]- nvak[i_pv][srj2] + Sarjat[srj2].pari[i_pv]-1)
							 /Sarjat[srj2].pari[i_pv]-1)
					+ SEK*VapAika;
			}
		}
	NaytaSarjat();
	Application->MessageBoxW(L"Toimenpide suoritettu. Uusia tietoja ei ole tallennettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SetColumns(void)
{
	int cols = 0;

	GridCols1[2].Visible = Alkunumero1->Checked;
	GridCols1[3].Visible = Sakollisuus1->Checked;
	GridCols1[4].Visible = N1vaihe1->Checked;
	GridCols1[5].Visible = Viimvaihe1->Checked;
	GridCols1[6].Visible = Vaiheenluonne1->Checked;
	GridCols1[7].Visible = Bibalku1->Checked;
	GridCols1[8].Visible = Lhtvli1->Checked;
	GridCols1[9].Visible = Nytlhtajat1->Checked;
	GridCols1[10].Visible = Jlkiilmlht1->Checked;
	GridCols1[11].Visible = Lhtpaikka1->Checked;
	GridCols1[13].Visible = Maali1->Checked;
	GridCols1[14].Visible = Ryhmkoko1->Checked;
	GridCols1[15].Visible = Ampumapaikat1->Checked;
	GridCols1[16].Visible = Sakkoyksikk1->Checked;
	GridCols1[18].Visible = Maksut1->Checked;
	GridCols1[19].Visible = Maksut1->Checked && kilpparam.n_pv_akt > 1;
	GridCols1[21].Visible = Matka1->Checked;
	GridCols1[22].Visible = Matka1->Checked && kilpparam.kilplaji == L'H';
	GridCols1[23].Visible = Pitknimi1->Checked;

	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridCols1[ic].Visible) {
			GridCols1[ic].Order = cols;
			if (GridCols1[ic].Count == N_PV)
				cols += kilpparam.n_pv_akt;
			else
				cols++;
			}
		}

	GridCols1Count = cols;

	if (Nytlhtajat1->Checked) {
		GridColsArv[8].Visible = true;
		GridColsArv[9].Visible = true;
		GridColsArv[10].Visible = true;
		GridColsArv[26].Visible = true;
		}
	else {
		GridColsArv[8].Visible = false;
		GridColsArv[9].Visible = false;
		GridColsArv[10].Visible = false;
		GridColsArv[26].Visible = false;
		}

	cols = 0;
	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridColsArv[ic].Visible) {
			GridColsArv[ic].Order = cols;
			cols++;
			}
		}

	GridColsArvCount = cols;

	cols = 0;
	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridColsPerus[ic].Visible) {
			GridColsPerus[ic].Order = cols;
			cols++;
			}
		}

	GridColsPerusCount = cols;

	NaytaSarjat();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Nytlhtajat1Click(TObject *Sender)
{
	Nytlhtajat1->Checked = !Nytlhtajat1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Alkunumero1Click(TObject *Sender)
{
	Alkunumero1->Checked = !Alkunumero1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Bibalku1Click(TObject *Sender)
{
	Bibalku1->Checked = !Bibalku1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Sakollisuus1Click(TObject *Sender)
{
	Sakollisuus1->Checked = !Sakollisuus1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::N1vaihe1Click(TObject *Sender)
{
	N1vaihe1->Checked = !N1vaihe1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Viimvaihe1Click(TObject *Sender)
{
	Viimvaihe1->Checked = !Viimvaihe1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Lhtvli1Click(TObject *Sender)
{
	Lhtvli1->Checked = !Lhtvli1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Jlkiilmlht1Click(TObject *Sender)
{
	Jlkiilmlht1->Checked = !Jlkiilmlht1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Lhtpaikka1Click(TObject *Sender)
{
	Lhtpaikka1->Checked = !Lhtpaikka1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Maali1Click(TObject *Sender)
{
	Maali1->Checked = !Maali1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Ryhmkoko1Click(TObject *Sender)
{
	Ryhmkoko1->Checked = !Ryhmkoko1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Ampumapaikat1Click(TObject *Sender)
{
	Ampumapaikat1->Checked = !Ampumapaikat1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Sakkoyksikk1Click(TObject *Sender)
{
	Sakkoyksikk1->Checked = !Sakkoyksikk1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Maksut1Click(TObject *Sender)
{
	Maksut1->Checked = !Maksut1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Matka1Click(TObject *Sender)
{
	Matka1->Checked = !Matka1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Pitknimi1Click(TObject *Sender)
{
	Pitknimi1->Checked = !Pitknimi1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Siiraikoja1Click(TObject *Sender)
{
	int i_pv, siirto = 0;
	wchar_t ch;

	i_pv = CBTiedot->ItemIndex - 2;
	if (i_pv < 0 || i_pv >= kilpparam.n_pv_akt)
		return;
	inputint_prompt(&siirto, L"Anna siirto sekunteina", &ch, this);
	if (ch == ESC)
		return;
	for (int r = 1; r < SG1->RowCount; r++) {
		int srj;
		srj = _wtoi(SG1->Cells[0][r].c_str())-1;
		if ((CBLVal->ItemIndex == 0 || CBLVal->ItemIndex == Sarjat[srj].lno[i_pv]) &&
			Sarjat[srj].enslahto[i_pv] != TMAALI0)
			Sarjat[srj].enslahto[i_pv] =
				NORMKELLO(Sarjat[srj].enslahto[i_pv] + SEK*siirto);
		}
	NaytaSarjat();
	Application->MessageBoxW(L"Toimenpide suoritettu. Uusia tietoja ei ole tallennettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Pisteidenlaskenta1Click(TObject *Sender)
{
	int srj = 0, pv = 0;

	srj = _wtoi(SG1->Cells[0][SG1->Row].c_str())-1;
	if (srj < 0)
		srj = 0;
	if (CBTiedot->ItemIndex > 1)
		pv = CBTiedot->ItemIndex - 2;
	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			BtnTallennaClick(Sender);
//			TallennaSarjat();
			}
		else
			BtnPeruutaClick(Sender);
		}
	if (!FormTuloskaava)
		FormTuloskaava = new TFormTuloskaava(this);
	FormTuloskaava->AsetaPiste(srj, pv, VALUKU+1);
	FormTuloskaava->Show();
	if (FormTuloskaava->WindowState == wsMinimized)
		FormTuloskaava->WindowState = wsNormal;
	FormTuloskaava->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kirjoitacsvtiedostoon1Click(TObject *Sender)
{
	TextFl *OutFl;

	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = UnicodeString(L"sarjat.csv");
	SaveDialog1->DefaultExt = L"csv";
	SaveDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		OutFl = new TextFl(SaveDialog1->FileName.c_str(), L"wt");
		if (OutFl->IsOpen()) {
			wchar_t line[2000], as[20], as2[20];
			wchar_t *sakstr[5] = {L"Ei-sakkoja", L"Lasten", L"RR-sakot", L"Tasoituksellinen", L"Sakollinen"};

			wcscpy(line, L"Sarja;\"Pitkä nimi\";\"Ens. vaihe\";\"Viim.vaihe\";Alkuno;Sakollisuus;Maksu;\"Maksu-1\";\"Maksu-al\"");
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				swprintf(line+wcslen(line), L";\"Luonne-%d\";\"Alkuno-%d\";\"EnsLähtö-%d\";\"EnsJIlm-%d\";\"Lväli-%d\";\"Aalto-%d\""
				";\"Lähtö-%d\";\"Jono-%d\";\"'Maali-%d\";\"Paikat-%d\";\"Sakkoyks-%d\";\"Matka-%d\";\"Tapa-%d\"",
					ipv+1, ipv+1, ipv+1, ipv+1, ipv+1, ipv+1, ipv+1,  ipv+1, ipv+1, ipv+1, ipv+1, ipv+1, ipv+1);
				}
			wcscat(line, L"\n");
			OutFl->WriteLine(line);
			for (int srj = 0; srj < sarjaluku; srj++) {
				swprintf(line, L"%s;\"%s\";%d;%d;%d;\"%s\";%d;%d;%d",
					Sarjat[srj].sarjanimi, Sarjat[srj].psarjanimi, Sarjat[srj].enspv+1, Sarjat[srj].viimpv+1,
					Sarjat[srj].sarjaalku, sakstr[Sarjat[srj].rrtas], Sarjat[srj].maksu, Sarjat[srj].maksu1, Sarjat[srj].maksual);
				for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
					aikatowstr_cols_n(as, Sarjat[srj].enslahto[ipv], t0, 0, 8);
					aikatowstr_cols_n(as2, Sarjat[srj].ensjlahto[ipv], t0, 0, 8);
					swprintf(line+wcslen(line), L";%d;%d;%s;%s;%d;%d;%d;%d;%d;%d;%d;\"%s\";\"%s\"",
						Sarjat[srj].luonne[ipv], Sarjat[srj].bibalku[ipv], as, as2, Sarjat[srj].lahtovali[ipv],
						Sarjat[srj].pari[ipv], Sarjat[srj].lno[ipv], Sarjat[srj].portti[ipv], Sarjat[srj].mno[ipv],
						Sarjat[srj].lsak[ipv], Sarjat[srj].tsak[ipv], Sarjat[srj].matka[ipv], Sarjat[srj].tapa[ipv]);
					}
				wcscat(line, L"\n");
				OutFl->WriteLine(line);
				}
			}
		delete OutFl;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Luecsvtiedostosta1Click(TObject *Sender)
{
	TextFl *InFl;
	int srj = -1, n1;
	wchar_t *fields[100], erottimet[] = L";\t";
	int nfld = 0, k = 0;

	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = UnicodeString(L"sarjat.csv");
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		InFl = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
		while (InFl->IsOpen() && !InFl->Feof()) {
			wchar_t line[2000];

			InFl->ReadLine(line, sizeof(line)/2-1);
			if (srj >= 0) {
				memset(fields, 0, sizeof(fields));
				nfld = getfields(line, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
					L"\"", true, false);
				if (nfld < 9 + 11*kilpparam.n_pv_akt)
					break;
				wcsncpy(Sarjat[srj].sarjanimi, fields[0], LSARJA);
				wcsncpy(Sarjat[srj].psarjanimi, fields[1], 31);
				Sarjat[srj].enspv = _wtoi(fields[2]) - 1;
				Sarjat[srj].viimpv = _wtoi(fields[3]) - 1;
				Sarjat[srj].sarjaalku = _wtoi(fields[4]);
				Sarjat[srj].rrtas = wcswcind(fields[5][0], L"ELRTS");
				if (Sarjat[srj].rrtas < 0)
					Sarjat[srj].rrtas = 0;
				Sarjat[srj].maksu = _wtoi(fields[6]);
				Sarjat[srj].maksu1 = _wtoi(fields[7]);
				Sarjat[srj].maksual = _wtoi(fields[8]);
				n1 = 9;
				for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
					Sarjat[srj].luonne[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].bibalku[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].enslahto[ipv] = wstrtoaika_vap(fields[n1++], t0);
					Sarjat[srj].ensjlahto[ipv] = wstrtoaika_vap(fields[n1++], t0);
					Sarjat[srj].lahtovali[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].pari[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].lno[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].portti[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].mno[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].lsak[ipv] = _wtoi(fields[n1++]);
					Sarjat[srj].tsak[ipv] = _wtoi(fields[n1++]);
					wcsncpy(Sarjat[srj].matka[ipv], fields[n1++], sizeof(Sarjat[srj].matka[ipv])/2-1);
					wcsncpy(Sarjat[srj].tapa[ipv], fields[n1++], sizeof(Sarjat[srj].tapa[ipv])/2-1);
					}
				}
			srj++;
			if (srj > sarjaluku)
				sarjaluku = srj;
			}
		delete InFl;
		}
	SetCurrentDir(FormMain->CurrentDir);
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1MouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (SG1->RowCount - SG1->FixedRows >= SG1->VisibleRowCount + SG1->TopRow)
		SG1->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1MouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SG1->TopRow > SG1->FixedRows)
		SG1->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kopioivalittunaolevasolukaikkiinsarjoihin1Click(TObject *Sender)

{
	UnicodeString Val;

	if (SG1->Options.Contains(goEditing)) {
		if (SG1->Row > 0 && SG1->Col > 0) {
			Val = SG1->Cells[SG1->Col][SG1->Row];
			for (int r = 1; r < SG1->RowCount; r++)
			   SG1->Cells[SG1->Col][r] = Val;
			}
		SG1->Refresh();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kiinteitsarakkeita11Click(TObject *Sender)
{
	int FC = SG1->FixedCols;
	wchar_t ret;
	inputint_prompt(&FC, L"Anna kiinteiden sarakkeiden luku (1..8)", &ret, this);
	if (ret != ESC) {
		if (FC < 1)
			FC = 1;
		if (FC > 8)
			FC = 8;
		if (FC > SG1->ColCount)
			FC = SG1->ColCount;
		SG1->FixedCols = FC;
		Kiinteitsarakkeita11->Caption = L"Kiinteitä sarakkeita "+UnicodeString(FC);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Hiihtokilpailunperusmalli1Click(TObject *Sender)

{
	int i_pv, vast;
	bool ohitatyhjat, enssrj = true, asetaperus = false;

	if ((i_pv = CBTiedot->ItemIndex-2) < 0 || CBJarj->ItemIndex != 1) {
		Application->MessageBoxW(L"Valitse näytettäväksi yksi vaihe ja järjestykseksi alkunumero",
			L"Edellytys", MB_OK);
		return;
		}
	if (Application->MessageBoxW(L"Sekä numerot että lähtöajat annetaan yhtenäiseksi jonoksi. "
			L"Jokaisen sarjan alkuun lisätään valittu määrä vakantteja."
			L"Valittu alkunumero annetaan ensimmäiselle vakantille."
			L"Alkunumerot ja lähtöajat merkitään arvontaan osallistuvien mukaan.",
			L"Vahvista", MB_YESNO) != IDYES) {
		return;
		}
	if ((vast = Application->MessageBoxW(L"Lisätäänkö vakanttaja myös tyhjiin sarjoihin?",
		L"Tyhjät sarjat", MB_YESNOCANCEL)) == IDCANCEL) {
		return;
		}
	ohitatyhjat = (vast == IDNO);

	for (int i = 1; i < SG1->RowCount; i++) {
		int srj1, srj2;
		int VapNum1 = 0;

		srj1 = _wtoi(SG1->Cells[0][i].c_str())-1;
		if (!ohitatyhjat || nilm_pv[i_pv][srj1] > 0)
			VapNum1 = VapNum;
		if (enssrj) {
			enssrj = false;

			Sarjat[srj1].bibalku[i_pv] += VapNum1;
			Sarjat[srj1].enslahto[i_pv] =
				Sarjat[srj1].enslahto[i_pv]
					+ SEK*Sarjat[srj1].lahtovali[i_pv]
						*((VapNum1 + Sarjat[srj1].pari[i_pv]-1)
							 /Sarjat[srj1].pari[i_pv]);
			}
		else {
			srj2 = _wtoi(SG1->Cells[0][i-1].c_str())-1;

			Sarjat[srj1].bibalku[i_pv] =
				Sarjat[srj2].bibalku[i_pv] + nilm_pv[i_pv][srj2] - nvak[i_pv][srj2] + VapNum1;
			Sarjat[srj1].enslahto[i_pv] =
				Sarjat[srj2].enslahto[i_pv]
					+ SEK*Sarjat[srj2].lahtovali[i_pv]
						*((nilm_pv[i_pv][srj2] - nvak[i_pv][srj2] + Sarjat[srj2].pari[i_pv]-1)
							 /Sarjat[srj2].pari[i_pv])
					+ SEK*Sarjat[srj1].lahtovali[i_pv]
						*((VapNum1 + Sarjat[srj1].pari[i_pv]-1)
							 /Sarjat[srj1].pari[i_pv]);
			}
		if (!kilpparam.bibvaihtelee || i_pv == 0)
			Sarjat[srj1].sarjaalku = Sarjat[srj1].bibalku[i_pv];
		if (!ohitatyhjat || nilm_pv[i_pv][srj1] > 0) {
			for (int k = 0; k < VapNum; k++) {
				int d;
				kilptietue kilp;

				kilp.nollaa();
				kilp.setSarjaAll(srj1);
				kilp.setIdAll(Sarjat[srj1].bibalku[i_pv] - VapNum + k);
				wcsncpy(kilp.sukunimi, Sarjat[srj1].sarjanimi, kilpparam.lsnimi-1);
				_itow(kilp.id(), kilp.etunimi, 10);
				for (int j_pv = 0; j_pv < kilpparam.n_pv_akt; j_pv++)
					kilp.set_tark(L'V', j_pv);
				kilp.tall_lajat_pv(Sarjat[srj1].enslahto[i_pv]
					- SEK*Sarjat[srj1].lahtovali[i_pv]
					   *((VapNum - k + Sarjat[srj1].pari[i_pv]-1)
							/Sarjat[srj1].pari[i_pv]),
					i_pv);
				kilp.addtall(&d, 0);
				}
			}
		}
	NaytaSarjat();
	Application->MessageBoxW(L"Toimenpide suoritettu. Uusia tietoja ei ole tallennettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::FormShow(TObject *Sender)
{
	if (ToimintaTila == 2) {
		Lissarja1->Enabled = false;
		}
	CBTiedot->Clear();
	CBTiedot->Items->Add(L"Laajat");
	CBTiedot->Items->Add(L"Perustiedot");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBTiedot->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	CBTiedot->ItemIndex = 0;
	SetColumns();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Lissarja1Click(TObject *Sender)
{
	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			TallennaSarjat();
			}
		}
	if (!FormSarja)
		FormSarja = new TFormSarja(this);

	FormSarja->lataaSarja(-1);
	FormSarja->SNimi->Text = L"";
	FormSarja->Show();
	if (FormSarja->WindowState == wsMinimized)
		FormSarja->WindowState = wsNormal;
	FormSarja->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Otalhthuomioon1Click(TObject *Sender)
{
	Otalhthuomioon1->Checked = !Otalhthuomioon1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Lisvakantitsarjojenedelle1Click(TObject *Sender)
{
	int i_pv;
	int ensVapaa = 1;
	char varattu[MAXKILPNO+1];

	memset(varattu, 0, sizeof(varattu));
	if ((i_pv = CBTiedot->ItemIndex-2) < 0 || CBJarj->ItemIndex != 1) {
		Application->MessageBoxW(L"Valitse näytettäväksi yksi vaihe ja järjestykseksi alkunumero",
			L"Edellytys", MB_OK);
		return;
		}
	for (int d = 1; d < nrec; d++) {
		kilptietue kilp;
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0 && kilp.kilpno == 0) {
			Application->MessageBoxW(L"Toimintoa voi käyttää vasta, kun kaikki kilpailijat on numeroitu.",
				L"Edellytys", MB_OK);
			return;
			}
		varattu[kilp.kilpno] = 1;
		}
	if (Application->MessageBoxW(L"Kaikki sarjaa edeltävät vapaat paikat täytetään edeltävillä "
			L"numeroilla ja ajoilla. ", L"Vahvista", MB_YESNO) != IDYES) {
		return;
		}
	for (int i = 1; i < SG1->RowCount; i++) {
		int srj1, bAlku;

		srj1 = _wtoi(SG1->Cells[0][i].c_str())-1;
		bAlku = Sarjat[srj1].bibalku[i_pv];
		for (; ensVapaa < bAlku; ensVapaa++) {
			int d;
			kilptietue kilp;
			if (varattu[ensVapaa] != 0)
				continue;
			kilp.nollaa();
			kilp.setSarjaAll(srj1);
			kilp.setIdAll(ensVapaa);
			wcsncpy(kilp.sukunimi, Sarjat[srj1].sarjanimi, kilpparam.lsnimi-1);
			_itow(kilp.id(), kilp.etunimi, 10);
			for (int j_pv = 0; j_pv < kilpparam.n_pv_akt; j_pv++)
				kilp.set_tark(L'V', j_pv);
			kilp.tall_lajat_pv(Sarjat[srj1].enslahto[i_pv]
				- SEK*Sarjat[srj1].lahtovali[i_pv]
				   *((bAlku - ensVapaa + Sarjat[srj1].pari[i_pv]-1)
						/Sarjat[srj1].pari[i_pv]),
				i_pv);
			kilp.addtall(&d, 0);
			}
		}
	NaytaSarjat();
	Application->MessageBoxW(L"Toimenpide suoritettu.", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::Haematkatratatiedoista1Click(TObject *Sender)
{
	int n = 0;
	if (ToimintaTila != 2 || nrata == 0) {
		Application->MessageBoxW(L"Toiminto on käytettävissä vain tulospalvelutilassa, kun radatiedot on luettu", L"Este", MB_OK);
		return;
		}
	if (!SG1->Options.Contains(goEditing))
		BtnMuokkausClick(Sender);
	for (int srj = 0; srj < sarjaluku; srj++) {
		int rr;
		if ((rr = onrata(Sarjat[srj].sarjanimi)) > 0 && rata[rr-1].ratapit > 0) {
			swprintf(Sarjat[srj].matka[k_pv], L"%d,%d", (rata[rr-1].ratapit+50)/1000, ((rata[rr-1].ratapit+50)%1000)/100);
			n++;
			}
		}
	if (n > 0)
		NaytaSarjat();
	Application->MessageBoxW((L"Siirretty "+UnicodeString(n)+" matkatietoa. Paina 'Tallenna muokatut tiedot' tallentaaksesi.").c_str(), L"", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kopioivaiheenalkunumerotperustietoihin1Click(TObject *Sender)

{
	for	(int srj = 0; srj < sarjaluku; srj++)
		Sarjat[srj].sarjaalku = Sarjat[srj].bibalku[CBTiedot->ItemIndex-2];
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kopioiperustietojenalkunumerotvaiheelle1Click(TObject *Sender)

{
	for	(int srj = 0; srj < sarjaluku; srj++)
		Sarjat[srj].bibalku[CBTiedot->ItemIndex-2] = Sarjat[srj].sarjaalku;
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Vaiheenluonne1Click(TObject *Sender)
{
	Vaiheenluonne1->Checked = !Vaiheenluonne1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

