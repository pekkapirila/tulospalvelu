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

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <butil.h>
#pragma hdrstop
#include <time.h>
#include "HkDeclare.h"
#include "HkMuotoilu.h"

#ifndef _CONSOLE
#include "WinHk.h"
#include "TulostusUnit2.h"
#include "UnitJoukkueTilanne.h"
#include "UnitJakelu.h"
#endif

int yhttl = 0;

#define SEURATUL
#define SEURALISTA 15

lisatxttp lisatxt[4];

void taustatulostus(void) {}

void yhteenveto(tulostusparamtp *tulprm);
int xmlots(tulostusparamtp *tulprm);
int xmlloppu(tulostusparamtp *tulprm);
int xmlsrjots(int sarja, tulostusparamtp *tulprm);
int xmlsrjloppu(tulostusparamtp *tulprm);
void xmltulos(kilptietue& kilp, int sarja, int sj, tulostusparamtp *tulprm);
int xmlfisots(tulostusparamtp *tulprm);
int xmlfisloppu(tulostusparamtp *tulprm);
void laskefispisteet(int srj, tulostusparamtp *tulprm);
int xmlfissrjots(int sarja, tulostusparamtp *tulprm);
int xmlfissrjloppu(tulostusparamtp *tulprm);
void xmlfistulos(kilptietue *kilp, INT sj, tulostusparamtp *tulprm);

void startAutojakelu(void);
INT avaa_tiedosto(tulostusparamtp *tulprm);
INT avaa_tiedosto_nm(wchar_t *fname);
void sulje_tiedosto(TextFl **tul_tied);

//wchar_t xmlva = L'E';

#define OPTALISRJ 0x01000000

PageFieldTp PageField[] = {
	F_KNO, L"Nro",  0, 6, 0, 1, 1,                      	// 1: numero
	F_NIMI,L"Nimi", 300, 500, 40, 1, 0,              	// 2: nimi
	F_SRA, L"Seura", 0, 45, 0, 1, 0,                    	// 7: seura
	F_MAA, L"Maa", 0, 56, 0, 1, 0,                      	// 9: maa
	F_TLS, L"Tulos", 0, 56, 0, 1, 1,                    	// 13: tulos
	F_SJ,  L"Sija", 0, 0, 0, 1, 1                       	// 0: sija
	};

UINT PageFieldCount = sizeof(PageField)/sizeof(PageField[0]);

INT mrivi[3] = {8,8,8};
wchar_t palk[MAXSARJALUKU+MAXYHD];
static INT lastpos;
static int TiedRivi;
static wchar_t wtitle[120] = L"";
static wchar_t wheader[120] = L"";

int ssltlsfl = 1;
INT lahetacr = 1;
INT potsfl = FALSE;
INT htmlpitkat;
int tulera;
wchar_t tul_tied_nimi[80] = L"TULOSLUE.1";
static int karkikierr = 0;
static int TLpKarki = 0;

#ifdef SEURATUL
typedef struct {
   wchar_t sr[LSEURA+2];
   INT piiri;
   INT pos;
   } srixtype;

static srixtype *srix;
static wchar_t *sr_nimet;

wchar_t seuranimi[SEURALISTA][LSEURA+1];
wchar_t piirinimi[][41] = {
   L"Etel‰-Pohjanmaa",
   L"H‰me",
   L"Kaakko",
   L"Kainuu",
   L"Keski-Pohjanmaa",
   L"Keski-Suomi",
   L"Lahti",
   L"Lappi",
   L"Pohj.-Pohjanmaa",
   L"Satakunta",
   L"Savo-Karjala",
   L"Uusimaa",
   L"Varsinais-Suomi",
   L"FSO",
	L"",
   L"",
   L"",
   L"",
   L"",
   L"Sweden",
   L"Norway",
   L"Other",
   L"",
	L"",
   L"",
   L"",
   L"",
   L"",
	L"",
   L"",
   L"",
   L"",
   L"",
   L"",
   L"",
   L"",
   L"",
   L""
   };

int piiriluku = sizeof(piirinimi)/sizeof(piirinimi[0]);

INT piirifl[200];

#define PM_LUKU 8

#ifdef MCPIST
static INT mcpist[30] =
	  {100, 80, 60, 50, 45, 40, 36, 32, 29, 26,
		24, 22, 20, 18, 16, 15, 14, 13, 12, 11,
		10,  9,  8,  7,  6,  5,  4,  3,  2,  1};
#endif

typedef struct {
	wchar_t *seura;
   } sraluettp;

sraluettp  *seurat;

#endif

tulostusparamtp::tulostusparamtp(void)
{
	nollaa();
}
tulostusparamtp::~tulostusparamtp(void)
{

}

void tulostusparamtp::operator=(tulostusparamtp& param)
{
	memcpy(this, &param, (char *)&sarjalista-(char *)this);
	memset(&sarjalista, 0, sizeof(tulostusparamtp) - ((char *)&sarjalista-(char *)this));
}

void tulostusparamtp::nollaa(void)
{
	memset(this, 0, sizeof(tulostusparamtp));
	tulostettava = L'T';
	vaihelkm = 1;
	sj_raja = 3;
	enssija = 1;
	viimsija = 5;
	erotin = L';';
	p_fld = prtflds;
	n_fld = n_prtflds;
	tulmuot = tulosmuot;
	wcscpy(muottied, L"tulosmuot.xsl");
	for (int i = 0; i < 9; i++)
		automaali[i] = 1;
}

static void hae_kenttajarj(FldFrmtTp *flds, int *jarj, int *njrj)
{
	int n = 0, fldarr[300][2];

	memset(fldarr, 0, sizeof(fldarr));
	for (int i = 0; i < 30; i++) {
		if (i > 0 && flds[i].id == 0 && flds[i].name[0] == 0)
			break;
		if (flds[i].len > 0) {
			fldarr[n][0] = flds[i].pos;
			fldarr[n][1] = i;
			n++;
			}
		}
	if (n) {
		qsortint(fldarr, n, 8, 0, 1);
		if (*njrj > n)
			*njrj = n;
		for (int i = 0; i < *njrj; i++)
			jarj[i] = fldarr[i][1];
		}
}

void tulostusparamtp::setActFlds(int srj)
{
	int k = 0, nv = 0;
	FldFrmtTp *fld = (FldFrmtTp *)p_fld;
	int n = 0, (*fldarr)[2];
	int maxafld = 50+2*kilpparam.valuku;

	fldarr = new int[maxafld][2];
	if (vaihelkm > 2) {
		// Hiihdon sprintin erikoistapaus otetaan huomioon
		nv = ((vaihelkm > 2 && k_pv == 0) ? (srj < 0 ? kilpparam.n_pv_akt : Sarjat[srj].viimpv+1) : k_pv+1);
		if (tulostettava == L'P')
			nv++;
		}
	else if (kaikkiva) {
		nv = (srj < 0 ? pvparam[k_pv].valuku : Sarjat[srj].valuku[k_pv]) +
			(kilpparam.alalaji == L'D' ? 0 : (optiot & 16)/16);
		}
	else if (tulostettava == L'Y') {
		for (wchar_t *p = pistelista; *p > 0; p++)
			if (*p <= (srj < 0 ? pvparam[k_pv].valuku : Sarjat[srj].valuku[k_pv]) + 2 ||
				*p == kilpparam.valuku + 3)
				nv++;
		}
	else
		nv = 1;
	memset(fldarr, 0, sizeof(fldarr));
	for (int i = 0; i < maxafld; i++) {
		if (i > 0 && fld[i].id == 0 && fld[i].name[0] == 0)
			break;
		if (fld[i].len > 0) {
			int nj = 1;
			if ((tulostettava == L'P' && vaihelkm > 2) && fld[i].id >= F_TLS  &&
				fld[i].id != F_ERO && fld[i].id != F_OSATLS)
				continue;
			if ((piilotasijat && (fld[i].id == F_SJ || fld[i].id == F_OSASJ)) ||
				(piilotatulos && (fld[i].id == F_TLS || fld[i].id == F_OSATLS || fld[i].id == F_ERO)))
				continue;
			if (fld[i].id == F_OSATLS || fld[i].id == F_OSASJ) {
				if (vaihelkm == 1 &&
					((!kaikkiva && tulostettava != L'Y') || (fld[i].id == F_OSASJ && !kaikkisijat)))
					continue;
				nj = nv;
				}
			if ((fld == fileflds || fld == mobilflds) && fld[i].id == F_ERO &&
				vaihelkm >= 2 && !(tulostettava == L'P' && vaihelkm > 2))
				continue;
			for (int j = 0; j < nj; j++) {
				fldarr[n][0] = fld[i].pos + j * fld[i].askel;
				fldarr[n][1] = i + 1000 * j;
				n++;
				}
			}
		}
	if (n) {
		qsortint(fldarr, n, 8, 0, 1);
		}

	if (aFld)
		delete[] aFld;
	aFld = new actFldTp[n+1];
	memset(aFld, 0, (n+1)*sizeof(aFld));
	n_aFlds = 0;
	totW = 0;
	for (int k = 0; k < n; k++) {
		aFld[k].id = fldarr[k][1] % 1000;

		// Yhden vaiheen pistetulosteessa pisteet kirjoitetaan tuloksen paikalle
		if (tulostettava == L'P' && vaihelkm == 1 && aFld[k].id == F_TLS)
			aFld[k].id = F_PIST;

		aFld[k].race = k_pv;
		aFld[k].point = 0;
		if ((((tulostettava == L'Y' || kaikkiva) && nv > 0) || nv > 1) &&
			aFld[k].id == F_OSATLS || aFld[k].id == F_OSASJ) {
			int ii = fldarr[k][1] / 1000;
			if (vaihelkm > 2) {
				aFld[k].race = ii;
/*
				if (tulostettava == L'P') {
					if (wcswcind(kilpparam.kilplaji, L"NP") >= 5) {
						for (int is = 0; is <= k_pv; is++) {
							if (is >= Sarjat[*srj].enspv)
								swprintf(prs+wcslen(prs), L" Dspl-%d", is+1);
							else
								swprintf(prs+wcslen(prs), L"       ");
							}
						swprintf(prs+wcslen(prs), L"   %5.5s\n\n", L"Sum");
						tulprm->writehtml(prs);
					}
				else
*/
					{
					if (ii > k_pv) { {
						wcsncpy(aFld[k].title, language ? L"Total" : L"Yht.", sizeof(aFld[k].title)/2-1);
						aFld[k].race = k_pv;
						aFld[k].point = kilpparam.valuku + 1;
						}
					}
					else {
						wcsncpy(aFld[k].title, fld[aFld[k].id].ots, sizeof(aFld[k].title)/2-1);
						if (isdigit(aFld[k].title[wcslen(aFld[k].title)-1]))
							aFld[k].title[wcslen(aFld[k].title)-1] = 0;
						if (aFld[k].title[wcslen(aFld[k].title)-1] >= L'A')
							wcscat(aFld[k].title, L" ");
						_itow(ii+1, aFld[k].title+wcslen(aFld[k].title), 10);
						}
					}
				}
			else if (kaikkiva) {
				aFld[k].point = ii + 1;
				if (srj >= 0)
					wcsncpy(aFld[k].title, Sarjat[srj].va_matka[k_pv][ii], sizeof(aFld[k].title)/2-1);
				}
			else if (tulostettava == L'Y') {
				int j = 0;
				for (wchar_t *p = pistelista; *p > 0; p++) {
					if (*p <= (srj < 0 ? pvparam[k_pv].valuku : Sarjat[srj].valuku[k_pv]) + 2 || *p == kilpparam.valuku + 3) {
						if (j == ii) {
							aFld[k].point = *p - 2;
							if (*p == 1)
								wcsncpy(aFld[k].title, language ? L"Start" : L"L‰htˆ", sizeof(aFld[k].title)/2-1);
							if (*p == 2)
								wcsncpy(aFld[k].title, language ? L"Time" : L"Tulos", sizeof(aFld[k].title)/2-1);
							if (*p == kilpparam.valuku + 3)
								wcsncpy(aFld[k].title, language ? L"Tot" : L"Yht.", sizeof(aFld[k].title)/2-1);
							if (*p > 2 && srj >= 0 && *p <= Sarjat[srj].valuku[k_pv] + 2)
								wcsncpy(aFld[k].title, Sarjat[srj].va_matka[k_pv][*p-3], sizeof(aFld[k].title)/2-1);
							break;
							}
						j++;
						}
					}
				}
			}
		else if (vaihelkm == 2 && !yhttuljarj && (aFld[k].id == F_OSATLS || aFld[k].id == F_OSASJ)) {
			wcsncpy(aFld[k].title, fld[aFld[k].id].ots, sizeof(aFld[k].title)/2-1);
			if (isdigit(aFld[k].title[wcslen(aFld[k].title)-1]))
				aFld[k].title[wcslen(aFld[k].title)-1] = language ? L'T' : L'Y';
			}
		else
			wcsncpy(aFld[k].title, fld[aFld[k].id].ots, sizeof(aFld[k].title)/2-1);
		if (yhttuljarj && (aFld[k].id == F_TLS || aFld[k].id == F_SJ)) {
			aFld[k].race = k_pv;
			aFld[k].point = kilpparam.valuku + 1;
			}
		if (!yhttuljarj && vaihelkm == 1 && (aFld[k].id == F_TLS || aFld[k].id == F_ERO || aFld[k].id == F_SJ)) {
			aFld[k].race = k_pv;
			aFld[k].point = piste;
			if (aFld[k].point > 0 && aFld[k].id == F_TLS) {
				wcsncpy(aFld[k].title, Sarjat[srj].va_matka[k_pv][aFld[k].point-1], sizeof(aFld[k].title)/2-1);
				}
			}
		if (yhttuljarj && vaihelkm == 2 && (aFld[k].id == F_OSATLS || aFld[k].id == F_OSASJ)) {
			aFld[k].race = k_pv;
			}
		if (!yhttuljarj && vaihelkm == 2 && (aFld[k].id == F_TLS || aFld[k].id == F_SJ)) {
			aFld[k].race = k_pv;
			}
		if (!yhttuljarj && vaihelkm == 2 && (aFld[k].id == F_OSATLS || aFld[k].id == F_OSASJ)) {
			aFld[k].race = k_pv;
			aFld[k].point = kilpparam.valuku + 1;
			}
		aFld[k].pos = fldarr[k][0];
		aFld[k].len = fld[aFld[k].id].len;
		aFld[k].align = fld[aFld[k].id].oik;
		totW += aFld[k].len + 1;
		}
	n_aFlds = n;
	delete[] fldarr;
}

static wchar_t *AlisarjaOts(wchar_t *str, int buflen, tulostusparamtp *tulprm)
{
	wchar_t *SukupRaj;
	INT16 *IkaSrj;
	wmemset(str, 0, buflen);
	if ((tulprm->optiot & OPTALISRJ) == 0)
		return(str);
	SukupRaj = tulprm->sukuprajaus;
	do {
		IkaSrj = tulprm->ikasarjat;
		do {
			if ((int) wcslen(str) > buflen - 6)
				break;
			if (wcslen(str) > 0)
				wcscat(str, L", ");
			if (SukupRaj && *SukupRaj > L' ')
				str[wcslen(str)] = *SukupRaj;
			if (IkaSrj && *IkaSrj) {
				switch (*IkaSrj) {
					case 1:
						wcscat(str, L"YL");
						break;
					case 2:
						wcscat(str, L"AV");
						break;
					default:
						if (*IkaSrj)
							swprintf(str+wcslen(str), L"%2d", *IkaSrj);
					}
				}
			} while(IkaSrj && *++IkaSrj);
		} while(SukupRaj && *++SukupRaj);
	return(str);
}

wchar_t *StrAlisarja(wchar_t *str, kilptietue *kilp)
{
	wmemset(str, 0, 4);
	if (kilp->sukup > L' ')
		str[0] = kilp->sukup;
	if (kilp->ikasarja) {
		switch (kilp->ikasarja) {
			case 1:
				wcscat(str, L"YL");
				break;
			case 2:
				wcscat(str, L"AV");
				break;
			default:
				swprintf(str+wcslen(str), L"%2d", kilp->ikasarja);
			}
		}
	return(str);
}

bool OnAlisarja(kilptietue *kilp, tulostusparamtp *tulprm)
{
	if ((tulprm->optiot & OPTALISRJ) == 0)
		return(true);
	if (tulprm->sukuprajaus && tulprm->sukuprajaus[0] && wcswcind(kilp->sukup, tulprm->sukuprajaus) < 0)
		return(false);
	if (tulprm->ikasarjat && tulprm->ikasarjat[0] &&
		wcswcind((wchar_t) kilp->ikasarja, (wchar_t *)tulprm->ikasarjat) < 0)
		return(false);
	return(true);
}

wchar_t *htmlSrjFName(wchar_t *otsFName, int srj)
	{
	static wchar_t fname[300], *p;

	memset(fname, 0, sizeof(fname));
	wcscpy(fname, otsFName);
	for (p = fname+wcslen(fname)-1; p > fname && *p != L'\\'; p--) ;
	if (p > fname)
		p[1] = 0;
	else
		fname[0] = 0;
	wcscat(fname, Sarjat[srj].sarjanimi);
	wcscat(fname, L".html");
	return(fname);
	}

static INT saksum(kilptietue *kilp, INT i_pv)
   {
   INT i, sak = 0, isak;

	for (i=0; kilp->pv[i_pv].asakot[i]; i++)
	  if ((isak = kilp->pv[i_pv].asakot[i] - L'0') >= 0 && isak <= kilpparam.maxasakko)
		 sak += isak;
   return(sak);
   }

wchar_t *kilptietue::tarkstr(wchar_t *st, int i_pv, bool yhttul, int len, int lang)
{
	static wchar_t *strs[2][11] = {{L"Tark.", L"Avoin", L"Hyl.es.", L"Ohit.", L"Puuttuu",
		L"Kesk.", L"Virhe", L"Hyl.", L"Ei l‰ht.", L"Poissa", L"Ei aikaa"},
		{L"OK", L"Open", L"Recheck", L"Skip", L"DNF", L"DNF", L"Missing", L"DQ", L"DNS", L"Absent", L"Exclude"}};

	if (i_pv < 0)
		i_pv = k_pv;
	memset(st, 0, sizeof(st));
	if (yhttul) {
		if (!y_hyv(i_pv)) {
			if (len == 1) {
				st[0] = L'-';
				}
			else
				wcsncpy(st, L"------", len);
			}
		}
	else {
		if (wcswcind(kilpparam.kilplaji, L"SB") < 0 && hyv() && (k_pv > 0 || i_pv == k_pv)) {
			if (toupper(pv[k_pv].qual) >= 'Q' && toupper(pv[k_pv].qual) <= 'Z')
				st[0] = (wchar_t)pv[k_pv].qual;
			}
		else if (len == 1) {
			st[0] = tark(i_pv);
			}
		else {
			switch (tark(i_pv)) {
				case L'T' :
					wcsncpy(st, strs[lang][0], len);
					break;
				case L'-' :
					wcsncpy(st, strs[lang][1], len);
					break;
				case L'I' :
					wcsncpy(st, strs[lang][2], len);
					break;
				case L'O' :
					wcsncpy(st, strs[lang][3], len);
					break;
				case L'K' :
					wcsncpy(st, (Sarjat[Sarja(i_pv)].rrtas == 1) ? strs[lang][4] : strs[lang][5], len);
					break;
				case L'H' :
					wcsncpy(st, (Sarjat[Sarja(i_pv)].rrtas == 1) ? strs[lang][6] : strs[lang][7], len);
					break;
				case L'M' :
					wcsncpy(st, strs[lang][6], len);
					break;
				case L'X' :
					wcsncpy(st, strs[lang][10], len);
					break;
				case L'V':
				case L'E' :
				case L'B' :
					wcsncpy(st, strs[lang][8], len);
					break;
				case L'P' :
					wcsncpy(st, strs[lang][9], len);
					break;
				}
			}
		}
	return(st);
}

static wchar_t *asuunnStr(wchar_t *str, int len, kilptietue *kilp, int i_pv, int piste, int lang)
{
	static wchar_t st[40];
	static wchar_t *strs[2][3] = {{L"A", L"K", L"X"}, {L"S", L"L", L"P"}};
	int s1 = 0, s2 = 0;

	if (str == NULL) {
		str = st;
		if (len > sizeof(st)/2-1)
			len = sizeof(st)/2-1;
		}
	str[0] = 0;
	if (piste <= 0)
		piste = 4;
	for (int i = 0; i <= piste; i++) {
		switch (Sarjat[kilp->Sarja(i_pv)].asuunnlaji[i_pv][i]) {
			case L'S':
				s1 = 0;
				s2 = 0;
				if (str[0])
					wcscat(str, L",");
				if ((int) wcslen(str) + 4 < len) {
					if (kilp->pv[i_pv].asakot[0] >= L'0')
						s1 = kilp->pv[i_pv].asakot[0] - L'0';
					s2 = kilp->pv[i_pv].asakot[1] - L'0';
					if (len < 15 || s2 < 0) {
						if (s2 > 0)
							s1 += s2;
						swprintf(str+wcslen(str), L"%s:%d", strs[lang][0], s1);
						}
					else {
						if (s2 < 0)
							s2 = 0;
						swprintf(str+wcslen(str), L"%s:%d+%d", strs[lang][0], s1, s2);
						}
					}
				break;
			case L'L':
				s1 = 0;
				if (str[0])
					wcscat(str, L",");
				if ((int) wcslen(str) + 5 < len) {
					if (kilp->pv[i_pv].asakot[3] > L'0')
						s1 += kilp->pv[i_pv].asakot[3] - L'0';
					swprintf(str+wcslen(str), L"%s:%d", strs[lang][1], s1);
					}
				break;
			}
		}
	s1 = 0;
	if (kilp->pv[i_pv].asakot[2] > L'0')
		s1 += kilp->pv[i_pv].asakot[2] - L'0';
	if (kilp->pv[i_pv].asakot[4] > L'0')
		s1 += kilp->pv[i_pv].asakot[4] - L'0';
	if (s1 > 0){
		if (str[0])
			wcscat(str, L",");
		if ((int)wcslen(str) + 4 <= len) {
			wchar_t fmt[10];
			swprintf(fmt, L"%s%dd", L"%s:%.", len - wcslen(str) - 3);
			swprintf(str+wcslen(str), fmt, strs[lang][2], s1);
			}
		}
	return(str);
}

wchar_t *kilptietue::strval(tulostusparamtp *tulprm, wchar_t *st, int len, int knt, int i_pv, int pst, int sj, int opt)
{
	int tls, pst1;

	memset(st, 0, len+1);
	switch (knt) {
		case F_SJ:
			if (((opt & 8) && tulprm->yhttuljarj && pisteet[0] != 0) || hyv()) {
				if (!(opt & 1) && sj)
					_itow(sj, st, 10);
				else if ((opt & 1) && p_sija(pst))
					_itow(p_sija(pst), st, 10);
				if ((opt & 2) && st[0] != 0)
					wcscat(st, L".");
				}
			return(st);
		case F_KNO:
			_itow(id(), st, 10);
			return(st);
		case F_NIMI:
			return(nimi(st, len, opt != 0));
		case F_ETUNIMI:
			wcsncpy(st, etunimi, len);
			return(st);
		case F_SUKUNIMI:
			wcsncpy(st, sukunimi, len);
			return(st);
		case F_ARVO:
			wcsncpy(st, arvo, len);
			elimwbl(st);
			return(st);
		case F_LISNO:
			if (lisno[0])
				_itow(lisno[0], st, 10);
			return(st);
		case F_LISNO2:
			if (lisno[1]) {
				_itow(lisno[1], st, 10);
				if (kilpparam.kvlisno > 0) {
					int len = wcslen(st);
					if (len < kilpparam.kvlisno) {
						wmemmove(st + kilpparam.kvlisno - len, st, len+1);
						wmemcpy(st, L"0000000000", kilpparam.kvlisno - len);
						}
					wmemmove(st + 3, st,  wcslen(st)+1);
					wmemcpy(st, L"FIN", 3);
					if (wcslen(maa) == 3)
					wmemcpy(st, maa, 3);
					}
				}
			return(st);
		case F_BADGE:
			if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
				break;
			if (pv[i_pv].badge[0])
				_itow(pv[i_pv].badge[0], st, 10);
			return(st);
		case F_SRA:
			wcsncpy(st, seura, len);
			return(st);
		case F_SRALYH:
			wcsncpy(st, seuralyh, len);
			return(st);
		case F_MAA:
			wcsncpy(st, maa, len);
			return(st);
		case F_YHD:
			wcsncpy(st, yhdistys, len);
			return(st);
		case F_JOUK:
			wcsncpy(st, joukkue, len);
			return(st);
		case F_ALISRJ:
			if (sukup > L' ')
				st[0] = sukup;
			if (ikasarja > 0) {
				switch (ikasarja) {
					case 1:
						wcscat(st, L"YL");
						break;
					case 2:
						wcscat(st, L"AV");
						break;
					default:
						swprintf(st+wcslen(st), L"%02d", ikasarja);
					}
				}
			return(st);
		case F_TLS:
			switch (opt & 3) {
				case 0:
					if (hyv() && (tls = p_aika(pst)) != 0) {
						if (opt & 16 && pst > 1) {
							int tls1 = p_aika(pst-1);
							if (tls1 > 0 && tls1 < tls)
								tls -= tls1;
							else
								tls = 0;
							}
						aikatowstr_cols_n(st, tls, 0, L',', pvparam[k_pv].laika2);
						}
					else
						if (!pst)
							tarkstr(st, i_pv, false, len, tulprm->language);
					break;
				case 1:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (y_hyv(i_pv) && tulos2(pst)) {
						aikatowstr_cols_n(st, tulos2(pst), 0, L',', pvparam[k_pv].laika2);
						}
					else
						if (!pst)
							tarkstr(st, i_pv, true, len, tulprm->language);
					break;
				case 2:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (y_hyv(i_pv) && ytulos(i_pv)) {
						ytulos_wstr_cols_n(st, i_pv, tulprm->tulmuot.des_sep, tulprm->tulmuot.pistedesim,
							tulprm->ypJrj, pvparam[i_pv].laika2);
						}
					else
						if (!pst)
							tarkstr(st, i_pv, true, len, tulprm->language);
					break;
					}
			elimwzb1(st);
			return(st);
		case F_ERO:
			switch (opt & 7) {
				case 0:
					if (hyv() && p_aika(pst) > opt / 8) {
						aikatowstr_cols_n(st+1, p_aika(pst) - opt/8, 0, L',', pvparam[k_pv].laika2);
						elimwzb1(st+1);
						st[0] = L'+';
						}
					break;
				case 1:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (y_hyv(i_pv) && tulos2(pst) > opt / 8) {
						aikatowstr_cols_n(st+1, tulos2(pst) - opt/8, 0, L',', pvparam[k_pv].laika2);
						elimwzb1(st+1);
						st[0] = L'+';
						}
					break;
				case 2:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (y_hyv(i_pv) && ytulos(i_pv) > opt / 8) {
						aikatowstr_cols_n(st+1, ytulos(i_pv) - opt/8, 0, L',', pvparam[i_pv].laika2);
						elimwzb1(st+1);
						st[0] = L'+';
						}
					break;
				}
			return(st);
		case F_SAK:
			if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
				break;
			if (kilpparam.kilplaji == L'B') {
				asuunnStr(st, len, this, i_pv, pst, tulprm->language);
				}
			else if (kilpparam.lsakmax > 0) {
				if (i_pv) {
					switch (opt) {
						case 0:
							swprintf(st, L"(%s)", asakkoStr(i_pv));
							break;
						case 1:
							swprintf(st, L"(%d)", saksum(this, 1));
							break;
						case 2:
							swprintf(st, L"(%d+%d)", saksum(this,0), saksum(this,1));
							break;
						}
					}
				else
					swprintf(st, L"(%s)", asakkoStr(i_pv));
				}
			else if (opt && pv[i_pv].tasoitus) {
				 wchar_t *saktxt[MAXLANG][3] = {{L"sakko", L"tasoitus"}, {L"penalty", L"handicap"}};
				 int k = Sarjat[Sarja(i_pv)].tsak[i_pv];
				 if (k == 0)
					k = kilpparam.sakkoyks ? kilpparam.sakkoyks : 1;
				if (opt > 7)
				swprintf(st, L"%s %d", pv[i_pv].tasoitus > 0 ? saktxt[tulprm->language][0] : saktxt[tulprm->language][1],
					abs(pv[i_pv].tasoitus/k));
				else
					swprintf(st, L"%d", abs(pv[i_pv].tasoitus/k));
				}
			return(st);
		case F_TARK:
			if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
				break;
			tarkstr(st, i_pv, (opt & 2) != 0, len, tulprm->language);
			return(st);
		case F_AIKA:
			if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
				break;
			if (hyv_pv(i_pv) && p_aika(pst)) {
				aikatowstr_cols_n(st, p_aika(pst)-p_sakko(pst), 0, L',', pvparam[i_pv].laika2);
				elimwzb1(st);
				}
			return(st);
		case F_PIST:
			if (!(opt & 4) && pisteet[opt & 3])
				_itow(pisteet[opt], st, 10);
			if ((opt & 4) && pv[i_pv].pvpisteet[opt & 1] != karkikierr) {
				if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
					break;
				_itow(pv[i_pv].pvpisteet[opt & 1]-karkikierr, st, 10);
				}
			if (tulprm->tulmuot.pistedesim) {
				insdes(st, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
				}
			return(st);
		case F_OSATLS:
			if (((opt & 3) == 0 && pst == 0) || (pst > kilpparam.valuku && (opt & 16) == 0)) {
				if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
					break;
				if (y_hyv(i_pv) && ytulos(i_pv)) {
					ytulos_wstr_cols_n(st, i_pv, tulprm->tulmuot.des_sep, tulprm->tulmuot.pistedesim,
						tulprm->ypJrj, pvparam[i_pv].laika2);
					elimwzb1(st);
					}
				else if (!pst)
					tarkstr(st, i_pv, true, len, tulprm->language);
				}
			else if ((opt & 3) == 1 && pst == 0) {
				if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
					break;
				if (tulos_pv(i_pv, true)) {
					aikatowstr_cols_n(st, tulos_pv(i_pv, true), 0, L',', pvparam[i_pv].laika2);
					elimwzb1(st);
					}
				else if (!pst) {
					if (k_pv != 0 || tulprm->vaihelkm != 3 || wcswcind(tark(i_pv), L"-VP") < 0)
						tarkstr(st, i_pv, false, len, tulprm->language);
					}
				}
			else {
				pst1 = pst;
				if (pst > Sarjat[Sarja()].valuku[k_pv])
					pst1 = 0;
				if ((hyv() || (tulprm->optiot2 & 0x01000000)) && (tls = p_aika(pst1)) != 0) {
					if ((opt & 16) && pst > 1) {
						int tls1 = p_aika(pst-1);
						if (tls1 > 0 && tls1 < tls)
							tls -= tls1;
						else if (tls1 > 0 && tls1 > tls + 20 * TUNTI)
							tls += 24*TUNTI - tls1;
						else
							tls = 0;
						}
					aikatowstr_cols_n(st, tls, 0, L',', pvparam[k_pv].laika2);
					elimwzb1(st);
					}
				else if (!pst)
					tarkstr(st, i_pv, false, len, tulprm->language);
				}
			return(st);
		case F_OSASJ:
			switch (opt & 3) {
				case 0:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (y_hyv(i_pv) && y_sija(i_pv)) {
						if (Sarjat[Sarja(i_pv)].pisteytulos() && tulprm->ypJrj) {
							int ii = findYp(tulprm->ypJrj, id());
							if (ii >= 0)
								_itow(tulprm->ypJrj[ii][2], st, 10);
							}
						else
							_itow(y_sija(i_pv), st, 10);
						if (opt & 4 && st[0] != 0)
							wcscat(st, L".");
						}
					break;
				case 1:
					if (Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
						break;
					if (hyv_pv(i_pv) && sija(i_pv)) {
						if (k_pv == 0 && tulprm->vaihelkm == 3) {
							swprintf(st, L"%s: %d", Sarjat[Sarja(i_pv)].sarjanimi, sija(i_pv));
							}
						else
							_itow(sija(i_pv), st, 10);
						if (opt & 4 && st[0] != 0)
							wcscat(st, L".");
						}
					else if (k_pv == 0 && tulprm->vaihelkm == 3 && wcswcind(tark(i_pv), L"HKEBMX") >= 0)
						swprintf(st, L"%s", Sarjat[Sarja(i_pv)].sarjanimi);
					break;
				case 2:
					if (hyv() && p_sija(pst)) {
						_itow(p_sija(pst), st, 10);
						if (opt & 4 && st[0] != 0)
							wcscat(st, L".");
						}
					break;
				}
			return(st);
		}
	return(st);
}

void sivuTuloste(kilptietue *kilp)
{
	tulostusparamtp tulprm;
	wchar_t st[80];

	tulprm = sivutulostusparam;
	if (tulprm.lstf == NULL || tulprm.printer != GDIPRINTER)
		return;
	EnterCriticalSection(&print_CriticalSection);
	initpage(&tulprm, NULL);
	for (UINT i = 0; i < PageFieldCount; i++) {
		if (PageField[i].len == 0)
			continue;
		kilp->strval(&tulprm, st, PageField[i].len, PageField[i].id, k_pv, 0, kilp->sija(k_pv), 4096);
		switch (PageField[i].koko) {
			case 1:
//				paaots_on(&tulprm);
				break;
			case 2:
				aliots_on(&tulprm);
				break;
			case 3:
				paaots_on(&tulprm);
				break;
			}
		putwfldGDI(st, PageField[i].hpos, 0, PageField[i].vpos, tulprm.lstf);
		switch (PageField[i].koko) {
			case 1:
//				paaots_on(&tulprm);
				break;
			case 2:
				aliots_pois(&tulprm);
				break;
			case 3:
				paaots_pois(&tulprm);
				break;
			}
		}
	endpage(&tulprm);
	enddoc(&tulprm);
	LeaveCriticalSection(&print_CriticalSection);
}

void autostatus(int kynnys, int korkein, wchar_t rajaus, int viimsija);

void autotulostus(void)
   {
   INT  l,sv,srj,korkein,k,i,piste=0;
   tulostusparamtp tulprm;

   if( (! autofl) || ((! spoolfl) && tauko ) || kynnys < 1) return;
//   if (yvfl) yhteenveto(&tulprm);
//   yvfl = FALSE;
	l = 0;
   sv = 0;
   srj = -1;
   korkein = kynnys - 1;
   k = 0;
   tulprm = autotulostusparam;
   if (!tulprm.lstf)
		return;
   EnterCriticalSection(&autotul_CriticalSection);
   for ( i = 0 ; i < sarjaluku + nsarjayhd ; i++) {
	  if (Sarjat[i].vakanttisarja) {
		  prior[i] = 0;
		  continue;
		  }
	  k = max(k,prior[i]);
	  if( prior[i] > korkein ) {
		 srj = i;
			korkein = prior[i];
		 }
	  }
#ifdef _CONSOLE
	   if (status_on && !ajat_on)
		   vidint(15,75,5,k);
#else
		autostatus(kynnys, k, tulprm.rajaus, tulprm.viimsija);
#endif
	if (srj >= 0) {
	  EnterCriticalSection(&print_CriticalSection);
//	  tulostasarja(&srj, piste, L'P', laajuus, L'K', cda, cja, daya, nparas, aakk, &l, &sv,
//		 TRUE);
	  tulprm = autotulostusparam;
	  tulprm.piste = piste;
//	  tulprm.ixjarj = aakk;
	  if (k_pv == Sarjat[srj].enspv || Sarjat[srj].luonne[k_pv] == 0) {
		  tulprm.vaihelkm = 1;
		  tulprm.yhttuljarj = false;
		  tulprm.yhttulpaino = false;
		  }
	  initpage(&tulprm, NULL);

	  tulostasarja(&srj, &tulprm, &l, &sv, TRUE);
	  if (l > 0)
		 endpage(&tulprm);
	  enddoc(&tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
	  if( srj >= sarjaluku ) {
#ifdef _CONSOLE
		 wchar_t ch;
		 char ch2;

		 ch = L' ';
		 clrln(ySize-1),
		 wselectopt(L"Lopetetaanko automaattinen tulostus (K/E)", L"KE", &ch);
		 do {
			ch = readkbd(&ch2,TRUE,spoolfl);
			ch = towupper(ch);
			} while (ch != L'K' && ch != L'E');
		 if( ch == L'K' )
			autofl = FALSE;
		 else tauko = TRUE;
			clrln(ySize-1);
#endif
		 LeaveCriticalSection(&autotul_CriticalSection);
		 return;
		 }
	  }
   LeaveCriticalSection(&autotul_CriticalSection);
   }

void tulostaParhaat(int srj, int lkm)
{
	INT  l = 0,sv = 0;
	tulostusparamtp tulprm;

	if (srj < 0 || lkm <= 0)
		return;
	tulprm = lsttulparam;
	if (!tulprm.lstf || tulprm.printer != GDIPRINTER)
		return;

	tulprm.kohde = L'P';
	if (k_pv == 0 || Sarjat[srj].enspv == Sarjat[srj].viimpv ||
		Sarjat[srj].viimpv != k_pv || Sarjat[srj].luonne[k_pv] == 0) {
		tulprm.vaihelkm = 1;
		tulprm.yhttuljarj = false;
		tulprm.yhttulpaino = false;
		tulprm.piste = 0;
		}
	else {
		tulprm.vaihelkm = 2;
		tulprm.yhttuljarj = true;
		tulprm.yhttulpaino = true;
		tulprm.piste = 0;
		}
	tulprm.rajaus = L'P';
	tulprm.enssija = 0;
	tulprm.viimsija = lkm;
	EnterCriticalSection(&print_CriticalSection);
	initpage(&tulprm, NULL);

	tulostasarja(&srj, &tulprm, &l, &sv, TRUE);
	if (l > 0)
		endpage(&tulprm);
	enddoc(&tulprm);
	LeaveCriticalSection(&print_CriticalSection);
}

int sarjayhdfl;
INT *yhdseur[VALUKU+3];

int seuraava(int yhd, int piste, int p)
	{
	if (yhdseur[piste])
		return(yhdseur[piste][p]);
	return(JarjSeur(yhd, piste, p));
	}

int SeuraavaSrj(int srj, int piste, int p)
{
//	assert(srj < sarjaluku+MAXYHD);
	return(seuraava((srj < sarjaluku) ? 0 : srj - sarjaluku + 1, piste, p));
}

wchar_t *SarjaNimi(wchar_t *buf, int buflen, int srj, bool pitka, wchar_t *alisarjat)
{
	wchar_t *p;

	p = buf;
	if (pitka && Sarjat[srj].psarjanimi[0]) {
		if (p == NULL)
			p = Sarjat[srj].psarjanimi;
		else {
			wcsncpy(p, Sarjat[srj].psarjanimi, buflen-1);
			buf[buflen-1] = 0;
			}
		}
	else {
		if (p == NULL)
			p = Sarjat[srj].sarjanimi;
		else {
			wcsncpy(p, Sarjat[srj].sarjanimi, buflen-1);
			buf[buflen-1] = 0;
			}
		}
	if (alisarjat != NULL && buf != NULL && alisarjat[0])
		wcsncpy(buf+wcslen(buf), alisarjat, buflen-wcslen(buf)-1);
	return(p);
}

void puuttuvat(wchar_t *paika, INT *psija, INT srj, INT df)
   {
	UINT32 pa[2];
	INT32 tm;
	INT p, lj = 0, lj1 = 0, d;
	kilptietue kilp;

	if (k_pv > 0 && df == 0) {
		lj = kilpparam.valuku+5;
		lj1 = kilpparam.valuku+1;
		}
    tm = KSEK*T_TIME(biostime(0,0));
	if ((d = jalku[srj][lj+4]) >= 0) {
		kilp.GETREC(d);
        pa[0] = kilp.tulos1(lj1, tm);
        pa[1] = kilp.tulos1(0, tm);
		if (pa[1] > 0 && pa[1] < 12*TUNTI) {
			AIKATOWSTRS(paika, pa[0],0);
			paika[8] = 0;
			p = jalku[srj][lj];
//         if (!lj) pa = pa << 12;
			*psija = intsija(pa, &p, lj1, 0, NULL);
         }
      else {
		 wcscpy(paika, L"00.00.00");
		 *psija = 1;
		  }
      }
   else {
      wcscpy(paika, L"        ");
      *psija = 0;
      }
   }

void sijat(INT srj)
   {
   sijat2(srj, false, false);
   }

void sijat2(INT srj, bool tall, bool yhdarr)
   {
   int d, p, sj, sj1,piste, laji, yhd = 0, *sjj, *djj, n;
   INT32 tls, tls1;
   kilptietue kilp;

   if (srj >= sarjaluku+nsarjayhd)
	   return;
   if (srj >= sarjaluku) {
		yhd = srj-sarjaluku+1;
		}
//	for (piste = 0; piste <= kilpparam.valuku+(k_pv > 0); piste++) {
   sjj = new int[nilm[srj]];
   djj = new int[nilm[srj]];
   for (piste = 0; piste <= kilpparam.valuku + (k_pv > 0 || (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S')) ? 1 : 0; piste++) {
	  if (piste && piste > Sarjat[srj].valuku[k_pv] && piste != kilpparam.valuku+1)
		  continue;
//	  if (!tall && !yhd && !sjfl[srj] && (piste == 0 || piste == kilpparam.valuku+1))
//		  continue;
	  if (yhd && yhdarr)
			memset(yhdsija[yhd-1][piste], 0, (maxrec+1)*sizeof(yhdsija[0][0][0]));
	  sj = 1;
	  tls = -1;
	  laji = piste ? piste+4 : 0;
	  n = 0;
	  p = jalku[srj][laji];
	  while (p > 0) {
		 d = p;
		 kilp.GETREC(d);
		 tls1 = kilp.tulos2(piste, 2);
		 if (tls1 != tls) {
			sj1 = sj;
			tls = tls1;
			}
		 sjj[n] = sj1;
		 djj[n] = d;
		 n++;
		 sj++;
		 p = seuraava(yhd, piste, p);
		 while (p < 0 && piste == 0 && laji < 4) {
			 laji++;
			 p = jalku[srj][laji];
			 }
		 }
	  for (int i = 0; i < n; i++) {
		  kilp.GETREC(djj[i]);
		  if (yhdarr && yhd) {
			  yhdsija[yhd - 1][piste][djj[i]] = sjj[i];
			  }
		  else if (sjj[i] != kilp.p_sija(piste)) {
			  kilp.set_sija(piste, sjj[i]);
			  if (tall) {
				  kilp.tallenna(djj[i], -1, 0, 0, 0);
				  }
			  else
				  kilp.PUTREC(djj[i], 1);
			  }
		  }
      }
   delete[] djj;
   delete[] sjj;
   sjfl[srj] = FALSE;
   }

static void set_pos(tulostusparamtp *tulprm, INT pos, INT fill, INT dots)
   {
	wchar_t st[30],*p;
	int suhtfont = 0;

	switch (tulprm->printer) {
	  case LJETII :
		 sendwline(L"\x1b&a",tulprm->lstf);
			pos = 10 * (pos + tulprm->tulmuot.numlev*fill);
		 if (suhtfont)
			pos += 4*tulprm->tulmuot.numlev*dots;
		 _itow(pos, st, 10);
		 wcscat(st, L"H");
		 sendwline(st, tulprm->lstf);
		 break;
	  case PROPRIEMU :
		 st[0] = L'\r';
		 st[1] = L'\x1b';
		 st[2] = L'D';
		 st[3] = (wchar_t) pos;
		 st[4] = 0;
		 st[5] = 9;
			p = st+6;
		 for (;fill > 0; fill--) *(p++) = L' ';
		 sendwchars(st, (int)(p-st), tulprm->lstf);
         break;
	  }
   }

void putfldstr(wchar_t *rivi, wchar_t *str, INT pos, INT len, INT oik)
   {
   INT v, ll;

   ll = wcslen(str);
   ll = min(ll, len);
   if (ll <= 0) return;
   if (oik)
      v = pos + len - ll;
	else
	  v = pos;
   wmemcpy(rivi+v, str, ll);
   }

void putflddsp(wchar_t *str, INT pos, INT len, INT oik, INT r);

void putfld_int(tulostusparamtp *tulprm, int k, INT pos, INT len, INT oik, INT r)
   {
   wchar_t st[20];

   _itow(k, st, 10);
   putfld(tulprm, st, pos, len, oik, r);
   }

void putfld(tulostusparamtp *tulprm, wchar_t *str, INT pos, INT len, INT oik, INT r)
   {
   INT v, o, ll, i, dots, tabsent;
	wchar_t tabst[2] = L"\x9", *p;
	wchar_t *str2;

	if (len == 0 || wcslen(str) == 0)
		return;
	if (r) {
		putflddsp(str, pos, len, oik, r);
		return;
	   }
   str2 = (wchar_t *) calloc(wcslen(str)+1, 2);
   wcscpy(str2, str);
   str = str2;
	pos += tulprm->tulmuot.marg;
   ll = wcslen(str);
   ll = min(ll, len);
	if (ll <= 0) {
		free(str2);
		return;
		}
   if (ll == len) str[ll] = 0;
	if (tulprm->printer == GDIPRINTER) {
	  if (oik) pos += tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth*len;
		putwfldGDI(str, pos, oik, tulprm->vpos, tulprm->lstf);
		}
	else if (tulprm->printer == POSTSCRIPT) {
	  if (oik) pos += tulprm->tulmuot.numlev*len;
	  tulprm->sline[0] = L'(';
	  p = tulprm->sline+1;
	  while (*str) {
		 if (*str == L'(' || *str == L')') *(p++) = L'\\';
		 *(p++) = *(str++);
		 }
		*(p++) = L')';
	  *(p++) = L' ';
	  if (oik) {
		 *(p++) = L'w';
		 *(p++) = L' ';
		 }
	  _itow(pos, p, 10);
	  wcscat(p, oik ? L" o\r\n" : L" v\r\n");
	  sendwline(tulprm->sline,tulprm->lstf);
	  }
   else {
	  if (oik) {
		 o = pos + len - 1;
		 v = o - ll + 1;
		 }
	  else {
			v = pos;
		 o = v + ll - 1;
		 }
	   {
		 if (tulprm->printer == LJETII || tulprm->printer == PROPRIEMU) {
			dots = 0;
				if (oik && tulprm->printer == LJETII && tulprm->tulmuot.numlev > 2) {
			   for (p = str; *p; p++) {
				  if (*p == L'.' || *p == L',' || *p == L':' || *p == L' ')
					 dots++;
				  if (*p == L'+') dots--;
						}
			   }
			set_pos(tulprm, pos, v - pos, dots);
			sendwline(str, tulprm->lstf);
			}
		 else {
			if (tulprm->tulmuot.tabs) {
				tabsent = 0;
			   for (i = 0; i < n_prtflds; i++) {
				  if (pos <= lastpos) break;
				  if (lastpos < prtflds[i].pos+tulprm->tulmuot.marg) {
					 sendwline(tabst, tulprm->lstf);
					 lastpos = prtflds[i].pos+tulprm->tulmuot.marg;
					 tabsent = 1;
					 }
				  }
			   for (i = 0; i < v - pos; i++) sendwline(L" ",tulprm->lstf);
			   sendwline(str, tulprm->lstf);
			   }
			else wmemcpy(tulprm->sline+v, str, ll);
			}
		 }
	  }
   free(str2);
   }

void putcell(tulostusparamtp *tulprm, wchar_t *str, INT pos, INT len, INT oik, wchar_t *param, int cellfl)
{
	if (!cellfl)
		putfld(tulprm, str, pos, len, oik, 0);
	swprintf(tulprm->sline + wcslen(tulprm->sline), L"<td %s align=%s>%s</td>", param,
		oik ? L"right" : L"left", str);
}

void newline(tulostusparamtp *tulprm, INT n)
   {
	if (tulprm->printer == GDIPRINTER) {
		if (n <= 0)
			n = 1;
		tulprm->vpos += n*tulprm->lstf->u.wp.GDIparam.Currentfont.line;
		}
	else if (tulprm->printer == POSTSCRIPT) {
	  if (n <= 0) {
         n = -n;
		 sendwline(L"ov\r\n", tulprm->lstf);
		 }
	  while (n--) sendwline(L"rv\r\n", tulprm->lstf);
	  }
   else {
		while (n--) sendwline(L"\n", tulprm->lstf);
      }
   }

void enddoc(tulostusparamtp *tulprm)
   {
	if (tulprm->printer == GDIPRINTER) {
		enddocGDI(tulprm->lstf);
		inGDIdoc = 0;
		tulprm->vpos = 0;
		return;
		}
   if (tulprm->printer == POSTSCRIPT) {
	  sendwline(L"p\r\n\x04", tulprm->lstf);
	  }
   else if (tulprm->lstf->prno != -2) {
	  if (!lahetacr)
		 sendwline(L"\xC",tulprm->lstf);
	  else
		 sendwline(L"\xC\r",tulprm->lstf);
	  }
   }

void abortdoc(tulostusparamtp *tulprm)
   {
	if (tulprm->printer == GDIPRINTER) {
		abortdocGDI(tulprm->lstf);
		inGDIdoc = 0;
		tulprm->vpos = 0;
		return;
		}
   }


void endpage(tulostusparamtp *tulprm)
   {
	if (tulprm->printer == GDIPRINTER) {
		endpageGDI(tulprm->lstf);
		tulprm->vpos = 0;
		}
	else
		enddoc(tulprm);
   }

void initline(tulostusparamtp *tulprm)
   {
   if (tulprm->printer != POSTSCRIPT && tulprm->printer != GDIPRINTER) {
	  if ((tulprm->kohde == L'H' || tulprm->kohde == L'M') && tulprm->tulmuot.tauluhtml)
		  memset(tulprm->sline, 0, sizeof(tulprm->sline));
	  else if (!tulprm->tulmuot.tabs)
		  wmemset(tulprm->sline, L' ', sizeof(tulprm->sline)/2-1);
	  else
		  lastpos = 0;
	  }
   }

void endline(tulostusparamtp *tulprm, INT n)
   {
   wchar_t *p;

   if (tulprm->printer == POSTSCRIPT || tulprm->printer == GDIPRINTER) {
	  newline(tulprm, n+1);
	  }
   else {
	  if (n < 0) n = -n-1;
		if (tulprm->printer == LJETII || tulprm->printer == PROPRIEMU || tulprm->tulmuot.tabs) {
		 p = tulprm->sline-1;
		 }
	  else {
		 p = tulprm->sline + sizeof(tulprm->sline)/2 - 2*(4+n);
		 while (p > tulprm->sline && (!*p || *p == L' ')) p--;
		 }
	  n++;
		while (n--) {
			if (!(tulprm->lstf && tulprm->lstf->prno == -2))
				*++p = L'\r';
			*++p = L'\n';
			}
	  *++p = 0;
	  sendwline(tulprm->sline, tulprm->lstf);
	  }
   initline(tulprm);
	}

void tulostusparamtp::puts_f(wchar_t *line)
   {
   TextFl *tul_tied;

   if (lstf == NULL || lstf->prno != -2 || lstf->u.Fl == NULL)
		return;
   tul_tied = lstf->u.Fl;
   if (tul_tied->IsOpen() && tul_tied->WriteLine(line) == WEOF) {
		wchar_t msg[200];
		swprintf(msg, 199, L"Virhe tiedostoon %s kirjoitettaessa", tul_tied->FileName);
		writeerror_w(msg, 0);
		closeprfile(lstf);
		}
   }

void tulostusparamtp::writehtml(wchar_t *rivi)
   {
   puts_f(rivi);
/*
   INT err;
   TextFl *tul_tied;

	if (lstf == NULL || lstf->prno != -2 || lstf->u.Fl == NULL)
	   return;
	tul_tied = lstf->u.Fl;
	err = tul_tied->WriteLine(rivi);
	if (err == WEOF) {
		wchar_t msg[200];
		swprintf(msg, 199, L"Virhe html-tiedostoon %s kirjoitettaessa", tul_tied->FileName);
		writeerror_w(msg, 0);
		closeprfile(lstf);
		}
*/
   }

void tulostusparamtp::writeline(wchar_t *rv)
   {
   wchar_t *p, *rivi;
   TextFl *tul_tied;

	if (lstf == NULL || lstf->prno != -2 || lstf->u.Fl == NULL)
	   return;
	tul_tied = lstf->u.Fl;
   rivi = (wchar_t *) calloc(wcslen(rv)+3, 2);
   wcscpy(rivi, rv);
   p = rivi + wcslen(rivi) - 1;
   for (; p > rivi && *p == L' '; p--) ;
   *(++p) = L'\n';
   *(++p) = 0;
   if (tul_tied->IsOpen() && tul_tied->WriteLine(rivi) == WEOF) {
		wchar_t msg[200];
		swprintf(msg, 199, L"Virhe tiedostoon %s kirjoitettaessa", tul_tied->FileName);
		writeerror_w(msg, 0);
	  }
   free(rivi);
   TiedRivi++;
   }

void initgdiprinter(tulostusparamtp *tulprm)
	{
	tulprm->printer = GDIPRINTER;
	tulprm->tulmuot.marg = k_pv ? 30 : 50;
	selectfontGDI(tulprm->lstf, &DefFnt);
	CurrFnt = DefFnt;
	tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
	}

void init_ps(tulostusparamtp *tulprm)
   {
   unsigned len,n;
   wchar_t  *p;

   if (psinit && (len = *(UINT16 *) psinit) != 0) {
	  sendwline(L"\x04",tulprm->lstf);
	  p = psinit+2;
	  while (len) {
		 n = min(len, sizeof(tulprm->sline)/2-1);
		 wmemcpy(tulprm->sline, p, n);
		 tulprm->sline[n] = 0;
		 sendwline(tulprm->sline,tulprm->lstf);
		 len -= n;
		 p += n;
		 }
	  }
   }

void initfont(tulostusparamtp *tulprm, const wchar_t *fnt)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &DefFnt);
		CurrFnt = DefFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
	if (tulprm->printer > 0 && fnt[0])
		sendwchars((wchar_t *)(fnt+1), fnt[0], tulprm->lstf);
	}

void bold_on(tulostusparamtp *tulprm)
	{
	static GDIfontTp BoldFnt;

	if (tulprm->printer == GDIPRINTER) {
		if ((BoldFnt.fontfamily != CurrFnt.fontfamily ||
			BoldFnt.pitch != CurrFnt.pitch ||
			BoldFnt.size != CurrFnt.size)
			&& !CurrFnt.bold) {
			BoldFnt = CurrFnt;
			BoldFnt.bold = 1;
			BoldFnt.hfont = NULL;
			}
		selectfontGDI(tulprm->lstf, &BoldFnt);
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		}
	}

void bold_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &CurrFnt);
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		}
	}

void paaots_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &TitleFnt);
		CurrFnt = TitleFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && pots_on[0])
		sendwchars(pots_on+1, pots_on[0], tulprm->lstf);
	}

void paaots_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &DefFnt);
		CurrFnt = DefFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && pots_pois[0])
	  sendwchars(pots_pois+1, pots_pois[0], tulprm->lstf);
	}

void rastivaots_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &RastiVaHeaderFnt);
		CurrFnt = RastiVaHeaderFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && pots_on[0])
		sendwchars(pots_on+1, pots_on[0], tulprm->lstf);
	}

void rastiva_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &RastiVaFnt);
		CurrFnt = RastiVaFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && pots_pois[0])
	  sendwchars(pots_pois+1, pots_pois[0], tulprm->lstf);
	}

void rastiva_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &DefFnt);
		CurrFnt = DefFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && pots_pois[0])
	  sendwchars(pots_pois+1, pots_pois[0], tulprm->lstf);
	}

void aliots_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &HeaderFnt);
		CurrFnt = HeaderFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
	if (tulprm->printer > 0 && aots_on[0])
	  sendwchars(aots_on+1, aots_on[0], tulprm->lstf);
	}

void aliots_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &DefFnt);
		CurrFnt = DefFnt;
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		return;
		}
   if (tulprm->printer > 0 && aots_pois[0])
	  sendwchars(aots_pois+1, aots_pois[0], tulprm->lstf);
	}

int initdoc(tulostusparamtp *tulprm, const wchar_t *fnt)
   {

   if (tulprm->printer == GDIPRINTER) {
		if (inGDIdoc)
			return(1);
		inGDIdoc = 1;
		startdocGDI(tulprm->lstf);
		}
   return (0);
}

#if defined(_BORLANDC_)
#pragma warn -par
#endif
int initpage(tulostusparamtp *tulprm, const wchar_t *fnt)
	{
#ifdef MATRTABS
   wchar_t st[30], *p;
   INT i;
#endif

   switch (tulprm->printer) {
		case GDIPRINTER:
			if (!inGDIdoc) {
				if (initdoc(tulprm, fnt))
					return(1);
				}
			startpageGDI(tulprm->lstf);
			if (fnt && fnt[0] == L'C') {
				selectfontGDI(tulprm->lstf, &CourFnt);
				CurrFnt = CourFnt;
				}
			else {
				selectfontGDI(tulprm->lstf, &DefFnt);
				CurrFnt = DefFnt;
				}
			if (tulprm->tulmuot.sivpit > tulprm->lstf->u.wp.GDIparam.Currentfont.lnheight)
				tulprm->tulmuot.sivpit = tulprm->lstf->u.wp.GDIparam.Currentfont.lnheight;
			if (tulprm->tulmuot.ymarg > 30)
				tulprm->vpos = tulprm->tulmuot.ymarg - 30;
			tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
			return(0);
	  case POSTSCRIPT:
		 init_ps(tulprm);
		 sendwline(L"n\r\n", tulprm->lstf);
		 break;
	  case LJETII:
	  case PROPRIEMU:
		 initline(tulprm);
		break;
	  case TXTTAB:
/*
		 if (fld == NULL) fld = prtflds;
		 p_fld = fld;
		 n_prtflds = sizeof(prtflds)/sizeof(FLD)-1;
		 if (fld == prtfldsm)
			n_prtflds = sizeof(prtfldsm)/sizeof(FLD)-1;
		 if (fld == prtflds1)
			n_prtflds = sizeof(prtflds1)/sizeof(FLD)-1;
#ifdef MAKI
		 if (fld == prtfldsmk)
			n_fld = sizeof(prtfldsmk)/sizeof(FLD)-1;
		 if (fld == prtfldsmy)
			n_fld = sizeof(prtfldsmy)/sizeof(FLD)-1;
#endif

//			aktflds = fld;
*/
#ifdef MATRTABS
		 if (tabs) {
			memset(st,0,sizeof(st));
			st[0] = ESC;
			st[1] = L'D';
			p = st + 2;
			for (i = 0; i < n_prtflds; i++)
			   if (fld[i].pos+tulprm->tulmuot.marg) *(p++) = fld[i].pos+tulprm->tulmuot.marg;
			sendchars(st, wcslen(st)+1, lstf);
				}
#endif
		 initline(tulprm);
		 break;
	  case EPSON:
		 initline(tulprm);
		 break;
	  }
   if (!fnt) fnt = init_fnt;
   initfont(tulprm, fnt);
   return(0);
   }
#if defined(_BORLANDC_)
#pragma warn +par
#endif

void tulostusparamtp::Putfld_int(int k, INT pos, INT len, INT oik, INT r)
{
	putfld_int(this, k, pos, len, oik, r);
}

void tulostusparamtp::Putfld(wchar_t *str, INT pos, INT len, INT oik, INT r)
{
	putfld(this, str, pos, len, oik, r);
}

void tulostusparamtp::Putcell(wchar_t *str, INT pos, INT len, INT oik, wchar_t *param, int cellfl)
{
	putcell(this, str, pos, len, oik, param, cellfl);
}

void tulostusparamtp::Newline(INT n)
{
	newline(this, n);
}

void tulostusparamtp::Enddoc()
{
	enddoc(this);
}

void tulostusparamtp::Abortdoc()
{
	abortdoc(this);
}

void tulostusparamtp::Endpage()
{
	endpage(this);
}

void tulostusparamtp::Initline()
{
	initline(this);
}

void tulostusparamtp::Endline(INT n)
{
	endline(this, n);
}

void tulostusparamtp::Initfont(const wchar_t *fnt)
{
	initfont(this, fnt);
}

void tulostusparamtp::Bold_on(void)
{
	bold_on(this);
}

void tulostusparamtp::Bold_pois(void)
{
	bold_pois(this);
}

void tulostusparamtp::Paaots_on(void)
{
	paaots_on(this);
}

void tulostusparamtp::Paaots_pois(void)
{
	paaots_pois(this);
}

void tulostusparamtp::Aliots_on(void)
{
	aliots_on(this);
}

void tulostusparamtp::Aliots_pois(void)
{
	aliots_pois(this);
}

int tulostusparamtp::Initdoc(const wchar_t *fnt)
{
	return(initdoc(this, fnt));
}

int tulostusparamtp::Initpage(const wchar_t *fnt)
{
	return(initpage(this, fnt));
}

void ssl_otsikot(INT k_pv, tulostusparamtp *tulprm)
   {
   wchar_t st[60], sm[16], *p;
   INT srj, len;

   wcscpy(st, L"999999");
   wcsncpy(st+6, kilpailu, 47);
   st[53] = 0;
   if ((len = wcslen(st)) < 53) {
	  wmemset(st+len, L' ', 53-len);
	  }
   if (st[52] == L' ')
	  st[52] = L'0';
   tulprm->writeline(st);
   for (srj = 0; srj < sarjaluku; srj++) {
	  if (!nilm[srj])
		 continue;
		 wcscpy(sm, Sarjat[srj].matka[k_pv]);
		for (p = sm; *p; p++) {
			if (*p == L'.') *p = L',';
			if (p > sm && *p > L'9') {
				*p = 0;
				break;
				}
			}
		swprintf(st, L"999998%-10.10s%6.6s%5d%4d%4d%4d%4d       %3d",
			Sarjat[srj].sarjanimi, sm, Sarjat[srj].maksu, nilm[srj], ntulos[srj][0],
			nkesk[srj], nhyl[srj], srj+1);
		tulprm->writeline(st);
		}
	}

void wretulos(kilptietue *kilp, tulostusparamtp *tulprm)
	{
   wchar_t samasija, st[100], st1[40], aika[16];
   INT32 t;
   static int edsija = -1;

   samasija = (kilp->sija(k_pv) == edsija) ? L'=' : L' ';
   edsija = kilp->sija(k_pv);
   if (edsija == 1 && samasija == L' ') {
		swprintf(st, L"%s %s", kilpparam.kilpkoodi, Sarjat[kilp->Sarja()].sarjanimi);
		tulprm->writeline(st);
		}
   wcscpy(st1, L"     ");
   switch (kilp->tark(k_pv)) {
	  case L'-' :
	  case L'T' :
	  case L'I' :
		 if ((t = kilp->p_aika(0)) <= 0) return;
		 swprintf(aika, L"%3d:%02.2d", t / MINUUTTI, (t / SEK) % 60);
		 swprintf(st1, L"%4d%c", kilp->sija(k_pv), samasija);
		 break;
	  case L'K' :
	  case L'O' :
		 wcscpy(aika, L"   DNF");
		 break;
	  case L'H' :
		 wcscpy(aika, L"  DISQ");
		 break;
	  default :
		 return;
	  }
   memset(st, 0, sizeof(st));
   if (kilp->lisno[1] > 0 && kilp->maa[0] >= L'A')
	   swprintf(st, L"%3.3s%-4d    %5.5s %-25s %-25s %s", kilp->maa, kilp->lisno[1],
			st1, kilp->etunimi, kilp->sukunimi, aika);
   else
	   swprintf(st, L"       %3.3s %5.5s %-25s %-25s %s", kilp->maa,
			st1, kilp->etunimi, kilp->sukunimi, aika);
   tulprm->writeline(st);
   }

void ssltulos(kilptietue *kilp, tulostusparamtp *tulprm)
	{
	wchar_t st[60], st1[40], aika[16];
   INT32 t, lisno = 0;

   switch (kilp->tark(k_pv)) {
	  case L'-' :
	  case L'T' :
	  case L'I' :
		 if ((t = kilp->p_aika(0)) <= 0) return;
		 swprintf(aika, L"%02d%02d%02d", (int) (t / TUNTI),
			(int) ((t / MINUUTTI) % 60), (int) ((t / SEK) % 60));
		 break;
	  case L'K' :
	  case L'O' :
		 wcscpy(aika, L"KES   ");
		 break;
	  case L'H' :
		 wcscpy(aika, L"HYL   ");
		 break;
	  case L'M' :
		 wcscpy(aika, L"HYL   ");
		 break;
	  default :
		 return;
	  }
   if (kilp->lisno[0] < 1000000)
		lisno = kilp->lisno[0];
   swprintf(st, L"%06ld%-23.23s%-8.8s%-10.10s%-6.6s", lisno,
		kilp->nimi(st1, 38, false), kilp->seuratieto(0, true), Sarjat[kilp->Sarja()].sarjanimi, aika);
   tulprm->writeline(st);
   }

static void shltulos(kilptietue *kilp, int Sarja, tulostusparamtp *tulprm)
   {
   static wchar_t vak = L'K', srj = -1, kilptunnus[6] = L"", laji[3] = L"  ",
		stsrj[11], stmatka[22] = L"";
   wchar_t ch, st1[16], st2[16], st3[16], st[120], aika[16], msg[82];
   INT ika;
   INT32 t = -1;

#ifdef _CONSOLE
   memset(st, 0, sizeof(st));
   if (!kilptunnus[0]) {
	  }
   wcscpy(st, kilptunnus);
   st[wcslen(st)] = L';';
   if (srj == -1 || srj > kilp->Sarja()) {
	   wselectopt(L"K‰ytet‰‰nkˆ rankisarjoille vakionimi‰?", L"KE", &vak);
	   }
   if (srj != kilp->Sarja()) {
	  srj = kilp->Sarja();
	  if (vak != L'K') {
		  clrln(ySize-3);
		  wcsncpy(stsrj, Sarjat[kilp->Sarja()].sarjanimi, 10);
		  stsrj[10] = 0;
		  swprintf(msg, L"Anna sarjalle %s k‰ytett‰v‰ nimi", Sarjat[kilp->Sarja()].sarjanimi);
		  vidspwmsg(ySize-3,0,7,0,msg);
		  inputwstr(stsrj, 10, 42, ySize-3, L"\r", &ch, 0);
		  }
	  else {
		  wcscpy(stsrj, L"MYL");
		  wcscpy(laji, L"  ");
		  wcsncpy(st2, Sarjat[kilp->Sarja()].sarjanimi+wcslen(Sarjat[kilp->Sarja()].sarjanimi)-2,2);
		  st2[2] = 0;
		  ika = _wtoi(st2);
		  if (Sarjat[kilp->Sarja()].sarjanimi[0] == L'N' || Sarjat[kilp->Sarja()].sarjanimi[0] == L'D'
			 || Sarjat[kilp->Sarja()].sarjanimi[0] == L'W' || Sarjat[kilp->Sarja()].sarjanimi[0] == L'L') {
			 stsrj[0] = L'N';
			 if (ika >= 35) {
				wmemcpy(stsrj+1, L"35", 2);
				}
			 if (ika >= 45) {
				wmemcpy(stsrj+1, L"45", 2);
				}
			 }
		  else {
			 if (ika >= 35) {
				wmemcpy(stsrj+1, L"35", 2);
				}
			 if (ika >= 45) {
				wmemcpy(stsrj+1, L"45", 2);
				}
			 if (ika >= 55) {
				wmemcpy(stsrj+1, L"55", 2);
				}
			 if (ika >= 65) {
				wmemcpy(stsrj+1, L"65", 2);
				}
			 }
		  if (ika == 20) {
			 wmemcpy(stsrj+1, L"20", 2);
			 }
		  if (ika == 17 || ika == 18) {
			 wmemcpy(stsrj+1, L"18", 2);
			 }
		  if ((tulprm->optiot & 0x40000000) == 0) {
			  if (ika > 0 && (ika < 21 || ika > 22))
				 swprintf(stsrj+1, L"%02d", ika);
			  }
		  }
	  if (Sarjat[kilp->Sarja()].tapa[k_pv][0] >= L'P')
		  wcsncpy(laji, Sarjat[kilp->Sarja()].tapa[k_pv], 2);
	  wcsncpy(stmatka, Sarjat[kilp->Sarja()].matka[k_pv],sizeof(stmatka)/2-1);
	  if (stmatka[0] > 0) {
		  if (stmatka[wcslen(stmatka)-1] >= L'P') {
			 laji[k_pv] = stmatka[wcslen(stmatka)-1];
			 stmatka[wcslen(stmatka)-1] = 0;
			 }
		  if (k_pv == 0 && stmatka[0] > 0 && stmatka[wcslen(stmatka)-1] >= L'P') {
			 laji[1] = laji[0];
			 laji[0] = stmatka[wcslen(stmatka)-1];
			 stmatka[wcslen(stmatka)-1] = 0;
			 }
		 }
	  if (k_pv) {
		 if (laji[1] != L' ' && laji[1] != 0)
			laji[0] = L'P' + L'V' - laji[1];
		 }
	  if (laji[1] == L' ')
		  laji[1] = 0;
	  clrln(ySize-3);
	  swprintf(msg, L"Sarja: %10s. Korjaa tarvittaessa matka       ja tapa %s", stsrj, laji);
	  vidspwmsg(ySize-3,0,7,0,msg);
	  inputwstr(stmatka, 4, 45, ySize-3, L"\r", &ch, 0);
	  inputwstr(laji, 2, 60, ySize-3, L"\r", &ch, 0);
		}
	wcscat(st, stsrj);
	st[wcslen(st)] = L';';
	wcscpy(st1, L"0");
	switch (kilp->tark()) {
		case L'K' :
		case L'X' :
			t = 0;
			wcscpy(st3, L"DNF");
			break;
		case L'H' :
		case L'M' :
			t = 0;
			wcscpy(st3, L"DSQ");
			break;
		case L'E' :
		case L'B' :
			t = 0;
			wcscpy(st3, L"DNS");
			break;
		case L'O' :
			t = 0;
			wcscpy(st3, L"LAP");
			break;
		case L'-' :
		case L'T' :
			if (k_pv > Sarjat[kilp->Sarja()].enspv && kilpparam.alalaji != L'S' &&
				Sarjat[kilp->Sarja()].luonne[k_pv] > 0) {
				t = kilp->ytulos(k_pv, 2);
				_itow(kilp->y_sija(k_pv), st3, 10);
				}
			else {
				t = kilp->tulos2(0, 2);
				_itow(kilp->sija(0), st3, 10);
				}
		}
	wcscat(st, stmatka);
	st[wcslen(st)] = L';';
	if (wcslen(laji) == 1)
		wcscat(laji, L" ");
	wcscat(st, laji);
	st[wcslen(st)] = L';';
	if (kilp->lisno[0]) {
		_ltow(kilp->lisno[0], st2, 10);
		}
	else
		wcscpy(st2, L"ULK");
	wcscat(st, st2);
	st[wcslen(st)] = L';';
	wcscat(st, kilp->etunimi);
	st[wcslen(st)] = L' ';
	wcscat(st, kilp->sukunimi);
	st[wcslen(st)] = L';';
	wcscat(st, kilp->seuratieto(0, false));
	if (wcslen(st) == 0 && wcslen(kilp->maa) == 3 && wcscmp(kilp->maa, L"FIN"))
		wcscat(st, kilp->maa);
	st[wcslen(st)] = L';';
	if (t > 0) {
		AIKATOWSTRS(aika, t, 0);
		aika[10] = 0;
		elimwzb1(aika);
		}
	else {
		wcscpy(aika, L"0,0");
		}
	wcscat(st, aika);
	st[wcslen(st)] = L';';
	wcscat(st, st3);
	st[wcslen(st)] = L';';
	if ((tulprm->optiot & 0x40000000) == 0) {
		swprintf(st+wcslen(st), L"%d;", kilp->id());
		}
	if (t >= 0)
		tulprm->writeline(st);
#else
	wchar_t st4[80];

	wcscpy(st3, L"0");
	switch (kilp->tark()) {
		case L'K' :
		case L'X' :
			t = 0;
			wcscpy(st3, L"DNF");
			break;
		case L'H' :
		case L'M' :
			t = 0;
			wcscpy(st3, L"DSQ");
			break;
		case L'E' :
		case L'B' :
			t = 0;
			wcscpy(st3, L"DNS");
			break;
		case L'O' :
			t = 0;
			wcscpy(st3, L"LAP");
			break;
		case L'-' :
		case L'T' :
			if (k_pv > Sarjat[kilp->Sarja()].enspv && kilpparam.alalaji != L'S' &&
				Sarjat[kilp->Sarja()].luonne[k_pv] > 0) {
				t = kilp->ytulos(k_pv, 2);
				_itow(kilp->y_sija(k_pv), st3, 10);
				}
			else {
				t = kilp->tulos2(0, 2);
				_itow(kilp->p_sija(0), st3, 10);
				}
		}
	if (kilp->lisno[0]) {
		_ltow(kilp->lisno[0], st2, 10);
		}
	else
		wcscpy(st2, L"ULK");
	wcscpy(st4, kilp->seuratieto(0, false));
	if (wcslen(st4) == 0 && wcslen(kilp->maa) == 3 && wcscmp(kilp->maa, L"FIN"))
		wcscat(st4, kilp->maa);
	if (t > 0) {
		AIKATOWSTRS(aika, t, 0);
		aika[10] = 0;
		elimwzb1(aika);
		}
	else {
		wcscpy(aika, L"0,0");
		}
	swprintf(st, L"%s;%s;%s;%s;%s;%s %s;%s;%s;%s;%d;",
		TulosteForm->SHLkilpailu,
		TulosteForm->SHLSarja[Sarja],
		TulosteForm->SHLMatka[Sarja],
		TulosteForm->SHLTyyli[Sarja],
		st2, kilp->etunimi, kilp->sukunimi, st4, aika, st3, kilp->id());
	if (t >= 0)
		tulprm->writeline(st);
#endif
	}

void liittotulos(kilptietue *kilp, int srj, tulostusparamtp *tulprm)
	{
	if (ssltlsfl == 1)
		ssltulos(kilp, tulprm);
	if (ssltlsfl == 0)
		shltulos(kilp, srj, tulprm);
	if (ssltlsfl == 2)
		wretulos(kilp, tulprm);
	}

#if defined(MAKI)

static int tuomtulos(char *line, kilptietue *kilp, int kierros)
	{
	int i;
	char st1[20], st2[20];

	if (kilp->pituus[kierros] == 0 && tyylipisteet(kilp, kierros) == 0)
		return(0);
	swprintf(line, "%5.5sm (%5.5s)  ", format1(kilp->pituus[kierros], st1, 0, ','),
		format1(pituuspisteet(kilp, kierros), st2, 0, ','));
	if (tuomariluku > 0) {
		for (i = 0; i < tuomariluku; i++) {
			strcat(line, format1(tuomaripisteet(kilp, kierros, i), 0, 0, ','));
			strcat(line, " ");
			}
		strcat(line, "(");
		strcat(line, format1(tyylipisteet(kilp, kierros), 0, 0, ','));
		strcat(line, ")");
		}
	swprintf(line+wcslen(line), " %3d. %5.5s", kilp->vasija[k_pv][2*kierros+1],
		format1(hyppypisteet(kilp, kierros), 0, 0, ','));
	return(1);
	}

static void htmlmakitulos(kilptietue *kilp, int day, char cd, int piste, int sj)
	{
   char prs[200], st[120];
   char asy[13],as1[13],as2[13],asp1[13],asp2[13];
   static INT32 tkarki, edtls;
   char sjs[6];
   INT32 tls = 0;

   memset(sjs,0,6);
   memset(as2,0,sizeof(as2));
   memset(as1,0,sizeof(as1));
   memset(asp1,0,sizeof(asp1));
   memset(asp2,0,sizeof(asp2));
   memset(asy,0,sizeof(asy));
   memset(st,0,sizeof(st));

   if (hyv(kilp)) {
		if (piste == 0)
			tls = yhtpisteet(kilp, nkierros[kilp->Sarja()]);
		else
			tls = hyppypisteet(kilp, (piste-1)/2);
	  }

   if (sj == 1 || (sj && tls != edtls)) {
		_itow(sj, sjs, 10);
      }
	edtls = tls;
   if (tulosmuot.vaihdanimet) {
	  vaihdanm(kilp->nimi, st);
      }
   else
      wcscpy(st, kilp->nimi);
   if (tls) {
		if (!piste) {
			format1(tls, asy, -1, 0);
			format1(hyppypisteet(kilp, 1), as1, -1, 0);
			format1(hyppypisteet(kilp, 2), as2, -1, 0);
			format1(kilp->pituus[1], asp1, -1, 0);
			format1(kilp->pituus[2], asp2, -1, 0);
			}
		else {
			format1(kilp->pituus[(piste-1)/2], as1, -1, 0);
			format1(tls, asy, -1, 0);
			}
      }
   else {
	  switch (kilp->tark(k_pv)) {
		 case 'K' :
			wcscpy(asy, "Kesk.");
			break;
		 case 'O' :
			wcscpy(asy, "Ohit.");
			break;
		 case 'H' :
			wcscpy(asy, "Hyl. ");
			break;
		 case L'V':
		 case 'E' :
		 case 'B' :
			wcscpy(asy, "Ei l‰ht.");
			break;
		 }
		}
	if (piste == 0) {
		if (htmlpitkat)
			swprintf(prs, "%5s  %-26.26s  %-30.30s %3.3s  %5.5s  (%5.5sm/%5.5s, %5.5sm/%5.5s)\n",
				sjs, st, kilp->seuratieto(0, false), kilp->maa, asy, asp1, as1, asp2, as2);
		else
			swprintf(prs, "%5s  %-26.26s  %-10.10s %3.3s  %5.5s  (%5.5sm/%5.5s, %5.5sm/%5.5s)\n",
				sjs, st, kilp->seuratieto(0, true), kilp->maa, asy, asp1, as1, asp2, as2);
		}
	else {
		if (htmlpitkat)
			swprintf(prs, "%5s  %-26.26s  %-30.30s %3.3s  %5.5sm  %5.5s\n",
				sjs, st, kilp->seuratieto(0, false), kilp->maa, as1, asy);
		else
			swprintf(prs, "%5s  %-26.26s  %-10.10s %3.3s  %5.5sm  %5.5s\n",
				sjs, st, kilp->seuratieto(0, true), kilp->maa, as1, asy);
		}
   tulprm->writehtml(prs);
	if (prttuomaripisteet) {
		wcscpy(prs, "            ");
		if (piste) {
			if (tuomtulos(st, kilp, (piste-1)/2)) {
				strcat(prs, st);
				}
			}
		else {
			if (tuomtulos(st, kilp, 1)) {
				wcscpy(prs+wcslen(prs), st);
				}
			if (tuomtulos(st, kilp, 2)) {
				if (wcslen(st) > 15) {
					strcat(prs, "\n");
				   tulprm->writehtml(prs);
					}
				wcscpy(prs, "            ");
				strcat(prs, st);
				}
			}
		if (wcslen(st) > 15) {
			strcat(prs, "\n");
		   tulprm->writehtml(prs);
			}
		}
	}

static INT prtmakitulos(kilptietue *kilp, INT sj, INT piste)
   {
   char st[120], as[13], ase[13];
   char asy[13],as1[13],ss[6],ss2[6],ss1[6];
   static INT32 tkarki, edtls;
   char sjs[6];
   INT32 tls = 0;
   FldFrmtTp *fld;
   INT i, nr = 1;
#ifdef FISWC
   char *wcpoint[] =
	  {"100", "80", "60", "50", "45", "40", "36", "32", "29", "26",
        "24", "22", "20", "18", "16", "15", "14", "13", "12", "11",
        "10",  "9",  "8",  "7",  "6",  "5",  "4",  "3",  "2",  "1"};
   INT32 fis;
   static INT fissija;
#endif

   memset(ss,0,4);
   memset(ss1,0,4);
   memset(ss2,0,4);
   memset(sjs,0,6);
   memset(as,0,sizeof(as));
   memset(as1,0,sizeof(as1));
   memset(asy,0,sizeof(asy));
   memset(ase,0,sizeof(ase));
   memset(st,0,sizeof(st));

   initline();

	if (!piste)
	   fld = prtfldsmy;
	else
	   fld = prtfldsmk;
   if (hyv(kilp)) {
		if (piste == 0)
			tls = yhtpisteet(kilp, nkierros[kilp->Sarja()]);
		else
			tls = hyppypisteet(kilp, (piste-1)/2);
      }

   if (sj == 1 || (sj && tls != edtls)) {
      _itow(sj, sjs, 10);
	  putfld(sjs, fld[0].pos, fld[0].len, fld[0].oik, 0);
      }
	edtls = tls;
   _itow(kilp->id(), st, 10);
   putfld(st, fld[1].pos, fld[1].len, fld[1].oik, 0);
   if (tulosmuot.vaihdanimet) {
	  st[0] = 0;
      vaihdanm(kilp->nimi, st+wcslen(st));
      putfld(st, fld[2].pos, fld[2].len, fld[2].oik, 0);
      }
   else
      putfld(kilp->nimi, fld[2].pos, fld[2].len, fld[2].oik, 0);
   st[0] = 0;
   if (fiskoodi) {
	  if (kilp->lisno[1] > 0 && kilp->lisno[1] < 100000000L)
		 _ltoa(kilp->lisno[1], st, 10);
	  }
   else
	  if (kilp->lisno[0] > 0 && kilp->lisno[0] < 100000000L)
		 _ltoa(kilp->lisno[0], st, 10);
   putfld(st, fld[3].pos, fld[3].len, fld[3].oik, 0);
   wcscpy(st, kilp->seuratieto(0));
   putfld(st, fld[4].pos, fld[4].len, fld[4].oik, 0);
   if (kilp->maa[0]) {
      putfld(st, fld[5].pos, fld[5].len, fld[5].oik, 0);
      }
   st[0] = 0;
   if (tls) {
		if (!piste) {
			format1(tls, asy, -1, 0);
			format1(hyppypisteet(kilp, 1), as1, -1, 0);
			format1(hyppypisteet(kilp, 2), as, -1, 0);
			}
		else {
			format1(kilp->pituus[(piste-1)/2], as1, -1, 0);
			format1(tls, as, -1, 0);
			}
		if (sj == 1) {
			tkarki = tls;
			if (piste == 0) {
				wcscpy(ase, asy);
				}
			else {
				wcscpy(ase, as);
				}
			}
		else {
			format1(tls-tkarki, ase, -1, 0);
			}
#ifdef FISWCx
      if (sj > 0) fissija = sj;
	  if (tkarki) {
         fis = (60000L*(tls-tkarki))/tkarki;
		 swprintf(st,"%d.%02.2d", (INT)(fis/100), (INT)(fis%100));
         }
      if (fissija > 0 && fissija <= 30 && fld[8].len) 
         putfld(wcpoint[sj-1], fld[8].pos, fld[8].len, fld[8].oik, 0);
      if (sj > 0) fissija = sj;
#endif
      }
   else {
	  switch (kilp->tark(k_pv)) {
		 case 'K' :
			wcscpy(ase, "Kesk.");
			break;
		 case 'O' :
			wcscpy(ase, "Ohit.");
			break;
		 case 'H' :
			wcscpy(ase, "Hyl. ");
			break;
		 case L'V':
		 case L'B':
		 case 'E' :
			wcscpy(ase, "Ei l‰");
			break;
		 }
		wcscpy(asy, ase);
		if (piste == 0) {
			if (fld[9].len && fld[10].len)
				ase[0] = 0;
			}
		else {
			if (fld[7].len && fld[8].len)
				ase[0] = 0;
			}
      }
	if (piste == 0) {
		if (kilp->vasija[k_pv][3]) {
			_itow(kilp->vasija[k_pv][3], ss1, 10);
			}
		if (kilp->vasija[k_pv][5]) {
			_itow(kilp->vasija[k_pv][5], ss2, 10);
			}
	   putfld(ss1, fld[6].pos, fld[6].len, fld[6].oik, 0);
		putfld(as1, fld[7].pos, fld[7].len, fld[7].oik, 0);
	   putfld(ss2, fld[8].pos, fld[8].len, fld[8].oik, 0);
		putfld(as, fld[9].pos, fld[9].len, fld[9].oik, 0);
		putfld(asy, fld[10].pos, fld[10].len, fld[10].oik, 0);
		if (sj > 1 || fld[10].len == 0)
			putfld(ase, fld[11].pos, fld[11].len, fld[11].oik, 0);
		}
	else {
		putfld(as1, fld[6].pos, fld[6].len, fld[6].oik, 0);
		putfld(as,  fld[7].pos, fld[7].len, fld[7].oik, 0);
		putfld(ase, fld[8].pos, fld[8].len, fld[8].oik, 0);
		}
	if (prttuomaripisteet) {
		endline(0);
		nr++;
      if (atxt_on[0])
		 sendchars(atxt_on+1, atxt_on[0], lstf);
		if (piste) {
			if (tuomtulos(st, kilp, (piste-1)/2)) {
				putfld(st, fld[2].pos+fld[2].pos/4 , wcslen(st), 0, 0);
				endline(0);
				nr++;
				}		
			}
		else {
			if (tuomtulos(st, kilp, 1)) {
				putfld(st, fld[2].pos+fld[2].pos/4 , wcslen(st), 0, 0);
				endline(0);
				nr++;
				}
			if (tuomtulos(st, kilp, 2)) {
				putfld(st, fld[2].pos+fld[2].pos/4 , wcslen(st), 0, 0);
				endline(0);
				nr++;
				}
			}
	  if (atxt_pois[0])
		 sendchars(atxt_pois+1, atxt_pois[0], lstf);
		}
   return(nr);
   }
#endif

#ifdef MERI5

INT32 meripiste(kilptietue *kilp, int i_pv);

void meri3pisteet(void)
	{
	int d, srj, i_pv, meri3on = 0;
	INT32 tls;
	kilptietue kilp;

	vidspwmsg(ySize-6,5,7,0, L"Pisteiden laskenta k‰ynniss‰");
	for (srj = 0; srj < sarjaluku; srj++) {
		if (vaiheLkm(srj) == 3) {
			meri3on = 1;
			for (i_pv = 0; i_pv <= k_pv; i_pv++)
				m3vert[i_pv][srj] = 999999999;
			}
		}
	if (meri3on) {
		EnterCriticalSection(&idindex_CriticalSection);
		d = getpos(minkilpno, true);
		while (d > 0) {
			kilp.GETREC(d);
			if (vaiheLkm(kilp.Sarja()) == 3) {
				for (i_pv = Sarjat[kilp.Sarja()].enspv; i_pv <= Sarjat[kilp.Sarja()].viimpv; i_pv++) {
					if (Sarjat[kilp.Sarja()].luonne[i_pv] >= 0 && (tls = kilp.tulos_pv(i_pv, true)) > 0 &&
						tls < m3vert[i_pv][kilp.Sarja()])
						m3vert[i_pv][kilp.Sarja()] = tls;
					}
				}
			d = nextpos();
			}
		d = getpos(minkilpno, true);
		while (d > 0) {
			kilp.GETREC(d);
			if (vaiheLkm(kilp.Sarja()) == 3) {
				kilp.pisteet[0] = 0;
				for (i_pv = Sarjat[kilp.Sarja()].enspv; i_pv <= Sarjat[kilp.Sarja()].viimpv; i_pv++) {
					if (Sarjat[kilp.Sarja()].luonne[i_pv] >= 0 && (tls = kilp.tulos_pv(i_pv, true)) > 0) {
						kilp.pv[i_pv].pvpisteet[0] = 1500 -
							(500 * tls + m3vert[i_pv][kilp.Sarja()]/2) / m3vert[i_pv][kilp.Sarja()];
						kilp.pisteet[0] += kilp.pv[i_pv].pvpisteet[0];
						}
					}
				kilp.tallenna(d, 0, 0, 1, 0);
				}
			d = nextpos();
			}
		LeaveCriticalSection(&idindex_CriticalSection);
		}
	clrtxt(ySize-6,5,40);
	}

static void meripisteet(kilptietue *kilp, INT32 *pst, INT32 *yhtpst)
	{
	int i_pv, ok = 1;

	*yhtpst = 0;
	if (k_pv < Sarjat[kilp->Sarja()].enspv)
		ok = 0;
	for (i_pv = Sarjat[kilp->Sarja()].enspv; i_pv < kilpparam.n_pv_akt; i_pv++) {
		if (Sarjat[kilp->Sarja()].luonne[i_pv] < 0 || kilp->tark(i_pv) == 'P') {
			pst[i_pv] = 0;
			continue;
			}
		if (i_pv <= k_pv && kilp->tulos_pv(i_pv)) {
			pst[i_pv] = kilp->pv[i_pv].pvpisteet[0];
			*yhtpst += pst[i_pv];
			}
		else {
			pst[i_pv] = -1;
			if (i_pv <= k_pv)
				ok = 0;
			}
		}
	if (!ok) {
		*yhtpst = -1;
		kilp->pisteet[0] = 0;
		}
	else
		kilp->pisteet[0] = *yhtpst;
	}

#if defined(__BORLANDC__)
#pragma warn -par
#endif
static INT prttulos_5ott(kilptietue *kilp, tulostusparamtp *tulprm, INT sj,
   INT32 tkarki)
   {
   wchar_t as[50], ase[20], strk[2] = L" ";
   wchar_t st[150];
//   static INT32 tkarki;
   wchar_t sjs[20];
   INT32 tls = 0;
   FldFrmtTp *fld;
   INT i, nr = 1, pv, piste;
   wchar_t asy[20],as1[20],ss[20],ssy[20],ss1[20];
   INT sj2 = 0;
   INT32 pst[N_PV], yhtpst = 0;
   wchar_t *txts[MAXLANG][6] = {{L" Hyl.     ", L" Kesk.    ", L"Ei l‰ht.  ", L"Sakko", L"Ammunta", L"V‰liajat:"},
								{L" DQ       ", L" DNF      ", L" DNS      ", L"Penalty", L"Shooting", L"Splits:  "}
							 /*,{L" Diskv.   ", L" Avbr.    ", L"Ei startat", L"Till‰gg"}*/};

   piste = tulprm->piste;
   if (piste > kilpparam.valuku) piste = 0;
   memset(asy,0,13);
   memset(as1,0,13);
   memset(ss,0,4);
   memset(ss1,0,4);
   memset(ssy,0,4);
   memset(sjs,0,6);
   memset(pst,0,sizeof(pst));

   initline(tulprm);
   memset(ase,0,sizeof(ase));
   memset(st,0,sizeof(st));
   wcscpy(as, L"  ----");

//	fld = tulprm->vaihelkm == 1 ? prtflds : prtflds1;
	fld = (FldFrmtTp *)tulprm->p_fld;
	if (tulprm->lahtoajat) {
		tulprm->vaihelkm = 1;
		tulprm->yhttuljarj = false;
		sj = 0;
		}
   else if (kilp->hyv() || (piste && piste <= kilpparam.valuku && kilp->tark(k_pv) == L'K')) {
	  tls = kilp->tulos2(piste);
	  }
   if (!piste) {
		meripisteet(kilp, pst, &yhtpst);
		}
   if (k_pv > 0 && (tulprm->vaihelkm > 1 || tulprm->yhttuljarj)) {
	  if (yhtpst >= 0) {
		 sj = kilp->y_sija(k_pv);
		 }
	  }

   if (sj) {
	  _itow(sj, sjs, 10);
	  putfld(tulprm, sjs, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
	  }
   _itow(kilp->id(), st, 10);
   putfld(tulprm, st, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, 0);
   putfld(tulprm, kilp->nimi(st, 60, false), fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
   putfld(tulprm, kilp->arvo, fld[F_ARVO].pos, fld[F_ARVO].len, fld[F_ARVO].oik, 0);
   putfld(tulprm, kilp->seura, fld[F_SRA].pos, fld[F_SRA].len, fld[F_SRA].oik, 0);
   putfld(tulprm, kilp->seuralyh, fld[F_SRALYH].pos, fld[F_SRALYH].len, fld[F_SRALYH].oik, 0);
   putfld(tulprm, kilp->yhdistys, fld[F_YHD].pos, fld[F_YHD].len, fld[F_YHD].oik, 0);
   putfld(tulprm, kilp->joukkue, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, 0);
   st[0] = 0;
	if (tulprm->lahtoajat) {
		AIKATOWSTRS(as, kilp->p_aika(-1), t0);
		as[pvparam[k_pv].laika2] = 0;
		}
	else {
	   if (tls) {
			_itow(pst[k_pv], ase, 10);
			insdes(ase, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
			AIKATOWSTRS(as, tls,0);
			as[pvparam[k_pv].laika2] = 0;
			elimwzb1(as);
			}
		else {
			as[0] = 0;
		  sj2 = 0;
			if (kilp->hyv()) {
				}
			else {
				switch(kilp->tark(k_pv)) {
				case L'H' :
					wcscpy(as, txts[tulprm->language][0]);
					break;
				case L'K' :
					wcscpy(as, txts[tulprm->language][1]);
					break;
				case L'V':
				case L'E' :
				case L'B' :
					wcscpy(as, txts[tulprm->language][2]);
					break;
					}
				}
			}
		if (yhtpst >= 0) {
			_itow(yhtpst, asy, 10);
			insdes(asy, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
			}
		if (sj2) _itow(sj2, ssy, 10);
	  }
   if (tulprm->vaihelkm == 1 && !tulprm->yhttuljarj) {
	   if (wcswcind(pvparam[k_pv].alalaji, L"HT") >= 0) {
			if (pvparam[k_pv].alalaji == L'H') {
				int ap = 0;
				wcscpy(as, L"(");
				for (int i = 0; i < Sarjat[kilp->Sarja()].lsak[k_pv]; i++) {
					if (kilp->pv[k_pv].asakot[i]-L'0' > 0) {
						ap += kilp->pv[k_pv].asakot[i]-L'0';
						if (i > 0)
							wcscat(as, L"+");
						_itow(kilp->pv[k_pv].asakot[i]-L'0', as+wcslen(as), 10);
						}
					}
				if (wcslen(as) > 4)
					wcscat(as, L")");
				else
					as[0] = 0;
				swprintf(st, L"%s%d %s", txts[tulprm->language][3], ap, as);

				}
			else if (pvparam[k_pv].alalaji == L'T') {
				swprintf(st, L"Tark: %d, Pit: %d,%d", (kilp->pv[k_pv].asakot[0]-L'0')/10,
						(kilp->pv[k_pv].asakot[1]-L'0')/10, (kilp->pv[k_pv].asakot[1]-L'0')%10);
				}
		 putfld(tulprm, st, fld[F_TLS].pos, fld[F_TLS].len+fld[F_SAK].len, 0, 0);
		 }
	  else {
			putfld(tulprm, as, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
			as[0] = 0;
			if (Sarjat[kilp->Sarja()].lsak[k_pv] && kilp->sakko_pv(k_pv)) {
				if (tls && fld[F_AIKA].len) {
					tls -= kilp->sakko_pv(k_pv);
					AIKATOWSTRS(as, tls,0);
					as[pvparam[k_pv].laika2] = 0;
					elimwzb1(as);
					putfld(tulprm, as, fld[F_AIKA].pos, fld[F_AIKA].len, fld[F_AIKA].oik, 0);
					}
				st[0] = 0;
				if (fld[F_SAK].len > 8)
					swprintf(st, L"%s %d", txts[tulprm->language][3],
						abs(int(kilp->sakko_pv(k_pv)/Sarjat[kilp->Sarja()].tsak[k_pv]/SEK)));
				if (st[0] == 0)
					swprintf(st, L"%d", abs(int(kilp->sakko_pv(k_pv)/Sarjat[kilp->Sarja()].tsak[k_pv]/SEK)));
				}
			if (st[0]) {
				putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
			}
		}
		if (ase[0]) putfld(tulprm, ase, fld[F_PIST].pos, fld[F_PIST].len, fld[F_PIST].oik, 0);
	  }
   else {       // Kaikkien vaiheiden pisteet
		for (pv = Sarjat[kilp->Sarja()].enspv; pv <= Sarjat[kilp->Sarja()].viimpv+1; pv++) {
			if (Sarjat[kilp->Sarja()].luonne[pv] >= 0) {
				int pstval = 0;
				as1[0] = 0;
				if (pv > Sarjat[kilp->Sarja()].viimpv) {
					pstval = yhtpst;
					}
				else if (Sarjat[kilp->Sarja()].luonne[pv] >= 0) {
					pstval = pst[pv];
					}
				if (pstval >= 0) {
					_itow(pstval, as1, 10);
					insdes(as1, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
					}
				int x = fld[F_OSATLS].pos + (pv-Sarjat[kilp->Sarja()].enspv)*fld[F_OSATLS].askel;
				putfld(tulprm, as1, x, fld[F_OSATLS].len, fld[F_OSATLS].oik, 0);
				}
			}
	  }
   if (tulprm->kaikkiva && Sarjat[kilp->Sarja()].valuku) {
	  endline(tulprm, 0);
	  nr++;
	  wcscpy(st, txts[tulprm->language][5]);
	  for (i = 0; i < Sarjat[kilp->Sarja()].valuku[k_pv]; i++) {
		 if ((tls = kilp->tulos2(i+1)) != 0) {
			if (wcslen(st) > 50) {
			   st[wcslen(st)-1] = 0;
			   putfld(tulprm, st, fld[2].pos, 80, 0, 0);
			   endline(tulprm, 0);
			   nr++;
			   wcscpy(st, L"        ");
			   }
			AIKATOWSTRS(as,tls,0);
			as[pvparam[k_pv].laika2] = 0;
			elimwzb1(as);
			swprintf(st+wcslen(st), L" %d: %s/%d.,", i+1, as, kilp->p_sija( i+1));
			}
		 }
	  st[wcslen(st)-1] = 0;
	  putfld(tulprm, st, fld[F_NIMI].pos, 80, 0, 0);
	  }
   return(nr);
   }
#if defined(__BORLANDC__)
#pragma warn +par
#endif

//static void outtulos_csv(INT l, kilptietue *kilp, INT sj, INT piste, INT32 karkitls, wchar_t co)
static void outtulos_csv_5ott(kilptietue *kilp, INT sj, INT32 karkitls, tulostusparamtp *tulprm)
   {
   wchar_t rivi[1020], st[132], tas[12];
   INT len, pv;
   INT32 tls;
   INT32 ytls = 0, pst[N_PV], yhtpst;
   static INT32 tkarki;

   static INT sj1;

	meripisteet(kilp, pst, &yhtpst);
   if (tulprm->kohde == L'R' || tulprm->kohde == L'Z') {
	  }
   else {
	  wmemset(rivi, L' ', sizeof(rivi)/2-1);
	  rivi[sizeof(rivi)/2-1] = 0;
	  if (sj) putintrightw(sj, rivi+dspflds[F_SJ].pos+dspflds[F_SJ].len-1);
      if (dspflds[F_KNO].len)
		 putintrightw(kilp->id(), rivi+dspflds[F_KNO].pos+dspflds[F_KNO].len-1);
      len = min((int)wcslen(kilp->nimi(st, 60, false)), dspflds[F_NIMI].len);
      wmemcpy(rivi+dspflds[F_NIMI].pos, kilp->nimi(st, 60, false), len);
	  len = min((int)wcslen(kilp->seuratieto(0)), dspflds[F_SRA].len);
	  wmemcpy(rivi+dspflds[F_SRA].pos, kilp->seuratieto(0), len);
	  if ((tls = kilp->tulos2(tulprm->piste)) > 0 && kilp->hyv()) {
			if (dspflds[F_TLS].len) {
				AIKATOWSTRS(st,tls,0);
				st[pvparam[k_pv].laika2] = 0;
				elimwzb1(st);
				len = min((int)wcslen(st), dspflds[F_TLS].len);
				wmemcpy(rivi+dspflds[F_TLS].pos+dspflds[F_TLS].len-len, st, len);
				}
			if (pst[k_pv] >= 0)
				putintrightw(pst[k_pv], rivi+79);
			}
		for (pv = 0; pv <= k_pv; pv++) {
			if (pst[pv] >= 0)
				putintrightw(pst[pv], rivi + 52 + 5*pv);
			}
      if (!kilp->hyv())
		 rivi[dspflds[F_TLS].pos+dspflds[F_TLS].len] = kilp->tark(k_pv);
      }
   if (tulprm->kohde == L'I') {
	  swprintf(tas, L"%-10.10s ", Sarjat[kilp->Sarja()].sarjanimi);
	  tulprm->writeline(rivi);
      }
   else if (tulprm->kohde == L'R' || tulprm->kohde == L'Z') {
      tulprm->writeline(rivi);
      }
   }

static void outtulos_5ott(int l, kilptietue *kilp, INT sj, int piste, INT32 karkitls, tulostusparamtp *tulprm)
   {
   wchar_t rivi[1020], st[132], tas[12];
   INT len, pv;
   INT32 tls;
   INT32 ytls = 0, pst[N_PV], yhtpst;
   static INT32 tkarki;

   static INT sj1;

	meripisteet(kilp, pst, &yhtpst);
   if (tulprm->kohde == L'R' || tulprm->kohde == L'Z') {
      }
   else {
	  wmemset(rivi, L' ', sizeof(rivi)/2-1);
	  rivi[sizeof(rivi)/2-1] = 0;
	  if (sj) putintrightw(sj, rivi+dspflds[F_SJ].pos+dspflds[F_SJ].len-1);
      if (dspflds[F_KNO].len)
		 putintrightw(kilp->id(), rivi+dspflds[F_KNO].pos+dspflds[F_KNO].len-1);
      len = min((int)wcslen(kilp->nimi(st, 60, false)), dspflds[F_NIMI].len);
	  wmemcpy(rivi+dspflds[F_NIMI].pos, kilp->nimi(st, 60, false), len);
	  len = min((int)wcslen(kilp->seuratieto(0)), dspflds[F_SRA].len);
	  wmemcpy(rivi+dspflds[F_SRA].pos, kilp->seuratieto(0), len);
	  if ((tls = kilp->tulos2(tulprm->piste)) > 0 && kilp->hyv()) {
			if (dspflds[F_TLS].len) {
				AIKATOWSTRS(st,tls,0);
				st[pvparam[k_pv].laika2] = 0;
				elimwzb1(st);
				len = min((int)wcslen(st), dspflds[F_TLS].len);
				wmemcpy(rivi+dspflds[F_TLS].pos+dspflds[F_TLS].len-len, st, len);
				}
			if (pst[k_pv] >= 0)
				putintrightw(pst[k_pv], rivi+79);
			}
		for (pv = 0; pv <= k_pv; pv++) {
			if (pst[pv] >= 0)
				putintrightw(pst[pv], rivi + 52 + 5*pv);
			}
      if (!kilp->hyv())
		 rivi[dspflds[F_TLS].pos+dspflds[F_TLS].len] = kilp->tark(k_pv);
	  }
   if (tulprm->kohde == L'I') {
	  swprintf(tas, L"%-10.10s ", Sarjat[kilp->Sarja()].sarjanimi);
	  tulprm->writeline(rivi);
      }
   else if (l >= 0 && l+3 < ySize-4)
	  viwrrectw(l+3,0,l+3,79,rivi,7,0,0);
   }

#else  // !MERI5

static INT prttulos_5ott(kilptietue *kilp, tulostusparamtp *tulprm, INT sj,
   INT32 tkarki)
   {
   return(0);
   }

static void outtulos_csv_5ott(kilptietue *kilp, INT sj, INT32 karkitls, tulostusparamtp *tulprm)
   {
   }

static void outtulos_5ott(int l, kilptietue *kilp, INT sj, int piste, INT32 karkitls, tulostusparamtp *tulprm)
   {
   }
#endif  // !MERI5

//#ifndef MERI5

#if defined(_BORLANDC_)
#pragma warn -par
#endif
static INT prttulos(kilptietue *kilp, tulostusparamtp *tulprm, INT sj, INT32 tkarki)
	{
	wchar_t as[16], ase[16], strk[2] = L" ";
	wchar_t asy[16],as1[16],ss[16],ssy[16],ss1[16];
	wchar_t st[150];
	wchar_t sjs[16];
	INT32 tm, tls = 0;
	FldFrmtTp *fld;
	INT piste, i, nr = 1, pos;
	INT ks, sj2 = 0;
	INT32 tls1, ytls = 0;
   wchar_t *txts[MAXLANG][11] = {{L"Hyl.", L"Kesk.", L"Ei l‰ht.", L"Virhe", L"Puuttuu", L"Sakko", L"Tasoitus", L"Ohitettu", L"Poissa", L"V‰liajat:", L"EiAikaa"},
								{L"DQ", L"DNF", L"DNS", L"Missing", L"DNF", L"Penalty", L"Handicap", L"Skipped", L"Absent", L"Splits", L"Exclude"}
							 /*,{L" Diskv.   ", L" Avbr.    ", L"Ei startat", L"Till‰gg", L"respit"}*/};

#ifdef FISWC
	wchar_t *wcpoint[] =
	  {L"100", L"80", L"60", L"50", L"45", L"40", L"36", L"32", L"29", L"26",
		L"24", L"22", L"20", L"18", L"16", L"15", L"14", L"13", L"12", L"11",
		L"10",  L"9",  L"8",  L"7",  L"6",  L"5",  L"4",  L"3",  L"2",  L"1"};
	INT32 fis;
	static INT fissija;
#endif

	if (kilpparam.kilplaji == L'N' || kilpparam.kilplaji == L'P') {
		return(prttulos_5ott(kilp, tulprm, sj, tkarki));
		}

	memset(asy,0,sizeof(asy));
	memset(as1,0,sizeof(as1));
	memset(ss,0,sizeof(ss));
	memset(ssy,0,sizeof(ssy));
	memset(sjs,0,sizeof(sjs));
	memset(ase,0,sizeof(ase));
	memset(st,0,sizeof(st));
	wcscpy(as, L"  ----");

	fld = prtflds;
	initline(tulprm);

	piste = tulprm->piste;

	if (piste > kilpparam.valuku)
		piste = 0;

	if (piste == -1) {
		tulprm->vaihelkm = 1;
		sj = 0;
		}

	if (kilpparam.alalaji == L'D' && (tulprm->optiot & 0x00200000)) {
		if (sj == 1)
			karkikierr = kilp->pv[k_pv].pvpisteet[0];
		}
	else
		karkikierr = 0;

	if (piste >= 0 && (kilp->hyv() ||
		(piste && piste <= kilpparam.valuku && (kilp->tark(k_pv) == L'K' || kilp->tark(k_pv) == L'O')))) {
		tls = kilp->tulos2(piste, 2);
		if (kilp->tark(k_pv) == L'T')
			strk[0] = L'T';
#ifdef LVAYLAX
		if (kilp->sija(k_pv) > 10)
			tls = 100 * (tls / 100);
#endif
		}

//	if (k_pv > 0 && (tulprm->vaihelkm > 1 || tulprm->yhttulpaino)) {
	if (tulprm->vaihelkm > 1 || tulprm->yhttulpaino) {
		if ((ytls = kilp->ytulos(k_pv, 2)) != 0) {
			sj2 = kilp->pv[k_pv].ysija;
			}
		switch (tulprm->vaihelkm) {
			case 1:
				tls = ytls;
				ytls = 0;
				break;
			case 2:
				if (tulprm->yhttuljarj) {
					if (kilp->hyv())
					sj2 = kilp->sija(k_pv);
					}
				fld = prtfldsm;
				break;
			case 3:
				fld = prtflds1;
				if (kilp->hyv())
					sj2 = kilp->sija(k_pv);
				break;
			}
		}

	int aspos = fld[F_TLS].pos;
	int aslen = fld[F_TLS].len;

   if (sj && !tulprm->piilotasijat && !Sarjat[kilp->Sarja()].piilotatulos) {
	  _itow(sj, sjs, 10);
		putfld(tulprm, sjs, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
		}
	_itow(kilp->id(), st, 10);
	putfld(tulprm, st, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, 0);
	if (fld[F_NIMI].len) {
		if (tulprm->tulmuot.vaihdanimet) {
			st[0] = 0;
	//      if (fld[12].len == 0) {
			if (kilpparam.larvo > 0 && fld[F_ARVO].len == 0) {
				wcscpy(st, kilp->arvo);
				elimwbl(st);
				if (wcslen(st))
					wcscat(st, L" ");
				}
			kilp->nimi(st+wcslen(st), 60, true);
			putfld(tulprm, st, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
			}
		else
			putfld(tulprm, kilp->nimi(st, 60, false), fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
		}
	if (fld[F_ETUNIMI].len)
		putfld(tulprm, kilp->etunimi, fld[F_ETUNIMI].pos, fld[F_ETUNIMI].len, fld[F_ETUNIMI].oik, 0);
	if (fld[F_SUKUNIMI].len)
		putfld(tulprm, kilp->sukunimi, fld[F_SUKUNIMI].pos, fld[F_SUKUNIMI].len, fld[F_SUKUNIMI].oik, 0);

	putfld(tulprm, kilp->arvo, fld[F_ARVO].pos, fld[F_ARVO].len, fld[F_ARVO].oik, 0);
	if (kilp->lisno[0])
		putfld_int(tulprm, kilp->lisno[0], fld[F_LISNO].pos, fld[F_LISNO].len, fld[F_LISNO].oik, 0);
	if (kilp->lisno[1]) {
		kilp->strval(tulprm, st, fld[F_LISNO2].len, F_LISNO2, k_pv, 0, 0, 0);
		putfld(tulprm, st, fld[F_LISNO2].pos, fld[F_LISNO2].len, fld[F_LISNO2].oik, 0);
		}
	if (kilp->pv[k_pv].badge[0])
		putfld_int(tulprm, kilp->pv[k_pv].badge[0], fld[F_BADGE].pos, fld[F_BADGE].len, fld[F_BADGE].oik, 0);
	putfld(tulprm, kilp->seura, fld[F_SRA].pos, fld[F_SRA].len, fld[F_SRA].oik, 0);
	putfld(tulprm, kilp->seuralyh, fld[F_SRALYH].pos, fld[F_SRALYH].len, fld[F_SRALYH].oik, 0);
	putfld(tulprm, kilp->yhdistys, fld[F_YHD].pos, fld[F_YHD].len, fld[F_YHD].oik, 0);
	putfld(tulprm, kilp->joukkue, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, 0);
	putfld(tulprm, kilp->maa, fld[F_MAA].pos, fld[F_MAA].len, fld[F_MAA].oik, 0);
	if (fld[F_ALISRJ].len > 0 && (kilp->sukup > L' ' || kilp->ikasarja > 0)) {
		memset(st, 0, sizeof(st));
		if (kilp->sukup > L' ')
			st[0] = kilp->sukup;
		if (kilp->ikasarja > 0) {
			switch (kilp->ikasarja) {
				case 1:
					wcscat(st, L"YL");
					break;
				case 2:
					wcscat(st, L"AV");
					break;
				default:
					swprintf(st+wcslen(st), L"%02d", kilp->ikasarja);
				}
			}
		putfld(tulprm, st, fld[F_ALISRJ].pos, fld[F_ALISRJ].len, fld[F_ALISRJ].oik, 0);
		}
	st[0] = 0;
	if (piste == -1) {
		if (pvparam[k_pv].hiihtolahto) {
			AIKATOWSTRS(as, kilp->p_aika(-1), t0);
			as[pvparam[k_pv].laika2] = 0;

			}
		else
			{
			AIKATOWSTRS(as, kilp->pv[k_pv].tlahto, t0);
			as[8] = 0;
			}
		}
	else if (!tulprm->piilotatulos && !Sarjat[kilp->Sarja()].piilotatulos) {
		if (tulprm->vaihelkm <= 2 || tulprm->tulostettava != L'P') {
			if (tls) {
				if (kilpparam.kilplaji == L'B') {
					asuunnStr(st, fld[F_SAK].len, kilp, k_pv, 0, tulprm->language);
					}
				else if (kilpparam.lsakmax > 0) {
					if (k_pv) {
						if (tulprm->vaihelkm < 3) {
							ks = (tulprm->vaihelkm == 2) ? F_SAK+1 : F_SAK;
							if ((int)fld[ks].len >= Sarjat[kilp->Sarja()].lsak[k_pv]+2)
								swprintf(st, L"(%s)", kilp->asakkoStr(k_pv));
							else
								swprintf(st, L"(%d)", saksum(kilp, 1));
							}
						else
							swprintf(st, L"(%d+%d)", saksum(kilp,0), saksum(kilp,1));
						}
					else
						swprintf(st, L"(%s)", kilp->asakkoStr(k_pv));
					}
				else if (prtflds[F_SAK].len && kilp->pv[k_pv].tasoitus) {
					int yks = Sarjat[kilp->Sarja()].tsak[k_pv];
					if (yks == 0)
						yks = kilpparam.sakkoyks ? kilpparam.sakkoyks : 1;
					if (prtflds[F_SAK].len > 7)
					swprintf(st, L"%s %d", kilp->pv[k_pv].tasoitus > 0 ? txts[tulprm->language][5] : txts[tulprm->language][6],
						abs(kilp->pv[k_pv].tasoitus/yks));
					else
						swprintf(st, L"%d", abs(kilp->pv[k_pv].tasoitus/yks));
					}
				if (fld[F_ERO].len) {
					if (sj == 1) {
						if (tulprm->yhttulpaino)
							tkarki = kilp->ytulos(k_pv, 2);
						else
							tkarki = tls;
						if (!tulprm->tulmuot.nollakarki)
							wcscpy(ase, L"00.00.00,0");
						else {
							AIKATOWSTRS(ase, tkarki,0);
							}
						ase[pvparam[k_pv].laika2] = 0;
						elimwzb1(ase);
						}
					else {
						if (tulprm->yhttulpaino) {
							if (kilp->ytulos(k_pv, 2))
								AIKATOWSTRS(ase, kilp->ytulos(k_pv, 2)-tkarki,0);
							}
						else
							AIKATOWSTRS(ase, tls-tkarki,0);
						ase[pvparam[k_pv].laika2] = 0;
						elimwzb1(ase);
						if (wcslen(ase) < pvparam[k_pv].laika2 && ase[0]) {
							wmemmove(ase+1,ase,wcslen(ase)+1);
							ase[0] = L'+';
							}
						}
					}
				}
			if (tls) {
				AIKATOWSTRS(as, tls,0);
				as[pvparam[k_pv].laika2] = 0;
				elimwzb1(as);
#ifdef FISWC
				if (sj > 0) fissija = sj;
				if (tkarki) {
					fis = (60000L*(tls-tkarki))/tkarki;
					swprintf(st,L"%d.%02.2d", (INT)(fis/100), (INT)(fis%100));
					}
				if (fissija > 0 && fissija <= 30 && fld[F_PIST].len)
					putfld(wcpoint[sj-1], fld[F_PIST].pos, fld[F_PIST].len, fld[F_PIST].oik, 0);
				if (sj > 0) fissija = sj;
#endif
				}
			else {
				sj2 = 0;
				if (kilp->hyv()) {
					if (tulprm->rajaus == L'A') {
						AIKATOWSTRS(as+1, kilp->TLahto(), t0);
						}
					else if (!k_pv || tulprm->vaihelkm > 1 || !tulprm->yhttulpaino)
						{
						tm = KSEK*T_TIME(biostime(0,0));
						if (NORMTULOS(tm - kilp->pv[k_pv].tlahto) < 12*TUNTI)
							AIKATOWSTRS(as+1,kilp->tulos1(piste, tm),0);
						else wcscpy(as, L"          ");
						}
					as[0]= L'[';
					as[9] = L']';
					as[10] = 0;
					if (!k_pv)
						aslen = 10;
					}
				else {
					switch(kilp->tark(k_pv)) {
						case L'K' :
							wcscpy(as, (Sarjat[kilp->Sarja()].rrtas == 1) ? txts[tulprm->language][4] : txts[tulprm->language][1]);
							break;
						case L'O' :
							wcscpy(as, txts[tulprm->language][7]);
							break;
						case L'H' :
							wcscpy(as, (Sarjat[kilp->Sarja()].rrtas == 1) ? txts[tulprm->language][3] : txts[tulprm->language][0]);
							break;
						case L'M' :
							wcscpy(as, txts[tulprm->language][3]);
							break;
						case L'X' :
							wcscpy(as, txts[tulprm->language][10]);
							break;
						case L'V':
						case L'E' :
						case L'B' :
							wcscpy(as, txts[tulprm->language][2]);
							break;
						}
					}
				if (k_pv && tulprm->vaihelkm > 1 || tulprm->yhttulpaino) {
					if (kilp->hyv() && (tls1 = kilp->ytulos(k_pv-1, 2)) != 0) {
						tm = KSEK*T_TIME(biostime(0,0));
						if (NORMTULOS(tm - kilp->pv[k_pv].tlahto) < 12*TUNTI)
							AIKATOWSTRS(asy+1, kilp->tulos1(0, tm) + tls1,0);
						asy[0] = L'[';
						asy[9] = L']';
						asy[10] = 0;
						if (tulprm->vaihelkm == 1)
							wcscpy(as, asy);
						}
					else
						wcscpy(asy, L"  ----");
					}
				}
			if (k_pv) {
				if (ytls) {
					kilp->ytulos_wstr_cols_n(asy, k_pv, tulprm->tulmuot.des_sep, tulprm->tulmuot.pistedesim,
						tulprm->ypJrj, pvparam[k_pv].laika2, 2);
					elimwzb1(asy);
					}
				if (sj2)
					_itow(sj2, ssy, 10);
				else
					wcscpy(ssy, L" ");
				if (tulprm->vaihelkm > 2) {
					for (int pv = 0; pv <= k_pv+1; pv++) {
						wmemset(as1,' ',pvparam[k_pv].laika2+2);
						as1[pvparam[k_pv].laika2+2] = 0;
						wmemset(ss1,' ',3);
						ss1[3] = 0;
						if (pv == k_pv+1) {
							pos = fld[F_TLS].pos;
							if ((tls = kilp->ytulos(k_pv, 2)) != 0) {
								_itow(kilp->y_sija(k_pv), ss1, 10);
								kilp->ytulos_wstr_cols_n(as1, k_pv, tulprm->tulmuot.des_sep, tulprm->tulmuot.pistedesim,
									tulprm->ypJrj, pvparam[k_pv].laika2, 2);
								elimwzb1(as1);
								}
							else {
								wcscpy(as1, L"----");
								}
							}
						else {
							pos = fld[F_OSATLS].pos + pv * fld[F_OSATLS].askel;
							if ((tls = kilp->tulos_pv(pv, true)) > 0) {
								_itow(kilp->sija(pv), ss1, 10);
								AIKATOWSTRS(as1, tls, 0);
								as1[pvparam[pv].laika2] = 0;
								elimwzb1(as1);
								}
							else {
								switch(kilp->tark(pv)) {
									case L'K' :
										wcscpy(as1, (Sarjat[kilp->Sarja()].rrtas == 1) ? txts[tulprm->language][4] : txts[tulprm->language][1]);
										break;
									case L'O' :
										wcscpy(as1, txts[tulprm->language][7]);
										break;
									case L'H' :
										wcscpy(as1, (Sarjat[kilp->Sarja()].rrtas == 1) ? txts[tulprm->language][3] : txts[tulprm->language][0]);
										break;
									case L'M' :
										wcscpy(as1, txts[tulprm->language][3]);
										break;
									case L'X' :
										wcscpy(as1, txts[tulprm->language][10]);
										break;
									case L'V':
									case L'E' :
									case L'B' :
										wcscpy(as1, txts[tulprm->language][2]);
										break;
									case L'P' :
										wcscpy(as1, txts[tulprm->language][8]);
										break;
									}
								}
							}
						if (pv <= k_pv)
							putfld(tulprm, ss1, fld[F_OSASJ].pos + pv * fld[F_OSASJ].askel, fld[F_OSASJ].len, fld[F_OSASJ].oik, 0);
						putfld(tulprm, as1, pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
						}
					}
				}
			}
		if (tulprm->vaihelkm == 1) {
			putfld(tulprm, as, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
			as[0] = 0;
			if (ase[0]) putfld(tulprm, ase, fld[F_ERO].pos, fld[F_ERO].len, fld[F_ERO].oik, 0);
			if (kilpparam.kilplaji != L'B' && kilpparam.lsakmax) {
				if (st[0]) {
					if ((int)fld[F_SAK].len >= Sarjat[kilp->Sarja()].lsak[k_pv]+2) {
						for (i = Sarjat[kilp->Sarja()].lsak[k_pv] -1; i >= 0; i--) {
							putfld(tulprm, st+i+1, fld[F_SAK].pos+(2*i)*tulprm->tulmuot.numlev, 1, 0, 0);
							}
						}
					else
						putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
					}
				if (fld[F_AIKA].len && tls) {
					tls = kilp->tulos2(piste, 2) - kilp->p_sakko(piste);
					AIKATOWSTRS(as, tls,0);
					as[pvparam[k_pv].laika2] = 0;
					elimwzb1(as);
					putfld(tulprm, as, fld[F_AIKA].pos, fld[F_AIKA].len, fld[F_AIKA].oik, 0);
					as[0] = 0;
					}
				}
			else {
				if (st[0]) {
					putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
					}
				putfld(tulprm, strk, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, 0);
				}
			if (!tulprm->yhttuljarj && kilp->pv[k_pv].pvpisteet[0] != karkikierr) {
				_itow(kilp->pv[k_pv].pvpisteet[0]-karkikierr, as, 10);
				if (tulprm->tulmuot.pistedesim)
					insdes(as, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
				putfld(tulprm, as, fld[F_PIST].pos, fld[F_PIST].len, fld[F_PIST].oik, 0);
				}
			}
		else if (tulprm->vaihelkm == 2) {
			putfld(tulprm, tulprm->yhttuljarj ? asy : as, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
			putfld(tulprm, ssy, fld[F_OSASJ].pos, fld[F_OSASJ].len, fld[F_OSASJ].oik, 0);
			putfld(tulprm, tulprm->yhttuljarj ? as : asy, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik, 0);
			if (ase[0]) putfld(tulprm, ase, fld[F_ERO].pos, fld[F_ERO].len, fld[F_ERO].oik, 0);
			as[0] = 0;
			if (st[0]) {
				putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
				}
			putfld(tulprm, strk, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, 0);
			}
		else if (tulprm->tulostettava == L'P') {
			for (int ipv = Sarjat[kilp->Sarja()].enspv; ipv <= k_pv + 1; ipv++) {
				if (Sarjat[kilp->Sarja()].luonne[ipv] >= 0) {
					as[0] = 0;
					int x = fld[F_TLS].pos + ipv * fld[F_TLS].askel;
					if (Sarjat[kilp->Sarja()].luonne[ipv] >= 0) {
						if (ipv <= k_pv && kilp->tulos_pv(ipv, true) && kilp->pv[ipv].pvpisteet[0] != 0)
							_itow(kilp->pv[ipv].pvpisteet[0], as, 10);
						if (ipv == k_pv+1 && kilp->pisteet[0] != 0)
							_itow(kilp->pisteet[0], as, 10);
						if (as[0] && tulprm->tulmuot.pistedesim)
							insdes(as, tulprm->tulmuot.pistedesim, tulprm->tulmuot.des_sep);
						}
					putfld(tulprm, as, x, fld[F_TLS].len, fld[F_TLS].oik, 0);
					}
				}
			}
#ifdef EI_LIENE
		else {
			putfld(tulprm, ss1, fld[5].pos, fld[5].len, fld[5].oik, 0);
			putfld(tulprm, as1, fld[6].pos, fld[6].len, fld[6].oik, 0);
			putfld(tulprm, ssy, fld[7].pos, fld[7].len, fld[7].oik, 0);
			putfld(tulprm, as,  fld[8].pos, fld[8].len, fld[8].oik, 0);
			putfld(tulprm, sjs, fld[9].pos, fld[9].len, fld[9].oik, 0);
			putfld(tulprm, asy, fld[10].pos, fld[10].len, fld[10].oik, 0);
#ifdef HIIHTO2
			if (st[0])
				putfld(tulprm, st, fld[11].pos, fld[11].len, fld[11].oik, 0);
#endif
			}
#endif // EI_LIENE
		if (k_pv == 0) {   // 1-p‰iv‰inen
			as[0] = 0;
			as[pvparam[k_pv].laika2] = 0;
			if (fld[F_TLS].len)
				putfld(tulprm, as, aspos, min(aslen,(int)fld[F_TLS].len+2), fld[F_TLS].oik, 0);
			if (ase[0]) putfld(tulprm, ase, fld[F_ERO].pos, fld[F_ERO].len,
				fld[F_ERO].oik, 0);
			if (kilpparam.kilplaji == L'B') {
				asuunnStr(st, fld[F_SAK].len, kilp, k_pv, 0, tulprm->language);
				putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
				}
			else if (kilpparam.lsakmax) {
				if (st[0]) {
					if ((int)fld[F_SAK].len >= Sarjat[kilp->Sarja()].lsak[k_pv]+2) {
						for (i = Sarjat[kilp->Sarja()].lsak[k_pv] -1; i >= 0; i--) {
							putfld(tulprm, st+i+1, fld[F_SAK].pos+(2*i)*tulprm->tulmuot.numlev, 1, 0, 0);
							}
						}
					else
						putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
					}
				if (fld[F_AIKA].len && tls) {
					tls = kilp->tulos2(piste, 2) - kilp->p_sakko(piste);
					AIKATOWSTRS(as, tls,0);
					as[pvparam[k_pv].laika2] = 0;
					elimwzb1(as);
					putfld(tulprm, as, fld[F_AIKA].pos, fld[F_AIKA].len, fld[F_AIKA].oik, 0);
					}
				}
			else {
				if (st[0]) {
					putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
					}
				}
#ifdef MCPIST
		if (fld[9].len && sj > 0 && sj < 31) {
			_itow(mcpist[sj-1], st, 10);
			putfld(tulprm, st, fld[F_PIST].pos, fld[F_PIST].len, fld[F_PIST].oik, 0);
			}
#endif
		}  // 1-p‰iv‰inen, loppu
		if (tulprm->kaikkiva && Sarjat[kilp->Sarja()].valuku) {
			endline(tulprm, 0);
			nr++;
			wcscpy(st, txts[tulprm->language][9]);
			for (i = 0; i < Sarjat[kilp->Sarja()].valuku[k_pv]; i++) {
				if ((tls = kilp->tulos2(i+1, 2)) != 0) {
					if (wcslen(st) > 50) {
						st[wcslen(st)-1] = 0;
						putfld(tulprm, st, fld[2].pos, 80, 0, 0);
						endline(tulprm, 0);
						nr++;
						wcscpy(st, L"        ");
						}
					AIKATOWSTRS(as, tls,0);
					as[pvparam[k_pv].laika2] = 0;
					elimwzb1(as);
					swprintf(st+wcslen(st), L" %d: %s/%d.,", i+1, as, kilp->p_sija(i+1));
					}
				}
			st[wcslen(st)-1] = 0;
			putfld(tulprm, st, fld[2].pos, 80, 0, 0);
			}
		}
	return(nr);
	}
#if defined(_BORLANDC_)
#pragma warn +par
#endif

//static void outtulos_csv(INT l, kilptietue *kilp, INT sj, INT piste, INT32 karkitls, wchar_t co)
static void outtulos_csv(kilptietue *kilp, INT sj, INT32 karkitls, tulostusparamtp *tulprm)
	{
	wchar_t rivi[8020], st[62], tas[16], erotin;
	INT  piste, i_va, i_pv;
	INT32 tls;
	static INT32 tkarki;
	static INT sj1;

	if (kilpparam.kilplaji == L'N' || kilpparam.kilplaji == L'P') {
		outtulos_csv_5ott(kilp, sj, karkitls, tulprm);
		return;
		}

	piste = tulprm->piste;
	erotin = tulprm->erotin;
#if defined(MAKI)

	if (k_pv == makipv && !aikaerot) {
	   if (sj == 1) {
			if (piste == 0)
				tkarki = yhtpisteet(kilp, nkierros[kilp->Sarja()]);
			else if (piste % 2)
				tkarki = hyppypisteet(kilp, piste/2);
			}
		}
	else
#endif
		tkarki = karkitls;
		if (sj == 1) {
		if (tkarki == 1)
			tkarki = 0;
		}
	if (sj)
		sj1 = sj;
	memset(rivi, 0, sizeof(rivi));
	kilp->nimi(st, 60, tulprm->tulmuot.vaihdanimet != 0);
	if (k_pv > 0 && tulprm->yhttuljarj)
		tls = kilp->ytulos(k_pv);
	else
		tls = kilp->tulos2(piste, 2);
	if (tls > 0 && kilp->hyv()) {
#if defined(MAKI)
		if (k_pv == makipv && !aikaerot) {
			if (!piste) {
				format1(yhtpisteet(kilp, nkierros[kilp->Sarja()]), tas, -1, 0);
				}
			else if (!(piste % 2)) {
				format1(kilp->pituus[piste/2-1], tas, -1, 0);
				}
			else {
				format1(hyppypisteet(kilp, piste/2), tas, -1, 0);
				}
			}
		else
#endif
			{

			aikatowstr_cols_n(tas, tls, 0, L',', pvparam[k_pv].laika2);
			}
		}
	else {
		if (piste <= kilpparam.valuku)
			tas[0] = kilp->tark(k_pv);
		else
			tas[0] = L' ';
		tas[1] = 0;
		sj1 = 0;
		}
	if ((tulprm->optiot & 0x00800000) == 0)
		swprintf(rivi, L"%s%c", Sarjat[kilp->Sarja()].sarjanimi, erotin);
	swprintf(rivi+wcslen(rivi), L"%d%c%s%c%ld%c%s",
		kilp->id(), erotin, st, erotin,	kilp->lisno[0], erotin, kilp->seuratieto(0));
	rivi[wcslen(rivi)] = erotin;
	wcscpy(rivi+wcslen(rivi), kilp->maa);
	if (!tulprm->kaikkiva && tulprm->tulostettava != L'Y')
		swprintf(rivi+wcslen(rivi), L"%c%d%c%s", erotin, sj1, erotin, tas);
	if (sarjayhdfl) {
		swprintf(rivi+wcslen(rivi), L"%c%d", erotin, kilp->p_sija(0));
		}
	if (Sarjat[kilp->Sarja()].lsak[k_pv])
		swprintf(rivi+wcslen(rivi), L"%c[%s]", erotin, kilp->asakkoStr(k_pv));
	if (wcswcind(kilpparam.kilplaji, L"SB") >= 0) {
		rivi[wcslen(rivi)] = erotin;
		_ltow(kilp->pv[k_pv].badge[0], rivi+wcslen(rivi),10);
		}
	if (tulprm->kaikkiva || tulprm->tulostettava == L'Y') {
		wchar_t *p;
		i_va = -1;

		if (tulprm->tulostettava == L'Y')
			p = tulprm->pistelista;
		while (i_va <= kilpparam.valuku) {
			if (tulprm->tulostettava == L'Y') {
				if (!p || !*p)
					break;
				i_va = *p-2;
				p++;
				}
			else {
				i_va++;
				if (i_va > Sarjat[kilp->Sarja()].valuku[k_pv])
					break;
				}
			tls = kilp->tulos2(i_va, 2);
			if (tls) {
				if ((tulprm->kaikkisijat || i_va == tulprm->piste) && i_va >= 0)
					swprintf(rivi+wcslen(rivi), L"%c%d", erotin, kilp->p_sija(i_va));
				aikatowstr_cols_n(tas, tls, 0, L',', pvparam[k_pv].laika2);
				swprintf(rivi+wcslen(rivi), L"%c%s", erotin, tas);
				}
			else {
				if ((tulprm->kaikkisijat || i_va == tulprm->piste) && i_va >= 0)
					swprintf(rivi+wcslen(rivi), L"%c %c%c", erotin, erotin, i_va ? L' ' : kilp->tark(k_pv));
				else
					swprintf(rivi+wcslen(rivi), L"%c%c", erotin, i_va ? L' ' : kilp->tark(k_pv));
				}
			}
		}
	else if (!tulprm->kaikkiva && tulprm->kohde == L'R' && k_pv > 0) {
		for (i_pv = 0; i_pv <= k_pv; i_pv++) {
			if ((tls = kilp->tulos_pv(i_pv, true)) != 0) {
				aikatowstr_cols_n(tas, tls, 0, L',', pvparam[k_pv].laika2);
				swprintf(rivi+wcslen(rivi), L"%c%d%c%s", erotin, kilp->sija(i_pv), erotin, tas);
				}
			else
				swprintf(rivi+wcslen(rivi), L"%c %c%c", erotin, erotin, kilp->tark(i_pv));
			}
		}
	else if (tulprm->kaikkiva) {
#if defined(MAKI)
		if (k_pv == makipv && !aikaerot) {
			}
		else
#endif
			{
			for (i_va = 1; i_va <= Sarjat[kilp->Sarja()].valuku[k_pv]; i_va++) {
				tls = kilp->tulos2(i_va, 2);
				if (tls) {
					aikatowstr_cols_n(tas, tls, 0, L',', pvparam[k_pv].laika2);
					sj1 = kilp->p_sija(i_va);
					if (tulprm->kaikkisijat)
						swprintf(rivi+wcslen(rivi), L"%c%s%c%d", erotin, tas, erotin, sj1);
					else
						swprintf(rivi+wcslen(rivi), L"%c%s%", erotin, tas);
					}
				else {
					if (tulprm->kaikkisijat)
						swprintf(rivi+wcslen(rivi), L"%c %c ", erotin, erotin);
					else
						swprintf(rivi+wcslen(rivi), L"%c ", erotin);
					}
				}
			}
		}
	tulprm->writeline(rivi);
	}

static void htmltulos(kilptietue *kilp, int sj1, int sarja, tulostusparamtp *tulprm);

static void outtulos_txt(INT l, kilptietue *kilp, INT sj, INT piste, INT32 karkitls, tulostusparamtp *tulprm)
	{
//	wchar_t rivi[8020], st[62], tas[16];
//	INT len,pos;
//	INT32 tls;
//	static INT32 tkarki;
//   static INT sj1;

	if (kilpparam.kilplaji == L'N' || kilpparam.kilplaji == L'P') {
		outtulos_5ott(l, kilp, sj, piste, karkitls, tulprm);
		return;
		}

	htmltulos(kilp, sj, kilp->Sarja(), tulprm);
	return;

/*

#if defined(MAKI)

	if (k_pv == makipv && !aikaerot) {
	   if (sj == 1) {
			if (piste == 0)
				tkarki = yhtpisteet(kilp, nkierros[kilp->Sarja()]);
			else if (piste % 2)
				tkarki = hyppypisteet(kilp, piste/2);
			}
		}
	else
#endif
		tkarki = karkitls;
		if (sj == 1) {
		if (tkarki == 1)
			tkarki = 0;
		}
	  wmemset(rivi, L' ', sizeof(rivi)/2-1);
	  rivi[sizeof(rivi)/2-1] = 0;
	  if (sj) putintrightw(sj, rivi+p_fld[0].pos+p_fld[0].len-1);
	  if (p_fld[1].len)
		 putintrightw(kilp->id(), rivi+p_fld[1].pos+p_fld[1].len-1);
		len = min(wcslen(kilp->nimi(st, 60, false)), p_fld[2].len);
		wmemcpy(rivi+p_fld[2].pos, kilp->nimi(st, 60, false), len);
		if (p_fld[3].len) {
			st[0] = 0;
		 if (kilp->lisno[0] > 0 && kilp->lisno[0] < 1000000L)
			_ltow(kilp->lisno[0], st, 10);
		 len = min(wcslen(st), p_fld[3].len);
		 wmemcpy(rivi+p_fld[3].pos+p_fld[3].len-len, st, len);
		 }
	  len = min(wcslen(kilp->seuratieto(0)), p_fld[4].len);
	  wmemcpy(rivi+p_fld[4].pos, kilp->seuratieto(0), len);
	  if ((tls = kilp->tulos2(piste, 2)) > 0 && kilp->hyv()) {

#if defined(MAKI)
			if (k_pv == makipv && !aikaerot) {
				if (p_fld[5].len) {


					if (!piste) {
						format1(yhtpisteet(kilp, nkierros[kilp->Sarja()]), tas, 5, 0);
						format1(tkarki-yhtpisteet(kilp, nkierros[kilp->Sarja()]), st, 5, 0);
						if (sj > 1)
							st[0] = '-';
						}
					else if (!(piste % 2)) {
						format1(kilp->pituus[piste/2-1], tas, 5, 0);
						wcscpy(st, "     ");
						}
					else {
						format1(hyppypisteet(kilp, piste/2), tas, 5, 0);
						format1(tkarki-hyppypisteet(kilp, piste/2), st, 5, 0);
						if (sj > 1)
							st[0] = '-';
						}
					len = min(5, p_fld[5].len);
					memcpy(rivi+p_fld[5].pos, tas, len);
					if (p_fld[6].len >= 5)
						memcpy(rivi+p_fld[6].pos, st, 5);
					}
				}
			else
#endif
		{

		  if (p_fld[5].len) {
			len = min(pvparam[k_pv].laika2, p_fld[5].len);
			wmemcpy(rivi+p_fld[5].pos, AIKATOWSTRS(st, tls,0)+pvparam[k_pv].laika2-len, len);
			}
		 if (p_fld[6].len) {
			if (pvparam[k_pv].laika2 == 10 && p_fld[6].len <= 8) {
			   len = p_fld[6].len;
			   pos = pvparam[k_pv].laika2-len-2;
			   }
			else {
			   len = min(pvparam[k_pv].laika2, p_fld[6].len);
			   pos = pvparam[k_pv].laika2-len;
			   }
			if (sj == 1)
			   wmemcpy(rivi+p_fld[6].pos, AIKATOWSTRS(st, tls,0)+pos, len);
			else
			   wmemcpy(rivi+p_fld[6].pos, AIKATOWSTRS(st, tls-tkarki,0)+pos, len);
			}
		 }
	  }

	  if (!kilp->hyv())
		 rivi[p_fld[5].pos+p_fld[5].len] = kilp->tark(k_pv);
#if defined(MAKI)
		if (k_pv != makipv || aikaerot)
#endif
	  if (p_fld[5].len && kilp->tulos2(piste) == 0 && kilp->hyv() &&
		 (tls = kilp->tulos1(piste, maaliajat[Sarjat[kilp->Sarja()].mno[k_pv]-1])) > 0) {
		 rivi[p_fld[5].pos-1] = L'(';
		 rivi[p_fld[5].pos+p_fld[5].len] = L')';
		 len = min(pvparam[k_pv].laika2, p_fld[5].len);
		 wmemcpy(rivi+p_fld[5].pos, AIKATOWSTRS(st, tls, 0)+pvparam[k_pv].laika2-len, len);
		 }
	  if (kilpparam.lsakmax > 0 && p_fld[7].len) {
		 swprintf(tas, L"(%s)", kilp->pv[k_pv].asakot);
		 putfldstr(rivi, tas, p_fld[7].pos, p_fld[7].len, p_fld[7].oik);
		 }
	  if (kilpparam.lsakmax == 0 && p_fld[7].len && kilp->pv[k_pv].tasoitus) {
		 _itow(kilp->pv[k_pv].tasoitus, tas, 10);
		 putfldstr(rivi, tas, p_fld[7].pos, p_fld[7].len, p_fld[7].oik);
		 }
#ifdef MCPIST
	  if (p_fld[8].len && sj > 0 && sj < 31) {
		 _itow(mcpist[sj-1], st, 10);
		 putfldstr(rivi, st, p_fld[8].pos, p_fld[8].len, p_fld[8].oik);
		 }
#endif
	  swprintf(tas, L"%-10.10s ", Sarjat[kilp->Sarja()].sarjanimi);
	  tulprm->writeline(rivi);
*/
   }

static void outtulos(INT l, kilptietue *kilp, INT sj, INT piste, INT32 karkitls, tulostusparamtp *tulprm)
	{
	wchar_t rivi[8020], st[62], tas[16];
	INT len,pos;
	INT32 tls;
	static INT32 tkarki;
   static INT sj1;

	if (kilpparam.kilplaji == L'N' || kilpparam.kilplaji == L'P') {
		outtulos_5ott(l, kilp, sj, piste, karkitls, tulprm);
		return;
		}

#if defined(MAKI)

	if (k_pv == makipv && !aikaerot) {
	   if (sj == 1) {
			if (piste == 0)
				tkarki = yhtpisteet(kilp, nkierros[kilp->Sarja()]);
			else if (piste % 2)
				tkarki = hyppypisteet(kilp, piste/2);
			}
		}
	else
#endif
		tkarki = karkitls;
		if (sj == 1) {
		if (tkarki == 1)
			tkarki = 0;
		}
	  wmemset(rivi, L' ', sizeof(rivi)/2-1);
	  rivi[sizeof(rivi)/2-1] = 0;
	  if (sj) putintrightw(sj, rivi+dspflds[F_SJ].pos+dspflds[F_SJ].len-1);
	  if (dspflds[F_KNO].len)
		 putintrightw(kilp->id(), rivi+dspflds[F_KNO].pos+dspflds[F_KNO].len-1);
		len = min((int)wcslen(kilp->nimi(st, 60, false)), dspflds[F_NIMI].len);
		wmemcpy(rivi+dspflds[F_NIMI].pos, kilp->nimi(st, 60, false), len);
		if (dspflds[F_LISNO].len) {
			st[0] = 0;
		 if (kilp->lisno[0] > 0 && kilp->lisno[0] < 1000000L)
			_ltow(kilp->lisno[0], st, 10);
		 len = min((int)wcslen(st), dspflds[F_LISNO].len);
		 wmemcpy(rivi+dspflds[F_LISNO].pos+dspflds[F_LISNO].len-len, st, len);
		 }
		len = min((int)wcslen(kilp->seuratieto(0)), dspflds[F_SRA].len);
	  wmemcpy(rivi+dspflds[F_SRA].pos, kilp->seuratieto(0), len);
	  if ((tls = kilp->tulos2(piste, 2)) > 0 && kilp->hyv()) {

#if defined(MAKI)
			if (k_pv == makipv && !aikaerot) {
				if (dspflds[F_TLS].len) {


					if (!piste) {
						format1(yhtpisteet(kilp, nkierros[kilp->Sarja()]), tas, 5, 0);
						format1(tkarki-yhtpisteet(kilp, nkierros[kilp->Sarja()]), st, 5, 0);
						if (sj > 1)
							st[0] = '-';
						}
					else if (!(piste % 2)) {
						format1(kilp->pituus[piste/2-1], tas, 5, 0);
						wcscpy(st, "     ");
						}
					else {
						format1(hyppypisteet(kilp, piste/2), tas, 5, 0);
						format1(tkarki-hyppypisteet(kilp, piste/2), st, 5, 0);
						if (sj > 1)
							st[0] = '-';
						}
					len = min(5, dspflds[F_TLS].len);
					memcpy(rivi+dspflds[F_TLS].pos, tas, len);
					if (dspflds[F_ERO].len >= 5)
						memcpy(rivi+dspflds[F_ERO].pos, st, 5);
					}
				}
			else
#endif
		{

		  if (dspflds[F_TLS].len) {
			len = min((int)pvparam[k_pv].laika2, dspflds[F_TLS].len);
			wmemcpy(rivi+dspflds[F_TLS].pos, AIKATOWSTRS(st, tls,0)+pvparam[k_pv].laika2-len, len);
			}
		 if (dspflds[F_ERO].len) {
			if (pvparam[k_pv].laika2 == 10 && dspflds[F_ERO].len <= 8) {
			   len = dspflds[F_ERO].len;
			   pos = pvparam[k_pv].laika2-len-2;
			   }
			else {
			   len = min((int)pvparam[k_pv].laika2, dspflds[F_ERO].len);
			   pos = pvparam[k_pv].laika2-len;
			   }
			if (sj == 1)
			   wmemcpy(rivi+dspflds[F_ERO].pos, AIKATOWSTRS(st, tls,0)+pos, len);
			else
			   wmemcpy(rivi+dspflds[F_ERO].pos, AIKATOWSTRS(st, tls-tkarki,0)+pos, len);
			}
		 }
	  }

	  if (!kilp->hyv())
		 rivi[dspflds[F_TLS].pos+dspflds[F_TLS].len] = kilp->tark(k_pv);
#if defined(MAKI)
		if (k_pv != makipv || aikaerot)
#endif
	  if (dspflds[F_TLS].len && kilp->tulos2(piste) == 0 && kilp->hyv() &&
		 (tls = kilp->tulos1(piste, maaliajat[Sarjat[kilp->Sarja()].mno[k_pv]-1])) > 0) {
		 rivi[dspflds[F_TLS].pos-1] = L'(';
		 rivi[dspflds[F_TLS].pos+dspflds[F_TLS].len] = L')';
		 len = min((int)pvparam[k_pv].laika2, dspflds[F_TLS].len);
		 wmemcpy(rivi+dspflds[F_TLS].pos, AIKATOWSTRS(st, tls, 0)+pvparam[k_pv].laika2-len, len);
		 }
	  if (kilpparam.lsakmax > 0 && dspflds[F_SAK].len) {
		 swprintf(tas, L"(%s)", kilp->asakkoStr(k_pv));
		 putfldstr(rivi, tas, dspflds[F_SAK].pos, dspflds[F_SAK].len, dspflds[F_SAK].oik);
		 }
	  if (kilpparam.lsakmax == 0 && dspflds[F_SAK].len && kilp->pv[k_pv].tasoitus) {
		 _itow(kilp->pv[k_pv].tasoitus, tas, 10);
		 putfldstr(rivi, tas, dspflds[F_SAK].pos, dspflds[F_SAK].len, dspflds[F_SAK].oik);
		 }
#ifdef MCPIST
	  if (dspflds[F_PIST].len && sj > 0 && sj < 31) {
		 _itow(mcpist[sj-1], st, 10);
		 putfldstr(rivi, st, dspflds[F_PIST].pos, dspflds[F_PIST].len, dspflds[F_PIST].oik);
		 }
#endif
   if (l >= 0 && l+3 < ySize-4)
	  viwrrectw(l+3,0,l+3,79,rivi,7,0,0);
   }

static void tekstitulos(tulostusparamtp *tulprm, wchar_t *prs, kilptietue *kilp, int piste, int sj1, int karkitls)
	{
	wchar_t as[20], st[60], sjst[10], srastr[LSEURA+1], skstr[20];
	int tls;
	bool lehtierot, lehtimuoto = true, lehtivaihdanm, lehtipitkat, lehtipilkut = true;;

	lehtierot = (tulprm->optiot & 0x100000) != 0;
	lehtivaihdanm = (tulprm->optiot & 0x200000) != 0;
	lehtipitkat = (tulprm->optiot & 0x40000) != 0;
	lehtipilkut = (tulprm->optiot & 0x80000) != 0;
	sjst[0] = 0;
	wcscpy(as, L"--");
	if (tulprm->yhttuljarj && tulprm->vaihelkm == 2) {
		if ((tls = kilp->ytulos(k_pv, 2)) != 0) {
			if (tulprm->tulmuot.nollakarki) {
				tls -= karkitls;
				AIKATOWSTRS(as, tls, 0);
				as[pvparam[k_pv].laika2];
				}
			else
				kilp->ytulos_wstr_cols_n(as, k_pv, tulprm->tulmuot.des_sep, tulprm->tulmuot.pistedesim,
					tulprm->ypJrj, pvparam[k_pv].laika2, 2);
			if (sj1)
				_itow(sj1, sjst, 10);
			}
		}
	else {
		switch (kilp->tark(k_pv)) {
			case L'-' :
			case L'I' :
			case L'T' :
				tls = kilp->tulos2(piste, 2);
				if (tulprm->tulmuot.nollakarki || (lehtierot && sj1 > 1))
					tls -= karkitls;
				AIKATOWSTRS(as, tls, 0);
				if (sj1)
					_itow(sj1, sjst, 10);
				break;
			default :
				kilp->tarkstr(as, k_pv, false, 10, tulprm->language);
				break;
			}
		}
	as[pvparam[k_pv].laika2] = 0;
	elimwzb1(as);
	if ((tulprm->tulmuot.nollakarki || lehtierot) && sj1 > 1) {
		wmemmove(as+1,as,pvparam[k_pv].laika2+1);
		as[0] = L'+';
		}
	kilp->nimi(st, 38, lehtivaihdanm);
	wcsncpy(srastr, kilp->seuratieto(0, !lehtipitkat), sizeof(srastr)/2);
	srastr[sizeof(srastr)/2-1] = 0;
	skstr[0] = 0;
	if (Sarjat[kilp->Sarja()].lsak[k_pv])
		swprintf(skstr, L" (%s)", kilp->asakkoStr(k_pv));
	if (tulprm->kaikkiva) {
		if (skstr[0] == L' ')
			skstr[0] = L'\t';
		swprintf(prs, L"%s\t%d\t%s\t%s\t%s%s",  sjst,
			kilp->id(), st, srastr, as, skstr);
		for (int is = 0; is < Sarjat[kilp->Sarja()].valuku[k_pv]; is++) {
			wcscat(prs, L"\t");
			if ((tls = kilp->p_aika(is+1)) != 0) {
				_itow(kilp->p_sija(is+1), sjst, 10);
				AIKATOWSTRS(as, tls, 0);
				as[pvparam[k_pv].laika2] = 0;
				elimwzb1(as);
				wcscat(prs, sjst);
				wcscat(prs, L"\t");
				wcscat(prs, as);
				}
			else
				wcscat(prs, L"\t");
			}
		wcscat(prs, L"\n");
		}
	else
		if (lehtimuoto) {
			if (sjst[0])
				wcscat(sjst, L")");
			if (lehtipilkut)
				swprintf(prs, L"%s %s, %s, %s%s, ",  sjst,
					st, srastr, as, skstr);
			else
				swprintf(prs, L"%s %s %s %s%s ",  sjst,
					st, srastr, as, skstr);
			}
		else {
			if (sjst[0])
				wcscat(sjst, L". ");
			swprintf(prs, L"%s(%d) %s, %s, %s%s, ",  sjst,
				kilp->id(), st, srastr, as, skstr);
			}
		tulprm->puts_f(prs);
		if (k_pv && tulprm->vaihelkm == 2 && (!piste || piste == kilpparam.valuku+1)) {
			as[0] = 0;
			st[0] = 0;
			if (!tulprm->yhttuljarj) {
				if (kilp->hyv() && kilp->p_aika(piste)) {
					if ((tls = kilp->ytulos( k_pv, 2)) != 0) {
						kilp->ytulos_wstr_cols_n(as, k_pv, tulprm->tulmuot.des_sep,
							tulprm->tulmuot.pistedesim, tulprm->ypJrj, pvparam[k_pv].laika2, 2);
						_itow(kilp->p_sija( kilpparam.valuku+1), sjst, 10);
						wcscat(sjst, L"./");
						}
					else {
						wcscpy(sjst, L"-/");
						wcscpy(as, L"--");
						}
					}
				}
			else {
				switch (kilp->tark(k_pv)) {
					case L'T' :
					case L'I' :
					case L'-' :
						AIKATOWSTRS(as, kilp->p_aika(0), 0);
						_itow(kilp->sija(k_pv), sjst, 10);
						wcscat(sjst, L"./");
						break;
					default :
						kilp->tarkstr(as, k_pv, false, 10, tulprm->language);
						break;
					}
				}
			as[pvparam[k_pv].laika2] = 0;
			elimwzb1(as);
			if (as[0]) {
				swprintf(prs, L"%s%s, ", sjst, as);
//				swprintf(prs, L"%s%s%s", sjst, as, lehtipilkut ? L", " : L" ");
				tulprm->puts_f(prs);
				}
			}
		tulprm->puts_f(L" ");
	}

#if defined(TAULU_2)
#define taulurivit 100
#define tauluqsize 8000

static wchar_t tauluval = L'E';

void opentaulu(void)
   {
	wchar_t *taulubuf;
   int er = 1;

	if ((taulubuf = malloc(tauluqsize+8)) != 0) {
	  if (!openport(taulu_com, taulu_irq, taulu_addr, taulu_baud, 4, tauluqsize,
		 taulubuf))
		 er = 0;
	  }
   if (er) {
	  writeerror_w(L"Tiedonsiirron avaaminen taululle ei onnistu", 0, true);
	  taulu_com = 0;
	  }
   }

static int taulu_av(int srj, int Va)
   {
   int n;
   wchar_t srjrv[20] = L"", rivi[40];

//   do {
      swprintf(srjrv, L"%7.7s ");
	  outquesize(taulu_com, &n);
	  if (!n) {
         if (Va > 0 && Va <= kilpparam.valuku) {
			swprintf(rivi, L"\x02%-10.10s  %2d. INTERMEDIARY\r\n",
			   Sarjat[srj].sarjanimi , Va);
            }
		 else if (Va == kilpparam.valuku+1) {
			swprintf(rivi, L"\x02%-10.10s    TOTAL TIME    \r\n"
                , Sarjat[srj].sarjanimi);
			}
         else {
				swprintf(rivi, L"\x02%-10.10s    FINISH        \r\n"
					 , Sarjat[srj].sarjanimi);
				}
			wrt_st(taulu_com, wcslen(rivi), rivi, &n);
//         break;
			}
		else {
				return(1);
			}
//      }
//   while (1);
	return(0);
	}

static void taulutulos(kilptietue *kilp, int Va, int sj, int l)
	{
	int n;
	wchar_t rivi[80], as[16], st[40];
	INT32 tls;
	static INT32 tkarki;

	if((tls = kilp->tulos2(Va, 2)) != 0 && kilp->hyv()) {
		if (sj == 1)
		 tkarki = tls;
	  else if (tauluval == L'E')
		 tls -= tkarki;
	  AIKATOWSTRS(as, tls, 0);
	  as[pvparam[k_pv].pvparam[k_pv].laika2] = 0;
	  elimwz(as);
	  wmemmove(as, as+pvparam[k_pv].laika2-7, 8);
	  if (l != 1 && tauluval == L'E') {
		 if (as[2] == L' ') as[2] = L'+';
		 else if (as[1] == L' ') as[1] = L'+';
		 else as[0] = L'+';
		 }
	  }
	else
	  wcscpy(as, L"        ");
	swprintf(rivi, L"%2d %-15.15s %3.3s %7.7s\r\n",
			sj, kilp->nimi(st, 16, false), kilp->seuratieto(0), as);
   wrt_st(taulu_com, wcslen(rivi), rivi, &n);
   }

#endif

//static void htmltulos(kilptietue *kilp, int day, wchar_t cd, int piste, int sj1, int sjfl, int sarja)
static void htmltulos(kilptietue *kilp, int sj1, int sarja, tulostusparamtp *tulprm)
{
	wchar_t st[200], prs[20000];
	static INT32 karkitls = 0;
	INT l, pistefl = 0, sjfl, n_knt = 30, opt = 0, vaslisa = 0;
	bool toistotehty = false, taulufl;
	FldFrmtTp *fld;
	FldFrmtTp *p_fld = (FldFrmtTp *)tulprm->p_fld;

	wchar_t *txts[MAXLANG][5] = {{L"Hyl.", L"Kesk.", L"Ohit.", L"Virhe", L"EiAikaa"},
								{L"DQ", L"DNF", L"Skip", L"Miss", L"Excl."}
							 /*,{L" Diskv.   ", L" Avbr.    ", L"Ei startat", L"Till‰gg", L"respit"}*/};

   if (tulprm->kohde == L'I') {
	   vaslisa = 12;
	   }
	if (tulprm->tulostettava == L'P') {
		pistefl = tulprm->yhttuljarj ? 2 : 1;
		}
	taulufl = tulprm->kohde != L'I' && tulprm->tulmuot.tauluhtml != 0;
	sjfl = tulprm->ixjarj || ((tulprm->ixjarj % 10) > 1 && (tulprm->ixjarj % 10) < 4);
	prs[0] = 0;

	if (kilpparam.alalaji == L'D' && (tulprm->optiot & 0x00200000)) {
		if (sj1 == 1)
			karkikierr = kilp->pv[k_pv].pvpisteet[0];
		}
	else
		karkikierr = 0;

	if (taulufl) {
/*		if ((tulprm->rivi+tulprm->tulmuot.riviryhma-1) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
			wcscpy(prs, L"<tr class='aa'>");
		else
			wcscpy(prs, L"<tr class='ab'>");
*/
		wcscpy(prs, L"<tr>");
		}
	else {
		wmemset(prs, L' ', sizeof(prs)/2);
		prs[sizeof(prs)/2-1] = 0;
		}
	st[199] = 0;

//	if (!tulprm->yhttuljarj ||
//		(Sarjat[sarja].pisteytulos() && Sarjat[sarja].luonne[k_pv] == 2))
	if (!tulprm->yhttuljarj)
		karkitls = pkarki[sarja][tulprm->piste];
	else
		karkitls = pkarki[sarja][kilpparam.valuku+1];
	if (tulprm->kohde == L'I') {
		kilp->SarjaNimi(k_pv, st);
		wmemcpy(prs, st, wcslen(st));
		}
	for (int i_knt = 0; i_knt < tulprm->n_aFlds; i_knt++) {
		int pos;
		int id = tulprm->aFld[i_knt].id;
		int race = tulprm->aFld[i_knt].race;
		int pst = tulprm->aFld[i_knt].point;

		fld = p_fld + id;
		memset(st, 0, sizeof(st));
		opt = 0;
		switch (id) {
			case F_SJ :
				opt = 2;
				if (pistefl == 2 || (tulprm->yhttuljarj && Sarjat[sarja].pisteytulos()))
					opt |= 8;
				break;
			case F_TLS :
				if (pistefl == 2)
					continue;
				opt = tulprm->yhttuljarj ? 2 : 0;
				break;
			case F_ERO :
				if (pistefl && tulprm->yhttuljarj && Sarjat[sarja].pisteytulos() &&
					Sarjat[kilp->Sarja()].luonne[k_pv] == 2) {
					opt = (tulprm->yhttuljarj ? 2 : 0) + 8 * karkitls;
//					opt = 2;
//					opt = 8 * karkitls;
					break;
					}
				if (pistefl)
					continue;
				opt = (tulprm->yhttuljarj ? 2 : 0) + 8 * karkitls;
				break;
			case F_NIMI :
				opt = tulprm->tulmuot.vaihdanimet;
				break;
			case F_SAK :
				if (pistefl)
					continue;
				opt = Sarjat[kilp->Sarja()].rrtas != 0 ? fld->len : 0;
				break;
			case F_TARK :
//				if (fld->pos == p_fld[F_TLS].pos && p_fld[F_TLS].len)
//					opt |= 4;
				if (pistefl == 2)
					continue;
				if (tulprm->yhttuljarj)
					opt |= 2;
				break;
			case F_OSATLS :
				if (pistefl == 2) {
					if (pst <= kilpparam.valuku)
						opt |= 4;
					break;
					}
				if (tulprm->vaihelkm == 1 && !tulprm->kaikkiva && tulprm->tulostettava != L'Y')
					continue;
				if (tulprm->vaihelkm == 1)
					opt |= 2;
				else
					opt |= tulprm->yhttuljarj ? 1 : 0;
				opt |= (tulprm->optiot & 16);
				break;
			case F_OSASJ :
				if (pistefl)
					continue;
				if ((tulprm->vaihelkm == 1 && !tulprm->kaikkiva && tulprm->tulostettava != L'Y')
					|| ((tulprm->kaikkiva || tulprm->tulostettava == L'Y') && !tulprm->kaikkisijat))
//					|| ((tulprm->kaikkiva || tulprm->tulostettava == L'Y') && (tulprm->optiot & 24) != 8))
					continue;
				if (tulprm->vaihelkm == 1)
					opt |= 2;
				else
					opt |= tulprm->yhttuljarj ? 1 : 0;
				opt |= 4;                           // piste sijoitukseen
				break;
			case F_PIST :
				if (pistefl == 2 && tulprm->vaihelkm > 1)
					continue;
				if (!tulprm->yhttuljarj && tulprm->vaihelkm == 1)
					opt |= 4;
				break;
			}
		if (tulprm->vaihelkm > 1 && id == F_ERO &&
			!(pistefl && tulprm->yhttuljarj && Sarjat[sarja].pisteytulos() &&
				Sarjat[kilp->Sarja()].luonne[k_pv] == 2))
			continue;
		if (tulprm->piilotatulos && (id == F_TLS || id == F_ERO || id == F_OSATLS))
			continue;
		if (tulprm->piilotasijat && (id == F_SJ ||id == F_OSASJ))
			continue;
//			if (kilpparam.alalaji == L'S' && k_pv == 0 && tulprm->vaihelkm == 3)
//				continue;
		if ((Sarjat[kilp->Sarja()].piilotatulos && (id == F_TLS || id == F_ERO || id == F_OSATLS)) ||
			(Sarjat[kilp->Sarja()].piilotatulos && (id == F_SJ || id == F_OSASJ)))
			st[0] = 0;
		else
			kilp->strval(tulprm, st, min(199, fld->len), (pistefl == 2 && id == F_OSATLS) ? F_PIST : id, race, pst, sj1, opt);
		if (taulufl) {
/*			if (fld->oik)
				wcscat(prs, L"<td align=right>");
			else
				wcscat(prs, L"<td align=left>");
*/
			wcscat(prs, L"<td>");
			wcscat(prs, st);
			wcscat(prs, L"</td>");
			}
		else {
			l = wcslen(st);
			pos = tulprm->aFld[i_knt].pos + vaslisa;
			if (fld->oik && l < (int) fld->len)
				pos += fld->len - l;
			if (pos + l > (int)sizeof(prs)/2-3)
				break;
			wmemcpy(prs+pos, st, l);
			}
		}
	if (!taulufl) {
		for (wchar_t *p = prs+sizeof(prs)/2-2; *p == L' '; p--)
			*p = 0;
		wcscat(prs, L"\n");
		}
	else
		wcscat(prs, L"</tr>\n");
	if (tulprm->kohde == L'I')
		tulprm->writeline(prs);
	else
		tulprm->writehtml(prs);
}

#ifdef EI_OLE
static void htmltulos(kilptietue *kilp, int sj1, int sarja, tulostusparamtp *tulprm)
{
	wchar_t st[200], st2[100], prs[4000];
	static INT32 karkitls = 0;
	INT l, pistefl = 0, sjfl, n_knt = 30, pst = 0, opt = 0, vaslisa = 0;
	int kntjrj[30];
	bool toistotehty = false, taulufl;
	FldFrmtTp *fld;
	FldFrmtTp *p_fld = (FldFrmtTp *)tulprm->p_fld;
	INT32 pstm[N_PV], yhtpst;

	wchar_t *txts[MAXLANG][5] = {{L"Hyl.", L"Kesk.", L"Ohit.", L"Virhe", L"EiAikaa"},
								{L"DQ", L"DNF", L"Skip", L"Miss", L"Excl."}
							 /*,{L" Diskv.   ", L" Avbr.    ", L"Ei startat", L"Till‰gg", L"respit"}*/};

	if (kilpparam.kilplaji == L'N' || kilpparam.kilplaji == L'P') {
		meripisteet(kilp, pstm, &yhtpst);
		}
   if (tulprm->kohde == L'I') {
	   vaslisa = 12;
	   }
	if (tulprm->tulostettava == L'P') {
		pistefl = tulprm->yhttuljarj ? 2 : 1;
		}
	taulufl = tulprm->kohde != L'I' && tulprm->tulmuot.tauluhtml != 0;
	sjfl = tulprm->ixjarj || ((tulprm->ixjarj % 10) > 1 && (tulprm->ixjarj % 10) < 4);
	prs[0] = 0;

	if (kilpparam.alalaji == L'D' && (tulprm->optiot & 0x00200000)) {
		if (sj1 == 1)
			karkikierr = kilp->pv[k_pv].pvpisteet[0];
		}
	else
		karkikierr = 0;

	hae_kenttajarj(p_fld, kntjrj, &n_knt);
	if (taulufl) {
/*		if ((tulprm->rivi+tulprm->tulmuot.riviryhma-1) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
			wcscpy(prs, L"<tr class='aa'>");
		else
			wcscpy(prs, L"<tr class='ab'>");
*/
		wcscpy(prs, L"<tr>");
		}
	else {
		wmemset(prs, L' ', sizeof(prs)/2);
		prs[sizeof(prs)/2-1] = 0;
		}
	st[199] = 0;

	if (!tulprm->yhttuljarj)
		karkitls = pkarki[sarja][pst];
	else
		karkitls = pkarki[sarja][kilpparam.valuku+1];
	if (tulprm->kohde == L'I') {
		kilp->SarjaNimi(k_pv, st);
		wmemcpy(prs, st, wcslen(st));
		}
	for (int i_knt = 0; i_knt < n_knt; i_knt++) {
		int pos;

		fld = p_fld + kntjrj[i_knt];
		memset(st, 0, sizeof(st));
		opt = 0;
		switch (kntjrj[i_knt]) {
			case F_SJ :
				opt = 2;
				if (pistefl == 2)
					opt |= 8;
				break;
			case F_TLS :
				if (pistefl == 2)
					continue;
				opt = tulprm->yhttuljarj ? 2 : 0;
				break;
			case F_ERO :
				if (pistefl)
					continue;
				opt = (tulprm->yhttuljarj ? 2 : 0) + 8 * karkitls;
				break;
			case F_NIMI :
				opt = tulprm->tulmuot.vaihdanimet;
				break;
			case F_SAK :
				if (pistefl)
					continue;
				opt = Sarjat[kilp->Sarja()].rrtas != 0 ? fld->len : 0;
				break;
			case F_TARK :
//				if (fld->pos == p_fld[F_TLS].pos && p_fld[F_TLS].len)
//					opt |= 4;
				if (pistefl == 2)
					continue;
				if (tulprm->yhttuljarj)
					opt |= 2;
				break;
			case F_OSATLS :
				if (pistefl == 2)
					break;
				if (tulprm->vaihelkm == 1 && !tulprm->kaikkiva && tulprm->tulostettava != L'Y')
					continue;
				opt = tulprm->yhttuljarj ? 1 : 0;
				opt |= (tulprm->optiot & 16);
				break;
			case F_OSASJ :
				if (pistefl)
					continue;
				if ((tulprm->vaihelkm == 1 && !tulprm->kaikkiva && tulprm->tulostettava != L'Y')
					|| ((tulprm->kaikkiva || tulprm->tulostettava == L'Y') && !tulprm->kaikkisijat))
//					|| ((tulprm->kaikkiva || tulprm->tulostettava == L'Y') && (tulprm->optiot & 24) != 8))
					continue;
				opt = tulprm->yhttuljarj ? 1 : 0;
				opt |= 4;                           // piste sijoitukseen
				break;
			case F_PIST :
				if (pistefl == 2)
					continue;
				if (!tulprm->yhttuljarj && tulprm->vaihelkm == 1)
					opt |= 4;
				break;
			}
		if (tulprm->vaihelkm > 1 && kntjrj[i_knt] == F_ERO)
			continue;
		if ((tulprm->piilotatulos || Sarjat[kilp->Sarja()].piilotatulos) &&
			(kntjrj[i_knt] == F_TLS || kntjrj[i_knt] == F_ERO || kntjrj[i_knt] == F_OSATLS))
			continue;
		if ((tulprm->piilotasijat || Sarjat[kilp->Sarja()].piilotatulos) &&
			(kntjrj[i_knt] == F_SJ ||kntjrj[i_knt] == F_OSASJ))
			continue;
//			if (kilpparam.alalaji == L'S' && k_pv == 0 && tulprm->vaihelkm == 3)
//				continue;
		if ((kntjrj[i_knt] != F_OSATLS && kntjrj[i_knt] != F_OSASJ) || (tulprm->vaihelkm < 3 &&
			!tulprm->kaikkiva && tulprm->tulostettava != L'Y'))
			{
			kilp->strval(tulprm, st, min(199, fld->len), kntjrj[i_knt], k_pv, pst, sj1, opt);
			if (taulufl) {
/*				if (fld->oik)
					wcscat(prs, L"<td align=right>");
				else
					wcscat(prs, L"<td align=left>");
*/
				wcscat(prs, L"<td>");
				wcscat(prs, st);
				wcscat(prs, L"</td>");
				}
			else {
				l = wcslen(st);
				pos = fld->pos + vaslisa;
				if (fld->oik && l < (int) fld->len)
					pos += fld->len - l;
				if (pos + l > (int)sizeof(prs)/2-3)
					break;
				wmemcpy(prs+pos, st, l);
				}
			}
		else if (kntjrj[i_knt] == F_OSATLS || kntjrj[i_knt] == F_OSASJ) {
//			else if (tulprm->vaihelkm < 3 && !tulprm->kaikkiva && tulprm->tulostettava != L'Y')
			int knt, knt2;
			FldFrmtTp *fld1 = NULL;

			if (pistefl) {
				opt = 4;
				knt = F_PIST;
				}
			else if (tulprm->vaihelkm == 1 && (opt & 16)) {
				knt = kntjrj[i_knt];
				knt2 = F_OSATLS;
				fld1 = p_fld + knt2;
				}
			else {
				knt = kntjrj[i_knt];
				knt2 = F_OSATLS + F_OSASJ - kntjrj[i_knt];
				fld1 = p_fld + knt2;
				}
			if (tulprm->vaihelkm >= 3) {
				int npv;

				npv = ((tulprm->vaihelkm > 2 && k_pv == 0) ? Sarjat[sarja].viimpv+1 : k_pv+1);
				if (npv == 1 && tulprm->vaihelkm > 2 && k_pv == 0)
					npv = kilpparam.n_pv_akt;
				for (int i_pv = 0; !toistotehty && (i_pv < npv || (pistefl && i_pv == npv)); i_pv++) {
					st[0] = 0;
					st2[0] = 0;
					if (Sarjat[sarja].luonne[i_pv] >= 0) {
						if (i_pv == npv)
							opt = (opt & 3);
						kilp->strval(tulprm, st, min(199, fld->len), knt, i_pv, pst, sj1, opt);
						}
					if (!taulufl) {
						l = wcslen(st);
						pos = fld->pos + vaslisa + i_pv * fld->askel;
						if (fld->oik && l < (int) fld->len)
							pos += fld->len - l;
						if (pos + l > (int)sizeof(prs)/2-3)
							break;
						wmemcpy(prs+pos, st, l);
						}
					if (fld1 && fld1->len) {
						if (Sarjat[sarja].luonne[i_pv] >= 0)
							kilp->strval(tulprm, st2, min(199, fld1->len), knt2, i_pv, pst, sj1, opt);
						if (!taulufl) {
							l = wcslen(st2);
							pos = fld1->pos + vaslisa + i_pv * fld1->askel;
							if (fld1->oik && l < (int) fld1->len)
								pos += fld1->len - l;
							if (pos + l > (int)sizeof(prs)/2-3)
								break;
							wmemcpy(prs+pos, st2, l);
							}
						}
					if (taulufl) {
/*						if (fld->oik)
							wcscat(prs, L"<td align=right>");
						else
							wcscat(prs, L"<td align=left>");
*/
						wcscat(prs, L"<td>");
						wcscat(prs, st);
						wcscat(prs, L"</td>");
						if (fld1 && fld1->len) {
/*							if (fld->oik)
								wcscat(prs, L"<td align=right>");
							else
								wcscat(prs, L"<td align=left>");
*/
							wcscat(prs, L"<td>");
							wcscat(prs, st2);
							wcscat(prs, L"</td>");
							}
						}
					}
				toistotehty = true;
				}
			else if (tulprm->kaikkiva) {
				opt &= 0xffffffe;
				opt |= 2;
				for (int pst1 = 0; !toistotehty && pst1 < Sarjat[kilp->Sarja()].valuku[k_pv]+ (kilpparam.alalaji == L'D' ? 0 : (tulprm->optiot & 16)/16);
					pst1++) {
					if (tulprm->kaikkisijat) {
						kilp->strval(tulprm, st, min(199, fld->len), kntjrj[i_knt], k_pv, pst1+1, sj1, opt);
						if (!taulufl) {
							l = wcslen(st);
							pos = fld->pos + vaslisa + pst1 * fld->askel;
							if (fld->oik && l < (int) fld->len)
								pos += fld->len - l;
							if (pos + l > (int)sizeof(prs)/2-3)
								break;
							wmemcpy(prs+pos, st, l);
							}
						else {
/*							if (fld->oik)
								wcscat(prs, L"<td align=right>");
							else
								wcscat(prs, L"<td align=left>");
*/
							wcscat(prs, L"<td>");
							wcscat(prs, st);
							wcscat(prs, L"</td>");
							}
						}
					if (fld1->len) {
						kilp->strval(tulprm, st, min(199, fld1->len), knt2, k_pv, pst1+1, sj1, opt | (tulprm->optiot & 16));
						if (!taulufl) {
							l = wcslen(st);
							pos = fld1->pos + vaslisa + pst1 * fld1->askel;
							if (fld1->oik && l < (int) fld1->len)
								pos += fld1->len - l;
							if (pos + l > (int)sizeof(prs)/2-3)
								break;
							wmemcpy(prs+pos, st, l);
							}
						else {
/*							if (fld->oik)
								wcscat(prs, L"<td align=right>");
							else
								wcscat(prs, L"<td align=left>");
*/
							wcscat(prs, L"<td>");
							wcscat(prs, st);
							wcscat(prs, L"</td>");
							}
						}
					}
				toistotehty = true;
				}
			else if(tulprm->tulostettava == L'Y') {
				wchar_t *p;
				int pst1 = -1, nt = 0;

				p = tulprm->pistelista;
				while (pst1 <= kilpparam.valuku) {
					if (tulprm->tulostettava == L'Y') {
						if (!p || !*p)
							break;
						pst1 = *p-2;
						p++;
						}
					else {
						pst1++;
						if (pst1 > Sarjat[kilp->Sarja()].valuku[k_pv])
							break;
						}
					if (pst1 <= Sarjat[kilp->Sarja()].valuku[k_pv] &&
						kilp->p_aika(pst1) && (kilp->hyv() || (((kilp->tark(k_pv) == L'K' || kilp->tark(k_pv) == L'O')
							&& pst1 > 0)))) {
						kilp->strval(tulprm, st, min(199, fld->len), kntjrj[i_knt], k_pv, pst1, sj1, opt | 2*(pst1 > 0));
						if (!taulufl) {
							l = wcslen(st);
							pos = fld->pos + vaslisa + nt * fld->askel;
							if (fld->oik && l < (int) fld->len)
								pos += fld->len - l;
							if (pos + l > (int)sizeof(prs)/2-3)
								break;
							wmemcpy(prs+pos, st, l);
							}
						else {
/*							if (fld->oik)
								wcscat(prs, L"<td align=right>");
							else
								wcscat(prs, L"<td align=left>");
*/
							wcscat(prs, L"<td>");
							wcscat(prs, st);
							wcscat(prs, L"</td>");
							}
						nt++;
						}
					}
				toistotehty = true;
				}
			}
		}
	if (!taulufl) {
		for (wchar_t *p = prs+sizeof(prs)/2-2; *p == L' '; p--)
			*p = 0;
		wcscat(prs, L"\n");
		}
	else
		wcscat(prs, L"</tr>\n");
	if (tulprm->kohde == L'I')
		tulprm->writeline(prs);
	else
		tulprm->writehtml(prs);
}
#endif  // EI_OLE
// #endif // !MERI5

#ifdef SEURATUL
static INT onko_seura(INT srj, bool yhttul)
   {
   INT i, d, p = -1,lj = 0, lj1 = 0, retval = 0, lseura;
   wchar_t seura[LSEURA+2];
   kilptietue kilp;

   if (yhttul) {
	  lj = kilpparam.valuku+1;
	  lj1 = lj;
	  }
   lseura = kilpparam.seuralyh_on ? kilpparam.lseuralyh : kilpparam.lseura;
   EnterCriticalSection(&tall_CriticalSection);
   while (1) {
	  while (p < 0 && lj < lj1 + 5)  p = jalku[srj][lj++];
	  if ( p < 0 || lj > lj1 + 5) break;
	  d = p;
	  p = SeuraavaSrj(srj, lj1, p);
	  kilp.GETREC(d);
	  wmemcpy(seura, kilp.seuratieto(0), lseura);
	  seura[lseura] = 0;
	  for (i = 0; seuranimi[i][0] && i < SEURALISTA; i++)
		 if (wcscmpU(seuranimi[i], seura) == 0) {
			 retval = 1;
			 break;
			}
	  if (retval)
		  break;
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

static INT onko_piiri(INT srj, bool yhttul)
   {
   INT d, p = -1,lj = 0, lj1 = 0, piiri, retval = 0;
   kilptietue kilp;

   if (yhttul) {
	  lj = kilpparam.valuku+1;
	  lj1 = lj;
	  }
   EnterCriticalSection(&tall_CriticalSection);
	while (1) {
	  while (p < 0 && lj < lj1 + 5)  p = jalku[srj][lj++];
	  if ( p < 0 || lj > lj1 + 5) break;
	  d = p;
	  p = SeuraavaSrj(srj, lj1, p);
	  kilp.GETREC(d);
	  piiri = kilp.piiri;
	  if (piiri <= 0 || piiri > piiriluku) continue;
	  if (piirifl[piiri-1]) {
			 retval = 1;
			 break;
			}
	  if (retval)
		  break;
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }
#endif

static int onko_tarkastamatta(int srj)
   {
   int d, p, retval = 0;
   kilptietue kilp;

   EnterCriticalSection(&tall_CriticalSection);
   p = jalku[srj][0];
   while ( p >= 0) {
	  d = p;
	  p = SeuraavaSrj(srj, 0, p);
		kilp.GETREC(d);
	  if (kilp.tark(k_pv) == L'-' || kilp.tark(k_pv) == L'I') {
			 retval = 1;
			 break;
			}
	  if (retval)
		  break;
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

//#if defined(UINTI) || defined(MERI5)
static int onkosrjera(int srj, int era)
   {
   int d, p = -1, retval = 0;
   kilptietue kilp;

   EnterCriticalSection(&tall_CriticalSection);
   p = jalku[srj][0];
   while ( p >= 0) {
	  d = p;
	  p = SeuraavaSrj(srj, 0, p);
	  kilp.GETREC(d);
	  if (kilp.pv[k_pv].era == era) {
		 retval = 1;
		 break;
		 }
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);

   }
//#endif

int srjtullkm(int srj, tulostusparamtp *tulprm)
//int piste, int day, int ixjarj, wchar_t ct, int np)
{
	int n, piste;

	if (k_pv && tulprm->yhttuljarj) {
		n = nyilm[srj];
		piste = kilpparam.valuku+1;
		}
	else {
		n = nilm[srj];
		piste = tulprm->piste;
		}
   if (!tulprm->ixjarj) {
	  switch(tulprm->rajaus) {
		 case 'L' :
			n = ntulos[srj][0] + nhyl[srj] + nkesk[srj];
			if (k_pv && tulprm->yhttuljarj)
				n += (nyilm[srj] - nilm[srj] + neil[srj]);
			if (k_pv && tulprm->vaihelkm == 2 && tulprm->yhttuljarj)
				n = nilm[srj];
			break;
		 case 'T' :
			n = ntulos[srj][piste];
			break;
		 case 'P' :
			n = min(tulprm->viimsija ,ntulos[srj][tulprm->piste]);
			break;
		 case 'H' :
			n = nhyl[srj];
			break;
		 case 'K' :
			n = nkesk[srj];
			break;
		 case 'E' :
			n = neil[srj];
			break;
		 case 'A' :
			n = n_avoin(srj);
			break;
		 }
	  }
   else {
	  n = nameindex.nrec;
	  }
   return(n);
}

static int tlsSeuraava(int *p, int *d, int ixjarj, int *srj, int piste, int *lj, int lj1, bool init, int *kilpno, tulostusparamtp *tulprm)
{
	int valmis = 0, ok;
	long pst;
	static char keyn[MAXINDL+1], keyv[MAXINDL+1];
	static int ySj = 0;

	if (init)
		memset(keyn, 0, MAXINDL);
	switch (ixjarj) {
		case 0:
			while (*p < 0 && *lj <= lj1)  {
				*p = jalku[*srj][(*lj)++];
				}
			if ( *p < 0 || (*lj > lj1+1)) {
				valmis = 1;
				break;
				}
			*d = *p;
//			*p = JarjSeur(0, piste, *p);
			*p = SeuraavaSrj(*srj, piste, *p);
			break;
		case 1:
		case 2:
		case 3:
			if (init)
				ok = !searchix(&nameindex, keyn, d);
			else {
				ok = !nextix(&nameindex, keyn, d);
			if (!ok) {
				valmis = 1;
				}
			}
			break;
		case 4:
			if (init) {
				_swab((char *)srj, keyn, 2);
				memcpy(keyv, keyn, 2);
				ok = !searchix(&pstindex, keyn, d);
				}
			else {
				ok = !nextix(&pstindex, keyn, d);
			memcpy(&pst, keyn+2, 4);
			lswab(&pst);
			if (pst == 1000000000)
				ok = 0;
			if (memcmp(keyv, keyn, 2))
				ok = 0;
			if (!ok) {
				valmis = 1;
				}
			}
			break;
		case 5:
			if (init) {
				_swab((char *)srj, keyn, 2);
				memcpy(keyv, keyn, 2);
				ok = !searchix(&pstindex, keyn, d);
				}
			else {
				ok = !nextix(&pstindex, keyn, d);
			if (*(int *)(keyn+2) == 0)
				ok = 0;
			if (memcmp(keyv, keyn, 2))
				ok = 0;
			if (!ok) {
				valmis = 1;
				}
			}
			break;
		case 9:
			if (init) {
				ySj = 0;
				}
			if (!tulprm->ypJrj)
				*d = -1;
			else
				*d = tulprm->ypJrj[ySj++][1];
			if (*d <= 0) {
				valmis = 1;
				ySj = 0;
				}
			break;
		case 11:
		case 12:
		case 13:
			if (init)
				*kilpno = minkilpno;
			else
				(*kilpno)++;
			if ((*d = getpos(*kilpno, true)) <= 0)
				valmis = 1;
			else {
				kilptietue kilp;
				kilp.GETREC(*d);
				*kilpno = kilp.id();
				}
			break;
		}
	return(valmis);
}

//static void kirjhtmlotsikot(int *srj, int piste, int day, wchar_t cd)
static void kirjhtmlotsikot(int *srj, tulostusparamtp *tulprm)
{
	int na, piste;
	wchar_t prs[10000], stas[80];
	static const wchar_t *txts[2][10] = {{L"Piiri", L"Seura", L"Tilanne", L"L‰hti", L"Keskeytti", L"Hyl‰tty", L"Tls", L"Yht.", L"Sj.", L"Avoinna" },
						  {L"District", L"Club", L"Status", L"Started", L"DNF", L"DQ", L"Race", L"Total", L"Pos", L"Open" }};
   FldFrmtTp *p_fld = (FldFrmtTp *)tulprm->p_fld;

	piste = tulprm->piste;
	if (tulprm->yksihtml) {
// Piiriotsikot
		if (tulprm->piiritulokset) {
			swprintf(prs, L"<P> %s : ", txts[tulprm->language][0]);
			for (int piiri = 0; piiri < piiriluku; piiri++) {
				if (piirifl[piiri] && piirinimi[piiri][0]) {
					if (wcslen(prs) + wcslen(piirinimi[piiri]) < sizeof(prs)/2-6) {
						wcscat(prs, L" ");
						wcscat(prs, piirinimi[piiri]);
						}
					}
				}
			wcscat(prs, L"\n<p><p>");
			tulprm->writehtml(prs);
			}
// Seuraotsikot
		else if (tulprm->seuratulokset) {
			swprintf(prs, L"<P> %s : ", txts[tulprm->language][1]);
			for (int is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
				if (wcslen(prs) + wcslen(seuranimi[is]) >= sizeof(prs)/2-6) {
					wcscat(prs, L",\n");
					tulprm->writehtml(prs);
					}
				else
					wcscat(prs, L", ");
				wcscat(prs, seuranimi[is]);
				}
			wcscat(prs, L"\n<p><p>");
			tulprm->writehtml(prs);
			}
		if ((potsfl && tulprm->tulmuot.sarjalinkit) || tulprm->tulmuot.sarjalinkit == 2) {
			tulprm->writehtml(L"<p><div class=sarjalinkit>\n");
			for (int isrj = 0; isrj < sarjaluku+nsarjayhd; isrj++) {
				if ((!tulprm->sarjalista || tulprm->sarjalista[isrj]) && (ntulos[isrj][piste] ||
					((piste == 0 || piste > kilpparam.valuku) && nkesk[isrj]+nhyl[isrj] > 0))) {
					tulprm->writehtml(L"<a href=\"#");
					tulprm->writehtml(Sarjat[isrj].sarjanimi);
					tulprm->writehtml(L"\">");
					tulprm->writehtml(Sarjat[isrj].sarjanimi);
					tulprm->writehtml(L"</a>  \n");
					}
				}
			tulprm->writehtml(L"</div>\n");
			}
		tulprm->writehtml(L"\n<H3 class=sarjanimi><a name=\"");
		tulprm->writehtml(Sarjat[*srj].sarjanimi);
		tulprm->writehtml(L"\">");
		tulprm->writehtml(L"</a>");
		}
	else {
/*
		if (tulprm->merkit == L'A')
			tulprm->writehtml(L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
		else
			tulprm->writehtml(L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
		css_string(tulprm, 2);
		tulprm->writehtml(L"</head><body>\n");
*/
		tulprm->writehtml(L"<H3 class=sarjanimi>");
		}
	tulprm->writehtml(SarjaNimi(prs, sizeof(prs)/2, *srj, true, NULL));
	AlisarjaOts(stas+3, 80, tulprm);
	if (wcslen(stas+3) > 0) {
		wmemcpy(stas, L" - ", 3);
		tulprm->writehtml(stas);
		}
#ifdef MAKI
	if (makitulokset)
		tulprm->writehtml("</H3>\n");
	else
#endif
		{
		if (((piste == 0 && tulprm->tulmuot.matkafl &&
			(tulprm->tulmuot.matkafl == 2 || Sarjat[*srj].psarjanimi[0] == 0)) ||
			piste > kilpparam.valuku) && Sarjat[*srj].matka[k_pv][0]) {
			tulprm->writehtml(L" : ");
			if (k_pv == 1 && tulprm->yhttuljarj) {
				tulprm->writehtml(Sarjat[*srj].matka[0]);
				tulprm->writehtml(L" + ");
				}
			wcscpy(stas, Sarjat[*srj].matka[k_pv]);
			if (stas[wcslen(stas)-1] <= L'9') {
				wcscat(stas, L" km");
				if (Sarjat[*srj].tapa[k_pv][0] >= L'P') {
					swprintf(stas+wcslen(stas), L" (%s)", Sarjat[*srj].tapa[k_pv]);
					}
				}
			else if (wcswcind(stas[wcslen(stas)-1], L"VP") >= 0) {
				 wchar_t *p, stl[10] = L"";
				 for (p = stas+wcslen(stas)-1; p > stas+1 && *p > L'9'; p--) ;
				 if (*p <= L'9') {
					  wcsncpy(stl, p+1, 9);
					  p[1] = 0;
					  swprintf(p+1, L" km (%s)", stl);
					  }
				 }
			tulprm->writehtml(stas);
			}

		if (piste > 0 && piste <= kilpparam.valuku && Sarjat[*srj].va_matka[k_pv][piste-1]) {
			tulprm->writehtml(L" : ");
			tulprm->writehtml(Sarjat[*srj].va_matka[k_pv][piste-1]);
			if (Sarjat[*srj].va_matka[k_pv][piste-1][wcslen(Sarjat[*srj].va_matka[k_pv][piste-1])-1] <= L'9')
				tulprm->writehtml(L" km");
			}
		tulprm->writehtml(L"</H3>\n");
		}
	na = n_avoin(*srj);
	prs[0] = 0;
	if (tulprm->tulostettava != L'P') {
		if (na)
			swprintf(prs, L"<p>%s %s</p>\n", txts[tulprm->language][2], wkello());
		if (tulprm->tulmuot.lkmfl && !tulprm->piilotatulos && !Sarjat[*srj].piilotatulos) {
			tulprm->writehtml(L"<p class=sarjatiedot>\n");
			swprintf(prs+wcslen(prs), L" %s: %d, %s: %d, %s: %d",
				txts[tulprm->language][3], ntulos[*srj][0]+nkesk[*srj]+nhyl[*srj],
				txts[tulprm->language][4], nkesk[*srj],
				txts[tulprm->language][5], nhyl[*srj]);
			if (na)
				swprintf(prs+wcslen(prs), L" %s: %d", txts[tulprm->language][9], na);
			wcscat(prs, L"</p>\n");
			}
		wcscat(prs, L"\n");
		tulprm->writehtml(prs);
		prs[0] = 0;
		}
	if (!tulprm->tulmuot.tauluhtml)
		tulprm->writehtml(L"<pre class=tulokset>\n");
	else {
		wcscat(prs, L"<p><table class='restbl'>\n");
/*
		if (tulprm->tulmuot.tauluhtml == 2) {
			wcscat(prs, L"<colgroup>\n");
			for (int i_knt = 0; i_knt < tulprm->n_aFlds; i_knt++) {
				int lp;
				lp = (100*(tulprm->aFld[i_knt].len)+150)/(tulprm->totW+tulprm->n_aFlds);
				swprintf(prs+wcslen(prs), L"<col width=\"%d%%\">\n",  lp);
				}
			wcscat(prs, L"</colgroup>\n");
			}
*/
		}
	if (prs[0])
		tulprm->writehtml(prs);

	if ((tulprm->tulmuot.otsikot & 1)) {
		if (tulprm->tulmuot.tauluhtml)
			wcscpy(prs, L"<tr>");
		else {
			wmemset(prs, L' ', sizeof(prs)/2);
			prs[sizeof(prs)/2-1] = 0;
			}
		for (int i_knt = 0; i_knt < tulprm->n_aFlds; i_knt++) {
			wchar_t st[100] = L"";
			wcsncpy(st, tulprm->aFld[i_knt].title, 29);
			if (!tulprm->tulmuot.tauluhtml) {
				int pos, l;
				l = wcslen(st);
				pos = tulprm->aFld[i_knt].pos;
				if (tulprm->aFld[i_knt].align && l < (int) tulprm->aFld[i_knt].len)
					pos += tulprm->aFld[i_knt].len - l;
				if (pos + l > (int)sizeof(prs)/2-3)
					break;
				wmemcpy(prs+pos, st, l);
				}
			else {
/*				if (tulprm->aFld[i_knt].align)
					wcscat(prs, L"<th align=right>");
				else
					wcscat(prs, L"<th align=left>");
*/
				wcscat(prs, L"<th>");
				wcscat(prs, st);
				wcscat(prs, L"</th>");
				}
			}
		if (!tulprm->tulmuot.tauluhtml) {
			for (wchar_t *p = prs + sizeof(prs)/2-2; p >= prs && *p <= L' '; p--)
				*p = 0;
			}
		else
			wcscat(prs, L"</tr>");
		if (prs[0]) {
			wcscat(prs, L"\n");
			tulprm->writehtml(prs);
			}
		}
	potsfl = FALSE;
}

void kirjTekstit(tulostusparamtp *tulprm, lisatxttp *lisatxt, int *l)
{
	int yOrig = 0, ylen;

	if (tulprm->lstf->prno != 2)
		return;
	if (lisatxt->n_txt == 0)
		return;
	ylen = tulprm->lstf->u.wp.yPage * 72 * (1000 - tulprm->tulmuot.ymarg) / 1000 / tulprm->lstf->u.wp.yppi ;
	if (lisatxt->ypos == 1)
		yOrig = 0;
	else if (lisatxt->ypos == -1)
		yOrig = ylen - lisatxt->ytila;
	else if (tulprm->vpos + lisatxt->ytila <= ylen)
		yOrig = tulprm->vpos;
	else {
		endpage(tulprm);
		initpage(tulprm, NULL);
		yOrig = 0;
		}

	for (int i = 0; i < lisatxt->n_txt; i++) {
		tulprm->vpos = yOrig + lisatxt->txts[i].y;
		if (lisatxt->txts[i].size == 3)
			paaots_on(tulprm);
		if (lisatxt->txts[i].size == 2)
			aliots_on(tulprm);
		putfld(tulprm, lisatxt->txts[i].txt, lisatxt->txts[i].x, wcslen(lisatxt->txts[i].txt), 0, 0);
		endline(tulprm, 0);
		if (lisatxt->txts[i].size == 3)
			paaots_pois(tulprm);
		if (lisatxt->txts[i].size == 2)
			aliots_pois(tulprm);
		}
	if (lisatxt->ypos >= 0) {
		tulprm->vpos = lisatxt->ytila;
		*l = 2;
		}
}

void kirjPaaAlku(tulostusparamtp *tulprm, int *l)
{
	kirjTekstit(tulprm, lisatxt, l);
}

void kirjSivuAlku(tulostusparamtp *tulprm, int *l)
{
	kirjTekstit(tulprm, lisatxt+1, l);
}

void kirjSivuAla(tulostusparamtp *tulprm, int *l)
{
	kirjTekstit(tulprm, lisatxt+2, l);
}

void kirjLoppu(tulostusparamtp *tulprm, int *l)
{
	kirjTekstit(tulprm, lisatxt+3, l);
}

static void kirjoitinpaaotsikot(int *l, int *sv, int tark_mat, int enssivu, wchar_t *snimi, tulostusparamtp *tulprm)
// int enssivu, int piste, wchar_t ct, wchar_t *snimi, FldFrmtTp *fld, int nfld)
{
	wchar_t prs[200];
	int pos;
	wchar_t *txts[MAXLANG][5] = {{L"Piiri", L"Seura", L"Tulostettu ", L"klo", L"Sivu  "},
						  {L"District", L"Club", L"Printed   ", L"at", L"Page  "}};
	FldFrmtTp *p_fld = (FldFrmtTp *)tulprm->p_fld;

	if (*l == 0 && tulprm->sivu_alkuteksti) {
		kirjSivuAlku(tulprm, l);
		}
	if (!tulprm->paa_alkuteksti && !tulprm->sivu_alkuteksti) {
		if (*l == 0 && tulprm->tulmuot.esirivit) {
			newline(tulprm, tulprm->tulmuot.esirivit);
			*l = tulprm->tulmuot.esirivit;
			}
		initline(tulprm);
		potsfl = FALSE;
		if (sarjaots[0] && !tark_mat) {
			paaots_on(tulprm);
			otsTeksti(prs, sarjaots, 100, k_pv);
			putfld(tulprm, prs, 0, wcslen(prs), 0, 0);
			endline(tulprm, -1);
			paaots_pois(tulprm);
			(*l)++;
			}
		}
	potsfl = FALSE;
	if (!tark_mat)
		aliots_on(tulprm);
// Piiriotsikot
	if (tulprm->piiritulokset) {
		swprintf(prs, L"  %s :", txts[tulprm->language][0]);
		for (int piiri = 0; piiri < piiriluku; piiri++) {
			if (piirifl[piiri] && piirinimi[piiri][0]) {
				if (wcslen(prs) + wcslen(piirinimi[piiri]) >= 76) {
					putfld(tulprm, prs, 0, wcslen(prs), 0, 0);
					endline(tulprm, -1);
					wcscpy(prs,L"          ");
					++*l;
					}
				else wcscat(prs, L" ");
					wcscat(prs, piirinimi[piiri]);
				}
			}
		if (wcslen(prs) > 10) {
			putfld(tulprm, prs, 0, wcslen(prs), 0, 0);
			endline(tulprm, -1);
			++*l;
			}
		}
// Seuraotsikot
	else if (tulprm->seuratulokset) {
		swprintf(prs, L"  %s :", txts[tulprm->language][1]);
		for (int is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
			wcscat(prs, seuranimi[is]);
			wcscat(prs, L" ");
			if (wcslen(prs) > 50) {
				prs[51] = L'.';
				prs[52] = L'.';
				prs[53] = 0;
				break;
				}
			}
		putfld(tulprm, prs, 0, 53, 0, 0);
		}
// Kellonaika ja p‰iv‰ys
	else if (tulprm->tulmuot.tpvafl || tulprm->tulmuot.tklofl) {
		wcscpy(prs, txts[tulprm->language][2]);
		if (tulprm->tulmuot.tpvafl)
			wcscat(prs, wpaiva());
		if (tulprm->tulmuot.tklofl) {
			swprintf(prs+wcslen(prs), L"  %s %s", txts[tulprm->language][3], wkello());
			}
		if (tulprm->printer == POSTSCRIPT || tulprm->printer == LJETII)
			pos = 72*3-30-tulprm->tulmuot.marg;
		else if (tulprm->printer == GDIPRINTER)
			pos = 120*3-50-tulprm->tulmuot.marg;
		else pos = 34;
			putfld(tulprm, prs, pos, 33, 0, 0);
		}
// Sivunumero
	wcscpy(prs, txts[tulprm->language][4]);
	_itow(++(*sv), prs+5, 10);
	if (tulprm->printer == POSTSCRIPT || tulprm->printer == LJETII)
		pos = 72*7-30-tulprm->tulmuot.marg;
	else if (tulprm->printer == GDIPRINTER)
		pos = 120*7-50-tulprm->tulmuot.marg;
	else
		pos = 67;
	putfld(tulprm, prs, pos, 8, 1, 0);
	endline(tulprm, -1);
	(*l)++;
// Jatkosivujen otsikot
	if (!enssivu) {
		if (tulprm->tulmuot.snimifl || tulprm->piste)
			putfld(tulprm, snimi, 0, wcslen(snimi), 0, 0);
		endline(tulprm, -1);
		*l += 2;
		if (alaots[0] && !tark_mat) {
			otsTeksti(prs, alaots, 100, k_pv);
			putfld(tulprm, prs, 0, wcslen(prs), 0, 0);
			endline(tulprm, -1);
			(*l)++;
			}
		if (tulprm->tulmuot.otsikot & 1) {
			FldFrmtTp *fld = (FldFrmtTp *) p_fld;
			newline(tulprm, 1);
			(*l)++;
			for (int f = 0; ; f++) {
				if (f > 0 && fld[f].ots[0] == 0 && fld[f].id == 0)
					break;
				if (fld[f].len) {
					putfld(tulprm, fld[f].ots, fld[f].pos,
					fld[f].len, fld[f].oik, 0);
					}
				}
			endline(tulprm, -2);
			(*l)++;
			}
		aliots_pois(tulprm);
		}
}

//static void kirjoitinsivuotsikot(int *l, int *srj, int piste, int day, wchar_t *snimi, int tark_mat, int ixjarj, FldFrmtTp *fld, int nfld)
static void kirjoitinsivuotsikot(int *l, int *srj, wchar_t *snimi, int tark_mat, tulostusparamtp *tulprm)
{
	wchar_t prs[200], st[60], stas[100];
	int len, na, psija;

	wchar_t *txts[MAXLANG][18] = {
		{L" ja ", L"Er‰", L"Yht.aikoja", L"Aikoja", L"Kesk",
		 L"Hyl", L"Avoinna", L"Paras mahd. puuttuva aika", L"ja sija", L"Vaihe",
		 L"L‰hti", L"Keskeytti", L"Hyl‰tty", L"Tuloksia", L"Avoinna",
		 L"Tilanne", L"osuuden j‰lkeen", L"V‰liajat"},
		{L" and ", L"Heat", L"Overall rslts", L"Results", L"DNF",
		 L"DQ", L"Open", L"Best possible new result", L"and position", L"Race",
		 L"Started", L"DNF", L"DQ", L"Results", L"Open",
		 L"Status after", L"legs", L"Split times"}};

	aliots_on(tulprm);
	if (!tulprm->piste || tulprm->piste == kilpparam.valuku+1) {
		if (tulprm->tulmuot.snimifl) {
#ifdef MAKI
			if (makitulokset) {
				if (strlen(snimi) > 18)
					sprintf(prs,"    %s ",snimi);
				else
					sprintf(prs,"    %-19s",snimi);
				}
			else if (yhdistetty) {
				sprintf(prs,"Yhdistetyn hiihto");
				}
			else
#endif
				{
				if ((tulprm->vaihelkm > 1 || tulprm->yhttuljarj) && Sarjat[*srj].viimpv == k_pv &&
				   Sarjat[*srj].enspv == k_pv-1 && Sarjat[*srj].matka[Sarjat[*srj].enspv][0]) {
				   wcscpy(st, Sarjat[*srj].matka[Sarjat[*srj].enspv]);
				   if (st[wcslen(st)-1] <= L'9') {
						wcscat(st, L" km");
						if (Sarjat[*srj].tapa[k_pv][0] >= L'P') {
							swprintf(st+wcslen(st), L" (%s)", Sarjat[*srj].tapa[k_pv]);
							}
						}
				   else if (wcswcind(st[wcslen(st)-1], L"VP") >= 0) {
						wchar_t *p, stl[10] = L"";
						for (p = st+wcslen(st)-1; p > st+1 && *p > L'9'; p--) ;
						if (*p <= L'9') {
							wcsncpy(stl, p+1, 9);
							p[1] = 0;
							swprintf(p+1, L" km (%s)", stl);
							}
						}
				   wcscat(st, txts[tulprm->language][0]);
				   }
				else
					st[0]=0;
				if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && tulera)
					swprintf(prs,L"    %s, %s %d",snimi, txts[tulprm->language][1], tulera);
				else
					{
					memset(stas, 0, sizeof(stas));
					AlisarjaOts(stas+3, 80, tulprm);
					if (wcslen(stas+3) > 0)
						wmemcpy(stas, L" - ", 3);
					if (!Sarjat[*srj].matka[k_pv][0] || !tulprm->tulmuot.matkafl ||
						(tulprm->tulmuot.matkafl == 1 && Sarjat[*srj].psarjanimi[0])) {
						if (wcslen(snimi) > 18)
							swprintf(prs,L"    %s%s ",snimi, stas);
						else
							swprintf(prs,L"    %-19s%s",snimi, stas);
						}
					else {
						len = wcslen(Sarjat[*srj].matka[k_pv]);
						if (Sarjat[*srj].matka[k_pv][len-1] >= L'0' &&
							Sarjat[*srj].matka[k_pv][len-1] <= L'9') {
							if (wcslen(snimi) > 18)
								swprintf(prs,L"    %s%s (%s%s km%s%s)",snimi, stas, st,
									Sarjat[*srj].matka[k_pv],
									Sarjat[*srj].tapa[k_pv][0] ? L" " : L"", Sarjat[*srj].tapa[k_pv]);
							else
								swprintf(prs,L"    %-19s%s(%s%s km%s%s)",snimi, stas, st,
									Sarjat[*srj].matka[k_pv],
									Sarjat[*srj].tapa[k_pv][0] ? L" " : L"", Sarjat[*srj].tapa[k_pv]);
							}
						else {
							if (wcslen(snimi) > 18)
								swprintf(prs,L"    %s%s (%s%s)",snimi, stas, st, Sarjat[*srj].matka[k_pv]);
							else
								swprintf(prs,L"    %-19s%s(%s%s)",snimi, stas, st, Sarjat[*srj].matka[k_pv]);
							}
						}
					}
				if (tulprm->rajaus == L'A')
					swprintf(prs+wcslen(prs), L" L‰htˆ %d", Sarjat[*srj].lno[k_pv]);
				}
			putfld(tulprm, prs, 0, 80, 0, 0);
			endline(tulprm, -1);
			(*l)++;
			}
//  Lukum‰‰r‰t
		if (tulprm->tulmuot.lkmfl && !tulprm->piilotatulos && !Sarjat[*srj].piilotatulos &&
			((wcswcind(kilpparam.kilplaji, L"NP") >= 0 && !tulera) ||
			(wcswcind(kilpparam.kilplaji, L"NP") < 0 && !tark_mat &&
				((tulprm->ixjarj % 10) < 3)))) {
			na = n_avoin(*srj);
			(*l)++;
			if (k_pv) {
//  K‰ynniss‰ olevan sarjan lukum‰‰r‰t monip‰iv‰inen kisa
				if (na != 0) {
					swprintf(prs,L"%s :%4d %s :%4d  %s :%3d  %s :%3d %s :%4d",
						txts[tulprm->language][2], ntulos[*srj][kilpparam.valuku+1],
						txts[tulprm->language][3], ntulos[*srj][0],
						txts[tulprm->language][4], nkesk[*srj],
						txts[tulprm->language][5], nhyl[*srj],
						txts[tulprm->language][6], na);
					putfld(tulprm, prs, 0, 80, 0, 0);
					endline(tulprm, -1);
					(*l)++;
#ifdef MAKI
					if (!makitulokset && *srj < sarjaluku)
#else
					if (*srj < sarjaluku && wcswcind(kilpparam.kilplaji, L"NP") < 0)
#endif
						{
						puuttuvat(st,&psija,*srj,!tulprm->yhttuljarj);
						swprintf(prs,L"%s : %s %s %d",
							txts[tulprm->language][7], st,
							txts[tulprm->language][8], psija);
						putfld(tulprm, prs, 0, 80, 0, 0);
						endline(tulprm, -1);
						(*l)++;
						}
					}
//  Valmistuneen sarjan lukumm‰r‰t
				else {
					swprintf(prs,L"%s %d   %s : %-4d   %s : %-3d"
						L"   %s : %d",
						txts[tulprm->language][9], k_pv+1,
						txts[tulprm->language][10], ntulos[*srj][0]+nkesk[*srj]+nhyl[*srj],
						txts[tulprm->language][11], nkesk[*srj],
						txts[tulprm->language][12], nhyl[*srj]);
					putfld(tulprm, prs, prtflds[1].pos, 80, 0, 0);
					endline(tulprm, -1);
					}
				}
// 1-p‰iv‰inen kisa tai 1. vaihe
			else
				{
#ifdef MAKI
				if (makitulokset) {
					if (na != 0) {
						swprintf(prs,L"%s :%4d  %s :%3d  %s :%3d  %s :%4d",
							txts[tulprm->language][13], ntulos[*srj][0],
							txts[tulprm->language][4], nkesk[*srj],
							txts[tulprm->language][5], nhyl[*srj],
							txts[tulprm->language][14], na);

						}
//  1. vaiheen valmiit tulokset
					else {
						swprintf(prs,L"%s :%4d  %s :%3d  %s :%3d",
							txts[tulprm->language][13], ntulos[*srj][0],
							txts[tulprm->language][4], nkesk[*srj],
							txts[tulprm->language][5], nhyl[*srj]);
						}
					}
				else
#endif  // MAKI
					{
					if (na != 0) {
						swprintf(prs,L"%s :%4d  %s :%3d  %s :%3d  %s :%4d",
							txts[tulprm->language][13], ntulos[*srj][0],
							txts[tulprm->language][4], nkesk[*srj],
							txts[tulprm->language][5], nhyl[*srj],
							txts[tulprm->language][14], na);
						putfld(tulprm, prs, 0, 80, 0, 0);
						endline(tulprm, -1);
						(*l)++;
						if (*srj < sarjaluku) {
							puuttuvat(st,&psija,*srj, 1);
							swprintf(prs,L"%s : %s  %s : %d",
								txts[tulprm->language][7], st,
								txts[tulprm->language][8], psija);
							}
						}
					else {
						swprintf(prs,L"%s : %-4d   %s : %-3d   %s : %d",
							txts[tulprm->language][10], ntulos[*srj][0]+nkesk[*srj]+nhyl[*srj],
							txts[tulprm->language][11], nkesk[*srj],
							txts[tulprm->language][12], nhyl[*srj]);
						}
					}
				putfld(tulprm, prs, 0, 80, 0, 0);
				endline(tulprm, -1);
				(*l)++;
				}
			}
		}
	else {
#ifdef MAKI
		if (makitulokset) {
			switch (piste) {
				case 1:
					wcscpy(prs, L"Koekierros");
					break;
				case 3:
					wcscpy(prs, L"1. kierros");
					break;
				case 5:
					wcscpy(prs, L"2. kierros");
					break;
				case 2:
					wcscpy(prs, L"Koekierros, pituudet");
					break;
				case 4:
					wcscpy(prs, L"1. kierros, pituudet");
					break;
				case 6:
					wcscpy(prs, L"2. kierros, pituudet");
					break;
				}
			}
		else
#endif
			swprintf(prs,L"%s   %s : %s",
				snimi, txts[tulprm->language][17],
				Sarjat[*srj].va_matka[k_pv][tulprm->piste-1]);
		if (prs[wcslen(prs)-1] >=L'0' && prs[wcslen(prs)-1] <=L'9') {
			if (_wtoi(Sarjat[*srj].va_matka[k_pv][tulprm->piste-1]) > 99)
				wcscat(prs, L" m");
			else
				wcscat(prs, L" km");
			}
		putfld(tulprm, prs, 0, 80, 0, 0);
		endline(tulprm, -2);
		*l += 5;
		}
	if (alaots[0]) {
		putfld(tulprm, alaots, 0, 80, 0, 0);
		endline(tulprm, -1);
		}
	else
		if (!tulprm->tulmuot.otsikot & 1)
			newline(tulprm, 1);
	if (tulprm->tulmuot.otsikot & 1) {
		FldFrmtTp *fld = (FldFrmtTp *)tulprm->p_fld;
		newline(tulprm, 1);
		(*l)++;
		for (int f = 0; ; f++) {
			if (f > 0 && fld[f].ots[0] == 0 && fld[f].id == 0)
				break;

			if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && tulprm->vaihelkm > 1 &&
				fld[f].id == F_PIST) {
				int x = fld[f].pos;
				for (int is = 0; is <= k_pv; is++) {
					if (is >= Sarjat[*srj].enspv) {
						swprintf(prs, L"Lj-%d", is+1);
						putfld(tulprm, prs, x, fld[f].len, fld[f].oik, 0);
						x += fld[F_OSATLS].pos - fld[F_PIST].pos;
						}
					}
				putfld(tulprm, L"Yht.", x, fld[f].len, fld[f].oik, 0);
				}
			else if (fld[f].len) {
				putfld(tulprm, fld[f].ots, fld[f].pos, fld[f].len, fld[f].oik, 0);
				}
			}
		endline(tulprm, -2);
		(*l)++;
		}
	(*l)++;
	aliots_pois(tulprm);
}

//			tulostasarja - argumentit
//
//  srj		osoitin tulostettavaan sarjaan
//  piste	tulostuksen ensisijainen kohdepiste, m‰‰r‰‰ yleens‰ j‰rjestyksen
//  kohde	tulostuksen kohde
//			P: kirjoitin
//			N: n‰yttˆ (konsoliversiossa)
//			H: html
//			X: XML
//			R: csv-tiedosto
//			Z: csv-tiedosto kaikki v‰liajat
//			I: kohdistettu tekstitiedosto
//			S: SSL / SHL
//			F: IOF / WRE
//			E: jatkuva teksti (lehteen)
//  ct      rajaus (ilmoittautuneet, lopettaneet, parhaat, seura, piiri, ..)
//  cn      nollaa autamaattitulostuksen laskuri, jos cn == L'K'
//  cd      M: tulosta sek‰ vaihe ett‰ yhteistulos, 1: tulosta lis‰ksi aiemmat vaiheet
//  cj		Y: yhteistulos ensisijainen, 2: vaiheen tulos ensisijainen
//  day     0: yhteistuloksen mukainen j‰rjestys, 1: vaiheen mukainen j‰rjestys
//  np		laajuus tulostettaessa parhaat
//  ixjarj  1: aakkosj‰rj., 11: numeroj‰rj., 4: laskevat pisteet, 5: nousevat pisteet
//  l		osoitin rivinumeroon
//  sv		osoitin sivunumeroon
//	autotl	automaattitulostus
//

//int tulostasarja(INT *srj, INT piste, wchar_t kohde, wchar_t ct, wchar_t cn,
//	wchar_t cd, wchar_t cj, INT day, INT np, INT ixjarj, INT *l, INT *sv, INT autotl)

int tulostasarja(INT *srj, tulostusparamtp *tulprm, INT *l, INT *sv, INT autotl)
{
	INT piste;
	int kilpno;
	wchar_t snimi[300];
	INT  sj, n, n1, i, lj, lj0 = 0, lj1 = 4, tark_mat, enssivu = 1, tulostettu = 0;
	INT  is, p, d, nalku = 0, uusisv, sots = 0, valmis = 0;
	wchar_t ch;
	wchar_t prs[200];
	kilptietue kilp;
	INT sj1, sj2, edpst = -1, tulosfl, xmlvaliajat = 0;
	bool piirisj = false;
	INT32 tls = 0, tls0 = 0, karkitls = 0;
//	INT nfld;
//	FldFrmtTp *fld;
	int ixjarj = 0;
#ifdef MAKI
	int makitulokset = 0, makikierrokset = 0;
#endif
#ifdef SEURATUL
	wchar_t seura[LSEURA+2];
	typedef struct {
		INT d;
		INT sj1;
		INT sjfl;
		} tul_atp;
	tul_atp *tul_a = NULL;
	INT ni = 0;

	piste = tulprm->piste;
	if (tulprm->seuratulokset)
		tulprm->rajaus = L'S';
	if (tulprm->piiritulokset)
		tulprm->rajaus = L'D';

//#if defined(UINTI) || defined(MERI5)
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && tulera) {
		if (!onkosrjera(*srj, tulera))
			return(0);
		piirisj = true;
		}
//#endif
#ifndef MPA
	if (tulprm->seuratulokset && !(tulprm->sj_raja && tulprm->karjet) &&
		!onko_seura(*srj, tulprm->yhttuljarj))
		return(0);
	if (tulprm->piiritulokset && !(tulprm->sj_raja && tulprm->karjet) &&
		!onko_piiri(*srj, tulprm->yhttuljarj))
		return(0);
	if (tulprm->piiritulokset)
		piirisj = tulprm->piirisijat;
	if ((tulprm->ikasarjat != NULL && tulprm->ikasarjat[0] > 0) ||
		(tulprm->sukuprajaus != NULL && tulprm->sukuprajaus[0] > L' '))
		piirisj = true;
#else
	if (tulprm->rajaus == L'J' || tulprm->rajaus == L'Y')
		piirisj = true;
#endif
#endif
	SarjaNimi(snimi, sizeof(snimi)/2, *srj, true, NULL);
	tark_mat = !autotl && tulprm->tarkastamattomat;
	if (tark_mat && !onko_tarkastamatta(*srj))
		return(0);

	tulprm->rivi = 0;
	tulprm->setActFlds(*srj);
	EnterCriticalSection(&tall_CriticalSection);

	sj = 0;
	sj1 = 0;
	sj2 = 0;
	uusisv = !tark_mat && (tulprm->optiot & 0x80000000) != 0;

#ifdef MAKI
	if (k_pv != makipv && tulprm->kohde == 'X' && piste == -1) {
		xmlvaliajat = 1;
		piste = 0;
		}
	if (k_pv == makipv && !aikaerot) {
		makitulokset = 1;
		if (piste == -1 || piste == -2) {
			makikierrokset = -piste;
			piste = 0;
			}
/*
		if (piste) {
			fld = prtfldsmk;
			nfld = sizeof(prtfldsmk)/sizeof(FldFrmtTp)-1;
			}
		else {
			fld = prtfldsmy;
			nfld = sizeof(prtfldsmy)/sizeof(FldFrmtTp)-1;
			}
*/
		}
//	else
#endif
/*
		{
		fld = prtflds;
		nfld = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
		}
	if (k_pv > 0) {
		switch (tulprm->vaihelkm) {
			case 2:
				fld = prtfldsm;
				nfld = sizeof(prtfldsm)/sizeof(FldFrmtTp)-1;
				break;
			case 3:
				fld = prtflds1;
				nfld = sizeof(prtflds1)/sizeof(FldFrmtTp)-1;
				break;
			}
		}
*/
	if (tulprm->yhttuljarj) {
		piste = kilpparam.valuku+1;
		lj0 = kilpparam.valuku+5;
		lj1 = lj0+4;
		}
	else
		{
		if (piste == L'K' - L'0') {
			piste = 0;
			tulprm->kaikkiva = true;
			}
		tls0 = 0;
		n = nilm[*srj];
		if (piste) {
			lj0 = piste+4;
			lj1 = piste+4;
			if (tulprm->rajaus != L'P')
				tulprm->rajaus = L'T';
			}
		}
	ixjarj = tulprm->ixjarj;
	if (!ixjarj && Sarjat[*srj].tuljarj) {
		ixjarj = 10 * Sarjat[*srj].tuljarj - 9;
		}
	if (!ixjarj) {
		switch(tulprm->rajaus) {
			case L'P' :
				if (!autotl && tulprm->enssija > 1) {
					nalku = tulprm->enssija;
					}
				uusisv = 0;
				break;
			case L'H' :
				uusisv = 0;
				lj0 = 2;
				tulprm->yhttuljarj = false;
				break;
			case L'K' :
			case L'X' :
				uusisv = 0;
				lj0 = 1;
				tulprm->yhttuljarj = false;
				break;
			case L'E' :
				uusisv = 0;
				lj0 = 3;
				tulprm->yhttuljarj = false;
				break;
			case L'A' :
				uusisv = 0;
				lj0 = 4;
				tulprm->yhttuljarj = false;
				break;
			}
		}
	n = srjtullkm(*srj, tulprm); //piste, day, tulprm->ixjarj, tulprm->rajaus, np);
	if (n > 0) {
		tulostettu = 1;
		tulfl = TRUE;
#ifdef MAKI
		if (makitulokset || ixjarj || k_pv || tulprm->kaikkiva ||
			tulprm->kohde == 'X' || tulprm->kohde == 'Z')
			sijat(*srj);
#else
		if (ixjarj || k_pv || tulprm->kaikkiva || (tulprm->tulostettava == L'Y' && tulprm->kaikkisijat) || tulprm->kohde == L'X')
			sijat(*srj);
		if ((k_pv == 0 && tulprm->vaihelkm == 3) || (tulprm->kohde == L'X' && nsarjayhd > 0))
			sijat2(*srj, true, false);
#endif
		if (tulprm->kohde == L'P') {
			if (tulprm->rajaus == L'P')
				n1 = min(n, tulprm->viimsija) - tulprm->enssija + 1;
			else
				n1 = n;
			clrln(ySize-1);
			}
		if (wcswcind(tulprm->kohde, L"NIRZ") >= 0) {
			*l = 0;
			tul_a = (tul_atp *) calloc(n, sizeof(tul_atp));
			}
		if (tulprm->kohde == L'P' && *l > 2) {
			if ((!uusisv && tulprm->mahtuusivulle(*l, n1 + 5)) ||
				((tulprm->seuratulokset || tulprm->piiritulokset ||
				(wcswcind(kilpparam.kilplaji, L"NP") >= 0 && tulera)) && tulprm->mahtuusivulle(*l, 10))) {
				newline(tulprm, 2);
				*l += 3;
				}
			else  {
				*l = 0;
				if (tulprm->sivu_alateksti)
					kirjSivuAla(tulprm, l);
				endpage(tulprm);
				initpage(tulprm, NULL);
				}
			}
		modif[*srj] = FALSE;
		i = 0;
		lj = lj0;
		p = -1;

		while ( ++i <= n ) {

#ifdef _CONSOLE
			if ( !autotl) {
				char che;
				do {
					ch = readkbd(&che,FALSE,spoolfl);
					if( ch == 27 ) goto lopeta;
					} while (che != 0);
				}
			if ( modif[*srj] ) {
				i = 1;
				lj = lj0;
				p = -1;
				if (!k_pv) tls0 = 0;
				sj = 0;
				sj1 = 0;
				sj2 = 0;
				*l = 0;
				writeerror_w(L"Uusi tulos, sarja aloitetaan alusta", 1500);
				modif[*srj] = FALSE;
				}
#endif
			if (i == 1)
				sots = 1;
			valmis = tlsSeuraava(&p, &d, ixjarj, srj, piste, &lj, lj1, i == 1, &kilpno, tulprm);
			if (valmis)
				break;
			kilp.GETREC(d);
			if (tulprm->kohde == L'X' && (tulprm->optiot & 0x00400000) &&
				kilp.lisno[1] == 0)
				continue;
			sj++;
			//  kilpparam.poissamukaan vaikuttaa l‰htˆluetteloihin
			//  tulprm->optiot & 4 kertoo yhteispisteiden tulostuksesta
			if (!kilpparam.poissamukaan && (tulprm->optiot & 4) == 0 && kilp.tark(k_pv) == L'V')
				continue;
			if (tulprm->tarkastamattomat && kilp.tark(k_pv) != L'-')
				continue;
			if (kilp.tark() == L'X' && wcswcind(tulprm->rajaus, L"IX") < 0 &&
				(tulprm->rajaus != L'L' || (tulprm->optiot & 0x1000) == 0))
				continue;
			if (kilp.tark() != L'X' && tulprm->rajaus == L'X')
				continue;
			if (ixjarj % 10 > 0 && ixjarj % 10 < 4) {
				if (!kilp.onSarja(Sarjat[*srj].sarjanimi, true))
					continue;
				switch (tulprm->rajaus) {
					case L'L':
						if (!kilp.p_aika(piste) && wcswcind(kilp.tark(k_pv), L"KOHM") < 0)
							continue;
						break;
					case L'T':
						if ((tulprm->tulostettava != L'P' && !kilp.p_aika(piste)) ||
						   (tulprm->tulostettava == L'P' && kilp.pisteet[0] == 0))
							continue;
						break;
					case L'P':
						if (!kilp.p_aika(piste) || kilp.p_sija(piste) > tulprm->viimsija)
							continue;
						break;
					}
				if ((ixjarj % 10) == 2 && kilp.hyv())
					sj = kilp.p_sija(piste);
				else
					sj = 0;
				sj1 = sj;
				}
			else if (ixjarj) {
				if (tulprm->yhttuljarj) {
					if (Sarjat[*srj].luonne[k_pv] == 2) {
						if (kilp.ytulos(k_pv, 2) != edpst)
							sj1 = sj;
						edpst = kilp.ytulos(k_pv, 2);
						}
					else {
						if (kilp.pisteet[0] != edpst)
							sj1 = sj;
						edpst = kilp.pisteet[0];
						}
					}
				else {
					if (kilp.pv[k_pv].pvpisteet[0] != edpst)
						sj1 = sj;
					edpst = kilp.pv[k_pv].pvpisteet[0];
					}
				if (edpst == 0 && (tulprm->ixjarj == 4 ||
					(tulprm->ixjarj == 5 && (pstindex.flags & 16) != 0)))
					sj1 = 0;
				switch (tulprm->rajaus) {
					case L'L':
						if ((!kilp.p_aika(piste) && wcswcind(kilp.tark(k_pv), L"KOHM") < 0) &&
							!(tulprm->ixjarj == 9 && kilp.tark(k_pv) != '-' && edpst > 0))
							continue;
						break;
					case L'T':
						if ((tulprm->ixjarj == 4 || tulprm->ixjarj == 9) && edpst == 0)
							continue;
						break;
					case L'P':
						if (((tulprm->ixjarj == 4 || tulprm->ixjarj == 9) && edpst == 0) || sj1 <= 0 || sj1 > tulprm->viimsija)
							continue;
						break;
					}
				}
			else {
				if (lj > lj0+1) {
					sj = 0;
					sj1 = 0;
					sj2 = 0;
					}
				else {
//               if (!k_pv) {
					if ((tulprm->tulostettava != L'P' && (tls = kilp.tulos2(piste, 2)) != tls0) ||
						(tulprm->tulostettava == L'P' &&  (tls = kilp.pisteet[0]) != tls0)){
						sj1 = sj;
						tls0 = tls;
						}
					kilp.set_sija(piste, sj1);
					kilp.PUTREC(d, 0);
//                  }
//               else sj1 = sj;
					}
				if (sj == 1)
					karkitls = kilp.tulos2(piste, 2);
				}

			if (!tulprm->yhttuljarj							 // monivaiheisessa vaihekoht. j‰rjestys
				|| tulprm->rajaus != L'L'                        // lopettaneet
				|| (tulprm->vaihelkm == 3 && k_pv == 1 &&       // monivaiheisessa myˆs 1. p‰iv‰ ja vaiheen j‰rj ja 1. pv osallistunut
					kilp.lasna(0))
				|| ((kilp.tulos_pv(k_pv) && kilp.hyv())        // p‰iv‰n tulos ja hyv‰ksytty tai kesk tai hyl.
					|| wcswcind(kilp.tark(k_pv), L"KOHM") >= 0)
				) {

				if (sj == 1) karkitls = kilp.tulos2(piste, 2);

			// Kirjoitintulostus
			if( tulprm->kohde == L'P' ) {
			   if( *l == 0 || potsfl) {
				  kirjoitinpaaotsikot(l, sv, tark_mat, enssivu, snimi, tulprm);
						}
			   if (sots) {
				  sots = 0;
				  kirjoitinsivuotsikot(l, srj, snimi, tark_mat, tulprm); //piste, day, snimi, tark_mat, ixjarj, fld, nfld);
				  }

				if (sj < nalku)
					continue;
				tulosfl = 0;
#ifdef MPA
				if ((tulprm->rajaus != L'J' && tulprm->rajaus != L'Y')
					|| (tulprm->rajaus == L'J' && kilp.seuratieto(0)[0])
					|| (tulprm->rajaus == L'Y' && kilp.yhdistys[0]))
					tulosfl = 1;
#else
				if (OnAlisarja(&kilp, tulprm) && ((!tulprm->seuratulokset && !tulprm->piiritulokset) ||
					(sj1 > 0 && sj1 <= tulprm->sj_raja)
					|| (tulprm->piiritulokset && kilp.piiri > 0 &&
					kilp.piiri <= piiriluku && piirifl[kilp.piiri - 1]))) {
//#if defined(UINTI) || defined(MERI5)
					if (wcswcind(kilpparam.kilplaji, L"NP") < 0 || !tulera || (tulera == kilp.pv[k_pv].era && (kilp.p_aika(piste) || !kilp.hyv())))
//#endif
						tulosfl = 1;
					}
				else {
					if (tulprm->seuratulokset) {
						for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
							if (!wcscmpU(seuranimi[is],
								wcscpy(seura,kilp.seuratieto(0)))) {
								tulosfl = 1;
								break;
								}
							}
						}
					}
#endif
				if (tulosfl && (!tark_mat || kilp.tark(k_pv) != L'T')) {
					if (piirisj && (lj <= 1 || lj == kilpparam.valuku+6))
						sj2++;
					if (!piirisj)
						sj2 = sj1;
#ifdef MAKI
				if (makitulokset)
					*l += prtmakitulos(&kilp, tulprm);
					 //	(piirisj || sj1 == sj) ? sj1 : 0, piste + 100*kaikkiva);
				else
#endif
//					*l += prttulos(&kilp, tulprm,          // cd, cj,
//						(tulprm->rajaus == L'J' || tulprm->rajaus == L'Y' ||
//						tulprm->piiritulokset || sj1 == sj) ? sj1 : 0, piste + 100*kaikkiva,
//						karkitls);
					*l += prttulos(&kilp, tulprm,
						(tulprm->rajaus == L'J' || tulprm->rajaus == L'Y' ||
						tulprm->piiritulokset || sj1 == sj) ? sj2 : 0, karkitls);

				tulprm->rivi++;
				endline(tulprm, 0);
				}
			if (!tulprm->mahtuusivulle(*l, 1)) {
				if (tulprm->sivu_alateksti)
					kirjSivuAla(tulprm, l);
				*l = 0;
				enssivu = 0;
				endpage(tulprm);
				initpage(tulprm, NULL);
				}
			}
 //            else if (tulprm->kohde == L'Z' || tulprm->kohde == L'E') {
		else if (wcswcind(tulprm->kohde, L"EIRZ") >= 0) {
			if (sots && (tulprm->kohde == L'E' ||
				((tulprm->optiot & 0x00800000) && tulprm->kohde == L'R'))) {
				sots = 0;
				if (Sarjat[*srj].psarjanimi[0]) {
					tulprm->puts_f(Sarjat[*srj].psarjanimi);
					}
				else {
					tulprm->puts_f(Sarjat[*srj].sarjanimi);
					}
				if (Sarjat[*srj].matka[k_pv][0] && tulprm->tulmuot.matkafl &&
					(tulprm->tulmuot.matkafl == 2 || Sarjat[*srj].psarjanimi[0] == 0)) {
					tulprm->puts_f(L", ");
					if (tulprm->yhttuljarj) {
						for (int i_pv = Sarjat[*srj].enspv; i_pv < k_pv; i_pv++) {
							if (Sarjat[*srj].matka[i_pv][0]) {
								tulprm->puts_f(Sarjat[*srj].matka[i_pv]);
								tulprm->puts_f(L"+");
								}
							}
						}
					tulprm->puts_f(Sarjat[*srj].matka[k_pv]);
					if (Sarjat[*srj].matka[k_pv][wcslen(Sarjat[*srj].matka[k_pv])-1] <= L'9')
						tulprm->puts_f(L" km");
					if (Sarjat[*srj].tapa[k_pv][0] >= L'P') {
						tulprm->puts_f(L" ");
						tulprm->puts_f(Sarjat[*srj].tapa[k_pv]);
						}
					}
				if (tulprm->tulmuot.lkmfl) {
					if (tulprm->language == EN)
						swprintf(prs, L" (Started: %d, DNF: %d, DQ: %d)\n",
							ntulos[*srj][0]+nkesk[*srj]+nhyl[*srj],
							nkesk[*srj], nhyl[*srj]);
					else
						swprintf(prs, L" (L‰hti: %d, Keskeytti: %d, Hyl‰tty: %d)\n",
							ntulos[*srj][0]+nkesk[*srj]+nhyl[*srj],
							nkesk[*srj], nhyl[*srj]);
					tulprm->puts_f(prs);
					}
				else
					tulprm->puts_f(L"\n");
				}
			tulosfl = 0;
			if (OnAlisarja(&kilp, tulprm) && ((!tulprm->seuratulokset && !tulprm->piiritulokset) ||
				(sj > 0 && sj <= tulprm->sj_raja)
				|| (tulprm->piiritulokset && kilp.piiri > 0 &&
				 kilp.piiri <= piiriluku && piirifl[kilp.piiri - 1]))) {
//#if defined(UINTI) || defined(MERI5)
				  if (wcswcind(kilpparam.kilplaji, L"NP") < 0 || !tulera || (tulera == kilp.pv[k_pv].era && (kilp.p_aika(piste) || !kilp.hyv())))
//#endif
					  tulosfl = 1;
				  }
			else {
				  if (tulprm->seuratulokset) {
					 for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
						if (!wcscmpU(seuranimi[is],
						   wcscpy(seura,kilp.seuratieto(0)))) {
						   tulosfl = 1;
						   break;
						   }
						}
					 }
				  }
			   if (tulosfl) {
				  if (piirisj && (lj <= 1 || lj == kilpparam.valuku+6))
					  sj2++;
				  if (!piirisj)
					  sj2 = sj1;

				  if (tulprm->kohde == L'E')
					  tekstitulos(tulprm, prs, &kilp, piste, sj2, karkitls);
				  else if (tulprm->kohde == L'R' || tulprm->kohde == L'Z')
					  outtulos_csv(&kilp, sj2, karkitls, tulprm);
				  else
					  outtulos_txt(0, &kilp, sj2, piste, karkitls, tulprm);
				  tulprm->rivi++;
				  }
			   }
#if defined(TAULU_2)
			else if (tulprm->kohde == L'V') {
			   if (*l == 0) {
				  if (taulu_av(*srj, piste))
					 return(0);
				  ++*l;
				  }
			   taulutulos(&kilp, piste, sj1, *l);
			   tulprm->rivi++;
			   ++*l;
			   }
#endif
			else if (tulprm->kohde == L'S') {
			   liittotulos(&kilp, *srj, tulprm);
			   tulprm->rivi++;
			   }
			else if (tulprm->kohde == L'X') {
			   if (sots) {
				  sots = 0;
#ifdef MAKI
						if (makikierrokset == 1) {
						 if (xmlkierrosots(*srj)) goto lopeta;
							}
						else if (makikierrokset == 2) {
						 if (xmltuomariots(*srj)) goto lopeta;
							}
						else if (xmlvaliajat == 1) {
						 if (xmlvaots(*srj)) goto lopeta;
							}
						else
#endif
						if (tulprm->xmlstd == L'F') {
							if (xmlfissrjots(*srj, tulprm)) goto lopeta;
							}
						else
						{
					  if (xmlsrjots(*srj, tulprm)) goto lopeta;
					  }
				  tulprm->rivi++;
				  ++*l;
				  }
#ifdef MAKI
				if (makikierrokset == 1) {
					xmlkierrospisteet(&kilp);
					}
				else if (makikierrokset == 2) {
					xmltuomaripisteet(&kilp);
					}
				else if (xmlvaliajat == 1) {
					xmlvajat(&kilp);
					}
				else
#endif
					{
					if (tulprm->xmlstd == L'F')
						xmlfistulos(&kilp, sj1, tulprm);
					else
						xmltulos(kilp, *srj, sj1, tulprm); // cd == L'Y');
					tulprm->rivi++;
					}
				}

// HTML-tulostus

			else if (tulprm->kohde == L'H' || tulprm->kohde == L'M') {
			   if (sots) {
				  sots = 0;
				  kirjhtmlotsikot(srj, tulprm); //, piste, day, cd);
				  tulprm->rivi++;
				  }
			   tulosfl = 0;
			   if (OnAlisarja(&kilp, tulprm) && ((!tulprm->seuratulokset && !tulprm->piiritulokset) ||
					(sj1 > 0 && sj1 <= tulprm->sj_raja)
				  || (tulprm->piiritulokset && kilp.piiri > 0 &&
					 kilp.piiri <= piiriluku && piirifl[kilp.piiri - 1])))
				  tulosfl = 1;
			   else {
				  if (tulprm->seuratulokset) {
					 for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
						if (!wcscmpU(seuranimi[is],
						   wcscpy(seura,kilp.seuratieto(0)))) {
						   tulosfl = 1;
						   break;
						   }
						}
					 }
				  }
			   if (tulosfl) {
				  if (piirisj && (lj <= 1 || lj == kilpparam.valuku+6))
						sj2++;
				  if (!piirisj)
					  sj2 = sj1;
#ifdef MAKI
				if (makitulokset)
					htmlmakitulos(&kilp, sj2, tulprm); //day, cd, piste, sj1);
				else
#endif
					htmltulos(&kilp, sj2, *srj, tulprm);//day, cd, piste, sj1, (ixjarj % 10) > 1 && (ixjarj % 10) < 4, *srj);
					tulprm->rivi++;
					}
				}
			else if (wcswcind(tulprm->kohde, L"NI") >= 0) {    // tulprm->kohde == L'N' || kohde == L'I'
			   if (sj < nalku || !tul_a ) continue;
			   tul_a[ni].d = d;
			   tul_a[ni].sj1 = sj1;
			   tul_a[ni].sjfl = (sj == sj1);
			   ni++;
			   }
			else {
				parametrivirhe(L"Tulostasarja", sj, L"loopin loppu", tulprm->kohde);
				}
			}
		 }  //  end of while (++i <= n)
	  tulfl = FALSE;
	  if (wcswcind(tulprm->kohde, L"NI") >= 0) {
		 if (tulprm->kohde == L'N') {
			scpclr();
			kehys(0);
			clrln(0);
			vidspwmsg(0,0,7,0,Sarjat[*srj].sarjanimi);
			}
		 if (piirisj) sj2 = 0;
		 *l = 0;
		 for (i = 0; i <= ni; i++) {
			if (wcswcind(tulprm->kohde, L"I") >= 0)
				*l = -5;
			if (*l == (ySize-7) || (i == ni && *l > 0)) {
			   clrln(ySize-3);
			   vidspwmsg(ySize-3,0,7,0,Sarjat[*srj].sarjanimi);
			   vidspwmsg(ySize-3,13,7,0,L"Paina <Return> jatkaaksesi tai "
				  L"<Esc> keskeytt‰‰ksesi");
			   sccurset(ySize-3,66);
			   LeaveCriticalSection(&tall_CriticalSection);
			   do {
				   char che;
				  ch = readkbd(&che,TRUE,spoolfl);
				  } while (ch != L'\r' && ch != 27 && tulprm->kohde == L'N');
			   EnterCriticalSection(&tall_CriticalSection);
			   if (ch == 27)
				   goto lopeta;
			   if (p != seuraava(0, piste, d)) {
				  LeaveCriticalSection(&tall_CriticalSection);
				  writeerror("Sarjan tulokset muuttuneet", 0);
				  EnterCriticalSection(&tall_CriticalSection);
				  goto lopeta;
				  }
			   clearframe();
			   *l = 0;
			   }
			if (i == ni) break;
			kilp.GETREC(tul_a[i].d);
			tulosfl = 0;
#ifdef MPA
			if ((tulprm->rajaus != L'J' && tulprm->rajaus != L'Y')
			   || (tulprm->rajaus == L'J' && kilp.seuratieto(0)[0])
			   || (tulprm->rajaus == L'Y' && kilp.yhdistys[0]))
			   tulosfl = 1;
#else
			if (OnAlisarja(&kilp, tulprm) && ((!tulprm->seuratulokset && !tulprm->piiritulokset) ||
				(sj1 > 0 && sj1 <= tulprm->sj_raja)
			   || (tulprm->piiritulokset && kilp.piiri > 0 &&
				  kilp.piiri <= piiriluku && piirifl[kilp.piiri - 1]))) {
//#if defined(UINTI) || defined(MERI5)
			   if (wcswcind(kilpparam.kilplaji, L"NP") < 0 || !tulera || (tulera == kilp.pv[k_pv].era && (kilp.p_aika(piste) || !kilp.hyv())))
//#endif
			   tulosfl = 1;
			   }
			else {
               if (tulprm->seuratulokset) {
				  for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
					 if (!wcscmpU(seuranimi[is],
						wcscpy(seura,kilp.seuratieto(0)))) {
						tulosfl = 1;
						break;
						}
                     }
				  }
               }
#endif
			if (tulosfl && (!tark_mat || kilp.tark(k_pv) != L'T')) {
			   if (piirisj) sj2++;
			   else sj2 = tul_a[i].sj1;
				outtulos(*l,&kilp, (tulprm->seuratulokset || tulprm->piiritulokset ||
					tulprm->rajaus == L'Y' || tul_a[i].sjfl)
					? sj2 : 0, piste, karkitls, tulprm);
			   ++*l;
               }
            }
			if (tul_a) free(tul_a);
         }
	  if (tulprm->kohde == L'X' && tulprm->rivi > 0) {
#ifdef MAKI
		if (makikierrokset == 1)
			xmlkierrosloppu();
		else if (makikierrokset == 2)
			xmltuomariloppu();
		else if (xmlvaliajat == 1)
			xmlvaloppu();
		else
#endif
		if (tulprm->xmlstd == L'F')
			xmlfissrjloppu(tulprm);
		else
			xmlsrjloppu(tulprm);
	  }
/*
	  if( tulprm->kohde == L'P' ) {
		 if (autotl) {
			if (*l > 1) endpage(tulprm);
			*l = 0;
			}
		 }
*/
	  if (tulprm->kohde == L'E') {
		 tulprm->puts_f(L".\n\n");
		 }
	  if ((tulprm->kohde == L'H' || tulprm->kohde == L'M') && tulprm->rivi) {
		 if (!tulprm->tulmuot.tauluhtml)
			tulprm->writehtml(L"</PRE>\n");
		 else
			tulprm->writehtml(L"</table>\n");
		 }
	  clrln(ySize-1);
	  }  //  end of if (n > 0)
   if (autotl || tulprm->nollaalaskuri) prior[*srj] = 0;
   LeaveCriticalSection(&tall_CriticalSection);
   return(tulostettu);
lopeta:
   tulfl = FALSE;
   if(tulprm->kohde == L'P') {
	  if (*l > 1) {
		 if (tulprm->sivu_alateksti)
			kirjSivuAla(tulprm, l);
		 endpage(tulprm);
		 }
	  }
   *l = 0;
   *srj = sarjaluku + 1;
	if (tul_a) free(tul_a);
   clrln(ySize-1);
   LeaveCriticalSection(&tall_CriticalSection);
   return(tulostettu);
   }

void yhteenveto(tulostusparamtp *tulprm)
   {
   wchar_t *ots;
   wchar_t  paika[16], kaika[16], snimi[LSEURA+1];
   wchar_t  ch;
   char  ch2;
   wchar_t line[84], st[40];
   INT l, ll, srj, psrj, psija, rivit[MAXSARJALUKU+MAXYHD], scale = 1;

   INT psija2;

   if (k_pv)
	  ots = L"Sarja      Ilm Yht.aika Aika Kesk  Hyl Auki K‰rkiaika "
											L"Mahd.aika  Mahd.sija";
   else
	  ots = L"  Sarja      Ilm   Aika  Kesk  Hyl   Auki  K‰rkiaika  "
											L"Mahd.aika  Mahd.sija";
   kaika[8] = 0;
   if (tulprm->kohde == L'N') {
#ifdef _CONSOLE
      status_on = 0;
      clearframe();
      vidspwmsg(0,0,7,0,L"YHTEENVETO");
	  vidspwmsg(2,0,7,0,ots);
      clrln(ySize-3);
//      swprintf(line,L"Viim. maaliaika : %8.8s",AIKATOWSTRS(st, tmaali,t0));
//      vidspwmsg(ySize-3,49,7,0,line);
	  memset(rivit, 0, sizeof(rivit));
      l = 4;
      srj = 0;
      do {
         if (nilm[srj] > 0) {
            if (k_pv) {
			   if (ntulos[srj][kilpparam.valuku+1] == 0) wcscpy(kaika, L"        ");
			   else
					AIKATOWSTRS(kaika, jarjtulos[0][kilpparam.valuku+1][jalku[srj][kilpparam.valuku+5]][0],0);
               kaika[8] = 0;
			   EnterCriticalSection(&tall_CriticalSection);
			   puuttuvat(paika,&psija2,srj,1);
			   puuttuvat(paika,&psija,srj,0);
			   LeaveCriticalSection(&tall_CriticalSection);
			   swprintf(line,L"%-11s%4d%6d%7d%5d%5d%5d%10s%10s%5d (%3d) %c",
				  Sarjat[srj].sarjanimi,nilm[srj],ntulos[srj][kilpparam.valuku+1],ntulos[srj][0],
                  nkesk[srj],nhyl[srj], n_avoin(srj),
				  kaika,paika,psija,psija2,palk[srj] ? L'P' : L' ');
               }
			else
               {
               if( ntulos[srj][0] == 0 ) wcscpy(kaika, L"        ");
					else
						AIKATOWSTRS(kaika, jarjtulos[0][0][jalku[srj][0]][0], 0);
//                  >> 12) & 0x000fffffL,0),8);
			   kaika[8] = 0;
			   EnterCriticalSection(&tall_CriticalSection);
			   puuttuvat(paika,&psija,srj,1);
			   LeaveCriticalSection(&tall_CriticalSection);
			   swprintf(line,L" %-11s%3d%6d%6d%6d%6d%11s%11s%6d %c",Sarjat[srj].sarjanimi,
                  nilm[srj],ntulos[srj][0],nkesk[srj],nhyl[srj], n_avoin(srj),
				  kaika,paika,psija,palk[srj] ? L'P' : L' ');
               }
			vidspwmsg(l,0,7,0,line);
            rivit[srj] = l;
			l++;
            }
		 if(l >= ySize-4 || srj == sarjaluku - 1) {
            l = 4;
            do {
			   vidspwmsg(ySize-3,0,7,0,
				  L"Paina <Return> jatkaaksesi tai P merkit‰ksesi palkintojenjako");
			   ch2 = readkbd(&ch2,TRUE,spoolfl);
			   if (ch2 == 'P' || ch2 == 'p') {

				  snimi[0] = 0;
				  clrln(ySize-3);
				  vidspwmsg(ySize-3, 0, 7, 0, L"Anna sarja");
				  psrj = luesarja(snimi, &ch);
                  if (psrj >= 0) {
                     palk[psrj] = 1 - palk[psrj];
                     ll = rivit[psrj];
                     if (ll > 3 && ll < ySize-4)
                        viwrrectw(ll, 72+5*(k_pv>0), ll, 72+5*(k_pv>0), palk[psrj] ? L"P" : L" ", 7, 0, 0);
                     }
                  }
			   } while (ch2 != L'\r');
            clearframe();
			vidspwmsg(2,0,7,0,ots);
			}
         } while (++srj < sarjaluku);
#endif  // _CONSOLE
      }
   else {
	  EnterCriticalSection(&print_CriticalSection);
		if (tulprm->printer == GDIPRINTER) {
		  initpage(tulprm, k_pv ? L"C" : NULL);
			scale = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		}
	  else
		 initpage(tulprm, NULL);
      l = 0;
      swprintf(line,
		 L"Viim. maaliaika : %8.8s            kirjausaika : %s\r\n",
		 AIKATOWSTRS(st, tmaali,t0),wkello());
      putfld(tulprm, line,prtflds[1].pos,80,0,0);
      endline(tulprm, 0);
      if (k_pv) {
         putfld(tulprm, ots,0,80,0,0);
		 }
      else
         {
		 putfld(tulprm, L"Sarja", 2*scale, 5, 0, 0);
         putfld(tulprm, L"Ilm", 14*scale, 3, 1, 0);
         putfld(tulprm, L"Aika", 19*scale, 4, 1, 0);
         putfld(tulprm, L"Kesk", 25*scale, 4, 1, 0);
		 putfld(tulprm, L"Hyl", 32*scale, 3, 1, 0);
         putfld(tulprm, L"Auki", 37*scale, 4, 1, 0);
		 putfld(tulprm, L"K‰rkiaika", 43*scale, 9, 1, 0);
         putfld(tulprm, L"Mahd.aika", 54*scale, 9, 1, 0);
		 putfld(tulprm, L"Mahd.sija", 64*scale, 9, 1, 0);
         }
      endline(tulprm, 1);
      for (srj = 0; srj < sarjaluku; srj++) {
         if (nilm[srj] > 0) {
            if (k_pv) {
               if (ntulos[srj][kilpparam.valuku+1] == 0 ) wcscpy(kaika, L"        ");
			   else
					AIKATOWSTRS(kaika, jarjtulos[0][kilpparam.valuku+1][jalku[srj][kilpparam.valuku+1]][0],0);
					kaika[8] = 0;
					EnterCriticalSection(&tall_CriticalSection);
					puuttuvat(paika,&psija2,srj,1);
					puuttuvat(paika,&psija,srj,0);
					LeaveCriticalSection(&tall_CriticalSection);
					swprintf(line,L" %-11s%4d%6d%7d%5d%5d%5d%10s%10s%5d (%3d)",
						Sarjat[srj].sarjanimi,nilm[srj],ntulos[srj][kilpparam.valuku+1],ntulos[srj][0],
						nkesk[srj],nhyl[srj],n_avoin(srj),kaika,paika,psija,psija2);
					putfld(tulprm, line, 0, 80, 0, 0);
					}
				else
					{
					if( ntulos[srj][0] == 0 ) wcscpy(kaika, L"        ");
					else
						AIKATOWSTRS(kaika, jarjtulos[0][0][jalku[srj][0]][0], 0);
//						>> 12) & 0x000fffffL,0),8);
					kaika[8] = 0;
			   EnterCriticalSection(&tall_CriticalSection);
			   puuttuvat(paika,&psija,srj,1);
			   LeaveCriticalSection(&tall_CriticalSection);
			   putfld(tulprm, Sarjat[srj].sarjanimi, 2*scale, 11, 0, 0);
               putfld(tulprm, _itow(nilm[srj],line,10), 13*scale, 4, 1, 0);
               putfld(tulprm, _itow(ntulos[srj][0],line,10), 19*scale, 4,
                  1, 0);
               putfld(tulprm, _itow(nkesk[srj],line,10), 25*scale, 4, 1, 0);
               putfld(tulprm, _itow(nhyl[srj],line,10), 31*scale, 4, 1, 0);
			   putfld(tulprm, _itow(n_avoin(srj),line,10), 37*scale, 4, 1, 0);
               putfld(tulprm, kaika, 44*scale,8, 1, 0);
               putfld(tulprm, paika, 53*scale,10, 1, 0);
               putfld(tulprm, _itow(psija,line,10), 67*scale, 4, 1, 0);
			   }
            endline(tulprm, 0);
			if (++l >= tulprm->tulmuot.sivpit - 3) {
			   endpage(tulprm);
			   initpage(tulprm, NULL);
               l = 0;
               }
			}
		 }
      newline(tulprm, 1);
      if (k_pv) {
		 swprintf(line,L"Yhteens‰  %6d%6d%7d%5d%5d%5d",nilmt,nytulost,
            ntulost,nkeskt,nhylt,(nilmt - ntulost - nkeskt - nhylt - neilt));
         putfld(tulprm, line, 0, 80, 0, 0);
         }
	  else
         {
         putfld(tulprm, L"Yhteens‰", 2*scale, 11, 0, 0);
         putfld(tulprm, _itow(nilmt,line,10), 13*scale, 4, 1, 0);
         putfld(tulprm, _itow(ntulost,line,10), 19*scale, 4, 1, 0);
         putfld(tulprm, _itow(nkeskt,line,10), 25*scale, 4, 1, 0);
         putfld(tulprm, _itow(nhylt,line,10), 31*scale, 4, 1, 0);
		 putfld(tulprm, _itow(nilmt-ntulost-nkeskt-nhylt-neilt,line,10),
            37*scale,4, 1, 0);
         }
	  endline(tulprm, 0);
	  endpage(tulprm);
	  enddoc(tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
	  }
   }

void autoruutu(void)
   {
#ifdef _CONSOLE
   wchar_t st[16];
   wchar_t sl[6];

   if (autofl) {
	  sl[0] = autotulostusparam.rajaus;
	  sl[1] = 0;
	  if (autotulostusparam.rajaus == L'P') {
		 _itow(autotulostusparam.viimsija, st, 10);
		 wcscat(sl ,st);
		 }
	  swprintf(st, L" Laajuus %5s", sl);
	  viwrrectw(12,66,12,79,L" Auto       ON",7,0,0);
	  viwrrectw(13,66,13,79,st,7,0,0);
	  swprintf(st, L" Kynnys %6d", kynnys);
	  viwrrectw(14,66,14,79,st,7,0,0);
	  viwrrectw(15,66,15,73,L" Korkein",7,0,0);
	  }
   else {
	  wcscpy(st, L"              ");
      viwrrectw(12,66,12,79,L" Auto       EI",7,0,0);
      viwrrectw(13,66,13,79,st,7,0,0);
      viwrrectw(14,66,14,79,st,7,0,0);
      viwrrectw(15,66,15,79,st,7,0,0);
	  }
#endif
   }

void autoalku(wchar_t *koodit)
   {
   wchar_t *p;
   INT k;
   extern INT intv[];

   if (!lsttulparam.lstf)
	   return;
   autotulostusparam = lsttulparam;
   autotulostusparam.kohde = L'P';
   autotulostusparam.vaihelkm = k_pv > 0 ? 2 : 1;
   if (k_pv == kilpparam.n_pv_akt - 1) {
	   autotulostusparam.yhttuljarj = true;
	   autotulostusparam.yhttulpaino = true;
	  }
   else {
	   autotulostusparam.yhttuljarj = false;
	   autotulostusparam.yhttulpaino = false;
	  }
   autotulostusparam.rajaus = L'L';
   autofl = 1;
   if (!koodit[0])
	  goto pois;
   p = wcstok(koodit, L"/,=");
   if (!p) goto pois;
   *p = towupper(*p);
   if (*p == L'P') {
	   autotulostusparam.rajaus = L'P';
	   autotulostusparam.viimsija = _wtoi(p+1);
      }
   if (*p == L'T' || *p == L'I')
	   autotulostusparam.rajaus = *p;
   p = wcstok(NULL, L"/,");
   if (!p) goto pois;
   k = _wtoi(p);
   if (k > 0) kynnys = k;
   p = wcstok(NULL, L"/,");
   if (!p) goto pois;
   k = _wtoi(p);
   if (k > 5 && k < 1000) intv[0] = 18*k;
   p = wcstok(NULL, L"/,");
   if (!p) goto pois;
   for (k = 0; k < 9; k++) {
	  autotulostusparam.automaali[k] = 0;
      }
   for (; *p; p++)
      if (*p > L'0' && *p <= L'9') autotulostusparam.automaali[*p - L'1'] = 1;
   if (k_pv) {
      p = wcstok(NULL, L"/,");
      if (!p) return;
      *p = towupper(*p);
      switch (*p) {
         case L'2' :
			 autotulostusparam.yhttuljarj = false;
			 autotulostusparam.yhttulpaino = false;
			 break;
         case L'V' :
			 autotulostusparam.yhttuljarj = false;
			 autotulostusparam.yhttulpaino = false;
			 autotulostusparam.vaihelkm = 1;
             break;
         case L'Y' :
			 autotulostusparam.yhttuljarj = true;
			 autotulostusparam.yhttulpaino = true;
			 autotulostusparam.vaihelkm = 1;
			 break;
         case L'1' :
			 autotulostusparam.yhttuljarj = true;
			 autotulostusparam.yhttulpaino = true;
			 autotulostusparam.vaihelkm = 3;
             break;
         }
      }
   pois:
	;
   autoruutu();
   }

static int htmlalkufl;

//void htmlalku(wchar_t *wtitle, wchar_t *wheader, int frame, int emitvali, tulostusparamtp *tulprm)
void htmlalku(wchar_t *wtitle, wchar_t *wheader, int frame, tulostusparamtp *tulprm)
	{
	TextFl *hfl;
	wchar_t rivi[400], fnm[13] = L"HTMLALKY.TXT", defots[1] = L"";

	htmlalkufl = 0;
	fnm[7] = tulprm->yksihtml ? L'Y' : L'M';
	if (tulprm->yksihtml || frame) {
		hfl = new TextFl(fnm, L"rt");
		if (hfl->IsOpen()) {
			while (!hfl->Feof()) {
				if (hfl->ReadLine(rivi, 198) != NULL) {
					tulprm->writehtml(rivi);
					htmlalkufl = 1;
					}
				}
			}
		delete hfl;
		}
	if (!htmlalkufl) {
		tulprm->writehtml(L"<!DOCTYPE html>\n<html>");
		if (tulprm->merkit == L'A')
			tulprm->writehtml(L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
		else
			tulprm->writehtml(L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
		css_string(tulprm, 4);
		if (!frame && wtitle && wtitle[0]) {
			tulprm->writehtml(L"<title>");
			tulprm->writehtml(wtitle);
			tulprm->writehtml(L"</title>\n");
			}
		tulprm->writehtml(L"</head>");
		if (tulprm->yksihtml) {
			tulprm->writehtml(L"<body>");
			if (wheader && wheader[0]) {
				tulprm->writehtml(L"<H2 CLASS=otsikko>");
				tulprm->writehtml(wheader);
				tulprm->writehtml(L"</H2>\n");
				}
			}
		else if (frame) {
			tulprm->writehtml(L"<body class='tulframe'>");
			}
		}
	}

void htmlloppu(tulostusparamtp *tulprm)
   {
	TextFl *hfl;
   wchar_t rivi[200], fnm[13] = L"HTMLLOPY.TXT";

   fnm[7] = tulprm->yksihtml ? L'Y' : L'M';
   if (htmlalkufl) {
		hfl = new TextFl(fnm, L"rt");
		if (hfl->IsOpen()) {
		  while (!hfl->Feof()) {
			 if (hfl->ReadLine(rivi, 198) != NULL) {
				tulprm->writehtml(rivi);
				}
			 }
		  }
	  delete hfl;
	  }
   if (!htmlalkufl)
	  tulprm->writehtml(L"</body>\n</html>\n");
   htmlalkufl = 0;
   }

#ifdef AUTOFILE
void startaf(INT kysy)
   {
   wchar_t *p;
   if (aftulparam.kohde == L'H' || aftulparam.kohde == L'M') {
//	   if (aftulparam.yksihtml)
		   {
		   if (!autofileparam.wtitlea[0]) {
			   if (wtitle[0])
					wcsncpy(autofileparam.wtitlea, wtitle, 76);
			   else if (paaots[0])
				   otsTeksti(autofileparam.wtitlea, paaots, 76, k_pv);
			   else
				   otsTeksti(autofileparam.wtitlea, sarjaots, 76, k_pv);
			   }
		   else
			  for (p = autofileparam.wtitlea; *p; p++) if (*p == L'_') *p = L' ';
		   autofileparam.wtitlea[76] = 0;
		   elimwbl(autofileparam.wtitlea);
		   if (!autofileparam.wheadera[0]) {
			  if (autofileparam.wheadera[0])
				   otsTeksti(autofileparam.wheadera, wheader, 76, k_pv);
			   else if (paaots[0])
				   otsTeksti(autofileparam.wheadera, paaots, 76, k_pv);
			   else
				   otsTeksti(autofileparam.wheadera, sarjaots, 76, k_pv);
			   }
		   else
			   for (p = autofileparam.wheadera; *p; p++) if (*p == L'_') *p = L' ';
		   autofileparam.wheadera[76] = 0;
		   elimwbl(autofileparam.wheadera);
		   }
		}
#if defined(TCPSIIRTO)
   if (!kysy && fileyht >= 0) {
//      fileyht = max(fileyht, NREGNLY);
//      fileyht += MAX_LAHPORTTI - 1;
	  if (!comopen[fileyht]) {
		 InitializeCriticalSection(&tcp_CriticalSection[fileyht]);
		 if (openportTCP(hComm+fileyht, ipparam+fileyht)) {
			writeerror_w(L"Tiedostosiirron TCP-yhteyden avaaminen ei onnistunut", 0);
			}
		 else
			comopen[fileyht] = 1;
		 }
	  }
#endif
   }

#if defined(TCPSIIRTO)
#define TCPBUF 1000

//DWORD WINAPI sendtcpfile(LPVOID lpCn)
void sendtcpfile(LPVOID lpCn)
   {
   char buf[TCPBUF+128];
   int err = 0, nsent, nread;
   TextFl *af_tied;

   tulostusparamtp *tulprm = (tulostusparamtp *) lpCn;

   if (!tulprm->lstf || tulprm->lstf->prno != -2 || tulprm->lstf->u.Fl == NULL)
	  return;
   af_tied = tulprm->lstf->u.Fl;
   EnterCriticalSection(&tcp_CriticalSection[fileyht]);
   af_tied->Rewind();
   if (af_tied->IsOpen()) {
	  buf[0] = STX;
	  if ((err = wrt_st_TCP(hComm[fileyht], 1, buf, &nsent)) == 0) {
		 while (taustaon && !af_tied->Feof()) {
			nread = af_tied->ReadBytes(buf, 1, TCPBUF);
			if (!nread)
			   break;
			if ((err = wrt_st_TCP(hComm[fileyht], nread, buf, &nsent)) != 0)
			   break;
			}
		 if (!err) {
			buf[0] = ETX;
			wrt_st_TCP(hComm[fileyht], 1, buf, &nsent);
			}
		 }
	  }
   closeprfile(tulprm->lstf);
   LeaveCriticalSection(&tcp_CriticalSection[fileyht]);
//   return(0);
   }
#endif

int autofile(int kaikki)
	{
	int nt, i, l = 0, sv = 0, yht = 0;
	int iSrj = 0, nSrj = 0, afSrj[MAXSARJALUKU+MAXYHD];
	bool mobilFl = false;
	bool kirjoitettu = false;
	wchar_t laajuus = L'L';
	TextFl *aflst = 0;
	wchar_t buf[1002], *p, *p1;
	wchar_t xmllaji;
#if defined(TCPSIIRTO)
	static HTHREAD FiletcpThread;
#endif

/*
	static tulostusparamtp afsrjtulprm;
	static bool initialisoitu = false;

	if (!initialisoitu) {
		afsrjtulprm = aftulparam;
		afsrjtulprm.lstf = NULL;
		initialisoitu = true;
		}
*/
	if (autofileparam.af_flag)
		return(0);
	if (aftulparam.lstf && aftulparam.lstf->prno == -2 && aftulparam.lstf->u.Fl)
		return(1);
#ifdef _BORLAND_
	if (autofileparam.autokomentolaji == 2) {
		if (!FormJakelu) {
			startAutojakelu();
			Sleep(1000);
			}
		if (!FormJakelu)
			return(0);
		if (FormJakelu->LahetysKesken)
			return(0);
		}
#endif
	autofileparam.af_kaikki = kaikki != 0;
#if defined(TCPSIIRTOxx)
	if (fileyht >= 0) {
//		EnterCriticalSection(&tcp_CriticalSection[fileyht]);
		}
#endif
//	autofileparam.kopiofl = 1;
	aftulparam.merkit = L'A';
	EnterCriticalSection(&autotul_CriticalSection);
	if ((aftulparam.kohde != L'H' && aftulparam.kohde != L'M')|| aftulparam.yksihtml) {
		aftulparam.lstf = openprfile(autofileparam.afname, -1, 1, 2, (char *)&aftulparam.merkit, 0);
//		af_tied = new TextFl(autofileparam.afname, L"w+t");
		if (!aftulparam.lstf) {
			wchar_t msg[200];
			swprintf(msg, L"Virhe autotulostustiedostoa %s avattaessa", autofileparam.afname);
			writeerror_w(msg, 2000);
			LeaveCriticalSection(&autotul_CriticalSection);
			return(1);
			}
		}
	autofileparam.af_flag = 1;
	aftulparam.xmlva = autofileparam.af_xmlva;
	do {
		switch (aftulparam.kohde) {
			case L'H':
//				autofileparam.kopiofl = 0;
				aftulparam.tulmuot = filetulosmuot;
				aftulparam.p_fld = fileflds;;
				if (aftulparam.yksihtml)
					htmlalku(autofileparam.wtitlea, autofileparam.wheadera, 0, &aftulparam);
				break;
			case L'M':
				aftulparam.tulmuot = mobiltulosmuot;
				aftulparam.p_fld = mobilflds;
				if (aftulparam.yksihtml)
					htmlalku(autofileparam.wtitlea, autofileparam.wheadera, 0, &aftulparam);
				break;
			case L'X':
				xmlots(&aftulparam);
				aftulparam.rajaus = aftulparam.xmlva == L'M' ? L'I' : L'L';
				break;
			case L'T':
				break;
			}

		aflst = new TextFl(autofileparam.aflstnm, L"rt");

		nSrj = 0;
		iSrj = 0;
		for (int srj = -1;;) {
			p1 = NULL;
			if (nSrj || (aflst->IsOpen() && (aftulparam.kohde == L'H' || aftulparam.kohde == L'M') && !aftulparam.yksihtml)) {
				if (iSrj < nSrj) {
					srj = afSrj[iSrj++];
					}
				else {
					nSrj = 0;
					iSrj = 0;
					aftulparam.yksihtml = false;
					if (aflst->Feof())
						break;
					if (aflst->ReadLine(buf, 1000) == NULL)
						continue;
					p = wcstok(buf, L" ;\t\n");
					if (wmemcmpU(p, L"HTML=", 5) == 0 || wmemcmpU(p, L"MHTML=", 6) == 0) {
						p1 = wcsstr(p, L"=")+1;
						if (wcslen(p1) < 3)
							continue;
						if (aftulparam.lstf) {
							htmlloppu(&aftulparam);
							closeprfile(aftulparam.lstf);
							aftulparam.lstf = 0;
							}
						if (towupper(*p) == L'M') {
							aftulparam.p_fld = mobilflds;
							aftulparam.tulmuot = mobiltulosmuot;
							}
						else {
							aftulparam.p_fld = fileflds;
							aftulparam.tulmuot = filetulosmuot;
							}
						aftulparam.piste = 0;
						if (aftulparam.sarjalista == NULL)
							aftulparam.sarjalista = new INT16[MAXSARJALUKU+MAXYHD];
						memset(aftulparam.sarjalista, 0, (MAXSARJALUKU+MAXYHD)*2);
						memset(afSrj, 0, sizeof(afSrj));
						for (int i = 0; i < MAXSARJALUKU+MAXYHD+1; i++) {
							p = wcstok(NULL, L" ;\t\n");
							if (p == NULL)
								break;
							if (wcscmpU(p, L"KAIKKI") == 0) {
								for (int j = 0; j < sarjaluku; j++) {
									if (ntulos[j][aftulparam.piste] > 0) {
										aftulparam.sarjalista[j] = 1;
										afSrj[nSrj++] = j;
										}
									}
								break;
								}
							srj = haesarja_w(p, true);
							if (i == 0 && srj < 0) {
								aftulparam.piste = _wtoi(p);
								if (aftulparam.piste > kilpparam.valuku)
									aftulparam.piste = 0;
								continue;
								}
							if (srj >= 0 && ntulos[srj][aftulparam.piste] > 0) {
								afSrj[nSrj++] = srj;
								aftulparam.sarjalista[srj] = 1;
								}
							}
						if (nSrj == 0) {
							continue;
							}
						if ((aftulparam.lstf = openprfile(p1, -1, 1, 2, (char *)&aftulparam.merkit, 0)) == NULL)
							continue;
						aftulparam.yksihtml = true;
						if (aftulparam.tulmuot.otsikot & 2)
							htmlalku(autofileparam.wtitlea, autofileparam.wheadera, 0, &aftulparam);
						else
							htmlalku(0, 0, 0, &aftulparam);
						srj = afSrj[0];
						iSrj = 1;
						potsfl = TRUE;
						}
					else {
						if (!p || (srj = haesarja_w(p, true)) < 0)
							continue;
						if ((p = wcstok(NULL, L" ;\t\n")) == 0)
							continue;
						aftulparam.piste = _wtoi(p);
						p1 = wcstok(NULL, L" ;\t\n");
						}
					}
				}
			else if (aftulparam.yksihtml || aflst == NULL) {
				srj++;
				if (srj >= sarjaluku+nsarjayhd)
					break;
				p = Sarjat[srj].sarjanimi;
				aftulparam.piste = 0;
//         p1 = L"";
				}
			else
				break;
			if (aftulparam.kohde == L'X') {
				nt = 0;
				if (xmlsarja < 0 || srj == xmlsarja) {
					if (!yht) {
						for (i = 0; i <= Sarjat[srj].valuku[k_pv]; i++) {
							if (i == 0 && aftulparam.rajaus == L'I')
								nt += nilm[srj];
							else
								nt += ntulos[srj][i];
							}
						}
					else
					nt += ntulos[srj][kilpparam.valuku+1];
					}
				}
			else {
				if (autofileparam.vainMuutt && fprior[srj] == 0)
					continue;
				fprior[srj] = 0;
				nt = ntulos[srj][aftulparam.piste];
				}
			if (nt) {
				if ((aftulparam.kohde == L'H' || aftulparam.kohde == L'M') && !aftulparam.yksihtml) {
					if (aftulparam.lstf) {
						htmlloppu(&aftulparam);
						closeprfile(aftulparam.lstf);
						aftulparam.lstf = 0;
						}
					if ((aftulparam.lstf = openprfile(p1 != NULL ? p1 : htmlSrjFName(autofileparam.afname, srj),
						-1, 1, 2, (char *)&aftulparam.merkit, 0)) == NULL)
						continue;
					htmlalku(0, 0, 1, &aftulparam);
					}
				if (aftulparam.kohde == L'X') {
					xmllaji = L'X';
					if (yht && k_pv) {
						aftulparam.yhttuljarj = true;
						aftulparam.yhttulpaino = true;
						aftulparam.vaihelkm = 1;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						kirjoitettu = true;
						}
					else {
						aftulparam.yhttuljarj = false;
						aftulparam.yhttulpaino = false;
						aftulparam.vaihelkm = 1;
						kirjoitettu = true;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						}
					}
				else {
					if (k_pv) {
						aftulparam.yhttuljarj = false;
						aftulparam.yhttulpaino = false;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						kirjoitettu = true;
						}
					else {
						aftulparam.yhttuljarj = false;
						aftulparam.yhttulpaino = false;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						kirjoitettu = true;
						}
					}
				}
			}
		} while (k_pv && aftulparam.kohde == L'X' && !yht++);
	if (aflst)
		delete aflst;
	switch (aftulparam.kohde) {
		case L'H':
		case L'M':
			htmlloppu(&aftulparam);
			break;
		case L'X':
			xmlloppu(&aftulparam);
			break;
		case L'T':
			break;
		}
#if defined(TCPSIIRTO)
	if (fileyht >= 0) {
//      FiletcpThread = CreateThread(NULL, 0, sendtcpfile, 0, 0, &IDThread);
		FiletcpThread = _beginthread(sendtcpfile, 40960, &aftulparam);
//	  LeaveCriticalSection(&tcp_CriticalSection[fileyht]);
		}
	else
#endif
		{
		if (aftulparam.lstf)
			closeprfile(aftulparam.lstf);
		aftulparam.lstf = NULL;
		}
	if (kirjoitettu && autofileparam.autokomentolaji == 1 && autofileparam.autokomento[0]) {
		char cst[210] = "";
		wcstooem(cst, autofileparam.autokomento, 200);
		strcat(cst, " >NUL");
		system(cst);
		}
#ifdef _BORLAND_
	if (kirjoitettu && autofileparam.autokomentolaji == 2) {
		if (FormJakelu)
			FormJakelu->SendFiles();
		}
#endif
	autofileparam.af_flag = 0;
	LeaveCriticalSection(&autotul_CriticalSection);
	return(0);
	}
#endif  // AUTOFILE

#ifdef TAULU_2
static int afsrj, afva, afnparas = 30;

int autofile(int kaikki)
   {
   int l = 0, sv = 0;
   if (ntulos[afsrj][afva])
	  tulostasarja(&afsrj, afva, L'V', L'P', L'E', cda, cja, k_pv, afnparas, 0, &l, &sv, 1);
   return(0);
   }

#endif

#ifdef JOUKKUEPISTEET
#ifndef _CONSOLE

int luePisteSrj(wchar_t *flnm)
{
	wchar_t msg[250], Buf[120], line[100], filenm[200] = L"pistesrj.lst";
	TextFl *psrjfile;
	int srj, ret = 0;

	for (srj = 0; srj < sarjaluku; srj++) {
		Sarjat[srj].npist[k_pv] = 0;
		}
	if (flnm && flnm[0])
		wcsncpy(filenm, flnm, 199);
	psrjfile = new TextFl(filenm, L"rt");
	if (!psrjfile || !psrjfile->IsOpen()) {
	   swprintf(msg, L"Tiedostoa %s ei lˆydy", filenm);
	   ret = 1;
//	   writeerror_w(msg, 0);
	   }
	else {
		int l, npiste;

		while (!psrjfile->Feof()) {
			psrjfile->ReadLine(Buf, 98);
		   if (swscanf(Buf, L"%s %d\n", line, &npiste) < 2)
				break;
		   if ((srj = haesarja_w(line, true)) >= 0) {
			   Sarjat[srj].npist[k_pv] = (wchar_t) npiste;
			  }
		   }
		}
	if (psrjfile)
		delete psrjfile;
	return(ret);
	}

// joukkuepisteet() laskee sijoituksiin perustuvat pisteet kaikille sarjoille, tai
// sarjoille, jotka luetellaan tiedostossa pistesrj.lst (luetaan aliohjelmassa luePisteSrj())
// Laskenta perustuu pisteit‰ saavien lukum‰‰r‰‰n, joka voi olla kiinte‰ tai riippua
// sarjan osanottajam‰‰r‰st‰.
// Jos tiedostoa luePisteSrj() ei ole, annetaan kaikille sarjoille sama m‰‰r‰ tulostuskaavakkeella
// ja v‰litet‰‰n parametrilla tulprm->optiot2
//
// naytolle					kertoo, ett‰ tulokset n‰ytet‰‰n heti ikkunassa FormMsgs1->Memo1
// tulprm->optiot & 0x1c0	kertoo, mik‰ tieto on joukkueen tunnus
// tulprm->optiot & 0x200 	kertoo saako voittaja lis‰pisteen
// tulprm->optiot & 0x400 	kertoo riippuvatko voittajan pisteet osanottajam‰‰r‰st‰


void joukkuepisteet(bool naytolle, tulostusparamtp *tulprm)
   {
   typedef struct {
	  INT piste;
	  wchar_t seura[LSEURA+1];
	  } pistetp;
   pistetp *pistetaul;
   wchar_t seura[LSEURA+1], line[50], ch, Buf[100];
   char ch2;
   INT srj, npiste, p, sj, sj1, is, nseura = 0, l, tunnlaji;
   INT32 tls1;
   TextFl *psrjfile;
   kilptietue kilp;

   if (luePisteSrj(0) == 1) {
	   for (srj = 0; srj < sarjaluku; srj++) {
		   if (!tulprm || !tulprm->sarjalista || tulprm->sarjalista[srj]) {
			   Sarjat[srj].npist[k_pv] = (tulprm->optiot2 & 0xff0) / 16;
			   }
		   }
	   }
   if ((pistetaul = (pistetp *)calloc((MAXNSEURA+1), sizeof(pistetp))) == NULL)
	  return;
   tunnlaji = tulprm ? ((tulprm->optiot & 0x1c0) / 64) : 3;
   for (srj = 0; srj < sarjaluku; srj++) {
	  if (!tulprm || tulprm->optiot & 0x400) {
		  npiste = min(ntulos[srj][0], Sarjat[srj].npist[k_pv]);
		  }
	  else
		  npiste = Sarjat[srj].npist[k_pv];
	  if (npiste == 0 || ntulos[srj][0] == 0)
		  continue;
	  EnterCriticalSection(&tall_CriticalSection);
	  p = jalku[srj][0];
	  sj = 1;
	  tls1 = 0;
	  while (p > 0) {
		 kilp.GETREC(p);
		 p = seuraava(0, 0, p);
		 if (tls1 != kilp.tulos_pv(k_pv, true)) {
			sj1 = sj;
			tls1 = kilp.tulos_pv(k_pv, true);
			}
		 if (sj1 > npiste) break;
		 memset(seura, 0, sizeof(seura));
		 switch(tunnlaji) {
			case 0:
				wcscpy(seura, kilp.seuratieto(0, false));
				break;
			case 1:
				wcscpy(seura, kilp.seuratieto(0, true));
				break;
			case 2:
				wcscpy(seura, kilp.yhdistys);
				break;
			case 3:
				wcscpy(seura, kilp.joukkue);
				break;
			case 4:
				if (kilp.piiri > 0)
					_itow(kilp.piiri, seura, 10);
				break;
			}
		 if (!seura[0])
			continue;
		 for (is = 0; pistetaul[is].piste; is++) {
			if (!wcscmpU(seura, pistetaul[is].seura)) break;
			}
		 if (!pistetaul[is].piste) {
			wcscpy(pistetaul[is].seura, seura);
			nseura++;
			}
		 pistetaul[is].piste += npiste + 1 - sj1 +
			((sj1 == 1 && (tulprm && tulprm->optiot & 0x200)) ? 1 : 0);
		 sj++;
		 }
	  LeaveCriticalSection(&tall_CriticalSection);
	  }
   l = 0;
   if (nseura) {
	  qsortint((void *) pistetaul, nseura, sizeof(pistetp), 0,  sizeof(pistetaul[0].piste)==4);
	  npiste = 0;
	 if (naytolle) {
		if (!FormJoukkueTilanne)
			return;
		FormJoukkueTilanne->Memo1->Clear();
		swprintf(line,L"   Pistetilanne        %s", wkello());
		FormJoukkueTilanne->Memo1->Lines->Add(line);
		FormJoukkueTilanne->Memo1->Lines->Add(L"  ");
		}
	  else if (tulprm->kohde == 'P') {
		 initpage(tulprm, L"C");
		 EnterCriticalSection(&print_CriticalSection);
		 }
	  else if (tulprm->kohde == 'H' || tulprm->kohde == 'M') {
		   tulprm->writehtml(L"<pre>\n");
		   swprintf(line,L"   Pisteet           %s\n\n", wkello());
		   tulprm->writehtml(line);
		   }
	  for (is = nseura-1; is >= 0; is--) {
		 wchar_t tunnst[LSEURA+1] = L"";
		 wcsncpy(tunnst, pistetaul[is].seura, LSEURA);
		 if (tunnlaji == 4) {
			int pri = _wtoi(pistetaul[is].seura);
			if (pri <= piiriluku && pri > 0 && wcslen(piirinimi[pri-1]) > 1)
				wcsncpy(tunnst, piirinimi[pri-1], LSEURA);
			}
		 if (npiste != pistetaul[is].piste) {
			sj1 = nseura - is;
			npiste = pistetaul[is].piste;
			}
		 if (naytolle) {
			swprintf(line, L"   %3d.  %-30.30s %4d", sj1,
			   tunnst, pistetaul[is].piste);
			FormJoukkueTilanne->Memo1->Lines->Add(line);
//			FormJoukkueTilanne->Show();
			}
		 else if (tulprm->kohde == L'P') {
			if (l >= tulprm->tulmuot.sivpit) {
			   l = 0;
			   endpage(tulprm);
			   }
			if (l == 0) {
				swprintf(line,L"   Pistetilanne        %s", wkello());
				putfld(tulprm, line, 10, 80, 0, 0);
				endline(tulprm, 1);
			   l = 2;
			   }
			swprintf(line, L"   %3d.  %-30.30s %4d", sj1,
			   tunnst, pistetaul[is].piste);
			putfld(tulprm, line, 10, 80, 0, 0);
			endline(tulprm, 0);
			l++;
			}
		 else if (tulprm->kohde == L'H' || tulprm->kohde == L'M') {
			swprintf(line, L"   %3d.  %-30.30s %4d\n", sj1,
			   tunnst, pistetaul[is].piste);
		   tulprm->writehtml(line);
		   }
		 }
	  if (naytolle) {
//			FormJoukkueTilanne->Show();
			}
	  else if (tulprm->kohde == L'P') {
		  enddoc(tulprm);
		  LeaveCriticalSection(&print_CriticalSection);
		  }
	  else if (tulprm->kohde == 'H' || tulprm->kohde == L'M') {
		   tulprm->writehtml(L"</pre>\n");
		   }
	  else {
		 }
	  }
   free(pistetaul);
   }

#endif // !_CONSOLE
#endif // JOUKKUEPISTEET


void luepiirit(wchar_t *flname)
   {
   TextFl *piirifile;
   wchar_t line[121], *p, *flnm0 = L"PIIRIT.LST";
   INT k;

   piirifile = new TextFl(flname != NULL ? flname : flnm0, L"rt");
   if (piirifile->IsOpen()) {
	  memset(piirinimi, 0, sizeof(piirinimi));
	  for (;;) {
		 if (piirifile->ReadLine(line, 120) == NULL)
			break;
		 p = wcstok(line, L" ;\t");
		 if ((k = _wtoi(p)) == 0)
			continue;
		 if (k >= piiriluku)
			continue;
		 p = wcstok(NULL, L";\t\n");
		 wcsncpy(piirinimi[k-1], p, sizeof(piirinimi[0])/2-1);
		 }
	  }
   delete piirifile;
	}

#if defined(JOUKKUETUL)

#define JKOKO 300

typedef struct {
   INT32 tls;
   INT jasenet[JKOKO];
   int jastls[JKOKO];
   INT jnro;
   wchar_t seura[LSEURA+1];
   } jluettp;

static int sijaluvut;

void joukkuetul(bool naytolle, int srj, tulostusparamtp *tulprm)
   {
   INT32 tls0 = 0;
   wchar_t st[120], stsj[6], st2[64], st1[60], tc, ch, wbuf[80], co;
   INT i, j, jno, njoukk, maxjoukk, n, p, d, sj;
   jluettp  *joukk, *jk;
   kilptietue kilp;
   static INT jkoko = JKOKO;
   static int ytfl = 0;
   TextFl *pistefile;
   wchar_t pistefilenm[] = L"SIJAPISTEET.LST";
   int sijapisteet[1000];
   int joukkuekentta;
   int nouseva = -1;
   bool vaadijoukkue;
   bool kaytaseuranimea;
   int minkoko;

	if (naytolle) {
		if (!FormJoukkueTilanne)
			return;
		FormJoukkueTilanne->Memo1->Clear();
		swprintf(st,L"   Joukkuetilanne        %s", wkello());
		FormJoukkueTilanne->Memo1->Lines->Add(st);
		FormJoukkueTilanne->Memo1->Lines->Add(L"  ");
		}
	if (tulprm->optiot & 2)
		nouseva = 1;
	joukkuekentta = 1 + (tulprm->optiot % 512) / 64;   // 0: seuratieto, 1: seura, 2: lyhenne, 3: yhdistys, 4: joukkue, 5: piirikoodi
	jkoko = (tulprm->optiot2 & 0x3f0) / 16;
	if (jkoko > JKOKO)
		jkoko = JKOKO;
	minkoko = (tulprm->optiot2 & 0x3f000) / 4096;
	if (minkoko < 1)
		minkoko = 1;
	if (minkoko > jkoko)
		minkoko = jkoko;
	kaytaseuranimea = tulprm->optiot % 64 / 32;
	vaadijoukkue = tulprm->optiot % 32 / 16;
	sijaluvut = tulprm->optiot % 16 / 4;
	co = tulprm->kohde;
	ytfl = tulprm->yhttulpaino;
		maxjoukk = ntulos[srj][0]/2+200;
		joukk = (jluettp *) calloc(maxjoukk, sizeof(jluettp));
		if (!joukk) {
			writeerror_w(L"Muisti ei riit‰ joukkuetuloksille", 0);
			return;
			}
		njoukk = 0;
		if (sijaluvut == 2) {
			EnterCriticalSection(&tall_CriticalSection);
			for (j = 0; j < sarjaluku; j++)
				sijat(j);
			LeaveCriticalSection(&tall_CriticalSection);
			for (j = 0; j < (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])); j++)
				sijapisteet[j] = j+1;
			if ((pistefile = new TextFl(pistefilenm, L"r")) != NULL) {
				j = 0;
				while (pistefile->IsOpen() && !pistefile->Feof()) {
					pistefile->ReadLine(st, sizeof(st)/2-1);
					if (wcslen(st) > 1)
						sijapisteet[j++] = _wtoi(st);
					if (j >= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
						break;
					}
				delete pistefile;
				}
			}
		EnterCriticalSection(&tall_CriticalSection);
		p = jalku[srj][ytfl ? kilpparam.valuku+5 : 0];
		while (p > 0) {
			int tls;
			d = p;
			kilp.GETREC(d);
			p = SeuraavaSrj(srj, ytfl ? kilpparam.valuku+1 : 0, p);
			if (!kilp.yhteisotieto(joukkuekentta)[0])
				continue;
			if (vaadijoukkue && (kilp.joukkue[0] == 0 ||
			   (kilp.joukkue[0] == L'0' && _wtoi(kilp.joukkue) == 0)))
				continue;
			if (sijaluvut == 1 && kilp.pisteet[0] == 0)
				continue;
			jno = 1;
			if (sijaluvut == 2) {
				sj = kilp.p_sija(0);
				if (sj > 0 && sj <= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
					sj = sijapisteet[sj-1];
				tls = sj;
				}
			else if (sijaluvut == 1) {
				tls = nouseva*kilp.pisteet[0];
				}
			else {
				tls = kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2);
				}
			for (jk = joukk; jk < joukk+njoukk; jk++) {
				if (!jk->seura[0]) break;

				if (wcscmpU(jk->seura, kilp.yhteisotieto(joukkuekentta)))
					continue;
/*
				if (jkoko == minkoko && jk->jasenet[jkoko-1]) {
					jno++;
					continue;
					}
				else
*/
					break;
				}
			if (jk - joukk >= maxjoukk)
				break;
			if (jk->seura[0]) {
				for (j = 0; j < JKOKO && jk->jasenet[j] && tls >= jk->jastls[j]; j++) ;
				if (j < JKOKO-1 && jk->jastls[j]) {
					jk->tls -= jk->jastls[JKOKO-1];
					memmove(jk->jasenet+j+1, jk->jasenet+j, (JKOKO - j - 1) * sizeof(jk->jasenet[0]));
					memmove(jk->jastls+j+1, jk->jastls+j, (JKOKO - j - 1) * sizeof(jk->jastls[0]));
					}
				if (j < JKOKO) {
					jk->jasenet[j] = d;
					jk->jastls[j] = tls;
					jk->tls += tls;
					}
				}
			else {
				wcscpy(jk->seura, kilp.yhteisotieto(joukkuekentta));
				jk->jnro = jno;
				jk->jasenet[0] = d;
				jk->jastls[0] = tls;
				jk->tls = tls;
				njoukk++;
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		for (jk = joukk; jk < joukk+maxjoukk; jk++) {
			if (njoukk >= maxjoukk || jk->jasenet[0] == 0)
				break;
			if (jk->jasenet[jkoko]) {
				joukk[njoukk].jnro = jk->jnro+1;
				wcscpy(joukk[njoukk].seura, jk->seura);
				for (int ii = jkoko; ii < JKOKO && jk->jasenet[ii]; ii++) {
					joukk[njoukk].jasenet[ii-jkoko] = jk->jasenet[ii];
					joukk[njoukk].jastls[ii-jkoko] = jk->jastls[ii];
					jk->tls -= jk->jastls[ii];
					joukk[njoukk].tls += jk->jastls[ii];
					jk->jasenet[ii] = 0;
					jk->jastls[ii] = 0;
					}
				njoukk++;
				}
			}

		for (jk = joukk+njoukk-1; jk >= joukk; jk--) {
			if (!jk->jasenet[minkoko-1]) {
				njoukk--;
				memmove(jk, jk+1, (joukk + njoukk - jk) * sizeof(jluettp));
				}
			}
		qsortint((void *) joukk, njoukk, sizeof(jluettp), 0, 1);
		n = 0;
		sj = 1;
		jk = joukk;
		EnterCriticalSection(&print_CriticalSection);
		for (j = 1; j <= njoukk; j++, jk++) {
			if (!n) {
				swprintf(st, L"Joukkuetulokset  -  %s", Sarjat[srj].sarjanimi);
				if (naytolle) {
					FormJoukkueTilanne->Memo1->Lines->Add(st);
					n = 2;
					}
				else if (co == L'P') {
					initpage(tulprm, NULL);
					putfld(tulprm, st, prtflds[0].pos, 80, 0, 0);
					endline(tulprm, 1);
					n = 2;
					}
				else {
					tulprm->writehtml(L"<h3>");
					tulprm->writehtml(st);
					tulprm->writehtml(L"</h3>\n<pre>\n");
					n++;
					}
				}
			if (jk->tls != tls0) sj = j;
			_itow(sj, stsj, 10);
			if (kaytaseuranimea) {
				kilp.GETREC(jk->jasenet[0]);
				if (jk->jnro > 1)
					swprintf(st1,L"%s -%d", kilp.seura, jk->jnro);
				else
					wcscpy(st1, kilp.seura);
				}
			else {
				if (jk->jnro > 1)
					swprintf(st1,L"%s -%d", jk->seura, jk->jnro);
				else
					wcscpy(st1, jk->seura);
				}
			if (sijaluvut) {
				_itow(jk->tls > 0 ? jk->tls : -jk->tls, st2, 10);
				}
			else {
				AIKATOWSTRS(st2, jk->tls, 0);
				st2[pvparam[k_pv].laika2] = 0;
				elimwzb1(st2);
				}
			if (naytolle) {
				swprintf(st, L"%4.4s.  %-33.33s %13s", stsj, st1, st2);
				FormJoukkueTilanne->Memo1->Lines->Add(st);
				}
			else if (co == 'P') {
				putfld(tulprm, stsj, prtflds[F_SJ].pos, prtflds[F_SJ].len, 1, 0);
				putfld(tulprm, st1, prtflds[1].pos, 40, 0, 0);
				putfld(tulprm, st2, prtflds[F_TLS].pos, prtflds[F_TLS].len, 1, 0);
				endline(tulprm, 0);
				}
			else {
				swprintf(st, L"<b>%4.4s.  %-33.33s %13s</b>\n", stsj, st1, st2);
				tulprm->writehtml(st);
				}
			st[0] = 0;
			for (i = 0; i < jkoko; i++) {
				st2[0] = 0;
				if (jk->jasenet[i] <= 0)
					break;
				kilp.GETREC(jk->jasenet[i]);
				if (sijaluvut == 2) {
					_itow(kilp.p_sija(0), st1, 10);
					}
				else if (sijaluvut == 1) {
					_itow(kilp.pisteet[0], st1, 10);
					if (k_pv > 0) {
						swprintf(st2, L"  (%d", kilp.pv[0].pvpisteet[0]);
						for (int ipv = 1; ipv <= k_pv; ipv++)
							swprintf(st2+wcslen(st2), L"+%d", kilp.pv[ipv].pvpisteet[0]);
						wcscat(st2, L")");
						}
					}
				else {
					AIKATOWSTRS(st1, kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2), 0);
					st1[pvparam[k_pv].laika2] = 0;
					elimwzb1(st1);
					}
				if (naytolle) {
					if (srj < sarjaluku)
						swprintf(st, L"           %-33.33s %13s", kilp.nimi(wbuf,33,0), st1);
					else
						swprintf(st, L"           %-33.33s %-10.10s %13s", kilp.nimi(wbuf,33,0), Sarjat[kilp.Sarja()].sarjanimi, st1);
					FormJoukkueTilanne->Memo1->Lines->Add(st);
					}
				else if (co == L'P') {
					putfld(tulprm, kilp.nimi(wbuf, 80, tulprm->tulmuot.vaihdanimet), prtflds[F_NIMI].pos+2*tulprm->tulmuot.numlev, 30, 0, 0);
					putfld(tulprm, st1, prtflds[F_TLS].pos+2*tulprm->tulmuot.numlev, prtflds[F_TLS].len, 1, 0);
					if (st2) {
						elimwbl(st2);
						putfld(tulprm, st2, prtflds[F_PIST].pos, prtflds[F_PIST].len, 0, 0);
						}
					endline(tulprm, 0);
					}
				else {
					if (srj < sarjaluku)
						swprintf(st, L"           %-33.33s %13s%s\n", kilp.nimi(wbuf,33,tulprm->tulmuot.vaihdanimet), st1, st2);
					else
						swprintf(st, L"           %-33.33s %-10.10s %13s%s\n", kilp.nimi(wbuf,33,tulprm->tulmuot.vaihdanimet), Sarjat[kilp.Sarja()].sarjanimi, st1, st2);
					tulprm->writehtml(st);
					}
				}
			if (co == L'P') {
				if (n > tulprm->tulmuot.sivpit-jkoko-5) {
					endpage(tulprm);
					n = 0;
					}
				else {
					newline(tulprm, 1);
					n += jkoko+2;
					}
				}
			else if (!naytolle)
				tulprm->writehtml(L"\n");
			}
		if (co == L'P') {
			if (n) endpage(tulprm);
			enddoc(tulprm);
			}
		else if (!naytolle)
			tulprm->writehtml(L"</pre>\n");
		LeaveCriticalSection(&print_CriticalSection);
		for (j = 0; j <= kilpparam.valuku + 1 + (k_pv != 0); j++) {
			if (yhdseur[j]) {
				free(yhdseur[j]);
				yhdseur[j] = 0;
				}
			}
		free(joukk);
   }

#endif

INT avaa_tiedosto_nm(wchar_t *fname, tulostusparamtp *tulprm)
   {
   tulprm->lstf = openprfile(fname, -1, 1, 0, (char *)&tulprm->merkit, 0);
   if (!tulprm->lstf) {
		return(1);
		}
   memset(tulprm->sline, 0, sizeof(tulprm->sline));
   wmemset(tulprm->sline, L' ', sizeof(tulprm->sline)/2-1);
   return(0);
   }

void sulje_tiedosto(TextFl **tul_tied)
   {
   if (*tul_tied)
		delete *tul_tied;
   *tul_tied = NULL;
   }

void kirjLisaRivit(tulostusparamtp *tulprm, wchar_t *LisaRivit)
{
	TextFl *LisaFile;

	LisaFile = new TextFl(LisaRivit, L"r");
	if (LisaFile->IsOpen()) {
		while (!LisaFile->Feof()) {
			wchar_t line[1000] = L"";

			LisaFile->ReadLine(line, sizeof(line)/2-1);
			tulprm->writeline(line);
			}
		}
	delete LisaFile;
}

INT htmlrunko(tulostusparamtp *tulprm, wchar_t *baseFName)
   {
   wchar_t ch, fname[300], *p;
   INT isrj;
   INT emitvali;
   TextFl *htmlotsfl = NULL;

   emitvali = tulprm->tulostettava == L'E';
#ifdef _CONSOLE
	  {
	  ch = tulprm->yksihtml ? L'Y' : L'S';
	  wselectopt(L"Y)hten‰ tiedostona vai S)arjat erikseen",
		 L"YS", &ch);
	  tulprm->yksihtml = ch == L'Y';
	  }
#else
	tulprm->yksihtml = (tulprm->optiot & 1) == 0;
#endif
   if (!wtitle[0]) {
	  if (paaots[0])
		 otsTeksti(wtitle, paaots, 76, k_pv);
	  else
		 otsTeksti(wtitle, sarjaots, 76, k_pv);
	  }
   wtitle[76] = 0;
   elimwbl(wtitle);
#ifdef _CONSOLE
   viwrrectw(ySize-1,0,ySize-1,12,L"SIVUN NIMI   ", 7, 0, 0);
   inputwstr(wtitle,76,0,ySize-3,L"\r\x1b", &ch, 0);
#endif
   if (tulprm->yksihtml || !htmlotsfl) {
	  if (!wheader[0]) {
		 if (paaots[0])
			otsTeksti(wheader, paaots, 76, k_pv);
		 else
			otsTeksti(wheader, sarjaots, 76, k_pv);
		 }
	  wheader[76] = 0;
	  elimwbl(wheader);
#ifdef _CONSOLE
	  viwrrectw(ySize-1,0,ySize-1,13,L"SIVUN OTSIKKO ", 7, 0, 0);
	  clrln(ySize-3);
	  inputwstr(wheader,76,0,ySize-3,L"\r\x1b", &ch, 0);
#endif
	  }
   htmlalku(wtitle, wheader, 0, tulprm);
   if (!tulprm->yksihtml) {
	  tulprm->writehtml(L"<frameset cols=\"12%,*\" frameborder=\"yes\" border=\"1\">\n");
	  tulprm->writehtml(L"<frame src=\"sarjalue.html\" name=\"sivu\""
		 L" border=\"1\" scrolling=\"auto\">\n");
	  tulprm->writehtml(L"<frameset rows=\"10%,*\" frameborder=\"yes\" border=\"1\">\n");
	  tulprm->writehtml(L"<frame src=\"tulosots.html\" name=\"yla\" "
		 L"border=\"1\" noresize scrolling=\"no\">\n");
	  tulprm->writehtml(L"<frame src=\"tulalku.html\" name=\"keski\""
		 L" border=\"1\" noresize scrolling=\"auto\">\n");
	  tulprm->writehtml(L"</frameset></frameset>\n");
	  tulprm->writehtml(L"</html>\n");
	  closeprfile(tulprm->lstf);
	  if (!htmlotsfl) {
		  wcscpy(fname, baseFName);
		  for (p = fname + wcslen(fname) - 1; p > fname && *p != L'\\'; p--) ;
		  if (p > fname)
			  p[1] = 0;
		  else
			  fname[0] = 0;
		  wcscat(fname, L"tulosots.html");
		 if (avaa_tiedosto_nm(fname, tulprm)) return(1);
		 if (tulprm->merkit == L'A')
			tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
		 else
			 tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
		 css_string(tulprm, 3);
		 tulprm->writehtml(L"</head>\n<body class='runko'>\n<center><H3>");
		 tulprm->writehtml(wheader);
		 tulprm->writehtml(L"</H3></center></body></html>\n");
		 closeprfile(tulprm->lstf);

		  wcscpy(fname, baseFName);
		  for (p = fname + wcslen(fname) - 1; p > fname && *p != L'\\'; p--) ;
		  if (p > fname)
			  p[1] = 0;
		  else
			  fname[0] = 0;
		  wcscat(fname, L"tulalku.html");
		 if (avaa_tiedosto_nm(fname, tulprm)) return(1);
		 if (tulprm->merkit == L'A')
			tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
		 else
			tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
		 css_string(tulprm, 3);
		 tulprm->writehtml(L"</head>\n<body>\n<H2><center>");
		 tulprm->writehtml(wheader);
		 tulprm->writehtml(L"\n</center></H2></body></html>\n");
		 closeprfile(tulprm->lstf);
		 }
	  wcscpy(fname, baseFName);
	  for (p = fname + wcslen(fname) - 1; p > fname && *p != L'\\'; p--) ;
	  if (p > fname)
		  p[1] = 0;
	  else
		  fname[0] = 0;
	  wcscat(fname, L"sarjalue.html");
	  if (avaa_tiedosto_nm(fname, tulprm)) return(1);
	  if (tulprm->merkit == L'A')
		  tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
	  else
		  tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
	  css_string(tulprm, 3);
	  tulprm->writehtml(L"</head><body class='runko'>\n");
	  for (isrj = 0; isrj < sarjaluku+nsarjayhd; isrj++) {
		 if (nilm[isrj] && (!tulprm->sarjalista || tulprm->sarjalista[isrj])) {
			tulprm->writehtml(L"<A href=\"");
				tulprm->writehtml(Sarjat[isrj].sarjanimi);
				tulprm->writehtml(L".html\" target=\"keski\">");
			tulprm->writehtml(Sarjat[isrj].sarjanimi);
			tulprm->writehtml(L"</A><br>\n");
			}
		 }
	  }
   return(0);
   }

#ifdef MAKI

static INT prthyppy(kilptietue *kilp, INT sj)
   {
   wchar_t st[150];
   wchar_t sjs[6];
   FldFrmtTp *fld;

   wmemset(sjs,0,6);

   initline();
   memset(st,0,sizeof(st));

   fld = prtflds;

   if (sj) {
      _itow(sj, sjs, 10);
      putfld(tulprm, sjs, fld[0].pos, fld[0].len, fld[0].oik, 0);
	  }
   _itow(kilp->id(), st, 10);
   putfld(tulprm, st, fld[1].pos, fld[1].len, fld[1].oik, 0);
   putfld(tulprm, kilp->nimi(st, 60, tulprm->tulmuot.vaihdanimet), fld[2].pos, fld[2].len, fld[2].oik, 0);
   st[0] = 0;
   wcscpy(st, kilp->seuratieto(0));
   putfld(tulprm, st, fld[4].pos, fld[4].len, fld[4].oik, 0);
   return(1);
   }

void tulostahyppyjarj(void)
	{
	int d, p = 0, sj = 1, r = 3;
	kilptietue kilp;
	wchar_t line[80];

	 EnterCriticalSection(&print_CriticalSection);
   initpage(tulprm, NULL);
	swprintf(line, L"Hyppyj‰rjestys  -  %skierros", 
		aktkierros ? (aktkierros == 1 ? L"1. " : L"2. ") : L"Koe");
   putfld(tulprm, line, prtflds[1].pos, 80, 0, 0);
   endline(tulprm, -2);
	while (makivuoro[p]) {
		d = getpos(makivuoro[p]);
		if (d <= 0)
			break;
		kilp.GETREC(d);
		prthyppy(&kilp, sj++);
      endline(tulprm, 0);
	  if (r++ >= tulprm->tulmuot.sivpit) {
		 r = 0;
         endpage();
		 initpage(tulprm, NULL);
         }
		p++;
      }
	if (r)
		endpage();
	 enddoc();
	 LeaveCriticalSection(&print_CriticalSection);
	}

static int makivalinnat(wchar_t co, wchar_t *rajaus)
	{
	wchar_t ch;
	int err = 0;

	if (co == L'P') {
		if (*rajaus == L'I') {
			ch = L' ';
			wselectopt(L"T)ulokset vai H)yppyj‰rjestys", L"TH", &ch);
			if (ch == L'H')
				*rajaus = L'J';
			}
		}
	if (wcswcind(*rajaus, L"ILTP") >= 0) {
		if (aktkierros != nkierros[0]) {
			writeerror_w(L"T‰m‰ valinta toimii vain, kun viimeinen kierros on aktiivinen", 0);
			err = 1;
			goto loppu;
			}			
		else {
			ch = prttuomaripisteet ? L'K' : L'E';
			prttuomaripisteet = (ch == L'K');
			}
		}
	else
		prttuomaripisteet = 0;
	if ((co == L'X' || co == L'H' || co == L'M') && aktkierros < 2) {
		ch = L'K';
		wselectopt(L"Tulostus ei koske kahden kierroksen yhteistuloksia. Jatketaanko (K/E)", L"KE", &ch);
		if (ch == L'E')
			err = 1;
			goto loppu;
			}
loppu:
	return(err);
	}
#endif

static void teesarjapisteindeksi(int srj)
	{
   int  i, ipv, d, pst[N_PV], tot;
   int n;
   int min_nosot = 2;
   int max_npst = 3;
//   char *keyn;
   char *ip;
   kilptietue kilp;
   char key[10];

   if (pstindex.ix != NULL) {
		free(pstindex.ix);
		pstindex.ix = NULL;
	   }
   if (k_pv < 3)
	   min_nosot = 1;
   if (max_npst > k_pv+1)
		max_npst = k_pv+1;
   n = 0;
   if ((pstindex.ix = (char *) calloc(nilmt+1,pstindex.recl)) == NULL) {
	  writeerror_w(L"Muistissa ei tilaa indeksille", 4000);
	  return;
	  }
   ip = pstindex.ix;
   for (d = 1; d < nrec; d++)
	   {
	   kilp.GETREC(d);
	   if(kilp.kilpstatus == 0) {
		  i = 0;
		  tot = 0;
		  memset(pst, 0, sizeof(pst));
		  memset(key, 0, sizeof(key));
		  for (ipv = 0; ipv <= k_pv; ipv++) {
			  if (kilp.Sarja(ipv) == srj && kilp.lasna(ipv)) {
				  i++;
				  if (kilp.pv[ipv].pvpisteet[0])
						pst[ipv] = kilp.pv[ipv].pvpisteet[0] * (1000 + ((pstindex.flags & 1) ? (-ipv-1) : (ipv+1)));
				  }
			  }
		  if (i >= 1) {
			 qsortint(pst, k_pv+1, 4, 0, 1);
			 tot = 0;
			 for (ipv = k_pv-max_npst+1; ipv <= k_pv; ipv++) {
				 tot += pst[ipv];
				 }
			 if (i < min_nosot)
				tot = tot % 1000;
			  }
		  kilp.pisteet[0] = tot/1000;
		  kilp.tallenna(d, 0, 0, 0, 0);

		  if (tot > 0) {
				_swab((char *)&srj, key, 2);
				if (pstindex.flags & 1)
					*(int *)(key+2) = tot;
				else
					*(int *)(key+2) = 1000000000 - tot;
				lswab((long *)(key+2));
				}
		  else
			  memcpy(key, "\xff\xff", 2);
		  memcpy(ip, key, pstindex.keyl);
		  *(int *)(ip + pstindex.keyl) = d;

		  n++;
		  ip += pstindex.recl;
		  }
	   }
	quicksort((char *) pstindex.ix, n, pstindex.keyl, pstindex.recl);
	pstindex.nrec = n;
	}

#ifndef _CONSOLE

int list(wchar_t kohde, wchar_t tiedlaji, wchar_t tulostettava, wchar_t jarjestys,
	wchar_t pisteval, wchar_t rajaus, int alaraja, int ylaraja, INT16 *sarjat,
	wchar_t *kohdenimi, wchar_t sralaji, wchar_t merkit, wchar_t erotin,
	wchar_t *PisteLista, wchar_t (*seuralista)[LSEURA+5], wchar_t *SukupRaj,
	INT16 *IkaSrj, wchar_t *LisaRivit, UINT32 options, UINT32 options2)
	{
	tulostusparamtp tulprm;
	INT  sv, enssrj, ixjarj = 0;
	int TulosJaEmit = 0;
	int l = 0;
	INT piiri = 0;
	int pitkat = 1;
	bool pageclosed = false;

	tulprm.merkit = merkit;
	tulprm.erotin = erotin;
	TiedRivi = 0;

	wcscpy(listfname, kohdenimi);
	if (kohde == L'P' && tulprm.lstf == NULL) {
		tulprm.printer = GDIPRINTER;
		merkisto(L'G');
		if ((tulprm.lstf = openprfile(listfname, 1, TRUE, FALSE, trlate, FALSE))
			== NULL) {
			writeerror_w(L"Kirjoitinyhteyden avaaminen ei onnistunut",0, true);
			return(0);
			}
		}
	if (tulprm.lstf && tulprm.lstf->prno == 2) {
		initgdiprinter(&tulprm);
		}

	if (kohde == L'P') {
		tulprm.tulmuot = tulosmuot;
		tulprm.paa_alkuteksti = lsttulparam.paa_alkuteksti;
		tulprm.sivu_alkuteksti = lsttulparam.sivu_alkuteksti;
		tulprm.sivu_alateksti = lsttulparam.sivu_alateksti;
		tulprm.lopputeksti = lsttulparam.lopputeksti;
		tulprm.tulmuot.amarg = lsttulparam.tulmuot.amarg;
		tulprm.tulmuot.ymarg = lsttulparam.tulmuot.ymarg;
		tulprm.tulmuot.marg = lsttulparam.tulmuot.marg;
		tulprm.p_fld = prtflds;
		tulprm.n_fld = n_prtflds;
		}
	else if (kohde == L'I' && tiedlaji == L'M') {
		tulprm.tulmuot = mobiltulosmuot;
		tulprm.p_fld = mobilflds;
		tulprm.n_fld = n_mobilflds;
		}
	else {
		tulprm.tulmuot = filetulosmuot;
		tulprm.p_fld = fileflds;
		tulprm.n_fld = n_fileflds;
		}

	tulprm.xmlstd = 0;
	if (kohde == L'I') {
		if (tiedlaji == L'X') {      // IOF
			tulprm.xmlstd = L'I';
			}
		if (tiedlaji == L'F') {
			tulprm.xmlstd = L'F';
			tiedlaji = L'X';
			tulprm.xmlstd = L'F';
			}
		if (tiedlaji == L'V') {     // Oma XML
			tiedlaji = L'X';
			tulprm.xmlstd = L'O';
			}
		if (tiedlaji == L'Y') {     // WRE = IOF ilman seuraa
			tiedlaji = L'X';
			tulprm.xmlstd = L'J';
			}
		if (tiedlaji == L'W') {     // Laajennettu oma XML
			tiedlaji = L'X';
			tulprm.xmlstd = L'L';
			}
		if (tiedlaji == L'R' && pisteval == L'Z') {
			tiedlaji = L'Z';
			tulprm.kaikkiva = true;
			}
		if (tiedlaji == L'X' && pisteval == L'E') {
			tulprm.kaikkiva = true;
			}
		if (tiedlaji == L'H' && pisteval == L'Z') {
//			tulprm.tulmuot.tauluhtml = 0;
			}
		kohde = tiedlaji;
		}

	tulprm.rajaus = rajaus;
	tulprm.kohde = kohde;
	tulprm.optiot = options;
	tulprm.optiot2 = options2;
	tulprm.language = (options & 0x10000000) ? 1 : 0;

	// T‰st‰ alkaa koko tulosteen tekev‰ luuppi, joka toistetaan, jos tulostettava == L'F'

	if (tulprm.kohde == L'X') {
		wcscpy(tulprm.muottied, lsttulparam.muottied);
		if (tulostettava == L'E')
			tulprm.xmlva = L'E';
		else
			tulprm.xmlva = L'M';
		}

	if (tulostettava == L'F') {
		if (tulprm.kohde != L'X') {
			TulosJaEmit = 1;
            tulprm.TlsJaRva = pisteval != L'N' ? 1 : 2;
			}
		tulostettava = L'T';
		}
	tulprm.tulostettava = tulostettava;

	do {
		tulprm.sarjalista = sarjat;

		if (tulprm.rajaus == L'S') {
			tulprm.seuratulokset = true;
			tulprm.rajaus = L'L';
			tulprm.sj_raja = (options / 8192) & 32;
			tulprm.karjet = tulprm.sj_raja != 0;
			}

		if (tulprm.rajaus == L'D') {
			tulprm.piiritulokset = true;
			tulprm.rajaus = L'L';
			tulprm.piirisijat = (options & 4096) ? 1 : 0;
			tulprm.sj_raja = (options / 8192) % 32;
			tulprm.karjet = tulprm.sj_raja != 0;
			}

		if (tulprm.rajaus == L'J' && tulprm.tulostettava != L'P')
			tulprm.tulostettava = L'J';

		tulprm.sukuprajaus = SukupRaj;
		tulprm.ikasarjat = IkaSrj;

		if (sralaji == L'S' || sralaji == L'T')
			pitkat = 1;
		else if (sralaji == L'L' || sralaji == L'M')
			pitkat = 0;
		else {
			if (tulprm.kohde != L'P') {
				pitkat = (fileflds[F_SRA].len != 0);
				}
			}

		if (tulprm.rajaus == L'-') {
			tulprm.rajaus = L'T';
			tulprm.tarkastamattomat = true;
			}

		if (tulprm.rajaus == L'P') {
			tulprm.enssija = alaraja;
			tulprm.viimsija = ylaraja;
			}

		switch (tulprm.tulostettava) {
			case L'V':
				tulprm.piste = pisteval - L'0';
				if (tulprm.piste == 0) {
					tulprm.kaikkiva = true;
					tulprm.tulostettava = L'T';
					tulprm.kaikkisijat = ((options & 8) != 0);
					}
				else
					tulprm.rajaus = L'T';
				break;
			case L'Y':
				tulprm.piste = jarjestys - L'0';
				tulprm.pistelista = PisteLista;
				tulprm.kaikkisijat = ((options & 8) != 0);
				if (wcslen(PisteLista) == 0)
					return(0);
				break;
			case L'P':
				if (tulprm.rajaus != L'J') {
					tulprm.yhttuljarj = pisteval != L'T';
					tulprm.yhttulpaino = pisteval != L'T';
					tulprm.vaihelkm = pisteval == L'Z' ? 3 : 1;
					tulprm.piste = pisteval == L'R' ? 2 : 0;
					if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && tulprm.vaihelkm == 3) {
						if (tulprm.kohde == L'P') {
//							tulprm.vaihelkm = 2;
							tulprm.p_fld = tulprm.vaihelkm == 1 ? prtflds : prtflds1;
							tulprm.n_fld = tulprm.vaihelkm == 1 ? n_prtflds : n_prtflds1;
							}
						}
					else if (tulprm.kohde == L'P') {
						tulprm.p_fld = tulprm.vaihelkm == 1 ? prtflds : prtflds1;
						tulprm.n_fld = tulprm.vaihelkm == 1 ? n_prtflds : n_prtflds1;
						}
					}
				break;
			case L'E':
				if (tulprm.kohde != L'X') {
					tulprm.piste = wcswcind(pisteval, L"KRMNS");
					if (tulprm.piste < 0)
						tulprm.piste = 2;
					}
				break;
			case L'O':
				tulprm.piilotatulos = true;
				tulprm.piilotasijat = true;
				// jatkuu tarkoituksella
			default:
				tulprm.yhttuljarj = false;
				tulprm.yhttulpaino = false;
				if (tulprm.kohde == L'P') {
					tulprm.p_fld = prtflds;
					tulprm.n_fld = n_prtflds;
					}
				tulprm.vaihelkm = 1;
				if (k_pv) {
					switch (pisteval) {
						case L'T':
							break;
						case L'Y':
							tulprm.yhttuljarj = true;
							tulprm.yhttulpaino = true;
							break;
						case L'Z':
							tulprm.yhttuljarj = true;
							tulprm.yhttulpaino = true;
/*
							if (tulprm.kohde == L'P') {
								tulprm.p_fld = prtflds1;
								tulprm.n_fld = n_prtflds1;
								}
*/
							tulprm.vaihelkm = 2;
							break;
						case L'K':
							tulprm.yhttuljarj = true;
							tulprm.yhttulpaino = true;
//							if (wcswcind(kilpparam.kilplaji, L"NP") < 0)
								{
								if (tulprm.kohde == L'P') {
									tulprm.p_fld = prtflds1;
									tulprm.n_fld = n_prtflds1;
									}
								tulprm.vaihelkm = 3;
								break;   // tarkoituksella sulkujen sis‰ll‰.
								}
						case L'U':
							if (tulprm.kohde == L'P') {
								tulprm.p_fld = prtfldsm;
								tulprm.n_fld = n_prtfldsm;
								}
							tulprm.vaihelkm = 2;
							break;
						}
					}
				break;
			}

		if (wcswcind(tulprm.rajaus, L"AHKE-G") >= 0)
			tulprm.yhttuljarj = false;

		switch (jarjestys) {
			case L'A':
				tulprm.ixjarj = 1;
				break;
			case L'N':
				tulprm.ixjarj = 11;
				break;
			case L'B':
				tulprm.ixjarj = 2;
				break;
			case L'M':
				tulprm.ixjarj = 12;
				break;
			case L'L':
			case L'P':
			case L'O':
				tulprm.ixjarj = (jarjestys == L'L') ? 4 : 5;
				pstindex.flags = (jarjestys == L'L') ? 0 : 1;
				if (!tulprm.yhttuljarj)
					pstindex.flags |= 8;
				if (tulprm.piste)
					pstindex.flags |= (2*tulprm.piste);
				if (jarjestys == L'P')
					pstindex.flags |= 16;
				arv_pv = k_pv;
				if ((options & 4) == 0)
					teeindeksi(&pstindex, true);
				break;
			}

		if (kilpparam.kilplaji == L'N')
			meri3pisteet();

		if (emitfl && tulprm.tulostettava == L'E' && pisteval == L'S') {
			kirjvaliajat(listfname);
			goto loppu2;
			}

		if ((tulprm.kohde == L'H' || tulprm.kohde == L'M') && tulprm.tulostettava != L'E') {
			tulprm.yksihtml = !(options & 1);
			htmlpitkat = pitkat ? 1 : 0;
			}

		if (wcswcind(kohde, L"IRJEHMFOSWXZ") >= 0 && tulprm.lstf == NULL) {
			if (avaa_tiedosto_nm(kohdenimi, &tulprm))
				goto loppu2;
			}

#ifdef JOUKKUEPISTEET
		if (tulprm.rajaus == L'J' && wcswcind(tulprm.kohde, L"MPH") >= 0 && tulprm.tulostettava == L'P') {
			joukkuepisteet(false, &tulprm);
			goto loppu;
			}
#endif

		if (k_pv == 0 && kilpparam.n_pv_akt >= 2 && kilpparam.alalaji == L'S') {
			if (pisteval == L'Y') {
				tulprm.yhttuljarj = true;
				tulprm.yhttulpaino = true;
				if (tulprm.tulostettava != L'P')
					tulprm.piilotatulos = true;
				}
			if (pisteval == L'K') {
				tulprm.yhttuljarj = true;
				tulprm.yhttulpaino = true;
				tulprm.piilotatulos = false;
				tulprm.vaihelkm = 3;
				}
			}
		if (tulprm.kohde == L'X') {
			if (tulprm.xmlstd == L'F') {
				if (tulprm.rajaus == L'I')
					tulprm.rajaus = L'L';
//				if (kilpparam.n_pv_akt >= 2 && kilpparam.alalaji == L'S' && pisteval == L'Y') {
//					tulprm.yhttuljarj = true;
//					tulprm.yhttulpaino = true;
//					}
				xmlfisots(&tulprm);
				}
			else
				xmlots(&tulprm);
			}

		if (tulprm.kohde == L'P') {
			switch (tulprm.vaihelkm) {
				case 2:
					tulprm.p_fld = prtfldsm;
					tulprm.n_fld = n_prtfldsm;
					break;
				case 3:
					tulprm.p_fld = prtflds1;
					tulprm.n_fld = n_prtflds1;
					break;
				default:
					tulprm.p_fld = prtflds;
					tulprm.n_fld = n_prtflds;
				}
			}

		ssltlsfl = 0;

		if (tulprm.kohde == L'W') {
			ssltlsfl = 2;
			tulprm.kohde = L'S';
			}

		if (tulprm.kohde == L'O') {
			ssltlsfl = 1;
			tulprm.kohde = L'S';
			ssl_otsikot(k_pv, &tulprm);
			}

		l = 0;
		sv = 0;
		enssrj = TRUE;

		if (tulprm.seuratulokset) {
			memset(seuranimi, 0, sizeof(seuranimi));
			for (int is = 0; is < SEURALISTA && seuralista[is][0]; is++)
				wmemcpy(seuranimi[is], seuralista[is]+((tulprm.optiot & 0x08000000) ? 3 : 0), LSEURA);
			}

		piiri = -1;
		do {

	/*		if (tulprm.piiritulokset) {
				piiri++;
				if (piiri >= piiriluku) {
					break;
					}
				if (!piirifl[piiri])
					continue;
				}
	*/
			if (tulprm.seuratulokset || tulprm.piiritulokset) {
				if (tulprm.kohde == L'P') {
					if (l != 0) {
						if (tulprm.sivu_alateksti)
							kirjSivuAla(&tulprm, &l);
						endpage(&tulprm);
						}
					initpage(&tulprm, NULL);
					}
				l = 0;
				sv = 0;
				}

			for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
				if (!sarjat[srj])
					continue;
				if (pageclosed)
					initpage(&tulprm, NULL);
				pageclosed = false;
				if (enssrj) {
					if (tulprm.kohde == L'P') {
						initpage(&tulprm, NULL);
						if (tulprm.paa_alkuteksti) {
							kirjPaaAlku(&tulprm, &l);
							potsfl = TRUE;
							}
						else if (paaots[0] && wcscmpU(paaots, sarjaots)) {
							if (tulprm.tulmuot.esirivit) {
								newline(&tulprm, tulprm.tulmuot.esirivit);
								l = tulprm.tulmuot.esirivit;
								}
							paaots_on(&tulprm);
							putfld(&tulprm, otsTeksti(NULL, paaots, 80, k_pv),0,80,0,0);
							endline(&tulprm, -1);
							paaots_pois(&tulprm);
							potsfl = TRUE;
							l += 2;
							}
						}
					else if (tulprm.kohde == L'E') {
						if (paaots[0]) {
							tulprm.puts_f(otsTeksti(NULL, paaots, 80, k_pv));
							tulprm.puts_f(L"\n\n");
							}
						}
					else if ((tulprm.kohde == L'H' || tulprm.kohde == L'M') &&
						(!TulosJaEmit || tulprm.tulostettava != L'E')) {
						potsfl = TRUE;
						if (htmlrunko(&tulprm, kohdenimi))
							break;
						if (LisaRivit) {
							kirjLisaRivit(&tulprm, LisaRivit);
							}
						}
					enssrj = FALSE;
					}
				if (nilm[srj]) {
					if (tulprm.kohde == L'V')                // ??
						l = 0;
					if ((tulprm.kohde == L'H' || tulprm.kohde == L'M') && !tulprm.yksihtml) {
						htmlloppu(&tulprm);
						closeprfile(tulprm.lstf);
						if (avaa_tiedosto_nm(htmlSrjFName(kohdenimi, srj), &tulprm))
							continue;
						htmlalku(0, 0, 1, &tulprm);
						}
					if (tulprm.tulostettava == L'E' && (wcswcind(tulprm.kohde, L"IHM") >= 0)) {
						if (!Sarjat[srj].piilotatulos)
							emitvaliajat(&srj, &tulprm);
						}
#if defined(JOUKKUETUL)
					else if (tulprm.rajaus == L'J' && tulprm.tulostettava == L'J') {
						joukkuetul(false, srj, &tulprm);
						}
#endif
					else
						{
						if ((jarjestys == L'L' || jarjestys == L'P') && (options & 4) != 0) {
							teesarjapisteindeksi(srj);
							}
						if (tulprm.ixjarj == 0 && Sarjat[srj].pisteytulos() && Sarjat[srj].YhtpistOn()) {
							TLpKarki = teeYPisteJarj(srj, &tulprm.ypJrj);
							if (tulprm.yhttuljarj)
								tulprm.ixjarj = 9;
							}

						if (k_pv == 0 && kilpparam.n_pv_akt >= 2 && kilpparam.alalaji == L'S' && pisteval == L'Y') {
							sijat2(srj, true, false);
							}

						tulostasarja(&srj, &tulprm, &l, &sv, FALSE);
						if (tulprm.ypJrj) {
							free(tulprm.ypJrj);
							tulprm.ypJrj = 0;
							}
						if (tulprm.ixjarj == 9)
							tulprm.ixjarj = 0;
						}
					}
				if (kohde == L'P' && (options & 0x80000000)) {
					if (l != 0) {
						if (tulprm.sivu_alateksti)
							kirjSivuAla(&tulprm, &l);
						endpage(&tulprm);
						pageclosed = true;
						}
					l = 0;
					}
				}
			} while (0);

		// Toinen tulostuskierros, jos tulostetaan sek‰ tulokset ett‰ Emit-v‰liajat
		if (TulosJaEmit && tulprm.tulostettava != L'E') {
			tulprm.tulostettava = L'E';
			}
		else
			break;
		} while(1);
	if (kohde == L'P') {
		if (l != 0) {
			if (tulprm.sivu_alateksti)
				kirjSivuAla(&tulprm, &l);
			if (tulprm.lopputeksti)
				kirjLoppu(&tulprm, &l);
			endpage(&tulprm);
			}
		else {
			if (tulprm.lopputeksti) {
				kirjLoppu(&tulprm, &l);
				endpage(&tulprm);
				}
			}
		}
	loppu:
	if (kohde == L'H' || kohde == L'M')
		htmlloppu(&tulprm);
	if (kohde == L'F')
		tulprm.writeline(L"E");
	if (kohde == L'X') {
		if (tulprm.xmlstd == L'F')
			xmlfisloppu(&tulprm);
		else
			xmlloppu(&tulprm);
		TiedRivi = 0;
		}
	if (wcswcind(kohde, L"IRJEHMSFOWXZ") >= 0) {
		closeprfile(tulprm.lstf);
		}
	loppu2:
	if (kohde == L'P') {
		enddocGDI(tulprm.lstf);
		closeprfile(tulprm.lstf);
		tulprm.lstf = NULL;
		}
	if (pstindex.ix)
		poistaindeksi(&pstindex);
	return(TiedRivi);
	}


#endif // !_CONSOLE
