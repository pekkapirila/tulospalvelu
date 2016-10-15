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

#include "VDeclare.h"
#include "VMuotoilu.h"

#ifndef _CONSOLE
#include "UnitMain.h"
#include "UnitMsgs.h"
#include "TulostusUnit2.h"
#include "UnitJakelu.h"
#endif

#define K_TIED
#define SEURALISTA 15
#ifdef EI_OLE
int yhttl = 0;

#define SEURATUL

void taustatulostus(void) {}

void yhteenveto(tulostusparamtp *tulprm);
INT srapiiri(kilptietue *kilp);
int sarjayhdistelma(void);
INT avaa_tiedosto(tulostusparamtp *tulprm);
void sulje_tiedosto(TextFl **tul_tied);
extern wchar_t FISMaarFileName[200];

static   char av = '2';                // ?????

#define OPTALISRJ 0x01000000
#endif // EI_OLE

void uusiKilpnoIx(void);
int xmlots(tulostusparamtp *tulprm);
int xmlloppu(tulostusparamtp *tulprm);
int xmlsrjots(int sarja, tulostusparamtp *tulprm);
int xmlsrjloppu(tulostusparamtp *tulprm);
void xmltulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm);
void liittotulos(kilptietue *kilp, int srj, tulostusparamtp *tulprm);
int EmitHtmlSrjOts(int sarja, tulostusparamtp *tulprm);
int EmitHtmlSrjLoppu(tulostusparamtp *tulprm);
void EmitHtmlTulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm);
void EmitHtmlOsuus(kilptietue& kilp, int osuus, INT sj, tulostusparamtp *tulprm);
INT avaa_tiedosto_nm(wchar_t *fname, tulostusparamtp *tulprm);

lisatxttp lisatxt[4];

INT mrivi[3] = {8,8,8};
wchar_t palk[MAXSARJALUKU+MAXYHD];

char *fldname[3];
INT scale = 1, n_fld[3];
//INT printer = GDIPRINTER, tabs;
static INT lastpos;
tulostusparamtp tul_val_prm, af_prm;
//TextFl *tul_tied_val, *af_tied;
static wchar_t seuranimi[SEURALISTA][LSEURA+1];

UINT32 karkiaika;
int ssltlsfl = 1;
INT piirisijat;
UINT rivpit = 76;
INT lahetacr = 1;
INT potsfl = FALSE;
int tulera;
static wchar_t cssstr[] = L"<LINK REL=STYLESHEET HREF=\"tuloslue.css\">\n";
static INT tsakfl2 = FALSE;
static INT tsakfly = FALSE;
static INT tsakflm;
INT pitkaseuram;
static int TiedRivi;
wchar_t tul_tied_nimi[80] = L"TULOSLUE.1";

typedef struct {
	int d;
	int os;
	} osjarjtp;

//#ifdef SEURATUL
typedef struct {
   wchar_t sr[LSEURA+2];
   INT piiri;
   INT pos;
   } srixtype;

static srixtype *srix;
static wchar_t *sr_nimet;

//wchar_t seuranimi[SEURALISTA][LSEURA+1];
wchar_t *piirinimi[] = {
   L"Etelä-Pohjanmaa",
   L"Häme",
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

int piiriluku = sizeof(piirinimi)/sizeof(void_pointer);

INT piirifl[200];

#define PM_LUKU 8

/*
typedef struct {
#if defined(AMPUSUUNN)
   INT32 jtls;
   INT d[PM_LUKU];
#endif
	wchar_t *seura;
   } sraluettp;
*/

typedef wchar_t *sraluettp;

sraluettp  *seurat;

//#endif

tulostusparamtp::tulostusparamtp(void)
{
	nollaa();
}

tulostusparamtp::~tulostusparamtp(void)
{

}

void tulostusparamtp::operator=(tulostusparamtp& param)
{
	memcpy(this, &param, sizeof(tulostusparamtp));
}

void tulostusparamtp::nollaa(void)
{
	memset(this, 0, sizeof(tulostusparamtp));
	tulostettava = L'T';
	vaihelkm = 1;
	sj_raja = 3;
	enssija = 1;
	viimsija = 9999;
	erotin = L';';
	decsep = L',';
	p_fld = prtflds;
	tulmuot = tulosmuot;
	for (int i = 0; i < 9; i++)
		automaali[i] = 1;
}

static wchar_t *html_avaus(tulostusparamtp *tulprm, const wchar_t *wtitle, int laji)
{
	static wchar_t st[1000];
	static const wchar_t *ffstr[] = {L"Verdana,Arial,Sans-serif", L"Times,Serif", L"Monaco,'Lucida Console',Courier,Monospace"};
	static const wchar_t *fszstr[] = {L"small", L"medium", L"large", L"x-large", L"xx-large"};
	static INT bgred = 0, bggreen = 255, bgblue=191;
	int colW[] = {45, 200, 80, 80, 80};
	int colWh[] = {45, 60, 250, 200, 60, 80};
	int fsz = 30, fsz0 = 130;

	st[0] = 0;
	tulprm->writehtml(L"<!DOCTYPE html>\n<HTML>");
	if (tulprm->merkit == L'A')
		tulprm->writehtml(L"<HEAD><meta http-equiv=\"content-type\" content=\"text/html; charset=iso-8859-1\" />\n");
	else
		 tulprm->writehtml(L"<HEAD><meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />\n");
	if (tulprm->tulmuot.cssfl == 1)
		wcscpy(st, cssstr);
	else if (tulprm->tulmuot.cssfl == 0) {
		tulprm->writehtml(L"\n<style>\n");
		if (tulprm->tulmuot.tauluhtml) {
			tulprm->tulmuot.fontfamily = 0;
			swprintf(st,
				L"body {background-color: #%6.6x; color: #%6.6x; font-family:%s; font-size:%s;}\n"
				L"h2 {font-size: %d%%;}\n"
				L"h3 {font-size: %d%%;}\n"
				L"td, th {padding: %dpx %dpx %dpx %dpx;}\n",
				tulprm->tulmuot.bgcolor, tulprm->tulmuot.fontcolor,
				ffstr[tulprm->tulmuot.fontfamily], fszstr[tulprm->tulmuot.fontsize],
				tulprm->tulmuot.paaotskoko, tulprm->tulmuot.alaotskoko,
				tulprm->tulmuot.vpad, tulprm->tulmuot.hpad, tulprm->tulmuot.vpad, tulprm->tulmuot.hpad
				);
			tulprm->writehtml(st);
			if (tulprm->tulostettava == L'E') {
				fsz = tulprm->tulmuot.efontsize + fsz;
				swprintf(st,
					L"table {cellspacing: 0; border-collapse: collapse; }\n"
					L"table {font-size: %d%%;}\n"
					L"table, td, th {border: %dpx solid #%6.6x;}\n"
					L".aa {background-color: #%6.6x; color: #%6.6x;}\n"
					L".ab {background-color: #%6.6x; color: #%6.6x;}\n"
					L"th {background-color: #%6.6x; color: #%6.6x; font-weight: %s; }\n",
					tulprm->tulmuot.efontsize,
					tulprm->tulmuot.eborder, tulprm->tulmuot.bordercolor,
					tulprm->tulmuot.td1color, tulprm->tulmuot.td1fontcolor,
					tulprm->tulmuot.tdecolor, tulprm->tulmuot.tdefontcolor,
					tulprm->tulmuot.thecolor, tulprm->tulmuot.thefontcolor,
					tulprm->tulmuot.sarotsbold ? L"bold" : L"normal"
					);
				tulprm->writehtml(st);
				if (tulprm->tulmuot.tauluhtml & 4) {
					if (!tulprm->hajontatulokset) {
						swprintf(st,
							L".jkrva tr td:nth-child(1),\n"
							L".jkrva tr th:nth-child(1) {text-align: right; min-width: %dpx; }\n", colW[0]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(2),\n"
							L".jkrva tr th:nth-child(2) {text-align: left; min-width: %dpx; }\n", colW[1]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(3),\n"
							L".jkrva tr th:nth-child(3) {text-align: left; min-width: %dpx; }\n", colW[2]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(4),\n"
							L".jkrva tr th:nth-child(4) {text-align: right; min-width: %dpx; }\n", colW[3]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(n+5) {text-align: right; white-space: pre-wrap; min-width: %dpx; }\n", colW[4]*fsz/fsz0);
						tulprm->writehtml(st);
						}
					else {
						swprintf(st,
							L".jkrva tr td:nth-child(1),\n"
							L".jkrva tr th:nth-child(1) {text-align: right; min-width: %dpx; }\n", colWh[0]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(2),\n"
							L".jkrva tr th:nth-child(2) {text-align: right; min-width: %dpx; }\n", colWh[1]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(3),\n"
							L".jkrva tr th:nth-child(3) {text-align: left; min-width: %dpx; }\n", colWh[2]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(4),\n"
							L".jkrva tr th:nth-child(4) {text-align: left; min-width: %dpx; }\n", colWh[3]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(5),\n"
							L".jkrva tr th:nth-child(5) {text-align: left; min-width: %dpx; }\n", colWh[4]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr th:nth-child(n+6) {white-space: pre-wrap; min-width: %dpx; }\n", colWh[5]*fsz/fsz0);
						tulprm->writehtml(st);
						swprintf(st,
							L".jkrva tr td:nth-child(n+6) {text-align: right; white-space: pre-wrap; min-width: %dpx; }\n", colWh[5]*fsz/fsz0);
						tulprm->writehtml(st);
						}
					}
				}
			else {
				swprintf(st,
					L"table {%s cellspacing:0; border-collapse: collapse}\n"
					L"table, td, th {border:%dpx solid #%6.6x;}\n"
					L".aa {background-color: #%6.6x; color: #%6.6x;}\n"
					L".ab {background-color: #%6.6x; color: #%6.6x;}\n"
					L"th {background-color: #%6.6x; color: #%6.6x; font-weight: %s; }\n",
					((tulprm->tulmuot.tauluhtml & 3) == 2) ? L"width:100%;" : L"",
					tulprm->tulmuot.border, tulprm->tulmuot.bordercolor,
					tulprm->tulmuot.td1color, tulprm->tulmuot.td1fontcolor,
					tulprm->tulmuot.td2color, tulprm->tulmuot.td2fontcolor,
					tulprm->tulmuot.thcolor, tulprm->tulmuot.thfontcolor,
					tulprm->tulmuot.sarotsbold ? L"bold" : L"normal"
					);
				tulprm->writehtml(st);
				}
			swprintf(st,
				L".tulosots, .sarjalue {background-color: #%6.6x; color: #%6.6x;}\n"
				L".jkk {font-size:%d%%; padding: 10px 5px 0px 5px; background-color: #%6.6x; color: #%6.6x;}\n"
				L".jktiiv {font-size:%d%%;}\n"
				L".os {background-color: #%6.6x; color: #%6.6x;}\n"
				L".srjnm {font-size:%d%%;}\n"
				L".svots {font-size:%d%%;}\n"
				L"</style>\n",
				tulprm->tulmuot.framecolor, tulprm->tulmuot.framefontcolor,
				tulprm->tulmuot.alaotskoko,
				tulprm->tulmuot.tdjkcolor, tulprm->tulmuot.tdjkfontcolor,
				(tulprm->tulmuot.alaotskoko + 100)/2,
				tulprm->tulmuot.td1color, tulprm->tulmuot.td1fontcolor,
				tulprm->tulmuot.alaotskoko, tulprm->tulmuot.alaotskoko
				);
			tulprm->writehtml(st);
			}
		else {
			tulprm->tulmuot.fontfamily = 2;
			swprintf(st,
				L"body {font-family:%s; font-size:%s;}\n"
				L"h2 {font-family:%s; font-size:%d%%;}\n"
				L"h3 {font-family:%s; font-size:%d%%;}\n"
				L".sarjalinkit {font-family:%s;}\n"
				L".srjnm {font-size:120%%;}\n"
				L".svots {font-size:120%%;}\n"
				L".tulosots, .sarjalue {background-color: #%6.6x; color: #%6.6x;}\n"
				L"</style>\n",
				ffstr[tulprm->tulmuot.fontfamily], fszstr[tulprm->tulmuot.fontsize],
				tulprm->tulmuot.paaotskoko, tulprm->tulmuot.alaotskoko,
				ffstr[1], ffstr[1], ffstr[1],
				tulprm->tulmuot.framecolor, tulprm->tulmuot.framefontcolor);
			tulprm->writehtml(st);
			}
		}
	st[0] = 0;
	if (tulprm->title && tulprm->title[0]) {
		 wcscpy(st, L"<TITLE>");
		 wcscat(st, tulprm->title);
		 wcscat(st, L"</TITLE>\n");
		}
	wcscat(st, L"</HEAD>\n");
	tulprm->writehtml(st);
	return(0);
}

static void setflds(FldFrmtTp *fld, FldFrmtTp* fld0, int maxfldno, int n_fld0)
{
	memset(fld, 0, maxfldno*sizeof(FldFrmtTp));
	for (int i = 0; i < n_fld0; i++) {
		if (fld0[i].id > maxfldno)
			continue;
		fld[fld0[i].id].id = fld0[i].id;
		fld[fld0[i].id].pos = fld0[i].pos;
		fld[fld0[i].id].len = fld0[i].len;
		fld[fld0[i].id].oik = fld0[i].oik;
		fld[fld0[i].id].akt = fld0[i].akt;
		wcscpy(fld[fld0[i].id].name, fld0[i].name);
		wcscpy(fld[fld0[i].id].ots, fld0[i].ots);
	}
}

static void hae_kenttajarj(tulostusparamtp *tulprm, FldFrmtTp *flds, int *jarj, int *njrj)
{
	int n = 0, fldarr[MAXFLD+1][2];

	memset(fldarr, 0, sizeof(fldarr));
	for (int i = 0; i < MAXFLD; i++) {
		if (i > 0 && flds[i].id == 0 && flds[i].name[0] == 0)
			continue;
		if (tulprm->tulostettava == L'H' && (
			flds[i].id == F_EDSJ ||
			flds[i].id == F_ERO ||
			flds[i].id == F_TARK ||
			flds[i].id == F_OSASJ ||
			flds[i].id == F_OSATLS ||
			flds[i].id == F_SEURSJ)
			)
			continue;
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

static void hae_kenttajarj_2(FldFrmtTp *flds, int (*jarj)[3], int *njrj, int rvlkm)
{
	int n = 0, nj[2] = {0,0}, fldarr[MAXFLD+1][2];

	memset(fldarr, 0, sizeof(fldarr));
	for (int i = 0; i < MAXFLD; i++) {
		jarj[i][0] = -1;
		jarj[i][1] = -1;
		jarj[i][2] = 0;
		if (i > 0 && flds[i].id == 0 && flds[i].name[0] == 0)
			continue;
		if (flds[i].len > 0) {
			fldarr[n][0] = flds[i].pos;
			fldarr[n][1] = i;
			n++;
			}
		}
	if (n) {
		qsortint(fldarr, n, 8, 0, 1);
		for (int i = 0; i < n; i++) {
			if (nj[0] < nj[1]-1)
				nj[0] = nj[1]-1;
			if (flds[fldarr[i][1]].akt & 2) {
				if (nj[0] < nj[1] && fldarr[i+1][0] == fldarr[i][0])
					nj[0] = nj[1];
				jarj[nj[0]++][0] = fldarr[i][1];
				}
			if (nj[1] < nj[0]-1)
				nj[1] = nj[0]-1;
			if (rvlkm > 1 && flds[fldarr[i][1]].akt & 1) {
				if (nj[1] < nj[0] && fldarr[i+1][0] == fldarr[i][0])
					nj[1] = nj[0];
				jarj[nj[1]++][1] = fldarr[i][1];
				}
			}
		*njrj = nj[0] > nj[1] ? nj[0] : nj[1];
		for (int i = 0; i < *njrj; i++) {
			int len = 0;
			if (jarj[i][0] >= 0)
				len = flds[jarj[i][0]].len;
			if (rvlkm > 1 && jarj[i][1] >= 0 && len < flds[jarj[i][1]].len)
				len = flds[jarj[i][1]].len;
			jarj[i][2] = len;
			}
		}
}

wchar_t *htmlSrjFName(tulostusparamtp *prm, wchar_t *otsFName, int srj)
	{
	static wchar_t fname[300];
	wchar_t *p, st[50];
	int i = 0;

	memset(fname, 0, sizeof(fname));
	wcscpy(fname, otsFName);
	for (p = fname+wcslen(fname)-1; p > fname && *p != L'.'; p--);
	if (wmemcmpU(p, L".HTM", 4) == 0)
		p[0] = 0;
	for (p = fname+wcslen(fname)-1; p > fname && *p == L'\\' && i < 2; p--) i++;
	if (p > fname)
		p[1] = 0;
	else
		fname[0] = 0;
	if (prm->viimos != L'K')
		swprintf(fname+wcslen(fname), L"_%s_%d_%d.html", Sarjat[srj].Sarjanimi(st), prm->osuus, prm->piste);
	else
		swprintf(fname+wcslen(fname), L"_%s.html", Sarjat[srj].Sarjanimi(st));
	return(fname);
	}

static INT saksum(kilptietue *kilp, INT os)
   {
   INT i, sak = 0;

	for (i=0; kilp->ostiet[os].ampsakot[i]; i++)
	  if (kilp->ostiet[os].ampsakot[i] > '0' && kilp->ostiet[os].ampsakot[i] <= '9')
		 sak += kilp->ostiet[os].ampsakot[i] - '0';
   return(sak);
   }

wchar_t *kilptietue::tarkstr(wchar_t *st, int os, bool yOsuus, int len, int lang)
{
	memset(st, 0, sizeof(st));
	if (yOsuus) {
		if (len == 1 && lang == 0) {
			st[0] = wtTark(os);
			}
		else {
			wcscpy(st, tTarkStr(os, lang));
			}
		}
	else {
		if (len == 1 && lang == 0) {
			st[0] = wTark(os, 1);
			}
		else {
			wcscpy(st, TarkStr(os, true, lang));
			}
		}
	return(st);
}

wchar_t *kilptietue::strval(wchar_t *st, int len, int knt, int os, int pst, int sj, int opt, int lang)
{
	memset(st, 0, len+1);
	switch (knt) {
		case F_SJ:
			if (osHyv(os)) {
				if (!(opt & 1) && sj)
					_itow(sj, st, 10);
				else if ((opt & 1) && Sija(os, pst))
					_itow(Sija(os, pst), st, 10);
				if ((opt & 2) && st[0] != 0)
					wcscat(st, L".");
				}
			else
				wcscpy(st, L" ");
			return(st);
		case F_KNO:
			_itow(KilpNo(true), st, 10);
			return(st);
		case F_NIMI:
			return(Nimi(st, len, os, (opt & 1) ? -1 : 1));
		case F_ARVO:
			return(Arvo(os, st));
		case F_OSSRA:
			return(OsSeura(os, st));
		case F_LISNO:
			if (ostiet[os].lisno)
				_itow(ostiet[os].lisno, st, 10);
			return(st);
		case F_BADGE:
			if (ostiet[os].badge[0])
				_itow(ostiet[os].badge[0], st, 10);
			return(st);
		case F_RATA:
			return(Hajonta(os, st));
		case F_SRA:
			return(Seura(st, len));
		case F_MAA:
			Maa(st);
			st[len] = 0;
			return(st);
		case F_JOUK:
			return(Joukkue(st, len));
		case F_JOUKNM:
			return(Joukknimi(st, len));
		case F_JOUKID:
			_itow(joukkid, st, 10);
			return(st);
		case F_TLS:
			if (opt & 8) {
				aikatowstr_cols_n(st, ostiet[os].ylahto, t0, 0, 8);
				return(st);
				}
			switch (opt&1) {
				case 0:
					if (Hyv(os) && Tulos(os, pst))
						aikatowstr_cols_n(st, Tulos(os, pst), 0, L',', kilpparam.laika2);
					else
						if (!pst && (opt&2))
							tarkstr(st, os, false, len, lang);
					break;
				case 1:
					if (osHyv(os) && osTulos(os, pst, false))
						aikatowstr_cols_n(st, osTulos(os, pst, false), 0, L',', kilpparam.laika2);
					else
						if (!pst && (opt&2))
							tarkstr(st, os, false, len, lang);
					break;
					}
			if ((opt&4) == 0)
				elimwzb1(st);
			return(st);
		case F_ERO:
			switch (opt & 7) {
				case 0:
					if (Hyv(os) && Tulos(os, pst) > opt / 8) {
						aikatowstr_cols_n(st+1, Tulos(os, pst) - opt/8, 0, L',', kilpparam.laika2);
						}
					break;
				case 1:
					if (osHyv(os) && osTulos(os, pst, false) > opt / 8) {
						aikatowstr_cols_n(st+1, osTulos(os, pst, false) - opt/8, 0, L',', kilpparam.laika2);
						}
					break;
				}
			if ((opt&4) == 0) {
				elimwzb1(st+1);
				st[0] = L'+';
				}
			return(st);
		case F_SAK:
			if (kilpparam.lsakmax > 0) {
				if (os) {
					switch (opt) {
						case 0:
							swprintf(st, L"(%s)", Ampsakot(os));
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
					swprintf(st, L"(%s)", Ampsakot(os));
				}
			if (ostiet[os].sakko) {
				 wchar_t *saktxt[MAXLANG][3] = {{L"sakko", L"hyvitys"}, {L"penalty", L"offset"}};
				 int k = (Sarjat[sarja].tsak[os] ? Sarjat[sarja].tsak[os] : 1) * SEK;
				 if (wcslen(st) > 0)
					wcscat(st, L", ");
				if (opt > 7)
					swprintf(st+wcslen(st), L"%s %d", saktxt[lang][ostiet[os].sakko > 0 ? 0 : 1], ostiet[os].sakko/k);
				else
					swprintf(st+wcslen(st), L"%d", ostiet[os].sakko/k);
				}
			return(st);
		case F_TARK:
			tarkstr(st, os, (opt & 2) != 0, len, lang);
			return(st);
		case F_OSATLS:
			if (osHyv(os) && osTulos(os, pst, false))
				aikatowstr_cols_n(st, osTulos(os, pst, false), 0, L',', kilpparam.laika2);
			if ((opt&4) == 0)
			elimwzb1(st);
			return(st);
		case F_OSASJ:
			if (osHyv(os) && osTulos(os, pst, false) && ostiet[os].ossija > 0)
				_itow(ostiet[os].ossija, st, 10);
			return(st);
		}
	return(st);
}

void autostatus(int kynnys, int korkein, wchar_t rajaus, int viimsija);

void autotulostus(void)
   {
   INT  l,sv,srj,korkein,k,i,os, osuus, piste=0;
//   char ch,ch2;
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
	  for (os=0; os < Sarjat[i].ntosuus; os++) {
		 k = max(k,prior[i][os]);
		 if( prior[i][os] > korkein ) {
			srj = i;
			osuus = os;
			korkein = prior[i][os];
			}
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
	  tulprm = autotulostusparam;
	  tulprm.osuus = osuus;
	  tulprm.piste = piste;
//	  prior[srj][osuus] = 0;
	  initpage(&tulprm, NULL);

	  tulostasarja(&srj,&tulprm, &l, &sv, TRUE);
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

static int ossijat(tulostusparamtp *tulprm, int srj, int osuus, osjarjtp *osjarj, int *hyltot, int *kesktot)
{
	int os, os1, sj = 0, sj1, i, nt = 0, yhd = 0;
	int p = -1, lj = -1, va, vamin, vamax;
	TULOS tls0,ost;
	kilptietue kilp1;
	typedef struct {
		TULOS ostls;
		int d;
		int os;
		} ossjtp;
	ossjtp *ossj, *ossj_0;
	int osluku;

	if (srj >= sarjaluku)
		return(0);

	if (hyltot)
		*hyltot = 0;
	if (kesktot)
		*kesktot = 0;
	if (osuus < 0) {
		if (tulprm->hajontatulokset)
			return(0);
		vamin = 0;
		vamax = Sarjat[srj].Vaikaluku();
		}
	else {
		vamin = tulprm->piste > kilpparam.valuku ? 0 : tulprm->piste;
		vamax = vamin;
		}
	i = (nilm[srj]+1)*sizeof(ossjtp);
	if (tulprm->hajontatulokset || tulprm->monios)
		i *= Sarjat[srj].osuusluku;
	ossj_0 = (ossjtp *) malloc(i);
	ossj = ossj_0;
	if (tulprm->viimos) {
		njoukkesk[srj] = 0;
		njoukhyl[srj] = 0;
		}
//	osluku = ((kilpparam.maxnosuus > 1 && tulprm->hajontatulokset) ||
//		(tulprm->tulostettava == L'H' && tulprm->monios)) ? 1 : Sarjat[srj].ntosuus;
	osluku = Sarjat[srj].ntosuus;
	for (os = 0; os < osluku; os++)
		{
		if (!tulprm->viimos && os != osuus)
			continue;
		if (tulprm->viimos == L'V' && os > 0)
			break;
		for (va = vamin; va <= vamax && va <= Sarjat[srj].valuku[os]; va++) {
			tls0 = 0;
			sj = 0;
			p = -1;
			lj = -1;
			while (1) {
				p = askellasarja(srj, os, 0, va, p, &lj);
				if (va || ((!tulprm->hksijafl || !os) && (!tulprm->viimos || os))) {
					if (lj != 0)
						break;
					}
				if (p <= 0)
					break;
				GETREC(&kilp1, p);
				if (!os && !va && tulprm->viimos) {
					if (kilp1.Tark(-1, 2) == 'H')
						njoukhyl[srj]++;
					if (kilp1.Tark(-1, 2) == 'K')
						njoukkesk[srj]++;
					if (hyltot || kesktot) {
						for (os1 = 0;  os1 < Sarjat[srj].ntosuus; os1++) {
							if (hyltot && kilp1.tTark(os1) == 'H') {
								++*hyltot;
								break;
								}
							if (kesktot && (kilp1.tTark(os1) == 'K' ||
								(os1 && kilp1.tTark(os1) == 'E'))) {
								++*kesktot;
								break;
								}
							}
						}
					}
				if ((!tulprm->hksijafl || !os) && lj != 0)
					continue;

				for (os1 = Sarjat[srj].aOsuus(os, 0); os1 < Sarjat[srj].aOsuus(os+1, 0); os1++) {
					if ((ost = osuustulos(&kilp1, os1, 0)) == 0)
						continue;
					ossj[nt].d = p;
					ossj[nt].os = os1;
					_swab(((char *)&ost)+2, (char *) (ossj+nt), 2);
					_swab((char *)&ost, ((char *) (ossj+nt))+2, 2);
					nt++;
					}
				}
			if (!tulprm->monios || (!tulprm->hajontatulokset && tulprm->tulostettava != L'H')) {
				tls0 = 0;
				quicksort((char *) ossj, nt, 4, sizeof(ossjtp));
				sj1 = 0;
				for (i = 0; i < nt; i++) {
					GETREC(&kilp1, ossj[i].d);
					if (ossj[i].ostls != tls0)
						sj1 = i+1;
					tls0 = ossj[i].ostls;
					kilp1.ostiet[ossj[i].os].ossija = sj1;
					PUTREC(&kilp1, ossj[i].d);
					if (osjarj) {
						osjarj->d = ossj[i].d;
						osjarj->os = ossj_0[i].os;
						osjarj++;
						}
					}
				nt = 0;
				}
			}
		}
	if (osjarj && tulprm->monios && (tulprm->hajontatulokset || tulprm->tulostettava == L'H')) {
		quicksort((char *) ossj_0, nt, 4, sizeof(ossjtp));
		for (i = 0; i < nt; i++) {
			osjarj->d = ossj_0[i].d;
			osjarj->os = ossj_0[i].os;
			osjarj++;
			}
		}
	free(ossj_0);
	if (osjarj)
		osjarj->d = -1;
	return(max(nt,sj));
}

void kaikkisijat(tulostusparamtp *tulprm, int srj)
	{
	EnterCriticalSection(&tall_CriticalSection);
	ossijat(tulprm, srj, -1, NULL, NULL, NULL);
	LeaveCriticalSection(&tall_CriticalSection);
	}

static void near set_pos(tulostusparamtp *tulprm, INT pos, INT fill, INT dots)
   {
	wchar_t st[30],*p;

	switch (tulprm->printer) {
	  case LJETII :
		 sendwline(L"\x1b&a",tulprm->lstf);
			pos = 10 * (pos + tulprm->tulmuot.numlev*fill);
//		 if (suhtfont)
//			pos += 4*tulprm->tulmuot.numlev*dots;
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

static void tulostusvirhe(wchar_t *msg)
{
#ifdef _DEBUG
	writeerror_w(msg, 0);
#endif
}

void putfld(tulostusparamtp *tulprm, wchar_t *str, INT pos, INT len, INT oik, INT r)
{
	INT v, o, ll, i, dots, tabsent;
	wchar_t tabst[2] = L"\x9", *p;
	wchar_t *str2;

	if (len == 0 || (wcslen(str) == 0 && (tulprm->kohde != L'H' || tulprm->tulmuot.tauluhtml < 2)))
		return;
	if (r) {
		putflddsp(str, pos, len, oik, r);
		return;
		}
	if (tulprm->kohde == L'H' && tulprm->tulmuot.tauluhtml) {
		if (tulprm->tulmuot.tauluhtml >= 2) {
			if (oik)
				wcscat(tulprm->sline, L"<td align=right>");
			else
				wcscat(tulprm->sline, L"<td align=left>");
			}
		wcscat(tulprm->sline, str);
		if (tulprm->tulmuot.tauluhtml >= 2)
			wcscat(tulprm->sline, L"</td>");
		return;
		}
	str2 = (wchar_t *) calloc(wcslen(str)+1, sizeof(wchar_t));
	wcscat(str2, str);
	str = str2;
	pos += tulprm->tulmuot.marg;
	ll = wcslen(str);
	ll = min(ll, len);
	if (ll <= 0) {
		free(str2);
		return;
		}
	if (ll == len)
		str[ll] = 0;

	// GDI-kirjoitin
	if (tulprm->printer == GDIPRINTER) {
		if (oik > 0)
			pos += tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth*len;
		tulprm->vpos = putwfldGDI(str, pos, oik, tulprm->vpos, tulprm->lstf);
		if (tulprm->vpos < 0)
			tulostusvirhe(L"GDI-virhe putfld");
		}

	// Postscript
	else if (tulprm->printer == POSTSCRIPT) {
		if (oik)
			pos += tulprm->tulmuot.numlev*len;
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

	// Laserjet ja Proprinter
		if (tulprm->printer == LJETII || tulprm->printer == PROPRIEMU) {
			dots = 0;
			if (oik && tulprm->printer == LJETII && tulprm->tulmuot.numlev > 2) {
				for (p = str; *p; p++) {
					if (*p == L'.' || *p == L',' || *p == L':' || *p == L' ')
						dots++;
					if (*p == L'+')
						dots--;
					}
				}
			set_pos(tulprm, pos, v - pos, dots);
			sendwline(str, tulprm->lstf);
			}

		else {

	// tekstiä tab-merkein
			if (tulprm->tulmuot.tabs) {
				tabsent = 0;
				for (i = 0; i < n_prtflds; i++) {
					if (pos <= lastpos)
						break;
					if (lastpos < prtflds[i].pos+tulprm->tulmuot.marg) {
						sendwline(tabst, tulprm->lstf);
						lastpos = prtflds[i].pos+tulprm->tulmuot.marg;
						tabsent = 1;
						}
					}
				for (i = 0; i < v - pos; i++)
					sendwline(L" ",tulprm->lstf);
				sendwline(str, tulprm->lstf);
				}
			else {

	// tekstiä
				if (pos < 0) {
					if (wcslen(tulprm->sline) < sizeof(tulprm->sline)/2 - 1) {
						if (ll > (int) (wcslen(tulprm->sline) - sizeof(tulprm->sline)/2 - 1))
							ll = wcslen(tulprm->sline) - sizeof(tulprm->sline)/2 - 1;
						wcsncpy(tulprm->sline+wcslen(tulprm->sline), str, ll);
						}
					}
				else
					wmemcpy(tulprm->sline+v, str, ll);
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
   else {
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
		if (endpageGDI(tulprm->lstf))
			tulostusvirhe(L"GDI-virhe endpage");
		tulprm->vpos = 0;
		}
	else
		enddoc(tulprm);
   }

void initline(tulostusparamtp *tulprm)
	{
	if (tulprm->printer != POSTSCRIPT && tulprm->printer != GDIPRINTER) {
		if (!tulprm->tulmuot.tabs && tulprm->kohde != L'R' &&
			!(tulprm->kohde == L'H' && tulprm->tulmuot.tauluhtml))
			wmemset(tulprm->sline, L' ', sizeof(tulprm->sline)/2-1);
		else
			memset(tulprm->sline, 0, sizeof(tulprm->sline));
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
				*++p = KB_CR;
			*++p = KB_LF;
			}
	  *++p = 0;
	  sendwline(tulprm->sline, tulprm->lstf);
	  }
   initline(tulprm);
	}

void tulostusparamtp::writehtml(wchar_t *rivi)
   {
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

INT tulostusparamtp::puts1(wchar_t *line)
   {
   switch (kohde) {
//      case L'P':
//		 return(addvaline(line));
	  case L'H':
		 writehtml(line);
		 break;
	  case L'I':
	  case L'J':
		 puts_f(line);
			break;
	  }
   return(0);
	}

void initgdiprinter(tulostusparamtp *tulprm)
	{
	tulprm->printer = GDIPRINTER;
	selectfontGDI(tulprm->lstf, &DefFnt);
	CurrFnt = DefFnt;
/*
	if (tulprm->tulmuot.ymarg < 1000 * 72 * tulprm->lstf->u.wp.top / tulprm->lstf->u.wp.yppi / tulprm->lstf->u.wp.yPage)
		tulprm->tulmuot.ymarg = 1000 * 72 * tulprm->lstf->u.wp.top / tulprm->lstf->u.wp.yppi / tulprm->lstf->u.wp.yPage;
	if (tulprm->tulmuot.amarg < 1000 * 72 * tulprm->lstf->u.wp.GDIparam.bottommargin / tulprm->lstf->u.wp.yppi / tulprm->lstf->u.wp.yPage)
		tulprm->tulmuot.amarg = 1000 * 72 * tulprm->lstf->u.wp.GDIparam.bottommargin / tulprm->lstf->u.wp.yppi / tulprm->lstf->u.wp.yPage;
*/
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
		return;
		}
	if (tulprm->printer > 0 && fnt[0])
		sendwchars((wchar_t *)(fnt+1), fnt[0], tulprm->lstf);
	}

void bold_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &BoldFnt);
		}
	}

void bold_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &CurrFnt);
		}
	}

void lihav_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		bold_on(tulprm);
		return;
		}
   if (tulprm->printer > 0 && lih_on[0])
	  sendwchars(lih_on+1, lih_on[0], tulprm->lstf);
	}

void lihav_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		bold_pois(tulprm);
		return;
		}
   if (tulprm->printer > 0 && lih_pois[0])
	  sendwchars(lih_pois+1, lih_pois[0], tulprm->lstf);
	}

void pien_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &SmallFnt);
		return;
		}
	if (tulprm->printer > 0 && pienet_on[0])
	  sendwchars(pienet_on+1, pienet_on[0], tulprm->lstf);
	}

void pien_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &CurrFnt);
		return;
		}
   if (tulprm->printer > 0 && pienet_pois[0])
	  sendwchars(pienet_pois+1, pienet_pois[0], tulprm->lstf);
	}

void paaots_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &TitleFnt);
		return;
		}
   if (tulprm->printer > 0 && pots_on[0])
		sendwchars(pots_on+1, pots_on[0], tulprm->lstf);
	}

void paaots_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &CurrFnt);
		return;
		}
   if (tulprm->printer > 0 && pots_pois[0])
	  sendwchars(pots_pois+1, pots_pois[0], tulprm->lstf);
	}

void aliots_on(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &HeaderFnt);
		return;
		}
	if (tulprm->printer > 0 && aots_on[0])
	  sendwchars(aots_on+1, aots_on[0], tulprm->lstf);
	}

void aliots_pois(tulostusparamtp *tulprm)
	{
	if (tulprm->printer == GDIPRINTER) {
		selectfontGDI(tulprm->lstf, &CurrFnt);
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
		if (tulprm->tulmuot.numlev == 1)
			tulprm->tulmuot.numlev = 12;
		startdocGDI(tulprm->lstf);
		if (fnt && fnt[0] == L'C') {
			selectfontGDI(tulprm->lstf, &CourFnt);
			CurrFnt = CourFnt;
			}
		else {
			selectfontGDI(tulprm->lstf, &DefFnt);
			CurrFnt = DefFnt;
			}
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
			if (tulprm->tulmuot.numlev == 1)
				tulprm->tulmuot.numlev = 12;
			startpageGDI(tulprm->lstf);
			if (fnt && fnt[0] == L'C') {
				selectfontGDI(tulprm->lstf, &CourFnt);
				CurrFnt = CourFnt;
				}
			else {
				selectfontGDI(tulprm->lstf, &DefFnt);
				CurrFnt = DefFnt;
				}
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

void tulostusparamtp::Lihav_on(void)
{
	lihav_on(this);
}

void tulostusparamtp::Lihav_pois(void)
{
	lihav_pois(this);
}

void tulostusparamtp::Pien_on(void)
{
	pien_on(this);
}

void tulostusparamtp::Pien_pois(void)
{
	pien_pois(this);
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

#ifdef LUENTA
static int tarkastettu(kilptietue *kilp, int osuus, int kaikki)
   {
   int os;

   if(!kaikki) {
	  return(kilp->ostiet[osuus].badge[0] != 0);
	  }
   else {
	  for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
		 if (kilp->ostiet[os].badge[0] == 0)
			return(0);
		 }
	  }
   return(1);
   }
#else
static int tarkastettu(kilptietue *kilp, int osuus, int kaikki)
   {
   int os, ltls = 0;

   if (kilp->tTulos(Sarjat[kilp->sarja].ntosuus-1, 0))
	  ltls = 1;
   if(!kaikki) {
	  return(!(wcswcind(kilp->wtTark(osuus), L"-I") >= 0 &&
		 (ltls || kilp->tTulos(osuus, 0) > 30)));
	  }
   else {
	  for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
		 if (wcswcind(kilp->wTark(os, 1), L"-I") >= 0 &&
			(ltls || kilp->Maali(os, 0) != TMAALI0))
			return(0);
		 }
	  }
   return(1);
   }
#endif

static char joukkuekh(kilptietue *kilp)
   {
   int os;

   if (stschind(kilp->Tark(0, 1), "ES") >= 0)
	  return('E');
   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
	  switch (kilp->Tark(os, 1)) {
		 case 'H' :
			return('H');
		 case 'K' :
		 case 'E' :
		 case 'S' :
			return('K');
		 }
	  }
   return(0);
   }

static wchar_t * osuusnimist(kilptietue *kilp, int osuus, wchar_t *ost, int lnm)
   {
   wchar_t *p1;
   wchar_t nm[MAXNOSUUS][OSNIMIL+1];
   int os, ll;
   long nmlent, nmlen[MAXNOSUUS];

   wmemset(ost, 0, lnm);
   if (Sarjat[kilp->sarja].nosuus[osuus] == 1) {
	   kilp->Nimi(ost, lnm, Sarjat[kilp->sarja].aosuus[osuus+1]);
	   return(ost);
	   }
   p1 = ost;
   nmlent = 0;
   for (os = 0; os < Sarjat[kilp->sarja].nosuus[osuus]; os++) {
	   kilp->Nimi(nm[os], OSNIMIL, os+Sarjat[kilp->sarja].aosuus[osuus] + 1);
	  nm[os][OSNIMIL] = 0;
	  nmlen[os] = wcslen(nm[os]);
	  nmlent += nmlen[os];
	  }
   for (os = 0; os < Sarjat[kilp->sarja].nosuus[osuus]; os++) {
	  if (nmlent > lnm) {
		 if (os < Sarjat[kilp->sarja].nosuus[osuus]-1) {
			ll = (int) ((nmlen[os] * lnm) / nmlent);
			nmlent -= nmlen[os];
			nmlen[os] = ll;
			lnm -= nmlen[os];
			}
		 else nmlen[os] = lnm;
		 }
	  wcsncpy(p1, nm[os], nmlen[os]);
	  p1 += wcslen(p1);
	  *(p1++) = L'/';
	  }
   p1[-1] = 0;
   return(ost);
   }

static wchar_t * osuustlsst(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, wchar_t *oas, int len)
   {
   wchar_t *p, as[20];
   int os;
//   int lnm, ll, j;
   unsigned long ostls;

   wmemset(oas, 0, len);
   if (Sarjat[kilp->sarja].nosuus[osuus] == 1) {
	   ostls = kilp->osTulos(Sarjat[kilp->sarja].aosuus[osuus+1],0,false);
	   if (ostls) {
		  aikatowstr_cols_n(oas, ostls, 0, 0, kilpparam.laika2);
		  elimwz(oas);
		  }
	   return(oas);
	   }
   p = oas;
   for (os = Sarjat[kilp->sarja].aosuus[osuus] + 1;
	  os <= Sarjat[kilp->sarja].aosuus[osuus+1]; os++) {
	  if (tulprm->tulmuot.aikapari != 2 && (os > 0 || Sarjat[kilp->sarja].nosuus[0] > 1)) {
		 ostls = kilp->osTulos(os,0,false);
		 if (ostls) {
			AIKATOWSTRS(as,ostls,0);
			as[kilpparam.laika2] = 0;
			elimwz(as);
			wcscat(p, as);
			}
		 else {
			wcscat(p, L"---");
			}
		 p += wcslen(p);
		 *(p++) = L'/';
		 }
	  }
   if (p > oas) p[-1] = 0;
   return(oas);
   }

/*
static INT naytatulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj, int l);
static INT csvtulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj);
static INT texttulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj);
static INT htmltulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj);
static INT prttulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj);
static int htmlkaikki_nt(kilptietue *kilp, tulostusparamtp *tulprm, int sj);
static int prtkaikki_nt(kilptietue *kilp, tulostusparamtp *tulprm, int sj);
*/
static INT naytatulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj, int l)
{
   wchar_t was[60]=L"",woas[60]=L"",weas[60]=L"",wst[120], wst2[10] = L" ", whksj[16] = L"";
   int j,oasl, va, aosuus;
//   int n_knt;
   static unsigned long tls;
   FldFrmtTp fld[MAXFLD+1];
	setflds(fld, dspflds, MAXFLD, n_dspflds);

//   if (kilpparam.maxnosuus > 1) {
//	   return (naytatulos_nt(kilp, tulprm, osuus, sj, l));
//	   }
	aosuus = Sarjat[kilp->sarja].aosuus[osuus+1];
	va = tulprm->piste;
   wmemset(woas, L' ', 15);
   oasl = kilpparam.laika2;

	if (tulprm->yhteislahdot) {
		AIKATOWSTRS(was, kilp->ostiet[aosuus].ylahto, t0);
		was[8] = 0;
		if (kilp->ostiet[aosuus].lahtolaji > 0)
			wcscpy(wst2, L" Y  ");
		if (kilp->ostiet[aosuus].ylahto != TMAALI0 && kilp->ostiet[aosuus].lahtolaji == 0)
			wcscpy(wst2, L" A  ");
		wst2[3] = kilp->wtTark(osuus);
		_itow(osuus+1, wst, 10);
		wcscat(wst, L".os");
		j = fld[F_SJ].len;
		if (j && (UINT32)j < wcslen(wst))
			j = wcslen(wst);
		putfld(tulprm, wst, fld[F_SJ].pos, j, 0, l);
		}
	else if (kierrokset && osuus == Sarjat[kilp->sarja].osuusluku - 1 && va == 0
		&& (tls = kilp->Tulos(osuus,va)) != 0 && kilp->osHyv(osuus)) {
		swprintf(was, L"%2d kierr.", kilpparam.n_os + 1 - tls / (24*TUNTI));
		AIKATOWSTRS(woas, tls%(24*TUNTI),0);
		woas[kilpparam.laika2] = 0;
		woas[oasl] = 0;
		elimwz(woas);
		_itow(sj, wst, 10);
		putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
		 }
	else {
		if (kilpparam.rogaining) {
			_itow(kilp->pisteet[0], woas, 10);
			}
		else {
			if((tls = kilp->tTulos(osuus, va)) != 0 && kilp->tHyv(osuus)) {
				if (Sarjat[kilp->sarja].maxnosuus == 1) {
					if (osuus > 0 && (kilp->Maali(osuus-1, va) !=
						TMAALI0 || kilp->ostiet[osuus].ylahto != TMAALI0)) {
						AIKATOWSTRS(woas, osuustulos(kilp,osuus,0),0);
						_itow(kilp->ostiet[osuus].ossija, whksj, 10);
						}
					woas[oasl] = 0;
					elimwz(woas);
					}
				else {
					osuustlsst(kilp, tulprm, osuus, woas, 12);
					}
				if (sj > 1) {
					AIKATOWSTRS(weas+1, tls-karkiaika,0);
					weas[0] = L'+';
					}
				weas[oasl+1] = 0;
				elimwzb1(weas+1);
				AIKATOWSTRS(was,tls,0);
				was[kilpparam.laika2] = 0;
				elimwzb1(was);
				kilp->settSija(osuus, va, sj);
				_itow(sj, wst, 10);
				if (tulprm->tulostettava == L'H')
					putfld(tulprm, whksj, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
				else
					putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
				if (osuus && fld[F_EDSJ].len && kilp->Maali(osuus-1, 0)) {
					_itow(kilp->Sija(osuus-1, 0), wst+1, 10);
					wst[0] = L'(';
					wcscat(wst, L")");
					putfld(tulprm, wst, fld[F_EDSJ].pos, fld[F_EDSJ].len, fld[F_EDSJ].oik, l);
					}
				}
			else {
				if (fld[F_TARK].len < 2) {
					was[0] = kilp->wtTark(osuus);
					was[1] = 0;
					}
				else {
					was[0] = 0;
					if (!kilp->tHyv(osuus))
						kilp->tarkstr(was, osuus, true, 10, tulprm->language);
					wmemset(was+wcslen(was), L' ', oasl - wcslen(was));
					}
				was[oasl] = 0;
				}
			}
		}
   _itow(kilp->KilpNo(true), wst, 10);
   putfld(tulprm, wst, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, l);
   kilp->Joukkue(wst, fld[F_JOUK].len);
   putfld(tulprm, wst, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, l);
   kilp->Maa(wst);
   putfld(tulprm, wst, fld[F_MAA].pos, fld[F_MAA].len, fld[F_MAA].oik, l);
   kilp->Hajonta(osuus, wst);
   putfld(tulprm, wst, fld[F_RATA].pos, fld[F_RATA].len, fld[F_RATA].oik, l);

//	  putfld(tulprm, was, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, l);

	if (tulprm->yhteislahdot) {
		putfld(tulprm, wst2, fld[F_TARK].pos, 4, fld[F_TARK].oik, l);
		}
	else {
		if (kilp->wtTark(osuus) == L'T') {
			wcscpy(wst2, L"T");
			putfld(tulprm, wst2, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, l);
			}
		}
	if (Sarjat[kilp->sarja].maxnosuus > 1)
		osuusnimist(kilp, osuus, wst, fld[F_NIMI].len);
	else
		kilp->Nimi(wst, fld[F_NIMI].len, osuus, tulprm->tulmuot.etusuku);
	putfld(tulprm, wst, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, l);
	putfld(tulprm, kilp->Arvo(osuus, wst), fld[F_ARVO].pos, fld[F_ARVO].len, fld[F_ARVO].oik, l);
	_itow(kilp->ostiet[osuus].lisno, wst, 10);
	putfld(tulprm, wst, fld[F_LISNO].pos, fld[F_LISNO].len, fld[F_LISNO].oik, 0);
	if (Sarjat[kilp->sarja].maxnosuus == 1) {
		kilp->Hajonta(osuus, wst);
		putfld(tulprm, wst, fld[F_RATA].pos, fld[F_RATA].len, fld[F_RATA].oik, l);
#ifdef LUENTA
		_itow(kilp->ostiet[osuus].badge[0], whksj, 10);
#endif
		}
	if (tulprm->tulostettava == L'H')
		putfld(tulprm, woas, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, l);
	else {
		putfld(tulprm, was, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, l);
		putfld(tulprm, weas, fld[F_ERO].pos, fld[F_ERO].len, fld[F_ERO].oik, l);
		putfld(tulprm, woas, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik, l);
		putfld(tulprm, whksj, fld[F_OSASJ].pos, fld[F_OSASJ].len, fld[F_OSASJ].oik, l);
		}
	  if (Sarjat[kilp->sarja].paikat[osuus]) {
		 memset(wst, 0, sizeof(wst));
		 wst[0] = L'(';
		 MbsToWcs(wst+1, kilp->ostiet[osuus].ampsakot, Sarjat[kilp->sarja].paikat[osuus]);
		 wst[1+Sarjat[kilp->sarja].paikat[osuus]] = L')';
		 putfld(tulprm, wst, fld[10].pos, fld[10].len, fld[10].oik, l);
		 }

   return(0);
}

static INT csvtulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
   wchar_t st[200], erotin[3] = L", ";
   int kntjrj[30], n_knt;
   int opt = 0;
   FldFrmtTp fld1[MAXFLD+1], *fld;
	setflds(fld1, fileflds, MAXFLD, n_fileflds);

   if (kilpparam.maxnosuus > 1) {
	   return (0);
	   }

	if (tulprm->kohde == L'R') {
		erotin[0] = tulprm->erotin;
		erotin[1] = 0;
		}
   initline(tulprm);
	memset(tulprm->sline, 0, sizeof(tulprm->sline));
	hae_kenttajarj(tulprm, fld1, kntjrj, &n_knt);

	for (int i_knt = 0; i_knt < n_knt; i_knt++) {

		fld = fld1 + kntjrj[i_knt];
		if (fld->len == 0)
			continue;
		memset(st, 0, sizeof(st));
		opt = 0;
		switch (kntjrj[i_knt]) {
			case F_NIMI :
				opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
				break;
			case F_ERO :
				opt = 8*karkiaika;
				opt |= 4;
				break;
			case F_TLS :
				if (tulprm->lahtoajat)
					opt = 8;
				else
					opt = 4;
				break;
			case F_OSATLS :
				opt = 4;
				break;
			}
		if (tulprm->kaikkiva && (kntjrj[i_knt] == F_OSATLS || kntjrj[i_knt] == F_OSASJ)) {
			for (int va = 0; va < Sarjat[kilp->sarja].valuku[Sarjat[kilp->sarja].yosuus[osuus]]; va++) {
				if (sizeof(tulprm->sline)/2 - wcslen(tulprm->sline) < 12)
					break;
				if (tulprm->kaikkisijat && kntjrj[i_knt] == F_OSASJ)
					swprintf(tulprm->sline+wcslen(tulprm->sline), L"%s%d", erotin,
						kilp->Sija(osuus, va+1));
				swprintf(tulprm->sline+wcslen(tulprm->sline), L"%s%s",  erotin,
					aikatowstr_cols_n(st, kilp->Tulos(osuus, va+1), 0, L',', kilpparam.laika2));
				if (tulprm->kaikkisijat && kntjrj[i_knt] == F_OSATLS)
					swprintf(tulprm->sline+wcslen(tulprm->sline), L"%s%d", erotin,
						kilp->Sija(osuus, va+1));
				}
			break;
			}
/*		if (!((tulprm->piilotatulos || Sarjat[kilp->sarja].piilotatulos) &&
			(kntjrj[i_knt] == F_TLS || kntjrj[i_knt] == F_ERO || kntjrj[i_knt] == F_OSATLS)) &&
			!((tulprm->piilotasijat || Sarjat[kilp->sarja].piilotatulos) &&
			(kntjrj[i_knt] == F_SJ ||kntjrj[i_knt] == F_OSASJ)))
*/
		kilp->strval(st, min(199, fld->len), kntjrj[i_knt], osuus, tulprm->piste, sj, opt, tulprm->language);
		if (tulprm->sline[0])
			wcscat(tulprm->sline, erotin);
		wcsncpy(tulprm->sline+wcslen(tulprm->sline), st,
			sizeof(tulprm->sline)/2 - wcslen(tulprm->sline) - 3);
		if (kntjrj[i_knt] == F_KNO && tulprm->viimos == L'K')
			swprintf(tulprm->sline+wcslen(tulprm->sline), L"%s%d", erotin, osuus+1);
		if (sizeof(tulprm->sline)/2 - wcslen(tulprm->sline) < 3)
			break;
		}
	if (tulprm->kohde == L'E')
		wcscat(tulprm->sline, L"; ");
   endline(tulprm, 0);
   return(0);
}

static INT textjtulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
   {
   wchar_t ln[300], oas[16],st[80], len = 0, stsj[6] = L"", stjk[60], stsk[20];

   if (kilpparam.maxnosuus > 1) {
		return(0);
	   }

   initline(tulprm);
   wmemset(oas, L' ',sizeof(oas)/2);
   if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus)) {
	  AIKATOWSTRS(oas,kilp->Tulos(osuus, 0, false),0);
	  _itow(sj, stsj, 10);
	  oas[kilpparam.laika2] = 0;
	  elimwzb1(oas);
//	  putfld(tulprm, st, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
	  }
   else {
		oas[0] = 0;
		if (!kilp->Hyv(osuus))
			kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
	  oas[kilpparam.laika2] = 0;
	  }
   kilp->Nimi(st, 50, osuus);
//   putfld(tulprm, st, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
//   _itow(kilp->KilpNo(true);, st, 10);
/*
   len = fld[F_KNO].len;
   if (tulprm->monios) {
	   swprintf(st, L"%d-%s", kilp->KilpNo(true);, wosuuskoodi(kilp->sarja, osuus, 0, 0));
	   if (len > 2 && len < 6)
		   len = 6;
	   }
   else
	   _itow(kilp->KilpNo(true);, st, 10);
   putfld(tulprm, st, fld[F_KNO].pos, len, fld[F_KNO].oik, 0);
*/
   kilp->Joukkue(stjk, 58);
   if (Sarjat[kilp->sarja].paikat[osuus]) {
	  memset(stsk, 0, sizeof(st));
	  stsk[0] = L'(';
	  kilp->Ampsakot(osuus, stsk+1);
	  wcscat(stsk, L")");
	  }
   swprintf(ln, L"%s. %s, %s, %s", stsj, stjk, st, oas);
   if (tulprm->kohde == L'E')
	  wcscat(ln, L"; ");
   putfld(tulprm, ln, 0, wcslen(ln)+1, 0, 0);
   endline(tulprm, 0);
   return(0);
   }

static INT texttulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
   wchar_t was[60]=L"",woas[60]=L"",weas[60]=L"",wst[120], wst2[10] = L" ", whksj[16] = L"";
   int j,oasl, va, l = 0, aosuus;
   static unsigned long tls;
   FldFrmtTp fld[MAXFLD+1];
	setflds(fld, fileflds, MAXFLD, n_fileflds);
//   int n_knt;
//   int kntjrj[30];

   if (tulprm->kohde == L'R') {
		csvtulos(kilp, tulprm, osuus, sj);
		return(0);
		}

   if (tulprm->kohde == L'E') {
		textjtulos(kilp, tulprm, osuus, sj);
		return(0);
		}

   if (kilpparam.maxnosuus > 1) {
//	   return (texttulos_nt(kilp, tulprm, osuus, sj));
	   }

	aosuus = Sarjat[kilp->sarja].aosuus[osuus+1];
	va = tulprm->piste;
	initline(tulprm);
   wmemset(woas, L' ', 15);
   oasl = kilpparam.laika2;

//   hae_kenttajarj(fileflds, kntjrj, &n_knt);


	if (tulprm->yhteislahdot) {
		AIKATOWSTRS(was, kilp->ostiet[aosuus].ylahto, t0);
		was[8] = 0;
		if (kilp->ostiet[aosuus].lahtolaji > 0)
			wcscpy(wst2, L" Y  ");
		if (kilp->ostiet[aosuus].ylahto != TMAALI0 && kilp->ostiet[aosuus].lahtolaji == 0)
			wcscpy(wst2, L" A  ");
		wst2[3] = kilp->wtTark(osuus);
		_itow(osuus+1, wst, 10);
		wcscat(wst, L".os");
		j = fld[F_SJ].len;
		if (j && (UINT32)j < wcslen(wst))
			j = wcslen(wst);
		putfld(tulprm, wst, fld[F_SJ].pos, j, 0, l);
		}
	else if (kierrokset && osuus == Sarjat[kilp->sarja].osuusluku - 1 && va == 0
		&& (tls = kilp->Tulos(osuus,va)) != 0 && kilp->osHyv(osuus)) {
		swprintf(was, L"%2d kierr.", kilpparam.n_os + 1 - tls / (24*TUNTI));
		AIKATOWSTRS(woas, tls%(24*TUNTI),0);
		woas[kilpparam.laika2] = 0;
		woas[oasl] = 0;
		elimwz(woas);
		_itow(sj, wst, 10);
		putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
		 }
	else {
		if (kilpparam.rogaining) {
			if(kilp->Maali(osuus, va) != TMAALI0 && kilp->osHyv(osuus))
				_itow(kilp->pisteet[0], woas, 10);
			}
		else {
			if((tls = kilp->tTulos(osuus, va)) != 0 && kilp->tHyv(osuus)) {
				if (Sarjat[kilp->sarja].maxnosuus == 1) {
					if (osuus > 0 && (kilp->Maali(osuus-1, va) !=
						TMAALI0 || kilp->ostiet[osuus].ylahto != TMAALI0)) {
						AIKATOWSTRS(woas, osuustulos(kilp,osuus,0),0);
						if (fld[F_OSASJ].len)
							_itow(kilp->ostiet[osuus].ossija, whksj, 10);
						}
					woas[oasl] = 0;
					elimwz(woas);
					}
				else {
					osuustlsst(kilp, tulprm, osuus, woas, 58);
					}
				if (sj > 1) {
					AIKATOWSTRS(weas+1, tls-karkiaika,0);
					weas[0] = L'+';
					}
				weas[oasl+1] = 0;
				elimwzb1(weas+1);
				AIKATOWSTRS(was,tls,0);
				was[kilpparam.laika2] = 0;
				elimwzb1(was);
				kilp->settSija(osuus, va, sj);
				_itow(sj, wst, 10);
				putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
				if (osuus && fld[F_EDSJ].len && kilp->tTulos(osuus-1, 0)) {
					_itow(kilp->Sija(osuus-1, 0), wst+1, 10);
					wst[0] = L'(';
					wcscat(wst, L")");
					putfld(tulprm, wst, fld[F_EDSJ].pos, fld[F_EDSJ].len, fld[F_EDSJ].oik, l);
					}
				}
			else {
				if (fld[F_TARK].len < 2) {
					was[0] = kilp->wtTark(osuus);
					was[1] = 0;
					}
				else {
					was[0] = 0;
					if (!kilp->tHyv(osuus))
						kilp->tarkstr(was, osuus, true, 10, tulprm->language);
					wmemset(was+wcslen(was), L' ', oasl - wcslen(was));
					}
				was[oasl] = 0;
				}
			}
		}
   _itow(kilp->KilpNo(true), wst, 10);
   putfld(tulprm, wst, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, l);
   kilp->Joukkue(wst, fld[F_JOUK].len);
   putfld(tulprm, wst, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, l);
   kilp->Joukknimi(wst, fld[F_JOUKNM].len);
   putfld(tulprm, wst, fld[F_JOUKNM].pos, fld[F_JOUKNM].len, fld[F_JOUKNM].oik, l);
   _itow(kilp->joukkid, wst, 10);
   putfld(tulprm, wst, fld[F_JOUKID].pos, fld[F_JOUKID].len, fld[F_JOUKID].oik, l);
   kilp->Maa(wst);
   putfld(tulprm, wst, fld[F_MAA].pos, fld[F_MAA].len, fld[F_MAA].oik, l);

//	  putfld(tulprm, was, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, l);

	if (tulprm->yhteislahdot) {
		putfld(tulprm, wst2, fld[F_TARK].pos, 4, fld[F_TARK].oik, l);
		}
	else {
		if (kilp->wtTark(osuus) == L'T') {
			wcscpy(wst2, L"T");
			putfld(tulprm, wst2, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, l);
			}
		}
	if (Sarjat[kilp->sarja].maxnosuus > 1)
		osuusnimist(kilp, osuus, wst, fld[F_NIMI].len);
	else {
#ifdef LUENTA
		_itow(kilp->ostiet[osuus].badge[0], whksj, 10);
#endif
		kilp->Nimi(wst, fld[F_NIMI].len, osuus, tulprm->tulmuot.etusuku);
		}
	putfld(tulprm, wst, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, l);
	putfld(tulprm, kilp->Arvo(osuus, wst), fld[F_ARVO].pos, fld[F_ARVO].len, fld[F_ARVO].oik, 0);
	_itow(kilp->ostiet[osuus].lisno, wst, 10);
	putfld(tulprm, wst, fld[F_LISNO].pos, fld[F_LISNO].len, fld[F_LISNO].oik, 0);
	putfld(tulprm, was, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, l);
	putfld(tulprm, weas, fld[F_ERO].pos, fld[F_ERO].len, fld[F_ERO].oik, l);
	putfld(tulprm, woas, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik, l);
	putfld(tulprm, whksj, fld[F_OSASJ].pos, fld[F_OSASJ].len, fld[F_OSASJ].oik, l);
	  if (Sarjat[kilp->sarja].paikat[osuus]) {
		 memset(wst, 0, sizeof(wst));
		 wst[0] = L'(';
		 MbsToWcs(wst+1, kilp->ostiet[osuus].ampsakot, Sarjat[kilp->sarja].paikat[osuus]);
		 wst[1+Sarjat[kilp->sarja].paikat[osuus]] = L')';
		 putfld(tulprm, wst, fld[10].pos, fld[10].len, fld[10].oik, l);
		 }
   endline(tulprm, 0);
   return(0);
}

static INT htmltulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
   wchar_t was[60]=L"",woas[60]=L"",weas[60]=L"",wst[120], wst2[10] = L" ", whksj[16] = L"";
   wchar_t wsj[16] = L"", wedsj[16] = L"";
   int oasl, va, l = 0, aosuus, sjlen;
   static unsigned long tls;
   int kntjrj[MAXFLD], n_knt = MAXFLD;
   FldFrmtTp fld[MAXFLD+1];
   FldFrmtTp *pfld;

	setflds(fld, fileflds, MAXFLD, n_fileflds);

   if (kilpparam.maxnosuus > 1) {
//	   return (htmltulos_nt(kilp, tulprm, osuus, sj));
	   }


	aosuus = Sarjat[kilp->sarja].aosuus[osuus+1];
	va = tulprm->piste;
   wmemset(woas, L' ', 15);
   oasl = kilpparam.laika2;

   hae_kenttajarj(tulprm, fld, kntjrj, &n_knt);

	initline(tulprm);
   if (tulprm->tulmuot.tauluhtml) {
		if ((tulprm->rivi + tulprm->tulmuot.riviryhma) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
			wcscpy(tulprm->sline, L"<tr class='aa'>");
		else
			wcscpy(tulprm->sline, L"<tr class='ab'>");
		}

	sjlen = fld[F_SJ].len;

	if (tulprm->lahtoajat) {
		AIKATOWSTRS(was, kilp->ostiet[osuus].ylahto, t0);
		was[8] = 0;
		}
	else if (tulprm->yhteislahdot) {
		AIKATOWSTRS(was, kilp->ostiet[aosuus].ylahto, t0);
		was[8] = 0;
		if (kilp->ostiet[aosuus].lahtolaji > 0)
			wcscpy(wst2, L" Y  ");
		if (kilp->ostiet[aosuus].ylahto != TMAALI0 && kilp->ostiet[aosuus].lahtolaji == 0)
			wcscpy(wst2, L" A  ");
		wst2[3] = kilp->wtTark(osuus);
		_itow(osuus+1, wsj, 10);
		wcscat(wsj, L".os");
		if (sjlen && (UINT32)sjlen < wcslen(wsj))
			sjlen = wcslen(wsj);
		}
	else if (kierrokset && osuus == Sarjat[kilp->sarja].osuusluku - 1 && va == 0
		&& (tls = kilp->Tulos(osuus,va)) != 0 && kilp->osHyv(osuus)) {
		swprintf(was, L"%2d kierr.", kilpparam.n_os + 1 - tls / (24*TUNTI));
		AIKATOWSTRS(woas, tls%(24*TUNTI),0);
		woas[kilpparam.laika2] = 0;
		woas[oasl] = 0;
		elimwz(woas);
		_itow(sj, wsj, 10);
		 }
	else if (kilpparam.rogaining) {
		 if(kilp->Maali(osuus, va) != TMAALI0 && kilp->osHyv(osuus))
			_itow(kilp->pisteet[0], woas, 10);
		 }
	else {
		if((tls = kilp->tTulos(osuus, va)) != 0 && kilp->tHyv(osuus)) {
			if (Sarjat[kilp->sarja].maxnosuus == 1) {
				if (osuus > 0 && (kilp->Maali(osuus-1, va) !=
					TMAALI0 || kilp->ostiet[osuus].ylahto != TMAALI0)) {
					AIKATOWSTRS(woas, osuustulos(kilp,osuus,0),0);
					if (fld[F_OSASJ].len)
						_itow(kilp->ostiet[osuus].ossija, whksj, 10);
					}
				woas[oasl] = 0;
				elimwz(woas);
				}
			else {
				osuustlsst(kilp, tulprm, osuus, woas, 58);
				}
			if (sj > 1) {
				AIKATOWSTRS(weas+1, tls-karkiaika,0);
				weas[0] = L'+';
				}
			weas[oasl+1] = 0;
			elimwzb1(weas+1);
			AIKATOWSTRS(was,tls,0);
			was[kilpparam.laika2] = 0;
			elimwzb1(was);
			if (Sarjat[kilp->sarja].maxnosuus == 1)
				kilp->settSija(osuus, va, sj);
			_itow(sj, wsj, 10);
			if (osuus && fld[F_EDSJ].len && kilp->tTulos(osuus-1, 0)) {
				_itow(kilp->Sija(osuus-1, 0), wedsj+1, 10);
				wedsj[0] = L'(';
				wcscat(wedsj, L")");
				}
			}
		else {
				{
				was[0] = 0;
				if (!kilp->tHyv(osuus))
					kilp->tarkstr(was, osuus, true, 10, tulprm->language);
				}
			was[oasl] = 0;
			}
		}

	for (int i_knt = 0; i_knt < n_knt; i_knt++) {
		int opt = 0;

		wst[0] = 0;
		opt = 0;
		pfld = fld + kntjrj[i_knt];
		switch (kntjrj[i_knt]) {
			case F_SJ :
				wcscpy(wst, wsj);
				break;
			case F_EDSJ :
				wcscpy(wst, wedsj);
				break;
			case F_TARK :
				if (tulprm->yhteislahdot) {
					putfld(tulprm, wst, fld[F_TARK].pos, 4, fld[F_TARK].oik, l);
					}
				else if (!tulprm->lahtoluettelo) {
					if (kilp->wtTark(osuus) == L'T') {
						wcscpy(wst, L"T");
						}
					}
				break;
			case F_NIMI :
				if (Sarjat[kilp->sarja].maxnosuus > 1)
					osuusnimist(kilp, osuus, wst, fld[F_NIMI].len);
				else {
					opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
					kilp->strval(wst, min(199, pfld->len), pfld->id, osuus, 0, 0, opt, tulprm->language);
					}
				break;
			case F_KNO :
			case F_JOUK :
			case F_JOUKNM :
			case F_JOUKID :
			case F_MAA :
			case F_ARVO:
			case F_LISNO:
			case F_BADGE:
			case F_RATA:
			case F_SAK:
			case F_OSSRA:
				wst[0] = 0;
				kilp->strval(wst, min(199, pfld->len), pfld->id, osuus, 0, 0, opt, tulprm->language);
				break;
			case F_TLS:
				wcscpy(wst, was);
				break;
			case F_ERO:
				wcscpy(wst, weas);
				break;
			case F_OSATLS:
				wcscpy(wst, woas);
				break;
			case F_OSASJ:
				wcscpy(wst, whksj);
				break;
			}
		putfld(tulprm, wst, pfld->pos, pfld->len, pfld->oik, l);
		}
	if (tulprm->tulmuot.tauluhtml)
		wcscat(tulprm->sline, L"</tr>");
	endline(tulprm, 0);
	return(0);
}

static INT prttulos(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
   wchar_t was[60]=L"",woas[60]=L"",weas[60]=L"",wst[120], wst2[10] = L" ", whksj[16] = L"";
   int j,oasl, va, l = 0, aosuus;
   static unsigned long tls;
   FldFrmtTp fld[MAXFLD+1];

   if (kilpparam.maxnosuus > 1) {
//	   return (prttulos_nt(kilp, tulprm, osuus, sj));
	   }

   setflds(fld, prtflds, MAXFLD, n_prtflds);

	aosuus = Sarjat[kilp->sarja].aosuus[osuus+1];
	va = tulprm->piste;

   wmemset(woas, L' ', 15);
   oasl = kilpparam.laika2;
   if (tulprm->lahtoajat) {
		AIKATOWSTRS(was, kilp->ostiet[osuus].ylahto, t0);
		was[8] = 0;
		}
	else if (tulprm->yhteislahdot) {
		AIKATOWSTRS(was, kilp->ostiet[aosuus].ylahto, t0);
		was[8] = 0;
		if (kilp->ostiet[aosuus].lahtolaji > 0)
			wcscpy(wst2, L" Y  ");
		if (kilp->ostiet[aosuus].ylahto != TMAALI0 && kilp->ostiet[aosuus].lahtolaji == 0)
			wcscpy(wst2, L" A  ");
		wst2[3] = kilp->wtTark(osuus);
		_itow(osuus+1, wst, 10);
		wcscat(wst, L".os");
		j = fld[F_SJ].len;
		if (j && (UINT32)j < wcslen(wst))
			j = wcslen(wst);
		putfld(tulprm, wst, fld[F_SJ].pos, j, 0, l);
		}
	else if (kierrokset && osuus == Sarjat[kilp->sarja].osuusluku - 1 && va == 0
		&& (tls = kilp->Tulos(osuus,va)) != 0 && kilp->osHyv(osuus)) {
		swprintf(was, L"%2d kierr.", kilpparam.n_os + 1 - tls / (24*TUNTI));
		AIKATOWSTRS(woas, tls%(24*TUNTI),0);
		woas[kilpparam.laika2] = 0;
		woas[oasl] = 0;
		elimwz(woas);
		_itow(sj, wst, 10);
		putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
		 }
	else {
		if (kilpparam.rogaining) {
			if(kilp->Maali(osuus, va) != TMAALI0 && kilp->osHyv(osuus))
				_itow(kilp->pisteet[0], woas, 10);
			}
		else {
			if((tls = kilp->tTulos(osuus, va)) != 0 && kilp->tHyv(osuus)) {
				if (Sarjat[kilp->sarja].maxnosuus == 1) {
					if (osuus > 0 && (kilp->Maali(osuus-1, va) !=
						TMAALI0 || kilp->ostiet[osuus].ylahto != TMAALI0)) {
						AIKATOWSTRS(woas, osuustulos(kilp,osuus,0),0);
						if (fld[F_OSASJ].len)
							_itow(kilp->ostiet[osuus].ossija, whksj, 10);
						}
					woas[oasl] = 0;
					elimwz(woas);
					}
				else {
					osuustlsst(kilp, tulprm, osuus, woas, 58);
					}
				if (sj > 1) {
					AIKATOWSTRS(weas+1, tls-karkiaika,0);
					weas[0] = L'+';
					}
				weas[oasl+1] = 0;
				elimwzb1(weas+1);
				AIKATOWSTRS(was,tls,0);
				was[kilpparam.laika2] = 0;
				elimwzb1(was);
				kilp->settSija(osuus, va, sj);
				_itow(sj, wst, 10);
				putfld(tulprm, wst, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
				if (osuus && fld[F_EDSJ].len && kilp->tTulos(osuus-1, 0)) {
					_itow(kilp->Sija(osuus-1, 0), wst+1, 10);
					wst[0] = L'(';
					wcscat(wst, L")");
					putfld(tulprm, wst, fld[F_EDSJ].pos, fld[F_EDSJ].len, fld[F_EDSJ].oik, l);
					}
				}
			else {
	/*			if (fld[F_TARK].len < 2) {
					was[0] = kilp->wTark(osuus, 1);
					was[1] = 0;
					}
				else
	*/
					{
					was[0] = 0;
					if (!kilp->tHyv(osuus))
						kilp->tarkstr(was, osuus, true, 10, tulprm->language);
					}
				was[oasl] = 0;
				}
			}
		}
   _itow(kilp->KilpNo(true), wst, 10);
   putfld(tulprm, wst, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, l);
   kilp->Joukkue(wst, fld[F_JOUK].len);
   putfld(tulprm, wst, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, l);
   kilp->Joukknimi(wst, fld[F_JOUKNM].len);
   putfld(tulprm, wst, fld[F_JOUKNM].pos, fld[F_JOUKNM].len, fld[F_JOUKNM].oik, l);
   _itow(kilp->joukkid, wst, 10);
   putfld(tulprm, wst, fld[F_JOUKID].pos, fld[F_JOUKID].len, fld[F_JOUKID].oik, l);
   kilp->Maa(wst);
   putfld(tulprm, wst, fld[F_MAA].pos, fld[F_MAA].len, fld[F_MAA].oik, l);

	if (tulprm->yhteislahdot) {
		putfld(tulprm, wst2, fld[F_TARK].pos, 4, fld[F_TARK].oik, l);
		}
	else if(tulprm->lahtoluettelo) {
		if (kilp->wtTark(osuus) == L'T') {
			wcscpy(wst2, L"T");
			putfld(tulprm, wst2, fld[F_TARK].pos, fld[F_TARK].len, fld[F_TARK].oik, l);
			}
		}
	if (Sarjat[kilp->sarja].maxnosuus > 1)
		osuusnimist(kilp, osuus, wst, fld[F_NIMI].len);
	else {
		for (int ifld = 0; ifld < n_prtflds; ifld++) {
			int opt = 0;
			wst[0] = 0;
			switch (ifld) {
				case F_NIMI:
					opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
				case F_ARVO:
				case F_LISNO:
				case F_BADGE:
				case F_RATA:
				case F_OSSRA:
					kilp->strval(wst, min(199, fld[ifld].len), ifld, osuus, 0, 0, opt, tulprm->language);
					if (wst[0])
						putfld(tulprm, wst, fld[ifld].pos, fld[ifld].len, fld[ifld].oik, l);
					break;
				}
			}
#ifdef LUENTA
		if (fld[F_BADGE].len == 0)
			_itow(kilp->ostiet[osuus].badge[0], whksj, 10);
#endif
		}
	putfld(tulprm, was, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, l);
	putfld(tulprm, weas, fld[F_ERO].pos, fld[F_ERO].len, fld[F_ERO].oik, l);
	putfld(tulprm, woas, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik, l);
	putfld(tulprm, whksj, fld[F_OSASJ].pos, fld[F_OSASJ].len, fld[F_OSASJ].oik, l);
	  if (Sarjat[kilp->sarja].paikat[osuus]) {
		 memset(wst, 0, sizeof(wst));
		 wst[0] = L'(';
		 MbsToWcs(wst+1, kilp->ostiet[osuus].ampsakot, Sarjat[kilp->sarja].paikat[osuus]);
		 wst[1+Sarjat[kilp->sarja].paikat[osuus]] = L')';
		 putfld(tulprm, wst, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, l);
		 }
   endline(tulprm, 0);
   return(0);
}

static void aikakentta_jk(wchar_t *as, wchar_t *stsj, kilptietue *kilp, tulostusparamtp *tulprm, int sj)
{
	int oluku;
	wchar_t oas[16];

	if (kilpparam.alalaji == L'D')
		oluku = kilp->pisteet[0];
	else
		oluku = Sarjat[kilp->sarja].osuusluku;
	if (tulprm->lahtoajat) {
		AIKATOWSTRS(as, kilp->ostiet[0].ylahto, t0);
		as[8] = 0;
		elimwz(as);
		}
	else if (!tulprm->piilotatulos && !tulprm->piilotasijat) {
		if(kilp->Maali(oluku-1, 0) != TMAALI0 && kilp->Hyv(oluku-1)) {
			int tls;

			tls = kilp->Tulos(oluku-1, 0);
			if (kierrokset) {
				if (tulprm->language > 0)
					swprintf(as, L"%d laps   ", kilpparam.n_os + 1 - tls / (24*TUNTI));
				else
					swprintf(as, L"%d kierr. ", kilpparam.n_os + 1 - tls / (24*TUNTI));
				AIKATOWSTRS(as+10, tls%(24*TUNTI),0);
				as[10+kilpparam.laika2] = 0;
				elimwz(as+10);
				}
			else {
				AIKATOWSTRS(as, tls, 0);
				as[kilpparam.laika2] = 0;
				elimwz(as);
				}
			if (kilpparam.rogaining) {
				wcscpy(oas, as);
				swprintf(as, L"%4d (%s)", kilp->pisteet[0], oas);
				}
			kilp->setSija(oluku-1, 0, sj);
			_itow(sj, stsj, 10);
			wcscat(stsj, L".");
#ifdef FIS
			if (kilp->joukkue < 2) {
				if (sj == 1) {
					fissija = 1;
					}
				else {
					if (tls != fistls)
						fissija++;
					}
				fistls = tls;
				if (fissija <= 30)
					fisp = wcpoint[fissija-1];
				}
#endif
			}
		else {
			as[0] = 0;
			if (!kilp->Hyv(oluku-1))
				wcscpy(as, kilp->TarkStr(-1, false, tulprm->language));
			}
		}
}

static int htmlkaikki_jkrivi(kilptietue *kilp, tulostusparamtp *tulprm, int sj,
	FldFrmtTp *fld, int (*kntjrj_2)[3], int n_knt)
{
	int oluku, mrg = 0, xpos = 0;;
	wchar_t as[26], st[80], stsj[20] = L"", *p, *class_str = L" ";
#ifdef FIS
	static long fistls, fissija;
	char *fisp = NULL;
	wchar_t *wcpoint[] = {
		L"200",L"160",L"120",L"100", L"90", L"80", L"72", L"64", L"58", L"52",
		L"48", L"44", L"40", L"36", L"32", L"30", L"28", L"26", L"24", L"22",
		L"20", L"18", L"16", L"14", L"12", L"10",  L"8",  L"6",  L"4",  L"2"};
#endif

	memset(as, 0, sizeof(as));

	if (kilpparam.alalaji == L'D')
		oluku = kilp->pisteet[0];
	else
		oluku = Sarjat[kilp->sarja].osuusluku;

	if (!tulprm->tiivis) {
		if (tulprm->viimos == L'K') {
			if (tulprm->tulmuot.tauluhtml) {
				if (tulprm->tiivis)
					class_str = L" class='jktiiv'";
				else
					class_str = L" class='jkk'";
				wcscat(tulprm->sline, L"<tr>");
				}
			else {
				putfld(tulprm, L"<b>", 0, 3, 0, 0);
				mrg = 3;
				}
			}
		else
			if (tulprm->tulmuot.tauluhtml)
				wcscpy(tulprm->sline, L"<tr>");
		}

	// aika- ja sijoituskentät
	aikakentta_jk(as, stsj, kilp, tulprm, sj);

	for (int ifld = 0; ifld < n_knt; ifld++) {
		int opt = 0;
		memset(st, 0, sizeof(st));
		switch (kntjrj_2[ifld][0]) {
			case F_SJ:
				wcscpy(st, stsj);
				break;
			case F_KNO:
				if (tulprm->tulmuot.knofl || tulprm->tulbadge)
					_itow(kilp->KilpNo(true), st, 10);
				break;
			case F_JOUK:
				kilp->Joukkue(st, fld[F_JOUK].len);
				break;
			case F_JOUKNM:
				kilp->Joukknimi(st, fld[F_JOUKNM].len);
				break;
			case F_JOUKID:
				_itow(kilp->joukkid, st, 10);
				break;
			case F_MAA:
				kilp->Maa(st);
				break;
			case F_OSATLS:
				if (kilpparam.alalaji == L'D') {
					if (tulprm->language > 0)
						swprintf(st, L"%d laps", oluku);
					else
						swprintf(st, L"%d kierrosta", oluku);
					}
				break;
			case F_TLS:
				wcscpy(st, as);
				break;
			case F_SAK:
				if (Sarjat[kilp->sarja].maxpaikat) {
					p = st;
					*(p++) = L'(';
					for (int osuus = 0; osuus < oluku; osuus++) {
						int sak;
						sak = 0;
						if (Sarjat[kilp->sarja].paikat[osuus] > 0) {
							for (char *pp = kilp->ostiet[osuus].ampsakot; *pp; pp++)
								if (*pp > L'0' && *pp <= L'9') sak += *pp - L'0';
							_itow(sak, p, 10);
							while (*p)
								p++;
							*(p++) = L'+';
							}
						}
					if (p[-1] == L'+')
						p--;
					*p = L')';
					}
				break;
			}
		if (kntjrj_2[ifld][0] < 0) {
			if (tulprm->tulmuot.tauluhtml)
				putcell(tulprm, L" ", 0, 1, 0, class_str, tulprm->tulmuot.tauluhtml);
			}
		else {
			putcell(tulprm, st, fld[kntjrj_2[ifld][0]].pos+mrg, fld[kntjrj_2[ifld][0]].len,
				fld[kntjrj_2[ifld][0]].oik, class_str, tulprm->tulmuot.tauluhtml);
			if (xpos < fld[kntjrj_2[ifld][0]].pos + mrg + fld[kntjrj_2[ifld][0]].len)
				xpos = fld[kntjrj_2[ifld][0]].pos + mrg + fld[kntjrj_2[ifld][0]].len;
			}
		}
	if (mrg == 3) {
		putfld(tulprm, L"</b>", xpos, 4, 0, 0);
		}
	if (tulprm->tulmuot.tauluhtml)
		wcscat(tulprm->sline, L"</tr>");
	endline(tulprm, 0);
	return(0);
}

static int htmlkaikkitiivis(kilptietue *kilp, tulostusparamtp *tulprm, int sj)
	{
	wchar_t as[26], st[80], stsj[20] = L"", st1[80], *p, *class_str = L" ";
	wchar_t buf[480];
	int oluku;
	int nr = 2;
	FldFrmtTp fld[MAXFLD+1];
	int kntjrj_2[MAXFLD][3], n_knt = MAXFLD;

	setflds(fld, filefldsk, MAXFLD, n_filefldsk);

	buf[0] = 0;
	if (tulprm->tulmuot.tiiv_rivit == 0) {
		hae_kenttajarj_2(fld, kntjrj_2, &n_knt, 1);
		memset(as, 0, sizeof(as));
		initline(tulprm);
		if (tulprm->tulmuot.tauluhtml) {
			int l = 0;
			wcscat(buf, L"<p><table>\n");
			wcscat(buf, L"<colgroup>");
			for (int i_knt = 0; i_knt < n_knt; i_knt++) {
				l += kntjrj_2[i_knt][2]+1;
				}
			for (int i_knt = 0; i_knt < n_knt; i_knt++) {
				int lp;
				if ((tulprm->tulmuot.tauluhtml & 3) == 2) {
					lp = (100*kntjrj_2[i_knt][2]+150)/l;
					swprintf(buf+wcslen(buf), L"<col width=\"%d%%\">", lp);
					}
				else {
					lp = tulprm->tulmuot.levkerr*tulprm->tulmuot.fontsize*(kntjrj_2[i_knt][2]+1);
					swprintf(buf+wcslen(buf), L"<col style=\"min-width: %dpx; \">", lp);
					}
				}
			wcscat(buf, L"</colgroup>");
			putfld(tulprm, buf, 0, 1000, 0, 0);
			endline(tulprm, 0);
			putfld(tulprm, L"<tr class='jkk'>", 0, 30, 0, 0);
			endline(tulprm, 0);
			}
		else if(tulprm->tulmuot.tiiv_fontit == 1) {
			putfld(tulprm, L"<b>", 0, 3, 0, 0);
			endline(tulprm, 0);
			}

		// Joukkuerivin sisältö

		htmlkaikki_jkrivi(kilp, tulprm, sj, fld, kntjrj_2, n_knt);

		if (tulprm->tulmuot.tauluhtml) {
			putfld(tulprm, L"</table>", 0, 20, 0, 0);
			}
		else {
			if(tulprm->tulmuot.tiiv_fontit == 0)
				putfld(tulprm, L"</b><br>", 0, 10, 0, 0);
			else
				putfld(tulprm, L"<br>", 0, 10, 0, 0);
			}
		endline(tulprm, 0);
		}

	if (tulprm->tulmuot.tiiv_rivit > 0) {

		aikakentta_jk(as, stsj, kilp, tulprm, sj);

		if(tulprm->tulmuot.tiiv_fontit == 1) {
			putfld(tulprm, L"<b>", 0, 3, 0, 0);
			endline(tulprm, 0);
			}
		if (stsj[0])
			putfld(tulprm, stsj, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
		if (!tulprm->piilotasijat)
			swprintf(buf, L"%s ", stsj);
		swprintf(buf+wcslen(buf), L"%d %s %s", kilp->KilpNo(true), kilp->Joukkue(st, fld[F_JOUK].len), kilp->Maa(st1));
		if (!tulprm->piilotatulos)
			swprintf(buf+wcslen(buf), L" %s", as);
		if (tulprm->tulmuot.tiiv_fontit == 1) {
			wcscat(buf, L"</b>");
			}
		putfld(tulprm, buf, 0, wcslen(buf), 0, 0);
		endline(tulprm, 0);
		buf[0] = 0;
		}

	if (tulprm->tulmuot.tiiv_fontit == 0) {
		initline(tulprm);
		putfld(tulprm, L"<font size=-1>", 0, 14, 0, 0);
		endline(tulprm, 0);
		}

	if (kilpparam.alalaji == L'D')
		oluku = kilp->pisteet[0];
	else
		oluku = Sarjat[kilp->sarja].osuusluku;

	for (int osuus = 0; osuus < oluku; osuus++) {
		if (osuus || tulprm->tulmuot.tiiv_rivit > 0) {
			if (buf[wcslen(buf)-1] == L' ')
				buf[wcslen(buf)-1] = 0;
			wcscat(buf, L", ");
			}
		else
			buf[0] = 0;
		if (tulprm->tulmuot.tiiv_osno)
			swprintf(buf+wcslen(buf), L"(%d) ", osuus+1);
		if (tulprm->tulmuot.tiiv_arvo > 0) {
			kilp->Arvo(osuus, buf+wcslen(buf));
			wcscat(buf, L" ");
			}
		kilp->Nimi(buf+wcslen(buf), OSNIMIL, osuus, tulprm->tulmuot.etusuku);
		if (!kilpparam.partio && !tulprm->piilotatulos) {
			wcscat(buf, L" ");
			if (st[0]) {
				wcscat(buf, st);
				wcscat(buf, L"-");
				}
			as[kilpparam.laika2] = 0;
			elimwbl(as);
			wcscat(buf, as);
			if (Sarjat[kilp->sarja].maxpaikat) {
				if (Sarjat[kilp->sarja].paikat[osuus]) {
					wcscat(buf, L" (");
					kilp->Ampsakot(osuus, buf+wcslen(buf));
					wcscat(buf, L")");
					}
				else if (Sarjat[kilp->sarja].maxpaikat) {
					wmemset(as, L' ', Sarjat[kilp->sarja].maxpaikat);
					as[Sarjat[kilp->sarja].maxpaikat] = 0;
					wcscat(buf, as);
					}
				}
			if (wcslen(buf) > (UINT32)tulprm->tulmuot.tiiv_len) {
				for (p = buf+tulprm->tulmuot.tiiv_len;
					p > buf && *p != L' '; p--) ;
				*p = 0;
				putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, 0, 0);
				putfld(tulprm, L"<br>", wcslen(buf), 4, 0, 0);
				endline(tulprm, 0);
				nr++;
				wmemmove(buf, p+1, wcslen(p+1)+1);
				}
			}
		endline(tulprm, 0);
		}      // for osuus
	if (buf[0]) {
		putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, 0, 0);
		endline(tulprm, 0);
		nr++;
		}
	if (tulprm->tulmuot.tiiv_fontit == 0) {
		initline(tulprm);
		putfld(tulprm, L"</font>", 0, 7, 0, 0);
		endline(tulprm, 0);
		}
	putfld(tulprm, L"<br>", 0, 7, 0, 0);
	endline(tulprm, 0);
	return(nr);
   }

static int htmlkaikki_osrivi(kilptietue *kilp, tulostusparamtp *tulprm, int sj,
	FldFrmtTp *fld, int (*kntjrj_2)[3], int n_knt, int osuus, int yosuus, int &khfl)
{
   wchar_t as[26], oas[16], st[80], stsj[20] = L"", st1[80], st2[20], *class_str = L" ";

	wmemset(oas, L' ',kilpparam.laika2);
	stsj[0] = 0;
	st1[0] = 0;
	st2[0] = 0;
	as[0] = 0;
	if (!tulprm->piilotatulos && fld[F_OSASJ].len) {
		st2[0] = L' ';
		st2[1] = 0;
		}
	if (kilpparam.alalaji == L'D') {
		if (kilp->Maali(0, 0) == TMAALI0)
		_itow(kilp->ostiet[osuus].badge[0], as, 10);
		}
	else {
		if (tulprm->tulbadge == 2) {
			as[0] = 0;
			}
		else if (tulprm->tulbadge == 1)
			_itow(kilp->ostiet[osuus].badge[0], as, 10);
		}
	if (tulprm->tulbadge == 0 && !tulprm->piilotatulos || !tulprm->piilotasijat)
		{
		if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus)) {
			if (!osuus || kilp->Maali(osuus-1, 0) != TMAALI0) {
				if (!tulprm->piilotatulos) {
					AIKATOWSTRS(oas,kilp->osTulos(osuus,0,false),0);
					oas[kilpparam.laika2] = 0;
					elimwz(oas);
					}
				if (!tulprm->piilotasijat && kilpparam.maxnosuus == 1 && fld[F_OSASJ].len) {
					_itow(kilp->osSija(osuus), st2, 10);
					}
				}
			if (!tulprm->piilotatulos && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
				AIKATOWSTRS(as, kilp->tTulos(yosuus,0),0);
				as[kilpparam.laika2] = 0;
				elimwz(as);
				}
			if (!tulprm->piilotasijat && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
				if (kilpparam.rogaining) {
					_itow(kilp->ostiet[osuus].sakko, stsj, 10);
					}
				else if (kilpparam.partio)
					stsj[0] = 0;
				else
				_itow(kilp->Sija(osuus, 0), stsj, 10);
				}
			}
		else {
			if (!tulprm->piilotatulos && !kilp->osHyv(osuus)) {
				kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
				khfl = yosuus + 2;
				}
			}
		if (!tulprm->piilotatulos && !kilp->tHyv(yosuus) && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
			kilp->tarkstr(as, yosuus, true, 10, tulprm->language);
			stsj[0] = 0;
			}
		}

	if (!tulprm->piilotatulos && Sarjat[kilp->sarja].maxpaikat) {
		wmemset(st1, L' ', Sarjat[kilp->sarja].maxpaikat+2);
		if (Sarjat[kilp->sarja].paikat[osuus]) {
			st1[0] = L'(';
			kilp->Ampsakot(osuus, st1+1);
			wcscat(st1, L")");
			}
		}

	for (int ifld = 0; ifld < n_knt; ifld++) {
		int opt = 0;
		st[0] = 0;
		switch (kntjrj_2[ifld][1]) {
			case F_OSOTS:
				if (tulprm->language == 0)
					swprintf(st, L"Os. %s", wosuuskoodi(kilp->sarja, osuus, 0, 0));
				else
					swprintf(st, L"Leg %s", wosuuskoodi(kilp->sarja, osuus, 0, 0));
				break;
			case F_TLS:
				wcscpy(st, as);
				break;
			case F_SAK:
				wcscpy(st, st1);
				break;
			case F_OSASJ :
				if (!tulprm->piilotatulos && (osuus || Sarjat[kilp->sarja].nosuus[0] > 1) && !kilpparam.rogaining)
					wcscpy(st, st2);
				break;
			case F_OSATLS :
				if (!tulprm->piilotatulos && (osuus || Sarjat[kilp->sarja].nosuus[0] > 1) && !kilpparam.rogaining)
					wcscpy(st, oas);
				break;
			case F_VSJ:
				wcscpy(st, stsj);
				break;
			case F_NIMI:
				opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
			case F_ARVO:
			case F_LISNO:
			case F_BADGE:
			case F_RATA:
			case F_OSSRA:
				kilp->strval(st, min(199, fld[kntjrj_2[ifld][1]].len), kntjrj_2[ifld][1], osuus,
					0, 0, opt, tulprm->language);
				break;
			}
		if (kntjrj_2[ifld][1] < 0) {
			if (tulprm->tulmuot.tauluhtml)
				putcell(tulprm, L" ", 0, 1, 0, class_str, tulprm->tulmuot.tauluhtml);
			}
		else
			putcell(tulprm, st, fld[kntjrj_2[ifld][1]].pos, fld[kntjrj_2[ifld][1]].len,
				fld[kntjrj_2[ifld][1]].oik, class_str, tulprm->tulmuot.tauluhtml);
		}
	endline(tulprm, 0);
	return(0);
}

static int htmlkaikki(kilptietue *kilp, tulostusparamtp *tulprm, int sj)
   {
   wchar_t as[26], stsj[20] = L"";
   int osuus,oluku;
   int nr = 2, khfl = 0;
   FldFrmtTp  fld[MAXFLD+1];
   int kntjrj_2[MAXFLD][3], n_knt = MAXFLD;

//   if (kilpparam.maxnosuus > 1) {
//	   return (htmlkaikki_nt(kilp, tulprm, sj));
//	   }

	if (tulprm->viimos == L'K' && tulprm->tiivis) {
		return(htmlkaikkitiivis(kilp, tulprm, sj));
		}

	setflds(fld, filefldsk, MAXFLD, n_filefldsk);
	if ((emitfl || sisaanluenta) && tulprm->tulbadge && kilp->Tark(0, 0) == 'E')
		return(0);

	hae_kenttajarj_2(fld, kntjrj_2, &n_knt, tulprm->viimos == L'K' ? 2 : 1);

	memset(as, 0, sizeof(as));
	initline(tulprm);

	// Joukkuerivin sisältö

	htmlkaikki_jkrivi(kilp, tulprm, sj, fld, kntjrj_2, n_knt);

	if (tulprm->viimos != L'K') {
		return(1);
		}

	// Osuusrivit

	if (kilpparam.alalaji == L'D')
		oluku = kilp->pisteet[0];
	else
		oluku = Sarjat[kilp->sarja].osuusluku;

	for (osuus = 0; osuus < oluku; osuus++) {
		int yosuus = Sarjat[kilp->sarja].yosuus[osuus];
		if (khfl == yosuus+1 || (!tulprm->piilotatulos && (kilp->Tark(osuus, 1) == 'S' ||
			kilp->Tark(osuus, 0) == 'E')))
			break;
		initline(tulprm);
		if (tulprm->tulmuot.tauluhtml) {
			wcscpy(tulprm->sline, L"<tr class='os'>");
			}
		htmlkaikki_osrivi(kilp, tulprm, sj, fld, kntjrj_2, n_knt, osuus, yosuus, khfl);
		nr++;

	   if (tulprm->tulmuot.tauluhtml) {
			wcscat(tulprm->sline, L"</tr>");
			endline(tulprm, 0);
			}
	   }      // for osuus
	return(nr);
}

void liittotulos(kilptietue *kilp, int srj, tulostusparamtp *tulprm)
{
#ifndef _CONSOLE
   static wchar_t vak = L'K', kilptunnus[6] = L"", laji[MAXOSUUSLUKU][2], stsrj[4], stmatka[MAXOSUUSLUKU][8];
   wchar_t ch, st1[80], st2[80], st3[20], st4[20], st[200], aika[16], msg[82];
   INT ika;
   INT32 t = -1;
   int laika_shl = 10;

	for (int os = -1; os < Sarjat[srj].osuusluku; os++) {
		wcscpy(st, TulosteForm->SHLkilpailu);
		wcscat(st, L";");
		wcscat(st, TulosteForm->SHLSarja[srj]);
		wcscat(st, L"-");
		wcscpy(aika, L"0,0");
		if (os == -1) {
			switch (kilp->Tark(-1, false)) {
				case 'S' :
				case 'K' :
					t = 0;
					wcscpy(st3, L"DNF");
					break;
				case 'H' :
					t = 0;
					wcscpy(st3, L"DSQ");
					break;
				case 'E' :
					t = 0;
					wcscpy(st3, L"DNS");
					break;
				case 'O' :
					t = 0;
					wcscpy(st3, L"LAP");
					break;
				case '-' :
				case 'T' :
					_itow(kilp->Sija(Sarjat[srj].osuusluku-1, 0), st3, 10);
					AIKATOWSTRS(aika, kilp->Tulos(Sarjat[srj].osuusluku-1, 0), 0);
					aika[laika_shl] = 0;
					break;
				}
			swprintf(st+wcslen(st), L"V;0.0; ;;%s;%s;%s;%s;%d;0;", kilp->Joukkue(st1, 78),
				kilp->Seura(st2, 78), aika, st3, kilp->KilpNo(true));
			}
		else {
			switch (kilp->Tark(os,false)) {
				case 'S' :
				case 'K' :
					t = 0;
					wcscpy(st3, L"DNF");
					break;
				case 'H' :
					t = 0;
					wcscpy(st3, L"DSQ");
					break;
				case 'E' :
					t = 0;
					wcscpy(st3, L"DNS");
					break;
				case 'O' :
					t = 0;
					wcscpy(st3, L"LAP");
					break;
				case '-' :
				case 'T' :
					_itow(kilp->osSija(os), st3, 10);
					AIKATOWSTRS(aika, kilp->osTulos(os, 0), 0);
					aika[laika_shl] = 0;
					break;
				}
			if (kilp->ostiet[os].lisno) {
				_itow(kilp->ostiet[os].lisno, st4, 10);
				}
			else
				wcscpy(st4, L"ULK");
			kilp->Nimi(st1, 78, os, -1);
			kilp->Seura(st2, 78);
			swprintf(st+wcslen(st), L"%d;%s;%s;%s;%s;%s;%s;%s;%d;%d;",
				os+1, TulosteForm->SHLMatka[srj][os], TulosteForm->SHLTyyli[srj][os], st4, st1, st2, aika, st3, kilp->KilpNo(true), os+1);
			}
		tulprm->writeline(st);
		}
#endif
}

static int prtkaikki(kilptietue *kilp, tulostusparamtp *tulprm, int sj)
   {
   wchar_t as[40],oas[40], st[120], st1[120], st2[20], *p;
   wchar_t buf[1000];
   int osuus,oluku, txtlen = 0;
   int nr = 2, khfl = 0;
   long tls;
   FldFrmtTp fld[MAXFLD+1];
   int sak;
   char *pp;
#ifdef FIS
	static long fistls, fissija;
	char *fisp = NULL;
	wchar_t *wcpoint[] = {
		L"200",L"160",L"120",L"100", L"90", L"80", L"72", L"64", L"58", L"52",
		L"48", L"44", L"40", L"36", L"32", L"30", L"28", L"26", L"24", L"22",
		L"20", L"18", L"16", L"14", L"12", L"10",  L"8",  L"6",  L"4",  L"2"};
#endif

//   if (kilpparam.maxnosuus > 1) {
//	   return (prtkaikki_nt(kilp, tulprm, sj));
//	   }

	setflds(fld, prtfldsk, MAXFLD, n_prtfldsk);
	if ((emitfl || sisaanluenta) && tulprm->tulbadge && kilp->Tark(0, 1) == 'E')
		return(0);

	memset(as, 0, sizeof(as));
	initline(tulprm);
	if (tulprm->viimos == L'K' &&
		(!tulprm->tiivis || tulprm->tulmuot.tiiv_fontit == 1)) {
		lihav_on(tulprm);
		}
	if (kilpparam.alalaji == L'D')
		oluku = kilp->pisteet[0];
	else
		oluku = Sarjat[kilp->sarja].osuusluku;
	if (tulprm->lahtoajat) {
		AIKATOWSTRS(as, kilp->ostiet[0].ylahto, t0);
		as[8] = 0;
		elimwz(as);
		}
	else if (!tulprm->piilotatulos && !tulprm->piilotasijat) {
		if(kilp->Maali(oluku-1, 0) != TMAALI0 && kilp->Hyv(oluku-1)) {
			tls = kilp->Tulos(oluku-1, 0);
			if (kierrokset) {
				if (tulprm->language > 0)
					swprintf(as, L"%d laps   ", kilpparam.n_os + 1 - tls / (24*TUNTI));
				else
					swprintf(as, L"%d kierr. ", kilpparam.n_os + 1 - tls / (24*TUNTI));
				AIKATOWSTRS(as+10, tls%(24*TUNTI),0);
				as[10+kilpparam.laika2] = 0;
				elimwz(as+10);
				}
			else {
				AIKATOWSTRS(as, tls, 0);
				as[kilpparam.laika2] = 0;
				elimwz(as);
				}
			if (kilpparam.rogaining) {
				wcscpy(oas, as);
				swprintf(as, L"%4d (%s)", kilp->pisteet[0], oas);
				}
			kilp->setSija(oluku-1, 0, sj);
			_itow(sj, st, 10);
			putfld(tulprm, st, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
#ifdef FIS
			if (kilp->joukkue < 2) {
				if (sj == 1) {
					fissija = 1;
					}
				else {
					if (tls != fistls)
						fissija++;
					}
				fistls = tls;
				if (fissija <= 30)
					fisp = wcpoint[fissija-1];
				}
#endif
			}
		else {
			as[0] = 0;
			if (!kilp->Hyv(oluku-1))
				wcscpy(as, kilp->TarkStr(-1, false, tulprm->language));
			}
		}
	if (tulprm->tiivis && tulprm->tulmuot.tiiv_rivit > 0) {
		swprintf(buf, L"%d %s %s,", kilp->KilpNo(true), kilp->Joukkue(st, fld[F_JOUK].len), kilp->Maa(st1));
		txtlen = textlengthGDI(tulprm->lstf, buf);
		putfld(tulprm, buf, 0, wcslen(buf), 0, 0);
		if (tulprm->tulmuot.tiiv_fontit == 1)
			lihav_pois(tulprm);
		buf[0] = 0;
		}
	else {
		if (tulprm->tulmuot.knofl
			|| tulprm->tulbadge
			) {
			_itow(kilp->KilpNo(true), st, 10);
			putfld(tulprm, st, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik,0);
			}
		kilp->Joukkue(st, fld[F_JOUK].len);
		putfld(tulprm, st, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik,0);
		kilp->Joukknimi(st, fld[F_JOUKNM].len);
		putfld(tulprm, st, fld[F_JOUKNM].pos, fld[F_JOUKNM].len, fld[F_JOUKNM].oik, 0);
		_itow(kilp->joukkid, st, 10);
		putfld(tulprm, st, fld[F_JOUKID].pos, fld[F_JOUKID].len, fld[F_JOUKID].oik, 0);
		kilp->Maa(st);
		putfld(tulprm, st, fld[F_MAA].pos, fld[F_MAA].len, fld[F_MAA].oik,0);
		if (kilpparam.alalaji == L'D') {
			if (tulprm->language > 0)
				swprintf(st, L"%d laps", oluku);
			else
				swprintf(st, L"%d kierrosta", oluku);
			putfld(tulprm, st, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik,0);
			}
		putfld(tulprm, as, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik,0);
//#ifdef FIS
//		if (fisp)
//			putfld(tulprm, fisp, tulprm->stdhtml ? 81 : prtflds[8].pos, wcslen(fisp), 0, 0);
//#else
		if (Sarjat[kilp->sarja].maxpaikat) {
			memset(st1, 0, sizeof(st1));
			p = st1;
			*(p++) = L'(';
			for (osuus = 0; osuus < oluku; osuus++) {
				sak = 0;
				if (Sarjat[kilp->sarja].paikat[osuus] > 0) {
					for (pp = kilp->ostiet[osuus].ampsakot; *pp; pp++)
					if (*pp > L'0' && *pp <= L'9') sak += *pp - L'0';
					_itow(sak, p, 10);
					while (*p)
						p++;
					*(p++) = L'+';
					}
				}
				if (p[-1] == L'+')
					p--;
				*p = L')';
			putfld(tulprm, st1, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik,0);
			}
//#endif
		endline(tulprm, 0);
		if (tulprm->viimos != L'K')
			return(1);
		if (!tulprm->tiivis || tulprm->tulmuot.tiiv_fontit == 1)
			lihav_pois(tulprm);
		}
	if (tulprm->tiivis && tulprm->tulmuot.tiiv_fontit == 0) {
		int h1, h2;
		h1 = tulprm->lstf->u.wp.GDIparam.Currentfont.size;
		pien_on(tulprm);
		h2 = tulprm->lstf->u.wp.GDIparam.Currentfont.size;
		tulprm->vpos += h1-h2;
		}
	for (osuus = 0; osuus < oluku; osuus++) {
		int yosuus = Sarjat[kilp->sarja].yosuus[osuus];

		if (khfl == yosuus+1 || (!tulprm->piilotatulos && (kilp->Tark(osuus, 1) == 'S' ||
			kilp->Tark(osuus, 0) == 'E')))
			break;
		initline(tulprm);
		memset(as, 0, sizeof(as));
		memset(oas, 0, sizeof(oas));
		if (!tulprm->tiivis) {
			if (tulprm->language > 0)
				swprintf(st, L"Leg %s", wosuuskoodi(kilp->sarja, osuus, 0, 0));
			else
				swprintf(st, L"Osuus %s", wosuuskoodi(kilp->sarja, osuus, 0, 0));
			putfld(tulprm, st, fld[F_OSOTS].pos, wcslen(st), fld[F_OSOTS].oik, 0);
			for (int ifld = 0; ifld < n_prtfldsk; ifld++) {
				int opt = 0;
				st[0] = 0;
				switch (ifld) {
					case F_NIMI:
						opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
					case F_ARVO:
					case F_LISNO:
					case F_BADGE:
					case F_RATA:
					case F_OSSRA:
						kilp->strval(st, min(199, fld[ifld].len), ifld, osuus, 0, 0, opt, tulprm->language);
						if (st[0])
							putfld(tulprm, st, fld[ifld].pos, fld[ifld].len, fld[ifld].oik, 0);
						break;
					}
				}
/*
			putfld(tulprm, kilp->Nimi(st, fld[F_NIMI].len, osuus, tulprm->tulmuot.etusuku),
				fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
			putfld(tulprm, kilp->Arvo(osuus, st), fld[F_ARVO].pos, fld[F_ARVO].len, fld[F_ARVO].oik, 0);
			_itow(kilp->ostiet[osuus].lisno, st, 10);
			putfld(tulprm, st, fld[F_LISNO].pos, fld[F_LISNO].len, fld[F_LISNO].oik, 0);
			kilp->Hajonta(osuus, st);
			putfld(tulprm, st, fld[F_RATA].pos, fld[F_RATA].len, fld[F_RATA].oik, 0);
*/
			}
		st[0] = 0;
		st1[0] = 0;
		if (fld[F_OSASJ].len) {
			st2[0] = L' ';
			st2[1] = 0;
			}
		else
			st2[0] = 0;
		if (kilpparam.alalaji == L'D') {
			if (kilp->Maali(0, 0) == TMAALI0)
			_itow(kilp->ostiet[osuus].badge[0], as, 10);
			}
		else {
			if (tulprm->tulbadge == 2) {
				as[0] = 0;
				}
			else if (tulprm->tulbadge == 1)
				_itow(kilp->ostiet[osuus].badge[0], as, 10);
			}
		if (tulprm->tulbadge == 0 && !tulprm->piilotatulos || !tulprm->piilotasijat)
			{
			if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus)) {
				if (!osuus || kilp->Maali(osuus-1, 0) != TMAALI0) {
					if (!tulprm->piilotatulos) {
						AIKATOWSTRS(oas,kilp->osTulos(osuus,0, false),0);
						oas[kilpparam.laika2] = 0;
						elimwz(oas);
						}
					if (!tulprm->piilotasijat && kilpparam.maxnosuus == 1 && fld[F_OSASJ].len) {
						_itow(kilp->osSija(osuus), st2, 10);
						}
					}
				if (!tulprm->piilotatulos && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
					AIKATOWSTRS(as, kilp->tTulos(yosuus,0),0);
					as[kilpparam.laika2] = 0;
					elimwz(as);
					}
				if (!tulprm->piilotasijat && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
					if (kilpparam.rogaining) {
						_itow(kilp->ostiet[osuus].sakko, st, 10);
						}
					else if (kilpparam.partio)
						st[0] = 0;
					else
					_itow(kilp->Sija(osuus, 0), st, 10);
					}
				}
			else {
				if (!tulprm->piilotatulos && !kilp->osHyv(osuus)) {
					kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
					khfl = yosuus+2;
					}
				}
			if (!tulprm->piilotatulos && !kilp->tHyv(yosuus) && osuus == Sarjat[kilp->sarja].aosuus[yosuus+1]) {
				kilp->tarkstr(as, yosuus, true, 10, tulprm->language);
				st[0] = 0;
				}
			}
		if (!tulprm->tiivis) {
			if (!tulprm->piilotatulos && Sarjat[kilp->sarja].maxpaikat) {
//				putfld(tulprm, oas, fld[F_SAK].pos, kilpparam.laika2, 1, 0);
				wmemset(st1, L' ', Sarjat[kilp->sarja].maxpaikat+2);
				if (Sarjat[kilp->sarja].paikat[osuus]) {
					st1[0] = L'(';
					kilp->Ampsakot(osuus, st1+1);
					wcscat(st1, L")");
					}
				if (st1[0])
					putfld(tulprm, st1, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
				st1[0] = 0;
				}
			if ((osuus || Sarjat[kilp->sarja].nosuus[0] > 1) && !kilpparam.rogaining) {
				putfld(tulprm, oas, fld[F_OSATLS].pos, fld[F_OSATLS].len, fld[F_OSATLS].oik, 0);
				if (st2[0])
					putfld(tulprm, st2, fld[F_OSASJ].pos, fld[F_OSASJ].len, fld[F_OSASJ].oik, 0);
				}
			if (kilpparam.rogaining && osuus)
				putfld(tulprm, oas, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
			else
				putfld(tulprm, as, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
			putfld(tulprm, st, fld[F_VSJ].pos, fld[F_VSJ].len, fld[F_VSJ].oik, 0);
			endline(tulprm, 0);
			nr++;
			}
		if (tulprm->tiivis) {
			int lisalen;

			if (osuus || tulprm->tulmuot.tiiv_rivit > 0) {
				if (txtlen == 0)
					txtlen = 30;
				wcscat(buf, L" ");
				}
			else {
				if (buf[0])
					endline(tulprm, 0);
				txtlen = 30;
				wcscpy(buf, L" ");
				}
			if (tulprm->tulmuot.tiiv_osno)
				swprintf(buf+wcslen(buf), L"(%d) ", osuus+1);
			if (tulprm->tulmuot.tiiv_arvo > 0) {
				kilp->Arvo(osuus, buf+wcslen(buf));
				wcscat(buf, L" ");
				}
			kilp->Nimi(buf+wcslen(buf), fld[F_NIMI].len, osuus, tulprm->tulmuot.etusuku);
			if (!kilpparam.partio && !tulprm->piilotatulos) {
				wcscat(buf, L" ");
				if (st[0]) {
					wcscat(buf, st);
					wcscat(buf, L"-");
					}
				as[kilpparam.laika2] = 0;
				elimwbl(as);
				wcscat(buf, as);
				if (Sarjat[kilp->sarja].maxpaikat) {
					if (Sarjat[kilp->sarja].paikat[osuus]) {
						wcscat(buf, L" (");
						kilp->Ampsakot(osuus, buf+wcslen(buf));
						wcscat(buf, L")");
						}
					else if (Sarjat[kilp->sarja].maxpaikat) {
						wmemset(as, L' ', Sarjat[kilp->sarja].maxpaikat);
						as[Sarjat[kilp->sarja].maxpaikat] = 0;
						wcscat(buf, as);
						}
					}
/*
				if (wcslen(buf) > (UINT32)tulprm->tulmuot.tiiv_len) {
					for (p = buf+tulprm->tulmuot.tiiv_len;
						p > buf && *p != L' '; p--) ;
					*p = 0;
					putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, 0, 0);
					endline(tulprm, 0);
					nr++;
					wcscpy(buf, p+1);
					}
*/
				}
			if (osuus < Sarjat[kilp->sarja].osuusluku - 1)
				wcscat(buf, L",");
			lisalen = textlengthGDI(tulprm->lstf, buf);
			if (txtlen + lisalen > 1000 - tulprm->tulmuot.marg - tulprm->tulmuot.omarg) {
				endline(tulprm, 0);
				nr = tulprm->vpos / 12;
				txtlen = 30;
				}
			putfld(tulprm, buf, txtlen, wcslen(buf), 0, 0);
			txtlen += lisalen;
			buf[0] = 0;
			}
		}      // for osuus
	if (tulprm->tiivis) {
		if (buf[0]) {
			putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, -1, 0);
			endline(tulprm, 0);
			nr = tulprm->vpos / 12;
//			nr++;
			}
		}
	if (tulprm->tiivis && tulprm->tulmuot.tiiv_fontit == 0)
		pien_pois(tulprm);
	newline(tulprm, 1);
	nr = tulprm->vpos / 12;
	return(nr);
   }

static void naytaosuus(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija, int l)
   {

	naytatulos(kilp, tulprm, osuus, sija, l);

/*
   wchar_t oas[16],st[30];
   FldFrmtTp *fld;

	fld = dspflds;
   wmemset(oas, L' ',sizeof(oas)/2);
   if(kilp->Maali(osuus, 0) != TMAALI0 && osuushyv(kilp, osuus) &&
	  (kilp->Maali(osuus-1, 0) != TMAALI0 ||
	  kilp->ostiet[osuus].ylahto != TMAALI0)) {
	  AIKATOWSTRS(oas,osuustulos(kilp,osuus, 0),0);
	  _itow(sija, st, 10);
	  oas[kilpparam.laika2] = 0;
	  elimwzb1(oas);
	  putfld(tulprm, st, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, l);
	  }
   else {
	  switch(kilp->wTark(osuus, 1)) {
#if defined(LANGUK)
		 case L'K' : wcscpy(oas, L"DNF       "); break;
		 case L'H' : wcscpy(oas, L"DQ        "); break;
		 case L'E' : wcscpy(oas, L"DNS       "); break;
		 case L'S' : wcscpy(oas, L"DNS       "); break;
#else
#if defined(LANGSV)
		 case L'K' : wcscpy(oas, L"Avbrytit  "); break;
		 case L'H' : wcscpy(oas, L"Diskv.    "); break;
		 case L'E' : wcscpy(oas, L"Ej start  "); break;
		 case L'S' : wcscpy(oas, L"Ej start  "); break;
#else
		 case L'K' : wcscpy(oas, L"Kesk.     "); break;
		 case L'H' : wcscpy(oas, L"Hyl.      "); break;
		 case L'E' : wcscpy(oas, L"Ei läht.  "); break;
		 case L'S' : wcscpy(oas, L"Sulj.     "); break;
#endif
#endif
		 default  : wmemset(oas, L' ', 10);
		 }
	  oas[kilpparam.laika2] = 0;
	  }
   kilp->Nimi(st, fld[F_NIMI].len, osuus);
   putfld(tulprm, st, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, l);
   _itow(kilp->KilpNo(true);, st, 10);
   putfld(tulprm, st, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, l);
   kilp->Joukkue(st, fld[F_JOUK].len);
   putfld(tulprm, st, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, l);
   putfld(tulprm, oas, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
   if ((ratavalpit < HAJONTA) && tulprm->monios)
	  putfld(tulprm, kilp->Hajonta(osuus)+ratavalpit, fld[F_OSATLS].pos,
		 HAJONTA-ratavalpit, 0, l);
   if (Sarjat[kilp->sarja].paikat[osuus]) {
	  memset(st, 0, sizeof(st));
	  st[0] = L'(';
	  kilp->Ampsakot(osuus, st+1);
	  wcscat(st, L")");
	  putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, l);
	  }
*/
}

static void htmlosuus(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija)
   {
   wchar_t oas[16],st[80], len = 0, stsj[10] = L"", *class_str = L" ";
	int kntjrj[MAXFLD], n_knt = MAXFLD;
   FldFrmtTp fld[MAXFLD+1];
   FldFrmtTp *pfld;


   if (kilpparam.maxnosuus > 1) {
//	   return(htmlosuus_nt(kilp, tulprm, osuus, sija));
	   }

   setflds(fld, fileflds, MAXFLD, n_fileflds);
   hae_kenttajarj(tulprm, fld, kntjrj, &n_knt);

   initline(tulprm);
   if (tulprm->tulmuot.tauluhtml) {
		if ((tulprm->rivi + tulprm->tulmuot.riviryhma) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
			wcscpy(tulprm->sline, L"<tr class='aa'>");
		else
			wcscpy(tulprm->sline, L"<tr class='ab'>");
		}
   wmemset(oas, L' ',sizeof(oas)/2);

   if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus) &&
	  (osuus == 0 || kilp->Maali(osuus-1, 0) != TMAALI0 ||
	  kilp->ostiet[osuus].ylahto != TMAALI0)) {
	  AIKATOWSTRS(oas,kilp->osTulos(osuus, 0, false),0);
	  if (!tulprm->hksijafl || tulprm->hajontatulokset || tulprm->piirisijat || sija == kilp->osSija(osuus))
		_itow(sija, stsj, 10);
	  oas[kilpparam.laika2] = 0;
	  elimwzb1(oas);
	  }
   else {
		oas[0] = 0;
		if (!kilp->Hyv(osuus))
			kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
	  oas[kilpparam.laika2] = 0;
	  }

	for (int i_knt = 0; i_knt < n_knt; i_knt++) {
		st[0] = 0;
		int opt = 0;
		pfld = fld + kntjrj[i_knt];
		switch (kntjrj[i_knt]) {
			case F_SJ :
				wcscpy(st, stsj);
				break;
			case F_KNO :
				_itow(kilp->KilpNo(true), st, 10);
				len = pfld->len;
				if (tulprm->monios) {
				   swprintf(st, L"%d-%s", kilp->KilpNo(true), wosuuskoodi(kilp->sarja, osuus, 0, 0));
				   if (len > 2 && len < 6)
					   len = 6;
				   }
				else
				   _itow(kilp->KilpNo(true), st, 10);
				break;
			case F_TLS :
				wcscpy(st, oas);
				break;
			case F_OSATLS :
				break;
			case F_NIMI :
				opt = tulprm->tulmuot.etusuku == -1 ? 1 : 0;
			case F_JOUK :
			case F_JOUKNM :
			case F_JOUKID :
			case F_ARVO:
			case F_LISNO:
			case F_BADGE:
			case F_SAK :
			case F_RATA:
			case F_OSSRA:
			   kilp->strval(st, min(199, pfld->len), pfld->id, osuus, 0, 0, opt, tulprm->language);
			   break;
			}
		if (st[0] || tulprm->tulmuot.tauluhtml)
			putcell(tulprm, st, pfld->pos, pfld->len, pfld->oik, class_str, tulprm->tulmuot.tauluhtml);
	  }
   if (tulprm->tulmuot.tauluhtml)
	  wcscat(tulprm->sline, L"</tr>");
   endline(tulprm, 0);
   }

static void textosuus(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija)
   {
   wchar_t ln[300], oas[16],st[80], len = 0, stsj[6] = L"", stjk[60], stsk[20];

   if (kilpparam.maxnosuus > 1) {
		return;
	   }

   initline(tulprm);
   wmemset(oas, L' ',sizeof(oas)/2);
   if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus) &&
	  (osuus == 0 || kilp->Maali(osuus-1, 0) != TMAALI0 ||
	  kilp->ostiet[osuus].ylahto != TMAALI0)) {
	  AIKATOWSTRS(oas,kilp->osTulos(osuus, 0, false),0);
	  _itow(kilp->osSija(osuus), stsj, 10);
	  oas[kilpparam.laika2] = 0;
	  elimwzb1(oas);
//	  putfld(tulprm, st, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
	  }
   else {
		oas[0] = 0;
		if (!kilp->Hyv(osuus))
			kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
	  oas[kilpparam.laika2] = 0;
	  }
   kilp->Nimi(st, 50, osuus);
//   putfld(tulprm, st, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
//   _itow(kilp->KilpNo(true);, st, 10);
/*
   len = fld[F_KNO].len;
   if (tulprm->monios) {
	   swprintf(st, L"%d-%s", kilp->KilpNo(true);, wosuuskoodi(kilp->sarja, osuus, 0, 0));
	   if (len > 2 && len < 6)
		   len = 6;
	   }
   else
	   _itow(kilp->KilpNo(true);, st, 10);
   putfld(tulprm, st, fld[F_KNO].pos, len, fld[F_KNO].oik, 0);
*/
   kilp->Joukkue(stjk, 58);
   if (Sarjat[kilp->sarja].paikat[osuus]) {
	  memset(stsk, 0, sizeof(st));
	  stsk[0] = L'(';
	  kilp->Ampsakot(osuus, stsk+1);
	  wcscat(stsk, L")");
	  }
   swprintf(ln, L"%s. %s, %s, %s", stsj, st, stjk, oas);
   if (tulprm->kohde == L'E')
	  wcscat(ln, L"; ");
   putfld(tulprm, ln, 0, wcslen(ln)+1, 0, 0);
   endline(tulprm, 0);
   }

static void prtosuus(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija)
   {
   wchar_t oas[16],st[80];
   FldFrmtTp fld[MAXFLD+1];

   if (kilpparam.maxnosuus > 1) {
//	   return(prtosuus_nt(kilp, tulprm, osuus, sija));
	   }

   setflds(fld, prtflds, MAXFLD, n_prtflds);

   wmemset(oas, L' ',sizeof(oas)/2);
   if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->osHyv(osuus) &&
	  (osuus == 0 || (kilp->Maali(osuus-1, 0) != TMAALI0 ||
	  kilp->ostiet[osuus].ylahto != TMAALI0))) {
	  AIKATOWSTRS(oas,kilp->osTulos(osuus, 0, false),0);
	  _itow(tulprm->monios ? sija : kilp->osSija(osuus), st, 10);
//	  _itow(kilp->osSija(osuus), st, 10);
	  oas[kilpparam.laika2] = 0;
	  elimwzb1(oas);
	  putfld(tulprm, st, fld[F_SJ].pos, fld[F_SJ].len, fld[F_SJ].oik, 0);
	  }
   else {
		oas[0] = 0;
		if (!kilp->Hyv(osuus))
			kilp->tarkstr(oas, osuus, false, 10, tulprm->language);
	  oas[kilpparam.laika2] = 0;
	  }
   kilp->Nimi(st, fld[F_NIMI].len, osuus);
   putfld(tulprm, st, fld[F_NIMI].pos, fld[F_NIMI].len, fld[F_NIMI].oik, 0);
   if (tulprm->monios) {
	   swprintf(st, L"%d-%d", kilp->KilpNo(true), osuus+1);
	   putfld(tulprm, st, fld[F_EDSJ].pos, fld[F_KNO].len+fld[F_EDSJ].len, fld[F_KNO].oik, 0);
	   }
   else {
	   _itow(kilp->KilpNo(true), st, 10);
	   putfld(tulprm, st, fld[F_KNO].pos, fld[F_KNO].len, fld[F_KNO].oik, 0);
	   }
   kilp->Joukkue(st, fld[F_JOUK].len);
   putfld(tulprm, st, fld[F_JOUK].pos, fld[F_JOUK].len, fld[F_JOUK].oik, 0);
   kilp->Joukknimi(st, fld[F_JOUKNM].len);
   putfld(tulprm, st, fld[F_JOUKNM].pos, fld[F_JOUKNM].len, fld[F_JOUKNM].oik, 0);
   _itow(kilp->joukkid, st, 10);
   putfld(tulprm, st, fld[F_JOUKID].pos, fld[F_JOUKID].len, fld[F_JOUKID].oik, 0);
   putfld(tulprm, oas, fld[F_TLS].pos, fld[F_TLS].len, fld[F_TLS].oik, 0);
   putfld(tulprm, kilp->Hajonta(osuus), fld[F_RATA].pos, fld[F_RATA].len, fld[F_RATA].oik, 0);
   if (Sarjat[kilp->sarja].paikat[osuus]) {
	  memset(st, 0, sizeof(st));
	  st[0] = L'(';
	  kilp->Ampsakot(osuus, st+1);
	  wcscat(st, L")");
	  putfld(tulprm, st, fld[F_SAK].pos, fld[F_SAK].len, fld[F_SAK].oik, 0);
	  }
   endline(tulprm, 0);
   }

//#else   // Tästä alkaa HALIKKO tai MKV

#ifdef EI_OLE

static void htmlosuus_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija);
static void prtosuus_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija);

static void prtosuus_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija)
{
}

static INT naytatulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj, int l)
{
	return(0);
}

static INT texttulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
	return(0);
}

static INT csvtulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, INT sj)
{
	return(0);
}

static void htmlosuus_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sija)
{

}

static int htmltulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sj)
   {
   return(0);
   }

static int prttulos_nt(kilptietue *kilp, tulostusparamtp *tulprm, int osuus, int sj)
   {
   wchar_t as[16],oas[50],st[30],ost[80];
   wchar_t was[20];
   int j, os, flisa = 0, va, l = 0;
   long lnm;
   static __int64 tls64;
   INT32 tls, tls1;
   FldFrmtTp *fld;
   wchar_t *txts[MAXLANG][10] = {{L"Hyl.", L"Kesk.", L"Ei läht.", L"Virhe", L"Puuttuu", L"Sakko", L"Tasoitus", L"Ohitettu", L"Poissa", L"Väliajat:"},
		{L"DQ", L"DNF", L"DNS", L"DQ", L"DNF", L"Penalty", L"Handicap", L"Skipped", L"Absent", L"Splits"}};

	fld = prtflds;
	va = tulprm->piste;
   if (Sarjat[kilp->sarja].nosuus[osuus] > 1) flisa = 3;
   memset(oas, 0,sizeof(oas));
   memset(ost, 0,sizeof(ost));
   lnm = fld[6+flisa].len - Sarjat[kilp->sarja].nosuus[osuus] + 1;

   tls64 = jtulos(kilp, osuus, va);
   tls = (INT32) tls64;

	if (va == -3) {
		AIKATOWSTRS(as, kilp->ostiet[osuus].ylahto, t0);
		as[8] = 0;
//		if (kilp->ostiet[osuus].lahtolaji > 0)
//			strcpy(st2, "Y");
//		else
//			strcpy(st2, "A");
		}
	else {
		if (tls64 < kilpparam.jtulosaskel) {
			if (tls >= 50L * TUNTI) tls = 0;
			osuusnimist(kilp, osuus, ost, lnm);
			osuustlsst(kilp, tulprm, osuus, oas);
			if (tulprm->tulmuot.aikapari == 2 && sj > 1) {
				tls1 = tls;
				if (tls1 > karkiaika) tls1 -= karkiaika;
				AIKATOWSTRS(oas, tls1,0);
				oas[kilpparam.laika2] = 0;
				elimwzb1(oas);
				}
			if (tulprm->tulmuot.aikapari == 3 && tls > karkiaika) tls -= karkiaika;
			wmemset(as, 0, kilpparam.laika2);
			if (!tls) tls = tulos(kilp,Sarjat[kilp->sarja].aosuus[osuus+1],0);
			if (tls)
				AIKATOWSTRS(as,tls,0);
			as[kilpparam.laika2] = 0;
			elimwzb1(as);
			if (tulprm->tulmuot.aikapari == 3 && sj > 1) {
				wmemmove(as+1, as, wcslen(as)+1);
				as[0] = L'+';
				}
			kilp->settSija(osuus, va, sj);
			_itow(sj, st, 10);
			putfld(tulprm, st, fld[0].pos, fld[0].len, fld[0].oik, l);
			if (osuus && fld[1].len) {
				_itow(kilp->Sija(Sarjat[kilp->sarja].aosuus[osuus], 0), st+1, 10);
				st[0] = L'(';
				wcscat(st, L")");
				putfld(tulprm, st, fld[1].pos, fld[1].len, fld[1].oik, l);
				}
			}
		else {
			if (tls64 < 2 * kilpparam.jtulosaskel) {
				wcscpy(was, txts[tulprm->language][1]);
				}
			else {
				if (tls64 < 3 * kilpparam.jtulosaskel) {
					wcscpy(was, txts[tulprm->language][0]);
					}
				else {
					if (tls64 < 4 * kilpparam.jtulosaskel) {
						wcscpy(was, txts[tulprm->language][2]);
						}
					else
						was[0] = 0;;
					}
				}
			was[kilpparam.laika2] = 0;
			}
		}
   _itow(kilp->KilpNo(true);, st, 10);
   putfld(tulprm, st, fld[2].pos, fld[2].len, fld[2].oik, l);
   kilp->Joukkue(st, fld[3].len);
   putfld(tulprm, st, fld[3].pos, fld[3].len, fld[3].oik, l);
   putfld(tulprm, was, fld[5].pos, fld[5].len, fld[5].oik, l);
	if (va != -3) {
		putfld(tulprm, ost, fld[6+flisa].pos, fld[6+flisa].len, fld[6+flisa].oik, l);
		putfld(tulprm, oas, fld[7+flisa].pos, fld[7+flisa].len, fld[7+flisa].oik, l);
		}
   endline(tulprm, 0);
   return(0);
   }

static int htmlkaikki_nt(kilptietue *kilp, tulostusparamtp *tulprm, int sj)
{
	return(0);
}

static int prtkaikki_nt(kilptietue *kilp, tulostusparamtp *tulprm, int sj)
   {
   wchar_t as[16],oas[16], st[80], st1[6], *p;
   wchar_t buf[480];
   int j,osuus,oluku,pos;
   int nr = 2;
   long ostls;
   FldFrmtTp *flds;

	flds = prtflds;
   oluku = Sarjat[kilp->sarja].osuusluku;
   if(kilp->Maali(oluku-1, 0) != TMAALI0 && !joukkuekh(kilp)) {
      AIKATOWSTRS(as,tulos(kilp,oluku-1,0),0);
      elimwz(as);
	  kilp->setSija(oluku-1, 0, sj);
      _itow(sj, st, 10);
      putfld(tulprm, st, flds[0].pos, 4, 1, 0);
      }
   else {
	  if (tulprm->language == 0) {
		  switch(kilp->wTark(-1, 1)) {
			 case L'K' : wcscpy(as, L"KESKEYTT. "); break;
			 case L'H' : wcscpy(as, L"HYLÄTTY   "); break;
			 case L'E' : wcscpy(as, L"EI LÄHT.  "); break;
			 default  : wmemset(as , L' ', 10);
						as[10] = 0;
			 }
		  }
	  else {
		  switch(kilp->wTark(-1, 1)) {
			 case L'K' : wcscpy(as, L"DNF       "); break;
			 case L'H' : wcscpy(as, L"DQ        "); break;
			 case L'E' : wcscpy(as, L"DNS       "); break;
			 default  : wmemset(as , L' ', 10);
						as[10] = 0;
			 }
		  }
	  }
   if (tulprm->tulmuot.knofl) {
	  pos = flds[2].pos;
      if (tulprm->printer == LJETII || tulprm->printer == POSTSCRIPT || tulprm->printer == GDIPRINTER) {
         pos = min(pos, flds[3].pos - 7*tulprm->tulmuot.numlev);
		 }
	  _itow(kilp->KilpNo(true);, st, 10);
      putfld(tulprm, st, pos, 4, 1, 0);
      }
   kilp->Joukkue(st, flds[3].len);
//   wcscat(st, kilp->maa);
   putfld(tulprm, st, flds[3].pos, 40, 0, 0);
   putfld(tulprm, as, flds[6].pos, kilpparam.laika2, 1, 0);
   endline(tulprm, 0);
	lihav_pois(tulprm);
	if (tulprm->viimos == L'V') 
		return(1);
   if (tulprm->tiivis) {
		pien_on(tulprm);
      buf[0] = 0;
      }
   for (osuus = 0; osuus < oluku; osuus++) {
	  if (stschind(kilp->Tark(osuus, 1), "ES") >= 0)
		  break;
	  initline(tulprm);
      wmemset(as, 0,kilpparam.laika2);
	  wmemset(oas, L' ',kilpparam.laika2);
	  if (!tulprm->tiivis) {
         if ( osuus == 0 ||
            Sarjat[kilp->sarja].aosuus[Sarjat[kilp->sarja].yosuus[osuus-1]+1] == osuus-1
			) {
			swprintf(st, L"%d. osuus", Sarjat[kilp->sarja].yosuus[osuus]+1);
            putfld(tulprm, st, flds[1].pos, 9, 1, 0);
			}
         putfld(tulprm, kilp->Nimi(st, OSNIMIL, osuus), flds[3].pos, OSNIMIL, 0, 0);
		 }
      memset(st, 0, sizeof(st));
      st1[0] = 0;
	  if(kilp->Maali(osuus, 0) != TMAALI0 && kilp->Hyv(osuus)) {
		 if ((ostls = kilp->osTulos(osuus,0, false)) != 0) {
            AIKATOWSTRS(oas, ostls,0);
			oas[kilpparam.laika2] = 0;
			elimwz(oas);
            }
         if (Sarjat[kilp->sarja].aosuus[Sarjat[kilp->sarja].yosuus[osuus]+1] == osuus) {
            AIKATOWSTRS(as,tulos(kilp,osuus,0),0);
            elimwz(as);
			_itow(kilp->Sija(osuus, 0), st, 10);
            }
         }
      else {
		 switch(kilp->Tark(osuus, 1)) {
			case 'K' : wcscpy(as, L"KESK.     "); break;
            case 'H' : wcscpy(as, L"HYL.      "); break;
            }
         }
	  if (!tulprm->tiivis) {
		 putfld(tulprm, oas, flds[5].pos, kilpparam.laika2, 1, 0);
		 if (st1[0])
            putfld(tulprm, st1, flds[5].pos+(kilpparam.laika2+2)*tulprm->tulmuot.numlev, 4, 1, 0);
         pos = flds[5].pos+(kilpparam.laika2+4)*tulprm->tulmuot.numlev;
         if (st1[0]) pos += 6*tulprm->tulmuot.numlev;
         putfld(tulprm, as, pos, kilpparam.laika2, 1, 0);
         pos += (kilpparam.laika2+3)*tulprm->tulmuot.numlev;
         putfld(tulprm, st, pos, 4, 1, 0);
         endline(tulprm, 0);
		 nr++;
         }
	  else {
		 if (osuus) wcscat(buf, L", ");
		 kilp->Nimi(buf+wcslen(buf), OSNIMIL, osuus);
         wcscat(buf, L" ");
         if (st[0]) {
            wcscat(buf, st);
            wcscat(buf, L"-");
			}
         as[kilpparam.laika2] = 0;
         elimwbl(as);
		 wcscat(buf, as);
		 if (wcslen(buf) > tulprm->tulmuot.tiiv_len) {
			for (p = buf+tulprm->tulmuot.tiiv_len;
			   p > buf && *p != L' '; p--) ;
            *p = 0;
			putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, 0, 0);
            endline(tulprm, 0);
            nr++;
			wcscpy(buf, p+1);
            }
         }
      }
   if (tulprm->tiivis) {
      if (buf[0]) {
		 putfld(tulprm, buf, 0, sizeof(tulprm->sline)/2-1, 0, 0);
         endline(tulprm, 0);
		 nr++;
		 }
	pien_pois(tulprm);
	  }
   newline(tulprm, 1);
   return(nr);
   }
#endif // EI_OLE


static int onko_piiri(int srj)
   {
   int piiri, retval = 0;
   int p = -1, lj = -1;
   kilptietue kilp;

	if (srj >= sarjaluku)
		return(0);
   EnterCriticalSection(&tall_CriticalSection);
   for (;;) {
	  p = askellasarja(srj, 0, 0, 0, p, &lj);
	  if (p <= 0)
		  break;
	  GETREC(&kilp, p);
	  piiri = kilp.piiri;
	  if (piiri <= 0 || piiri > piiriluku) continue;
	  if (piirifl[piiri-1]) {
		 retval = 1;
		 break;
		 }
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

static int onko_seura(int srj)
   {
   int i, retval = 0;
   wchar_t seura[LSEURA+1];
   kilptietue kilp;
   int p = -1, lj = -1;

	if (srj >= sarjaluku)
		return(0);
   seura[LSEURA] = 0;
   EnterCriticalSection(&tall_CriticalSection);
   for (;;) {
	  p = askellasarja(srj, 0, 0, 0, p, &lj);
	  if (p <= 0)
		  break;
	  GETREC(&kilp, p);
	  kilp.Seura(seura, LSEURA);
	  for (i = 0; seuranimi[i][0] && i < SEURALISTA; i++) {
		 if (wcscmp(seuranimi[i], upcasewstr(seura)) == 0) {
            retval = 1;
            break;
            }
		 }
	  if (retval)
         break;
      }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

static int onko_tarkastamatta(int srj, int os, int kaikki)
   {
   kilptietue kilp;
   int retval = 0;
   int p = -1, lj = -1;

   if (srj < sarjaluku && kaikki)
      os = 0;
   EnterCriticalSection(&tall_CriticalSection);
   for (;;) {
	  p = askellasarja(srj, 0, 0, 0, p, &lj);
	  if (p <= 0)
		  break;
#ifdef LUENTA
	  if (lj > 2)
		  break;
#else
	  if (lj > 0)
		  break;
#endif
      GETREC(&kilp, p);
      if (!tarkastettu(&kilp, os, kaikki)) {
         retval = 1;
         break;
		 }
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

static int onko_yhtlahto(int srj, int os)
   {
   kilptietue kilp;
   int retval = 0;
   int p = -1, lj = -1;

//   if (srj < sarjaluku)
//	  os = 0;
   EnterCriticalSection(&tall_CriticalSection);
   for (;;) {
	  p = askellasarja(srj, 0, 0, 0, p, &lj);
	  if (p <= 0)
		  break;
	  if (lj > 0)
		  break;
        GETREC(&kilp, p);
		if (kilp.ostiet[os].ylahto != TMAALI0) {
			retval = 1;
			break;
         }
      }
   LeaveCriticalSection(&tall_CriticalSection);
   return(retval);
   }

static char nayta_srj_os(int srj, int os, int jatko)
   {
   char ch = 0;

   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,(char *)Sarjat[srj].sarjanimi);
   if (os < Sarjat[srj].ntosuus)
	  vidint(ySize-3,12,2,os+1);
   else viwrrect(ySize-3,14,ySize-3,14,"*", 7,0,0);
   if (jatko) {
      vidspmsg(ySize-3,14,7,0,".os.  Paina <Return> jatkaaksesi tai"
         " <Esc> keskeyttääksesi");
	  sccurset(ySize-3,73);
	  do {
//         tarkcom();
		 ch = readkbd(&ch,TRUE,spoolfl);
		 } while (ch != KB_CR && ch != 27);
      }
   else {
      vidspmsg(ySize-3,14,7,0,".os.   Paina <Esc> keskeyttääksesi");
      sccurset(ySize-3,49);
      }
   return(ch);
   }

static int seur_kno(int kno, int srj, int osuus, int *yl, int *dkilp)
	{
	int n, os, aosuus;
	kilptietue kilp;

	aosuus = Sarjat[srj].aosuus[osuus+1];
	if (kno < minkilpno)
		kno = minkilpno - 1;
	for (n = kno + 1; n <= maxkilpno; n++) {
		if ((*dkilp = getpos(n)) <= 0)
			continue;
		if (srj >= 0 || yl)
			GETREC(&kilp, *dkilp);
		if (srj >= 0 && kilp.sarja != srj)
			continue;
		if (yl) {
			*yl = 0;
			if (aosuus >= 0) {
				if (kilp.ostiet[aosuus].ylahto != TMAALI0)
					++*yl;
				}
			else {
				for (os = 0; os < Sarjat[kilp.sarja].ntosuus; os++) {
					if (kilp.ostiet[Sarjat[srj].aosuus[os+1]].ylahto != TMAALI0)
						++*yl;
					}
				}
			if (*yl == 0)
				continue;
			}
		break;
		}
	if (n > maxkilpno) {
		*dkilp = 0;
		n = 0;
		}
	return(n);
	}

int srjtullkm(int srj, tulostusparamtp *tulprm)
//int piste, int day, int ixjarj, wchar_t ct, int np)
{
	int n, piste, osuus;

	n = nilm[srj];
	osuus = tulprm->osuus;
	piste = tulprm->piste;
   if (!tulprm->ixjarj) {
	  switch(tulprm->rajaus) {
		 case 'L' :
			n = ntulos[srj][osuus][0] + nhyl[osuus][srj] + nkesk[osuus][srj];
			break;
		 case 'T' :
			n = ntulos[srj][osuus][piste];
			break;
		 case 'P' :
			n = min(tulprm->viimsija ,ntulos[srj][osuus][tulprm->piste]);
			break;
		 case 'H' :
			n = nhyl[osuus][srj];
			break;
		 case 'K' :
			n = nkesk[osuus][srj];
			break;
		 case 'E' :
			n = neil[osuus][srj];
			break;
		 case 'A' :
			n = nilm[srj] - ntulos[srj][osuus][0] - nhyl[osuus][srj] - nkesk[osuus][srj]
				- neil[osuus][srj];
			break;
		 }
	  }
   else {
	  n = nameindex.nrec;
	  }
   return(n);
}

static int tlsSeuraava(tulostusparamtp *tulprm, int *d, int lisa, int srj, int *lj, int lj1,
	bool init, int *nkesktot, int *nhyltot, int *n_ostot, int *os)
{
	int valmis = 0;
	static char keyn[MAXINDL+1], keyv[MAXINDL+1];
	kilptietue kilp;
	static osjarjtp *osjarj = NULL, *osdk;
	static int n_osjarj, kilpno = 0, n_varaus;
	int ok = 1;

	if (lisa < 0) {
		if (osjarj)
			delete[] osjarj;
		osjarj = NULL;
		return(0);
		}
	if (lisa == 0)
		lisa = 1;
	if (init)
		memset(keyn, 0, MAXINDL);
	*os = 0;
	switch (tulprm->ixjarj) {
		case 0:
			if (init) {
				*d = -1;
				--*lj;
				}
			for (int i = 0; i < lisa; i++) {
				*d = askellasarja(srj, tulprm->osuus, 0, tulprm->piste, *d, lj);
				if ( *d < 0 || (*lj > lj1+1)) {
					*d = -1;
					valmis = 1;
					break;
					}
				}
			break;
		case 1:
		case 2:
		case 3:
			if (init)
				ok = !searchix(&nameindex, keyn, d);
			else
				ok = !nextix(&nameindex, keyn, d);

			while (ok) {
				kilp.getrec(*d);
				if (!tulprm->kaikkisarjat && kilp.sarja == srj)
					break;
				if (tulprm->kaikkisarjat && tulprm->sarjalista[kilp.sarja])
					break;
				ok = !nextix(&nameindex, keyn, d);
				}
			if (!ok) {
				valmis = 1;
				}
			break;
		case 4:
/*
			if (init) {
				_swab((char *)srj, keyn, 2);
				memcpy(keyv, keyn, 2);
				searchix(&pstindex, keyn, d);
				}
			else {
				nextix(&pstindex, keyn, d);
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
*/
			break;
		case 5:
/*
			if (init) {
				_swab((char *)srj, keyn, 2);
				memcpy(keyv, keyn, 2);
				searchix(&pstindex, keyn, d);
				}
			else {
				nextix(&pstindex, keyn, d);
			if (*(int *)(keyn+2) == 0)
				ok = 0;
			if (memcmp(keyv, keyn, 2))
				ok = 0;
			if (!ok) {
				valmis = 1;
				}
			}
*/
			break;
		case 9:
			if (init) {
				if (reindex || ostlsindex->nrec < (int)(kilpparam.ntosuus*datf2.numrec)) {
					if (tulprm->optiot & 0x2000)
						ostlsindex->flags |= 0x200;
					if (tulprm->hajontatulokset)
						ostlsindex->flags |= 0x100;
					else
						ostlsindex->flags &= 0xfffffeff;
					if (teeindeksi(ostlsindex, false)) {
						valmis = 1;
						break;
						}
					}
				if (tulprm->monios || tulprm->hajontatulokset) {
					_swab((char *)&srj, keyn, 2);
					if (tulprm->hajontatulokset)
						wcstoansi(keyn+2, tulprm->hajrata.rata, HAJONTA);
					memcpy(keyv, keyn, ostlsindex->keyl);
					}
				ok = !searchix(ostlsindex, keyn, d);
				}
			else {
				ok = !nextix(ostlsindex, keyn, d);
				}
			if (tulprm->monios || tulprm->hajontatulokset) {
				if ((tulprm->hajontatulokset && memcmp(keyv, keyn, 2+HAJONTA) != 0) ||
					keyn[HAJONTA+2] == '\xff' ||
					((tulprm->optiot & 0x2000) && (UCHAR) keyn[HAJONTA+2] > 248))
					ok = 0;
				}

			while (ok) {
				_swab(keyn+HAJONTA+6, (char *)os, 2);
				kilp.getrec(*d);
				if (!tulprm->kaikkisarjat && kilp.sarja == srj)
					break;
				if (tulprm->kaikkisarjat && tulprm->sarjalista[kilp.sarja])
					break;
				ok = !nextix(ostlsindex, keyn, d);
				}
			if (!ok) {
				*d = 0;
				valmis = 1;
				}
			break;
		case 10:
			if (init) {
				if (osjarj)
					delete[] osjarj;
				if (tulprm->monios)
					n_osjarj = nilm[srj]*Sarjat[srj].osuusluku+1;
				else
					n_osjarj = nilm[srj]+1;
				osjarj = new osjarjtp[n_osjarj];
				n_varaus = ossijat(tulprm, srj, tulprm->osuus, osjarj, nhyltot, nkesktot);
				if (n_ostot)
					 *n_ostot = n_varaus;
				osdk = osjarj;
				}
			if (n_ostot)
				n_varaus = *n_ostot;
			if (n_ostot && osdk+lisa < osjarj + n_varaus)
				osdk += lisa-1;
			*d = osdk->d;
			if (os)
				*os = osdk->os;
			if (*d <= 0) {
				valmis = 1;
				delete[] osjarj;
				osjarj = NULL;
				}
			osdk++;
			break;
		case 11:
		case 12:
		case 13:
			if (init) {
				if (ToimintaTila == 1) {
					uusiKilpnoIx();
					}
				kilpno = minkilpno;
				}
			else
				kilpno++;
			while (kilpno <= maxkilpno) {
				if ((*d = getpos(kilpno)) > 0) {
					kilp.getrec(*d);
					if (!tulprm->kaikkisarjat && kilp.sarja == srj)
						break;
					if (tulprm->kaikkisarjat && tulprm->sarjalista[kilp.sarja])
						break;
					}
				kilpno++;
				}
			if (kilpno > maxkilpno)  {
				valmis = 1;
				}
			break;
		}
	return(valmis);
}

static void htmlotsikot(int *srj, tulostusparamtp *tulprm)
{
	wchar_t wline[800];

	initline(tulprm);
	if (sarjaots[0] && (wcscmpU(paaots, sarjaots) || !potsfl)) {
		swprintf(wline, L"<h3>%s</h3>", sarjaots);
		putfld(tulprm, wline, 0, wcslen(wline)+5, 0, 0);
		endline(tulprm, 0);
		if (tulprm->tulmuot.tpvafl || tulprm->tulmuot.tklofl) {
//			endline(tulprm, 0);
			if (tulprm->language > 0)
				wcscpy(wline, L"<p>Written    ");
			else
				wcscpy(wline, L"<p>Tulostettu ");
			if (tulprm->tulmuot.tpvafl)
				wcscat(wline, wpaiva());
			if (tulprm->tulmuot.tklofl) {
				if (tulprm->language > 0)
					wcscat(wline, L"  at ");
				else
					wcscat(wline, L"  klo ");
				wcscat(wline, wkello());
				}
			wcscat(wline, L"\n");
			tulprm->writehtml(wline);
//			putfld(tulprm, wline, 0, 33, 0, 0);
			}
	}
//	putfld(tulprm, L"<span class='svots'>", 0, 30, 0, 0);
//	endline(tulprm, 0);
	tulprm->writehtml(L"<span class='svots'>");
	if (tulprm->piiritulokset) {
		if (tulprm->language > 0)
			wcscpy(wline, L"<p>DISTRICT :");
		else
			wcscpy(wline, L"<p>PIIRI :");
		for (int piiri = 0; piiri < piiriluku; piiri++) {
			if (piirifl[piiri] && piirinimi[piiri][0]) {
				wcscat(wline, L" ");
				wcscat(wline, piirinimi[piiri]);
				}
			}
//		if (wcslen(wline) > 13) {
//			putfld(tulprm, wline, 0, wcslen(wline), 0, 0);
//			endline(tulprm, 0);
//			}
//		putfld(tulprm, L"</span>\n", 0, 10, 0, 0);
//		endline(tulprm, 0);
		wcscat(wline, L"</span>\n");
		tulprm->writehtml(wline);
		}
	else if (tulprm->seuratulokset) {
		if (tulprm->language > 0)
			wcscpy(wline, L"<p>CLUB :");
		else
			wcscpy(wline, L"<p>SEURA : ");
		for (int is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
			wcscat(wline, L" ");
			wcscat(wline, seuranimi[is]);
			}
		wcscat(wline, L"</span>\n");
//		putfld(tulprm, wline, 0, 53, 0, 0);
		tulprm->writehtml(wline);
		}
	else {
		tulprm->writehtml(L"</span>\n");
//		putfld(tulprm, L"</span>\n", 0, 10, 0, 0);
		}
	newline(tulprm, 2);
}

static void htmlsarjaotsikot(int *srj, tulostusparamtp *tulprm, int ei_lukum, int nkesktot, int nhyltot)
{
	wchar_t wline[400] = L"", snimi[80] = L"";
//	int na;

	if (!tulprm->kaikkisarjat) {
		wcscpy(wline, L"<p><span class='srjnm'>");
		if (tulprm->yhteislahdot > 1) {
			if (tulprm->language > 0)
				wcscat(snimi, L"All");
			else
				wcscat(snimi, L"Kaikki");
//			putfld(tulprm, snimi, 0, 10, 0, 0);
			wcscat(wline, L"</span>\n");
			tulprm->writehtml(wline);
			}
		else {
			if (tulprm->hajontatulokset) {
				Sarjat[*srj].Sarjanimi(snimi);
				if (tulprm->language == 0) {
					if (tulprm->monios)
						swprintf(wline+wcslen(wline),L"%s",snimi);
					else
						swprintf(wline+wcslen(wline),L"%s - %d. osuus",snimi,tulprm->osuus+1);
					wcscat(wline, L" - Hajonta : ");
					wcscat(wline, tulprm->hajrata.rata);
					if (tulprm->hajrata.ratapit[0]) {
						swprintf(wline+wcslen(wline), L", Ratapituus %s km",
							tulprm->hajrata.ratapit);
						}
					}
				else {
					if (tulprm->monios)
						swprintf(wline+wcslen(wline),L"%s",snimi);
					else
						swprintf(wline+wcslen(wline),L"%s - Leg %d",snimi,tulprm->osuus+1);
					wcscat(wline, L" - course : ");
					wcscat(wline, tulprm->hajrata.rata);
					if (tulprm->hajrata.ratapit[0]) {
						swprintf(wline+wcslen(wline), L", Leglength %s km",
							tulprm->hajrata.ratapit);
						}
					}
				wcscat(wline, L"</span>\n");
//				putfld(tulprm, wline, 0, 40, 0, 0);
//				endline(tulprm, 1);
				tulprm->writehtml(wline);
				}
			else if (tulprm->viimos) {
				Sarjat[*srj].Sarjanimi(snimi, true);
				wcscat(wline, snimi);
				if (!Sarjat[*srj].psarjanimi[0] && Sarjat[*srj].matka[0][0]) {
					wcscat(wline, L" - (");
					for (int o = tulprm->osuus > 15 ? tulprm->osuus : 0; o <= tulprm->osuus; o++) {
						if (Sarjat[*srj].matka[0][0] && Sarjat[*srj].matka[o][0]) {
							MbsToWcs(wline+wcslen(wline), Sarjat[*srj].matka[o], 20);
							if (o < tulprm->osuus && Sarjat[*srj].matka[o+1][0]) {
								if (Sarjat[*srj].ntosuus < 7)
									wcscat(wline,L" + ");
								else
									wcscat(wline,L"+");
								}
							}
						}
					if (wcslen(wline) > 2)
						wcscat(wline, L" km)");
					}
				wcscat(wline, L"</span>\n");
				tulprm->writehtml(wline);
//				putfld(tulprm, wline, 0, 180, 0, 0);
//				endline(tulprm, 1);
				if (tulprm->lahtoluettelo) {
					if (tulprm->language == 0)
						swprintf(wline,L"<p>Ilmoittautuneita : %d", nilm[*srj]);
					else
						swprintf(wline,L"<p>Participants : %d", nilm[*srj]);
					}
				else {
					wline[0] = 0;
	//				if (kilpparam.maxnosuus == 1)
	//					na = nilm[*srj] - ntulos[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//						nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
	//				else
	//					na = nilm[*srj] - ntulos2[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//						nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
					if (!ei_lukum && navoin[*srj][tulprm->osuus] != nilm[*srj]) {
						int los = 0;
						if (kilpparam.partio)
							los = Sarjat[*srj].osuusluku-1;
						if (tulprm->language > 0) {
							swprintf(wline,L"<p>Started : %d,   Did not finish : %d,"
								L"   Disqualified : %d",ntulos[*srj][los][tulprm->piste]+
								nkesk[*srj][los]+nhyl[*srj][los],nkesktot,nhyltot);
							if (navoin[*srj][tulprm->osuus]) {
								wcscat(wline, L",   Open : ");
								_itow(navoin[*srj][tulprm->osuus], wline+wcslen(wline), 10);
								}
							}
						else {
							swprintf(wline,L"<p>Lähti : %d,   Keskeytti : %d,"
								L"   Hylätty : %d",ntulos[*srj][los][tulprm->piste]+
								nkesk[*srj][los]+nhyl[*srj][los],nkesktot,nhyltot);
							if (navoin[*srj][tulprm->osuus]) {
								wcscat(wline, L",   Avoinna : ");
								_itow(navoin[*srj][tulprm->osuus], wline+wcslen(wline), 10);
								}
							}
						}
					if (wline[0]) {
						wcscat(wline, L"\n");
						tulprm->writehtml(wline);
//						putfld(tulprm, wline,0,70,0,0);
//						endline(tulprm, 0);
						}
					}
	//			wcscat(wline, L"</p>");
//				newline(tulprm, 1);
				}
			else if (!tulprm->tarkastamattomat) {
				Sarjat[*srj].Sarjanimi(snimi);
				if (tulprm->monios)
					swprintf(wline+wcslen(wline),L"<p>%s",snimi);
				else
					{
					if (tulprm->language > 0) {
						if (!tulprm->piste)
							swprintf(wline+wcslen(wline),L"<p>%s - Leg %d", snimi,tulprm->osuus+1);
						else if (tulprm->piste > kilpparam.valuku)
							swprintf(wline+wcslen(wline),L"<p>%s  Leg %d - Legtime",	snimi,tulprm->osuus+1);
						else
							swprintf(wline+wcslen(wline),L"<p>%s - Leg %d - Lap %d",
								snimi,tulprm->osuus+1,tulprm->piste);
						}
					else {
						if (!tulprm->piste)
							swprintf(wline+wcslen(wline),L"<p>%s - %d. osuus",	snimi,tulprm->osuus+1);
						else if (tulprm->piste > kilpparam.valuku)
							swprintf(wline+wcslen(wline),L"<p>%s - %d. osuus - osuusaika",	snimi,tulprm->osuus+1);
						else
							swprintf(wline+wcslen(wline),L"<p>%s - %d. osuus - %d. väliaika",
								snimi,tulprm->osuus+1,tulprm->piste);
						}
					}

				if (!Sarjat[*srj].psarjanimi[0] && !tulprm->hajontatulokset && !tulprm->piste && Sarjat[*srj].matka[tulprm->osuus][0]) {
					swprintf(wline+wcslen(wline),L"    (%s km)", Sarjat[*srj].Matka(tulprm->osuus));
					}
				wcscat(wline, L"</span>\n");
				tulprm->writehtml(wline);
//				putfld(tulprm, wline,0,40,0, 0);
//				endline(tulprm, !ei_lukum);
	//			if (kilpparam.maxnosuus == 1) {
	//				na = nilm[*srj] - ntulos[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//					nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
					if (!ei_lukum && navoin[*srj][tulprm->osuus] != nilm[*srj]) {
						if (navoin[*srj][tulprm->osuus] != 0 ) {
							if (tulprm->language > 0)
								swprintf(wline,L"<p>Results :%d  DNF :%d  DQ :%d  Open :%d\n",
									ntulos[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],navoin[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"<p>Tuloksia :%d  Kesk :%d  Hyl :%d  Avoinna :%d\n",
									ntulos[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],navoin[*srj][tulprm->osuus]);
							}
						else {
							if (tulprm->language > 0)
								swprintf(wline,L"<p>Started : %d   DNF : %d   DQ : %d\n",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"<p>Lähti : %d   Keskeytti : %d   Hylätty : %d\n",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							}
						tulprm->writehtml(wline);
//						putfld(tulprm, wline,0,60,0, 0);
//						endline(tulprm, 0);
						}
	/*				}
				else {
					na = nilm[*srj] - ntulos2[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
						nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
					if (!ei_lukum && na != nilm[*srj]) {
						if( na != 0 ) {
							if (tulprm->language > 0)
								swprintf(wline,L"<p>Results :%d  DNF :%d  DQ :%d  Open :%d",
									ntulos2[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],na);
							else
								swprintf(wline,L"<p>Tuloksia :%d  Kesk :%d  Hyl :%d  Avoinna :%d",
									ntulos2[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],na);
							}
						else {
							if (tulprm->language > 0)
								swprintf(wline,L"<p>Started : %d   DNF : %d   DQ : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"<p>Lähti : %d   Keskeytti : %d   Hylätty : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							}
						putfld(tulprm, wline,0,60,0, 0);
						endline(tulprm, 0);
						}
					}
	*/
//				newline(tulprm, 1);
				}
			}
		}
	if (!(tulprm->viimos == L'K' && tulprm->tiivis)) {
		if (!tulprm->tulmuot.tauluhtml) {
			putfld(tulprm, L"<PRE>", 0, 5, 0, 0);
			endline(tulprm, 0);
			if (tulprm->tulmuot.otsikot) {
				FldFrmtTp *fld = tulprm->viimos == L'K' ? filefldsk : fileflds;
				newline(tulprm, 1);
				for (int f = 0; ; f++) {
					if (f > 0 && fld[f].ots[0] == 0 && fld[f].id == 0)
						break;
					if (fld[f].len && (fld[f].akt & 1) != 0) {
						putfld(tulprm, fld[f].ots, fld[f].pos,
						fld[f].len, fld[f].oik, 0);
						}
					}
				endline(tulprm, 1);
				}
			}
//		else if (!tulprm->hajontatulokset) {
		else {
			int kntjrj[MAXFLD], kntjrj_2[MAXFLD][3], n_knt = MAXFLD, l = 0;
			FldFrmtTp p_fld[MAXFLD+1];

			if ((tulprm->viimos == L'K' || tulprm->viimos == L'V') && tulprm->tulostettava != L'H')
				setflds(p_fld, filefldsk, MAXFLD, n_filefldsk);
			else
				setflds(p_fld, fileflds, MAXFLD, n_fileflds);
			tulprm->writehtml(L"<p><table>\n");
//			putfld(tulprm, L"<p><table>", 0, 10, 0, 0);
//			endline(tulprm, 0);
			wcscpy(wline, L"<colgroup>\n");
			if ((tulprm->viimos == L'K' || tulprm->viimos == L'V') && tulprm->tulostettava != L'H') {
				hae_kenttajarj_2(p_fld, kntjrj_2, &n_knt, tulprm->viimos == L'K' ? 2 : 1);
				for (int i_knt = 0; i_knt < n_knt; i_knt++) {
					l += kntjrj_2[i_knt][2]+1;
					}
				for (int i_knt = 0; i_knt < n_knt; i_knt++) {
					int lp;
					if ((tulprm->tulmuot.tauluhtml & 3) == 2) {
						lp = (100*kntjrj_2[i_knt][2]+150)/l;
						swprintf(wline+wcslen(wline), L"<col width=\"%d%%\">\n", lp);
						}
					else {
						lp = tulprm->tulmuot.levkerr*tulprm->tulmuot.fontsize*(kntjrj_2[i_knt][2]+1);
						swprintf(wline+wcslen(wline), L"<col style=\"min-width: %dpx; \">", lp);
						}
					}
				}
			else {
				hae_kenttajarj(tulprm, p_fld, kntjrj, &n_knt);
				for (int i_knt = 0; i_knt < n_knt; i_knt++) {
	//				if ((kntjrj[i_knt] >= F_OSASJ && tulprm->vaihelkm == 1 && !tulprm->kaikkiva) ||
	//					(kntjrj[i_knt] == F_ERO && tulprm->vaihelkm == 2))
	//					continue;
					l += p_fld[kntjrj[i_knt]].len+1;
					}
				for (int i_knt = 0; i_knt < n_knt; i_knt++) {
					int lp = 0;
	//				if ((kntjrj[i_knt] >= F_OSASJ && tulprm->vaihelkm == 1 && !tulprm->kaikkiva) ||
	//					(kntjrj[i_knt] == F_ERO && tulprm->vaihelkm == 2))
	//					continue;
					if ((tulprm->tulmuot.tauluhtml & 3) == 2) {
						lp = (100*p_fld[kntjrj[i_knt]].len+150)/l;
						if (tulprm->monios && kntjrj[i_knt] == F_EDSJ)
							lp = 0;
						if (tulprm->monios && kntjrj[i_knt] == F_KNO)
							lp += (100*p_fld[F_EDSJ].len+150)/l;
						swprintf(wline+wcslen(wline), L"<col width=\"%d%%\">\n", lp);
						}
					else {
						lp = tulprm->tulmuot.levkerr*tulprm->tulmuot.fontsize*(p_fld[kntjrj[i_knt]].len+1);
						if (tulprm->monios && kntjrj[i_knt] == F_EDSJ)
							lp = 0;
						if (tulprm->monios && kntjrj[i_knt] == F_KNO)
							lp += tulprm->tulmuot.levkerr*tulprm->tulmuot.fontsize*(p_fld[F_EDSJ].len+1);
						swprintf(wline+wcslen(wline), L"<col style=\"min-width: %dpx; \">", lp);
						}
					}
				}
			wcscat(wline, L"</colgroup>\n");
			tulprm->writehtml(wline);
//			putfld(tulprm, wline, 0, 1000, 0, 0);
//			endline(tulprm, 0);
			wline[0] = 0;
			if (tulprm->tulmuot.otsikot) {
				if ((tulprm->viimos == L'K' || tulprm->viimos == L'V') && tulprm->tulostettava != L'H') {
					for (int rr = 0; rr < 2; rr++) {
						if (rr == 0)
							wcscat(wline, L"<tr class='jkk'>");
						else
							wcscat(wline, L"<tr>");
						for (int i_knt = 0; i_knt < n_knt; i_knt++) {
							if (kntjrj_2[i_knt][rr] < 0) {
								wcscat(wline, L"<th></th>");
								continue;
								}
							else if (p_fld[kntjrj_2[i_knt][rr]].oik) {
								wcscat(wline, L"<th align=right>");
								}
							else {
								wcscat(wline, L"<th align=left>");
								}
							if (p_fld[kntjrj_2[i_knt][rr]].len)
								wcscat(wline, p_fld[kntjrj_2[i_knt][rr]].ots);
							wcscat(wline, L"</th>");
							}
						wcscat(wline, L"</tr>");
						if (tulprm->viimos == L'V')
							break;
						if (rr == 0)
							wcscat(wline, L"\n");
						}
					}
				else {
					wcscpy(wline, L"<tr>");
					for (int i_knt = 0; i_knt < n_knt; i_knt++) {
						if (p_fld[kntjrj[i_knt]].oik)
							wcscat(wline, L"<th align=right>");
						else
							wcscat(wline, L"<th align=left>");
						wcscat(wline, p_fld[kntjrj[i_knt]].ots);
						wcscat(wline, L"</th>");
						}
					wcscat(wline, L"</tr>\n");
					}
				}
			tulprm->writehtml(wline);
//			putfld(tulprm, wline, 0, 1000, 0, 0);
//			endline(tulprm, 0);
			tulprm->tulmuot.tauluhtml = 2;
			}
//		else {
//			tulprm->writehtml(L"<p><table class='jkrva'>\n");
//			}
		}
	else {
		putfld(tulprm, L"<p>", 0, 5, 0, 0);
		endline(tulprm, 0);
		}
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

static void kirjoitinpaaotsikot(int *l, int *sv, int *srj, tulostusparamtp *tulprm, int *potsfl)
{
	wchar_t wline[200];

	if (*l == 0 && tulprm->sivu_alkuteksti) {
		kirjSivuAlku(tulprm, l);
		}
	if (!tulprm->paa_alkuteksti && !tulprm->sivu_alkuteksti) {
		if (tulprm->viimos == L'K') {
			*l += tulprm->tiivis ? ((Sarjat[*srj].ntosuus+5) / 3) : (Sarjat[*srj].ntosuus+1);
			}
		if (!*potsfl)
			initpage(tulprm,0);
		initline(tulprm);
		if (kilpailu[0] && !tulprm->tarkastamattomat) {
			paaots_on(tulprm);
			putfld(tulprm, kilpailu, 0, wcslen(kilpailu), 0, 0);
			endline(tulprm, 1);
			paaots_pois(tulprm);
			(*l)++;
			}
		}
	*potsfl = FALSE;
	aliots_on(tulprm);
	if (tulprm->piiritulokset) {
		if (tulprm->language > 0)
			wcscpy(wline, L"  DISTRICT :");
		else
			wcscpy(wline, L"  PIIRI :");
		for (int piiri = 0; piiri < piiriluku; piiri++) {
			if (piirifl[piiri] && piirinimi[piiri][0]) {
				if (wcslen(wline) + wcslen(piirinimi[piiri]) >=
					(UINT32) tulprm->tulmuot.rivpit) {
					putfld(tulprm, wline, 0, wcslen(wline), 0, 0);
					endline(tulprm, 0);
					(*l)++;
					wcscpy(wline,L"          ");
					}
				else
					wcscat(wline, L" ");
				wcscat(wline, piirinimi[piiri]);
				}
			}
		if (wcslen(wline) > 10) {
			putfld(tulprm, wline, 0, wcslen(wline), 0, 0);
			endline(tulprm, 0);
			(*l)++;
			}
		}
	else if (tulprm->seuratulokset) {
		if (tulprm->language > 0)
			wcscpy(wline, L"  CLUB :");
		else
			wcscpy(wline, L"  SEURA : ");
		for (int is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
			wcscat(wline, seuranimi[is]);
			wcscat(wline, L" ");
			if (wcslen(wline) > 50) {
				wline[51] = L'.';
				wline[52] = L'.';
				wline[53] = 0;
				break;
				}
			}
		putfld(tulprm, wline, 0, 53, 0, 0);
		}
	else if (tulprm->tulmuot.tpvafl || tulprm->tulmuot.tklofl) {
		if (tulprm->language > 0)
			wcscpy(wline, L"Printed    ");
		else
			wcscpy(wline, L"Tulostettu ");
		if (tulprm->tulmuot.tpvafl)
			wcscat(wline, wpaiva());
		if (tulprm->tulmuot.tklofl) {
			if (tulprm->language > 0)
				wcscat(wline, L"  at ");
			else
				wcscat(wline, L"  klo ");
			wcscat(wline, wkello());
			}
		putfld(tulprm, wline, 400, 33, 0, 0);
		}
	if (tulprm->language > 0)
		wcscpy(wline, L"Page ");
	else
		wcscpy(wline, L"Sivu  ");
	_itow(++(*sv), wline+5, 10);
	putfld(tulprm, wline, 800, 8, 1, 0);
	endline(tulprm, 1);
	aliots_pois(tulprm);
	(*l)++;
	(*l)++;
	}

static void kirjoitinjatko_otsikot(int *l, int *srj, tulostusparamtp *tulprm)
{
	wchar_t wline[200] = L"", snimi[80] = L"";

	if (tulprm->kaikkisarjat)
		return;
	aliots_on(tulprm);
	if (tulprm->yhteislahdot > 1) {
		if (tulprm->language > 0)
			wcscpy(snimi, L"All");
		else
			wcscpy(snimi, L"Kaikki");
		}
	else
		Sarjat[*srj].Sarjanimi(snimi, true);
	if (tulprm->viimos == L'K' || tulprm->monios)
		swprintf(wline,L"%-10s",snimi);
	else if (tulprm->viimos == L'V') {
		if (tulprm->language > 0)
			swprintf(wline,L"%-10s    Finish",snimi);
		else
			swprintf(wline,L"%-10s    Yhteistulokset",snimi);
		}
	else {
		Sarjat[*srj].Sarjanimi(snimi);
		if (tulprm->language > 0) {
			if (!tulprm->piste)
				swprintf(wline,L"%-10s  Leg %d", snimi,tulprm->osuus+1);
			else if (tulprm->piste > kilpparam.valuku)
				swprintf(wline,L"%-10s  Leg %d - Legtime",	snimi,tulprm->osuus+1);
			else
				swprintf(wline,L"%-10s  Leg %d, Lap %d", snimi,tulprm->osuus+1,tulprm->piste);
			}
		else {
			if (!tulprm->piste)
				swprintf(wline,L"%-10s  %d. osuus", snimi,tulprm->osuus+1);
			else if (tulprm->piste > kilpparam.valuku)
				swprintf(wline,L"%-10s  %d. osuus - osuusaika",	snimi,tulprm->osuus+1);
			else
				swprintf(wline,L"%-10s  %d. osuus, %d. väliaika",snimi,tulprm->osuus+1,tulprm->piste);
			}
		}
	putfld(tulprm, wline, 0, wcslen(wline), 0, 0);
	endline(tulprm, 1);
	if (tulprm->tulmuot.otsikot) {
		FldFrmtTp *fld = tulprm->viimos == L'K' ? prtfldsk : prtflds;
		newline(tulprm, 1);
		(*l)++;
		for (int f = 0; ; f++) {
			if (f > 0 && fld[f].ots[0] == 0 && fld[f].id == 0)
				break;
			if (fld[f].len && (fld[f].akt & 1) != 0) {
				putfld(tulprm, fld[f].ots, fld[f].pos,
				fld[f].len, fld[f].oik, 0);
				}
			}
		endline(tulprm, 1);
		(*l)++;
		}
	aliots_pois(tulprm);
	(*l)++;
	(*l)++;
}

static void kirjoitinalkuotsikot(int *l, int *srj, tulostusparamtp *tulprm, int ei_lukum, int nkesktot, int nhyltot)
{
	wchar_t wline[200] = L"", snimi[80] = L"";
//	int na;

	aliots_on(tulprm);
	if (!tulprm->kaikkisarjat) {
		if (tulprm->yhteislahdot > 1) {
			if (tulprm->language > 0)
				wcscpy(snimi, L"All");
			else
				wcscpy(snimi, L"Kaikki");
			putfld(tulprm, snimi, 0, 10, 0, 0);
			endline(tulprm, 1);
			(*l)++;
			}
		else {
			if (tulprm->hajontatulokset) {
				Sarjat[*srj].Sarjanimi(snimi);
				if (tulprm->monios)
					swprintf(wline,L"%-10s",snimi);
				else {
					if (tulprm->language > 0)
						swprintf(wline,L"%-10s Leg %d",snimi,tulprm->osuus+1);
					else
						swprintf(wline,L"%-10s %d. osuus",snimi,tulprm->osuus+1);
					}
				putfld(tulprm, wline, 0, 40, 0, 0);
				endline(tulprm, 0);
				if (tulprm->language > 0)
					wcscpy(wline, L"  Course : ");
				else
					wcscpy(wline, L"  Hajonta : ");
				wcscat(wline, tulprm->hajrata.rata);
				if (tulprm->hajrata.ratapit[0]) {
					if (tulprm->language > 0)
						swprintf(wline+wcslen(wline), L",   Leglength %s km",
							tulprm->hajrata.ratapit);
					else
						swprintf(wline+wcslen(wline), L",   Ratapituus %s km",
							tulprm->hajrata.ratapit);
					}
				putfld(tulprm, wline, 0, 40, 0, 0);
				endline(tulprm, 0);
				(*l)++;
				}
			else if (tulprm->viimos) {
				Sarjat[*srj].Sarjanimi(snimi, true);
				putfld(tulprm, snimi, 0, 10, 0, 0);
				if (!Sarjat[*srj].psarjanimi[0] && Sarjat[*srj].matka[0][0]) {
					wcscpy(wline, L"(");
					for (int o = tulprm->osuus > 15 ? tulprm->osuus : 0; o < tulprm->osuus; o++) {
						if (Sarjat[*srj].matka[o][0]) {
							MbsToWcs(wline+wcslen(wline), Sarjat[*srj].matka[o], 20);
							if (Sarjat[*srj].matka[o+1][0])
								{
								if (Sarjat[*srj].ntosuus < 7)
									wcscat(wline,L" + ");
								else
									wcscat(wline,L"+");
								}
							}
						}
					if (Sarjat[*srj].matka[tulprm->osuus][0])
						MbsToWcs(wline+wcslen(wline), Sarjat[*srj].matka[tulprm->osuus], 20);
					wcscat(wline, L" km)");
					}
				putfld(tulprm, wline, prtflds[3].pos, 58, 0, 0);
				endline(tulprm, 1);
				(*l)++;
				(*l)++;
	//			if (kilpparam.maxnosuus == 1)
	//				na = nilm[*srj] - ntulos[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//					nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
	//			else
	//				na = nilm[*srj] - ntulos2[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//					nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
				if (!ei_lukum && navoin[*srj][tulprm->osuus] != nilm[*srj]) {
					int los = 0;
					if (kilpparam.partio)
						los = Sarjat[*srj].osuusluku-1;
					if (tulprm->language > 0) {
						swprintf(wline,L"Started : %-4d   Did not finish : %-3d"
							L"   Disqualified : %-4d",ntulos[*srj][los][tulprm->piste]+
							nkesk[*srj][los]+nhyl[*srj][los],nkesktot,nhyltot);
						if (navoin[*srj][tulprm->osuus]) {
							wcscat(wline, L"   Open : ");
							_itow(navoin[*srj][tulprm->osuus], wline+wcslen(wline), 10);
							}
						}
					else {
						swprintf(wline,L"Lähti : %-4d   Keskeytti : %-3d"
							L"   Hylätty : %-4d",ntulos[*srj][los][tulprm->piste]+
							nkesk[*srj][los]+nhyl[*srj][los],nkesktot,nhyltot);
						if (navoin[*srj][tulprm->osuus]) {
							wcscat(wline, L"   Avoinna : ");
							_itow(navoin[*srj][tulprm->osuus], wline+wcslen(wline), 10);
							}
						}
					putfld(tulprm, wline,0,70,0,0);
					endline(tulprm, 0);
					}
				newline(tulprm, 1);
				}
			else if (!tulprm->tarkastamattomat) {
				Sarjat[*srj].Sarjanimi(snimi);
				if (tulprm->monios)
					swprintf(wline,L"%-10s",snimi);
				else
					{
					if (tulprm->language > 0) {
						if (!tulprm->piste)
							swprintf(wline,L"%-10s  Leg %d", snimi,tulprm->osuus+1);
						else if (tulprm->piste > kilpparam.valuku)
							swprintf(wline,L"%-10s  Leg %d - Legtime",	snimi,tulprm->osuus+1);
						else
							swprintf(wline,L"%-10s  Leg %d, Lap %d",
								snimi,tulprm->osuus+1,tulprm->piste);
						}
					else {
						if (!tulprm->piste)
							swprintf(wline,L"%-10s  %d. osuus",	snimi,tulprm->osuus+1);
						else if (tulprm->piste > kilpparam.valuku)
							swprintf(wline,L"%-10s  %d. osuus - osuusaika",	snimi,tulprm->osuus+1);
						else
							swprintf(wline,L"%-10s  %d. osuus, %d. väliaika",
								snimi,tulprm->osuus+1,tulprm->piste);
						}
					}
				if (!tulprm->hajontatulokset && !tulprm->piste && Sarjat[*srj].matka[tulprm->osuus][0]) {
					swprintf(wline+wcslen(wline),L"    (%s km)", Sarjat[*srj].Matka(tulprm->osuus));
					}
				putfld(tulprm, wline,0,40,0, 0);
				endline(tulprm, !ei_lukum);
	//			if (kilpparam.maxnosuus == 1) {
	//				na = nilm[*srj] - ntulos[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
	//					nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
					if (!ei_lukum && navoin[*srj][tulprm->osuus] != nilm[*srj]) {
						if( navoin[*srj][tulprm->osuus] != 0 ) {
							if (tulprm->language > 0)
								swprintf(wline,L"Results :%d  DNF :%d  DQ :%d  Open :%d",
									ntulos[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],navoin[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"Tuloksia :%d  Kesk :%d  Hyl :%d  Avoinna :%d",
									ntulos[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],navoin[*srj][tulprm->osuus]);
							}
						else {
							if (tulprm->language > 0)
								swprintf(wline,L"Started : %d   DNF : %d   DQ : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"Lähti : %d   Keskeytti : %d   Hylätty : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							}
						putfld(tulprm, wline,0,60,0, 0);
						endline(tulprm, 0);
						(*l)++;
						}
	/*			}
				else {
					na = nilm[*srj] - ntulos2[*srj][tulprm->osuus][tulprm->piste] - nkesk[*srj][tulprm->osuus] -
						nhyl[*srj][tulprm->osuus] - neil[*srj][tulprm->osuus];
					if (!ei_lukum && na != nilm[*srj]) {
						if( na != 0 ) {
							if (tulprm->language > 0)
								swprintf(wline,L"Results :%d  DNF :%d  DQ :%d  Open :%d",
									ntulos2[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],na);
							else
								swprintf(wline,L"Tuloksia :%d  Kesk :%d  Hyl :%d  Avoinna :%d",
									ntulos2[*srj][tulprm->osuus][tulprm->piste],nkesk[*srj][tulprm->osuus],
									nhyl[*srj][tulprm->osuus],na);
							}
						else {
							if (tulprm->language > 0)
								swprintf(wline,L"Started : %d   DNF : %d   DQ : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							else
								swprintf(wline,L"Lähti : %d   Keskeytti : %d   Hylätty : %d",
									ntulos[*srj][tulprm->osuus][tulprm->piste]+nkesk[*srj][tulprm->osuus]+
									nhyl[*srj][tulprm->osuus],nkesk[*srj][tulprm->osuus],nhyl[*srj][tulprm->osuus]);
							}
						putfld(tulprm, wline,0,60,0, 0);
						endline(tulprm, 0);
						(*l)++;
						}
					}
	*/
				*l += 2;
				newline(tulprm, 1);
				}
			}
		}
	if (tulprm->tulmuot.otsikot) {
		FldFrmtTp *fld = tulprm->viimos == L'K' ? prtfldsk : prtflds;
		newline(tulprm, 1);
		(*l)++;
		for (int f = 0; ; f++) {
			if (f > 0 && fld[f].ots[0] == 0 && fld[f].id == 0)
				break;
			if (fld[f].len && (fld[f].akt & 1) != 0) {
				putfld(tulprm, fld[f].ots, fld[f].pos,
				fld[f].len, fld[f].oik, 0);
				}
			}
		endline(tulprm, 1);
		(*l)++;
		}
	aliots_pois(tulprm);
	if (!ei_lukum) (*l)++;
		initline(tulprm);
}

static bool on_avoin(kilptietue &kilp)
{
	for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		if (stschind(kilp.Tark(os), "PVEKH") >= 0)
			return(false);
		if (kilp.Maali(os, 0) == TMAALI0)
			return(true);
	}
	return(false);
}

static bool onHajonta(wchar_t *krata, wchar_t *trata)
	{
	wchar_t *hp1, *hp2;

	for (hp1 = trata, hp2 = krata; *hp1 && (*hp1 == L'-' || *hp1 == *hp2); hp1++, hp2++) ;
	if (!*hp1 || (hp1 - trata) >= ratavalpit)
		return(true);
	return(false);
	}

int tulostasarja(INT *srj, tulostusparamtp *tulprm, INT *l, INT *sv, INT autotl)
{
	int os;
	int yhd = 0;
	int sj1 = 0, etene = 0, ret = 0, tulostettu = 0;
	TULOS tls0;
	wchar_t ct1;
	int  sj, n, vm, i, is, piiri, ei_lukum = 0;
//	int skip;
	int  d = 0, ntls, nalku = 0, lj=0, lj1 = 4;
	char ch,ch2;
	kilptietue kilp;
	wchar_t seura[LSEURA+1];
	int tulosfl;
	int nkesktot = 0, nhyltot = 0, n_ostot = 0;
	int nyl = 0, *p_nyl = 0;
	int os1, osd;
	bool enstietue = true;
	wchar_t wst[200];

	if (*srj < 0 || *srj >= sarjaluku+nsarjayhd || nilm[*srj] == 0)
		return(0);
	os = tulprm->osuus;
	tls0 = 0;

	if (tulprm->seuratulokset && !onko_seura(*srj))
		return(0);

	if (tulprm->piiritulokset && !tulprm->sj_raja && !onko_piiri(*srj))
		return(0);

	if (tulprm->yhteislahdot) {
		p_nyl = &nyl;
		if ((!tulprm->kaikkisarjat || sarjaluku == 1) && tulprm->viimos != L'K' &&
			(os >= Sarjat[*srj].ntosuus || !onko_yhtlahto(*srj, os)))
			return(0);
		}

	if (tulprm->tulostettava == L'H')
		tulprm->hksijafl = !tulprm->viimos;

	if (tulprm->viimos) {
		if (tulprm->viimos == L'K' && tulprm->tulostettava == L'H')
			os = 0;
		else
			os = Sarjat[*srj].ntosuus-1;
		if (kilpparam.maxnosuus > 1)
			ei_lukum = 1;
		}
	else {
		if (os >= Sarjat[*srj].ntosuus)
			return(0);
		}

	if (tulprm->viimos == L'K') {
		if (tulprm->yhteislahdot) {
			tulprm->yhteislahdot = 2;
			os = 0;
			}
		}

	EnterCriticalSection(&tall_CriticalSection);

	tulprm->rivi = 0;
	if (tulprm->monios) {
		os = Sarjat[*srj].osuusluku - 1;
		n = Sarjat[*srj].osuusluku * nilm[*srj];
		tulprm->ixjarj = tulprm->hajontatulokset ? 9 : 10;
		}
	else if (!tulprm->kaikkisarjat && tulprm->piste == 0) {

		n = nilm[*srj];

//		if (tulprm->monios)
//			os = 0;

		if (tulprm->tarkastamattomat &&
			!onko_tarkastamatta(*srj, os, tulprm->viimos == L'K')) {
			goto poistu;
			}

		if (tulprm->piste && tulprm->rajaus != L'P')
			tulprm->rajaus = L'T';

		if (tulprm->rajaus == L'Y')
			ct1 = L'H';
		else
			ct1 = tulprm->rajaus;

//		if (kilpparam.maxnosuus == 1)
			ntls = ntulos[*srj][os][0];
//		else
//			ntls = ntulos2[*srj][os][0];

		switch(tulprm->rajaus) {
			case L'L' :
//				if (Sarjat[*srj].maxnosuus > 1) {
//					n -= neil[*srj][0];
//					}
//				else
					{
					n = ntls + nhyl[*srj][os] + nkesk[*srj][os];
					if (tulprm->viimos)
						n += neil[*srj][os] - neil[*srj][0];
					}
				lj1 = 3;
				break;
			case L'H' :
				n = ntulos[*srj][os][tulprm->piste];
				lj1 = 0;
				break;
			case L'P' :
				if (tulprm->tulostettava != L'H')
					n = ntulos[*srj][os][tulprm->piste];
				n = min(tulprm->viimsija,n);
				if (!autotl && tulprm->enssija > 1) {
					n -= tulprm->enssija-1;
					nalku = tulprm->enssija-1;
					ei_lukum = 1;
					}
				lj1 = 0;
				break;
			case L'Y' :
				if (!kilpparam.sailhyl) {
					if (tulprm->viimos != L'K') {
						n = nhyl[*srj][os];
						lj = 2;
						lj1 = 2;
						}
					else {
						n = nilm[*srj] - ntls - neil[0][0];
						lj = 2;
						lj1 = 4;
						}
					}
				else {
					n = ntls + nhyl[*srj][os] + nkesk[*srj][os];
					if (tulprm->viimos)
						n += neil[*srj][os] - neil[*srj][0];
					}
				tulprm->piste = 0;
				break;
			case L'K' :
				if (tulprm->viimos != L'K') {
					n = nkesk[*srj][os];
					lj = 1;
					lj1 = 1;
					}
				else {
					n = nilm[*srj] - ntls - neil[0][0];
					lj = 1;
					lj1 = 4;
					}
				tulprm->piste = 0;
				break;
			case L'E' :
				n = neil[*srj][os];
				lj = 3;
				lj1 = 3;
				tulprm->piste = 0;
				break;
			case L'A' :
				if (tulprm->viimos == L'K') {
					os = 0;
					}
				else {
					n = nilm[*srj] - ntls - nhyl[*srj][os] -
						nkesk[*srj][os] - neil[*srj][os];
					lj = 4;
					lj1 = 4;
					}
				tulprm->piste = 0;
				break;
			}
		}

	if (tulprm->piste) {
		if (tulprm->piste <= kilpparam.valuku) {
			lj = tulprm->piste + 4;
			ntls = ntulos[*srj][os][tulprm->piste];
			}
		else {
			lj = kilpparam.valuku+1;
			ntls = ntulosix[*srj][os][kilpparam.valuku+1];
			}
		lj1 = lj;
		n = ntls;
		}

	if ((tulprm->viimos || tulprm->piste > kilpparam.valuku) &&
		!tulprm->hajontatulokset && tulprm->tulostettava != L'H')
		ossijat(tulprm, *srj, tulprm->osuus, NULL, NULL, NULL);

	if (!tulprm->kaikkisarjat) {
		if (kilpparam.partio) {
			nkesktot = nkesk[*srj][Sarjat[*srj].osuusluku-1];
			nhyltot = nhyl[*srj][Sarjat[*srj].osuusluku-1];
			}
		else if (tulprm->viimos == 0) {
			nkesktot = nkesk[*srj][os];
			nhyltot = nhyl[*srj][os];
			}
		else
			{
			nkesktot = njoukkesk[*srj];
			nhyltot = njoukhyl[*srj];
			}
		}
	if (tlsSeuraava(tulprm, &d, 1, *srj, &lj, lj1, true, &nkesktot, &nhyltot, &n_ostot, &osd))
		goto lopeta;
	if (tulprm->hajontatulokset && tulprm->monios)
		os1 = osd;
	else
		os1 = os;

//	if (!tulprm->kaikkisarjat && tulprm->tulostettava == L'H')
//		n = n_ostot;
//	else
		if (tulprm->ixjarj != 0 && tulprm->ixjarj != 9 && tulprm->ixjarj != 10
			&& tulprm->kaikkisarjat)
			n = nilmt;

	if (tulprm->kohde == L'X') {
		 if (xmlsrjots(*srj, tulprm))
			 goto lopeta;
		 }

	if (tulprm->kohde == L'H' && tulprm->tulostettava == L'E' && !tulprm->hajontatulokset) {
		if (EmitHtmlSrjOts(*srj, tulprm))
			goto lopeta;
		}

	if (n > 0) {

		tulostettu = 1;
#ifdef _CONSOLE
		if (tulprm->kohde == L'P') {
			clrln(ySize-1);
			vidspmsg(ySize-1,0,7,0,"Tulostaa sarjaa");
			vidspwmsg(ySize-1,17,7,0,Sarjat[*srj].Sarjanimi());
			if (!autotl)
				vidspmsg(ySize-1,30,7,0,"Paina <Esc> keskeyttääksesi");
			tulfl =TRUE;
			tcomfl = FALSE;
			}
		else if (tulprm->kohde == L'N') {
			scpclr();
			kehys(0);
			clrln(0);
			tulfl =TRUE;
			nayta_srj_os(*srj, os, 0);
			}
#endif

		if (tulprm->yhteislahdot && tulprm->viimos == L'K')
			vm = maxosuusluku+1;
		else
			vm = min(15,tulprm->viimsija + 8);

		if (tulprm->kohde != L'P')
			*l = 0;
		else if ((*l > 2 && (tulprm->viimos != L'K' || tulprm->tulostettava == L'H')) ||
			*l > Sarjat[*srj].osuusluku + 3) {
			if((tulprm->rajaus == L'P' || tulprm->seuratulokset || tulprm->piiritulokset ||
				tulprm->yhteislahdot || tulprm->tarkastamattomat ||
				!tulprm->tulmuot.uusisivu) && mahtuusivulle(*l, vm, tulprm)) {
				newline(tulprm, 1);
				*l += 2;
				}
			else {
				*l = 0;
				if (tulprm->sivu_alateksti)
					kirjSivuAla(tulprm, l);
				endpage(tulprm);
				initpage(tulprm,0);
				}
			}
		modif[*srj][os] = FALSE;
/*
		skip = 0;
		if (autotl && enstls[*srj][os] > tulprm->tulmuot.sivpit - 8 + 2*!kilpailu[0]) {
			skip = tulprm->tulmuot.sivpit - 8 + 2 * !kilpailu[0];
			(*sv)++;
			while (enstls[*srj][os] > skip + tulprm->tulmuot.sivpit - 6 + 2*!kilpailu[0]) {
				skip += tulprm->tulmuot.sivpit - 6 + 2 * !kilpailu[0];
				(*sv)++;
				}
			}
*/
		sj = 0;
		if (d > 0 && (tulprm->ixjarj == 0 || tulprm->ixjarj == 10)) {
			GETREC(&kilp, d);
			if (tulprm->ixjarj == 0)
				karkiaika = kilp.tTulos(os,tulprm->piste);
			else
				karkiaika = kilp.osTulos(os,tulprm->piste,true);
			}
		if (nalku > 0 && (tulprm->ixjarj == 0 || tulprm->ixjarj == 10)) {
			sj = nalku;
			tlsSeuraava(tulprm, &d, nalku, *srj, &lj, lj1, false, NULL, NULL, NULL, &osd);
			}

		for (i = 1; i <= n || tulprm->monios; i++) {
#ifdef _CONSOLE
			if (!autotl) {
				do {
					ch = readkbd(&ch2,FALSE,spoolfl);
					if( ch == 27 ) {
						ret = 1;
						goto lopeta;
						}
					} while (ch2 != 0);
				}
			if (tulprm->kohde == L'N' && modif[*srj][os] && (tulprm->ixjarj == 0 || tulprm->ixjarj == 10)) {
				i = 1;
				sj = 0;
				tls0 = 0;
				*l = 0;
				lj--;
				enstietue = true;
				tlsSeuraava(tulprm, &d, 1, *srj, &lj, lj1, true, &nkesktot, &nhyltot, &n_ostot, &osd);
				if (nalku > 0) {
					sj = nalku;
					tlsSeuraava(tulprm, &d, nalku, *srj, &lj, lj1, false, NULL, NULL, NULL, &osd);
					}
				writeerror("Uusi tulos, sarja aloitetaan alusta", 1500);
				modif[*srj][os] = FALSE;
				}
#endif
			if (!tulprm->hajontatulokset && !tulprm->piirisijat)
				sj++;
			do {
				if (!enstietue) {
					if (tlsSeuraava(tulprm, &d, 1, *srj, &lj, lj1, false, NULL, NULL, NULL, &osd)) {
						enstietue = true;
						break;
						}
					}
				enstietue = false;
				if (tulprm->hajontatulokset && tulprm->monios)
					os1 = osd;
				if (d > 0)
					GETREC(&kilp, d);
				else {
					enstietue = true;
					break;
					}
				if (tulprm->hajontatulokset && ((!tulprm->monios && osd != os) ||
					!onHajonta(kilp.Hajonta(os1, wst), tulprm->hajrata.rata)))
					continue;
				if (tulprm->viimos == 0)
					break;
				if (wcswcind(ct1, L"HK") < 0 || ct1 == kilp.wTark(tulprm->osuus, 2))
					break;
				} while(1);
			if (enstietue)
				break;
#ifdef EI_OLE
			else {
				memset(&kilp, 0, sizeof(kilptietue));
				kilp.setMaali(os, tulprm->piste, (JarjTulos(yhd, os, tulprm->piste, d) + Sarjat[*srj].lahto +
					(24L+t0)*TUNTI) % (24L*TUNTI) - t0*TUNTI);
				kilp.ostiet[os].keskhyl = '-';
				kilp.setSija(os, tulprm->piste, sj);
				kilp.ostiet[os].ylahto = TMAALI0;
				}
#endif
			if (tulprm->yhteislahdot && (tulprm->ixjarj == 0 || tulprm->ixjarj == 10) && kilp.ostiet[os].ylahto == TMAALI0)
				continue;
			if (kilp.Tark(0, 0) == 'P')
				continue;
			if (tulprm->viimos == L'K' && tulprm->tulostettava != L'H' && tulprm->rajaus == L'A' && !on_avoin(kilp))
				continue;
			if (!kilpparam.sailhyl && tulprm->rajaus != L'I' && tulprm->rajaus != L'E' &&
				tulprm->ixjarj == 0 && stschind(kilp.Tark(tulprm->viimos ? 0 : os, 1), "ES") >= 0) {
				if (tulprm->viimos != L'K')
					break;
				if (kilp.Tark(0, 0) == 'E')
					continue;
				}
			if (tulprm->viimos != L'K') {
#ifdef LUENTA
				if (tulprm->rajaus == L'-' && kilp.ostiet[os].badge[0])
					continue;
#endif
				if (tulprm->rajaus == L'Y' && kilp.Tark(os, 0) != 'H')
					continue;
				}
			else {
				if ((tulprm->rajaus == L'Y' || tulprm->rajaus == L'K') && kilp.Tark(os, 0) != ct1)
					continue;
				}

			tulosfl = 0;
			if (tulprm->piiritulokset && (piiri = kilp.piiri) > 0 &&
				piiri <= piiriluku && piirifl[piiri - 1]) {
				if (tulprm->piirisijat)
					sj++;
				tulosfl = 1;
				}
			else if (tulprm->seuratulokset) {
				for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
					if (!wcscmp(seuranimi[is], upcasewstr(kilp.Seura(seura)))) {
						if (tulprm->piirisijat)
							sj++;
						tulosfl = 1;
						break;
						}
					}
				}

			if (tulprm->tulostettava == L'H' && tulprm->monios) {
				os = osd;
				if (tulos(&kilp,os,tulprm->piste) != tls0)
					sj1 = sj;
				tls0 = tulos(&kilp,os,tulprm->piste);
				}
			else if (!tulprm->hajontatulokset && !tulprm->yhteislahdot) {
				if (kilpparam.alalaji == L'D') {
					sj1 = sj;
					}
				else {
					if (tulprm->ixjarj != 0) {
						sj = kilp.Sija(os, 0);
						sj1 = sj;
						}
					else if(!tulprm->piirisijat ||
						((tulprm->piiritulokset || tulprm->seuratulokset) && tulosfl)) {
						if (tulos(&kilp,os,tulprm->piste) != tls0)
							sj1 = sj;
						tls0 = tulos(&kilp,os,tulprm->piste);
						}
					}
				}
			if (tulprm->ixjarj == 0) {
				if (tulprm->rajaus == L'P' && sj1 > tulprm->viimsija)
					break;
				}
//			if (skip) {
//				skip--;
//				continue;
//				}
			if ((!tulprm->piiritulokset && !tulprm->seuratulokset) ||
				(!tulprm->piirisijat && sj1 > 0 && sj1 <= tulprm->sj_raja))
				tulosfl = 1;

			if( tulprm->kohde == L'P') {
				enstls[*srj][os] = 10000;
				if (!tulprm->mahtuusivulle(*l, 1)) {
					*l = 0;
					if (tulprm->sivu_alateksti)
						kirjSivuAla(tulprm, l);
					endpage(tulprm);
					}
				if( *l == 0 || potsfl) {
					kirjoitinpaaotsikot(l, sv, srj, tulprm, &potsfl);
					if (i > 1)
						kirjoitinjatko_otsikot(l, srj, tulprm);
					}
				if (i == 1) {
					if (!tulprm->mahtuusivulle(*l, 5)) {
						*l = 0;
						if (tulprm->sivu_alateksti)
							kirjSivuAla(tulprm, l);
						endpage(tulprm);
						}
					kirjoitinalkuotsikot(l, srj, tulprm, ei_lukum, nkesktot, nhyltot);
					}
				if (tulprm->hajontatulokset) {
					sj++;
					if (tulos(&kilp,os1,0) != tls0)
						sj1 = sj;
					if (sj1 > tulprm->viimsija)
						break;
					tls0 = tulos(&kilp,os1,0);
					if (sj1 >= tulprm->enssija) {
						prtosuus(&kilp, tulprm, os1, sj1);
						++*l;
						}
					}
				else
					{
					if (tulprm->yhteislahdot) {
						if (tulprm->viimos == L'K') {
							for (os1 = 0; os1 < Sarjat[kilp.sarja].ntosuus; os1++) {
								if (kilp.ostiet[Sarjat[kilp.sarja].aosuus[os1+1]].ylahto != TMAALI0) {
									prttulos(&kilp, tulprm, os1, -3);
									++*l;
									}
								}
							}
						else {
							prttulos(&kilp, tulprm, os, -3);
							++*l;
							}
						}
					else if (tulosfl &&
						(!tulprm->tarkastamattomat || !tarkastettu(&kilp, os, tulprm->viimos == L'K'))) {
						if (tulprm->viimos && tulprm->tulostettava != L'H') {
							*l += prtkaikki(&kilp,tulprm,sj1);
							}
						else {
							if (tulprm->tulostettava != L'H')
								prttulos(&kilp,tulprm, os,sj1);
							else if (kilpparam.maxnosuus == 1)
								prtosuus(&kilp, tulprm, os, sj1);
//								prtosuus(&kilp, tulprm, os, os ? kilp.ostiet[os].ossija : sj1);
							++*l;
							}
						}
					if(!mahtuusivulle(*l, (tulprm->viimos != L'K' || tulprm->tulostettava == L'H') ? 1 :
						(tulprm->tiivis ? 3 : 3+Sarjat[*srj].osuusluku), tulprm)) {
						*l = 0;
						if (tulprm->sivu_alateksti)
							kirjSivuAla(tulprm, l);
						endpage(tulprm);
						}
					}
				}
			else if (tulprm->kohde == L'X') {
			   xmltulos(kilp, sj1, tulprm); // cd == L'Y');
			   }
			else if (tulprm->kohde == L'S') {
			   liittotulos(&kilp, *srj, tulprm); // cd == L'Y');
			   }
			else if(tulprm->kohde == L'H' && tulprm->tulostettava == L'E' && !tulprm->hajontatulokset) {
				EmitHtmlTulos(kilp, sj, tulprm);
				}
			else if(tulprm->kohde == L'H') {
				enstls[*srj][os] = 10000;
				if (i == 1) {
					htmlotsikot(srj, tulprm);
					htmlsarjaotsikot(srj, tulprm, ei_lukum, nkesktot, nhyltot);
					}
				if (tulprm->hajontatulokset) {
					sj++;
					if (tulos(&kilp,os1,0) != tls0)
						sj1 = sj;
					if (sj1 > tulprm->viimsija)
						break;
					tls0 = tulos(&kilp,os1,0);
					if (sj1 >= tulprm->enssija) {
						if (tulprm->tulostettava == L'E')
							EmitHtmlOsuus(kilp, os1, sj1, tulprm);
						else
							htmlosuus(&kilp, tulprm, os1, sj1);
						tulprm->rivi++;
						}
					}
				else
					{
					if (tulprm->yhteislahdot) {
						if (tulprm->viimos == L'K') {
							for (os1 = 0; os1 < Sarjat[kilp.sarja].ntosuus; os1++) {
								if (kilp.ostiet[Sarjat[kilp.sarja].aosuus[os1+1]].ylahto != TMAALI0) {
									htmltulos(&kilp, tulprm, os1, -3);
									tulprm->rivi++;
									}
								}
							}
						else {
							htmltulos(&kilp, tulprm, os, -3);
							tulprm->rivi++;
							}
						}
					else if (tulosfl &&
						(!tulprm->tarkastamattomat || !tarkastettu(&kilp, os, tulprm->viimos == L'K'))) {
						if (tulprm->viimos && tulprm->tulostettava != L'H') {
							htmlkaikki(&kilp,tulprm,sj1);
							}
						else {
							if (tulprm->tulostettava != L'H')
								htmltulos(&kilp,tulprm, os,sj1);
							else if (kilpparam.maxnosuus == 1)
								htmlosuus(&kilp, tulprm, os, sj1);
							tulprm->rivi++;
							}
						}
					}
				}
			else if (tulprm->kohde == L'I' || tulprm->kohde == L'R' || tulprm->kohde == L'E') {
				if (tulprm->hajontatulokset) {
					sj++;
					if (tulos(&kilp,os1,0) != tls0)
						sj1 = sj;
					if (sj1 > tulprm->viimsija)
						break;
					tls0 = tulos(&kilp,os1,0);
					if (sj1 >= tulprm->enssija) {
						textosuus(&kilp, tulprm, os1, sj1);
						}
					}
				else
					{
					if (tulprm->yhteislahdot) {
						if (tulprm->viimos == L'K') {
							for (os1 = 0; os1 < Sarjat[kilp.sarja].ntosuus; os1++) {
								if (kilp.ostiet[Sarjat[kilp.sarja].aosuus[os1+1]].ylahto != TMAALI0) {
									texttulos(&kilp, tulprm, os1, 0);
									}
								}
							}
						else {
							texttulos(&kilp, tulprm, os, 0);
							}
						}
					else if (tulosfl &&
						(!tulprm->tarkastamattomat || !tarkastettu(&kilp, os, tulprm->viimos == L'K'))) {
						for (os1 = tulprm->viimos == L'K' ? 0 : os; os1 <= os; os1++) {
							if (tulprm->tulostettava != L'H')
								texttulos(&kilp, tulprm, os1, os == os1 ? sj1 : kilp.Sija(os1, 0));
							else if (kilpparam.maxnosuus == 1)
								textosuus(&kilp, tulprm, os1, os ? kilp.Sija(os1, 0) : sj1);
							}
						}
					}
				}
#ifdef _CONSOLE
			else if (tulprm->kohde == L'N') {
				if( *l == ySize-7 ) {
					LeaveCriticalSection(&tall_CriticalSection);
					ret = nayta_srj_os(*srj, os, 1) == 27;
					EnterCriticalSection(&tall_CriticalSection);
					if (ret) {
						goto lopeta;
						}
					clearframe();
					nayta_srj_os(*srj, os, 0);
					*l = 0;
					}
				if (tulprm->hajontatulokset) {
					sj++;
					if (tulos(&kilp,os1,0) != tls0)
						sj1 = sj;
					if (sj1 > tulprm->viimsija)
						break;
					tls0 = tulos(&kilp,os1,0);
					if (sj1 >= tulprm->enssija) {
						naytaosuus(&kilp, tulprm, os1, sj1, 3 + *l);
						++*l;
						}
					}
				else
					{
					if (tulprm->yhteislahdot) {
						if (tulprm->viimos == L'K') {
							for (os1 = 0; os1 < Sarjat[kilp.sarja].ntosuus; os1++) {
								if (kilp.ostiet[Sarjat[kilp.sarja].aosuus[os1+1]].ylahto != TMAALI0) {
									prttulos(&kilp, tulprm, os1, 0);
									(*l)++;
									}
								}
							}
						else {
							naytatulos(&kilp, tulprm, os, 0, 3 + *l);
							(*l)++;
							}
						}
					else if (tulosfl &&
						(!tulprm->tarkastamattomat || !tarkastettu(&kilp, os, tulprm->viimos == L'K'))) {
						if (tulprm->tulostettava != L'H')
							naytatulos(&kilp, tulprm, os, sj1, 3 + *l);
						else if (kilpparam.maxnosuus == 1)
							naytaosuus(&kilp, tulprm, os, os ? kilp.ostiet[os].ossija : sj1, 3 + *l);
						(*l)++;
						}
					}
				}
			}
		if (tulprm->kohde == L'N') {
			LeaveCriticalSection(&tall_CriticalSection);
			ret = nayta_srj_os(*srj, os, 1) == 27;
			EnterCriticalSection(&tall_CriticalSection);
			if (ret) {
				goto lopeta;
				}
			clearframe();
			}
#else  
			}
#endif // _CONSOLE
		}
lopeta:
	if (tulprm->nollaalaskuri)
		prior[*srj][os] = 0;
	if (tulprm->kohde == L'P')
		{
		if ((ret || autotl) && *l > 1) {
			if (tulprm->sivu_alateksti)
				kirjSivuAla(tulprm, l);
			endpage(tulprm);
			*l = 0;
			}
		if (!ret && (autotl || (tulprm->rajaus == L'I' || tulprm->rajaus == L'L' ||
			tulprm->rajaus == L'H' || tulprm->rajaus == L'P')))
			prior[*srj][os] = 0;
		}
	if (tulprm->kohde == L'H') {
		if (tulprm->tulostettava == L'E') {
			if (!tulprm->hajontatulokset)
				EmitHtmlSrjLoppu(tulprm);
			else if (tulprm->rivi > 0)
				tulprm->writehtml(L"\n</table>\n");
			}
		else {
			if (!(tulprm->viimos == L'K' && tulprm->tiivis)) {
				if (!tulprm->tulmuot.tauluhtml) {
					putfld(tulprm, L"</PRE>", 0, 6, 0, 0);
					}
				else {
					tulprm->tulmuot.tauluhtml = 1;
					putfld(tulprm, L"\n</table>", 0, 12, 0, 0);
					}
				endline(tulprm, 0);
				}
			}
		}
	if (tulprm->kohde == L'X') {
		xmlsrjloppu(tulprm);
		}
	tulfl = FALSE;
	tcomfl = TRUE;
	tlsSeuraava(tulprm, NULL, -1, 0, 0, 0, false, NULL, NULL, NULL, NULL);
	if (ret || tulprm->kohde == L'P')
		clrln(ySize-1);
poistu:
	LeaveCriticalSection(&tall_CriticalSection);
	return(tulostettu);
}

void yhteenveto(tulostusparamtp *tulprm)
   {
   wchar_t *ots;
   wchar_t  kaika[20], as[20], wline[200];
   char  ch2;
   int l, srj, os;

#if !defined(LUENTA) && defined(EMIT)
	int d, yla[MAXSARJALUKU][MAXOSUUSLUKU], yly[MAXSARJALUKU][MAXOSUUSLUKU],
		nemit[MAXSARJALUKU][MAXOSUUSLUKU];
	kilptietue kilp;

	memset(yla, 0, sizeof(yla));
	memset(yly, 0, sizeof(yly));
	memset(nemit, 0, sizeof(nemit));
	for (l = minkilpno; l <= maxkilpno; l++) {
		if ((d = getpos(l)) > 0) {
			GETREC(&kilp, d);
			for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				if (kilp.ostiet[os].badge[0]) {
					nemit[kilp.sarja][Sarjat[kilp.sarja].yosuus[os]]++;
					if (kilp.ostiet[os].ylahto >= -12*TUNTI) {
						if (kilp.ostiet[os].lahtolaji == 0)
							yla[kilp.sarja][os]++;
						else
							yly[kilp.sarja][os]++;
						}
					}
				}
			}
		}

   ots = L" Sarja      Ilm    Aika   Kesk    Hyl   Auki  Kärkiaika  Emit  Yht.lähtö";
#else
#ifdef LUENTA
   ots = L" Sarja      Ilm    Aika   Kesk    Hyl   Auki Koodi Ei koodia";
#else
   ots = L" Sarja      Ilm    Aika   Kesk    Hyl   Auki  Kärkiaika";
#endif
#endif
   if (tulprm->kohde  == L'N' ) {
	  clearframe();
	  vidspmsg(0,0,7,0,"YHTEENVETO");
	  vidspwmsg(2,0,7,0,ots);
	  clrln(ySize-3);
	  swprintf(wline,L"Viim. maaliaika : %s",AIKATOWSTRS(as, tmaali,t0));
	  vidspwmsg(ySize-3,49,7,0,wline);
	  l = 4;
	  srj = 0;
	  do {
		 if( nilm[srj] > 0 ) {
			swprintf(wline,L" %-11s%3d",Sarjat[srj].Sarjanimi(as),
			nilm[srj]);
			vidspwmsg(l,0,7,0,wline);
			l++;
			for (os=0; os < Sarjat[srj].ntosuus; os++) {
			   if( ntulos[srj][os][0] == 0 ) wcscpy(kaika, L"00.00.00");
			   else
				   AIKATOWSTRS(kaika, JarjTulos(0, os, 0, jalku[srj][os][0]), 0);
			   kaika[kilpparam.laika2] = 0;
//			   if (kilpparam.maxnosuus == 1)
#ifdef LUENTA
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d%7d%7d", os+1,
					  ntulos[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),nekoodi[srj][os],
					  (nilm[srj]-nekoodi[srj][os]-neil[srj][0]));
#else
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d  %s", os+1,
					  ntulos[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),kaika);
#endif
/*			   else
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d  %s", os+1,
					  ntulos2[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos2[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),kaika);
*/
#if !defined(LUENTA) && defined(EMIT)
					swprintf(wline+wcslen(wline), L"%8d%6d/%4d", nemit[srj][os], yla[srj][os], yly[srj][os]);
#endif
			   vidspwmsg(l,0,7,0,wline);
			   l++;
			   }
			}
		 if( (l >= 16) || (srj == sarjaluku - 1) ) {
			vidspmsg(ySize-3,0,7,0,"Paina <Return> jatkaaksesi");
			do { } while (readkbd(&ch2,TRUE,spoolfl) != KB_CR);
			clearframe();
			l = 4;
			vidspwmsg(2,0,7,0,ots);
			}
		 } while ( ++srj < sarjaluku );
	  }
   else {
		if (tulprm->printer == GDIPRINTER)
			initpage(tulprm,L"C");
		else
			initpage(tulprm,0);
		if (tulprm->kohde == L'H') {
			initline(tulprm);
			putfld(tulprm, L"<html>\n<body>\n<pre>\n", 0, 80, 0, 0);
			endline(tulprm, 0);
			}
	  initline(tulprm);
	  swprintf(wline,L"   Maaliaika : %-16.8s Kirjausaika : %s",
		 AIKATOWSTRS(as, tmaali,t0),wkello());
	  putfld(tulprm, wline, 0, 80, 0, 0);
	  endline(tulprm, 1);
	  putfld(tulprm, ots, 0, 80, 0, 0);
	  endline(tulprm, 0);
	  l = 4;
	  for (srj = 0; srj < sarjaluku; srj++) {
		 if( nilm[srj] > 0 ) {
			newline(tulprm, 1);
			swprintf(wline, L" %-11s%3d", ansitowcs(as, Sarjat[srj].sarjanimi, 20),
				nilm[srj]);
			putfld(tulprm, wline, 0, 80, 0, 0);
			endline(tulprm, 0);
			l++;
			l++;
			for (os=0; os < Sarjat[srj].ntosuus; os++) {
			   if( ntulos[srj][os][0] == 0 ) wcscpy(kaika, L"00.00.00");
			   else
				   AIKATOWSTRS(kaika, JarjTulos(0, os, 0, jalku[srj][os][0]), 0);
			   kaika[kilpparam.laika2] = 0;
//			   if (kilpparam.maxnosuus == 1)
#ifdef LUENTA
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d%7d%7d", os+1,
					  ntulos[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),nekoodi[srj][os],
					  (nilm[srj]-nekoodi[srj][os]-neil[srj][0]));
#else
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d%12s", os+1,
					  ntulos[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),kaika);
#endif
/*			   else
				   swprintf(wline,L"%6d. osuus%9d%7d%7d%7d%12s", os+1,
					  ntulos2[srj][os][0], nkesk[srj][os], nhyl[srj][os],
					  (nilm[srj]-ntulos2[srj][os][0]-nkesk[srj][os]-
					  nhyl[srj][os]-neil[srj][os]),kaika);
*/
#if !defined(LUENTA) && defined(EMIT)
					swprintf(wline+wcslen(wline), L"%6d%6d/%4d", nemit[srj][os], yla[srj][os], yly[srj][os]);
#endif
			   putfld(tulprm, wline, 0, 80, 0, 0);
			   endline(tulprm, 0);
			   l++;
			   }
			}
		 if(tulprm->kohde == L'P' && !mahtuusivulle(l, 4, tulprm)) {
			l = 2;
			endpage(tulprm);
			initpage(tulprm,0);
			putfld(tulprm, ots, 0, 80, 0, 0);
			endline(tulprm, 0);
			}
		 }
	  if (sarjaluku > 1) {
		 swprintf(wline,L"  Yhteensä%5d",nilmt);
		 newline(tulprm, 1);
		 putfld(tulprm, wline, 0, 80, 0, 0);
		 endline(tulprm, 0);
		for (os = 0; os < kilpparam.ntosuus; os++) {
			l = ntulost[os]+nkeskt[os]+nhylt[os]+neilt[os];
			if (l == 0) break;
			l = -l;
			for (srj = 0; srj < sarjaluku; srj++)
			   if (os < Sarjat[srj].ntosuus) 
				   l += nilm[srj];
			swprintf(wline,L"%6d. osuus%10d%8d%8d%8d", os+1,
			   ntulost[os], nkeskt[os], nhylt[os],l);
			putfld(tulprm, wline, 0, 80, 0, 0);
			endline(tulprm, 0);
			}
		 }
		if (tulprm->kohde == L'H') {
			initline(tulprm);
			putfld(tulprm, L"</pre>\n</body>\n</html>\n", 0, 80, 0, 0);
			endline(tulprm, 0);
			}
	  endpage(tulprm);
	  }
   }

void yhtveto(wchar_t kohde, wchar_t tiedlaji, wchar_t yvlaji, wchar_t *listflnm, wchar_t merkit,
	wchar_t erotin)
{
	tulostusparamtp tulprm;
	int l = 0;
	bool pageclosed = false;

	tulprm.merkit = merkit;
	tulprm.erotin = erotin;

	wcscpy(listfname, listflnm);
	if (kohde == L'P' && tulprm.lstf == NULL) {
		tulprm.printer = GDIPRINTER;
		if ((tulprm.lstf = openprfile(listfname, 1, TRUE, FALSE, trlate, FALSE))
			== NULL) {
			writeerror_w(L"",0);
			return;
			}
		}

	if (kohde == L'P') {
		tulprm.tulmuot = tulosmuot;
		tulprm.tulmuot.amarg = lsttulparam.tulmuot.amarg;
		}
	else
		tulprm.tulmuot = filetulosmuot;

	if (tulprm.lstf && tulprm.lstf->prno == 2)
		initgdiprinter(&tulprm);

	if (kohde == L'I') {
		kohde = tiedlaji;
		}

	tulprm.kohde = kohde;

	if (wcswcind(kohde, L"IRJEHFOSWXZ") >= 0 && tulprm.lstf == NULL) {
		if (avaa_tiedosto_nm(listflnm, &tulprm))
			goto loppu2;
		}

	yhteenveto(&tulprm);

	if (kohde == L'P') {
		endpage(&tulprm);
		}
	if (wcswcind(kohde, L"IREHSXZ") >= 0) {
		closeprfile(tulprm.lstf);
		}
	loppu2:
	if (kohde == L'P') {
		enddocGDI(tulprm.lstf);
		closeprfile(tulprm.lstf);
		tulprm.lstf = NULL;
		}

}

void kaikki_tiedostoon(tulostusparamtp *tulprm, int sarja, int ens)
   {
   wchar_t *ptr, *pp, as[20], ch, line[500], st[80], seura[LSEURA+1];
   char tc;
   int np,srj,os,osv,i,sj,sjy,nkesktot,nhyltot;
   int p, lj;
   kilptietue kilp;
   static int vaihda = FALSE;
   int nalku = 0, enssarja = 1, n_henk;
   TULOS tls,tulos0;
   static wchar_t ca = L'V';
   static wchar_t cs = L'V';
   int osv2, is, tulosfl;

#ifdef _CONSOLE
   header=L"TIEDOSTOON TULOSTUS";
   if (kilpparam.maxnosuus == 1) {
	   wselectopt(L"Tulosta V)aihtojen ajat, O)suusajat, E)i kumpiakaan", L"VOE", &ca);
	   wselectopt(L"Tulosta V)aihtojen sijat, O)suussijat, E)i kumpiakaan", L"VOE", &cs);
	   }
   else {
	   ca = L' ';
	   cs = L' ';
	   wselectopt(L"Tulosta O)suusajat, E)i osuusaikoja", L"OE", &ca);
	   wselectopt(L"Tulosta vaihtojen S)ijat, vaihtojen A)jat, M)olemmat, E)i kumpiakaan",
		  L"SAME", &cs);
	   }
   ch = vaihda ? L'K' : L'E';
   wselectopt(L"Kirjoitetaanko etunimi ensin (K/E)",L"KE",&ch);
   vaihda = (ch == L'K');
   srj = 0;
   sarja = -1;
#else
	enssarja = ens;
	srj = sarja;
	if (tulprm->optiot & 0x01000000)
		ca = L'V';
	else if (tulprm->optiot & 0x02000000)
		ca = L'O';
	else
		ca = L'E';
	if (tulprm->optiot & 0x04000000)
		cs = L'V';
	else if (tulprm->optiot & 0x08000000)
		cs = L'O';
	else
		cs = L'E';
	vaihda = tulprm->tulmuot.etusuku;
#endif
   tulprm->hksijafl = cs == L'O';
   ptr = line;
   if (tulprm->rajaus == L'P')
	   nalku = tulprm->enssija-1;
   do {
#ifdef _CONSOLE
	  scpclr();
	  kehys(0);
	  do {
		 if (sarjaluku > 1) {
			vidspwmsg(ySize-3,0,7,0,L"       Anna sarja :             "
			   L"  <Esc> : Poistu päävalintaan");
			srj = luesarja(Sarjat[srj].sarjanimi, &tc);
			if (tc == ESC) goto escape;
			}
		 else srj = 0;
		 clrln(ySize-3);
#endif
		 osv = Sarjat[srj].ntosuus - 1;
		 osv2 = Sarjat[srj].osuusluku - 1;
		 if (tulprm->rajaus == L'P')
			np = min(tulprm->viimsija, ntulos[srj][osv][0]);
		 else
			np = ntulos[srj][0][0] + nkesk[srj][0] + nhyl[srj][0];
		 if (np <= nalku ||
			(tulprm->seuratulokset && !onko_seura(srj)) ||
			(tulprm->piiritulokset && !onko_piiri(srj))) {
			if (np <= nalku)
			   writeerror("Ei tulostettavia, tyhjä sarja ?",0);
			if (sarjaluku == 1 || sarja == srj)
				goto escape;
			if (srj < sarjaluku - 1) srj++;
			}
#ifdef _CONSOLE
		 } while (np <= nalku);
#endif
	  EnterCriticalSection(&tall_CriticalSection);
	  ossijat(tulprm, srj, -1, NULL, &nhyltot, &nkesktot);
	  if (enssarja) {
		 tulprm->lstf->u.Fl->WriteLine(kilpailu);
		 tulprm->lstf->u.Fl->WriteLine(L"\n");
		 enssarja = 0;
		 }
	  swprintf(line, L"%s (%s", Sarjat[srj].Sarjanimi(), Sarjat[srj].Matka(0));
//      nhyltot = nhyl[srj][0];
	  for (os = 1; os <= osv; os++) {
//         nhyltot += nhyl[srj][os];
		  wcscat(line, L"+");
		  wcscat(line, Sarjat[srj].Matka(os));
         }
//      nkesktot = nkesk[srj][osv] + neil[srj][osv] -
//         neil[srj][0] - nhyltot + nhyl[srj][osv];
      swprintf(line+wcslen(line), L" km) (%d-%d-%d): \n",
		 ntulos[srj][0][0] + nkesk[srj][0] + nhyl[srj][0],
         nkesktot, nhyltot);
	  tulprm->lstf->u.Fl->WriteLine(line);
	  line[0] = 0;
	  ptr = line;
      tulos0 = 0;
	  p = -1;
	  lj = -1;
      for (i = 0; i < np; i++) {
		  p = askellasarja(srj, osv, 0, 0, p, &lj);
		  if (p < 0 || (lj > 0 && wcswcind(tulprm->rajaus, L"HP") >= 0))
			  break;
		  if (i < nalku)
			  continue;
         GETREC(&kilp, p);
		 if (stschind(kilp.Tark(0, 1), "ES") >= 0)
			 break;
         tulosfl = 0;
         if ((!tulprm->seuratulokset && !tulprm->piiritulokset)
			|| (tulprm->piiritulokset && kilp.piiri > 0 &&
               kilp.piiri <= piiriluku && piirifl[kilp.piiri - 1]))
            tulosfl = 1;
         else {
            if (tulprm->seuratulokset) {
				for (is = 0; seuranimi[is][0] && is < SEURALISTA; is++) {
					if (!wcscmp(seuranimi[is], upcasewstr(kilp.Seura(seura)))) {
						tulosfl = 1;
						break;
						}
					}
				}
			}
         if (!tulosfl)
            continue;
         if (kilp.tHyv(osv) && (tls = kilp.tTulos(osv, 0)) != 0) {
            if (tls != tulos0)
				sjy = i + 1;
            tulos0 = tls;
			_itow(sjy,ptr,10);
			ptr++;
            if (sjy > 9) ptr++;
            if (sjy > 99) ptr++;
            if (sjy > 999) ptr++;
			*(ptr++) = L')';
            *(ptr++) = L' ';
            AIKATOWSTRS(as,tls,0);
            as[kilpparam.laika2] = 0;
			elimwz(as);
            elimwbl(as);
			}
		 else {
			switch (kilp.Tark(-1, 0)) {
			   case 'H':
				  wcscpy(as, L"Hyl.");
				  break;
			   default :
                  wcscpy(as, L"Kesk.");
				  break;
			   }
            }
		 kilp.Joukkue(ptr);
		 ptr += wcslen(ptr);
         if (kilp.maa[0] && kilp.maa[0] != L' ') {
			*(ptr++) = L',';
            *(ptr++) = L' ';
            ptr = wcpcpy(ptr, kilp.Maa());
            }
         *(ptr++) = L' ';
         ptr = wcpcpy(ptr, as);
         n_henk = 0;
         for (os = 0; os <= osv2; os++) {
			if (stschind(kilp.Tark(os, 1), "ES") >= 0)
				break;
			if (!n_henk) {
               *(ptr++) = L' ';
               *(ptr++) = L'(';
               }
			n_henk++;
			kilp.Nimi(st, OSNIMIL, os, vaihda ? -1 : 1);
            elimwbl(st);
			locasewname(st);
            wcscpy(ptr, st);
            ptr += wcslen(ptr);
			if (kilpparam.maxnosuus)
				*(ptr++) = ' ';
			else
				if (ca != L'E')
					*(ptr++) = L' ';
			switch (kilp.Tark(os, 0)) {
               case '-' :
			   case 'I' :
			   case 'T' :
                  if (ca != L'E') {
                     if (ca == L'V')
						AIKATOWSTRS(as, kilp.Tulos(os, 0), 0);
					 else
						AIKATOWSTRS(as, kilp.osTulos(os, 0),0);
                     as[kilpparam.laika2] = 0;
                     elimwz(as);
					 elimwbl(as);
					 ptr = wcpcpy(ptr, as);
					 if (kilpparam.maxnosuus == 1) {
	                     if (cs != L'E') 
							 *(ptr++) = L'/';
						 }
					 else
						 if (cs != L'E' && Sarjat[srj].aosuus[Sarjat[srj].yosuus[os]+1] == os)
							*(ptr++) = L'/';
                     }
				 if (kilpparam.maxnosuus == 1) {
					  if (cs != L'E') {
						 if (cs == L'V')
							sj = kilp.Sija(os, 0);
						 else {
							if (os)
							   sj = kilp.ostiet[os].ossija;
							else
							   sj = kilp.Sija(0, 0);
							}
						}
					  else {
						  if ((cs == L'A' || cs == L'M') &&
							 Sarjat[srj].aosuus[Sarjat[srj].yosuus[os]+1] == os &&
							 (os || ca == L'E')) {
							 if (ca == L'E') *(ptr++) = L' ';
							 AIKATOWSTRS(as,kilp.Tulos(os, 0),0);
							 as[kilpparam.laika2] = 0;
							 elimwz(as);
							 elimwbl(as);
							 ptr = wcpcpy(ptr, as);
							 if (cs == L'M') *(ptr++) = L'/';
							 }
						  if ((cs == L'S' || cs == L'M') &&
							 Sarjat[srj].aosuus[Sarjat[srj].yosuus[os]+1] == os) {
							 if (ca == L'E' && cs == L'S') 
								 *(ptr++) = L' ';
							 sj = kilp.Sija(os, 0);
							}
						}
					_itow(sj,ptr,10);
					if (sj >= 1000) ptr++;
					if (sj >= 100) ptr++;
					if (sj >= 10) ptr++;
					ptr++;
					}
				  break;
               case 'H':
			   case 'K':
				  *(ptr++) = kilp.wTark(os, 0);
				  break;
				  }
			*(ptr++) = L',';
			*(ptr++) = L' ';
            }
         ptr--;
         ptr--;
         if (n_henk) *(ptr++) = L')';
		 if (i < np-1) {
            *(ptr++) = L',';
			*(ptr++) = L' ';
            }
         else *(ptr++) = L'.';
         *(ptr++) = L' ';
         *ptr = 0;
         while (ptr - line > 70) {
			pp = line + 70;
            while (pp > line && *pp != L' ') pp--;
			*pp = 0;
			tulprm->lstf->u.Fl->WriteLine(line);
			tulprm->lstf->u.Fl->WriteLine(L"\n");
            wmemmove(line, pp+1, (unsigned) (ptr - pp));
            ptr -= (pp-line+1);
			}
         }
      *(ptr++) = L'\n';
	  *(ptr++) = 0;
	  tulprm->lstf->u.Fl->WriteLine(line);
	  LeaveCriticalSection(&tall_CriticalSection);
#ifndef _CONSOLE
	  break;
#endif
	  } while (sarjaluku > 1 && srj++ >= 0);
   escape:
   return;
   }

static int htmlalkufl;

//void htmlalku(wchar_t *wtitle, wchar_t *wheader, int frame, int emitvali, tulostusparamtp *tulprm)
void htmlalku(wchar_t *wtitle, wchar_t *wheader, int frame, tulostusparamtp *tulprm)
	{
	TextFl *hfl;
	wchar_t rivi[200], fnm[13] = L"HTMLALKY.TXT";

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
		html_avaus(tulprm, frame ? L"" : wtitle, 1);
		}
	if (tulprm->yksihtml) {
		tulprm->writehtml(L"<H2 CLASS=otsikko>");
		tulprm->writehtml(wheader);
		tulprm->writehtml(L"</H2>\n");
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
	  tulprm->writehtml(L"</BODY>\n</HTML>\n");
   htmlalkufl = 0;
   }

#ifdef AUTOFILE
void startaf(INT kysy)
   {
   wchar_t ch, *p;
   TextFl *aflst;

   if (aftulparam.kohde == L'H') {
		if (!aftulparam.yksihtml) {
			aflst = new TextFl(autofileparam.aflstnm, L"rt");
			if (!aflst || !aflst->IsOpen())
				aftulparam.yksihtml = true;
			if (aflst)
				delete aflst;
			aflst = NULL;
			}

	   if (kysy) {
//		   ch = aftulparam.yksihtml ? L'Y' : L'S';
//		   wselectopt(L"Y)hteen tiedostoon vai S)arjat erikseen", L"YS", &ch);
//		   aftulparam.yksihtml = ch == L'Y';
		   ch = aftulparam.htmlkno ? L'K' : L'E';
		   wselectopt(L"Näytetäänkö kilpailunumerot (K/E)", L"KE", &ch);
		   aftulparam.htmlkno = ch == L'K';
		   }
	   aftulparam.tulmuot = filetulosmuot;
	   if (aftulparam.yksihtml) {
		   if (!autofileparam.wtitlea[0]) {
			   if (aftulparam.title[0])
					wcsncpy(autofileparam.wtitlea, aftulparam.title, 76);
			   else if (paaots[0])
				   wcsncpy(autofileparam.wtitlea, paaots, 76);
			   else
				   wcsncpy(autofileparam.wtitlea, sarjaots, 76);
			   }
		   else
			  for (p = autofileparam.wtitlea; *p; p++) if (*p == L'_') *p = L' ';
		   autofileparam.wtitlea[76] = 0;
		   elimwbl(autofileparam.wtitlea);
		   if (!autofileparam.wheadera[0]) {
			  if (aftulparam.header[0])
				   wcsncpy(autofileparam.wheadera, aftulparam.header, 76);
			   else if (paaots[0])
				   wcsncpy(autofileparam.wheadera, paaots, 76);
			   else
				   wcsncpy(autofileparam.wheadera, sarjaots, 76);
			   }
		   else
			   for (p = autofileparam.wheadera; *p; p++) if (*p == L'_') *p = L' ';
		   autofileparam.wheadera[76] = 0;
		   elimwbl(autofileparam.wheadera);
		   }
		}
#if defined(TCPSIIRTO)
   if (fileyht >= 0) {
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
		 while (comfl && !af_tied->Feof()) {
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

int autofile(void)
	{
	int l = 0, sv = 0, yht = 0, srj;
	wchar_t laajuus = L'L';
	TextFl *aflst = 0;
	bool kirjoitettu = false;
//	wchar_t xmllaji;
#if defined(TCPSIIRTO)
	static HTHREAD FiletcpThread;
#endif

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
#if defined(TCPSIIRTOxx)
	if (fileyht >= 0) {
		EnterCriticalSection(&tcp_CriticalSection[fileyht]);
		}
#endif
	EnterCriticalSection(&autotul_CriticalSection);
	autofileparam.af_flag = 1;
//	aftulparam.ansi = true;
	if (!aftulparam.yksihtml)
		aflst = new TextFl(autofileparam.aflstnm, L"rt");
	if (!aflst || !aflst->IsOpen()) {
		if (aflst)
			delete aflst;

		aftulparam.lstf = openprfile(autofileparam.afname, -1, 1, 2, (char *) &aftulparam.merkit, 0);
		for (srj = 0; srj < sarjaluku+nsarjayhd; srj++)
			if (aftulparam.sarjalista[srj])
				break;
		if (srj == sarjaluku+nsarjayhd) {
			wmemset((wchar_t *)aftulparam.sarjalista, 1, sizeof(aftulparam.sarjalista)/2);
			}
		if (!aftulparam.lstf) {
			writeerror_w(L"Virhe autotulostustiedostoa avattaessa", 0);
			autofileparam.af_flag = 0;
			LeaveCriticalSection(&autotul_CriticalSection);
			return(1);
			}
		if (aftulparam.kohde == L'X') {
	//		writexml(&aftulparam, 0, 1);
			xmlloppu(&aftulparam);
			closeprfile(aftulparam.lstf);
			autofileparam.af_flag = 0;
			LeaveCriticalSection(&autotul_CriticalSection);
			return(0);
			}

		if (aftulparam.kohde == L'H') {
			htmlalku(aftulparam.title, aftulparam.header, 0, &aftulparam);
			for (int srj = 0; srj < sarjaluku; srj++) {
				if (aftulparam.sarjalista[srj] && ntulos[srj][aftulparam.osuus][aftulparam.piste]) {
					tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
					kirjoitettu = true;
					}
				}
			htmlloppu(&aftulparam);
			}
		closeprfile(aftulparam.lstf);
		}
// #ifdef EI_OLE
	else {
		wchar_t buf[100], *p, *p1;
		int nt;
//		aftulparam.yksihtml = false;
		for (int srj = -1;;) {
			p1 = NULL;
			if (aflst->IsOpen() && aftulparam.kohde == L'H' && !aftulparam.yksihtml) {
				if (aflst->Feof())
					break;
				if (aflst->ReadLine(buf, 98) == NULL)
					continue;
				p = wcstok(buf, L" ;\t\n");
				if (!p || (srj = haesarja_w(p, true)) < 0)
					continue;
				if ((p = wcstok(NULL, L" ;\t\n")) == 0)
					continue;
				aftulparam.osuus = _wtoi(p) - 1;
				if ((p = wcstok(NULL, L" ;\t\n")) == 0)
					continue;
				aftulparam.piste = _wtoi(p);
				p1 = wcstok(NULL, L" ;\t\n");
				}
			else {
				srj++;
				if (srj >= sarjaluku+nsarjayhd)
					break;
//				p = Sarjat[srj].sarjanimi;
				aftulparam.osuus = Sarjat[srj].ntosuus - 1;
				aftulparam.viimos = L'K';
				aftulparam.piste = 0;
//         p1 = L"";
				}
			if (aftulparam.kohde == L'X') {
/*
				nt = 0;
				if (xmlsarja < 0 || srj == xmlsarja) {
					if (!yht) {
						for (i = 0; i <= Sarjat[srj].vaikaluku; i++) {
							if (i == 0 && aftulparam.rajaus == L'I')
								nt += nilm[srj];
							else
//								nt += ntulos[srj][i];
							}
						}
					else
					nt += ntulos[srj][kilpparam.valuku+1];
					}
*/
				}
			else {
				if (autofileparam.vainMuutt && fprior[srj][aftulparam.osuus] == 0)
					continue;
				fprior[srj][aftulparam.osuus] = 0;
				nt = ntulos[srj][aftulparam.osuus][aftulparam.piste];
				}
			if (nt) {
				if (aftulparam.kohde == L'H' && !aftulparam.yksihtml) {
					if (aftulparam.lstf) {
						htmlloppu(&aftulparam);
						closeprfile(aftulparam.lstf);
						aftulparam.lstf = 0;
						}
					if (p1 == NULL)
						p1 = htmlSrjFName(&aftulparam, autofileparam.afname, srj);
					if ((aftulparam.lstf = openprfile(p1, -1, 1, 2,	(char *) &aftulparam.merkit, 0)) == NULL)
						continue;
					htmlalku(0, 0, 1, &aftulparam);
					}
				if (aftulparam.kohde == L'X') {
/*
					xmllaji = L'X';
					if (yht k_pv) {
						aftulparam.yhttuljarj = true;
						aftulparam.yhttulpaino = true;
						aftulparam.vaihelkm = 1;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						}
					else {
						aftulparam.yhttuljarj = false;
						aftulparam.yhttulpaino = false;
						aftulparam.vaihelkm = 1;
						tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
						}
*/
					}
				else {
					tulostasarja(&srj, &aftulparam, &l, &sv, TRUE);
					kirjoitettu = true;
					}
				}
			}
		};
	if (aflst)
		delete aflst;
	switch (aftulparam.kohde) {
		case L'H':
			htmlloppu(&aftulparam);
			break;
		case L'X':
//			xmlloppu(&aftulparam);
			break;
		case L'T':
			break;
		}
#if defined(TCPSIIRTO)
	if (fileyht >= 0) {
#ifdef TCPSIIRTO_2
		af_tied->Close();
		af_tied->Open(L"r");
#endif
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
//#endif
	if (autofileparam.autokomentolaji == 1 && autofileparam.autokomento[0]) {
		wchar_t cst[300] = L"start /min ";
		wcscat(cst, autofileparam.autokomento);
		wcscat(cst, L" >NUL 2> cmderrors.txt");
		_wsystem(cst);
//		_wspawnl(P_DETACH, L"cmd.exe", L"/C", cst, NULL);
		}
#ifdef __BORLANDC__
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

int autofile(void)
   {
   int l = 0, sv = 0;
   if (ntulos[afsrj][afva])
	  tulostasarja(&afsrj, afva, L'V', L'P', L'E', cda, cja, k_pv, afnparas, 0, &l, &sv, 1);
   return(0);
   }

#endif

#ifdef JOUKKUEPISTEET
#ifdef _CONSOLExx
void joukkuepistemaarittelyt(void);

void joukkuepisteval(tulostusparamtp *tulprm)
   {
   typedef struct {
	  INT piste;
	  wchar_t seura[LSEURA+1];
	  } pistetp;
   pistetp *pistetaul;
   wchar_t seura[LSEURA+1], line[50], ch, Buf[100];
   char ch2;
   INT srj, npiste, p, sj, sj1, is, nseura = 0, l;
   INT32 tls1;
   TextFl *psrjfile;
   kilptietue kilp;
   wchar_t co;

   do {
	  wselectopt(L"T)ulosta pisteet, L)ue määrittelyt, M)äärittele, Esc: peruuta",
		 L"TLM\x1b", &ch);
	  switch (ch) {
		 case L'L' :
			luePisteSrj(0);
			clrln(ySize-3);
			vidspwmsg(ySize-3,0,7,0,L"Paina jotain näppäintä");
			readkbd(&ch2,TRUE,spoolfl);
			break;
		 case L'M' :
			pistemaarittelyt();
			break;
		 }
	  } while (ch == L'L' || ch == L'M');
   if (ch == ESC) return;
   }
#endif

#ifndef _CONSOLE

int luePisteSrj(wchar_t *flnm)
{
	wchar_t msg[250], Buf[120], line[100], filenm[200] = L"pistesrj.lst";
	TextFl *psrjfile;

	if (flnm && flnm[0])
		wcsncpy(filenm, flnm, 199);
	psrjfile = new TextFl(filenm, L"rt");
	if (!psrjfile->IsOpen()) {
	   swprintf(msg, L"Tiedostoa %s ei löydy", filenm);
	   writeerror_w(msg, 0, true);
	   }
	else {
		int l, srj, npiste;

		for (srj = 0; srj < sarjaluku; srj++) {
			Sarjat[srj].npist[k_pv] = 0;
			}
#ifdef _CONSOLE
		for (l = 3; l < ySize-4; l++) clrtxt(l, 0, 64);
		l = 3;
#endif
		while (!psrjfile->Feof()) {
			psrjfile->ReadLine(Buf, 98);
		   if (swscanf(Buf, L"%s %d\n", line, &npiste) < 2)
				break;
		   if ((srj = haesarja_w(line, true)) >= 0) {
			   Sarjat[srj].npist[k_pv] = (wchar_t) npiste;
#ifdef _CONSOLE
			   if (l < ySize-4) {
				 vidspwmsg(l, 0, 7, 0, Sarjat[srj].sarjanimi);
				 vidint(l, 12, 3, npiste);
				 }
			  l++;
#endif
			  }
		   }
		}
	if (psrjfile)
		delete psrjfile;
	return(0);
	}

void joukkuepisteet(bool naytolle, tulostusparamtp *tulprm)
   {
   typedef struct {
	  INT piste;
	  wchar_t seura[LSEURA+1];
	  } pistetp;
   pistetp *pistetaul;
   wchar_t seura[LSEURA+1], line[50], ch, Buf[100];
   char ch2;
   INT srj, npiste, p, sj, sj1, is, nseura = 0, l;
   INT32 tls1;
   TextFl *psrjfile;
   kilptietue kilp;

   luePisteSrj(0);
   if ((pistetaul = (pistetp *)calloc((MAXNSEURA+1), sizeof(pistetp))) == NULL)
	  return;
   for (srj = 0; srj < sarjaluku; srj++) {
	  npiste = min(ntulos[srj][0], Sarjat[srj].npist[k_pv]);
	  if (!npiste)
		continue;
	  EnterCriticalSection(&tall_CriticalSection);
	  p = jalku[srj][0];
	  sj = 1;
	  tls1 = 0;
	  while (p >= 0) {
		 kilp.GETREC(p);
		 if (tls1 != kilp.tulos_pv(k_pv, true)) {
			sj1 = sj;
			tls1 = kilp.tulos_pv(k_pv, true);
			}
		 if (sj1 > npiste) break;
			wmemcpy(seura, kilp.seuratieto(0),
				kilpparam.seuralyh_on ? kilpparam.lseuralyh : kilpparam.lseura+1);
		 for (is = 0; pistetaul[is].piste; is++) {
			if (!wcscmp(seura, pistetaul[is].seura)) break;
			}
		 if (!pistetaul[is].piste) {
			wcscpy(pistetaul[is].seura, seura);
			nseura++;
			}
		 pistetaul[is].piste += npiste + 1 - sj1 + (sj1 == 1);
		 p = seuraava(0, 0, p);
		 sj++;
		 }
	  LeaveCriticalSection(&tall_CriticalSection);
	  }
   l = 0;
   if (nseura) {
	  qsortint((void *) pistetaul, nseura, sizeof(pistetp), 0,  sizeof(pistetaul[0].piste)==4);
	  npiste = 0;
	 if (naytolle) {
		if (!FormMsgs)
			FormMsgs = new TFormMsgs(FormWK);
		FormMsgs->Memo1->Clear();
		FormMsgs->Memo1->Font->Name = L"Lucida Console";
		FormMsgs->Memo1->Font->Size = 10;
		swprintf(line,L"   Pistetilanne        %s", wkello());
		FormMsgs->Memo1->Lines->Add(line);
		FormMsgs->Memo1->Lines->Add(L"  ");
		}
	  else if (tulprm->kohde == 'P') {
		 initpage(tulprm, L"C");
		 EnterCriticalSection(&print_CriticalSection);
		 }
	  else if (tulprm->kohde == 'H') {
		   tulprm->writehtml(L"<pre>\n");
		   swprintf(line,L"   Pistetilanne        %s\n\n", wkello());
		   tulprm->writehtml(line);
		   }
//	  if (tulprm->kohde != L'P')
//		   for (is = 3; is <= ySize-5; is++) clrtxt(is,0,65);
	  for (is = nseura-1; is >= 0; is--) {
		 if (npiste != pistetaul[is].piste) {
			sj1 = nseura - is;
			npiste = pistetaul[is].piste;
			}
		 if (naytolle) {
			swprintf(line, L"   %3d.  %-12.12s  %4d", sj1,
			   pistetaul[is].seura, pistetaul[is].piste);
			FormMsgs->Memo1->Lines->Add(line);
			FormMsgs->Show();
			if (FormMsgs->WindowState == wsMinimized)
				FormMsgs->WindowState = wsNormal;
			FormMsgs->BringToFront();
			}
		 else if (tulprm->kohde == L'P') {
			if (!mahtuusivulle(l, 1, tulprm)) {
			   l = 0;
			   endpage(tulprm);
			   }
			if (l == 0) {
				swprintf(line,L"   Pistetilanne        %s", wkello());
				putfld(tulprm, line, 10, 80, 0, 0);
				endline(tulprm, 1);
			   l = 2;
			   }
			swprintf(line, L"   %3d.  %-12.12s  %4d", sj1,
			   pistetaul[is].seura, pistetaul[is].piste);
			putfld(tulprm, line, 10, 80, 0, 0);
			endline(tulprm, 0);
			l++;
			}
		 else if (tulprm->kohde == L'H') {
			swprintf(line, L"   %3d.  %-12.12s  %4d\n", sj1,
			   pistetaul[is].seura, pistetaul[is].piste);
		   tulprm->writehtml(line);
		   }
/*		 else if  {
			if (l > 0 && (l % (ySize-7)) == 0) {
			   vidspwmsg(ySize-3,13,7,0,L"Paina <Return> jatkaaksesi tai "
				  L"<Esc> keskeyttääksesi");
			   sccurset(ySize-3,66);

			   do {
				  ch = readkbd(&ch2,TRUE,spoolfl);
				  } while (ch != KB_CR && ch != 27);
			   for (l = 3; l <= ySize-5; l++) clrtxt(l,0,65);
			   if( ch == 27 ) break;
			   clrln(ySize-3);
			   l = 0;
			   }
			swprintf(line, L"   %3d.  %-12.12s  %4d", sj1,
			   pistetaul[is].seura, pistetaul[is].piste);
			vidspwmsg(3 + (l % (ySize-7)), 3, 7, 0, line);
			l++;
			}
*/
		 }
	  if (naytolle) {
			FormMsgs->Show();
			if (FormMsgs->WindowState == wsMinimized)
				FormMsgs->WindowState = wsNormal;
			FormMsgs->BringToFront();
			}
	  else if (tulprm->kohde == L'P') {
		  enddoc(tulprm);
		  LeaveCriticalSection(&print_CriticalSection);
		  }
	  else if (tulprm->kohde == 'H') {
		   tulprm->writehtml(L"</pre>\n");
		   tulprm->writehtml(line);
		   }
	  else {
#ifdef _CONSOLE
		 vidspwmsg(ySize-3,13,7,0,L"Paina <Return> jatkaaksesi");
		 sccurset(ySize-3,66);
		 do {
			char che;
			ch = readkbd(&che,TRUE,spoolfl);
			} while (ch != KB_CR && ch != 27);
		 clrln(ySize-3);
#endif
		 }
	  }
   free(pistetaul);
   }
#endif

#endif // !_CONSOLE

#ifdef SEURATUL
void seuralista(void);

void seuraluettelo(void)
   {
   sraluettp  *lp;
   int i;
   INT ns = 0, v, lseura;
   kilptietue kilp;
   wchar_t  *p;
   wchar_t seura[LSEURA+1];

   if (seurat) return;
   if ((seurat = (sraluettp *) calloc(MAXNSEURA+2, sizeof(sraluettp))) == NULL) {
	  writeerror_w(L"Ei tilaa seuraluettelolle", 0);
	  return;
	  }
   lseura = kilpparam.seuralyh_on ? kilpparam.lseuralyh : kilpparam.lseura;
   seura[kilpparam.lseura] = 0;
   for (i = 0; i < nrec-1; i++) {
	  kilp.GETREC(i+1);
	  if (kilp.kilpstatus == 0 && kilp.id() != 0) {
		 lp = seurat;
		 p = lp->seura;
		 v = 0;
		 wmemcpy(seura, kilp.seuratieto(0),	lseura);
		 upcasewstr(seura);
		 while (p && (v = wcscmp(p, seura)) < 0) p = (++lp)->seura;
		 if ((!p || v > 0) && seura[0]) {
			if (ns < MAXNSEURA && (p = (wchar_t *) calloc(lseura+1, 2)) != NULL) {
			   wmemmove((wchar_t  *)lp+1, (wchar_t  *)lp,
				  (int)((wchar_t  *) (seurat+ns) - (wchar_t  *) lp));
			   lp->seura = p;
			   wmemcpy(p, seura, lseura+1);
			   ns++;
			   }
			else {
			   writeerror_w(L"Ei tilaa täydelliselle seuraluettelolle", 0);
			   break;
			   }
			}
		 }
	  }
   return;
   }

void haeseura(wchar_t *seura);
#endif

void luepiirit(wchar_t *flname)
   {
   TextFl *piirifile;
   wchar_t line[61], *p, *flnm0 = L"PIIRIT.LST";
   INT k;

   piirifile = new TextFl(flname != NULL ? flname : flnm0, L"rt");
   if (piirifile->IsOpen()) {
	  for (;;) {
		 if (piirifile->ReadLine(line, 60) == NULL)
			break;
		 if (line[wcslen(line)-1] == L'\n')
			line[wcslen(line)-1] = 0;
		 p = wcstok(line, L" \t");
		 if ((k = _wtoi(p)) == 0)
			break;
		 if (k >= piiriluku)
			continue;
		 for (p = line + wcslen(line) + 1; isspace(*p) ; p++) ;
		 if (!*p)
			break;
		 piirinimi[k-1] = (wchar_t *) malloc(wcslen(p) + 1);
		 wcscpy(piirinimi[k-1], p);
		 }
	  }
   delete piirifile;
	}

#if defined(JOUKKUETUL) && !defined(AMPUSUUNN)

#define JKOKO 10

typedef struct {
   INT32 tls;
   INT jasenet[JKOKO];
   INT jnro;
   wchar_t seura[LSEURA+1];
   } jluettp;

static int sijaluvut;

void joukkuetul(tulostusparamtp *tulprm)
   {
   INT32 tls0 = 0;
   wchar_t st[120], stsj[6], st2[14], st1[60], tc, ch, wbuf[80], co;
   INT i, j, jno, njoukk, maxjoukk, n, p, d, sj, srj, aos;
   jluettp  huge *joukk, huge *jk;
   kilptietue kilp;
   static INT jkoko = JKOKO;
   static int ytfl = 0;
   TextFl *pistefile;
   wchar_t pistefilenm[] = L"SIJAPISTEET.LST";
   int sijapisteet[1000];
#ifdef ECROSS
	static wchar_t srlj = L'U';
#else
	static wchar_t srlj = L'Y';
#endif

	co = tulprm->kohde;
	if (co != L'P') {
		avaa_tiedosto(tulprm);
		tulprm->writehtml(L"<!DOCTYPE html>\n<html>\n<body>\n");
		}
	if (k_pv > 0) {
		ch = ytfl ? L'Y' : L'P';
		wselectopt(L"Y)hteistuloksista, P)äivän tuloksista", L"YP", &ch);
		ytfl = ch == L'Y';
		}
	while (1) {
		clrln(ySize-3);
		vidspwmsg(ySize-3,0,7,0,L"       Anna sarja :               "
			L"<F4>:Yhdistelmä, <Esc>:Poistu");
		sarjayhdfl = 0;
		srj = luesarja(L"", &tc);
		if (tc == 204) {
			srj = sarjayhdistelma();
			}
		if (tc == ESC || srj < 0) break;
		wselectopt(L"U)rheiluseurat, muut Y)hteisöt, K)aikki", L"UYK", &srlj);
#if JKOKO > 3
		ch = L'0' + jkoko;
		wselectopt(L"Anna joukkueen koko", L"234567", &ch);
		jkoko = ch - L'0';
		if (jkoko > JKOKO)
			jkoko = JKOKO;
#endif
		ch = sijaluvut ? L'S' : L'A';
		wselectopt(L"A)ikojen / S)ijoitusten mukainen järjestys", L"AS", &ch);
		sijaluvut = (ch == 'S');
		maxjoukk = ntulos[srj][0]/2+200;
		joukk = (jluettp *) calloc(maxjoukk, sizeof(jluettp));
		if (!joukk) {
			writeerror_w(L"Muisti ei riitä joukkuetuloksille", 0);
			break;
			}
		njoukk = 0;
		if (sijaluvut) {
			EnterCriticalSection(&tall_CriticalSection);
			for (j = 0; j < sarjaluku; j++)
				sijat(j);
			LeaveCriticalSection(&tall_CriticalSection);
			for (j = 0; j < (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])); j++)
				sijapisteet[j] = j+1;
			if ((pistefile = new TextFl(pistefilenm, L"r")) != NULL) {
				j = 0;
				while (!pistefile->Feof()) {
					pistefile->ReadLine(st, sizeof(st)/2-1);
					if (wcslen(st) > 1)
						sijapisteet[j++] = _wtoi(st);
					}
				delete pistefile;
				}
			}
		lue_sr_nimet(1);
		EnterCriticalSection(&tall_CriticalSection);
		if (ytfl)
			p = JosAlku(srj, os, &aos);
		else
			p = jalku[srj][os][0];
		while (p >= 0) {
			d = p;
			kilp.GETREC(d);
			p = seuraava(0, ytfl ? kilpparam.valuku+1 : 0, p);
			if (!kilp.seuratieto(0)[0] || (srlj == 'Y' && srapiiri(&kilp) >= 30)
				|| (srlj == 'U' && srapiiri(&kilp) < 30))
				continue;
			jno = 1;
			for (jk = joukk; jk < joukk+njoukk; jk++) {
				if (!jk->seura[0]) break;
				if (wcscmp(jk->seura, kilp.seuratieto(0))) continue;
				if (jk->jasenet[jkoko-1]) {
					jno++;
					continue;
					}
				else break;
				}
			if (jk - joukk >= maxjoukk)
				break;
			if (jk->seura[0]) {
				for (j = 1; j < jkoko && jk->jasenet[j]; j++) ;
				if (sijaluvut) {
					sj = kilp.p_sija(0);
					if (sj > 0 && sj <= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
						sj = sijapisteet[sj-1];
					jk->tls += sj;
					}
				else {
#ifdef LVAYLAX
					jk->tls += (kilp.tulos2(0)/100) * 100;
#else
					jk->tls += kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2);
#endif
					}
				if (j < jkoko)
					jk->jasenet[j] = d;
				}
			else {
				wcscpy(jk->seura, kilp.seuratieto(0));
				jk->jnro = jno;
				jk->jasenet[0] = d;
				if (sijaluvut) {
					sj = kilp.p_sija(0);
					if (sj > 0 && sj <= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
						sj = sijapisteet[sj-1];
					jk->tls += sj;
					}
				else {
#ifdef LVAYLAX
					jk->tls = (kilp.tulos2(0)/100) * 100;
#else
					jk->tls = kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2);
#endif
					}
				njoukk++;
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		for (jk = joukk+njoukk-1; jk >= joukk; jk--) {
			if (!jk->jasenet[jkoko-1]) {
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
				if (co == L'P') {
					initpage(tulprm, NULL);
					swprintf(st, L"Joukkuetulokset  -  %s", Sarjat[srj].sarjanimi);
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
			if (jk->jnro > 1)
				swprintf(st1,L"%s -%d", jk->seura, jk->jnro);
			else
				wcscpy(st1, jk->seura);
			if (sijaluvut) {
				_itow(jk->tls > 0 ? jk->tls : -jk->tls, st2, 10);
				}
			else {
				AIKATOWSTRS(st2, jk->tls, 0);
				st2[pvparam[k_pv].laika2] = 0;
				elimwzb1(st2);
				}
			if (co == 'P') {
				putfld(tulprm, stsj, prtflds[F_SJ].pos, prtflds[F_SJ].len, 1, 0);
				putfld(tulprm, st1, prtflds[1].pos, 40, 0, 0);
				putfld(tulprm, st2, prtflds[F_TLS].pos, prtflds[F_TLS].len, 1, 0);
				endline(tulprm, 0);
				}
			else {
				swprintf(st, L"<b>%4.4s.  %-30.30s  %13s</b>\n", stsj, st1, st2);
				tulprm->writehtml(st);
				}
			st[0] = 0;
			for (i = 0; i < jkoko; i++) {
//         if (i) strcat(st, " - ");
				kilp.GETREC(jk->jasenet[i]);
//         strcat(st, kilp.nimi);
				if (sijaluvut) {
					_itow(kilp.p_sija(0), st1, 10);
					}
				else {
#ifdef LVAYLAX
					AIKATOWSTRS(st1, (kilp.(0)/(10*SEK))*10*SEK, 0);
#else
					AIKATOWSTRS(st1, kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2), 0);
#endif
					st1[pvparam[k_pv].laika2] = 0;
					elimwzb1(st1);
					}
				if (co == L'P') {
					putfld(tulprm, kilp.nimi(wbuf, 80, 0), prtflds[F_NIMI].pos+2*tulprm->tulmuot.numlev, 30, 0, 0);
					putfld(tulprm, st1, prtflds[F_TLS].pos+2*tulprm->tulmuot.numlev, prtflds[F_TLS].len, 1, 0);
					endline(tulprm, 0);
					}
				else {
					swprintf(st, L"           %-30.30s  %13s\n", kilp.nimi(wbuf,30,0), st1);
					tulprm->writehtml(st);
					}
				}
//      putfld(tulprm, st, prtflds[1].pos, 80, 0, 0);
//      endline(tulprm, 0);
			if (co == L'P') {
				if (!mahtuusivulle(n, jkoko+2, tulprm) {
					endpage(tulprm);
					n = 0;
					}
				else {
					newline(tulprm, 1);
					n += jkoko+2;
					}
				}
			else
				tulprm->writehtml(L"\n");
			}
		if (co == L'P') {
			if (n) endpage(tulprm);
			enddoc(tulprm);
			}
		else
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
	if (co != L'P') {
		tulprm->writehtml(L"</body>\n</html>\n");
		closeprfile(tulprm->lstf);
		}
   }

void joukkuetul(int srj, tulostusparamtp *tulprm)
   {
   INT32 tls0 = 0;
   wchar_t st[120], stsj[6], st2[14], st1[60], tc, ch, wbuf[80], co;
   INT i, j, jno, njoukk, maxjoukk, n, p, d, sj;
   jluettp  *joukk, huge *jk;
   kilptietue kilp;
   static INT jkoko = JKOKO;
   static int ytfl = 0;
   TextFl *pistefile;
   wchar_t pistefilenm[] = L"SIJAPISTEET.LST";
   int sijapisteet[1000];
   int joukkuekentta;
   bool vaadijoukkue;
   bool kaytaseuranimea;

	joukkuekentta = 1 + (tulprm->optiot % 256) / 32;   // 0: seuratieto, 1: seura, 2: lyhenne, 3: yhdistys, 4: joukkue
	jkoko = (tulprm->optiot % 16384) / 1024;
	kaytaseuranimea = tulprm->optiot % 32 / 16;
	vaadijoukkue = tulprm->optiot % 16 / 8;
	sijaluvut = tulprm->optiot % 8 / 4;
	co = tulprm->kohde;
	ytfl = tulprm->yhttulpaino;
		if (jkoko > JKOKO)
			jkoko = JKOKO;
		maxjoukk = ntulos[srj][0]/2+200;
		joukk = (jluettp *) calloc(maxjoukk, sizeof(jluettp));
		if (!joukk) {
			writeerror_w(L"Muisti ei riitä joukkuetuloksille", 0);
			return;
			}
		njoukk = 0;
		if (sijaluvut) {
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
		while (p >= 0) {
			d = p;
			kilp.GETREC(d);
			p = SeuraavaSrj(srj, ytfl ? kilpparam.valuku+1 : 0, p);
			if (!kilp.yhteisotieto(joukkuekentta)[0])
				continue;
			if (vaadijoukkue && (kilp.joukkue[0] == 0 ||
			   (kilp.joukkue[0] == L'0' && _wtoi(kilp.joukkue) == 0)))
				continue;
			jno = 1;
			for (jk = joukk; jk < joukk+njoukk; jk++) {
				if (!jk->seura[0]) break;

				if (wcscmp(jk->seura, kilp.yhteisotieto(joukkuekentta)))
					continue;
				if (jk->jasenet[jkoko-1]) {
					jno++;
					continue;
					}
				else break;
				}
			if (jk - joukk >= maxjoukk)
				break;
			if (jk->seura[0]) {
				for (j = 1; j < jkoko && jk->jasenet[j]; j++) ;
				if (sijaluvut) {
					sj = kilp.p_sija(0);
					if (sj > 0 && sj <= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
						sj = sijapisteet[sj-1];
					jk->tls += sj;
					}
				else {
					jk->tls += kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2);
					}
				if (j < jkoko)
					jk->jasenet[j] = d;
				}
			else {
				wcscpy(jk->seura, kilp.yhteisotieto(joukkuekentta));
				jk->jnro = jno;
				jk->jasenet[0] = d;
				if (sijaluvut) {
					sj = kilp.p_sija(0);
					if (sj > 0 && sj <= (int)(sizeof(sijapisteet) / sizeof(sijapisteet[0])))
						sj = sijapisteet[sj-1];
					jk->tls += sj;
					}
				else {
					jk->tls = kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2);
					}
				njoukk++;
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		for (jk = joukk+njoukk-1; jk >= joukk; jk--) {
			if (!jk->jasenet[jkoko-1]) {
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
				if (co == L'P') {
					initpage(tulprm, NULL);
					swprintf(st, L"Joukkuetulokset  -  %s", Sarjat[srj].sarjanimi);
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
			if (co == 'P') {
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
				kilp.GETREC(jk->jasenet[i]);
				if (sijaluvut) {
					_itow(kilp.p_sija(0), st1, 10);
					}
				else {
					AIKATOWSTRS(st1, kilp.tulos2(ytfl ? kilpparam.valuku+1 : 0, 2), 0);
					st1[pvparam[k_pv].laika2] = 0;
					elimwzb1(st1);
					}
				if (co == L'P') {
					putfld(tulprm, kilp.nimi(wbuf, 80, 0), prtflds[F_NIMI].pos+2*tulprm->tulmuot.numlev, 30, 0, 0);
					putfld(tulprm, st1, prtflds[F_TLS].pos+2*tulprm->tulmuot.numlev, prtflds[F_TLS].len, 1, 0);
					endline(tulprm, 0);
					}
				else {
					swprintf(st, L"           %-33.33s %13s\n", kilp.nimi(wbuf,33,0), st1);
					tulprm->writehtml(st);
					}
				}
			if (co == L'P') {
				if (!mahtuusivulle(n, jkoko+2, tulprm) {
					endpage(tulprm);
					n = 0;
					}
				else {
					newline(tulprm, 1);
					n += jkoko+2;
					}
				}
			else
				tulprm->writehtml(L"\n");
			}
		if (co == L'P') {
			if (n) endpage(tulprm);
			enddoc(tulprm);
			}
		else
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

#ifdef AMPUSUUNN
void as_muotoilu(void)
   {
   int i, dlisa = 0, plisa = 0;

   for (i = 1; i <= 3; i++) {
	  switch (aslaji[asjarjestys][i%3]) {
		 case 0:
			dspflds[7].pos = 44+dlisa;
            dlisa += 10;
            prtflds[7].pos = 65+plisa;
            prtflds[8].pos = 75+plisa;
				plisa += 14;
			break;
         case 1:
            dspflds[9].pos = 44+dlisa;
            dlisa += 10;
			prtflds[9].pos = 65+plisa;
			prtflds[10].pos = 75+plisa;
            plisa += 13;
			break;
         case 2:
            dspflds[11].pos = 44+dlisa;
            dlisa +=10;
            prtflds[11].pos = 65+plisa;
            plisa +=10;
			break;
		 }
      }
   }

void joukkuetul(tulostusparamtp *tulprm)
   {
   INT32 tls;
   static wchar_t ch = L' ';
   wchar_t seura[LSEURA+1], st[50], tc;
   INT pm, i, j, nseura, n, sj, srj, lseura;
   sraluettp  *lp;
   kilptietue kilp, kilp2;

   if (co != L'P')
	   return;
   lseura = kilpparam.seuralyh_on ? kilpparam.lseuralyh : kilpparam.lseura;
   seuraluettelo();
   if (!seurat) return;
   wselectopt(L"S)otilas-SM, P)M, Esc : Peruuta", L"SP\x1B", &ch);
   if (ch == ESC) return;
   pm = ch == L'P';
   if (pm) {
	  srj = 0;
	  if (sarjaluku > 1) {
		 clrln(ySize-3);
         vidspwmsg(ySize-3,0,7,0,L"Anna sarja:");
		 srj = luesarja(L"", &tc);
		 }
	  n = 6;
      clrln(ySize-3);
      vidspwmsg(ySize-3,0,7,0,L"Joukkueeseen laskettavien luku :");
      INPUTINTW(&n, 2, 33, ySize-3, L"\r", &ch);
      if (n > PM_LUKU) {
         n = PM_LUKU;
         vidint(ySize-3, 33, 2, n);
		 writeerror_w(L"Lukumäärä pienennetty maksimiarvoonsa 8", 0, true);
         }
      clrln(ySize-3);
	  }
   else
	  n = 4;
   for (i = 0; i < MAXNSEURA && seurat[i].seura; i++)
      for (j = 0; j < PM_LUKU; j++) seurat[i].d[j] = -1;
   for (i = 0; i < nrec-1; i++) {
      kilp.GETREC(i+1);
      if (kilp.kilpstatus == 0 && kilp.id() != 0 &&
		 (kilp.tark(0) == L'-' || kilp.tark(0) == L'T') &&
         (!pm || kilp.Sarja() == srj) &&
         (tls = kilp.p_aika(0)) != 0) {
		 wmemcpy(seura, kilp.seuratieto(0), lseura);
         if (!seura[0]) continue;
         upcasewstr(seura);
		 for (lp = seurat; lp->seura && wcscmp(seura, lp->seura); lp++) ;
         if (wcscmp(seura, lp->seura)) continue;
         if (pm) {
            for (j = 0; j < n; j++) {
               if (lp->d[j] < 0) break;
               kilp2.GETREC(lp->d[j]+1);
               if (kilp2.p_aika(0) > tls) break;
               }
			if (j < n && lp->d[j]) {
               if (j < n-1) 
                  memmove(lp->d+j+1, lp->d+j, (n - j - 1) * sizeof(int));
               lp->d[j] = i;
			   }
			}
         else {
            j = -1;
            while (j < 0) {
               switch (kilp.ryhma[0]) {
                  case L'U' :
					 j = 0;
					 break;
                  case L'O' :
                     j = 1;
					 break;
				  case L'V' :
                     j = -j + 1;
                     break;
                  }
               if (lp->d[j] >= 0) {
                  kilp2.GETREC(lp->d[j]+1);
                  if (kilp2.p_aika(0) < tls) {
                     if (j == 2 && kilp.ryhma[0] == L'V') j = -2;
                     else j = PM_LUKU+1;
					 }
                  }
               }
            if (j < PM_LUKU) {
               if (j == 2 && kilp.ryhma[0] == L'V') lp->d[3] = lp->d[2];
               lp->d[j] = i;
               }
			}
         }
      }
   nseura = 0;
   for (i = 0; i < MAXNSEURA && seurat[i].seura; i++) {
      seurat[i].jtls = 1000000000L;
      if ((pm && seurat[i].d[n-1] < 0) ||
		 (!pm && (seurat[i].d[0] < 0 || seurat[i].d[1] < 0 ||
         seurat[i].d[3] < 0))) continue;
	  nseura = i + 1;
      seurat[i].jtls = 0;
      for (j = 0; j < n; j++) {
         kilp2.GETREC(seurat[i].d[j]+1);
		 seurat[i].jtls += kilp2.p_aika(, 0);
         }
      }
   if (nseura) {
      qsortint((void  *) seurat, nseura, sizeof(sraluettp), 0, 1);
      EnterCriticalSection(&print_CriticalSection);
      initpage(tulprm, NULL);
      l = 0;
      if (paaots[0]) {
         paaots_on(tulprm);
         putfld(tulprm, paaots,0,80,0,0);
         endline(tulprm, -1);
         paaots_pois();
		 l = 1;
         }
	  tls = 0;
	  for (i = 0; i < nseura; i++) {
		 if (seurat[i].jtls > 999999999L) break;
		 if (!mahtuusivulle(l, n+4, &tulprm) {
            l = 0;
            endpage();
            initpage(tulprm, NULL);
			}
         else if (l) {
            if (l > 1) {
               newline(tulprm, 2);
               l++;
			   }
            l++;
            }
         aliots_on();
         if (l < 4) {
            putfld(tulprm, L"Joukkuetulokset", prtflds[0].pos, 15, 0, 0);
			endline(tulprm, -2);
            l += 2;
            }
         if (seurat[i].jtls != tls) {
            sj = i + 1;
			_itow(sj, st, 10);
            putfld(tulprm, st, prtflds[0].pos, prtflds[0].len, prtflds[0].oik, 0);
            }
         tls = seurat[i].jtls;
		 kilp.GETREC(seurat[i].d[0]+1);
		 wcscpy(st, kilp.seuratieto(0, false));
         putfld(tulprm, st, prtflds[2].pos, prtflds[2].len, prtflds[2].oik, 0);
         AIKATOWSTRS(st, tls, 0);
		 st[pvparam[k_pv].laika2] = 0;
         elimwz(st);
         putfld(tulprm, st, prtflds[5].pos, prtflds[5].len, prtflds[5].oik, 0);
         endline(tulprm, 0);
         aliots_pois();
		 l++;
         for (j = 0; j < n; j++) {
            if (j)
			   kilp.GETREC(seurat[i].d[j]+1);
            if (!pm) {
               switch (j) {
                  case 0 :
                     wcscpy(st, L"(u)");
                     break;
				  case 1 :
                     wcscpy(st, L"(ou)");
                     break;
                  case 2 :
					 wcscpy(st, L"(vm)");
					 break;
				  case 3 :
                     st[0] = 0;
                     break;
                  }
			   putfld(tulprm, st, prtflds[1].pos, prtflds[1].len, 0, 0);
               }
			if (tulprm->tulmuot.etusuku) {
               st[0] = 0;
			   if (kilpparam.arvo_on) {
				  wcscpy(st, kilp.arvo);
                  if (wcslen(st)) wcscat(st, L" ");
                  }
					kilp.nimi(st+wcslen(st), 38, true);
               putfld(tulprm, st, prtflds[2].pos, prtflds[2].len, prtflds[2].oik, 0);
               }
            else
					putfld(tulprm, kilp.nimi(st, 38, false), prtflds[2].pos, prtflds[2].len,
						prtflds[2].oik, 0);
			AIKATOWSTRS(st, kilp.p_aika(0),0);
			st[pvparam[k_pv].laika2] = 0;
            elimwz(st);
            putfld(tulprm, st, prtflds[5].pos, prtflds[5].len, prtflds[5].oik, 0);
			endline(tulprm, 0);
			l++;
            }
         }
      endpage();
	  enddoc();
	  LeaveCriticalSection(&print_CriticalSection);
      }
   }
#endif

INT avaa_tiedosto_nm(wchar_t *fname, tulostusparamtp *tulprm)
   {
   // avataan TextFl nimellä fname
   tulprm->lstf = openprfile(fname, -1, 1, 0, (char *) &tulprm->merkit, 0);
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
   TextFl *htmlotsfl = NULL;

#ifdef _CONSOLE
	  {
	  ch = tulprm->yksihtml ? L'Y' : L'S';
	  wselectopt(L"Y)htenä tiedostona vai S)arjat erikseen",
		 L"YS", &ch);
	  tulprm->yksihtml = ch == L'Y';
	  }
#endif
   htmlalku(tulprm->title, tulprm->header, 0, tulprm);
   if (!tulprm->yksihtml) {
	  tulprm->writehtml(L"<frameset rows=15%,* frameborder=no border=0>\n");
	  tulprm->writehtml(L"<frame src='tulosots.html' name=yla "
		 L"border=0 noresize scrolling=no>\n");
	  tulprm->writehtml(L"<frameset cols=15%,* frameborder=no border=0>\n");
	  tulprm->writehtml(L"<frame src='sarjalue.html' name=sivu"
		 L" border=0 noresize scrolling=auto>\n");
	  tulprm->writehtml(L"<frame src='tulalku.html' name=keski"
		 L" border=0 noresize scrolling=auto>\n");
	  tulprm->writehtml(L"</frameset></frameset>\n");
	  tulprm->writehtml(L"</html>\n");
	  closeprfile(tulprm->lstf);
	  if (!htmlotsfl) {
		  wcscpy(fname, baseFName);
		  for (p = fname + wcslen(fname) - 1; p > fname && wcswcind(*p, L":/\\") < 0; p--) ;
		  if (p > fname)
			  p[1] = 0;
		  else
			  fname[0] = 0;
		  wcscat(fname, L"tulosots.html");
		 if (avaa_tiedosto_nm(fname, tulprm)) return(1);
		 html_avaus(tulprm, 0, 2);
		 tulprm->writehtml(L"<BODY class='tulosots'>\n<H2><CENTER>");
		 tulprm->writehtml(tulprm->header);
		 tulprm->writehtml(L"\n</CENTER></H2></BODY></HTML>\n");
		 closeprfile(tulprm->lstf);

		  wcscpy(fname, baseFName);
		  for (p = fname + wcslen(fname) - 1; p > fname && wcswcind(*p, L":/\\") < 0; p--) ;
		  if (p > fname)
			  p[1] = 0;
		  else
			  fname[0] = 0;
		  wcscat(fname, L"tulalku.html");
		 if (avaa_tiedosto_nm(fname, tulprm)) return(1);
		 html_avaus(tulprm, 0, 3);
		 tulprm->writehtml(L"<BODY class='tulalku'>\n<H2><CENTER>");
		 tulprm->writehtml(tulprm->header);
		 tulprm->writehtml(L"\n</CENTER></H2></BODY></HTML>\n");
		 closeprfile(tulprm->lstf);
		 }
	  wcscpy(fname, baseFName);
	  for (p = fname + wcslen(fname) - 1; p > fname && wcswcind(*p, L":/\\") < 0; p--) ;
	  if (p > fname)
		  p[1] = 0;
	  else
		  fname[0] = 0;
	  wcscat(fname, L"sarjalue.html");
	  if (avaa_tiedosto_nm(fname, tulprm)) return(1);
	  html_avaus(tulprm, 0, 4);
	  tulprm->writehtml(L"<body class='sarjalue'>\n");
	  for (p = baseFName + wcslen(baseFName) - 1; p > baseFName && wcswcind(p[-1], L":/\\") < 0; p--) ;
	  for (isrj = 0; isrj < sarjaluku+nsarjayhd; isrj++) {
		 if (nilm[isrj] && tulprm->sarjalista[isrj]) {
			tulprm->writehtml(L"<A href=\"");
			tulprm->writehtml(htmlSrjFName(tulprm, p, isrj));
			tulprm->writehtml(L"\" target=keski>");
			tulprm->writehtml(Sarjat[isrj].Sarjanimi());
			tulprm->writehtml(L"</A><br>\n");
			}
		 }
	  }
   return(0);
   }

void ongelmat(tulostusparamtp *tulprm)
{
	kilptietue kilp;
	int d, kno, os;
	wchar_t jktark, kh;
	wchar_t st[80], st1[80], as[20] = L"", ch;
	wchar_t line[300];
	long tl;
	TextFl *OutFl = tulprm->lstf->u.Fl;

	ch = L' ';
	for (kno = minkilpno; kno < maxkilpno; kno++) {
		if ((d = getpos(kno)) > 0) {
			GETREC(&kilp, d);
			jktark = L' ';
			for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				kh = kilp.wTark(os, 0);
				tl = kilp.Maali(os, 0);
				if (os == 0 && kh == L'E')
					jktark = kh;
				if (kh == L'-') {
					swprintf(line, L"%4d-%d\t%s\t%s\t%s\t%s\n", kno, os+1, kilp.Joukkue(st1, 40),
						kilp.Maa(), kilp.Nimi(st, OSNIMIL, os), L"Tarkastus avoinna");
					OutFl->WriteLine(line);
					}
				if (kh == L'E' && tl != TMAALI0) {
					swprintf(line, L"%4d-%d\t%s\t%s\t%s\t%s\n", kno, os+1, kilp.Joukkue(st1, 40),
						kilp.Maa(), kilp.Nimi(st, OSNIMIL, os), L"Tulos ei-lähteneessä joukkueessa");
					OutFl->WriteLine(line);
					}
				if (emitfl || sisaanluenta) {
					if (kh != L'E' && kilp.ostiet[os].badge[0] == 0) {
						swprintf(line, L"%4d-%d\t%s\t%s\t%s\t%s\n", kno, os+1, kilp.Joukkue(st1, 40),
							kilp.Maa(), kilp.Nimi(st, OSNIMIL, os), L"Emit-koodi puuttuu");
						OutFl->WriteLine(line);
						}
					}
				if (os > 0 && tl != TMAALI0 && kilp.Maali(os-1, 0) == TMAALI0 &&
					kilp.ostiet[os].lahtolaji == 0) {
					if (kilp.ostiet[os].ylahto == TMAALI0)
						swprintf(line, L"%4d-%d\t%s\t%s\t%s\t%s\n", kno, os+1, kilp.Joukkue(st1, 40),
							kilp.Maa(), kilp.Nimi(st, OSNIMIL, os), L"Lähtöaika avoinna");
					OutFl->WriteLine(line);
					}
				}
			}
		}
//	fclose(tul_tied);
//	tul_tied = NULL;
}

void metsassa(tulostusparamtp *tulprm)
{
	kilptietue kilp;
	int d, kno, os, va;
	wchar_t line[300], as[20] = L"", st[80], st1[80], ch;
	int tl;
	static int traja;
	TextFl *OutFl = tulprm->lstf->u.Fl;

	ch = L' ';
	wselectopt(L"K)aikki, E)nnen aikavalintaa lähteneet", L"KE", &ch);
	clrln(ySize-3);
	if (ch == L'E') {
#ifdef _CONSOLE
		vidspwmsg(ySize-3, 0, 7, 0, L"Lähtöajan yläraja:");
		INPUTAIKAW(&traja, t0, 8, 21, ySize-3, L"\r", &ch);
#else
		inputaika_prompt(&traja, t0, 8, L"\r", &ch);
#endif
		}
	for (kno = minkilpno; kno < maxkilpno; kno++) {
		if ((d = getpos(kno)) > 0) {
			GETREC(&kilp, d);
			for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				if (kilp.ostiet[os].badge[0] > 0 && kilp.Maali(os, 0) == TMAALI0
					&& stschind(kilp.Tark(os, 0), "KHT") < 0) {
					tl = TMAALI0;
					if (os == 0)
						tl = Sarjat[kilp.sarja].lahto;
					else if (kilp.ostiet[os].ylahto != TMAALI0)
						tl = kilp.ostiet[os].ylahto;
					else
						tl = kilp.Maali(os-1, 0);
					if (ch != L'K' && tl != TMAALI0 && (traja - tl + 24*TUNTI) % (24*TUNTI) > 16 * TUNTI)
						continue;
					AIKATOWSTRS(as,tl , t0);
					as[8] = 0;
					swprintf(line, L"%4d-%d\t%s\t%s\t%s\tL: %s", kno, os+1, kilp.Joukkue(st1, 40),
						kilp.Maa(), kilp.Nimi(st, OSNIMIL, os), as);
					for (va = 1; va <= kilpparam.maxvaluku; va++) {
						if (kilp.Maali(os, va) != TMAALI0)
							swprintf(line+wcslen(line), L"\t%d: %s", va, AIKATOWSTRS(as, kilp.Maali(os, va), t0));
						}
					wcscat(line, L"\n");
					OutFl->WriteLine(line);
					}
				}
			}
		}
//	fclose(tul_tied);
//	tul_tied = NULL;
}

static void sulje_tiedosto(void)
   {
//   if (tul_tied) fclose(tul_tied);
//   tul_tied = NULL;
   }

static hajtp *hajalku[MAXSARJALUKU][MAXOSUUSLUKU];
static char hajfname[] = "HAJONNAT.LST";
static FILE *hajfile;

int seur_rata(int sarja, tulostusparamtp *tulprm)
   {
#ifndef LUENTA
   static int init;
   static hajtp *haj;
   hajtp *haj0;
   wchar_t line[20], *p;
   int srj=0, os=0;
   static bool tiedostosta = false;

   if (!init && !tulprm->monios) {
	  if ((hajfile = fopen(hajfname, "rt")) != NULL)
		  tiedostosta = true;
	  fclose(hajfile);
	  }
   if (tulprm->monios || !tiedostosta) {
	   if (nrata == 0)
		  return(1);
	   if (tulprm->hajrata.rata[0] == 0)
		   wcsncpy(tulprm->hajrata.rata, rata[0].tunnus, sizeof(tulprm->hajrata.rata)/2-1);
	   else {
			ratatp *rt;
			rt = getrata(tulprm->hajrata.rata);
			if (rt-rata < nrata-1) {
				rt++;
				wcsncpy(tulprm->hajrata.rata, rt->tunnus, sizeof(tulprm->hajrata.rata)/2-1);
				}
			else
			  memset(&tulprm->hajrata, 0, sizeof(hajtp));
			}
	   return(tulprm->hajrata.rata[0] == 0);
	  }
   if (!init) {
	  init = 1;
	  if ((hajfile = fopen(hajfname, "rt")) == NULL)
			return(-1);
	  while (!feof(hajfile)) {
		 if (!fgetws(line, 19, hajfile))
			  break;
		 if (wcslen(line) < 2)
			  break;
		 line[wcslen(line)-1] = 0;
		 if (towupper(line[0]) == L'S') {
			for (p = line+1; *p == ' '; p++) ;
			if ((srj = haesarja_w(p, 0)) < 0)
				break;
			haj0 = NULL;
			}
		 else if (towupper(line[0]) == L'O') {
			for (p = line+1; *p == ' '; p++) ;
			if ((os = _wtoi(p)-1) < 0)
				break;
			haj0 = NULL;
			}
		 else {
			haj = (hajtp *) calloc(sizeof(hajtp), 1);
			p = wcstok(line, L" \t");
			wcsncpy(haj->rata, p, HAJONTA);
			p = wcstok(NULL, L" \t");
			if (p)
				wcsncpy(haj->ratapit, p, 5);
			if (!haj0)
				hajalku[srj][os] = haj;
			else
				haj0->seur = (void *)haj;
			haj0 = haj;
			}
		 }
	  fclose(hajfile);
	  }
   if (tulprm->osuus >= Sarjat[sarja].ntosuus)
	   tulprm->osuus = 0;
   if (tulprm->hajrata.rata[0] == 0) {
	  haj = hajalku[sarja][tulprm->osuus];
	  }
   else {
	  haj = (hajtp *) haj->seur;
	  }
   if (haj) {
	  tulprm->hajrata = *haj;
	  return(tulprm->hajrata.rata[0] == 0);
	  }
   else {
	  memset(&tulprm->hajrata, 0, sizeof(hajtp));
	  }
#endif // LUENTA
   return(1);
   }

#ifndef _CONSOLE

int list(wchar_t kohde, wchar_t tiedlaji, wchar_t tulostettava, wchar_t jarjestys,
	wchar_t pisteval, wchar_t rajaus, int alaraja, int ylaraja, INT16 *sarjat,
	int osuus, wchar_t *kohdenimi, wchar_t merkit, wchar_t erotin,
	wchar_t *PisteLista, wchar_t (*seuralista)[LSEURA+5], wchar_t *LisaRivit, int options)
	{
	tulostusparamtp tulprm;
	INT  sv, enssrj, ixjarj = 0;
	int l = 0;
	INT piiri = 0;
	int pitkat = 1;
	bool pageclosed = false;

	if (kohde != L'P')
		tulprm = filetulparam;
	tulprm.merkit = merkit;
	tulprm.erotin = erotin;
	TiedRivi = 0;

	wcscpy(listfname, kohdenimi);
	if (kohde == L'P' && tulprm.lstf == NULL) {
		tulprm.printer = GDIPRINTER;
		if ((tulprm.lstf = openprfile(listfname, 1, TRUE, FALSE, trlate, FALSE))
			== NULL) {
			writeerror_w(L"",0);
			return(0);
			}
		}

	if (kohde == L'P') {
		tulprm.tulmuot = tulosmuot;
		wcsncpy(tulprm.htmlNimirunko, listfname, sizeof(tulprm.htmlNimirunko)/2-1);
/*
		tulprm.paa_alkuteksti = lsttulparam.paa_alkuteksti;
		tulprm.sivu_alkuteksti = lsttulparam.sivu_alkuteksti;
		tulprm.sivu_alateksti = lsttulparam.sivu_alateksti;
		tulprm.lopputeksti = lsttulparam.lopputeksti;
		tulprm.tulmuot.amarg = lsttulparam.tulmuot.amarg;
*/
		}
	else {
		tulprm.tulmuot = filetulosmuot;
		}

	if (tulprm.lstf && tulprm.lstf->prno == 2)
		initgdiprinter(&tulprm);

	tulprm.tulostettava = tulostettava;

	if (osuus > kilpparam.ntosuus) {
		if (osuus == kilpparam.ntosuus+1)
			options |= 0x00040000;
		else
			options |= 0x00080000;
		tulprm.osuus = 0;
		tulprm.viimos = L'K';
		}
	else if (osuus > 0) {
		tulprm.osuus = osuus-1;
		tulprm.viimos = 0;
		}
	else {
		tulprm.osuus = 0;
		tulprm.viimos = (tulprm.tulostettava == L'E' || pisteval == L'K') ? L'K' : L'V';
		if (tulprm.tulostettava == L'E' && wcswcind(tulprm.rajaus, L"ILTHP") < 0)
			tulprm.monios = true;
		}

	if (kohde == L'I') {
		if (tiedlaji == L'R' && pisteval == L'Z') {
			tiedlaji = L'Z';
			tulprm.kaikkiva = true;
			}
		if (tiedlaji == L'X' && pisteval == L'E') {
			if (tulprm.viimos == L'V')
				tulprm.viimos = L'K';
			tulprm.kaikkiva = true;
			if (tulprm.viimos == L'K')
				tulprm.osuus = 0;
			}
		if (tiedlaji == L'S') {
			tulprm.tulostettava = L'J';
			tulprm.viimos = L'K';
			tulprm.hksijafl = 1;
			}
		kohde = tiedlaji;
		}

	switch (jarjestys) {
		case L'A':
			tulprm.ixjarj = 1;
			break;
		case L'B':
			tulprm.ixjarj = 1;
			tulprm.kaikkisarjat = true;
			break;
		case L'M':
			tulprm.ixjarj = 11;
			tulprm.kaikkisarjat = true;
			break;
		case L'N':
			tulprm.ixjarj = 11;
			break;
		}

	if (tulprm.viimos == L'K') {
		tulprm.hksijafl = true;
		tulprm.tiivis = (options & 128) != 0;
		}

	tulprm.rajaus = rajaus;
	tulprm.kohde = kohde;
	tulprm.optiot = options;
	tulprm.language = (options & 0x10000000) ? 1 : 0;

	if (tulprm.rajaus == L'R') {
		tulprm.rajaus = L'L';
		if (tulprm.tulostettava == L'T')
			tulprm.tulostettava = L'H';
		tulprm.hajontatulokset = true;
		tulprm.ixjarj = 9;
		}

	if (tulprm.tulostettava == L'H' && tulprm.viimos == L'K')
		tulprm.monios = true;

	// Tästä alkaa koko tulosteen tekevä luuppi, joka toistetaan, jos tulostettava == L'F'

	do {
		if (tulostettava == L'F') {
			tulprm.tulostettava = L'T';
			}

		if (sarjat)
			memcpy(tulprm.sarjalista, sarjat, sizeof(tulprm.sarjalista));
		else
			wmemset((wchar_t *)tulprm.sarjalista, 1, sizeof(tulprm.sarjalista)/2);
		if (tulprm.rajaus == L'S') {
			tulprm.seuratulokset = true;
			tulprm.rajaus = L'L';
			tulprm.sj_raja = (options / 8192) & 63;
			}

		if (tulprm.rajaus == L'D') {
			tulprm.piiritulokset = true;
			tulprm.rajaus = L'L';
			tulprm.piirisijat = (options & 4096) ? 1 : 0;
			tulprm.sj_raja = (options & 0x0007e000) / 8192;
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
			case L'H':   // Osuuskohtaiset tulokset
				if (!tulprm.hajontatulokset)
					tulprm.ixjarj = tulprm.monios ? 10 : 0;
				tulprm.piste = kilpparam.valuku+1;
				break;
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
			case L'Y':    // Yhdistelmä vaihto+väliaikoja
				tulprm.viimos = L'K';
				tulprm.kaikkiva = true;
				tulprm.kaikkisijat = ((options & 8) != 0);
				break;
			case L'P':   // Pisteet
				return(0);
			case L'E':	 // Emitväliajat
				if (tulprm.hajontatulokset) {
					tulprm.rastivali = pisteval == L'R';
					}
				else if (tulprm.kohde != L'X') {
					tulprm.piste = 0;
					tulprm.viimos = L'K';
					tulprm.rastivali = pisteval == L'R';
					}
				break;
			case L'O':    // Ei tuloksia
				tulprm.piilotatulos = true;
				tulprm.piilotasijat = true;
				break;
			case L'L':    // Lähtöluettelo
				tulprm.tulbadge = (options & 4) != 0;
				tulprm.lahtoluettelo = true;
				tulprm.piilotatulos = true;
				tulprm.piilotasijat = true;
				break;
			default:
				tulprm.p_fld = prtflds;
				break;
			}

		if (emitfl && tulprm.tulostettava == L'E' && tulprm.kohde == L'I') {
			kirjvaliajat(listfname);
			goto loppu2;
			}

//		if (tulprm.kohde == L'H' && tulprm.tulostettava != L'E') {
		if (tulprm.kohde == L'H') {
			tulprm.yksihtml = !(options & 1);
			}

		if (tulprm.kohde == L'X') {
			if (tulprm.tulostettava == L'E') {
				tulprm.emitva = true;
				tulprm.poistanollat = true;
				tulprm.monios = 0;
				}
			else {
				tulprm.emitva = false;
				tulprm.poistanollat = false;
				}
			}

		if (wcswcind(kohde, L"IRJEHFOSWXZ") >= 0 && tulprm.lstf == NULL) {
			if (avaa_tiedosto_nm(kohdenimi, &tulprm))
				goto loppu2;
			}

		if (tulprm.rajaus == L'Z') {
			ongelmat(&tulprm);
			break;
			}

		if (tulprm.rajaus == L'M') {
			metsassa(&tulprm);
			break;
			}

#ifdef IOF
		if (tulprm.kohde == L'F')
			iofots();
#endif

		if (tulprm.kohde == L'X') {
			xmlots(&tulprm);
			}
		if (tulprm.kohde == L'P') {
			switch (tulprm.vaihelkm) {
				case 2:
					tulprm.p_fld = prtfldsk;
					break;
				default:
					tulprm.p_fld = prtflds;
				}
			}

		ssltlsfl = 0;

		if (tulprm.kohde == L'W') {
			ssltlsfl = 2;
			tulprm.kohde = L'S';
			}

		l = 0;
		sv = 0;
		enssrj = TRUE;

		if (tulprm.seuratulokset) {
			memset(seuranimi, 0, sizeof(seuranimi));
			for (int is = 0; is < SEURALISTA && seuralista[is][0]; is++)
				wmemcpy(seuranimi[is], seuralista[is]+3, LSEURA);
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
					if (l != 0)
						endpage(&tulprm);
						initpage(&tulprm, NULL);
					}
				l = 0;
				sv = 0;
				}

			for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
				if (!sarjat[srj])
					continue;
				merk_yht_lahdot(srj, false);
				if (tulprm.kohde == L'E' && tulprm.viimos == L'K' && tulprm.tulostettava == L'T') {
					kaikki_tiedostoon(&tulprm, srj, enssrj);
					enssrj = FALSE;
					continue;
					}
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
							putfld(&tulprm, paaots,0,80,0,0);
							endline(&tulprm, -1);
							paaots_pois(&tulprm);
							potsfl = TRUE;
							l += 2;
							}
						}
					else if (tulprm.kohde == L'E') {
						if (paaots[0]) {
							tulprm.puts_f(paaots);
							tulprm.puts_f(L"\n\n");
							}
						}
					else if (tulprm.kohde == L'H') {
						if (htmlrunko(&tulprm, kohdenimi))
							break;
						if (LisaRivit) {
							kirjLisaRivit(&tulprm, LisaRivit);
							}
						}
					enssrj = FALSE;
					}
				if (nilm[srj]) {

					if (tulprm.viimos && !tulprm.kaikkisarjat)
						tulprm.osuus = Sarjat[srj].ntosuus - 1;

					tulprm.lahtoajat = (tulprm.lahtoluettelo && Sarjat[srj].lahtovali[0]);
					if (tulprm.kohde == L'V')                // ??
						l = 0;
					if (tulprm.kohde == L'H' && !tulprm.yksihtml) {
						htmlloppu(&tulprm);
						closeprfile(tulprm.lstf);
						if (avaa_tiedosto_nm(htmlSrjFName(&tulprm, kohdenimi, srj), &tulprm))
							continue;
						htmlalku(0, 0, 1, &tulprm);
						}
					if (tulprm.kaikkisijat) {
						kaikkisijat(&tulprm, srj);
						}

					tulprm.hajrata.rata[0] = 0;
					do {
						if (tulprm.hajontatulokset) {
							if (seur_rata(srj, &tulprm))
								break;
							}

						tulostasarja(&srj, &tulprm, &l, &sv, FALSE);

						} while(tulprm.hajontatulokset);
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
				if (tulprm.kaikkisarjat)
					break;
				if (tulprm.kohde == L'H' &&	tulprm.tulostettava == L'E' && pisteval == L'M') {
					if (!tulprm.rastivali) {
						srj--;
						}
					tulprm.rastivali = !tulprm.rastivali;
					}
				}
			} while (0);
		if (tulostettava == L'F') {
			tulostettava = L'E';
			tulprm.tulostettava = tulostettava;
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
	if (kohde == L'H')
		htmlloppu(&tulprm);
	if (kohde == L'F')
		tulprm.writeline(L"E");
	if (kohde == L'X') {
		xmlloppu(&tulprm);
		}
	if (wcswcind(kohde, L"IREHSXZ") >= 0) {

		closeprfile(tulprm.lstf);
		}
	loppu2:
	if (kohde == L'P') {
		enddocGDI(tulprm.lstf);
		closeprfile(tulprm.lstf);
		tulprm.lstf = NULL;
		}
	return(TiedRivi);
}

#endif // !_CONSOLE

