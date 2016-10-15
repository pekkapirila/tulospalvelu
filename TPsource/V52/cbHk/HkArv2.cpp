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
#include "mtrand.h"
#include "HkDeclare.h"

#include <vcl.h>
#include "UnitArvonta.h"
extern TFormArvonta *FormArvonta;

//#define DBGARVONTA
#define SNMLEN  30
#define RNDDIVIDOR 131072L
#define SEKOITA

// Taulukoiden sisällöt tyypillisessä käyttötilanteessa
// Taulukoiden tiedot koskevat aina yhtä sarjaa kerrallaan
//    jsat    lähtöjärjestyksen ilmaisevat numerot indeksin antamassa
//            järjestyksessä, ennen rutiinia jarj arvotut luvut
//    jd      tietueiden sijainnit tietokannassa lähtöjärjestyksessä
//    jd[1]     tilapäisversio jd1:stä uutta muodostettaessa
//    jsa     seurat lähtöjärjestyksessä

#ifdef DBGARVONTA
FILE *dbgarvfile;
#endif
static INT o_ens = 1, o_viim;

static int *jsat[N_PV];
static int *ji[N_PV], *jd[N_PV];

static INT poistaranki = 1, naalto = 1;
static INT listavaat;
typedef wchar_t smnsarja[SNMLEN+1];

//  Huom: Taulukolle js1[] varataan kaksi erikoispaikkaa alkuun
//  Ensimmäinen seuranimi tallennetaan paikkaan js1[2]
static smnsarja  *jsa[N_PV];
static kilpindex  *aktindex;

//  Aliohjelma jarj määrää taulukon ia alkioiden ia[a-1] ... ia[y-1]
//  suuruusjärjestyksen. Suuruusjärjestystä kuvaava järjestysnumero
//  1, 2, ..., y-a+1 tallennetaan ao. luvun paikalle.

static UINT32 *lsat;

static void ljarj(UINT32 *ia, INT a, INT y)
   {
   INT i,n,j;
   INT *ja;

   n = y - a + 1;
   if ((ja = (int *) malloc((n+1)*sizeof(INT))) == NULL) {
	  writeerror_w(L"Ei tilaa arvonnalle", 0);
	  return;
	  }
   ja[0] = a;
   ja[1] = a;
   for (i = 1; i < n; i++) {
	  j = i ;
	  while (j > 0 && ia[i+a-1] < ia[ja[j]-1]) {
		 ja[j+1] = ja[j];
		 j--;
		 }
	  ja[j+1] = a + i;
	  }
   for (i = 1; i <= n; i++) ia[ja[i]-1] = i;
   free(ja);
   }

void jarj(INT *ia, INT a, INT y)
   {
   INT   i,n,j;
   INT  *ja;

   n = y - a + 1;
   if ((ja = (int *) malloc((n+1)*sizeof(INT))) == NULL) {
	  writeerror_w(L"Ei tilaa arvonnalle", 0);
	  return;
	  }
   ja[0] = a;
   ja[1] = a;
   for ( i = 1; i < n; i++)
   {
	  j = i ;
	  while (j > 0 && ia[i+a-1] < ia[ja[j]-1]) {
		 ja[j+1] = ja[j];
		 j--;
		 }
	  ja[j+1] = a + i;
	  }
   for ( i = 1; i <= n; i++) ia[ja[i]-1] = i;
   free(ja);
   }

static void arvoseura(INT l, INT n, int nilm)
   {
   INT   i,is,ix,nn;
   MTRand_int32 irand;

   for ( i = 1; i <= l; i++)
	  jsat[0][n-i] = irand() / RNDDIVIDOR;
   is = 32767 / l;
   nn = n - l + 1;
   jarj(jsat[0],nn,n);
   ix = irand()/RNDDIVIDOR / l;
   if (l > 1 && 2*l > nilm) {
	   is = 32767 / (l-1);
	   ix = 0;
	   }
   if (l > i && 2*l == nilm) {
	   is = (99*32767) / 100 / (l-1);
	   ix = ix / 100;
	   }
   else if (l > 1 && 5 * l > 2 * nilm) {
	   is = (9*32767) / 10 / (l-1);
	   ix = ix / 10;
	   }
   for ( i = 1; i <= l; i++)
	  jsat[0][n-i] = ix + is * (jsat[0][n-i] - 1);
   }

static void satunn_sarja(int n)
   {
   INT   i;
   MTRand_int32 irand;

   for ( i = 0; i < n; i++) jsat[0][i] = irand()/RNDDIVIDOR;
   }

// change vaihtaa j1:ntenä lähtevälle kilpailijalle j2:nnen
// lähtöajan ja tekee vastaavat vaihdot taulukoihin jd, ja js.
// lisäksi näytetään seura uudella paikalla, jos nayta on tosi.

static void change(INT *jd, wchar_t (*js)[SNMLEN+1],
   INT j1, INT j2, INT a, INT nayta)
   {
   int l;
   kilptietue kilp1, kilp2;
   INT32 tl;
   wchar_t   seurn[SNMLEN+1];

   kilp1.GETREC(jd[j1-1]);
   kilp2.GETREC(jd[j2-1]);
   tl = kilp1.pv[arv_pv].tlahto;
   kilp1.pv[arv_pv].tlahto = kilp2.pv[arv_pv].tlahto;
   kilp2.pv[arv_pv].tlahto = tl;
   kilp1.pv[arv_pv].va[0].vatulos = kilp1.pv[arv_pv].tlahto;
   kilp2.pv[arv_pv].va[0].vatulos = kilp2.pv[arv_pv].tlahto;
   if (ToimintaTila == 1) {
	   kilp1.PUTREC(jd[j1-1],1);
	   kilp2.PUTREC(jd[j2-1],1);
	   }
   else {
	   kilp1.tallenna(jd[j1-1],0,0,0,0);
	   kilp2.tallenna(jd[j2-1],0,0,0,0);
	   }
   l = jd[j1-1];
   jd[j1-1] = jd[j2-1];
   jd[j2-1] = l;
   wmemcpy(seurn, js[j1+1], SNMLEN);
   wmemcpy(js[j1+1], js[j2+1], SNMLEN);
   wmemcpy(js[j2+1], seurn, SNMLEN);
   }

static void sekoita(INT *jd, wchar_t (*js)[SNMLEN+1],
   INT n, INT teho)
   {
   INT   i,j,k,i1,i2,kerroin;
   static INT vuoro;
   MTRand_int32 irand;

   if (n < 3) return;

   //  Alkuun tehdään kaksi erilaista 'seuraa'. Kilpailijoiden
   //  seurat ovat alueella js[2] ... js[n+1]
   //  Vaihdettavaa paria ilmaisevat muuttujat i1 ja i2 ovat välillä
   //  1 ... n, jos vaihto tehdään, muuten niiden arvo om 0.

   js[1][0] = L'-';
   js[0][0] = L'+';
   js[1][1] = 0;
   js[0][1] = 0;
   kerroin = teho ? 10 : 3;
   for (k = 0; k < kerroin*n; k++) {

	  // Ensin etsitään purettavaksi saman seuran peräkkäisiä lähtijöitä

	  for ( i = 2; i <= n; i++) {
		 i1 = 0;
		 i2 = 0;
		 if (!wmemcmp(js[i], js[i+1], SNMLEN)) {

			// Vaihtoyritysten järjestystä vuorotellaan, jotta ei syntyisi
			// biasia

			for (j = 0; j < 2; j++) {

			   switch (vuoro) {

			// Tässä yritetään parin alemman jäsenen vaihtoa edellisen kanssa
			// Edellytyksenä on, että vaihdettavat eroavat ja, että ei
			// synny uutta paria yhtä alemman kanssa

				  case 0 :
					 if (i > 2 && wmemcmp(js[i], js[i-1], SNMLEN) &&
						wmemcmp(js[i], js[i-2], SNMLEN)) {
						i1 = i-1;
						i2 = i-2;
						}
					 break;

			// Tässä yritetään vaihtaa ylempi jäsen seuraavan kanssa
			// Edellytyksenä on, että vaihdettavat eroavat ja, että ei
			// synny uutta paria yhtä ylemmän kanssa

				  case 1:
					 if (i < n && wmemcmp(js[i+1], js[i+2], SNMLEN) &&
						(i == n-1 || wmemcmp(js[i+1], js[i+3], SNMLEN))) {
						i1 = i;
						i2 = i+1;
						}
					 break;
				  }
			   vuoro = vuoro ? 0 : 1;
			   if (i1) break;
			   }
		   if (i1) break;
			}
		 }

	  // Jos edellä ei löytynyt purettavaa valitaan satunnainen kohta
	  // vaihdolle ja tarkastetaan, onko kohta sopiva
	  // i voi saada arvon väliltä 2 ... n ja osoittaa vaihtoehdokkaan
	  // ensimmäiseen jäseneen.
	  // Tämä vaihe ohitetaan monipäiväisen arvonnassa, jossa ylimääräinen
	  // sekoittaminen on turhaa.

	  if (teho >= 0 && !i1) {
		 i = (INT) ((irand()/RNDDIVIDOR * (n-1)) / 32768L + 2);
		 if (wmemcmp(js[i], js[i+1], SNMLEN) &&
			wmemcmp(js[i-1], js[i+1], SNMLEN) &&
			(i == n || wmemcmp(js[i], js[i+2], SNMLEN)) &&
			   (
			   wmemcmp(js[i+1], js[i-2], SNMLEN) ||
			   (i < n-1 && !wmemcmp(js[i+1],js[i+3], SNMLEN))
			   )
			) {
			i1 = i-1;
			i2 = i;
			}
		 }

	  if (i1 != i2) {
		 change(jd,js,i1,i2,1,0);
		 }
	  }
   }

static int arvioitulos(wchar_t (*js)[SNMLEN+1], INT n, int maxosuus)
   {
   INT   i,a,b,x,y,i1,i2;
   char   ch,ci;

   if (maxosuus >= 60 || n < 3)
		return(0);

   for (i = 1; i < n; i++)
	   if (!wmemcmp(js[i+1], js[i+2], SNMLEN))
			return(1);
   return(0);
   }

/*
static void tarkarv(INT *jd, wchar_t (*js)[SNMLEN+1], INT n)
   {
   INT   i,a,b,x,y,i1,i2;
   char   ch,ci;
	wchar_t prs[200];

   wcscpy(prs, L" . PÄIVÄ");
   prs[0] = L'1' + arv_pv;
   js[1][0] = L'-';
   js[0][0] = L'+';
   js[1][1] = 0;
   js[0][1] = 0;
   a = 1;
   b = min(n,6*(ySize-7));
   do {
	  do {
		 ch = L'H';
		 for (i = a; i <= b; i++) {
			x = (i - a) / (ySize-7);
			y = i - a - (ySize-7) * x + 3;
			x = 12 * x;
			vidint(y,x,4,i);
			vidspmsg(y,x+5,7,0,js[i+1]);
			if (!memcmp(js[i-1], js[i+1], SNMLEN)) vidspmsg(y,x,7,0,"-");
			if (!memcmp(js[i], js[i+1], SNMLEN)) {
			   vidspmsg(y,x,7,0,"*");
			   ch = 'T';
			   }
			}
		 selectopt("H)yväksy, T)ee vaihtoja      <Esc> : keskeytä arvonta",
			"HT\x1B",&ch);
		 if (ch == 27) exfl = TRUE;
		 if (ch == 'T') {
			clrln(ySize-3);
			vidspmsg(ySize-3,0,7,0,"Vaihda sijat :      ja :");
			i1 = 0;
			i2 = 0;
			INPUTINT(&i1,3,15,ySize-3,"+\xD\x1B",&ci);
			if (ci != ESC)
			   INPUTINT(&i2,3,25,ySize-3,"+\xD\x1B",&ci);
			if ((ci != ESC) && (i1 != i2) && (i1 >= a) &&
			   (i2 >= a) && (i1 <= b) && (i2 <= b))
			   {
			   change(jd,js,i1,i2,a,1);
			   }
			else attnbeep();
			}
		 } while ( ch == 'T');
	  a = a + 6*(ySize-8);
	  b = min((b+6*(ySize-8)),n);
	  } while ((a+6) <= b);
   }
*/

static void arvosarja(INT srj, INT n, INT pv, INT sailpo, INT *nd, kilpindex *aktindex)
   {
   INT i, d;
   char keys[MAXINDL];
   kilptietue kilp;

   jarj(jsat[0],1,n);
   memset(keys, 0, sizeof(keys));
   _swab((char *)&srj, keys, 2);
   searchix(aktindex, keys, &d);
   for (i = 0; i < o_ens - 1; i++) {
	  nextix(aktindex, keys, &d);
	  }
   for (i = 1; i <= n; i++) {
	  jd[1][jsat[0][i-1]-1] = d;
	  nextix(aktindex,keys,&d);
	  }
   *nd = 0;
   for (i = 0; i < n; i++) {
	  d = jd[1][i];
	  if (d < 1)
		  break;
	  kilp.GETREC(d);
	  if (sailpo || kilp.tark(pv) != L'P') {
		 (*nd)++;
		 wmemset(jsa[0][*nd+1], 0, SNMLEN+1);
		 wcsncpy(jsa[0][*nd+1], kilp.seuratieto(NULL), SNMLEN);
		 jd[0][*nd-1] = d;
		 kilp.pv[pv].tlahto = Sarjat[kilp.Sarja(pv)].enslahto[pv] +
			SEK*Sarjat[kilp.Sarja(pv)].lahtovali[pv] * ((*nd+o_ens-2)/naalto);
		 }
	  else
		 kilp.pv[pv].tlahto = TMAALI0;
	  kilp.pv[pv].va[0].vatulos = kilp.pv[pv].tlahto;
	  if (ToimintaTila == 2)
		  kilp.tallenna(d,0,0,0,0);
	  else
		  kilp.PUTREC(d, 1);
	  }
   }


static void arvosarja_1(INT srj, INT n)
   {
   INT i, d;
   char keys[MAXINDL];

   jarj(jsat[0],1,n);
#ifdef EI_OLE
   for ( i = 1; i <= n; i++) previx(sarjaindex,keys,&d);
#endif
   memset(keys, 0, sizeof(keys));
   _swab((char *)&srj, keys, 2);
   searchix(sarjaindex, keys, &d);
   for ( i = 1; i <= n; i++) {
	  jd[0][jsat[0][i-1]-1] = d;
	  if (nextix(sarjaindex,keys,&d))
		   break;
	  }
   }

static void ranki(INT n)
   {
   INT i, d, jrj = 1;
   char keys[MAXINDL];
   kilptietue kilp;

   jarj(jsat[0],1,n);
   for ( i = 1; i <= n; i++)
	   previx(sarjaindex,keys,&d);
   for ( i = 1; i <= n; i++) {
		kilp.GETREC(d);
		if (kilp.tulos_pv(0))
			jsat[0][i-1] = - jrj * kilp.tulos_pv(0);
	  nextix(sarjaindex,keys,&d);
	  }
   }

static void jaatasan(INT nseura, INT nsrj, INT njako, INT *rs, INT *nilmrs, char *vs)
   {
//	nseura:	seurasta mukaan tulevien lukumäärä
//	nsrj:	sarjasta poimittujen määrä mukaanlukien aiemmin käsitellyt seura ja tämä seura
//	njako:	kohdesarjojen lukumäärä
//	rs[i], 0 <= i < njako:	kohdesarjat, joihin jaetaan
//	nilmrs[i], 0 <= i < njako:	tähänastinen lukumäärä kohdesarjassa i ml. kaikki seurat
//	vs		avain hakuun indeksistä sarjaindex, sisältää seuralyhenteen, vs[0] ilmaisee sarjan

   INT   d, i,is,js,ix,nn,nsra,x[10],rsjako[10], nsra0 = 0, smin, smax, jmin, jmax;
   char keys[MAXINDL];
   MTRand_int32 irand;

   memset(rsjako, 0, sizeof(rsjako));
   for (i = 0; i < njako; i++) {
	  vs[0] = rs[i];
	  memset(keys, 0, sizeof(keys));
	  memcpy(keys, vs, 22);
	  searchix(sarjaindex,keys,&d);
	  while (!memcmp(keys, vs, 22)) {
		 rsjako[i]++;
		 if (nextix(sarjaindex, keys, &d))
			break;
		 }
	  nsra0 += rsjako[i];
	  }

//	Nyt rsjako sisältää kussakin muodostettavassa sarjassa tarkasteltavasta seurasta jo olevien suunnistajien lukumäärän

   memset(x, 0, sizeof(x));
   nsra = nseura;
   for ( i = 1; i <= nseura; i++)
	  jsat[0][nsrj-i] = irand()/RNDDIVIDOR;
   nn = nsrj - nseura + 1;
   jarj(jsat[0],nn,nsrj);

//	Nyt jsat[0][i], i = nn, .., nsrj-1  sisältää nyt satunnaiset järjestysluvut jaettaville kilpailijoille

//	Ensin jaetaan kilpailijat, jotka menevät tasan kaikkiin kohdesarjoihin

   while (nseura >= njako) {
	  nn = 0;
	  for ( i = 0; i < nseura; i++) {
		 if ((js = jsat[0][nsrj-nseura+i]) <= njako) {
			jsat[0][nsrj-nseura+i] = js*1000 + nilmrs[js-1] + 1;
			nilmrs[js-1]++;
			nn++;
			}
		 else
			jsat[0][nsrj-nseura+i] -= njako;
		 }
	  nseura -= nn;
	  }

//	Sitten sijoitetaan kilpailijoita molempien jakotaulukoiden kannalta parhaisiin paikkoihin

   while (nseura) {

//	etsi suurin ja pienin rsjako

	  smax = 0;
	  smin = 32767;
	  for (i = 0; i < njako; i++) {
		 if (rsjako[i] > smax)
			smax = rsjako[i];
		 if (rsjako[i] < smin)
			smin = rsjako[i];
		 }

//	etsi suurin ja pienin nilmrs

	  jmax = 0;
	  jmin = 32767;
	  for (i = 0; i < njako; i++) {
		 if (nilmrs[i] > jmax)
			jmax = nilmrs[i];
		 if (nilmrs[i] < jmin)
			jmin = nilmrs[i];
		 }

//	jos pienin rsjako ja pienin nilmrs osuvat samaan paikkaan, sijoita kilpailija siihen

	  for (i = 0; i < njako; i++) {
		 if (rsjako[i] == smin && nilmrs[i] == jmin) {
			jsat[0][nsrj-nseura] = (i+1)*1000 + nilmrs[i] + 1;
			nilmrs[i]++;
			rsjako[i]++;
			nseura--;
			}
		 }

//	poistu, kun tällaista paikkaa ei enää löydy

	  if (i == njako)
		 break;
	  }

//	Jos kilpailijoita on sijoittamatta valitse muu kelvollinen paikka

   if (nseura) {
	  nn = njako - (nsrj-nseura) % njako;
	  is = (nsrj-nseura) / njako;
	  if (nseura >= nn) {
		 ix = 0;
		 for (js = 1; js <= nn; js++) {
			for ( i = 0; i < nsra; i++) {
			   if (jsat[0][nsrj-nsra+i] != js)
				  continue;
			   while (nilmrs[ix] > is && ix < njako)
				  ix++;
			   if (ix < njako) {
				  jsat[0][nsrj-nsra+i] = (ix+1)*1000 + nilmrs[ix] + 1;
				  nilmrs[ix]++;
				  x[ix] = 1;
				  }
			   else
				  writeerror_w(L"Ongelma 1", 0);
			   }
			}
		 for ( i = 0; i < nsra; i++)
			if (jsat[0][nsrj-nsra+i] < 1000)
			   jsat[0][nsrj-nsra+i] -= nn;
		 nseura -= nn;
		 }
	  else
		 nn = 0;
	  }
   if (nseura) {
	  is = (nsrj-1) / njako;
	  nn = njako * (is+1) - nsrj - nn;
	  ix = 0;
	  for (js = 1; js <= nseura; js++) {
		 for ( i = 0; i < nsra; i++) {
			if (jsat[0][nsrj-nsra+i] != js)
			   continue;
			while ((nilmrs[ix] > is || x[ix]) && ix < njako)
			   ix++;
			while (nn) {
			   if (irand()/RNDDIVIDOR > 32767L * nn / (nn + nseura - js + 1))
				  break;
			   do {
				  ix++;
				  } while ((nilmrs[ix] > is || x[ix]) && ix < njako);
			   nn--;
			   }
			if (ix < njako) {
			   jsat[0][nsrj-nsra+i] = (ix+1)*1000 + nilmrs[ix] + 1;
			   nilmrs[ix]++;
			   ix++;
			   }
			else
			   writeerror_w(L"Ongelma 2", 0);
			}
		 }
	  }
   }

static void as(INT l, INT n)
   {
   INT   ix,i;
   MTRand_int32 irand;

   do {
	  ix = irand()/RNDDIVIDOR;
	  } while (ix >= 32767 - l);
   for ( i = 1; i <= l; i++) jsat[0][n-i] = ix + i;
   }

static INT jarj2pv(INT srj, wchar_t tyyli, INT sailpo)
   {
   char   key[MAXINDL], srjswab[2];
   INT  i, ok, n = 0, nd, d, pv = 1, jsatmin = 32767, jsatmax = -32767;
   kilptietue kilp;
   MTRand_int32 irand;

   arv_pv = 0;
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Indeksointi\n");
	fclose(dbgarvfile);
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
   ok = !teeindeksi(ljarjindex, true);
   memset(key, 0, sizeof(key));
   _swab((char *) &srj, srjswab, 2);
   memcpy(key, srjswab, 2);
   if (ok)
	   ok = !searchix(ljarjindex,key,&d);
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Satunnaisnumeroiden jako\n");
	fclose(dbgarvfile);
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
   while (ok && !memcmp(key, srjswab, 2)) {
	  kilp.GETREC(d);
	  if (kilp.pv[0].tlahto != TMAALI0) {
		 jsat[0][n] = (INT) (kilp.pv[0].tlahto / (32*KSEK));
		 if (jsat[0][n] < jsatmin) jsatmin = jsat[0][n];
		 if (jsat[0][n] > jsatmax) jsatmax = jsat[0][n];
		 }
	  else {
		 jsat[0][n] = (INT) (jsatmin +
			(irand()/RNDDIVIDOR * (jsatmax - jsatmin)) / 32768L);
		 }
	  ok = !nextix(ljarjindex,key,&d);
	  n++;
	  }
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Järjestä jsat[0]\n");
	fclose(dbgarvfile);
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
   jarj(jsat[0],1,n);
   if (tyyli == L'K' || tyyli == L'S') {
	  for (i = 0; i < n; i++)
		 jsat[0][i] = n+1-jsat[0][i];
	  }
   if (tyyli == L'P') {
	  for (i = 0; i < n; i++)
		 jsat[0][i] = (jsat[0][i] + (n-1)/2) % n + 1;
	  }
   memset(key, 0, sizeof(key));
   _swab((char *) &srj, key, 2);
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Tietueindeksit taulukkoon jd[1]\n");
	fclose(dbgarvfile);
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
   searchix(ljarjindex,key,&d);
   for (i = 1; i <= n; i++) {
	  jd[1][jsat[0][i-1]-1] = d;
	  if (nextix(ljarjindex,key,&d))
		  break;
	  }
   nd = 0;
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Lähtöaikojen merkintä\n");
	fclose(dbgarvfile);
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
   for (i = 0; i < n; i++) {
	  d = jd[1][i];
	  kilp.GETREC(d);
	  if (sailpo || kilp.tark(pv) != L'P') {
		 nd++;
		 wmemset(jsa[0][nd+1], 0, SNMLEN+1);
		 wcsncpy(jsa[0][nd+1], kilp.seuratieto(NULL), SNMLEN);
		 jd[0][nd-1] = d;
		 kilp.pv[pv].tlahto = Sarjat[kilp.Sarja(pv)].enslahto[pv] +
			SEK*Sarjat[kilp.Sarja(pv)].lahtovali[pv] * ((nd+o_ens-2)/naalto);
		 }
	  else
		 kilp.pv[pv].tlahto = TMAALI0;
	  kilp.pv[pv].va[0].vatulos = kilp.pv[pv].tlahto;
	  if (ToimintaTila == 2)
		  kilp.tallenna(d,0,0,0,0);
	  else
		  kilp.PUTREC(d, 1);
	  }
   arv_pv = 1;
   return(nd);
   }

void arvonumerot(INT srj, bool yhtlahtoajat, bool kaikkipaivat, kilpindex *aktindex)
   {
   INT i, ok, nn, *jsatn, d;
   kilptietue kilp;
   char  key[MAXINDL], srjswab[2];
   MTRand_int32 irand;

   if (kaikkipaivat)
	   nn = nilm[srj];
   else
	   nn = nilm_pv[arv_pv][srj] - (FormArvonta->CBOhitaVakantit->Checked ? nvak[arv_pv][srj] : 0) - 1;
   if ((jsatn = (int *) malloc(nn*sizeof(INT))) == NULL) {
	  writeerror_w(L"Muisti ei riitä arvonnalle", 0);
	  }
   else {
	  for (i = 0; i < nn; i++) {
		 jsatn[i] = irand()/RNDDIVIDOR;
		 }
	  jarj(jsatn, 1, nn);
	  memset(key, 0, sizeof(key));
	  _swab((char *) &srj, srjswab, 2);
	  memcpy(key, srjswab, 2);
	  ok = !searchix(aktindex,key,&d);
	  i = 0;
	  while (ok && !memcmp(key, srjswab, 2)) {
		kilp.GETREC(d);
		if (yhtlahtoajat) {
			kilp.pv[arv_pv].tlahto = Sarjat[srj].enslahto[arv_pv];
			kilp.pv[arv_pv].va[0].vatulos = kilp.pv[arv_pv].tlahto;
			}
		 if (kaikkipaivat)
			kilp.setIdAll(Sarjat[srj].sarjaalku + jsatn[i++] - 1);
		 else
			kilp.setId(Sarjat[srj].bibalku[arv_pv] + jsatn[i++] - 1, arv_pv);
		 if (ToimintaTila == 2)
			kilp.tallenna(d,0,0,0,0);
		 else
			kilp.PUTREC(d, 1);
		 ok = !nextix(aktindex,key,&d);
		 }
	  }
   if (jsatn) free(jsatn);
   }

int arvovaihe(int srj, int nn, int ipv, int sailpo, int hiihtoarv, bool osasarja, wchar_t tyyli, wchar_t cpv, kilpindex *aktindex)
{
	int uusinta = 0, exfl = 0, eimahdu = 0, maxosuus = 0, d;
	int n, na, l, viim = 0, nd;
	char  keyv[2], srjswab[2];
	char  vs[MAXINDL];
	char  keys[MAXINDL];
	wchar_t msg[200];
	MTRand_int32 irand;

	n = nn;
	_swab((char *)&srj, srjswab, 2);
	for (int toisto = 0; toisto < 1000; toisto++) {
		if (uusinta == -1) {
			break;
			}
		if (ipv == 0 || tyyli == 'A' || cpv == '2')
			{
			naalto = Sarjat[srj].pari[ipv];
			memset(keys, 0, sizeof(keys));
			memcpy(keys, srjswab, 2);
			searchix(aktindex,keys,&d);
			if (memcmp(keys, srjswab, 2) < 0) {
				exfl = 1;
				continue;
				}
			memcpy(srjswab, keys, 2);
			if (!Sarjat[srj].lahtovali[arv_pv]) {
				int retval;

				swprintf(msg, L"Sarjassa %s yhteislähtö. Lähtöajat merkitään. Arvotaanko samalla kaikkien vaiheiden numerot?, Peruuta keskeyttää arvonnan",
					Sarjat[srj].sarjanimi);
				if ((retval = Application->MessageBoxW(msg, L"Yhteislähtösarja", MB_YESNOCANCEL)) == IDYES) {
#ifdef DBGARVONTA
					fwprintf(dbgarvfile, L"Yhteislähdön numeroiden arvonta\n");
					fclose(dbgarvfile);
					dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
					arvonumerot(srj, true, true, aktindex);
					uusinta = -1;
					continue;
					}
				if (retval == IDCANCEL) {
					exfl = 1;
					break;
					}
				}
			n = 0;
			memcpy(keyv, keys, 2);
			if (hiihtoarv) {
				_swab((char *)&srj, srjswab, 2);
				memcpy(keyv, srjswab, 2);
				memset(keys, 0, sizeof(keys));
				memcpy(keys, keyv, 2);
				searchix(aktindex,keys,&d);
				if (jsat[0]) free(jsat[0]);
				if ((jsat[0] = (int *) malloc((nn+10)*sizeof(int))) == NULL) {
					eimahdu = 1;
					break;
					}
				if (osasarja) {
					int o_v = o_viim;
					if (o_v > nn)
						o_v = nn;
					n = o_v - o_ens + 1;
					}
				else
					n = nn;
				satunn_sarja(n);
				}
			else
				{
				memcpy(srjswab, keyv, 2);
				_swab(srjswab, (char *)&srj, 2);
				if (nn) {
					if (jsat[0]) free(jsat[0]);
					if ((jsat[0] = (int *) calloc(nn+10, sizeof(int))) == NULL) {
						eimahdu = 1;
						break;
						}
					}
				if (!osasarja) {
					do {
						memcpy(vs, keys, aktindex->keyl);
						l = 0;
						do {
							l = l + 1;
							n = n + 1;
							viim = nextix(aktindex,keys,&d) || keys[0] >= 64;
							} while ( !memcmp(keys, vs, 22) && !viim);
						if ((100*l)/nn > maxosuus)
							maxosuus = (100*l)/nn;
#ifdef DBGARVONTA
						fwprintf(dbgarvfile, L"Seuran arvonta, %3d/%4d/%4d\n", l,n, nn);
						fclose(dbgarvfile);
						dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
						arvoseura(l,n, nn);
						} while (!memcmp(keys, keyv, 2) && !viim);
					}
				else {
					int o_v = o_viim;
					if (o_v > nn)
						o_v = nn;
					for (l = o_ens - 1; l < o_v; l++) {
						nextix(aktindex, keys, &d);
						jsat[0][l-o_ens+1] = irand()/RNDDIVIDOR;
						}
					n = o_v - o_ens + 1;
					}
				}
			}
		eimahdu = 1;
		na = max(nilm[srj], nn) + 10;
		if (jsa[0]) free(jsa[0]);
		if ((jsa[0] = (smnsarja *) calloc(na, sizeof(smnsarja))) == NULL)
			break;
		if (jd[0]) free(jd[0]);
		if ((jd[0] = (int *) calloc(na, sizeof(int))) == NULL)
			break;
		if (jd[1]) free(jd[1]);
		if ((jd[1] = (int *) calloc(na, sizeof(int))) == NULL)
			break;
		eimahdu = 0;
		if (ipv > 0 && tyyli != L'A') {
			if (jsat[0]) free(jsat[0]);
			if ((jsat[0] = (int *) calloc(na, sizeof(int))) == NULL) {
				eimahdu = 1;
				break;
				}
#ifdef DBGARVONTA
			fwprintf(dbgarvfile, L"Sarja %s toiseen vaiheeseen\n", Sarjat[srj].sarjanimi);
			fclose(dbgarvfile);
			dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
			nd = jarj2pv(srj, tyyli, sailpo);
			}
		else {
#ifdef DBGARVONTA
			fwprintf(dbgarvfile, L"Sarja %s arvontavaiheeseen\n", Sarjat[srj].sarjanimi);
			fclose(dbgarvfile);
			dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
			arvosarja(srj, n, ipv, sailpo, &nd, aktindex);
			}
		exfl = 0;
		if (hiihtoarv) {
		//			h_numerot(srj);
			}
		else
			{
			if (ipv == 0 || tyyli != L'K')
				{
				if (Sarjat[srj].lahtovali[ipv] != 0 && ipv == 0 || nn > 5)
#ifdef DBGARVONTA
				fwprintf(dbgarvfile, L"Sarja %s sekoitukseen\n", Sarjat[srj].sarjanimi);
				fclose(dbgarvfile);
				dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
				sekoita(jd[0],jsa[0],nd,osasarja);
				if (Sarjat[srj].lahtovali[ipv] != 0) {
					if (nd)
						maxosuus = maxosuus * nn / nd;
#ifdef DBGARVONTA
					fwprintf(dbgarvfile, L"Sarja %s arviointiin\n", Sarjat[srj].sarjanimi);
					fclose(dbgarvfile);
					dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
					uusinta = arvioitulos(jsa[0],nd,maxosuus);
#ifdef DBGARVONTA
					fwprintf(dbgarvfile, L"Sarja %s arvioitu\n", Sarjat[srj].sarjanimi);
					fclose(dbgarvfile);
					dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
					}
	//							tarkarv(jd[0],jsa[0],nd);
				}
			}
		if (!uusinta)
			break;
		}  // Tähän päättyy toistosilmukka
	if (eimahdu)
		exfl = -2;
	return(exfl);
}

static void arvoseura_5(int l, int n, int enspv, int npv)
{
   int i,is,nn, dividor;
   int ipv[N_PV],jpv[N_PV-1];
   MTRand_int32 irand;

   dividor = RNDDIVIDOR/256;
   for ( i = 0; i < npv; i++)
	  ipv[i] = irand() / dividor;
   for ( i = 0; i < npv-1; i++)
	  jpv[i] = irand() / dividor;
   jarj(ipv, 1, npv);
   jarj(jpv, 1, npv-1);
   is = (RNDDIVIDOR/dividor)*32767 / npv;
   for ( i = 0; i < npv; i++)
	  ipv[i] = ((ipv[i]-1)*is + irand() / dividor/npv) / l;
   for ( i = 1; i <= l; i++)
	  jsat[enspv][n-i] = irand() / dividor;
   is = (RNDDIVIDOR/dividor)*32767 / l;
   nn = n - l + 1;
   jarj(jsat[enspv],nn,n);
   for ( i = 1; i <= l; i++) {
	  if (npv > 4)
		 jsat[enspv+jpv[3]][n-i] = ipv[4] + is * ((l - jsat[enspv][n-i] +
			(2*(l+1))/(npv-2)) % l);
	  if (npv > 3)
		 jsat[enspv+jpv[2]][n-i] = ipv[3] + is * (l - jsat[enspv][n-i]);
	  if (npv > 2) {
		 jsat[enspv+jpv[1]][n-i] = ipv[2] + is * ((jsat[enspv][n-i] +
			(l-1)/2) % l);
		 jsat[enspv+jpv[0]][n-i] =
			ipv[1] + is * ((l - jsat[enspv][n-i] + (l+npv/4)/(npv-2)) % l);
		 }
	  if (npv == 2)
		 jsat[enspv+1][n-i] = ipv[1] + is * ((jsat[enspv][n-i] +
			(l-1)/2) % l);
	  jsat[enspv][n-i] = ipv[0] + is * (jsat[enspv][n-i] - 1);
	  }
}

void arvo_satnumerot(int n, int enspv, int npv)
{
   unsigned int is, dividor;
   int nn, i;
   unsigned int ipv[N_PV], jpv[N_PV];
   MTRand_int32 irand;

   dividor = RNDDIVIDOR;
   for (nn = 0; nn < n; nn++) {
	   is = 0;
	   for ( i = 0; i < npv; i++) {
		  ipv[i] = irand() / dividor;
		  is += ipv[i];
		  jpv[i] = irand() / dividor;
		  }
	   jarj((int *)jpv, 1, npv);
	   for (i = 0; i < npv; i++)
		  jsat[enspv+i][nn] = (dividor*ipv[i])/is + (jpv[i]-1) * dividor;
	   }
}

static void arvosarja_5(int srj, int nn, int n, int sailpo, int enspv, int npv, int *nd, kilpindex *aktindex)
{
	int uusinta = 0, maxosuus0 = 0, maxosuus = 0;
   int i, d, dd, pos0, viim;
   char keys[MAXINDL];
	char   keyv[2], srjswab[2];
	char   vs[MAXINDL];
   kilptietue kilp;

//   pos0 = aktindex->pos;
//   previx(aktindex,keys,&d);
//   aktindex->pos -= n;

	memset(keys, 0, MAXINDL);
	_swab((char *)&srj, srjswab, 2);
	memcpy(keyv, srjswab, 2);
	memcpy(keys, keyv, 2);
	searchix(aktindex,keys,&d);
	do {
		int l;

		memcpy(vs, keys,22);
		l = 0;
		do {
			l++;
			viim = nextix(aktindex,keys,&d);
			} while (!memcmp(keys, vs, 22) && !viim);
		if ((100*l)/nn > maxosuus0)
			maxosuus0 = (100*l)/nn;
		} while (!memcmp(keys, keyv, 2) && !viim);


	for (int toisto = 0; toisto < 1000; toisto++) {
		if (uusinta == -1) {
			break;
			}
		arvo_satnumerot(n, enspv, npv);

		memset(keys, 0, MAXINDL);
		_swab((char *)&srj, srjswab, 2);
		memcpy(keyv, srjswab, 2);
		memcpy(keys, keyv, 2);
		searchix(aktindex,keys,&d);

		for (dd = enspv; dd < enspv+npv; dd++)
			jarj(jsat[dd],1,n);
		for ( i = 1; i <= n; i++) {
			for (dd = enspv; dd < enspv+npv; dd++) {
				ji[dd][jsat[dd][i-1]-1] = i;
				jd[dd][jsat[dd][i-1]-1] = d;
				}
			nextix(aktindex,keys,&d);
			}
		for (dd = enspv; dd < enspv+npv; dd++) {
			arv_pv = dd;
			nd[dd] = 0;
			for ( i = 0; i < n; i++) {
				d = jd[dd][i];
				kilp.GETREC(d);
				if (sailpo || kilp.tark(dd) != 'P') {
					ji[dd][nd[dd]] = ji[dd][i];
					jd[dd][nd[dd]] = d;
					kilp.pv[dd].tlahto = Sarjat[kilp.sarja].enslahto[dd] +
						SEK * Sarjat[kilp.sarja].lahtovali[dd] * nd[dd];
					nd[dd]++;
					wmemset(jsa[dd][nd[dd]+1], 0, SNMLEN+1);
					wcsncpy(jsa[dd][nd[dd]+1], kilp.seuratieto(NULL), SNMLEN);
					}
				else
					kilp.pv[dd].tlahto = TMAALI0;
				kilp.pv[dd].va[0].vatulos = kilp.pv[dd].tlahto;
				kilp.tallenna(d, 0, 0, 0, 0);
				}
			if (Sarjat[srj].lahtovali[dd] != 0) {
				sekoita(jd[dd],jsa[dd],nd[dd],-1);
				maxosuus = maxosuus0;
				if (nd[dd])
					maxosuus = maxosuus * n / nd[dd];
				uusinta = arvioitulos(jsa[dd],nd[dd],maxosuus);
				if (uusinta)
					break;
				}
			}
		if (!uusinta)
			break;
		}
}

void arvo_5(int srj, int nn, bool sailpo, kilpindex *aktindex)
{
	wchar_t   ca,cp,ct;
	int   d,l,n, nd[N_PV],npv;
	char   keyv[2], srjswab[2];
	char   vs[MAXINDL];
	char   keys[MAXINDL];
	int    viim;

	for (n = 0; n < kilpparam.n_pv_akt; n++) {
		jsat[n] = (int *) calloc(nn+10, sizeof(int));
		jd[n] = (int *) calloc(nn+10, sizeof(int));
		ji[n] = (int *) calloc(nn+10, sizeof(int));
		jsa[n] = (smnsarja *) calloc(nn+11, sizeof(smnsarja));
		}
	memset(keys, 0, MAXINDL);
	_swab((char *)&srj, srjswab, 2);
	memcpy(keyv, srjswab, 2);
	memcpy(keys, keyv, 2);
	searchix(aktindex,keys,&d);
	npv = Sarjat[srj].viimpv - Sarjat[srj].enspv + 1 - (Sarjat[srj].lahtovali[Sarjat[srj].viimpv] == 1);
/*
	n = 0;
	do {
		memcpy(vs, keys,22);
		l = 0;
		do {
			l++;
			n++;
			nextix(aktindex,keys,&d);
			viim = !ok;
			} while (!memcmp(keys, vs, 22) && !viim);
		arvoseura_5(l,n,Sarjat[srj].enspv, npv);
		} while (!memcmp(keys, keyv, 2) && !viim);
*/
	n = nn;

	arvosarja_5(srj, nn, n, sailpo, Sarjat[srj].enspv, npv, nd, aktindex);
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		if (jsa[ipv])
			free(jsa[ipv]);
		if (jd[ipv])
			free(jd[ipv]);
		if (ji[ipv])
			free(ji[ipv]);
		if (jsat[ipv])
			free(jsat[ipv]);
		jsa[ipv] = NULL;
		jd[ipv] = NULL;
		ji[ipv] = NULL;
		jsat[ipv] = NULL;
		}
}

void arvo(int Toiminto)
{
	INT  d, l, n, nn;
	INT  srj, arv_pv0;
	INT hiihtoarv;
	int   ret = 0;
	time_t  tl;
	unsigned long sd;
	bool osasarja = false, ohitavak = false, sailpo = false;
	wchar_t  ch, msg[100];
	wchar_t cp;
	static wchar_t tyyli, cpv;
	wchar_t tyylit[] = L"AAKSP", paivaval[] = L"M12345";
	//   MTRand_int32 irand;

	sd = time(&tl);
	//   srand((UINT) n);
	//	MTRand_int32::MTRand_int32(sd);
	MTRand_int32 irand(sd);

#ifdef DBGARVONTA
	dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
	hiihtoarv = Toiminto == 1;
	ohitavak = FormArvonta->CBOhitaVakantit->Checked;
	tyyli = tyylit[FormArvonta->CB2pv->ItemIndex+1];
	cpv = paivaval[FormArvonta->CBPaiva->ItemIndex];
	sailpo = FormArvonta->RGPoissa->ItemIndex == 0;
	if (cpv != L'2' && cpv != L'M')
		tyyli = L'A';
	arv_pv0 = FormArvonta->CBPaiva->ItemIndex - 1;
	if (arv_pv0 < 0)
		arv_pv0 = 0;
	arv_pv = arv_pv0;
	aktindex = sarjaindex;
	if (Toiminto == 3) {
		osasarja = true;
		aktindex = ljarjindex;
		}
	else
		o_ens = 1;
	if (ohitavak)
		aktindex->flags = 0x4000;
	teeindeksi(aktindex, true);
	aktindex->flags = 0;
	for (srj = 0; srj < sarjaluku; srj++) {
		if (kilpparam.n_pv_akt > 1 && cpv == L'M') {
			nn = nilm[srj];
			}
		else {
			nn = nilm_pv[arv_pv0][srj] - (FormArvonta->CBOhitaVakantit->Checked ? nvak[arv_pv0][srj] : 0);
			}
		if (!FormArvonta->SarjaValittu(srj) || !nn) {
			continue;
			}
		if (Toiminto == 3) {
			switch (FormArvonta->RGRajaustapa->ItemIndex) {
				case 0:
					o_ens = _wtoi(FormArvonta->EditAlku->Text.c_str());
					o_viim = _wtoi(FormArvonta->EditLoppu->Text.c_str());
					if (o_ens < 1)
						o_ens = 1;
					break;
				case 1:
					o_ens = 1;
					o_viim = nn/2;
					break;
				case 2:
					o_ens = nn/2 + 1;
					o_viim = nn;
					break;
				}
			}
		FormArvonta->EdtArvottava->Text = Sarjat[srj].sarjanimi;
#ifdef DBGARVONTA
		fwprintf(dbgarvfile, L"Sarja %s aloitettu\n", Sarjat[srj].sarjanimi);
		fclose(dbgarvfile);
		dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
		FormArvonta->EdtArvottava->Refresh();
		Sleep(100);
		if (!hiihtoarv && kilpparam.n_pv_akt > 2 && cpv == L'M' && Sarjat[srj].viimpv > 1) {
#ifdef DBGARVONTA
			fwprintf(dbgarvfile, L"Monipäiväisen arvonta\n");
			fclose(dbgarvfile);
			dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
			arvo_5(srj, nn, sailpo, aktindex);
			}
		else {
			for (arv_pv = (cpv == L'M') ? 0 : cpv - L'1'; arv_pv <= Sarjat[srj].viimpv; arv_pv++) {
				wchar_t srjtyyli = tyyli;
				if (cpv != L'M' && arv_pv > cpv - L'1') break;
				if (Sarjat[srj].luonne[arv_pv] < 0)
					continue;
				if (Sarjat[srj].enspv > 0 || arv_pv != 1)
					srjtyyli = L'A';
#ifdef DBGARVONTA
				fwprintf(dbgarvfile, L"Vaihe %d alkaa\n", arv_pv+1);
			fclose(dbgarvfile);
			dbgarvfile = _wfopen(L"ArvDebug.txt", L"at");
#endif
				ret = arvovaihe(srj, nn, arv_pv, sailpo, hiihtoarv, osasarja, srjtyyli, cpv, aktindex);
				if (ret == -2)
					break;
				} // Tähän päättyy päivät läpikäyvä for - silmukka
			if (kilpparam.n_pv_akt < 2)
				arv_pv = 0;
			else
				arv_pv = arv_pv0;
			}
		reindex = 1;
		if (ret)
			break;
		}
	escape:
	if (jsa[0])
		free(jsa[0]);
	if (jd[0])
		free(jd[0]);
	if (jd[1])
		free(jd[1]);
	if (ji[0])
		free(ji[0]);
	if (jsat[0])
		free(jsat[0]);
	jsa[0] = NULL;
	jd[0] = NULL;
	jd[1] = NULL;
	ji[0] = NULL;
	jsat[0] = NULL;
	reindex = 1;
	if (ToimintaTila == 2)
		arv_pv = k_pv;
	else
		arv_pv = 0;
	if (ret == -2) {
		writeerror_w(L"Ei tilaa arvonnalle", 0);
	}
#ifdef DBGARVONTA
	fwprintf(dbgarvfile, L"Arvonta loppu\n");
	fclose(dbgarvfile);
#endif
	return;
}

