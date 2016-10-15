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

#if defined(__BORLANDC__)
#pragma option -K
#endif
#include <stdio.h>
#include <stdlib.h>
#ifdef __linux__
//#include <sys/io.h>
#include <unistd.h>
#include <sys/types.h>
#else
#include <windows.h>
#include <io.h>
#include <process.h>
#endif
#include <sys/timeb.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <bstrings.h>
#include <butil.h>
//#include <wincom.h>
#pragma hdrstop
#include <sys/stat.h>
#include "VDeclare.h"
#ifndef _CONSOLE
#include "UnitMain.h"
#endif

#define UDPYHTEYS

#if !defined(LUENTA)
#define LAH_VAIKA
#endif
#ifndef __linux__
//#define VERTKELLO
#endif
#ifndef DEBUG1
#define DEBUG1 1
#endif
#ifndef DEBUG2
#define DEBUG2 0
#endif
//#define DBGXX
//#define PING

#define ALKUT 0
#define KILPT 1
#define OSUUST 2
#define VAIN_TULOST 3
#define AIKAT 4
#define TARKT 5
#define EMITT 6
#define VEMITT 8
#define YHTEISLAHTOT 9
#define FILESEND  10
#define EMITVA 13
#define EXTRA 14
#define N_CLASS 15

#define INQ_EMPTY 1

#define LUEBUF 1000    
#define LUECLIBUF 100

void buflah(int cn, combufrec *obuf);

int alkut = ALKUT;
HANDLE hComm[MAX_PORTTI];
int ackreq[MAX_LAHPORTTI];              /* != 0 Ilmaisee, että yhteys toimii */
int yhtfl[MAX_LAHPORTTI];              /* != 0 Ilmaisee, että yhteys toimii */
int keyclose[MAX_LAHPORTTI];                   /* Käyttäjä sulkenut portin */
int autoclose;               /* Ohjelma sulkenut vastaanoton */
int  initid[MAX_LAHPORTTI];                    /* inpakid initialisoimatta */
extern int intv[];                      /* Tiedonsiirron odotusväli     */
//int intv[2];                      /* Tiedonsiirron odotusväli     */
combufrec *inbuf;      /* Saapuvien jono */
combufrec *outbuf[MAX_LAHPORTTI];     /* Lähtevien jono */
int combufsize;
char *combuf[MAX_LAHPORTTI];          /* IO-portin puskuri */
int  lahfl[MAX_LAHPORTTI];            /* Lähetys käynnissä */
int  vkesken[MAX_LAHPORTTI];              /* Vastaanotto kesken */
int  chcomkesto[MAX_LAHPORTTI];           /* Yhden merkin lähettämiskesko */
int  hyvkesken[MAX_LAHPORTTI];            /* Hyväksyntä kesken */
char outpakid[MAX_LAHPORTTI];    /* seuraava lähetettävä id */
int vastcom0[MAX_LAHPORTTI];
int lahcomserver[MAX_LAHPORTTI];
int in_tark_kello[MAX_LAHPORTTI];
int class_len[N_CLASS];     // Lähetettävän tiedon pituus mukaanlukien
                                   // checksum ja itse tieto
extern int timerfl, tbase, trate;
extern INT vpiste[];
//int comtype[MAX_PORTTI];
INT yhteysalku = 0;
int tcpautoalku;
INT ts_close[MAX_LAHPORTTI];
int acn[MAX_PORTTI];
void tall_ec(UINT32 bdg, INT valeim, INT32 aika, INT kielto);
static void tark_extra(int cn);

#ifdef PING
HTHREAD hPingThread;
#endif

#define R_BUFLEN_C 900

comparamtp comparam[MAX_PORTTI];
/*
typedef struct {
   char code;
   USHORT t1;
   } ctrltype;

typedef union {
   struct {
      USHORT alku;
      char badge[3];
      char fill1;
      char week;
      char year;
      char fill2;
      char chksum;
      ctrltype c[50];
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
   } san_type;
*/

#ifdef DBGXX
    FILE *dfl;
    char dflb[20000];

//    dfl = fopen("comdump", "wb");
//    memset(dfbl, 0, sizeof(dflb));
#endif

#ifdef _CONSOLE
void WRITEYHT(wchar_t *st, INT cn)
   {
   INT r;
	wchar_t st2[3];

   if (!status_on)
	  return;
	wcscpy(st2, L"  ");
	if (st && *st)
		wmemcpy(st2, st, 2);
   if (!server_on) {
	  if (ySize <= 25) {
		 if (((maxyhteys+cn-dcom)%maxyhteys)<3)
			viwrrectw(20,(72+3*((maxyhteys+cn-dcom)%maxyhteys)),20,
			   (73+3*((maxyhteys+cn-dcom)%maxyhteys)),st2,7,0,0);
		 }
	  else {
		 if ((r = ((maxyhteys+cn-dcom)%maxyhteys)) < ySize - 24) {
			if (maxyhteys < ySize-24)
			   r = cn;
			viwrrectw(19+r, 78, 19+r, 79, st2 ,7,0,0);
			}
		 }
	  }
   else {
	  if (cn >= yhteysalku && cn < yhteysalku+ySize-7)
		 viwrrectw(cn-yhteysalku+3, 43,cn-yhteysalku+3, 44, st2, 7, 0, 0);
	  }
   }

void DSPQ(INT r, INT cn)
   {
   INT nj, y;

   if (!status_on)
	  return;
   nj = lahettamatta(cn);
   if (!levypusk) nj = (nj + OUTBUFL) % OUTBUFL;
   nj += (lah_alku[cn] > 0);
   if (!server_on) {
	  if (ySize <= 25) {
		 y = (maxyhteys+cn-dcom)%maxyhteys;
		 if (y < 3) {
			if (nj > 999) {
			   vidint((r),71+3*y,3,(nj%100));
			   vidspmsg(r, 71+3*y,7,0,"*");
			   }
			else
			   vidint((r),71+3*y,3,nj);
			}
		 }
	  else {
		 if ((r = ((maxyhteys+cn-dcom)%maxyhteys)) < ySize - 24) {
			if (maxyhteys < ySize-24)
			   r = cn;
			vidint(19+r, 67, 2, cn+1);
			vidint(19+r, 71, 5, nj);
			}
		 }
	  }
   else {
	  if (cn >= yhteysalku && cn < yhteysalku+ySize-7) {
		 vidint(cn-yhteysalku+3, 47, 6, nj);
		 vidint(cn-yhteysalku+3, 56, 6, cjens[cn]);
		 }
	  }
   }
#endif

void DSPIQ(int nj)
   {
#ifdef _CONSOLE
   vidint(18, 75, 5, nj);
#endif
   }

int tarkkielto(int cn, int kielto)
	{
	if ( !comopen[cn] || !lahcom[cn] || cn == kielto - 1 ||
		 (kielto < 0 && cn != -kielto - 1))
		 return(1);
	if (kielto > 0 && estavalitys[cn] &&
		 (kielto > 32 || (estavalitys[cn] & _lrotl(1, kielto-1))))
		 return(1);
	if (lahec[cn] == 2)
		return(8);
	return(0);
	}

void laheta_yl(int srj, int kielto)
   {
   int  v,cn;
   char ch, ch2;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2)
		  continue;
	  if (!levypusk) {
		 v = (cjseur[cn] + 1) % OUTBUFL;
		 ch = 0;
		 while (ch != ESC && v == cjens[cn]) {
			clrln(ySize-3);
#ifndef LANGSV_K
			vidspmsg(ySize-3,0,7,0,"Odottaa tiedonsiirron etenemistä.  'Esc' "
			   "lopettaa yhteislähtöaikojen siirron");
#else // LANGSV_K
			vidspmsg(ySize-3,0,7,0,"Odottaa tiedonsiirron etenemistä.  'Esc' "
			   "lopettaa yhteislähtöaikojen siirron");
#endif // LANGSV_K
			ch = readkbd(&ch2, FALSE, spoolfl);
			Sleep(20);
			}
		 if (ch == ESC) break;
		 }
	  obuf.pkgclass = YHTEISLAHTOT;
	  obuf.len = class_len[YHTEISLAHTOT];
	  obuf.d.y.sarja = srj;
	  if (srj >= 0) {
		 if (srj < sarjaluku) {
			obuf.d.y.fl = ylmyoh;
			for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
				obuf.d.y.ylahto[os][0] = Sarjat[srj].ylahto[os];
				obuf.d.y.ylahto[os][1] = Sarjat[srj].ylkynnys[os];
				}
			}
		 }
	  else {
		 obuf.d.y.ylahto[0][0] = Sarjat[-srj-1].lahto;
		 }
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
	}

static void tark_ylahto(int cn)
   {
   combufrec *ibuf;
   int srj, muut = 0, os;
   int ylahto[MAXOSUUSLUKU], ylkynnys[MAXOSUUSLUKU];

//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
	srj = ibuf->d.y.sarja;
	if (srj >= 0) {
		if (srj < sarjaluku) {
			for (os = 0; os < Sarjat[srj].ntosuus; os++) {
				ylahto[os] = ibuf->d.y.ylahto[os][0];
				ylkynnys[os] = ibuf->d.y.ylahto[os][1];
				}
			if (memcmp(Sarjat[srj].ylkynnys, ylkynnys, Sarjat[srj].ntosuus*sizeof(ylkynnys[0])) ||
				memcmp(Sarjat[srj].ylahto, ylahto, Sarjat[srj].ntosuus*sizeof(ylahto[0])) ||
				ylmyoh != (ibuf->d.y.fl & 1))
				{
				muut = 1;
				for (os = 0; os < Sarjat[srj].ntosuus; os++) {
					Sarjat[srj].ylahto[os] = ylahto[os];
					Sarjat[srj].ylkynnys[os] = ylkynnys[os];
					}
				ylmyoh = ibuf->d.y.fl & 1;
				}
			}
		}
	else {
		if (Sarjat[-srj-1].lahto != ibuf->d.y.ylahto[0][0])
			muut = 1;
		Sarjat[-srj-1].lahto = ibuf->d.y.ylahto[0][0];
#if MAXOSUUSLUKU == 1
		paivjarjarr(-srj-1);
#endif
		}
	if (srj < sarjaluku && (muut || !katkkierto[cn])) {
		kirjEventXml(sarjaxmlfname, true, false, false);
		laheta_yl(srj, cn+1);
		}
	}

//  viestin laheta()
//
// luokka == 1,   tietoja ei lähetetä koneelle, jolle
//                lähetään aikataulukko; muuten vain aika
// luokka == 2,   lähetään aika
// luokka & 4,    lähetään osuustietoja, lisäkoodia kerrottuna 16:lla
// luokka == 8,   lähetään muut osuustiedot
//                paitsi, jos maalilähetysrajoitus
//

void laheta(int dkilp, int entno, int os, int piste, int comtarfl, int kielto,
   UINT32 luokka, int pakota_lah)
   {
   int  cn, lahTCP;
   combufrec obuf;
   kilptietue kilp;

   lahTCP = comtarfl & 0x0010;
	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto))
		  continue;
	  if ((com_ajat[cn] && luokka == 1) || com_ajat[cn] == 2
		 ) continue;
	  if ((comtype[cn] == comtpTCPXML || comtype[cn] == comtpTCPSRVXML) && !lahTCP)
		 continue;
	  obuf.d.k.dk = dkilp;
	  obuf.d.k.tarf = comtarfl && tarlahcom[cn];
	  obuf.d.k.entno = entno;
	  obuf.d.k.pakota = (char) pakota_lah;
	  GETREC(&kilp, dkilp);
	  if (os == 0) {
		 obuf.pkgclass = KILPT;
		 obuf.len = class_len[KILPT];
		 memmove(&(obuf.d.k.ckilp), (char *)&kilp, kilpparam.kilprecsize0);
		 }
	  else if (luokka & 4) {
		 obuf.pkgclass = OSUUST;
		 obuf.len = class_len[OSUUST];
		 obuf.d.os.osuus = os;
		 obuf.d.os.muutos = luokka / 16;
		 memmove(&(obuf.d.os.cos), (char *)&kilp.ostiet[os-1], kilpparam.osrecsize0);
		 }
	  else {
		 obuf.pkgclass = VAIN_TULOST;
		 obuf.len = class_len[VAIN_TULOST];
		 obuf.d.p.osuus = os;
		 obuf.d.p.piste = piste;
		 memcpy(obuf.d.p.cva, &kilp.ostiet[os-1].va[piste], sizeof(vatp));
//		 obuf.d.p.kone = kilp.ostiet[os-1].kone;
		 }
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
   }

#if defined(MAALI) && !defined(LUENTA)
extern int odota;

void laheta_aika(aikatp *dtm, aikatp *itm, int kielto, int pakota_lah)
   {
   int  cn ;
   combufrec obuf;
   aikatp ntm = {TMAALI0*AIKAJAK, 0 ,0 ,0
	  ,0
	  };

   if (dtm == 0) dtm = &ntm;
   if (itm == 0) itm = &ntm;
	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || !com_ajat[cn])
		  continue;
	  obuf.pkgclass = AIKAT;
	  obuf.len = class_len[AIKAT];
	  obuf.d.a.pakota = (char) pakota_lah;
	  memcpy((char *)&(obuf.d.a.daika),
		 (char *)dtm, sizeof(aikatp));
	  memcpy((char *)&(obuf.d.a.iaika),
		 (char *)itm, sizeof(aikatp));
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
   }

#define MAXINITAJAT 30
static void tark_aika(int cn)
   {
   aikatp ia, da;
   combufrec *ibuf;
   int i = 0;
   static int initca[MAX_LAHPORTTI], initbufptr[MAX_LAHPORTTI];
   static char initbuf[MAX_LAHPORTTI][MAXINITAJAT][9+2*sizeof(aikatp)];
   kilptietue kilp, entkilp;
   int d, os, pak_os;
   int piste, ino;

   if (!com_ajat[cn]) {
      writeerror("Vastaanotettu MAALI-ohjelman ajanottotieto."
		 " LISÄÄ PARAMETRI LÄHAIKA", 0);
	  return;
      }

   // Jos ajanottoa ei ole vielä käynnistetty, sijoitetaan saapuva tieto
   // puskuriin initbuf odottamaan myöhempää käsittelyä ja poistutaan
   if (!timerfl) {
	  if (initbufptr[cn] < MAXINITAJAT) {
         memcpy((char *) initbuf[cn][initbufptr[cn]++],
				inbuf + inbens, 9+2*sizeof(aikatp));
//			   (char *)inbuf + inbens*combufsize, 9+2*sizeof(aikatp));
         }
      return;
      }

   // Jos tähän tullaan, on ajanotto käynnistetty ja uusinta sanomaa ei ole
   // pantu puskuriin initbuf

   do {
      // ensin katsotaan onko puskurissa tietoja, jotka pitää käsitellä ensin
      // jos on, käydään tätä looppia läpi, kunnes aiemmat on käsitelty ja
      // sitten käsitellään se sanoma, jonka johdosta tähän aliohjelmaan tultiin

      if (!initca[cn]) {
         if (i == initbufptr[cn]) initca[cn] = 1;
         else ibuf = (combufrec *) initbuf[cn][i++];
         }
	  if (initca[cn])
	    ibuf = inbuf + inbens;
//		ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
      memcpy(&ia, &ibuf->d.a.iaika, sizeof(aikatp));
	  memcpy(&da, &ibuf->d.a.daika, sizeof(aikatp));
		if (maxjono > 0) {
			ino = 0;
			if (purajak(ibuf->d.a.iaika.t) != TMAALI0)
				ino = ibuf->d.a.iaika.jono;
			else
				if (purajak(ibuf->d.a.daika.t) != TMAALI0)
					ino = ibuf->d.a.daika.jono;
			 if (yhtjonot)
				 ino = yhtjono(cn, ino);
			if (ino > maxjono)
				ino = 0;
			}
		else
			ino = 0;

	  tall_rivi(ino, &ia, &da, 0, 0, (com_ajat[cn] != 3)*(cn+1), !katkkierto[cn] && ibuf->d.a.pakota);

	  //  Viestiohjelmassa tallennetaan uusi osuusaika myös kilpailija-
	  //  tietoihin tämän sanoman perusteella.

	  if (!da.kno && ia.kno >= minkilpno && ia.kno <= maxkilpno &&
			ia.piste < kilpparam.valuku+1 &&
		 (d = getpos(ia.kno)) > 0) {
		 EnterCriticalSection(&tall_CriticalSection);
		 GETREC(&kilp, d);
		 memcpy(&entkilp, &kilp, sizeof(kilptietue));
		 os = ia.osuus;
		 piste = ia.piste;
		 kilp.setMaali(os, piste, pyoristatls(purajak(ia.t), 1));
		 pak_os = (ibuf->d.a.pakota && !katkkierto[cn]) ? os+1 : 0;
//		 if (ia.kone)
//			kilp.ostiet[os].kone = ia.kone;
//		 if (piste) {
//			tallva(ia.t, os, piste, ia.kno, cn+1, 0);
//			}
//		 else
			tallenna(&kilp, d, TRUE, cn+1, pak_os, &entkilp);
		 LeaveCriticalSection(&tall_CriticalSection);
		 }

	  if (od_fl && purajak(od_aika) == TMAALI0 && purajak(da.t) == TMAALI0 &&
		 ia.t == aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows-1)->t  && ia.kno == 0) {
		 od_aika = ibuf->d.a.iaika.t;
		 odota = aikajono[ino]->aktrows-1;
		 }
      } while (!initca[cn]);
   }
#endif


static void tark_kilp(int cn)
   {
   int  er = 0;
   UINT32  d; 
	int dd, stat = 0, kno = 0;
   INT  entno;
   combufrec *ibuf;
   kilptietue kilp, *pkilp;

//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   d=ibuf->d.k.dk;
   pkilp = (kilptietue *) ibuf->d.k.ckilp;
   entno=ibuf->d.k.entno;
   if (d < datf2.numrec) {
		GETREC(&kilp, d);
		stat = kilp.kilpstatus;
		if (!stat)
			kno = kilp.kilpno;
		}

   if (entno == 0) {
	   if (d < datf2.numrec) {
		  if (stat == 0) {
			  if (getpos(kno) == (int) d &&
				  kno == pkilp->kilpno)
				  er = -1;
			  else
				  er = 1;
			  }
		  else if (getpos(kno) > 0)
			  er = 1;
		  }
	   if (er == 0) {
		   kilp.nollaa();
		   memcpy((char *) &kilp, ibuf->d.k.ckilp, kilpparam.kilprecsize0);
		   if (d >= datf2.numrec || stat != 0) {
				dd = -1;
				if (getpos(kilp.kilpno) <= 0)
					addtall(&kilp, &dd, cn + 1);
				if ((int)d != dd)
					er = 1;
				}
		   else {
			   er = 1;
			   }
		   }
	   }
	else if (pkilp->kilpstatus != 0) {
	  if ((int)d == getpos(entno))
		  deltall(d, cn + 1);
	  else {
		  if (naytavaroitukset || getpos(entno) > 0)
			 er = 1;
		  }
	  }
   else {
	  EnterCriticalSection(&tall_CriticalSection);
	  GETREC(&kilp, d);
	  if (kilp.kilpstatus != 0 || kilp.kilpno == entno) {
		  memcpy((char *) &kilp, ibuf->d.k.ckilp, kilpparam.kilprecsize0);
		  tallenna(&kilp,d,ibuf->d.k.tarf, cn + 1, 0, 0);
		  }
	  else {
		  if (naytavaroitukset || kilp.kilpno != ((kilptietue *) ibuf->d.k.ckilp)->kilpno)
			  er = 1;
		  }
	  LeaveCriticalSection(&tall_CriticalSection);
	  }
   if (er > 0) {
#ifndef LANGSV_K
	  writeerror(
		 "Tiedostot eri koneilla eivät ole yhtäpitäviä",0);
	  writeerror("Poistu ohjelmasta, kopioi KILP.DAT"
		 "lähettävältä tälle koneelle",0);
#else // LANGSV_K
	  writeerror(
		 "Okonsistent data mellan datorer",0);
	  writeerror("Avsluta program, kopiera KILP.DAT"
		 "fr†n den sändande dator till denna dator",0);
#endif // LANGSV_K
	  }
   }

static void tark_osuus(int cn)
   {
   int  er = 0;
   INT  d, os;
   combufrec *ibuf;
   kilptietue kilp;
   osrec *ost;

//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   d = ibuf->d.os.dk;
	EnterCriticalSection(&tall_CriticalSection);
   GETREC(&kilp, d);
   if (kilp.kilpstatus != 0 || ibuf->d.os.kno != kilp.kilpno) {
	   er = 1;
	   }
   else {
	   os = ibuf->d.os.osuus - 1;
	   ost = (osrec *) ibuf->d.os.cos;
	   if (ibuf->d.os.muutos & 1)
		   memcpy(kilp.ostiet[os].nimi, ost->nimi, kilpparam.lnimi);
	   if (ibuf->d.os.muutos & 2)
		   kilp.ostiet[os].lisno = ost->lisno;
	   if (ibuf->d.os.muutos & 4)
		   memcpy(kilp.ostiet[os].osseura, ost->osseura, sizeof(ost->osseura));
	   if (ibuf->d.os.muutos & 8)
		   memcpy(kilp.ostiet[os].arvo, ost->arvo, sizeof(ost->arvo));
	   if (ibuf->d.os.muutos & 0x10)
		   memcpy(kilp.ostiet[os].hajonta, ost->hajonta, sizeof(ost->hajonta));
	   if (ibuf->d.os.muutos & 0x20)
		   kilp.ostiet[os].badge[0] = ost->badge[0];
	   if (ibuf->d.os.muutos & 0x40)
		   kilp.ostiet[os].badge[1] = ost->badge[1];
	   if (ibuf->d.os.muutos & 0x80)
		   kilp.ostiet[os].laina[0] = ost->laina[0];
	   if (ibuf->d.os.muutos & 0x100)
		   kilp.ostiet[os].laina[1] = ost->laina[1];
	   if (ibuf->d.os.muutos & 0x200)
		   memcpy(kilp.ostiet[os].selitys, ost->selitys, sizeof(ost->selitys));
	   if (ibuf->d.os.muutos & 0x400)
		   kilp.ostiet[os].uusi = ost->uusi;
	   if (ibuf->d.os.muutos & 0x800)
		   kilp.ostiet[os].seuranta = ost->seuranta;
//	   if (ibuf->d.os.muutos & 0x1000)
//		   kilp.ostiet[os].kone = ost->kone;
	   if (ibuf->d.os.muutos & 0x4000)
		   kilp.ostiet[os].ylahto = ost->ylahto;
	   if (ibuf->d.os.muutos & 0x8000)
		   kilp.ostiet[os].lahtolaji = ost->lahtolaji;
	   if (ibuf->d.os.muutos & 0x10000)
		   kilp.ostiet[os].keskhyl = ost->keskhyl;
	   if (ibuf->d.os.muutos & 0x20000)
		   memcpy(kilp.ostiet[os].ampsakot, ost->ampsakot, sizeof(ost->ampsakot));
	   if (ibuf->d.os.muutos & 0x40000)
		   kilp.ostiet[os].sakko = ost->sakko;
		tallenna(&kilp,d,ibuf->d.os.tarf, cn + 1,
			(!katkkierto[cn] && ibuf->d.os.pakota) ? 10000*ibuf->d.os.osuus : 0, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
   if (er) {
#ifndef LANGSV_K
	  writeerror(
		 "Tiedostot eri koneilla eivät ole yhtäpitäviä",0);
	  writeerror("Poistu ohjelmasta, kopioi KILP.DAT"
		 "lähettävältä tälle koneelle",0);
#else // LANGSV_K
	  writeerror(
		 "Okonsistent data mellan datorer",0);
	  writeerror("Avsluta program, kopiera KILP.DAT"
		 "fr†n den sändande dator till denna dator",0);
#endif // LANGSV_K
	  }
   }

static void tark_trk(int cn)
   {
   int  er = 0;
   INT  d;
   combufrec *ibuf;
   kilptietue kilp;

//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   d = ibuf->d.trk.dk;
	EnterCriticalSection(&tall_CriticalSection);
   GETREC(&kilp, d);
   if (kilp.kilpstatus != 0 || ibuf->d.trk.kno != kilp.kilpno) {
	   er = 1;
	   }
   else {
	   kilp.ostiet[ibuf->d.trk.osuus - 1].sakko = ibuf->d.trk.sakko;
	   memcpy(kilp.ostiet[ibuf->d.trk.osuus - 1].ampsakot, ibuf->d.trk.ampsakot,
			sizeof(kilp.ostiet[0].ampsakot));
	   if (kuuluttaja && ibuf->d.trk.keskhyl == L'I' &&
			wcswcind(kilp.wTark(ibuf->d.trk.osuus - 1), L"KH") >= 0)
			return;
	   kilp.SetTark(ibuf->d.trk.osuus - 1, ibuf->d.trk.keskhyl);
	   tallenna(&kilp,d,ibuf->d.trk.tarf, cn + 1,
			(!katkkierto[cn] && ibuf->d.trk.pakota) ? (10000*ibuf->d.p.osuus) : 0, 0);
	   }
	LeaveCriticalSection(&tall_CriticalSection);
   if (er) {
#ifndef LANGSV_K
	  writeerror(
		 "Tiedostot eri koneilla eivät ole yhtäpitäviä",0);
	  writeerror("Poistu ohjelmasta, kopioi KILP.DAT"
		 "lähettävältä tälle koneelle",0);
#else // LANGSV_K
	  writeerror(
		 "Okonsistent data mellan datorer",0);
	  writeerror("Avsluta program, kopiera KILP.DAT"
		 "fr†n den sändande dator till denna dator",0);
#endif // LANGSV_K
	  }
   }

static void tark_tls(int cn)
   {
   int  er = 0;
   INT  d;
   combufrec *ibuf;
   kilptietue kilp;
   vatp *va;

//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   d = ibuf->d.p.dk;
	EnterCriticalSection(&tall_CriticalSection);
   GETREC(&kilp, d);
   if (kilp.kilpstatus != 0 || ibuf->d.p.kno != kilp.kilpno) {
	   er = 1;
	   }
   else {
		va = (vatp *) ibuf->d.p.cva;
		kilp.setMaali(ibuf->d.p.osuus - 1, ibuf->d.p.piste, va->aika);
		tallenna(&kilp,d,ibuf->d.k.tarf, cn + 1,
			(!katkkierto[cn] && ibuf->d.p.pakota) ? (10000*ibuf->d.p.osuus+ibuf->d.p.piste+1) : 0, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
   if (er) {
#ifndef LANGSV_K
	  writeerror(
		 "Tiedostot eri koneilla eivät ole yhtäpitäviä",0);
	  writeerror("Poistu ohjelmasta, kopioi KILP.DAT"
		 "lähettävältä tälle koneelle",0);
#else // LANGSV_K
	  writeerror(
		 "Okonsistent data mellan datorer",0);
	  writeerror("Avsluta program, kopiera KILP.DAT"
		 "fr†n den sändande dator till denna dator",0);
#endif // LANGSV_K
	  }
   }

#if defined(EMITLEIMAT) && defined(MAALI) && !defined(LUENTA)
void lahkvaihto(INT ekno, INT eos, INT kno, INT os, INT kielto)
   {
   INT  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < MAX_LAHPORTTI; cn++) {
	  if (tarkkielto(cn, kielto) || !com_emit[cn] || com_ajat[cn] == 2)
		  continue;
	  obuf.pkgclass = VEMITT;
	  obuf.len = class_len[VEMITT];
	  obuf.d.ev.ekno = ekno;
	  obuf.d.ev.eos = eos;
	  obuf.d.ev.kno = kno;
	  obuf.d.ev.os = os;
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
   }

static void tark_vemit(INT cn)
   {
   combufrec *ibuf;

   if (!com_emit[cn]) return;
//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   tallkvaihto(ibuf->d.ev.ekno, ibuf->d.ev.eos, ibuf->d.ev.kno,
      ibuf->d.ev.os, cn+1);
   }

void laheta_em(emittp *em, int rajaus, INT kielto)
   {
   INT  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2 || com_emit[cn] <= 0
		  || (rajaus && com_emit[cn] == 3))
		  continue;
	  obuf.pkgclass = EMITT;
	  obuf.len = class_len[EMITT];
	  memmove(&obuf.d.em, em, sizeof(emittp));
	  buflah(cn, &obuf);
	  }
#if !defined(LUENTA)
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2 || com_emitva[cn] <= 0)
		  continue;
	  obuf.pkgclass = EMITVA;
	  obuf.len = class_len[EMITVA];
		if (!tee_emva(&obuf.d.emva, em)) {
			buflah(cn, &obuf);
			}
	  }
#endif
	LeaveCriticalSection(&outb_CriticalSection);
	}

static void tark_emit(INT cn)
   {
   combufrec *ibuf;

   if (!com_emit[cn] || com_emit[cn] > 1) return;
//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
   addem(&ibuf->d.em, cn+1);
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
	obuf->d.alku.vaihe = 0;
	obuf->d.alku.nrec = datf2.numrec;
	memcpy(obuf->d.alku.konetunn, konetunn, 2);
	obuf->d.alku.flags = flags;
}

bool tark_alku(const combufrec * const cbuf, int cn)
{
	static int var[MAX_LAHPORTTI];
	wchar_t ln[84] = L"";

	if (cbuf->d.alku.tunn != 1)
		return(true);
	if (cbuf->len == 3)
		return(false);
	if (cbuf->d.alku.vaihe != 0) {
		if (var[cn] == 0) {
			swprintf(ln, L"Henkilökohtainsen kilpailun ohjelma pyytää yhteyttä", cn+1);
			writeerror_w(ln, 0);
			}
		var[cn]++;
		return(true);
		}
	if (cbuf->d.alku.nrec != datf2.numrec) {
		if (var[cn] == 0) {
			swprintf(ln, L"Yhteyspyyntö koneelta, jonka KILP.DAT eri pituinen, yhteys %d", cn + 1);
			writeerror_w(ln, 0);
			}
		var[cn]++;
//		return(true);
		}
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

#if defined(__BORLANDC__)
#pragma warn -par
#endif

void tarkcom(LPVOID lpCn)
	{
    int  nj,cn, lisa, inb;
	 combufrec *ibuf;

    while (comfl) {
        if (inbens == inbseur)
           Sleep(15);
//           Sleep(50);
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
//				   ibuf = (combufrec *)((char *)inbuf + inb*combufsize);
			   ibuf = inbuf + inb;
			   cn = ibuf->portti-1;
			   switch (ibuf->pkgclass) {
                  case ALKUT:
#ifdef VERTKELLO
					 tark_kello(cn);
#endif
					 break;
				  case KILPT:
					 tark_kilp(cn);
					 break;
				  case OSUUST:
					 tark_osuus(cn);
					 break;
				  case TARKT:
					 tark_trk(cn);
					 break;
				  case VAIN_TULOST:
					 tark_tls(cn);
					 break;
				  case AIKAT:
#if defined(MAALI) && !defined(LUENTA)
					 tark_aika(cn);
#else
#ifndef LANGSV_K
					 writeerror("Vastaanotettu MAALI-ohjelman ajanottotieto."
						" Korjaa konfigurointi", 0);
#else // LANGSV_K
					 writeerror("Programmet MAALI har sänt tidtagningsdata."
						" Korrigera konfiguration", 0);
#endif // LANGSV_K
#endif
					 break;
				  case YHTEISLAHTOT:
					 tark_ylahto(cn);
					 break;
				  case EMITT:
#if defined(EMITLEIMAT) && defined(MAALI) && !defined(LUENTA)
					 tark_emit(cn);
#endif
					 break;
				  case EMITVA:
#if defined(EMITLEIMAT) && defined(MAALI) && !defined(LUENTA)
					 tark_emitva(cn);
#endif
					 break;
				  case VEMITT:
#if defined(EMITLEIMAT) && defined(MAALI) && !defined(LUENTA)
					 tark_vemit(cn);
#endif
                     break;
                  case EXTRA:
                     tark_extra(cn);
                     break;
				  case FILESEND:
					 tark_tiedosto(cn);
					 break;
                  default :
                     writeerror("Tuntematon sanomalaji, ohjelmat eivät sovi yhteen", 1000);
                     lisa = 0;
                     break;
				  }
               EnterCriticalSection(&inb_CriticalSection);
               ibuf->lahetetty = 1;
#ifdef DBGXX
               sprintf(dflb+strlen(dflb),"%ld:%ld:%d:%d<T>", inb, inbseur, (int)ibuf->id, (int)ibuf->d.v.entno);
#endif
               inbens = (inb + lisa) % INBUFL;
               LeaveCriticalSection(&inb_CriticalSection);
			   if (status_on) {
                  nj = (inbseur + INBUFL - inbens) % INBUFL;
				  vidint(18, 75, 5, nj);
                  }
               }
           }
        if( autoclose && (nj <= INBUFL / 2) ) {
            autoclose = 0;
			for (cn = 0; cn < maxyhteys; cn++)
				vastcom[cn] = vastcom0[cn];
      		writeerror("Puskurissa taas tilaa, portti avataan",1000);
	      	}
		}
    }
#if defined(__BORLANDC__)
#pragma warn +par
#endif


#ifdef TCPSIIRTO

void merk_uusinta(int cna, int cny, long ta, long ty, int autouusinta);

void lahetakaikkiTCP(int cn)
   {
   int kno, srj, os, vali, len, dk, nsent = -1;
   char buf[200], st[80], st1[80];
   kilptietue kilp;

   if (!comopen[cn] || !lahcom[cn])
      return;
   for (srj = 0; srj < sarjaluku; srj++) {
      for (os = 0; os < Sarjat[srj].osuusluku; os++) {
         for (vali = 0; vali <= kilpparam.maxvaluku; vali++) {
//            if (vali && !com_valiajat[cn]) break;
			if (ntulos[srj][os][vali]) {
			   EnterCriticalSection(&tall_CriticalSection);
               merk_sijat(srj, os, vali, 0);
               LeaveCriticalSection(&tall_CriticalSection);
               }
            }
		 }
      }
   for (kno = minkilpno; kno <= maxkilpno; kno++) {
      if ((dk = getpos(kno)) <= 0)
         continue;
      GETREC(&kilp, dk);
	  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		 for (vali = 1; vali <= kilpparam.maxvaluku+1; vali++) {
			if (vali <= kilpparam.maxvaluku &&
			   !tulos(&kilp, os, vali))
//               (!com_valiajat[cn] || !tulos(&kilp, os, vali)))
			   continue;
			if (vali > kilpparam.maxvaluku)
				vali = 0;
			sprintf(buf, "%s:%d:%d:%d:%s:%d:%s:%s:%ld:%ld:%c:%s\n", Sarjat[kilp.sarja].sarjanimi, kilp.kilpno,
				os+1, vali%kilpparam.valuku, kilp.Seura(st1), kilp.joukkue, kilp.maa, kilp.Nimi(st, 70, os),
			   tulos(&kilp, os, vali%kilpparam.valuku), osuustulos(&kilp, os, vali%kilpparam.valuku),
			   kilp.Tark(os, 1), kilp.ostiet[os].hajonta);
			len = strlen(buf);
			if (len) {
			   EnterCriticalSection(&tcp_CriticalSection[cn]);
			   wrt_st_x(cn, len, buf, &nsent);
			   LeaveCriticalSection(&tcp_CriticalSection[cn]);
			   }
			if (vali == 0)
				break;
			}
		 }
	  }
   }

void lahetaXML_TCP(combufrec *obuf, int cn)
   {
	int os, vali, len = 0, dk = 0, nsent = -1, nsent_tot;
	__int64 pos;
	long t, cje;
	char buf[8000], hajonta[HAJONTA+1], *p;
	kilptietue kilp;
	char nmstr[200], st[80];
	int yos, i;
	TextFl *tmpfile = NULL;

	if (lah_alku[cn]) {
//      autofile();
		lah_alku[cn] = 0;
		lahfl[cn] = FALSE;
		return;
		}

	if (tcpkaynnistys[(int)hComm[cn]]) {
		merk_uusinta(cn, cn, 0, 0, 1);
		tcpkaynnistys[(int)hComm[cn]] = 0;
		}
	cje = cjens[cn];

	if (obuf->pkgclass == VAIN_TULOST ||
		obuf->pkgclass == TARKT || obuf->pkgclass == OSUUST) {
		dk = obuf->d.p.dk;
		os = obuf->d.p.osuus;
		if (obuf->pkgclass == VAIN_TULOST)
			vali = obuf->d.p.piste;
		else
			vali = 0;
		if (dk && os > 0) {
			GETREC(&kilp, dk);
			memset(buf, 0, sizeof(buf));
			if (XMLtietue) {
				tmpfile = new TextFl(autofileparam.sanafname, L"w+t");
				if (tmpfile->IsOpen()) {
					kirjXmlOoOsuus(tmpfile, kilp, os-1, vali, -1);
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
				memset(hajonta, 0, sizeof(hajonta));
				if (lahhajonta)
					strncpy(hajonta, kilp.ostiet[os-1].hajonta, HAJONTA);
				else
					hajonta[0] = ' ';
/*
	  buf[0] = STX;
	  len = xmlsanoma(buf+1, sizeof(buf)-3, &kilp, dk, piste);
	  if (len) {
		 buf[len+1] = ETX;
		 EnterCriticalSection(&tcp_CriticalSection);
		 wrt_st_x(cn, len+2, buf, &nsent);
		 LeaveCriticalSection(&tcp_CriticalSection);
		 }
*/
//         sprintf(buf, "%s:%d:%d:%d:%s:%d:%s:%s:%ld:%d\n", Sarjat[kilp.sarja].sarjanimi, kilp.kilpno,
//            os, vali, kilp.seura, kilp.joukkue, kilp.maa, kilp.ostiet[os-1].nimi, tulos(&kilp, os-1, vali), kilp.ostiet[os-1].sija[vali]);
				if (kilpparam.maxnosuus > 1) {
					int tls;
					yos = Sarjat[kilp.sarja].yosuus[os-1];
					buf[0] = 0;
					if ((tls = kilp.tTulos(yos, vali)) > 5*SEK || stschind(kilp.tTark(yos), "KH") >= 0) {
						int ostls;
						ostls = tls;
						if (stschind(kilp.tTark(yos), "KH") >= 0) {
							tls = 0;
							ostls = 0;
							}
						else if (yos > 0) {
							ostls -= kilp.tTulos(yos-1, 0);
							}
						if (Sarjat[kilp.sarja].nosuus[yos] == 1)
							kilp.Nimi(nmstr, OSNIMIL, os-1);
						else {
							len = Sarjat[kilp.sarja].nosuus[yos]-1;
							for (i = Sarjat[kilp.sarja].aosuus[yos]; i < Sarjat[kilp.sarja].aosuus[yos+1]; i++)
								len += strlen(kilp.ostiet[i+1].nimi);
							if (len > 60)
								len = (len-61+Sarjat[kilp.sarja].nosuus[yos])/Sarjat[kilp.sarja].nosuus[yos];
							else
								len = 0;
							memset(nmstr, 0, sizeof(nmstr));
							for (i = Sarjat[kilp.sarja].aosuus[yos]+1; i <= Sarjat[kilp.sarja].aosuus[yos+1]; i++) {
								kilp.Nimi(nmstr+strlen(nmstr), strlen(kilp.ostiet[i].nimi)-len, i);
								if (i < Sarjat[kilp.sarja].aosuus[yos+1])
								strcat(nmstr, "-");
								}
							}
						sprintf(buf, "%s:%d:%d:%d:%s:%d:%s:%s:%ld:%ld:%c:%s\n", Sarjat[kilp.sarja].sarjanimi, kilp.kilpno, yos+1, vali, kilp.Seura(st),
							kilp.joukkue, kilp.maa, nmstr, tls/KSEK, ostls/KSEK, kilp.tTark(yos), hajonta);
						}
					}
				else {
					sprintf(buf, "%s:%d:%d:%d:%s:%d:%s:%s:%ld:%ld:%c:%s\n", Sarjat[kilp.sarja].sarjanimi, kilp.kilpno, os, vali, kilp.Seura(st),
						kilp.joukkue, kilp.maa, kilp.Nimi(nmstr, OSNIMIL, os-1), tulos(&kilp, os-1, vali), osuustulos(&kilp, os-1, vali),
						kilp.Tark(os-1, 1), hajonta);
					}
				len = strlen(buf);
				}
			}
		}
	nsent_tot = 0;
	if (len) {
		while (comfl && nsent_tot < len) {
			int sanlen = 1000;
			EnterCriticalSection(&tcp_CriticalSection[cn]);
			if (sanlen > len-nsent_tot)
				sanlen = len-nsent_tot;
			wrt_st_x(cn, sanlen, buf, &nsent);
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
			pos = (cje*maxyhteys+cn)*(__int64)combufsize;
			pos = _lseeki64(obfile, pos ,SEEK_SET);
			if (pos > 0) {
				t = T_TIME(biostime(0, 0));
				if (t == 0) t = 1;
				if (_write(obfile, &t, 4) < 4)
				   writeerror_w(L"Virhe rutiinissa 'lahetaXML_TCP', koodi 3", 0);
				}
			cjens[cn] = cje;
			}
		else
			cjens[cn] = cje % OUTBUFL;
		LeaveCriticalSection(&outb_CriticalSection);
//	  nj = (int) (cjseur[cn] - cjens[cn]);
//	  if (!levypusk) nj = (nj + OUTBUFL) % OUTBUFL;
		}
	lahfl[cn] = FALSE;
}

#endif  // TCPSIIRTO

void init_class_len(void)
   {
   combufrec cbrec;

   class_len[ALKUT] = 10;
   class_len[KILPT] = kilpparam.kilprecsize0 + 6;
   class_len[OSUUST] = kilpparam.osrecsize0 + 12;
   class_len[TARKT] = sizeof(cbrec.d.trk);
   class_len[AIKAT] = sizeof(cbrec.d.a);
   class_len[VAIN_TULOST] = sizeof(cbrec.d.p);
   class_len[YHTEISLAHTOT] = 8 * kilpparam.n_os + 4;
#if defined(EMIT)
   class_len[EMITT] = sizeof(emittp);
   class_len[EMITVA] = sizeof(emitvatp);
   class_len[VEMITT] = 8;
#endif
   class_len[EXTRA] = sizeof(cbrec.d.x);
   combufsize = sizeof(combufrec);
#if defined(EMIT)
	if (maxemvalah > MAXEMVALAH) {
		class_len[EMITVA] += 4*(maxemvalah-MAXEMVALAH);
		if (class_len[EMITVA] > class_len[EMITT])
			combufsize += class_len[EMITVA] - class_len[EMITT];
		}
#endif
	combufsize = 0;
	for (int i = 0; i < N_CLASS; i++)
		if (combufsize < class_len[i])
			combufsize = class_len[i];
	class_len[FILESEND] = combufsize - 14;
   }

#ifndef LUENTA
void laheta_emva_yht(emittp *em, int cn)
   {
   combufrec obuf;

   if (!comopen[cn] || !lahcom[cn] || com_emitva[cn] <= 0)
	  return;
   obuf.pkgclass = EMITVA;
   obuf.len = class_len[EMITVA];
	if (!tee_emva(&obuf.d.emva, em)) {
		buflah(cn, &obuf);
	  }
   }

void emitva_uusi_sarja(int cn, int srj, int osuus)
{
	for (int i = minkilpno; i <= maxkilpno; i++) {
		int d;
		kilptietue kilp;
		emittp em;

		if ((d = getpos(i)) <= 0)
			continue;
		kilp.getrec(d);
		if (srj >= 0) {
			if (kilp.sarja != srj)
				continue;
			}
		for (int os = (osuus >= 0 ? osuus : 0); os < (osuus >= 0 ? osuus+1 : Sarjat[kilp.sarja].osuusluku); os++) {
			if (os >= Sarjat[kilp.sarja].osuusluku)
				break;
			if (getem(&em, kilp.kilpno, os) >= 0) {
				EnterCriticalSection(&outb_CriticalSection);
				laheta_emva_yht(&em, cn);
				LeaveCriticalSection(&outb_CriticalSection);
				}
			}
		}
}

void laheta_emva(kilptietue *kilp, int osuus)
{
	emittp em;
	combufrec obuf;

	if (osuus < Sarjat[kilp->sarja].osuusluku && getem(&em, kilp->kilpno, osuus) >= 0) {
		EnterCriticalSection(&outb_CriticalSection);
		for (int cn = 0; cn < maxyhteys; cn++) {
			if (!comopen[cn] || !lahcom[cn] || com_emitva[cn] <= 0)
				continue;
			obuf.pkgclass = EMITVA;
			obuf.len = class_len[EMITVA];
			if (!tee_emva(&obuf.d.emva, &em))
				buflah(cn, &obuf);
			}
		LeaveCriticalSection(&outb_CriticalSection);
		}
}
#endif

void uusintaTCPvalitut(int sarja, wchar_t cp, int alku, int loppu, int cn)
{
	INT32 tm;
	INT srj = 0, os, d, piste, kdi;
	kilptietue kilp;
	int p, lj;

	if (sarja >= 0)
		srj = sarja;
	else {
		srj = 0;
		sarja = sarjaluku - 1;
		}
	for (; srj <= sarja; srj++) {
		for (int yos = 0; yos < Sarjat[srj].ntosuus; yos++) {
			os = Sarjat[srj].aosuus[yos]+1;
			p = -1;
			lj = -1;
			if (cp == 'H')
				lj = 0;
			for (;;) {
				p = askellasarja(srj, yos, 0, 0, p, &lj);
				d = p;
				if (d < 0)
					break;
				if (cp == 'H') {
					if (lj == 0)
						continue;
					if (lj > 2)
						break;
					}
				if (cp == 'T')
					if (lj > 0)
						break;
				GETREC(&kilp, d);
				if (kilp.sarja != srj)
					break;
				for (piste = 0; piste <= Sarjat[kilp.sarja].valuku[yos]; piste++) {
					if (!piste && cp == 'V')
						continue;
					if (piste && (cp == 'T' || cp == 'H'))
						break;
					tm = kilp.Maali(yos, piste);
					if (((cp != 'H' && cp != 'K') || piste) && (tm == TMAALI0 || kilp.tTulos(yos, piste) < 5*SEK))
						continue;
					if (alku != TMAALI0 && NORMT(tm) < NORMT(alku))
						continue;
					if (loppu != TMAALI0 && NORMT(loppu) < NORMT(tm))
						continue;
					if (piste)
						kdi = 2;
					else {
						if (cp == 'H' || (cp == 'K' && kilp.tTulos(yos, piste) < 5*SEK))
							kdi |= 4;
						else
							kdi |= 2;
						}
					laheta(d, kilp.kilpno, os+1, piste, 16, -cn-1, kdi, 0);
					}
				}
			}
		}
}

#ifdef SERVER

void lahetanumerotTCP(int cn, INT32 alku, INT32 loppu)
   {
   INT32 cj;
   __int64 pos, reclen;
   combufrec ibuf;

   reclen = sizeof(combufrec);
   if (alku >= 0 && alku < cjens[cn] && loppu >= alku) {
	  if (loppu >= cjens[cn])
		 loppu = cjens[cn]-1;
	  for (cj = alku; cj <= loppu; cj++) {
		 EnterCriticalSection(&outb_CriticalSection);
		 pos = _lseeki64(obfile, ((cj+1)*maxyhteys+cn)*reclen,
			SEEK_SET);
		 if (pos >= 0)
			 if (_read(obfile, &ibuf, sizeof(combufrec)) < sizeof(combufrec))
			   writeerror_w(L"Virhe rutiinissa 'lahetanumerotTCP', koodi 1", 0);
		 LeaveCriticalSection(&outb_CriticalSection);
		 if (pos < 0) {
			writeerror_w(L"Virhe rutiinissa 'lahetanumerotTCP', koodi 2", 0);
			return;
			}
		 if (ibuf.pkgclass == VAIN_TULOST || ibuf.pkgclass == KILPT  || ibuf.pkgclass == OSUUST ||
			 ibuf.pkgclass == TARKT) {
			 UINT32 luokka = 0;

			 if (ibuf.pkgclass == OSUUST)
				 luokka = 4 | 16*ibuf.d.os.muutos;
			 if (ibuf.pkgclass == VAIN_TULOST)
				 luokka = 2;
			 laheta(ibuf.d.k.dk, ibuf.d.k.entno, ibuf.pkgclass == KILPT ? 0 : ibuf.d.p.osuus,
				ibuf.d.p.piste, 16, -cn-1, luokka, 0);
			 }
		 }
	  }
   }

void uusintaTCP(int cn)
   {
   char ch = ' ', cs = ' ', cp = 'T';
   INT32 alku = TMAALI0, loppu = TMAALI0;
   INT srj = 0;

   selectopt("K)aikki sarjat, V)alitse sarjat, S)anomanumerot", "VKS", &cs);
   if (cs != 'S') {
	  selectopt("T)ulokset, V)äliajat, M)olemmat, H)yl+kesk+avoimet, K)aikki", "TVMHK", &cp);
	  selectopt("K)oko kilpailu, V)alittava aikavali, Esc: peruuta", "KV\x1b", &ch);
	  if (ch == 'V') {
		 clrln(ySize-3);
		 vidspmsg(ySize-3, 0, 7, 0, "Aikarajat: alku          loppu           Esc: peruuta");
		 INPUTAIKA(&alku,t0,8,16, ySize-3,"\r\x1b",&ch);
		 if (ch == ESC)
			return;
		 INPUTAIKA(&loppu,t0,8,31, ySize-3,"\r\x1b",&ch);
		 }
	  if (ch == ESC)
		 return;
	  do {
		 if (cs == 'V') {
			vidspmsg(ySize-3,0,7,0,"    Valitse sarja :               <Esc>: lopeta");
			srj = luesarja("", &ch);
			if (ch == ESC || srj < 0)
			   break;
			uusintaTCPvalitut(srj, cp, alku, loppu, cn);
			}
		 } while(cs == 'V' || ++srj < sarjaluku);
	  }
   if (cs == 'S' && levypusk) {
      clrln(ySize-3);
      vidspmsg(ySize-3, 0, 7, 0, "Numeroalue: alku         loppu           Esc: peruuta");
	  alku = 0;
      INPUTINT(&alku,6,17, ySize-3,"\r\x1b",&ch);
      if (ch == ESC)
         return;
	  loppu = alku;
      INPUTINT(&loppu,6,31, ySize-3,"\r\x1b",&ch);
      if (ch == ESC)
		 return;
	  lahetanumerotTCP(cn, alku, loppu);
	  }
   }

//#if defined(EMIT) && !defined(ESITARK)
#if defined(EMIT) && !defined(LUENTA)
void emitva_uusinta(int cn, int tietue)
	{
	char ch, tc, snimi[12] = "";
	INT16 srj = -1, osuus = -1;
	emittp em;

	if (cn < 0 && tietue >= 0) {
		lueem(&em, tietue);
		for (cn = 0; cn < maxyhteys; cn++) {
			if (com_emitva[cn] && comopen[cn] && lahcom[cn]) {
				EnterCriticalSection(&outb_CriticalSection);
				laheta_emva_yht(&em, cn);
				LeaveCriticalSection(&outb_CriticalSection);
				}
			}
		if (cn >= maxyhteys)
			return;
		}
	if (tietue < 0 && cn >= 0) {
		while (1) {
			ch = ' ';
			selectopt("Lähetä K)aikki tai valitse S)arja, O)suus, M)olemmat, Esc: lopeta", "KSOM\x1b", &ch);
			if (ch == ESC)
				break;
			if (ch == 'S' || ch == 'M') {
				if (srj >= 0)
					strcpy(snimi, Sarjat[srj].sarjanimi);
				srj = luesarja(snimi, &tc);
				if (tc == ESC)
					break;
				}
			else
				srj = -1;
			if (ch == 'M' || ch == 'O') {
				clrln(ySize-3);
				vidspmsg(ySize-3, 0, 7, 0, "Anna osuus");
				osuus++;
				inputint(&osuus, 2, 12, ySize-3, "\r\x1b", &tc);
				if (tc == ESC)
					break;
				osuus--;
				}
			else
				osuus = -1;
			emitva_uusi_sarja(cn, srj, osuus);
			}
		}
	}
#endif

#endif

#ifdef VALUKIJA

void laheta_ec(UINT32 badge, int piste, INT32 aika, int kielto)
   {
   int  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (lahec[cn] == 0 || (tarkkielto(cn, kielto) & 7) != 0 || com_ajat[cn] == 2)
		  continue;
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
#endif

void lahetasulku(char *tn, INT kielto)
	{
   INT  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2)
		  continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 9;
		obuf.d.x.d2 = 0;
		memcpy(&obuf.d.x.d2, tn, 2);
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
	}

void lahetaKnoSiirto(int sarja, int siirto, int kielto)
	{
   INT  cn ;
   combufrec obuf;

	EnterCriticalSection(&outb_CriticalSection);
   for (cn = 0; cn < maxyhteys; cn++) {
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2)
		  continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
		obuf.d.x.d1 = 3;
		obuf.d.x.d2 = sarja;
		obuf.d.x.d3 = siirto;
		obuf.d.x.d4 = 0;
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
	  if (tarkkielto(cn, kielto) || com_ajat[cn] == 2)
		  continue;
	  obuf.pkgclass = EXTRA;
	  obuf.len = class_len[EXTRA];
	  obuf.d.x.d1 = 7;
	  obuf.d.x.d2 = 0;
	  buflah(cn, &obuf);
	  }
	LeaveCriticalSection(&outb_CriticalSection);
	}

void lahetakello(int cn, INT kielto)
	{
	combufrec obuf;

	if (!comopen[cn] || !lahcom[cn] || !lahkello[cn] ||
		cn == kielto - 1 || (kielto && estavalitys[cn]))
		return;
	obuf.pkgclass = EXTRA;
	obuf.len = class_len[EXTRA];
	obuf.d.x.d1 = 4;
	obuf.d.x.d2 = KSEK*t_time_l(biostime(0, 0), t0);
	EnterCriticalSection(&outb_CriticalSection);
	buflah(cn, &obuf);
	LeaveCriticalSection(&outb_CriticalSection);
	}

void kaynnistatesti(int kielto);

static void tark_extra(int cn)
   {
   combufrec *ibuf;
	char tn[3];

   if (!comopen[cn]) return;
//   ibuf = (combufrec *)((char *)inbuf + inbens*combufsize);
   ibuf = inbuf + inbens;
	switch (ibuf->d.x.d1) {
#ifdef VALUKIJA
		case 1:
			tall_ec(ibuf->d.x.d3, ibuf->d.x.d4, ibuf->d.x.d2, cn+1);
			break;
#endif
		case 3:
			tallKnoSiirto(ibuf->d.x.d2, ibuf->d.x.d3, cn+1);
			break;
		case 7:
			kaynnistatesti(cn+1);
			break;
		case 9:
			memcpy(tn, &ibuf->d.x.d2, 2);
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
			&& !(obuf.pkgclass == VEMITT && !com_emit[cc])
			&& !(obuf.pkgclass == EMITVA && com_emit[cc] <= 0)
			&& !(obuf.pkgclass == EXTRA && (obuf.d.x.d1 == 4 || obuf.d.x.d1 == 9))
			) {
			EnterCriticalSection(&outb_CriticalSection);
			buflah(cc, &obuf);
			LeaveCriticalSection(&outb_CriticalSection);
			}
		}
}

