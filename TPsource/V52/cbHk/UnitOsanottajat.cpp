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

#include "UnitOsanottajat.h"
#include "UnitKilpailijatiedot.h"
#include "UnitSuodatus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOsanottajat *FormOsanottajat;
//---------------------------------------------------------------------------

#define COLIDKilpNo		0
#define COLIDSarja		1
#define COLIDSukup		2
#define COLIDIkaSrj		3
#define COLIDIdno		4
#define COLIDKvId		5
#define COLIDSukunimi	6
#define COLIDEtunimi	7
#define COLIDArvo		8
#define COLIDSeura		9
#define COLIDSeuralyh	10
#define COLIDYhdistys	11
#define COLIDJoukkue	12
#define COLIDMaa		13
#define COLIDPiiri		14
#define COLIDIlmlista	15
#define COLIDArvryhma	16
#define COLIDPist1		17
#define COLIDPist2		18
#define COLIDPist3		19
#define COLIDRintaNo	20
#define COLIDVaihesarja 21
#define COLIDBadge		22
#define COLIDLaina		23
#define COLIDTas		24
#define COLIDLasna		25
#define COLIDLahto		26
#define COLIDTulos		27
#define COLIDAsakot		28
#define COLIDRata		29
#define COLIDEra		30
#define COLIDQual		31
#define COLIDGSM		32
#define COLIDVPist1		33
#define COLIDVPist2		34
#define COLIDTeksti		35


ColTp OoGridCols0[] = {
	{L"KilpNo", 40, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, true},
	{L"Sukup", 40, 0, 1, 0, false},
	{L"IkäSrj", 40, 0, 1, 0, false},
	{L"Id-no", 50, 0, 1, 0, true},
	{L"KvId", 50, 0, 1, 0, true},
	{L"Sukunimi", 80, 0, 1, 0, true},
	{L"Etunimi", 80, 0, 1, 0, true},
	{L"Arvo", 50, 0, 1, 0, false},
	{L"Seura", 100, 0, 1, 0, true},
	{L"Seuralyh", 50, 0, 1, 0, true},
	{L"Yhdistys", 50, 0, 1, 0, false},
	{L"Joukkue", 50, 0, 1, 0, false},
	{L"Maa", 30, 0, 1, 0, true},
	{L"Piiri", 40, 0, 1, 0, false},
	{L"Ilm.lista", 50, 0, 1, 0, false},
	{L"Arv.ryhmä", 50, 0, 1, 0, false},
	{L"Pist-1", 40, 0, 1, 0, false},
	{L"Pist-2", 40, 0, 1, 0, false},
	{L"Pist-3", 40, 0, 1, 0, false},
	{L"RintaNo", 50, 0, N_PV, 0, true},
	{L"Vaihesarja", 60, 0, N_PV, 0, false},
	{L"Badge", 50, 0, N_PV, 0, true},
	{L"Laina", 30, 0, N_PV, 0, true},
	{L"Tas.", 40, 0, N_PV, 0, false},
	{L"Läsnä", 35, 0, N_PV, 0, true},
	{L"Lähtö", 65, 0, N_PV, 0, true},
	{L"Tulos", 70, 0, N_PV, 0, true},
	{L"A-sakot", 50, 0, N_PV, 0, false},
	{L"Rata", 50, 0, N_PV, 0, false},
	{L"Erä", 50, 0, N_PV, 0, false},
	{L"Qual", 30, 0, N_PV, 0, false},
	{L"GSM", 30, 0, N_PV, 0, false},
	{L"VPist-1", 45, 0, N_PV, 0, false},
	{L"VPist-2", 45, 0, N_PV, 0, false},
	{L"Teksti", 80, 0, N_PV, 0, false}
};

ColTp OoGridCols[sizeof(OoGridCols0)/sizeof(ColTp)];

//---------------------------------------------------------------------------

__fastcall TFormOsanottajat::TFormOsanottajat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	memcpy(OoGridCols, OoGridCols0, sizeof(OoGridCols0));
	sallimuokkaus = true;
	memset(&ooindex, 0, sizeof(ooindex));
}

//---------------------------------------------------------------------------
void __fastcall TFormOsanottajat::FormResize(TObject *Sender)
{
	OoGrid->Width = ClientWidth - 16 * Screen->PixelsPerInch / 96;
	OoGrid->Height = ClientHeight - OoGrid->Top - 8 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

bool __fastcall TFormOsanottajat::Mukana(int d)
{
	wchar_t st[100], st1[100];
	int i_pv, tls;

	st[99] = 0;
	st1[99] = 0;
	if (!FormSuodatus)
		return(true);
	if (CBPaivat->ItemIndex == 0)
		i_pv = FormSuodatus->CBVaihe->ItemIndex;
	else
		i_pv = Oo_pv;
	if (FormSuodatus->CBSarja->ItemIndex > 0) {
		int srj;
		srj = KilpA[d].Sarja(i_pv);
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
		wcsncpy(seura, KilpA[d].seuratieto(0), LSEURA);
		seura[LSEURA] = 0;
		if (wcscmp(st1, KilpA[d].maa) || wcscmp(seura, st1+4))
			return(false);
		}

	if (FormSuodatus->CBTark->ItemIndex > 0) {
		wchar_t kh;
		kh = KilpA[d].tark(i_pv);
		if (L" -THKIEVPXMB"[FormSuodatus->CBTark->ItemIndex] != kh)
			return(false);
		}

	if (FormSuodatus->RGPeruste->ItemIndex <= 0)
		return(true);

	wcsncpy(st, FormSuodatus->EdtTieto->Text.c_str(), 99);
	switch (FormSuodatus->RGPeruste->ItemIndex-1) {
		case 0:
			KilpA[d].nimi(st1, 60, false);
			upcasewstr(st1);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 1:
			wcscpy(st1, KilpA[d].yhdistys);
			upcasewstr(st1);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 2:
			wcscpy(st1, KilpA[d].joukkue);
			upcasewstr(st1);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 3:
			wcscpy(st1, KilpA[d].maa);
			upcasewstr(st1);
			upcasewstr(st);
			return(wmemcmp(st, st1, wcslen(st)) == 0);
		case 4:
			return(KilpA[d].kilpno == _wtoi(st));
		case 5:
			return(KilpA[d].id(i_pv) == _wtoi(st));
		case 6:
			return(KilpA[d].pv[i_pv].badge[0] == _wtoi(st));
		case 7:
			return(KilpA[d].lisno[0] == _wtoi(st));
		case 8:
			return(KilpA[d].lisno[1] == _wtoi(st));
		case 9:
			if (st[0] > L' ')
				return(KilpA[d].pv[i_pv].qual >= 'A');
			else
				return(KilpA[d].pv[i_pv].qual < 'A');
		case 10:
			if (st[0] > L' ')
				return(KilpA[d].pv[i_pv].gps >= 'A');
			else
				return(KilpA[d].pv[i_pv].gps < 'A');
		case 11:
			tls = wstrtoaika_vap(st, 0);
			return(KilpA[d].p_aika(0) <= tls);
		}
	return(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::KilpKey(int d, char * key)
{
	wchar_t st[82];
	kilptietue *kilp;
	int kno;

	kilp = KilpA+d;
//	if (ToimintaTila < 2 && CBPaivat->ItemIndex == 0)
//		kno = kilp->id();
//	else
	kno = kilp->id(Oo_Jrjpv);
	switch (CBJarjestys->ItemIndex) {
		case 0:
			putintright(kno, key+5);
			break;
		case 1:
			keyfromwname(key, kilp->nimi(st, 60, false), 60, 0);
			break;
		case 2:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			putintright(kno, key+8);
			break;
		case 3:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			keyfromwname(key+4, kilp->nimi(st, 60, false), 60, 0);
			break;
		case 4:
			arv_pv = Oo_Jrjpv;
			keyjrj(kilp, key, 20, 0);
			arv_pv = k_pv;
			break;
		case 5:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			if (kilp->tulos_pv(Oo_Jrjpv, true) > 0)
				AIKATOSTRS(key+5, kilp->tulos_pv(Oo_Jrjpv, true), 0);
			else
				key[4] = "6033122445334"[wcswcind(kilp->tark(Oo_Jrjpv),L"T-IHKOEVPMXB")+1];
			break;
		case 6:
			keyfromwname(key, kilp->seuratieto(0), 30, 0);
			keyfromwname(key+30, kilp->nimi(st, 60, false), 50, 0);
			break;
		case 7:
			putintright(d, key+5);
			break;
		case 8:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			putintright(kilp->pisteet[0], key+12);
			break;
		case 9:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			putintright(kilp->pisteet[1], key+12);
			break;
		case 10:
			putintright(kilp->Sarja(Oo_Jrjpv), key+3);
			putintright(kilp->pisteet[2], key+12);
			break;
		case 11:
			putintright(kilp->pv[Oo_Jrjpv].badge[0], key+12);
			break;
		case 12:
			putintright(kilp->lisno[0], key+12);
			break;
		case 13:
			putintright(kilp->lisno[1], key+12);
			break;
		case 14:
			key[0] = "9102345678236"[wcswcind(kilp->tark(Oo_Jrjpv),L"T-IHKOEVPMXB")+1];
			putintright(kilp->id(Oo_Jrjpv), key+7);
			break;
		case 15:
			keyfromwname(key, kilp->maa, 3, 0);
			keyfromwname(key+3, kilp->seuratieto(0), 30, 0);
			keyfromwname(key+33, kilp->nimi(st, 60, false), 50, 0);
			break;
		case 16:
			key[0] = (char) Sarjat[kilp->Sarja(Oo_Jrjpv)].lno[Oo_Jrjpv];
			AIKATOSTRS(key+1, kilp->TLahto(Oo_Jrjpv), t0);
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::teeOoJarj(void)
{
	if (KilpA)
		delete[] KilpA;
	KilpA = new kilptietue[nrec];
	if (OoJarj)
		delete[] OoJarj;
	OoJarj = new OoJarjTp[nrec];
	memset(OoJarj, 0, nrec*sizeof(OoJarj[0]));
	NKilp = 0;
	for (int d = 1; d < nrec; d++) {
		KilpA[d].GETREC(d);
		if (KilpA[d].kilpstatus == 0 && Mukana(d)) {
			KilpKey(d, OoJarj[NKilp].key);
			OoJarj[NKilp].Dk = d;
			NKilp++;
			}
		}
	quicksort((char *)OoJarj, NKilp, sizeof(OoJarj[0].key), sizeof(OoJarj[0]));
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::setOoGrid(void)
{
	int n;

	OoGridCols[COLIDKilpNo].Visible = Perusnumero2->Checked;
	OoGridCols[COLIDSarja].Visible = Sarja1->Checked;
	OoGridCols[COLIDSukup].Visible = Sukupx1->Checked;
	OoGridCols[COLIDIkaSrj].Visible = Iksarjag1->Checked;
	OoGridCols[COLIDIdno].Visible = Lisenssinumero1->Checked;
	OoGridCols[COLIDKvId].Visible = Kvidnumero1->Checked;
	OoGridCols[COLIDSukunimi].Visible = Nimi1->Checked;
	OoGridCols[COLIDEtunimi].Visible = Nimi1->Checked;
	OoGridCols[COLIDArvo].Visible = Arvo1->Checked;
	OoGridCols[COLIDSeura].Visible = Seura1->Checked;
	OoGridCols[COLIDSeuralyh].Visible = Seuralyhenne1->Checked;
	OoGridCols[COLIDYhdistys].Visible = Yhdistys1->Checked;
	OoGridCols[COLIDJoukkue].Visible = Joukkue1->Checked;
	OoGridCols[COLIDMaa].Visible = Maa1->Checked;
	OoGridCols[COLIDPiiri].Visible = Aluepiiri1->Checked;
	OoGridCols[COLIDIlmlista].Visible = Ilmoittautumislista1->Checked;
	OoGridCols[COLIDPist1].Visible = Pisteet1->Checked;
	OoGridCols[COLIDPist2].Visible = Pisteet2->Checked;
	OoGridCols[COLIDPist3].Visible = Pisteet3->Checked;
	OoGridCols[COLIDRintaNo].Visible = Rintanumero1->Checked;
	OoGridCols[COLIDVaihesarja].Visible = Vaiheensarja1->Checked;
	OoGridCols[COLIDBadge].Visible = Emitkoodi1->Checked;
	OoGridCols[COLIDLaina].Visible = Emitkoodi1->Checked;
	OoGridCols[COLIDTas].Visible = (kilpparam.lsakmax == 0 && Sakot1->Checked);
	OoGridCols[COLIDLasna].Visible = Lsn1->Checked;
	OoGridCols[COLIDLahto].Visible = Lhtaika1->Checked;
	OoGridCols[COLIDTulos].Visible = Tulos1->Checked;
	OoGridCols[COLIDAsakot].Visible = (kilpparam.lsakmax > 0 && Sakot1->Checked);
	OoGridCols[COLIDRata].Visible = Rata1->Checked;
	OoGridCols[COLIDEra].Visible = Er1->Checked;
	OoGridCols[COLIDQual].Visible = Qual1->Checked;
	OoGridCols[COLIDGSM].Visible = GSM1->Checked;
	OoGridCols[COLIDVPist1].Visible = Vaihepist11->Checked;
	OoGridCols[COLIDVPist2].Visible = Vaihepist21->Checked;
	OoGrid->RowCount = 1;
	OoGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	if (OoGrid->FixedCols > 5)
		OoGrid->ColCount = OoGrid->FixedCols;
	else
		OoGrid->ColCount = 5;
	n = 0;
	for (int i = 0; i < sizeof(OoGridCols)/sizeof(ColTp); i++) {
		if (OoGridCols[i].Visible ||
			(i == 0 && kilpparam.bibvaihtelee && CBPaivat->ItemIndex > 0) ||
			(i == COLIDSarja && kilpparam.sarjavaihtelee && CBPaivat->ItemIndex > 0)) {
			int ii, nlisa;

			ii = i;
			if (kilpparam.bibvaihtelee && CBPaivat->ItemIndex > 0) {
				if (i == COLIDRintaNo)
					continue;
				if (i == 0)
					ii = COLIDRintaNo;
				}
			if (kilpparam.sarjavaihtelee && CBPaivat->ItemIndex > 0) {
				if (i == COLIDVaihesarja)
					continue;
				if (i == COLIDSarja)
					ii = COLIDVaihesarja;
				}
			if (OoGridCols0[ii].Count > 1)
				nlisa = npv;
			else
				nlisa = 1;
			if (n+nlisa > OoGrid->ColCount) {
				if (OoGrid->FixedCols > n+nlisa)
					OoGrid->FixedCols = n+nlisa;
				OoGrid->ColCount = n+nlisa;
				}
			OoGridCols[ii].Order = n;
			OoGridCols[ii].Count = nlisa;
			for (int ipv = 0; ipv < nlisa; ipv++) {
				OoGrid->Cells[n+ipv][0] = UnicodeString(OoGridCols[ii].Caption);
				OoGrid->ColWidths[n+ipv] = OoGridCols[ii].Width * Screen->PixelsPerInch / 96;
				}
			n += nlisa;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::naytaTiedot(void)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
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
	if (CBPaivat->ItemIndex > 0) {
		if (CBPaivat->ItemIndex > kilpparam.n_pv_akt)
		   CBPaivat->ItemIndex = kilpparam.n_pv_akt;
		Oo_pv = CBPaivat->ItemIndex - 1;
		epv = Oo_pv;
		Oo_Jrjpv = Oo_pv;
		}
	if (CBPaivat->ItemIndex == 0) {
		if (ToimintaTila == 2) {
			Oo_pv = k_pv;
			}
		else {
			Oo_pv = 0;
			}
		Oo_Jrjpv = CBJarjestys->ItemIndex == 0 ? -1 : Oo_pv;
		npv = kilpparam.n_pv_akt;
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
					case COLIDKilpNo:
						if (CBPaivat->ItemIndex == 0)
							OoGrid->Cells[col][k] = UnicodeString(Kilp->id());
						else
							OoGrid->Cells[col][k] = UnicodeString(Kilp->id(Oo_pv));
						break;
					case COLIDSarja:
						if (CBPaivat->ItemIndex == 0)
							OoGrid->Cells[col][k] = UnicodeString(Sarjat[Kilp->Sarja()].sarjanimi);
						else
							OoGrid->Cells[col][k] = UnicodeString(Sarjat[Kilp->Sarja(Oo_pv)].sarjanimi);
						break;
					case COLIDSukup:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->sukup);
						break;
					case COLIDIkaSrj:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->ikasarja);
						break;
					case COLIDIdno:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->lisno[0]);
						break;
					case COLIDKvId:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->lisno[1]);
						break;
					case COLIDSukunimi:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->sukunimi);
						break;
					case COLIDEtunimi:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->etunimi);
						break;
					case COLIDArvo:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->arvo);
						break;
					case COLIDSeura:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->seura);
						break;
					case COLIDSeuralyh:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->seuralyh);
						break;
					case COLIDYhdistys:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->yhdistys);
						break;
					case COLIDJoukkue:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->joukkue);
						break;
					case COLIDMaa:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->maa);
						break;
					case COLIDPiiri:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->piiri);
						break;
					case COLIDIlmlista:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->ilmlista);
						break;
					case COLIDArvryhma:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->arvryhma);
						break;
					case COLIDPist1:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->pisteet[0]);
						break;
					case COLIDPist2:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->pisteet[1]);
						break;
					case COLIDPist3:
						OoGrid->Cells[col][k] = UnicodeString(Kilp->pisteet[2]);
						break;
					case COLIDRintaNo:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].bib);
							}
						break;
					case COLIDVaihesarja:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Sarjat[Kilp->Sarja(epv+ipv)].sarjanimi);
							}
						break;
					case COLIDBadge:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].badge[0]);
							}
						break;
					case COLIDLaina:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							if (Kilp->pv[epv+ipv].laina[0] == L'L')
								OoGrid->Cells[col+ipv][k] = UnicodeString(L"Laina");
							else
								OoGrid->Cells[col+ipv][k] = UnicodeString(L"");
							}
						break;
					case COLIDTas:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].tasoitus);
							}
						break;
					case COLIDLasna:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							switch (Kilp->tark(epv+ipv)) {
								case L'K' :
									wcscpy(line, L"Kesk");
									break;
								case L'O' :
									wcscpy(line, L"Ohit");
									break;
								case L'X' :
									wcscpy(line, L"EiAikaa");
									break;
								case L'H' :
									wcscpy(line, L"Hyl");
									break;
								case L'M' :
									wcscpy(line, L"Virhe");
									break;
								case L'E' :
									wcscpy(line, L"Eiläht.");
									break;
								case L'B' :
									wcscpy(line, L"Havaittu");
									break;
								case L'T' :
									wcscpy(line, L"Tark.");
									break;
								case L'I' :
									wcscpy(line, L"esItys");
									break;
								case L'V' :
									wcscpy(line, L"Vakantti");
									break;
								case L'P' :
									wcscpy(line, L"Poissa");
									break;
								default :
									wcscpy(line, L"Läsnä");
									break;
								}
							OoGrid->Cells[col+ipv][k] = UnicodeString(line);
							}
						break;
					case COLIDLahto:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp->pv[epv+ipv].tlahto, t0)).SubString(1, pvparam[epv+ipv].laika);
							}
						break;
					case COLIDTulos:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] =
								UnicodeString(AIKATOWSTRS(line, Kilp->tulos_pv(epv+ipv), 0)).SubString(1, pvparam[epv+ipv].laika);
							}
						break;
					case COLIDAsakot:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							if (Sarjat[Kilp->Sarja(epv+ipv)].lsak[epv+ipv])
								OoGrid->Cells[col+ipv][k] = Kilp->asakkoStr(epv+ipv);
//								OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].asakot).SubString(1, Sarjat[Kilp->Sarja(epv+ipv)].lsak[epv+ipv]);
							else
								OoGrid->Cells[col+ipv][k] = L"";
							}
						break;
					case COLIDRata:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].rata);
							}
						break;
					case COLIDEra:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].era);
							}
						break;
					case COLIDQual:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].qual);
							}
						break;
					case COLIDGSM:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].gps);
							}
						break;
					case COLIDVPist1:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].pvpisteet[0]);
							}
						break;
					case COLIDVPist2:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].pvpisteet[1]);
							}
						break;
					case COLIDTeksti:
						for (int ipv = 0; ipv < npv; ipv++) {
							ColIx[col+ipv] = i;
							ColPv[col+ipv] = ipv;
							OoGrid->Cells[col+ipv][k] = UnicodeString(Kilp->pv[epv+ipv].txt);
							}
						break;
					}
				}
			}
		}
}

//---------------------------------------------------------------------------

int __fastcall TFormOsanottajat::tallennaTiedot(void)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj, errrivi = 0;
	int tl, tlsum, erolkm = 0, muutlkm = 0, dellkm = 0;
	bool delrivi;
	kilptietue Kilp, entKilp;

	OoGrid->Options >> goEditing;
	Lueemitkoodeja1->Enabled = false;
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
			Kilp.GETREC(d);
			entKilp = Kilp;
			for (int i = 0; i < sizeof(OoGridCols0)/sizeof(ColTp); i++) {
				if (OoGridCols[i].Visible) {
					int col = OoGridCols[i].Order;
					int srj;

					switch (i) {
						case COLIDSarja:
							srj = haesarja_w(OoGrid->Cells[col][k].c_str(), false);
							if (srj < 0) {
								errrivi = kno;
								break;
								}
							if (!kilpparam.sarjavaihtelee)
								Kilp.setSarjaAll(srj);
							else
								Kilp.setSarja(srj, ToimintaTila == 2 ? Oo_pv : -1);
							break;
						case COLIDSukup:
							Kilp.sukup = towupper(OoGrid->Cells[col][k].c_str()[0]);
							break;
						case COLIDIkaSrj:
							Kilp.ikasarja = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDIdno:
							Kilp.lisno[0] = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDKvId:
							Kilp.lisno[1] = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDSukunimi:
							wcsncpy(Kilp.sukunimi, OoGrid->Cells[col][k].c_str(), kilpparam.lsnimi);
							break;
						case COLIDEtunimi:
							wcsncpy(Kilp.etunimi, OoGrid->Cells[col][k].c_str(), kilpparam.lenimi);
							break;
						case COLIDArvo:
							wcsncpy(Kilp.arvo, OoGrid->Cells[col][k].c_str(), kilpparam.larvo);
							break;
						case COLIDSeura:
							wcsncpy(Kilp.seura, OoGrid->Cells[col][k].c_str(), kilpparam.lseura);
							break;
						case COLIDSeuralyh:
							wcsncpy(Kilp.seuralyh, OoGrid->Cells[col][k].c_str(), kilpparam.lseuralyh);
							break;
						case COLIDYhdistys:
							wcsncpy(Kilp.yhdistys, OoGrid->Cells[col][k].c_str(), kilpparam.lyhdistys);
							break;
						case COLIDJoukkue:
							wcsncpy(Kilp.joukkue, OoGrid->Cells[col][k].c_str(), kilpparam.ljoukkue);
							break;
						case COLIDMaa:
							wcsncpy(Kilp.maa, OoGrid->Cells[col][k].c_str(), 3);
							break;
						case COLIDPiiri:
							Kilp.piiri = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDIlmlista:
							Kilp.ilmlista = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDArvryhma:
							Kilp.arvryhma = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDPist1:
							Kilp.pisteet[0] = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDPist2:
							Kilp.pisteet[1] = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDPist3:
							Kilp.pisteet[2] = OoGrid->Cells[col][k].ToInt();
							break;
						case COLIDRintaNo:
							if (kilpparam.bibvaihtelee) {
								for (int ipv = 0; ipv < npv; ipv++)
									Kilp.pv[epv+ipv].bib = OoGrid->Cells[col+ipv][k].ToInt();
								}
							else
								Kilp.setIdAll(OoGrid->Cells[col][k].ToInt());
							break;
						case COLIDVaihesarja:
							for (int ipv = 0; ipv < npv; ipv++) {
								srj = haesarja_w(OoGrid->Cells[col+ipv][k].c_str(), false);
								if (srj < 0) {
									errrivi = kno;
									break;
									}
								Kilp.setSarja(srj, epv+ipv);
								}
							break;
						case COLIDBadge:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.pv[epv+ipv].badge[0] = OoGrid->Cells[col+ipv][k].ToInt();
							break;
						case COLIDLaina:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.pv[epv+ipv].laina[0] = OoGrid->Cells[col+ipv][k].c_str()[0];
							break;
						case COLIDTas:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.tall_sakko(OoGrid->Cells[col+ipv][k].ToInt(), epv+ipv);
							break;
						case COLIDLasna:
							delrivi = false;
							for (int ipv = 0; ipv < npv; ipv++) {
								switch (towupper(OoGrid->Cells[col+ipv][k].c_str()[0])) {
									case L'E' :
										if (towupper(OoGrid->Cells[col+ipv][k].c_str()[1]) == L'S')
											Kilp.set_tark(L'I', epv+ipv);
										else if (OoGrid->Cells[col+ipv][k].Length() > 2 &&
											towupper(OoGrid->Cells[col+ipv][k].c_str()[2]) == L'A')
											Kilp.set_tark(L'X', epv+ipv);
										else
											Kilp.set_tark(L'E', epv+ipv);
										break;
									case L'V' :
										if (towupper(OoGrid->Cells[col+ipv][k].c_str()[1]) == L'I')
											Kilp.set_tark(L'M', epv+ipv);
										else
											Kilp.set_tark(L'V', epv+ipv);
										break;
									case L'H' :
										if (towupper(OoGrid->Cells[col+ipv][k].c_str()[1]) == L'A')
											Kilp.set_tark(L'B', epv+ipv);
										else
											Kilp.set_tark(L'H', epv+ipv);
										break;
									case L'L' :
										Kilp.set_tark(L'-', epv+ipv);
										break;
									case L'D' :
										delrivi = true;
										break;
									default :
										Kilp.set_tark(towupper(OoGrid->Cells[col+ipv][k].c_str()[0]), epv+ipv);
										break;
									}
								}
							break;
						case COLIDLahto:
							for (int ipv = 0; ipv < npv; ipv++) {
								bool paivtot;
								paivtot = (Kilp.pv[epv+ipv].va[0].vatulos == Kilp.pv[epv+ipv].tlahto);
								Kilp.pv[epv+ipv].tlahto = wstrtoaika_vap(OoGrid->Cells[col+ipv][k].c_str(), t0);
								if (paivtot)
									Kilp.pv[epv+ipv].va[0].vatulos = Kilp.pv[epv+ipv].tlahto;
								}
							break;
						case COLIDTulos:
							for (int ipv = 0; ipv < npv; ipv++) {
								Kilp.tall_tulos_pv(wstrtoaika_vap(OoGrid->Cells[col+ipv][k].c_str(), 0), epv+ipv);
								}
							break;
						case COLIDAsakot:
							for (int ipv = 0; ipv < npv; ipv++) {
								Kilp.tall_asakot(OoGrid->Cells[col+ipv][k].c_str(), epv+ipv);
//								Kilp.pv[epv+ipv].asakot[Sarjat[Kilp.Sarja(epv+ipv)].lsak[epv+ipv]] = 0;
								}
/*
							if (kilpparam.kilplaji == L'A') {
								for (int ipv = 0; ipv < npv; ipv++) {
									Kilp.tall_asakot(OoGrid->Cells[col+ipv][k].c_str(), epv+ipv);
									Kilp.pv[epv+ipv].asakot[Sarjat[Kilp.Sarja(epv+ipv)].lsak[epv+ipv]] = 0;
									}
								}
*/
							break;
						case COLIDRata:
							for (int ipv = 0; ipv < npv; ipv++)
								wcsncpy(Kilp.pv[epv+ipv].rata, OoGrid->Cells[col+ipv][k].c_str(), sizeof(Kilp.pv[epv+ipv].rata)/2-1);
							break;
						case COLIDEra:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.pv[epv+ipv].era = OoGrid->Cells[col+ipv][k].ToInt();
							break;
						case COLIDQual:
							for (int ipv = 0; ipv < npv; ipv++)
								if (OoGrid->Cells[col+ipv][k].Length() > 0)
									Kilp.pv[epv+ipv].qual = (char)OoGrid->Cells[col+ipv][k].c_str()[0];
								else
									Kilp.pv[epv+ipv].qual = 0;
							break;
						case COLIDGSM:
							for (int ipv = 0; ipv < npv; ipv++)
								if (OoGrid->Cells[col+ipv][k].Length() > 0)
									Kilp.pv[epv+ipv].gps = (char)OoGrid->Cells[col+ipv][k].c_str()[0];
								else
									Kilp.pv[epv+ipv].gps = 0;
							break;
						case COLIDVPist1:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.pv[epv+ipv].pvpisteet[0] = _wtoi(OoGrid->Cells[col+ipv][k].c_str());
							break;
						case COLIDVPist2:
							for (int ipv = 0; ipv < npv; ipv++)
								Kilp.pv[epv+ipv].pvpisteet[1] = _wtoi(OoGrid->Cells[col+ipv][k].c_str());
							break;
						case COLIDTeksti:
							for (int ipv = 0; ipv < npv; ipv++)
								wcsncpy(Kilp.pv[epv+ipv].txt, OoGrid->Cells[col+ipv][k].c_str(), sizeof(Kilp.pv[epv+ipv].txt)/2-1);
							break;
						}
					}
				}
			if (errrivi) {
				LeaveCriticalSection(&tall_CriticalSection);
				Application->MessageBox((UnicodeString(L"Virhe kilpailijan ") + UnicodeString(errrivi)
					+ UnicodeString(L" tiedoissa. Loppuja ei tallennettu.")).c_str(), L"Virhe", MB_OK);
				EnterCriticalSection(&tall_CriticalSection);
				break;
				}
			if (vaihe == 0) {
				if (delrivi) {
					dellkm++;
					}
				else if (!(Kilp == entKilp)) {
					erolkm++;
					if (!(entKilp == KilpA[d]))
						muutlkm++;
					}
				}
			else {
				if (delrivi) {
					deltall(d, 0);
					}
				else if (!(Kilp == entKilp) && entKilp == KilpA[d]) {
					Kilp.tallenna(d, 0, 0, 0, &entKilp);
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
				if (dellkm) {
					if (Application->MessageBox((UnicodeString(dellkm)+L" kilpailijan tiedot poistetaan kokonaan. Vahvista poisto").c_str(),
						L"Tietojen poisto", MB_YESNO) != IDYES) {
						EnterCriticalSection(&tall_CriticalSection);
						break;
						}
					}
				if (muutlkm == erolkm) {
					Application->MessageBox(L"Ei tallennettavissa olevia muutoksia johtuen muualla tehdyistä samojen osanottajien muutoksista", L"Ei tallenneta muutoksia", MB_OK);
					EnterCriticalSection(&tall_CriticalSection);
					break;
					}
				else if (muutlkm > 0) {
					if (Application->MessageBox((UnicodeString(erolkm)+L" kilpailijan tiedoissa muutos, näistä "+
						UnicodeString(muutlkm)+L" ei voida tallentaa muualla tehdyn muutoksen takia, tallennetaanko loput").c_str(),
						L"Muutoksia", MB_YESNO) != IDYES) {
						EnterCriticalSection(&tall_CriticalSection);
						break;
						}
					}
				else {
					if (Application->MessageBox((UnicodeString(erolkm)+L" kilpailijan tiedoissa muutos, tallennetaanko").c_str(),
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
	return(0);
}

//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::FormShow(TObject *Sender)
{
	if (!initialisoitu) {
		initOoGrid();
		if (ToimintaTila == 1)
			CBJarjestys->ItemIndex = 1;
		else
			CBJarjestys->ItemIndex = 0;
		CBPaivat->Clear();
		CBPaivat->Items->Add(L"Kaikki vaiheet");
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			CBPaivat->Items->Add(UnicodeString(i+1)+UnicodeString(L". vaihe"));
			}
		Oo_pv = k_pv;
		Oo_Jrjpv = Oo_pv;
		CBPaivat->ItemIndex = 0;
		CBPaivatChange(Sender);
		initialisoitu = true;
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

int __fastcall TFormOsanottajat::paivitaMuutos(int col, int row)
{
	int err = 0;
	int i, k, srj, ipv, i_pv;
	int tl;
	wchar_t line[20], kh;
	kilptietue Kilp;

	k = row;
	i_pv = 0;
	for (i = col-1; i > 0 && ColIx[i] == ColIx[col]; i--)
		i_pv++;
	srj = haesarja_w(OoGrid->Cells[1][k].c_str(), false);
	switch (ColIx[col]) {
		case COLIDSarja:
		case COLIDVaihesarja:
			srj = haesarja_w(OoGrid->Cells[col][k].c_str(), false);
			if (srj < 0) {
				err = 1;
				break;
				}
			OoGrid->Cells[col][k] = UnicodeString(Sarjat[srj].sarjanimi);
			if (!kilpparam.sarjavaihtelee) {
				if (OoGridCols[COLIDVaihesarja].Visible) {
					for (ipv = 0; ipv < npv; ipv++)
					   OoGrid->Cells[OoGridCols[COLIDVaihesarja].Order+ipv][k] = OoGrid->Cells[col][k];
					}
				if (OoGridCols[COLIDSarja].Visible && ColIx[col] == COLIDVaihesarja)
				   OoGrid->Cells[OoGridCols[COLIDSarja].Order][k] = OoGrid->Cells[col][k];
				}
			break;
		case COLIDSukup:
			if (OoGrid->Cells[col][k].Length() > 0)
				OoGrid->Cells[col][k] = OoGrid->Cells[col][k].UpperCase();
			break;
		case COLIDIkaSrj:
		case COLIDIdno:
		case COLIDKvId:
			OoGrid->Cells[col][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
			break;
		case COLIDSukunimi:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lsnimi);
			break;
		case COLIDEtunimi:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lenimi);
			break;
		case COLIDArvo:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.larvo);
			break;
		case COLIDSeura:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lseura);
			break;
		case COLIDSeuralyh:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lseuralyh);
			break;
		case COLIDYhdistys:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.lyhdistys);
			break;
		case COLIDJoukkue:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, kilpparam.ljoukkue);
			break;
		case COLIDMaa:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, 3);
			break;
		case COLIDPiiri:
		case COLIDIlmlista:
		case COLIDArvryhma:
		case COLIDPist1:
		case COLIDPist2:
		case COLIDPist3:
		case COLIDBadge:
		case COLIDTas:
		case COLIDEra:
			OoGrid->Cells[col][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
			break;
		case COLIDRintaNo:
			if (kilpparam.bibvaihtelee) {
				OoGrid->Cells[col][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
				}
			else {
				for (ipv = 0; ipv < npv; ipv++)
					OoGrid->Cells[OoGridCols[COLIDRintaNo].Order+ipv][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
				OoGrid->Cells[0][k] = UnicodeString(_wtoi(OoGrid->Cells[col][k].c_str()));
				}
			break;
		case COLIDLaina:
			if (towupper(OoGrid->Cells[col][k].c_str()[0]) == L'L')
				OoGrid->Cells[col][k] = UnicodeString(L"Laina");
			else
				OoGrid->Cells[col][k] = UnicodeString(L"");
			break;
		case COLIDLasna:
			switch (towupper(OoGrid->Cells[col][k].c_str()[0])) {
				case L'E' :
					if (towupper(OoGrid->Cells[col][k].c_str()[1]) == L'S')
						kh = L'I';
					else if (OoGrid->Cells[col][k].Length() > 2 &&
						towupper(OoGrid->Cells[col][k].c_str()[2]) == L'A')
						kh = L'X';
					else
						kh = L'E';
					break;
				case L'V' :
					if (towupper(OoGrid->Cells[col][k].c_str()[1]) == L'I')
						kh = L'M';
					else
						kh = L'V';
					break;
				case L'L' :
					kh = L'-';
					break;
				default :
					kh = towupper(OoGrid->Cells[col][k].c_str()[0]);
					break;
				}
			switch (kh) {
				case L'D' :
					wcscpy(line, L"Delete");
					break;
				case L'K' :
					wcscpy(line, L"Kesk");
					break;
				case L'O' :
					wcscpy(line, L"Ohit");
					break;
				case L'X' :
					wcscpy(line, L"EiAikaa");
					break;
				case L'H' :
					wcscpy(line, L"Hyl");
					break;
				case L'M' :
					wcscpy(line, L"Virhe");
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
				case L'V' :
					wcscpy(line, L"Vakantti");
					break;
				case L'P' :
					wcscpy(line, L"Poissa");
					break;
				default :
					wcscpy(line, L"Läsnä");
					break;
				}
			OoGrid->Cells[col][k] = UnicodeString(line);
			break;
		case COLIDLahto:
			wcsncpy(line,OoGrid->Cells[col][k].c_str(), 10);
			line[10] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else {
				int np = 0;
				for (wchar_t *p = line; *p; p++) {
					if (*p == L'.' || *p == L':' || *p == L'/') {
						*p = L':';
						np++;
						}
					if (*p == L',') {
						np++;
						}
					}
				if (np == 1)
					wcscat(line, L":00");
				tl = wstrtoaika_vap(line, t0);
				}
			OoGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, pvparam[epv+i_pv].laika);
			break;
		case COLIDTulos:
			tl = wstrtoaika_vap(OoGrid->Cells[col][k].c_str(), 0);
			OoGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, 0)).SubString(1, pvparam[epv+i_pv].laika);
			break;
		case COLIDAsakot:
			if (srj < 0)
			   break;
			Kilp.tall_asakot(OoGrid->Cells[col][k].c_str(), epv+i_pv);
			OoGrid->Cells[col][k] = Kilp.asakkoStr(epv+i_pv);
//			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, Sarjat[srj].lsak[epv+i_pv]);
			break;
		case COLIDRata:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.pv[0].rata)/2-1).UpperCase();
			break;
		case COLIDTeksti:
			OoGrid->Cells[col][k] = OoGrid->Cells[col][k].SubString(1, sizeof(Kilp.pv[0].txt)/2-1);
			break;
		}
	OoGrid->Refresh();
	return(err);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::OoGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (OoGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Button1Click(TObject *Sender)
{
	OoGrid->Options << goEditing;
	Suodata1->Enabled = false;
	CBJarjestys->Enabled = false;
	LblMuokkaus->Visible = true;
	if (ToimintaTila == 1)
		Lueemitkoodeja1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Button3Click(TObject *Sender)
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

void __fastcall TFormOsanottajat::OoGridExit(TObject *Sender)
{
	if (OoGrid->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Button2Click(TObject *Sender)
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
	if (aktrow >= OoGrid->RowCount)
		aktrow = 1;
	OoGrid->Row = aktrow;
	OoGrid->Col = aktcol;
	Suodata1->Enabled = true;
	CBJarjestys->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::OoGridDblClick(TObject *Sender)
{
	if (OoGrid->Row == 0)
		return;
	aktrow = OoGrid->Row;
	if (!FormKilpailijatiedot)
		FormKilpailijatiedot = new TFormKilpailijatiedot(this);

	FormKilpailijatiedot->sallimuokkausvalinta = true;
	FormKilpailijatiedot->naytaKilpailija(DKilp[aktrow]);
	FormKilpailijatiedot->Show();
	if (FormKilpailijatiedot->WindowState == wsMinimized)
		FormKilpailijatiedot->WindowState = wsNormal;
	FormKilpailijatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::CBPaivatChange(TObject *Sender)
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
//	Perusnumero2->Enabled = CBPaivat->ItemIndex == 0;
	Perusnumero2->Checked = CBPaivat->ItemIndex == 0 || !kilpparam.bibvaihtelee;
	Rintanumero1->Enabled = CBPaivat->ItemIndex == 0 && kilpparam.bibvaihtelee;
	Rintanumero1->Checked = kilpparam.bibvaihtelee;
	Sarja1->Enabled = CBPaivat->ItemIndex == 0;
	Sarja1->Checked = CBPaivat->ItemIndex == 0 || !kilpparam.sarjavaihtelee;
	Vaiheensarja1->Enabled = CBPaivat->ItemIndex == 0 && kilpparam.sarjavaihtelee;
	Vaiheensarja1->Checked = kilpparam.sarjavaihtelee;
	Oo_Jrjpv = CBPaivat->ItemIndex > 0 ? CBPaivat->ItemIndex - 1 : 0;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::FormClose(TObject *Sender, TCloseAction &Action)

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

void __fastcall TFormOsanottajat::CBJarjestysChange(TObject *Sender)
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


void __fastcall TFormOsanottajat::OoGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	wchar_t msg[100], buf[50];
	kilptietue kilp;
	int rr, cc;

	rr = OoGrid->Row;
	cc = OoGrid->Col;
	aktrow = rr;
	aktcol = cc;
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
		kilp.GETREC(DKilp[aktrow]);
		swprintf(msg, L"Vahvista kilpailijan %s pysyvä poisto", kilp.nimi(buf, 49, false));
		if (Application->MessageBox(msg, L"Poisto", MB_YESNO) == IDYES) {
			EnterCriticalSection(&tall_CriticalSection);
			deltall(DKilp[aktrow], 0);
			LeaveCriticalSection(&tall_CriticalSection);
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

void __fastcall TFormOsanottajat::OoGridKeyPress(TObject *Sender, System::WideChar &Key)

{
/*	if (Key == L'\t') {

		OoGrid->Col++;
		aktcol = OoGrid->Col;
		Key = 0;
	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::BtnPaivitaClick(TObject *Sender)
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

void __fastcall TFormOsanottajat::OoGridFixedCellClick(TObject *Sender, int ACol,
		  int ARow)
{
	aktrow = ARow;
	if (ARow == 0) {
		if (kilpparam.n_pv_akt > 1 && CBPaivat->ItemIndex == 0) {
			int i_pv = 0;
			for (int i = ACol-1; i > 0 && ColIx[i] == ColIx[ACol]; i--)
				i_pv++;
			Oo_Jrjpv = i_pv;
			}
		else
			Oo_Jrjpv = Oo_pv;
		switch (ColIx[ACol]) {
			case COLIDKilpNo:
				Oo_Jrjpv = -1;
			case COLIDRintaNo:
				CBJarjestys->ItemIndex = 0;
				break;
			case COLIDEtunimi:
			case COLIDSukunimi:
				CBJarjestys->ItemIndex = 1;
				break;
			case COLIDVaihesarja:
			case COLIDSarja:
				CBJarjestys->ItemIndex = 2;
				break;
			case COLIDLahto:
				CBJarjestys->ItemIndex = 4;
				break;
			case COLIDTulos:
				CBJarjestys->ItemIndex = 5;
				break;
			case COLIDSeura:
			case COLIDSeuralyh:
				CBJarjestys->ItemIndex = 6;
				break;
			case COLIDPist1:
				CBJarjestys->ItemIndex = 8;
				break;
			case COLIDPist2:
				CBJarjestys->ItemIndex = 9;
				break;
			case COLIDPist3:
				CBJarjestys->ItemIndex = 10;
				break;
			case COLIDBadge:
				CBJarjestys->ItemIndex = 11;
				break;
			case COLIDIdno:
				CBJarjestys->ItemIndex = 12;
				break;
			case COLIDKvId:
				CBJarjestys->ItemIndex = 13;
				break;
			case COLIDLasna:
				CBJarjestys->ItemIndex = 14;
				break;
			case COLIDMaa:
				CBJarjestys->ItemIndex = 15;
				break;
			default:
				return;
			}
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Osanottajataulukko");
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Perusnumero2Click(TObject *Sender)
{
	Perusnumero2->Checked = !Perusnumero2->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Rintanumero1Click(TObject *Sender)
{
	Rintanumero1->Checked = !Rintanumero1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Vaiheensarja1Click(TObject *Sender)
{
	Vaiheensarja1->Checked = !Vaiheensarja1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Sarja1Click(TObject *Sender)
{
	Sarja1->Checked = !Sarja1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Lisenssinumero1Click(TObject *Sender)
{
	Lisenssinumero1->Checked = !Lisenssinumero1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Kvidnumero1Click(TObject *Sender)
{
	Kvidnumero1->Checked = !Kvidnumero1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Nimi1Click(TObject *Sender)
{
	Nimi1->Checked = !Nimi1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Arvo1Click(TObject *Sender)
{
	Arvo1->Checked = !Arvo1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Seura1Click(TObject *Sender)
{
	Seura1->Checked = !Seura1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Seuralyhenne1Click(TObject *Sender)
{
	Seuralyhenne1->Checked = !Seuralyhenne1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Yhdistys1Click(TObject *Sender)
{
	Yhdistys1->Checked = !Yhdistys1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Joukkue1Click(TObject *Sender)
{
	Joukkue1->Checked = !Joukkue1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Maa1Click(TObject *Sender)
{
	Maa1->Checked = !Maa1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Aluepiiri1Click(TObject *Sender)
{
	Aluepiiri1->Checked = !Aluepiiri1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Emitkoodi1Click(TObject *Sender)
{
	Emitkoodi1->Checked = !Emitkoodi1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Lsn1Click(TObject *Sender)
{
	Lsn1->Checked = !Lsn1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Lhtaika1Click(TObject *Sender)
{
	Lhtaika1->Checked = !Lhtaika1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Tulos1Click(TObject *Sender)
{
	Tulos1->Checked = !Tulos1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Sakot1Click(TObject *Sender)
{
	Sakot1->Checked = !Sakot1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::initOoGrid(void)
{
//	OoGridCols[5].Visible = kilpparam.larvo != 0;
//	OoGridCols[7].Visible = kilpparam.lseuralyh != 0;
//	OoGridCols[8].Visible = kilpparam.lyhdistys != 0;
//	OoGridCols[9].Visible = kilpparam.ljoukkue != 0;
	Perusnumero2->Checked = OoGridCols[COLIDKilpNo].Visible;
	Sarja1->Checked = OoGridCols[COLIDSarja].Visible;
	Sukupx1->Checked = OoGridCols[COLIDSukup].Visible;
	Iksarjag1->Checked = OoGridCols[COLIDIkaSrj].Visible;
	Lisenssinumero1->Checked = OoGridCols[COLIDIdno].Visible;
	Kvidnumero1->Checked = OoGridCols[COLIDKvId].Visible;
	Nimi1->Checked = OoGridCols[COLIDSukunimi].Visible;
	Arvo1->Checked = OoGridCols[COLIDArvo].Visible;
	Seura1->Checked = OoGridCols[COLIDSeura].Visible;
	Seuralyhenne1->Checked = OoGridCols[COLIDSeuralyh].Visible;
	Yhdistys1->Checked = OoGridCols[COLIDYhdistys].Visible;
	Joukkue1->Checked = OoGridCols[COLIDJoukkue].Visible;
	Maa1->Checked = OoGridCols[COLIDMaa].Visible;
	Aluepiiri1->Checked = OoGridCols[COLIDPiiri].Visible;
	Ilmoittautumislista1->Checked = OoGridCols[COLIDIlmlista].Visible;
	Pisteet1->Checked = OoGridCols[COLIDPist1].Visible;
	Pisteet2->Checked = OoGridCols[COLIDPist2].Visible;
	Pisteet3->Checked = OoGridCols[COLIDPist3].Visible;
	Rintanumero1->Checked = OoGridCols[COLIDRintaNo].Visible;
	Vaiheensarja1->Checked = OoGridCols[COLIDVaihesarja].Visible;
	Emitkoodi1->Checked = OoGridCols[COLIDBadge].Visible;
	Lsn1->Checked = OoGridCols[COLIDLasna].Visible;
	Lhtaika1->Checked = OoGridCols[COLIDLahto].Visible;
	Tulos1->Checked = OoGridCols[COLIDTulos].Visible;
	if (kilpparam.lsakmax > 0)
		Sakot1->Checked = OoGridCols[COLIDAsakot].Visible;
	else
		Sakot1->Checked = OoGridCols[COLIDTas].Visible;
	Rata1->Checked = OoGridCols[COLIDRata].Visible;
	Er1->Checked = OoGridCols[COLIDEra].Visible;
	Qual1->Checked = OoGridCols[COLIDQual].Visible;
	GSM1->Checked = OoGridCols[COLIDGSM].Visible;
	Vaihepist11->Checked = OoGridCols[COLIDVPist1].Visible;
	Vaihepist21->Checked = OoGridCols[COLIDVPist2].Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::MuokkaussallittuF91Click(TObject *Sender)
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

void __fastcall TFormOsanottajat::Suodata1Click(TObject *Sender)
{
	if (!FormSuodatus)
		FormSuodatus = new TFormSuodatus(this);
	FormSuodatus->Show();
	if (FormSuodatus->WindowState == wsMinimized)
		FormSuodatus->WindowState = wsNormal;
	FormSuodatus->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::OoGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (OoGrid->RowCount - OoGrid->FixedRows >= OoGrid->VisibleRowCount + OoGrid->TopRow)
		OoGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::OoGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (OoGrid->TopRow > OoGrid->FixedRows)
		OoGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Sukupx1Click(TObject *Sender)
{
	Sukupx1->Checked = !Sukupx1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Iksarjag1Click(TObject *Sender)
{
	Iksarjag1->Checked = !Iksarjag1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Rata1Click(TObject *Sender)
{
	Rata1->Checked = !Rata1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

int start_regnly(int r_no);

void __fastcall TFormOsanottajat::Lueemitkoodeja1Click(TObject *Sender)
{
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
			 kaikki_ajat[r_no+1] = 2;
			 timerfl = 1;
			 start_regnly(r_no);
			 Oo_r_no = r_no;
			 hae_puutt_badge();
			 break;
			 }
		  }
		}
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormOsanottajat::ECodeHandler(TMyMessage &msg)
{
	if (sallimuokkaus && OoGrid->Row > 0 && Emitkoodi1->Checked &&
		Lueemitkoodeja1->Enabled) {
		for (int ipv = 0; ipv < npv; ipv++) {
			OoGrid->Cells[OoGridCols[COLIDBadge].Order+ipv][OoGrid->Row] = msg.wparam;
			if (merklaina)
				OoGrid->Cells[OoGridCols[COLIDLaina].Order+ipv][OoGrid->Row] = L'L';
			}
		hae_puutt_badge();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormOsanottajat::hae_puutt_badge(void)
{
	for (int r = OoGrid->Row; r < OoGrid->RowCount; r++) {
		if (_wtoi(OoGrid->Cells[OoGridCols[COLIDBadge].Order][r].c_str()) == 0) {
			OoGrid->Row = r;
			break;
			}
		}
}

//---------------------------------------------------------------------------

void tall_badge(UINT32 badge)
{
	SendMessage(FormOsanottajat->Handle,WM_MYECODE,badge,0);
}

void __fastcall TFormOsanottajat::Qual1Click(TObject *Sender)
{
	Qual1->Checked = !Qual1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::GSM1Click(TObject *Sender)
{
	GSM1->Checked = !GSM1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Kiinteitsarakkeita11Click(TObject *Sender)
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

void __fastcall TFormOsanottajat::Er1Click(TObject *Sender)
{
	Er1->Checked = !Er1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Pisteet1Click(TObject *Sender)
{
	Pisteet1->Checked = !Pisteet1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Pisteet2Click(TObject *Sender)
{
	Pisteet2->Checked = !Pisteet2->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Pisteet3Click(TObject *Sender)
{
	Pisteet3->Checked = !Pisteet3->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Ilmoittautumislista1Click(TObject *Sender)
{
	Ilmoittautumislista1->Checked = !Ilmoittautumislista1->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Vaihepist11Click(TObject *Sender)
{
	Vaihepist11->Checked = !Vaihepist11->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsanottajat::Vaihepist21Click(TObject *Sender)
{
	Vaihepist21->Checked = !Vaihepist21->Checked;
	naytaTiedot();
}
//---------------------------------------------------------------------------

