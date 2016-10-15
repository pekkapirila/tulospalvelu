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
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <malloc.h>
#include <errno.h>
//#include <dos.h>
#include <ctype.h>
#include "VDeclare.h"
#include "UnitImport.h"

#define VAIKALUKU0 12
#ifndef VAIKALUKU0
#define VAIKALUKU0 VAIKALUKU
#endif
#define VAIKALUKUA 4

#ifndef MAXOSUUSLUKU0
#define MAXOSUUSLUKU0 MAXOSUUSLUKU
#endif

#define HAJONTA0 6

#ifndef HAJONTA0
#define HAJONTA0 HAJONTA
#endif

#define OSNIMIL0 24
#define LSEURA0  27
#define LSAKOT0 5

#pragma pack(push,1)

typedef struct {
   char nimi[OSNIMIL0+1];
   char hajonta[HAJONTA0+1];
   long badge;
   char laina;
   char selitys[17];
   char keskhyl;
   TMAALI ylahto;
   TMAALI maali[VAIKALUKU0+1];
   UINT16 kone;
   INT16  sija[VAIKALUKU0+1];
   INT16  ossija;
   } osrec0;

#define OSSIZE0 sizeof(osrec0)

typedef struct {
   INT16  kilpstatus;
   INT16  kilpno;
   long paivitys;
   char seura[LSEURA0+1];
   char joukkue;
   char sarja;
   char maa[4];
   INT16 ilmlista;
   int  pisteet[4];
   char fill[OSSIZE0-LSEURA0-33];
   osrec0 ostiet[MAXOSUUSLUKU0];
} kilptietue0;

#define KILPRECSIZE_0 sizeof(kilptietue0)

static   kilptietue0 u;

typedef struct {
   char nimi[OSNIMIL0+1];
   char keskhyl;
   char sakot[LSAKOT0+1];
   TMAALI ylahto;
   TMAALI maali[VAIKALUKUA+1];
   UINT16 kone;
   INT16  sija[VAIKALUKUA+1];
   INT16  ossija;
   } osrec_a;

#define OSSIZEA sizeof(osrec_a)

typedef struct {
   INT16  kilpstatus;
   INT16  kilpno;
   long paivitys;
   char seura[LSEURA0+1];
   char joukkue;
   char sarja;
   char maa[4];
   INT16 ilmlista;
   int  pisteet[4];
   char fill[OSSIZEA-LSEURA0-33];
//#endif
   osrec_a ostiet[MAXOSUUSLUKU0];
} kilptietue_a;

#define KILPRECSIZE_A sizeof(kilptietue_a)

static   kilptietue_a u_a;

#pragma pack(pop)
int valuku_vanha;

int luevanha(wchar_t *tiednimi)
   {
   int y,os, n = 0, d, hajonta, vahajonta, vaikaluku, maxosuusluku, va, size1, lsak;
   int osnimil, laji, vavanha, osuusvanha, voslen;
   char ss[200];
   int vaihdanm;
	wchar_t wst[200], st[200];
	FILE *vkilp;
	kilptietue u2;

   laji = FormImport->RGLaji->ItemIndex;
   vavanha = _wtoi(FormImport->EditNva->Text.c_str());
   vahajonta = _wtoi(FormImport->EdtHaj->Text.c_str());
   osuusvanha = _wtoi(FormImport->EditNpv->Text.c_str());
   vaikaluku = min(kilpparam.valuku, vavanha);
   maxosuusluku = min(MAXOSUUSLUKU, osuusvanha);
   hajonta = min(HAJONTA, vahajonta);
   lsak = min(LSAKOT0, LSAKOT);
   vaihdanm = 1 - 2*FormImport->RGNimiJarj->ItemIndex;
   if (laji == 0) {
	   voslen = OSSIZE0 - (VAIKALUKU0-vavanha) * 6 + vahajonta-HAJONTA0;
	   }
   else {
	   voslen = OSSIZEA - (VAIKALUKUA-vavanha) * 6;
	   }
   size1 = voslen*(osuusvanha+1);
   vkilp = _wfopen(tiednimi, L"rb");
   if (vkilp == NULL)
	  return(0);
   for (;;) {
	   if (fread(&u, 1, size1, vkilp) < size1) break;
	   u2.nollaa();
	   if (u.kilpstatus) {
		   continue;
		  }
	   else {
		   if (laji == 0) {
			   for (int os = osuusvanha; os > 0; os--) {
				   memmove(&u.ostiet[os-1], (char *)&u+os*voslen, voslen);
				   memmove(&u.ostiet[os-1].ossija, (char *)&u.ostiet[os]-2, 2);
				   memmove(&u.ostiet[os-1].kone, (char *)&u.ostiet[os].kone-4*(VAIKALUKU0-vavanha),
					   4+2*vavanha);
				   memmove((char *)&u.ostiet[os-1].badge+vahajonta-HAJONTA0, (char *)&u.ostiet[os-1].badge,
					   voslen - ((char *)&u.ostiet[os-1].nimi - u.ostiet[os-1].nimi));
				   }
			   memcpy(&u2, &u, 8);
			   u2.setSeura(oemtowcs(wst, u.seura, 100, 0));
			   u2.joukkue = u.joukkue;
			   u2.sarja = u.sarja;
			   u2.ilmlista = u.ilmlista;
			   memcpy(u2.maa, u.maa, sizeof(u.maa));
			   memcpy((char *)u2.pisteet, (char *) u.pisteet, sizeof(u.pisteet));
			   for (os = 0; os < maxosuusluku; os++) {
					u2.setNimi(oemtowcs(wst, u.ostiet[os].nimi, 100, 0), os, vaihdanm);
					memcpy(u2.ostiet[os].hajonta, u.ostiet[os].hajonta,hajonta+1);
					if (u.ostiet[os].keskhyl != 'S')
						u2.ostiet[os].keskhyl = u.ostiet[os].keskhyl;
					u2.ostiet[os].ylahto = SEK/10*u.ostiet[os].ylahto;
					if (u2.ostiet[os].ylahto > 12*TUNTI) {
						u2.ostiet[os].lahtolaji = 1;
						u2.ostiet[os].ylahto -= 24*TUNTI;
						}
					for (va = 0; va <= vaikaluku; va++) {
					   u2.setMaali(os, va, SEK/10*u.ostiet[os].maali[va]);
					   u2.setSija(os, va, u.ostiet[os].sija[va]);
					   }
					u2.ostiet[os].kone = u.ostiet[os].kone;
					u2.ostiet[os].ossija = u.ostiet[os].ossija;
					u2.ostiet[os].badge[0] = u.ostiet[os].badge;
					u2.ostiet[os].laina[0] = u.ostiet[os].laina;
					memcpy(u2.ostiet[os].selitys, u.ostiet[os].selitys,17);
					}
				}
			else {
			   for (int os = osuusvanha; os > 0; os--) {
				   memmove(&u_a.ostiet[os-1], (char *)&u_a+os*voslen, voslen);
				   memmove(&u_a.ostiet[os-1].ossija, (char *)&u_a.ostiet[os]-2, 2);
				   memmove(&u_a.ostiet[os-1].kone, (char *)&u_a.ostiet[os].kone-4*(VAIKALUKU0-vavanha),
					   4+2*vavanha);
				   }
			   memcpy(&u2, &u_a, 8);
			   u2.setSeura(oemtowcs(wst, u_a.seura, 100, 0));
			   u2.joukkue = u_a.joukkue;
			   u2.sarja = u_a.sarja;
			   u2.ilmlista = u_a.ilmlista;
			   memcpy(u2.maa, u_a.maa, sizeof(u_a.maa));
			   memcpy((char *)u2.pisteet, (char *) u_a.pisteet, sizeof(u_a.pisteet));
			   for (os = 0; os < maxosuusluku; os++) {
					u2.setNimi(oemtowcs(wst, u_a.ostiet[os].nimi, 100, 0), os, vaihdanm);
					if (u_a.ostiet[os].keskhyl != 'S')
						u2.ostiet[os].keskhyl = u_a.ostiet[os].keskhyl;
					u2.ostiet[os].ylahto = SEK/10*u_a.ostiet[os].ylahto;
					if (u2.ostiet[os].ylahto > 12*TUNTI) {
						u2.ostiet[os].lahtolaji = 1;
						u2.ostiet[os].ylahto -= 24*TUNTI;
						}
					for (va = 0; va <= vaikaluku; va++) {
					   u2.setMaali(os, va, SEK/10*u_a.ostiet[os].maali[va]);
					   u2.setSija(os, va, u_a.ostiet[os].sija[va]);
					   }
					u2.ostiet[os].kone = u_a.ostiet[os].kone;
					u2.ostiet[os].ossija = u_a.ostiet[os].ossija;
					memcpy(u2.ostiet[os].ampsakot, u_a.ostiet[os].sakot,lsak+1);
					}
				}
			addtall(&u2, &d, 0);
			n++;
			}
		FormImport->EditLuettu->Text = UnicodeString(n);
		if (n%10 == 0) {
			FormImport->EditLuettu->Refresh();
			Sleep(1);
			}
		}
	fclose(vkilp);
	return(n);
	}
