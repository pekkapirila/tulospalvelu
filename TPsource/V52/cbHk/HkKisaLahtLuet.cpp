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
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "HkDeclare.h"
#include "HkMuotoilu.h"

#define BUFLGT  100000

void initfont(tulostusparamtp *tulprm, wchar_t *fnt);
void paaots_on(tulostusparamtp *tulprm);
void paaots_pois(tulostusparamtp *tulprm);
void aliots_on(tulostusparamtp *tulprm);
void aliots_pois(tulostusparamtp *tulprm);
void jarj(INT *ia, INT a, INT y);
wchar_t *css_string(tulostusparamtp *tulprm, int laji);

static int  jikorotus = 100;
static TextFl *lstfile;
static int tiedostoon;
static wchar_t fldstr[NLLFIELDS+NFLISA+1][80], tul_line[270];
static  wchar_t tabst[2] = L"\x9";
static	int vmarg[2];

static void initline(void)
   {
   if (tiedostoon && !llparam.tabs[tiedostoon]) {
	  wmemset(tul_line, L' ', sizeof(tul_line)/2);
		tul_line[sizeof(tul_line)/2-1] = 0;
	  }
   }

static void sendlnfl(wchar_t *line)
   {
	lstfile->WriteLine(line);
   }

static void sendln(PRFILE *lstf, wchar_t *line)
   {

   if (*line == 0) return;
//   if (tiedostoon)
//		sendlnfl(line);
//   else
		sendwline(line, lstf);
   }

static void putllfld(tulostusparamtp *tulprm, INT laji, INT knt)
{
	wchar_t *str;
	INT pos, len, oik;
	INT v, o, ll;

	if ((len = tulkentta[tiedostoon][knt].len[laji]) == 0)
		return;
	str = fldstr[knt];
	ll = wcslen(str);
	ll = min(ll, len);
	oik = tulkentta[tiedostoon][knt].tas[laji];

	if (tulprm->kohde == L'H' && tulprm->tulmuot.tauluhtml) {
		if (oik)
			wcscat(tulprm->sline, L"<td align=right>");
		else
			wcscat(tulprm->sline, L"<td align=left>");
		wcscat(tulprm->sline, str);
		wcscat(tulprm->sline, L"</td>");
		}
	else {
		pos = tulkentta[tiedostoon][knt].pos[laji] + llparam.marg[tiedostoon][laji];
	   if (llparam.tabs[tiedostoon] && pos) sendln(tulprm->lstf, tabst);
	   if (ll <= 0)
			return;
	   if (ll == len)
			str[ll] = 0;
	   if (llparam.tabs[tiedostoon]) {
			sendln(tulprm->lstf, str);
			return;
			}
		putfld(tulprm, str, pos, len, oik, 0);
	   }
   }

static void put_str(tulostusparamtp *tulprm, wchar_t* str, INT pos)
   {
   INT ll;

   ll = wcslen(str);
   if (ll <= 0) return;
   pos += vmarg[tiedostoon];
   if (llparam.tabs[tiedostoon] && pos) sendln(tulprm->lstf, tabst);
   if (llparam.tabs[tiedostoon]) {
	  sendln(tulprm->lstf, str);
	  return;
	  }
	putfld(tulprm, str, pos, ll, 0, 0);
   }

static void prt_kilp(tulostusparamtp *tulprm, INT laji)
   {
   INT i, kntjrj[NLLFIELDS+NFLISA], knt[NLLFIELDS+NFLISA];

   initline();
	if (tulprm->kohde == L'H' && tulprm->tulmuot.tauluhtml) {
		if ((tulprm->rivi+tulprm->tulmuot.riviryhma-1) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
			wcscpy(tulprm->sline, L"<tr class='ab'>");
		else
			wcscpy(tulprm->sline, L"<tr class='aa'>");
		}
   for (i = 0; i < (int) NLLFIELDS+NFLISA; i++) {
	  kntjrj[i] = tulkentta[tiedostoon][i].pos[laji];
	  }
   jarj(kntjrj, 1, NLLFIELDS+NFLISA);
   for (i = 0; i < (int) NLLFIELDS+NFLISA; i++) knt[kntjrj[i]-1] = i;
   for (i = 0; i < (int) NLLFIELDS+NFLISA; i++) putllfld(tulprm, laji, knt[i]);
	if (tulprm->kohde == L'H' && tulprm->tulmuot.tauluhtml)
		wcscat(tulprm->sline, L"</tr>");
   endline(tulprm, 0);
   }

static void lisakentat(int laji, int lisaa)
   {
   INT i,j;

   if (kilpparam.n_pv_akt < 2)
		return;
   for (j = NLLFIELDS; j < (int) NLLFIELDS+NFLISA; j++) {
	  memset(&tulkentta[laji][j], 0, sizeof(tulkentta[laji][j]));
	  for (i = 4; i < 7; i++) {
		 tulkentta[laji][j].pos[i] = tulkentta[laji][LLF_AIKA].pos[i] +
			lisaa * (j - NLLFIELDS + 1) * llparam.aikaporras[laji][i-4];
		 tulkentta[laji][j].len[i] = tulkentta[laji][LLF_AIKA].len[i];
		 tulkentta[laji][j].tas[i] = tulkentta[laji][LLF_AIKA].tas[i];
		 }
	  }
   for (j = 0; j < (int) NLLFIELDS; j++) {
	  for (i = 4; i < 7; i++) {
		 if (tulkentta[laji][j].pos[i] > tulkentta[laji][LLF_AIKA].pos[i])
			 tulkentta[laji][j].pos[i] +=
				lisaa * (kilpparam.n_pv_akt-2) * llparam.aikaporras[laji][i-4];
		 }
	  }
   }

static void sarakeleveydet(tulostusparamtp &tulprm, wchar_t *lots, int laji)
{
	wcscat(lots, L"<table>\n");
	if (tulprm.tulmuot.tauluhtml > 1) {
		INT kntjrj[NLLFIELDS+NFLISA], knt[NLLFIELDS+NFLISA];
		int ll = 0;
		wcscat(lots, L"<colgroup>\n");
		for (int i = 0; i < (int) NLLFIELDS+NFLISA; i++) {
			kntjrj[i] = tulkentta[tiedostoon][i].pos[laji];
			}
		jarj(kntjrj, 1, NLLFIELDS+NFLISA);
		for (int i = 0; i < (int) NLLFIELDS+NFLISA; i++) knt[kntjrj[i]-1] = i;
		for (int i = 0; i < (int) NLLFIELDS+NFLISA; i++) {
			if (tulkentta[tiedostoon][knt[i]].len[laji])
				ll += tulkentta[tiedostoon][knt[i]].len[laji] + 1;
			}
		for (int i = 0; i < (int) NLLFIELDS+NFLISA; i++) {
			int lp;
			if (tulkentta[tiedostoon][knt[i]].len[laji]) {
				if (tulprm.tulmuot.tauluhtml & 4) {
					lp = (100*tulkentta[tiedostoon][knt[i]].len[laji]+150)/ll;
					swprintf(lots+wcslen(lots), L"<col width=\"%d%%\">\n", lp);
					}
				else {
					lp = (1000*tulkentta[tiedostoon][knt[i]].len[laji]+30)/130;
					swprintf(lots+wcslen(lots), L"<col width=\"%dpx\">\n", lp);
					}
				}
			}
		wcscat(lots, L"</colgroup>\n");
		}
}

INT lueseura(wchar_t *seura, INT *d)
   {
   wchar_t tc = 0, sn[12];
   static wchar_t sm[4] = L"   ";

   // pyytää valitsemaan maakoodin ja seuran, jotka yhdistetään avaimeen, sitten haetaan kilpailija (d)
   // palautetaan 1, jos ok ja 0, jos peruutetaan
   return( tc != L'P');
   }

INT teejonot(int **ljonot, INT njono, wchar_t lahto, INT d, kilpindex *ind)
   {
   kilptietue kilp;
   INT  k, l, lmax;
   char   key[MAXINDL];
   INT ksrj[MAXSARJALUKU];

   memset(ksrj, 0, sizeof(ksrj));
   lmax = 2*nilmt + 10*(njono-1)*sarjaluku;
   if (lmax > 2000)
	  lmax /= 2;
   *ljonot = (int *) calloc(lmax, sizeof(INT));
   if (*ljonot == 0) {
	  writeerror_w(L"Ei muistitilaa jonojen muodostamiselle", 0);
	  return(0);
	  }
   l = -1;
   do {
	  kilp.GETREC(d);
	  if (l >= 0) {
		 k = l % njono;
		 l += (ksrj[kilp.Sarja()] - k + njono - 1) % njono + 1;
		 }
	  else
		 l = 0;
	  if (l < lmax)
		 (*ljonot)[l] = d;
	  else {
		 writeerror_w(L"Jonojen muodostus keskeytetty. Liian epätasaiset jonot", 0, true);
		 break;
		 }
	  ksrj[kilp.Sarja()] = (ksrj[kilp.Sarja()]+1) % njono;
	  if (nextix(ind,key,&d))
		  break;
	  } while (!lahto || lahto == key[0]);
   return((l + njono) / njono);
   }

static bool onkolasna(int srj, int ipv)
{
	INT d;
	kilptietue kilp;
	bool lasna = false;

	EnterCriticalSection(&tall_CriticalSection);
	for (d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus)
			continue;
		if (ipv >= 0 && kilp.Sarja(ipv) != srj)
			continue;
		if (ipv < 0 && kilp.sarja != srj)
			continue;
		if (ipv >= 0 && wcswcind(kilp.tark(ipv), L"PEV") < 0)
			lasna = true;
		if (ipv < 0) {
			for (int jpv = Sarjat[srj].enspv; jpv <= Sarjat[srj].viimpv; jpv++)
				if (Sarjat[srj].luonne[jpv] >= 0 && wcswcind(kilp.tark(jpv), L"PEV") < 0) {
					lasna = true;
					break;
					}
			}
		if (lasna)
			break;
		}
	LeaveCriticalSection(&tall_CriticalSection);
	return(lasna);
}

static void webScorerLahdot(tulostusparamtp *tulprm, short *sarjat, int i_pv)
{
	kilpindex ind;
	char   key[MAXINDL];
	int d, srj;
	kilptietue kilp;
	bool yhtlahto = false;
	wchar_t line[200], st[62];

	arv_pv = i_pv;
	for (srj = 0; srj < sarjaluku; srj++)
		if (sarjat[srj] && nilm_pv[i_pv][srj])
			break;
	if (srj >= sarjaluku)
		return;
	if (Sarjat[srj].lahtovali[i_pv] == 0)
		yhtlahto = true;
	initpage(tulprm, 0);
	if (yhtlahto) {
//		putfld(tulprm, L"Start type =\tMass", 0, 30, 0, 0);
//		endline(tulprm, 0);
		putfld(tulprm, L"First name\tLast name\tTeam name\tTeam name 2\tDistance\tCategory\tBib", 0, 200, 0, 0);
		endline(tulprm, 0);
		}
	else {
//		putfld(tulprm, L"Start type =\tInterval", 0, 30, 0, 0);
//		endline(tulprm, 0);
//		swprintf(line, L"Number of racers per interval =\t%d", Sarjat[srj].pari[i_pv]);
//		putfld(tulprm, line, 0, wcslen(line)+1, 0, 0);
//		endline(tulprm, 0);
//		swprintf(line, L"Start racers every (mm:ss) =\t%02d:%02d", Sarjat[srj].lahtovali[i_pv]/60, Sarjat[srj].lahtovali[i_pv]%60);
//		putfld(tulprm, line, 0, wcslen(line)+1, 0, 0);
//		endline(tulprm, 0);
		putfld(tulprm, L"First name\tLast name\tTeam name\tTeam name 2\tDistance\tCategory\tBib\tStart time", 0, 200, 0, 0);
		endline(tulprm, 0);
		}
	ind = *lahtoindex;
	ind.ix = 0;
	ind.flags = 1;
	lahtojarjfl = 1;
	if (!teeindeksi(&ind, true)) {
		memset(key, 0, MAXINDL);
		searchix(&ind, key, &d);
		while (d > 0) {
			kilp.GETREC(d);
			if (sarjat[kilp.Sarja(i_pv)] && kilp.tark(i_pv) != L'P') {
				swprintf(line, L"%s\t%s\t%s\t%s\t%s\t%s\t%d",
					kilp.etunimi, kilp.sukunimi, kilp.seura, kilp.seuralyh,
					Sarjat[kilp.Sarja(i_pv)].matka[i_pv],
					Sarjat[kilp.Sarja(i_pv)].sarjanimi,
					kilp.id(i_pv));
				if (!yhtlahto) {
					swprintf(line+wcslen(line), L"\t\%s",
						aikatowstr_cols_n(st, kilp.TLahto(i_pv, false, 0) , t0, L':', 8));
					}
				putfld(tulprm, line, 0, wcslen(line)+1, 0, 0);
				endline(tulprm, 0);
				}
			if (nextix(&ind, key, &d))
				break;
			}
		}
	lahtojarjfl = 0;
}

static void ASCLahdot(tulostusparamtp *tulprm, int luetlaji, short *sarjat, int i_pv)
{
	kilpindex ind;
	char   key[MAXINDL];
	int d, srj, enst = TMAALI0;
	kilptietue kilp;
	wchar_t line[200], st[62];

	arv_pv = i_pv;
	memset(key, 0, MAXINDL);
	if (luetlaji != L'L') {
		for (srj = 0; srj < sarjaluku; srj++)
			if (sarjat[srj] && nilm_pv[i_pv][srj])
				break;
		if (srj >= sarjaluku)
			return;
		}
	else {
		for (srj = 1; srj < 256; srj++)
			if (sarjat[srj])
				break;
		if (srj > 255)
			return;
		key[0] = srj;
		}
	initpage(tulprm, 0);
	if (tulprm->optiot & 16)
		putfld(tulprm, L"BIB;Delay", 0, 20, 0, 0);
	else
		putfld(tulprm, L"BIB;Time", 0, 20, 0, 0);
	endline(tulprm, 0);
	ind = *lahtoindex;
	ind.ix = 0;
	ind.flags = luetlaji == L'L' ? 0 : 1;
	lahtojarjfl = 1;
	if (!teeindeksi(&ind, true)) {
		searchix(&ind, key, &d);
		while (d > 0) {
			kilp.GETREC(d);
			if (kilp.tark(i_pv) != L'P' &&
				((luetlaji != L'L' && sarjat[kilp.Sarja(i_pv)]) ||
				(luetlaji == L'L' && sarjat[Sarjat[kilp.Sarja(i_pv)].lno[i_pv]]))) {
				if (enst == TMAALI0)
					enst = kilp.TLahto(i_pv, false, 0);
				if (tulprm->optiot & 16)
					swprintf(line, L"%d;%s", kilp.id(i_pv),
						aikatowstr_cols_n(st, kilp.TLahto(i_pv, false, 0) - enst, 0, L',', 8));
				else
					swprintf(line, L"%d;%s", kilp.id(i_pv),
						aikatowstr_cols_n(st, kilp.TLahto(i_pv, false, 0), t0, L',', 8));
	//				aikatowstr_cols_n(st, kilp.TLahto(i_pv, false, 0) - Sarjat[kilp.Sarja(i_pv)].enslahto[i_pv], 0, L',', 8));
				putfld(tulprm, line, 0, wcslen(line)+1, 0, 0);
				endline(tulprm, 0);
				}
			if (nextix(&ind, key, &d))
				break;
			}
		}
	lahtojarjfl = 0;
}

// kohde    		K: kirjoitin, I: Tiedosto
// tiedlaji			R: erottimin, I: kohdistettu, E: jatkuva, H: html, X: XML
// luetlaji   		A: sarja/aakkos, B: sarja/lähtö, C: sarja/numero, sE)uroittain, L)ähdöittäin, aaK)kosin, N)umero
// paiva            1, 2, .. 0 = kaikki
// njono  			takaa-ajon jonot
// emitrajaus       K: kaikki, L: lainakortilliset, O: kortittomat

void lahtoluettelo(wchar_t kohde, wchar_t tiedlaji, wchar_t luetlaji, int paiva, int jarjpv,
	int njono, wchar_t emitrajaus, wchar_t sranimi, wchar_t *listflnm, wchar_t merkit,
	wchar_t erotin, short *sarjat, wchar_t (*seuralista)[LSEURA+5], wchar_t *LisaRivit, UINT32 Options,
	int pistejakaja)
   {
   char   key[MAXINDL] ,keyv[MAXINDL];
   wchar_t lots[500];
   wchar_t   as1[40], st[120];
   INT32   keyv2;
   INT   d,l,lno = 0, nk,ni,ns,pp, laji, ens, ok;
   INT   naika,naikamax;
   INT   nkilp,l0,kesken = 1, nilmsrj, ansifl = 0, llhtml = 0;
   INT32   mks, ms, cent = 1, edlahto = TMAALI0, ensl = 0, vakiovali = 60 * SEK;
   INT   ilmvert,lahtono = 0;
   INT16 isrj = 0, srj = 0, ksrj;
   INT   lotsf, ots_pv;
   kilpindex ind;
   INT nj, kjono = 0, pv, kj, ljpit, n_pv, npv, iSeura = 0;
   INT *ljonot = 0;
   INT nlaina, nlainatot, nomatot, neiemit;
   kilptietue kilp;
   tulostusparamtp tulprm;
   bool alkusivu = true, uusisivu = false;
   int LLvaat;
   bool seuralistaMaa = false;

   l = 0;
   npv = kilpparam.n_pv_akt;
   ind.ix = 0;
   tulprm.optiot = Options;
   LLvaat = (Options & 0xe0) / 32;
   seuralistaMaa = Options & 0x08000000;

	if (kohde == L'P') {
		tiedostoon = 0;
		tulprm.printer = GDIPRINTER;
		merkisto(L'G');
		tulprm.tulmuot.amarg = lsttulparam.tulmuot.amarg;
		tulprm.tulmuot.ymarg = lsttulparam.tulmuot.ymarg;
		tulprm.tulmuot.marg = lsttulparam.tulmuot.marg;
		if ((tulprm.lstf = openprfile(listflnm, 1, TRUE, FALSE, trlate, FALSE))
			== NULL) {
			return;
			}
		initgdiprinter(&tulprm);
		EnterCriticalSection(&print_CriticalSection);
		}
	else {
		tulprm.tulmuot = filetulosmuot;
		tiedostoon = 1;
		tulprm.printer = 1;
		tulprm.kohde = tiedlaji;
		tabst[0] = erotin;
		llparam.tabs[1] = tiedlaji == L'R';
		llhtml = tiedlaji == L'H';
		ansifl = merkit == L'A';
		if ((tulprm.lstf = openprfile(listflnm, -1, TRUE, FALSE, (char *)&merkit, FALSE))
			== NULL) {
			return;
			}
		if (tiedlaji == L'B') {
			webScorerLahdot(&tulprm, sarjat, jarjpv < 0 ? 0 : jarjpv);
			closeprfile(tulprm.lstf);
			tulprm.lstf = 0;
			return;
			}
		if (tiedlaji == L'A') {
			ASCLahdot(&tulprm, luetlaji, sarjat, jarjpv < 0 ? 0 : jarjpv);
			closeprfile(tulprm.lstf);
			tulprm.lstf = 0;
			return;
			}
		/*
		lstfile = new TextFl(listflnm, L"wt", merkit);
		if (!lstfile->IsOpen()) {
			writeerror_w(L"Tiedoston avaaminen ei onnistunut", 0);
			delete lstfile;
			lstfile = 0;
			return;
			}
		*/
		}
	  lisakentat(tiedostoon, 1);
	  nlaina = 0;
	  nlainatot = 0;
	  nomatot = 0;
	  neiemit = 0;
	  ilmvert = 0;
	  if (paiva) {
		 arv_pv = paiva - 1;
		 n_pv = 1;
		 ots_pv = arv_pv;
		 }
	  else {
		 arv_pv = jarjpv;
		 n_pv = kilpparam.n_pv_akt;
		 ots_pv = -1;
		 }

	  memset(key, 0, sizeof(key));
	  switch(luetlaji) {
		 case L'A' :
			ind = *srjaakindex;
			ind.ix = 0;
			nk = 2;
			laji = (paiva == 0) ? 4 : 0;
			for (isrj = 0; isrj < sarjaluku; isrj++) {
				nilmsrj = paiva == 0 ? nilm[isrj] : nilm_pv[arv_pv][isrj];
				if (sarjat[isrj] && nilmsrj) {
					SWAB(&isrj, key);
					break;
					}
				}
			break;
		 case L'B' :
			ind = *ljarjindex;
			ind.ix = 0;
			ind.flags = 0;
			nk = 2;
			laji = (paiva == 0) ? 4 : 0;
			for (isrj = 0; isrj < sarjaluku; isrj++) {
				nilmsrj = paiva == 0 ? nilm[isrj] : nilm_pv[arv_pv][isrj];
				if (sarjat[isrj] && nilmsrj) {
					SWAB(&isrj, key);
					break;
					}
				}
			break;
		 case L'C' :
			ind = *srjnroindex;
			ind.ix = 0;
			nk = 2;
			laji = (paiva == 0) ? 4 : 0;
			for (isrj = 0; isrj < sarjaluku; isrj++) {
				nilmsrj = paiva == 0 ? nilm[isrj] : nilm_pv[arv_pv][isrj];
				if (sarjat[isrj] && nilmsrj) {
					SWAB(&isrj, key);
					break;
					}
				}
			break;
		 case L'N' :
			ind = *nroindex;
			ind.ix = 0;
			ind.flags = jarjpv < 0 ? 1 : 0;
			nk = 0;
			laji = (paiva == 0) ? 5 : 1;
			break;
		 case L'K' :
			ind = *aakkosindex;
			ind.ix = 0;
			nk = 0;
			laji = (paiva == 0) ? 5 : 1;
			break;
//         case L'T' :
		 case L'E' :
			ind = *seuraindex;
			ind.ix = 0;
			ind.flags = 0;
			laji = (paiva == 0) ? 6 : 2;
			if (seuralista && seuralista[0][0]) {
				if (seuralistaMaa)
					keyfromwname(key, seuralista[0], 22, 0);
				else {
					wcscpy(st, L"FIN");
					wcscpy(st+3, seuralista[0]);
					keyfromwname(key, st, 22, 0);
					}
				iSeura = 0;
				}
			nk = 22;
			break;
		 case L'F' :
			ind = *seuranoindex;
			ind.ix = 0;
			ind.flags = 0;
			laji = (paiva == 0) ? 6 : 2;
			if (seuralista && seuralista[0][0]) {
				if (seuralistaMaa)
					keyfromwname(key, seuralista[0], 22, 0);
				else {
					wcscpy(st, L"FIN");
					wcscpy(st+3, seuralista[0]);
					keyfromwname(key, st, 22, 0);
					}
				iSeura = 0;
				}
			nk = 22;
			break;
		 case L'L' :
			ind = *lahtoindex;
			ind.ix = 0;
			lahtojarjfl = 1;
			for (lno = 0; lno < 256 && !sarjat[lno]; lno++) ;
			if (arv_pv < 0 || lno >= 256)
				goto escape;
			key[0] = lno;
//			keyv2 = lno;
			keyv2 = 0;
			nk = 1;
			laji = 3;
			for (int i = 0; i < sarjaluku; i++) {
				if (Sarjat[i].luonne[arv_pv] >= 0 && nilm_pv[arv_pv][i] > 0
					&& !Sarjat[i].vakanttisarja) {
					ensl = Sarjat[i].enslahto[arv_pv];
					break;
					}
				}
			break;
		 case L'R' :
			if (arv_pv < 0)
				goto escape;
			ind = *rataindex;
			ind.ix = 0;
			nk = 2;
			laji = 0;
			for (isrj = 0; isrj < sarjaluku; isrj++) {
				nilmsrj = nilm_pv[arv_pv][isrj];
				if (sarjat[isrj] && nilmsrj) {
					SWAB(&isrj, key);
					break;
					}
				}
			break;
		 }
	  ok = !teeindeksi(&ind, true);
	  if (arv_pv < 0)
		 arv_pv = 0;
	  memcpy(keyv, key, MAXINDL);
	  if (ok)
		  ok = !searchix(&ind, key, &d);

	  vmarg[tiedostoon] = llparam.marg[tiedostoon][laji];
	  srj = -1;
	  ms = 0;
	  nkilp = 0;
	  ok = 1;
	  initpage(&tulprm, 0);
	  if (tiedostoon)
		  tulprm.tulmuot.sivpit = 99999;
	  if (llhtml) {
			sendln(tulprm.lstf, L"<!DOCTYPE html>\n<html>");
			if (tulprm.merkit == L'A')
				sendln(tulprm.lstf, L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
			else
				sendln(tulprm.lstf, L"<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
			css_string(&tulprm, 4);
			sendln(tulprm.lstf, L"<title>");
			sendln(tulprm.lstf, otsTeksti(NULL, llparam.luetots0, 100, ots_pv));
			sendln(tulprm.lstf, L"</title>\n");
			sendln(tulprm.lstf, L"</head>");
			sendln(tulprm.lstf, L"<body>");
			sendln(tulprm.lstf, L"<H2 CLASS=otsikko>");
			sendln(tulprm.lstf, otsTeksti(NULL, llparam.luetots0, 100, ots_pv));
			sendln(tulprm.lstf, L"</H2>\n");
/*
		 if (ansifl)
			sendln(tulprm.lstf, L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n<title>");
		 else
			sendln(tulprm.lstf, L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n<title>");
		 sendln(tulprm.lstf, llparam.luetots0);
		 sendln(tulprm.lstf, L"</title>\n</head>\n<body>\n<H2>");
		 sendln(tulprm.lstf, llparam.luetots0);
		 sendln(tulprm.lstf, L"</H2>\n");
*/
		 if (LisaRivit) {
			 kirjLisaRivit(&tulprm, LisaRivit);
			 }
		 if (wcswcind(luetlaji, L"KN") >= 0) {
			if (!tulprm.tulmuot.tauluhtml)
				sendln(tulprm.lstf, L"<pre>\n");
			 else {
				lots[0] = 0;
				sarakeleveydet(tulprm, lots, laji);
				sendln(tulprm.lstf, lots);
				lots[0] = 0;
				}
			}
		 }
	  else {
		 if (llparam.luetots0[0]) {
				paaots_on(&tulprm);
			put_str(&tulprm, otsTeksti(NULL, llparam.luetots0, 100, ots_pv), 0);
			endline(&tulprm, 1);
			paaots_pois(&tulprm);
			l += 3;
			}
		 }
	  lotsf = TRUE;
	  l0 = l;
	  kjono = 0;
	  ens = 1;

	  // Tästä alkaa kilpailijat läpi käyvä päälooppi

	  do {
		 cent = 1;
		 initline();
		 memset(lots, 0, sizeof(lots));
		 if (kilpparam.n_pv_akt > 1) {
			 if (njono > 1) {
				if ((luetlaji == L'B' || luetlaji == L'C') && srj >= 0) {
				   if (kjono == njono)
					  kjono = 1;
				   else {
					  kjono++;
					  }
				   ok = 1;
				   for (nj = 1; ok && nj < kjono && i16swab(key) == srj; nj++)
					   ok = !nextix(&ind,key,&d);
				   if (!ok) break;
				   }
				}
			 else
				kjono = 1;
			 if ((luetlaji == 'B' || luetlaji == 'C') && memcmp(key, keyv, nk))
				kjono = 1;
			}

		 if (memcmp(key, keyv, nk)) {

		 // Sarja, lähtöpaikka tai seura vaihtunut. Uusi ryhmä alkaa

			nkilp = 0;
			switch (luetlaji) {
			   case 'A' :
			   case 'B' :
			   case 'C' :
			   case 'R' :
				  for (srj = srj+1 ; srj < sarjaluku; srj++) {
					  if (sarjat[srj] && (paiva == 0 ? nilm[srj] : nilm_pv[arv_pv][srj]) > 0
						  && (!(Options & 8) || onkolasna(srj, paiva ? arv_pv : -1)))
						  break;
					  }
				  if (srj >= sarjaluku)
					 goto escape;
				  memset(key,0,sizeof(key));
				  SWAB(&srj, key);
				  ok = !searchix(&ind, key, &d);
				  break;
			   case 'E' :
				  if (seuralista && seuralista[0][0]) {
					 if (seuralista[++iSeura][0] == 0)
						 goto escape;
					 if (seuralistaMaa)
						keyfromwname(key, seuralista[iSeura], 22, 0);
					 else {
						wcscpy(st, L"FIN");
						wcscpy(st+3, seuralista[iSeura]);
						keyfromwname(key+3, st, 22, 0);
						}
					 ok = !searchix(&ind, key, &d);
					 }
				  break;
			   case 'F' :
				  if (seuralista && seuralista[0][0]) {
					 if (seuralista[++iSeura][0] == 0)
						 goto escape;
					 if (seuralistaMaa)
						keyfromwname(key, seuralista[iSeura], 22, 0);
					 else {
						wcscpy(st, L"FIN");
						wcscpy(st+3, seuralista[iSeura]);
						keyfromwname(key+3, st, 22, 0);
						}
					 ok = !searchix(&ind, key, &d);
					 }
				  break;
			   case 'L' :
				  for (lno = lno+1 ; lno < 256 && !sarjat[lno]; lno++) ;
				  if (lno >= 256)
					goto escape;
				  memset(key,0,sizeof(key));
				  key[0] = lno;
				  ok = !searchix(&ind, key, &d);
			   }
			if (!alkusivu && l > 1 && tiedostoon == 0 && (Options & 0x80000000)) {
				l = 0;
				uusisivu = false;
				endpage(&tulprm);
				initpage(&tulprm, 0);
				}
			alkusivu = false;
			}
		 memcpy(keyv, key, nk);
		 if (kilpparam.n_pv_akt > 1) {
			 if (njono && luetlaji == L'L') {
				if (!ljonot) {
				   ljpit = teejonot(&ljonot, njono, key[0], d, &ind);
				   ok = !searchix(&ind, key, &d);
				   if (!ljpit) break;
				   kj = 0;
				   kjono = 1;
				   }
				}
			}
		 if ( (ilmvert == 0 && l > 3) || uusisivu || !tulprm.mahtuusivulle(l, 3)) {
			if (luetlaji != L'L' && !uusisivu && tulprm.mahtuusivulle(l, 10 + 2 * (wcswcind(luetlaji, L"EF") >= 0))) {
			   if (wcswcind(luetlaji, L"EFT") >= 0) {
				  pp = ind.pos;
				  ns = 0;
				  do {
					 ok = !nextix(&ind,key,&d);
					 ns++;
					 } while ( !memcmp(key, keyv, nk) );
				  ind.pos = pp-1;
				  ok = !nextix(&ind,key,&d);
				  }
			   else {
					if (paiva == 0)
						ns = nilm[key[1]+256*key[0]];
					else
						ns = nilm_pv[arv_pv][key[1]+256*key[0]];
					}
				if (kilpparam.n_pv_akt > 1)
					ns = ns/njono + 1;
			   if ((Options & 0x80000000) == 0 && tulprm.mahtuusivulle(l, 9 + ns + 2 * (wcswcind(luetlaji, L"EF") >= 0))) {
				  newline(&tulprm, 3);
				  l += 3;
				  }
			   else {
				  l = 0;
				  uusisivu = false;
				  endpage(&tulprm);
				  initpage(&tulprm, 0);
				  }
			   }
			else {
			   l = (luetlaji == L'L') ? 1 : 0;
			   uusisivu = false;
			   endpage(&tulprm);
			   initpage(&tulprm, 0);
			   }
			lotsf = TRUE;
			l0 = l;
			}
		 else if( l > l0 ) {
			l++;
			newline(&tulprm, 1);
			}
		 aliots_on(&tulprm);
		 if (lotsf && (llparam.luetots1[0] != 0 || llparam.pvfl[tiedostoon][laji] ||
			llparam.klfl[tiedostoon][laji])) {
			if (llhtml)
				sendln(tulprm.lstf, L"<H3>\n");
			lots[0] = 0;
			wcscat(lots, otsTeksti(NULL, llparam.luetots1, 100, ots_pv));
			if (llparam.pvfl[tiedostoon][laji] && l <= 10) {
			   wcscat(lots,L"   ");
			   wcscat(lots,wpaiva());
			   }
			if (llparam.klfl[tiedostoon][laji] && l <= 10) {
			   wcscat(lots,L"   ");
			   wcscat(lots,wkello());
			   }
			put_str(&tulprm, lots, 0);
			lots[0] = 0;
			endline(&tulprm, 1);
			if (llhtml)
				sendln(tulprm.lstf, L"</H3>\n");
			l += 2;
			}
		 lotsf = FALSE;
		 if (laji != 1 && !llhtml) {
			wcscat(lots, otsTeksti(NULL, llparam.luetots21, 100, ots_pv));
			if (lots[0])
				wcscat(lots, L" ");
			}
		 if (wcslen(lots) == 0) wcscpy(lots, L"            ");
		 kilp.GETREC(d);
		 keyv[nk] = 0;
		 switch(luetlaji) {
			case L'A' :
			case L'B' :
			case L'C' :
			case L'R' :
			   srj = i16swab(keyv);
			   if (llhtml) {
				  sendln(tulprm.lstf, L"<P>\n");
				  for (isrj = 0; isrj < sarjaluku; isrj++) {
						if (!sarjat[isrj] || (paiva == 0 ? nilm[isrj] : nilm_pv[arv_pv][isrj]) < 1)
							continue;
						if ((Options & 8) && !onkolasna(isrj, paiva ? arv_pv : -1))
							continue;
						sendln(tulprm.lstf, L"<A href=\x22#");
						sendln(tulprm.lstf, Sarjat[isrj].sarjanimi);
						sendln(tulprm.lstf, L"\x22>");
						sendln(tulprm.lstf, Sarjat[isrj].sarjanimi);
						sendln(tulprm.lstf, L"</A>\n");
						}
				  sendln(tulprm.lstf, L"</P>\n");
				  sendln(tulprm.lstf, L"\n<H3><A NAME=\x22");
				  sendln(tulprm.lstf, Sarjat[srj].sarjanimi);
				  sendln(tulprm.lstf, L"\x22>");
				  lots[0] = 0;
				  if (laji != 1) {
					  otsTeksti(lots, llparam.luetots21, 80, ots_pv);
					  if (lots[0])
						  wcscat(lots, L" ");
					  }
				  }
			   if (Sarjat[srj].psarjanimi[0] && njono == 1) {
				   wcscat(lots, Sarjat[srj].psarjanimi);
				   }
			   else {
					wchar_t mtkst[80] = L"", *plj = NULL;
					int ll = 1;
				   wcscat(lots, Sarjat[srj].sarjanimi);
				   if (llparam.matkafl[tiedostoon][laji] && Sarjat[srj].matka[arv_pv][0]) {
					  wcscat(lots, L"   (");
					  if (n_pv > 1) {
						  for (int ii = Sarjat[srj].enspv; ii <= Sarjat[srj].viimpv; ii++) {
							  if (mtkst[0])
								  wcscat(mtkst, L"+");
							  wcscat(mtkst, Sarjat[srj].matka[ii]);
							  }
					  	  }
					  else
						  wcscpy(mtkst, Sarjat[srj].matka[arv_pv]);
					  if (mtkst[wcslen(mtkst)-1] >= L'0' && mtkst[wcslen(mtkst)-1] <= L'9' &&
						  _wtoi(Sarjat[srj].matka[arv_pv]) <= 100) {
						  wcscat(mtkst, L" km");
						  }
					  else if (wcslen(mtkst) >= 2 && wcswcind(mtkst[wcslen(mtkst)-1], L"VP") >= 0 &&
						  (mtkst[wcslen(mtkst)-2] <= L'9' ||
						  (wcslen(mtkst) >= 3 && (ll = 2) > 0 && wcswcind(mtkst[wcslen(mtkst)- ll], L"VP") >= 0 &&
						   mtkst[wcslen(mtkst)-3] <= L'9'))) {
						  wchar_t *plj;
						  plj = mtkst+wcslen(mtkst)-ll;
						  wmemmove(plj+3 , plj, ll);
						  wmemcpy(plj, L"km ", 3);
						  }
					  if (plj == NULL && Sarjat[srj].tapa[arv_pv][0] >= L'P') {
						 wcscat(mtkst, L" ");
						 wcscat(mtkst, Sarjat[srj].tapa[arv_pv]);
						 }
					  wcscat(mtkst, L")");
					  wcscat(lots, mtkst);
					  }
				   if (njono > 1) {
					  wcscat(lots, L"  Jono  ");
					  lots[wcslen(lots)-1] = '0'+kjono;
					  }
				   }
			   if (llhtml) {
					wcscat(lots, L"</A>");
					if (laji != 1 && llparam.luetots22[0]) {
						wcscat(lots, L" ");
						wcscat(lots, otsTeksti(NULL, llparam.luetots22, 100, ots_pv));
						}
					wcscat(lots, L"</H3>\n");
					}
			   break;
			case 'T' :
			case 'E' :
			case 'F' :
			   if (llhtml) {
					wcscpy(lots, L"<H3>");
					if (laji != 1) {
						wcscat(lots, otsTeksti(NULL, llparam.luetots21, 100, ots_pv));
						if (lots[0])
							wcscat(lots, L" ");
						}
					}
			   switch (sranimi) {
				   case L'S':
					 wcscat(lots, kilp.seura);
					  break;
				   case L'L':
					 wcscat(lots, kilp.seuralyh);
					  break;
				   case L'Y':
					 wcscat(lots, kilp.yhdistys);
					  break;
				   }
			   if (kilp.maa[0]) {
				   wcscat(lots, L"  -  ");
				   wcscat(lots, kilp.maa);
				   }
			   if (llhtml) {
					if (laji != 1 && llparam.luetots22[0]) {
						wcscat(lots, L" ");
						wcscat(lots, otsTeksti(NULL, llparam.luetots22, 100, ots_pv));
						}
					wcscat(lots, L"</H3>\n");
					}
			   break;
			case 'L' :
			   if (llhtml) {
					wcscat(lots, L"<H3>\n");
					if (laji != 1) {
						wcscat(lots, otsTeksti(NULL, llparam.luetots21, 100, ots_pv));
						if (lots[0])
							wcscat(lots, L" ");
						}
					}
			   if (njono <= 1)
				  {
				  swprintf(lots + wcslen(lots), L"Lähtö %d", (int)keyv[0]);
				  }
			   if (llhtml) {
					if (laji != 1 && llparam.luetots22[0]) {
						wcscat(lots, L" ");
						wcscat(lots, otsTeksti(NULL, llparam.luetots22, 100, ots_pv));
						}
					wcscat(lots, L"</H3>\n");
					}
			   naika = 0;
			   naikamax = 0;
			   break;
			default:
			   if (llhtml) wcscat(lots, L"\n");
			}
		 if (llhtml) {
			 if (!tulprm.tulmuot.tauluhtml)
				wcscat(lots, L"<pre>\n");
			 else {
				sarakeleveydet(tulprm, lots, laji);
			 }
		 }
		 if (laji != 1 && !llhtml && llparam.luetots22[0]) {
			wcscat(lots, L" ");
			wcscat(lots, otsTeksti(NULL, llparam.luetots22, 100, ots_pv));
			}
		 if (ilmvert == 0) {
			if (laji != 1) {
			   put_str(&tulprm, lots, 0);
			   endline(&tulprm, 1);
			   l++;
			   l++;
			   }
			if (llparam.luetots3[0] != 0) {
			   put_str(&tulprm, otsTeksti(NULL, llparam.luetots3, 100, ots_pv), 0);
			   endline(&tulprm, 1);
			   l++;
			   l++;
			   }
			}
		 aliots_pois(&tulprm);
		 l0 = l;

		 // Sisempi kilpailijoita läpi käyvä looppi

		 do {
			int bdg = 0;

			kilp.GETREC(d);
			ksrj = kilp.Sarja(paiva-1);
			if (ljonot && luetlaji == L'L' && njono > 1) {
			   if (kilp.Sarja(arv_pv) != srj && kilp.tark(arv_pv) != L'V' && kilp.tark(arv_pv) != L'P') {
				  srj = kilp.Sarja(arv_pv);
				  newline(&tulprm, 1);
				  initline(&tulprm);
				  swprintf(lots, L"   %s   Jono %d", Sarjat[srj].sarjanimi, kjono);
				  put_str(&tulprm, lots, 0);
				  endline(&tulprm, 1);
				  l++;
				  l++;
				  l++;
				  }
			   }

			if (paiva == 0) {
				for (pv = Sarjat[ksrj].enspv; pv <= Sarjat[ksrj].viimpv; pv++) {
					if (Sarjat[ksrj].luonne[pv] < 0)
						continue;
					if (!bdg)
						bdg = kilp.pv[pv].badge[0];
					if ((kilp.tark(pv) != L'V' || (njono == 1 && (Options & 1))) &&
						kilp.tark(pv) != L'P' && kilp.Sarja(pv) == ksrj)
						break;
					}
				}
			else
				bdg = kilp.pv[arv_pv].badge[0];

			if ((paiva == 0 && pv <= Sarjat[ksrj].viimpv)
			   || (paiva != 0 && Sarjat[ksrj].luonne[arv_pv] >= 0 &&
				(kilp.tark(arv_pv) != L'V' || (Options & 1)) &&
				kilp.tark(arv_pv) != L'P')) {
				bool LLmukaan;

				LLmukaan = luetlaji != L'L' ||
					(LLvaat == 0) ||
					(LLvaat == 1 && kilp.tark(arv_pv) == L'-') ||
					(LLvaat == 2 && kilp.tark(arv_pv) == L'E') ||
					(LLvaat == 3 && (wcswcind(kilp.tark(arv_pv), L"-EPV") < 0 || kilp.tulos_pv(arv_pv)));
				LLmukaan = LLmukaan && (ilmvert == 0 || kilp.ilmlista == ilmvert);
				LLmukaan = LLmukaan &&
					 (emitrajaus == L'K' ||
					 (emitrajaus == L'L' && kilp.pv[arv_pv].laina[0] == L'L') ||
					 (emitrajaus == L'O' && kilp.pv[arv_pv].badge[0] == 0));

			   memset(fldstr, 0, sizeof(fldstr));

			   if (LLmukaan) {
				  wcscpy(fldstr[LLF_SARJA], Sarjat[ksrj].sarjanimi);
				  if ((wcswcind(luetlaji, L"ABC") >= 0) && (ilmvert == 0))
					 fldstr[LLF_SARJA][0] = 0;
				  if (kilp.tark(arv_pv) == L'V') {
					   wcscpy(fldstr[LLF_NIMI], L"VAKANTTI");
					  }
				  else {
					  elimwbl(kilp.arvo);
					  if (llparam.vaihdanimet[tiedostoon][laji]) {
						 fldstr[LLF_NIMI][0] = 0;
						 if (kilp.arvo[0]) {
							wcscpy(fldstr[LLF_NIMI], kilp.arvo);
							wcscat(fldstr[LLF_NIMI], L" ");
							}
						 kilp.nimi(st, 60, true);
						 wcsncpy(fldstr[LLF_NIMI]+wcslen(fldstr[LLF_NIMI]), st, 60);
						 }
					  else {
						 wcsncpy(fldstr[LLF_NIMI], kilp.nimi(st, 60, false), 60);
						 if (kilp.arvo[0]) {
							wcscat(fldstr[LLF_NIMI], L", ");
							wcscat(fldstr[LLF_NIMI], kilp.arvo);
							}
						 }
					  }
				  _itow(kilp.id(arv_pv), fldstr[LLF_KNO], 10);
				  if (llparam.tarknofl[tiedostoon][laji]) {
					 _itow(tarkno(kilp.id(arv_pv)), as1+1, 10);
					 as1[0] = L'-';
					 wcscat(fldstr[LLF_KNO], as1);
					 }
					switch (sranimi) {
						case L'S':
							wcscpy(fldstr[LLF_SEURA], kilp.seura);
							break;
						case L'L':
							wcscpy(fldstr[LLF_SEURA], kilp.seuralyh);
							break;
						case L'Y':
							wcscpy(fldstr[LLF_SEURA], kilp.yhdistys);
							break;
						}
				  StrAlisarja(fldstr[LLF_ALISARJA], &kilp);
				  wcscpy(fldstr[LLF_MAA], kilp.maa);
				  if (kilp.pv[arv_pv].tlahto != TMAALI0 &&
					 (kilp.tark(arv_pv) != L'V' || n_pv == 1) && kilp.tark(arv_pv) != L'P') {
					 wcsncpy(fldstr[LLF_AIKA], AIKATOWSTRS(st, kilp.pv[arv_pv].tlahto,t0), 8);
					 fldstr[LLF_AIKA][8] = 0;
					 if (!wmemcmp(fldstr[LLF_AIKA]+5,L".00",3) &&
						(Sarjat[kilp.Sarja(arv_pv)].lahtovali[0] % 60) == 0)
						wmemset(fldstr[LLF_AIKA]+5, L' ', 3);
					 }
				  else
					 wcscpy(fldstr[LLF_AIKA], L"__.__");
				  if (paiva == 0) {
					  int vah = 0;
					  if (Sarjat[kilp.Sarja(arv_pv)].viimpv > arv_pv &&
							Sarjat[kilp.Sarja(arv_pv)].lahtovali[Sarjat[kilp.Sarja(arv_pv)].viimpv] == 1)
							vah = 1;
					  for (pv = arv_pv+1; pv <= Sarjat[kilp.Sarja(arv_pv)].viimpv-vah; pv++) {
						 if (kilp.pv[pv].tlahto != TMAALI0 && kilp.tark(pv) != L'V' && kilp.tark(pv) != L'P') {
							 wcsncpy(fldstr[LLF_AIKA2+pv-1], AIKATOWSTRS(st, kilp.pv[pv].tlahto,t0), 8);
							 fldstr[LLF_AIKA2+pv-1][8] = 0;
							 if (!wmemcmp(fldstr[LLF_AIKA2+pv-1]+5,L".00",3) &&
								 (Sarjat[kilp.Sarja(arv_pv)].lahtovali[0] % 60) == 0)
								 wmemset(fldstr[LLF_AIKA2+pv-1]+5, L' ', 3);
							 }
						  else
							 wcscpy(fldstr[LLF_AIKA2+pv-1], L"__.__");
						  }
					  for (pv = Sarjat[kilp.Sarja(arv_pv)].viimpv+1; pv < kilpparam.n_pv_akt; pv++) {
						  fldstr[LLF_AIKA2+pv-1][0] = 0;
						  }
					  }
				  if (kilp.lisno[0])
					  _itow(kilp.lisno[0], fldstr[LLF_LISNO], 10);
				  if (kilp.lisno[1])
					  _itow(kilp.lisno[1], fldstr[LLF_LISNO2], 10);
				  if (bdg)
					  _itow(bdg, fldstr[LLF_EMIT], 10);
				  if (kilp.pv[arv_pv].badge[1])
					  _itow(kilp.pv[arv_pv].badge[1], fldstr[LLF_EMIT2], 10);
				  if (!bdg)
					 neiemit++;
				  else if (kilp.pv[arv_pv].laina[0] == L'L') {
					 nlaina++;
					 nlainatot++;
					 }
				  else
					 nomatot++;
				  if (bdg && (Options & 4)) {
					 wmemset(as1, L' ', sizeof(as1)/2);
					 if (kilp.pv[arv_pv].laina[0] == L'L')
						 wcscat(fldstr[LLF_EMIT],L"L");
					 else
						 wcscat(fldstr[LLF_EMIT],L" ");
					 }
				  nj = 0;
				  mks = 0;
				  npv = vaiheLkm(kilp.Sarja(-1));
				  for (pv = 0; pv < kilpparam.n_pv_akt; pv++) {
					 if (Sarjat[kilp.Sarja(pv)].luonne[pv] >= 0) {
						 if (kilp.tark(pv) != L'V' && kilp.tark(pv) != L'P') {
							nj++;
							if (Sarjat[kilp.Sarja(pv)].maksu1)
								mks += Sarjat[kilp.Sarja(pv)].maksu1;
							else if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S')
								mks = Sarjat[kilp.Sarja(pv)].maksu;
							else
								mks += Sarjat[kilp.Sarja(pv)].maksu/npv;
							}
						 }
					 }
				  if (nj == npv)
					 mks = Sarjat[kilp.Sarja(-1)].maksu;
				  if (kilp.ilmlista >= 900) {
					 if (kilp.ilmlista >= 950)
						mks += jikorotus*mks/100;
					 else
						mks = 0;
					 }
				  if (mks > 199)
					 cent = 100;
				  else
					 cent = 1;
//				  mks /= cent;
				  if (tulkentta[tiedostoon][LLF_MAKSU].len[laji]) {
					 if (cent && mks%100) {
						swprintf(fldstr[LLF_MAKSU], L"%ld,%2.2ld", mks/100, mks%100);
						}
					 else
						_itow(mks/100, fldstr[LLF_MAKSU], 10);
					 }
				  if (Options & 0x1000000) {
					  if (arv_pv == k_pv+1) {
						  kilp.ytulos_wstr_cols_n(st, k_pv, tulprm.tulmuot.des_sep,
							  tulprm.tulmuot.pistedesim, 0, 8, 2);
						  elimwzb1(st);
						  wcscpy(fldstr[LLF_RANKI], st);
						  aikatowstr_cols_n(st, kilp.pv[arv_pv].pvpisteet[1], 0, tulprm.tulmuot.des_sep, 8);
						  elimwzb1(st);
						  wcscpy(fldstr[LLF_PISTE2], st);
						  }
					  }
				  else if (pistejakaja > 1) {
						wchar_t fmtstr[20];

						if (pistejakaja > 1 && pistejakaja <= 10)
							wcscpy(fmtstr, L"%.1lf");
						else if (pistejakaja <= 100)
							wcscpy(fmtstr, L"%.2lf");
						else
							wcscpy(fmtstr, L"%.3lf");
						swprintf(fldstr[LLF_RANKI], fmtstr, (double)kilp.pisteet[2]/pistejakaja);
						swprintf(fldstr[LLF_PISTE2], fmtstr, (double)kilp.pisteet[1]/pistejakaja);
					  }
				  else {
					  _itow(kilp.pisteet[2], fldstr[LLF_RANKI], 10);
					  _itow(kilp.pisteet[1], fldstr[LLF_PISTE2], 10);
					  }
				  _itow(kilp.ilmlista, fldstr[LLF_LISTA], 10);
				  prt_kilp(&tulprm, laji);
				  ms += mks;
				  l++;
				  }
			   }
			if (!keyv2)
				keyv2 = *(INT32 *)(key+1);

			// Monien jonojen tapauksessa vaihtuu indeksi loopin alussa, ellei kyseessä lähtöjen tuloste

			ok = 1;
			if (njono > 1 && luetlaji == L'L' && ljonot) {
			   do {
				  if (kj >= njono*ljpit) {
					 kjono++;
					 kj = kjono-1;
					 }
				  else {
					 kj += njono;
					 }
				  } while (kjono <= njono &&
					 (kj >= njono*ljpit || !ljonot[kj]));
			   if (kjono > njono) break;
			   d = ljonot[kj];
			   }
			else {

				// looppi mahdollinen vain lähtökohtaiselle tulosteelle. Muuten njono on aina 1
				for (nj = 0; nj < njono; nj++) {
					bool pois = false;

					// looppi ohittamaan lähtökohtaisesta luettelosta ilman lähtöaikaa olevat sekä
					// vakanttisarjojen vakantit
					while(1) {
						ok = !nextix(&ind,key,&d);
						if (!ok) {
							pois = true;
							break;
							}
						if (luetlaji == L'L') {
							kilptietue kilp1;
							kilp1.GETREC(d);
							if (kilp1.TLahto(arv_pv, false) == TMAALI0 || kilp1.tark(arv_pv) == L'P' ||
								(kilp1.tark(arv_pv) == L'V' && Sarjat[kilp1.Sarja(arv_pv)].vakanttisarja))
								continue;
							}
						if (key[0] != keyv[0]) {
							pois = true;
							}
						break;
						}
					if (pois)
						break;
					}
				}

			if (ok && luetlaji == L'L' && njono == 1 && LLvaat < 2) {

			   if (++naika > naikamax) naikamax = naika;
			   if (keyv2 != * (INT32 *) (key+1)) {
				   int tl1, tl2;
				   memcpy(&tl1, key+1, 4);
				   lswab((long *) &tl1);
				   tl2 = keyv2;
				   lswab((long *) &tl2);
				   tl1 = (tl1 - tl2 + 24 * TUNTI) % (24 * TUNTI);
				   if (tl1 > 0 && tl1 < vakiovali)
						vakiovali = tl1;
				   if ((Options & 1) && key[0] == keyv[0]) {
						tl1 /= vakiovali;
						if (tl1 > 3)
							tl1 = 2;
						for (int i = 1; i < tl1; i++) {
							  if (!tulprm.mahtuusivulle(l, 3)) {
								 uusisivu = false;
								 endpage(&tulprm);
								 swprintf(lots, L"Lähtö %d", (int)keyv[0]);
								 put_str(&tulprm, lots, 0);
								 endline(&tulprm, 1);
								 l = 2;
								 }
							  else {
								 if (llhtml && tulprm.tulmuot.tauluhtml) {
									   memset(fldstr, 0, sizeof(fldstr));
									   fldstr[LLF_AIKA][0] = L'-';
									  prt_kilp(&tulprm, laji);
									}
								 else
									newline(&tulprm, 1);
								 l++;
								 }
							 AIKATOWSTRS(as1, tl2+ensl+i*vakiovali-t0*TUNTI, t0);
							 as1[8] = 0;
							 if (wcscmp(as1+5, L".00") == 0)
								as1[5] = 0;
							 if (llhtml && tulprm.tulmuot.tauluhtml) {
								  memset(fldstr, 0, sizeof(fldstr));
								  wcsncpy(fldstr[LLF_AIKA], as1, 8);
								  wcscpy(fldstr[LLF_NIMI], L"EI LÄHTIJÖITÄ");
								  prt_kilp(&tulprm, laji);
								}
							 else {
								  swprintf(st, L"%-8s    EI LÄHTIJÖITÄ", as1);
								  put_str(&tulprm, st, 0);
								  }
							  endline(&tulprm, 0);
							  l++;
							  }
						}
				  if (!tulprm.mahtuusivulle(l, naikamax+2)) {
					 uusisivu = true;
					 naikamax = 0;
					 }
				  else {
					 if (llhtml && tulprm.tulmuot.tauluhtml) {
						   memset(fldstr, 0, sizeof(fldstr));
						   fldstr[LLF_AIKA][0] = L'-';
						  prt_kilp(&tulprm, laji);
						}
					 else
						 newline(&tulprm, 1);
					 l++;
					 }
				  naika = 0;
				  }
			   keyv2 = *(INT32 *)(key+1);
			   }
			key[nk] = 0;

			} while ( ok && (!memcmp(key, keyv, nk)) &&
				!uusisivu && tulprm.mahtuusivulle(l, 2 * (wcswcind(luetlaji, L"EF") >= 0)));

		 // Sisemmästä loopista poistutaan, kun ryhmittelyn määrävä tieto vaihtuu
		 // (sarja, seura tai lähtöpaikka)

		 if (wcswcind(luetlaji, L"EF") >= 0 && ilmvert == 0 ) {
			initline(&tulprm);
		  if (nlaina) {
			   swprintf(lots, L"Lainakortteja %d", nlaina);
			   put_str(&tulprm, lots, tulkentta[tiedostoon][LLF_NIMI].pos[laji]);
			   }
			nlaina = 0;
			if (cent && ms%100) {
			   swprintf(as1, L"%ld,%2.2ld", ms/100, ms%100);
			   }
			else
			   _itow(ms/100, as1, 10);
			put_str(&tulprm, as1, tulkentta[tiedostoon][LLF_MAKSU].pos[laji]+
			   tulprm.tulmuot.numlev*(tulkentta[tiedostoon][LLF_MAKSU].len[laji]-wcslen(as1)));
			endline(&tulprm, 1);
			l += 2;
			}
		 if( !ok || memcmp(key, keyv,  nk) ) ms = 0;
		 tul_line[0] = 0;
		 if (llhtml) {
			   if (!tulprm.tulmuot.tauluhtml)
					sendln(tulprm.lstf, L"</pre>\n");
			   else
					sendln(tulprm.lstf, L"</table>\n");
				}
		 if (luetlaji == L'L' && kjono > njono) break;

		 } while (ok || (njono > 1 && kjono < njono));

		// Tähän päättyi kilpailijat läpi käyvä päälooppi

	  if ((wcswcind(luetlaji, L"EFKN") >= 0) && kilpparam.badgelaji > L' ' && !llhtml) {
		 newline(&tulprm, 1);
		 initline(&tulprm);
		 swprintf(lots, L"Listalla emitkortteja: omat %d, laina %d, puuttuu %d ",
			nomatot, nlainatot, neiemit);
		 nomatot = 0;
		 nlainatot = 0;
		 neiemit = 0;
		 put_str(&tulprm, lots, 0);
		 endline(&tulprm, 1);
		 l += 3;
		 }
escape:
  lisakentat(tiedostoon, -1);
  if (llhtml && kesken) sendln(tulprm.lstf, L"</body>\n</html>\n");
  if (l > 2) endpage(&tulprm);
  enddoc(&tulprm);
  if (tulprm.lstf) {
		if (tulprm.printer == GDIPRINTER)
			LeaveCriticalSection(&print_CriticalSection);
	  closeprfile(tulprm.lstf);
	  tulprm.lstf = 0;
	  }
  if (ind.ix) {
	  poistaindeksi(&ind);
	  }
  if (lstfile) {
	  delete lstfile;
	  lstfile = 0;
	  tiedostoon = 0;
	  }
  if (ljonot) free(ljonot);
  lahtojarjfl = 0;
  if (ToimintaTila == 2)
	arv_pv = k_pv;
  else
	arv_pv = 0;
  }

void puuttuvat(wchar_t *paika, INT *psija, INT srj, INT df);

void tulosyhteenveto(tulostusparamtp *tulprm)
   {
   wchar_t *ots;
   wchar_t  paika[16], kaika[16], snimi[LSEURA+1];
   wchar_t  ch;
   char  ch2;
   wchar_t line[84], st[40], co;
   INT l, ll, srj, psrj, psija;

   INT psija2;

   co = tulprm->kohde;
   if (k_pv)
	  ots = L"Sarja      Ilm Yht.aika Aika Kesk  Hyl Auki Kärkiaika "
											L"Mahd.aika  Mahd.sija\n";
   else
	  ots = L"  Sarja      Ilm   Aika  Kesk  Hyl   Auki  Kärkiaika  "
											L"Mahd.aika  Mahd.sija\n";
   kaika[8] = 0;
   if (co != L'P') {
	   sendln(tulprm->lstf, ots);
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
			   swprintf(line,L"%-11s%4d%6d%7d%5d%5d%5d%10s%10s%5d (%3d)\n",
				  Sarjat[srj].sarjanimi,nilm[srj],ntulos[srj][kilpparam.valuku+1],ntulos[srj][0],
				  nkesk[srj],nhyl[srj], n_avoin(srj),
				  kaika,paika,psija,psija2);
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
			   swprintf(line,L" %-11s%3d%6d%6d%6d%6d%11s%11s%6d\n",Sarjat[srj].sarjanimi,
				  nilm[srj],ntulos[srj][0],nkesk[srj],nhyl[srj], n_avoin(srj),
				  kaika,paika,psija);
			   }
			sendln(tulprm->lstf, line);
			}
		 } while (++srj < sarjaluku);
	  }
   else {
		int scale;

		if (tulprm->printer == GDIPRINTER) {
		  initpage(tulprm, NULL);
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
		 putfld(tulprm, L"Sarja", 2*scale, 5, 0, 0);
		 putfld(tulprm, L"Ilm", 14*scale, 3, 1, 0);
		 putfld(tulprm, L"Yht.aika", 18*scale, 8, 1, 0);
		 putfld(tulprm, L"Aika", 26*scale, 4, 1, 0);
		 putfld(tulprm, L"Kesk", 31*scale, 4, 1, 0);
		 putfld(tulprm, L"Hyl", 38*scale, 3, 1, 0);
		 putfld(tulprm, L"Auki", 43*scale, 4, 1, 0);
		 putfld(tulprm, L"Kärkiaika", 49*scale, 9, 1, 0);
		 putfld(tulprm, L"Mahd.aika", 60*scale, 9, 1, 0);
		 putfld(tulprm, L"Mahd.sj", 70*scale, 9, 1, 0);
		 putfld(tulprm, L"Mahd.ysj", 80*scale, 9, 1, 0);
		 }
	  else
		 {
		 putfld(tulprm, L"Sarja", 2*scale, 5, 0, 0);
		 putfld(tulprm, L"Ilm", 14*scale, 3, 1, 0);
		 putfld(tulprm, L"Aika", 19*scale, 4, 1, 0);
		 putfld(tulprm, L"Kesk", 25*scale, 4, 1, 0);
		 putfld(tulprm, L"Hyl", 32*scale, 3, 1, 0);
		 putfld(tulprm, L"Auki", 37*scale, 4, 1, 0);
		 putfld(tulprm, L"Kärkiaika", 43*scale, 9, 1, 0);
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
					putfld(tulprm, Sarjat[srj].sarjanimi, 2*scale, 11, 0, 0);
					putfld(tulprm, _itow(nilm[srj],line,10), 13*scale, 4, 1, 0);
					putfld(tulprm, _itow(ntulos[srj][kilpparam.valuku+1],line,10), 19*scale, 4, 1, 0);
					putfld(tulprm, _itow(ntulos[srj][0],line,10), 25*scale, 4, 1, 0);
					putfld(tulprm, _itow(nkesk[srj],line,10), 31*scale, 4, 1, 0);
					putfld(tulprm, _itow(nhyl[srj],line,10), 37*scale, 4, 1, 0);
					putfld(tulprm, _itow(n_avoin(srj),line,10), 43*scale, 4, 1, 0);
					putfld(tulprm, kaika, 50*scale,8, 1, 0);
					putfld(tulprm, paika, 59*scale,10, 1, 0);
					putfld(tulprm, _itow(psija,line,10), 73*scale, 4, 1, 0);
					putfld(tulprm, _itow(psija2,line,10), 81*scale, 4, 1, 0);
					}
				else {
					if( ntulos[srj][0] == 0 )
						wcscpy(kaika, L"        ");
					else
						AIKATOWSTRS(kaika, jarjtulos[0][0][jalku[srj][0]][0], 0);
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
				if (!mahtuusivulle(l, 2, tulprm)) {
				   endpage(tulprm);
				   initpage(tulprm, NULL);
				   l = 0;
				   }
				}
			 }
	  newline(tulprm, 1);
	  if (k_pv) {
		 swprintf(line,L"Yhteensä  %6d%6d%7d%5d%5d%5d",nilmt,nytulost,
			ntulost,nkeskt,nhylt,(nilmt - ntulost - nkeskt - nhylt - neilt));
		 putfld(tulprm, line, 0, 80, 0, 0);
		 }
	  else
		 {
		 putfld(tulprm, L"Yhteensä", 2*scale, 11, 0, 0);
		 putfld(tulprm, _itow(nilmt,line,10), 13*scale, 4, 1, 0);
		 putfld(tulprm, _itow(ntulost,line,10), 19*scale, 4, 1, 0);
		 putfld(tulprm, _itow(nkeskt,line,10), 25*scale, 4, 1, 0);
		 putfld(tulprm, _itow(nhylt,line,10), 31*scale, 4, 1, 0);
		 putfld(tulprm, _itow(nilmt-ntulost-nkeskt-nhylt-neilt,line,10),
			37*scale,4, 1, 0);
		 }
	  endline(tulprm, 0);
	  endpage(tulprm);
	  l = 0;
	  }
   }

void ooyhteenveto(tulostusparamtp *tulprm)
{
	wchar_t ots[120] = L"Sarja      Ilmoitt.";
	wchar_t line[120], co;
	INT l, srj;
	int ni_pv[N_PV];

	co = tulprm->kohde;
	memset(ni_pv, 0, sizeof(ni_pv));
	if (co != L'P') {
		if (kilpparam.n_pv_akt > 1) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
				swprintf(ots+wcslen(ots), L" %4d. vaihe", ipv+1);
			}
		wcscat(ots, L"\n");
		sendln(tulprm->lstf, ots);
		for (srj = 0; srj < sarjaluku; srj++) {
			if (nyilm[srj] > 0) {
				swprintf(line,L" %-11s    %4d",Sarjat[srj].sarjanimi,
					nyilm[srj]);
				if (kilpparam.n_pv_akt > 1) {
					for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
						swprintf(line+wcslen(line), L"%12d", nilm_pv[ipv][srj]);
						ni_pv[ipv] += nilm_pv[ipv][srj];
						}
					}
				wcscat(line, L"\n");
				sendln(tulprm->lstf, line);
				}
			}
		swprintf(line,L" %-11s    %4d",L"Yhteensä",
			nyilmt);
		if (kilpparam.n_pv_akt > 1) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				swprintf(line+wcslen(line), L"%12d", ni_pv[ipv]);
				}
			}
		wcscat(line, L"\n");
		sendln(tulprm->lstf, line);
		}
	else {
		int scale;

		if (tulprm->printer == GDIPRINTER) {
			initpage(tulprm, L"C");
//			initpage(tulprm, arv_pv ? L"C" : NULL);
			scale = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
			}
		else
			initpage(tulprm, NULL);
		l = 0;
		putfld(tulprm, L"Sarja", 2*scale, 5, 0, 0);
		putfld(tulprm, L"IlmYht", 10*scale, 6, 1, 0);
		if (kilpparam.n_pv_akt > 1) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				swprintf(line, L" %4d. vaihe", ipv+1);
				putfld(tulprm, line, (20+10*ipv)*scale, 8, 1, 0);
				}
			}
		endline(tulprm, 1);
		for (srj = 0; srj < sarjaluku; srj++) {
			if (nyilm[srj] > 0) {
				putfld(tulprm, Sarjat[srj].sarjanimi, 2*scale, 11, 0, 0);
				putfld(tulprm, _itow(nyilm[srj],line,10), 13*scale, 4, 1, 0);
				if (kilpparam.n_pv_akt > 1) {
					for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
						putfld(tulprm, _itow(nilm_pv[ipv][srj],line,10), (23+10*ipv)*scale, 4, 1, 0);
						ni_pv[ipv] += nilm_pv[ipv][srj];
						}
					}
				endline(tulprm, 0);
				if (!tulprm->mahtuusivulle(++l, 3)) {
					endpage(tulprm);
					initpage(tulprm, NULL);
					l = 0;
					}
				}
			}
		newline(tulprm, 1);
		putfld(tulprm, L"Yhteensä", 2*scale, 11, 0, 0);
		putfld(tulprm, _itow(nyilmt,line,10), 13*scale, 4, 1, 0);
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			putfld(tulprm, _itow(ni_pv[ipv],line,10), (23+10*ipv)*scale, 4, 1, 0);
			}
		endline(tulprm, 0);
		endpage(tulprm);
		l = 0;
		}
}

void emityhteenveto(tulostusparamtp *tulprm)
{
	wchar_t ots[120] = L"            ";
	wchar_t line[120], co;
	INT l, srj;
	int lkm[MAXSARJALUKU+1][N_PV][4];
	int ni_pv[N_PV];

	memset(lkm, 0, sizeof(lkm));
	for (int d = 1; d < nrec; d++) {
		kilptietue kilp;

		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			if (!kilp.lasna(ipv))
				continue;
			lkm[kilp.Sarja(ipv)][ipv][0]++;
			lkm[MAXSARJALUKU][ipv][0]++;
			if (kilp.pv[ipv].badge[0]) {
				lkm[kilp.Sarja(ipv)][ipv][1]++;
				lkm[MAXSARJALUKU][ipv][1]++;
				if (kilp.pv[ipv].laina[0] == L'L') {
					lkm[kilp.Sarja(ipv)][ipv][3]++;
					lkm[MAXSARJALUKU][ipv][3]++;
					}
				}
			else {
				lkm[kilp.Sarja(ipv)][ipv][2]++;
				lkm[MAXSARJALUKU][ipv][2]++;
				}
			}
		}
	co = tulprm->kohde;
	if (co != L'P') {
		if (kilpparam.n_pv_akt > 1) {
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
				swprintf(ots+wcslen(ots), L"     %4d. vaihe      ", ipv+1);
			wcscat(ots, L"\n");
			sendln(tulprm->lstf, ots);
			}
		wcscpy(ots, L"Sarja       ");
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
			swprintf(ots+wcslen(ots), L" Läsnä   On   Ei Laina");
		wcscat(ots, L"\n");
		sendln(tulprm->lstf, ots);
		for (int i = -1; i < sarjaluku; i++) {
			if (i < 0) {
				swprintf(line,L"%-12s",L"Yhteensä");
				srj = MAXSARJALUKU;
				}
			else {
				srj = i;
				swprintf(line,L"%-12s", Sarjat[srj].sarjanimi);
				}
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				swprintf(line+wcslen(line), L" %5d%5d%5d%5d ", lkm[srj][ipv][0],
					lkm[srj][ipv][1], lkm[srj][ipv][2], lkm[srj][ipv][3]);
				}
			wcscat(line, L"\n");
			sendln(tulprm->lstf, line);
			}
		}
	else {
		int scale;

		if (tulprm->printer == GDIPRINTER) {
			initpage(tulprm, NULL);
//			initpage(tulprm, arv_pv ? L"C" : NULL);
			scale = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
			}
		else
			initpage(tulprm, NULL);
		l = 0;
		putfld(tulprm, L"Sarja", 0, 5, 0, 0);
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			swprintf(line, L"%4d. vaihe", ipv+1);
			putfld(tulprm, line, (19+22*ipv)*scale, 8, 0, 0);
			}
		endline(tulprm, 1);
		l++;
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			putfld(tulprm, L"Läsnä", (12+22*ipv)*scale, 5, 1, 0);
			putfld(tulprm, L"On", (17+22*ipv)*scale, 5, 1, 0);
			putfld(tulprm, L"Ei", (22+22*ipv)*scale, 5, 1, 0);
			putfld(tulprm, L"Laina", (27+22*ipv)*scale, 5, 1, 0);
			}
		endline(tulprm, 1);
		l++;
		for (int i = -1; i < sarjaluku; i++) {
			if (!mahtuusivulle(l, 2, tulprm)) {
				endpage(tulprm);
				initpage(tulprm, NULL);
				}
			if (i < 0) {
				putfld(tulprm, L"Yhteensä", 0, 8, 0, 0);
				srj = MAXSARJALUKU;
				}
			else {
				srj = i;
				putfld(tulprm, Sarjat[srj].sarjanimi, 0, 10, 0, 0);
				}
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				putfld_int(tulprm, lkm[srj][ipv][0], (12+22*ipv)*scale, 5, 1, 0);
				putfld_int(tulprm, lkm[srj][ipv][1], (17+22*ipv)*scale, 5, 1, 0);
				putfld_int(tulprm, lkm[srj][ipv][2], (22+22*ipv)*scale, 5, 1, 0);
				putfld_int(tulprm, lkm[srj][ipv][3], (27+22*ipv)*scale, 5, 1, 0);
				}
			endline(tulprm, 0);
			l++;
			}
		endpage(tulprm);
		}
}

void yhtveto(wchar_t kohde, wchar_t tiedlaji, wchar_t yvlaji, wchar_t *listflnm, wchar_t merkit,
	wchar_t erotin)
{
	wchar_t line[500];
	int ok, l = 0, ansifl = 0, llhtml = 0;
	tulostusparamtp tulprm;

	tulprm.kohde = kohde;
	if (kohde == L'P') {
		tulprm.printer = GDIPRINTER;
		merkisto(L'G');
		if ((tulprm.lstf = openprfile(listflnm, 1, TRUE, FALSE, trlate, FALSE))
			== NULL) {
			return;
			}
		initgdiprinter(&tulprm);
		EnterCriticalSection(&print_CriticalSection);
		initpage(&tulprm, yvlaji == L'S' ? L"C" : NULL);
		}
	else {
		tulprm.printer = 1;
//		tabst[0] = erotin;
		llparam.tabs[1] = kohde == L'R';
		llhtml = tiedlaji == L'H';
		ansifl = merkit == L'A';
		if ((tulprm.lstf = openprfile(listflnm, -1, TRUE, FALSE, (char *)&merkit, FALSE))
			== NULL) {
			return;
			}
		}
	if (llhtml) {
		if (ansifl)
			sendln(tulprm.lstf, L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n<title>");
		else
			sendln(tulprm.lstf, L"<!DOCTYPE html>\n<html>\n<head><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n<title>");
		sendln(tulprm.lstf, otsTeksti(NULL, llparam.luetots0, 100, k_pv));
		sendln(tulprm.lstf, L"</title>\n</head>\n<body>\n<H2>");
		sendln(tulprm.lstf, otsTeksti(NULL, llparam.luetots0, 100, k_pv));
		sendln(tulprm.lstf, L"</H2>\n<pre>\n");
		}
	if (yvlaji == L'S') {
		char key[MAXINDL+1], keyv[MAXINDL+1];
		int d, nk, nlasna[N_PV], nkilp, mstot = 0;
		kilpindex ind;
		kilptietue kilp;

		ind = *seuraindex;
		ind.ix = 0;
		nk = 22;
		ok = !teeindeksi(&ind, true);
		memset(key, 0, MAXINDL);
		if (ok)
			ok = !searchix(&ind, key, &d);

		nkilp = 0;
		while(ok) {
			int cent, ms;
			wchar_t maa[4], seura[LSEURA+1], seuralyh[LLYH+1];

			if (l == 0) {
				swprintf(line, L"   %-30.30s %-10.10s %3.3s %4.4s    %5s",
					L"Seura", L"Lyhenne", L"Maa", L"Lkm", L"Maksu");
				for	(int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
					swprintf(line+wcslen(line), L" %d.pv", ipv+1);
				wcscat(line, L"\n");
				sendln(tulprm.lstf, line);
//				endline(&tulprm, 0);
				l++;
				}
			memcpy(keyv, key, MAXINDL);
			cent = 1;
			nkilp = 0;
			ms = 0;
			memset(nlasna, 0, sizeof(nlasna));
			kilp.GETREC(d);
			keyv[nk] = 0;
			wcscpy(seura, kilp.seura);
			wcscpy(seuralyh, kilp.seuralyh);
			wcscpy(maa, kilp.maa);

			 do {

				int nj, npv, mks;

				kilp.GETREC(d);
				nj = 0;
				for (int pv = Sarjat[kilp.Sarja()].enspv; pv <= Sarjat[kilp.Sarja()].viimpv;
					pv++) {
					if (Sarjat[kilp.Sarja()].luonne[pv] >= 0 && kilp.tark(pv) != L'P') {
						nlasna[pv]++;
						if (nj == 0)
							nkilp++;
						nj++;
						}
					}
				if (nj == (npv = vaiheLkm(kilp.Sarja())))
					mks = Sarjat[kilp.Sarja()].maksu;
				else {
					if (Sarjat[kilp.Sarja()].maksu1)
						mks = nj * Sarjat[kilp.Sarja()].maksu1;
					else if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S')
						mks = nj * Sarjat[kilp.Sarja()].maksu;
					else
						mks = nj * Sarjat[kilp.Sarja()].maksu/npv;
					}
				if (kilp.ilmlista >= 900) {
					if (kilp.ilmlista >= 950)
						mks += jikorotus*mks/100;
					else
						mks = 0;
					}
				ms += mks;
				mstot += mks;
				ok = !nextix(&ind,key,&d);

				} while (ok && (!memcmp(key, keyv, nk)));

			initline();
			if (nkilp) {
				swprintf(line, L"   %-30.30s %-10.10s %3.3s %4d %5d,%2.2d",
					seura, seuralyh, maa, nkilp, ms/100, ms%100);
				for	(int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
					swprintf(line+wcslen(line), L" %4d", nlasna[ipv]);
				wcscat(line, L"\n");
				sendln(tulprm.lstf, line);
				}
			l += 1;

			if (tulprm.printer != 1 && !tulprm.mahtuusivulle(l, 2)) {
				endpage(&tulprm);
				initpage(&tulprm, L"C");
				l = 0;
				}

			}

		initline(&tulprm);
		swprintf(line, L"   %-30.30s                     %5d,%2.2d\n",
				L"Yhteensä", mstot/100, mstot%100);
		sendln(tulprm.lstf, line);
		poistaindeksi(&ind);
		}
	else if (yvlaji == L'O')
		ooyhteenveto(&tulprm);
	else if (yvlaji == L'E')
		emityhteenveto(&tulprm);
	else
		tulosyhteenveto(&tulprm);
	if (llhtml) sendln(tulprm.lstf, L"</pre>\n");
	if (llhtml) sendln(tulprm.lstf, L"</body>\n</html>\n");
	if (l > 0) endpage(&tulprm);
	enddoc(&tulprm);
	if (tulprm.lstf) {
		if (tulprm.printer == GDIPRINTER)
			LeaveCriticalSection(&print_CriticalSection);
		closeprfile(tulprm.lstf);
		tulprm.lstf = 0;
		}
}

