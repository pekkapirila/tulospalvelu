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
#ifndef _CONSOLE
#include "UnitMain.h"
#endif

int JarjSeur(int yhd, int osuus, int piste, int dKilp)
{
	if (piste > kilpparam.maxvaluku)
		return(-1);
	return((int) jrjseur[yhd][(kilpparam.valuku+1)*(kilpparam.ntosuus*dKilp+osuus)+piste]);
}

void SetJarjSeur(int yhd, int osuus, int piste, int dKilp, int jp)
{
	if (piste > kilpparam.maxvaluku)
		return;
	jrjseur[yhd][(kilpparam.valuku+1)*(kilpparam.ntosuus*dKilp+osuus)+piste] = (INT16) jp;
}

int JarjTulos(int yhd, int os, int piste, int d)
{
	if (piste > kilpparam.maxvaluku)
		return(0);
	return((int)(jarjtulos[yhd][os][piste][d]/kilpparam.jtulosjak));
}

void SetJarjTulos(int yhd, int os, int piste, int d, __int64 jtls)
{
	if (piste > kilpparam.maxvaluku)
		return;
	jarjtulos[yhd][os][piste][d] = jtls;
}

int OsJarjSeur(int yhd, int srj, int *aos, int dKilp)
{
	int ip, yos = Sarjat[srj].yosuus[*aos];
	ip = (int) jrjosseur[yhd][(kilpparam.maxnosuus*kilpparam.ntosuus*dKilp+
		kilpparam.maxnosuus*yos + *aos - Sarjat[srj].aosuus[yos] - 1)];
	*aos = Sarjat[srj].aosuus[yos] + 1 + ip%kilpparam.maxnosuus;
	return(ip/kilpparam.maxnosuus);
}

void SetOsJarjSeur(int yhd, int srj, int aos, int dKilp, int jos, int jp)
{
	int yos = Sarjat[srj].yosuus[aos];

#ifdef _DEBUG
	if (jp < -1 || jp == 0 || yos < 0 || yos >= Sarjat[srj].ntosuus || aos < 0 ||
		aos >= Sarjat[srj].osuusluku || jos - Sarjat[srj].aosuus[yos] - 1 < 0 ||
		jos - Sarjat[srj].aosuus[yos] - 1 >= kilpparam.maxnosuus) {
		jrjosseur[yhd][0] =
			(INT16) (kilpparam.maxnosuus*jp + jos - Sarjat[srj].aosuus[yos] - 1);
		jrjosseur[yhd][1] = kilpparam.maxnosuus*kilpparam.ntosuus*dKilp +
			kilpparam.maxnosuus*yos + aos - Sarjat[srj].aosuus[yos] - 1;
		return;
		}
#endif
	if (jp < 0)
		jrjosseur[yhd][(kilpparam.maxnosuus*kilpparam.ntosuus*dKilp +
			kilpparam.maxnosuus*yos + aos - Sarjat[srj].aosuus[yos] - 1)] =
				-kilpparam.maxnosuus;
	else
		jrjosseur[yhd][(kilpparam.maxnosuus*kilpparam.ntosuus*dKilp +
			kilpparam.maxnosuus*yos + aos - Sarjat[srj].aosuus[yos] - 1)] =
			(INT16) (kilpparam.maxnosuus*jp + jos - Sarjat[srj].aosuus[yos] - 1);
}

int JarjOsTulos(int yhd, int os, int d)
{
	return((int)(jarjostulos[yhd][os][d]/kilpparam.jtulosjak));
}

void SetJarjOsTulos(int yhd, int os, int d, __int64 jtls)
{
	jarjostulos[yhd][os][d] = jtls;
}

static int AllocJarjSeur(int nsrjyhd, int maxnrec)
{
	for (int yhd = 0; yhd <= nsrjyhd; yhd++) {
		if ((jrjseur[yhd] = (INT16 *) malloc((maxnrec+1)*kilpparam.ntosuus*(kilpparam.valuku+1)*sizeof(jrjseur[0][0]))) == NULL)
			return(1);
		for (int j = 0; j < (maxnrec+1)*kilpparam.ntosuus*(kilpparam.valuku+1); j++)
			jrjseur[yhd][j] = -1;
		if ((jrjosseur[yhd] = (INT16 *) malloc((maxnrec+1)*kilpparam.maxnosuus*kilpparam.ntosuus*sizeof(jrjosseur[0][0]))) == NULL)
			return(1);
		for (int j = 0; j < (maxnrec+1)*kilpparam.maxnosuus*kilpparam.ntosuus; j++)
			jrjosseur[yhd][j] = -kilpparam.maxnosuus;
		for (int os = 0; os < kilpparam.ntosuus; os++) {
			for (int i = 0; i < kilpparam.valuku + 4; i++) {
				if ((jarjtulos[yhd][os][i] = (__int64 *) calloc(maxrec+1,sizeof(__int64))) == NULL) {
					return(1);
					}
				}
			}
		for (int os = 0; os < kilpparam.osuusluku; os++) {
			if ((jarjostulos[yhd][os] = (__int64 *) calloc((maxrec+1)*kilpparam.maxnosuus,sizeof(__int64))) == NULL)
				return(1);
			}
		}
	return(0);
}

int askellasarja(int srj, int osuus, int *aos, int piste, int p, int *lj)
{
	int yhd = 0;

	if (srj >= sarjaluku && srj < sarjaluku+nsarjayhd)
		yhd = srj-sarjaluku+1;
	if (osuus == -1)
		osuus = Sarjat[srj].ntosuus-1;
	if (piste <= kilpparam.valuku) {
		if (*lj >= 0 && p > 0)
			p = JarjSeur(yhd, osuus, piste, p);
		while ((p < 0 || *lj < 0) && *lj < piste+4) {
			p = jalku[srj][osuus][++*lj];
			}
		if (p == 0)
			p = -1;
		return(p);
		}
	else {
		int osseur = Sarjat[srj].aosuus[osuus]+1;

		if (aos == 0)
			aos = &osseur;
		if (p < 0) {
			p = JosAlku(srj, osuus, aos);
			}
		else
			p = OsJarjSeur(yhd, srj, aos, p);
		if (p == 0)
			p = -1;
		return(p);
		}
}

//  Indeksit
//	0	sarjaindex	keysarja	53	sarja:2   	lahtoryhma:1 seura:20	nimi:30
//	1	seuraindex	keyse    	52	maa:2   	seura:20	nimi:30
//	2	srjaakindex	keysa   	32	sarja:2 	nimi:30
//	3	lahtoindex	keyla   	23	lno:1   	tlahto:4	sarja:2	kilpno:2
//		- ilman aikoja		    	lno:1   	seura:20	kilpno:2
//	4	ljarjindex	keyjrj  	16	sarja:2 	tlahto:4	tulos:4	edlahto:4	kilpno:2		jos takaa-ajo tai l‰htˆvali != 0
//									sarja:2 	kilpno:2								tulosfl=0
//									sarja:2 	edtulos:4								tulosfl=1 tai -1
//	5	aakkosindex	keyaak    	32	nimi:30 	sarja:2
//	6	nroindex	keynro    	2	kilpno:2
//	7	srjnroindex	keysrjnro	4	sarja:2 	kilpno:2
//

#define NINDEX 3
kilpindex ixarr[NINDEX];
kilpindex *srjaakindex;
kilpindex *srjnroindex;
kilpindex *ostlsindex;
char *keysa(void *vkilp, char *key, int keylen, int flags);
char *keysrjnro(void *vkilp, char *key, int keylen, int flags);
char *keyostls(void *vkilp, char *key, int keylen, int flags);
int reindex;

void initindex(void)
   {
   INT i;
/*
   sarjaindex = ixarr;
   sarjaindex->keyf = keysarja;
   sarjaindex->keyl = 53;

   seuraindex = ixarr + 1;
   seuraindex->keyf = keyse;
   seuraindex->keyl = 52;
*/
   srjaakindex = ixarr;
   srjaakindex->keyf = keysa;
   srjaakindex->keyl = NIMIINDL+2;
/*
   lahtoindex = ixarr + 3;
   lahtoindex->keyf = keyla;
   lahtoindex->keyl = 23;

   ljarjindex = ixarr + 4;
   ljarjindex->keyf = keyjrj;
   ljarjindex->keyl = 16;

   aakkosindex = ixarr + 5;
   aakkosindex->keyf = keyaak;
   aakkosindex->keyl = 32;
   nroindex = ixarr + 6;
   nroindex->keyf = keynro;
   nroindex->keyl = 2;
*/
   srjnroindex = ixarr + 1;
   srjnroindex->keyf = keysrjnro;
   srjnroindex->keyl = 2;

   ostlsindex = ixarr + 2;
   ostlsindex->keyf = keyostls;
   ostlsindex->keyl = 8+HAJONTA;

   for (i=0; i < NINDEX; i++)
	  ixarr[i].recl = ixarr[i].keyl + sizeof(int);
/*
   pstindex.keyf = keysrjpst;
   pstindex.keyl = 6;
   pstindex.recl = 10;
   pstindex.flags = 0;
*/
   }

int teeindeksi(kilpindex *ind, bool uusi)
{
   int  i, d;
   int n, ixsize;
//   char *keyn;
   char *ip;
   kilptietue kilp;
   int iflags = 0;
   bool osuusix = false;

   reindex = 1;
   ind->pos = -1;
   if (!uusi) {
	   for (i=0; i < NINDEX; i++) {
		  if (ixarr[i].ix != NULL && ind == ixarr + i) {
			  free(ixarr[i].ix);
			  ixarr[i].ix = NULL;
			 }
		  }
	   }
   n = 0;
   if (ind == ostlsindex) {
		ixsize = kilpparam.osuusluku * datf2.numrec;
		iflags = ind->flags & 0xffffff80;
		osuusix = true;
		}
   else {
		ixsize = datf2.numrec;
		iflags = ind->flags;
		}
   if (ind->ix && ind->nrec < ixsize) {
		free(ind->ix);
		ind->ix = NULL;
		}
   if (ind->ix == NULL) {
	   if ((ind->ix = (char *) calloc(ixsize+1,ind->recl)) == NULL) {
		  writeerror_w(L"Muistissa ei tilaa indeksille", 4000);
		  return(1);
		  }
	   }
   else
	   memset(ind->ix, 0, ind->recl*ind->nrec);
   ip = ind->ix;
   for ( d = 1; (UINT32)d < datf2.numrec; d++) {
	  kilp.getrec(d);
	  if (kilp.kilpstatus == 0) {
		 int os = 0;
		 do {
			if (osuusix)
				iflags = (iflags & 0xffffff80) | os;
			ind->keyf(&kilp, ip, ind->keyl, iflags);
			*(int *)(ip + ind->keyl) = d;
			n++;
			ip += ind->recl;
			os++;
			} while (osuusix && os < Sarjat[kilp.sarja].osuusluku);
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
/*
char *keysarja(void *vkilp, char *key, int keylen, int flags)
{
	char Buf[80];
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->sarja;
	_swab((char *)&srj, key, 2);
	if ((flags & 0x4000) && kilp->Tark(0) == 'V')
		key[0] += 0x40;
	keyfromU8name(key+3, kilp->seuratieto(NULL), min(20, keylen-2), 0);
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
*/
char *keysa(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->sarja;
	_swab((char *)&srj, key, 2);
	if (keylen > 2)
		memcpy(key+2, keyname(kilp), NIMIINDL);
   return(key);
}

char *keyostls(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT32 tls;
	INT16 srj, os, piste = 0;

	memset(key, 0, keylen);
	kilp = (kilptietue *)vkilp;
	srj = kilp->sarja;
	_swab((char *)&srj, key, 2);
	os = flags & 0x7f;
	if (os < Sarjat[kilp->sarja].osuusluku) {
		if (flags & 0x100)
			memcpy(key+2, kilp->ostiet[os].hajonta, HAJONTA);
		_swab((char *)&os, key+HAJONTA+6, 2);
		if ((tls = kilp->osTulos(os, piste, false)) != 0) {
			lswab((long *)&tls);
			*(INT32 *) (key+2+HAJONTA) = tls;
			}
		else
			key[HAJONTA+3] = '\x80';
		switch (kilp->wTark(os, flags & 0x200)) {
			case L'K':
				key[2+HAJONTA] = '\xfe';
				break;
			case L'H':
				key[2+HAJONTA] = '\xfd';
				break;
			case L'S':
				key[2+HAJONTA] = '\xfe';
				break;
			case L'E':
			case L'P':
				key[2+HAJONTA] = '\xff';
				break;
			}
		}
	else {
		key[2+HAJONTA] = '\xff';
		}
   return(key);
}
/*
char *keyla(void *vkilp, char *key, int keylen, int flags)
{
	INT dd;
	INT32 tl;
	INT16 kno;
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->Sarja(arv_pv);
	kno = kilp->id(arv_pv);
	memset(key, 0, keylen);
	dd = max(arv_pv, Sarjat[srj].enspv);
	if ((flags & 1) == 0)
		key[0] = (char) Sarjat[srj].lno[dd];
	if (lahtojarjfl || Sarjat[srj].lahtovali[dd] != 0) {
		if (!sailpo && kilp->tark(dd) == L'P')
			memset(key+1, '\xff', 4);
		else if (kilp->pv[dd].tlahto == TLAHTO0)
			memset(key+1, '\xff', 3);
		else {
			tl = (kilp->pv[dd].tlahto + 129600L*SEK) % (86400L*SEK);
			memcpy(key+1, &tl, 4);
			lswab((long *)(key+1));
			}
		_swab((char *) &srj, key+5, 2);
		_swab((char *)&kno, key+7, 2);
		memset(key+9, 0, keylen-9);
		}
	else {
		keyfromwname(key+1, kilp->seuratieto(NULL), 20, 0);
		_swab((char *)&kno, key+21, 2);
		}
	return(key);
}

char *keyjrj(void *vkilp, char *key, int keylen, int flags)
{
	INT dd;
	INT32 tl, tls;
	kilptietue *kilp;
	INT16 kno;
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
		if (!sailpo && kilp->tark(dd) == L'P')
			memset(key+2, '\xff', 4);
		else if (kilp->pv[dd].tlahto == TLAHTO0)
			memset(key+2, '\xff', 3);
		else {
			tl = (kilp->pv[dd].tlahto + 129600L*SEK) % (86400L*SEK);
			memcpy(key+2, &tl, 4);
			lswab((long *)(key+2));
			if (dd > 0) {
				if ((tl = kilp->tulos_pv(dd-1, true, 2)) != 0 &&
					(kilp->tark(dd-1) == L'-' || kilp->tark(dd-1) == L'T')) {
					memcpy(key+6, &tl, 4);
					lswab((long *)(key+6));
					}
				else
					memset(key+6, '\xff', 4);
				if (kilp->pv[dd-1].tlahto == TLAHTO0)
					memset(key+10, '\xff', 4);
				else {
					tl = (kilp->pv[dd-1].tlahto + 129600L*SEK) % (86400L*SEK);
					memcpy(key+10, &tl, 4);
					lswab((long *)(key+10));
					}
				}
			}
		_swab((char *)&kno, key+14, 2);
		}
	else {
		switch (tulosfl) {
			case 0 :
				_swab((char *)&kno, key+2, 2);
				break;
			case -1 :
			case 1 :
				tls = tulosfl * kilp->tulos_pv(0, true, 2);
				if (tls == 0) {
					tls = kilp->id(arv_pv);
					if (tulosfl == 1)
						tls += 100000000L;
					}
				memcpy(key+2, &tls, 4);
				lswab((long *)(key+2));
				break;
			}
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
	INT16 kno;

	kilp = (kilptietue *)vkilp;
	kno = kilp->id(arv_pv);
   _swab((char *)&kno, key, 2);
   return(key);
}
*/
char *keysrjnro(void *vkilp, char *key, int keylen, int flags)
{
	kilptietue *kilp;
	INT16 srj;

	kilp = (kilptietue *)vkilp;
	srj = kilp->sarja;
	_swab((char *)&srj, key, 2);
   return(key);
}

//  entsija etsii kilpailijan sijan linkitetyll‰ listalla l‰htien
//  kilpailijasta *p seuraten j‰senen JarjSeur(yhd, piste, dKilp) mukaista linkki‰ listan
//  seuraavaan. Etsint‰ p‰‰ttyy, kun seuraava on -1. Numerot viittaavat
//  sijaintiin ketjussa JarjSeur. (Sijainti on sama kuin tiedostossa KILP.DAT).
//  Jos kilpailijaa ei lˆydy on sija 9999.
//  p osoittaa lˆytymist‰ edelt‰v‰‰n tietueeseen, paitsi tapauksessa sj=1
//  itse tietueeseen (siis t‰llˆin *p == d)

INT entsija(INT d, INT *p, int osuus, INT piste, INT yhd, int *keskeyta)
	{
	INT  sj;
	INT  jp;

	sj = 1;
	jp = *p;
	while ( (jp >= 0) && (d != jp) ) {
		*p = jp;
		jp = JarjSeur(yhd, osuus, piste, jp);
		sj++;
		if (sj > nilmt+1 || jp > maxrec) {
			if (keskeyta)
				*keskeyta = *keskeyta | 1;
			break;
			}
		}
	if (d != jp) sj = 9999;
	return(sj);
	}

INT entossija(INT d, INT *p, int srj, int osuus, int *osseur, INT yhd, int *keskeyta)
	{
	INT  sj, os;
	INT  jp;

	sj = 1;
	jp = *p;
	os = *osseur;
	while (jp >= 0 && (d != jp || osuus != os)) {
		*p = jp;
		*osseur = os;
		jp = OsJarjSeur(yhd, srj, &os, jp);
		sj++;
		if (sj > kilpparam.maxnosuus*nilmt+1 || jp > maxrec) {
			if (keskeyta)
				*keskeyta = *keskeyta | 1;
			break;
			}
		}
	if (d != jp) sj = 9999;
	return(sj);
	}

//  intsija etsii kilpailijan sijan linkitetyll‰ listalla l‰htien
//  kilpailijasta *p seuraten j‰senen JarjSeur(yhd, piste, dKilp) mukaista linkki‰ listan
//  seuraavaan. Etsint‰ p‰‰ttyy, kun on lˆytyy huonompi tulos. Numerot viittaavat
//  sijaintiin ketjussa JarjSeur. (Sijainti on sama kuin tiedostossa KILP.DAT).
//  Jos kilpailijaa ei lˆydy on sija 9999.
//  p osoittaa lˆytymist‰ edelt‰v‰‰n tietueeseen, paitsi tapauksessa sj=1
//  itse tietueeseen (siis t‰llˆin *p == d)

INT intsija(__int64 tl, INT *p, int osuus, INT piste, INT yhd, int *keskeyta)
	{
	INT sj ;
	INT jp ;

	sj = 1;
	jp = *p;
	while (jp >= 0 && tl > jarjtulos[yhd][osuus][piste][jp]) {
		*p = jp;
		jp = JarjSeur(yhd, osuus, piste, jp);
		sj++;
		if (sj > nilmt || jp > maxrec || jp < -1) {
			if (keskeyta)
				*keskeyta = *keskeyta | 2;
			break;
			}
		}
	return(sj);
	}

INT intossija(__int64 tl, INT *p, int srj, int *aos, INT yhd, int *keskeyta)
	{
	INT sj ;
	INT jp ;
	int eos;

	sj = 1;
	jp = *p;
	eos = *aos;
	while (jp >= 0 && tl > jarjostulos[yhd][eos][jp]) {
		*p = jp;
		*aos = eos;
		jp = OsJarjSeur(yhd, srj, &eos, jp);
		sj++;
		if (sj > kilpparam.maxnosuus*nilmt || jp > maxrec || jp < -1) {
			if (keskeyta)
				*keskeyta = *keskeyta | 2;
			break;
			}
		}
	return(sj);
	}

void paivJoukkLkm(int srj, int muutos)
{
	nilmt += muutos;
	nilmtt += muutos * (kilpparam.partio ? 1 : Sarjat[srj].osuusluku);
	nilm[srj] += muutos;
}

#define RAJA 12*TUNTI
#define RAJA2 12*TUNTI

//  J‰rjestystiedoista huolehditaan taulukoiden jarjtulos ja jrjseur avulla.
//  Taulukoissa on maxrec+2 alkiota (viimeist‰ ei tarvita). Taulukko jrjseur
//  on
//  Osuustulosta vastaava indeksi on aina kilpparam.valuku+1.
//  Tietojen sijainti taulukoissa vastaa kilpailijan sijaintia kilpailija-
//  tietotaulukossa ja se saadaan taulukosta posarr kilpailijanumeron
//  perusteella.
//  Taulukko jarjtulos sis‰lt‰‰ kutakin tulosta ja v‰liaikaa vastaavat 64-bittisen
//  j‰rjestysmuuttujan ja taulukko jrjseur viittauksen j‰rjestyksess‰ seuraavaan kilpailijaan.
//  Jos seuraavaa kilpailijaa ei ole on viittaus -1.
//  Taulukon jrjseur ketjulinkit on jaettu osajonoihin, joita on jokaiselle
//  sarjalle korkeintaan 10+kilpparam.valuku. Kunkin jonon alkupiste kerrotaan
//  taulukossa jalku seuraavassa annetun indeksin kohdalla.
//  Osuuden tuloksille on varattu osajonot
//    0 : hyv‰ksytty tulos
//    1 : keskeytt‰neet
//    2 : hyl‰tyt
//    3 : ei-l‰hteneet
//    4 : muut eli avoimet
//  Poissa olevia ei sis‰llytet‰ taulukkoon.
//  Osuuden tulosten osalta vastaava tulos on tapauksessa
//    0 : tulos kertaa AIKAJAK
//    1, 2, 3 : aikaisempiin osuuksiin perustuva luku. Viimeisin hyv. vaihe ja sen tulos
//    4 : Viimeisin hyv. vaihe ja sen tulos
//
//  V‰liajoille jonot
//    5 .. kilpparam.valuku+4
//  vastava j‰rjestystulos on tulos. Ilman v‰liaikaa olevia ei indeksoida.
//
//  Osuustuloksille on varattu jono:
//
//    kilpparam.valuku+5 : osuustulos vaihdossa/maalissa

void addjarjarr(kilptietue *kilp, int d, int osuus, INT piste, int *keskeyta)
	{
	INT  tllj = -1,p,sj = 99999, yhd = 0, srj;
	__int64 tl, lisa;
	char kh;

	if (ToimintaTila == 1 && piste != 0)
		return;
	srj = kilp->sarja;
	if (kilpparam.partio && osuus != Sarjat[srj].osuusluku - 1)
		return;

	for (yhd = 0; yhd <= nsarjayhd; yhd++) {
		if (yhd > 0 && yhdsarjat[yhd-1][kilp->sarja] == 0)
			continue;
		if (yhd > 0)
			srj = sarjaluku + yhd - 1;
	   modif[srj][osuus] = TRUE;
	   kh = kilp->tTark(osuus);
	   if (piste == 0) {
		   for (int aos = Sarjat[srj].aosuus[osuus] + 1; aos <= Sarjat[srj].aosuus[osuus+1]; aos++) {
			   if (emitfl && kilp->Tark(aos, 0) == 'I') {
				  nesitt++;
				  nesit[srj]++;
				  }
			   if (kilp->osHyv(aos) && kilp->Maali(aos, 0) == TMAALI0) {
				  navoint[osuus]++;
				  navoin[srj][osuus]++;
				  }
			   if (kilp->ostiet[aos].badge[0])
				  nekoodi[srj][osuus]++;
			   }
		   if (kilp->ostiet[osuus].ylahto != TMAALI0)
			  nylahto[srj][osuus]++;
		   }
	   tl = jtulos(kilp,osuus,piste);
	   tllj = -1;

	   if (tl && (tl < kilpparam.jtulosaskel || (kilpparam.rogaining && tl < 72LL*TUNTI*30001LL))) {

		  tllj = piste ? 4+piste : 0;
		  ntulosix[srj][osuus][piste]++;
		  if (Sarjat[srj].nosuus[osuus] == 1 || (kilp->tTulos(osuus, piste, &lisa) && lisa == 0))
			  ntulos[srj][osuus][piste]++;
		  if (piste == 0)
			  ntulost[osuus]++;
		  }
	   else if (piste == 0) {
		  switch (kh) {
			 case 'K' :
				nkesk[srj][osuus]++;
				nkeskt[osuus]++;
				tllj = 1;
				break;
			 case 'P' :
			 case 'E' :
			 case 'S' :
				neil[srj][osuus]++;
				neilt[osuus]++;
				tllj = 3;
				break;
			 case 'H' :
				nhylt[osuus]++;
				nhyl[srj][osuus]++;
				tllj = 2;
				break;
			 default  :
				tllj = 4;
			 }
		   }
		if (ToimintaTila < 2)
		   return;
		if (tllj >= 0) {
			p = jalku[srj][osuus][tllj];
			sj = intsija(tl,&p,osuus, piste,yhd,keskeyta);
				// p osoittaa alkuun tai uutta kilpailijaa paremmuudessa edelt‰v‰‰n
			if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) || p > maxrec)
				{
				if (keskeyta)
					*keskeyta = *keskeyta | 4;
				return;
				}
			if (tllj == (piste == 0 ? 0 : 4+piste))
				kilp->settSija(osuus, piste, sj);
			else
				kilp->settSija(osuus, piste, 0);
			if (sj == 1) {   // Jos tulos menee k‰rkeen, uudelle kilpailijalle tallentuu aiempi alkulinkki
				SetJarjSeur(yhd, osuus, piste, d, p);
						 // Jos tulos menee k‰rkeen, vaihtuu jalku osoittamaan uuteen kilpailijaan
				jalku[srj][osuus][tllj] = d;
						 //  K‰rkitulos tallennetaan hyv‰ksytyn tuloksen saaneille
				if (piste > 0 || tllj == 0) {
					pkarki[srj][osuus][piste] = kilp->tTulos(osuus, piste);
					}
				}
			else  {  // Jos tulos ei mene k‰rkeen, vaihdetaan edellisen jatkolinkki uudelle
				SetJarjSeur(yhd, osuus, piste, d, JarjSeur(yhd, osuus, piste, p));
					 // ja edellisellelinkki uuteen
				SetJarjSeur(yhd, osuus, piste, p, d);
				}
				// tauluun jarjtulos tallennetaan j‰rjestystulos.
			SetJarjTulos(yhd, osuus, piste, d, tl);
			}
		if (yhd == 0 && tllj == (piste == 0 ? 0 : piste+4) && ntulosix[kilp->sarja][osuus][piste] >= sj && !lataus)
			merk_sijat(kilp->sarja,osuus,piste,sj);
		tlsmuutos[srj][osuus][piste] = 1;
		if (ToimintaTila == 2 && piste == 0) {    //  Vaihdon ja maalin osuustulosten j‰rjestys
				// Vain hyv‰ksytyt tulokset kirjataan
			for (int aos = Sarjat[srj].aosuus[osuus] + 1; aos <= Sarjat[srj].aosuus[osuus+1]; aos++) {
				sj = 0;
				if ((tl = jtulos(kilp, aos, kilpparam.valuku+1)) < kilpparam.jtulosaskel) {
					int osseur;
					ntulosix[kilp->sarja][osuus][kilpparam.valuku+1]++;
					ntulos[kilp->sarja][osuus][kilpparam.valuku+1]++;
					p = JosAlku(srj, osuus, &osseur);
					sj = intossija(tl, &p, srj, &osseur, yhd, keskeyta);
					// p osoittaa alkuun tai uutta kilpailijaa paremmuudessa edelt‰v‰‰n
					if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) ||
						p > kilpparam.maxnosuus*maxrec) {
						if (keskeyta)
							*keskeyta = *keskeyta | 8;
						return;
						}
					if (sj == 1) {   // Jos tulos menee k‰rkeen, uudelle kilpailijalle tallentuu aiempi alkulinkki
						SetOsJarjSeur(yhd, srj, aos, d, osseur, p);
							 // Jos tulos menee k‰rkeen, vaihtuu jalku osoittamaan uuteen kilpailijaan
						setJosAlku(srj, aos, d);
						pkarki[srj][osuus][kilpparam.valuku+1] = kilp->osTulos(aos, 0, true);
						}
					else  {  // Jos tulos ei mene k‰rkeen, vaihdetaan edellisen jatkolinkki uudelle
						int eos = osseur;
						int p1 = OsJarjSeur(yhd, srj, &eos, p);
						SetOsJarjSeur(yhd, srj, aos, d, eos, p1);
							 // ja edellisellelinkki uuteen
						SetOsJarjSeur(yhd, srj, osseur, p, aos, d);
						}
					SetJarjOsTulos(yhd, aos, d, tl);
					}
				kilp->settSija(osuus, kilpparam.valuku+1, sj);
				if (ntulosix[kilp->sarja][osuus][kilpparam.valuku+1] >= sj && !lataus)
					merk_sijat(kilp->sarja,osuus,kilpparam.valuku+1,sj);
				}
			tlsmuutos[srj][osuus][kilpparam.valuku+1] = 1;
			}
		}
	}

void remjarjarr(kilptietue *kilp, int d, int osuus, INT piste, int *keskeyta)
{
	INT tllj = -1, p, sj = 99999, yhd = 0, srj;
	__int64 tl, lisa;
	char kh;

	if (ToimintaTila == 1 && piste != 0)
		return;
	srj = kilp->sarja;
	if (kilpparam.partio && osuus != Sarjat[srj].osuusluku - 1)
		return;
	for (yhd = 0; yhd <= nsarjayhd; yhd++) {
		if (yhd > 0 && yhdsarjat[yhd-1][kilp->sarja] == 0)
			continue;
		if (yhd > 0)
			srj = sarjaluku + yhd - 1;
		modif[srj][osuus] = TRUE;
		kh = kilp->tTark(osuus);
		if (piste == 0) {
		   for (int aos = Sarjat[srj].aosuus[osuus] + 1; aos <= Sarjat[srj].aosuus[osuus+1]; aos++) {
			   if (emitfl && kilp->Tark(aos, 0) == 'I') {
				  nesitt--;
				  nesit[srj]--;
				  }
			   if (kilp->osHyv(aos) && kilp->Maali(aos, 0) == TMAALI0) {
				  navoint[osuus]--;
				  navoin[srj][osuus]--;
				  }
			   if (kilp->ostiet[aos].badge[0])
				  nekoodi[srj][osuus]--;
			   }
		   if (kilp->ostiet[osuus].ylahto != TMAALI0)
			  nylahto[srj][osuus]--;
		   }
		tl = jtulos(kilp,osuus,piste);
		tllj = -1;
		if (tl && (tl < kilpparam.jtulosaskel || (kilpparam.rogaining && tl < 72LL*TUNTI*30001LL))) {
			tllj = piste ? piste + 4 : 0;
			if (piste == 0) {
				ntulosix[srj][osuus][0]--;
				if (Sarjat[srj].nosuus[osuus] == 1 || (kilp->tTulos(osuus, 0, &lisa) && lisa == 0))
					ntulos[srj][osuus][0]--;
				ntulost[osuus]--;
				}
			}
		else if (piste == 0) {
			switch (kh) {
				case 'K' :
					nkesk[srj][osuus]--;
					nkeskt[osuus]--;
					tllj = 1;
					break;
				case 'P' :
				case 'E' :
				case 'S' :
					neil[srj][osuus]--;
					neilt[osuus]--;
					tllj = 3;
					break;
				case 'H' :
					nhylt[osuus]--;
					nhyl[srj][osuus]--;
					tllj = 2;
					break;
				default  :
					tllj = 4;
				}
			}
		if (ToimintaTila < 2)
			return;

		if (tllj >= 0) {
			p = jalku[srj][osuus][tllj];
			if (p < 0) {
				sj = 9999;
				}
			else {
				sj = entsija(d,&p,osuus,piste,yhd,keskeyta);
				if (d < 0 || d > maxrec || p < 0 || p > maxrec) {
					if (keskeyta)
						*keskeyta = *keskeyta | 16;
					return;
					}
				}
			if (sj < 9999) {
				if (piste > 0) {
					ntulosix[srj][osuus][piste]--;
					if (Sarjat[srj].nosuus[osuus] == 1 || (kilp->tTulos(osuus, piste, &lisa) && lisa == 0))
						ntulos[srj][osuus][piste]--;
					}
				if (sj == 1) {
					jalku[srj][osuus][tllj] = JarjSeur(yhd, osuus, piste, d);
					if (piste || tllj == 0) {
						kilptietue tkilp;
						int td;
						if ((td = jalku[srj][osuus][tllj]) > 0) {
							GETREC(&tkilp, td);
							pkarki[srj][osuus][piste] = tkilp.tTulos(osuus, piste);
							}
						else
							pkarki[srj][osuus][piste] = 0;
						}
					}
				else
					SetJarjSeur(yhd, osuus, piste, p, JarjSeur(yhd, osuus, piste, d));
				SetJarjSeur(yhd, osuus, piste, d, -1);
				}
			}
		if (tllj == (piste == 0 ? 0 : piste+4) && ntulosix[kilp->sarja][osuus][piste] >= sj)
			merk_sijat(srj,osuus,piste,sj);
		tlsmuutos[srj][osuus][piste] = 1;
		if (ToimintaTila == 2 && piste == 0) {
			for (int aos = Sarjat[srj].aosuus[osuus] + 1; aos <= Sarjat[srj].aosuus[osuus+1]; aos++) {
				sj = 0;
				if ((tl = jtulos(kilp, aos, kilpparam.valuku+1)) < kilpparam.jtulosaskel) {
					int p1;
					int eos1;
					int eos = aos;

					ntulosix[kilp->sarja][osuus][kilpparam.valuku+1]--;
					ntulos[kilp->sarja][osuus][kilpparam.valuku+1]--;
					p = JosAlku(srj, osuus, &eos);
					sj = entossija(d, &p, srj, aos, &eos, yhd, keskeyta);
					// p osoittaa alkuun tai uutta kilpailijaa paremmuudessa edelt‰v‰‰n
					if (d < 0 || d > maxrec || p < -1 || (p == -1 && sj > 1) ||
						p > maxrec) {
						if (keskeyta)
							*keskeyta = *keskeyta | 8;
						return;
						}
					eos1 = aos;
					p1 = OsJarjSeur(yhd, srj, &eos1, d);
					if (sj == 1) {   // Jos tulos menee k‰rkeen, uudelle kilpailijalle tallentuu aiempi alkulinkki
	//					kilptietue tkilp;
						setJosAlku(srj, eos1, p1);
						if (p1 > 0) {
							pkarki[srj][osuus][kilpparam.valuku+1] = JarjOsTulos(yhd, eos1, p1);
	//						GETREC(&tkilp, p1);
	//						pkarki[srj][osuus][kilpparam.valuku+1] = tkilp.osTulos(aos, 0, 0);
							}
						else
							pkarki[srj][osuus][kilpparam.valuku+1] = 0;
						}
					else {
						SetOsJarjSeur(yhd, srj, eos, p, eos1, p1);
						}
					SetOsJarjSeur(yhd, srj, aos, d, 0, -1);
					}
				}
			tlsmuutos[srj][osuus][kilpparam.valuku+1] = 1;
			if (ntulosix[kilp->sarja][osuus][kilpparam.valuku+1] >= sj)
				merk_sijat(srj,osuus,kilpparam.valuku+1,sj);
			}
		}
}

void haesija(kilptietue *kilp, int osuus, INT piste, INT yhd, int *keskeyta)
	{
	INT  sj = 0, p;
	__int64 tl = 0;

	if ((tl = jtulos(kilp, osuus, 0)) != 0 && tl < kilpparam.jtulosaskel) {
		p = jalku[kilp->sarja][osuus][piste ? 4+piste : 0];
		sj = intsija(tl, &p, osuus, piste,yhd,keskeyta);
		p = jalku[kilp->sarja][osuus][piste ? 4+piste : 0];
		if (entsija(getpos(kilp->kilpno),&p,osuus, piste, yhd,keskeyta) < sj)
			sj--;
		}
	if (piste > kilpparam.valuku)
		kilp->ostiet[osuus].ossija = sj;
	if (piste >= 0 && piste <= kilpparam.maxvaluku)
		kilp->settSija(osuus, piste, sj);
}

void uusiKilpnoIx(void)
{
	memset(kilpnoix, 0, (maxkilpno - minkilpno + 1) * sizeof(int));
	for (int d = 1; d < (int) datf2.numrec; d++) {
		kilptietue kilp;
		kilp.getrec(d);
		if (kilp.kilpstatus == 0)
			setpos(kilp.kilpno, d);
		}
}

int teejarj(void)
{
   int  d,ld,i,nr,os,er = 0, muuta = 0;
   unsigned int mx;
   int  okk;
   kilptietue vkilp,kilp;
   char ch, st[80], prs[100];
   char *ip;
   char ppnimi[] = "Odjj`OhqhkÉ";
   INT32 bdg;
   int ok = 1;

//	nrec=datf2.numrec;
   if (filelen(&datf2)+200 > (UINT32)maxrec)
		maxrec=filelen(&datf2) + 200;
#ifdef DEMO
   maxrec=20;                                       /*  DEMO  */
#endif
	if ((kilparr = (char *) calloc(maxrec+1, kilpparam.kilprecsize)) == NULL)
		er = 1;
   kaynnistysvaihe = 1;
   if (maxnvaika > maxrec)
	  maxnvaika = maxrec;
   nameindex.keyl = NIMIINDL;
   nameindex.recl = nameindex.keyl + sizeof(int);
   if (!er && (nameindex.ix = (char *) calloc(maxrec+1,nameindex.recl)) == NULL)
	  er = 1;
	initindex();
   mx = maxkilpno - minkilpno + 1;
   if (!er && (kilpnoix = (int *) calloc(mx,sizeof(int))) == NULL)
	  er = 1;
   if (er) {
	  writeerror("Muistissa ei tilaa kilpailijatiedoille",0);
	  return(1);
	  }
#if defined(EMIT) && (defined(MAALI) || defined(AUTOVA))
//   if (emitfl || sisaanluenta || port_autova) {
   if (emitfl || sisaanluenta || vaajat) {
	  bdg_kno[0] = (bdg_kno_tp *) calloc(maxrec*kilpparam.n_os_akt, sizeof(bdg_kno_tp));
	  if (kilpparam.kaksibadge)
		  bdg_kno[1] = (bdg_kno_tp *) calloc(maxrec*kilpparam.n_os_akt, sizeof(bdg_kno_tp));
	  if (sisaanluenta < 2 && bdg_kno[0]) {
		  for (os = 0; os < kilpparam.n_os_akt; os++) {
			emitjarr[os] = (int *) calloc(maxrec+1, sizeof(int));
			for (int i = 0; i <= maxrec; i++)
				emitjarr[os][i] = -1;
			}
		 }
	  if (sisaanluenta < 2 && !emitjarr[kilpparam.n_os_akt-1]) {
		 if (bdg_kno[0]) {
			free(bdg_kno[0]);
			bdg_kno[0] = NULL;
			}
		 writeerror("Muisti ei riit‰ leimantarkastukselle", 0);
		 emitfl = 0;
		 }
	  }
#endif
   nilmt = 0;
   nr = 0;
#ifdef _CONSOLE
   vidspmsg(ySize-7,10,7,0,"kilpailijan tiedot luettu");
#endif
   d = -1;
   ld = filelen(&datf2)-1;
   ip = nameindex.ix;
   do {
#ifdef _CONSOLE
	  vidint(ySize-7,4,5, nr);
#endif
	  okk = FALSE;
	  while (!okk && d < ld) {
		 getrec_i(&datf2,++d,&kilp);
		 PUTREC(&kilp, d);
		 okk = (kilp.kilpstatus == 0);
		 }
	  if( okk ) {
		 nr ++;
		 ch = 'J';
		 clrln(19);
		 clrln(20);
		 clrln(21);
		 clrln(ySize-3);
		 if (ToimintaTila == 2) {
			 if (kilp.kilpno < minkilpno || kilp.kilpno >= maxkilpno) {
				ok = FALSE;
				sprintf(prs,"Joukkueella %.24s %d on virheellinen numero %d",
				   kilp.Seura(st), kilp.joukkue, kilp.kilpno);
				}
			 else if(getpos(kilp.kilpno) > 0) {
				char st2[60];
				getrec(&datf2, getpos(kilp.kilpno),&vkilp);
				ok = FALSE;
				sprintf(prs,"Joukkueilla %.20s-%d ja %.20s-%d on sama no %d  ",
				   kilp.Seura(st),kilp.joukkue, vkilp.Seura(st2), vkilp.joukkue, kilp.kilpno);
				}
			}
		 if (kilp.sarja >= sarjaluku) {
			ok = FALSE;
			sprintf(prs,"Joukkueella %.24s %d on virheellinen sarjanumero"
			   " : %d", kilp.Seura(st), kilp.joukkue, (int) kilp.sarja + 1);
			}
		 if (!ok) {
			erbeep();
			kirj_err_file(prs, 1);
			ch = ' ';
			selectopt("J)atka,  P)oistu ohjelmasta","JP",&ch);
			if( ch == 'P' )
				return(1);
			else ok = TRUE;
			}
		 nilm[kilp.sarja]++;
		 if (ToimintaTila == 2)
			setpos(kilp.kilpno, d);
		 memmove((char *)(ip + nameindex.keyl), (char *) &d, 2);
		 memmove((char *)ip, (char *) keyname(&kilp),
			nameindex.keyl);
		 ip += nameindex.recl;
#if defined(EMIT) && (defined(MAALI) || defined(AUTOVA))
//		 if (sisaanluenta || emitfl || port_autova) {
		 if (sisaanluenta || emitfl || vaajat) {
			for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			   if (kilp.ostiet[os].badge[0]) {
					 {
					 for (i = 0; i < os; i++)
						if (kilp.ostiet[i].badge[0] == kilp.ostiet[os].badge[0])
						   break;
					 if (i == os) {
						bdg_kno[0][nbadge[0]].badge = kilp.ostiet[os].badge[0];
						bdg_kno[0][nbadge[0]].kilpno = kilp.kilpno;
						nbadge[0]++;
						}
					 }
				  if (!emittoisto) {
					 for (i = 0; i < os; i++) {
						if (kilp.ostiet[i].badge[0] == kilp.ostiet[os].badge[0]) {
						   sprintf(prs, "Joukkueella %d sama Emit-koodi %s"
							  "osuuksilla %d ja %d", kilp.kilpno, kilp.ostiet[os].badge[0], i+1, os+1);
						   kirj_err_file(prs, 1);
						   }
						}
					 }
				  }
			   if (kilpparam.kaksibadge && kilp.ostiet[os].badge[1]) {
					 {
					 for (i = 0; i < os; i++)
						if (kilp.ostiet[i].badge[1] == kilp.ostiet[os].badge[1])
						   break;
					 if (i == os) {
						bdg_kno[1][nbadge[1]].badge = kilp.ostiet[os].badge[1];
						bdg_kno[1][nbadge[1]].kilpno = kilp.kilpno;
						nbadge[1]++;
						}
					 }
				  if (!emittoisto) {
					 for (i = 0; i < os; i++) {
						if (kilp.ostiet[i].badge[1] == kilp.ostiet[os].badge[1]) {
						   sprintf(prs, "Kilpailijalla %d sama tunnistinkoodi2 %d"
							  "osuuksilla %d ja %d", kilp.kilpno, kilp.ostiet[os].badge[1], i+1, os+1);
						   kirj_err_file(prs, 1);
						   }
						}
					 }
				  }
			   }
			}
#endif
		 }
	  } while (okk);

		if (AllocJarjSeur(nsarjayhd, maxrec)) {
		 writeerror("Muistissa ei tilaa indekseille", 0);
		 return(1);
		 }
	  for (int i = 0 ; i < sarjaluku+nsarjayhd ; i++) {
		for (int os = 0 ; os < kilpparam.n_os_akt; os++) {
			for (int lj = 0 ; lj < VAIKALUKU+6 ; lj++)
				jalku[i][os][lj] = -1;
			josalku[i][os] = -kilpparam.maxnosuus;
			}
		 }
	memset(nilm, 0, sizeof(nilm));
	d = 0;
#ifdef _CONSOLE
	vidspmsg(ySize-6,10,7,0,"kilpailijan tiedot indeksoitu");
#endif
	for (d = 1; d <= ld; d++) {
		GETREC(&kilp, d);
#ifdef _CONSOLE
		vidint(ySize-6,4,5, d);
#else
		if (d % 10 == 0) {
			FormMain->EdtTietueita->Text = d;
			FormMain->EdtTietueita->Refresh();
			}
#endif
		if (kilp.kilpstatus)
			continue;
	  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		 if (!kilp.ostiet[os].keskhyl ||
			stschind(kilp.ostiet[os].keskhyl, "-KHEPIT") < 0) {
			kilp.ostiet[os].keskhyl = '-';
			muuta = 1;
			}
		 if (kilp.ostiet[os].lahtolaji > 2) {
			kilp.ostiet[os].lahtolaji = 0;
			muuta = 1;
			}
		 if (kilpparam.partio && emitfl && !kilp.ostiet[os].hajonta[0]) {
			strncpy(kilp.ostiet[os].hajonta, (char *) Sarjat[kilp.sarja].sarjanimi, HAJONTA);
			muuta = 1;
			}
		for (i = 0; i < (int)(sizeof(kilp.ostiet[0].ampsakot)/sizeof(kilp.ostiet[0].ampsakot[0]))-1; i++) {
			if (i < Sarjat[kilp.sarja].paikat[os]) {
				if (kilp.ostiet[os].ampsakot[i] < '0' || kilp.ostiet[os].ampsakot[i] > '9') {
					kilp.ostiet[os].ampsakot[i] = '-';
					muuta = 1;
					}
				}
			else if (kilp.ostiet[os].ampsakot[i] != 0) {
				kilp.ostiet[os].ampsakot[i] = 0;
				muuta = 1;
				}
			}
		 if (muuta) {
			UINT32 kirjheti0 = kirjheti;
			kirjheti = 0;
			PUTREC(&kilp, d);
			putrec_i(&datf2, d, &kilp);
			kirjheti = kirjheti0;
            }
	     }
	  paivJoukkLkm(kilp.sarja, +1);
	  for (os = 0; os < Sarjat[kilp.sarja].ntosuus; os++) {
		 for (i = 0; i <= kilpparam.maxvaluku; i++) {
			addjarjarr(&kilp, d, os, i, 0);
			}
		 }
      }
   clrln(18);
   keylen = NIMIINDL;
   nameindex.nrec=nr;
   vidspmsg(20,5,7,0,"Laaditaan nimi-indeksi");
   quicksort((char *) nameindex.ix, nr, nameindex.keyl, nameindex.recl);
#if defined(EMIT) && (defined(MAALI) || defined(AUTOVA))

//   if ((emitfl || sisaanluenta || port_autova) && nbadge[0]) {
	if ((emitfl || sisaanluenta || vaajat) && nbadge[0]) {
      vidspmsg(20,5,7,0,"Laaditaan Emit-koodi-indeksi");
	  qsortint(bdg_kno[0], nbadge[0], sizeof(bdg_kno_tp), 0, 1);
      nr = nbadge[0];
	  bdg = bdg_kno[0][0].badge;
      vidspmsg(20,5,7,0,"Tarkastetaan Emit-koodien toistuminen");
      for (i = 1; i < nr; i++) {
         if (bdg_kno[0][i].badge == bdg) {
            bdg_kno[0][i].badge = 9999999L;
			sprintf(prs, "Kilpailijoilla %d ja %d sama Emit-koodi %d",
               bdg_kno[0][i-1].kilpno, bdg_kno[0][i].kilpno, bdg);
			kirj_err_file(prs, 1);
            nbadge[0]--;
			}
         else
            bdg = bdg_kno[0][i].badge;
         }
      if (nbadge[0] < nr) {
         vidspmsg(20,5,7,0,"Indeksoidaan Emit-koodit uudelleen    ");
         qsortint(bdg_kno[0], nr, sizeof(bdg_kno_tp), 0, 1);
         for (i = nbadge[0]; i < nr; i++) {
			bdg_kno[0][i].badge = 0;
            bdg_kno[0][i].kilpno = 0;
            }
         }
	  if (kilpparam.kaksibadge) {
		  qsortint(bdg_kno[1], nbadge[1], sizeof(bdg_kno_tp), 0, 1);
		  nr = nbadge[1];
		  bdg = bdg_kno[1][0].badge;
		  vidspmsg(20,5,7,0,"Tarkastetaan tunnistinkoodien toistuminen");
		  for (i = 1; i < nr; i++) {
			 if (bdg_kno[1][i].badge == bdg) {
				bdg_kno[1][i].badge = 9999999L;
				sprintf(prs, "Kilpailijoilla %d ja %d sama tunnistinkoodi",
				   bdg_kno[1][i-1].kilpno, bdg_kno[1][i].kilpno);
				kirj_err_file(prs, 1);
				nbadge[1]--;
				}
			 else
				bdg = bdg_kno[1][i].badge;
			 }
		  if (nbadge[1] < nr) {
			 vidspmsg(20,5,7,0,"Indeksoidaan Emit-koodit uudelleen    ");
			 qsortint(bdg_kno[1], nr, sizeof(bdg_kno_tp), 0, 1);
			 for (i = nbadge[1]; i < nr; i++) {
				bdg_kno[1][i].badge = 0;
				bdg_kno[1][i].kilpno = 0;
				}
			 }
		}
	  }
#endif
   clrln(20);
//   ok = TRUE;
   if (ToimintaTila == 2) {
	   hae_enslahto();
	   for (i = 0; i < sarjaluku; i++) {
		  if (nilm[i]) {
			  merk_yht_lahdot(i, false);
			  for (os=0; os<Sarjat[i].ntosuus; os++) {
				  for (d = 0; d <= kilpparam.maxvaluku; d++) {
					   merk_sijat(i, os, d, 1);
					}
				 merk_sijat(i, os, kilpparam.valuku+1, 1);
				 }
			  }
		  }
	   }
   lataus = 0;
   return(0);
   }

