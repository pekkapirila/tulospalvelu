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

static int njarjpst;
char *keysarja(void *kilp, char *key, int keylen, int flags);
char *keysa(void *kilp, char *key, int keylen, int flags);
char *keyse(void *kilp, char *key, int keylen, int flags);
char *keyseno(void *kilp, char *key, int keylen, int flags);
char *keynro(void *kilp, char *key, int keylen, int flags);
char *keysrjnro(void *kilp, char *key, int keylen, int flags);
char *keyla(void *kilp, char *key, int keylen, int flags);
char *keyrata(void *kilp, char *key, int keylen, int flags);
char *keysrjpst(void *vkilp, char *key, int keylen, int flags);
char *keyvaihetls(void *vkilp, char *key, int keylen, int flags);

static bool onMukana(kilptietue *kilp, int i_pv)
{
	return(kilp->tark(i_pv) != L'P' || kilpparam.poissamukaan >= 4 ||
		(ToimintaTila == 2 && i_pv == k_pv && kilpparam.poissamukaan > 0 &&
		(Sarjat[kilp->Sarja()].pistekaava[k_pv].yhtpiston & 6) == 6));
}

int JarjSeur(int yhd, int piste, int dKilp)
{
	return((int) jrjseur[yhd][njarjpst*dKilp+piste]);
}

void SetJarjSeur(int yhd, int piste, int dKilp, int jp)
{
	jrjseur[yhd][njarjpst*dKilp+piste] = (INT16) jp;
}

static int AllocJarjSeur(int nsrjyhd, int maxnrec)
{
	njarjpst = kilpparam.valuku+5;
	if (ToimintaTila == 2) {
		if (k_pv > 0)
			njarjpst += 5;
//		if (kilpparam.alalaji == L'D') {
//			kilpparam.kierrospiste = njarjpst++;
//			}
		}
	for (int y = 0; y <= nsrjyhd; y++) {
		if ((jrjseur[y] = (INT16 *) malloc((maxnrec+1)*njarjpst*sizeof(jrjseur[0][0]))) == NULL)
			return(1);
		for (int j = 0; j < (maxnrec+1)*njarjpst; j++)
			jrjseur[y][j] = -1;
		for (int i = 0; i < kilpparam.valuku + 4; i++) {
			if ((jarjtulos[y][i] = (jarjtulostp *) calloc(maxrec+1,sizeof(jarjtulostp))) == NULL) {
				return(1);
				}
			}
		}
	return(0);
}

static void DeAllocJarjSeur(int nsrjyhd)
{
	for (int y = 0; y <= nsrjyhd; y++) {
		for (int i = 0; i < kilpparam.valuku + 4; i++) {
			if (jarjtulos[y][i]) {
				free(jarjtulos[y][i]);
				jarjtulos[y][i] = NULL;
				}
			}
		if (jrjseur[y])
			free(jrjseur[y]);
		jrjseur[y] = NULL;
		}
}

void initindex(void)
   {
   INT i;

   idindex = ixarr;
   idindex->keyf = keynro;
   idindex->keyl = 4;

   sarjaindex = ixarr + 1;
   sarjaindex->keyf = keysarja;
   sarjaindex->keyl = 53;

   seuraindex = ixarr + 2;
   seuraindex->keyf = keyse;
   seuraindex->keyl = 52;

   seuranoindex = ixarr + 10;
   seuranoindex->keyf = keyseno;
   seuranoindex->keyl = 26;

   srjaakindex = ixarr + 3;
   srjaakindex->keyf = keysa;
   srjaakindex->keyl = 32;

   lahtoindex = ixarr + 4;
   lahtoindex->keyf = keyla;
   lahtoindex->keyl = 25;

   ljarjindex = ixarr + 5;
   ljarjindex->keyf = keyjrj;
   ljarjindex->keyl = 18;

   aakkosindex = ixarr + 6;
   aakkosindex->keyf = keyaak;
   aakkosindex->keyl = 32;

   nroindex = ixarr + 7;
   nroindex->keyf = keynro;
   nroindex->keyl = 4;

   srjnroindex = ixarr + 8;
   srjnroindex->keyf = keysrjnro;
   srjnroindex->keyl = 6;

   rataindex = ixarr + 9;
   rataindex->keyf = keyrata;
   rataindex->keyl = 24;

   for (i=0; i < NINDEX; i++)
	  ixarr[i].recl = ixarr[i].keyl + sizeof(int);

   pstindex.keyf = keysrjpst;
   pstindex.keyl = 6;
   pstindex.recl = 10;
   pstindex.flags = 0;

   vaihetlsindex.keyf = keyvaihetls;
   vaihetlsindex.keyl = 6;
   vaihetlsindex.recl = 10;
   vaihetlsindex.flags = 0;
   }

INT getpos(INT kilpno, bool hyvseuraava /* = false */)
	{
	int d;
	char key[4], key1[4];

	if (kilpno < minkilpno || kilpno > maxkilpno)
		return(-1);
	EnterCriticalSection(&idindex_CriticalSection);
	memcpy(key, &kilpno, 4);
	lswab((long *)key);
	memcpy(key1, key, 4);
	if (searchix(idindex, key, &d) || (!hyvseuraava && memcmp(key, key1, 4)))
		d = -1;
	LeaveCriticalSection(&idindex_CriticalSection);
	if (d  < 1 || d >= nrec)
		return(-1);
	return(d);
	}

int nextpos(void)
{
	int d, ret;
	char key[4];

	EnterCriticalSection(&idindex_CriticalSection);
	ret = nextix(idindex, key, &d);
	LeaveCriticalSection(&idindex_CriticalSection);
	return(ret ? -1 : d);
}

int prevpos(void)
{
	int d, ret;
	char key[4];

	EnterCriticalSection(&idindex_CriticalSection);
	ret = previx(idindex, key, &d);
	LeaveCriticalSection(&idindex_CriticalSection);
	return(ret ? -1 : d);
}

int setpos(int kilpno, int d)
{
	int dd, er = 0, ret = 0;
	char key[4], key1[4];

	EnterCriticalSection(&idindex_CriticalSection);
	if (kilpno < minkilpno || kilpno > maxkilpno || d < -1 || d >= nrec)
		er = -1;
	else {
		memcpy(key, &kilpno, 4);
		lswab((long *)key);
		memcpy(key1, key, 4);
		ret = searchix(idindex, key, &dd);
		if (ret || d != dd) {
			if (!ret && d == dd && !memcmp(key, key1, 4))
				delix(idindex, key, dd);
			addix(idindex, key1, d);
			}
		}
	LeaveCriticalSection(&idindex_CriticalSection);
	return(er);
/*
	if (!pakota && d > 0 && getpos(kilpno) > 0)
		return(d == getpos(kilpno) ? 0 : 1);
	posarr[kilpno - minkilpno] = d;
	return(0);
*/
}

int teeindeksi(kilpindex *ind, bool uusi)
{
   int  i, d;
   int n;
//   char *keyn;
   char *ip;
   static int eday;
   kilptietue kilp;

   reindex = 1;
   ind->pos = -1;
   if (!uusi) {
	   for (i=0; i < NINDEX; i++) {
		  if (ixarr[i].ix != NULL && ind == ixarr + i) {
			 if ((arv_pv == eday) && !reindex)
				return(1);
			 else {
				free(ixarr[i].ix);
				ixarr[i].ix = NULL;
				}
			 }
		  }
	   }
   eday = arv_pv;
   n = 0;
   if (ind->ix && ind->nrec < nilmt) {
		free(ind->ix);
		ind->ix = NULL;
		}
   if (ind->ix == NULL) {
	   if ((ind->ix = (char *) calloc(nrec+1,ind->recl)) == NULL) {
		  writeerror_w(L"Muistissa ei tilaa indeksille", 4000);
		  return(1);
		  }
	   }
   else
	   memset(ind->ix, 0, ind->recl*ind->nrec);
   ip = ind->ix;
   for ( d = 1; d < nrec; d++)
   {
	  kilp.GETREC(d);
	  if( (kilp.kilpstatus == 0) )
	  {
		if (ToimintaTila == 2 && arv_pv >= 0 &&
			(!onMukana(&kilp, arv_pv) || Sarjat[kilp.Sarja(arv_pv)].luonne[arv_pv] < 0))
			continue;
//		 keyn = ind->keyf(&kilp, ip, ind->keyl, ind->flags);
		 ind->keyf(&kilp, ip, ind->keyl, ind->flags);
		 *(int *)(ip + ind->keyl) = d;

		 n++;
		 ip += ind->recl;
	  }
   }
   quicksort((char *) ind->ix, n, ind->keyl, ind->recl);
   ind->nrec = n;
   reindex = FALSE;
   return(0);
}

void poistaindeksi(kilpindex *ind)
{
	if (ind == NULL)
		return;
	if (ind->ix)
		free(ind->ix);
	ind->ix = NULL;
	ind->nrec = 0;
}

void luo_posarr(void)
{
	kilptietue kilp;
//	UINT16 kno;

	EnterCriticalSection(&idindex_CriticalSection);
	if (!idindex->ix) {
		idindex->ix = (char *)calloc(maxrec+2, idindex->recl);
		}
	else {
		idindex->nrec = 0;
		memset(idindex->ix, 0, (maxrec+2) * idindex->recl);
		}
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		setpos(kilp.id(), d);
		}
	LeaveCriticalSection(&idindex_CriticalSection);

/*
	if (!posarr) {
		if ((posarr = (int *)calloc(maxkilpno-minkilpno+1, sizeof(posarr[0]))) == NULL)
			return;
		}
	else
		memset(posarr, 0, (maxkilpno-minkilpno+1)*sizeof(posarr[0]);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		kno = kilp.id();
		if (kilp.kilpstatus == 0 && kno >= minkilpno && kno <= maxkilpno) {
			setpos(kno, d);
			}
		}
*/
}

char *keysarja(void *vkilp, char *key, int keylen, int flags)
{
	wchar_t Buf[80];
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	if ((flags & 0x4000) && kilp->tark(arv_pv) == L'V')
		key[0] += 0x40;
	key[2] = (char) kilp->arvryhma;
	elimwbl2(kilp->seura);
	keyfromwname(key+3, kilp->seuratieto(NULL), min(20, keylen-2), 0);
	if (keylen > 23)
		keyfromwname(key+23, kilp->nimi(Buf, 45, 0), min(30, keylen-23), 0);
	return(key);
}

char *keyse(void *vkilp, char *key, int keylen, int flags)
{
	wchar_t buf[60];
	kilptietue *kilp;

	kilp = (kilptietue *)vkilp;
	elimwbl(kilp->maa);
	wcscpy(buf, L"   ");
	wmemcpy(buf, aakjarjwstr2(kilp->maa), wcslen(kilp->maa));
	if (finfirst) {
		if (!wcscmp(buf, L"FIN"))
			wcscpy(buf, L"   ");
		}
	else {
		if (!wcscmp(buf, L"   ") || !wcscmp(buf, L"  ") || !wcscmp(buf, L" ") || !wcscmp(buf, L""))
			wcscpy(buf, L"FIN");
		}
	keyfromwname(key, buf, 2, 0);

	elimwbl2(kilp->seura);
	keyfromwname(key+2, kilp->seuratieto(NULL), min(20, keylen-2), 0);

	keyfromwname(key+22, kilp->nimi(buf, 45, 0), min(30, keylen-2), 0);
   return(key);
}

char *keyseno(void *vkilp, char *key, int keylen, int flags)
{
	wchar_t buf[60];
	kilptietue *kilp;
	int kno;

	kilp = (kilptietue *)vkilp;
	kno = kilp->id(arv_pv);
	elimwbl(kilp->maa);
	wcscpy(buf, L"   ");
	wmemcpy(buf, aakjarjwstr2(kilp->maa), wcslen(kilp->maa));
	if (finfirst) {
		if (!wcscmp(buf, L"FIN"))
			wcscpy(buf, L"   ");
		}
	else {
		if (!wcscmp(buf, L"   ") || !wcscmp(buf, L"  ") || !wcscmp(buf, L" ") || !wcscmp(buf, L""))
			wcscpy(buf, L"FIN");
		}
	keyfromwname(key, buf, 2, 0);

	elimwbl2(kilp->seura);
	keyfromwname(key+2, kilp->seuratieto(NULL), min(20, keylen-2), 0);
   _swab((char *)&kno, key + 24, 2);
   _swab(((char *)&kno)+2, key + 22, 2);
   return(key);
}

char *keysa(void *vkilp, char *key, int keylen, int flags)
{
	wchar_t Buf[80];
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	if (keylen > 2)
		keyfromwname(key+2, kilp->nimi(Buf, 45, 0), min(30, keylen-2), 0);
   return(key);
}

char *keyla(void *vkilp, char *key, int keylen, int flags)
{
	INT dd;
	INT32 tl, tl0 = 3*TUNTI;
	int kno;
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	kno = kilp->id(arv_pv);
	memset(key, 0, keylen);
	dd = max(arv_pv, Sarjat[srj].enspv);
	for (int i = 0; i < sarjaluku; i++) {
		if (Sarjat[i].luonne[arv_pv] >= 0 && nilm_pv[arv_pv][i] > 0
			&& !Sarjat[i].vakanttisarja) {
			tl0 = Sarjat[i].enslahto[arv_pv];
			break;
			}
		}
	if ((flags & 1) == 0)
		key[0] = (char) Sarjat[srj].lno[dd];
	if (lahtojarjfl || Sarjat[srj].lahtovali[dd] != 0) {
		if (!onMukana(kilp, dd))
			memset(key+1, '\xff', 4);
		else if (kilp->pv[dd].tlahto == TMAALI0)
			memset(key+1, '\xff', 3);
		else {
			tl = (kilp->pv[dd].tlahto - tl0 + 129600L*SEK) % (86400L*SEK);
			memcpy(key+1, &tl, 4);
			lswab((long *)(key+1));
			}
		_swab((char *) &srj, key+5, 2);
		memcpy(key+7, &kno, 4);
		lswab((long *)(key+7));
		memset(key+11, 0, keylen-11);
		}
	else {
		keyfromwname(key+1, kilp->seuratieto(NULL), 20, 0);
		memcpy(key+21, &kno, 4);
		lswab((long *)(key+21));
		}
	return(key);
}

char *keyjrj(void *vkilp, char *key, int keylen, int flags)
{
	INT dd;
	INT32 tl;
	kilptietue *kilp;
	int kno;
	INT16 srj;

	memset(key, 0, keylen);
	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	if ((flags & 0x4000) && kilp->tark(arv_pv) == L'V')
		key[0] += 0x40;
	kno = kilp->id(arv_pv);
	dd = max(arv_pv, Sarjat[srj].enspv);
	if (takaa_ajofl || Sarjat[srj].lahtovali[dd] != 0) {
		if (!onMukana(kilp, dd))
			memset(key+2, '\xff', 4);
		else if (kilp->pv[dd].tlahto == TMAALI0)
			memset(key+2, '\xff', 3);
		else {
			tl = (kilp->pv[dd].tlahto - Sarjat[srj].enslahto[dd] + 129600L*SEK) % (86400L*SEK);
			memcpy(key+2, &tl, 4);
			lswab((long *)(key+2));
			if (dd > 0) {
				if ((tl = kilp->tulos_pv(dd-1, true, 1)) != 0 &&
					kilp->hyv_pv(dd-1)) {
//					(kilp->tark(dd-1) == L'-' || kilp->tark(dd-1) == L'T' ||  kilp->tark(dd-1) == L'I' ||
//					(kilpparam.hyvHavaittu && kilp->tark(dd-1) == L'B'))) {
					tl = pyoristatls(tl, 2, dd-1);
					memcpy(key+6, &tl, 4);
					lswab((long *)(key+6));
					}
				else
					memset(key+6, '\xff', 4);
				if (kilp->pv[dd-1].tlahto == TMAALI0)
					memset(key+10, '\xff', 4);
				else {
					tl = (kilp->pv[dd-1].tlahto - Sarjat[srj].enslahto[dd-1] + 129600L*SEK) % (86400L*SEK);
					memcpy(key+10, &tl, 4);
					lswab((long *)(key+10));
					}
				}
			}
		_swab((char *)&kno, key + 16, 2);
		_swab(((char *)&kno)+2, key + 14, 2);
		}
	else {
		_swab((char *)&kno, key + 4, 2);
		_swab(((char *)&kno)+2, key + 2, 2);
		}
	return(key);
}

char *keyaak(void *vkilp, char *key, int keylen, int flags)
{
	wchar_t buf[60];
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	keyfromwname(key, kilp->nimi(buf, 45, 0), min(30, keylen-2), 0);
	_swab((char *)&srj, key+30, 2);
	return(key);
}

char *keynro(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	int kno;

	kilp = (kilptietue *)vkilp;
	if (flags & 1)
		kno = kilp->id(-1);
	else
		kno = kilp->id(arv_pv);
   _swab((char *)&kno, key + 2, 2);
   _swab(((char *)&kno)+2, key, 2);
   return(key);
}

char *keysrjnro(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	int kno;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	kno = kilp->id(arv_pv);
   _swab((char *)&kno, key + 4, 2);
   _swab(((char *)&kno)+2, key + 2, 2);
   return(key);
}

char *keyrata(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT16 srj, era;
	char rata[20];
	int i;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	era = kilp->pv[arv_pv].era;
	_swab((char *)&era, key+2, 2);
	elimwbl(kilp->pv[arv_pv].rata);
	for (i = 0; i < 19; i++)
		if (kilp->pv[arv_pv].rata[i] < L'0' || kilp->pv[arv_pv].rata[i] > L'9')
			break;
	if (kilp->pv[arv_pv].rata[i] == 0) {
		era = (INT16) _wtoi(kilp->pv[arv_pv].rata);
		_swab((char *)&era, key+4, 2);
		}
	else {
		wcstoansi(rata, kilp->pv[arv_pv].rata, 19);
		rata[19] = 0;
		memcpy(key+4, rata, strlen(rata));
		}
   return(key);
}

//  Pistejärjestys määrätään seuraavasti:
//
//  flags & 1:  1: alhaiset pisteet parempia, 0: korkeat pisteet parempia
//  flags & 6:  kahdella jaettuna indeksi pistetietoihin
//  flags & 8:  0: kokonaispisteet, 8: vaiheen pisteet
//  flags & 16: 0: 0 pistettä käsitellään nollana, 16: 0 pistettä muutetaan huonoimmaksi.
//  flags & 32: yhdistelmä 1000*piste[2]+piste[1]/100
//  flags & 64: poissaolevalle pisteet 0
//  flags & 128: poissaolevalle pistelisäys 2^31

char *keysrjpst(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT16 srj, pst;
	UINT32 pisteet;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	_swab((char *)&srj, key, 2);
	if (flags & 32) {
		pisteet = max((UINT32)kilp->pisteet[2], 4000000);
		pisteet = 1000U*pisteet+(UINT32)kilp->pisteet[1]/100;
		}
	else {
		pst = (flags & 6) / 2;
		if (flags & 8)
			pisteet = kilp->pv[arv_pv].pvpisteet[pst];
		else {
//			if (pvparam[arv_pv].takaaAjoLisa && Sarjat[srj].luonne[arv_pv] == 2) {
			if (Sarjat[srj].luonne[arv_pv] == 2) {
//				pisteet = kilp->tulos_pv(arv_pv, true);
				pisteet = kilp->ytulos(arv_pv, true);
				flags |= 17;
				}
			else
				pisteet = kilp->pisteet[pst];
			}
		}
	if (pisteet == 0 && (flags & 1) == 1 && (flags & 16) == 16)
		pisteet = 0x7fffffff;
	if (pisteet == 0 && (flags & 64))
		pisteet = 1;
	if ((flags & 64) && !kilp->lasna(arv_pv))
		pisteet = 0;
	if ((flags & 128) && !kilp->lasna(arv_pv))
		pisteet += 0x80000000;
	if (flags & 1) {
		*(UINT32 *)(key+2) = pisteet;
		}
	else {
		*(UINT32 *)(key+2) = 0xffffffff - pisteet;
		}
	lswab((long *)(key+2));
	return(key);
}

char *keyvaihetls(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT16 srj, ipv;
	UINT32 tls;

	kilp = (kilptietue *)vkilp;
	ipv = arv_pv;
	srj = kilp->Sarja(ipv);
	_swab((char *)&srj, key, 2);
	tls = kilp->tulos_pv(ipv, true, pvparam[ipv].pyor[2]);
	if (tls)
		memcpy(key+2, &tls, 4);
	else
		memcpy(key+2, "\xff\xff\xff\xff", 4);
	lswab((long *)(key+2));
	return(key);
}

int askellasarja(int srj, int piste, int p, int *lj)
{
	int yhd = 0;

	if (srj >= sarjaluku && srj < sarjaluku+nsarjayhd)
		yhd = srj-sarjaluku+1;

	if (*lj >= 0 && p > 0)
		p = JarjSeur(yhd, piste, p);
	while ((p < 0 || *lj < 0) && *lj < (piste ? 0 : 4)) {
		p = jalku[srj][++*lj];
		}
	return(p);
}

//  entsija etsii kilpailijan sijan linkitetyllä listalla lähtien
//  kilpailijasta *p seuraten jäsenen JarjSeur(yhd, piste, dKilp) mukaista linkkiä listan
//  seuraavaan. Etsintä päättyy, kun seuraava on -1. Numerot viittaavat
//  sijaintiin ketjussa JarjSeur. (Sijainti on sama kuin tiedostossa KILP.DAT).
//  Jos kilpailijaa ei löydy on sija 9999.
//  p osoittaa löytymistä edeltävään tietueeseen, paitsi tapauksessa sj=1
//  itse tietueeseen (siis tällöin *p == d)

INT entsija(INT i, INT *p, INT piste, INT yhd, int *keskeyta)
	{
	INT  sj;
	INT  jp;

	sj = 1;
	jp = *p;
	while ( (jp >= 0) && (i != jp) ) {
		*p = jp;
		jp = JarjSeur(yhd, piste, jp);
		sj++;
		if (sj > nilmt+1 || jp > maxrec) {
			if (keskeyta)
				*keskeyta = *keskeyta | 1;
			break;
			}
		}
	if (i != jp) sj = 9999;
	return(sj);
	}

//  intsija etsii kilpailijan sijan linkitetyllä listalla lähtien
//  kilpailijasta *p seuraten jäsenen JarjSeur(yhd, piste, dKilp) mukaista linkkiä listan
//  seuraavaan. Etsintä päättyy, kun on löytyy huonompi tulos. Numerot viittaavat
//  sijaintiin ketjussa JarjSeur. (Sijainti on sama kuin tiedostossa KILP.DAT).
//  Jos kilpailijaa ei löydy on sija 9999.
//  p osoittaa löytymistä edeltävään tietueeseen, paitsi tapauksessa sj=1
//  itse tietueeseen (siis tällöin *p == d)

INT intsija(UINT32 *tl, INT *p, INT piste, INT yhd, int *keskeyta)
	{
	INT sj ;
	INT jp ;

	sj = 1;
	jp = *p;
	while (jp >= 0 && (
		tl[0] > jarjtulos[yhd][piste][jp][0] ||
		(tl[0] == jarjtulos[yhd][piste][jp][0] && tl[1] > jarjtulos[yhd][piste][jp][1])
		)) {
		*p = jp;
		jp = JarjSeur(yhd, piste, jp);
		sj++;
		if (sj > nilmt || jp > maxrec || jp < -1) {
			if (keskeyta)
				*keskeyta = *keskeyta | 2;
			break;
			}
		}
	return(sj);
	}

#define RAJA 12*TUNTI
#define RAJA2 12*TUNTI

//  Järjestystiedoista huolehditaan taulukoiden jarjtulos ja jrjseur
//  avulla. Kumpiakin taulukoita on yksipäiväisessä kisassa ja ensimmäisenä
//  päivänä kilpparam.valuku+1 ja muina päivinä kilpparam.valuku+2 kappaletta. Taulukoissa
//  on maxrec+2 alkiota (viimeistä ei tarvita). Jos väliaikoja ei oteta
//  on taulukoita vain 1 tai 2, mutta yhteistulosta vastaava indeksi on
//  aina kilpparam.valuku+1.
//  Tietojen sijainti taulukoissa vastaa kilpailijan sijaintia kilpailija-
//  tietotaulukossa ja se saadaan taulukosta posarr kilpailijanumeron
//  perusteella.
//  Taulukko jarjtulos sisältää tulosta vastaavat järjestysmuuttujan ja
//  taulukko jrjseur viittauksen järjestyksessä seuraavaan kilpailijaan.
//  Jos seuraavaa kilpailijaa ei ole on viittaus -1.
//  Taulukon jrjseur ketjulinkit on jaettu osajonoihin, joita on jokaiselle
//  sarjalle korkeintaan 10+kilpparam.valuku. Kunkin jonon alkupiste kerrotaan
//  taulukossa jalku seuraavassa annetun indeksin kohdalla.
//  Päivän tuloksille on varattu osajonot
//    0 : hyväksytty tulos
//    1 : keskeyttäneet
//    2 : hylätyt
//    3 : ei-lähteneet
//    4 : muut eli avoimet
//  Poissa olevia ei sisällytetä taulukkoon.
//  Päivän tulosten osalta vastaava tulos on tapauksessa
//    0 : tulos kertaa 4096 + lähtöaikaan perustuva lisäys, joka on korkein-
//        taan 4095.
//    1, 2, 3 : nimeen perustuva luku
//    4 : lähtöaikaan perustuva luku (tulos klo 24.00 yöllä)
//
//  Väliajoille jonot
//    5 .. kilpparam.valuku+4
//  vastava järjestystulos on tulos. Ilman väliaikaa olevia ei indeksoida.
//
//  Yhteistuloksille on varattu jonot:
//
//    kilpparam.valuku+5 : on yhteistulos
//    kilpparam.valuku+6 : aiempi yhteistulos, päivä hylätty tai ei-lähtenyt
//    kilpparam.valuku+7 : ei yhteistulosta, mutta päivän tulos
//    kilpparam.valuku+8 : ei aiempaa tulosta eikä päivän tulosta, päivä voi olla
//               avoinna
//    kilpparam.valuku+9 : aiempien päivien (yhteis)tulos, päivä avoinna
//
//  Vastaavat tulokset ovat
//
//    kilpparam.valuku+5 : yhteistulos
//    kilpparam.valuku+6 : aiempi yhteistulos + lisä hylätyille ja ei-lähteneille
//    kilpparam.valuku+7 : ei yhteistulosta, mutta päivän tulos
//    kilpparam.valuku+8 : perustuu nimeen + lisä hylätyille ja ei-lähteneille
//    kilpparam.valuku+9 : aiempi yhteistulos + tulos klo 24.00

void addjarjarr(kilptietue *kilp, int d, INT piste, int *keskeyta)
	{
	INT  tllj = -1,p,sj, lpiste, lahtotllj, yhd, srj, ik_pv;
	UINT32 tl[2];
	wchar_t st[40];
//   INT32 tl1;


	if (kilp->Sarja() < 0 || (ToimintaTila == 1 && piste != 0))
		return;

	ik_pv = (ToimintaTila == 2) ? k_pv : 0;

	if (piste == 0) {
		nyilm[kilp->Sarja()]++;
		for (yhd = 1; yhd <= nsarjayhd; yhd++) {
			if (yhdsarjat[yhd-1][kilp->Sarja()])
				nyilm[sarjaluku + yhd - 1]++;
			}
		nyilmt++;
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			if (onMukana(kilp, ipv) && Sarjat[kilp->Sarja(ipv)].luonne[ipv] >= 0) {
				nilm_pv[ipv][kilp->Sarja(ipv)]++;
				if (kilp->tark(ipv) == L'V' || kilp->tark(ipv) == L'P')
					nvak[ipv][kilp->Sarja(ipv)]++;
				}
			}
		}

	// Poissaolevien osalta jatketaan tulospalvelutilassa vain jos parametri POISSAMUKAAN on
	// käytössä tai sarjalle on määritelty yhteispisteiden käyttö yhteistuloksena sekä yhteospisteet
	// poissaolevillekin
	if (ToimintaTila == 2 && !onMukana(kilp, k_pv))
		return;

	for (yhd = 0; yhd <= nsarjayhd; yhd++) {
		if (yhd > 0 && yhdsarjat[yhd-1][kilp->Sarja()] == 0)
			continue;
		if (yhd == 0)
			srj = kilp->Sarja();
		else {
			srj = sarjaluku + yhd - 1;
			}
		tl[1] = 0;
		if (piste == 0) {
			modif[srj] = TRUE;
			nilm[srj]++;
			if (yhd == 0) {
				nilmt++;
				if (ToimintaTila == 1) {     // Esivalmistelutilan laskurit
					switch (kilp->tark(0)) {
						case L'H':
						case L'M':
							nhyl[srj]++;
							nhylt++;
							break;
						case L'K':
						case L'O':
						case L'X':
							nkesk[srj]++;
							nkeskt++;
							break;
						case L'P':
						case L'V':
							nvakt++;
						case L'E':
						case L'B':
							neil[srj]++;
							neilt++;
							if (kilp->p_aika(0))
								neilaika[srj]++;
							break;
						case L'I':
							nesit[srj]++;
							nesitt++;
						default:
							if (kilp->p_aika(0, 1) != 0) {
								ntulos[srj][0]++;
								ntulost++;
								}
							break;
						}
					return;        // Esivalmistelutilan käsittely loppuu tähän
					}
#if defined(UINTIxx)
				for (p = 0; p < maxrec && erataul[p].kilpno > 0; p++) ;
				erataul[p].kilpno = kilp->id();
				erataul[p].era = kilp->era;
				erataul[p].rata = kilp->rata;
#endif
				if (kilp->tark(ik_pv) == L'I') {
					nesitt++;
					nesit[kilp->Sarja()]++;
					}
				}
			if (kilp->lasna(ik_pv)) {
//				lpiste = kilpparam.valuku + 1 + (ik_pv > 0);
//				lahtotllj = kilpparam.valuku + 5 + 5 * (ik_pv > 0);
				lpiste = kilpparam.valuku + 2;
				lahtotllj = kilpparam.valuku + 10;
				if (yhd == 0)
					tl[0] = (kilp->pv[ik_pv].tlahto - Sarjat[kilp->Sarja()].enslahto[ik_pv] + 36*TUNTI) % (24*TUNTI);
				else
					tl[0] = (kilp->pv[ik_pv].tlahto - Sarjat[srj].enslahto[ik_pv] + 36*TUNTI) % (24*TUNTI);
				tl[1] = kilp->id();
				p = jalku[srj][lahtotllj];
				sj = intsija(tl,&p,lpiste,yhd,keskeyta);
				if (sj == 1) {
					SetJarjSeur(yhd, lpiste, d, p);
					jalku[srj][lahtotllj] = d;
					}
				else {
					SetJarjSeur(yhd, lpiste, d, JarjSeur(yhd, lpiste, p));
					SetJarjSeur(yhd, lpiste, p, d);
					}
				jarjtulos[yhd][lpiste][d][0] = tl[0];
				jarjtulos[yhd][lpiste][d][1] = tl[1];
				}
			tl[1] = 0;
			if (kilp->tulos_pv(ik_pv, true, 1) != 0) {
				tl[0] = kilp->tulos_pv(ik_pv, true, 2);
				if (kilpparam.alalaji == L'D') {
					tl[1] = tl[0];
					tl[0] = 1000 - kilp->pv[k_pv].pvpisteet[0];
					}
				else {
					if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S') {
						tl[1] = kilp->id();
						}
					else
						tl[1] = ((kilp->pv[ik_pv].tlahto - Sarjat[kilp->Sarja()].enslahto[ik_pv] + 36*TUNTI) % (24*TUNTI))/KSEK;
					}
				tllj = 0;
				ntulos[srj][0]++;
				if (yhd == 0) {
					ntulost++;
					}
				}
			else {
				wchar_t wctrk = kilp->tark(ik_pv);
				if (wctrk == L'B')
					wctrk = kilpparam.hyvHavaittu ? L'-' : L'E';
				tl[0] = wnimilong(kilp->nimi(st, 38, false));
				switch(wctrk) {
					case L'X':
						wmemmove(st+2, st, 36);
						st[38] = 0;
						st[0] = L'A';
						st[1] = L'A';
							// jatkuu tarkoituksella
					case L'K' :
					case L'O' :
						tllj = 1;
						nkesk[srj]++;
						if (yhd == 0) {
							nkeskt++;
							}
						break;
					case L'H' :
					case L'M' :
						tllj = 2;
						nhyl[srj]++;
						if (yhd == 0) {
							nhylt++;
							}
						break;
					case L'P' :
					case L'V' :
						if (yhd == 0)
							nvakt++;
							// jatkuu tarkoituksella
					case L'E' :
						if  (!onMukana(kilp, ik_pv)) {
							tllj = 999;
							}
						else {
							tllj = 3;
							neil[srj]++;
							if (yhd == 0) {
								neilt++;
								}
							if (kilp->p_aika(0))
								neilaika[srj]++;
							}
						break;
					default  :
						if (!Sarjat[kilp->Sarja()].lahtovali[ik_pv])
							tl[0] = kilp->id();
						else {
							if (kilpparam.n_pv_akt == 1)
								tl[0] = 24*TUNTI - (kilp->pv[ik_pv].tlahto - Sarjat[kilp->Sarja()].enslahto[ik_pv] + 36*TUNTI) % (24*TUNTI);
							else
								tl[0] = kilp->tulos1(0, 12L*TUNTI);
							}
						tllj = 4;
					}
				}
			}
		else {  // piste > 0
			if (kilp->hyv() && (tl[0] = kilp->tulos2(piste, 1)) != 0 ) {
				tl[0] = pyoristatls(tl[0], 2);
				tllj = 4+piste;
				ntulos[srj][piste]++;
				if (yhd == 0) {
					nva[piste-1]++;
					}
				}
			else tllj = -1;
			}
		if (tllj >= 0) {
			p = jalku[srj][tllj];
			sj = intsija(tl,&p,piste,yhd,keskeyta);
			if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) || p > maxrec)
				{
				*keskeyta = *keskeyta | 4;
				return;
				}
			if (yhd == 0 && piste && tllj == 4+piste)
				kilp->set_sija(piste, sj);
			if (sj == 1) {
				SetJarjSeur(yhd, piste, d, p);
				jalku[srj][tllj] = d;
				if (piste || !tllj) {
					pkarki[srj][piste] = kilp->tulos2(piste, 2);
					}
				}
			else {
				SetJarjSeur(yhd, piste, d, JarjSeur(yhd, piste, p));
				SetJarjSeur(yhd, piste, p, d);
				}
			jarjtulos[yhd][piste][d][0] = tl[0];
			jarjtulos[yhd][piste][d][1] = tl[1];
			}
/*
		if (ToimintaTila == 2 && kilpparam.alalaji == L'D') {
			if ((tl[0] = kilp->p_aika(0, 0)) > 0) {
				tllj = njarjpst - 1;
				tl[1] = 1000 - kilp->pv[k_pv].pvpisteet[0];
				ntulos[srj][kilpparam.valuku+1]++;
				if (yhd == 0)
					nytulost++;
				p = jalku[srj][tllj];
				sj = intsija(tl,&p,kilpparam.valuku+1,yhd,keskeyta);
				if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) ||
					p > maxrec) {
					*keskeyta = *keskeyta | 8;
					return;
					}
				if (sj == 1) {
					SetJarjSeur(yhd, kilpparam.valuku+1, d, p);
					jalku[srj][tllj] = d;
					}
				else {
					SetJarjSeur(yhd, kilpparam.valuku+1, d, JarjSeur(yhd, kilpparam.valuku+1, p));
					SetJarjSeur(yhd, kilpparam.valuku+1, p, d);
					}
				if (yhd == 0) {
					jarjtulos[kilpparam.valuku+1][d][0] = tl[0];
					jarjtulos[kilpparam.valuku+1][d][1] = tl[1];
					}
				}
			}
*/
		if (ToimintaTila == 2 && piste == 0 && (ik_pv > Sarjat[kilp->Sarja()].enspv ||
			((kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S') &&
			 ik_pv == Sarjat[kilp->Sarja()].enspv &&
			 ((ik_pv == 0 && Sarjat[kilp->sarja].sprintSrj.vaihelkm > 1) ||
			 Sarjat[kilp->Sarja()].viimpv > Sarjat[kilp->Sarja()].enspv)))) {
			tl[0] = 0;
			tl[1] = 0;
			if (kilp->ytulos(ik_pv, 1) != 0 ) {
				tl[0] = kilp->ytulos(ik_pv, 2);
				if (ik_pv > 0 || kilp->tulos_pv(0, true, 1) != 0 ||
					Sarjat[kilp->Sarja()].yhtpistkaikille()) {
					tllj = kilpparam.valuku + 5;
					ntulos[srj][kilpparam.valuku+1]++;
					if (yhd == 0) {
						nytulost++;
						}
					}
				else {
					if (kilp->lasna(0)) {
						if (!kilp->hyv_pv(0))
							tllj = kilpparam.valuku + 6;
						else
							tllj = kilpparam.valuku + 9;
						}
					else
						tllj = kilpparam.valuku + 8;
					}
				}
			else if (ik_pv > 0) {
				if (tllj == 0) {
					tllj = kilpparam.valuku + 7;
					}
				else if ((tl[1] = kilp->ytulos(ik_pv-1, 1)) != 0) {
					if (kilp->hyv()) {
						tllj = kilpparam.valuku + 9;
						tl[1] = pyoristatls(tl[1], 2);
						tl[0] = kilp->tulos1(kilpparam.valuku+1, 12L*TUNTI);
//						tl[0] = tl[1] + kilp->tulos1(0, RAJA);
						}
					else
						tllj = kilpparam.valuku + 6;
					}
				else {
					tl[1] = wnimilong(kilp->nimi(st, 38, false));
					tllj = kilpparam.valuku + 8;
					if (kilpparam.n_pv_akt == 2) {
						switch (kilp->tark(0)) {
							case L'K':
							case L'O':
							case L'X':
								break;
							case L'H':
							case L'M':
								tl[0] += RAJA2;
								break;
							default:
								tl[0] += 3*RAJA2;
								break;
							}
						}
					}
				if (tllj == kilpparam.valuku + 6 || tllj == kilpparam.valuku + 8) {
					wchar_t wctrk = kilp->tark(ik_pv);
					if (wctrk == L'B')
						wctrk = kilpparam.hyvHavaittu ? L'-' : L'E';
					switch(wctrk) {
						case L'K' :
						case L'O' :
						case L'X':
							break;
						case L'H' :
						case L'M':
							tl[0] += 4*RAJA2;
							break;
						case L'E' :
						case L'V' :
						case L'P' :
							tl[0] += 12L*RAJA2;
							break;
						}
					}
				}
			else
				tllj = kilpparam.valuku + 9;
			if (tllj > kilpparam.valuku+4) {
				p = jalku[srj][tllj];
				sj = intsija(tl,&p,kilpparam.valuku+1,yhd,keskeyta);
				if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) ||
					p > maxrec) {
					*keskeyta = *keskeyta | 8;
					return;
					}
				if (sj == 1) {
					SetJarjSeur(yhd, kilpparam.valuku+1, d, p);
					jalku[srj][tllj] = d;

					if (tllj == kilpparam.valuku+5)
						pkarki[srj][kilpparam.valuku+1] = tl[0];
					}
				else {
					SetJarjSeur(yhd, kilpparam.valuku+1, d, JarjSeur(yhd, kilpparam.valuku+1, p));
					SetJarjSeur(yhd, kilpparam.valuku+1, p, d);
					}
				jarjtulos[yhd][kilpparam.valuku+1][d][0] = tl[0];
				jarjtulos[yhd][kilpparam.valuku+1][d][1] = tl[1];
				}
			}
		}
	}

void remjarjarr(kilptietue *kilp, int d, INT piste, int *keskeyta)
	{
	INT tllj = -1, p, sj, lpiste, lahtotllj, yhd, srj, ik_pv;


	if (kilp->Sarja() < 0 || (ToimintaTila == 1 && piste != 0))
		return;

	ik_pv = (ToimintaTila == 2) ? k_pv : 0;

	if (piste == 0) {
		nyilmt--;
		nyilm[kilp->Sarja()]--;
		for (yhd = 1; yhd <= nsarjayhd; yhd++) {
			if (yhdsarjat[yhd-1][kilp->Sarja()])
				nyilm[sarjaluku + yhd - 1]--;
			}
		for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
			if (onMukana(kilp, ipv) && Sarjat[kilp->Sarja(ipv)].luonne[ipv] >= 0) {
				nilm_pv[ipv][kilp->Sarja(ipv)]--;
				if (kilp->tark(ipv) == L'V')
					nvak[ipv][kilp->Sarja(ipv)]--;
				}
			}
		}

	// Poissaolevien osalta jatketaan tulospalvelutilassa vain jos parametri POISSAMUKAAN on
	// käytössä tai sarjalle on määritelty yhteispisteiden käyttö yhteistuloksena sekä yhteospisteet
	// poissaolevillekin
	if (ToimintaTila == 2 && !onMukana(kilp, k_pv))
		return;

	for (yhd = 0; yhd <= nsarjayhd; yhd++) {
		if (yhd > 0 && yhdsarjat[yhd-1][kilp->Sarja()] == 0)
			continue;
		if (yhd == 0)
			srj = kilp->Sarja();
		else
			srj = sarjaluku + yhd - 1;
		if (piste == 0) {
			modif[srj] = TRUE;
			nilm[srj]--;
			if (yhd == 0) {
				nilmt--;
				if (ToimintaTila == 1) {       // Esivalmistelutilan laskurit
					switch (kilp->tark(0)) {
						case L'H':
						case L'M':
							nhyl[srj]--;
							nhylt--;
							break;
						case L'K':
						case L'O':
						case L'X':
							nkesk[srj]--;
							nkeskt--;
							break;
						case L'P':
						case L'V':
							nvakt--;
						case L'E':
						case L'B':
							neil[srj]--;
							neilt--;
							if (kilp->p_aika(0))
								neilaika[srj]--;
							break;
						case L'I':
							nesit[srj]--;
							nesitt--;
						default:
							if (kilp->p_aika(0, 1) != 0) {
								ntulos[srj][0]--;
								ntulost--;
								}
							break;
						}
					return;     // Esivalmistelutilan käsittely loppuu tähän
					}
				if (kilp->tark(ik_pv) == L'I') {
					nesitt--;
					nesit[kilp->Sarja()]--;
					}
				}
			if (kilp->lasna(ik_pv)) {
//					lpiste = kilpparam.valuku + 1 + (ik_pv > 0);
//					lahtotllj = kilpparam.valuku + 5 + 5 * (ik_pv > 0);
				lpiste = kilpparam.valuku + 2;
				lahtotllj = kilpparam.valuku + 10;
				p = jalku[srj][lahtotllj];
				sj = entsija(d,&p,lpiste,yhd,keskeyta);
				if (sj < 9999 && d >= 0 && d <= maxrec && p >= 0 && p <= maxrec) {
					if (sj == 1) {
						jalku[srj][lahtotllj] = JarjSeur(yhd, lpiste, d);
						}
					else
						SetJarjSeur(yhd, lpiste, p, JarjSeur(yhd, lpiste, d));
					SetJarjSeur(yhd, lpiste, d, -1);
					}
				}
			if (kilp->tulos_pv(ik_pv, true, 1)) {
				tllj = 0;
				ntulos[srj][0]--;
				if (yhd == 0) {
					ntulost--;
					}
				}
			else {
				wchar_t wctrk = kilp->tark(ik_pv);
				if (wctrk == L'B')
					wctrk = kilpparam.hyvHavaittu ? L'-' : L'E';
				switch(wctrk) {
					case L'O' :
					case L'X' :
					case L'K' : tllj = 1;
								nkesk[srj]--;
								if (yhd == 0) {
									nkeskt--;
									}
								break;
					case L'M' :
					case L'H' : tllj = 2;
								nhyl[srj]--;
								if (yhd == 0) {
									nhylt--;
									}
								break;
					case L'P' :
					case L'V' :
								if (yhd == 0)
									nvakt--;
								// jatkuu tarkoituksella
					case L'E' :
								if  (!onMukana(kilp, ik_pv)) {
									tllj = 999;
									}
								else {
									tllj = 3;
									neil[srj]--;
									if (yhd == 0) {
										neilt--;
										}
									if (kilp->p_aika(0))
										neilaika[srj]--;
									}
								break;
					default  : tllj = 4;
					}
				}
			}
		else if (kilp->tulos2(piste, 1)) {
			tllj = 4+piste;
			}
		if (tllj >= 0) {
			p = jalku[srj][tllj];
			if (p < 0) {
				sj = 9999;
				}
			else {
				sj = entsija(d,&p,piste,yhd,keskeyta);
				if (d < 0 || d > maxrec || p < 0 || p > maxrec) {
					*keskeyta = *keskeyta | 16;
					return;
					}
				}
			if (sj < 9999) {
				if (piste > 0) {
					ntulos[srj][piste]--;
					if (yhd == 0) {
						nva[piste-1]--;
						}
					}
				if (sj <= autotulostusparam.viimsija && tllj == 0)
					innpfl = TRUE;
				if (sj == 1) {
					jalku[srj][tllj] = JarjSeur(yhd, piste, d);
					if (piste || !tllj) {
						kilptietue tkilp;
						int td;
						if ((td = jalku[srj][tllj]) > 0) {
							tkilp.GETREC(td);
							pkarki[srj][piste] = tkilp.tulos2(piste, 2);
							}
						else
							pkarki[srj][piste] = 0;
						}
					}
				else
					SetJarjSeur(yhd, piste, p, JarjSeur(yhd, piste, d));
				SetJarjSeur(yhd, piste, d, -1);
				}
			}
/*
		if (ToimintaTila == 2 && kilpparam.alalaji == L'D') {
			if (kilp->p_aika(0, 0) > 0) {
				tllj = njarjpst - 1;
				ntulos[srj][kilpparam.valuku+1]--;
				if (yhd == 0)
					nytulost--;
				p = jalku[srj][tllj];
				sj = entsija(d,&p,kilpparam.valuku+1, yhd,keskeyta);
				if (d < 0 || d > maxrec || p < 0 || p > maxrec) {
					*keskeyta = *keskeyta | 32;
					return;
					}
				if (sj <= autotulostusparam.viimsija && tllj == 0)
					innpfl = TRUE;
				if (sj == 1) {
					jalku[srj][tllj] = JarjSeur(yhd, kilpparam.valuku+1, d);
					}
				}
			else
				SetJarjSeur(yhd, kilpparam.valuku+1, p, JarjSeur(yhd, kilpparam.valuku+1, d));
			SetJarjSeur(yhd, kilpparam.valuku+1, d, -1);
			}
*/
		if (ToimintaTila == 2 && piste == 0 && (ik_pv > Sarjat[kilp->Sarja()].enspv ||
			((kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S') &&
			 ik_pv == Sarjat[kilp->Sarja()].enspv &&
			 ((ik_pv == 0 && Sarjat[kilp->sarja].sprintSrj.vaihelkm > 1) ||
			 Sarjat[kilp->Sarja()].viimpv > Sarjat[kilp->Sarja()].enspv)))) {
			if (kilp->ytulos(ik_pv, 1) != 0 ) {
				if (ik_pv > 0 || kilp->tulos_pv(0, true, 1) != 0 ||
					Sarjat[kilp->Sarja()].yhtpistkaikille()) {
					tllj = kilpparam.valuku + 5;
					ntulos[srj][kilpparam.valuku+1]--;
					if (yhd == 0) {
						nytulost--;
						}
					}
				else {
					if (kilp->lasna(0)) {
						if (!kilp->hyv_pv(0))
							tllj = kilpparam.valuku + 6;
						else
							tllj = kilpparam.valuku + 9;
						}
					else
						tllj = kilpparam.valuku + 8;
					}
				}
			else if (ik_pv > 0) {
				if (tllj == 0) {
					tllj = kilpparam.valuku + 7;
					}
				else if (kilp->ytulos(ik_pv-1, 1) != 0) {
					if (kilp->hyv()) {
						tllj = kilpparam.valuku + 9;
						}
					else
						tllj = kilpparam.valuku + 6;
					}
				else {
					tllj = kilpparam.valuku + 8;
					}
				}
			else
				tllj = kilpparam.valuku + 9;
			p = jalku[srj][tllj];
			sj = entsija(d,&p,kilpparam.valuku+1, yhd,keskeyta);
			if (d < 0 || d > maxrec || p < 0 || p > maxrec) {
				*keskeyta = *keskeyta | 32;
				return;
				}
			if (sj <= autotulostusparam.viimsija && tllj == 0)
				innpfl = TRUE;
			if (sj == 1) {
				jalku[srj][tllj] = JarjSeur(yhd, kilpparam.valuku+1, d);
				if (tllj == kilpparam.valuku+5) {
					kilptietue tkilp;
					int td;
					if ((td = jalku[srj][tllj]) > 0) {
					  tkilp.GETREC(td);
					  pkarki[srj][kilpparam.valuku+1] = tkilp.ytulos(ik_pv, 2);
					  }
					else
					  pkarki[srj][kilpparam.valuku+1] = 0;
					}
				}
			else
				SetJarjSeur(yhd, kilpparam.valuku+1, p, JarjSeur(yhd, kilpparam.valuku+1, d));
			SetJarjSeur(yhd, kilpparam.valuku+1, d, -1);
			}
		}
	}

void haesija(kilptietue *kilp, INT piste, INT yhd, int *keskeyta)
	{
	INT  sj = 0, p, ik_pv;
	UINT32  tl[2];

	ik_pv = (ToimintaTila == 2) ? k_pv : 0;

	tl[1] = 0;
	if ((piste == 0 && (tl[0] = kilp->tulos_pv(ik_pv, true, 1)) != 0)
		|| (piste == kilpparam.valuku+1 && (tl[0] = kilp->ytulos(ik_pv, 1)) != 0)
		|| (piste > 0 && piste <= kilpparam.valuku && (tl[0] = kilp->tulos2(piste, 1)) != 0)
		) {
		tl[0] = pyoristatls(tl[0], 2);
		p = jalku[yhd == 0 ? kilp->Sarja() : sarjaluku+yhd][piste ? 4+piste : 0];
		sj = intsija(tl, &p, piste,yhd,keskeyta);
		p = jalku[yhd == 0 ? kilp->Sarja() : sarjaluku+yhd][piste ? 4+piste : 0];
		if (entsija(getpos(kilp->id()),&p,piste, yhd,keskeyta) < sj) sj--;
		}
	if (piste > kilpparam.valuku)  kilp->pv[ik_pv].ysija = sj;
	if (piste >= 0 && piste <= kilpparam.valuku) kilp->pv[ik_pv].va[piste+1].vasija = sj;
	}

void suljekilpailu(void)
{
	vapautaemitmuisti(true, true, true, true);
	if (bdg_kno)
		free(bdg_kno);
	bdg_kno = 0;
	for (int yhd = 0; yhd <= nsarjayhd; yhd++) {
//		for (int i = 0; i < kilpparam.valuku + ((k_pv > 0) ? 3 : 2) ; i++) {
		if (jrjseur[yhd]) {
			free(jrjseur[yhd]);
			jrjseur[yhd] = 0;
			}
		for (int i = 0; i < kilpparam.valuku + 3; i++) {
			if (i > kilpparam.maxvaluku && i <= kilpparam.valuku)
				continue;
			if (jarjtulos[yhd][i]) {
				free(jarjtulos[yhd][i]);
				jarjtulos[yhd][i] = 0;
				}
			}
		}
	free(nameindex.ix);
	nameindex.ix = 0;
	nameindex.nrec = 0;
	free(idindex->ix);
	idindex->ix = 0;
	idindex->nrec = 0;
//	free(posarr);
//	posarr = 0;
	delete[] kilparr;
	kilparr = 0;
}

int uusijarj(void)
{
	kilptietue kilp;
	int keskeyta = 0;

	nilmt = 0;
	ntulost = 0;
	nhylt = 0;
	nkeskt = 0;
	neilt = 0;
	nesitt = 0;
	memset((char *)nva, 0, sizeof(nva));
	memset((char *)nilm, 0, sizeof(nilm));
	memset((char *)nyilm, 0, sizeof(nyilm));
	memset((char *)ntulos, 0, sizeof(ntulos));
	memset((char *)nhyl, 0, sizeof(nhyl));
	memset((char *)nkesk, 0, sizeof(nkesk));
	memset((char *)neil, 0, sizeof(neil));
	memset((char *)nvak, 0, sizeof(nvak));
	memset((char *)neilaika, 0, sizeof(neilaika));
	memset((char *)nilm_pv, 0, sizeof(nilm_pv));
	memset((char *)nesit, 0, sizeof(nesit));
	for (int yhd = 0; yhd <= nsarjayhd; yhd++) {
		for (int d = 0; d < (maxrec+1)*(kilpparam.valuku+4) ; d++)
			jrjseur[yhd][d] = -1;
		for (int i = 0; i < kilpparam.valuku + 4 ; i++) {
			memset((char *)jarjtulos[yhd][i], 0, (maxrec+1)*sizeof(jarjtulostp));
			}
		}
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0) {
			 for (int piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
				if (piste == 0 || kilp.p_aika(piste, 1))
					addjarjarr(&kilp, d, piste, &keskeyta);
		}
	}
	return(0);
}

INT teejarj(bool uusinta)
{
	INT  d,ld,i,lj,nr = 0,piste, er = 0;
	INT  yhtlahtoero = 0;
//	static UINT mx;
	char keyn[2*NIMIINDL];
	INT  okk, ok = 1;
	kilptietue kilp,vkilp;
	int keskeyta = 0;
	wchar_t ch, prs[100], sra[40], st[40], st2[40];
	char *ip;
	INT32 bdg;
	int muutos;

	if (!uusinta) {
		kfirstfree=datf2.firstfree;
		nrec=datf2.numrec;
		keylen = NIMIINDL;
		if (ToimintaTila == 2)
			maxrec=filelen(&datf2) + n_lisaa;
		else
			maxrec = maxkilpno;
		if (maxrec < 3000)
			maxrec = 3000;
		if (maxrec > maxkilpno)
			maxrec = maxkilpno;

#ifdef DEMO
		maxrec=30;                                       /*  DEMO  */
#endif
		kilparr = new kilptietue[maxrec+1];
#if defined(UINTIxx)
		erataul = (eratp *) calloc(maxrec+1, sizeof(eratp));
#endif
		for (int y = 0; y < nsarjayhd; y++)
			for (int v = 0; v < kilpparam.valuku+2; v++)
				yhdsija[y][v] = (INT16 *) calloc(maxrec+1, sizeof(INT16));
		kaynnistysvaihe = 1;
		initindex();
		nameindex.keyl = NIMIINDL;
		nameindex.recl = nameindex.keyl + sizeof(int);
		//   setcmp(nameindex.keyl, nameindex.recl);
		if (!er && (nameindex.ix = (char *)calloc(maxrec+1,nameindex.recl))== NULL)
			er = 1;
//		mx = maxkilpno - minkilpno + 1;
//		if (!er && (posarr = (int *)calloc(mx,sizeof(posarr[0]))) == NULL)
		if (!er && (idindex->ix = (char *)calloc(maxrec+2, idindex->recl)) == NULL)
			er = 1;
		}
	else {			// uusinta
		for (int y = 1; y <= nsarjayhd; y++) {
			if (y > 0) {
				for (int v = 0; v < kilpparam.valuku + 2; v++)
					memset(yhdsija[y-1][v], 0, (maxrec + 1) *sizeof(INT16));
				}
			}
		memset(idindex->ix, 0, (maxrec+2) * idindex->recl);
//		memset(posarr, 0, mx * sizeof(posarr[0]));
		DeAllocJarjSeur(nsarjayhd);
		}
	if (!er) {
		er = AllocJarjSeur(nsarjayhd, maxrec);
		}
   if (er) {
	  writeerror_w(L"Muistissa ei tilaa kilpailijatiedoille",0);
	  return(1);
	  }
   if (emitfl) {
	   if (uusinta) {
		   if (emitfl > 0) {
			   for (i = 0; i < maxrec + 1; i++) {
				   emitjarr[0][i] = -1;
				   if (kilpparam.kaksibadge == 2)
					   emitjarr[1][i] = -1;
				   }
			   }
		   memset(bdg_kno, 0, (kilpparam.kaksibadge ? 2 : 1)*maxrec*sizeof(bdg_kno_tp));
		   }
	   else {
		   bdg_kno = (bdg_kno_tp *)calloc((kilpparam.kaksibadge ? 2 : 1)*maxrec, sizeof(bdg_kno_tp));
		   if (emitfl > 0) {
			   if (bdg_kno) {
				   emitjarr[0] = (int *)malloc(sizeof(int)*(maxrec + 1));
				   if (kilpparam.kaksibadge == 2)
					   emitjarr[1] = (int *)malloc(sizeof(int)*(maxrec + 1));
				   for (i = 0; i < maxrec + 1; i++) {
					   emitjarr[0][i] = -1;
					   if (kilpparam.kaksibadge == 2)
						   emitjarr[1][i] = -1;
						}
					}
			   if (!emitjarr[0]) {
				   if (bdg_kno) {
					   free(bdg_kno);
					   bdg_kno = NULL;
					   }
				   writeerror_w(L"Muisti ei riitä leimantarkastukselle", 0);
				   emitfl = 0;
				   }
				}
			}
	   }
   for ( i = 0 ; i < sarjaluku+nsarjayhd ; i++) {
	  for ( lj = 0 ; lj < 11+kilpparam.valuku ; lj++) jalku[i][lj] = -1;
		}

	d = -1;
	nbadge = 0;
	ld = filelen(&datf2)-1;
	ip = (char *) nameindex.ix;
	if (ld > 0) do {
		okk = FALSE;
		muutos = 0;
		while (!okk && d < ld) {
			okk = !kilp.read(++d, in_data);
			if (kilpparam.badgelaji != L'E')
				if (okk && Sarjat[kilp.Sarja()].rrtas < 2)
					kilp.pv[k_pv].tasoitus = 0;
			kilp.kilpstatus = !okk;
			kilp.PUTREC(d, 0);
			}
		if (okk) {
			nr++;
			ch = L'J';
			if (!uusinta) {
				clrln(19);
				clrln(20);
				clrln(21);
				clrln(22);
				}
			if (ToimintaTila == 2 && (kilp.id() < minkilpno || kilp.id() > maxkilpno)) {
				ok = FALSE;
				swprintf(prs,L"Kilpailijalla %s on virheellinen numero %d",
				kilp.nimi(st, 38, false), kilp.id());
				kirj_err_file(prs, 1);
				}
			else if(ToimintaTila == 2 && getpos(kilp.id()) > 0 ) {
				vkilp.GETREC(getpos(kilp.id()));
				ok = FALSE;
				swprintf(prs,L"Kilpailijoilla %s ja %s on sama numero %d ",kilp.nimi(st, 24, false), vkilp.nimi(st2, 24, false), kilp.id());
				kirj_err_file(prs, 1);
				}
			if (kilp.Sarja() < 0 || kilp.Sarja() >= sarjaluku) {
				ok = FALSE;
				swprintf(prs,L"Kilpailijalla %s on virheellinen sarjanumero : %d, vaihdetaan %s",
				kilp.nimi(st, 24, false), (int) kilp.Sarja() + 1, Sarjat[sarjaluku-1].sarjanimi);
				kilp.setSarja(sarjaluku-1);
				muutos = 1;
				kirj_err_file(prs, 1);
				}
			for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
				if (kilp.Sarja(ipv) < 0 || kilp.Sarja(ipv) >= sarjaluku) {
					kilp.setSarja(kilp.Sarja(), ipv);
					muutos = 1;
					}
				if (!kilpparam.sarjavaihtelee && kilp.Sarja(ipv) != kilp.Sarja()) {
					ok = FALSE;
					swprintf(prs,L"Kilpailijan %s sarja vaihtelee vastoin kilpailun määritystä",
						kilp.nimi(st, 38, false));
					kirj_err_file(prs, 1);
					}
				}
			if (!ok) {
#ifdef _CONSOLE
				wselectopt(L"J)atka,  P)oistu ohjelmasta",L"JP",&ch);
				if( ch == L'P' )
#else
				if (select3(2, L"Jatketaanko kipailijatietojen lukemista?", L"Kilpailijatietojen lukeminen",
					L"Jatka", L"Keskeytä", L"", 0) == 2)
#endif
					return(1);
				else
					ok = TRUE;
				}
			if (ToimintaTila == 2) {
				if (yhtlahto) {
					if (Sarjat[kilp.Sarja()].enslahto[k_pv] == TMAALI0)
						Sarjat[kilp.Sarja()].enslahto[k_pv] = kilp.pv[k_pv].tlahto;
					else if (Sarjat[kilp.Sarja()].enslahto[k_pv] != kilp.pv[k_pv].tlahto)
						yhtlahtoero = 1;
					}
				if (kilp.p_aika(-1) == TMAALI0) {
					kilp.tall_tulos(-1, kilp.pv[k_pv].tlahto);
					muutos = 1;
					}
				}
			if (!uusinta) {
				keyaak(&kilp, keyn, nameindex.keyl, 0);
				memcpy((char *)(ip + nameindex.keyl), (char *)&d, sizeof(int));
				memcpy((char *)ip, (char *)keyn, nameindex.keyl);
				ip += nameindex.recl;
				}
			if (!kilp.tark(k_pv) || wcswcind(kilp.tark(k_pv), L"-KOHEVPTIXMB") < 0) {
				kilp.set_tark(L'-', k_pv);
				muutos = 1;
				}
			wcscpy(sra,kilp.seura);
			elimwbl2(sra);
			if (wcscmp(sra,kilp.seura)) {
				wcscpy(kilp.seura,sra);
				}
			if (muutos)
				kilp.PUTREC(d, 1);
			if (kilp.id() >= minkilpno && kilp.id() <= maxkilpno)
				setpos(kilp.id(), d);
			for (piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
				if (piste == 0 || kilp.p_aika(piste, 1))
					addjarjarr(&kilp, d, piste, &keskeyta);

			// Jos tunnusten käsittely on voimassa, lisätään tunnus indeksiinsä, ellei vaiheen merkintä P

			if (emitfl && kilp.tark(k_pv) != L'P' && kilp.pv[k_pv].badge[0]) {
				bdg_kno[nbadge].badge = kilp.pv[k_pv].badge[0];
				bdg_kno[nbadge].kilpno = kilp.id();
				nbadge++;
				}
			if (emitfl && kilpparam.kaksibadge && kilp.tark(k_pv) != L'P' && kilp.pv[k_pv].badge[1]) {
				bdg_kno[nbadge].badge = kilp.pv[k_pv].badge[1];
				bdg_kno[nbadge].kilpno = kilp.id();
				nbadge++;
				}
			}
		if (kilp.sukup > L' ')
			kilpparam.sukup = true;
		if (kilp.ikasarja > 0)
			kilpparam.ikasarjat = true;
		}
	while (okk);

	if (nr > 0) {
		if (!uusinta) {
			nameindex.nrec = nr;
			quicksort((char *)nameindex.ix, nr, nameindex.keyl, nameindex.recl);
			}
		if (emitfl && nbadge) {
			qsortint(bdg_kno, nbadge, sizeof(bdg_kno_tp), 0, 1);
			nr = nbadge;
			bdg = bdg_kno[0].badge;
			for (i = 1; i < nr; i++) {
				if (bdg_kno[i].badge == bdg) {
					bdg_kno[i].badge = 2000000000L;
					swprintf(prs, L"Kilpailijoilla %d ja %d sama E-koodi %d",
					bdg_kno[i-1].kilpno, bdg_kno[i].kilpno, bdg);
					kirj_err_file(prs, 1);
					nbadge--;
					}
				else
					bdg = bdg_kno[i].badge;
				}
			if (nbadge < nr) {
				qsortint(bdg_kno, nr, sizeof(bdg_kno_tp), 0, 1);
				for (i = nbadge; i < nr; i++) {
					bdg_kno[i].badge = 0;
					bdg_kno[i].kilpno = 0;
					}
				}
			}
		}
#ifdef MAKI
	if (ToimintaTila == 2)
		teevuorotaulu(0, 0);
#endif
	if (yhtlahtoero)
		writeerror_w(L"Yhteislähtökisa - Jossain sarjassa useampia lähtöaikoja.", 0);
#ifndef _CONSOLE
	if (nrec < 2 && Sarjat[kilpparam.vakanttisarja].vakanttisarja) {
		if (select3(2, (L"Kilpailussa vakanttisarja, mutta ei yhtään kilpailijaa. Lisätäänkö " +
			UnicodeString(minvakanttiluku) + L" vakanttia?").c_str(), L"Vakantit", L"Lisää", L"Jätä lisäämättä", L"", NULL) == 1)
			lisaavakantit(minvakanttiluku);
		}
#endif
	if (ToimintaTila == 2) {
		bool paivitatiedot = false;
		for (int srj = 0; srj < sarjaluku; srj++) {
			if (Sarjat[srj].YhtpistOn() || Sarjat[srj].pistekaava[k_pv].vaiheon) {
				paivitatiedot = true;
				break;
				}
			}
		if (paivitatiedot) {
			for (d = 1; i < nrec; i++) {
				kilp.GETREC(d);
				if (kilp.kilpstatus == 0 &&
					(Sarjat[kilp.Sarja()].YhtpistOn() || Sarjat[kilp.Sarja()].pistekaava[k_pv].vaiheon))
					kilp.tallenna(d, 0, 0, 0, 0);
				}
			}
//		if (k_pv > 0)
//			PisteKarjet();
		}
   return(0);
   }

extern INT lainatied;
extern int portparam[MAX_PORTTI];
int opencomfile(int vaihto);
void initajat(INT ino, INT vaihda);
#ifndef _CONSOLE
void haeIkkunat(void);
#endif

int vaihdavaihe(int vaihe, int kielto)
{
	int tmpcomopen[MAX_PORTTI];
	bool comfileopened = false;
	bool ajatFl = false;
	wchar_t msg[80];

#ifdef _CONSOLE
	char *svscr = 0;
	wchar_t  stvaihe[] = L"VAIHE   ";
	char saveline[80];
	wchar_t *header0;

	if (vaiheenvaihto)
		return(0);
	vaiheenvaihto = true;
	for (int cn = 0; cn < maxyhteys; cn++) {
		if (comopen[cn] && !keyclose[cn] && lahettamatta(cn)) {
			writeerror_w(L"Avoimen yhteyden lähettämättä olevat sanomat estävät vaihdon", 0, true);
			return(1);
			}
		}
	header0 = header;
	vidspwmsg(0, 0, 0, 7, L"     VAIHEEN VAIHTO     ");
	Sleep(500);
	svscr = savescr(2, 0, ySize - 5, 64);
	virdrect(ySize - 3, 0, ySize - 3, 79, saveline, 0);
	clearscreen();
	header = L"VAIHEEN VAIHTO";
	kehys(1);
#endif
	memcpy(tmpcomopen, comopen, sizeof(tmpcomopen));
	memset(comopen, 0, sizeof(tmpcomopen));
	if (obfilename[7] == L'1' + k_pv)
		obfilename[7] = L'1' + vaihe;
	for (int ino = 0; ino <= maxjono || (lajatfl && ino < 2); ino++) {
		wchar_t *p;
		if (aikajono[ino] && (p = wcsstr(aikajono[ino]->aikafname, L".")) != NULL && p[-1] == L'1' + k_pv)
			p[-1] = L'1' + vaihe;
		}
	for (int srj = 0; srj < sarjaluku; srj++)
		sijat2(srj, true, false);
	EnterCriticalSection(&tall_CriticalSection);
	EnterCriticalSection(&ajat_CriticalSection);
	EnterCriticalSection(&emit_CriticalSection);
	if (aikajono[0]) {
//	if (aikajono[0] && aikajono[0]->rwtime > 0 ||
//		((lajatfl || maxjono) && aikajono[1] && aikajono[1]->rwtime > 0)) {
		ajatFl = true;
		suljeajat(-1, false);
		}
//	remtimer();
	if (emitfl > 0) {
		vapautaemitmuisti(true, true, true, true);
		if (femitfile)
			fclose(femitfile);
		femitfile = NULL;
		}
	nilmt = 0;
	ntulost = 0;
	nhylt = 0;
	nkeskt = 0;
	neilt = 0;
	nesitt = 0;
	nvakt = 0;
	memset((char *)nva, 0, sizeof(nva));
	memset((char *)nilm, 0, sizeof(nilm));
	memset((char *)nyilm, 0, sizeof(nyilm));
	memset((char *)ntulos, 0, sizeof(ntulos));
	memset((char *)nhyl, 0, sizeof(nhyl));
	memset((char *)nkesk, 0, sizeof(nkesk));
	memset((char *)neil, 0, sizeof(neil));
	memset((char *)nvak, 0, sizeof(nvak));
	memset((char *)neilaika, 0, sizeof(neilaika));
	memset((char *)nilm_pv, 0, sizeof(nilm_pv));
	memset((char *)nesit, 0, sizeof(nesit));
	k_pv = vaihe;
	arv_pv = k_pv;
	vidspwmsg(6, 0, 7, 0, L"Käynnistää uuteen vaiheeseen");
	luesarjat();
	teejarj(true);
	if (emitfl > 0 && !esitark && esiluenta != 1) {
		if (lue_radat_xml(7) == -1) {
			writeerror_w(L"Ratatietojen lukeminen xml-tiedostosta ei onnistunut", 0);
			emitfl = 0;
			}
		else {
			emitfname[4] = L'1' + k_pv;
			if (emitfl == 1 && lueemitfile()) {
				writeerror_w(L"Leimaustietotiedoston lukeminen ei onnistunut", 0);
				emitfl = 0;
				}
			if (emitfl == 1 && lainatied)
				lue_lainat(0, 1);
//			uusiemitanalyysi();
			}
		}
	if (ToimintaTila == 2 && emitfl && kaikki_ajat[0] < 2) {
		if (Sarjat[kilpparam.vakanttisarja].enspv > k_pv)
			Sarjat[kilpparam.vakanttisarja].enspv = k_pv;
		if (Sarjat[kilpparam.vakanttisarja].viimpv < k_pv)
			Sarjat[kilpparam.vakanttisarja].viimpv = k_pv;
		}
	if (ajatFl) {
		for (int ino = 0; ino <= maxjono || (lajatfl && ino < 2); ino++) {
			if (ino == 0) {
				int nc;
				for (nc = 0; nc < NREGNLY; nc++) {
					if (ajanottofl > 0 || regnly[nc]) {
						break;
						}
					}
				if (nc == NREGNLY)
					continue;
				}
			initajat(ino, 2);
			}
		}

	for (int i = 0; i < maxyhteys; i++) {
		if (levypusk && tmpcomopen[i]) {
			if (comfileopened)
				opencomfile(1);
			comfileopened = true;
			lah_alku[i] = 1;
			lah_vaihto[i] = (i == kielto-1) ? 0 : 1;
			}
		}
	memcpy(comopen, tmpcomopen, sizeof(tmpcomopen));
	LeaveCriticalSection(&emit_CriticalSection);
	LeaveCriticalSection(&ajat_CriticalSection);
	LeaveCriticalSection(&tall_CriticalSection);
#ifdef _CONSOLE
	restorescr(2, 0, ySize - 5, 64, svscr);
	viwrrect(ySize - 3, 0, ySize - 3, 79, saveline, 7, 0, 0);
	vidspwmsg(0, 0, 7, 0, L"               ");
	header = header0;
	vidspwmsg(0, 0, 7, 0, header);
	stvaihe[6] = L'1' + k_pv;
	vidspwmsg(0, 55, 7, 0, stvaihe);
	vaiheenvaihto = false;
#endif
	if (vaihevalinta)
		kirjvaihevalinta();
#ifndef _CONSOLE
	haeIkkunat();
#endif
	swprintf(msg, L"Käynnistys vaiheeseen %d valmis", k_pv+1);
	vidspwmsg(ySize - 5, 0, 7, 0, msg);
	return(0);
	}