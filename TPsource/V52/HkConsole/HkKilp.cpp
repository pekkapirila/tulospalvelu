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
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//#include <bvideo.h>
#include <bstrings.h>
#include "HkDeclare.h"
#include "HkConsoleDef.h"
#include <size.h>


extern INT16  setySize;
extern int maxEditNo;
extern EDITFLD editfld[];


void outform(void)
  {
  viwrrect(2,0,ySize-5,64,(char *)form,7,0,0);
  lukumaarat();
  }

int varivi(int piste)
   {
	int r = 0;
//   if (kilpparam.valuku)
//		r = editfld[VAE-1].y+piste;

   if (kuulpiste < ySize-editfld[VAE-1].y-4) {
      if (editfld[VAE-1].y+piste < ySize-4)
         return(editfld[VAE-1].y+piste);
      else
         return(-1);
      }
   else {
      if (piste > kuulpiste-ySize+editfld[VAE-1].y+4 && piste <= kuulpiste)
         return(piste+ySize-5-kuulpiste);
      else
         return(-1);
      }
   }

#ifdef SEURAVAL

seuraLuettelo sraLuett;

INT tall_seura(wchar_t *snimi, wchar_t *lyhenne, wchar_t *maa, int piiri, int kielto)
{
	sraLuett.TallennaSeura(piiri, maa, lyhenne, snimi, kielto);
	return(0);
}

/*
typedef struct {
   char nix[LSEURA+1];
   wchar_t nimi[LSEURA+1];
   wchar_t lyhenne[LLYH+1];
   wchar_t maa[4];
   INT piiri;
   } sratp;

sratp *seurataul;
static INT lue_seurat(INT kielto);

static INT nsrtaul, sramuutos, nsrmax;
INT tall_seura(wchar_t *snimi, wchar_t *lyhenne, wchar_t *maa, int piiri, int kielto)
   {
   sratp *sra;
   static int yritys = 0;
   char anm[LSEURA+1];
   wchar_t wanm[LSEURA+1] = L"";

	lah_seura(snimi, lyhenne, maa, piiri, kielto);
   if (!seurataul) {
      if (kielto && !yritys) {
         lue_seurat(kielto);
         yritys = 1;
         }
      if (!seurataul)
         return(1);
      }

	if (!snimi) return(1);

	wcstooem(anm, aakjarjwstr2(snimi), LSEURA);
    anm[LSEURA] = 0;


   if (nsrtaul >= nsrmax) {
      kirj_seurat();
      if (lue_seurat(kielto))
         return(1);
      }
   
   // Lähdetään taulukon alusta ja kasvatetaan osoitinta, kunnes
   // osoittimen rivillä sama tai myöhempi lyhenne

   for (sra = seurataul; sra - seurataul < nsrtaul; sra++) {
      if (strcmp(sra->nix, anm) >= 0)
         break;
      }

   // Jos sama lyhenne, vapautetaan ainakin pitka.
   // Jos snimi[0] == 0 poistetaan koko tietue,

   if (!strcmp(sra->nix, anm)) {
      if (!snimi[0]) {
         memmove(sra, sra + 1,
            (nsrtaul - 1 - (int) (sra - seurataul)) * sizeof(sratp));
         nsrtaul--;
         memset(seurataul+nsrtaul, 0, sizeof(sratp));
         sramuutos = 1;
         }
      }

   if (snimi) {
      if (strcmp(sra->nix, anm)) {
         memmove(sra+1, sra,
            (nsrtaul - (int) (sra - seurataul)) * sizeof(sratp));
         nsrtaul++;
         }
      strncpy(sra->nix, anm, LSEURA);
      wcsncpy(sra->nimi, snimi, LSEURA);
      wcsncpy(sra->lyhenne, lyhenne, LLYH);
      wcsncpy(sra->maa, maa, 3);
      sra->nix[LSEURA] = 0;
      sra->nimi[LSEURA] = 0;
      sra->piiri = piiri;
      sramuutos = 1;
      }
   if (sramuutos)
      lah_seura(snimi, lyhenne, maa, piiri, kielto);
   return(0);
   }
*/

INT hae_seura(wchar_t *snimi, wchar_t *lyhenne, UINT16 *piiri, INT etsi)
   {
   static INT init = 0;
   char *svscr = 0;
   wchar_t ch, ch2 = 0, line[100], sn[LSEURA+1], maa[4] = L"   ";
   INT i, fore, selattu = 0;
   seuraTieto *sra;

   if (init == -1) return(1);
   if (!init) {
	  sraLuett.jarj = kilpparam.seuralyh_on ? 2 : 1;
	  wcscpy(sraLuett.erotin, L" ;\t");
	  sraLuett.lueSeurat(L"seurat.csv");
	  if (sraLuett.NSeura == 0) {
         init = -1;
         return(1);
         }
      init = 1;
      }
   if (sraLuett.NSeura == 0)
      return(1);
   status_on = 0;
   if (kilpparam.seuralyh_on)
		sra = sraLuett.haelyhenteella(lyhenne, false);
	else
		sra = sraLuett.haenimella(snimi, false);
	if (sra == NULL)
		return(1);
   if (!etsi) {
	  if ((!kilpparam.seuralyh_on && !wcscmp(snimi, sra->nimi)) 
	  || (kilpparam.seuralyh_on && !wcscmp(lyhenne, sra->lyhenne))) {
	      *piiri = sra->piiri;
		  wcscpy(snimi, sra->nimi);
		  wcscpy(lyhenne, sra->lyhenne);
		  return(0);
		  }
	  else 
		return(1);
	  }
//   if (etsi || (!kilpparam.seuralyh_on && wcscmp(snimi, sra->nimi)) 
//	  || (kilpparam.seuralyh_on && wcscmp(snimi, sra->lyhenne))) {
   if (etsi) {
      svscr = savescr(3,34,ySize-6,79);
      scbox(3, 34, ySize-6, 79, 15, 0, 7);
      do {
         clrln(ySize-5);
         vidspwmsg(ySize-5,0,7,0,
            L"Selaa: Up Down PgUp PgDn, Enter : hyväksy, Esc: Peruuta");
//            L"Selaa: Up Down PgUp PgDn, Enter : hyväksy, +: Lisää, -: Poista, Esc: Peruuta");
         for (i = -7; i < 8; i++) {
            if (sra - sraLuett.Seura >= -i && sra - sraLuett.Seura < sraLuett.NSeura - i) {
               swprintf(line, L"%-10.10s %-30.30s %2d",
                  sra[i].lyhenne, sra[i].nimi, sra[i].piiri);
               fore = i ? 7 : 0;
               viwrrectw(i+11, 35, i+11, 78, line, fore, 7-fore, 0);
               }
            else
               clrtxt(i+11, 35, 78);
            }
         ch = readkbd_w(&ch2, TRUE);
         switch (ch) {
/*
            case L'+' :
               ch2 = selattu ? L'K' : L'E';
               wselectopt(L"Valitse korostettu rivi pohjaksi (K/E)", L"KE", &ch2);
               clrln(ySize-1);
               vidspwmsg(ySize-1,0,7,0,
                  L"Lyhenne:             Pitkä nimi:              "
                  L"                  Piiri:");
               if (ch2 == L'K') {
                  wcsncpy(sn, sra[0].lyhenne, LLYH);
                  wcsncpy(line, sra[0].nimi, LSEURA);
                  line[LSEURA] = 0;
                  piiri = sra[0].piiri;
                  }
               else {
                  wcsncpy(sn, snimi, LLYH);
                  wcsncpy(line, snimi, LSEURA);
                  piiri = 0;
                  }
               inputwstr(sn, LLYH, 9, ySize-1, L"\r", &ch2, 0);
               inputwstr(line, LSEURA, 34, ySize-1, L"\r", &ch2, 0);
               INPUTINTW(&piiri, 2, 71, ySize-1, L"\r", &ch2);
               ch2 = L' ';
               wselectopt(L"Vahvista lisäys (K/E)", L"KE", &ch2);
               if (ch2 == L'K') {
                  elimwbl(sn);
                  for (p = sn; *p; p++)
                     if (*p == L' ') *p = L'_';
                  wcstooem((char*) animi, aakjarjwstr2(sn), LLYH);
                  animi[LSEURA] = 0;
                  sra = (sratp *) bsrch((char *) animi, (char *) seurataul,
                     &nsrtaul, sizeof(sra->nix), sizeof(sratp));
                  if (!strcmp(sra->nix, animi)) {
                     ch2 = L'E';
                     wselectopt (L"Lyhenne jo käytössä, korvataanko vanha tieto (K/E)",
                        L"KE", &ch2);
                     }
                  else
                     ch2 = L'K';
                  if (ch2 == L'K')
                     tall_seura(line, sn, maa, piiri, 0);
                  }
               clrln(ySize-3);
               clrln(ySize-1);
               break;
            case L'-' :
               ch2 = L' ';
               wselectopt(L"Korostettu tieto poistetaan - Vahvista (K/E)", L"KE",
                  &ch2);
               if (ch2 == L'K')
                  tall_seura(sra->nimi, 0, 0, 0, 0);
               clrln(ySize-3);
               break;
*/
            case KB_CR :
               break;
            case ESC :
               break;
            case 0:
               selattu = 1;
               switch (ch2) {
                  case 72:
                     if (sra > sraLuett.Seura)
                        sra--;
                     break;
                  case 80:
                     sra++;
                     break;
                  case 73:
                     i = (INT) (sra - sraLuett.Seura);
                     sra -= min(i, 14);
                     break;
                  case 81:
                     sra += 14;
                     break;
                  case 132:
                     sra = sraLuett.Seura;
                     break;
                  case 118:
                     sra = sraLuett.Seura+sraLuett.NSeura-1;
                     break;
                  }
               break;
            default:
               if (ch >= L'A') {
                  memset(sn, 0, sizeof(sn));
                  sn[0] = ch;
				  if (kilpparam.seuralyh_on)
					 sra = sraLuett.haelyhenteella(sn, false);
				  else
					 sra = sraLuett.haenimella(sn, false);
                  }
               break;
            }
         if (sra > sraLuett.Seura+sraLuett.NSeura-1)
            sra = sraLuett.Seura+sraLuett.NSeura-1;
         } while (ch != KB_CR && ch != ESC);
      restorescr(3,34,ySize-6,79,svscr);
      }
   clrln(ySize-5);
   status_on = 1;
   if (ch != ESC) {
		wcscpy(snimi, sra->nimi);
		wcscpy(lyhenne, sra->lyhenne);
		*piiri = sra->piiri;
		}
   return(ch == ESC);
   }
#endif

   void draw_form(int osa)
{
	INT i, r, tuy, vay, n_pv, nva;

	tuy = editfld[LAHE-1].y;
	vay = editfld[VAE-1].y;
	n_pv = kilpparam.n_pv_akt;
	nva = kilpparam.valuku;
	if (kilpparam.seuralyh_on)
		editfld[SELE-1].active = 1;

#ifdef AMPUSUUNN
	char lajistr[3][11] = {"Kartanluku", "   Ammunta", "Suunnistus"};
#endif

	switch(osa) {
		case 2:
			for (i = 0; i < ySize-3; i++)
				clrtxt(i, 66, 79);
			draw_vline2(0, 65, ySize-3);
			draw_grchar2(1, 65, 14);
			draw_grchar2(7, 65, 11);
			draw_grchar2(16, 65, 11);
			draw_grchar2(ySize-4, 65, 7);
			draw_hline2(7,66,14);
			draw_hline2(16,66,14);
			draw_hline2(ySize-4,66,14);
#ifndef LANGSV_K
			viwrrect(LUKUMY,67,LUKUMY,74,"Ilmoitt.",7,0,0);
			viwrrect(LUKUMY+1,67,LUKUMY+1,71,"Tulos",7,0,0);
			viwrrect(LUKUMY+2,67,LUKUMY+2,75,"Keskeytt.",7,0,0);
			viwrrect(LUKUMY+3,67,LUKUMY+3,73,"Hylätty",7,0,0);
			viwrrect(LUKUMY+4,67,LUKUMY+4,74,"Ei läht.",7,0,0);
			viwrrect(LUKUMY+5,67,LUKUMY+5,73,"Avoinna",7,0,0);
#ifdef EMIT
			viwrrect(LUKUMY+6,67,LUKUMY+6,72,"Esitys",7,0,0);
#endif
			viwrrect(8,67,8,74,"TULOSTUS",7,0,0);
			if (spoolfl) {
				viwrrect(9,67,9,79,"Tausta     ON",7,0,0);
				}
			else
				viwrrect(9,67,9,79,"Tausta     EI",7,0,0);
			viwrrect(12,67,12,79,"Auto       EI",7,0,0);
			if (comfl) {
				viwrrect(17,67,17,77,"TIED.SIIRTO",7,0,0);
				viwrrect(19,67,19,71,"Läht:",7,0,0);
				viwrrect(18,67,18,71,"Saap:",7,0,0);
				viwrrect(20,67,20,71,"Yht.:",7,0,0);
				}
#else  // LANGSV_K
			viwrrect(LUKUMY,2,LUKUMY,8,"Anmälda",7,0,0);
			viwrrect(LUKUMY+1,2,LUKUMY+1,6,"Tider",7,0,0);
			viwrrect(LUKUMY+2,2,LUKUMY+2,9,"Avbrytna",7,0,0);
			viwrrect(LUKUMY+3,2,LUKUMY+3,7,"Diskv.",7,0,0);
			viwrrect(LUKUMY+4,2,LUKUMY+4,9,"Ej start",7,0,0);
			viwrrect(LUKUMY+5,2,LUKUMY+5,6,"Öppna",7,0,0);
#ifdef EMIT
			viwrrect(LUKUMY+6,2,LUKUMY+6,8,"Förslag",7,0,0);
#endif
			viwrrect(8,2,8,12,"UTSKRIVNING",7,0,0);
			if (spoolfl) {
				viwrrect(9,2,9,14,"Bakgrund   På",7,0,0);
				}
			else
				viwrrect(9,2,9,14,"Bakgrund   Av",7,0,0);
			viwrrect(12,2,12,14,"Auto       Av",7,0,0);
			if (comfl) {
				viwrrect(17,2,17,11,"ÖVERFÖRING",7,0,0);
				viwrrect(19,2,19,6,"Ut  :",7,0,0);
				viwrrect(18,2,18,10,"Mottagna:",7,0,0);
				viwrrect(20,2,20,7,"Total:",7,0,0);
				}
#endif  // LANGSV_K
			break;

		case 1:
			for (i = 0; i < ySize-4; i++)
				clrtxt(i, 0 , 64);

#if !defined(AMPUSUUNN)
#if N_PV > 1
			if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
//#ifdef MERI5
				viwrrect(tuy-2,1,tuy-2,62,
					 "Pv | Lähtöaika |   Tulos   | Tark | Pisteet   | Sija  | Yht.sj", 7,0,0);
				}
			else {
//#else
				viwrrect(tuy-2,1,tuy-2,62,
 					"Pv | Lähtöaika |   Tulos   | Tark | Yht.tulos | Sija  | Yht.sj", 7,0,0);
				}
//#endif
			for (i = 0; i < n_pv; i++)
				vidint(tuy + i * RV, 2, 1, i+1);
#else
#ifndef LANGSV_K
			viwrrect(TUY-2,1,TUY-2,62,
			  "   | Lähtöaika |   Tulos   | Tark |           | Sija  |       ",7,0,0);
#else
			viwrrect(TUY-2,1,TUY-2,62,
			  "   | Starttid  |  Resultat | Kntrl|           | Plats |       ",7,0,0);
#endif
#endif
			draw_hline(tuy-1,1,62);
			draw_vline(tuy-2, 4, 2+RV*n_pv);
			draw_vline(tuy-2, 16, 2+RV*n_pv);
			draw_vline(tuy-2, 28, 2+RV*n_pv);
			draw_vline(tuy-2, 35, 2+RV*n_pv);
			draw_vline(tuy-2, 47, 2+RV*n_pv);
			draw_vline(tuy-2, 55, 2+RV*n_pv);
			draw_grchar(tuy-1, 4, 15);
			draw_grchar(tuy-1,16, 15);
			draw_grchar(tuy-1,28, 15);
			draw_grchar(tuy-1,35, 15);
			draw_grchar(tuy-1,47, 15);
			draw_grchar(tuy-1,55, 15);
#if N_PV == 1
			draw_hline(tuy+1,1,62);
			draw_grchar(tuy+1, 4, 7);
			draw_grchar(tuy+1,16, 15);
			draw_grchar(tuy+1,28, 15);
			draw_grchar(tuy+1,35, 15);
			draw_grchar(tuy+1,47, 15);
			draw_grchar(tuy+1,55, 15);
#endif
			if (nva > 0) {
				i = nva;
				if (i > ySize-vay-5)
					i = ySize-vay-5;
				draw_vline(vay , 4, i);
				draw_vline(vay ,16, i);
				draw_vline(vay ,28, i);
				draw_vline(vay ,35, i);
				draw_vline(vay ,47, i);
				draw_vline(vay ,55, i);
#ifndef LANGSV_K
				vidspmsg(vay,1,7,0,"Väliaika");
#else
				vidspmsg(vay,1,7,0,"Mellantid");
#endif
				vidspmsg(vay+1,1,7,0,"        ");
				for (i = 0; i < nva-1; i++)
					if ((r = varivi(i)) >= 0) 
						vidint(r , 9, 4, i+1);
				}

			for (i = 0; i < maxEditNo; i++) {
				if (editfld[i].active != 0 && editfld[i].ots[0] != 0)
					vidspwmsg(editfld[i].y, editfld[i].xots, 7, 0, editfld[i].ots);
				}
		}
}

void initscreens(void)
{
#ifdef AMPUSUUNN
	char lajistr[3][11] = {"Kartanluku", "   Ammunta", "Suunnistus"};
#endif

   ySize = setySize;
	editfld[LAH1E-1].active = pvparam[k_pv].hiihtolahto;
	editfld[SAKE-1].active = kilpparam.lsakmax > 0;
//#ifdef MERI5
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		editfld[SAKE-1].len = 4;
		if (kilpparam.kilplaji == L'P' && wcswcind(pvparam[k_pv].alalaji, L"HT") >= 0)
			wcscpy(editfld[SAKE-1].ots, L"  Pisteet");
		else
			wcscpy(editfld[SAKE-1].ots, L"Sakkosek.");
		editfld[RATAE-1].y = editfld[EME-1].y;
		editfld[RATAE-1].active = true;
		editfld[EME-1].active = false;
		editfld[LAINAE-1].active = false;
		editfld[ERE-1].y = editfld[EME-1].y;
		editfld[ERE-1].active = true;
		}
	if (wcswcind(kilpparam.kilplaji, L"U") >= 0) {
		editfld[RATAE-1].y = editfld[EME-1].y;
		editfld[RATAE-1].active = true;
		editfld[EME-1].active = false;
		editfld[LAINAE-1].active = false;
		editfld[ERE-1].y = editfld[EME-1].y;
		editfld[ERE-1].active = true;
		}
//#endif
   editfld[VAE-1].y = editfld[TLSE-1].y + kilpparam.n_pv_akt + 2;
   if (ySize == 50)
      resizeConBufAndWindow(hConOut, xSize, ySize);
//	  resizeConBufAndWindow2(xSize, ySize);
   draw_form(2);
	if (!autofl) {
      virdrect(0,65,ySize-4,79,(char *)kehys1,0);
      }
   else {
      virdrect(0,65,ySize-14,79,(char *)kehys1,0);
      virdrect(ySize-9,65,ySize-4,79,(char *)kehys1 + (ySize-9)*15,0);
		if (ySize > 30)
			virdrect(ySize-9,65,ySize-6,79,(char *)kehys1 + (ySize-13)*15,0); 
      }
	draw_form(1); 
	virdrect(2,0,ySize-5,64,(char *)form,0);
	chgchar((char *)form, 65*(ySize-6));
	scpclr();
	sccurset(0,0);
}

void kilptietue::outkilp(INT ntlisa)
   {
   wchar_t tarkstr[16], *snimi, prs[100];
   wchar_t strtmaali[20] = L"           ", strtulos[20] = L"           ";
   INT i, pst = 0, i_pv, e_pv = 0, v_pv = N_PV - 1;
   INT32 tls;
   time_t tm_t;
   int paivays = 0;
   struct tm *dttm;
#ifdef  AMPUSUUNN
   wchar_t ostlsst[16], os1[3], os2[3], mms[4];
   int fore, r;
   INT32 tl, ostls;
#endif
   INT hv = 1;
   INT32 ytls = 0;
   wchar_t strytulos[20] = L"           ";
   kilptietue kilp1;

   kilp1 = *this;
   v_pv = kilpparam.n_pv_akt - 1;
   outform();
   if (kilpparam.kilplaji == L'S' && Sarja() >= 0) { 
		editfld[TAE-1].active = (Sarjat[Sarja()].rrtas > 1);
		editfld[RATAE-1].active = monirata;
		if (editfld[RATAE-1].active)
			vidspwmsg(editfld[RATAE-1].y, editfld[RATAE-1].xots-7, 7, 0, editfld[RATAE-1].ots);
		}
   if (editfld[TAE-1].active) {
	   vidspwmsg(editfld[TAE-1].y, editfld[TAE-1].xots, 7, 0, editfld[TAE-1].ots);
	   }
   if (Sarja() >= 0 && Sarja() < sarjaluku) {
      pst = kuulpiste <= Sarjat[Sarja()].valuku[k_pv] ? kuulpiste : 0;
	  EnterCriticalSection(&tall_CriticalSection);
      for (i_pv = 0; i_pv <= Sarjat[Sarja()].valuku[k_pv]; i_pv++)
         haesija(this,i_pv, 0, 0);
	  LeaveCriticalSection(&tall_CriticalSection);
      snimi = Sarjat[Sarja()].sarjanimi;
#if N_PV > 1
      e_pv = Sarjat[Sarja()].enspv;
      v_pv = Sarjat[Sarja()].viimpv;
#endif
      }
   else snimi = L"";
   vidspwmsg(editfld[SRJE-1].y,editfld[SRJE-1].x,7,0,snimi);
   if (tarknofl)
      swprintf(prs,L"%4d-%1d",id(),tarkno(id()));
   else
      swprintf(prs,L"%4d",id());
   vidspwmsg(editfld[NOE-1].y, editfld[NOE-1].x,7,0,prs);
   vidspwmsg(editfld[SNME-1].y, editfld[SNME-1].x,7,0,sukunimi);
   vidspwmsg(editfld[ENME-1].y, editfld[ENME-1].x,7,0,etunimi);
	if (editfld[ARVE-1].active)
		vidspwmsg(editfld[ARVE-1].y, editfld[ARVE-1].x, FORE_T, BKG_T, arvo);
#ifndef TUNTURI
   vidint(editfld[LISE-1].y, editfld[LISE-1].x, LISNOL, lisno[0]);
#endif
	if (editfld[WRE-1].active)
	   vidspwmsg(editfld[WRE-1].y, editfld[WRE-1].x, FORE_T, BKG_T, wrkoodi);
//	if (wcswcind(kilpparam.kilplaji, L"NPU") < 0) {
	if (editfld[EME-1].active) {
		vidint(editfld[EME-1].y, editfld[EME-1].x, EML, pv[k_pv].badge[0]);
		if (kilpparam.kaksibadge)
			vidint(editfld[EME-1].y+1, editfld[EME-1].x, EML, pv[k_pv].badge[1]);
		}
	if (editfld[LAINAE-1].active) {
		wcscpy(tarkstr, pv[k_pv].laina[0] == L'L' ? L"Laina" : L"Oma  ");
		vidspwmsg(editfld[LAINAE-1].y, editfld[LAINAE-1].x, 7, 0, tarkstr);
		}
#ifdef TUNTURI
	    vidint(LIY, EMX+12, EML, badge2);
	    wcscpy(tarkstr, laina2 == L'L' ? L"Laina" : L"Oma  ");
	    viwrrectw(LIY, EMX+EML+13, LIY, EMX+EML+17, tarkstr, 7,0,0);
#endif
	if (editfld[RATAE-1].active)
	   vidspwmsg(editfld[RATAE-1].y, editfld[RATAE-1].x,7,0,pv[k_pv].rata);
//		}
//	else {
//		vidspwmsg(editfld[ERE-1].y, editfld[ERE-1].xots,7,0, L"           Erä : ");
	if (editfld[ERE-1].active)
	    vidint(editfld[ERE-1].y, editfld[ERE-1].x, 2, pv[k_pv].era);
//		if (editfld[RATAE-1].active)
//			vidint(editfld[RATAE-1].y, editfld[RATAE-1].x,2,pv[k_pv].rata[0]);
//		}
	if (editfld[SEE-1].active)
	   vidspwmsg(editfld[SEE-1].y, editfld[SEE-1].x,7,0,seura);
	if (editfld[SELE-1].active)
	   vidspwmsg(editfld[SELE-1].y, editfld[SELE-1].x,7,0,seuralyh);
	if (editfld[MAAE-1].active)
	   vidspwmsg(editfld[MAAE-1].y, editfld[MAAE-1].x,7,0,maa);
	if (editfld[YHDE-1].active)
	   vidspwmsg(editfld[YHDE-1].y, editfld[YHDE-1].x,7,0,yhdistys);
	if (editfld[ALUE-1].active)
		vidint(editfld[ALUE-1].y, editfld[ALUE-1].x,2,piiri);

   for (i_pv = e_pv; i_pv <= v_pv; i_pv++) {
#ifndef AMPUSUUNN
      if (pv[i_pv].tlahto != TMAALI0)
         VIDSPAIKA(editfld[LAHE-1].y+i_pv*RV, editfld[LAHE-1].x,pv[i_pv].tlahto,t0);
      if ((tls = tulos_pv(i_pv)) != 0) {
         AIKATOWSTRS(strtulos, tls, 0);
         strtulos[pvparam[i_pv].laika] = 0;
         if (Sarja() >= 0 && Sarja() < sarjaluku && i_pv == k_pv && p_sija(0) < 1000)
			swprintf(prs,L"%3d/%-3d", p_sija(0), ntulos[Sarja()][0]+ntlisa);
		 else
			swprintf(prs,L"%5d", sija(i_pv));
         vidspwmsg(editfld[SJE-1].y+i_pv*RV, editfld[SJE-1].x,7,0,prs);
//#ifdef MERI5
		if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 || (i_pv > e_pv && Sarja() >= 0)) {
			 if (hv) {
				if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
					swprintf(strytulos,L"%4d/%5d", pv[i_pv].pvpisteet[0], (PISTEKATTO-meriytulos(this, i_pv))/SEK);
					}
				else {
// #else
					ytls = ytulos(i_pv);
					AIKATOWSTRS(strytulos, ytls, 0);
					strytulos[pvparam[i_pv].laika] = 0;
					}
//#endif
			   if (i_pv == k_pv) {
				  EnterCriticalSection(&tall_CriticalSection);
				  haesija(this, kilpparam.valuku+1, 0, 0);
				  LeaveCriticalSection(&tall_CriticalSection);
				  swprintf(prs,L"%3d/%-3d",y_sija(k_pv),
					 ntulos[Sarja()][kilpparam.valuku+1]+ntlisa);
				  vidspwmsg(editfld[YSJE-1].y+i_pv*RV, editfld[YSJE-1].x,7,0,prs);
				  }
			   else
				  vidint(editfld[YSJE-1].y+i_pv*RV, editfld[YSJE-1].x,3, y_sija(i_pv));
				}
			}
	    }
      else {
         hv = 0;
         wcscpy(strtulos, L"  ----  ");
         }
#endif // !AMPUSUUNN
	  if (kilpparam.lsakmax == 0) {
		  switch (tark(i_pv)) {
			 case L'K' :
				wcscpy(tarkstr, L"Kesk.");
				break;
			 case L'H' :
				wcscpy(tarkstr, L"Hyl.");
				break;
			 case L'E' : 
				wcscpy(tarkstr, L"Eil.");
				break;
			 case L'T' :
				wcscpy(tarkstr, L"Trk.");
				break;
			 case L'I' :
				wcscpy(tarkstr, L"esIt");
				break;
			 case L'V' : 
				wcscpy(tarkstr, L"Vak.");
				break;
			 case L'P' : 
				wcscpy(tarkstr, L"Pois");
				break;
			 default  : 
				wcscpy(tarkstr, L"-");
				break;
			 }
		  }
	else {
//#ifdef MERI5
		if (wcswcind(kilpparam.kilplaji, L"NP") >= 0)
			  _itow(sakko_pv(i_pv)/SEK, tarkstr, 10);
		else
			  wcscpy(tarkstr, pv[i_pv].asakot);
//#endif
		  if (!hyv_pv(i_pv)) {
			 tarkstr[wcslen(tarkstr) + 1] = 0;
			 tarkstr[wcslen(tarkstr)] = tark(i_pv);
			 }
		  if (Sarja(i_pv) >= 0 && Sarja(i_pv) < sarjaluku && i_pv == k_pv) {
			 if (Sarjat[Sarja(i_pv)].lsak[i_pv]) {
				for (i = 0; i < Sarjat[Sarja(i_pv)].lsak[i_pv]; i++) {
					if (editfld[SAKE-1].len == 1 || pv[i_pv].asakot[i] < L'0')
						viwrrectw(editfld[SAKE-1].y+i_pv*RV, editfld[SAKE-1].x+(editfld[SAKE-1].len+1)*i, 
							editfld[SAKE-1].y+i_pv*RV, editfld[SAKE-1].x+(editfld[SAKE-1].len+1)*i, tarkstr+i, 7,0,0);
					else {
						vidint(editfld[SAKE-1].y, editfld[SAKE-1].x+(editfld[SAKE-1].len+1)*i, editfld[SAKE-1].len, 
							pv[i_pv].asakot[i]-L'0');
						}
					}
				}
			 }
		}
#endif
#ifndef AMPUSUUNN
      vidspwmsg(editfld[TRKE-1].y+i_pv*RV, editfld[TRKE-1].x,7,0,tarkstr);
      vidspwmsg(editfld[TLSE-1].y+i_pv*RV, editfld[TLSE-1].x,7,0,strtulos);
//#ifndef MERI5
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 || i_pv > 0)  {
// #endif
         if (!hv)
            wcscpy(strytulos, L"  ----  ");
	     vidspwmsg(editfld[YTLSE-1].y+i_pv*RV, editfld[YTLSE-1].x,7,0,strytulos);
         }
      }
#ifndef EBA
   wcscpy(strtmaali, L"00.00.00");
   if (pst) {
      vidspwmsg(editfld[MAE-1].y, editfld[MAE-1].xots-7, 7, 0, L"Väliajan    aika");
      vidint(editfld[MAE-1].y, editfld[MAE-1].xots+1, 3, pst);
      }
   else
      vidspwmsg(editfld[MAE-1].y, editfld[MAE-1].xots-7, 7, 0, L"       Maaliaika");
   if (p_aika( pst) != 0) {
      AIKATOWSTRS(strtmaali, maali(pst), t0);
      }
   strtmaali[pvparam[k_pv].laika] = 0;
   vidspwmsg(editfld[MAE-1].y, editfld[MAE-1].x,7,0,strtmaali);
#endif
   if (editfld[LAH1E-1].active) {
	   AIKATOWSTRS(strtmaali, p_aika(-1), t0);
	   strtmaali[pvparam[k_pv].laika] = 0;
	   vidspwmsg(editfld[LAH1E-1].y, editfld[LAH1E-1].x,7,0,strtmaali);
	   }
   if (Sarja() >= 0 && Sarja() < sarjaluku) {
		for (int iva = 1; iva <= 2; iva++) {
	      if (Sarjat[Sarja()].valuku[k_pv] < kilpparam.valuku-iva+1 && 
			  pv[0].va[kilpparam.valuku+2-iva].vatulos > 24*TUNTI) {
			  int r;

		      tm_t = pv[0].va[kilpparam.valuku+2-iva].vatulos;
			   dttm = localtime(&tm_t);
				swprintf(strtulos, L"%02d.%02d.%4d", dttm->tm_mday, dttm->tm_mon+1,
                                                   dttm->tm_year+1900);
	          r = varivi(kilpparam.valuku-iva);
			  if (r > 0) {
				  vidspwmsg(r, editfld[VAE-1].x, 7, 0, strtulos);
			      vidint(r, editfld[SJE-1].x, 3, pv[0].va[kilpparam.valuku+2-iva].vasija);
				  paivays = 1;
				  }
				}
         }
	  if (Sarja() >= 0) {
		  for (int iva = 0; iva < Sarjat[Sarja()].valuku[k_pv]; iva++) {
			 if (varivi(iva) > 0) {
				vidint(varivi(iva) , 10, 3, iva+1);
				if ((tls = p_aika(iva+1)) != 0) {
					AIKATOWSTRS(strtulos, tls, 0);
					strtulos[pvparam[k_pv].laika] = 0;
					vidspwmsg(varivi(iva), editfld[VAE-1].x, 7, 0, strtulos);
					i = editfld[SJE-1].x;
					if (kilpparam.n_pv_akt == 1) {
						if (k_pv) {
							if (tls && (ytls = ytulos(k_pv-1)) != 0) {
								ytls = pyoristatls(ytls, 4);
								tls += ytls;
								}
							else 
								tls = 0;
							}
						AIKATOWSTRS(strtulos, tls, 0);
						strtulos[pvparam[k_pv].laika] = 0;
						vidspwmsg(varivi(iva), editfld[VAE-1].x, 7, 0, strtulos);
						if (Sarjat[Sarja()].luonne[k_pv] == 2) i = editfld[SJE-1].x;
						}
					vidint(varivi(iva),i, 3, p_sija(iva+1));
					}
				}
			 }
		  for (int iva = Sarjat[Sarja()].valuku[k_pv]; iva < kilpparam.valuku; iva++) {
			 if (varivi(iva) > 0) {
				clrtxt(varivi(iva), 0, 64);
				}
			 }
		  }
      }
   if (editfld[TAE-1].active && Sarja() >= 0 && Sarjat[Sarja()].tsak[k_pv] > 0)
		vidint(editfld[TAE-1].y, editfld[TAE-1].x,0,pv[k_pv].tasoitus/Sarjat[Sarja()].tsak[k_pv]);
#else  // AMPUSUUNN
      }
   viwrrectw(RYY, RYX, RYY, RYX, ryhma, 7, 0, 0);
   if (Sarja() >= 0 && Sarja() < sarjaluku) {
      for (i = 0; i <= Sarjat[Sarja()].valuku; i++) {
         fore = (i == kuulpiste) ? 0 : 7;
         wcscpy(ostlsst, L"00.00.00,0");
         ostls = 0;
         if ((tls = kilp->p_aika( i)) != 0) {
            AIKATOWSTRS(strtmaali, maali(i), t0);
            ostls = ostulos(kilp, i, maali(i));
            }
         else
            wcscpy(strtmaali, L"  .  .  , ");
         r = i ? TUY - 6 + 2*i : TUY;
         switch (aslaji[asjarjestys][i]) {
            case 0:      // Kartanluku
               if (mmsakko < 0)
                  wcscpy(mms, L" ?");
               else
                  itoa(mmsakko, mms, 10);
               viwrrectw(r, SKX, r, SKX+1, mms, fore, 7-fore, 0);
               break;
            case 1:      // Ammunta
               if (ohi[0] < 0)
                  wcscpy(os1, L" ?");
               else
                  itoa(ohi[0], os1, 10);
               if (ohi[1] < 0)
                  wcscpy(os2, L" ?");
               else
                  itoa(ohi[1], os2, 10);
               viwrrectw(r, SKX-1, r, SKX, os1, fore, 7-fore, 0);
               viwrrectw(r, SKX+2, r, SKX+3, os2, fore, 7-fore, 0);
               break;
            case 2:      // Suunnistus
               vidspwmsg(r,TRX,fore,7-fore,tarkstr);
               break;
            }
         switch (i) {
            case 1:
               tl = pv[0].tlahto;
               break;
            case 2:
               tl = maali(1);
               break;
            case 0:
               tl = maali(2);
               break;
            }
         if (tl != TMAALI0)
            AIKATOWSTRS(strtulos, tl, t0);
         else
            wcscpy(strtulos, L"  .  .  ");
         viwrrectw(r, LAX, r, LAX+laika-1, strtulos, fore, 7-fore, 0);
         viwrrectw(r, MAX, r, MAX+laika-1, strtmaali, fore, 7-fore, 0);
         if (tls && (i || hyv(kilp))) {
            AIKATOWSTRS(strtulos, tls, 0);
            swprintf(prs,L"%d",sija[i]);
            vidspwmsg(r,SIX,fore,7-fore,prs);   
            }
         else
            wcscpy(strtulos, L"00.00.00,0");
         if (ostls)
            AIKATOWSTRS(ostlsst, ostls, 0);
         strtulos[laika] = 0;
         ostlsst[laika] = 0;
         vidspwmsg(r,TUX,fore,7-fore,strtulos);
         vidspwmsg(r,OSX,fore,7-fore,ostlsst);
         }
      }
#endif
   *this = kilp1;
   }

INT kilptietue::inputkilp(INT addfl)
   {
   INT  l,r,n,nt0,nc, i_pv = 0, va, x, y, len;
   wchar_t tc,kh,ch;
   wchar_t tarkstr[11];
   static wchar_t msg[81] = L"";
   wchar_t snimi[11];
   INT srj, vapiste;
	INT16 kno;
   INT32 tm[VALUKU+1], tls;
   EDITFLD *efld;
#ifdef SEURAVAL
   static wchar_t term[] = L"+\x2\x3\x5\x9\xD\x12\x13\x18\x1B\xCA";
#else
   static wchar_t term[] = L"+\x2\x3\x5\x9\xD\x12\x13\x18\x1B";
#endif
   static wchar_t termp[] = L"+\x2\x3\x5\x9\xD\x12\x13\x18\x1B";
   INT nt;
   INT32 bdg;
#if defined(AMPUSUUNN)
   int fore;
#endif
   i_pv = min(k_pv, Sarjat[Sarja()].viimpv);
   i_pv = max(i_pv, Sarjat[Sarja()].enspv);
   vapiste = kuulpiste;
   if (vapiste > Sarjat[Sarja()].valuku[k_pv])
	   vapiste = Sarjat[Sarja()].valuku[k_pv];
   tc = KB_CR;
   nc = maxEditNo;
   kno = id();
   l = 1;
#ifdef EBA
   if (!vapiste)
      vapiste = 1;
#endif
   if (korjfl && !addfl)
#ifdef EMIT
      if (emitkd) 
		  l = EME;
      else
#endif
      if (Sarjat[Sarja()].lsak[k_pv]) 
		  l = TAE;
      else if (kilpparam.kilplaji == L'S')
         l = TRKE;
	  else
		 l = SNME;
   clrln(ySize-3);
   do {
      viwrrectw(ySize-3,0,ySize-3,72,L"<Return>, \x18 \x19  : Vaihda kenttää,  "
         L"+ : Hyväksy,  <Esc> : Peruuta ja poistu",7,0,0);
	  efld = editfld+l-1;
	  x = efld->x;
	  y = efld->y;
	  len = efld->len;
	  if (efld->active != 0) {
		  switch(l) {
			 case SNME :
				inputwstr(sukunimi,len,x, y,term,&tc,FALSE);
				elimwbl(sukunimi);
				wmemset(sukunimi+wcslen(sukunimi)+1, 0,
				   LSNIMI-wcslen(sukunimi));
				break;
			 case ENME :
				inputwstr(etunimi,len,x, y,term,&tc,FALSE);
				elimwbl(etunimi);
				wmemset(etunimi+wcslen(etunimi)+1, 0,
				   LENIMI-wcslen(etunimi));
				break;
			 case ARVE :
				inputwstr(arvo, len, x, y,term,&tc,FALSE);
				break;
			 case LISE :
			    inputlongw(&lisno[0], LISNOL,x, y,term,&tc);
				break;
			 case WRE :
				inputwstr(wrkoodi, len, x, y, term, &tc, 0);
				break;
			 case EME :
				if (kilpparam.kilplaji != L'N' && kilpparam.kilplaji != L'P') {				
					bdg = pv[k_pv].badge[0];
					inputlongw(&pv[k_pv].badge[0],len , x, y,term,&tc);
					if (tc == ESC) break;
					if (!emitfl && bdg != pv[k_pv].badge[0])
					   writeerror_w(L"Emit-toimintoa ei käynnistetty - "
						  L"haitallisia syöttövirheitä ei estetä", 0);
					if (bdg != pv[k_pv].badge[0] && k_pv < kilpparam.n_pv_akt - 1) {
						for (i_pv = k_pv+1; i_pv <= kilpparam.n_pv_akt; i_pv++)
							pv[i_pv].badge[0] = pv[k_pv].badge[0];
						}
					if (kilpparam.kaksibadge) {
						bdg = pv[k_pv].badge[1];
						inputlongw(&pv[k_pv].badge[1],len , x, y+1,term,&tc);
						if (tc == ESC) break;
						if (!emitfl && bdg != pv[k_pv].badge[1])
						   writeerror_w(L"Emit-toimintoa ei käynnistetty - "
							  L"haitallisia syöttövirheitä ei estetä", 0);
						if (bdg != pv[k_pv].badge[1] && k_pv < Sarjat[Sarja()].viimpv) {
							for (i_pv = k_pv+1; i_pv <= Sarjat[Sarja()].viimpv; i_pv++)
								pv[i_pv].badge[1] = pv[k_pv].badge[1];
							}
						}
					}
				break;
			 case LAINAE :
				if (kilpparam.kilplaji != L'N' && kilpparam.kilplaji != L'P') {				
					if (tc == L'\r' || tc == KB_TAB || tc == DOWN) {
					   wcscpy(tarkstr, pv[k_pv].laina[0] == L'L' ? L"Laina" : L"Oma");
					   inputwstr(tarkstr, len, x, y, term, &tc, FALSE);
					   pv[k_pv].laina[0] = (towupper(tarkstr[0]) == L'L') ? L'L' : 0;
					   }
#ifdef TUNTURI
					if (tc == L'\r' || tc == KB_TAB || tc == DOWN)
					   inputlongw(&pv[k_pv].badge[1],EML,EMX+12,LIY,term,&tc);
					if (tc == L'\r' || tc == KB_TAB || tc == DOWN) {
					   wcscpy(tarkstr, pv[k_pv].laina[1] == L'L' ? L"Laina" : L"Oma");
					   inputwstr(tarkstr, 5, EMX+EML+1, LIY, term, &tc, FALSE);
					   pv[k_pv].laina[1] = (towupper(tarkstr[0]) == L'L') ? L'L' : 0;
					   }
#endif
					}
				break;
			 case SEE :
				inputwstr(seura,len,x,y,term,&tc,FALSE);
#ifdef SEURAVAL
				if (tc == 202) {
				   hae_seura(seura, seuralyh, &piiri, tc == 202);
				   }
#endif
				elimwbl(seura);
				wmemset(seura+wcslen(seura)+1, 0,
				   LSEURA-wcslen(seura));
				break;         
			 case SELE :
				inputwstr(seuralyh,len,x,y,term,&tc,FALSE);
#ifdef SEURAVAL
				if (tc == 202) {
				   hae_seura(seura, seuralyh, &piiri, tc == 202);
				   }
#endif
				elimwbl(seuralyh);
				wmemset(seuralyh+wcslen(seuralyh)+1, 0,
				   LLYH-wcslen(seuralyh));
				break;         
			 case ALUE :
				if (!kunto)
				   inputintw((short *)&piiri,len,x,y,term,&tc);
				break;
			 case MAAE :
				if (!kunto) {
				   inputwstr(maa,len,x,y,term,&tc,FALSE);
				   elimwbl(maa);
				   wmemset(maa+wcslen(maa)+1, 0,
					  3-wcslen(maa));
				   }
				break;
			 case SRJE :
				if (Sarja() >= 0 && Sarja() < sarjaluku )
						 wcscpy(snimi, Sarjat[Sarja()].sarjanimi);
				else snimi[0] = 0;
				do {
				   inputwstr(snimi,len,x,y,term,&tc,FALSE);
				   srj = haesarja_w(snimi, false);
				   if(srj < 0) {
					  writeerror_w(L"Virheellinen sarja",0);
					  setSarja(-1);
					  }
				   else setSarja(srj);
				   } while ((srj < 0) && (tc != ESC));
				outkilp(0);
				break;
			 case ERE :
				if (i_pv == k_pv && wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
				   inputintw(&pv[k_pv].era,len,x,y,term,&tc);
				   }
				break;
			 case RATAE :
				if (i_pv == k_pv && (monirata || wcswcind(kilpparam.kilplaji, L"NPU") >= 0)) {
				   inputwstr(pv[k_pv].rata,sizeof(pv[k_pv].rata)/2-1,x,y,term,&tc,FALSE);
				   }
				break;
#ifndef AMPUSUUNN
			 case SAKE :
				if (i_pv == k_pv) {
				   if (Sarjat[Sarja()].lsak[k_pv]) {
					  len = editfld[SAKE-1].len;
					  for (n = 0; n < Sarjat[Sarja()].lsak[k_pv] - 1; n++)
						 if (pv[k_pv].asakot[n] < L'0') break;
					  do {
						  wchar_t sl[12];
						  INT32 tma[VALUKU+1];
						  if (pv[k_pv].asakot[n] > L'0')
							  _itow(pv[k_pv].asakot[n] - L'0', sl, 10);
						  else {
							   sl[0] = pv[k_pv].asakot[n];
							   sl[1] = 0;
							   }
						 inputwstr(sl,len,x+(len+1)*n,y,term,&tc, TRUE);
						 if (tc == ESC) break;
						 if ((sl[1] == 0  && pv[k_pv].asakot[n] != sl[0]) || (len > 1 && (_wtoi(sl) != pv[k_pv].asakot[n] - L'0'))) {
							for (va = 0; va <= Sarjat[Sarja()].valuku[k_pv]; va++) {
								if (kilpparam.kilplaji == L'P' && (wcswcind(pvparam[k_pv].alalaji, L"TH") >= 0))
									tma[va] = p_aika(-1);
								else 
									tma[va] = maali(va);
							   }
							if (sl[1] == 0)
								pv[k_pv].asakot[n] = sl[0];
							else
								pv[k_pv].asakot[n] = _wtoi(sl) + L'0';
							for (va = 0; va <= Sarjat[Sarja()].valuku[k_pv]; va++)
							   set_tulos(va, tma[va]);
							}
						 outkilp(0);
						 if (tc == KB_TAB || tc == KB_CR || tc == DOWN) n++;
						 else if (tc == UP) n--;
						 else break;
						 } while (n >= 0 && n < Sarjat[Sarja()].lsak[k_pv]);
					  }
				   }
				break;
			 case TAE :
				 if (Sarjat[Sarja()].lsak[i_pv]) {
					if (i_pv != k_pv) {
					   inputwstr(pv[i_pv].asakot,len,x,y+i_pv*RV,termp,&tc, TRUE);
						}
					break;
				   }
				if( Sarjat[Sarja()].rrtas > 1 && Sarjat[Sarja()].tsak[k_pv] > 0) {
				   nt = pv[k_pv].tasoitus/Sarjat[Sarja()].tsak[k_pv];
				   INPUTINTW(&nt,len,x,y,term,&tc);
				   if((tm[0] = tulos_pv(k_pv)) != 0)
					  tall_tulos_pv(tm[0] - SEK*(pv[k_pv].tasoitus-Sarjat[Sarja()].tsak[k_pv]*nt), k_pv, 0);
				   pv[k_pv].tasoitus = Sarjat[Sarja()].tsak[k_pv]*nt;
				   }
				break;
			 case LAHE :
				r = y+i_pv*RV;
#else  // AMPUSUUNN
			 case TAE :
				if(!addfl || kunto) {
				   r = vapiste ? TUY - 6 + 2*vapiste : TUY;
				   for (nt = 0; nt < 3; nt++) {
					  tm[nt] = maali(kilp, nt);
					  }
				   switch (aslaji[asjarjestys][vapiste]) {
					  case 0:      // Kartanluku
						 inputint(&mmsakko, 2, SKX, r, term, &tc);
						 break;
					  case 1:      // Ammunta
						 inputint(&ohi[0], 2, SKX-1, r, term, &tc);
						 if (tc != ESC && tc != UP && tc != L'+')
							inputint(&ohi[1], 2, SKX+2, r, term, &tc);
						 break;
					  case 2:      // Suunnistus
						 break;
					  }
				   for (nt = 0; nt < 3; nt++) {
					  if (tm[nt] != TMAALI0)
						 set_tulos(kilp, nt, tm[nt]);
					  }
				   outkilp(kilp,0);
				   }
				break;
			 case TUE :
				if(addfl && !kunto) break;
				r = vapiste ? TUY - 6 + 2*vapiste : TUY;
				if (vapiste && vapiste <= Sarjat[Sarja()].valuku[k_pv]) {
				   INPUTAIKA(&pv[k_pv].va[vapiste+1].vatulos,0,laika,TUX,
					  r,termp,&tc);
				   outkilp(kilp, 0);
				   }
				else {
				   if (hyv_pv(kilp, i_pv)) {
					  INPUTAIKA(&pv[i_pv].tulos,0,laika,TUX,r,termp,&tc);
					  outkilp(kilp, 0);
					  }
				   }
				break;
			 case MAE :
				if((!addfl || kunto) && tark(k_pv) != L'P') {
				   r = vapiste ? TUY - 6 + 2*vapiste : TUY;
				   nt0 = 0;
				   tm[0] = maali(kilp, vapiste <= Sarjat[Sarja()].valuku[k_pv] ? vapiste : 0);
				   INPUTAIKA(&tm[0],t0,laika,MAX,r,term,&tc);
				   set_tulos(kilp, vapiste <= Sarjat[Sarja()].valuku[k_pv] ? vapiste : 0, tm[0]);
				   outkilp(kilp,nt0);
				   }
				break;
			 case RYE :
				inputstr(ryhma, 1, RYX, RYY, termp, &tc, 0);
				break;
			 case LAHE :
				if (vapiste != 1)
				   break;
				r = TUY - 4;
#endif            
				if(tark(i_pv) != L'P') {
					if (kilpparam.n_pv_akt > 1)
						viwrrectw(ySize-3,48,ySize-3,72,L"PgUp, PgDn : Vaihda päivä",7,0,0);
					tm[0] = pv[i_pv].tlahto;
					INPUTAIKAW(&pv[i_pv].tlahto,t0,pvparam[i_pv].laika,x,r,termp,&tc);
					if (tm[0] != pv[i_pv].tlahto) {
						if (pvparam[k_pv].hiihtolahto) {
							tm[0] = pv[i_pv].tlahto;
							if (i_pv == k_pv && pv[i_pv].va[0].vatulos != pv[i_pv].tlahto) {
								ch = L' ';
								wselectopt(L"Muutetaanko myös toteutunut lähtöaika (K/E)",
								L"KE", &ch);
								if (ch == L'K') {
									tm[0] = pv[i_pv].va[0].vatulos;
									pv[i_pv].va[0].vatulos = pv[i_pv].tlahto;
									}
								}
							}
						else
							pv[i_pv].va[0].vatulos = pv[i_pv].tlahto;
						if (i_pv == k_pv && tm[0] != pv[i_pv].tlahto) {
							ch = L' ';
							for (va = 0; va <= Sarjat[Sarja()].valuku[k_pv]; va++) {
								if ((tls = p_aika( va)) != 0) {
									if (ch != L'K')
									wselectopt(L"Muutetaanko myös tulokset vastaavasti (K/E)",
										L"KE", &ch);
									if (ch == L'E') break;
									tall_tulos(va, tls + tm[0] - pv[i_pv].tlahto);
									}
								}
							}
						}
					}
				else 
					viwrrectw(ySize-1,1,ySize-1,9,L"POISSA   ",7,0,0);
				break;
			 case LAH1E :
				if (pvparam[k_pv].hiihtolahto) {				 
					if(tark(i_pv) != L'P') {
						tm[0] = pv[i_pv].va[0].vatulos;
						INPUTAIKAW(&pv[i_pv].va[0].vatulos,t0,pvparam[k_pv].laika,x,y,termp,&tc);
						if (tm[0] != pv[i_pv].va[0].vatulos) {
							for (va = 0; va <= Sarjat[Sarja()].valuku[k_pv]; va++) {
								if ((tls = p_aika( va)) != 0) {
									if (ch != L'K')
										wselectopt(L"Muutetaanko myös tulokset vastaavasti (K/E)",
											L"KE", &ch);
									if (ch == L'E') 
										break;
									tall_tulos(va, tls + tm[0] - pv[i_pv].va[0].vatulos);
									}
								}
							}
						}
					else 
						viwrrectw(ySize-1,1,ySize-1,9,L"POISSA   ",7,0,0);
					}
				break;
#ifndef AMPUSUUNN
			 case TLSE :
				if(addfl && !kunto) 
					break;
				if (vapiste && vapiste <= Sarjat[Sarja()].valuku[k_pv]) {
					tls = p_aika(vapiste);
					INPUTAIKAW(&tls,0,pvparam[k_pv].laika,x,varivi(vapiste-1),termp,&tc);
					tall_tulos(vapiste, tls);
					}
				else if(hyv_pv(i_pv)) {
					if (kilpparam.n_pv_akt > 1)
						viwrrectw(ySize-3,48,ySize-3,72,L"PgUp, PgDn : Vaihda päivä",7,0,0);
					tls = tulos_pv(i_pv);
					INPUTAIKAW(&tls,0,pvparam[i_pv].laika,x,y+i_pv*RV,termp,&tc);
					tall_tulos_pv(tls, i_pv);
					}
				outkilp(0);
				break;
#endif         
			 case TRKE :
#ifdef AMPUSUUNN
				if (aslaji[asjarjestys][vapiste] != 2)
				   break;
				r = vapiste ? y - 6 + 2*vapiste : y;
#else
				r = y+i_pv*RV;
#endif            
				if(addfl && !kunto)
				   break;
				if (kilpparam.n_pv_akt > 1)
					viwrrectw(ySize-3,48,ySize-3,72,L"PgUp, PgDn : Vaihda päivä",7,0,0);
				do {
				   kh = tark(i_pv);
					if (Sarjat[Sarja()].lsak[i_pv] == 0) {
					   switch(tark(i_pv)) {
						  case L'K' : wcscpy(tarkstr, L"Kesk"); break;
						  case L'H' : wcscpy(tarkstr, L"Hyl."); break;
						  case L'E' : wcscpy(tarkstr, L"Eil."); break;
						  case L'T' : wcscpy(tarkstr, L"Trk."); break;
						  case L'I' : wcscpy(tarkstr, L"esIt"); break;
						  case L'V' : wcscpy(tarkstr, L"Vak."); break;
						  case L'P' : wcscpy(tarkstr, L"Pois"); break;
						  default  : tarkstr[0] = 0;
						  }
					   inputwstr(tarkstr,4,x,r,termp,&tc,FALSE);
						}
					else {
					   tarkstr[0] = kh;
					   tarkstr[1] = 0;
					   inputwstr(tarkstr,1,x+Sarjat[Sarja()].lsak[k_pv],y+i_pv*RV,termp,
						  &tc,FALSE);
					}
				   tarkstr[0] = towupper(tarkstr[0]);
				   if (tarkstr[0] == L'E' && towupper(tarkstr[1]) == L'S')
					  tarkstr[0] = L'I';
				   if (i_pv == k_pv && (tarkstr[0] == L'E' || tarkstr[0] == L'V' || tarkstr[0] == L'P')
					  && (p_aika(0) !=0 || tark(k_pv) == L'K'
					  || tark(k_pv) == L'H') && tc != ESC) {
					  erbeep();
					  ch = L'E';
					  wselectopt(L"Tulos jo annettu, merkitäänkö (K/E)", L"KE", &ch);
					  if (ch == L'E') {
						 tc = L' ';
						 break;
						 }
					  }
				   if (tarkstr[0] == 0 || tarkstr[0] == L' ')
						set_tark(L'-', i_pv);
				   else 
						set_tark(tarkstr[0], i_pv);
				   n = 0;
				   if ((wcswcind(tark(i_pv), L"KHEVP") >= 0) &&
					  (p_aika(0) != 0) && (wcswcind(kh, L"-TI") >= 0)) {
					  set_sija(0, 0);
					  if (i_pv > 0)
						 pv[k_pv].ysija = 0;
					  if (i_pv == k_pv) n = -1;
					  }
				   outkilp(n);
				   if(wcswcind(tark(i_pv), L"KHETIVP-") < 0) erbeep();
				   } while (wcswcind(tark(i_pv), L"KHETIVP-") < 0);
				break;
			 case NOE :
				do {
					int ok = 1;
				   kno = id();
				   if (kno <= 0) 
					   kno = 0;
				   vidspwmsg(y,x,7,0,L"      ");
				   inputintw(&kno, len, x, y,term,&tc);
				   if((tc != ESC) && (kno != id())) {
					  if (kno < minkilpno || kno > maxkilpno) {
						 swprintf(msg, L"Anna kilp.numero väliltä : %d ... %d",
							minkilpno,maxkilpno);
						 writeerror_w(msg,0);
						 ok = 0;
						 }
					  else if (getpos(kno) > 0) {
						 writeerror_w(L"Numero jo käytössä",0);
						 ok = 0;
						 }
					  }
				   if (ok)
					   break;
				   } while (tc != ESC);
				if (tc != ESC)
					setId(kno);
				break;
#if !defined(AMPUSUUNN) && !defined(EBA)
			 case MAE :
				if((!addfl || kunto) && tark(k_pv) != L'P') {
				   nt0 = 0;
				   tm[0] = maali(vapiste <= Sarjat[Sarja()].valuku[k_pv] ? vapiste : 0);
				   INPUTAIKAW(&tm[0],t0,pvparam[k_pv].laika, x, y,term,&tc);
				   set_tulos(vapiste <= Sarjat[Sarja()].valuku[k_pv] ? vapiste : 0, tm[0]);
				   outkilp(nt0);
				   }
				break;
#endif
			 }
		}
	  outkilp(0);
      if( tark(k_pv) == L' ') 
			set_tark(L'-', k_pv);
      if (tc == PGUP || tc == PGDN || tc == L'+') {
         if((Sarja() < 0 || Sarja() >= sarjaluku)) {
            writeerror_w(L"ANNA SARJA",0);
            l = 4;
            tc = KB_CR;
            }
         }
#ifdef AMPUSUUNN
      r = vapiste;
#endif      
      switch (tc) {
         case KB_TAB:
         case KB_CR:
         case DOWN:
            if(l == nc) l = 1;
            else l++;
            break;
         case UP:
            if(l == 1) l = nc;
            else l--;
            break;
         case PGDN:
			 if (kilpparam.n_pv_akt) {
	            i_pv++;
		        if (i_pv > Sarjat[Sarja()].viimpv) 
					i_pv = Sarjat[Sarja()].enspv;
				}
			 else {
				vapiste++;
				if (vapiste > Sarjat[Sarja()].valuku[k_pv])
				   vapiste = 0;
				kuulpiste = vapiste;
				outkilp(0);
			 }
            break;
         case PGUP:
			 if (kilpparam.n_pv_akt) {
	            i_pv--;
		        if (i_pv < Sarjat[Sarja()].enspv) 
					i_pv = Sarjat[Sarja()].viimpv;
				}
			 else {
				vapiste--;
				if (vapiste < 0)
				   vapiste = Sarjat[Sarja()].valuku[k_pv];
				kuulpiste = vapiste;
				outkilp(0);
			 }
            break;
         case C_PGDN:
            vapiste++;
            if (vapiste > Sarjat[Sarja()].valuku[k_pv])
               vapiste = 0;
			kuulpiste = vapiste;
            outkilp(0);
            break;
         case C_PGUP:
            vapiste--;
            if (vapiste < 0)
               vapiste = Sarjat[Sarja()].valuku[k_pv];
			kuulpiste = vapiste;
            outkilp(0);
            break;
         case L'+':
            break;
         case ESC :
            break;
         default :
            tc = KB_CR;
         }
#ifdef AMPUSUUNN
      if (r != vapiste) {
         if (aslaji[asjarjestys][vapiste] != 2) {
            if (l == TRKE)
               l = TAE;
            }
         else {
            if (l == TAE)
               l = TRKE;
            }
         }
#endif
      } while (tc != L'+' && tc != ESC);
   clrln(ySize-1);
   return(tc != L'+');
   }

