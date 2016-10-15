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
#include <bvideo.h>
#include <bkeybrd.h>
#include <bstrings.h>
#ifndef  BIT32
#include <bios.h>
#include <bintrupt.h>
#include <bkeys.h>
#include <asynch_1.h>
#endif
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>
#include "laskent.h"

#define TAULU_NRIVI 5
#define TAULU_RIVPIT 23
#define tauluqsize 400

#ifdef BIT32
extern HANDLE hComm[MAX_PORTTI];
#endif

int cn_gaz;

static int taulusrj, avoin, in_taulu, alku, rivpit, nrivi, etumerk;
static int nosuus = 0;
int jatktaulu, eronaytto, autokarki[MAXOSUUSLUKU];
long tkarki[MAXOSUUSLUKU], kviive[MAXOSUUSLUKU];
int htaulu = -1, tosuus[TAULU_NRIVI], lkmaikana[TAULU_NRIVI];
char tauluflnm[41] = "numtaulu.txt";

static void opentaulu(void)
   {
   char far *taulubuf;
   int os, er = 1;

   if (!udptaulu && taulu_com > 64) {
      return;
      }
   for (os = 0; os < MAXOSUUSLUKU; os++) {
      tkarki[os] = TMAALI0;
      }
   taulu_baud = 5;
   cn_gaz = GAZCOM;
   if (udptaulu) {
	   rivpit = 21;
	   nrivi = 3;
	   etumerk=4;
	   }
   else {
	   rivpit = 22;
	   nrivi = 5;
	   etumerk = 1;
	   }
#ifdef BIT32
   if (openport_x(cn_gaz, taulu_com, taulu_baud, 'n', 8, 1, 0)) {
      writeerror("GAZ-tiedonsiirron avaaminen ei onnistunut", 0);
      taulu_com = 0;
	  udptaulu = 0;
      return;
      }
#else
   if ((taulubuf = farmalloc(tauluqsize+8)) != 0) {
      if (!openport(taulu_com, taulu_irq, taulu_addr, taulu_baud, 4,
         tauluqsize, taulubuf))
         er = 0;
      }
   if (er) {
      writeerror("Tiedonsiirtoa taululle ei avata", 0);
      taulu_com = 0;
      }
#endif
   }

static near int taulu_tied(char *text1[], int nrv)
   {
   int i;

   if (htaulu < 0) {
      htaulu = _open(tauluflnm, O_WRONLY | O_CREAT | O_TRUNC | O_TEXT, S_IREAD | S_IWRITE);
      }
   if (htaulu < 0) {
      return(1);
      }
   for (i = 0; i < nrv; i++) {
      _write(htaulu, text1[i], strlen(text1[i]));
      }
   if (!jatktaulu) {
      _close(htaulu);
      htaulu = -1;
      }
   return(0);
   }

static near int taulu_lah(char *text1[], int nrv)
   {
   int i, j, n, nread;
   static long prev_t = -1000;
   static char prev_a[2] = "#";
   char vast[200];

   static char vertrivi[TAULU_NRIVI][TAULU_RIVPIT+6];

   if (mstimer() - prev_t < 1000)
	   return(0);
   for (i = 0; i < nrv; i++) {
      if (memcmp(text1[i], vertrivi[i], rivpit+etumerk))
         break;
      }
   if (i >= nrv) {
 	  prev_t = mstimer();
      return(0);
	  }
   if (taulu_com > 64) {
      if (!taulu_tied(text1, nrv)) {
         for (i = 0; i < nrv; i++)
            memcpy(vertrivi[i], text1[i], rivpit+etumerk);
         }
	  prev_t = mstimer();
      return(0);
      }
   outquesize_x(cn_gaz, &n);
   if (udptaulu || n < tauluqsize - nrv*30) {
      for (i = 0; i < nrv; i++) {
		  if (udptaulu) {
			if (mstimer() - prev_t < 80) {
				for (j = 0; j < 2; j++) {
					Sleep(30);
					read_UDPcli_0(hComm[cn_gaz], 200, vast, &nread);
					if (nread > 0 && vast[2] == prev_a[0] && vast[3] == prev_a[1])
						break;
					}
				}
			wrt_st_UDP_0(hComm[cn_gaz], etumerk+strlen(text1[i]+etumerk), text1[i], &n);
			prev_t = mstimer();
			memcpy(prev_a, text1+2, 2);
			}
		 else
			wrt_st_x(cn_gaz, etumerk+strlen(text1[i]+etumerk), text1[i], &n);
         memcpy(vertrivi[i], text1[i], rivpit+etumerk);
         }
      }
   prev_t = mstimer();
   return(0);
   }

void paiv_taulu(void)
   {
   int k, r, it, os;
   int d, nt;
   char st[12], st2[30] , *rivi[TAULU_NRIVI], r1[TAULU_RIVPIT+6],r2[TAULU_RIVPIT+6];
   char r3[TAULU_RIVPIT+6],r4[TAULU_RIVPIT+6],r5[TAULU_RIVPIT+6];
   long t, tls;
   jrec far *p;
   kilptietue kilp;

   if (!avoin) 
      return;
   if (nosuus < 1 || tosuus[0] < 1 || tosuus[0] > osuusluku[taulusrj])
      return;
   rivi[0] = r1;
   rivi[1] = r2;
   rivi[2] = r3;
   rivi[3] = r4;
   rivi[4] = r5;
   for (k = 0; k < nrivi; k++) {
      memset(rivi[k]+etumerk, ' ', rivpit);
      memcpy(rivi[k]+rivpit+etumerk, "\r", 2);
	  if (udptaulu) {
		  rivi[k][0] = '\x4a';
		  rivi[k][1] = '\x57';
		  rivi[k][2] = (alku + (rivpit+1)*k)/256;
		  rivi[k][3] = (alku + (rivpit+1)*k)%256;
		  }
	  else
		  rivi[k][0] = 'A'+k;
      }
   for (k = 0; k < nosuus; k++) {
      os = tosuus[k] - 1;
      if (os < 0 || os >= osuusluku[taulusrj])
         continue;
      nt = ntulos[taulusrj][os][0];
      if (!lkmaikana[k]) {
         tls = t_time_l(biostime(0,0));
         if (autokarki[os]) {
            tkarki[os] = TMAALI0;
            if (nt) {
               p = jalku[taulusrj][os][0];
               d = p->dkilp;
               if (d > 0) {
                  GETREC(&kilp, d);
                  tkarki[os] = kilp.ostiet[os].maali[0] + kviive[os];
                  }
               }
            }
         if (tkarki[os] != TMAALI0) {
            t = (tls - tkarki[os] + 48L*36000L) % (24*36000L);
//            if (t < 8L*36000L) 
               {
               memcpy(st, AIKATOSTR(t,0), 8);
               st[8] = 0;
               st[2] = st[5] = ':';
//#ifdef GAZ2
               if (st[0] == '0') {
                  st[0] = ' ';
                  if (!udptaulu && st[1] == '0') {
                     st[1] = ' ';
                     if (st[3] == '0') {
                        st[3] = ' ';
                        if (st[4] == '0') {
                           st[4] = ' ';
                           if (st[6] == '0') {
                              st[6] = ' ';
                              }
                           }
                        }
                     }
                  }
//#endif
               }
            }
		 else {
			 memset(st, ' ', 8);
			 st[8] = 0;
			}
		 if (udptaulu) {
			 sprintf(st2, "%2d. os %4d %8s", os+1, nt, st);
			 memcpy(rivi[k]+etumerk, st2, strlen(st2));
			 }
		 else {
 		    memcpy(rivi[k]+etumerk+1, st, 8);
			rivi[k][etumerk+21] = os + '1';
			putintright(nt%1000, rivi[k] + etumerk + 2);
			}
#ifdef GAZ2
         if (nt >= 1000) {
            rivi[k][8] = nt/1000 + '0';
            }
#endif
         }
      else {
         rivi[k][etumerk+15] = '0' + nt%10;
         if (nt >= 10)
            rivi[k][etumerk+14] = '0' + ((nt/10)%10);
         if (nt >= 100)
            rivi[k][etumerk+12] = '0' + ((nt/100)%10);
         if (nt >= 1000)
            rivi[k][etumerk+11] = '0' + ((nt/1000)%10);
         rivi[k][etumerk+8] = os + '1';
         rivi[k][etumerk+10] = rivi[k][etumerk+13] = ':';
         }
      if (in_taulu) {
         viwrrect(4+k, 10, 4+k, 8+rivpit, rivi[k]+etumerk, 7, 0, 0);
         }
      }
   taulu_lah(rivi, nosuus);
   }

void closetaulu(void)
   {
   if (taulu_com <= 64 || udptaulu)
      closeport_x(cn_gaz);
   else
      if (htaulu >= 0)
         _close(htaulu);
   }


void algetaulu(void)
   {
   char cv, ch;
   int rv, k;
   char msg[80], st[12];
   long t;

   if (!taulu_com && !udptaulu) {
      writeerror("Tiedonsiirtoa ei m„„ritelty", 0);
      return;
      }
   clrscr();   
   header="TULOSTAULU";
   kehys(1);
   in_taulu = 1;
   if (!avoin && (taulu_com || udptaulu)) {
      opentaulu();
      if (taulu_com || udptaulu) avoin = 1;
      else return;
      }
   for (;;) {
      vidspmsg(9,0,7,0,"Valinnat");
      for (k = 0; k < nosuus; k++) {
         sprintf(msg, "%d. taulu   ", k+1);
         vidspmsg(11+k, 1, 7, 0, msg);
         strcpy(st, AIKATOSTR(tkarki[tosuus[k]-1], t0));
         sprintf(msg, "%d. osuus   K„rki: %8.8s", tosuus[k], st);
         vidspmsg(11+k, 11, 7, 0, msg);
         }
      ch = nosuus + '0';
      selectopt("Anna n„ytett„vien osuuksien lukum„„r„ (0 : Ei n„ytet„)  Esc: Poistu valinnoista", "012345\x1b", &ch);
      if (ch == ESC) {
         break;
         }
      nosuus = ch - '0';
      for (k = 0; k < nosuus; k++) {     
         sprintf(msg, "%d. taulu   ", k+1);
         vidspmsg(11+k, 1, 7, 0, msg);
         cv = tosuus[k] + '0';
         sprintf(msg, "Anna %d taulun osuuden numero", k+1);
         selectopt(msg, "123456789", &cv);
         tosuus[k] = cv - '0';
         sprintf(msg, "%d. osuus   ", tosuus[k]);
         vidspmsg(11+k, 11, 7, 0, msg);
		 if (udptaulu)
			lkmaikana[k] = 0;
		 else {
	         cv = lkmaikana[k] ? 'K' : 'E';
		     selectopt("Kirjoitentaanko lukum„„r„ aikakent„„n (K/E)", "KE", &cv);
			 lkmaikana[k] = cv == 'K';
			 }
         if (!lkmaikana[k]) {
            cv = 'E';
            selectopt("Muutetaanko osuuden k„rkiajan k„sittely„ (K/E)", "KE", &cv);
            if (cv == 'K') {
               cv = ' ';
               selectopt("K„rkiaika T)uloslaskennasta, N)yt, S)y”t„", "TNS", &cv);
               clrln(ySize-3);
               t = tkarki[tosuus[k]-1];
               switch (cv) {
                  case 'T' :
                     autokarki[tosuus[k]-1] = 1;
                     vidspmsg(ySize-3,0,7,0,"Anna viive ajanotosta");
                     inputaika_l(&kviive[tosuus[k]-1], 0, 8, 23, ySize-3, "\r", &ch);
                     break;
                  case 'N' :
                     t = t_time_l(biostime(0,0));
                  case 'S' :
                     vidspmsg(ySize-3,0,7,0,"Vahvista tai sy”t„ aika, Esc: aikaa ei muuteta");
                     inputaika_l(&t, t0, 8, 48, ySize-3, "\r\x1b", &ch);
                     if (ch == '\r')
                        tkarki[tosuus[k]-1] = t;
                     autokarki[tosuus[k]-1] = 0;
                     break;
                  }
               }
            }
         clrln(ySize-3);
         }
      for (k = 4; k < 11; k++)
         clrtxt(k,0,60);
      }
   in_taulu = 0;
   }

