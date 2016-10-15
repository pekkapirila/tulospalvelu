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
#include <string.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bvideo.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "VDeclare.h"

#define EMIT1 2

static int tulkrivi(char *line, int *srj, int *kno, int *jno);

extern int writeerrorOn;
static int korjtarfl;
static char erotin = ',', jjline[MAXOSUUSLUKU+1][80];
static FILE *efile;

static int pkno = 0;
//extern int tall_osuus;

#define  VAVAH  1
static int ma_piste, ma_os;
static kilptietue *ma_kilp;

static wchar_t tulk(wchar_t ch, int key)
   {
//	if (ch == 1 && key == 30)    // Ctl-A
//		return(247);
	if (ch == 4 && key == 32)    // Ctl-D
		return(249);
	if (ch == 5 && key == 18)    // Ctl-E
		return(231);
	if (ch == 6 && key == 33)    // Ctl-F
		return(247);
	if (ch == 7 && key == 34)    // Ctl-G
		return(243);
	if (ch == 8 && key == 35)    // Ctl-H
		return(234);
	if (ch == 9 && key == 23)    // Ctl-I
		return(253);
	if (ch == 10 && key == 36)    // Ctl-J
		return(246);
	if (ch == 11 && key == 37)    // Ctl-K
		return(251);
	if (ch == 12 && key == 38)    // Ctl-L
		return(252);
	if (ch == 14 && key == 49)    // Ctl-N
		return(233);
	if (ch == 15 && key == 24)    // Ctl-O
		return(239);
	if (ch == 16 && key == 25)    // Ctl-P
		return(241);
	if (ch == 17 && key == 16)    // Ctl-Q
		return(263);
	if (ch == 18 && key == 19)    // Ctl-R
		return(235);
	if (ch == 19 && key == 31)    // Ctl-S
		return(232);
	if (ch == 20 && key == 20)    // Ctl-T
		return(262);
	if (ch == 21 && key == 22)    // Ctl-U
		return(240);
	if (ch == 22 && key == 47)    // Ctl-V
		return(242);
	if (ch == 23 && key == 17)    // Ctl-W
		return(250);
	if (ch == 24 && key == 45)    // Ctl-X
		return(245);
	if (ch == 25 && key == 21)    // Ctl-Y
		return(248);
	if (ch == 26 && key == 44)    // Ctl-Z
		return(261);
	if (ch) return(towupper(ch));
   switch (key) {
      case 15 : ch = 11; break;         // backtab 
      case 59 :                         // F1  ch = 201
      case 60 :                         // F2  ch = 202
      case 61 :                         // F3  ch = 203
      case 62 :                         // F4  ch = 204
      case 64 :                         // F6  ch = 206
      case 65 :                         // F7  ch = 207
      case 66 :                         // F8  ch = 208
      case 68 : ch = key + 142; break;  // F10 ch = 210
      case 109:                         // Alt-F6 
      case 111:                         // Alt-F8 
                ch = key + 127; break;  // ch == 236 tai 238
      case 32 : ch = 249; break;        // Alt-D 
      case 18 : ch = 231; break;        // Alt-E 
      case 31 : ch = 232; break;        // Alt-S 
      case 49 : ch = 233; break;        // Alt-N 
      case 35 : ch = 234; break;        // Alt-H 
      case 19 : ch = 235; break;        // Alt-R 
      case 24 : ch = 239; break;        // Alt-O 
      case 22 : ch = 240; break;        // Alt-U 
      case 25 : ch = 241; break;        // Alt-P 
	  case 47 : ch = 242; break;        // Alt-V
      case 34 : ch = 243; break;        // Alt-G 
      case 129: ch = 244; break;        // Alt-0 
      case 45 : ch = 245; break;        // Alt-X 
      case 36 : ch = 246; break;        // Alt-J 
      case 30 : ch = 247; break;        // Alt-A
      case 21 : ch = 248; break;        // Alt-Y 
      case 37 : ch = 251; break;        // Alt-K 
      case 38 : ch = 252; break;        // Alt-L 
      case 23 : ch = 253; break;        // Alt-I 
      case 63 :                         // F5  
      case 72 : ch = 5;
                break;                  // up
	  case 67 :                         // F9
      case 80 : ch = 24;
                break;                  // down
      case 73 : ch = 18; break;         // pgup
      case 81 : ch = 3; break;          // pgdn
      case 118: ch = 2; break;          // Ctrl-PgDn
      case 132: ch = 19; break;         // Ctrl-PgUp
      }
   return(ch);
   }

static void outform(void)
   {
   viwrrect(2,0,ySize-5,64,form,7,0,0);
   lukumaarat();
   }

void outkilp(kilptietue *kilp, int osuus)
   {
   int os, fore, back, len, osr;
   char tarkstr[5]="", sjstr[5], prs[100], st[5];
   char strtmaali[20], strtulos[20], strostls[20], as[20];
   wchar_t wst[80];
   TULOS tls;
   kilptietue kilp1;
   char *snimi;
   int va = 0;
   __int64 puutelisa;
   int vax, vay;
   int minos, maxos;

   maxos = max(osuus, (ySize-OSY-5)/osd);
   minos = maxos - (ySize - OSY - 5)/osd;
   if (minos < 0)
	   minos = 0;

   kilp1 = *kilp;
   if (Sarjat[kilp1.sarja].maxnosuus == 1)
		merk_kilp_yl(&kilp1, osuus);
   else
		merk_yl(&kilp1);
   outform();
   if ((kilp1.sarja < sarjaluku))
	  snimi = (char *)Sarjat[kilp1.sarja].sarjanimi;
   else snimi = "";
   vidspmsg(SAY,SAX,7,0,snimi);
	if (kilpparam.maxnosuus == 1) {
	   if (tarkfl)
		  sprintf(prs,"%4d-%1d",kilp1.kilpno,tarkno(kilp1.kilpno));
	   else
		  sprintf(prs,"  %4d",kilp1.kilpno);
	   }
	else
		sprintf(prs,"  %4d-%-2d",kilp1.kilpno,osuus+1);
   vidspmsg(COY,COX,7,0,kilp1.maa);
   vidspmsg(NOY,NOX,7,0,prs);
   vidspwmsg(SEY,SEX,7,0,kilp1.Seura(wst));
   if (kilp1.joukkue) vidint(JOY,JOX,0,kilp1.joukkue);
   viwrrect(YAY,YAX-19,YAY,YAX-2,"Sarjan lähtöaika :",7,0,0);
   viwrrect(YAY,YAX,YAY,YAX+7,
	 AIKATOSTRS(as, Sarjat[kilp1.sarja].lahto,t0),7,0,0);
   if (kilpparam.rogaining) {
	  vidspmsg(MAY, MAX-28, 7, 0, "Pisteet:");
	  vidint(MAY, MAX-19, 5, kilp1.pisteet[0]);
	  }
   for (os = 0; os < Sarjat[kilp1.sarja].osuusluku; os++) {
      if (os < minos)
         continue;
      if (os > maxos)
         break;
      osr = OSY + osd * (os - minos);
//      vidint(osr, 0, 3, os+1);
	  wosuuskoodi(kilp1.sarja, os, wst, 3);
	  vidspwmsg(osr, 0, 7, 0, wst);
      EnterCriticalSection(&tall_CriticalSection);
	if (kilpparam.maxnosuus == 1) 
		haesija(&kilp1, os, 0, 0, 0);
	else if (Sarjat[kilp1.sarja].aosuus[Sarjat[kilp1.sarja].yosuus[os]+1] == os)
         haesija(&kilp1, Sarjat[kilp1.sarja].yosuus[os], 0, 0, 0);
      LeaveCriticalSection(&tall_CriticalSection);
      sjstr[0] = ' ';
      sjstr[1] = 0;
      switch (kilp1.Tark(os, 0)) {
         case 'K' : memcpy(tarkstr, "Kes", 4); break;
         case 'H' : memcpy(tarkstr, "Hyl", 4); break;
         case 'E' : memcpy(tarkstr, "Eil", 4); break;
         case 'S' : memcpy(tarkstr, "Slj", 4); break;
         case 'I' : memcpy(tarkstr, "esI", 4); break;
         case 'T' : memcpy(tarkstr, "Trk", 4); break;
         case 'P' : memcpy(tarkstr, "Poi", 4); break;
         default  : memcpy(tarkstr, "  -", 4);
         }
	  if (kilp1.tTark(os) == 'S')
		  strcpy(sjstr, "Slj");
      strcpy(strtulos, "00.00.00,000");
      memcpy(strostls, strtulos, kilpparam.laika);
	  strtulos[kilpparam.laika] = 0;
      memcpy(strtmaali, strtulos, kilpparam.laika);
	  strtmaali[kilpparam.laika] = 0;
      if( kilp1.Maali(os, 0) != TMAALI0 ) {
         if (os == osuus) 
            AIKATOSTRS(strtmaali, kilp1.Maali(os, 0), t0);
         if (kilp1.Hyv(os)) {
			if (kilpparam.maxnosuus == 1) {
				tls = tulos(&kilp1,os,0);
				AIKATOSTRS(strtulos, tls, 0);
				}
			else {
				tls = 0;
				if (os == Sarjat[kilp1.sarja].aosuus[Sarjat[kilp1.sarja].yosuus[os]+1]) {
				   tls = kilp1.tTulos(Sarjat[kilp1.sarja].yosuus[os], 0, &puutelisa);
				   if (!puutelisa)
					  AIKATOSTRS(strtulos, tls, 0);
				   }
				}
            AIKATOSTRS(strostls, kilp1.osTulos(os,0), 0);
            if (tls) _itoa(kilp1.Sija(os, 0), sjstr, 10);
            }
         }
      if (os == osuus) {
         viwrrect(MAY,MAX,MAY,MAX+kilpparam.laika-1,strtmaali,7,0,0);
         fore = 0;
         back = 7;
         }
      else {
         fore = 7;
         back = 0;
         }
	  kilp1.Nimi(wst, OSNIMIL, os);
      len = min(wcslen(wst), TUX-NMX-2);
      viwrrectw(osr, NMX, osr, NMX+len-1, wst, fore, back, 0);
      if (kilpparam.badgelaji) 
         {
         vidint(osr+1-(osd==1), osd > 1 ? NMX+1 : NMX+19-HAJONTA, 7, 
            kilp1.ostiet[os].badge[0]);
		 strcpy(st, "   ");
		 st[0] = kilp1.ostiet[os].laina[0];
         vidspmsg(osr+1-(osd==1), osd > 1 ? NMX+8 : NMX+25-HAJONTA, 7, 0, st);
         vidspmsg(osr+1-(osd==1), osd > 1 ? NMX+10 : NMX+26-HAJONTA, 7, 
            0, kilp1.ostiet[os].hajonta);
         }

      if (kilpparam.maxnosuus == 1 || os == Sarjat[kilp1.sarja].aosuus[Sarjat[kilp1.sarja].yosuus[os]+1])
         {
         viwrrect(osr, TUX, osr, TUX+kilpparam.laika-1, strtulos,
            fore, back, 0);
         viwrrect(osr, SJX+3-strlen(sjstr), osr, SJX+2,
            sjstr, fore, back, 0);
         }
      viwrrect(osr, OTX, osr, OTX+kilpparam.laika-1, strostls,
         fore, back, 0);
      viwrrect(osr, KHX, osr, KHX+strlen(tarkstr)-1, tarkstr, fore, back, 0);
	  if (kilp1.ostiet[os].seuranta != 'G') {
		  vidspmsg(osr+1, 62, 7, 0, "   ");
		  }
	  else {
		  vidspmsg(osr+1, 62, 7, 0, "GPS");
		  }
#if defined(SAKOT) && defined(EMIT)
		if ((sakkosek && !ampusakot) || rogaining)
			vidint(osr+1, KHX, 3, kilp1.ostiet[os].sakko);
#endif
	  if (osd > 1 && valiajat) {
		 for (va = 1; va <= kilpparam.valuku; va++) {
				if (va > 4*osd-4)
					break;
			if (kilp1.Maali(os, va) != TMAALI0) {
//               vax = TUX+(va-(va>4)*(kilpparam.valuku-4)-VAVAH)*(OTX-TUX);
//               vay = osr+1+(va > 4);
			   vax = TUX+((va-1)%4-VAVAH)*(OTX-TUX);
			   vay = osr+1+(va-1)/4;
			   viwrrect(vay, vax, vay, vax+7,AIKATOSTRS(as, tulos(&kilp1,os,va), 0),
				  7,0,0);
#if !defined(MAALI) && VAIKALUKU < 4
			   if (kilp1.Sija(os, va) < maxnvaika)
				  vidint(osr+1, KHX + (va-1)*(SJX-KHX), 3,
					 kilp1.Sija(os, va));
#endif
			   }
			}
		 }
		if (ampusakot)
			vidspmsg(osr+ampusakot-1, YLX+4-Sarjat[kilp1.sarja].paikat[os], fore, back, kilp1.ostiet[os].ampsakot);
#ifdef EMIT
	  if(kilp1.ostiet[os].ylahto != TMAALI0) {
			if (kilp1.ostiet[os].lahtolaji > 0)
				viwrrect(osr, YLX, osr, YLX, "Y",7,0,0);
			else
				viwrrect(osr, YLX, osr, YLX, "A",7,0,0);
		 if (os == osuus) {
			viwrrect(YAY,YAX-20,YAY,YAX-2,"Yhteislähdön aika :",7,0,0);
			viwrrect(YAY,YAX,YAY,YAX+7,
			   AIKATOSTRS(as,kilp1.ostiet[os].ylahto,t0),7,0,0);
			}
		 }
#endif
	  }
   viwrrect(OSY+(osuus-minos)*osd, 0, OSY+(osuus-minos)*osd, 0, ".", 7,0,0);
   }

static int inputkilp(kilptietue *kilp, int *osuus, int addfl)
   {
   static char term[] = "+\x3\x5\x9\xB\xD\x12\x18\x1B\xCC\xCE\xD2";
   int  exfl, l,n,kno,nc,m,o1,o2,osr, p;
   char tc,ch;
   char tarkstr[11];
   static char msg[81] = "";
   TMAALI tm0;
	static TMAALI ylahto1 = -12*TUNTI;
#if MAXSARJALUKU > 1
   int  srj = 255;
   char snimi[11];
#endif
   TMAALI vaika;
   int sj0, va = 0, vax, vay;
   int maxm = 5 + kilpparam.valuku + EMIT1;
   INT32 bdg;
   wchar_t wst[100], enimi[OSNIMIL+1], wtc, ws1[40];

   tc = KB_CR;
   nc = 8;
   exfl = FALSE;
   m = 1;
   l = 2;
   if( korjfl && (! addfl) ) {
	  l = nc;
		if (!jjfl) {
			m = 3+EMIT1;
			if (ampusakot)
				m = 3+EMIT1+2*(Sarjat[kilp->sarja].paikat[*osuus] > 0);
			}
	  }
   clrln(ySize-3);
   kno = kilp->kilpno;
   do {
	   int ok;
	  viwrrect(ySize-3,0,ySize-3,74,"<Return>, Up Dn: Vaihda kenttää,  "
		 "+ : Hyväksy,  <Esc> : Peruuta ja poistu  ",7,0,0);
	  if (jjfl)
		 vidspmsg(ySize-1,0,7,0,
			"F4 : Vaihda nimet, PgUp, F6 : Edell os, PgDn, F10 : Seur os");
	  else
		 vidspmsg(ySize-1,0,7,0,
			"PgUp, F6 : Edell. osuus, PgDn, F10 : Seur. osuus");
	  switch(l) {
		 case 1 : do {
					 ok = TRUE;
					 if(kilp->kilpno <= 0) kilp->kilpno = 0;
					 vidspmsg(NOY,NOX,7,0,"      ");
					 inputint(&kilp->kilpno,4,NOX,NOY,term,&tc);
					 if((tc != ESC) && (kilp->kilpno != kno)) {
						if (getpos(kilp->kilpno) > 0) {
                           writeerror("Virheellinen numero tai numero "
                              "jo käytössä",0);
                           ok = FALSE;
                           }
                        }
                     } while (!( ok || (tc == ESC)));
				  if (kilpparam.rogaining) {
                     inputlong(&kilp->pisteet[0],5,MAX-19,MAY,term,&tc);
				     }
                  break;
         case 2 :
#if MAXSARJALUKU > 1
                  if ( kilp->sarja < sarjaluku )
                     strcpy(snimi, (char *)Sarjat[kilp->sarja].sarjanimi);
                  else strcpy(snimi, "");
                  do {
                     inputstr(snimi,10,SAX,SAY,term,&tc,FALSE);
                     if (tc == ESC)
                        break;
                     srj = haesarja(snimi, false);
                     if (srj < 0) {
                        writeerror("Virheellinen sarja",0);
                        continue;
                        }
                     kilp->sarja = srj;
                     outkilp(kilp,*osuus);
                     } while (srj < 0);
#endif
				  break;
		 case 3 : kilp->Seura(wst, kilpparam.lseura);
				  inputwstr(wst,LSEURA,SEX,SEY,oemtowcs(ws1, term, 20, 0),&wtc,FALSE);
				  tc = char(wtc);
				  elimwbl(wst);
				  kilp->setSeura(wst);
				  break;
		 case 4 :
				  inputstr(kilp->maa,3,COX,COY,term,&tc,FALSE);
				  elimbl(kilp->maa);
				  break;
		 case 5 : n = kilp->joukkue;
				  INPUTINT(&n,2,JOX,JOY,term,&tc);
				  kilp->joukkue = n;
				  break;
		 case 6 : if(kilp->Tark(*osuus, false) != 'P' && !addfl) {
					 tm0 = kilp->Maali(*osuus, 0);
					 INPUTAIKA(&tm0,t0,kilpparam.laika,MAX,MAY,term,&tc);
					 if (tm0 != Sarjat[kilp->sarja].lahto)
						kilp->setMaali(*osuus, 0, tm0);
					 else kilp->setMaali(*osuus, 0, TMAALI0);
				  }
				  break;
		 case 7 : if(kilp->ostiet[*osuus].lahtolaji > 0) {
						tm0 = kilp->ostiet[*osuus].ylahto;
						INPUTAIKA(&tm0,t0,8,YAX,YAY,term,&tc);
						if (tm0 == TMAALI0) {
							kilp->ostiet[*osuus].ylahto = TMAALI0;
							kilp->ostiet[*osuus].lahtolaji = 0;
							}
						else {
							kilp->ostiet[*osuus].ylahto = tm0;
							}
						}
                  break;
         case 8 :
                  viwrrect(ySize-3,0,ySize-3,74,"<Return> Up Dn Tab : "
                     "Vaihda kenttää, + : Hyväksy, <Esc> : Peruuta ja poistu",7,0,0);
				  osr = OSY + osd * min(*osuus, (ySize-5-OSY)/osd);
                  switch (m) {
                     case 1 :
						 kilp->Nimi(enimi, OSNIMIL, *osuus);
						 wcscpy(wst, enimi);
                         inputwstr(enimi,OSNIMIL,NMX,osr,oemtowcs(ws1, term, 20, 0),&wtc,FALSE);
						 tc = (char)wtc;
                         if (wcscmp(enimi,wst)) {
						    kilp->paivitys = (INT32) time(NULL);
							elimwbl(enimi);
							kilp->setNimi(enimi, *osuus);
							}
						break;
					 case 2 :
						if (kilpparam.badgelaji) {
						   bdg = kilp->ostiet[*osuus].badge[0];
						   inputlong(&bdg, 7, osd > 1 ? NMX+1 : NMX+19-HAJONTA,
							  osr+1-(osd==1), term, &tc);
						   if (bdg != kilp->ostiet[*osuus].badge[0]) {
							  if (!emitfl)
								 writeerror("Emit-toimintoa ei käynnistetty - "
									"haitallisia syöttövirheitä ei estetä", 0);
							  kilp->ostiet[*osuus].badge[0] = bdg;
							  }
						   }
						break;
					 case 3 :
						 if (kilpparam.badgelaji) {
                           tarkstr[0] = kilp->ostiet[*osuus].laina[0];
						   tarkstr[1] = 0;
						   inputstr(tarkstr, 1, osd > 1 ? NMX+8 : NMX+25-HAJONTA,
							  osr+1-(osd==1), term, &tc, 0);
						   kilp->ostiet[*osuus].laina[0] =
							  toupper(tarkstr[0]) == 'L' ? 'L' : ' ';
						   }
						break;
					 case 2+EMIT1 :
						 if (kilpparam.badgelaji) {
						   inputstr(kilp->ostiet[*osuus].hajonta, HAJONTA,
							  osd > 1 ? NMX+10 : NMX+26-HAJONTA,
							  osr+1-(osd==1), term, &tc, 0);
						   }
						break;
					 case 3+EMIT1 :
						if(!addfl) {
						   do {
							  n = 0;
							  switch(kilp->Tark(*osuus, false)){
								 case 'K' : strcpy(tarkstr,"Kes");
											 break;
                                 case 'H' : strcpy(tarkstr,"Hyl");
                                             break;
                                 case 'P' : strcpy(tarkstr,"Poi");
                                             break;
                                 case 'E' : strcpy(tarkstr,"Eil");
                                             break;
                                 case 'I' : strcpy(tarkstr,"esI");
                                             break;
                                 case 'T' : strcpy(tarkstr,"Trk");
                                             break;
                                 default  : strcpy(tarkstr,"   ");
                                 }
                              ch = tarkstr[0];
                              inputstr(tarkstr,3,KHX,osr,term,&tc,FALSE);
                              if (ch == tarkstr[0]) break;
                              if (tarkstr[0] == 0 ||
                                 tarkstr[0] == ' ')
                                 tarkstr[0] = '-';
                              else
                                 tarkstr[0] = toupper(tarkstr[0]);
							  if (tarkstr[0] == 'G') {
								  if (kilp->ostiet[*osuus].seuranta == 'G') {
									  kilp->ostiet[*osuus].seuranta = 0;
									  vidspmsg(osr+1, 62, 7, 0, "   ");
									  }
								  else {
									  kilp->ostiet[*osuus].seuranta = 'G';
									  vidspmsg(osr+1, 62, 7, 0, "GPS");
									  }
								  break;
								  }
                              if(stschind(tarkstr[0], "KHETI-") < 0)
                                 erbeep();
                              else {
                              if ((tarkstr[0] == 'E' || tarkstr[0] == 'S')
                                 && tarkstr[0] != kilp->Tark(*osuus, false)
                                 && (kilp->Maali(*osuus, 0) != TMAALI0 || 
									stschind(kilp->Tark(*osuus, false), "KH") >= 0)) {
                                 erbeep();
                                 ch = 'E';
                                 selectopt("Tulos jo annettu, "
									"merkitäänkö (K/E)", "KE", &ch);
								 if (ch == 'E') {
									tc = ' ';
									break;
									}
								 }
								 kilp->SetTark(*osuus, tarkstr[0]);
								 outkilp(kilp,*osuus);
								 }
							  } while (
								 stschind(tarkstr[0], "KHEIT-") < 0);
						   }
						break;
							case 4+EMIT1 :
								if (!addfl && (Sarjat[kilp->sarja].tsak[*osuus] || kilpparam.rogaining) && !ampusakot)
									inputlong(&kilp->ostiet[*osuus].sakko, 3,
										KHX, osr+1,term, &tc);
								break;
					 case 5+EMIT1 :
						if(!addfl && ampusakot && Sarjat[kilp->sarja].paikat[*osuus]) {
						   inputstr(kilp->ostiet[*osuus].ampsakot,
							  Sarjat[kilp->sarja].paikat[*osuus],YLX+4-Sarjat[kilp->sarja].paikat[*osuus],
							  osr+ampusakot-1,term,&tc,FALSE);
									}
								if (!addfl) {
						   tarkstr[1] = 0;
						   if (kilp->ostiet[*osuus].lahtolaji > 0)
							  tarkstr[0] = 'Y';
						   else if (kilp->ostiet[*osuus].ylahto!=TMAALI0)
							  tarkstr[0] = 'A';
						   else
								tarkstr[0] = ' ';
						   do {
							  inputstr(tarkstr,1,YLX,osr,term,&tc,FALSE);
									ch = toupper(tarkstr[0]);
							  } while (ch != 0 && ch != ' ' && ch != 'Y' && ch != 'A');
						   if (ch != 'Y' && kilp->ostiet[*osuus].ylahto >= 12*TUNTI) {
							   kilp->ostiet[*osuus].ylahto = TMAALI0;
							   kilp->ostiet[*osuus].lahtolaji = 0; 
							   }
						   if (ch == 'Y') {
								if (kilp->ostiet[*osuus].lahtolaji > 0)
									tm0 = kilp->ostiet[*osuus].ylahto;
								else
									tm0 = ylahto1;
							    viwrrect(YAY,YAX-20,YAY,YAX-2,
									"Yhteislähdön aika :",7,0,0);
								INPUTAIKA(&tm0,t0,8,YAX,YAY,term, &tc);
								if (tm0 == TMAALI0) {
									kilp->ostiet[*osuus].ylahto = TMAALI0;
									kilp->ostiet[*osuus].lahtolaji = 0;
									}
								else {
									ylahto1 = tm0;
									kilp->ostiet[*osuus].ylahto = tm0;
									kilp->ostiet[*osuus].lahtolaji = 1;
									}
								}
						   }
						break;
					 default:
						sj0 = 9999;
						if (!addfl && valiajat && osd > 1) {
						   va = m - 5 - EMIT1;
						   if (va < 1 || va > kilpparam.valuku || va > 4*osd-4)
							  break;
						   sj0 = kilp->Sija(*osuus, va);
						   if (kilp->Maali(*osuus, va) != TMAALI0)
							  tm0 = tulos(kilp, *osuus, va);
						   else tm0 = 0;
						   vaika = tm0;
						   vax = TUX+((va-1)%4-VAVAH)*(OTX-TUX);
						   vay = osr+1+(va-1)/4;
						   INPUTAIKA(&vaika, 0, kilpparam.laika, vax, vay, term, &tc);
						   if (vaika == TMAALI0 || (vaika > -10 && vaika < 10))
							  kilp->setMaali(*osuus, va, TMAALI0);
						   else if (tm0)
							  kilp->setMaali(*osuus, va, pyoristatls(kilp->Maali(*osuus, va) + vaika-tm0, 1));
						   else
							   kilp->setMaali(*osuus, va, pyoristatls(Sarjat[kilp->sarja].lahto + vaika, 1));
									EnterCriticalSection(&tall_CriticalSection);
									p = jalku[kilp->sarja][*osuus][va];
								   n = intsija(jtulos(kilp, *osuus, va), &p, *osuus, va, 0, 0);
								 LeaveCriticalSection(&tall_CriticalSection);
							 if (sj0 < n) n--;
							}
						break;
					 }
				  break;
		 }
	  if(tc != ESC && kilp->sarja >= sarjaluku) {
		 writeerror("ANNA SARJA",0);
		 l = 2;
		 tc = KB_CR;
		 continue;
		 }
	  *osuus = min(*osuus, Sarjat[kilp->sarja].osuusluku-1);
	  if( kilp->Tark(*osuus, false) == ' ')
		 kilp->SetTark(*osuus, '-');
      switch (tc) {
         case KB_CR      : if (jjfl) {
                           *osuus = (*osuus < (Sarjat[kilp->sarja].osuusluku-1))
                              ? *osuus+1 : 0;
                           break;
                           }
         case DOWN    : l = (l == nc) ? 1 : l+1;
                        break;
         case UP      : l = (l == 1)?  nc : l-1;
						break;
		 case 210     :
		 case PGDN    : *osuus = (*osuus < (Sarjat[kilp->sarja].osuusluku-1)) ?
						   *osuus+1 : 0;
								if (Sarjat[kilp->sarja].paikat[*osuus] == 0)
							if (m == 5+EMIT1) m = 3+EMIT1;
						break;
		 case 206     :
		 case PGUP    : *osuus = (*osuus == 0) ? Sarjat[kilp->sarja].osuusluku-1 :
						   *osuus-1;
								if (Sarjat[kilp->sarja].paikat[*osuus] == 0)
							  if (m == 5+EMIT1) m = 3+EMIT1;
						break;
		 case KB_TAB     : if (l == nc) m = (m >= maxm) ? 1 : m+1;
						break;
		 case BACKTAB : if (l == nc) m = (m == 1) ? maxm : m-1;
						break;
		 case 204     :
//#if MAXOSUUSLUKU >= 70
//						   {
//#else
						if (jjfl) {
//#endif
						   clrln(ySize-3);
						   vidspmsg(ySize-3,0,7,0, "Vaihdettavat osuudet     "
							  "ja        Esc : Peruuta");
						   o1 = *osuus+1;
						   o2 = o1 % Sarjat[kilp->sarja].osuusluku + 1;
						   INPUTINT(&o1, 3, 21, ySize - 3, "\r\x1b", &ch);
						   if (ch != ESC && o1 > 0 &&
							  o1 <= Sarjat[kilp->sarja].osuusluku) {
							  INPUTINT(&o2, 3, 28, ySize - 3, "\r\x1b", &ch);
							  if (ch != ESC && o2 > 0 &&
								 o2 <= Sarjat[kilp->sarja].osuusluku) {
								  if (kilp->ostiet[o1 - 1].lisno != kilp->ostiet[o2 - 1].lisno) {
									  ch = ' ';
									  selectopt("Vaihdetaanko myös Lisenssinumerot (K/E)",
										  "KE", &ch);
									  if (ch == 'K') {
										  bdg = kilp->ostiet[o1 - 1].lisno;
										  kilp->ostiet[o1 - 1].lisno =
											  kilp->ostiet[o2 - 1].lisno;
										  kilp->ostiet[o2 - 1].lisno = bdg;
										  }
									  }
								  if (kilp->ostiet[o1 - 1].badge[0] != kilp->ostiet[o2 - 1].badge[0] || 
									  kilp->ostiet[o1 - 1].badge[1] != kilp->ostiet[o2 - 1].badge[1]) {
									ch = ' ';
									selectopt("Vaihdetaanko myös Emit-koodit (K/E)",
									   "KE", &ch);
									if (ch == 'K') {
									   bdg = kilp->ostiet[o1-1].badge[0];
									   kilp->ostiet[o1-1].badge[0] =
										  kilp->ostiet[o2-1].badge[0];
									   kilp->ostiet[o2-1].badge[0] = bdg;
									   bdg = kilp->ostiet[o1 - 1].badge[1];
									   kilp->ostiet[o1 - 1].badge[1] =
										   kilp->ostiet[o2 - 1].badge[1];
									   kilp->ostiet[o2 - 1].badge[1] = bdg;
									   ch = kilp->ostiet[o1 - 1].laina[0];
									   kilp->ostiet[o1-1].laina[0] =
										  kilp->ostiet[o2-1].laina[0];
									   kilp->ostiet[o2-1].laina[1] = ch;
									   ch = kilp->ostiet[o1 - 1].laina[1];
									   kilp->ostiet[o1 - 1].laina[1] =
										   kilp->ostiet[o2 - 1].laina[1];
									   kilp->ostiet[o2 - 1].laina[1] = ch;
									   }
									}
								 memcpy(enimi, kilp->ostiet[o1-1].nimi,
									OSNIMIL+1);
								 if (kilp->ostiet[o2-1].nimi[0] > ' ')
									memcpy(kilp->ostiet[o1-1].nimi,
									   kilp->ostiet[o2-1].nimi,OSNIMIL+1);
								 if (enimi[0] > ' ')
									memcpy(kilp->ostiet[o2-1].nimi,
									   enimi,OSNIMIL+1);
								 memcpy(enimi, kilp->ostiet[o1 - 1].arvo,
									 sizeof(kilp->ostiet[0].arvo));
								 memcpy(kilp->ostiet[o1 - 1].arvo,
									 kilp->ostiet[o2 - 1].arvo, sizeof(kilp->ostiet[0].arvo));
								 memcpy(kilp->ostiet[o2 - 1].arvo,
									 enimi, sizeof(kilp->ostiet[0].arvo));
								 kilp->paivitys = (INT32)time(NULL);
								 }
							  }
						   }
						break;
		 case '+'      : if((kilp->kilpno < minkilpno) ||
						   (kilp->kilpno > maxkilpno)) {
						   sprintf(msg,
							  "Anna kilp.numero väliltä : %d ... %d",
							  minkilpno, maxkilpno);
						   writeerror(msg,0);
						   tc = '0';
						   l = 1;
						   kilp->kilpno = 0;
						   }
						break;
		 }
		if (Sarjat[kilp->sarja].maxnosuus == 1)
			merk_kilp_yl(kilp, *osuus);
		else
			merk_yl(kilp);
	  if (tc != ESC)
		 outkilp(kilp, *osuus);
	  } while (tc != '+' && tc != ESC);
   if( tc == ESC ) exfl = TRUE;
   clrln(ySize-1);
   return(exfl);
   }

static void tarra(kilptietue *kilp, int osuus)
   {
/*
   int  i, sd  ;
   char st[81],kh,*buf;
   int  wf[20];

   if (!tarfl) {
      writeerror("Tarratulostus ei käytössä tällä koneella", 2000);
      return;
      }
   if (!eiltarra && (kilp->ostiet[osuus].keskhyl == 'E' || 
	   kilp->ostiet[osuus].keskhyl == 'S')) return;
   buf = (char *) malloc(CBUFL);
   if (!buf) return;
   memset(buf, ' ', CBUFL);
   kh = kilp->ostiet[osuus].keskhyl;
   wf[0] = TRUE;
   wf[1] = kilp->Maali(osuus, 0) != TMAALI0 &&
      kilp->ostiet[osuus].keskhyl == '-' ||
      kilp->ostiet[osuus].keskhyl == 'I' ||
      kilp->ostiet[osuus].keskhyl == 'T';
   wf[2] = kh == 'K';
   wf[3] = kh == 'H';
   wf[4] = (kh == 'E' || kh == 'S');
   wf[5] = korjtarfl;
   wf[7] = wf[1] && osuus;
   for ( i = 0 ; i < nktarra ; i++) {
      st[0] = 0;
      if( kentta[i].df ) sd = 2;
      else sd = 1;
      kentta[i].w = min(kentta[i].w,(wtarra - kentta[i].x + 1) / sd);
      sd = 0;
      switch (kentta[i].tunnus) {
         case Jk      : if (kilp->joukkue) sprintf(st,"%s %d",
                           kilp->Seura(st), (int)kilp->joukkue);
                        else kilp->Seura(st);
                        break;
         case Os      : _itoa(osuus+1, st, 10); break;
         case Srj     : strcpy(st, (char *)Sarjat[kilp->sarja].sarjanimi); break;
         case Nm      : kilp->Nimi(st, OSNIMIL, osuus); break;
         case Kno     : _itoa(kilp->kilpno, st, 10); break;
         case Tls     : if(kh == '-' || kh == 'T') {
                           AIKATOSTRS(st,tulos(kilp,osuus,0),0);
                           st[kilpparam.laika] = 0;
                           elimz(st);
                           } 
                        else strcpy(st, "--.--");
                        break;
         case OTls    : if(osuus && (kh == '-' || kh == 'T')) {
                           AIKATOSTRS(st,osuustulos(kilp,osuus,0),0);
                           st[kilpparam.laika] = 0;
						   elimz(st);
                           } 
                        else strcpy(st, "--.--");
                        break;
         case Ksk     : sd = 1; break;
         case Hl      : sd = 2; break;
         case El      : sd = 3; break;
         case OsO     : sd = 4; break;
         case Maika   : AIKATOSTRS(st,kilp->Maali(osuus, 0),t0);
                        st[8] = 0;
                        break;
         case Kaika   : strcpy(st, kello()); break;
         case O1      : sd = 5; break;
         case O2      : sd = 6; break;
         case O3      : sd = 7; break;
         case O4      : sd = 8; break;
         case O5      : sd = 9; break;
         case O6      : sd = 10; break;
         case O7      : sd = 11; break;
         case O8      : sd = 12; break;
		 case RNo     : if (pkno != 0) _itoa(pkno, st, 10);
						break;
		 }
	  if (sd > 0) strcpy(st, tarstr[sd]);
	  if (((kentta[i].ehto >= 0) && wf[kentta[i].ehto]) ||
		 ((kentta[i].ehto < 0) && ! wf[-kentta[i].ehto])) {
		if (!kaksoistar || kentta[i].tunnus != RNo)
			putstr(buf,st,kentta[i].x,kentta[i].y,kentta[i].w,kentta[i].rf,
			   kentta[i].df);
		 if( kaksoistar )
			putstr(buf,st,kentta[i].x+wtarra,kentta[i].y,kentta[i].w,
			kentta[i].rf,FALSE);
		 }
	  }
   printbuf(tarf,buf,nrtarra);
*/
   }

static void testitarra(void)
   {
/*
	kilptietue tkilp;

   strncpy(tkilp.ostiet[1].nimi, "Kilpailijan Nimi", OSNIMIL);
   tkilp.ostiet[1].nimi[OSNIMIL] = 0;
#if MAXOSUUSLUKU < 1
   strcpy(tkilp.ostiet[1].oseura, "Seuranimi");
#endif
   tkilp.joukkue = 1;
   tkilp.kilpno = 1234;
   strcpy(tkilp.seura, "Seurannimi_Nimi");
   tkilp.sarja = 0;
   tkilp.ostiet[0].keskhyl = '-';
   tkilp.ostiet[1].keskhyl = '-';
   tkilp.setMaali(0, 0, Sarjat[0].lahto+38830L);
   tkilp.setMaali(1, 0, Sarjat[0].lahto+76830L);
   tkilp.ostiet[0].ylahto = TMAALI0;
   tkilp.ostiet[1].ylahto = TMAALI0;
   tarra(&tkilp,1);
*/
}

static int seur_aukko(int *aukko)
   {
   int kno, d, fl, os;
   kilptietue kilp;

   if (*aukko < minkilpno) *aukko = minkilpno;
   for (kno = *aukko; kno <= maxkilpno; kno++) {
	  if ((d = getpos(kno)) <= 0) continue;
	  GETREC(&kilp, d);
	  fl = 0;
	  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		 if (fl && kilp.Maali(os, 0) != TMAALI0 &&
			kilp.ostiet[os].ylahto == TMAALI0) {
			*aukko = kno+1;
			return(kno);
			}
		 if (kilp.Maali(os, 0) == TMAALI0) fl = 1;
		 }
	  }
   *aukko = -1;
   return(-1);
   }

static void dspjjline(void)
   {
   int i;

   clrtxt(OSY-2,TUX,64);
   vidspmsg(OSY-2,TUX,7,0,jjline[0]);
   for (i = 0; i <= (MAXOSUUSLUKU-1)*osd; i++)
	  clrtxt(OSY+i,TUX,64);
   for (i = 0; i < MAXOSUUSLUKU; i++)
	  vidspmsg(OSY+i*osd,TUX,7,0,jjline[i+1]);
   }

static int lueuusirivi(int n)
   {
   int i, err = 0;
   char line[200], *p;

   for (i = 0; i <= MAXOSUUSLUKU; i++) {
	  if (i <= MAXOSUUSLUKU - n)
		 memcpy(jjline[i], jjline[i+n], sizeof(jjline[0]));
	  else {
		 memset(jjline[i], 0, sizeof(jjline[0]));
		 if (fgets(line, 198, efile)) {
			strncpy(jjline[i], line, 78);
			for (p = jjline[i] + strlen(jjline[i])-1;
			   p >= jjline[i] && *p <= ' '; p--)
			   *p = 0;
			scanconv(jjline[i]);
			}
		 else {
			err = 1;
			strcpy(jjline[i], "TIEDOSTO LOPPU");
			}
		 }
	  }
   return(err);
   }

int haejjkilp(int *d, kilptietue *kilp, int *osuus)
{
	static int dd;
	char st[60], keyn[NIMIINDL+10] = "";
	int ne = 0, srj, kno, jno, res = 1;

	strcpy(st, jjline[0]);
	if (tulkrivi(st, &srj, &kno, &jno))
		return(1);
	st[LSEURA] = 0;
	if (!kno) {
		int ok;
		if (!st[0])
			return(1);
		keyfromU8name(keyn, st, NIMIINDL-3, 0);
		searchix(&nameindex, keyn, &dd);
		ok = 1;
		while (ok && dd > 0) {
			GETREC(kilp, dd);
			if (strcmp(st, kilp->seura))
				break;
			if (srj == kilp->sarja && jno == kilp->joukkue) {
				res = 0;
				break;
				}
			ok = nextix(&nameindex, keyn, &dd);
			}
		}
	else {
		dd = getpos(kno);
		if( dd > 0 ) {
			GETREC(kilp, dd);
			res = 0;
			}
		}
	*osuus = 0;
	if (res == 0) {
		*d = dd;
		}
	return(res);
}

static int tulkrivi(char *line, int *srj, int *kno, int *jno)
   {
   char *p, ero[2] = " ";
   int plen, res = 1;

   ero[0] = erotin;
   *srj = -1;
   *kno = 0;
   *jno = 0;
   p = strtok(line, ero);
   if (!p) return(1);
   plen = strlen(p);
   elimbl(p);
   upcasestr(p);
   while (*p == ' ') p++;
   *srj = haesarja(p, false);
   if (p[plen+1] != erotin) {
      p = strtok(NULL, ero);
      if (p) {
		 while (*p == ' ') p++;
		 elimbl(p);
		 *kno = atoi(p);
		 if (*kno) res = 0;
		 }
	  }
   if ((p = strtok(NULL, ero)) != NULL) {
	  res = 0;
	  strcpy(line, p);
	  if ((p = strtok(NULL, ero)) != NULL)
		 *jno = atoi(p);
	  }
   return(res);
   }

int find(int *d, kilptietue *kilp, int *osuus)
   {
   int exfl = 0;
   int kno, srj, jno, fljj;
   char tc, *p;
   char keyn[NIMIINDL+1], keyn0[NIMIINDL+1];
   char str[34];
   wchar_t wstr[60], wtc;
   static int aukko = -1;
   static int edkno,nimifl;
   static char edkey[NIMIINDL+1];
#if defined(JUKOLAVANHA) || defined(HALIKKO) 
   static int edosuus;
#endif

   alku:
   fljj = 0;
   exfl = FALSE;
   clrscr();
   kehys(1);
   outform();
   if( ! korjfl ) VIDSPAIKA(MAY,MAX,tmaali,t0);
   kno = 0;
   if (efile) {
      dspjjline();
      strncpy(str, jjline[0], sizeof(str));
      p = str;
      if (tulkrivi(p, &srj, &kno, &jno)) p = NULL;
      fljj = 1;
      }
   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,"<Return> : Hyväksy kenttä,   <Esc> : Poistu,"
      "   <F4> : Testitarra");
   if (kno || !efile) {
      do {
		 *d = 0;
         if (!fljj) {
            sccurset(NOY,NOX+5);
            while (!kbdrdy()) {
               Sleep(100);
               sccurset(NOY,NOX+5);
               }
//      tcomfl = FALSE;
#if !defined(JUKOLAVANHA) && !defined(HALIKKO)
            vidspmsg(NOY,NOX+4,7,0,"-");
            if (!tarkfl)
               input_no(&kno,NOX+2,NOY,"eEsSaA\r\x1B\xCB\xCC*",&tc);
            else
               inputkno(&kno,NOX,NOY,"eEsSaA\r\x1B\xCB\xCC*",&tc);
            tc = toupper(tc);
#else
            *osuus = 1;
			if (jjfl) {
			   vidspmsg(NOY,NOX+4,7,0,"    ");
               INPUTINT(&kno, 4, NOX, NOY, "eEsSaA\r\x1B\xCB\xCC*", &tc);
               tc = toupper(tc);
               *osuus = 0;
               }
            else {
#if MAXOSUUSLUKU < 10
               vidspmsg(NOY,NOX+4,7,0,"- ");
               inputkno_os(&kno,osuus,NOX,NOY,"eEsSaA\r\x1B\xCB\xCC*",&tc);
#else
               vidspmsg(NOY,NOX+4,7,0,"-  ");
               inputkno_os2(&kno,osuus,NOX,NOY,"eEsSaA\r\x1B\xCB\xCC*",&tc);
#endif
               tc = toupper(tc);
               if (tc == 'A') *osuus = 0;
               else if (tc == 'E' || tc == 'S') *osuus = edosuus;
               else
                  --*osuus;
               }
#endif
            tcomfl = TRUE;
            if ((tc == 'E' || tc == 'S') && nimifl) {
               memcpy(keyn, edkey, sizeof(keyn));
               goto seurnimi;
               }
            if (tc == 'A') {
               kno = seur_aukko(&aukko);
               if (kno < minkilpno) {
                  writeerror("Ei löydy", 0);
                  continue;
                  }
			   }
            if (tc == 'E') {
               kno = edkno-1;
               while (kno > minkilpno && getpos(kno) <= 0)
                  kno--;
               }
            if (tc == 'S') {
               kno = edkno + 1;
               while (kno < maxkilpno && getpos(kno) <= 0)
                  kno++;
               }
            if(tc == 204 ) {
               testitarra();
               continue;
               }
            }
         if(tc == ESC || kno == 0) break;
		 if( (kno < minkilpno) || (kno > maxkilpno) )
			writeerror("Numero liian suuri tai pieni",0);
         else {
            edkno = kno;
            nimifl = 0;
            *d = getpos(kno);
            if( *d > 0 ) {
               GETREC(kilp, *d);
               if (kilpparam.partio)
                  *osuus = Sarjat[kilp->sarja].osuusluku-1;
               else {
#if !defined(JUKOLAVANHA) && !defined(HALIKKO)
                  *osuus = 0;
                  if (!jjfl)
                     while (*osuus < Sarjat[kilp->sarja].osuusluku-1 &&
                        (kilp->Maali(*osuus, 0) !=TMAALI0 ||
                        stschind(kilp->Tark(*osuus, false),"KH")>=0))
                        (*osuus)++;
#else
                  edosuus = *osuus;
#endif
                  *osuus = min(*osuus,Sarjat[kilp->sarja].osuusluku-1);
                  }
               outkilp(kilp,*osuus);
               if (efile) {
                  dspjjline();
                  if (fljj) {
                     tc = 'H';
                     selectopt("H)yväksy joukkuenumero, N)imihaku, O)hita rivi",
                        "HNO", &tc);
                     if (tc == 'N') {
                        kno = 0;
						*d = 0;
                        break;
                        }
                     if (tc == 'O') {
                        lueuusirivi(1);
                        goto alku;
                        }
                     }
                  }
               }
            else writeerror("EI LÖYDY!",2000); 
            }
         } while (*d == 0);
      }
   if(tc != ESC) {
      if( kno == 0 ) {
         clrln(ySize-3);
		 vidspmsg(ySize-3,0,7,0,
			"<Return> : Hyväksy kenttä,   <Esc> : Poistu");
         if (!fljj) {
            memset(str, 0, 31);
            while (!kbdrdy()) {
               Sleep(100);
               sccurset(SEY,SEX);
               }
//          tcomfl = FALSE;

            muunimi:

            inputwstr(wstr,20,SEX,SEY,L"\xD\x1B",&wtc,FALSE);
			tc = (char)wtc;
            tcomfl = TRUE;
            }
         if( tc != ESC )  {
			memset(keyn, 0, sizeof(keyn));
            keyfromU16name(keyn, wstr, NIMIINDL-3, 0);
            memcpy(keyn0, keyn, NIMIINDL);

            seurnimi:

            searchix(&nameindex, keyn, d);
            if (fljj) {
               do {
                  GETREC(kilp, *d);
                  if (memcmp(keyn, keyn0, NIMIINDL-3) > 0 ||
                     (kilp->sarja == srj && (jno <= 1 || kilp->joukkue >= jno)))
                     break;
                  if (nextix(&nameindex, keyn, d))
					  break;
                  } while (1);
			   }
            fljj = 0;
            do {
               if (tc != 'S' && tc != 'H') {
                  GETREC(kilp, *d);
                  if (kilpparam.partio)
                     *osuus = Sarjat[kilp->sarja].osuusluku-1;
                  else {
                     *osuus = 0;
                     if (!jjfl)
                        while (*osuus < Sarjat[kilp->sarja].osuusluku-1 &&
                           (kilp->Maali(*osuus, 0) !=TMAALI0 ||
                           stschind(kilp->Tark(*osuus, false), "KH")>=0))
                           (*osuus)++;
                     }
                  outkilp(kilp,*osuus);
                  if (efile) dspjjline();
				  listfl = TRUE;
				  tc = 'H';
                  if (!efile)
                     selectopt("Hae : S)euraava, E)dellinen, M)uu, H)yväksy valinta,"
                             " <Esc> : Poistu","SEMH\x1B",&tc);
                  else
                     selectopt("Hae : S)eur., E)dell., M)uu, H)yväksy valinta,"
                             " O)hita rivi  <Esc> : Poistu","SEOMH\x1B",&tc);
                  }
               switch(tc) {
                  case 'S' : nextix(&nameindex, keyn, d);
                             tc = ' ';
                             break;
                  case 'E' : previx(&nameindex, keyn, d);
                             tc = ' ';
                             break;
                  case 'M' : goto muunimi;
                  case 'O' : lueuusirivi(1);
                             goto alku;
                  }
               } while (tc != 'H' && tc != ESC);
            if (tc == 'H') {
               memcpy(edkey, keyn, sizeof(edkey));
               nimifl = 1;
               }
            }
         }
      }
   if( tc == ESC) exfl = TRUE;
   return(exfl);
   }

void korjaa(int d, kilptietue *kilp, int *osuus, int tausta, kilptietue *entkilp)
   {
   int  exfl, comtarfl;
   kilptietue ekilp, kilp1;

   if (!tausta) header=L"KORJAA    ";
   kehys(1);
   for (;;) {
	   ekilp = *kilp;
	  //memcpy(&ekilp, kilp, sizeof(kilptietue));
//    tcomfl = FALSE;
	  exfl = inputkilp(&ekilp,osuus, 0);
	  tcomfl = TRUE;
	  if (exfl) break;
	  comtarfl = FALSE;
	  if( (tarfl || lahtarfl) &&
		  (ekilp.Maali(*osuus, 0) != TMAALI0 || !ekilp.Hyv(*osuus)))
			comtarfl = TRUE;
	  if (ekilp.Maali(*osuus, 0) != TMAALI0 &&
          ekilp.Hyv(*osuus)) {
         if ((osuustulos(&ekilp,*osuus,0) <
            varkynnys[ekilp.sarja][*osuus]) ||
            (osuustulos(&ekilp, *osuus,0) > 12*TUNTI))
            writeerror("Poikkeavan hyvä osuusaika",0);
         }
      GETREC(&kilp1, d);
	  if (!(*entkilp == kilp1)) {
		 writeerror("Kilpailijatietoihin tullut muutossanoma - tehdyt "
            "muutokset peruutetaan", 0);
         if (!memcmp(entkilp, kilp, sizeof(kilptietue))) {
            *kilp = kilp1;
            *entkilp = kilp1;
            continue;
            }
         else break;
         }
      tallenna(&ekilp,d,comtarfl, 0, 0, 0);
      break;
      }
   }

static TMAALI maaliaika(TMAALI tmaali, int maikay, int maikax,
   char *kh)
   {
   char tarr[7], tarrold[7], prs[100];
   char ch, ch2;
   int    tt, tt1, i, j, tm, ltarr;
   char st[4];
   TMAALI  ttm,tmaali0;
#if !defined(MAALI) && VAIKALUKU > 0
   char ma_st[20];

   ma_st[8] = 0;
#endif
   ltarr = kilpparam.laika == 10 ? 7 : 6;
   tmaali0 = tmaali;
   tt = (int)((tmaali >= 0) ? tmaali / TUNTI + t0 :
      (tmaali - TUNTI + 1) / TUNTI + t0);
   ttm = (tmaali - (tt - t0) * TUNTI);
   _itoa((tt % 24), st, 10);
   if( strlen(st) == 1 ) inschar('0', st, 0);
   tarrold[0] = st[0];
   tarrold[1] = st[1];
   tt = (int)(ttm / MINUUTTI);
   _itoa(tt, st, 10);
   if( strlen(st) == 1 ) inschar('0', st, 0);
   tarrold[2] = st[0];
   tarrold[3] = st[1];
   tt = (int) (ttm - MINUUTTI * tt);
   _itoa(tt, st, 10);
   if( strlen(st) == 1 ) inschar('0', st, 0);
   if( strlen(st) == 2 ) inschar('0', st, 0);
   if (kilpparam.laika == 10) tarrold[6] = st[2];
   tarrold[4] = st[0];
   tarrold[5] = st[1];
   for ( i = 0 ; i < ltarr ; i++) tarr[i] = tarrold[i];
   if (kilpparam.laika == 10)
      sprintf(prs,"%c%c.%c%c.%c%c,%c",tarrold[0],tarrold[1],
                 tarrold[2],tarrold[3],tarrold[4],tarrold[5],tarrold[6]);
   else
      sprintf(prs,"%c%c.%c%c.%c%c",tarrold[0],tarrold[1],
                 tarrold[2],tarrold[3],tarrold[4],tarrold[5]);
#ifndef KUUL
   vidspmsg(maikay-1,maikax-22,7,0,"Edellinen maaliaika :");
   vidspmsg(maikay-1, maikax,7,0,prs);
#endif
   if (kilpparam.laika == 10)
      sprintf(prs,"%c%c.%c%c.%c%c,%c",tarr[0],tarr[1],
                 tarr[2],tarr[3],tarr[4],tarr[5],tarr[6]);
   else
      sprintf(prs,"%c%c.%c%c.%c%c",tarr[0],tarr[1],
                 tarr[2],tarr[3],tarr[4],tarr[5]);
   vidspmsg(maikay, maikax, 7, 0, prs);
lue:
   clrln(ySize-3);
   if (maikax == MAX) {
      vidspmsg(ySize-3,0,7,0,"<Ret.> : Hyv. aika K)esk. H)yl. E)i läht. N)yt "
         "S)eur. aika  <Esc> : Poistu");
      vidspmsg(ySize-1,0,7,0,"PgUp, F6 : Edell. osuus, PgDn, F10 : Seur. osuus");
      }
   else
      vidspmsg(ySize-3,0,7,0,"<Ret.> : Hyväksy aika, Up Dn : Siirry,  "
         "<Esc> : Poistu");
   sccurset(maikay,maikax+kilpparam.laika-1);
   i = 0;
   do {
       *kh = 0;
       ch = readkbd(&ch2,TRUE,spoolfl);
       switch(ch) {
#if !defined(MAALI) && VAIKALUKU > 0
         case 9  :
            if (valiajat) {
               if (ma_piste) {
                  if (ma_kilp->Maali(ma_os, ma_piste) != TMAALI0)
                     AIKATOSTRS(ma_st, tulos(ma_kilp,ma_os,ma_piste), 0);
                  else strcpy(ma_st, "00.00.00");
				  ma_st[8] = 0;
                  viwrrect(OSY+ma_os*osd+1, TUX+(ma_piste-VAVAH)*(OTX-TUX),
                     OSY+ma_os*osd+1, TUX+(ma_piste-VAVAH)*(OTX-TUX)+7,
                     ma_st,7,0,0);
                  }
               ma_piste = (ma_piste+1) % (kilpparam.valuku+1);
               if (ma_piste) {
                  if (ma_kilp->Maali(ma_os, ma_piste) != TMAALI0)
                     AIKATOSTRS(ma_st, tulos(ma_kilp,ma_os,ma_piste), 0);
                  else strcpy(ma_st, "00.00.00");
				  ma_st[8] = 0;
                  viwrrect(OSY+ma_os*osd+1, TUX+(ma_piste-VAVAH)*(OTX-TUX),
                     OSY+ma_os*osd+1, TUX+(ma_piste-VAVAH)*(OTX-TUX)+7,
                     ma_st,0,7,0);
                  }
               }
            break;
#endif
         case 13 :
         case '+':   break;
         case 8  :   if( i>0 ) {
                        i--;
                        if( i > 0 ) for ( j = 0; j <= i; j++)
                           tarr[ltarr-1-j] = tarr[ltarr-2-j];
                        tarr[ltarr-1-i] = tarrold[ltarr-1-i];
                        }
                     break;
         case 48 :
         case 49 :
         case 50 :
         case 51 :
         case 52 :
         case 53 :
         case 54 :
         case 55 :
         case 56 :
         case 57 :   if( i==6 ) erbeep();
                     else {
                        i++;
                        for (j = ltarr - i; j < ltarr-1; j++)
                           tarr[j] = tarr[j+1];
                        tarr[ltarr-1] = ch;
                        }
                     break;
         case 'e':
         case 'E':   *kh = 'E';
                     ch = 200;
                     break;
         case 'h':
         case 'H':   *kh = 'H';
                     ch = 200;
                     break;
         case 'k':
         case 'K':   *kh = 'K';
                     ch = 200;
                     break;
         case 't':
         case 'T':   *kh = 'T';
                     break;
         case 'n':
         case 'N':   *kh = 'N';
                     ch = 200;
                     break;
         case 's':
         case 'S':   *kh = 'S';
                     ch = 200;
                     break;
         case 'y':
         case 'Y':   *kh = 'Y';
                     ch = KB_CR;
                     break;
         case ESC:   *kh = ESC;
                     break;
         case 0 :
            switch (ch2) {
               case 64 :                      /* F6 */ 
               case 73 : *kh = 211; break;      /*pgup*/
               case 68 :                      /* F10 */
               case 81 : *kh = 212; break;      /*pgdn*/
#ifdef KUUL
               case 25 : *kh = 25; break;
               case 72 : if (maikay != MAY) *kh = 5; break;
               case 80 : if (maikay != MAY) *kh = 24; break;
               case 118: if (maikay != MAY) *kh = 2; break;
               case 132: if (maikay != MAY) *kh = 19; break;
#endif
               default : erbeep();
               }
            break;
         default :   erbeep();
          }
      if (*kh > 210 || (*kh >= 2 && *kh <= 25)) break;
      if (kilpparam.laika == 10)
         sprintf(prs,"%c%c.%c%c.%c%c,%c",tarr[0],tarr[1],
                 tarr[2],tarr[3],tarr[4],tarr[5],tarr[6]);
      else
         sprintf(prs,"%c%c.%c%c.%c%c",tarr[0],tarr[1],
                 tarr[2],tarr[3],tarr[4],tarr[5]);
      vidspmsg(maikay, maikax, 7, 0, prs);
      sccurset(maikay, maikax + kilpparam.laika-1);
      } while (ch != KB_CR && ch != 200 && ch != ESC && ch != '+');
   if(ch == KB_CR || ch == '+') {
      tt = 10 * (tarr[0] - '0') + tarr[1] - '0'; 
      tt1 = 10 * (tarr[2] - '0') + tarr[3] - '0'; 
      tm = 10 * (tarr[4] - '0') + tarr[5] - '0'; 
      tm *= 10;
      if (kilpparam.laika == 10) tm += tarr[6] - '0';
/*      if( 60 * (tt-t0) + tt1 < -MRAJA ) tt = tt + 24;
      if( (tm < MINUUTTI) && (tt1 < 60) && (abs(60*(tt-t0)+tt1) < MRAJA))
*/
      if( (tm < MINUUTTI) && (tt1 < 60) && (tt < 24))
         tmaali = tm + MINUUTTI*tt1 + TUNTI*(tt-t0);
      else {
         erbeep();
         goto lue;
         }
/*
      if (MAY == maikay &&
         ABS(tmaali - tmaali0) > 15*MINUUTTI && ntulost[ma_kilp->sarja][0] > 0) {
         erbeep();
         ch = ' ';
         selectopt("Maaliaika muuttunut yli 15 min - Hyväksytäänkö (K/E)?",
            "KE", &ch);
         if (ch == 'E') goto lue;
         }
*/
      }
   clrln(ySize-1);
   return(tmaali);
   }

#if !defined(LUENTA)
void laskenta()
   {
   int  exfl, d,osuus,os;
   char cs,kh,tc;
   kilptietue kilp,ekilp,entkilp;
   TMAALI poimi;
	static TMAALI ylahto1 = -12*TUNTI;
   aikatp ttm;

   if (nilmt <= 0) return;
   korjfl = FALSE;
   do {
      alku:
      header=L"LASKENTA  ";
      exfl = find(&d,&kilp,&osuus);
      memcpy(&entkilp, &kilp, sizeof(kilptietue));
      poimi = TMAALI0;
      if (exfl) return;
      do {
         cs = 'M';
         while (kilp.Maali(osuus, 0) != TMAALI0 || !kilp.Hyv(osuus)) {
            erbeep();
            cs = ' ';
            selectopt("Tulos jo annettu. K)orjaa, PgUp PgDn F6 F10 : Vaihda osuus, "
               "Esc : Peruuta","K\xA0\xA1\xCE\xD2\x1B",&cs);
            switch (cs) {
               case 'K' :
                  korjfl = TRUE;
                  korjaa(d,&kilp,&osuus, 0, &entkilp);
                  korjfl = FALSE;
                  break;
               case 161 :
               case 210 :
                  if (osuus < Sarjat[kilp.sarja].osuusluku-1) osuus++;
                  cs = 'M';
                  outkilp(&kilp, osuus);
                  break;
               case 160 :
               case 206 :
                  if (osuus > 0) osuus--;
                  cs = 'M';
                  outkilp(&kilp, osuus);
                  break;
               }
            if (cs == ESC || cs == 'K') goto alku;
            }
#if !defined(MAALI) && VAIKALUKU > 0
         ma_os = osuus;
         ma_piste = 0;
         ma_kilp = &kilp;
#endif
         memcpy(&ekilp, &kilp, sizeof(kilptietue));
//         tcomfl = FALSE;
         tmaali = maaliaika(tmaali, MAY, MAX, &kh);
         if (kh == 211) {
            if (osuus > 0) osuus--;
            outkilp(&kilp, osuus);
            }
         if (kh == 212) {
            if (osuus < Sarjat[kilp.sarja].osuusluku-1) osuus++;
            outkilp(&kilp, osuus);
            }
         } while (kh > 210);
      if (cs == 'M' && kh != ESC ) {
         if (kh == 'N') {
nyt:
            tmaali = (TMAALI) (KSEK*T_TIME(biostime(0,0L)));
            kh = 0;
            }
         if (kh == 'S') {
seuraika:
			 if (aikajono[akttm])
				vidint(ySize-1,76,4,(aikajono[akttm]->aktrows - aikajono[akttm]->rtime));
			 if (aikajono[akttm] && (aikajono[akttm]->aktrows - aikajono[akttm]->rtime) > 0) {
				tmaali = purajak(aikajono[akttm]->getAktTime(0, aikajono[akttm]->rtime++)->t);
               vidint(ySize-1,71,4,aikajono[akttm]->aktrows);
               vidint(ySize-1,76,4,aikajono[akttm]->rtime+1);
			   vidint(ySize - 1, 54, 4, aikajono[akttm]->getAktTime(&ttm, aikajono[akttm]->aktrows-1)->kno);
               VIDSPAIKA(ySize-1,60,purajak(ttm.t),t0);
               poimi = tmaali;
               }
            else writeerror("EI AIKOJA", 1500);
            kh = 0;
            }
         if (kh == 'Y') {
//            if (ylkynnys[kilp.sarja] == TMAALI0) 
					{
					if (ekilp.ostiet[os].lahtolaji > 0)
						ylahto1 = ekilp.ostiet[os].ylahto;
               viwrrect(YAY,YAX-20,YAY,YAX-2,"Yhteislähdön aika :",7,0,0);
               INPUTAIKA(&ylahto1,t0,8,YAX,YAY,"+\r",&tc);
               cs = ' ';
               selectopt("T)ämä vai S)euraava osuus", "TS", &cs);
               for (os = osuus+(cs == 'S'); os<Sarjat[ekilp.sarja].osuusluku;
                  os++) {
                  ekilp.ostiet[os].ylahto = ylahto1;
				  ekilp.ostiet[os].lahtolaji = ylahto1 != TMAALI0;
                  if (!ylmyoh || ekilp.ostiet[os].ylahto != TMAALI0) 
					  break;
                  }
               }
//           else writeerror("Yhteislähtöautomatiikka käytössä", 0);
            kh = 0;
            }
#if !defined(MAALI) && VAIKALUKU > 0
		 if( kh == 0) ekilp.setMaali(osuus, ma_piste, pyoristatls(tmaali, 1));
#else
		 if (kh == 0) ekilp.setMaali(osuus, 0, pyoristatls(tmaali, 1));
#endif
         else {
            ekilp.SetTark(osuus, kh);
            }
         do {
            EnterCriticalSection(&tall_CriticalSection);
			if (kilpparam.maxnosuus == 1)
				haesija(&ekilp,osuus, 0, 0, 0);
            else if (Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[osuus]+1] == osuus)
               haesija(&ekilp,Sarjat[kilp.sarja].yosuus[osuus], 0, 0, 0);
            LeaveCriticalSection(&tall_CriticalSection);
            outkilp(&ekilp,osuus);
            if (ekilp.Maali(osuus, 0) != TMAALI0 &&
                ekilp.Hyv(osuus)) {
               if (ntulos[ekilp.sarja][osuus][0] == 0)
                  writeerror("Osuuden ensimmäinen tulos",0);
               else if ((osuustulos(&ekilp,osuus,0) <
                  varkynnys[ekilp.sarja][osuus]) ||
                  (osuustulos(&ekilp, osuus,0) > 12*TUNTI))
                  writeerror("Poikkeavan hyvä osuusaika",0);
               }
            cs = ' ';
			if (aikajono[akttm] && aikajono[akttm]->aktrows > 0)
               selectopt("+) hyväksy, K)orjaa,   S)eur. aika, E)dell."
                  " aika, N)yt,  <Esc> : peruuta", "+KESN\x1B",&cs);
            else
               selectopt("+) hyväksy, K)orjaa, <Esc> : peruuta",
                  "+KESN\x1B",&cs);
            switch(cs) {
               case 'K' :  clrln(ySize-3);
                           vidspmsg(ySize-3,0,7,0,"<Return> : hyväksy "
                              "kenttä,  + : hyväksy korjaukset, "
                              " <Esc> : peruuta ja poistu");
                           exfl = inputkilp(&ekilp,&osuus, 0);
                           break;
			   case 'E':  if (aikajono[akttm] && aikajono[akttm]->rtime > 1) 
								aikajono[akttm]->rtime--;
						  if (aikajono[akttm] && aikajono[akttm]->rtime > 0) 
								aikajono[akttm]->rtime--;
               case 'S' :  goto seuraika;
               case 'N' :  goto nyt;
               case ESC :  exfl = TRUE;
               }
            } while (cs != '+' && !exfl);
         if( ! exfl ) {
			 if (aikajono[akttm] && poimi == tmaali) {
				aikatp utm;
				aikajono[akttm]->getAktTime(&ttm, aikajono[akttm]->rtime - 1);
				utm = ttm;
               utm.kno = ekilp.kilpno;
			   tall_rivi(akttm, &utm, &ttm, 0, 0, 0, 0);
               }
            tallenna(&ekilp,d,TRUE, 0, 0, &entkilp);
            }
         exfl = FALSE;
         }
      tcomfl = TRUE;
      } while (1);
   }
#endif

static void add(void)
   {
   int  exfl, dataf,osuus;
   kilptietue kilp;

   do {
      header=L"LISÄYS    ";
      kehys(1);
      outform();
	  kilp.nollaa();
      if (sarjaluku > 1) 
		 kilp.sarja = 255;
      osuus = 0;
      exfl = inputkilp(&kilp,&osuus, 1);
      if( ! exfl ) {
         if (nilmt == maxrec-1)
            writeerror("Kilpailijataulukko täyttyi. "
            "Kilpailijamäärää ei voi enää lisätä",3000);
         if (nilmt >= maxrec) {
            writeerror(
               "Kilpailijataulukko täynnä - lisäystä ei tallennettu",
                  0);
            return;
            }
         addtall(&kilp,&dataf, 0);
         }
      } while (!exfl);
   }

static void editkilp(void)
   {
   int  d, osuus;
   kilptietue kilp, entkilp;

   if (nilmt <= 0) return;
   do {
      if (find(&d,&kilp,&osuus))
         break;
      entkilp = kilp;
      korjaa(d,&kilp,&osuus, 0, &entkilp);
      } while (1);
   }

static void luejuoksujarj(void)
   {
   int  i, d, osuus, tiedots;
   kilptietue kilp, entkilp;
   char etiednimi[40] = "", ch, ct, cvahv = ' ', line[200], e[2] = " ";
#ifdef EMIT
   char *p;
#endif

   if (nilmt <= 0) return;
   ct = ' ';
   selectopt("Tiedostossa E)i otsikoita, V)akio-otsikot, "
      "M)uu edeltävä teksti", "EVM", &ct);
   switch (ct) {
      case 'E':
         tiedots = 0;
         break;
      case 'V':
         tiedots = 1;
         break;
      case 'M':
         tiedots = 2;
         break;
      }
   clrln(ySize-3);
   inputstr_pr("Luettava tiedosto:", etiednimi, 39, 20, ySize-3, "\r\x1b",
      &ct, 0);
   if (ct == 27) return;
   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,"Anna erotinmerkki:");
   e[0] = erotin;
   do {
      sccurset(ySize-3,19);
      viwrrect(ySize-3,19,ySize-3,19,e,7,0,0);
      ct = readkbd(&ch,TRUE,0);
      if (ct != '\r') erotin = ct;
      e[0] = erotin;
      } while (erotin != '\t' && (erotin < 32 || erotin >= 127
         || (erotin >= 'A' && erotin <= 'Z')
         || (erotin >= 'a' && erotin <= 'z')
         || (erotin >= '0' && erotin <= '9')));
   clrln(ySize-3);
   efile = fopen(etiednimi, "rt");
   if (!efile) {
      sprintf(line, "Tiedoston %s avaaminen ei onnistu", etiednimi);
      writeerror(line, 0);
      return;
      }
   if (tiedots == 1) {
      do {
         if (!fgets(line, 198, efile)) {
            writeerror("Otsikkoriviä ==ILMOITTAUTUMISET ei löytynyt",0);
            fclose(efile);
            efile = NULL;
            return;
            }
         if (line[0] != '=') continue;
         line[199] = 0;
         upcasestr(line);
         if (strstr(line, "ILMOITT")) break;
         } while (1);
      }
   if (lueuusirivi(MAXOSUUSLUKU+1)) {
      writeerror("Tiedostosta ei löydy ilmoittautumisia",0);
      fclose(efile);
      efile = NULL;
      return;
      }
   ct = 'S';
   selectopt("S)iirrä nimet pyytämättä vahvistuksia, V)ahvista jokainen siirto",
         "SV\x1b", &cvahv);
   if (cvahv == 'S') {
		selectopt("V)aihda etu- ja sukunimet, E)i vaihtoa", "VE", &ct);
		if (ct == 'E')
			ct = 'S';
		}
   do {
	  if (cvahv == 'S') {
		  if (haejjkilp(&d, &kilp, &osuus))
			  break;
		  }
	  else {
	      if (find(&d,&kilp,&osuus)) break;
		  selectopt("S)iirrä nimet suoraan, V)aihtaen etu- ja sukunimet, E)i siirtoa",
			 "SVE", &ct);
		  }
	  entkilp = kilp;
#ifdef EMIT
      if (ct != 'E') {
         for (i = 1; i <= Sarjat[kilp.sarja].osuusluku; i++) {
            p = strtok(jjline[i], e);
            if (p) {
               p = strtok(NULL, e);
               if (p)
                  kilp.ostiet[i-1].badge[0] = atol(p);
               }
            }
         }
#endif
      if (ct != 'E') {
         for (i = 1; i <= Sarjat[kilp.sarja].osuusluku; i++) {
			kilp.setNimi(line, osuus, ct == 'V' ? -1 : 1);
            }
         }
      outkilp(&kilp, 0);
      dspjjline();
	  if (cvahv != 'S')
	      korjaa(d,&kilp,&osuus, 0, &entkilp);
	  else
		  tallenna(&kilp, d, 0, 0, 0, &entkilp);
      lueuusirivi(Sarjat[kilp.sarja].osuusluku+1);
      if (jjline[0][0] == '=') break;
      } while (1);
   fclose(efile);
   efile = NULL;
   }

static void deletekilp(void)
   {
   int  d,osuus,va;
   char cs,ch;
   kilptietue kilp;

   if (nilmt <= 1) return;
   do {
      nextkilp:
      header=L"POISTO    ";
      kehys(1);
      osuus = 0;
      if (find(&d,&kilp,&osuus)) return;
      for (osuus = 0; osuus<Sarjat[kilp.sarja].osuusluku; osuus++) {
         for (va = 0; va <= kilpparam.valuku; va++)
            if (kilp.Maali(osuus, va) != TMAALI0) break;
         if (va <= kilpparam.valuku || !kilp.Hyv(osuus)) {
            clrln(ySize-3);
            vidspmsg(ySize-3,0,7,0,"Ei voi enää poistaa, merkitse ei "
               "läht.,  Paina <Return>");
            erbeep();
            do {} while (readkbd(&ch,TRUE,spoolfl) != KB_CR);
            goto nextkilp;
            }
         }
      cs = ' ';
      selectopt("P)oista kilpailija,  L)uovu poistosta","PL",&cs);
      if( cs == 'P' ) deltall(d, 0);
      } while (1);
   }

void korjaukset()
   {
   char ch;
   extern scr korjvalikko;

   korjfl = TRUE;
   do {
      clrscr();
      header=L"KORJAUKSET";
      kehys(1);
      prscr(&korjvalikko);
      listfl = TRUE;
      ch = ' ';
      selectopt("L)isää, K)orjaa, P)oista, J)uoksujärjestys, .. T)iedostosta  <Esc> : poistu",
                                                   "LKPEJT\x1B",&ch);
      switch(ch) {
         case 'L' : add(); break;
         case 'T' : jjfl = TRUE;
                    luejuoksujarj();
                    break;
#if defined(EMIT) && !defined(LUENTA)
		 case 'E' : if (emitfl)
						lueemitkoodit();
			        break;
#endif
         case 'J' : jjfl = TRUE;
         case 'K' : editkilp(); break;
         case 'P' : deletekilp(); break;
         }
      jjfl = FALSE;
      } while (ch != ESC);
   }

#if defined(MAALI) && !defined(LUENTA)

int maali_on = 0, nayta_sija, nayta_ero, nayta_nimi;

static void haevirhe(void)
   {
   kilptietue kilp;
   aikatp pt;
   int r,ptosuus, ptpiste;

   for (r = aikajono[akttm]->rtime+1; ; r++) {
      if (r - aikajono[akttm]->aktrows >= 0) r = 0;
      if (r - aikajono[akttm]->rtime == 0) {
         writeerror("Ei löydy", 0);
         break;
         }
	  aikajono[akttm]->getAktTime(&pt, r);
      if (pt.kno) {
         GETREC(&kilp, getpos(pt.kno));
         ptpiste = pt.piste;
         ptosuus = pt.osuus;
         if (abs(kilp.Maali(ptosuus, ptpiste) - purajak(pt.t)) > kilpparam.pyor[2]) {
            aikajono[akttm]->rtime = r;
            break;
            }
         }
      }
   }

static void naytarivi(int pt, int r, int r0,
   int r1, int k)
   {
   kilptietue kilp;
   int d, n,kno,tlen,tpos,rivpit,sn,l,ptosuus, ptpiste, sj;
   wchar_t line[82], nimi[80], as[20], *p;
   aikatp ptime;
   INT32 tls = 0;
#if defined(HALIKKO)
   int lmax;
   wchar_t *lsra, st[60];
#endif

   if (nayta_sija || nayta_nimi) {
      tlen = 8;
      tpos = 53;
      }
   else {
      tlen = kilpparam.laika;
      tpos = 54;
      }
   rivpit = nayta_nimi ? 80 : 65;
   wmemset(line, L' ', rivpit);
   if (pt >= 0) {
      n = pt;
	  aikajono[akttm]->getAktTime(&ptime, pt);
      ptpiste = ptime.piste;
      ptosuus = ptime.osuus;
      itow1(line,n+1,4);
      if (purajak(ptime.t) != TMAALI0)
         wmemcpy(line+5,AIKATOWSTRS(as,purajak(ptime.t),t0),kilpparam.laika);
      itow1(line+16, ptime.kno, 5);
      if (ptime.kno > 10000 && ptime.kno < 19998)
         kno = ptime.kno - 10000;
      else kno = ptime.kno;
#if !defined(HALIKKO)
      line[21] = L'-';
      if (tarkfl == 2) {
         line[22] = tarkno(kno)+L'0';
         line[23] = L'-';
         }
#endif
#ifdef EMITLEIMAT
      if (autokilp && seurkrivi == pt) {
         line[13] = L'\x10';
         if (ptime.kno < 10000) line[14] = L'\x10';
         }
#endif
      if ((d = getpos(kno)) > 0) {
          GETREC(&kilp, getpos(kno));
		  if (kilp.kilpno == kno) {
			  if (ptime.kno < 10000) {
				  wmemcpy(line + 21, wosuuskoodi(kilp.sarja, ptosuus, 0, 3), 3);
				  if (valiajat) {
					  if (ptpiste == 0)
						  line[25] = L'M';
					  else if (ptpiste == -1)
						  line[25] = L'L';
					  else if (ptpiste < -1)
						  line[25] = L'A';
					  else {
						  line[25] = L'0' + (ptpiste % 10);
						  if (ptpiste > 9)
							  line[24] = L'0' + (ptpiste / 10);
					  }
				  }
			  }
			  if (ptpiste > kilpparam.valuku || (ptpiste >= 0 && kilp.Maali(ptosuus, ptpiste) == TMAALI0)) {
				  //               kilp.Maali(ptosuus, 0) = ptime.t;
				  line[64] = L'*';
			  }
			  if (nayta_nimi) {
				  kilp.Nimi(nimi, OSNIMIL, ptosuus);
				  if (wcslen(nimi) > 15){
					  p = nimi + wcslen(nimi) - 1;
					  while (p > nimi && *(p - 1) != L' ') p--;
					  if (p > nimi) {
						  sn = wcslen(p);
						  sn = min(sn, 13);
						  wcscpy(nimi + 14 - sn, p - 1);
					  }
				  }
				  l = wcslen(nimi);
				  if (l > 15)
					  l = 15;
				  wmemcpy(line + 65, nimi, l);
			  }
			  l = wcslen(kilp.Seura(nimi));
#if !defined(HALIKKO)
			  if (l > 13)
				  l = 13;
			  wmemcpy(line + 28, kilp.Seura(nimi), l);
			  if (kilp.joukkue > 9)
				  line[42] = L'0' + kilp.joukkue / 10;
			  if (kilp.joukkue)
				  line[43] = (kilp.joukkue % 10) + L'0';
			  if (sarjaluku > 1) {
				  l = wcslen(Sarjat[kilp.sarja].Sarjanimi());
				  if (l > 8)
					  l = 8;
				  wmemcpy(line + 45, Sarjat[kilp.sarja].Sarjanimi(), l);
			  }
#else  // HALIKKO
			  if (kilp.maa[0]) {
				  wmemcpy(line+48, kilp.Maa(), wcslen(kilp.Maa()));
				  lmax = kilp.joukkue ? 21 : 23;
			  }
			  else
				  lmax = 25;
			  lsra = line + 24;
#if MAXOSUUSLUKU > 9
			  lmax--;
			  lsra++;
#endif
			  l = min(l,lmax);
			  wmemcpy(lsra, kilp.Seura(st), l);
			  if (kilp.joukkue > 9) {
				  lsra[l+1] = L'0' + kilp.joukkue/10;
				  lsra[l+2] = (kilp.joukkue % 10) + L'0';
			  }
			  else if (kilp.joukkue)
				  lsra[l+1] = kilp.joukkue + L'0';
#endif  // HALIKKO
			  if (ptime.kno > 10000)
				  wmemcpy(line + 54, L"NOLLAUS", 7);
			  else {
				  switch (kilp.Tark(ptosuus, true)) {
				  case L'H':
					  wmemcpy(line + 54, L"HYLÄTTY", 7);
					  break;
				  case L'K':
					  wmemcpy(line + 54, L"KESKEYT", 7);
					  break;
				  case L'P':
				  case L'E':
					  wmemcpy(line + 54, L"EI LÄHT", 7);
					  break;
				  case L'S':
					  wmemcpy(line + 54, L"SULJ.  ", 7);
					  break;
				  default:
					  if (nayta_sija || nayta_nimi) {
						  sj = kilp.Sija(ptosuus, ptpiste);
						  putintrightw(sj, line + 63);
					  }
					  if (ptpiste <= kilpparam.valuku)
						  tls = tulos(&kilp, ptosuus, ptpiste);
					  if (nayta_ero && sj > 1 && ptpiste <= kilpparam.valuku && tls > kilpparam.maxnosuus*SEK) {
						  tls = tls - KarkiTulos(kilp.sarja, Sarjat[kilp.sarja].yosuus[ptosuus], ptpiste);
					  }
					  wmemcpy(line + tpos, AIKATOWSTRS(as, tls, 0), tlen);
				  }
				  if (ptpiste > kilpparam.valuku || (ptpiste > -2 && 
					  abs(NORMKELLO(kilp.Maali(ptosuus, ptpiste) - purajak(ptime.t))) > kilpparam.pyor[2]))
					  line[64] = '*';
			  }
		    }
         }
      else if (ptime.kno >= 10000) {
         switch (ptime.kno) {
            case 10000: wmemcpy(line+28,L"TAHDISTUSAIKA", 13);
                        break;
            case 19998: wmemcpy(line+28,L"NOLLAUS - YLIMÄÄRÄINEN AIKA", 27);
                        break;
            case 19999: wmemcpy(line+28,L"NOLLAUKSEN LOPPU", 16);
                        break;
			default:
				if (ptime.kno > 30000 && ptime.kno <= 30000 + sarjaluku) {
					wmemcpy(line + 16, L" S", 2);
					wmemcpy(line + 23, L"1 L  Sarjalähtö: ", 17);
					wmemcpy(line + 40, Sarjat[ptime.kno - 30001].Sarjanimi(), wcslen(Sarjat[ptime.kno - 30001].Sarjanimi()));
					}
				if (ptime.kno > 31000 && ptime.kno < 32000) {
					wmemcpy(line + 16, L" R", 2);
					wmemcpy(line + 23, L"1 L  Erälähtö: ", 15);
					line[39] = L'0' + (ptime.kno % 1000) / 100;
					line[40] = L'0' + (ptime.kno % 100) / 10;
					line[41] = L'0' + (ptime.kno % 10);
				}
		 }
         }
      else if (purajak(ptime.t) != TMAALI0) {
		  if (ptpiste == 0)
			  line[25] = L'M';
		  else if (ptpiste == -1)
			  line[25] = L'L';
		  else if (ptpiste < -1)
			  line[25] = L'A';
		  else {
			  line[25] = L'0' + (ptpiste % 10);
			  if (ptpiste > 9)
				  line[24] = L'0' + (ptpiste / 10);
		  }
		  swprintf(line + 26, L" Bdg:%8d Port:%4d Lähde:%4d", ptime.badge, (int)ptime.kanava, (int)ptime.lahde);
//         line[wcslen(line+27)] = L' ';
         }
      }
   if (r == r1+2) {
      line[4] = 16;
      line[13] = 17;
      }
   if (r == r0+2) {
      switch (k) {
         case 1:
            viwrrectw(r,0,r,16,line,0,7,0);
            viwrrectw(r,17,r,25,line+17,7,0,0);
            viwrrectw(r,26,r,rivpit-1,line+26,0,7,0);
            break;
         case 2:
            viwrrectw(r,0,r,4,line,0,7,0);
            viwrrectw(r, 5, r, 14, line+5, 7,0,0);
            viwrrectw(r,15,r,rivpit-1,line+15,0,7,0);
            break;
         }
      }
   else
      viwrrectw(r,0,r,rivpit-1,line,7,0,0);
   }

static void tulostamaali(void)
   {
   static INT alku[MAXJONO] = {1, 1};
   static INT rivi[MAXJONO];
   wchar_t tc, line[68],*ss, tark, as[20], *p;
   int ptime;
   int d;
   kilptietue kilp;
   int ptpiste, ptosuus;
   aikatp ptm;

	FILE *outfl;
	wchar_t ln[120], st[80];

	tc = L' ';
	wselectopt(L"T)iedostoon vai K)irjoittimelle", L"TK", &tc);
	if (tc == L'T') {
	   clrln(ySize-3);
		vidspwmsg(ySize-3, 0, 7, 0, L"Tiedostoon:");
		wcscpy(line, L"ajat.txt");
		inputwstr(line, 30, 13, ySize-3, L"\x1B\r", &tc, 0);
		if (tc == KB_CR) {
			if ((outfl = _wfopen(line, L"wt")) != NULL) {
		      for (ptime = 0; ptime - aikajono[akttm]->aktrows < 0; ptime++) {
					rivi[akttm]++;
					aikajono[akttm]->getAktTime(&ptm, ptime);
				   ptpiste = ptm.piste;
					ptosuus = ptm.osuus;
		         if (purajak(ptm.t) > TMAALI0)
						AIKATOWSTRS(line, purajak(ptm.t), t0);
					else
						wcscpy(line, L"  .  .  , ");
					swprintf(ln, L"%5d\t%6d\t%10.10s\t%5d\t%d\t%d\t%d\t%d\t%d\t%d",rivi[akttm], ptm.date, line, ptm.kno, ptosuus, 
						ptpiste, ptm.badge, (int)ptm.kanava, (int)ptm.lahde, (int)ptm.jono);
		         if (ptm.kno >= minkilpno && ptm.kno <= maxkilpno
				      && (d = getpos(ptm.kno)) != 0) {
						GETREC(&kilp, d);
		            if (ptpiste > kilpparam.valuku || (ptpiste > -2 && kilp.Maali(ptosuus, ptpiste) != purajak(ptm.t)))
				         tark = L'*';
						else tark = L' ';
						if (ptpiste >= 0 && ptpiste <= kilpparam.valuku)
							AIKATOWSTRS(line, tulos(&kilp,ptosuus,ptpiste),0);
						else
							wcscpy(line, L"00.00.00,0");
						swprintf(ln+wcslen(ln), L"\t%s-%d\t%s\t%c\t%c", kilp.Seura(st), kilp.joukkue, line, 
							kilp.wTark(ptosuus, true), tark);
						}
					wcscat(ln, L"\n");
					fputws(ln, outfl);
					}
				fclose(outfl);
				}
			}
		return;
		}
   clrln(ySize-3);
   viwrrect(ySize-3,0,ySize-3,39,"Alkaen numerosta :         Esc : Peruuta",7,0,0);
   INPUTINTW(&alku[akttm],4,19,ySize-3,L"\x1B\xD",&tc);
   if (tc != ESC) {
      tc = 1;
	  if (lsttulparam.printer == LJETII) 
		 p = initfont_emva;
	  if (lsttulparam.printer == GDIPRINTER)
		 p = L"C";
	  initpage(&lsttulparam, p);
      for (ptime = alku[akttm]-1; ptime - aikajono[akttm]->aktrows < 0; ptime++, alku[akttm]++) {
		  aikajono[akttm]->getAktTime(&ptm, ptime);
         ptpiste = ptm.piste;
         ptosuus = ptm.osuus;
         if (rivi[akttm] == 0 || tc == 1) {
            tc = 2;
            sendwline(L"Tulostettu ",lsttulparam.lstf);
            sendwline(wkello(),lsttulparam.lstf);
            sendwline(L"\r\n\n",lsttulparam.lstf);
            rivi[akttm]++;
            rivi[akttm]++;
            }
         wcscpy(line, L"      ");
         itow1(line,alku[akttm],4);
         sendwline(line,lsttulparam.lstf);
			ss = L"";
         if (purajak(ptm.t) > TMAALI0) {
            ss = AIKATOWSTRS(as, purajak(ptm.t), t0);
            sendwline(ss,lsttulparam.lstf);
            }
         if (ptm.kno >= minkilpno && ptm.kno <= maxkilpno
            && (d = getpos(ptm.kno)) != 0) {
			 wchar_t ps[6] = L" A";
            GETREC(&kilp, d);
            if (ptpiste > kilpparam.valuku || (ptpiste > -2 && kilp.Maali(ptosuus, ptpiste) != purajak(ptm.t)))
               tark = L'*';
            else tark = L' ';
				if (ptpiste >= 0 && ptpiste <= kilpparam.valuku)
					ss = AIKATOWSTRS(as, tulos(&kilp,ptosuus,ptpiste),0);
				if (ptpiste <= 0)
					ps[1] = L"AALM"[ptpiste + 3];
				else
					_itow(ptpiste, ps, 10);
            swprintf(line, L" %c %4d %3d%2s %-28.28s %-3.3s %s%c",
               kilp.wTark(ptosuus, true), ptm.kno, ptosuus+1, ps, 
			   kilp.Joukkue(st, 28),  kilp.Maa(as), ss, tark);
            sendwline(line,lsttulparam.lstf);
            }
         else if (ptm.badge) {
            swprintf(line, L"Emit-koodi %7ld, Lähde:%4hd", ptm.badge, ptm.lahde);
            sendwline(line,lsttulparam.lstf);
            }
         sendwline(L"\r\n",lsttulparam.lstf);
         rivi[akttm]++;
         if (rivi[akttm] >= lsttulparam.tulmuot.sivpit) {
			endpage(&lsttulparam);
            rivi[akttm] = 0;
            }
         }
      tc = L'K';
      if (rivi[akttm] < lsttulparam.tulmuot.sivpit - 8)
		 endpage(&lsttulparam);
      rivi[akttm] = 0;
 	  enddoc(&lsttulparam);
	  LeaveCriticalSection(&print_CriticalSection);
      }
   }

static int jakorivi;

void naytamaali(int *r1, int *r0, int k)
   {
   int perusrivi;
   int r2, r02, n1, n2, r, r_alin = ySize-5;
   static int k0 = 1;

   perusrivi = (jakorivi-2)/2;
   if (ennakoi && ol_osuus > 0 && ol_osuus <= kilpparam.osuusluku)
      r_alin = ySize-6;
   if (nayta_nimi) {
      draw_hline2(1,66,14);
      draw_grchar2(1,65,7);
      draw_hline2(ySize-4,65,1);
      }
   if (k) k0 = k;
   *r1 = aikajono[akttm]->rtime;
   r2 = aikajono[akttm]->aktrows;
   if (r2 < *r1 + r_alin - perusrivi - 1) {
      *r0 = max(1, r2 - r_alin + 3) - 1;
//      r02 = *r0 + 10;
      n1 = r2 - *r0 + 1;
      n2 = 0;
      for (r = n1+2; r < r_alin + 1; r++) 
         naytarivi(-1,r,-1,-1,k0);
      }
   else {
      *r0 = max(0, *r1 - perusrivi);
      r02 = r2 - r_alin + jakorivi + 1;
      n1 = jakorivi - 2;
      n2 = r_alin - jakorivi;
      naytarivi(-1, jakorivi,-1,-1,k0);
      }
   for (r = 0; r < n1; r++) 
      naytarivi(*r0 + r, r+2 , *r1-*r0, r2-*r0,k0);
   for (r = 0; r < n2; r++) 
      naytarivi(r02 + r, r+jakorivi+1, *r1-*r0, r2-r02+jakorivi-1,k0);
   }

#ifdef TESTIAJAT
void lahetatestiajat(void)
   {
   char ch = ' ';
   int d, aikaaskel = 0, r1, r2, osuus, piste = 0;
   kilptietue kilp, entkilp;
   aikatp rtm;

   if (testi != 1)
      return;
   selectopt("Lähetä A)ikatiedon hetkellä, V)akiovälein", "AV", &ch);
   if (ch == 'V') {
      clrln(ySize-3);
      vidspmsg(ySize-3, 0, 7, 0, "Anna aikaväli sekunnin kymmenyksinä");
      INPUTINT(&aikaaskel, 5, 37, ySize-3, "\r", &ch);
      clrln(ySize-3);
      vidspmsg(ySize-3, 0, 7, 0, "Paina Esc keskeyttääksesi tallennus");
      }
   for (; aikajono[akttm]->rtime < aikajono[akttm]->aktrows; aikajono[akttm]->rtime++) {
	   aikajono[akttm]->getAktTime(&rtm, aikajono[akttm]->rtime);
		if (va_mpiste[akttm] > 0)
			piste = va_mpiste[akttm];
		else
			piste = rtm.piste;
      osuus = rtm.osuus;
      if(rtm.kno >= minkilpno && rtm.kno <= maxkilpno && rtm.osuus >= 0 && 
         (d = getpos(rtm.kno)) > 0) {
         if (piste > 0) {
            GETREC(&kilp, d);
            memcpy(&entkilp, &kilp, sizeof(kilptietue));
			kilp.setMaali(osuus, piste, pyoristatls(purajak(rtm.t), 1));
            }
         while (!aikaaskel && ((purajak(rtm.t)/KSEK - T_TIME(biostime(0,0)) + 12*36000L)%(24*36000L) > 12*36000L)) {
            Sleep(50);
            if (readkbd(&ch, 0, 0) == ESC)
               return;
            }
         tallenna(&kilp, d, 0, 0, osuus+1, &entkilp);
         }
      naytamaali(&r1, &r2, 0);
      if (readkbd(&ch, 0, 0) == ESC)
         return;
      if (aikaaskel)
         Sleep(100*aikaaskel);
      }
   }
#endif

void aikakorj(void)
	{
	char ch;

	clrln(ySize-3);
	vidspmsg(ySize-3, 0, 7, 0, "Anna aikakorjaus        (1/10 s)");
	inputlong(&aikajono[akttm]->aikakorjaus, 5, 17, ySize-3, "\r\x1b", &ch);
	}

void ajanottoheader(int akttm, int ol_osuus, int va_mpiste)
{
	static wchar_t headerline[32];

	wcscpy(headerline, L"AJANOTTO");
	if (maxjono) {
		wcscat(headerline, L" ");
		_itow(akttm+1, headerline+9, 10);
		}
	if (ol_osuus) {
		swprintf(headerline+wcslen(headerline), L" - Os %d", ol_osuus);
		}
	if (va_mpiste == 0)
		wcscat(headerline, L" - Maali");
	else if (va_mpiste > 0)
		swprintf(headerline+wcslen(headerline), L" - Piste %d", va_mpiste);
	else if (va_mpiste == -1)
		wcscat(headerline, L" - Lähtö");
	else
		wcscat(headerline, L" - Autopiste");
	if (wcslen(headerline) < 30)
		wmemset(headerline+wcslen(headerline), L' ', 30-wcslen(headerline));
	header = headerline;
//	clrtxt(0,0,25);
	vidspwmsg(0,0,7,0,header);
}

static int lahestyjat_on;
extern int writeerrorOn;

void ajanotto()
   {
   int r, r1, r0, i, k, kno, key, ekno, d, de, uusi, os, os0, x[2] = {22,5};
   int edkno,edosuus, eitulos;
   wchar_t tc;
   char tce, ch, ch2, line[81], as[20];
   wchar_t wline[80], wst[80];
   kilptietue kilp, entkilp;
   TMAALI tm,tm0;
   int edaika, htime;
   aikatp utime, vert_time, rtm, ttm;
   static int askel=1;
   int haku = 0;
   int piste = 0;
   int piste0 = 0;
	int vapiste = 0;
#ifdef ENNAKOI
   int njono, jkno = 0, jono[5], j;
   int maxennjono = 5;
#endif
//   char *p;

   clrscr();
   header = L"AJANOTTO";
   kehys(1);
   jakorivi=3*ySize/4-7;
   maali_on = 1;
   status_on = !nayta_nimi;
   if (nayta_nimi) for (r = 2; r < ySize-4; r++) clrln(r);
   if (aikajono[0] == NULL || aikajono[0]->atime == NULL) 
	   inittimer();
   if (aikajono[0] == NULL || aikajono[0]->atime == NULL) {
      writeerror(
         "Ajanottotoiminta ei käytettävissä, tarkista käynnistysparametrit", 0);
      return;
      }
   if (tarkfl < 2) x[0] -= 2;
   k = 1;
   do {
	   if (!taustaon)
		   return;
	   if (lahestyjat_on)
		  continue;
	  eitulos = 0;
      tc = 0;
//      tall_osuus = -1;
      viwrrect(ySize-3,0,ySize-2,75,
         "Up Dn PgUp PgDn Alt-R: vaihda riviä, "
         "Tab : vaihda kenttää, Ret : hyväksy,   "
         "F2/F3 : lisää/poista rivi, F4 : testitarra"
         ", F6/F8 : ota seur./edell. kilpailija,",7,0,0);
	  if (writeerrorOn == 0)
		  viwrrect(ySize-1,0,ySize-1,38,"F10 : tulosta, Esc : peruuta ja poistu ",7,0,0);
	  vapiste = va_mpiste[akttm];
	  ajanottoheader(akttm, ol_osuus, va_mpiste[akttm]);
      naytamaali(&r1, &r0, k);
      Sleep(100);
	  if (!taustaon)
		  return;
      sccurset(r1-r0+2, x[k-1]);
      muut_time = aikajono[akttm]->mxtime;
	  aikajono[akttm]->getAktTime(&vert_time, aikajono[akttm]->rtime);
      od_fl = 0;
      if (kbready(&tce,&key) && (tc = tce) != ESC) {
		  tc = tce;
         switch (k) {
            case 1 :
               uusi = 0;
               do {
                  d = 0;
				  aikajono[akttm]->getAktTime(&rtm, aikajono[akttm]->rtime);
				  if (vapiste < -1)
					  vapiste = rtm.piste;
#ifdef ENNAKOI
                  if (jkno) kno = jkno;
                  else
#endif
                     {
                     kno = rtm.kno;
                     if (kno) 
						 os = rtm.osuus;
                     else 
						 os = 0;
                     }
                  od_fl = purajak(vert_time.t) == TMAALI0;
                  od_aika = TMAALI0*AIKAJAK;
                  if ((tc >= L'0' && tc <= L'9') || tc == 8 ||
                     (tc == 0 && key == 75)) {
                     if (tarkfl == 2)
                        inputknow(&kno, 17, r1-r0+2, L"*\r\t\x1B\x0B\xCA\xCB\xCC"
                           L"\x05\x18\x12\x03\xCE\xD0\xD2\x04\x15\x02\x13",&tc);
                     else
                        input_now(&kno, 17, r1-r0+2, L"*\r\t\x1B\x0B\xCA\xCB\xCC"
                           L"\x05\x18\x12\x03\xCE\xD0\xD2\x04\x15\x02\x13",&tc);
                     }
                  else {
                     tc = tulk(tc, key);
                     kbflush();
                     }
                  if (tc == 248) {					// Alt-Y
					  if (emitfl && rtm.badge != 0) {
	                     for (int ii = 9; ii >= 0; ii--) {
							if (bdg2kno(rtm.badge+ii*BADGEASKEL, 0) > 0) {
								kno = bdg2kno(rtm.badge+ii*BADGEASKEL, 0);
								vidint(r1-r0+2, 17, 4, kno);
								tc = KB_CR;
								break;
								}
						    }
						}
					 if (kno == 0) {
						 tc = 0;
						 goto pois;
						}
                     }
				  if (tc == L'S') {
					  int srj1;
					  clrln(ySize - 2);
					  clrln(ySize - 1);
					  vidspwmsg(ySize - 3, 0, 7, 0, L"Anna lähtöajan sarj                                       ");
					  srj1 = luesarja(0, &ch);
					  if (ch != ESC && srj1 >= 0) {
						  ch = ' ';
						  selectopt("Sarjan kaikkien joukkueiden lähtöaika vaihdetaan. Vahvista (K/E)", "KE", &ch);
						  if (ch == 'K') {
							  utime = vert_time;
							  asetaSrjLahto(srj1, purajak(utime.t), false);
							  utime.kno = 30001 + srj1;
							  kno = utime.kno;
							  utime.piste = -1;
							  tc = 0;
							  utime.kanava = vert_time.kanava;
							  utime.jono = vert_time.jono;
							  utime.date = tm_copydate(utime.t, vert_time.t, vert_time.date);
							  tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0);
						  }
					  }
					  continue;
				  }
				  if (tc == L'R') {
					  int ryh = 0;

					  vidspwmsg(r1 - r0 + 2, 17, 7, 0, L"     Eränumero lähtöajalle     ");
					  inputlong(&ryh, 4, 17, r1 - r0 + 2, "+\r\x1b", &ch);
					  if (ch != ESC && ryh > 0) {
						  ch = ' ';
						  selectopt("Erän kaikkien joukkueiden lähtöaika vaihdetaan. Vahvista (K/E)", "KE", &ch);
						  if (ch == 'K') {
							  utime = vert_time;
							  asetaEraLahto(ryh, purajak(utime.t));
							  utime.kno = 31001 + ryh;
							  kno = utime.kno;
							  utime.piste = -1;
							  tc = 0;
							  utime.kanava = vert_time.kanava;
							  utime.jono = vert_time.jono;
							  utime.date = tm_copydate(utime.t, vert_time.t, vert_time.date);
							  tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0);
						  }
					  }
					  continue;
				  }
				  if (tc == L'T') {                         // T
                     kno = 10000;
                     tc = KB_CR;
                     }
                  if (tc == 249) {                         // Alt-D
					 tc = 0;
                     goto pois;
                     }
                  if (tc == 231) {                         // Alt-E
                     nayta_ero = !nayta_ero;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 232) {                         // Alt-S
                     nayta_sija = !nayta_sija;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 233) {                         // Alt-N
                     if (vert_time.kno) {
                        if ((d = getpos(vert_time.kno)) > 0) {
                           GETREC(&kilp, d);
                           outkilp(&kilp,vert_time.osuus);
                           readkbd(&ch,1,spoolfl);
                           }
                        }
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 234) {                         // Alt-H
                     clrln(ySize-3);
                     vidspmsg(ySize-3,0,7,0,"Anna haettava numero:");
                     INPUTINT(&haku, 4, 22, ySize-3, "\r", &ch);
                     }
                  if (tc == 244)                           // Alt-0
                     haku = 0;
                  if (tc == 234 || tc == 243 || tc == 244) {  // Alt-H Alt-G Alt-0
                     htime = aikajono[akttm]->rtime+1;
                     if (htime - aikajono[akttm]->aktrows >= 0) htime = 0;
                     for (; htime - aikajono[akttm]->aktrows < 0; htime++)
                        if (haku == aikajono[akttm]->getAktTime(&ttm, htime)->kno)
							break;
                     if (haku != ttm.kno)
                        for (htime = 0; htime - aikajono[akttm]->rtime <= 0; htime++)
							if (haku == aikajono[akttm]->getAktTime(&ttm, htime)->kno)
								break;
                     if (haku != ttm.kno)
                        writeerror("EI LÖYDY",0);
                     else aikajono[akttm]->rtime = htime;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 245) {                         // Alt-X
                     haevirhe();
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 235) {                         // Alt-R
                     clrln(ySize-3);
                     d = aikajono[akttm]->rtime+1;
                     vidspmsg(ySize-3,0,7,0,"Anna rivi:");
                     INPUTINTW(&d, 4, 11, ySize-3, L"\r", &tc);
                     d = max(d, 1);
                     aikajono[akttm]->rtime = d - 1;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 236 || tc == 238) {         // Alt-F6, Alt-F8
                     clrln(ySize-3);
                     vidspmsg(ySize-3,0,7,0,"Anna siirtoaskeleen pituus:");
                     INPUTINTW(&askel, 2, 28, ySize-3, L"\r", &tc);
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 239) {                         // Alt-O
                     if (os_raja == -1)
                        tc = L'E';
                     else if (os_raja == -2)
                        tc = L'A';
                     else if (ol_osuus)
                        tc = L'V';
                     else
                        tc = L' ';
                     wselectopt(L"V)alittu osuus, Y)läraja, A)nkkuri, E)i ankkuri, "
                        L"P)oista rajoitus", L"VAYEP", &tc);
                     clrln(ySize-3);
                        vidspmsg(ySize-3,0,7,0,"Anna osuus:");
                     switch (tc) {
                        case L'V' :
                           INPUTINTW(&ol_osuus, 3, 13, ySize-3, L"\r", &tc);
                           os_raja = 0;
                           break;
                        case L'Y' :
                           if (os_raja < 0) os_raja = 0;
                           INPUTINTW(&os_raja, 3, 13, ySize-3, L"\r", &tc);
                           ol_osuus = 0;
                           break;
                        case L'E' :
                           os_raja = -1;
                           ol_osuus = 0;
                           break;
                        case L'A' :
                           os_raja = -2;
                           ol_osuus = 0;
                           break;
                        case L'P' :
                           os_raja = 0;
                           ol_osuus = 0;
                           break;
                        }
					 if (ol_osuus == 0) {
						 }
                     tc = 0;
                     goto pois;
                     }
                  if (valiajat && tc == 241) {           // Alt-P
                     clrln(ySize-3);
                     vidspmsg(ySize-3,0,7,0,"Anna käsiteltävä piste:    A: Automaattinen");
					 INPUTINTW(&va_mpiste[akttm], 2, 24, ySize - 3, L"aAlLmM\r", &tc);
                     if (towupper(tc) == L'M') 
						 va_mpiste[akttm] = 0;
					 if (towupper(tc) == L'L')
						 va_mpiste[akttm] = -1;
					 if (towupper(tc) == L'A')
						 va_mpiste[akttm] = -2;
					 if (va_mpiste[akttm] != -2 || rtm.piste == -2)
						vapiste = va_mpiste[akttm];
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 246) {                       // Alt-J
                     nayta_nimi = !nayta_nimi;
                     status_on = !nayta_nimi;
                     if (nayta_nimi) for (r = 2; r < ySize-4; r++) clrln(r);
                     else kehys(1);
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 206 || tc == 208) {          // F6, F8
                     viwrrect(ySize-1,0,ySize-1,41,
                        "Siirtoaskel =      (Alt-F6 : Vaihda askel)",7,0,0);
                     vidint(ySize-1, 14, 2, askel);
                     }
                  if (tc == 251) {					// Alt-K
                     aikakorj();
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 252) {					// Alt-L
					 clrscr();
				     header = L"LÄHESTYJÄT";
					 kehys(1);
                     lahestyjat_on = 1;
                     continue;
                     }
				  if (tc == 261) {			// Ctrl-O
					  utime = vert_time;
					  utime.status = 1;
					  tall_rivi(akttm, &utime, &vert_time, NULL, 0, 0, 0);
					  tc = 0;
					  goto pois;
				  }
				  if (tc == 262) {			// Ctrl-T
					  utime = vert_time;
					  utime.status = 0;
					  tall_rivi(akttm, &utime, &vert_time, NULL, 0, 0, 0);
					  tc = 0;
					  goto pois;
				  }
				  if (tc == 263) {			// Ctrl-Q
					  aikajono[akttm]->atimemask = aikajono[akttm]->atimemask ? 0 : '\xff';
					  aikajono[akttm]->haeAktAjat();
					  tc = 0;
					  goto pois;
				  }
				  if (tc == 247) {					// Alt-A
                     if (maxjono) {
                        akttm = (akttm + 1) % (maxjono+1);
						ajanottoheader(akttm, ol_osuus, va_mpiste[akttm]);
                        }
					 tc = 0;
                     goto pois;
					}
                  if (tc == 253) {					// Alt-I
                     if (maxjono) {
						clrln(ySize-3);
						vidspmsg(ySize-3, 0, 7, 0, "Anna jonon numero");
						i = akttm+1;
						INPUTINTW(&i, 2, 24, ySize-3, L"\r", &tc);
						if (i > 0 && i <= maxjono+1)
							akttm = i-1;
						ajanottoheader(akttm, ol_osuus, va_mpiste[akttm]);
                        }
					 tc = 0;
                     goto pois;
					}
#ifdef TESTIAJAT
                  if (tc == 20) {                        // Ctrl-T
                     lahetatestiajat();
                     tc = 0;
                     goto pois;
                     }
#endif
#ifdef EMITLEIMAT
                  if (tc == 22) {                        // Ctrl-V
                     seurkrivi = aikajono[akttm]->rtime;
                     tc = 0;
                     goto pois;
                     }
#endif
                  if (!esta_tall && tc == 206) {         // F6
                     if ((aikajono[akttm]->rtime + askel - aikajono[akttm]->aktrows) <= 0) {
                        edaika = aikajono[akttm]->rtime+askel;
						kno = aikajono[akttm]->getAktTime(&ttm, edaika)->kno;
                        os = ttm.osuus;
                        edkno = kno;
                        edosuus = os;
                        }
                     else tc = 0;
                     }                                   // F8
                  else if (!esta_tall && tc == 208) { 
                     if (aikajono[akttm]->rtime < askel) {
                        tc = 0;
                        }
                     else
                        {
                        for (edaika = aikajono[akttm]->rtime-askel; edaika > 0 &&
							aikajono[akttm]->getAktTime(&ttm, edaika)->t == rtm.t; edaika--) ;
                        if (ttm.t == rtm.t) tc = 0;
                        else {
                           edkno = ttm.kno;
                           edosuus = ttm.osuus;
                           ch = 'K';
                           if (aikajono[akttm]->rtime - edaika > askel) {
                              ch = ' ';
                              selectopt("Edellinen maaliaika sama, poimitaanko"
                                 " viimeisin eroava (K/E)", "KE", &ch);
                              if (ch == 'E') tc = 0;
                              }
                           if (ch == 'K') {
                              kno = edkno;
                              os = edosuus;
                              }
                           }
                        }
                     }
                  else if (tc != 240) uusi = 1;
                  if(tc != KB_CR && tc != L'+' && tc != L'*' && tc != 206 &&
                     tc != 208 && tc != 240 && tc != L'V') {
                     if (tc == ESC) tc = 0;
                     goto pois;
                     }
                  if(kno < minkilpno || kno > maxkilpno) {
                     if (kno != 0 && kno != 10000) {
                        tc = L'\xff';
                        writeerror("Numero liian suuri tai pieni",0);
                        }
                     }
                  else {
                     if ((d = getpos(kno)) <= 0) {
                        tc = L'\xff';
                        writeerror("EI LÖYDY",2000);
                        }
                     }
                  } while (tc == 255);
				  if (kno >= 30000)
					  continue;
               if (kno == 10000) {
                  viwrrect(r1-r0+2, 22, r1-r0+2, 38, "TAHDISTUSAIKA    ",
                     0,7,0);
                  }
               else if (!esta_tall && kno != 0) {
#ifdef HALIKKO
                  if (rtm.kno && rtm.kno != kno) {
                     ch = 'E';
                     selectopt("Rivillä jo kilpailunumero - korvataanko (K/E)",
                        "KE", &ch);
                     if (ch == 'E') {
                        tc = 0;
                        break;
                        }
                     }
#endif
                  GETREC(&kilp, d);
                  memcpy(&entkilp, &kilp, sizeof(kilptietue));
                  if (tc != 206 && tc != 208 && tc != L'V') {
#if !defined(HALIKKO)
                     if (kilpparam.partio)
                        os = Sarjat[kilp.sarja].osuusluku-1;
                     else {
                        if (tc == 240) {                    // Alt-U
                            os = rtm.osuus;
							vapiste = rtm.piste;
//			                tall_osuus = 10000*(os + 1) + vapiste;
							}
                        else {
                           if (kno == rtm.kno)
                              os = rtm.osuus;
                           else  {
                              if (ol_osuus)
                                 os = ol_osuus - 1;
                              else if (os_raja == -2)
                                 os = Sarjat[kilp.sarja].osuusluku - 1;
                              else if (vapiste <= kilpparam.valuku) {
                                 for (os = 0; os < Sarjat[kilp.sarja].osuusluku-1 &&
                                    (kilp.Maali(os, vapiste >= 0 ? vapiste : 0) != TMAALI0 ||
                                    stschind(kilp.Tark(os, false), "KH") >= 0);
                                    os++);
                                    }
                                 }
						   if (!ol_osuus || muutoskysely) {
	                           vidspmsg(ySize-3,0,0,7,"Hyväksy tai valitse osuus.     "
		                          "<Esc>: Peruuta                               ");
			                   do {
								   int ossrj = 0;

								   if (kilp.sarja >= 0)
									   ossrj = kilp.sarja;
								   inputosuus(&os, ossrj, 3, (tarkfl == 2) ? 23 : 21,r1-r0+2,
										L"\r\t\x1B\x0B\xCC",&tc);
//				                  os++;
//					              INPUTINT(&os,2,(tarkfl == 2) ? 24 : 22,r1-r0+2,
//						             "\r\t\x1B\x0B\xCC",&tc);
//							      os--;
								  if (tc == ESC) {
									 tc = 0;
	                                 goto pois;
		                             }
			                      } while (os < 0 || os >= Sarjat[kilp.sarja].osuusluku);
							   }
                           }
                        }
                     }
                  swprintf(wline, L" %-14.14s %2d %-8.8s ", kilp.Seura(wst),
                     (int)kilp.joukkue, Sarjat[kilp.sarja].Sarjanimi());
                  viwrrectw(r1-r0+2, 26, r1-r0+2, 53, wline, 0,7,0);
                  vidspwmsg(r1-r0+2, (tarkfl == 2) ? 23 : 21, 7, 0, wosuuskoodi(kilp.sarja, os, 0, 3));
//				  vidint(r1-r0+2, (tarkfl == 2) ? 24 : 22, 2, os+1);
#else
                     }
#if MAXOSUUSLUKU < 10
                  sprintf(wline, L"%-21.21s %-2d%3.3s  ", kilp.Seura(wst),
                     (int)kilp.joukkue, kilp.Maa());
                  viwrrectw(r1-r0+2, 24, r1-r0+2, 52, wline, 0,7,0);
#else
                  swprintf(wline, L"%-24.24s %-2d", kilp.Seura(wst),
                     (int)kilp.joukkue);
                  viwrrectw(r1-r0+2, 25, r1-r0+2, 52, wline, 0,7,0);
#endif
#endif
                  os = min(os, Sarjat[kilp.sarja].osuusluku - 1);
				  if (vapiste < -1)
					  vapiste = haeseurpiste(&kilp, os);
                  if (kno == rtm.kno &&
                     rtm.osuus == os &&
                     rtm.piste == vapiste
                     )
                     uusi = 0;
                  if (uusi && vapiste >= -1 && vapiste <= Sarjat[kilp.sarja].valuku[os]) {
                     if (kilp.Maali(os, vapiste) != TMAALI0 ||
                        !kilp.Hyv(os)) {
                        ch = ' ';
                        erbeep();
                        switch (kilp.Tark(os, true)) {
                           case '-':
                           case 'I':
                           case 'T':
                              AIKATOSTRS(line, tulos(&kilp,os,0),0);
                              break;
                           case 'K':
                              strcpy(line, "KESKEYTT. ");
                              break;
                           case 'H':
                              strcpy(line, "HYLÄTTY   ");
                              break;
                           case 'E':
                              strcpy(line, "EI LÄHT.  ");
                              break;
                           case 'S':
                              strcpy(line, "SULJ.     ");
                              break;
                           }
                        viwrrect(r1-r0+2, 54, r1-r0+2, 53+kilpparam.laika, line,
                           0,7,0);
						eitulos = 0;
						if (muutoskysely) {
							if (kilp.Tark(os, false) == 'E') {
							   ch = 'K';
							   selectopt("Merkitty ei-lähteneeksi, kirjataanko"
								  " aika (K/E)", "KE", &ch);
							   }
							else
							   selectopt("Tulos jo annettu, muutetaanko (K/E)",
							   "KE", &ch);
							}
						else {
							ch = 'K';
							eitulos = 1;
							}
						if (ch == 'E') {
						   tc = 0;
						   break;
							}
                        kilp.SetTark(os, '-');
                        }
					 kilp.setMaali(os, vapiste, pyoristatls(purajak(rtm.t), 1));
                     }
                  viwrrect(r1-r0+2, 54, r1-r0+2, 53+kilpparam.laika,
                     AIKATOSTRS(as, tulos(&kilp,os,0), 0), 0, 7, 0);
                  }
               ch = ' ';
               if (!esta_tall && purajak(vert_time.t) == TMAALI0) {
                  vidspmsg(ySize-3,0,0,7, "Odottaa aikaa.     Paina <Esc> "
                     "keskeyttääksesi odotus.");
#ifdef ENNAKOI
                  njono = 0;
                  jkno = 0;
#endif
                  while (ch != ESC) {
                     if (purajak(od_aika) != TMAALI0 && vapiste >= -1 && vapiste <= kilpparam.valuku) {
                        viwrrect(r1-r0+2, 5, r1-r0+2, 4+kilpparam.laika,
                           AIKATOSTRS(as, purajak(od_aika), t0), 0,7,0);
						kilp.setMaali(os, vapiste, pyoristatls(purajak(od_aika), 1));
                        viwrrect(r1-r0+2, 54, r1-r0+2, 53+kilpparam.laika,
                           AIKATOSTRS(as, tulos(&kilp,os,0), 0), 0, 7, 0);
#ifdef ENNAKOI

                        if (!njono)
#endif
                           break;
                        }
#ifdef ENNAKOI
                     sccurset(ySize-4, 20+6*njono);
#endif
                     ch = 0;
                     ch = readkbd(&ch2,FALSE,spoolfl);
#ifdef ENNAKOI
					 if (ennakoi && ol_osuus > 0 && ol_osuus <= kilpparam.osuusluku) {
                        if (ch >= '0' && ch <= '9') {
                           jkno = 10*jkno + ch - '0';
                           }
                        if (ch == 8) jkno = jkno/10;
                        if (jkno && ch == '\r') {
                           if (njono < maxennjono) {
                              jono[njono] = jkno;
                              njono++;
                              }
                           jkno = 0;
                           }
                        for (j = 0; j < njono; j++)
                           vidint(ySize-4,17+6*j,4,jono[j]);
                        if (jkno) {
                           memset(line, ' ', 5);
                           putintright(jkno, line+3);
                           viwrrect(ySize-4, 17+6*njono, ySize-4, 20+6*njono, line, 0, 7, 0);
                           }
                        clrtxt(ySize-4, 17+6* (njono + (jkno!=0)) , 64);
                        if (purajak(od_aika) != TMAALI0) {
                           aikajono[akttm]->rtime = odota;
                           vert_time.t = od_aika;
						   vert_time.date = tm_date(vert_time.t);
						   utime = vert_time;
//                           memset(&utime, 0, sizeof(utime));
//                           utime.t = vert_time.t;
                           utime.kno = kno;
                           utime.osuus = os;
                           utime.piste = vapiste;
//						   utime.badge = vert_time.badge;
//                         utime.kanava = vert_time.kanava;
//                         utime.lahde = vert_time.lahde;
                           if (tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0))
                              break;
                           if(kno && kno < 10000 &&
                              (d = getpos(kno)) > 0 && vapiste >= -1 && vapiste <= kilpparam.valuku) {
                              GETREC(&kilp, d);
                              memcpy(&entkilp, &kilp, sizeof(kilptietue));
							  if (muutoskysely || kilp.Maali(os, vapiste) == TMAALI0) {
								  kilp.setMaali(os, vapiste, pyoristatls(purajak(utime.t), 1));
//			                      kilp.ostiet[os].kone = utime.kone;
				                  pkno = aikajono[akttm]->rtime + 1;
								  tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
		                          pkno = 0;
								  }
                              naytamaali(&r1, &r0, k);
                              if (njono) vidint(19,17,4,jono[0]);
                              tc = 255;
                              }
                           else {
                              tall_rivi(akttm, &vert_time, &utime, NULL, 1, 0, 0);
                              }
                           if ((++aikajono[akttm]->rtime - aikajono[akttm]->aktrows) > 0) 
							   aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
                           clrtxt(ySize-4, 11+6*(njono + (jkno!=0)) , 64);
                           if (njono) {
                              kno = jono[0];
                              if (kno < minkilpno || kno > maxkilpno)
                                 kno = 0;
                              os = ol_osuus-1;
                              vert_time.t = TMAALI0*AIKAJAK;
                              od_aika = TMAALI0*AIKAJAK;
                              njono--;
                              for (j = 0; j < njono; j++)
                                 jono[j] = jono[j+1];
                              }
                           }
                        }
#endif
                     }
                  }
               if (ch == ESC) break;
               if (tc == L'V') tc = L'+';
               if (!esta_tall && tc != L'+' && tc != 240 && tc != 255) {
                  ch = ' ';
                  selectopt(" + : Tallenna,  <Esc> : Peruuta","+\x1b", &ch);
                  }
               if (!esta_tall && (tc == L'+' || ch == '+' || tc == 240)) {
                  if ((tc == 206 || tc == 208) &&
					 aikajono[akttm]->getAktTime(0, edaika)->kno != edkno) {
                     writeerror("Siirto peruutettu muutosanoman takia",0);
                     break;
                     }
                  if (od_fl && odota) {
                     aikajono[akttm]->rtime = odota;
                     vert_time.t = od_aika;
					 vert_time.date = tm_date(vert_time.t);
                     vert_time.osuus = 0;
                     }
				  utime = vert_time;
//                  memset(&utime, 0, sizeof(utime));
//                  utime.t = vert_time.t;
                  utime.kno = kno;
                  utime.osuus = os;
                  utime.piste = vapiste;
//                  utime.kanava = vert_time.kanava;
//                  utime.lahde = vert_time.lahde;
                  ekno = rtm.kno;
                  if (ekno > 0) {
                     os0 = vert_time.osuus;
                     piste0 = vert_time.piste;
                     }
                  if (tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0,
                     tc == 240)) break;
                  if (ekno >= minkilpno && ekno <= maxkilpno &&
                     ekno != kno &&
                     (de = getpos(ekno)) > 0) {
                     GETREC(&kilp, de);
                     memcpy(&entkilp, &kilp, sizeof(kilptietue));
                     ch = 'K';
                     if (abs(NORMKELLO(kilp.Maali(os0, piste0) - purajak(utime.t))) > kilpparam.pyor[1]) {
                        ch = 'E';
                        }
                     if (ch == 'K') {
                        kilp.setMaali(os0, piste0, TMAALI0);
//                        if (piste0)
//                           tallva(TMAALI0, os0, piste0, ekno, 0, 0);
//                        else
                           {
                           pkno = aikajono[akttm]->rtime + 1;
//                           kilp.ostiet[os0].kone = utime.kone;
                           tallenna(&kilp,de,TRUE, 0, 0, &entkilp);
                           pkno = 0;
                           }
                        }
                     }
                  if (kno != 0 && kno != 10000 && purajak(utime.t) != TMAALI0) {
                     if((d = getpos(kno)) > 0 && vapiste >= -1 && vapiste <= kilpparam.valuku) {
                        GETREC(&kilp, d);
                        memcpy(&entkilp, &kilp, sizeof(kilptietue));
                        if (kno == ekno && os0 != os) {
                           ch = 'K';
                           if (abs(NORMKELLO(kilp.Maali(os0, piste0) - purajak(utime.t))) > kilpparam.pyor[1]) {
                              ch = 'E';
                              }
                           if (ch == 'K')
                              kilp.setMaali(os0, piste0, TMAALI0);
                           }
						kilp.setMaali(os, vapiste, pyoristatls(purajak(utime.t), 1));
                        pkno = aikajono[akttm]->rtime + 1;
						if (!eitulos)
							tallenna(&kilp,d,TRUE, 0, tc == 240 ? 10000*(os+1)+vapiste+1 : 0, &entkilp);
                        pkno = 0;
                        }
                     else {
                        tall_rivi(akttm, &vert_time, &utime, NULL, 1, 0, 0);
                        writeerror("EI LÖYDY!",2000);
                        tc = 0;
                        }
                     }
                  if ((tc == 206 || tc == 208) &&
					  aikajono[akttm]->getAktTime(&ttm, edaika)->kno != 0 &&
                     ttm.kno == edkno &&
                     ttm.osuus == edosuus) {
					  aikajono[akttm]->getAktTime(&vert_time, edaika);
                     memset(&utime, 0, sizeof(utime));
                     utime.t = vert_time.t;
					 utime.date = vert_time.date;
                     tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0);
                     }
                  }
               else tc = 0;
               break;
            case 2 :
               d = 0;
               if(rtm.kno >= minkilpno && rtm.kno <= maxkilpno) {
                  d = getpos(rtm.kno);
                  os = rtm.osuus;
                  piste = rtm.piste;
                  }
               else if (rtm.kno) {
                  writeerror("KILPAILUNUMEROA EI LÖYDY!",2000);
                  tc = 0;
                  break;
                  }
               if (d > 0) {
                  GETREC(&kilp, d);
                  memcpy(&entkilp, &kilp, sizeof(kilptietue));
                  }
               tm = purajak(rtm.t);
               tm0 = tm;
               if ((tc >= L'0' && tc <= L'9') || (tc == 0 && key == 65)
                  || (tc == 0 && key == 66) || (tc == 0 && key == 77))
                  INPUTAIKAW(&tm,t0,kilpparam.laika,5,r1-r0+2,
                     L"hHkK-\r\t\x1B\x0B\xCA\xCB\xCC\xD2\x05\x18\x12\x03"
                     L"\x02\x13",&tc);
               else {
                  tc = tulk(tc, key);
                  kbflush();
                  }
               if (tc == ESC) tc = 0;
               tc = towupper(tc);
               switch (tc) {
                  case L'K':
                     strcpy(line, "KESKEYTT. ");
                     break;
                  case L'H':
                     strcpy(line, "HYLÄTTY   ");
                     break;
                  case L'E':
                     strcpy(line, "EI LÄHT.  ");
                     break;
                  case KB_CR :
					  if (d > 0) kilp.setMaali(os, piste, pyoristatls(tm, 1));
                  case L'-':
                  case L'T':
                     if (d > 0) 
                        AIKATOSTRS(line, tulos(&kilp,os,0),0);
                     break;
                  default :
                     goto pois;
                  }
               if (d > 0 && (tc != KB_CR || kilp.Hyv(os)))
                  viwrrect(r1-r0+2, 54, r1-r0+2, 53+kilpparam.laika2, line, 0, 7, 0);
               ch = ' ';
               selectopt(" + : Tallenna,  <Esc> : Peruuta","+\x1b", &ch);
               if (ch == '+' && !esta_tall) {
                  switch (tc) {
                     case KB_CR :
                        memset(&utime, 0, sizeof(utime));
                        utime.t = tm*AIKAJAK;
						utime.date = tm_copydate(utime.t, vert_time.t, vert_time.date);
                        utime.kno = vert_time.kno;
                        utime.osuus = vert_time.osuus;
                        utime.kanava = vert_time.kanava;
                        utime.lahde = vert_time.lahde;
                        if (tall_rivi(akttm, &utime, &vert_time, 0, 1, 0, 0))
                           break;
                        if( d > 0 && abs(NORMKELLO(kilp.Maali(os, piste) - tm0)) > kilpparam.pyor[1]) {
                           pkno = aikajono[akttm]->rtime + 1;
//                           kilp.ostiet[os].kone = utime.kone;
//                           if (piste && tm0 != tm) {
//                              tallva(tm, os, piste, kilp.kilpno, 0, 0);
//                              }
//                           else
                              tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
                           pkno = 0;
                           }
                        break;
                     case L'H' :
                     case L'K' :
                     case L'E' :
                     case L'-' :
                     case L'I' :
                     case L'T' :
                        if (d > 0 && kilp.wTark(os, false) != tc) {
                           kilp.SetTark(os, tc);
                           tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
                           }
                        tc = KB_CR;
                        break;
                     }
                  }
               else tc = 0;
               break;
            }
         pois:
         switch (tc) {
            case 206: 
            case KB_CR :
            case L'*':
            case L'+':
            case 24 :
            case 240:
               aikajono[akttm]->rtime++;
               break;
            case 208:
               if (aikajono[akttm]->rtime-edaika == askel) aikajono[akttm]->rtime--;
               else aikajono[akttm]->rtime = edaika;
               break;
            case  5 :
               if (aikajono[akttm]->rtime > 0) aikajono[akttm]->rtime--;
               break;
            case  3 :
               aikajono[akttm]->rtime += jakorivi-3;
               break;
            case 18 :
               if(aikajono[akttm]->rtime > jakorivi-3) 
                  aikajono[akttm]->rtime -= jakorivi-3;
               else aikajono[akttm]->rtime = 0;
               break;
            case  2 :
               aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
               break;
            case 19 :
               aikajono[akttm]->rtime = 0;
               break;
            case  9 :
               k = k % 2 + 1;
               break;
            case 11 :
               k--;
               if (k < 1) k = 2;
               break;
            case 204:                           // F4
               testitarra();
               break;
            case 202:                           // F2
               if (!esta_tall) {
				   if (lisaa_edelle && (tm = purajak(aikajono[akttm]->getAktTime(0, aikajono[akttm]->rtime)->t)) != TMAALI0) {
                     tm -= 1;
                     }
				   else {
					   tm = aikajono[akttm]->getAktTime(0, aikajono[akttm]->rtime-1)->t + 1;
					   }
				  lisaa_aika(akttm, tm, 0, vert_time.t, vert_time.date);
                  }
               break;
            case 203:                           // F3
               if (!esta_tall) {
                  if (rtm.kno) 
                     writeerror("Poista ensin kilpailijanumero", 0);
                  else if (aikajono[akttm]->rtime - aikajono[akttm]->aktrows < 0) {
                     ch = ' ';
                     selectopt("Poistetaanko (K/E)", "KE", &ch);
                     if (ch == 'K') poista_aika(akttm, &vert_time);
                     }
                  }
               break;
            case 210:                           // F10
               tulostamaali();
               break;
            }
         if ((aikajono[akttm]->rtime - aikajono[akttm]->aktrows) > 0) 
			 aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
         if (tc != 9 && tc != 11 && tc != 204) k = 1;
         }
      } while(tc != ESC);
   maali_on = 0;
   }
#endif

void lahestyjat(void)
{
#if !defined(LUENTA)
	char tc;
	wchar_t wline[66], was[20], wst[30], wst1[80];
	int t, os, va, srj = 0, r, sj, p;
	kilptietue kilp;

	if (!lahestyjat_on) 
		return;
	if ((va_mpiste[akttm] < 0 || ol_osuus < 0 || (va_mpiste[akttm] == 1 && ol_osuus < 1)) && lahestyjat_on) {
		vidspmsg(ySize-3, 0, 7, 0, "Seurattavaa pistettä ei voida päätellä. Paina Esc           ");
		Sleep(2000);
		lahestyjat_on = 0;
		}
    if (readkbd(&tc, 0, spoolfl) == ESC) {
		lahestyjat_on = 0;
		}
	if (!lahestyjat_on) {
		clrscr();
		header = L"AJANOTTO";
		kehys(1);
		return;
		}
	vidspmsg(ySize-3, 0, 7, 0, "  Esc: palaa ajanottonäytölle                               ");
	if (va_mpiste[akttm] == 1) {
		os = ol_osuus - 2 ;
		if (os < 0)
			os = 0;
		va = 0;
		}
	else {
		os = ol_osuus-1;
		for (va = va_mpiste[akttm] ? va_mpiste[akttm] - 1 : kilpparam.maxvaluku; va > 0 && ntulosix[srj][os][va] == 0; va--);
		if (va == 0 && os > 0)
			os--;
		}
	r = 2;
	sj = 0;
	t = KSEK*T_TIME(biostime(0,0));
	p = jalku[srj][os][va ? va+4 : 0];
    for (; p > 0 && r < ySize - 4;) {
		sj++;
		GETREC(&kilp, p);
		p = JarjSeur(0, os, va, p);
		if (kilp.Maali(ol_osuus - 1, va_mpiste[akttm]) != TMAALI0)
			continue;
		AIKATOWSTRS(was, NORMTULOS(t-kilp.Maali(os, va)), 0); 
		swprintf(wst, L"%-.28s", kilp.Joukkue(wst1));
		swprintf(wline, L"%8.8s (%3d) %4d %-28.28s %-15.15s", was, sj, kilp.kilpno, 
			wst, kilp.Nimi(wst1, OSNIMIL, ol_osuus-1));
		vidspwmsg(r,0,7,0,wline);
		r++;
		}
	for (; r < ySize-4; r++)
		clrtxt(r, 0, 64);
#endif
}

