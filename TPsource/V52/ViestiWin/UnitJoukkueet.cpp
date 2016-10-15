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

#include "UnitJoukkueet.h"
#include "UnitJoukkuetiedot.h"
#include "UnitSuodatus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormJoukkueet *FormJoukkueet;
//---------------------------------------------------------------------------

#define OCOLKNO		0
#define OCOLSARJA 	1
#define OCOLSEURA 	2
#define OCOLJK 		3
#define OCOLMAA		4
#define OCOLIDNO	5
#define OCOLJNIMI	6
#define OCOLPIIRI	7
#define OCOLERA		8
#define OCOLSNIMI	9
#define OCOLENIMI	10
#define OCOLLIS		11
#define OCOLARVO	12
#define OCOLOSRA	13
#define OCOLBDG		14
#define OCOLBDG2	15
#define OCOLLNA		16
#define OCOLLAHTO	17
#define OCOLLHTLJ	18
#define OCOLAIKA	19
#define OCOLTARK	20
#define OCOLASAKOT	21
#define OCOLSAKKO	22
#define OCOLRATA	23
#define OCOLGPS		24

ColTp OoGridCols0[] = {
	{L"KilpNo", 40, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, true},
	{L"Seura", 120, 0, 1, 0, true},
	{L"Jk", 30, 0, 1, 0, true},
	{L"Maa", 30, 0, 1, 0, true},
	{L"Id-no", 50, 0, 1, 0, false},
	{L"JoukkNimi", 70, 0, 1, 0, false},
	{L"Piiri", 40, 0, 1, 0, false},
	{L"Erä", 40, 0, 1, 0, false},
	{L"Sukunimi", 80, 0, MAXOSUUSLUKU, 0, true},
	{L"Etunimi", 80, 0, MAXOSUUSLUKU, 0, true},
	{L"Lisenssi", 55, 0, MAXOSUUSLUKU, 0, false},
	{L"Arvo", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"OsSeura", 80, 0, MAXOSUUSLUKU, 0, false},
	{L"Badge", 55, 0, MAXOSUUSLUKU, 0, true},
	{L"Badge2", 55, 0, MAXOSUUSLUKU, 0, false},
	{L"Lna", 30, 0, MAXOSUUSLUKU, 0, true},
	{L"Lähtö", 70, 0, MAXOSUUSLUKU, 0, true},
	{L"Lht-lj", 40, 0, MAXOSUUSLUKU, 0, true},
	{L"Aika", 70, 0, MAXOSUUSLUKU, 0, true},
	{L"Tark", 40, 0, MAXOSUUSLUKU, 0, true},
	{L"A-sakot", 55, 0, MAXOSUUSLUKU, 0, false},
	{L"Sakko", 40, 0, MAXOSUUSLUKU, 0, false},
	{L"Rata", 60, 0, MAXOSUUSLUKU, 0, true},
	{L"GPS", 30, 0, MAXOSUUSLUKU, 0, true}
};

ColTp OoGridCols[sizeof(OoGridCols0)/sizeof(ColTp)];

static wchar_t *LLaji[4] = {L"Auto", L"Auto", L"Yksil", L"Kiint"};

//---------------------------------------------------------------------------

__fastcall TFormJoukkueet::TFormJoukkueet(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	memcpy(OoGridCols, OoGridCols0, sizeof(OoGridCols0));
	sallimuokkaus = !kuuluttaja;
	memset(&ooindex, 0, sizeof(ooindex));
}

//---------------------------------------------------------------------------
void __fastcall TFormJoukkueet::FormResize(TObject *Sender)
{
	OoGrid->Width = ClientWidth - 16 * Screen->PixelsPerInch / 96;
	OoGrid->Height = ClientHeight - OoGrid->Top - 8 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

bool __fastcall TFormJoukkueet::Mukana(int d)
{
	wchar_t st[100], st1[100], st2[40];
	int os;

	if (!FormSuodatus)
		return(true);
	os = CBOsuudet->ItemIndex > 0 ? (CBOsuudet->ItemIndex - 1) : FormSuodatus->CBOsuus->ItemIndex;
	st[99] = 0;
	st1[99] = 0;
	if (!FormSuodatus)
		return(true);
	if (FormSuodatus->CBSarja->ItemIndex > 0) {
		int srj;
		srj = KilpA[d].sarja;
		if (FormSuodatus->CBSarja->ItemIndex != srj + 1)
			return(false);
		}

	if (FormSuodatus->CBSeura->ItemIndex > 0) {
		wchar_t maa[4], seura[LSEURA+1];
		wcsncpy(st1, FormSuodatus->CBSeura->Items->Strings[FormSuodatus->CBSeura->ItemIndex].c_str(), 99);
		if (wcslen(st1) < 5)
			st1[4] = 0;
		st1[3] = 0;
		elimwbl(st1);
		elimwbl(st1+4);
		wcsncpy(seura, KilpA[d].Seura(st, 80), LSEURA);
		seura[LSEURA] = 0;
		if (wcscmp(st1, KilpA[d].Maa(st2)) || wcscmp(seura, st1+4))
			return(false);
		}

	if (FormSuodatus->CBTark->ItemIndex > 0) {
		wchar_t kh;
		kh = KilpA[d].wTark(os, 0);
		if (L" -THKIEP"[FormSuodatus->CBTark->ItemIndex] != kh)
			return(false);
		}

	if (FormSuodatus->RGPeruste->ItemIndex <= 0)
		return(true);

	wcsncpy(st, FormSuodatus->EdtTieto->Text.c_str(), 99);
	switch (FormSuodatus->RGPeruste->ItemIndex-1) {
		case 0:
			KilpA[d].Seura(st1, 60, false);
			upcasewstr(st1);
			if (FormSuodatus->CBTyhja->Checked)
				return(st1[0] == 0);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 1:
			KilpA[d].Maa(st1);
			upcasewstr(st1);
			if (FormSuodatus->CBTyhja->Checked)
				return(st1[0] == 0);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 2:
			return(KilpA[d].kilpno == _wtoi(st));
		case 3:
			KilpA[d].Nimi(st1, 60, os, false, 1);
			upcasewstr(st1);
			if (FormSuodatus->CBTyhja->Checked)
				return(st1[0] == 0);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 4:
			return(KilpA[d].ostiet[os].badge[0] == _wtoi(st));
		case 5:
			return(KilpA[d].ostiet[os].lisno == _wtoi(st));
		case 6:
			return(towupper(KilpA[d].Seuranta(os)) == towupper(st[0]));
		}
	return(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::KilpKey(int d, char * key)
{
	wchar_t st[82];
	kilptietue *kilp;
	int kno;

	kilp = KilpA+d;
	switch (CBJarjestys->ItemIndex) {
		case 0:
			putintright(kilp->kilpno, key+5);
			break;
		case 1:
			keyfromU16name(key, kilp->Seura(st, 60, false), 60, 0);
			break;
		case 2:
			putintright(kilp->sarja, key+3);
			putintright(kilp->kilpno, key+8);
			break;
		case 3:
			putintright(kilp->sarja, key+3);
			keyfromU16name(key+4, kilp->Seura(st, 60, false), 60, 0);
			break;
		case 4:
			putintright(kilp->sarja, key+3);
			if (kilp->Tulos(Oo_os, 0) > 0)
				AIKATOSTRS(key+5, kilp->Tulos(Oo_os, 0), 0);
			else
				key[4] = "603312445"[wcswcind(kilp->wTark(Oo_os, 1),L"T-IHKSEP")+1];
			break;
		case 5:
			putintright(d, key+5);
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::teeOoJarj(void)
{
	if (KilpA)
		delete[] KilpA;
	KilpA = new kilptietue[datf2.numrec];
	if (OoJarj)
		delete[] OoJarj;
	OoJarj = new OoJarjTp[datf2.numrec];
	memset(OoJarj, 0, datf2.numrec*sizeof(OoJarj[0]));
	NKilp = 0;
	for (int d = 1; d < datf2.numrec; d++) {
		KilpA[d].getrec(d);
		if (KilpA[d].kilpstatus == 0 && Mukana(d)) {
			KilpKey(d, OoJarj[NKilp].key);
			OoJarj[NKilp].Dk = d;
			NKilp++;
			}
		}
	quicksort((char *)OoJarj, NKilp, sizeof(OoJarj[0].key), sizeof(OoJarj[0]));
//	FormSuodatus->nMukana = NKilp;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::setOoGrid(void)
{
	int n, fixmax;

	OoGridCols[OCOLKNO].Visible = true;
	OoGridCols[OCOLSARJA].Visible = true;
	OoGridCols[OCOLSEURA].Visible = true;
	OoGridCols[OCOLJK].Visible = true;
	OoGridCols[OCOLMAA].Visible = Maa1->Checked;
	OoGridCols[OCOLIDNO].Visible = JoukkId->Checked;
	OoGridCols[OCOLJNIMI].Visible = JoukkNimi->Checked;
	OoGridCols[OCOLPIIRI].Visible = Aluepiiri1->Checked;
	OoGridCols[OCOLERA].Visible = Era1->Checked;
	OoGridCols[OCOLSNIMI].Visible = Nimi1->Checked;
	OoGridCols[OCOLENIMI].Visible = Nimi1->Checked;
	OoGridCols[OCOLLIS].Visible = Lisenssinumero1->Checked || Osuuspisteet11->Checked
		|| Osuuspisteet21->Checked || Osuuspisteet31->Checked;
	OoGridCols[OCOLARVO].Visible = Arvo1->Checked;
	OoGridCols[OCOLOSRA].Visible = Seura1->Checked;
	OoGridCols[OCOLBDG].Visible = Emitkoodi1->Checked;
	OoGridCols[OCOLBDG2].Visible = N2ekoodi1->Checked;
	OoGridCols[OCOLLNA].Visible = Laina1->Checked;
	OoGridCols[OCOLLAHTO].Visible = Lhtaika1->Checked;
	OoGridCols[OCOLLHTLJ].Visible = Lhtlaji1->Checked;
	OoGridCols[OCOLAIKA].Visible = Tulos1->Checked;
	OoGridCols[OCOLTARK].Visible = true;
	OoGridCols[OCOLASAKOT].Visible = (kilpparam.lsakmax > 0 && Sakot1->Checked);
	OoGridCols[OCOLSAKKO].Visible = (kilpparam.lsakmax == 0 && Sakot1->Checked);
	OoGridCols[OCOLRATA].Visible = Rata1->Checked;
	OoGridCols[OCOLGPS].Visible = GPS1->Checked;
	OoGrid->RowCount = 1;
	OoGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	if (OoGrid->FixedCols > 5)
		OoGrid->ColCount = OoGrid->FixedCols+1;
	else
		OoGrid->ColCount = 5;
	n = 0;
	n_monios = 0;
	fixmax = OoGrid->FixedCols;
	for (int i = 0; i < sizeof(OoGridCols)/sizeof(ColTp); i++) {
		if (OoGridCols[i].Visible) {
			if (OoGridCols0[i].Count > 1) {
				n_monios++;
				if (n <= fixmax && npv > 1)
					fixmax = n-1;
				n += npv;
				}
			else
				n++;
			}
		}
	if (fixmax > OoGrid->FixedCols)
		OoGrid->FixedCols = fixmax;
	if (OoGrid->FixedCols > n)
		OoGrid->FixedCols = n;
	OoGrid->ColCount = n;
	n = 0;
	for (int i = 0; i < sizeof(OoGridCols)/sizeof(ColTp); i++) {
		if (OoGridCols[i].Visible) {
			OoGridCols[i].Order = n;
			OoGridCols[i].Count = OoGridCols0[i].Count > 1 ? npv : 1;
			for (int ipv = 0; ipv < OoGridCols[i].Count; ipv++) {
				if (i == OCOLLIS && !Lisenssinumero1->Checked) {
					if (Osuuspisteet11->Checked)
						OoGrid->Cells[n+ipv*n_monios][0] = L"OsPist-1";
					if (Osuuspisteet21->Checked)
						OoGrid->Cells[n+ipv*n_monios][0] = L"OsPist-2";
					if (Osuuspisteet31->Checked)
						OoGrid->Cells[n+ipv*n_monios][0] = L"OsPist-3";
					}
				else
					OoGrid->Cells[n+ipv*n_monios][0] = UnicodeString(OoGridCols[i].Caption);
				OoGrid->ColWidths[n+ipv*n_monios] = OoGridCols[i].Width * Screen->PixelsPerInch / 96;
				}
			n++;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::naytaTiedot(void)
{
	wchar_t ch, cs, ch2, ct, line[80], trk, st[100];
	int i, n, d, ep, rvali, sj;
	int tl, tlsum;
	kilptietue *Kilp;

	npv = 1;
	epv = 0;
	aktrow = 0;
	aktcol = 0;
	memset(line, 0, sizeof(line));
	Button1->Visible = sallimuokkaus;
	Button2->Visible = sallimuokkaus;
	Button3->Visible = sallimuokkaus;
	LblMuokkaus->Visible = OoGrid->Options.Contains(goEditing);
	if (CBOsuudet->ItemIndex > 0) {
		if (CBOsuudet->ItemIndex > kilpparam.osuusluku)
		   CBOsuudet->ItemIndex = kilpparam.osuusluku;
		Oo_os = CBOsuudet->ItemIndex - 1;
		epv = Oo_os;
		}
	if (CBOsuudet->ItemIndex == 0) {
		Oo_os = 0;
		npv = kilpparam.osuusluku;
		}
	setOoGrid();
	EnterCriticalSection(&tall_CriticalSection);
	teeOoJarj();
	LeaveCriticalSection(&tall_CriticalSection);
	OoGrid->RowCount = NKilp + 1;
	if (NKilp == 0)
		return;
	OoGrid->FixedRows = 1;
	for (int k = 1; k <= NKilp; k++) {
		Kilp = KilpA+OoJarj[k-1].Dk;
		DKilp[k] = OoJarj[k-1].Dk;
		for (int i = 0; i < sizeof(OoGridCols0)/sizeof(ColTp); i++) {
			if (OoGridCols[i].Visible) {
				int col = OoGridCols[i].Order;

				ColIx[col] = i;
				ColPv[col] = 0;

				switch (i) {
					case OCOLKNO:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->kilpno);
						break;
					case OCOLSARJA:
						OoGrid->Cells[col][k] = UnicodeString(Sarjat[Kilp->sarja].sarjanimi);
						break;
					case OCOLSEURA:
						OoGrid->Cells[col][k] = Kilp->Seura(st, 60);;
						break;
					case OCOLJK:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->joukkue);
						break;
					case OCOLMAA:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->Maa(st));
						break;
					case OCOLIDNO:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->joukkid);
						break;
					case OCOLJNIMI:
						MbsToWcs(st, Kilp->joukknimi, 40);
						OoGrid->Cells[col][k] = UnicodeString(st);
						break;
					case OCOLPIIRI:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->piiri);
						break;
					case OCOLERA:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->era);
						break;
					case OCOLSNIMI:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->SukuNimi(st, 50, epv+ipv));
							}
						break;
					case OCOLENIMI:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->EtuNimi(st, 50, epv+ipv));
							}
						break;
					case OCOLLIS:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							if (Lisenssinumero1->Checked)
								tl = Kilp->ostiet[epv+ipv].lisno;
							if (Osuuspisteet11->Checked)
								tl = Kilp->ostiet[epv+ipv].pisteet[0];
							if (Osuuspisteet21->Checked)
								tl = Kilp->ostiet[epv+ipv].pisteet[1];
							if (Osuuspisteet31->Checked)
								tl = Kilp->ostiet[epv+ipv].pisteet[2];
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(tl);
							}
						break;
					case OCOLARVO:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							MbsToWcs(st, Kilp->ostiet[epv+ipv].arvo, 60);
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(st);
							}
						break;
					case OCOLOSRA:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							MbsToWcs(st, Kilp->ostiet[epv+ipv].osseura, 60);
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(st);
							}
						break;
					case OCOLBDG:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->ostiet[epv+ipv].badge[0]);
							}
						break;
					case OCOLBDG2:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->ostiet[epv+ipv].badge[1]);
							}
						break;
					case OCOLLNA:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							if (Kilp->ostiet[epv+ipv].laina[0] == 'L')
								OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(L"Laina");
							else
								OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(L"");
							}
						break;
					case OCOLLAHTO:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp->ostiet[epv+ipv].ylahto, t0)).SubString(1, kilpparam.laika);
							}
						break;
					case OCOLLHTLJ:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(LLaji[Kilp->ostiet[epv+ipv].lahtolaji+1]);
							}
						break;
					case OCOLAIKA:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp->Maali(epv+ipv, 0), t0)).SubString(1, kilpparam.laika);
							}
						break;
					case OCOLTARK:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							switch (Kilp->wTark(epv+ipv, 0)) {
								case L'K' :
									wcscpy(line, L"Kesk");
									break;
								case L'O' :
									wcscpy(line, L"Ohit");
									break;
								case L'H' :
									wcscpy(line, L"Hyl");
									break;
								case L'E' :
									wcscpy(line, L"Eiläht.");
									break;
								case L'T' :
									wcscpy(line, L"Tark.");
									break;
								case L'I' :
									wcscpy(line, L"esItys");
									break;
								case L'P' :
									wcscpy(line, L"Poissa");
									break;
								default :
									wcscpy(line, L"Avoin");
									break;
								}
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(line);
							}
						break;
					case OCOLASAKOT:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							if (Sarjat[Kilp->sarja].paikat[epv+ipv])
								OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->Ampsakot(epv+ipv));
							else
								OoGrid->Cells[col+ipv*n_monios][k] = L"";
							}
						break;
					case OCOLSAKKO:
						for (int ipv = 0; ipv < npv; ipv++) {
							int kerr;
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							kerr = Sarjat[Kilp->sarja].tsak[epv+ipv]*SEK;
							if (kerr == 0)
								kerr = 1;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->ostiet[epv+ipv].sakko/kerr);
							}
						break;
					case OCOLRATA:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->Hajonta(epv+ipv));
							}
						break;
					case OCOLGPS:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv*n_monios] = i;
							ColPv[col+ipv*n_monios] = ipv;
							OoGrid->Cells[col+ipv*n_monios][k] = UnicodeString(Kilp->Seuranta(epv+ipv));
							}
						break;
					}
				}
			}
		}
}

//---------------------------------------------------------------------------

int __fastcall TFormJoukkueet::tallennaTiedot(void)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj, errrivi = 0;
	int tl, tlsum, erolkm = 0, muutlkm = 0, viimos;
	kilptietue Kilp, entKilp;

	LblMuokkaus->Caption = L"Tallentaa tietoja";
	LblMuokkaus->Visible = true;
	OoGrid->Options >> goEditing;
//	Lueemitkoodeja1->Enabled = false;
	Suodata1->Enabled = true;
	CBJarjestys->Enabled = true;
	memset(line, 0, sizeof(line));
	EnterCriticalSection(&tall_CriticalSection);
	for (int vaihe = 0; vaihe < 2; vaihe++) {
		for (int k = 1; k < OoGrid->RowCount; k++) {
			int kno = _wtoi(OoGrid->Cells[0][k].c_str());
			if ((d = OoJarj[k-1].Dk) <= 0) {
				errrivi = kno;
				continue;
				}
			Kilp.getrec(d);
			entKilp = Kilp;
			viimos = npv;
			if (viimos >= Sarjat[Kilp.sarja].osuusluku)
				viimos > Sarjat[Kilp.sarja].osuusluku - 1;
			for (int i = 0; i < (int) (sizeof(OoGridCols0)/sizeof(ColTp)); i++) {
				if (OoGridCols[i].Visible) {
					int col = OoGridCols[i].Order;
					int srj;

					switch (i) {
						case OCOLSARJA:
							srj = haesarja_w(OoGrid->Cells[col][k].c_str(), false);
							if (srj < 0) {
								errrivi = kno;
								break;
								}
							Kilp.sarja = srj;
							break;
						case OCOLSEURA:
							Kilp.setSeura(OoGrid->Cells[col][k].c_str());
							break;
						case OCOLJK:
							Kilp.joukkue = OoGrid->Cells[col][k].ToInt();
							break;
						case OCOLMAA:
							Kilp.setMaa(OoGrid->Cells[col][k].c_str());
							break;
						case OCOLIDNO:
							Kilp.joukkid = OoGrid->Cells[col][k].ToInt();
							break;
						case OCOLJNIMI:
							WcsToMbs(Kilp.joukknimi, OoGrid->Cells[col][k].c_str(), sizeof(Kilp.joukknimi)-1);
							break;
						case OCOLPIIRI:
							Kilp.piiri = OoGrid->Cells[col][k].ToInt();
							break;
						case OCOLERA:
							Kilp.era = OoGrid->Cells[col][k].ToInt();
							break;
						case OCOLSNIMI:
							for (int ipv = 0; ipv < viimos; ipv++)
								Kilp.setNimi(OoGrid->Cells[col+ipv*n_monios+1][k].c_str(),
									OoGrid->Cells[col+ipv*n_monios][k].c_str(), epv+ipv);
							break;
						case OCOLENIMI:
							break;
						case OCOLLIS:
							for (int ipv = 0; ipv < viimos; ipv++) {
								tl = OoGrid->Cells[col+ipv*n_monios][k].ToInt();
								if (Lisenssinumero1->Checked)
									Kilp.ostiet[epv+ipv].lisno = tl;
								if (Osuuspisteet11->Checked)
									Kilp.ostiet[epv+ipv].pisteet[0] = tl;
								if (Osuuspisteet21->Checked)
									Kilp.ostiet[epv+ipv].pisteet[1] = tl;
								if (Osuuspisteet31->Checked)
									Kilp.ostiet[epv+ipv].pisteet[2] = tl;
								}
							break;
						case OCOLARVO:
							for (int ipv = 0; ipv < viimos; ipv++)
								WcsToMbs(Kilp.ostiet[epv+ipv].arvo, OoGrid->Cells[col+ipv*n_monios][k].c_str(),
									sizeof(Kilp.ostiet[0].arvo)-1);
							break;
						case OCOLOSRA:
							for (int ipv = 0; ipv < viimos; ipv++)
								WcsToMbs(Kilp.ostiet[epv+ipv].osseura, OoGrid->Cells[col+ipv*n_monios][k].c_str(),
									sizeof(Kilp.ostiet[0].osseura)-1);
							break;
						case OCOLBDG:
							for (int ipv = 0; ipv < viimos; ipv++)
								Kilp.ostiet[epv+ipv].badge[0] = OoGrid->Cells[col+ipv*n_monios][k].ToInt();
							break;
						case OCOLBDG2:
							for (int ipv = 0; ipv < viimos; ipv++)
								Kilp.ostiet[epv+ipv].badge[1] = OoGrid->Cells[col+ipv*n_monios][k].ToInt();
							break;
						case OCOLLNA:
							for (int ipv = 0; ipv < viimos; ipv++) {
								if (OoGrid->Cells[col+ipv*n_monios][k].Length() > 0 &&
									towupper(OoGrid->Cells[col+ipv*n_monios][k][0]) == L'L')
									Kilp.ostiet[epv+ipv].laina[0] = 'L';
								else
									Kilp.ostiet[epv+ipv].laina[0] = 0;
								}
							break;
						case OCOLLAHTO:
							for (int ipv = 0; ipv < viimos; ipv++) {
								Kilp.ostiet[epv+ipv].ylahto =
									wstrtoaika_vap(OoGrid->Cells[col+ipv*n_monios][k].c_str(), t0);
								if (Kilp.ostiet[epv+ipv].ylahto == 0 &&
									OoGrid->Cells[col+ipv*n_monios][k].c_str()[0] != L'0')
									Kilp.ostiet[epv+ipv].ylahto = TMAALI0;
								}
							break;
						case OCOLLHTLJ:
							for (int ipv = 0; ipv < viimos; ipv++) {
								if (OoGrid->Cells[col+ipv*n_monios][k].Length() > 0) {
									Kilp.ostiet[epv+ipv].lahtolaji =
										wcswcind(towupper(OoGrid->Cells[col+ipv*n_monios][k].c_str()[0]), L"AYK");
									if (Kilp.ostiet[epv+ipv].lahtolaji > 2)
										Kilp.ostiet[epv+ipv].lahtolaji = 0;
									}
								else
									Kilp.ostiet[epv+ipv].lahtolaji = 0;
								}
							break;
						case OCOLAIKA:
							for (int ipv = 0; ipv < viimos; ipv++) {
								Kilp.ostiet[epv+ipv].va[0].aika =
									wstrtoaika_vap(OoGrid->Cells[col+ipv*n_monios][k].c_str(), t0);
								if (Kilp.ostiet[epv+ipv].va[0].aika == 0 &&
									OoGrid->Cells[col+ipv*n_monios][k].c_str()[0] != L'0')
									Kilp.ostiet[epv+ipv].va[0].aika = TMAALI0;
								}
							break;
						case OCOLTARK:
							for (int ipv = 0; ipv < viimos; ipv++) {
								if (OoGrid->Cells[col+ipv*n_monios][k].Length() > 0) {
									switch (towupper(OoGrid->Cells[col+ipv*n_monios][k].c_str()[0])) {
										case L'E' :
											if (towupper(OoGrid->Cells[col+ipv*n_monios][k].c_str()[1]) != L'S')
												Kilp.SetTark(epv+ipv, L'E');
											else
												Kilp.SetTark(epv+ipv, L'I');
											break;
										case L'L' :
										case L'A' :
											Kilp.SetTark(epv+ipv, L'-');
											break;
										default :
											Kilp.SetTark(epv+ipv,
												towupper(OoGrid->Cells[col+ipv*n_monios][k].c_str()[0]));
											break;
										}
									}
								else
									Kilp.SetTark(epv+ipv, L'-');
								}
							break;
						case OCOLASAKOT:
							for (int ipv = 0; ipv < viimos; ipv++) {
								WcsToMbs(Kilp.ostiet[epv+ipv].ampsakot,
									OoGrid->Cells[col+ipv*n_monios][k].c_str(), LSAKOT);
								Kilp.ostiet[epv+ipv].ampsakot[Sarjat[Kilp.sarja].paikat[epv+ipv]] = 0;
								}
							break;
						case OCOLSAKKO:
							for (int ipv = 0; ipv < viimos; ipv++) {
								int kerr;
								kerr = Sarjat[Kilp.sarja].tsak[epv+ipv]*SEK;
								if (kerr == 0)
									kerr = 1;
								Kilp.ostiet[epv+ipv].sakko =
									_wtoi(OoGrid->Cells[col+ipv*n_monios][k].c_str()) * kerr;
								}
							break;
						case OCOLRATA:
							for (int ipv = 0; ipv < viimos; ipv++) {
								WcsToMbs(Kilp.ostiet[epv+ipv].hajonta,
									OoGrid->Cells[col+ipv*n_monios][k].c_str(), sizeof(Kilp.ostiet[0].hajonta)-1);
								}
							break;
						case OCOLGPS:
							for (int ipv = 0; ipv < viimos; ipv++) {
								if (OoGrid->Cells[col+ipv*n_monios][k].Length() > 0)
									Kilp.ostiet[epv+ipv].seuranta =
										OoGrid->Cells[col+ipv*n_monios][k].c_str()[0];
								else
									Kilp.ostiet[epv+ipv].seuranta = 0;
								}
							break;
						}
					}
				}
			if (errrivi) {
				LeaveCriticalSection(&tall_CriticalSection);
				Application->MessageBox((UnicodeString(L"Virhe joukkueen ") + UnicodeString(errrivi)
					+ UnicodeString(L" tiedoissa. Loppuja ei tallennettu.")).c_str(), L"Virhe", MB_OK);
				EnterCriticalSection(&tall_CriticalSection);
				break;
				}
			if (vaihe == 0) {
				if (!(Kilp == entKilp)) {
					erolkm++;
					if (!(entKilp == KilpA[d]))
						muutlkm++;
					}
				}
			else {
				if (!(Kilp == entKilp) && entKilp == KilpA[d]) {
					Kilp.Tallenna(d, 0, 0, 0, &entKilp);
					KilpA[d] = Kilp;
					}
				}
			}
		if (vaihe == 0) {
			if (erolkm <= 0) {
				LeaveCriticalSection(&tall_CriticalSection);
				Application->MessageBox(L"Ei muutoksia tallennettavaksi", L"Ei muutoksia", MB_OK);
				EnterCriticalSection(&tall_CriticalSection);
				break;
				}
			if (erolkm > 0) {
				LeaveCriticalSection(&tall_CriticalSection);
				if (muutlkm == erolkm) {
					Application->MessageBox(L"Ei tallennettavissa olevia muutoksia johtuen muualla tehdyistä samojen joukkueiden muutoksista", L"Ei tallenneta muutoksia", MB_OK);
					EnterCriticalSection(&tall_CriticalSection);
					break;
					}
				else if (muutlkm > 0) {
					if (Application->MessageBox((UnicodeString(erolkm)+L" joukkueen tiedoissa muutos, näistä "+
						UnicodeString(muutlkm)+L" ei voida tallentaa muualla tehdyn muutoksen takia, tallennetaanko loput").c_str(),
						L"Muutoksia", MB_YESNO) != IDYES) {
						EnterCriticalSection(&tall_CriticalSection);
						break;
						}
					}
				else {
					if (Application->MessageBox((UnicodeString(erolkm)+L" joukkueen tiedoissa muutos, tallennetaanko").c_str(),
						L"Muutoksia", MB_YESNO) != IDYES) {
						EnterCriticalSection(&tall_CriticalSection);
						break;
						}
					}
				EnterCriticalSection(&tall_CriticalSection);
				}
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	LblMuokkaus->Caption = L"Muokkaus sallittu";
	LblMuokkaus->Visible = OoGrid->Options.Contains(goEditing);
	return(0);
}

//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::FormShow(TObject *Sender)
{
	static bool initialisoitu = false;

	if (!initialisoitu) {
		initOoGrid();
		if (ToimintaTila == 1)
			CBJarjestys->ItemIndex = 1;
		else
			CBJarjestys->ItemIndex = 0;
		CBOsuudet->Clear();
		CBOsuudet->Items->Add(L"Kaikki osuudet");
		for (int i = 0; i < kilpparam.osuusluku; i++) {
			CBOsuudet->Items->Add(UnicodeString(i+1)+UnicodeString(L". osuus"));
			}
		Oo_os = 0;
		CBOsuudet->ItemIndex = 0;
		initialisoitu = true;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

int __fastcall TFormJoukkueet::paivitaMuutos(int col, int row)
{
	int err = 0;
	int i, k, srj, ipv, i_pv;
	int tl;
	wchar_t line[20], kh;
	kilptietue Kilp;

	k = row;
	i_pv = 0;
	for (i = col-n_monios; i > 0 && ColIx[i] == ColIx[col]; i--)
		i_pv++;
	srj = haesarja_w(OoGrid->Cells[1][k].c_str(), false);
	switch (ColIx[col]) {
		case OCOLSARJA:
			srj = haesarja_w(OoGrid->Cells[col][k].c_str(), false);
			if (srj < 0) {
				err = 1;
				break;
				}
			OoGrid->Cells[col][k] = UnicodeString(Sarjat[srj].sarjanimi);
			break;
		case OCOLSEURA:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lseura);
			break;
		case OCOLJK:
		case OCOLIDNO:
		case OCOLPIIRI:
		case OCOLERA:
		case OCOLLIS:
		case OCOLBDG:
		case OCOLBDG2:
		case OCOLSAKKO:
			OoGrid->Cells[col][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
			break;
		case OCOLMAA:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, 3);
			break;
		case OCOLJNIMI:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.joukknimi)-1);
			break;
		case OCOLSNIMI:
		case OCOLENIMI:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lnimi);
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lnimi);
			break;
		case OCOLARVO:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.ostiet[0].arvo)-1);
			break;
		case OCOLOSRA:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.ostiet[0].osseura)-1);
			break;
		case OCOLLNA:
			if (towupper(OoGrid->Cells[col][k].c_str()[0]) == L'L')
				OoGrid->Cells[col][k] = UnicodeString(L"Laina");
			else
				OoGrid->Cells[col][k] = UnicodeString(L"");
			break;
		case OCOLLAHTO:
		case OCOLAIKA:
			tl = wstrtoaika_vap(OoGrid->Cells[col][k].c_str(), 0);
			if (tl == 0 && OoGrid->Cells[col][k].c_str()[0] != L'0')
				tl = TMAALI0;
			OoGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, 0)).SubString(1, kilpparam.laika);
			break;
		case OCOLLHTLJ:
			OoGrid->Cells[col][k] = LLaji[wcswcind(towupper(OoGrid->Cells[col][k].c_str()[0]), L"AYK")+1];
			break;
		case OCOLTARK:
			switch (towupper(OoGrid->Cells[col][k].c_str()[0])) {
				case L'E' :
					if (towupper(OoGrid->Cells[col][k].c_str()[1]) != L'S')
						kh = L'E';
					else
						kh = L'I';
					break;
				case L'L' :
					kh = L'-';
					break;
				default :
					kh = towupper(OoGrid->Cells[col][k].c_str()[0]);
					break;
				}
			switch (kh) {
				case L'K' :
					wcscpy(line, L"Kesk");
					break;
				case L'H' :
					wcscpy(line, L"Hyl");
					break;
				case L'E' :
					wcscpy(line, L"Eiläht.");
					break;
				case L'T' :
					wcscpy(line, L"Tark.");
					break;
				case L'I' :
					wcscpy(line, L"esItys");
					break;
				case L'P' :
					wcscpy(line, L"Poissa");
					break;
				default :
					wcscpy(line, L"Avoinna");
					break;
				}
			OoGrid->Cells[col][k] = UnicodeString(line);
			break;
		case OCOLASAKOT:
			if (srj < 0)
			   break;
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, Sarjat[srj].paikat[epv+i_pv]);
			break;
		case OCOLRATA:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.ostiet[0].hajonta)-1).UpperCase();
			break;
		case OCOLGPS:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].c_str()[0] > L' ' ? L"GPS" : L"";
			break;
		}
	OoGrid->Refresh();
	return(err);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (OoGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Button1Click(TObject *Sender)
{
	OoGrid->Options << goEditing;
	Suodata1->Enabled = false;
	CBJarjestys->Enabled = false;
	LblMuokkaus->Visible = true;
	if (ToimintaTila == 1)
		Lueemitkoodeja1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Button3Click(TObject *Sender)
{
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;
	OoGrid->Options >> goEditing;
	naytaTiedot();
	aktrow = rr;
	aktcol = cc;
	OoGrid->Row = aktrow;
	OoGrid->Col = aktcol;
	Suodata1->Enabled = true;
	CBJarjestys->Enabled = true;
	Lueemitkoodeja1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridExit(TObject *Sender)
{
	if (OoGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Button2Click(TObject *Sender)
{
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;
	if (tallennaTiedot() == 0) {
		OoGrid->Options >> goEditing;
		Lueemitkoodeja1->Enabled = false;
		naytaTiedot();
		}
	aktrow = rr;
	aktcol = cc;
	OoGrid->Row = aktrow;
	OoGrid->Col = aktcol;
	Suodata1->Enabled = true;
	CBJarjestys->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridDblClick(TObject *Sender)
{
	if (OoGrid->Row == 0)
		return;
	if (!FormJoukkuetiedot)
		FormJoukkuetiedot = new TFormJoukkuetiedot(this);

	if (!kuuluttaja)
		FormJoukkuetiedot->sallimuokkausvalinta = true;
	FormJoukkuetiedot->naytaKilpailija(DKilp[OoGrid->Row]);
	FormJoukkuetiedot->Show();
	if (FormJoukkuetiedot->WindowState == wsMinimized)
		FormJoukkuetiedot->WindowState = wsNormal;
	FormJoukkuetiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::CBOsuudetChange(TObject *Sender)
{
	if (OoGrid->Options.Contains(goEditing)) {
//		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
//			MB_YESNO) == IDYES)
			{
			tallennaTiedot();
			}
		OoGrid->Options >> (goEditing);
		Lueemitkoodeja1->Enabled = false;
		Suodata1->Enabled = true;
		CBJarjestys->Enabled = true;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::FormClose(TObject *Sender, TCloseAction &Action)

{
	if (OoGrid->Options.Contains(goEditing)) {
//		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
//			MB_YESNO) == IDYES)
			{
			tallennaTiedot();
			}
		OoGrid->Options >> (goEditing);
		Lueemitkoodeja1->Enabled = false;
		Suodata1->Enabled = true;
		CBJarjestys->Enabled = true;
		}
	poistaindeksi(&ooindex);
	delete[] KilpA;
	KilpA = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::CBJarjestysChange(TObject *Sender)
{
	if (OoGrid->Options.Contains(goEditing)) {
//		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
//			MB_YESNO) == IDYES)
			{
			tallennaTiedot();
			}
		OoGrid->Options >> (goEditing);
		Lueemitkoodeja1->Enabled = false;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkueet::OoGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	wchar_t msg[100], buf[50];
	kilptietue kilp;
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
		{
		MuokkaussallittuF91Click(Sender);
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (OoGrid->Col < OoGrid->ColCount-1)
			OoGrid->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (OoGrid->Col > 0)
			OoGrid->Col--;
		}
	if (aktrow > 0 && Key == VK_DELETE && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		kilp.getrec(DKilp[aktrow]);
		swprintf(msg, L"Vahvista joukkueen %s pysyvä poisto", kilp.Joukkue(buf, 49));
		if (Application->MessageBox(msg, L"Poisto", MB_YESNO) == IDYES) {
			deltall(DKilp[aktrow], 0);
			naytaTiedot();
			aktrow = rr;
			aktcol = cc;
			if (aktrow >= OoGrid->RowCount)
				aktrow = OoGrid->RowCount - 1;
			OoGrid->Row = aktrow;
			OoGrid->Col = aktcol;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridKeyPress(TObject *Sender, System::WideChar &Key)

{
/*	if (Key == L'\t') {

		OoGrid->Col++;
		aktcol = OoGrid->Col;
		Key = 0;
	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::BtnPaivitaClick(TObject *Sender)
{
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;

	if (OoGrid->Options.Contains(goEditing)) {
		OoGrid->Options >> (goEditing);
		Lueemitkoodeja1->Enabled = false;
		}
	naytaTiedot();
	if (rr >= OoGrid->RowCount)
		rr = OoGrid->RowCount - 1;
	if (rr > 0)
		OoGrid->Row = rr;
	if (cc > 0)
		OoGrid->Col = cc;
	aktrow = rr;
	aktcol = cc;
	Suodata1->Enabled = true;
	CBJarjestys->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------





void __fastcall TFormJoukkueet::JoukkIdClick(TObject *Sender)
{
	JoukkId->Checked = !JoukkId->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Nimi1Click(TObject *Sender)
{
	Nimi1->Checked = !Nimi1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Arvo1Click(TObject *Sender)
{
	Arvo1->Checked = !Arvo1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Seura1Click(TObject *Sender)
{
	Seura1->Checked = !Seura1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Laina1Click(TObject *Sender)
{
	Laina1->Checked = !Laina1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------


void __fastcall TFormJoukkueet::JoukkNimiClick(TObject *Sender)
{
	JoukkNimi->Checked = !JoukkNimi->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Maa1Click(TObject *Sender)
{
	Maa1->Checked = !Maa1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Aluepiiri1Click(TObject *Sender)
{
	Aluepiiri1->Checked = !Aluepiiri1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Era1Click(TObject *Sender)
{
	Era1->Checked = !Era1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Emitkoodi1Click(TObject *Sender)
{
	Emitkoodi1->Checked = !Emitkoodi1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::N2ekoodi1Click(TObject *Sender)
{
	N2ekoodi1->Checked = !N2ekoodi1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Lhtlaji1Click(TObject *Sender)
{
	Lhtlaji1->Checked = !Lhtlaji1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Lhtaika1Click(TObject *Sender)
{
	Lhtaika1->Checked = !Lhtaika1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Tulos1Click(TObject *Sender)
{
	Tulos1->Checked = !Tulos1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Sakot1Click(TObject *Sender)
{
	Sakot1->Checked = !Sakot1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::initOoGrid(void)
{
	Maa1->Checked = OoGridCols[OCOLMAA].Visible;
	JoukkId->Checked = OoGridCols[OCOLIDNO].Visible;
	JoukkNimi->Checked = OoGridCols[OCOLJNIMI].Visible;
	Aluepiiri1->Checked = OoGridCols[OCOLPIIRI].Visible;
	Era1->Checked = OoGridCols[OCOLERA].Visible;
	Nimi1->Checked = OoGridCols[OCOLSNIMI].Visible;
	Lisenssinumero1->Checked = OoGridCols[OCOLLIS].Visible;
	Arvo1->Checked = OoGridCols[OCOLARVO].Visible;
	Seura1->Checked = OoGridCols[OCOLOSRA].Visible;
	Emitkoodi1->Checked = OoGridCols[OCOLBDG].Visible;
	N2ekoodi1->Checked = OoGridCols[OCOLBDG2].Visible;
	Laina1->Checked = OoGridCols[OCOLLNA].Visible;
	Lhtaika1->Checked = OoGridCols[OCOLLAHTO].Visible;
	Lhtlaji1->Checked = OoGridCols[OCOLLHTLJ].Visible;
	Tulos1->Checked = OoGridCols[OCOLAIKA].Visible;
	if (kilpparam.lsakmax > 0)
		Sakot1->Checked = OoGridCols[OCOLASAKOT].Visible;
	else
		Sakot1->Checked = OoGridCols[OCOLSAKKO].Visible;
	Rata1->Checked = OoGridCols[OCOLRATA].Visible;
	GPS1->Checked = OoGridCols[OCOLGPS].Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridFixedCellClick(TObject *Sender, int ACol,
		  int ARow)
{
	aktrow = ARow;
	if (ARow == 0) {
		switch (ColIx[ACol]) {
			case OCOLKNO:
				CBJarjestys->ItemIndex = 0;
				break;
			case OCOLSEURA:
				CBJarjestys->ItemIndex = 1;
				break;
			case OCOLSARJA:
				CBJarjestys->ItemIndex = 2;
				break;
			case OCOLAIKA:
				CBJarjestys->ItemIndex = 4;
				break;
			default:
				return;
			}
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::MuokkaussallittuF91Click(TObject *Sender)
{
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;
	if (OoGrid->Options.Contains(goEditing)) {
		tallennaTiedot();
		OoGrid->Options >> goEditing;
		Suodata1->Enabled = true;
		CBJarjestys->Enabled = true;
		Lueemitkoodeja1->Enabled = false;
		}
	else {
		OoGrid->Options << goEditing;
		Suodata1->Enabled = false;
		CBJarjestys->Enabled = false;
		if (ToimintaTila == 1)
			Lueemitkoodeja1->Enabled = true;
		}
	naytaTiedot();
	aktrow = rr;
	aktcol = cc;
	OoGrid->Row = aktrow;
	OoGrid->Col = aktcol;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Suodata1Click(TObject *Sender)
{
	if (!FormSuodatus)
		FormSuodatus = new TFormSuodatus(this);
	FormSuodatus->Show();
	if (FormSuodatus->WindowState == wsMinimized)
		FormSuodatus->WindowState = wsNormal;
	FormSuodatus->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (OoGrid->RowCount - OoGrid->FixedRows >= OoGrid->VisibleRowCount + OoGrid->TopRow)
		OoGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::OoGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (OoGrid->TopRow > OoGrid->FixedRows)
		OoGrid->TopRow--;
}
//---------------------------------------------------------------------------



void __fastcall TFormJoukkueet::Rata1Click(TObject *Sender)
{
	Rata1->Checked = !Rata1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

int start_regnly(int r_no);

void __fastcall TFormJoukkueet::Lueemitkoodeja1Click(TObject *Sender)
{
/*
	Lueemitkoodeja1->Checked = !Lueemitkoodeja1->Checked;
	if (Lueemitkoodeja1->Checked) {
		int vast = Application->MessageBoxW(L"Luettavan kortin koodi menee valitulle riville kaikille vaiheille."
			L" Merkitäänkö lainakortiksi? (Cancel peruuttaa luennan)", L"Lainakortti?", MB_YESNOCANCEL);
		if (vast == IDCANCEL) {
			Lueemitkoodeja1->Checked = false;
			return;
			}
		merklaina = vast == IDYES;
		for (int r_no = 0; r_no < NREGNLY; r_no++) {
		  if (regnly[r_no]) {
			 kaikki_ajat = 2;
			 timerfl = 1;
			 start_regnly(r_no);
			 Oo_r_no = r_no;
			 hae_puutt_badge();
			 break;
			 }
		  }
		}
*/		
}
//---------------------------------------------------------------------------
/*
MESSAGE void __fastcall TFormJoukkueet::ECodeHandler(TMyMessage &msg)
{
	if (sallimuokkaus && OoGrid->Row > 0 && Emitkoodi1->Checked &&
		Lueemitkoodeja1->Enabled) {
		for (int ipv = 0; ipv < npv; ipv++) {
			OoGrid->Cells[OoGridCols[19].Order+ipv][OoGrid->Row] = msg.wparam;
			if (merklaina)
				OoGrid->Cells[OoGridCols[20].Order+ipv][OoGrid->Row] = L'L';
			}
		hae_puutt_badge();
		}
}
*/
//---------------------------------------------------------------------------
void __fastcall TFormJoukkueet::hae_puutt_badge(void)
{
	for (int r = OoGrid->Row; r < OoGrid->RowCount; r++) {
		if (_wtoi(OoGrid->Cells[OoGridCols[OCOLBDG].Order][r].c_str()) == 0) {
			OoGrid->Row = r;
			break;
			}
		}
}

//---------------------------------------------------------------------------

void tall_badge(UINT32 badge)
{
//	SendMessage(FormJoukkueet->Handle,WM_MYECODE,badge,0);
}


void __fastcall TFormJoukkueet::GPS1Click(TObject *Sender)
{
	GPS1->Checked = !GPS1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Kiinteitsarakkeita11Click(TObject *Sender)
{
	int FC = OoGrid->FixedCols;
	wchar_t ret;
	inputint_prompt(&FC, L"Anna kiinteiden sarakkeiden luku (1..12)", &ret, this);
	if (ret != ESC) {
		if (FC < 1)
			FC = 1;
		if (FC > 12)
			FC = 12;
		if (FC > OoGrid->ColCount)
			FC = OoGrid->ColCount;
		OoGrid->FixedCols = FC;
		Kiinteitsarakkeita11->Caption = L"Kiinteitä sarakkeita "+UnicodeString(FC);
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Lisenssinumero1Click(TObject *Sender)
{
	Lisenssinumero1->Checked = !Lisenssinumero1->Checked;
	if (Lisenssinumero1->Checked) {
		Osuuspisteet11->Checked = false;
		Osuuspisteet21->Checked = false;
		Osuuspisteet31->Checked = false;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Osuuspisteet11Click(TObject *Sender)
{
	Osuuspisteet11->Checked = !Osuuspisteet11->Checked;
	if (Osuuspisteet11->Checked) {
		Lisenssinumero1->Checked = false;
		Osuuspisteet21->Checked = false;
		Osuuspisteet31->Checked = false;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Osuuspisteet21Click(TObject *Sender)
{
	Osuuspisteet21->Checked = !Osuuspisteet21->Checked;
	if (Osuuspisteet21->Checked) {
		Lisenssinumero1->Checked = false;
		Osuuspisteet11->Checked = false;
		Osuuspisteet31->Checked = false;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueet::Osuuspisteet31Click(TObject *Sender)
{
	Osuuspisteet31->Checked = !Osuuspisteet31->Checked;
	if (Osuuspisteet31->Checked) {
		Lisenssinumero1->Checked = false;
		Osuuspisteet11->Checked = false;
		Osuuspisteet21->Checked = false;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

