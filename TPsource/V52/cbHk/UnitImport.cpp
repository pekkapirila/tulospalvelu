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

#include "UnitImport.h"
#include <bstrings.h>
#include "WinHk.h"
#include "UnitMySQL.h"
#include "UnitTietoTulk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormImport *FormImport;

static int n_pv_v, n_va_v, k_arvo = 1, k_yhd = 1, valuku_v;
static wchar_t erotin = L';', st1[80], st2[80];
static bool fis = false, luearvo = false;
static   rivitulkintatp *RiviTulk;
static   int nTulkRivi;
static   bool TulkintaOn;
static   int Jatko;
static	 int nTiedRivi;
static  int laikaerot;
extern int lseuramax;
extern int valuku_vanha;
int luevanha(wchar_t *dataf2nm);
int NaytaTulkinta(rivitulkintatp *RiviTulk, int nRivi, int rNo, const wchar_t *sanoma);
int lueIOF30EventXml(wchar_t *filename, bool lueSarjat, bool lueRadat, bool lueOsanottajat);

typedef struct {
   int kilprecsize;
   char kilplaji;
   char alalaji[3];
   int n_pv;
   int valuku;
   int n_pv_va;
   int n_tlahto1;
   int n_xtext;
   int n_xint;
   int lnimi;
   int lseura;
   int arvo_on;
	int yhdistys_on;
	int lsakmax;
   }  kilpparamtp_v;

extern kilpparamtp_v kilpparam_v;

static FLD irmaflds[] = {
	{L"Sarja", 1, 0, 1},
	{L"LisNo", 1, 0, 2},
	{L"Nimi", 1, 0, 3},
	{L"Seura", 1, 0, 4},
	{L"Emit", 1, 0, 5},
	{L"Synt", 1, 0, 6},
	{L"Maa", 1, 0, 7},
	{L"Vaiheet", 1, 0, 8},
	{L"Maksu", 1, 0, 9},
	{L"Ranki", 1, 0, 10},
	{L"KilpNo", 0, 0, 11},
	{L"Lähtö", 0, 0, 12}
};

static FLD kilmoflds[] = {
	{L"SporttiId", 1, 0, 1},
	{L"IdLaji", 1, 0, 2},
	{L"Etunimi", 1, 0, 3},
	{L"Sukunimi", 1, 0, 4},
	{L"Sarja", 1, 0, 5},
	{L"Synt", 1, 0, 6},
	{L"Seura", 1, 0, 7},
	{L"FIS-Id", 1, 0, 8}
};

static struct {
	int fld;
	int ipv;
	int va;
	} fldorder[200];
static int nflds;

//---------------------------------------------------------------------------
void kilptietue::TallEhdoin(int pos)
{
	int d;

	if (sailTiedPos) {
		kilptietue kilp1;
		kilp1.nollaa();
		kilp1.kilpstatus = 0xffff;
		while (nrec <= pos)
			kilp1.addtall(&d, 0);
		tallenna(pos, 0, 0, 0, 0);
		}
	else
		addtall(&d, 0);
}
//---------------------------------------------------------------------------
void kilptietue::SiirraPvEhdoin(kilppvtp *Pv, int ipv)
{
	if (ipv >= 0 && ipv < kilpparam.n_pv_akt)
		pv[ipv] = *Pv;
}
//---------------------------------------------------------------------------
__fastcall TFormImport::TFormImport(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
int TFormImport::lue_kilpcsv2(TextFl *afile, kilptietue *kilp, int lineno, int toiminto, int *dKilp, wchar_t erotin)
{
	wchar_t rivi[10000], *p, *p1, *pp;
	INT16 i,res = 0;
	INT16 srj;

	wchar_t *fields[400], st1[100] = L"", st2[100] = L"", erottimet[] = L",";
	int nfld = 0, k = 0, ret = 0;
	rivitulkintatp tlk[400];
	char ast[100];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
	memset(tlk, 0, sizeof(tlk));

	if (afile->ReadLine(rivi, sizeof(rivi)/2-1) == NULL)
		return(-1);
	if (wcslen(rivi) < 4)
		return(-1);
	kilp->nollaa();
	nfld = getfields(rivi, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"" , true, false);
	if (lineno < 0) {
		memset(fldorder, 0, sizeof(fldorder));
		nflds = 0;
		for (UINT ifld = 0; ifld < nfld; ifld++) {
			int ipv = 0;
			int va = 0;
			for (int i = 0; i < Kexp_kentat+Pexp_kentat+Vexp_kentat+Xexp_kentat; i++) {
				if (ipv == 0 && wcscmp(exp_kentat_local[i].name, fields[ifld]) == 0) {
					fldorder[ifld].fld = exp_kentat_local[i].id;
					if (nflds <= ifld)
						nflds = ifld+1;
					break;
					}
				if (i >= Kexp_kentat) {
					if (i == Kexp_kentat) {
						p1 = NULL;
						pp = NULL;
						p = wcstok(fields[ifld], L"-");
						if (p)
							p1 = wcstok(NULL, L"-");
						if (!p1)
							continue;
						if ((ipv = _wtoi(p1)) < 1 || ipv > kilpparam.n_pv_akt)
							continue;
						pp = wcstok(NULL, L"-");
						if (pp)
							va = _wtoi(pp);
						}
					if (ipv < 1 || ipv > kilpparam.n_pv_akt)
						continue;
					if (va == 0 && i < Kexp_kentat+Pexp_kentat &&
						wcscmp(exp_kentat_local[i].name, p) == 0) {
						fldorder[ifld].fld = exp_kentat_local[i].id;
						fldorder[ifld].ipv = ipv;
						if (nflds <= ifld)
							nflds = ifld+1;
						break;
						}
					if (i < Kexp_kentat+Pexp_kentat || va < 1 || va > kilpparam.valuku)
						continue;
					if (wcscmp(exp_kentat_local[i].name, p) == 0) {
						fldorder[ifld].fld = exp_kentat_local[i].id;
						fldorder[ifld].ipv = ipv;
						fldorder[ifld].va = va;
						if (nflds <= ifld)
							nflds = ifld+1;
						break;
						}
					}
				}
			}
		deletefields(fields, sizeof(fields)/sizeof(fields[0]));
		return(1);
		}
	k = 0;
	for (UINT ifld = 0; ifld < nfld; ifld++) {
		if (fldorder[ifld].fld == 0)
			continue;
		tlk[k].no = k + 1;
		wcsncpy(tlk[k].teksti, fields[k], sizeof(tlk[k].teksti)/2-1);
		k++;
		}
	for (UINT ifld = 0; res == 0 && ifld < nflds; ifld++) {
		wchar_t tfld[200];
		if (fldorder[ifld].fld == 0)
			continue;
		if (ifld < nfld) {
			wcsncpy(tfld, fields[ifld], 199);
			elimwbl(tfld);
			}
		else
			tfld[0] = 0;
		switch (fldorder[ifld].fld) {
			case FLDID_KILPNO:
				if (toiminto) {
					*dKilp= getpos(_wtoi(tfld));
					if (*dKilp <= 0) {
						res = 1;
						break;
						}
					kilp->GETREC(*dKilp);
					}
				kilp->kilpno = _wtoi(tfld);
				if (kilp->kilpno > maxkilpno)
					kilp->kilpno = 0;
				if (toiminto == 0 || kilpparam.sarjavaihtelee == false) {
					for (int ip = 0; ip < kilpparam.n_pv_akt; ip++)
						kilp->pv[ip].bib = kilp->kilpno;
					}
				break;
			case FLDID_KILPID:
				kilp->lisno[0] = _wtoi(tfld);
				break;
			case FLDID_INTID:
				kilp->lisno[1] = _wtoi(tfld);
				break;
			case FLDID_PIIRI:
				kilp->piiri = _wtoi(tfld);
				break;
			case FLDID_SUKUNIMI:
				wcsncpy(kilp->sukunimi, tfld, kilpparam.lsnimi);
				break;
			case FLDID_ETUNIMI:
				wcsncpy(kilp->etunimi, tfld, kilpparam.lenimi);
				break;
			case FLDID_ARVO:
				wcsncpy(kilp->arvo, tfld, kilpparam.larvo);
				break;
			case FLDID_SEURA:
				wcsncpy(kilp->seura, tfld, kilpparam.lseura);
				break;
			case FLDID_SEURALYH:
				wcsncpy(kilp->seuralyh, tfld, kilpparam.lseuralyh);
				break;
			case FLDID_YHD:
				wcsncpy(kilp->yhdistys, tfld, kilpparam.lyhdistys);
				break;
			case FLDID_JK:
				wcsncpy(kilp->joukkue, tfld, kilpparam.ljoukkue);
				break;
			case FLDID_MAA:
				wcsncpy(kilp->maa, tfld, sizeof(kilp->maa)/2-1);
				break;
			case FLDID_SARJA:
				srj = haesarja_w(tfld, false);
				if (srj >= 0) {
					if (toiminto == 0)
						kilp->setSarjaAll(srj);
					else
						kilp->setSarja(srj, -1);
					}
				else
					return(1);
				break;
			case FLDID_SUKUP:
				kilp->sukup = tfld[0];
				break;
			case FLDID_SYNT:
				kilp->synt = _wtoi(tfld);
				break;
			case FLDID_IKASRJ:
				kilp->ikasarja = _wtoi(tfld);
				break;
			case FLDID_ARVRYH:
				kilp->arvryhma = _wtoi(tfld);
				break;
			case FLDID_ILMLISTA:
				kilp->ilmlista = _wtoi(tfld);
				break;
			case FLDID_PISTE1:
				kilp->pisteet[0] = _wtoi(tfld);
				break;
			case FLDID_PISTE2:
				kilp->pisteet[1] = _wtoi(tfld);
				break;
			case FLDID_PISTE3:
				kilp->pisteet[2] = _wtoi(tfld);
				break;
			case FLDID_BIB:
				kilp->pv[fldorder[ifld].ipv-1].bib = _wtoi(tfld);
				if (kilp->pv[fldorder[ifld].ipv-1].bib > maxkilpno)
					kilp->pv[fldorder[ifld].ipv-1].bib = 0;
				break;
			case FLDID_VSARJA:
				if (kilpparam.sarjavaihtelee) {
					srj = haesarja_w(tfld, false);
					if (srj >= 0)
						kilp->setSarja(srj, fldorder[ifld].ipv-1);
					}
				break;
			case FLDID_BADGE:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].badge[0] = _wtoi(tfld);
				break;
			case FLDID_LAINA:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].laina[0] = tfld[0];
				break;
			case FLDID_BADGE2:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].badge[1] = _wtoi(tfld);
				break;
			case FLDID_LAINA2:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].laina[1] = tfld[0];
				break;
			case FLDID_LAHTOARV:
				if (fldorder[ifld].ipv > 0) {
					kilp->pv[fldorder[ifld].ipv-1].tlahto = wstrtoaika_vap(tfld, t0);
					kilp->pv[fldorder[ifld].ipv-1].va[0].vatulos = kilp->pv[fldorder[ifld].ipv-1].tlahto;
					}
				break;
			case FLDID_LAHTOTOT:
				if (!CBLAjat->Checked && fldorder[ifld].ipv > 0) {
					kilp->pv[fldorder[ifld].ipv-1].va[0].vatulos = wstrtoaika_vap(tfld, t0);
					laikaerot += abs(kilp->pv[fldorder[ifld].ipv-1].va[0].vatulos -
						kilp->pv[fldorder[ifld].ipv-1].tlahto);
					}
				break;
			case FLDID_TULOS:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].va[1].vatulos = wstrtoaika_vap(tfld, 0);
				break;
			case FLDID_TARK:
				if (fldorder[ifld].ipv > 0)
					kilp->set_tark(tfld[0], fldorder[ifld].ipv-1);
				break;
			case FLDID_SIJA:
				break;
			case FLDID_AMPSAKKO:
				if (fldorder[ifld].ipv > 0 && kilpparam.lsakmax > 0)
					kilp->set_Asakot(tfld, fldorder[ifld].ipv-1);
				break;
			case FLDID_SAKKO:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].tasoitus = _wtoi(tfld);
				break;
			case FLDID_RATA:
				if (fldorder[ifld].ipv > 0)
					wcsncpy(kilp->pv[fldorder[ifld].ipv-1].rata, tfld, sizeof(kilp->pv[0].rata)/2-1);
				break;
			case FLDID_ERA:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].era = _wtoi(tfld);
				break;
			case FLDID_SELITYS:
				if (fldorder[ifld].ipv > 0)
					wcsncpy(kilp->pv[fldorder[ifld].ipv-1].selitys, tfld, sizeof(kilp->pv[0].selitys)/2-1);
				break;
			case FLDID_PVPISTE1:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].pvpisteet[0] = _wtoi(tfld);
				break;
			case FLDID_PVPISTE2:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].pvpisteet[1] = _wtoi(tfld);
				break;
			case FLDID_ENNATYS:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].enn = wstrtoaika_vap(tfld, 0);
				break;
			case FLDID_TAVOITE:
				if (fldorder[ifld].ipv > 0)
					kilp->pv[fldorder[ifld].ipv-1].tav = wstrtoaika_vap(tfld, 0);
				break;
			case FLDID_FLAGS:
				if (fldorder[ifld].ipv > 0)
					swscanf(tfld, L"%x/%x", &kilp->pv[fldorder[ifld].ipv-1].flags[0],
						&kilp->pv[fldorder[ifld].ipv-1].flags[1]);
				break;
			case FLDID_VATULOS:
				if (fldorder[ifld].ipv > 0 && fldorder[ifld].va > 0)
					kilp->pv[fldorder[ifld].ipv-1].va[fldorder[ifld].va+1].vatulos = wstrtoaika_vap(tfld, 0);
				break;
			case FLDID_VASIJA:
				break;
			case FLDID_NIMI:
				wesnimet(tfld, st1, st2);
				wcsncpy(kilp->sukunimi, st2, kilpparam.lsnimi);
				wcsncpy(kilp->etunimi, st1, kilpparam.lenimi);
				break;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[kilp->Sarja(ipv)].lsak[ipv]; i++) {
			if (kilp->pv[ipv].asakot[i] == 0)
				kilp->pv[ipv].asakot[i] = L'-';
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(res);
}
//---------------------------------------------------------------------------

int TFormImport::lue_kilpcsv(TextFl *afile, kilptietue *kilp, wchar_t erotin)
{
	wchar_t rivi[10000],s[40], *p, *p1, *pp;
	INT16 i,il,res = 0;
	INT16 srj, l, kbg;

	if (afile->ReadLine(rivi, 1999) == NULL)
		return(ESC);
	if (wcslen(rivi) < 10)
		return(1);
	kilp->nollaa();
	pp = rivi;
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Kexp_kentat+Pexp_kentat; i++) {
			if (!exp_kentat_local[i].len)
				continue;
			if (ipv > 0 && i < Kexp_kentat)
				continue;
			p = wcstokch(pp, erotin);
			if (!p)
				break;
			pp = 0;
			elimwbl2(p);
			switch (exp_kentat_local[i].oik) {
				case 1:
					if (*p < L'0' || *p > L'9')
						return(1);
					kilp->kilpno = _wtoi(p);
					if (kilp->kilpno > maxkilpno)
						kilp->kilpno = 0;
					for (int ip = 0; ip < kilpparam.n_pv_akt; ip++)
						kilp->pv[ip].bib = kilp->kilpno;
					break;
				case 2:
					kilp->lisno[0] = _wtoi(p);
					break;
				case 3:
					kilp->lisno[1] = _wtoi(p);
					break;
				case 4:
					kilp->piiri = _wtoi(p);
					break;
				case 5:
					wcsncpy(kilp->sukunimi, p, kilpparam.lsnimi);
					break;
				case 6:
					wcsncpy(kilp->etunimi, p, kilpparam.lenimi);
					break;
				case 7:
					wcsncpy(kilp->arvo, p, kilpparam.larvo);
					break;
				case 8:
					wcsncpy(kilp->seura, p, kilpparam.lseura);
					break;
				case 9:
					wcsncpy(kilp->seuralyh, p, kilpparam.lseuralyh);
					break;
				case 10:
					wcsncpy(kilp->yhdistys, p, kilpparam.lyhdistys);
					break;
				case 11:
					wcsncpy(kilp->joukkue, p, kilpparam.ljoukkue);
					break;
				case 12:
					wcsncpy(kilp->maa, p, 3);
					break;
				case 13:
					srj = haesarja_w(p, false);
					if (srj >= 0)
						kilp->setSarjaAll(srj);
					else
						return(1);
					break;
				case 14:
					kilp->sukup = p[0];
					break;
				case 15:
					kilp->synt = _wtoi(p);
					break;
				case 16:
					kilp->ikasarja = _wtoi(p);
					break;
				case 17:
					kilp->arvryhma = _wtoi(p);
					break;
				case 18:
					kilp->ilmlista = _wtoi(p);
					break;
				case 19:
					kilp->pisteet[0] = _wtoi(p);
					break;
				case 20:
					kilp->pisteet[1] = _wtoi(p);
					break;
				case 21:
					kilp->pisteet[2] = _wtoi(p);
					break;
				case 22:
					kilp->pv[ipv].bib = _wtoi(p);
					if (kilp->pv[ipv].bib > maxkilpno)
						kilp->pv[ipv].bib = 0;
					break;
				case 23:
					if (kilpparam.sarjavaihtelee) {
						srj = haesarja_w(p, false);
						if (srj >= 0)
							kilp->setSarja(srj, ipv);
						}
					break;
				case 24:
					kilp->pv[ipv].badge[0] = _wtoi(p);
					break;
				case 25:
					kilp->pv[ipv].laina[0] = *p;
					break;
				case 26:
					kilp->pv[ipv].badge[1] = _wtoi(p);
					break;
				case 27:
					kilp->pv[ipv].laina[1] = *p;
					break;
				case 28:
					kilp->pv[ipv].tlahto = wstrtoaika_vap(p, t0);
					kilp->pv[ipv].va[0].vatulos = kilp->pv[ipv].tlahto;
					break;
				case 29:
					kilp->pv[ipv].va[0].vatulos = wstrtoaika_vap(p, t0);
					break;
				case 30:
					kilp->pv[ipv].va[1].vatulos = wstrtoaika_vap(p, 0);
					break;
				case 31:
					kilp->set_tark(*p, ipv);
					break;
				case 32:
					break;
				case 33:
					if (kilpparam.lsakmax > 0)
						kilp->set_Asakot(p, ipv);
					break;
				case 34:
					kilp->pv[ipv].tasoitus = _wtoi(p);
					break;
				case 35:
					wcsncpy(kilp->pv[ipv].rata, p, sizeof(kilp->pv[ipv].rata)/2-1);
					break;
				case 36:
					wcsncpy(kilp->pv[ipv].selitys, p, sizeof(kilp->pv[ipv].selitys)/2-1);
					break;
				case 37:
					kilp->pv[ipv].pvpisteet[0] = _wtoi(p);
					break;
				case 38:
					kilp->pv[ipv].pvpisteet[1] = _wtoi(p);
					break;
				}
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[kilp->Sarja(ipv)].lsak[ipv]; i++) {
			if (kilp->pv[ipv].asakot[i] == 0)
				kilp->pv[ipv].asakot[i] = L'-';
			}
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			for (int i = Kexp_kentat+Pexp_kentat; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
				p = wcstokch(NULL, erotin);
				if (!p)
					break;
				if (!exp_kentat_local[i].len)
					continue;
				switch (exp_kentat_local[i].oik) {
					case 39:
						kilp->pv[ipv].va[iva+2].vatulos = wstrtoaika_vap(p, 0);
						break;
					case 40:
						break;
					}
				}
			}
		}
	return(res);
}

//---------------------------------------------------------------------------

static INT16 lue_ascii_e(TextFl *afile, kilptietue *kilp, wchar_t erotin)
	{
	wchar_t rivi[400],s[40], *p, *p1, *pp;
	INT16 i,il,res = 0;
	INT16 srj, l, kbg;

	if (afile->ReadLine(rivi, 399) == NULL)
		res = 2;
	else {
		kilp->nollaa();
		p = rivi;
		srj = -1;
		il = 0;
		for (i = 0; il < 23+3*n_pv_v; i++) {
			il = i;
			if (il >= 3)
				il += k_arvo;
			if (il >= 5)
				il += k_yhd;
			if (kilpparam_v.kilplaji == L'S' || fis) {
				if (il >= 7+n_pv_v)
					il += 5-n_pv_v;
				}
			else {
				if (il >= 7)
					il += 6;
				}
			if (kilpparam_v.kilplaji == L'H' && fis) {
				if (il >= 12)
					il += 1;
				}
			if (!fis && il >= 17)
				il += 3;
			p1 = p;
			while (*p1 && *p1 != erotin && *p1 != L'\n')
				p1++;
			*p1 = 0;
			if (*p) {
				switch (il) {
					case 0 :
						srj = haesarja_w(p, false);
						if (srj >= 0)
							kilp->setSarjaAll(srj);
						break;
					case 1 :
						kilp->lisno[0] = _wtol(p);
							break;
					case 2 :
						wesnimet(p, st1, st2);
						wcsncpy(kilp->sukunimi, st2, kilpparam.lsnimi);
						wcsncpy(kilp->etunimi, st1, kilpparam.lenimi);
						break;
					case 3 :
						if (kilpparam.larvo)
							wcsncpy(kilp->arvo, p, kilpparam.larvo);
						break;
					case 4 :
						wcsncpy(kilp->seura, p, kilpparam.lseura);
						wcsncpy(kilp->seuralyh, p, kilpparam.lseuralyh);
						break;
					case 5 :
						wcsncpy(kilp->yhdistys, p, kilpparam.lyhdistys);
						break;
					case 6 :
						wcsncpy(kilp->maa, p, 3);
						break;
					case 7 :
					case 8 :
					case 9 :
					case 10 :
					case 11 :
						kilp->pv[il-7].badge[0] = _wtol(p);
						break;
					case 12 :
						kilp->pv[il-7].laina[0] = *p;
						break;
					case 13 :
						for (l = Sarjat[kilp->Sarja()].enspv; l <= Sarjat[kilp->Sarja()].viimpv; l++) {
							if (wcswcind(L'0'+l, p) == -1) {
								kilp->set_tark(L'P', l);
								}
							}
						break;
					case 14 :
						kilp->kilpno = _wtoi(p);
						if (kilp->kilpno > maxkilpno)
							kilp->kilpno = 0;
						for (int ip = 0; ip < kilpparam.n_pv_akt; ip++)
							kilp->pv[ip].bib = kilp->kilpno;
						break;
					case 15 :
						kilp->ilmlista = _wtoi(p);
						break;
					case 16:
						kilp->piiri = _wtoi(p);
						break;
					case 17:
						kilp->pisteet[0] = _wtoi(p);
						break;
					case 18:
						kilp->pisteet[1] = _wtoi(p);
						break;
					case 19:
						kilp->pisteet[2] = _wtoi(p);
						break;
					default :
						if (il < 20 + n_pv_v) {
							if (p[0] >= L'0' && p[0] <= L'9' && wcslen(p) <= 8) {
								wcscpy(s, L"00.00.00");
								wcscpy(s+8-wcslen(p), p);
								wcscat(s, L",00");
								for (pp = s; *pp; pp++)
									if (*pp == L':')
										*pp = L'.';
								kilp->pv[il-20].tlahto = WSTRTOAIKA(s, t0);
								kilp->pv[il-20].va[0].vatulos = kilp->pv[il-20].tlahto;
								}
							}
						else {
							if (((il + n_pv_v) % 2) == 0) {
								kilp->set_tark(p[0], (il-20-n_pv_v)/2);
								}
							else {
								kilp->pv[(il-21-n_pv_v)/2].va[1].vatulos = wstrtoaika_vap(p, 0);
								if (kilp->pv[(il-21-n_pv_v)/2].va[1].vatulos < 0)
									kilp->pv[(il-21-n_pv_v)/2].va[1].vatulos = 0;
								}
							}
						break;
					}
				}
			if (srj < 0) {
				res = 1;
				break;
				}
			p = p1+1;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[kilp->Sarja(ipv)].lsak[ipv]; i++) {
			if (kilp->pv[ipv].asakot[i] == 0)
				kilp->pv[ipv].asakot[i] = L'-';
			}
		}
	return(res);
	}
//---------------------------------------------------------------------------

int NaytaTulkinta(rivitulkintatp *RiviTulk, int nRivi, int rNo, const wchar_t *sanoma)
{
	if (!FormTulkinta) {
		FormTulkinta = new TFormTulkinta(FormImport);
		}
	FormTulkinta->Vastaus = 0;
   FormTulkinta->NaytaTulkinta(RiviTulk, nRivi, rNo, nTiedRivi, sanoma);
	FormTulkinta->ShowModal();
   Jatko = FormTulkinta->Vastaus;
   return(0);
}
//---------------------------------------------------------------------------

int tulkIrmaRivi(wchar_t *iBuf, kilptietue *kilp, int rNo, wchar_t erotin)
{
	wchar_t *p, *fields[50], st[100] = L"", st1[100] = L"", st2[100] = L"", erottimet[] = L",";
	int nfld = 0, k = 0, ret = 0, nRivi = 9;
	rivitulkintatp tlk[30];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
	memset(tlk, 0, sizeof(tlk));
	kilp->nollaa();
	nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		0 , true, false);
	for (UINT i = 0; i < nfld && i < sizeof(irmaflds)/sizeof(irmaflds[0]); i++) {
		if (irmaflds[i].pos == 0)
			continue;
		tlk[k].no = k + 1;
		wcsncpy(tlk[k].teksti, fields[k], sizeof(tlk[k].teksti)/2-1);
		k++;
		}
	k = 0;
	while (1) {
		if (nfld < ++k)
			break;
		wcsncpy(st, fields[k-1], 10);
		st[10] = 0;
		kilp->setSarjaAll(haesarja_w(st, false));
		if (kilp->sarja < 0) {
			kilp->setSarjaAll(sarjaluku-1);
			Application->MessageBoxW((UnicodeString(L"Virheellinen sarja ")+
				UnicodeString(st)).c_str(), L"Virhe", MB_OK);
			ret = ESC;
			break;
			}
		wcscpy(tlk[k-1].nimi, irmaflds[0].name);
		wcscpy(tlk[k-1].arvo, Sarjat[kilp->Sarja()].sarjanimi);
		if (nfld < ++k)
			break;
		if (irmaflds[1].pos) {
			kilp->lisno[0] = _wtoi(fields[k-1]);
			wcscpy(tlk[k-1].nimi, irmaflds[1].name);
			_itow(kilp->lisno[0], tlk[k-1].arvo, 10);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[2].pos) {
			wesnimet(fields[k-1], st1, st2);
			wcsncpy(kilp->sukunimi, st2, kilpparam.lsnimi);
			wcsncpy(kilp->etunimi, st1, kilpparam.lenimi);
			wcscpy(tlk[k-1].nimi, irmaflds[2].name);
			kilp->nimi(tlk[k-1].arvo, sizeof(tlk[k-1].arvo)/2-1, 0);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[3].pos) {
			wcsncpy(kilp->seuralyh, fields[k-1], kilpparam.lseuralyh);
			wcscpy(tlk[k-1].nimi, irmaflds[3].name);
			wcscpy(tlk[k-1].arvo, kilp->seuralyh);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[4].pos) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
				kilp->pv[ipv].badge[0] = _wtoi(fields[k-1]);
			wcscpy(tlk[k-1].nimi, irmaflds[4].name);
			_itow(kilp->pv[0].badge[0], tlk[k-1].arvo, 10);
			if (nfld < ++k)
				break;
//		kilp->synt = _wtoi(fields[k-1]);
			}
		if (irmaflds[5].pos) {
			wcscpy(tlk[k-1].nimi, irmaflds[5].name);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[6].pos) {
			wcsncpy(kilp->maa, fields[k-1], 3);
			wcscpy(tlk[k-1].nimi, irmaflds[6].name);
			wcscpy(tlk[k-1].arvo, kilp->maa);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[7].pos) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				if (!FormImport->CBKaikkVaih->Checked && wcswcind(L'1'+ipv, fields[k-1]) < 0) {
					kilp->set_tark(L'P', ipv);
					st1[ipv] = L'P';
					}
				else
					st1[ipv] = L'L';
				st1[kilpparam.n_pv_akt] = 0;
				}
			wcscpy(tlk[k-1].nimi, irmaflds[7].name);
			wcscpy(tlk[k-1].arvo, st1);
			if (nfld < ++k)
				break;
			}
		if (irmaflds[8].pos) {
			if (nfld < ++k)
				break;
			}
		if (FormImport->CBRankipist->Checked && irmaflds[9].pos) {
			for (p = fields[k-1]; *p != 0; p++)
				if (*p == L',')
					*p = L'.';
			kilp->pisteet[2] = (int) floor(1000 * _wtof(fields[k-1]) + 0.5);
			wcscpy(tlk[k-1].nimi, irmaflds[9].name);
			_itow(kilp->pisteet[2], tlk[k-1].arvo, 10);
			}
		break;
		}
	if (TulkintaOn || ret) {
		NaytaTulkinta(tlk, nRivi, rNo, ret ? L"Virheellinen tietue" : 0);
		switch (Jatko) {
			case 1:
				ret = 0;
				break;
			case 2:
				TulkintaOn = false;
				ret = 0;
				break;
			case 3:
				ret = 1;
				break;
			case 99:
				ret = ESC;
				break;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[kilp->Sarja(ipv)].lsak[ipv]; i++) {
			if (kilp->pv[ipv].asakot[i] == 0)
				kilp->pv[ipv].asakot[i] = L'-';
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(ret);
	}

//---------------------------------------------------------------------------

 int tulkKilmoRivi(wchar_t *iBuf, kilptietue *kilp, int rNo, wchar_t erotin)
{
	wchar_t *p, *fields[50], st[100] = L"", st1[20], erottimet[] = L";";
	int nfld = 0, k = 0, ret = 0, nRivi = 9, srj;
	static int srjcol=4;
	rivitulkintatp tlk[30];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
	memset(tlk, 0, sizeof(tlk));
	kilp->nollaa();
	nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"", true, false);
	for (UINT i = 0; i < nfld && i < sizeof(kilmoflds)/sizeof(kilmoflds[0]); i++) {
		tlk[i].no = i + 1;
		wcsncpy(tlk[i].teksti, fields[i], sizeof(tlk[i].teksti)/2-1);
		}
	while (1) {
		if (nfld < ++k)
			break;
		kilp->lisno[0] = _wtoi(fields[k-1]);
		wcscpy(tlk[k-1].nimi, kilmoflds[0].name);
		_itow(kilp->lisno[0], tlk[k-1].arvo, 10);
		if (nfld < ++k)
			break;
		if (FormImport->RGKilmo->ItemIndex == 0) {
			wcsncpy(st, fields[k-1], 99);
			st[99] = 0;
			if ((p = wcsstr(st+2, L" VAN ")) == NULL
				&& (p = wcsstr(st+2, L" VON ")) == NULL
				&& (p = wcsstr(st+2, L" DE ")) == NULL
				&& (p = wcsstr(st+2, L" AF ")) == NULL
				&& (p = wcsstr(st+2, L" AV ")) == NULL
				&& (p = wcsstr(st+2, L" MC ")) == NULL
				&& (p = wcsstr(st+2, L" BEN ")) == NULL)
				for (p = st + wcslen(st) - 1; p > st && *p != L' '; p--) ;
			wcsncpy(kilp->sukunimi, p+1, kilpparam.lsnimi);
			locasewname(kilp->sukunimi);
			*p = 0;
			wcsncpy(kilp->etunimi, st, kilpparam.lenimi);
			locasewname(kilp->etunimi);
			if (nfld < ++k)
				break;
			wcsncpy(st, fields[k-1], 10);
			st[10] = 0;
			kilp->sarja = haesarja_w(st, false);
			if (kilp->sarja < 0) {
				kilp->sarja = sarjaluku-1;
				Application->MessageBoxW((UnicodeString(L"Virheellinen sarja ")+
					UnicodeString(st)).c_str(), L"Virhe", MB_OK);
				return(ESC);
				}
//			kilp->synt = _wtoi(fields[2]);
			if (nfld < ++k)
				break;
//			sukupuoli = sarja[0] == L'N' ? L'L' : L'M';
			if (nfld < ++k)
				break;
			wcsncpy(kilp->seura, fields[k-1], kilpparam.lseura);
			locasewname(kilp->seura);
			break;
			}
		else {
			wcscpy(tlk[k-1].nimi, kilmoflds[1].name);
			if (nfld < ++k)
				break;
			wcsncpy(kilp->etunimi, fields[k-1], kilpparam.lenimi);
			wcscpy(tlk[k-1].nimi, kilmoflds[2].name);
			wcscpy(tlk[k-1].arvo, kilp->etunimi);
			if (nfld < ++k)
				break;
			wcsncpy(kilp->sukunimi, fields[k-1], kilpparam.lsnimi);
			wcscpy(tlk[k-1].nimi, kilmoflds[3].name);
			wcscpy(tlk[k-1].arvo, kilp->sukunimi);
			if (nfld < ++k)
				break;
			wcsncpy(st, fields[srjcol], 10);
			st[10] = 0;
			srj = haesarja_w(st, false);
			if (srj < 0) {
				wcsncpy(st1, fields[9-srjcol], 10);
				st1[10] = 0;
				if (haesarja_w(st1, false) >= 0) {
					srj = haesarja_w(st1, false);
					srjcol=9-srjcol;
					wcscpy(st1, kilmoflds[9-srjcol].name);
					wcscpy(kilmoflds[9-srjcol].name, kilmoflds[srjcol].name);
					wcscpy(kilmoflds[srjcol].name, st1);
					}
				}
			if (srj >= 0)
				kilp->setSarjaAll(srj);
			else {
				kilp->setSarjaAll(sarjaluku-1);
				Application->MessageBoxW((UnicodeString(L"Virheellinen sarja ")+
					UnicodeString(st)).c_str(), L"Virhe", MB_OK);
				ret = ESC;
				break;
				}
			if (nfld > 9-srjcol) {
				kilp->synt = _wtoi(fields[9-srjcol]);
				}
			wcscpy(tlk[k-1].nimi, kilmoflds[4].name);
			wcscpy(tlk[srjcol].arvo, Sarjat[kilp->Sarja()].sarjanimi);
			_itow(kilp->synt, tlk[9-srjcol].arvo, 10);
			if (nfld < ++k)
				break;
			wcscpy(tlk[k-1].nimi, kilmoflds[5].name);
//			kilp->synt = _wtoi(fields[k-1]);
			if (nfld < ++k)
				break;
			wcsncpy(kilp->seura, fields[k-1], kilpparam.lseura);
			wcscpy(tlk[k-1].nimi, kilmoflds[6].name);
			wcscpy(tlk[k-1].arvo, kilp->seura);
			if (nfld < ++k)
				break;
			kilp->lisno[1] = _wtoi(fields[k-1]);
			wcscpy(tlk[k-1].nimi, kilmoflds[7].name);
			_itow(kilp->lisno[1], tlk[k-1].arvo, 10);
			break;
			}
		}
	if (TulkintaOn || ret) {
		NaytaTulkinta(tlk, nRivi, rNo, ret ? L"Virheellinen tietue" : 0);
		switch (Jatko) {
			case 1:
				ret = 0;
				break;
			case 2:
				TulkintaOn = false;
				ret = 0;
				break;
			case 3:
				ret = 1;
				break;
			case 99:
				ret = ESC;
				break;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[kilp->Sarja(ipv)].lsak[ipv]; i++) {
			if (kilp->pv[ipv].asakot[i] == 0)
				kilp->pv[ipv].asakot[i] = L'-';
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(ret);
	}
//---------------------------------------------------------------------------

int __fastcall TFormImport::lue_SQL(void)
{
	int nSQLrec, n = 0, d;
	kilptietue kilp;

	nSQLrec = FormMySQL->lukumSQL();
	for (int i = 1; i < 9999 && n < nSQLrec; i++) {
		if (FormMySQL->readSQLrivi(&kilp, i)) {
			n++;
			}
		else {
			kilp.nollaa();
			kilp.kilpstatus = 0xffff;
			}
		if (i < nrec)
			kilp.tallenna(i, 0, 0, 0, 0);
		else
			kilp.addtall(&d, 0);
		EditLuettu->Text = UnicodeString(n);
		EditLuettu->Refresh();
		if ((n%10) == 0)
			Sleep(100);
		}
	return(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::Button1Click(TObject *Sender)
{
	wchar_t lajit[] = L" SH", erottimet[] = L";\t,;", Buf[10000];
	char cbuf[200];
	TextFl *InFile;
	kilptietue kilp;
	int d, n = 0, ret, ntot = 0;
	int vast = 0;
	int rNo = 0;

	laikaerot = 0;
	TulkintaOn = true;
	kilpparam_v.kilplaji = lajit[RGLaji->ItemIndex+1];
	n_pv_v = EditNpv->Text.ToInt();
	valuku_v = EditNva->Text.ToInt();
	OpenDialog1->InitialDir = FormMain->CurrentDir;

	switch (RGtiedlaji->ItemIndex) {
		case 0:
			erotin = erottimet[CBerotin->ItemIndex];

			OpenDialog1->DefaultExt = L"csv";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.csv), *.txt|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
					int nfld = Kexp_kentat + Pexp_kentat + Vexp_kentat + Xexp_kentat;
					exp_kentat_local = new FLD2[nfld];
					for (int i = 0; i < nfld; i++) {
						exp_kentat_local[i] = exp_kentat[i];
						if ((CBKaikkiKentat->Checked && i < Kexp_kentat + Pexp_kentat)
							|| (CBValiajat->Checked && i >= Kexp_kentat + Pexp_kentat)
							|| i >= Kexp_kentat + Pexp_kentat + Vexp_kentat)
							exp_kentat_local[i].len = 1;
						}
					if (RGCsvType->ItemIndex == 0)
						n = -1;
					EnterCriticalSection(&tall_CriticalSection);
					while (!InFile->Feof()) {
						if (RGCsvType->ItemIndex == 1)
							ret = lue_kilpcsv(InFile, &kilp, erotin);
						else
							ret = lue_kilpcsv2(InFile, &kilp, n, RGToiminto->ItemIndex, &d, erotin);
						if (ret == 0) {
							if (RGToiminto->ItemIndex > 0)
								kilp.tallenna(d, 0, 0, 0, 0);
							else
								kilp.addtall(&d, 0);
							n++;
							ntot++;
							}
						else if (ret == 1) {
							ntot++;
							}
						if (n < 0)
							n = 0;
						EditLuettu->Text = UnicodeString(n);
						EditLuettuTot->Text = UnicodeString(ntot);
						if (ntot%10 == 0) {
							EditLuettu->Refresh();
							EditLuettuTot->Refresh();
							Sleep(1);
							}
						}
					LeaveCriticalSection(&tall_CriticalSection);
					delete[] exp_kentat_local;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 1:
			n = -nilmt;
			sailTiedPos = CBsailPos->Checked;
			OpenDialog1->DefaultExt = L"xml";
			OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				if (RGXMLLaji->ItemIndex == 1)
					lueEventXml(OpenDialog1->FileName.c_str(), false, false, true);
				if (RGXMLLaji->ItemIndex == 0)
					lueIOF30EventXml(OpenDialog1->FileName.c_str(), false, false, true);
				}
			n += nilmt;
			break;
		case 2:
			erotin = erottimet[CBerotin->ItemIndex];

			OpenDialog1->DefaultExt = L"csv";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
//					InFile->ReadLine(Buf, 998);
					EnterCriticalSection(&tall_CriticalSection);
					while (!InFile->Feof()) {
						if ((ret = lue_ascii_e(InFile, &kilp, erotin)) == 0) {
							kilp.addtall(&d, 0);
							n++;
							ntot++;
							}
						else if (ret == 1) {
							ntot++;
							}
						EditLuettu->Text = UnicodeString(n);
						EditLuettuTot->Text = UnicodeString(ntot);
						}
					LeaveCriticalSection(&tall_CriticalSection);
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 3:
			OpenDialog1->DefaultExt = L"dat";
			OpenDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				kilpparam_v.n_pv = n_pv_v;
				kilpparam_v.n_pv_va = n_pv_v;
				kilpparam_v.valuku = valuku_v;
				lseuramax = RGseura->ItemIndex;

				n = luevanha(OpenDialog1->FileName.c_str());
				EditLuettu->Text = UnicodeString(n);
				}
			break;
		case 4:
			erotin = erottimet[CBerotin->ItemIndex];
			irmaflds[6].pos = RGmaa->ItemIndex;
			OpenDialog1->DefaultExt = L"txt";
			OpenDialog1->FileName = L"Ilmoittautumiset.csv";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.txt, *.csv)|*.txt;*.csv|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {

				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
					TextFl *SeuraFile = NULL;
					struct sralsttp {
						wchar_t lyh[LLYH+1];
						wchar_t seura[LSEURA+1];
						int alue;
					} *seuralst = NULL;
					int nsra = 0;

					if (CBPitkat->Checked) {
						SeuraFile = new TextFl(seuraTiedNm, L"rt");
						if (!SeuraFile || !SeuraFile->IsOpen()) {
							if (SeuraFile) {
								delete SeuraFile;
								SeuraFile = NULL;
								}
							OpenDialog2->DefaultExt = L"csv";
							OpenDialog2->FileName = seuraTiedNm;
							OpenDialog2->Filter = L"Teksti/csv-tiedostot (*.txt, *.csv)|*.txt;*.csv|Kaikki tiedostot (*.*)|*.*";
							if (OpenDialog2->Execute()) {
								SeuraFile = new TextFl(OpenDialog2->FileName.c_str(), L"rt");
								if (SeuraFile && !SeuraFile->IsOpen()) {
									delete SeuraFile;
									SeuraFile = NULL;
									}
								}
							}
						if (SeuraFile) {
							wchar_t ln[200];
							while (!SeuraFile->Feof()) {
								SeuraFile->ReadLine(ln, 198);
								if (wcslen(ln) > 3)
									nsra++;
								}
							seuralst = new struct sralsttp[nsra];
							nsra = 0;
							SeuraFile->Rewind();
							while (!SeuraFile->Feof()) {
								memset(ln, 0, sizeof(ln));
								SeuraFile->ReadLine(ln, 198);
								if (wcslen(ln) > 3) {
									wchar_t *p;

									p = wcstok(ln, L" ;\t\n");
									if (p) {
										memset(&seuralst[nsra], 0, sizeof(seuralst[nsra]));
										seuralst[nsra].alue = _wtoi(p);
										p = wcstok(NULL, L" ;\t\n");
										if (p) {
											wcsncpy(seuralst[nsra].lyh, p, kilpparam.lseuralyh);
											for (p = p+wcslen(p)+1; p < ln + 198 && *p <= L' '; p++) ;
											for (wchar_t *pp = ln+199; pp >= p && *pp <= L' '; pp--)
												*pp = 0;
											if (wcslen(p) > 1) {
												wcsncpy(seuralst[nsra].seura, p, kilpparam.lseura);
												nsra++;
												}
											}
										}
									}
								}
							delete SeuraFile;
							}
						}
					nTiedRivi = 0;

					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3 && Buf[0] != L'=') {
							nTiedRivi++;
							}
						}
					if (nTiedRivi == 0) {
						Application->MessageBoxW(L"Tiedostosta ei löytynyt osanottajatietoja", L"Virhe", MB_OK);
						delete InFile;
						return;
						}
					InFile->Rewind();
					kilptietue *temparr = new kilptietue[nTiedRivi];
					kilptietue *pkilp = temparr;

					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
//						if (wcslen(Buf) > 20 && (vast = tulkrivi(Buf, &kilp)) == 0)
						if (wcslen(Buf) > 3 && Buf[0] != L'=' && (vast = tulkIrmaRivi(Buf, pkilp, ++rNo, erotin)) == 0) {
							if (CBPitkat->Checked && nsra) {
								for (int isra = 0; isra < nsra; isra++) {
									if (!wcscmp(pkilp->seuralyh, seuralst[isra].lyh)) {
										wcscpy(pkilp->seura, seuralst[isra].seura);
										pkilp->piiri = seuralst[isra].alue;
										break;
										}
									}
								}
							pkilp++;
							n++;
							EditLuettu->Text = UnicodeString(n);
							}
						if (vast == ESC)
							break;
						}
					if (n == 0 || n < nTiedRivi) {
						wchar_t msg[200];
						swprintf(msg, L"Tiedostossa %d riviä, joista luettu %d osanottajaa. "
							L"Tallennetaanko puutteellisen luennan tiedot? "
							L"(Ellei tallenneta, voidaan lukeminen aloitta alusta, kun ongelma on korjattu.)", nTiedRivi, n);
						if (n > 0)
							vast = select3(2, msg, L"Luenta puutteellinen", L"Tallenna luetut tiedot",
								L"Jätä tallentamatta", L"", this);
						if (n == 0 || vast == 2) {
							vast = ESC;
							delete InFile;
							delete[] temparr;
							return;
							}
						}
					ntot = nTiedRivi;
					EnterCriticalSection(&tall_CriticalSection);
					for (int i = 0; i < n; i++)
						temparr[i].addtall(&d, 0);
					LeaveCriticalSection(&tall_CriticalSection);
					delete[] temparr;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 5:
			erotin = erottimet[CBerotin->ItemIndex];

			OpenDialog1->DefaultExt = L"txt";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.txt, *.csv)|*.txt;*.csv|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
					nTiedRivi = 0;

					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3) {
							nTiedRivi++;
							}
						}
					if (nTiedRivi == 0) {
						Application->MessageBoxW(L"Tiedostosta ei löytynyt osanottajatietoja", L"Virhe", MB_OK);
						delete InFile;
						return;
						}
					InFile->Rewind();
					kilptietue *temparr = new kilptietue[nTiedRivi];
					kilptietue *pkilp = temparr;

					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3 && (vast = tulkKilmoRivi(Buf, pkilp, ++rNo, erotin)) == 0) {
							pkilp++;
							n++;
							EditLuettu->Text = UnicodeString(n);
							}
						if (vast == ESC)
							break;
						}
					if (n == 0 || n < nTiedRivi) {
						wchar_t msg[200];
						swprintf(msg, L"Tiedostossa %d riviä, joista luettu %d osanottajaa. "
							L"Tallennetaanko puutteellisen luennan tiedot? "
							L"(Ellei tallenneta, voidaan lukeminen aloitta alusta, kun ongelma on korjattu.)", nTiedRivi, n);
						if (n > 0)
							vast = select3(2, msg, L"Luenta puutteellinen", L"Tallenna luetut tiedot",
								L"Jätä tallentamatta", L"", this);
						if (n == 0 || vast == 2) {
							vast = ESC;
							delete InFile;
							delete[] temparr;
							return;
							}
						}
					ntot = nTiedRivi;
					EnterCriticalSection(&tall_CriticalSection);
					for (int i = 0; i < n; i++)
						temparr[i].addtall(&d, 0);
					LeaveCriticalSection(&tall_CriticalSection);
					delete[] temparr;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 6:
			if (!FormMySQL)
				FormMySQL = new TFormMySQL(FormMain);
			if (!FormMySQL->SQLConnection1->Connected) {
				FormMySQL->ShowModal();
				FormImport->Refresh();
				}
			if (FormMySQL->SQLConnection1->Connected) {
				n = lue_SQL();
				Application->MessageBoxW(L"Tiedot luettu", L"Valmis", MB_OK);
				}
			break;
		}
	closefile(&datf2);
	SetCurrentDir(FormMain->CurrentDir);
	openfile(&datf2,wcstooem(cbuf, dataf2name, 100),kilpparam.kilprecsize);
	EditLuettu->Text = UnicodeString(n);
	EditLuettuTot->Text = UnicodeString(ntot);
	if (n > 1 && ((laikaerot > SEK && !pvparam[k_pv].hiihtolahto) || laikaerot > n * 3 * SEK))
		Application->MessageBoxW(L"Toteutunnet lähtöajat eroavat odottamattomasti arvotuista. Tarkasta tiedot", L"Varoitus", MB_OK);
	if (vast == ESC) {
		Application->MessageBoxW(L"Toiminto keskeytetty mahdolliseen virheeseen", L"Valmis", MB_OK);
		}
	else
		Application->MessageBoxW(L"Toiminto suoritettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormImport::FormShow(TObject *Sender)
{
	if (valuku_vanha == 0) {
		valuku_vanha = kilpparam.valuku;
		}
	RGLaji->ItemIndex = (kilpparam.kilplaji == L'H' || kilpparam.kilplaji == L'A');
	EditNpv->Text = UnicodeString(kilpparam.n_pv_akt);
	EditNva->Text = UnicodeString(valuku_vanha);
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::RGtiedlajiClick(TObject *Sender)
{
	Panel1->Visible = false;
	Panel2->Visible = false;
	Panel2->Enabled = false;
	GBPvVa->Visible = false;
	GBirma->Visible = false;
	RGLaji->Visible = false;
	RGXMLLaji->Visible = false;
	CBsailPos->Visible = false;
	CBKaikkiKentat->Visible = false;
	CBValiajat->Visible = false;
	LblMySQLvar->Visible = false;
	RGnimet->Enabled = false;
	CBerotin->Enabled = false;
//	RGmaa->Enabled = false;
	RGmaa->ItemIndex = 1;
	switch (RGtiedlaji->ItemIndex) {
		case 0:
			RGnimet->ItemIndex = 1;
			CBerotin->Enabled = true;
			CBerotin->ItemIndex = 0;
			Panel2->Visible = true;
			Panel2->Enabled = true;
			CBKaikkiKentat->Visible = true;
			CBValiajat->Visible = true;
			GBPvVa->Visible = RGCsvType->ItemIndex == 1;
			Panel3->Visible = RGCsvType->ItemIndex == 1;
			RGToiminto->Visible = !Panel3->Visible;
//			RGKilmo->Visible = false;
			break;
		case 1:
			RGXMLLaji->Visible = true;
			CBsailPos->Visible = RGXMLLaji->ItemIndex == 1;
//			RGKilmo->Visible = false;
			break;
		case 2:
			RGnimet->ItemIndex = 0;
			CBerotin->Enabled = true;
			CBerotin->ItemIndex = 0;
			Panel2->Visible = true;
			Panel2->Enabled = true;
			Panel3->Visible = true;
			GBPvVa->Visible = true;
			RGLaji->Visible = true;
			RGToiminto->Visible = !Panel3->Visible;
//			RGKilmo->Visible = false;
			break;
		case 3:
			RGnimet->ItemIndex = 0;
			Panel1->Visible = true;
			GBPvVa->Visible = true;
			RGLaji->Visible = true;
//			RGKilmo->Visible = false;
			break;
		case 4:
			RGnimet->ItemIndex = 0;
			CBerotin->Enabled = true;
			CBerotin->ItemIndex = 2;
			Panel2->Visible = true;
			Panel3->Visible = true;
			CBPitkat->Checked = true;
			GBirma->Visible = true;
			RGToiminto->Visible = !Panel3->Visible;
//			RGKilmo->Visible = false;
			break;
		case 5:
			RGnimet->ItemIndex = RGKilmo->ItemIndex;
			RGLaji->ItemIndex = 1;
			CBerotin->Enabled = true;
			CBerotin->ItemIndex = 3;
			Panel2->Visible = true;
			Panel3->Visible = true;
			RGToiminto->Visible = !Panel3->Visible;
//			RGKilmo->Visible = true;
			break;
		case 6:
//			RGKilmo->Visible = false;
			LblMySQLvar->Visible = true;
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::RGKilmoClick(TObject *Sender)
{
	RGnimet->ItemIndex = RGKilmo->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::RGCsvTypeClick(TObject *Sender)
{
	if (RGCsvType->ItemIndex == 0) {
		RGToiminto->Visible = true;
		Panel3->Visible = false;
		GBPvVa->Visible = false;
		}
	else {
		RGToiminto->Visible = false;
		RGToiminto->ItemIndex = 0;
		Panel3->Visible = true;
		GBPvVa->Visible = true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::RGXMLLajiClick(TObject *Sender)
{
	CBsailPos->Visible = RGXMLLaji->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::BtnKatseleClick(TObject *Sender)
{
	if (!FormTietoTulk)
		FormTietoTulk = new TFormTietoTulk(this);
	FormTietoTulk->NaytaSar = false;
	FormTietoTulk->Show();
	if (FormTietoTulk->WindowState == wsMinimized)
		FormTietoTulk->WindowState = wsNormal;
	FormTietoTulk->BringToFront();
}
//---------------------------------------------------------------------------

