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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <sys/timeb.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <process.h>
#include <bkeybrd.h>
#include <bstrings.h>
#include <butil.h>
#include <wincom.h>
#pragma hdrstop
#include <sys/stat.h>
#include "HkDeclare.h"
#ifdef _BORLAND_
#include "WinHk.h"
#endif

#define UDPYHTEYS

#ifndef DEBUG1
#define DEBUG1 1
#endif
#ifndef DEBUG2
#define DEBUG2 0
#endif

#define ALKUT 0
#define KILPT 1
#define KILPPVT 2
#define VAIN_TULOST 3
#define AIKAT 4
#define EMITT 6
#define SEURAT 7
#define FILESEND  10
#define EMITVA 13
#define EXTRA 14
#define N_CLASS 15

#define INQ_EMPTY 1

#define LUEBUF 1000
#define LUECLIBUF 100
#define YHTFL 3

int alkut = ALKUT;
HANDLE hComm[MAX_PORTTI];
int ackreq[MAX_LAHPORTTI];              /* != 0 Ilmaisee, että yhteys toimii */
int yhtfl[MAX_LAHPORTTI];              /* != 0 Ilmaisee, että yhteys toimii */
int keyclose[MAX_LAHPORTTI];                   /* Käyttäjä sulkenut portin */
int autoclose;               /* Ohjelma sulkenut vastaanoton */
int  initid[MAX_LAHPORTTI];                    /* inpakid initialisoimatta */
extern int ivtime[];                    /* Seuraavan keskeytyksen hetki */
extern int intv[];                      /* Tiedonsiirron odotusväli     */
//int intv[2];                      /* Tiedonsiirron odotusväli     */
combufrec *inbuf;      /* Saapuvien jono */
combufrec *outbuf[MAX_LAHPORTTI];     /* Lähtevien jono */
int  combufsize;
char *combuf[MAX_LAHPORTTI];          /* IO-portin puskuri */
int  lahfl[MAX_LAHPORTTI];            /* Lähetys käynnissä */
int  vkesken[MAX_LAHPORTTI];              /* Vastaanotto kesken */
int  chcomkesto[MAX_LAHPORTTI];           /* Yhden merkin lähettämiskesko */
int  hyvkesken[MAX_LAHPORTTI];            /* Hyväksyntä kesken */
char outpakid[MAX_LAHPORTTI];    /* seuraava lähetettävä id */
int vastcom0[MAX_LAHPORTTI];
int lahcomserver[MAX_LAHPORTTI];
int class_len[N_CLASS];     // Lähetettävän tiedon pituus mukaanlukien
                                   // checksum ja itse tieto
extern int timerfl, tbase, trate;
extern INT vpiste[];
extern int comtype[MAX_PORTTI];
INT yhteysalku = 0;
int tcpautoalku;
INT ts_close[MAX_LAHPORTTI];
int acn[MAX_PORTTI];

void buflah(int cn, combufrec *obuf);
void kaynnistatesti(int k);

#ifdef PING
HTHREAD hPingThread;
#endif

#define R_BUFLEN_C 900

comparamtp comparam[MAX_PORTTI];

typedef struct {
   wchar_t code;
	USHORT t1;
	} C_ctrltype;

typedef union {
   struct {
      USHORT alku;
      char badge[3];
      char fill1;
      char week;
      char year;
      char fill2;
      char chksum;
	  C_ctrltype c[50];
      char fill3[40];
		char chs;
      char cs[4];
      char chp;
      char cp[4];
      char chl;
      char cl[4];
      char fill4;
      char chksum2;
      } r12;
   char bytes[R_BUFLEN_C+1];
   } C_san_type;

void wrt_wst_broadcast(int len, wchar_t *wline)
{
	char line[1000];

	wcstombs(line, wline, len);
	wrt_st_broadcast(len, line);
}

void broadcasttulos(kilptietue *kilp, int piste)
{
	char line[80];
	char st[14];
	static int msgno;

	aikatostr_s(st, kilp->p_aika(piste), t0);
	st[pvparam[k_pv].laika2] = 0;
	sprintf(line, "%-2.2s:%05.5d:%04.4d:%d:%d:%s\r\n", konetunn, ++msgno, kilp->id(), k_pv+1, piste, st);
	wrt_st_broadcast(strlen(line), line);
}

//  henkilökohtaisen lähetä
//
//  piste = 0  :              Lähetä kaikki perustiedot
//  piste < 0  :              Lähetä kaikki päivän 1-piste tiedot
//							  aktiivinen päivä ilman aikoja, muut aikoineen
//  piste = 1 .. valuku+2     Lähetä pisteen piste-2 aika
//                            (-1: lähtö, 0: maali, >0: väliaika)
//  piste = valuku+3          Lähetä keskhyl
//  piste = valuku+4          Lähetä ammunnan sakkotiedot
//  piste = valuku+5          Lähetä muu sakkotieto
//

void laheta(INT dkilp, INT entno, INT piste, int hyv_muutos, INT comtarfl, INT kielto,
	INT pakota_lah)
	{
	INT  cn, lah_kaikkiajat = 0;
//	int hyv_muutos;
	combufrec obuf;
	kilptietue kilp;

	// hyv_muutos == -1 pyytää lähettämään ajanottotiedot vaiheen tietojen mukana
	if (hyv_muutos == -1) {
		lah_kaikkiajat = 1;
		hyv_muutos = 0;
		}
//   hyv_muutos = piste & 0x0400;
//   piste = piste & 0xfbff;
   if (broadcasts && piste >= 1 && piste <= kilpparam.valuku+2 && kielto == 0) {
		kilp.GETREC(dkilp);
		broadcasttulos(&kilp, piste-2);
		}
   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	   if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || 
		   cn == kielto - 1 || (kielto && estavalitys[cn]) || (kielto < 0 && cn != -kielto - 1)) 
		  continue;
	  if (hyv_muutos && (comtype[cn] == comtpTCPXML || comtype[cn] == comtpTCPSRVXML))
			continue;
	  obuf.d.k.dk = dkilp;
	  obuf.d.k.tarf = comtarfl && tarlahcom[cn];
	  obuf.d.k.entno = entno;
	  obuf.d.k.pakota = (char) pakota_lah;
	  if (!piste) {
		 obuf.pkgclass = KILPT;
		 obuf.len = class_len[KILPT];
		 kilp.GETREC(dkilp);
		 kilp.pack0(obuf.d.k.ckilp);
		 }
	  else if (piste < 0) {
		 obuf.pkgclass = KILPPVT;
		 obuf.len = class_len[KILPPVT];
		 memset(obuf.d.pv.cpv, 0, sizeof(obuf.d.pv.cpv));
		 kilp.GETREC(dkilp);
		 obuf.d.pv.pv = -1 - piste;
		 if (!lah_kaikkiajat && obuf.d.pv.pv == k_pv) {
			 obuf.d.pv.valuku = 0;
			 kilp.pv[k_pv].pack0(obuf.d.pv.cpv);
			 }
		 else {
			 obuf.d.pv.valuku = kilpparam.valuku;
			 kilp.pv[obuf.d.pv.pv].pack(obuf.d.pv.cpv);
			 }
		 }
	  else {
		 kilp.GETREC(dkilp);
		 obuf.pkgclass = VAIN_TULOST;
		 obuf.len = class_len[VAIN_TULOST];
		 obuf.d.v.bib = kilp.id(k_pv);
		 obuf.d.v.k_pv = k_pv;
		 obuf.d.v.vali = piste-2;
		 if (piste <= kilpparam.valuku+2)
			obuf.d.v.aika = kilp.p_aika(piste-2);
		 else if (piste == kilpparam.valuku+3)
			*(wchar_t *)&obuf.d.v.aika = kilp.tark(k_pv);
		 else if (piste == kilpparam.valuku+4)
			memcpy((char *)&obuf.d.v.aika, kilp.pv[k_pv].asakot, sizeof(kilp.pv[k_pv].asakot));
		 else if (piste == kilpparam.valuku+5)
			obuf.d.v.aika = kilp.pv[k_pv].tasoitus;
		 }
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
   //   in_com = FALSE;
	}

//  henkilökohtaisen tark_kilp

static void tark_kilp(INT cn, INT kaikki)
   {
   INT  d, dd;
   INT er,entno = -1;
//   int ekilpno = -1;
	combufrec *ibuf;
	kilptietue tkilp;
	kilptietue kilp;

	tkilp.nollaa();
	ibuf = inbuf + inbens;
	d=ibuf->d.k.dk;
	if (d <= 0) {
		writeerror_w(L"Virheellinen sanoma tiedonsiirrosta", 0);
		return;
		}
	er = FALSE;
	if (d > 0 && d < nrec) {
		kilp.GETREC(d);
//		ekilpno = kilp.kilpno;
		tkilp = kilp;
		}

	if (kaikki == 1) {
		entno=ibuf->d.k.entno;
		tkilp.unpack0(ibuf->d.k.ckilp);
		if (tkilp.sarja < 0)
			tkilp.sarja = sarjaluku-1;
		}
	else if (kaikki == 2) {
		entno=ibuf->d.k.entno;
		if (ibuf->d.pv.valuku == 0) {
			tkilp.pv[ibuf->d.pv.pv].unpack0(ibuf->d.pv.cpv);
			}
		else
			tkilp.pv[ibuf->d.pv.pv].unpack(ibuf->d.pv.cpv);
		 if (tkilp.Sarja(ibuf->d.pv.pv) < 0)
			tkilp.setSarja(sarjaluku-1, ibuf->d.pv.pv);
		 }
	else {
		if (ibuf->d.v.vali < kilpparam.valuku+1)
			tkilp.tall_tulos(ibuf->d.v.vali, ibuf->d.v.aika);
		else if (ibuf->d.v.vali == kilpparam.valuku+1)
			tkilp.set_tark(*(wchar_t *) &ibuf->d.v.aika, k_pv);
		else if (ibuf->d.v.vali == kilpparam.valuku+2)
			wcsncpy(tkilp.pv[k_pv].asakot, (wchar_t *)&ibuf->d.v.aika, sizeof(tkilp.pv[k_pv].asakot)/2);
		else if (ibuf->d.v.vali == kilpparam.valuku+3)
			tkilp.pv[k_pv].tasoitus = ibuf->d.v.aika;
		}
	if (kaikki == 1 && entno == 0) {
		if (d >= nrec || kilp.kilpstatus != 0 ) {
			dd = 0xffffffffU;
//			if (setpos(tkilp.id(), 0))
			if (tkilp.Sarja() < 0)
				tkilp.setSarjaAll(sarjaluku-1);
			EnterCriticalSection(&tall_CriticalSection);
			tkilp.addtall(&dd, cn + 1);
			LeaveCriticalSection(&tall_CriticalSection);
			if( d != dd )
				er = TRUE;
			}
		else {
		 if (tkilp.kilpno != kilp.kilpno)
			er = 2;
		 }
	  }
   else if(tkilp.kilpstatus) {
//	  if (kaikki && d == (UINT) getpos(entno))
	  if (kaikki == 1 && kilp.kilpstatus == 0 && kilp.kilpno == entno) {
		 EnterCriticalSection(&tall_CriticalSection);
		 deltall(d, cn + 1);
		 LeaveCriticalSection(&tall_CriticalSection);
		 }
	  else {
//		 if (getpos(entno) > 0)
		 if (kilp.kilpstatus == 0)
			er = 3;
		 }
	  }
	else {
//	  if ((kaikki != 1 || entno == kilp.kilpno) &&
//		 (getpos(tkilp.id()) == 0 ||
//		 (UINT) getpos(tkilp.id()) == d )) {
	  if ((kaikki != 1 || entno == kilp.kilpno)) {
		 EnterCriticalSection(&tall_CriticalSection);
		 if (tkilp.Sarja() < 0)
			tkilp.setSarja(sarjaluku-1);
		 if (tkilp.sarja < 0)
			tkilp.sarja = sarjaluku-1;
		 tkilp.tallenna(d,ibuf->d.k.tarf, cn + 1, ibuf->d.k.pakota, 0);
		 LeaveCriticalSection(&tall_CriticalSection);
		 }
		else {
//		 if (entno != tkilp.kilpno || (UINT) getpos(tkilp.id()) != d)
//		 if (entno != tkilp.kilpno || ekilpno != entno)
			er = 1;
		 }
	  }
   if (er) {
#ifdef _CONSOLE
		writeerror_w(
		 L"Tiedostot eri koneilla eivät ole yhtäpitäviä",0);
	  writeerror_w(L"Poistu ohjelmasta, kopioi KILP.DAT"
		 L"lähettävältä tälle koneelle",0);
#else
		writeerror_w(L"Tiedostot eri koneilla eivät ole yhtäpitäviä"
			L"Poistu ohjelmasta ja kopioi KILP.DAT lähettävältä tälle koneelle",0);
#endif
	  }
   }

#if defined(MAALI) && !defined(EIMAALI)
// extern INT odota;

void laheta_aika(aikatp *dtm, aikatp *itm, INT kielto,
   INT pakota_lah)
   {
   INT  cn ;
   combufrec obuf;
#ifdef EMIT
   aikatp ntm = {AIKAJAK*TMAALI0, 0 ,0, 0};
#else
   aikatp ntm = {AIKAJAK*TMAALI0, 0 ,0};
#endif

	if (dtm == 0) dtm = &ntm;
   if (itm == 0) itm = &ntm;
   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || !com_ajat[cn] ||
		 cn == kielto - 1 || (kielto && estavalitys[cn])) continue;
      obuf.pkgclass = AIKAT;
      obuf.len = class_len[AIKAT];
	  obuf.d.a.pakota = (char) pakota_lah;
	  memcpy((char *)&(obuf.d.a.daika), (char *)dtm, sizeof(aikatp));
	  memcpy((char *)&(obuf.d.a.iaika), (char *)itm, sizeof(aikatp));
      buflah(cn, &obuf);
      }
   LeaveCriticalSection(&outb_CriticalSection);
}

#if defined(TARK_LEIMAT)
static void talltulos(INT kno, INT32 t, INT cn)
   {
	INT d;
   kilptietue kilp;

   if ((d = getpos(kno)) <= 0)
      return;
   EnterCriticalSection(&tall_CriticalSection);
   kilp.GETREC(d);
	kilp.set_tulos(0,
	  t - kilp.tlahto + 36L*TUNTI) % (24L*TUNTI) - (12L*TUNTI));
	kilp.tallenna(d, 1 , cn + 1, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
   }
#endif

#define MAXINITAJAT 30
static void tark_aika(INT cn)
   {
   aikatp ia, da;
   combufrec *ibuf;
	INT i = 0;
   static INT initca[MAX_LAHPORTTI], initbufptr[MAX_LAHPORTTI];
   static char initbuf[MAX_LAHPORTTI][MAXINITAJAT][7+2*sizeof(aikatp)];
	INT ino;

   if (!com_ajat[cn]) return;
   if (!timerfl) {
      if (initbufptr[cn] < MAXINITAJAT) {
		  memcpy(&((combufrec *)initbuf[cn][initbufptr[cn]++])->so, inbuf + inbens, 7+2*sizeof(aikatp));
         }
      return;
		}
	do {
	  if (!initca[cn]) {
			if (i == initbufptr[cn])
				initca[cn] = 1;
			else
				ibuf = (combufrec *) initbuf[cn][i++];
			}
		if (initca[cn])
			ibuf = inbuf + inbens;
		memcpy(&ia, &ibuf->d.a.iaika, sizeof(aikatp));
		memcpy(&da, &ibuf->d.a.daika, sizeof(aikatp));
		if (maxjono > 0) {
			ino = 0;
			if (ibuf->d.a.iaika.t != TMAALI0*AIKAJAK)
				ino = ibuf->d.a.iaika.jono;
			else
				if (ibuf->d.a.daika.t != TMAALI0*AIKAJAK)
					ino = ibuf->d.a.daika.jono;
//#ifdef MAXOSUUSLUKU
			 if (yhtjonot)
				 ino = yhtjono(cn, ino);
//#endif
			if (ino > maxjono)
				ino = 0;
			}
		else if (lajatfl && (ibuf->d.a.daika.piste == -1 ||
			(ibuf->d.a.daika.t == TMAALI0*AIKAJAK && ibuf->d.a.iaika.piste == -1)))
			ino = 1;
		else ino = 0;
		tall_rivi(ino, &ia, &da, 0, 0, cn+1, ibuf->d.a.pakota, false);
#if defined(TARK_LEIMAT)
		if (leimacomopen[cn]) talltulos(ia.kno,  purajak(ia.t), cn);
#endif

		if (od_fl && od_aika == TMAALI0*AIKAJAK && da.t == TMAALI0*AIKAJAK && aikajono[ino] &&
		 ia.t == aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows-1)->t  && ia.kno == 0) {
		 od_aika = ibuf->d.a.iaika.t;
//         odota = aikajono[ino]->rwtime-1;
		 }
	  } while (!initca[cn]);
   }
#endif

#ifdef SEURAVAL
void lah_seura(wchar_t *snimi, wchar_t *lyhenne, wchar_t *maa, int piiri,
   int kielto)
   {
   INT  cn ;
   combufrec obuf;

   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_ajat[cn] == 2 ||
			cn == kielto - 1 || (kielto && estavalitys[cn])) continue;
	  obuf.pkgclass = SEURAT;
		obuf.len = class_len[SEURAT];
	  memset(&obuf.d.sra, 0, class_len[SEURAT]);
	  if (snimi)
			wmemcpy(obuf.d.sra.snimi, snimi, kilpparam.lseura);
	  if (lyhenne)
			wcsncpy(obuf.d.sra.lyhenne, lyhenne, kilpparam.lseuralyh);
	  if (maa)
			wcsncpy(obuf.d.sra.maa, maa, 3);
	  obuf.d.sra.piiri = piiri;
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
	}

static void tark_seura(INT cn)
	{
	combufrec *ibuf;

	ibuf = inbuf + inbens;
	tall_seura(ibuf->d.sra.snimi, ibuf->d.sra.lyhenne, ibuf->d.sra.maa, ibuf->d.sra.piiri, cn+1);
	}
#endif

void tall_ec(UINT32 badge, INT piste, INT32 t, INT kielto);

#ifdef VALUKIJA
void laheta_ec(UINT32 badge, int piste, INT32 aika, int kielto)
   {
   int  cn ;
   combufrec obuf;

   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || !lahec[cn] ||
		 cn == kielto - 1 || (kielto && estavalitys[cn])) continue;
//		obuf.pkgclass = ECT;
//      obuf.len = class_len[ECT];
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 1;
	  obuf.d.x.d2 = aika;
	  obuf.d.x.d3 = badge;
	  obuf.d.x.d4 = piste;
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
	}

#endif // VALUKIJA

void lahetasulku(char *tn, INT kielto)
	{
	INT  cn ;
	combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
	for (cn = 0; cn < maxyhteys; cn++) {
		if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_ajat[cn] == 2 ||
			cn == kielto - 1 || (kielto && estavalitys[cn]))
			continue;
		obuf.pkgclass = EXTRA;
		obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 9;
		obuf.d.x.d2 = 0;
		memcpy((char *) &obuf.d.x.d2, tn, 2);
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
	}

void lahetakello(int cn, INT kielto)
	{
	combufrec obuf;

	if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || !lahkello[cn] ||
		cn == kielto - 1 || (kielto && estavalitys[cn]))
		return;
	obuf.pkgclass = EXTRA;
	obuf.len = class_len[EXTRA];
	obuf.d.x.d1 = 4;
	obuf.d.x.d2 = (SEK/10)*t_time_l(biostime(0, 0), t0);
	EnterCriticalSection(&outb_CriticalSection);
	buflah(cn, &obuf);
	LeaveCriticalSection(&outb_CriticalSection);
	}

void lahetaeralahto(int era, INT kielto)
	{
	combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
	for (int cn = 0; cn < maxyhteys; cn++) {
		if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 ||
			cn == kielto - 1 || (kielto && estavalitys[cn]))
			continue;
		obuf.pkgclass = EXTRA;
		obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 5;
		obuf.d.x.d2 = era;
		obuf.d.x.d3 = eralahto[era];
		obuf.d.x.d4 = era_akt;
		buflah(cn, &obuf);
		}
	LeaveCriticalSection(&outb_CriticalSection);
	}

void tulkKurvinen(char *sanoma, int kielto);
void lahetaKurvinen(char *sanoma, int kielto)
{
   int  cn ;
   combufrec obuf;
   char buf[20];

   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] ||
		 cn == kielto - 1 || (kielto && estavalitys[cn])) continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
//	  obuf.d.x.d1 = 6;
	  memset(buf, 0, sizeof(buf));
	  buf[0] = 6;
	  strncpy(buf+1, sanoma, 15);
	  memcpy(&obuf.d.x.d1, buf, 16);
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
}

void lahetatestikaynnistys(INT kielto)
	{
   INT  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	   if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] ||
		   cn == kielto - 1 || (kielto && estavalitys[cn])) continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
	  obuf.d.x.d1 = 7;
	  obuf.d.x.d2 = 0;
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
	}

static void tark_extra(int cn)
   {
	combufrec *ibuf;
	char tn[3];

	if (!comopen[cn]) return;
	ibuf = inbuf + inbens;
	switch (ibuf->d.x.d1 & 0x0000000f) {
#ifdef VALUKIJA
		case 1:
			tall_ec(ibuf->d.x.d3, ibuf->d.x.d4, ibuf->d.x.d2, cn+1);
			break;
#endif
#ifdef MAKIx
		case 2:
			aktivointi(ibuf->d.x.d2, ibuf->d.x.d3, cn+1);
			break;
		case 3:
			vaihdakierros(ibuf->d.x.d2, cn+1);
			break;
#endif
		case 5:
			if (ibuf->d.x.d2 == ibuf->d.x.d4)
				era_akt = ibuf->d.x.d2;
			eralahto[ibuf->d.x.d2] = ibuf->d.x.d3;
			lahetaeralahto(ibuf->d.x.d2, cn+1);
			break;
		case 6:
			tulkKurvinen(((char *)&ibuf->d.x.d1)+1, cn+1);
			lahetaKurvinen(((char *)&ibuf->d.x.d1)+1, cn+1);
			break;
		case 7:
			kaynnistatesti(cn+1);
			break;
		case 9:
			memcpy(tn, (char *) &ibuf->d.x.d2, 2);
			tn[2] = 0;
			if (memcmp(tn, konetunn, 2))
				lahetasulku(tn, cn+1);
			if (!tn[0] || !memcmp(tn, konetunn, 2)) {
#ifdef _CONSOLE
				writeerror_w(L"Ohjelma suljetaan 3 s kuluttua verkkokomennon perusteella", 3000);
				suljesuoraan = true;
				lopetus();
#else
				int wparam = 0;
				int lparam = 1;
				SendMessage(FormMain->Handle,WM_MYMSGCLOSEAPPL,wparam,lparam);
#endif
				}
			break;
		}
   }

#ifdef MAKI
void lahetaaktivointi(int kno, int kierros, int kielto)
	{
   INT  cn ;
   combufrec obuf;

   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_ajat[cn] == 2 ||
		 cn == kielto - 1 || (kielto && estavalitys[cn]))
		 continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 2;
		obuf.d.x.d2 = kno;
		obuf.d.x.d3 = kierros;
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
	}

void lahetakierros(int kierros, int kielto)
	{
   INT  cn ;
   combufrec obuf;

   EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_ajat[cn] == 2 ||
		 cn == kielto - 1 || (kielto && estavalitys[cn]))
		 continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 3;
		obuf.d.x.d2 = kierros;
	  buflah(cn, &obuf);
	  }
   LeaveCriticalSection(&outb_CriticalSection);
	}
#endif

#if defined(EMIT) && (defined(MAALI) || defined(TALLEMIT)) && !defined(EILEIMAT)
void laheta_emva_yht(emittp *em, int cn)
	{
   combufrec obuf;

   if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_emitva[cn] <= 0)
	  return;
   obuf.pkgclass = EMITVA;
   obuf.len = class_len[EMITVA];
	if (!tee_emva(&obuf.d.emva, em)) {
		buflah(cn, &obuf);
		}
   }

void laheta_emva_pois_yht(int kno, int cn)
	{
    combufrec obuf;
    kilptietue kilp;

	if (!comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_emitva[cn] <= 0)
		return;
	memset(&obuf, 0, sizeof(obuf));
    obuf.pkgclass = EMITVA;
	obuf.len = class_len[EMITVA];
    kilp.GETREC(getpos(kno));
    obuf.d.emva.kilpno = kno;
    obuf.d.emva.tulos = kilp.tulos_pv(k_pv, true);
    if (!kilp.hyv())
		obuf.d.emva.ok = kilp.tark(k_pv);	
	buflah(cn, &obuf);
	}

void laheta_emva(emittp *em)
	{
	INT  cn ;

	EnterCriticalSection(&outb_CriticalSection);
	for (cn = 0; cn < maxyhteys; cn++)
		laheta_emva_yht(em, cn);
	LeaveCriticalSection(&outb_CriticalSection);
	}

void laheta_emva_pois(int kno)
	{
	INT  cn ;

	EnterCriticalSection(&outb_CriticalSection);
	for (cn = 0; cn < maxyhteys; cn++)
		laheta_emva_pois_yht(kno, cn);
	LeaveCriticalSection(&outb_CriticalSection);
	}

void laheta_em(emittp *em, int rajaus, INT kielto)
	{
	INT  cn ;
	combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
	for (cn = 0; cn < maxyhteys; cn++) {
		if ( !comopen[cn] || !lahcom[cn] || lahec[cn] == 2 || lahkellovain[cn] || com_emit[cn] <= 0 ||
			(rajaus && com_emit[cn] == 3) ||
			cn == kielto - 1 || (kielto && estavalitys[cn]))
			continue;
		obuf.pkgclass = EMITT;
		obuf.len = class_len[EMITT];
		memmove(&obuf.d.em, em, sizeof(emittp));
		buflah(cn, &obuf);
		}

   for (cn = 0; cn < maxyhteys; cn++) {
		if (cn != kielto - 1 && !(kielto && estavalitys[cn]))
			laheta_emva_yht(em, cn);
		}
   LeaveCriticalSection(&outb_CriticalSection);
	}

static void tark_emit(INT cn)
	{
	combufrec *ibuf;

	if (!com_emit[cn] || com_emit[cn] > 1) return;
	ibuf = inbuf + inbens;
	if (addem(&ibuf->d.em, cn+1))
		return;
	}

static void tark_emitva(INT cn)
   {
   combufrec *ibuf;

   if (!emitvaloki)
	  return;
   ibuf = inbuf + inbens;
   kirjemvaloki(ibuf);
   }
#endif // EMIT

void alkusanoma(combufrec *obuf, int cn, int flags)
{
	obuf->d.alku.tunn = 1;
	obuf->d.alku.vaihe = k_pv+1;
	obuf->d.alku.nrec = datf2.numrec;
	memcpy(obuf->d.alku.konetunn, konetunn, 2);
	obuf->d.alku.flags = (flags | lah_vaihto[cn]);
	lah_vaihto[cn] = 0;
}

bool tark_alku(const combufrec * const cbuf, int cn)
{
	static int var[MAX_LAHPORTTI];
	wchar_t ln[84] = L"";

	if (cbuf->d.alku.tunn != 1)
		return(true);
	if (cbuf->len == 3)
		return(false);
	if (cbuf->d.alku.nrec != datf2.numrec) {
		erbeep();
		if (cbuf->d.alku.flags & 1) {
			swprintf(ln, L"Vaihen vaihtopyyntö koneelta, jonka KILP.DAT eri pituinen, yhteys %d", cn+1);
			writeerror_w(ln, 0);
			}
		else if (var[cn] == 0) {
			swprintf(ln, L"Yhteyspyyntö koneelta, jonka KILP.DAT eri pituinen, yhteys %d", cn+1);
			writeerror_w(ln, 0);
			}
		var[cn]++;
		//		return(true);
		}
	if (cbuf->d.alku.flags & 1) {
		static int edvaihto;
		if (mstimer() > edvaihto + 5000 || cbuf->d.alku.vaihe != k_pv + 1) {
			edvaihto = mstimer();
			pyyda_vaihdavaihe(cbuf->d.alku.vaihe - 1, cn+1);
			}
		}
	else if (cbuf->d.alku.vaihe != k_pv + 1) {
		if (var[cn] == 0) {
			erbeep();
			swprintf(ln, L"Eri vaiheessa oleva kone pyytää yhteyttä, yhteys %d", cn+1);
			writeerror_w(ln, 0);
			}
		var[cn]++;
		return(true);
		}
//	var = 0;
#ifdef _CONSOLE
	if (ln[0]) {
		wchar_t ch = L' ';
		wselectopt(L"J)atka, P)oistu ohjelmasta", L"JP", &ch);
		if (ch == L'P') {
			lopetus();
			return(true);
			}
		}
#endif
	return(false);
}

static int vlkm;

void tarkcom(LPVOID lpCn)
    {
	 int  nj,cn, lisa, inb;
//    static int varoitettu;

    while (comfl) {
        if (inbens == inbseur)
           Sleep(50);
        else {
		   for (;;) {
               EnterCriticalSection(&inb_CriticalSection);
               if (inbens == inbseur)
						inb = -1;
					else
                  inb = inbens;
               LeaveCriticalSection(&inb_CriticalSection);
			   if (inb < 0)
						break;
               lisa = 1;
					cn = inbuf[inb].portti-1;
					switch (inbuf[inb].pkgclass) {
					case ALKUT:
						break;
					case KILPT:
						tark_kilp(cn, 1);
						break;
					case KILPPVT:
						tark_kilp(cn, 2);
						break;
					case AIKAT:
						tark_aika(cn);
						/*
											 if (!varoitettu)
											 writeerror_w(L"Vastaanotettu MAALI-ohjelman ajanottotieto."
											 L" Korjaa konfigurointi", 0);
											 varoitettu = 1;
											 */
						break;
					case VAIN_TULOST:
						tark_kilp(cn, 0);
						break;
					case EMITT:
#if defined(EMIT) && (defined(MAALI) || defined(TALLEMIT)) && !defined(EILEIMAT)
						tark_emit(cn);
#endif
						break;
					case EMITVA:
#if defined(EMIT) && (defined(MAALI) || defined(TALLEMIT)) && !defined(EILEIMAT)
						tark_emitva(cn);
#endif
						break;
					case SEURAT:
#if defined(SEURAVAL)
						tark_seura(cn);
#endif
						break;
					case EXTRA:
						//#if defined(VALUKIJA) || defined(MAKI)
						tark_extra(cn);
						if (suljesuoraan) {
							return;
						}
						//#endif
						break;
					case FILESEND:
						tark_tiedosto(cn);
						break;
					default:
						if (testi) {
							wchar_t line[200];
							swprintf(line, L"Vastaanotettu sanoma - luokka %d", inbuf[inb].pkgclass);
							wkirjloki(line);
							}
					  break;
                  }
					EnterCriticalSection(&inb_CriticalSection);
               inbuf[inb].lahetetty = 1;
               inbens = (inb + lisa) % INBUFL;
               LeaveCriticalSection(&inb_CriticalSection);
               nj = (inbseur + INBUFL - inbens) % INBUFL;
               }
           }
        if( autoclose && (nj <= INBUFL / 2) ) {
			autoclose = 0;
            for (cn = 0; cn < maxyhteys; cn++)
                vastcom[cn] = vastcom0[cn];
				writeerror_w(L"Puskurissa taas tilaa, portti avataan",1000, true);
	      	}
        }
	 }

#ifdef TCPSIIRTO

void merk_uusinta(int cna, int cny, long ta, long ty, int autouusinta);

bool vanhaXMLsan = false;

void lahetaXML_TCP(combufrec *obuf, int cn)
   {
   int piste, piste2, len = 0, dk, nsent = -1, nsent_tot = 0;
	long pos, t, cje;
	char buf[8000], *p;
   kilptietue kilp;

	if (lah_alku[cn]) {
	  if (tcpautoalku)
         autofile(0);
      lah_alku[cn] = 0;
      lahfl[cn] = FALSE;
      return;
      }
   if (tcpkaynnistys[(int)hComm[cn]]) {
      merk_uusinta(cn, cn, 0, 0, 1);
      tcpkaynnistys[(int)hComm[cn]] = 0;
      }
   cje = cjens[cn];
   if (obuf->pkgclass == KILPT || (obuf->pkgclass == VAIN_TULOST && obuf->d.v.vali >= -1)) {
      dk = obuf->d.v.dk;
		if (obuf->pkgclass == VAIN_TULOST) {
			piste = obuf->d.v.vali;
			if (piste > kilpparam.valuku)
				piste = 0;
			}
		else {
			piste = -2;
			}
		kilp.GETREC(dk);
		memset(buf, 0, sizeof(buf));
		piste2 = piste;
		if (!vanhaXMLsan) {
			TextFl *tmpfile = new TextFl(autofileparam.sanafname, L"w+t");
			if (k_pv && piste == 0)
				piste2 = kilpparam.valuku+1;
			if (tmpfile->IsOpen()) {
				kirjXmlOo(tmpfile, kilp, 0, piste2, -1);
				tmpfile->Rewind();
				p = buf;
				while (!tmpfile->Feof()) {
					if (autofileparam.sanafchars == L'8')
						tmpfile->ReadLine8(p, buf+sizeof(buf)-p-2);
					else
						tmpfile->ReadLineA(p, buf+sizeof(buf)-p-2);
					p += strlen(p);
					}
				len = strlen(buf);
				}
			if (tmpfile)
				delete tmpfile;
			}
		else {
			if (k_pv && piste == 0 && tcptulos == L'Y')
				piste2 = kilpparam.valuku+1;
			buf[0] = STX;
			len = xmlsanoma(buf+1, sizeof(buf)-3, kilp, dk, piste2, cje);
			if (k_pv && piste == 0 && tcptulos == L'M') {
				buf[++len] = ETX;
				buf[++len] = STX;
				len += xmlsanoma(buf+len+1, sizeof(buf)-3, kilp, dk, 0, cje);
				}
			}
		}
#ifdef EMIT
   if (obuf->pkgclass == EMITVA) {
		memset(buf, 0, sizeof(buf));
		buf[0] = STX;
	  len = xmlemitvasanoma(buf+1, sizeof(buf)-3, obuf->d.emva, cje);
	  }
#endif
	if (len) {
		if (vanhaXMLsan) {
			buf[len+1] = ETX;
			len += 2;
			}
		while (comfl && nsent_tot < len) {
			EnterCriticalSection(&tcp_CriticalSection[cn]);
			wrt_st_x(cn, len-nsent_tot, buf, &nsent);
			LeaveCriticalSection(&tcp_CriticalSection[cn]);
			nsent_tot += nsent;
			if (len-nsent_tot)
				Sleep(100);
			}
		}
	if (len == nsent_tot) {
		cje++;
		EnterCriticalSection(&outb_CriticalSection);
		if (levypusk) {
			Sleep(1);
			pos = (cje*maxyhteys+cn)*combufsize;
			pos = _lseek(obfile, pos ,SEEK_SET);
			if (pos > 0) {
				t = t_time_l(biostime(0, 0), t0);
				if (t == 0)
					t = 1;
				if (_write(obfile, &t, 4) < 0)
					iowerr(obfilename,2,pos);
				}
			cjens[cn] = cje;
			}
		else
			cjens[cn] = cje % OUTBUFL;
		LeaveCriticalSection(&outb_CriticalSection);
		}
	lahfl[cn] = FALSE;
	}

#endif

void init_class_len(void)
   {
	combufrec *cbuf;

	class_len[ALKUT] = 10;
	class_len[KILPT] = kilpparam.kilprecsize0 + 6;
	class_len[KILPPVT] = kilpparam.kilppvtpsize + 8;
	class_len[VAIN_TULOST] = sizeof(INT32) + 20;
	class_len[SEURAT] = sizeof(cbuf->d.sra);
	class_len[AIKAT] = 2*sizeof(aikatp) + 1;
	class_len[EMITT] = sizeof(emittp);
	class_len[EMITVA] = sizeof(emitvatp);
	class_len[EXTRA] = 20;
	combufsize = class_len[KILPT] + 14;
	if (combufsize < class_len[KILPPVT] + 14) {
		combufsize = class_len[KILPPVT] + 14;
		}
	if (combufsize < class_len[EMITT] + 14) {
		combufsize = class_len[EMITT] + 14;
		}
	if (combufsize < class_len[EMITVA] + 14) {
		combufsize = class_len[EMITVA] + 14;
		}
	class_len[FILESEND] = combufsize - 14;
	}

void emitva_uusi_sarja(int cn, int srj, int osuus)
{
	for (int i = minkilpno; i <= maxkilpno; i++) {
		int d;
		kilptietue kilp;
		emittp em;

		if ((d = getpos(i)) <= 0)
			continue;
		if (srj >= 0) {
			kilp.GETREC(d);
			if (kilp.Sarja() != srj)
				continue;
		}
		if (getem(&em, i, 0) >= 0) {
			EnterCriticalSection(&outb_CriticalSection);
			laheta_emva_yht(&em, cn);
			LeaveCriticalSection(&outb_CriticalSection);
			}
		}
}

void uusintaTCPvalitut(int sarja, wchar_t cp, int alku, int loppu, int cn)
{
	int p, lj, lj1, d, piste, srj, tm;
	kilptietue kilp;

	if (sarja >= 0)
		srj = sarja;
	else {
		srj = 0;
		sarja = sarjaluku - 1;
	}
	for (; srj <= sarja; srj++) {
		p = -1;
		switch (cp) {
		case L'T':
			lj = 0;
			lj1 = 0;
			break;
		case L'V':
		case L'M':
		case L'L':
			lj = 0;
			lj1 = 4;
			break;
		case L'H':
			lj = 1;
			lj1 = 4;
			break;
		}
		EnterCriticalSection(&tall_CriticalSection);
		for (;;) {
			while (p < 0 && lj <= lj1)
				p = jalku[srj][lj++];
			if (p < 0 || (lj > lj1 + 1))
				break;
			d = p;
			p = JarjSeur(0, 0, p);
			kilp.GETREC(d);
			if (cp == L'L')
				piste = -1;
			else
				piste = 0;
			for (; piste <= Sarjat[srj].valuku[k_pv]; piste++) {
				if (!piste && cp == L'V')
					continue;
				if (piste && (cp == L'T' || cp == L'H'))
					break;
				if (piste >= 0 && cp == L'L')
					break;
				tm = kilp.maali(piste);
				if (alku != TMAALI0 && NORMTULOS(tm - alku) > 12 * TUNTI)
					continue;
				if (loppu != TMAALI0 && NORMTULOS(loppu - tm) > 12 * TUNTI)
					continue;
				laheta(d, kilp.kilpno, piste + 2, 0, 0, -cn - 1, 0);
			}
		}
	}
	LeaveCriticalSection(&tall_CriticalSection);
}

#ifdef SERVER
void lahetanumerotTCP(int cn, INT32 alku, INT32 loppu)
   {
   INT32 cj, pos;
	combufrec ibuf;

   if (alku >= 0 && alku < cjens[cn] && loppu >= alku) {
      if (loppu >= cjens[cn])
         loppu = cjens[cn]-1;
      for (cj = alku; cj <= loppu; cj++) {
         EnterCriticalSection(&outb_CriticalSection);
			pos = _lseek(obfile, ((cj+1)*maxyhteys+cn)*sizeof(combufrec),
				SEEK_SET);
			if (pos >= 0)
				pos = _read(obfile, &ibuf, sizeof(combufrec));
         LeaveCriticalSection(&outb_CriticalSection);
         if (pos <= 0) {
            iowerr(obfilename,1,((cj+1)*maxyhteys+cn)*sizeof(combufrec));
			return;
            }
         if (ibuf.pkgclass == VAIN_TULOST && ibuf.d.v.vali >= -1)
			laheta(ibuf.d.k.dk, ibuf.d.k.entno, ibuf.d.v.vali+2, 0, 0, -cn-1, 0);
         if (ibuf.pkgclass == KILPT)
			laheta(ibuf.d.k.dk, ibuf.d.k.entno, 0, 0, 0, -cn-1, 0);
         }
	  }
   }


void uusintaTCP(int cn)
	{
	wchar_t ch = L' ', cs = L' ', cp = L'T';
   INT32 alku = TMAALI0, loppu = TMAALI0;
   INT srj = 0;
   kilptietue kilp;

   wselectopt(L"K)aikki sarjat, V)alitse sarjat, S)anomanumerot", L"VKS", &cs);
   if (cs != L'S') {
      wselectopt(L"T)ulokset, V)äliajat, M)olemmat, H)yl+kesk+avoimet, L)ähtöajat", L"TVMLH", &cp);
      wselectopt(L"K)oko kilpailu, V)alittava aikavali, Esc: peruuta", L"KV\x1b", &ch);
      if (ch == L'V') {
      clrln(ySize-3);
         vidspwmsg(ySize-3, 0, 7, 0, L"Aikarajat: alku          loppu           Esc: peruuta");
		 INPUTAIKAW(&alku,t0,8,16, ySize-3,L"\r\x1b",&ch);
   if (ch == ESC)
	  return;
         INPUTAIKAW(&loppu,t0,8,31, ySize-3,L"\r\x1b",&ch);
         }
      if (ch == ESC)
         return;
      do {
         if (cs == L'V') {
            vidspwmsg(ySize-3,0,7,0,L"    Valitse sarja :               <Esc>: lopeta");
            srj = luesarja(L"", &ch);
            if (ch == ESC)
				break;
            }
		 uusintaTCPvalitut(srj, cp, alku, loppu, cn);
         } while(cs == L'V' || ++srj < sarjaluku);
      }
   if (cs == L'S' && levypusk) {
	   clrln(ySize-3);
      vidspwmsg(ySize-3, 0, 7, 0, L"Numeroalue: alku         loppu           Esc: peruuta");
      alku = 0;
      INPUTINTW(&alku,6,17, ySize-3,L"\r\x1b",&ch);
         if (ch == ESC)
            return;
      loppu = alku;
      INPUTINTW(&loppu,6,31, ySize-3,L"\r\x1b",&ch);
      if (ch == ESC)
   return;
      lahetanumerotTCP(cn, alku, loppu);
      }
	}

#ifdef EMIT
void emitva_uusinta(int cn, int tietue)
	{
	emittp em;
	kilptietue kilp;
	wchar_t ch, tc, snimi[12] = L"";
	INT16 srj = -1;

	if (cn < 0 && tietue >= 0) {
		lueem(&em, tietue);
		EnterCriticalSection(&outb_CriticalSection);
		for (cn = 0; cn < maxyhteys; cn++) {
			if (com_emitva[cn] && comopen[cn] && lahcom[cn] && lahec[cn] != 2 && !lahkellovain[cn]) {
				laheta_emva_yht(&em, cn);
				}
			}
		LeaveCriticalSection(&outb_CriticalSection);
		return;
		}
	if (cn >= 0 && tietue < 0) {
		while (1) {
			ch = L' ';
			wselectopt(L"Lähetä K)aikki tai valitse S)arja, Esc: lopeta", L"KS\x1b", &ch);
			if (ch == ESC)
				break;
			if (ch == L'S' || ch == L'M') {
				if (srj >= 0)
					wcscpy(snimi, Sarjat[srj].sarjanimi);
				srj = luesarja(snimi, &tc);
				if (tc == ESC)
					break;
				}
			else
				srj = -1;
			emitva_uusi_sarja(cn, srj, 0);
			}
		}
	}
#endif
#endif

void lahetaKopiot(int cn, combufrec obuf)
{
	int cnala = 0, cnyla;

	if (cn < 0)
		cnyla = maxyhteys-1;
	else {
		cnala = cn;
		cnyla = cn;
		}
	for (int cc = cnala; cc <= cnyla; cc++) {
		if (comopen[cc] && lahcom[cc]
			&& !(obuf.pkgclass != AIKAT && com_ajat[cc] == 2)
			&& !(obuf.pkgclass == AIKAT && !com_ajat[cc])
			&& !(obuf.pkgclass == EMITT && !com_emit[cc])
			&& !(obuf.pkgclass == EMITVA && com_emit[cc] <= 0)
			&& !(obuf.pkgclass == EXTRA && (obuf.d.x.d1 == 4 || obuf.d.x.d1 == 9))
			) {
			EnterCriticalSection(&outb_CriticalSection);
			buflah(cc, &obuf);
			LeaveCriticalSection(&outb_CriticalSection);
			}
		}
}


