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
#include "UnitMain.h"
#include "UnitMySQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormImport *FormImport;

static int n_os_v, n_va_v, k_arvo = 1, k_yhd = 1, valuku_v;
static wchar_t erotin = L';', st1[80], st2[80], Merkit[4] = L"8AO";
static bool fis = false, luearvo = false;
static   rivitulkintatp *RiviTulk;
static   int nTulkRivi;
static   bool TulkintaOn;
static   int Jatko;
static	 int nTiedRivi;
extern int lseuramax;
extern int valuku_vanha;
int luevanha(wchar_t *dataf2nm);
int NaytaTulkinta(rivitulkintatp *RiviTulk, int nRivi, int rNo, const wchar_t *sanoma);
int lueIOF30EventXml(wchar_t *filename, bool lueSarjat, bool lueRadat, bool lueOsanottajat);
void TallEhdoin(kilptietue *kilp, int pos);
char *keysa(void *vkilp, char *key, int keylen, int flags);
extern kilpindex *srjaakindex;

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
#if  defined(AMPUSUUNN)
   char ryhma[2];
   INT16  ohi[2];
   INT16  mmsakko;
#endif
   }  kilpparamtp_v;

extern kilpparamtp_v kilpparam_v;

static FLD2 kilmoflds[] = {
	{L"SporttiId", 1, 1, 0, 1},
	{L"IdLaji", 2, 1, 0, 2},
	{L"Nimi", 3, 1, 0, 3},
	{L"Synt", 4, 1, 0, 4},
	{L"Sarja", 5, 1, 0, 5},
	{L"Seura", 6, 1, 0, 6},
	{L"No", 7, 1, 0, 7},
	{L"Osuus", 8, 1, 0, 8},
	{L"FIS-Id", 9, 1, 0, 9}
};

static struct {
	int fld;
	int os;
	int va;
	} fldorder[15+MAXOSUUSLUKU*(19+2*VAIKALUKU)];
static int nflds;

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

int TFormImport::lue_kilpcsv(TextFl *afile, kilptietue *kilp, int lineno, int toiminto, int *dKilp, wchar_t erotin)
{
	wchar_t *rivi,s[40], *p, *p1, *pp;
	INT16 i,il,res = 0;
	INT16 srj, l, kbg;
	int len;

	wchar_t *fields[15+MAXOSUUSLUKU*(19+2*VAIKALUKU)], st[100] = L"", st1[100] = L"", st2[100] = L"", erottimet[] = L",";
	int nfld = 0, k = 0, ret = 0, nRivi = 9;
	char ast[100];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
	len = 250 + kilpparam.osuusluku * (300+32*kilpparam.maxvaluku);
	rivi = (wchar_t *)calloc(len+1, sizeof(wchar_t));
	if (afile->ReadLine(rivi, len) == NULL) {
		free(rivi);
		return(-1);
		}
	if (wcslen(rivi) < 4)
		return(-1);
	kilp->nollaa();
	nfld = getfields(rivi, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"" , true, false);
	free(rivi);
	if (lineno < 0) {
		memset(fldorder, 0, sizeof(fldorder));
		nflds = 0;
		for (UINT ifld = 0; ifld < nfld; ifld++) {
			int os = 0;
			int va = 0;
			for (int i = 0; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
				if (i < Kexp_kentat && wcscmp(exp_kentat_local[i].name, fields[ifld]) == 0) {
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
						if ((os = _wtoi(p1)) < 1 || os > kilpparam.n_os_akt)
							continue;
						pp = wcstok(NULL, L"-");
						if (pp)
							va = _wtoi(pp);
						}
					if (os < 1 || os > kilpparam.n_os_akt)
						continue;
					if (va == 0 && i < Kexp_kentat+Pexp_kentat &&
						wcscmp(exp_kentat_local[i].name, p) == 0) {
						fldorder[ifld].fld = exp_kentat_local[i].id;
						fldorder[ifld].os = os;
						if (nflds <= ifld)
							nflds = ifld+1;
						break;
						}
					if (i < Kexp_kentat+Pexp_kentat || va < 1 || va > kilpparam.valuku)
						continue;
					if (wcscmp(exp_kentat_local[i].name, p) == 0) {
						fldorder[ifld].fld = exp_kentat_local[i].id;
						fldorder[ifld].os = os;
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
	rivitulkintatp *tlk;
	tlk = (rivitulkintatp *)calloc(15+MAXOSUUSLUKU*(19+2*VAIKALUKU), sizeof(rivitulkintatp));
	for (UINT ifld = 0; ifld < nfld; ifld++) {
		if (fldorder[ifld].fld == 0)
			continue;
		tlk[k].no = k + 1;
		wcsncpy(tlk[k].teksti, fields[k], sizeof(tlk[k].teksti)/2-1);
		k++;
		}
	free(tlk);
	for (UINT ifld = 0; res == 0 && ifld < nflds; ifld++) {
		wchar_t tfld[200];
		if (fldorder[ifld].fld == 0)
			continue;
		if (ifld < nfld) {
			wcsncpy(tfld, fields[ifld], 199);
			tfld[199] = 0;
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
					kilp->getrec(*dKilp);
					}
				kilp->kilpno = _wtoi(tfld);
				break;
			case FLDID_JOUKKID:
				kilp->joukkid = _wtoi(tfld);
				break;
			case FLDID_PIIRI:
				kilp->piiri = _wtoi(tfld);
				break;
			case FLDID_SEURA:
				WcsToMbs(kilp->seura, tfld, kilpparam.lseura);
				break;
			case FLDID_JOUKKNO:
				kilp->joukkue = _wtoi(tfld);
				break;
			case FLDID_JOUKKNIMI:
				WcsToMbs(kilp->joukknimi, tfld, kilpparam.lseura);
				break;
			case FLDID_MAA:
				WcsToMbs(kilp->maa, tfld, sizeof(kilp->maa)-1);
				break;
			case FLDID_SARJA:
				k = haesarja_w(tfld, false);
				if (k >= 0)
					kilp->sarja = k;
				break;
			case FLDID_ERA:
				kilp->era = _wtoi(tfld);
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
			case FLDID_KILPID:
				kilp->ostiet[fldorder[ifld].os-1].lisno = _wtoi(tfld);
				break;
			case FLDID_SUKUNIMI:
				memset(ast, 0, sizeof(ast));
				WcsToMbs(ast, tfld, kilpparam.lnimi);
				if (kilp->ostiet[fldorder[ifld].os-1].nimi[0] && strlen(ast) < kilpparam.lnimi-2) {
					if (kilp->ostiet[fldorder[ifld].os-1].nimi[0] != '|')
						strcat(ast, "|");
					strncpy(ast+strlen(ast), kilp->ostiet[fldorder[ifld].os-1].nimi, 
						kilpparam.lnimi-strlen(ast));
					for (char *p = ast+strlen(ast)-1; *p > 127 && p > ast; p--)
						*p = 0;
					}
				memset(kilp->ostiet[fldorder[ifld].os-1].nimi, 0, 
					sizeof(kilp->ostiet[fldorder[ifld].os-1].nimi));
				strncpy(kilp->ostiet[fldorder[ifld].os-1].nimi, ast, kilpparam.lnimi);
				break;
			case FLDID_ETUNIMI:
				if (strlen(kilp->ostiet[fldorder[ifld].os-1].nimi) >= kilpparam.lnimi - 1)
					break;
				if (kilp->ostiet[fldorder[ifld].os-1].nimi[0] == 0 ||
					kilp->ostiet[fldorder[ifld].os-1].nimi[strlen(kilp->ostiet[fldorder[ifld].os-1].nimi)-1] != '|')
					strcat(kilp->ostiet[fldorder[ifld].os-1].nimi, "|");
				WcsToMbs(kilp->ostiet[fldorder[ifld].os-1].nimi+strlen(kilp->ostiet[fldorder[ifld].os-1].nimi),
					tfld, kilpparam.lnimi-strlen(kilp->ostiet[fldorder[ifld].os-1].nimi));
				break;
			case FLDID_NIMI:
				kilp->setNimi(tfld, fldorder[ifld].os-1, 1 - 2*RGNimiJarj->ItemIndex);
				break;
			case FLDID_ARVO:
				WcsToMbs(kilp->ostiet[fldorder[ifld].os-1].arvo, tfld,
					sizeof(kilp->ostiet[fldorder[ifld].os-1].arvo)-1);
				break;
			case FLDID_OSSEURA:
				WcsToMbs(kilp->ostiet[fldorder[ifld].os-1].osseura, tfld,
					sizeof(kilp->ostiet[fldorder[ifld].os-1].osseura)-1);
				break;
			case FLDID_BADGE:
				kilp->ostiet[fldorder[ifld].os-1].badge[0] = _wtoi(tfld);
				break;
			case FLDID_LAINA:
				kilp->ostiet[fldorder[ifld].os-1].laina[0] = wchartoansi(tfld[0]);
				break;
			case FLDID_BADGE2:
				kilp->ostiet[fldorder[ifld].os-1].badge[1] = _wtoi(tfld);
			case FLDID_LAINA2:
				kilp->ostiet[fldorder[ifld].os-1].laina[1] = wchartoansi(tfld[0]);
				break;
			case FLDID_RATA:
				WcsToMbs(kilp->ostiet[fldorder[ifld].os-1].hajonta, tfld,
					sizeof(kilp->ostiet[fldorder[ifld].os-1].hajonta)-1);
			case FLDID_TARK:
				kilp->SetTark(fldorder[ifld].os-1, tfld[0]);
				break;
			case FLDID_AMPSAKKO:
				WcsToMbs(kilp->ostiet[fldorder[ifld].os-1].ampsakot, tfld,
					sizeof(kilp->ostiet[fldorder[ifld].os-1].ampsakot)-1);
			case FLDID_SAKKO:
				kilp->ostiet[fldorder[ifld].os-1].sakko = _wtoi(tfld);
				break;
			case FLDID_OSPISTE1:
			case FLDID_OSPISTE2:
			case FLDID_OSPISTE3:
				kilp->ostiet[fldorder[ifld].os-1].pisteet[fldorder[ifld].fld-FLDID_OSPISTE1] =
					_wtoi(tfld);
				break;
			case FLDID_SELITYS:
				WcsToMbs(ast, tfld, SELITYS);
				memcpy(kilp->ostiet[fldorder[ifld].os-1].selitys, ast,
					sizeof(kilp->ostiet[fldorder[ifld].os-1].ufill_1));
				break;
			case FLDID_LAHTOLAJI:
				kilp->ostiet[fldorder[ifld].os-1].lahtolaji = _wtoi(tfld);
				break;
			case FLDID_LAHTO:
				kilp->ostiet[fldorder[ifld].os-1].ylahto = wstrtoaika_vap(tfld, t0);
				break;
			case FLDID_MAALI:
				kilp->setMaali(fldorder[ifld].os-1, 0, wstrtoaika_vap(tfld, t0));
				break;
			case FLDID_SIJA:
				kilp->setSija(fldorder[ifld].os-1, 0, _wtoi(tfld));
				break;
			case FLDID_VAAIKA:
				kilp->setMaali(fldorder[ifld].os-1, fldorder[ifld].va, wstrtoaika_vap(tfld, t0));
				break;
			case FLDID_VASIJA:
				kilp->setSija(fldorder[ifld].os-1, fldorder[ifld].va,_wtoi(tfld));
				break;
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
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

#define  JJLINE 80

static int luenro = 1;
static int lisnox = 9;
static wchar_t jjline[MAXOSUUSLUKU+1][JJLINE+1];

static int lueuusirivi(TextFl *etied, int n, int seurjoukk)
   {
   int i, k, r;
   wchar_t line[200], *p;

   for (i = 0; i < n; i++) {
	  if (seurjoukk < 0) {
		 r = i+1;
		 if (r > kilpparam.n_os_akt)
			break;
		 }
	  else {
		 r = kilpparam.n_os_akt;
		 memmove(jjline[0], jjline[1], kilpparam.n_os_akt*sizeof(jjline[0]));
		 }
	  memset(jjline[r], 0, sizeof(jjline[r]));
	  memset(line, 0, sizeof(line));
	  if (!etied->Feof())
		  etied->ReadLine(line, 198);
	  if (wcslen(line) > 0) {
		 wcsncpy(jjline[r], line, JJLINE);
		 for (p = jjline[r] + wcslen(jjline[r])-1;
			p >= jjline[r] && *p <= L' '; p--)
			*p = 0;
		 }
	  else {
		 wcscpy(jjline[r], L"TIEDOSTO LOPPU");
		 }
	  if (seurjoukk > 0) {
		 k = 0;
		 for (p = jjline[0]; *p; p++)
			if (*p == erotin)
				k++;
		 if (k > 2) break;
		 }
	  }
   return(i);
   }

int tulkrivi(wchar_t *line, int *srj, int *kno, int *jno, wchar_t *maa, int *piiri)
   {
   wchar_t *p, ero[2] = L" ";
   int plen, res = 1;

   ero[0] = erotin;
   *srj = -1;
   *kno = 0;
   *jno = 0;
   *piiri = 0;
   maa[0] = 0;

   p = wcstok(line, ero);
   if (!p)
		return(1);
   plen = wcslen(p);
   elimwbl(p);
   upcasewstr(p);
   while (*p == L' ')
		p++;
   *srj = haesarja_w(p, false);
   if (p[plen+1] != erotin) {
	  p = wcstok(NULL, ero);
	  if (p) {
		 while (*p == L' ')
			p++;
		 elimwbl(p);
		 *kno = _wtoi(p);
		 }
	  }
   if ((p = wcstok(NULL, ero)) != NULL) {
	  res = 0;
	  elimwbl(p);
	  wcscpy(line, p);
	  if ((p = wcstok(NULL, ero)) != NULL) {
		 elimwbl(p);
		 *jno = _wtoi(p);
		 if ((p = wcstok(NULL, ero)) != NULL) {
			elimwbl(p);
			wcsncpy(maa, p, 3);
			if ((p = wcstok(NULL, ero)) != NULL) {
			   elimwbl(p);
			   *piiri = _wtoi(p);
			   }
			}
		 }
	  }
   return(res);
   }

//---------------------------------------------------------------------------
/*
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
				if (wcswcind(L'1'+ipv, fields[k-1]) < 0) {
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
			if ((p = wcsstr(fields[k-1], L".")) != NULL) {
				wmemmove(fields[k-1]+1, fields[k-1], p - fields[k-1]);
				kilp->piste[2] = _wtoi(fields[k-1]+1);
				}
			wcscpy(tlk[k-1].nimi, irmaflds[9].name);
			_itow(kilp->piste[2], tlk[k-1].arvo, 10);
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
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(ret);
	}
*/
//---------------------------------------------------------------------------
static int tulkkilmojoukkue(wchar_t *line, int *srj, int *kno, int *jno, wchar_t *maa, int *piiri)
   {
   int res = 0;
/*
	wchar_t *p, *fields[20], st[100] = "", erottimet[] = ";";
	int nfld = 0, k;

	memset(fields, 0, sizeof(fields));
	nfld = getfields(line, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"", TRUE, FALSE);
	if (nfld < 8 || _wtoi(fields[7]) > 0)
		res = 1;
	else {
		wcscpy(st, fields[4]);
		upcasewstr(st);
		if ((k = strlen(st)) > 2 && st[k-1] == L'V' && st[k-2] == L'-')
			st[k-2] = 0;
		*srj = haesarja_w(st, false);
		if (*srj < 0)
			res = 1;
		}
	if (res == 0) {
		wcscpy(line, fields[5]);
		wcscpy(st, fields[2]);
		for (p = st+wcslen(st); p > st+1 && p[-1] >= L'0' && p[-1] <= L'9'; p--) ;
		*jno = _wtoi(p);
		if (*jno > 20)
			*jno = 0;
		*kno = _wtoi(fields[6]);
		}
	maa[0] = 0;
	*piiri = 0;
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
*/
   return(res);
   }

static int tulkkilmoosuus(wchar_t *line, osrec *ostiet, int osuus)
   {
   int res = 0;
/*
	wchar_t *p, *fields[20], st[100] = "", erottimet[] = ";";
	int nfld = 0, k;

	memset(fields, 0, sizeof(fields));
	nfld = getfields(line, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"", TRUE, FALSE);
	if (nfld < 8 || _wtoi(fields[7]) != osuus)
		res = 1;
	else {
		ostiet->lisno = _wtoi(fields[0]);
		strncpy(ostiet->nimi, fields[2], sizeof(ostiet->nimi)-1);
		if (!wcscmp(ostiet->nimi, _"Nimi puuttuu"))
			memset(ostiet->nimi, 0, sizeof(ostiet->nimi));
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
*/
   return(res);
   }

 int tulkKilmoRivi(wchar_t *iBuf, kilptietue &kilp, int rNo, wchar_t erotin)
{
	int nfld = 0, k = 0, ret = 0, nRivi = 9, srj, os, d, kno;

	wchar_t *p, *fields[50], st[100] = L"", st1[20], erottimet[] = L";";
	static int srjcol=4;
	rivitulkintatp tlk[30];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
	memset(tlk, 0, sizeof(tlk));
	nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"", true, false);
	for (UINT i = 0; i < nfld && i < sizeof(kilmoflds)/sizeof(kilmoflds[0]); i++) {
		tlk[i].no = i + 1;
		wcsncpy(tlk[i].teksti, fields[i], sizeof(tlk[i].teksti)/2-1);
		}
	if (nfld < 8 || (kno = _wtoi(fields[6])) < 1)
		ret = -1;
	wcscpy(tlk[6].nimi, L"KilpNo");
	_itow(kno, tlk[6].arvo, 10);
	while (ret == 0) {
//		d = getpos(kno);
		if ((os = _wtoi(fields[7]) - 1) < 0) {
			kilp.nollaa();
			p = wcsstr(fields[4], L"-V");
			if (wmemcmpU(fields[4]+wcslen(fields[4])-2, L"-V", 2) == 0)
				fields[4][wcslen(fields[4])-2] = 0;
			kilp.sarja = haesarja_w(fields[4], false);
			if (kilp.sarja < 0) {
				ret = -1;
				break;
				}
			kilp.kilpno = kno;
			kilp.joukkid = kno;
			kilp.setSeura(fields[5]);
			kilp.joukkue = _wtoi(fields[3]);
			wcscpy(tlk[2].nimi, L"Joukkue");
			kilp.Joukkue(tlk[2].arvo, 58);
			ret = 1;
			break;
//			addtall(&kilp, &d, 0);
			}
		else {
			osrec *ostiet;
//			if (d < 1) {
			if (kno != kilp.kilpno || os >= Sarjat[kilp.sarja].osuusluku) {
				ret = -1;
				break;
				}
//			kilp.getrec(d);
			ostiet = &kilp.ostiet[os];
			wcscpy(tlk[7].nimi, L"Osuus");
			_itow(os+1, tlk[7].arvo, 10);
			ostiet->lisno = _wtoi(fields[0]);
			wcscpy(tlk[0].nimi, L"LisNo");
			_itow(ostiet->lisno, tlk[0].arvo, 10);
			ostiet->setNimi(fields[2], -1);
			wcscpy(tlk[2].nimi, L"Etunimi Sukunimi");
			ostiet->Nimi(tlk[2].arvo, 58, -1);
			WcsToMbs(ostiet->osseura, fields[5], sizeof(ostiet->osseura)-1);
			wcscpy(tlk[5].nimi, L"Osuusseura");
			ostiet->OsSeura(tlk[5].arvo);
			if (nfld > 8) {
				ostiet->pisteet[0] = _wtoi(fields[8]);
				wcscpy(tlk[8].nimi, L"FISkoodi");
				_itow(ostiet->pisteet[0], tlk[8].arvo, 10);
				}
			ret = 0;
			break;
			}
		}
	wcscpy(tlk[4].nimi, L"Sarja");
	if (kilp.sarja >= 0)
		Sarjat[kilp.sarja].Sarjanimi(tlk[4].arvo, false);
	wcscpy(tlk[5].nimi, L"Seura");
	kilp.Seura(tlk[5].arvo, LSEURA);
	if (TulkintaOn || ret < 0) {
		NaytaTulkinta(tlk, nRivi, rNo, ret < 0 ? L"Virheellinen tietue" : 0);
		switch (Jatko) {
			case 1:
				break;
			case 2:
				TulkintaOn = false;
				break;
			case 3:
				break;
			case 99:
				ret = ESC;
				break;
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
		if (i < datf2.numrec)
			tallenna(&kilp, i, 0, 0, 0, 0);
		else
			addtall(&kilp, &d, 0);
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
	int d, n = 0, nkorv = 0, ret, ntot = 0, nos = 0;
	int vast = 0;
	int rNo = 0;

	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;
	TulkintaOn = true;
/*
	kilpparam_v.kilplaji = lajit[RGLaji->ItemIndex+1];
	n_pv_v = EditNpv->Text.ToInt();
	valuku_v = EditNva->Text.ToInt();
*/
	switch (RGtiedlaji->ItemIndex) {
		case 0:
			if (RGtoiminto->ItemIndex < 0) {
				Application->MessageBoxW(L"Valitse toteutettava toiminto", L"Este", MB_OK);
				kirjheti = kirjheti0;
				return;
				}
			erotin = erottimet[CBerotin->ItemIndex];

			OpenDialog1->DefaultExt = L"csv";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.csv), *.txt|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
					int nfld = Kexp_kentat + Pexp_kentat + Vexp_kentat;
					exp_kentat_local = new FLD2[nfld];
					Label5->Visible = true;
					for (int i = 0; i < nfld; i++) {
						exp_kentat_local[i] = exp_kentat[i];
						}
					n = -1;
					while (!InFile->Feof()) {
						if ((ret = lue_kilpcsv(InFile, &kilp, n, RGtoiminto->ItemIndex, &d, erotin)) == 0) {
							if (RGtoiminto->ItemIndex > 0)
								tallenna(&kilp, d, 0, 0, 0, 0);
							else
								addtall(&kilp, &d, 0);
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
					delete[] exp_kentat_local;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				Label5->Visible = false;
				delete InFile;
				}
			break;
		case 1:
			n = -nilmt;
			OpenDialog1->DefaultExt = L"xml";
			OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				lueIOF30EventXml(OpenDialog1->FileName.c_str(), false, false, true);
				}
			n += nilmt;
			break;
		case 2:
			OpenDialog1->DefaultExt = L"dat";
			OpenDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
//				kilpparam_v.n_pv = n_pv_v;
//				kilpparam_v.n_pv_va = n_pv_v;
//				kilpparam_v.valuku = valuku_v;
//				lseuramax = RGseura->ItemIndex;
				n = luevanha(OpenDialog1->FileName.c_str());
				EditLuettu->Text = UnicodeString(n);
				}
			break;
		case 3:
			if (RGtoiminto->ItemIndex < 0) {
				Application->MessageBoxW(L"Valitse toteutettava toiminto", L"Este", MB_OK);
				kirjheti = kirjheti0;
				return;
				}
			erotin = erottimet[CBerotin->ItemIndex];
//			irmaflds[6].pos = RGmaa->ItemIndex;
			OpenDialog1->DefaultExt = L"txt";
			OpenDialog1->FileName = L"Ilmoittautumiset.txt";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.txt, *.csv)|*.txt;*.csv|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {

				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt", Merkit[RGMerkit->ItemIndex]);
				if (InFile->IsOpen()) {
					nTiedRivi = 0;
					int nr;

					if (RGtoiminto->ItemIndex == 1)
						if (teeindeksi(srjaakindex, true))
							return;
					Label5->Visible = true;
					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3 && Buf[0] != L'=') {
							nTiedRivi++;
							}
						}
					if (nTiedRivi == 0) {
						Application->MessageBoxW(L"Tiedostosta ei löytynyt osanottajatietoja", L"Virhe", MB_OK);
						delete InFile;
						kirjheti = kirjheti0;
						return;
						}
					InFile->Rewind();

					nr = kilpparam.n_os_akt+1;
					while (true) {
						int k, bdg = 0, etusuku, srj, piiri, jno;
						int luelaina;
						wchar_t maa[4], cpl;
						bool paivita = false;
						char key[NIMIINDL+3] = "", key0[NIMIINDL+3];

						kilp.nollaa();
						etusuku = 1 - 2*RGNimiJarj->ItemIndex;

						lueuusirivi(InFile, nr, 1);

						if (!wmemcmp(jjline[0], L"TIEDOSTO LOPPU", 10))
							break;
						nr = 1;
						if (jjline[0][0] == L'=') {
							continue;
							}
						wcscpy(Buf, jjline[0]);
						wmemset(maa, 0, 4);
						if (!tulkrivi(Buf, &srj, &k, &jno, maa, &piiri)) {
							if (srj < 0) {
								if (Application->MessageBoxW((UnicodeString(L"Rivillä\r\n")+jjline[0]+L"\r\nvirheellinen sarja.  Keskeytä lukeminen?").c_str(),
									L"Virhe", MB_YESNO) == IDYES)
									break;
								continue;
								}
							kilp.setSeura(Buf);
							kilp.joukkue = jno;
							kilp.sarja = srj;
							kilp.setMaa(maa);
							kilp.piiri = piiri;
							if (luenro)
								kilp.kilpno = k;
							if (RGtoiminto->ItemIndex == 1) {
								if (kilp.kilpno > 0 && (d = getpos(kilp.kilpno)) > 0) {
									paivita = true;
									}
								else {
									keysa(&kilp, key, srjaakindex->keyl, 0);
									memcpy(key0, key, srjaakindex->keyl);
									searchix(srjaakindex, key, &d);
									if (d > 0 && memcmp(key, key0, srjaakindex->keyl) == 0) {
										paivita = true;
										}
									}
								if (paivita) {
									kilp.getrec(d);
									if (k)
										kilp.kilpno = k;
									kilp.setSeura(Buf);
									kilp.joukkue = jno;
									kilp.sarja = srj;
									kilp.setMaa(maa);
									if (piiri)
										kilp.piiri = piiri;
									}
								}
							}
						else {
							continue;
							}
						for (int i = 0; i < Sarjat[srj].osuusluku; i++) {
							k = 0;
							bdg = 0;
							cpl = 0;
							if (jjline[i+1][0] == L'=' || !wmemcmp(jjline[i+1], L"TIEDOSTO LOPPU", 10))
								break;
							wcsncpy(Buf, jjline[i+1], 199);
							elimwbl(Buf);
							for (wchar_t *p = Buf; *p; p++) {
								if (*p == erotin) {
									k++;
									*p = 0;
									while (p[1] == L' ')
										p++;
									if (k == 1 && (p[1] >= L'0' && p[1] <= L'9') || p[1] == L'L') {
										if (luelaina) {
											wchar_t *pl;
											for (pl = p+1; *pl && *pl != L'L'; pl++) ;
											cpl = (*pl == L'L') ? L'L' : L' ';
											}
										if (p[1] != L'L')
											bdg = _wtol(p+1);
										}
									}
								}
							if (k > 2)
								break;
							nr++;
							elimwbl(Buf);
							if (Buf[0])
								kilp.setNimi(Buf, i, etusuku);
							if (cpl)
								kilp.setLaina(i, cpl > L' ', 0);
							kilp.ostiet[i].badge[0] = bdg;
							}
						if (paivita) {
							tallenna(&kilp, d, 0, 0, 0, 0);
							nkorv++;
							}
						else
							addtall(&kilp, &d, 0);
						n++;
						EditLuettu->Text = UnicodeString(n);
						if (n%10 == 0) {
							EditLuettu->Refresh();
							Sleep(1);
							}
						}
					Label5->Visible = false;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 4:
			erotin = erottimet[CBerotin->ItemIndex];

			OpenDialog1->DefaultExt = L"csv";
			OpenDialog1->Filter = L"Teksti/csv-tiedostot (*.txt, *.csv)|*.txt;*.csv|Kaikki tiedostot (*.*)|*.*";
			if (OpenDialog1->Execute()) {
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
				if (InFile->IsOpen()) {
					nTiedRivi = 0;

					Label5->Visible = true;
					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3) {
							nTiedRivi++;
							}
						}
					if (nTiedRivi == 0) {
						Application->MessageBoxW(L"Tiedostosta ei löytynyt osanottajatietoja", L"Virhe", MB_OK);
						delete InFile;
						kirjheti = kirjheti0;
						return;
						}
					InFile->Rewind();
					kilptietue pkilp;
					int d;

					while (!InFile->Feof()) {
						InFile->ReadLine(Buf, 998);
						if (wcslen(Buf) > 3 && (vast = tulkKilmoRivi(Buf, pkilp, ++rNo, erotin)) == 0 || vast == 1) {
							if (vast == 1) {
								addtall(&pkilp, &d, 0);
								n++;
								}
							if (vast == 0) {
								pkilp.Tallenna(d, 0, 0, 0, 0);
								nos++;
								}
							EditLuettu->Text = UnicodeString(n);
							}
						if (vast == ESC)
							break;
						}
 /*
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
					EnterCriticalSection(&tall_CriticalSection);
					for (int i = 0; i < n; i++)
						temparr[i].addtall(&d, 0);
					LeaveCriticalSection(&tall_CriticalSection);
					delete[] temparr;
*/
					ntot = nTiedRivi;
					Label5->Visible = false;
					}
				else {
					Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
					}
				delete InFile;
				}
			break;
		case 5:
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
	kirjheti = kirjheti0;
	SetCurrentDir(FormMain->CurrentDir);
	openfile(&datf2,dataf2name,kilpparam.kilprecsize);
	EditLuettu->Text = UnicodeString(n);
	EditLuettuTot->Text = UnicodeString(ntot);
	if (vast == ESC) {
		Application->MessageBoxW(L"Toiminto keskeytetty mahdolliseen virheeseen", L"Valmis", MB_OK);
		}
	else {
		if (nkorv > 0)
			Application->MessageBoxW((UnicodeString(nkorv)+L" joukkuetta päivitetty, "+
				UnicodeString(n-nkorv)+L" joukkuetta lisätty").c_str(), L"Valmis", MB_OK);
		else
			Application->MessageBoxW(L"Toiminto suoritettu", L"Valmis", MB_OK);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormImport::FormShow(TObject *Sender)
{
	if (valuku_vanha == 0) {
		valuku_vanha = kilpparam.valuku;
		}
	RGLaji->ItemIndex = (kilpparam.kilplaji == L'H' || kilpparam.kilplaji == L'A');
	EditNpv->Text = UnicodeString(kilpparam.n_os_akt);
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
	LblXMLtieto->Visible = false;
	switch (RGtiedlaji->ItemIndex) {
		case 0:
			CBerotin->ItemIndex = 0;
			Panel2->Visible = true;
			GBPvVa->Visible = false;
			RGLaji->Visible = false;
			RGtoiminto->Visible = true;
			break;
		case 1:
			CBerotin->ItemIndex = 0;
			Panel2->Visible = false;
			GBPvVa->Visible = false;
			RGLaji->Visible = false;
			LblXMLtieto->Visible = true;
			break;
		case 2:
			CBerotin->ItemIndex = 0;
			Panel2->Visible = false;
			GBPvVa->Visible = true;
			RGLaji->Visible = true;
			if (RGLaji->ItemIndex == 0) {
				Label11->Visible = true;
				EdtHaj->Visible = true;
			}
			break;
		case 3:
			CBerotin->ItemIndex = 2;
			Panel2->Visible = true;
			GBPvVa->Visible = false;
			RGLaji->Visible = false;
			RGtoiminto->Visible = true;
			RGNimiJarj->ItemIndex = 1;
			break;
		case 4:
			RGLaji->ItemIndex = 1;
			CBerotin->ItemIndex = 3;
			Panel2->Visible = true;
			GBPvVa->Visible = false;
			RGLaji->Visible = false;
			RGtoiminto->Visible = false;
			RGNimiJarj->ItemIndex = 1;
			break;
		case 5:
			RGLaji->ItemIndex = 0;
			CBerotin->ItemIndex = 0;
			Panel2->Visible = false;
			GBPvVa->Visible = false;
			RGLaji->Visible = false;
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormImport::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

