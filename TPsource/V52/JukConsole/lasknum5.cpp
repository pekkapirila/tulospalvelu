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

#if defined(TAULU) || defined(ALGETAULU)
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
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>
#include "VDeclare.h"

#define TAULU_NRIVI 5
#define TAULU_RIVPIT 23
#define tauluqsize 400

extern HANDLE hComm[MAX_PORTTI];

int cn_gaz;

static int taulusrj, avoin, in_taulu;
static int nosuus = 0;
int jatktaulu, eronaytto, autokarki[MAXOSUUSLUKU];
int tkarki[MAXOSUUSLUKU], kviive[MAXOSUUSLUKU];
int htaulu = -1, tosuus[TAULU_NRIVI], lkmaikana[TAULU_NRIVI];
wchar_t tauluflnm[41] = L"numtaulu.txt";


static void opentaulu(void)
   {
   int os, er = 1;

   if (taulu_com > 64) {
      return;
      }
   for (os = 0; os < MAXOSUUSLUKU; os++) {
      tkarki[os] = TMAALI0;
      }
   taulu_baud = 5;
   cn_gaz = GAZCOM;
   if (openport_x(cn_gaz, taulu_com, taulu_baud, 'n', 8, 1, 0)) {
      writeerror("GAZ-tiedonsiirron avaaminen ei onnistunut", 0);
      taulu_com = 0;
      return;
      }
   }

static int taulu_tied(char *text1[], int nrv)
   {
   int i;
   char st[100];

   if (htaulu < 0) {
      htaulu = _open(wcstoansi(st, tauluflnm, 98), O_WRONLY | O_CREAT | O_TRUNC | O_TEXT, S_IREAD | S_IWRITE);
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

static int taulu_lah(char *text1[], int nrv)
   {
   int i, n;
   static char vertrivi[TAULU_NRIVI][TAULU_RIVPIT+3];

   for (i = 0; i < nrv; i++) {
      if (strcmp(text1[i], vertrivi[i]))
         break;
      }
   if (i >= nrv) 
      return(0);
   if (taulu_com > 8) {
      if (!taulu_tied(text1, nrv)) {
         for (i = 0; i < nrv; i++)
            strcpy(vertrivi[i], text1[i]);
         }
      return(0);
      }
   outquesize_x(cn_gaz, &n);
   if (n < tauluqsize - nrv*30) {
      for (i = 0; i < nrv; i++) {
         wrt_st_x(cn_gaz, strlen(text1[i]), text1[i], &n);
         strcpy(vertrivi[i], text1[i]);
         }
      }
   return(0);
   }

void paiv_taulu(void)
   {
   int k, os;
   int d, nt;
   char st[20] , *rivi[TAULU_NRIVI], r1[TAULU_RIVPIT+3],r2[TAULU_RIVPIT+3];
   char r3[TAULU_RIVPIT+3],r4[TAULU_RIVPIT+3],r5[TAULU_RIVPIT+3];
   long t, tls;
   int p = -1, lj = -1;
   kilptietue kilp;

   if (!avoin) 
      return;
   if (nosuus < 1 || tosuus[0] < 1 || tosuus[0] > Sarjat[taulusrj].osuusluku)
      return;
   rivi[0] = r1;
   rivi[1] = r2;
   rivi[2] = r3;
   rivi[3] = r4;
   rivi[4] = r5;
   for (k = 0; k < TAULU_NRIVI; k++) {
      memset(rivi[k], ' ', TAULU_RIVPIT);
      memcpy(rivi[k]+TAULU_RIVPIT, "\r", 2);
      rivi[k][0] = 'A'+k;
      }
   for (k = 0; k < nosuus; k++) {
      os = tosuus[k] - 1;
      if (os < 0 || os >= Sarjat[taulusrj].osuusluku)
         continue;
      nt = ntulos[taulusrj][os][0];
      if (!lkmaikana[k]) {
         tls = KSEK*T_TIME(biostime(0,0));
         if (autokarki[os]) {
            tkarki[os] = TMAALI0;
            if (nt) {
               d = jalku[taulusrj][os][0];
               if (d > 0) {
                  GETREC(&kilp, d);
                  tkarki[os] = kilp.Maali(os, 0) + kviive[os];
                  }
               }
            }
         if (tkarki[os] != TMAALI0) {
            t = NORMTULOS(tls - tkarki[os]);
//            if (t < 8L*36000L) 
               {
               AIKATOSTRS(st, t,0);
               st[8] = 0;
               st[2] = st[5] = ':';
//#ifdef GAZ2
               if (st[0] == '0') {
                  st[0] = ' ';
                  if (st[1] == '0') {
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
               memcpy(rivi[k]+9, st, 8);
               }
            }
         rivi[k][22] = os + '1';
         putintright(nt%1000, rivi[k] + 3);
#ifdef GAZ2
         if (nt >= 1000) {
            rivi[k][8] = nt/1000 + '0';
            }
#endif
         }
      else {
         rivi[k][16] = '0' + nt%10;
         if (nt >= 10)
            rivi[k][15] = '0' + ((nt/10)%10);
         if (nt >= 100)
            rivi[k][13] = '0' + ((nt/100)%10);
         if (nt >= 1000)
            rivi[k][12] = '0' + ((nt/1000)%10);
         rivi[k][9] = os + '1';
         rivi[k][11] = rivi[k][14] = ':';
         }
      if (in_taulu) {
         viwrrect(4+k, 10, 4+k, 9+TAULU_RIVPIT, rivi[k], 7, 0, 0);
         }
      }
   taulu_lah(rivi, nosuus);
   }

void closetaulu(void)
   {
   if (taulu_com <= 8)
      closeport_x(cn_gaz);
   else
      if (htaulu >= 0)
         _close(htaulu);
   }


void algetaulu(void)
   {
   char cv, ch;
   int  k;
   char msg[80], st[20];
   int t;

   if (!taulu_com) {
      writeerror("Tiedonsiirtoa ei m‰‰ritelty", 0);
      return;
      }
   clrscr();   
   header=L"TULOSTAULU";
   kehys(1);
   in_taulu = 1;
   if (!avoin && taulu_com) {
      opentaulu();
      if (taulu_com) avoin = 1;
      else return;
      }
   for (;;) {
      vidspmsg(9,0,7,0,"Valinnat");
      for (k = 0; k < nosuus; k++) {
         sprintf(msg, "%d. taulu   ", k+1);
         vidspmsg(11+k, 1, 7, 0, msg);
         AIKATOSTRS(st, tkarki[tosuus[k]-1], t0);
         sprintf(msg, "%d. osuus   K‰rki: %8.8s", tosuus[k], st);
         vidspmsg(11+k, 11, 7, 0, msg);
         }
      ch = nosuus + '0';
      selectopt("Anna n‰ytett‰vien osuuksien lukum‰‰r‰ (0 : Ei n‰ytet‰)  Esc: Poistu valinnoista", "012345\x1b", &ch);
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
         cv = lkmaikana[k] ? 'K' : 'E';
         selectopt("Kirjoitentaanko lukum‰‰r‰ aikakent‰‰n (K/E)", "KE", &cv);
         lkmaikana[k] = cv == 'K';
         if (!lkmaikana[k]) {
            cv = 'E';
            selectopt("Muutetaanko osuuden k‰rkiajan k‰sittely‰ (K/E)", "KE", &cv);
            if (cv == 'K') {
               cv = ' ';
               selectopt("K‰rkiaika T)uloslaskennasta, N)yt, S)yˆt‰", "TNS", &cv);
               clrln(ySize-3);
               t = tkarki[tosuus[k]-1];
               switch (cv) {
                  case 'T' :
                     autokarki[tosuus[k]-1] = 1;
                     vidspmsg(ySize-3,0,7,0,"Anna viive ajanotosta");
                     INPUTAIKA(&kviive[tosuus[k]-1], 0, 8, 23, ySize-3, "\r", &ch);
                     break;
                  case 'N' :
                     t = KSEK*T_TIME(biostime(0,0));
                  case 'S' :
                     vidspmsg(ySize-3,0,7,0,"Vahvista tai syˆt‰ aika, Esc: aikaa ei muuteta");
                     INPUTAIKA(&t, t0, 8, 48, ySize-3, "\r\x1b", &ch);
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

#endif // defined(TAULU) || defined(ALGETAULU)
