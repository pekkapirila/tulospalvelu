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

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bvideo.h>
#include <bkeybrd.h>
#include <bstrings.h>
#ifndef BIT32
#include <bios.h>
#include <bintrupt.h>
#include <binterv.h>
#include <bkeys.h>
#endif
#pragma hdrstop
#include <conio.h>
#include <dos.h>
#include <sys\stat.h>
#include "VDeclare.h"

//#define DBG_1
//   int emitloki = 1;

#ifndef MAXTUNNUS
#define MAXTUNNUS  899
#endif
#define RINNTUNNUS  5

// Leimantarkastuksen pohjana on taulukko kuvio, johon on tallennettu
// luettelo kaikista hyväksyttävistä rastitunnus-leimasinkoodi -yhdistelmistä
// Luettelo on aina järjestettynä rastitunnusten mukaiseen järjestykseen.
// Koodin arvo 1 tarkoittaa, että kaikki leimaukset hyväksytään.

typedef struct {
   INT32 tm;
   INT32 tl;
   UINT *va;
   INT *sj;
   } emitvatype;

typedef struct {
   INT rasti;
   char koodi;
   } ekditp;

INT32 vapaaemit = 1;
static ekditp *kuvio;
static INT nkuvio, maxnkuvio;
static INT emitosa, partiorap;
extern INT valinelen, vapagelen;
extern char sline[300];
extern INT32 uusi_emit;
extern INT tarkavoimet, luentatesti;

void naytatulos(int tulos);

INT haebdg(INT32 badge, int *toinen)
   {
	int ibdg = -1;

	// bsrchint palauttaa sijainnin kertovan negatiivisen arvon, ellei tarkkaa osumaa löydy
	// haebdg palauttaa tämän arvon ellei *toinen == -1, jolloin tarkkaa osumaa haetaan kummastakin 
	// jonosta ja palautettava arvo on >= 0 tai -1.

	if (toinen == NULL || *toinen <= 0) {
		ibdg = bsrchint(badge, bdg_kno[0], nbadge[0], sizeof(bdg_kno_tp), 0, 1);
		if (toinen == NULL || *toinen == 0 || ibdg >= 0) {
			if (toinen)
				*toinen = 0;
			return(ibdg);
			}
		}
   if (kilpparam.kaksibadge && (toinen != NULL || *toinen != 0)) {
		ibdg = bsrchint(badge, bdg_kno[1], nbadge[1], sizeof(bdg_kno_tp), 0, 1);
		if (*toinen == 1 || ibdg >= 0) {
			if (toinen)
				*toinen = 1;
			return(ibdg);
			}
		}
   return(-1);
   }

INT bdg2kno(INT32 badge, int *toinen)
   {
   INT k;

   k = haebdg(badge, toinen);
   if (k >= 0)
	   return(bdg_kno[toinen == 0 ? 0 : *toinen][k].kilpno);
   return(0);
   }

INT addbadge(INT32 badge, INT kno, INT d, int toinen, INT msgfl)
   {
   INT i;
   char msg[50];

   if (!d && (i = getpos(kno)) <= 0)
      return(1);
   i = haebdg(badge, &toinen);
   if (i >= 0) {
      if (bdg_kno[toinen][i].kilpno == kno) return(0);
      if (msgfl) {
         sprintf(msg, "Joukkueella %d jo Emit-koodi %ld",
            bdg_kno[toinen][i].kilpno, badge);
         writeerror(msg, 0);
         }
      return(2);
      }
   i = -i - 1;
   memmove((char *)(bdg_kno[toinen]+i+1), (char *)(bdg_kno[toinen]+i),
      (nbadge[toinen]-i) * sizeof(bdg_kno_tp));
   nbadge[toinen]++;
   bdg_kno[toinen][i].kilpno = kno;
   bdg_kno[toinen][i].badge = badge;
#ifdef DBG_1
   if (loki && emitloki) {
	  sprintf(msg, "addbadge %ld, kno %d, d %d\n",
         badge, kno, d);
      sendline(msg, lokifile);
      }
#endif
#ifdef TESTBDG
   bdgstatus();
#endif
   return(0);
   }

INT rembadge(INT kno, INT os, int toinen, INT d)
   {
   INT i;
   INT32 bdg;
   kilptietue kilp;
#ifdef DBG_1
   char msg[80];
#endif

   if (d)
      i = d;
   else
      if ((i = getpos(kno)) <= 0)
         return(1);
   GETREC(&kilp, i);
      {
      if ((bdg = kilp.ostiet[os].badge[toinen]) != 0) {
         i = haebdg(bdg, &toinen);
         if (i < 0 || i >= nbadge[toinen]) {
/*
            if (loki && emitloki) {
               sprintf(msg, "Ekoodia %ld ei indeksissä\n", kilp.badge[0]);
               sendline(msg, lokifile);
               }
*/
#ifdef TESTBDG
            bdgstatus();
#endif
            return(1);
			}
         }
      if (bdg_kno[toinen][i].kilpno != kno) {
/*
         if (loki && emitloki) {
            sprintf(msg, "Ekoodi %ld liitetty joukkueeseen %d, ei poistettu\n",
               kilp.badge[0], kilp.kilpno);
            sendline(msg, lokifile);
            }
*/
         return(0);
         }
//      for (j = 0; j < Sarjat[kilp.sarja].osuusluku; j++)
//         if (j != os && kilp.ostiet[j].badge[0] == bdg) return(0);
      nbadge[toinen]--;
      memmove((char *)(bdg_kno[toinen]+i), (char *)(bdg_kno[toinen]+i+1),
         (nbadge[toinen]-i) * sizeof(bdg_kno_tp));
      bdg_kno[toinen][nbadge[toinen]].badge = 0;
      bdg_kno[toinen][nbadge[toinen]].kilpno = 0;
#ifdef DBG_1
      if (loki && emitloki) {
         sprintf(msg, "rembadge %ld, kno %d, os %d, d %d\n",
            kilp.ostiet[os].badge[toinen], kno, os, d);
         sendline(msg, lokifile);
         }
#endif
      }
#ifdef TESTBDG
   bdgstatus();
#endif
   return(0);
   }

INT32 ensvapaa(void)
   {
   INT32 kdi;

   for (kdi = vapaaemit; ; kdi++) {
      if (bdg2kno(kdi, 0) < 1) break;
      }
   vapaaemit = kdi + 1;
   return(kdi);
   }

INT remjkbadge(INT kno, INT32 bdg)
   {
   INT i, d;
   INT32 eno;
   kilptietue kilp, entkilp;
#ifdef DBG_1
   char msg[80];
#endif

   if ((d = getpos(kno)) <= 0)
      return(1);
   GETREC(&kilp, d);
   entkilp = kilp;
   eno = ensvapaa();
   for (i = 0; i < Sarjat[kilp.sarja].osuusluku; i++) {
      if (kilp.ostiet[i].badge[0] == bdg)
         kilp.ostiet[i].badge[0] = eno;
      }
   tallenna(&kilp, d, 0, 0, 0, &entkilp);
   return(0);
   }

#ifdef BIT32
extern int valo_on;

void asetavalo(int tila)
   {
#ifndef __linux__
   DCB dcb;

   GetCommState(hComm[cn_regnly[NREGNLY-1]], &dcb);
	switch (tila) {
		case 0:    // pois
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			break;
		case 1:    // punainen
	      dcb.fRtsControl = RTS_CONTROL_DISABLE;
		   dcb.fDtrControl = DTR_CONTROL_ENABLE;
			break;
		case 2:    // vihrea
	      dcb.fRtsControl = RTS_CONTROL_ENABLE;
		   dcb.fDtrControl = DTR_CONTROL_DISABLE;
			break;
		case 3:    // molemmat
	      dcb.fRtsControl = RTS_CONTROL_DISABLE;
		   dcb.fDtrControl = DTR_CONTROL_DISABLE;
			break;
      }
   SetCommState(hComm[cn_regnly[NREGNLY-1]], &dcb);
#endif
   }
#endif

int haeVapaaKoodi(int badge)
{
	int askel;

	if (badge == 200)
		askel = 1;
	else {
		if (!esta_uusikaytto)
			return(badge);
		askel = BADGEASKEL;
		}
	for (int i = 1; i < kilpparam.osuusluku; i++) {
		if (bdg2kno(badge+i*askel, 0) <= 0)
			return(badge+i*askel);
		}
	return(0);
}


INT leimat(void)
   {
   char ch, line[80], *p, msg[80];
   wchar_t wline[80], wst[80], was[20];
   INT i, r, d, os = 0, os1, kno, kno1, fore, key;
   int aukkovaroitus = 1;
   static INT selaus, emithead0;
   INT32 badge = 0, bdg;
   static INT32 ed_emit, seurt, alkut, aikaaskel, enst;
   kilptietue kilp, entkilp;
   static char valtxt[] =   "R)aportti, selaa: eD)ell S)eur V)iim, "
      "tU)hoa tietue, kO)odit, Esc: Pois      ";
   static char valtxt_m[] = "R)aportti, selaa: eD)ell S)eur V)iim, "
      "tU)hoa tietue, M)TR, kO)odit, Esc: Pois";
   static char numval[] = "Numero : kilpailijavalinta, Tab : "
      "Leimauskortin valinta, X : Kilpailijan vaihto";
   static FILE *testfile;
   int r1 = 3, r2 =5, r3 = 7, r4 = 8, r5 = 10;
   if (kilpparam.osuusluku > 10) {
	  r1 = 2;
	  r2 =3;
	  r3 = 4;
	  r4 = 5;
	  r5 = 6;
	  }

   if (emitfl <= 0) {
      writeerror("Leimantarkastustoimintoa ei ole käynnistetty", 0);
      return(0);
      }
   if (ajat_on) status_on = 0;
   if (regnly[0] == 13 || regnly[1] == 13 || regnly[NREGNLY-1] == 13)
      strcpy(valtxt, valtxt_m);
   header = L"EMIT-TUNNISTUS";
   if (luentatesti) {
      clrscr();
      kehys(1);
      vidspmsg(r2,0,7,0,"Viivakoodi");
      vidspmsg(r3,0,7,0,"Joukkue");
      vidspmsg(r4,0,7,0,"Osuus");
      if (!testfile)
         testfile = fopen("EMITREK.LST", "rt");
      if (!testfile) {
         writeerror("Tiedoston EMITREK.LST avaaminen ei onnistu", 0);
         return(0);
         }
      if (luentatesti == 2) {
         ch = 'A';
         if (alkut) {
            ch = ' ';
            selectopt("J)atka testiä, A)loita testaus alusta", "JA", &ch);
            }
         if (ch == 'A') {
            ch = ' ';
            selectopt("A)loita heti ensimmäisestä kilpailijasta, V)alitse aloitushetki", "AV", &ch);
            if (ch == 'A') {
               ch = ' ';
               selectopt("T)iedoston EMITREK.LST mukaiset ajat, V)akioväliset ajat", "TV", &ch);
               if (ch == 'V') {
                  clrln(ySize-3);
                  vidspmsg(ySize-3, 0, 7, 0, "Anna aikaväli sekunnin kymmenyksinä");
                  INPUTINT(&aikaaskel, 5, 37, ySize-3, "\r", &ch);
                  }
               else
                  aikaaskel = 0;
               alkut = T_TIME(biostime(0,0));
               }
            else {
               ch = ' ';
               selectopt("T)iedoston EMITREK.LST mukainen aloitus, M)uu aloitusaika", "TM", &ch);
               if (ch == 'M') {
                  clrln(ySize-3);
                  vidspmsg(ySize-3, 0, 7, 0, "Anna ensimmäisen kilpailijan lukemisaika");
                  alkut = T_TIME(biostime(0,0));
                  INPUTAIKA(&alkut, t0, 8, 43, ySize-3, "\r", &ch);
                  }
               else
                  alkut = 0;
               aikaaskel = 0;
               }
            }
         }
      for (r = 0;; r++) {
         i = 1;
         clrtxt(r1,0,60);
         clrln(ySize-3);
         if (luentatesti != 2) {
            vidspmsg(ySize-3,0,7,0, "Odottaa Emit-koodia - Esc: keskeytä odotus");
            for (;;) {
               ch = readkbd(&ch, 0, spoolfl);
               if (ch == ESC) {
                  return(0);
                  }
               if (uusi_emit)
                  break;
#ifdef BIT32
               Sleep(50);
#endif
               }
            badge = uusi_emit;
            }
         clrln(ySize-3);
         bdg = -1;
         do {
            if (!fgets(line, 78, testfile))
               break;
            p = strtok(line, " ,;\t");
            if (!p)
               continue;
            kno = atoi(p);
            p = strtok(NULL, " ,;\t");
            if (!p)
               continue;
            os = atoi(p);
//			ansitowcs(wst, p, 3);
//			wst[3] = 0;
//			os = tulkOsuuskoodi(sarjaKno(kno), wst);
            p = strtok(NULL, " ,;\t");
            if (!p)
               continue;
            bdg = atol(p);
            if (luentatesti == 2) {
               p = strtok(NULL, " ,;\t");
               if (!p)
                  continue;
               if (!seurt && !enst && alkut)
                  enst = atol(p);
               seurt = (atol(p)-enst)/100 + alkut;
               badge = bdg;
               }
            } while (bdg != badge);
         if (bdg != badge) {
            fclose(testfile);
            writeerror("Kaikki tiedot luettu", 0);
            return(0);
            }
         if (luentatesti == 2) {
            badge = bdg;
            vidspmsg(ySize-3,0,7,0, "Esc : Keskeytä testi");
            for (;;) {
               ch = readkbd(&ch, 0, spoolfl);
               if (ch == ESC) {
                  return(0);
                  }
               if ((T_TIME(biostime(0,0)) - seurt + 24*36000L) % (24*36000L) < 18*36000L)
                  break;
#ifdef BIT32
               Sleep(50);
#endif
               }
            }
         if (os < 1 || os > kilpparam.osuusluku || (d = getpos(kno)) <= 0) {
            continue;
            }
         clrtxt(r2, 23, 65);
         vidint(r3, 12, 4, kno);
         vidint(r4, 12, 2, os);
         os--;
         GETREC(&kilp, d);
         entkilp = kilp;
         swprintf(wline, L"%s -%d", kilp.Seura(wst), (int) kilp.joukkue);
         clrtxt(r3, 18, 65);
         vidspwmsg(r3, 18, 7, 0, wline);
         for (os1 = 0; os1 < Sarjat[kilp.sarja].osuusluku; os1++) {
            swprintf(wline, L"Osuus%3s:  %-25.25s %7ld %8.8s  %c %s", wosuuskoodi(kilp.sarja, os1, 0, 0), kilp.Nimi(wst, 27, os1),   
               kilp.ostiet[os1].badge[0], AIKATOWSTRS(was, tulos(&kilp, os1, 0), 0), 
               kilp.wTark(os1, false), kilp.wTark(os1, true) == L'S' ? L"Sulj" : L"    ");
            fore = (os1 == os) ? 0 : 7;
            vidspwmsg(r5+os1, 0, fore, 7-fore, wline);
            }
         vidint(r5+os, 37, 7, badge);
         kno1 = bdg2kno(badge, 0);
         if (kno1 && kno1 != kno) {
            remjkbadge(kno1, badge);
            }
         kilp.ostiet[os].badge[0] = badge;
         tallenna(&kilp, d, 0, 0, 0, &entkilp);
         uusi_emit = 0;
         }
      }
   for (;;) {
      clrscr();
      kehys(1);
      kbflush();
		if (!valo_on) 
			asetavalo(0);
      vidspmsg(r2,0,7,0,"Viivakoodi");
      vidspmsg(r3,0,7,0,"Joukkue");
      vidspmsg(r4,0,7,0,"Osuus");
      for (i = 0; i < 2;) {
         clrln(ySize-3);
         switch (i) {
            case 0 :
               clrtxt(r1,0,60);
               vidspmsg(r1,0,7,0,"Lue viivakoodi tai anna joukkue ja osuus,  Esc: poistu");   
               if (salli_emit_ensin) {
                  gotoxy(12, r2);
                  while (!kbready(&ch, &key)) {
                     if (uusi_emit) {
                        sprintf(line, "Luettu Emit-kortti: %ld   ", uusi_emit);
                        vidspmsg(ySize-5,0,7,0,line);
                        }
                     Sleep(100);
                     }
                  }
               memset(line, 0, sizeof(line));
               do {
                  inputstr(line, 15, 12, r2, "\r\x1b", &ch, 0);
                  } while(ch == '\r' && !line[0]);
               if (ch == ESC) {
                  i = 2;
                  break;
                  }
               kbflush();
			   if (kilpparam.maxnosuus == 1 && kilpparam.ntosuus < 10) {
				   if (strlen(line) > 6)
					  memmove(line, line+strlen(line)-6, 7);
				   if (strlen(line) == 6 && line[0] == '%') {
					  os = line[5] - '0';
					  line[5] = 0;
					  kno = atol(line+1);
					  }
				   else if (line[0] == '*' && strlen(line) <= 6) {
					  os = line[strlen(line)-1] - '0'; 
					  line[strlen(line)-1] = 0;
	                  kno = atol(line+1);
		              }
                  else {
						naytatulos(0);
						sprintf(msg, "Virheellinen merkkijono viivakoodista tai näppäimiltä: %s", line);
						writeerror(msg, 0);
						}
					}
				else {
				   if (strlen(line) > 6)
					  memmove(line, line+strlen(line)-6, 7);
				   if (strlen(line) == 6 && line[0] == '%') {
					   ansitowcs(wst, line+4, 3);
					   wst[2] = 0;
					  line[4] = 0;
					  kno = atol(line+1);
					  os = tulkOsuuskoodi(sarjaKno(kno), wst);
					  }
				   else if (line[0] == '*' && strlen(line) <= 7) {
					   if (line[strlen(line)-1] >= 'A') {
							ansitowcs(wst, line+strlen(line)-2, 3);
							wst[3] = 0;
						    line[strlen(line)-2] = 0;
							}
					   else {
							ansitowcs(wst, line+strlen(line)-1, 2);
							wst[2] = 0;
						    line[strlen(line)-1] = 0;
							}
	                  kno = atol(line+1);
					  os = tulkOsuuskoodi(sarjaKno(kno), wst) + 1;
					  }
               else {
						naytatulos(0);
						sprintf(msg, "Virheellinen merkkijono viivakoodista tai näppäimiltä: %s", line);
						writeerror(msg, 0);
						}
                   }
               if (os < 1 || os > kilpparam.osuusluku || (d = getpos(kno)) <= 0) {
						asetavalo(2);
                  vidspmsg(r2, 27, 0, 7, "Joukkue tai osuus väärä -lue uudelleen");
                  continue;
                  }
               clrtxt(r2, 23, 65);
               vidint(r3, 12, 4, kno);
               vidint(r4, 12, 2, os);
               os--;
               GETREC(&kilp, d);
               if (os >= Sarjat[kilp.sarja].osuusluku) {
                  vidspmsg(r2, 27, 0, 7, "Osuus väärä -lue uudelleen");
				  asetavalo(2);
                  continue;
                  }
               entkilp = kilp;
               swprintf(wline, L"%s -%d", kilp.Seura(wst), (int) kilp.joukkue);
               clrtxt(r3, 18, 60);
               vidspwmsg(r3, 18, 7, 0, wline);
               for (os1 = 0; os1 < Sarjat[kilp.sarja].osuusluku; os1++) {
                  swprintf(wline, L"Osuus %2.2s:  %-25.25s %7ld %8.8s  %c %s", wosuuskoodi(kilp.sarja, os1, 0, 2), kilp.Nimi(wst, OSNIMIL, os1), 
                     kilp.ostiet[os1].badge[0], AIKATOWSTRS(was, tulos(&kilp, os1, 0), 0), 
                     kilp.wTark(os1, false), kilp.wTark(os1, true) == L'S' ? L"Sulj" : L"    ");
                  fore = (os1 == os) ? 0 : 7;
                  vidspwmsg(r5+os1, 0, fore, 7-fore, wline);
                  }
				if (toupper(kilp.ostiet[os].seuranta) == 'G') {
					viwrrect(r5+kilpparam.osuusluku,0,r5+kilpparam.osuusluku+2,64,
						"                                                                 "
						"      KILPAILIJA OSALLISENA GPS-SEURANTAAN - HAE LÄHETIN         "
						"                                                                 ", 0, 3, 0);
					}
/*
			   if (kilpparam.maxnosuus > 1) {
				   os1 = MAXOSUUSLUKU;
				   if (Sarjat[kilp.sarja].yosuus[os] > 0) {
					  for (os1 = Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[os]-1]+1; 
						 os1 <= Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[os]]; os1++)
						 if (kilp.ostiet[os1].badge == 0)
							break;
					  }
				   }
               if ((kilpparam.maxnosuus > 1 && os1 <= Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[os]]) ||
				  (kilpparam.maxnosuus == 1 && os > 0 && kilp.ostiet[os-1].badge == 0)) {
 */
               if (kilp.ostiet[os-1].badge == 0) {
                  ch = 'K';
				  asetavalo(2);
				  if (aukkovaroitus) {
	                  erbeep();
		              selectopt("Edellisellä osuudella ei korttia. K)irjaa, E)i kirjata, L)opeta varoitus",
			             "KEL", &ch);
					  if (ch == 'L') {
						 aukkovaroitus = 0;
						 ch = 'K';
						 }
				      if (ch == 'E')
					     continue;
					  }
                  }
               if (kilp.Maali(os, 0) != TMAALI0 || kilp.Tark(os, true) != '-') {
                  ch = 'E';
				  asetavalo(2);
                  erbeep();
                  selectopt("Osuudella jo tulos tai muu suoritusmerkintä. Kirjataanko silti (K/E)",
                     "KE", &ch);
                  if (ch == 'E')
                     continue;
                  }
               i = 1;
            case 1:
               clrtxt(r1,0,64);
               if (uusi_emit) {
                  ed_emit = uusi_emit;
                  if (!salli_emit_ensin) {
                     uusi_emit = 0;
                     sprintf(line, "Lue uusi kortti tai P)oimi edellinen luettu:"
                        " %ld, Esc: keskeytä", ed_emit);
                     vidspmsg(r1,0,0,7,line);
                     }
                  }
               else {
                  ed_emit = 0;
                  vidspmsg(r1,0,0,7,"Lue Emit-kortti, K)orjaa tieto näppäimistöltä, Esc: keskeytä");
                  }
               clrln(ySize-3);
               if (!salli_emit_ensin || !ed_emit) {
				  asetavalo(3);
                  vidspmsg(ySize-3,0,7,0, "Odottaa Emit-koodia - Esc: keskeytä odotus");
                  for (;;) {
                     ch = toupper(readkbd(&ch, 0, spoolfl));
                     if (ch == 'K')
                        break;
                     if (ch == 'P' && ed_emit)
                        uusi_emit = ed_emit;
                     if (uusi_emit)
                        break;
                     if (ch == ESC) {
                        i = 0;
                        break;
                        }
                     Sleep(50);
                     }
                  }
               clrln(ySize-3);
               if (ch == 'K') {
                  badge = kilp.ostiet[os].badge[0];
                  inputlong(&badge, 7, 37, r5+os, "\r\x1b", &ch);
                  if (ch == ESC)
                     i = 0;
                  }
               else
                  badge = uusi_emit;
               if (i == 0) {
                  uusi_emit = ed_emit;
                  continue;
                  }
               vidint(r5+os, 37, 6, badge);
               ch = 'H';
			   if (badge == 200) {
                    ch = 'E';
					viwrrect(r5+kilpparam.osuusluku,0,r5+kilpparam.osuusluku+4,64,
						"                                                                 "
						"                                                                 "
						"       VIALLINEN EMIT-KORTTI - PAINA ESC JA VAIHDA KORTTI        "
						"                                                                 "
						"                                                                 ", 0, 3, 0);
					}
                kno1 = bdg2kno(badge, 0);
				if (esta_uusikaytto && kno1 > 0) {
					kilptietue vkilp;
					int vos = 0, vd;

					vd = getpos(kno1);
					if (vd > 0) {
						vkilp.getrec(vd);
						if (kno1 == kno && vkilp.ostiet[os].badge[0] == badge) {
							viwrrect(r5+kilpparam.osuusluku,0,r5+kilpparam.osuusluku+4,64,
								"                                                                 "
								"                                                                 "
								"       SAMA KORTTI JO KIRJATTU KILPAILIJALLE. EI MUUTETTAVAA     "
								"       PAINA ESC JA SIIRRY SEURAAVAN KILPAILIJAN KÄSITTELYYN     "
								"                                                                 ", 0, 3, 0);
							i = 0;
							readkbd(&ch, TRUE,  spoolfl);
		                    kbflush();
						    clrscr();
						    kehys(1);
							ch = 0;
							break;
							}
						else {
							for (vos = 0; vos < Sarjat[vkilp.sarja].osuusluku; vos++)
								if (vkilp.ostiet[vos].badge[0] == badge)
									break;
							if (vos < Sarjat[vkilp.sarja].osuusluku && !vkilp.Hyv(vos)) {
								vkilp.ostiet[vos].badge[0] = haeVapaaKoodi(vkilp.ostiet[vos].badge[0]);
								tallenna(&vkilp, vd, 0, 0, 0, 0);
								vd = -1;
								}
							}
						}
					if (vd > 0) {
						sprintf(line, "          Kortti käytössä joukkueen %d osuudella %s                ", kno1, osuuskoodi(vkilp.sarja, vos, 0, 0));
						viwrrect(r5+kilpparam.osuusluku,0,r5+kilpparam.osuusluku+4,64,
							"                                                                 "
							"                                                                 "
							"                                                                 "
							"                 PAINA ESC JA VAIHDA KORTTI                      "
							"                                                                 ", 0, 3, 0);
						viwrrect(r5+kilpparam.osuusluku+1,0,r5+kilpparam.osuusluku+1,64, line, 0, 3, 0);
						ch = 'E';
						kbflush();
						}
					}
               if (!esta_uusikaytto && kno1 && kno1 != kno) {
                  sprintf(line, "Emit-kortti jo käytössä joukkueella %d.", kno1);
                  kbflush();
                  writeerror(line, 0);
                  ch = 'E';
                  kbflush();
                  }
               if (tarkavoimet) {
                  for (os1 = 0; os1 < os; os1++) {
                     if (kilp.ostiet[os1].badge[0] == badge && 
                         kilp.Maali(os1, 0) == TMAALI0)
                        break;
                     }
                  if (os1 < os) {
                     kbflush();
                     writeerror("Emit-kortti lukematta aiemmalta osuudelta", 0);
                     ch = 'E';
                     kbflush();
                     }
                  }
               if (ch == 'E') {
                  selectopt("Tallennetaanko ongelmasta huolimatta (K/E)", "KE", &ch);
                  if (ch == 'K')
                     remjkbadge(kno1, badge);
                  }
//               if (kilp.ostiet[os].badge[0]) 
//                  rembadge(kno, os, d);
               if (ch != 'E')
                  kilp.ostiet[os].badge[0] = badge;
               if (ch == 'H')
                  selectopt("H)yväksy tiedot,  Esc: peruuta ja lue seuraava viivakoodi", "H\x1b", &ch);
               if (ch == 'H' || ch == 'K') {
//                  addbadge(badge, kno, d, 0);
                  tallenna(&kilp, d, 0, 0, 0, &entkilp);
                  uusi_emit = 0;
#ifdef BIT32
						naytatulos(0);
#endif
                  }
               ch = ' ';
               kbflush();
               i = 2;
               break;
            }
         }
      if (ch == ESC)
         return(0);
      }
   }
