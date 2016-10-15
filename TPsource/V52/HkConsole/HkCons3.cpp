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
#include "HkDeclare.h"
#include "HkMuotoilu.h"

#define EMITXML
#define XML

#define AIKAAMPU

#if defined(SEURATUL) && !defined(AMPUSUUNN)
#undefine SEURATUL
#endif
#define SEURALISTA 15

INT tulkhp(wchar_t *fnt, wchar_t *pap, wchar_t *as, wchar_t *msto, wchar_t *extra,
   INT *mlj, INT *mtasm, INT *mkoko, INT *mrivi);
void vasivu(void);
void seuraluettelo(void);
void luepiirit(wchar_t *flname);
int xmlots(tulostusparamtp *tulprm);
void ssl_otsikot(INT k_pv, tulostusparamtp *tulprm);
void pisteet(tulostusparamtp *tulostusparam);
int srjtullkm(int srj, tulostusparamtp *tulostusparam);
//int srjtullkm(int srj, int piste, int day, int aakkos, wchar_t ct, int np);
INT htmlrunko(tulostusparamtp *tulprm, wchar_t *baseFName);
void htmlloppu(tulostusparamtp *tulprm);
int xmlloppu(tulostusparamtp *tulprm);
void htmlalku(wchar_t *wtitle, wchar_t *wheader, int frame, tulostusparamtp *tulprm);
void meri3pisteet(void);

void yhteenveto(tulostusparamtp *tulostusparam);
int valitsetulpv (tulostusparamtp *tulostusparam, int oikaise);

INT avaa_tiedosto(tulostusparamtp *tulprm);
INT avaa_tiedosto_nm(wchar_t *fname, tulostusparamtp *tulprm);
wchar_t *htmlSrjFName(wchar_t *otsFName, int srj);
//INT avaa_tiedosto_n(wchar_t *fname);
void sulje_tiedosto(TextFl **tul_tied);

extern int vpos;

extern INT mrivi[3];
extern wchar_t palk[MAXSARJALUKU];
extern INT lastpos;

extern int ssltlsfl;
extern INT nens;
extern INT potsfl;
extern INT htmlpitkat;
extern wchar_t tul_tied_nimi[80];
extern int sarjayhdfl;
extern INT *yhdseur[VALUKU+3];
extern int flatxml;
extern int tulera;

typedef struct {
   wchar_t sr[LSEURA+2];
   INT piiri;
   INT pos;
   } srixtype;

extern srixtype *srix;
extern wchar_t *sr_nimet;

extern wchar_t seuranimi[SEURALISTA][LSEURA+1];
extern wchar_t piirinimi[][41];

extern int piiriluku;

extern INT piirifl[200];

#define PM_LUKU 8

#ifdef MCPIST
extern INT mcpist[30]
#endif

typedef struct {
#if defined(AMPUSUUNN)
   INT32 jtls;
   INT d[PM_LUKU];
#endif
	wchar_t *seura;
   } sraluettp;

extern sraluettp  *seurat;

void putflddsp(wchar_t *str, INT pos, INT len, INT oik, INT r)
   {
   INT v, o, ll;

   ll = wcslen(str);
   ll = min(ll, len);
	if (ll <= 0) return;
   if (ll == len) str[ll] = 0;
    {
      if (oik) {
         o = pos + len - 1;
         v = o - ll + 1;
         }
      else {
			v = pos;
         o = v + ll - 1;
         }
      if (r > 0) {
         viwrrectw(r,v,r,o,str,7,0,0);
         }
      }
   }

INT avaa_tiedosto(tulostusparamtp *tulprm)
   {
   wchar_t ch;
   INT i, ii;
   extern scr tiedostoohje;

   clrscr();
   kehys(1);
   prscr(&tiedostoohje);
   viwrrectw(ySize-3, 0, ySize-3, 14, L"Tiedoston nimi:", 7, 0, 0);
   inputwstr(tul_tied_nimi, 39, 16, ySize-3, L"\r\x1b", &ch, 0);
   if (ch == ESC) return(1);
   if (tulprm->kohde == L'S' || tulprm->kohde == L'F')
		tulprm->merkit = L'A';
   else {
		ch = L' ';
		wselectopt(L"U)TF-8 vai I)SO-8858-1 merkistö", L"UI", &ch);
		tulprm->merkit = (ch == L'I' ? L'A' : L'8');
		}

	tulprm->lstf = openprfile(tul_tied_nimi, -1, TRUE, FALSE, (char *)&tulprm->merkit, 0);
   if (!tulprm->lstf) {
      writeerror_w(L"Virhe tiedostoa avattaessa", 0);
      return(1);
      }
   ii = -1;
   for (i = 0; tul_tied_nimi[i]; i++)
      if (tul_tied_nimi[i] == L':' || tul_tied_nimi[i] == L'\\') ii = i;
   return(0);
   }


#ifdef IOF
#ifdef WRKOODI


static int near iofots(void)
   {

   return(0);
   }

static int near iofsrjots(int sarja)
   {
	static wchar_t iofots[32];
	wchar_t iofsrj[12], iofrsrj[12] = L"M21";
   int i;
   wchar_t ch, st[102];
   const wchar_t term[] = L"+\x5\x9\xD\x18\x1B";

   for (i = 12; i < 21; i++) clrtxt(i, 0, 64);
   vidspwmsg(14,0,7,0, L"Kisan nimi");
   vidspwmsg(15,0,7,0, L"WRE rankisarja (M21 tai W21)");
	if (!iofots[0])
		wcsncpy(iofots, kilpailu, 30);
   wcscpy(iofsrj, Sarjat[sarja].sarjanimi);
   if (iofsrj[0] == L'D' || iofsrj[0] == L'N') iofrsrj[0] = L'W';
   vidspwmsg(14,30,7,0,iofots);
   vidspwmsg(15,30,7,0,iofrsrj);
   i = 0;
   do {
      switch (i) {
         case 0 :
            inputwstr(iofots,30,30,14,term,&ch,FALSE);
            elimblw(iofots);
            break;
         case 1 :
            inputwstr(iofrsrj,8,30,15,term,&ch,FALSE);
            elimblw(iofrsrj);
            break;
         }
      switch (ch) {
         case KB_CR:
            if (i == 1) {
               i = 10;
               break;
               }
         case KB_TAB:
         case DOWN:
            if(i == 1) i = 0;
            else i++;
            break;
         case UP:
            if(i == 0) i = 1;
            else i--;
            break;
         case L'+':
         case ESC:
            i = 10;
            break;
         }
      } while (i < 10);
   if (ch == ESC) return(1);
   swprintf(st, L"%s %s", iofots, iofrsrj);
   writewline(st);
   return(0);
   }

#else // !WRKOODI

static int near iofots(tulostusparampt *tulprm)
   {
   static wchar_t iofkilp[42], iofjarj[42], iofpv[11], jarjmaa[] = L"FIN";
   int i,l;
   wchar_t ch, st[202];
   const wchar_t term[] = L"+\x5\x9\xD\x18\x1B";

   status_on = 0;
   vidspwmsg(6,0,7,0, L"Kilpailun nimi");
   vidspwmsg(7,0,7,0, L"Järjestäjä(t)");
	vidspwmsg(8,0,7,0, L"Kilpailupäivä (mm-pp-vvvv)");
   vidspwmsg(9,0,7,0, L"Kilpailumaa");
   vidspwmsg(3,0,7,0, L"Otsikkorivi");
   memset(iofkilp, 0, sizeof(iofkilp));
   memset(iofjarj, 0, sizeof(iofjarj));
   if (iofkilp[0] == 0) wcsncpy(iofkilp, kilpailu, 40);
   if (iofjarj[0] == 0) wcsncpy(iofjarj, kilpailu, 40);
   if (iofpv[0] == 0) {
      wcscpy(st, wpaiva());
      memcpy(iofpv, st+3, 2);
      memcpy(iofpv+3, st, 2);
      memcpy(iofpv+8, st+6, 2);
      iofpv[2] = iofpv[5] = L'-';
      if (st[6] > L'8') {
         iofpv[6] = L'1';
         iofpv[7] = L'9';
         }
      else {
         iofpv[6] = L'2';
         iofpv[7] = L'0';
         }
      }
   i = 0;
   vidspwmsg(6,20,7,0,iofkilp);
   vidspwmsg(7,20,7,0,iofjarj);
   vidspwmsg(8,30,7,0,iofpv);
   vidspwmsg(9,20,7,0,jarjmaa);
   do {
      switch (i) {
         case 0 :
            inputwstr(iofkilp,40,20,6,term,&ch,FALSE);
            elimwbl(iofkilp);
            break;
         case 1 :
            inputwstr(iofjarj,40,20,7,term,&ch,FALSE);
            elimwbl(iofjarj);
            break;
         case 2 :
            inputwstr(iofpv,10,30,8,term,&ch,FALSE);
            break;
         case 3 :
            inputwstr(jarjmaa,3,20,9,term,&ch,FALSE);
            elimwbl(jarjmaa);
            break;
			}
      swprintf(st, L"R,1,\"%s\L",\"%s\L",%s,%s",
         iofkilp, iofjarj, iofpv, jarjmaa);
      iofskand(st, 100);
      l = min(80, wcslen(st));
      clrln(4);
      viwrrectw(4,0,4,l-1,st,7,0,0);
      switch (ch) {
         case KB_CR:
            if (i == 3) {
               i = 10;
               break;
               }
         case KB_TAB:
         case DOWN:
            if(i == 3) i = 0;
            else i++;
            break;
         case UP:
            if(i == 0) i = 3;
            else i--;
            break;
         case L'+':
         case ESC:
            i = 10;
            break;
         }
      } while (i < 10);
   status_on = 1;
   if (ch == ESC) return(1);
   tulprm->writeline(st);
   if (!sr_nimet) {
//      lue_sr_nimet(1);
//      luepiirit();
      }
   return(0);
   }

static int iofsrjots(int sarja, tulostusparampt *tulprm)
   {
   wchar_t iofsrj[12], iofmatka[12], iofrsrj[4] = L"M21";
   int rluku = 0, nousu = 0;
   int i;
   wchar_t *p, ch, st[102];
	const wchar_t term[] = L"+\x5\x9\xD\x18\x1B";

   for (i = 12; i < 21; i++) clrln(i);
   vidspwmsg(12,0,7,0, L"Sarjan nimi");
   vidspwmsg(13,0,7,0, L"Matka (km, esim 11.5)");
   vidspwmsg(14,0,7,0, L"IOF rankisarja (M21 tai F21)");
   vidspwmsg(15,0,7,0, L"Rastiluku");
   vidspwmsg(16,0,7,0, L"Noususumma (m)");
   wcscpy(iofsrj, (wchar_t *)Sarjat[sarja].sarjanimi);
   wcscpy(iofmatka, (wchar_t *)matka[sarja]);
   for (p = iofmatka; *p; p++) {
      if (*p == L',') {
         *p = L'.';
         break;
         }
      }
   if (iofsrj[0] == L'D' || iofsrj[0] == L'N') iofrsrj[0] = L'F';
   vidspwmsg(12,30,7,0,iofsrj);
   vidspwmsg(13,30,7,0,iofmatka);
   vidspwmsg(14,30,7,0,iofrsrj);
   vidint(15,30,2,rluku);
   vidint(16,30,4,nousu);
   i = 0;
   do {
      switch (i) {
         case 0 :
            inputwstr(iofsrj,10,30,12,term,&ch,FALSE);
            elimwbl(iofsrj);
            break;
         case 1 :
            inputwstr(iofmatka,10,30,13,term,&ch,FALSE);
            elimwbl(iofmatka);
            break;
         case 2 :
            inputwstr(iofrsrj,3,30,14,term,&ch,FALSE);
            elimwbl(iofrsrj);
            break;
         case 3 :
            INPUTINTW(&rluku,2,30,15,term,&ch);
            break;
         case 4 :
            INPUTINTW(&nousu,4,30,16,term,&ch);
            break;
         }
		swprintf(st, L"C,\"%s\L",%s,\"%s\L",%d,%d",
         iofsrj, iofmatka, iofrsrj, rluku, nousu);
      iofskand(st, 100);
      l = min(80, wcslen(st));
      clrln(18);
      viwrrectw(18,0,18,l-1,st,7,0,0);
      switch (ch) {
         case KB_CR:
            if (i == 4) {
               i = 10;
               break;
               }
         case KB_TAB:
         case DOWN:
            if(i == 4) i = 0;
            else i++;
            break;
         case UP:
            if(i == 0) i = 4;
            else i--;
            break;
         case L'+':
         case ESC:
            i = 10;
            break;
         }
      } while (i < 10);
   if (ch == ESC) return(1);
   tulprm->writeline(st);
   return(0);
   }

#endif // !WRkoodi
#endif // IOF

#if !defined(AMPUSUUNN) && defined EI_OLE
static void near lehdisto(void)
   {
	wchar_t *ptr, *buf, ch, as[16], tc, st[60];
	INT np,l = 0,srj,p,i,k,lgt,sj;
	INT ontls = 0;
	kilptietue kilp;
	extern wchar_t *rivptr[];
	static wchar_t cr = L'2';
	static INT vaihda = TRUE;
	static INT pitkat = FALSE;
	INT32 tulos0;
	INT piste, day;

	header=L"LEHDISTÖTULOSTEN EDITOINTI";
	if ((buf = (wchar_t *) malloc(CBUFL)) == 0)
		return;
	ptr = buf;
	ch = vaihda ? L'K' : L'E';
	wselectopt(L"Vaihdetaanko etu- ja sukunimien järjestys (K/E)",L"KE",&ch);
	vaihda = (ch == L'K');
	ch = pitkat ? L'K' : L'E';
	wselectopt(L"Kirjoitetaanko L)yhenne vai S)euranimikenttä",L"LS",&ch);
   pitkat = (ch == L'S');
   if ( wcslen(paaots) > 0) {
      ptr = wcpcpy(ptr, paaots);
      *(ptr++) = L'|';
      *(ptr++) = L'|';
      }
   else if ( wcslen(kilpailu) > 0) {
      ptr = wcpcpy(ptr, kilpailu);
      *(ptr++) = L'|';
      *(ptr++) = L'|';
      }
   srj = 0;
   np = 5;
   day = 1;
   piste = 0;
   EnterCriticalSection(&print_CriticalSection);
   do {
      do {
			vidspwmsg(ySize-3,0,7,0,L"       Anna sarja :             "
                                   L"  <Esc> : Poistu päävalintaan");
         srj = luesarja(L"", &tc);
         if (tc == ESC) goto loppu;
         clrln(ySize-3);
         if (k_pv) {
            tc = day ? L'2' : L'Y';
            wselectopt(L"2) päivä vai Y)hteistulokset", L"2Y", &tc);
			clrln(ySize-3);
			if (tc == L'Y') {
               day = 0;
               piste = kilpparam.valuku+1;
               }
            }
         vidspwmsg(ySize-3,0,7,0,L"Montako parasta :");
         INPUTINTW(&np,3,18,ySize-3,L"\xD",&tc);
         clrln(ySize-3);
         if (!day) np = min(np, ntulos[srj][kilpparam.valuku+1]);
			else
            np = min(np, ntulos[srj][0]);
         if( np <= 0 ) writeerror_w(L"Ei tulostettavia, tyhjä sarja ?",0);
         } while (np <= 0);
      vidspwmsg(ySize-3,0,7,0,L" + : Hyväksy,   <Esc> : Peruuta sarja,   "
         L"<Ret.> : Uusi rivi (ruudulla |)");
      clrln(ySize-2);
	  vidspwmsg(ySize-2,0,7,0,L"\x1A  \x1B      Ctrl-\x1A  Ctrl-\x1B  Home  "
         L"End : liiku,   Ctrl-End : poista loput");
      ptr = wcpcpy(ptr, Sarjat[srj].sarjanimi);
      if (day) {
         *(ptr++) = L' ';
         if (Sarjat[srj].matka[k_pv][0]) {
            *(ptr++) = L'(';
            ptr = wcpcpy(ptr, Sarjat[srj].matka[k_pv]);
            if (Sarjat[srj].matka[k_pv][wcslen(Sarjat[srj].matka[k_pv])-1] <= L'9')
               ptr = wcpcpy(ptr, L" km");
            ptr = wcpcpy(ptr, L"): ");
            }
         }
      if (k_pv) {
         *(ptr++) = L':';
         *(ptr++) = L' ';
         }
      p = jalku[srj][day ? 0 : kilpparam.valuku+5];
      tulos0 = 0;
      for (i = 0; i < np && ptr - buf < CBUFL - 100; i++) {
         kilp.GETREC(p);
         p = JarjSeur(0, piste, p);
         if (kilp.p_aika(piste) != tulos0) sj = i + 1;
         else sj = 0;
         tulos0 = kilp.p_aika(piste);
         if (sj) {
            itoa(sj,ptr,10);
            ptr++;
            if (sj > 9) ptr++;
            *(ptr++) = L')';
            *(ptr++) = L' ';
            }
			elimwbl(kilp.nimi(st, 58, vaihda));
			locasewname(st);
			ptr = wcpcpy(ptr, st);
			*(ptr++) = L',';
         *(ptr++) = L' ';
		 ptr = wcpcpy(ptr, kilp.seuratieto(0), !pitkat);
         *(ptr++) = L',';
         *(ptr++) = L' ';
         AIKATOWSTRS(as, tulos0,0);
         as[laika2] = 0;
         elimwz(as);
         elimwbl(as);
         ptr = wcpcpy(ptr, as);
         if (Sarjat[kilp.Sarja()].lsak[k_pv])
            *(ptr++) = L' ';
            *(ptr++) = L'(';
			wcpcpy(ptr, kilp.pv[k_pv].asakot);
            *(ptr++) = L')';
         if (i < np-1) {
            *(ptr++) = L',';
            *(ptr++) = L' ';
            }
         else *(ptr++) = L'.';
         *(ptr++) = L' ';
         }
      *ptr = 0;
      pageedit(buf,CBUFL,2,10,20,70,L"+\x1B", &ch);
      lgt = wcslen(buf);
      if (ch == L'+') {
         for (k = 0; (rivptr[k] - buf) < lgt; k++) ;
         l += k;
         if (!ontls) {
            initpage(tulostusparam, NULL);
            clrln(ySize-2);
            wselectopt(L"Tulostuksen riviväli 1) vai 2)", L"12", &cr);
            clrln(ySize-3);
            }
         else if (l * (cr-L'0') >= sivpit) {
            endpage();
            initpage(tulostusparam, NULL);
            l = 0;
            }
         else {
            newline(cr-L'0');
            l += cr-L'0';
            }
         ontls = 1;
         for (k = 0; (rivptr[k] - buf) < lgt; k++) {
            initline();
			rivptr[k+1][-1] = 0;
			putfld(rivptr[k],prtflds[1].pos, 80, 0, 0);
			endline(cr-L'1');
			}
		 }
	  ptr = buf;
	  } while (srj >= 0);
   loppu:
   if (l > 0 && ontls) endpage();
   if (!pitkaseura) {
	  if (srix) free(srix);
	  srix = NULL;
	  if (sr_nimet) free(sr_nimet);
	  sr_nimet = NULL;
	  }
   enddoc();
   LeaveCriticalSection(&print_CriticalSection);
   free(buf);
   }
#endif

static wchar_t tulostettavat(tulostusparamtp *tulprm)
{
   wchar_t ct = L' ';
   wchar_t tc;

   ct = tulprm->rajaus;
   if (tulprm->seuratulokset || tulprm->piiritulokset)
	   ct = L'V';
#ifndef LANGSV_K
   if (tulprm->tulostettava == L'V') {
      ct = tulprm->rajaus == L'P' ? L'P' : L'T';
      wselectopt(L"T)ulos, P)arhaat", L"TP", &ct);
      }
   else {
#ifdef SEURATUL
#ifndef MPA
      wselectopt(L"I)lmoitt, L)opett, T)ulos, P)arhaat, H)yl, K)esk, E)i läht,"
			L" A)voimet"
			L" V)alitut",L"ILTPHKEAV-\x1b",&ct);
		if (ct == ESC)
			return(ESC);
		if (ct == L'V') {
			wchar _t ch = L' ';
			wselectopt(L"S)euroittaiset vai P)iireittäiset tulokset, Esc: peruuta", L"SP\x1b", &ch);
			if (ch == ESC)
				return(ESC);
			if (ch == L'S')
				tulprm->seuratulokset = true;
			if (ch == L'P')
				tulprm->piiritulokset = true;
			ct = L'L';
			tulprm->tulostettava = L'T';
			}
#else // MPA
      wselectopt(L"I)lmoitt, L)opett, T)ulos, P)arhaat, H)yl, K)esk, E)i läht,"
         L" A)voimet"
			L", V)alitut",L"ILTPHKEAV",&ct);
		if (ct == L'V') {
			ct = L' ';
			wselectopt(L"J)oukko-osastot vai Y)hdistykset", L"JY", &ct);
			}
#endif // MPA
#else  // !SEURATUL
      wselectopt(L"I)lmoitt, L)opett, T)ulos, P)arhaat, H)yl, K)esk, E)i läht,"
         L" A)voimet"
			,L"ILTPHKEA-",&ct);
#endif // !SEURATUL
		}
#else // LANGSV_K
	wtransl(L"ILTPHKEAV", L"AKRBDVEÖL", &ct);
	if (cy == L'V') {
		if (ct != L'B') ct = L'R';
		wselectopt(L"R)esultat, B)ästa", L"RB", &ct);
		wtransl(L"RB", L"TP", &ct);
		}
	else {
      wselectopt(L"A)nmälda, K)lara, R)esultat, B)ästa, D)isk, aV)br, E)j start,"
         L" Ö)ppna"
         L" vaL)da",L"AKRBDVEÖL-",&ct);
      wtransl(L"AKRBDVEÖL", L"ILTPHKEAV", &ct);
      if (ct == L'V') {
         wchar_t ch = L' ';
         wselectopt(L"F)örening- eller D)istriktresultater", L"FD", &ch);
         wtransl(L"FD", L"SP", &ch);
         ct = (ch == L'P') ? L'D' : L'S';
         }
      }
#endif // LANGSV_K
   if (ct == L'P') {
      clrln(ySize-3);
#ifndef LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Alkaen sijasta :");
      INPUTINTW(&tulprm->enssija,4,17,ySize-3,L"\xD\x1b",&tc);
	  if (tc == ESC)
		  return(ESC);
      vidspwmsg(ySize-3,24,7,0,L"Päättyen sijaan :");
#else // LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Från placering :");
      INPUTINTW(&tulprm->enssija,4,17,ySize-3,L"\xD\x1b",&tc);
	  if (tc == ESC)
		  return(ESC);
      vidspwmsg(ySize-3,24,7,0,L"Till placering  :");
#endif // LANGSV_K
      INPUTINTW(&tulprm->viimsija,4,42,ySize-3,L"\xD\x1b",&tc);
	  if (tc == ESC)
		  return(ESC);
      if (tulprm->enssija < 1)
         tulprm->enssija = 1;
      if (tulprm->viimsija < tulprm->enssija) erbeep();
      }
   if (ct == L'-') {
      ct = L'T';
      tulprm->tarkastamattomat = true;
      }
   if (ct == ESC)
	   return(ESC);
   tulprm->rajaus = ct;
   return(0);
   }

#ifdef AUTOFILE
   
static void near afasetukset(void)
   {
   wchar_t ch;
   int vali;

   if (!autofileparam.afvali) autofileparam.afvali = 60 * SEK;
   wselectopt(L"Tiedostomuoto: H)TML, X)ML", L"HX", &aftulparam.kohde);
   if (aftulparam.kohde == L'H')
      startaf(0);
   clrln(ySize-3);
   clrln(ySize-1);
   if (aftulparam.kohde != L'H' || aftulparam.yksihtml) {
#ifndef LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Tiedoston nimi : ");
#else // LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Tiedoston nimi : ");
#endif // LANGSV_K
	  inputwstr(autofileparam.afname, 38, 17, ySize - 3, L"\r\x1b", &ch, 0);
      if (ch == ESC) return;
      clrln(ySize-3);
      }
   if (aftulparam.kohde == L'H' && aftulparam.yksihtml) {
#ifndef LANGSV_K
      viwrrectw(ySize-1,0,ySize-1,12,L"SIVUN NIMI   ", 7, 0, 0);
#else // LANGSV_K
      viwrrectw(ySize-1,0,ySize-1,12,L"SIVUN NIMI   ", 7, 0, 0);
#endif // LANGSV_K
	  inputwstr(autofileparam.wtitlea, 76, 0, ySize - 3, L"\r\x1b", &ch, 0);
#ifndef LANGSV_K
      viwrrectw(ySize-1,0,ySize-1,13,L"SIVUN OTSIKKO ", 7, 0, 0);
#else // LANGSV_K
      viwrrectw(ySize-1,0,ySize-1,13,L"SIVUN OTSIKKO ", 7, 0, 0);
#endif // LANGSV_K
      clrln(ySize-3);
	  inputwstr(autofileparam.wheadera, 76, 0, ySize - 3, L"\r\x1b", &ch, 0);
      clrln(ySize-3);
      clrln(ySize-1);
      }
   if (aftulparam.kohde == L'X') {
      wselectopt(L"Kirjoitetaanko E)mitväliajat, M)uut väliajat, I)lman väliaikoja", L"EMI", &autofileparam.af_xmlva);
      }
   if (aftulparam.kohde == L'H' && k_pv > 0) {
	  ch = aftulparam.vaihelkm == 1 ? L'V' : L'Y';
      wselectopt(L"V)ain tämä vaihe, myös Y)hteistulokset", L"VY", &ch);
	  aftulparam.vaihelkm = ch == L'V' ? 1 : 2;
      }
#ifndef LANGSV_K
   vidspwmsg(ySize-3,0,7,0,L"Tulostusväli       sek");
#else // LANGSV_K
   vidspwmsg(ySize-3,0,7,0,L"Tulostusväli       sek");
#endif // LANGSV_K
   vali = autofileparam.afvali / SEK;
   INPUTINTW(&vali, 4, 13, ySize-3, L"\r", &ch);
   autofileparam.afvali = SEK * vali;
   }
#endif  // AUTOFILE

#ifdef TAULU_2
static int afsrj, afva, afnparas = 30;

static void near afasetukset(void)
   {
   wchar_t ch, snimi[20] = L"";
   int vali;

   if (!afvali) afvali = 60*SEK;
   clrln(ySize-3);
   clrln(ySize-1);
   vidspwmsg(ySize-3,0,7,0,L"Tulostusväli      sek");
   vali = afvali/SEK;
   inputint(&vali, 4, 13, ySize-3, L"\r", &ch);
   afvali = SEK * vali;
   if (afsrj >= 0) {
      wcscpy(snimi, Sarjat[afsrj].sarjanimi);
      }
   vidspwmsg(ySize-3,0,7,0,L"Sarjan nimi            ");
   afsrj = luesarja(snimi, &ch);
   if (afsrj < 0) {
      afvali = 0;
      return;
      }
   vidspwmsg(ySize-3,0,7,0,L"Väliaika     (0 : osuuden loppu)");
   inputint(&afva, 1, 9, ySize-3, L"\r", &ch);
   vidspwmsg(ySize-3,0,7,0,L"Montako parasta                           ");
   inputint(&afnparas, 4, 16, ySize-3, L"\r", &ch);
   }
#endif

static void autotulasetukset(void)
   {
   wchar_t co, maalistr[10], *p;
   extern INT intv[];
   INT sek, i;

#ifdef AUTOFILE
   wchar_t ci;

#ifndef LANGSV_K
   ci = L'K';
   wselectopt(L"K)irjoitintulostus, T)iedostotulostus", L"KT", &ci);
#else
   ci = L'P';
   wselectopt(L"P)apperutskrift, F)ilutskrift", L"PF", &ci);
   wtransl(L"PF", L"KT", &ci);
#endif
#endif
#ifdef TAULU_2
   wchar_t ci;
   
   ci = L'V';
   wselectopt(L"K)irjoitintulostus, V)alotaululle", L"KV", &ci);
#endif
   co = L'K';
#ifndef LANGSV_K
   wselectopt(L"K)äynnistä, L)opeta automaattinen tulostus",L"KL",&co);
#else
   wselectopt(L"S)tarta, sL)uta automatisk utskrift",L"SL",&co);
   wtransl(L"S", L"K", &co);
#endif
#if defined(AUTOFILE) || defined(TAULU_2)
   if (ci != L'K') {
      if (co == L'K') afasetukset();
	  else autofileparam.afvali = 0;
      return;
      }
#endif
   if( co == L'L' ) {
      autofl = FALSE;
      }
   else
      {
	  if (!autotulostusparam.lstf)
		  autotulostusparam = lsttulparam;
	  if (!autotulostusparam.lstf) {
		  writeerror_w(L"Kirjoitinta ei määritelty", 0);
		  return;
		  }
	  autotulostusparam.kohde = L'P';
	  if (valitsetulpv(&autotulostusparam, 0))
		  return;
	  if (tulostettavat(&autotulostusparam) == ESC)
		  return;
      vidspwmsg(ySize-5,4,7,0,L"Anna tulostuskynnys :");
      INPUTINTW(&kynnys,5,27,ySize-5,L"\xD",&co);
      kynnys = max(kynnys,1);
      vidspwmsg(ySize-5,4,7,0,L"Anna aika-askel (sek) :       ");
      sek = intv[0] / 18;
      INPUTINTW(&sek,4,29,ySize-5,L"\xD",&co);
      intv[0] = 18*sek;
      p = maalistr;
      for (i = 0; i < 9; i++) {
         if (autotulostusparam.automaali[i]) *(p++) = i + L'1';
         autotulostusparam.automaali[i] = 0;
         }
      *p = 0;
      vidspwmsg(ySize-5,0,7,0,L"Anna tulostettavat maalinumerot yhtenä numerosarjana :");
      inputwstr(maalistr, 9, 55, ySize-5,L"\r", &co, TRUE);
      for (p = maalistr; *p; p++)
         if (*p > L'0' && *p <= L'9') autotulostusparam.automaali[*p - L'1'] = 1;
      autofl = TRUE;
      }
   autoruutu();
   }

static void editkoodi(wchar_t *s, wchar_t *nimi)
   {
   wchar_t ch,*p,ss[58],k;
   INT i,len,nml,el;

   clrln(ySize-3);
   nml = wcslen(nimi);
   viwrrectw(ySize-3,0,ySize-3,nml-1,nimi,7,0,0);
   vidspwmsg(ySize-1,0,7,0,L"Ascii-arvot huutomerkkien väliin "
      L"(Esim. ESC: !27!), Ei välilyöntejä");
   len = *s;
   p = ss;
   for (i = 1; i <= len; i++) {
      if (s[i] >= 34 && s[i] <= 125) *(p++) = s[i];
      else {
         *(p++) = L'!';
         *(p++) = s[i]/100 + L'0';
         *(p++) = (s[i]/10) % 10 + L'0';
         *(p++) = s[i]%10 + L'0';
         *(p++) = L'!';
         }
      }
   *p = 0;
   el = min (57, 76 - nml);
   inputwstr(ss, el, nml+1, ySize-3, L"\r\x1b", &ch, FALSE);
   if (ch == KB_CR) {
      for (i = 1, p = ss; i < PRKOODIPIT && *p > 32; i++, p++) {
         if (*p == L'!') {
            k = 0;
            p++;
            while (*p >= L'0' && *p <= L'9') {
               k = 10*k + *p - L'0';
               p++;
               }
            s[i] = k;
            }
         else
            s[i] = *p;
         }
      s[0] = i-1;
      }
   clrln(ySize-3);
   clrln(ySize-1);
   }

#ifdef _CONSOLE
static void near hpkoodit(void)
   {
   wchar_t ch, st[80], pap[3] = L"A4", as[2] = L"P", vaas[2] = L"V",
      msto[2] = L"P", extra[30]=L"";
   wchar_t term[] = L"+\r\x1b\x5\x9\xb\x18";
   INT i,j,k,mlj[4] = {4101,4101,4101,4099},mtasm[4] = {0,0,0,0};
   INT mkoko[4] = {12,12,12,8};

   if (merkit == L'R') msto[0] = L'R';
   if (init_fnt[0] && !tulkhp(init_fnt, pap, as, msto, extra, &mlj[0],
      &mtasm[0], &mkoko[0], &mrivi[0])) {
      for (i = 1; i < 3; i++) {
         mlj[i] = mlj[0];
         mkoko[i] = mkoko[0];
         mtasm[i] = mtasm[0];
         mrivi[i] = mrivi[0];
         }
      }
   if (pots_on[0])
      tulkhp(pots_on, NULL, NULL, NULL, NULL, &mlj[1],
         &mtasm[1], &mkoko[1], &mrivi[1]);
   if (aots_on[0])
      tulkhp(aots_on, NULL, NULL, NULL, NULL, &mlj[2],
         &mtasm[2], &mkoko[2], &mrivi[2]);
   if (initfont_emva[0])
      tulkhp(initfont_emva, NULL, vaas, NULL, NULL, &mlj[3],
         &mtasm[3], &mkoko[3], &mrivi[3]);
   vidspwmsg(4,0,7,0,L"Mahdollinen lisäinitialisointi (ESC = \x1b) :");
   vidspwmsg(6,10,7,0,L"Merkkilaji  täsmennys  koko (pistettä)   riviväli (1/48 tuumaa)");
   viwrrectw(7,0,9,9,L"perus     "
                     L"pääotsikko"
                     L"alaotsikot" , 7, 0, 0);
   vidspwmsg(10,0,7,0,L"emitväliajat");
   vidspwmsg(10,62,7,0,L"Asento");
   vidspwmsg(10,70,7,0,vaas);
   vidspwmsg(12,0,7,0, L"Merkkilajeja. 4099: Courier, 4101: Times, 4113: Omega, "
      L"4148: Univers");
   vidspwmsg(13,0,7,0, L"Merkkilajitäsmennys: 1: lihav., 2: italic, "
      L"3: lihav. italic, 8: kapea");
   vidspwmsg(15,0,7,0,
      L"Siirry:  Tab Shift-Tab   Hyväksy muutokset : +  Peruuta : Esc"); 
   i = 0;
   j = 0;
   do {
      swprintf(st, L"Paperikoko %2.2s   asento (P/V) %1.1s  merkistö "
         L"(PC-8/Roman-8) %1.1s", pap, as, msto);
      vidspwmsg(3,0,7,0, st);
      clrtxt(4, 43, 63);
      if (extra[0]) vidspwmsg(4,43,7,0,extra+1);
      for (k = 0; k < 4; k++) {
         swprintf(st, L"%4d %9d %13d %15d", mlj[k], mtasm[k], mkoko[k], mrivi[k]);
         vidspwmsg(7+k,14,7,0, st);
         }
      switch (i) {
         case 0:
            inputwstr(pap, 2, 11, 3, term, &ch, 0);
            if (pap[1] == L'3') wcscpy(pap, L"A3");
            else wcscpy(pap, L"A4");
            if (ch == BACKTAB) ch = UP;
            if (ch == KB_TAB) ch = DOWN;
            break;
         case 1:
            inputwstr(as, 1, 29, 3, term, &ch, 0);
            as[0] = towupper(as[0]);
            if (as[0] != L'V') as[0] = L'P';
            if (ch == BACKTAB) ch = UP;
            if (ch == KB_TAB) ch = DOWN;
            break;
         case 2:
            inputwstr(msto, 1, 56, 3, term, &ch, 0);
            msto[0] = towupper(msto[0]);
            if (msto[0] != L'R') msto[0] = L'P';
            if (ch == BACKTAB) ch = UP;
            if (ch == KB_TAB) ch = DOWN;
            break;
         case 3:
            clrln(ySize-3);
            editkoodi(extra, L"Lisäinitialisointi");
            break;
         case 4:
         case 5:
         case 6:
         case 7:
            switch (j) {
               case 0:
                  if (i < 7)
                     INPUTINTW(mlj+i-4, 4, 14, i+3, term, &ch);
                  break;
               case 1:
                  INPUTINTW(mtasm+i-4, 1, 27, i+3, term, &ch);
                  break;
               case 2:
                  INPUTINTW(mkoko+i-4, 2, 40, i+3, term, &ch);
                  break;
               case 3:
                  INPUTINTW(mrivi+i-4, 2, 56, i+3, term, &ch);
                  if (ch == L'\r' && i != 7) {
                     j = 0;
                     ch = DOWN;
                     }
                  break;
               case 4:
                  if (i == 7) {
                     inputwstr(vaas, 1, 70, i+3, term, &ch, 0);
                     vaas[0] = towupper(vaas[0]);
                     if (ch == L'\r') {
                        ch = DOWN;
                        }
                     }
                  break;
               }
            break;
         }
      switch (ch) {
         case UP:
            i--;
            if (i < 4) j = 0;
            break;
         case DOWN:
            i++;
            break;
         case KB_CR:
         case KB_TAB:
            if (i < 4) i++;
            j = (j+1) % (5);
            break;
         case BACKTAB:
            if (i < 4) i--;
            j = (j+4) % 5;
            break;
         }
         if (i != 7 && j == 4) j = 3;
         if (i == 7 && j == 0) j = 1;
         if (i < 0) i = 7;
         if (i > 7) {
            i = 0;
            j = 0;
            }
      } while (ch != ESC && ch != L'+');
   if (ch == L'+') {
//      suhtfont = mlj[0] == 4099 ? 0 : 1;
      swprintf(st,
         L"%s\x1b&l%sA\x1b&l%cO\x1b(%sU\x1b(s%cp%d%c%1.1ds%1.1db%dT\x1b&l%1.1dC",
         extra, pap[1] == L'3' ? L"27" : L"26", as[0] == L'P' ? L'0' : L'1',
         msto[0] == L'P' ? L"10" : L"8", mlj[0] == 4099 ? L'0' : L'1',
         mlj[0] == 4099 ? 120/mkoko[0] : mkoko[0], mlj[0] == 4099 ? L'h' : L'v',
         mtasm[0] / 2, 3*(mtasm[0] & 1), mlj[0], mrivi[0]);
      wcsncpy(init_fnt+1, st, PRKOODIPIT-1);
      init_fnt[PRKOODIPIT] = 0;
      init_fnt[0] = (wchar_t) wcslen(init_fnt+1);
      swprintf(pots_on+1,
         L"\x1b(s%cp%d%c%1.1ds%1.1db%dT\x1b&l%dC",
         mlj[1] == 4099 ? L'0' : L'1',
         mlj[1] == 4099 ? 120/mkoko[1] : mkoko[1], mlj[1] == 4099 ? L'h' : L'v',
         mtasm[1] / 2, 3*(mtasm[1] & 1), mlj[1], mrivi[1]);
      pots_on[0] = (wchar_t) wcslen(pots_on+1);
      swprintf(pots_pois+1,
         L"\x1b(s%cp%d%c%1.1ds%1.1db%dT\x1b&l%dC",
         mlj[0] == 4099 ? L'0' : L'1',
         mlj[0] == 4099 ? 120/mkoko[0] : mkoko[0], mlj[0] == 4099 ? L'h' : L'v',
         mtasm[0] / 2, 3*(mtasm[0] & 1), mlj[0], mrivi[0]);
      pots_pois[0] = (wchar_t) wcslen(pots_pois+1);
      swprintf(aots_on+1,
         L"\x1b(s%cp%d%c%1.1ds%1.1db%dT\x1b&l%dC",
         mlj[2] == 4099 ? L'0' : L'1',
         mlj[2] == 4099 ? 120/mkoko[2] : mkoko[2], mlj[2] == 4099 ? L'h' : L'v',
         mtasm[2] / 2, 3*(mtasm[2] & 1), mlj[2], mrivi[2]);
      aots_on[0] = (wchar_t) wcslen(aots_on+1);
      swprintf(aots_pois+1,
         L"\x1b(s%cp%d%c%1.1ds%1.1db%dT\x1b&l%dC",
         mlj[0] == 4099 ? L'0' : L'1',
         mlj[0] == 4099 ? 120/mkoko[0] : mkoko[0], mlj[0] == 4099 ? L'h' : L'v',
         mtasm[0] / 2, 3*(mtasm[0] & 1), mlj[0], mrivi[0]);
      aots_pois[0] = (wchar_t) wcslen(aots_pois+1);
      swprintf(initfont_emva+1,
         L"\x1b&l%sA\x1b&l%cO\x1b(%sU\x1b(s0p%dh%1.1ds%1.1db4099T\x1b&l%1.1dC",
         pap[1] == L'3' ? L"27" : L"26", vaas[0] == L'P' ? L'0' : L'1',
         msto[0] == L'P' ? L"10" : L"8", 120/mkoko[3], mtasm[3] / 2,
         3*(mtasm[3] & 1), mrivi[3]);
      initfont_emva[0] = (wchar_t) wcslen(initfont_emva+1);
      }
   }
#endif 

static void GDIfontit(void)
   {
   wchar_t ch, st[80];
   INT i,j,k,nr = 3,mtasm;
   wchar_t term[] = L"+\r\x1b\x5\x9\xb\x18";
	GDIfontTp fnt[4];


   clrscr();
   kehys(0);
	fnt[0] = DefFnt;
	fnt[1] = TitleFnt;
	fnt[2] = HeaderFnt;
	fnt[3] = CourFnt;
   vidspwmsg(6,10,7,0,L"Merkkilaji  täsmennys  koko (pistettä)   riviväli (1/72 tuumaa)");
   viwrrectw(7,0,9,9, L"perus     "
                     L"pääotsikko"
                     L"alaotsikot" , 7, 0, 0);
#ifndef MAKI
   vidspwmsg(10,0,7,0,L"emitväliajat");
	nr = 4;
#else
   vidspwmsg(10,0,7,0,L"tuomarit");
	nr = 4;
#endif
   vidspwmsg(12,0,7,0, L"Merkkilajt 0:TimesNewRoman, 1:Arial, 2:ArialNarrow, 3:Calibri, 4:CourierNew");
   vidspwmsg(13,0,7,0, L"Merkkilajitäsmennys: 1: lihav., 2: italic, "
      L"3: lihav. italic");
   vidspwmsg(15,0,7,0,
      L"Siirry: .. Tab Shift-Tab   Hyväksy muutokset : +  Peruuta : Esc"); 
   i = 0;
   j = 0;
   do {
      clrtxt(4, 43, 63);
      for (k = 0; k < nr; k++) {
			if (fnt[k].fontfamily >= 4)
				fnt[k].size = 120/fnt[k].pitch;
         swprintf(st, L"%4d %9d %13d %15d", fnt[k].fontfamily,
				fnt[k].bold+fnt[k].italic, fnt[k].size, fnt[k].line);
         vidspwmsg(7+k,14,7,0, st);
         }
      switch (j) {
         case 0:
            INPUTINTW(&fnt[i].fontfamily, 1, 17, i+7, term, &ch);
            break;
         case 1:
				mtasm = fnt[i].bold+fnt[i].italic;
            INPUTINTW(&mtasm, 1, 27, i+7, term, &ch);
				fnt[i].bold = mtasm & 1;
				fnt[i].italic = mtasm & 2;
            break;
         case 2:
            INPUTINTW(&fnt[i].size, 2, 40, i+7, term, &ch);
            break;
         case 3:
            INPUTINTW(&fnt[i].line, 2, 56, i+7, term, &ch);
            break;
         }
      switch (ch) {
         case UP:
            i--;
            break;
         case DOWN:
            i++;
            break;
         case KB_CR:
            if (j == 3 && i < 4) i++;
         case KB_TAB:
            j = (j+1) % 4;
            break;
         case BACKTAB:
            if (j == 0 && i > 0) i--;
            j = (j+3) % 4;
            break;
         }
         if (i < 0) i = nr-1;
         if (i >= nr) {
            i = 0;
            }
      } while (ch != ESC && ch != L'+');
   if (ch == L'+') {
      for (k = 0; k < nr; k++) {
			if (fnt[k].fontfamily >= 4) {
				fnt[k].pitch = 120/fnt[k].size;
				fnt[k].size = 0;
				}
			else
				fnt[k].pitch = 0;
			}
		DefFnt = fnt[0];
		TitleFnt = fnt[1];
		HeaderFnt = fnt[2];
		CourFnt = fnt[3];
      }
   }

#ifdef _CONSOLE
static void near muotoilu(void)
   {
   wchar_t ch, s[6], ln[100], term[] = L"+\r\t\x0b\x05\x18\x1b";
   INT i,j,k,nfld;
   FldFrmtTp *fld;

   clrscr();
   kehys(0);
   clrln(ySize - 3);
   ch = L' ';
   wselectopt(L"Luetaanko muotoilu levyltä (K/E)", L"KE", &ch);
   clrln(ySize - 3);
   if (ch == L'K') {
	   vidspwmsg(ySize - 3, 0, 7, 0, L"Tiedoston nimi:");
	   inputwstr(luetfnm, 39, 16, ySize - 3, L"\r\x1b", &ch, FALSE);
	   if (ch != ESC)
		   LueXMLMuotoilu(luetfnm);
	   }
	if (lsttulparam.printer == GDIPRINTER) {
		vidspwmsg(12,0,7,0,L"Käytössä Windowsin kirjoitinajurit. Kohdistus rivin tuhannesosina");
		}
	else {
      viwrrectw(12,0,12,34,L"Valitse sarakkeiden kohdistustapa :",7,0,0);
//   viwrrectw(13,4,19,51,
      viwrrectw(13,4,18,51,
         L"1 : Merkkipohjainen kohdistus                   "
         L"2 : Matriisikirjoitintyyppinen tabulaattori     "
         L"3 : HP Laserjet kohdistus yksiköissä 1/72 tuumaa"
         L"4 : Postscript kohdistus yksiköissä 1/72 tuumaa "
         L"5 : Proprinter emulointi                        "
         L"6 : Tekstitiedosto erottimina tabulaattori      "
         ,7,0,0);
//      L"7 : Canon laser kohdistus yksiköissä 1/72 tuumaa"
      ch = lsttulparam.printer + L'1';
      if (lsttulparam.printer == 1 && !lsttulparam.tulmuot.tabs) ch = L'1';
//   wselectopt(L"Valitse kohdistustapa 1, 2, 3, 4, 5, 6 tai 7", L"1234567", &ch);
      wselectopt(L"Valitse kohdistustapa 1, 2, 3, 4, 5 tai 6", L"123456", &ch);

      for (j = 12; j < ySize-5; j++) clrln(j);
      switch (ch) {
         case L'1':
            lsttulparam.printer = EPSON;
            lsttulparam.tulmuot.tabs = 0;
            break;
         case L'2':
            lsttulparam.printer = EPSON;
            lsttulparam.tulmuot.tabs = 1;
            break;
         case L'3':
            lsttulparam.printer = LJETII;
            lsttulparam.tulmuot.tabs = 0;
            break;
         case L'4':
            lsttulparam.printer = POSTSCRIPT;
            lsttulparam.tulmuot.tabs = 0;
            break;
         case L'5':
            lsttulparam.printer = PROPRIEMU;
            lsttulparam.tulmuot.tabs = 0;
            break;
         case L'6':
            lsttulparam.printer = TXTTAB;
            lsttulparam.tulmuot.tabs = 1;
            break;
         case L'7':
			lsttulparam.printer = GDIPRINTER;
            lsttulparam.tulmuot.tabs = 0;
            break;
         }
	  tulosmuot.tabs = lsttulparam.tulmuot.tabs;
      }

   vidspwmsg(2,0,7,0,L"Sivun pituus      riviä, joista alkuun tyhjiä");
   vidint(2,13,4,tulosmuot.sivpit);
   vidint(2,46,2,tulosmuot.esirivit);
   viwrrectw(3,0,4,72,L"Pääotsikko tulostetaan kerran erikseen pyydet"
      L"tävän tulosluettelon alussa,toistettavat otsikot jokaisen "
      L"sarjan alussa ennen ja jälkeen sarjanimen. ",7,0,0);
   viwrrectw(5,0,5,12,L"Pääotsikko :",7,0,0);
   vidspwmsg(6,0,7,0,paaots);
   viwrrectw(7,0,7,40,L"Sarjanimeä edeltävä toistettava otsikko :",7,0,0);
   vidspwmsg(8,0,7,0,kilpailu);
   viwrrectw(9,0,9,40,L"Sarjanimeä seuraava toistettava otsikko :",7,0,0);
   vidspwmsg(10,0,7,0,alaots);
//   ch = uusisivu ? L'K' : L'E';
//   wselectopt(L"Tulostetaanko jokainen sarja uudelle sivulle (K/E)", L"KE", &ch);
//   uusisivu = (ch == L'K');
   ch = tulosmuot.snimifl ? L'K' : L'E';
   wselectopt(L"Tulostetaanko otsikkotiedoissa sarjanimi (K/E)", L"KE", &ch);
   tulosmuot.snimifl = (ch == L'K');
   ch = tulosmuot.tpvafl ? L'K' : L'E';
   wselectopt(L"Tulostetaanko otsikkotiedoissa tulostuspäivä (K/E)", L"KE", &ch);
   tulosmuot.tpvafl = (ch == L'K');
   ch = tulosmuot.tklofl ? L'K' : L'E';
   wselectopt(L"Tulostetaanko otsikkotiedoissa kellonaika (K/E)", L"KE", &ch);
   tulosmuot.tklofl = (ch == L'K');
   ch = tulosmuot.otsikot ? L'K' : L'E';
   wselectopt(L"Tulostetaanko sarakkeiden otsikot (K/E)", L"KE", &ch);
   tulosmuot.otsikot = (ch == L'K');
   clrln(ySize-3);
   INPUTINTW(&tulosmuot.sivpit,4,13,2,L"\r\x1B",&ch);
   if (ch == ESC) goto loppu;
   INPUTINTW(&tulosmuot.esirivit,2,46,2,L"\r\x1B",&ch);
   if (ch == ESC) goto loppu;
   viwrrectw(ySize-3,0,ySize-3,35,L"Paina <Esc> lopettaaksesi muutokset",7,0,0);
   inputwstr(paaots,80,0,6,L"\r\x1B",&ch,FALSE);
   if (ch == ESC) goto loppu;
   inputwstr(kilpailu,80,0,8,L"\r\x1B",&ch,FALSE);
   if (ch == ESC) goto loppu;
   inputwstr(alaots,80,0,10,L"\r\x1B",&ch,FALSE);
   if (ch == ESC) goto loppu;
   ch = tulosmuot.vaihdanimet ? L'K' : L'E';
   wselectopt(L"Vaihdetaanko etu- ja sukunimien järjestys (KE)", L"KE", &ch);
   tulosmuot.vaihdanimet = (ch == L'K');
   ch = tulosmuot.nollakarki ? L'E' : L'K';
   wselectopt(L"Tulostetaanko erosarkakkeessa voittajalle kokonaisaika (KE)", L"KE", &ch);
   tulosmuot.nollakarki = (ch == L'E');
   clrln(ySize-3);
   vidspwmsg(ySize-3,0,7,0,L"Anna numeromerkin leveys");
   INPUTINTW(&tulosmuot.numlev, 3, 26, ySize-3, L"\r", &ch);
   clrln(ySize-3);
   vidspwmsg(ySize-3,0,7,0,L"Anna vasen marginaali");
   INPUTINTW(&tulosmuot.marg, 3, 23, ySize-3, L"\r", &ch);
   clrln(ySize-3);

   lsttulparam.tulmuot = tulosmuot;

   for (i = 2; i < ySize-4; i++) clrln(i);
   vidspwmsg(2,0,7,0,L"Anna paperitulostuksen kenttien sijainnit");
   vidspwmsg(2,42,7,0,L"Yksi kilpailupäivä tai yhteisaika");
   vidspwmsg(4,3,7,0,L"Kenttä     Sijainti Leveys Tasaus");
	if (lsttulparam.printer == GDIPRINTER)
		wcscpy(ln,L"Sijainti 1/1000 rivistä, leveys merkkeinä");
	else
      swprintf(ln,L"Sijainti %s, leveys merkkeinä",
         (lsttulparam.printer == LJETII || lsttulparam.printer == POSTSCRIPT) ? L"yksiköissä 1/72 in" :
         L"merkkeinä");
   vidspwmsg(20,0,7,0, ln);
   fld = prtflds;
   nfld = n_prtflds;
   for (;;) {
      for (j = 0; j < ySize-10; j++) {
         if (j < nfld) {
            swprintf(ln, L"%-10.10s   %3d    %3d     %3.3s",
               fld[j].name, fld[j].pos, fld[j].len,
               fld[j].oik ? L"Oik" : L"Vas");
            vidspwmsg(j+5, 3, 7, 0, ln);
            }     
         else clrln(j+5);
         }
      for (j = 0, k = 0;;) {
         vidspwmsg(ySize-3,0,7,0,L"Paina <Esc> lopettaaksesi tämän sivun muutokset");
         switch (k) {
            case 0:
               inputwstr(fld[j].name, 10, 3, j+5, term, &ch, 0); 
               break;
            case 1:
               INPUTINTW(&fld[j].pos, 3, 16, j+5, term, &ch);
               break;
            case 2:
               INPUTINTW((INT *)&fld[j].len, 3, 23, j+5, term, &ch);
               break;
            case 3:
               wcscpy(s, fld[j].oik ? L"Oik" : L"Vas");
               inputwstr(s, 3, 31, j+5, term, &ch, 0);
               fld[j].oik = (s[0] == L'O' || s[0] == L'o');
               break;
            }
         if (ch == ESC || ch == L'+') break;
         switch (ch) {
            case KB_CR:
               if (k == 3)
                  j++;      // case jatkuu tarkoituksella
            case KB_TAB:
               k = (k+1) % 4;
               break;
            case BACKTAB:
               k = (k+3) % 4;
               break;
            case UP:
               j = (j+nfld-1) % nfld;
               break;
            case DOWN:
               j = (j+1) % nfld;
               break;
            }
         if (j >= nfld) {
            ch = L'K';
            wselectopt(L"Poistutaanko tältä sivulta (K/E)", L"KE", &ch);
            if (ch == L'K')
               break;
            j = 0;
            }
         }
      if (fld == dspflds) break;
         if (fld == prtflds) {
            fld = prtfldsm;
            nfld = n_prtfldsm;
            vidspwmsg(2,42,7,0,L"Kilpailupäivä sekä yhteisaika    ");
            continue;
            }
         if (fld == prtfldsm) {
            fld = prtflds1;
            nfld = n_prtflds1;
            vidspwmsg(2,42,7,0,L"1. ja 2. pv. sekä yhteisaika     ");
            continue;
            }
         if (fld == prtflds1) {
            fld = fileflds;
            nfld = n_fileflds;
            vidspwmsg(2,42,7,0,L"Tiedostotulosteet                ");
            continue;
            }
         fld = dspflds;
         nfld = n_dspflds;
         clrln(2);
         clrln(20);
         vidspwmsg(2,0,7,0,L"Anna näyttötulostuksen kenttien sijainnit");
      }
loppu:
   ch = L' ';
   wselectopt(L"Muutetaanko käytettäviä fonttimäärittelyjä (K/E)",
      L"KE", &ch);
   if (ch == L'K') {
		if (lsttulparam.printer == GDIPRINTER) {
			GDIfontit();
			}
		else {
         ch = L' ';
         wselectopt(L"Asetetaanko kirjoittimen vakio-oletuskoodit (K/E)", L"KE", &ch);
         if (lsttulparam.printer == LJETII) {
            if (ch == L'K') {
               init_fnt[0] = 0;
               pots_on[0] = 0;
               aots_on[0] = 0;
               }
            hpkoodit();
            }
         else {
            if (ch == L'K') {
               switch (lsttulparam.printer) {
                  case POSTSCRIPT:
                     wcscpy(init_fnt, L"\x03nf ");
                     break;
                  case EPSON:
                     break;
                  }
               editkoodi(init_fnt,L"Perusfontin valinta");
               switch (lsttulparam.printer) {
                  case POSTSCRIPT:
                     wcscpy(pots_on, L"\x03pf ");
                     memcpy(pots_pois, init_fnt, 2*PRKOODIPIT);
                     break;
                  case EPSON:
                     break;
                  }
               editkoodi(pots_on,L"Pääotsikkofontin valinta");
               if (lsttulparam.printer != POSTSCRIPT)
                  editkoodi(pots_pois,L"Pääotsikkofontin loppu");
               switch (lsttulparam.printer) {
                  case POSTSCRIPT:
                     wcscpy(aots_on, L"\x03of ");
                     memcpy(aots_pois, init_fnt, 2*PRKOODIPIT);
                     break;
                  case EPSON:
                     break;
                  }
               editkoodi(aots_on,L"Aliotsikkofontin valinta");
               if (lsttulparam.printer != POSTSCRIPT)
                  editkoodi(aots_pois,L"Aliotsikkofontin loppu");
               }
            else {
               editkoodi(init_fnt,L"Perusfontin valinta");
               editkoodi(pots_on,L"Pääotsikkofontin valinta");
               if (lsttulparam.printer != POSTSCRIPT)
                  editkoodi(pots_pois,L"Pääotsikkofontin loppu");
               editkoodi(aots_on,L"Aliotsikkofontin valinta");
               if (lsttulparam.printer != POSTSCRIPT)
                  editkoodi(aots_pois,L"Aliotsikkofontin loppu");
               if (lsttulparam.printer == POSTSCRIPT) {
                  wcscpy(pots_pois, init_fnt);
                  wcscpy(aots_pois, init_fnt);
                  }
               }
            }
         }
      }
   ch = L' ';
   wselectopt(L"Tallennetaanko uusi muotoilu levylle (K/E)", L"KE", &ch);
   if (ch == L'K') {
      clrln(ySize-3);
      vidspwmsg(ySize-3,0,7,0,L"Tiedoston nimi:");
      inputwstr(luetfnm, 39, 16, ySize-3, L"\r\x1b", &ch, FALSE);
	  if (ch != ESC)
		  TallXMLMuotoilu(luetfnm);
//		  tall_muotoilu(luetfnm);
   }
//   if (lsttulparam.printer == POSTSCRIPT || lsttulparam.printer == LJETII)
//      scale = 7;
//   else scale = 1;
   if (lsttulparam.printer == LJETII) vasivu();
   }
#endif

#ifdef JOUKKUEPISTEET
void joukkuepistemaarittelyt(void)
   {
   typedef struct {
      INT piste;
	  wchar_t seura[LSEURA+1];
      } pistetp;
   wchar_t ch2, linebuf[100];
   INT srj, npiste, nseura = 0, l;
   TextFl *psrjfile;

	do {
		clrln(ySize-1);
		clrln(ySize-3);
		vidspwmsg(ySize-3,0,7,0,L"Anna sarja");
		vidspwmsg(ySize-1,0,7,0,L"Valitse muutettava sarja, lopuksi Esc");
		for (l = 3; l < ySize-4; l++) 
			clrtxt(l, 0, 64);
		l = 3;
		for (srj = 0; srj < sarjaluku; srj++) {
			if (Sarjat[srj].npist[k_pv]) {
				if (l < ySize-4) {
					vidspwmsg(l, 0, 7, 0, Sarjat[srj].sarjanimi);
					vidint(l, 12, 3, Sarjat[srj].npist[k_pv]);
					}
				l++;
				}
			}
		srj = luesarja(NULL, &ch2);
		if (ch2 != ESC && srj >= 0) {
			vidspwmsg(ySize-3,40,7,0,L"Lukumäärä:");
			npiste = Sarjat[srj].npist[k_pv];
			INPUTINTW(&npiste, 3, 52, ySize-3, L"\r", &ch2);
			Sarjat[srj].npist[k_pv] = (wchar_t) npiste;
			}
		} while (ch2 != ESC);
	ch2 = L' ';
	wselectopt(L"Tallennetaanko muutokset tiedostoon PISTESRJ.LST (K/E)",
		L"KE", &ch2);
	if (ch2 == L'K') {
		if ((psrjfile = new TextFl(L"PISTESRJ.LST", L"w")) == NULL) {
			writeerror_w(L"Tiedoston PISTESRJ.LST avaaminen ei onnistu", 0);
			return;
			}
		for (srj = 0; srj < sarjaluku; srj++) {
			if (Sarjat[srj].npist[k_pv]) {
				swprintf(linebuf, L"%-10.10s %3d\n", Sarjat[srj].sarjanimi,
					(int) Sarjat[srj].npist[k_pv]);
				psrjfile->WriteLine(linebuf);
				}
			}
		delete psrjfile;
		}
	}
#endif

#ifdef SEURATUL

void seuralista(void)
   {
   wchar_t sra[LSEURA+1], ch, *p, tc, *buf = 0, linebuf[200];
   int lista, i, lstfl;
   TextFl *sralstf;

   for (i = 0; i < SEURALISTA; i++) seuranimi[i][0] = 0;
   do {
      ch = L' ';
#ifndef LANGSV_K
#ifndef _CONSOLE
      wselectopt(L"Hae T)iedostosta tai L)uettele seurat, Esc : Poistu", L"TL\x1b", &ch);
#else
      wselectopt(L"Hae T)iedostosta tai L)uettele seurat, M)uokkaa tiedostoa, Esc : Poistu", L"TLM\x1b", &ch);
#endif
#else
      wselectopt(L"L)äs fråm fil eller F)örtäckna föreningar, B)earbeta fil, Esc : Avsluta", L"LFB\x1b", &ch);
      wtransl(L"LFB", L"TLM", &ch);
#endif
#ifdef _CONSOLE
      if (ch == L'M') {
		  sralstf = new TextFl(L"SEURATUL.LST", L"r+");
		  if (!sralstf->IsOpen()) 
			  sralstf->Open(L"w");
		  if (sralstf->IsOpen()) {
#ifndef LANGSV_K
            writeerror_w(L"Tiedoston SEURATUL.LST avaaminen ei onnistu", 0);
#else // LANGSV_K
            writeerror_w(L"Filet SEURATUL.LST kan inte öppnas", 0);
#endif // LANGSV_K
			delete sralstf;
            return;
            }
         if ((buf = (wchar_t *) calloc(CBUFL, 2)) == 0)
            return;
         p = buf;
         *p = 0;
		 while (!sralstf->Feof()) {
			 sralstf->ReadLine(linebuf, 198);
			 if (wcslen(linebuf) > 1) {
	            if (wcslen(buf) + wcslen(linebuf) >= CBUFL-1) {
#ifndef LANGSV_K
               writeerror_w(L"Puskuritila ei riitä, osa tiedoista menetetään"
                  L" tallennettaessa", 0);
#else // LANGSV_K
               writeerror_w(L"Puskuritila ei riitä, osa tiedoista menetetään"
                  L" tallennettaessa", 0);
#endif // LANGSV_K
               break;
               }
            wcscat(p, linebuf);
			 }
            }
         if (!*buf) wcscpy(buf,L"1\n");
         clrln(ySize-3);
#ifndef LANGSV_K
         vidspwmsg(ySize-3,0,7,0,L"Paina Esc lopettaaksesi muokkaus");
#else // LANGSV_K
         vidspwmsg(ySize-3,0,7,0,L"Tryck Esc att sluta bearbeting");
#endif // LANGSV_K
         wpageedit(buf,CBUFL,3,10,20,40,L"+\x1B", &ch);
         tc = L' ';
#ifndef LANGSV_K
         wselectopt(L"Tallennetaanko muutokset (K/E)", L"KE", &tc);
#else
         wselectopt(L"Bevara ändringart (J/E)", L"JE", &tc);
         wtransl(L"J", L"K", &tc);
#endif
         if (tc == L'K') {
			 sralstf->Rewind();
			 sralstf->WriteLine(buf);
            }
         delete sralstf;
         free(buf);
         }
      } while (ch == L'M');
#endif // _CONSOLE
   if (ch == L'L') {
      clrln(ySize-3);
#ifndef LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Anna seurojen nimilyhenteet, lopuksi tyhjä");
#else // LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Ge in föreningförkortningar, sluta med tom");
#endif // LANGSV_K
      for (i = 0; i < SEURALISTA; i++) {
         sra[0] = 0;
         inputwstr(sra, kilpparam.lseura, 46, ySize-3, L"\x0D\x1B", &ch, 0);
         if (ch == ESC) {
            seuranimi[0][0] = 0;
            break;
            }
         if (!sra[0]) break;
         upcasewstr(sra);
         wcscpy(seuranimi[i], sra);
         }
      }
   if (ch == L'T') {
      clrln(ySize-3);
#ifndef LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Anna seuralistan numero tai Esc lopettaaksesi");
#else // LANGSV_K
      vidspwmsg(ySize-3,0,7,0,L"Ge in föreninglistans nummer. Esc att sluta  ");
#endif // LANGSV_K
      lista = 0;
      INPUTINTW(&lista, 4, 46, ySize-3, L"\x0D\x1B", &ch);
      if (ch == 27 || !lista) {
		 return;
         }
      sralstf = new TextFl(L"SEURATUL.LST", L"r");
	  if (!sralstf->IsOpen()) {
#ifndef LANGSV_K
         writeerror_w(L"Seuralistatiedostoa SEURATUL.LST ei löydy", 0);
#else // LANGSV_K
         writeerror_w(L"Filet SEURATUL.LST kan inte öppnas", 0);
#endif // LANGSV_K
		 delete sralstf;
         return;
         }
      i = 0;
      lstfl = 0;
	  while (!sralstf->Feof()) {
		  sralstf->ReadLine(linebuf, 198);
		  if (wcslen(linebuf) < 2) 
            break;
         if (!lstfl) {
            if (_wtoi(linebuf) == lista) lstfl = 1;
            }
         else {
            if (_wtoi(linebuf) > 0) break;
            else {
               if (wcslen(linebuf) > 1) {
                  linebuf[wcslen(linebuf)-1] = 0;
                  upcasewstr(linebuf);
                  wcsncpy(seuranimi[i++], linebuf, LSEURA);
                  if (i >= SEURALISTA) break;
                  }
               }
            }
         }
      delete sralstf;
      }
   }

void haeseura(wchar_t *seura)
   {
   wchar_t tc;

   clrln(ySize-3);
   seura[LSEURA] = 0;
   vidspwmsg(ySize-3,0,7,0,
	  L"Anna seuran nimi :                                       <Esc> : Lopeta");
   inputwstr(seura,kilpparam.lseura,19,ySize-3,L"\x0D\x1B",&tc,FALSE);
   if (tc == ESC) seura[0] = 0;
   else {
      elimwbl(seura);
	  upcasewstr(seura);
      }
   }

int seuratulval_pohja(wchar_t *ss, int *ssrj, tulostusparamtp *tulostusparam)
{
	wchar_t ch, tc;
	static INT initpiirit = 0;

	if (tulostusparam->seuratulokset) {

#ifndef LANGSV_K
		wselectopt(L"Y)ksi, L)ista, K)aikki seurat,  <Esc> : Peruuta",
			L"YLK\x1B", ss);
#else
		wtransl(L"YLK", L"ELA", ss);
		wselectopt(L"E)n, L)ista, A)lla föreningar,  <Esc> : ngra",
			L"ELA\x1B", &ss);
		wtransl(L"ELA", L"YLK", ss);
#endif
#ifdef ORAVA
		ch = (*ssrj == -1) ? L'K' : L'Y';
#ifndef LANGSV_K
        wselectopt(L"K)aikki sarjat vai Y)ksi sarja", L"KY", &ch);
#else
		wtransl(L"KY", L"AE", &ch);
		wselectopt(L"A)lla klasser vai E)n klass", L"AE", &ch);
		wtransl(L"AE", L"KY", &ch);
#endif
		if (ch == L'Y') {
			clrln(ySize-3);
#ifndef LANGSV_K
			vidspwmsg(ySize-3,0,7,0,L"       Anna sarja :");
#else // LANGSV_K
            vidspwmsg(ySize-3,0,7,0,L"       Anna sarja :");
#endif // LANGSV_K
			*ssrj = luesarja((*ssrj == -1) ? L"" : Sarjat[*ssrj+1].sarjanimi, &tc);
			if( tc == ESC ) return(2);
			}
		else 
			*ssrj = -1;
#endif
		switch (*ss) {
			case ESC :
				return(2);
			case L'K' :
				seuraluettelo();
				if (!seurat) 
					return(2);
			}
		}
	if (tulostusparam->piiritulokset) {
		if (!initpiirit) {
			luepiirit(0);
			initpiirit = 1;
			}
		wselectopt(L"V)alittavat vai K)aikki piirit,  <Esc> : Peruuta",L"VK\x1B",
			ss);
		if (*ss == ESC) 
			return(2);
		ch = tulostusparam->piirisijat ? L'P' : L'S';
		wselectopt(L"Tulostetaanko S)arja- vai P)iirikohtaiset sijat", L"SP",
			&ch);
		tulostusparam->piirisijat = ch == L'P';
		}

	if (tulostusparam->piirisijat) 
		tulostusparam->sj_raja = 0;
	else {
		clrln(ySize-3);
#ifndef LANGSV_K
		vidspwmsg(ySize-3,0,7,0,L"Kuinka monta parasta tulostetaan, "
			L"vaikka seura/piiri ei ole pyydetty");
#else // LANGSV_K
		vidspwmsg(ySize-3,0,7,0,L"Antal bästa att skriva oberoende, "
			L"av förening eller distrikt");
#endif // LANGSV_K
		INPUTINTW(&tulostusparam->sj_raja, 2, 69, ySize-3, L"\r", &ch);
		if (tulostusparam->sj_raja) {
			ch = tulostusparam->karjet ? L'K' : L'E';
#ifndef LANGSV_K
			wselectopt(L"Tulostetaanko kärjet, vaikka sarjassa ei "
				L"valintaehdon täyttäviä (K/E)", L"KE", &ch);
#else
			wselectopt(L"Skall de bästa skrivas även om inga av "
				L"valda föreningar (J/E)", L"JE", &ch);
			wtransl(L"J", L"K", &ch);
#endif
			tulostusparam->karjet = ch == L'K';
			}
		}
	return(0);
}

int seuratulval_toist(wchar_t ss, int *is, int *piiri, tulostusparamtp *tulprm)
{
	int i, r;
	wchar_t prs[82], ch;

	if (tulprm->seuratulokset) {
		if ((ss == L'K') && seurat[++*is].seura)
			memmove(seuranimi[0], seurat[*is].seura, LSEURA+1);
		else if (ss == L'Y') 
			haeseura(seuranimi[0]);
		else if (ss == L'L') 
			seuralista();
		else 
			seuranimi[0][0] = 0;
		seuranimi[0][LSEURA] = 0;
		if (!seuranimi[0][0]) 
			return(1);
		clrln(ySize-3);
#ifndef LANGSV_K
		wcscpy(prs,L"Tulostaa seuraa :");
#else // LANGSV_K
		wcscpy(prs,L"Utskrift av föreningen :");
#endif // LANGSV_K
		for (i = 0; seuranimi[i][0] && i < SEURALISTA; i++) {
			if (wcslen(prs) > 65) 
				break;
			wcscat(prs, L" ");
			wcscat(prs, seuranimi[i]);
			vidspwmsg(ySize-3,0,7,0,prs);
			}
		}
	if (tulprm->piiritulokset) {
		++*piiri;
		memset(piirifl, 0, sizeof(piirifl));
		if (ss == L'V') {
			clrln(ySize-3);
			for (r = 0; r < 18; r++) {
				clrtxt(r+2,0,62);
				vidint(r+2,0,2,r+1);
				vidspwmsg(r+2,4,7,0,piirinimi[r]);
				vidint(r+2,30,2,r+19);
				vidspwmsg(r+2,34,7,0,piirinimi[r+18]);
				}
			vidspwmsg(ySize-3,0,7,0,L"Luettele piirien numerot ("
				L"Lopuksi 0)     Esc : keskeytä");
			r = 0;
			do {
				INPUTINTW(piiri, 2, 37, ySize-3,L"\r\x1B", &ch);
				if (*piiri > 0 && *piiri <= piiriluku) {
					piirifl[*piiri-1] = 1;
					r = *piiri;
					}
				} while (ch != ESC && *piiri);
			if (!r || ch == ESC) 
				return(1);
			*piiri = r;
			}
		else {
			if (*piiri <= piiriluku) 
				piirifl[*piiri-1] = 1;
			}
		if (*piiri < 1 || *piiri > piiriluku) {
			*piiri = 0;
			return(1);
			}
		clrln(ySize-3);
		viwrrectw(ySize-3,0,ySize-3,16,L"Tulostaa piiriä :", 7, 0, 0);
		if (ss == L'K') 
			vidspwmsg(ySize-3,18,7,0,piirinimi[*piiri-1]);
		}
	return(0);
}

#endif

static void tiedostovalinnat(tulostusparamtp *tulostusparam)
	{
	wchar_t ch, opts[12], msg[90], co = L' ';
	static bool lehtierot = true, lehtivaihdanm = true, lehtipitkat = true;
#ifdef EMIT
	if (pvparam[k_pv].laika > 8)
		wcscpy(msg, L"K)ohdistettu, E)rottimin, L)ehteen, S)SL/SHL, H)TML, X)ML, V)äliajoin");
	else
      wcscpy(msg, L"K)ohdistettu, E)rottimin, L)ehteen, S)SL, H)TML, X)ML, V)äliajoin");
   wcscpy(opts, L"KELSHVX");
#else
#ifdef JOUKKUETUL
      wcscpy(msg, L"K)ohdistettu, E)rottimin, L)ehteen, H)TML, X)ML, V)äliajoin, J)oukkueet");
   wcscpy(opts, L"KELHVXJ");
#else
      wcscpy(msg, L"K)ohdistettu, E)rottimin, L)ehteen, S)HL, H)TML, X)ML, V)äliajoin");
   wcscpy(opts, L"KELSHVX");
#endif
#endif
#ifdef IOF
   wcscat(msg, L", I)OF");
   wcscat(opts, L"I");
#endif
   wselectopt(msg, opts, &co);
#ifndef LANGSV_K
   wtransl(L"KELSIW", L"IRESFF", &co);
#else
   wtransl(L"KETSIW", L"IRESFF", &co);
#endif
#if defined(EMIT)
	if (pvparam[k_pv].laika > 8 && co == L'S') {
		ch = ssltlsfl ? L'S' : L'H';
		wselectopt(L"S)uunnistuksen, H)iihdon tiedostomuoto", L"SH", &ch);
		ssltlsfl = ch == L'S';
		}
#endif
   tulostusparam->kaikkiva = 0;
   if (co == L'V') {
      tulostusparam->kaikkiva = 1;
      co = 'Z';
      }
   if (co == L'R') {
      switch (tulostusparam->erotin) {
         case L';': 
            ch = L'U';
            break;
         case L',': 
            ch = L'P';
            break;
         case L'\t': 
            ch = L'T';
            break;
         }
      wselectopt(L"Erotin: pU)olipiste, P)ilkku, T)abulaattori", L"UPT;,\t", &ch);
      switch (ch) {
         case L'U':
         case L';':
            tulostusparam->erotin = L';';
            break;
         case L'P':
         case L',':
            tulostusparam->erotin = L',';
            break;
         case L'T':
         case L'\t':
            tulostusparam->erotin = L'\t';
            break;
         }
      }
   if (co == L'E') {
//      ch = lehtimuoto ? L'E' : L'K';
//      wselectopt(L"Tulostetaanko kilpailunumero (K/E)", L"KE", &ch);
//      lehtimuoto = ch == L'E';
      ch = lehtierot ? L'E' : L'K';
      wselectopt(L"Tulostetaanko E)rot kärkeen vai K)okonaisajat", L"KE",
         &ch);
      lehtierot = ch == L'E';
      lehtivaihdanm = 0;
      lehtipitkat = 1;
      ch = lehtivaihdanm ? L'K' : L'E';
      wselectopt(L"Vaihdetaanko etu- ja sukunimen järjestys (K/E)", L"KE", &ch);
      lehtivaihdanm = ch == L'K';
      ch = lehtipitkat ? L'K' : L'E';
      wselectopt(L"Tulostetaanko L)yhenne vai S)euranimikenttä", L"LS", &ch);
      lehtipitkat = ch == L'S';
	  tulostusparam->optiot &= 0xfff8ffff;
	  if (lehtierot)
		  tulostusparam->optiot |= 0x10000;
	  if (lehtivaihdanm)
		  tulostusparam->optiot |= 0x20000;
	  if (lehtipitkat)
		  tulostusparam->optiot |= 0x40000;
      }
   tulostusparam->kohde = co;
   }

int valitsekohde(wchar_t *co)
{
#if defined(TAULU_2)
   wchar_t msg[90];
   wchar_t opts[20];

      wcscpy(msg, L"N)äyttö, P)aperi, A)utomaattinen, M)uotoilu, "
         L"tI)edostoon");
      wcscpy(opts, L"NPAMI\x1b");
      if (taulu_com) {
         wcscat(msg, L", V)alotaulu");
         wcscat(opts, L"V");
         }
      wselectopt(msg, opts, co);
#else
      if (spoolfl)
         wselectopt(L"N)äytölle, P)aperille, A)utomaattinen, M)uotoilu, "
            L"tI)edostoon", L"NPAMI\x1b",co);
      else {
#if defined(TCPSIIRTO)
		  if (aftulparam.kohde == L'X' && autofileparam.afvali == 0 && autofileparam.afname[0]) {
            wselectopt(L"N)äytölle, P)aperille, A)utomaattinen, M)uotoilu"
               L", tI)edostoon, L)ähetä",L"NPAMIL\x1b",co);
            if (*co == L'L') {
               wselectopt(L"T)uloksen saaneet, I)lmoittautuneet", L"TI", co);
               autofile(*co == L'I');
			   *co = ESC;
               return(1);
               }
            }
         else
#endif
            wselectopt(L"N)äytölle, P)aperille, A)utomaattinen, M)uotoilu"
               L", tI)edostoon",L"NPAMI\x1b",co);
         }
#endif
	  return(*co == ESC);
}

int valitsesisalto(int oikaise, tulostusparamtp *tulprm)
{
	wchar_t cy = L'K'; 

	if (oikaise)
		return(0);
	if (tulprm->kohde == L'N') {

#if !defined(JOUKKUEPISTEET)
#ifdef MAKI
		if (k_pv == makipv) {
			aikaerot = 0;
	      wselectopt(L"K)ilpailijat, Y)hteenveto, A)ikaerot, H)ypyt, Esc: Peruuta",
            L"HKYA\x1B", cy);
			if (*cy == L'H')
				*cy = L'V';
			if (*cy == L'A') {
				*cy = L'K';
				aikaerot = 1;
				}
			}
		else
#else
	       wselectopt(L"K)ilpailijat, Y)hteenveto, V)äliajat, Esc: Peruuta",
		       L"KYV\x1B", &cy);
#endif
#else     // JOUKKUEPISTEET
         wselectopt(L"K)ilpailijat, Y)hteenveto, V)äliajat, P)isteet, "
            L"Esc: Peruuta", L"KYVP\x1B", &cy);
#endif

      header=L"          ";
      scpclr();
      kehys(0);
      }
	else {
#if !defined(AMPUSUUNN) && !defined(JOUKKUEPISTEET)
#if defined(JOUKKUETUL)
      if (tulprm->kohde == L'P') 
		   wselectopt(L"K)ilpailijat, J)oukkueet, Y)hteenveto, "
				L"V)äliajat, M)uut, Esc: Peruuta",L"KJYVM\x1B",&cy);
#else
#ifdef MAKI
		if (k_pv == makipv) {
			aikaerot = 0;
			if (tulprm->kohde == L'P') {
				wselectopt(L"K)ilpailijat, H)ypyt, Y)hteenveto, "
					L"aI)koina, A)akkos, Esc: Peruuta",L"HKYLIAB\x1B",&cy);
				if (cy == L'I') {
					cy = L'K';
					aikaerot = 1;
					}
				if (cy == L'H')
					cy = L'V';
				}
			}
		else
#endif
	      if (tulprm->kohde == L'P') 
			  wselectopt(L"K)ilpailijat, Y)hteenveto, "
				L"V)äliajat, M)uut, Esc: Peruuta",L"KYVM\x1B",&cy); 
#endif 
#endif

#if !defined(AMPUSUUNN) && defined(JOUKKUEPISTEET)
      if (tulprm->kohde == L'P') 
		  wselectopt(L"K)ilpailijat, Y)hteenveto, "
			L"V)äliajat, P)isteet, M)uut, Esc: Peruuta",L"KYLVMP\x1B",&cy);
#endif 

#if defined(AMPUSUUNN)
      if (tulprm->kohde == L'P') 
		  wselectopt(L"K)ilpailijat, V)äliajat, Y)hteenveto, "
         L"J)oukkueet, Esc: Peruuta",L"KYJV\x1B",&cy);
#endif 
		
		if (tulprm->kohde == L'P' && cy == L'M') {
			cy = L' ';
			wselectopt(L"A)akkostetut, N)umerojärjestyksessä",
				L"AN", &cy);
			tulprm->ixjarj = 1 + 10 * (cy == L'N');
			cy = L' ';
			wselectopt(L"Tulokset I)lman sijoituksia, S)ijoituksin, L)ähtöajat",
				L"ISL", &cy);
			switch (cy) {
				case L'I':
					tulprm->piilotasijat = true;
					cy = L'A';
					break;
				case L'S':
					tulprm->piilotasijat = false;
					cy = L'A';
					break;
				case L'L':
					tulprm->lahtoajat = true;
					cy = L'K';
				}
			}

      if (wcswcind(tulprm->kohde, L"IEHVR") >= 0) {
#ifdef MAKI
			if (k_pv == makipv) {
				aikaerot = 0;
				wselectopt(L"K)ilpailijat, H)ypyt, aI)koina, A)akkos, Esc: Peruuta",L"HKIA\x1B",&cy);
				if (*cy == L'I') {
					*cy = L'K';
					aikaerot = 1;
					}
				if (*cy == L'H')
					*cy = L'V';
				}
			else
#endif
	         wselectopt(L"K)ilpailijat, V)äliajat, A)akkos, Esc: Peruuta", L"KVA\x1B",&cy);

			}
	  }
	  switch (cy) {
		case ESC:
			return(ESC);
		case L'K':
			tulprm->tulostettava = L'T';
			break;
		case L'V':
			tulprm->tulostettava = L'V';
			break;
		case L'A':
			tulprm->ixjarj = 1;
			tulprm->tulostettava = L'O';
			break;
		case L'J':
			tulprm->tulostettava = L'J';
			break;
		case L'P':
			tulprm->tulostettava = L'Q';  // Joukkuepisteet
			break;
		case L'Y':
			tulprm->tulostettava = L'Y';
			break;
		}
	return(0);
   }

int valitsevapiste(tulostusparamtp *tulprm)  // int *piste, wchar_t *vali, wchar_t *cr, wchar_t *co)
{
wchar_t  opts[20], ch, vali = 0;

	if (emitfl) {
		if (!kilpparam.maxvaluku) {
			if (tulprm->kohde == L'I' || tulprm->kohde == L'H' || tulprm->kohde == L'P')
				vali = L'E';
			else
				return(ESC);
			}
		else {
			if (kilpparam.maxvaluku >= 10) {
				wselectopt(L"Valitse E)mit, Y)ksi väliaikapiste tai K)aikki pisteet", L"EYK", &vali);
				if (vali == L'Y') {
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Väliaikapisteen numero:");
					INPUTINTW(&tulprm->piste, 2, 25, ySize-3, L"\r", &vali);
					vali = tulprm->piste + L'0';
					}
				}
			else
				{
				wcscpy(opts, L"EK123456789:;");
				opts[kilpparam.maxvaluku+2] = 0;
				wselectopt(L"Valitse E)mit, väliaikapisteen numero tai K)aikki pisteet", opts, &vali);
				if (vali <= L'9')
					tulprm->piste = vali - L'0';
				}
			if (vali == L'K')
				tulprm->kaikkiva = true;
			}
		if (vali == L'E' && tulprm->kohde == L'I') {
			ch = L' ';
			wselectopt(L"A)nalysoidut väliajat vai L)uetut tiedot jatkokäsittelyyn",
				L"AL", &ch);
			if (ch == L'L') {
				kirjvaliajat(0);
				return(1);
				}
			ch = L' ';
			wselectopt(L"Kilpailija Y)hdelle riville vai J)aettuna monelle riville",
				L"YJ", &ch);
			if (ch == L'J') 
				tulprm->kohde = L'J';
			}
		if (vali == L'E') {
			tulprm->tulostettava = L'E';	
			ch = L' ';
			wselectopt(L"K)okonaisajat rasteille, R)astivälien ajat, "
				L"M)olemmat", L"KRM", &ch);
			tulprm->piste = wcswcind(ch, L"KRM");
			}
		}
	else
//#endif
		{
#ifdef MAKI
		if (k_pv == makipv) {
			ch = L' ';
			wselectopt(L"K)oekierros, 1) kierros, 2) kierros", L"K12", &vali);
			wtransl(L"K12", L"135", &vali);
			tulprm->piste = vali - L'0';
			}
		else
#endif
			{
			if (kilpparam.maxvaluku > 0) {
				if (kilpparam.maxvaluku == 1) 
					vali = L'1';
				else {
					vali = L' ';
					if (kilpparam.maxvaluku >= 10) {
						wselectopt(L"Valitse Y)ksi väliaikapiste tai K)aikki pisteet", L"YK", &vali);
						if (vali == L'Y') {
							clrln(ySize-3);
							vidspwmsg(ySize-3,0,7,0,L"Väliaikapisteen numero:");
							INPUTINTW(&tulprm->piste, 3, 25, ySize-3, L"\r", &vali);
							}
						}
					else
						{
						wcscpy(opts, L"K123456789:;");
						opts[kilpparam.maxvaluku+1] = 0;
						wselectopt(L"Valitse väliaikapisteen numero, tai K)aikki", opts, &vali);
						if (vali <= L'9')
							tulprm->piste = vali - L'0';
						}
					if (vali == L'K')
						tulprm->kaikkiva = true;
					}
				}
			else {
				writeerror_w(L"Väliaikojen käsittelyä ei käynnistetty", 0);
				return(1);
				}
			}
		}
	return(0);
}

int xmlvalinnat(tulostusparamtp *tulprm)
{
	wchar_t ch;
	wchar_t xml_tied_nimi[60] = L"tulokset.xml";

#ifdef EMITXML
	if (emitfl || kilpparam.maxvaluku)
		wselectopt(L"Kirjoitetaanko E)mitväliajat, M)uut väliajat, I)lman väliaikoja", L"EMI", &tulprm->xmlva);
	else
		tulprm->xmlva = L'I';
#else
#ifdef MAKI
	tulprm->xmlva = L'M';
	flatxml = 1;
#else
	if (kilpparam.maxvaluku)
		wselectopt(L"Kirjoitetaanko V)äliajoin, I)lman väliaikoja", L"VI", &tulprm->xmlva);
	else
		tulprm->xmlva = L'I';
	wtransl(L"V", L"M", &tulprm->xmlva);
#endif
#endif
#ifndef MAKI
	if (tulprm->xmlva != L'E') {
		ch = flatxml ? L'A' : L'S';
		wselectopt(L"S)trukturoitu XML, A)ccess-siirtoon", L"SA", &ch);
		flatxml = ch == L'A';
		}
#endif
    ch = L' ';
    wselectopt(L"U)TF-8 vai I)SO-8858-1 merkistö", L"UI", &ch);
	tulprm->merkit = (ch == L'I' ? L'A' : L'8');
	clrln(ySize-3);
	vidspwmsg(ySize-3, 0, 7, 0, L"Anna tiedoston nimi ");
	inputwstr(xml_tied_nimi, 40, 20, ySize-3, L"\r", &ch, 0);
	if (avaa_tiedosto_nm(xml_tied_nimi, tulprm)) 
		return(1);
#ifdef XML
#ifdef XMLFIS
	if (fiskoodi)
		xmlfisots();
	else
#endif
		{
		if (!kilpparam.kilpkoodi[0]) {
			clrln(ySize-3);
			vidspwmsg(ySize-3, 0, 7, 0, L"Anna kilpailukoodi");
			inputwstr(kilpparam.kilpkoodi,7,20,ySize-3,L"\r",&ch,FALSE);
			}
		xmlots(tulprm);
		}
#endif
	return(0);
}

int valitsetulpv (tulostusparamtp *tulostusparam, int oikaise)
{
	wchar_t ch;

	if (!k_pv || tulostusparam->tulostettava != L'T') {
		tulostusparam->yhttuljarj = false;
		tulostusparam->yhttulpaino = false;
		tulostusparam->vaihelkm = 1;
		}
	else {
		if (wcswcind(tulostusparam->kohde, L"NIRXZ") >= 0) {
			tulostusparam->yhttuljarj = true;
			tulostusparam->yhttulpaino = true;
			tulostusparam->vaihelkm = 2;
			}
		else if (tulostusparam->kohde == L'S') {
			tulostusparam->yhttuljarj = false;
			tulostusparam->yhttulpaino = false;
			tulostusparam->vaihelkm = 1;
			}
//#ifdef MERI5
		else {
			if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
				if (!oikaise) {
					wtransl(L"2M", L"PK", &ch);
					wselectopt(L"P)äivän aika ja pisteet, K)aikki pisteet",L"PK", &ch);
					wtransl(L"PK", L"2M", &ch);
					}
				switch (ch) {
					case L'M':
						tulostusparam->yhttuljarj = true;
						tulostusparam->yhttulpaino = true;
						tulostusparam->vaihelkm = 2;
						if (tulostusparam->kohde == L'P')
							tulostusparam->p_fld = prtfldsm;
						break;
					default:
						tulostusparam->yhttuljarj = false;
						tulostusparam->yhttulpaino = false;
						tulostusparam->vaihelkm = 1;
						if (tulostusparam->kohde == L'P')
							tulostusparam->p_fld = prtflds;
					}
				}
			else {
//#else
				if (!oikaise) {
					if (tulostusparam->vaihelkm == 2)
						ch = tulostusparam->yhttulpaino ? L'Y' : L'T';
					else
						ch = L"YYMK"[tulostusparam->vaihelkm];
					wselectopt(
						L"Tulosta Y)hteistulokset, T)ämä päivä, M)olemmat, K)aikki "
						L"päivät",L"YTMK", &ch);

					switch (ch) {
						case L'T':
							tulostusparam->yhttuljarj = false;
							tulostusparam->yhttulpaino = false;
							tulostusparam->vaihelkm = 1;
							break;
						case L'Y':
							tulostusparam->yhttuljarj = true;
							tulostusparam->yhttulpaino = true;
							tulostusparam->vaihelkm = 1;
							break;
						case L'M':
							tulostusparam->yhttuljarj = true;
							tulostusparam->yhttulpaino = true;
							tulostusparam->vaihelkm = 2;
							break;
						case L'K':
							tulostusparam->yhttuljarj = true;
							tulostusparam->yhttulpaino = true;
							tulostusparam->vaihelkm = k_pv+2;
							break;
						}
					}
				}
			}
		if (wcswcind(kilpparam.kilplaji, L"NP") < 0) {
			if  (tulostusparam->vaihelkm == 2) {
				if (tulostusparam->kohde == L'H')
					tulostusparam->yhttuljarj = true;
				else {
					if (!oikaise && tulostusparam->vaihelkm > 1) {		 
						ch = tulostusparam->yhttuljarj ? L'Y' : L'T';
						wselectopt(L"Järjestys: Y)hteistulokset vai T)ämä päivä", L"YT", &ch);
						tulostusparam->yhttuljarj = ch == L'Y';
						}
					}
				}
			else {
				tulostusparam->yhttuljarj = tulostusparam->yhttulpaino;
				}
			if (tulostusparam->kohde == L'P') {
				switch (tulostusparam->vaihelkm) {
					case 2:
						tulostusparam->p_fld = prtfldsm;
						break;
					case 3:
						tulostusparam->p_fld = prtflds1;
						break;
					default:
						tulostusparam->p_fld = prtflds;
					}
				}
			}
	    }
// #endif  // !MERI5
		return(0);
}

void tulostus(int oikaise)
{
	static wchar_t tiedlaji, tulostettava, jarjestys;
	static int pisteval, alaraja, ylaraja, sarjat[400];
	static wchar_t kohdenimi[100];
	static bool pitkat;
	wchar_t kohde = L' ';
	static tulostusparamtp tulprm;

   static INT lineno = 0;
   INT  er = 0, i,is,srj,vsrj,sv,enssrj,inlistCS = 0;
	int tulostettu = 0;
   wchar_t tc = 0, ss, ch;
   extern scr tulvalikko;
   extern scr tulvalikko2;
   INT piiri = 0;
#if defined(TAULU_2)
   char msg[90];
#endif
   static int ssrj = -1;
#if defined(UINTI) || defined(MERI5)
//   int uusisivu0;

//   uusisivu0 = uusisivu;
#endif
   bool autoasetukset = false;

   lineno = 0;
   clrscr();
   header=L"LISTAT    ";
   kehys(1);
   prscr(&tulvalikko);

	if (!oikaise) {
	   tulprm = lsttulparam;
	   tulprm.tulostettava = L'T';
	   tulprm.tarkastamattomat = false;
	   tulprm.seuratulokset = false;
	   tulprm.piiritulokset = false;
	   tulprm.piilotatulos = false;
	   tulprm.ixjarj = 0;
	   tulprm.kaikkiva = false;
	   tulprm.kaikkisijat = false;
	   tulprm.optiot = 0;
		tulprm.ixjarj = 0;
		tulprm.lahtoajat = false;
		tulprm.piilotasijat = false;
		}

#if N_PV > 2000
	if (k_pv > 2 && !oikaise) {
		wselectopt(L"P)äivän tulokset, M)onipäivälistaukset", L"PM", &tc);
		if (tc == L'M') {
			list_pv();
			return;
			}
		}
#endif
	kohde = tulprm.kohde;
   if (kohde != L'P') 
	   kohde = L'N';

   if (!oikaise) {
	   if (valitsekohde(&kohde))
		   return;
		}

	if (kohde != L'P')
		tulprm.lstf = NULL;
	else if (tulprm.lstf == NULL) {
		writeerror_w(L"Tulostinta ei ole määritelty", 0);
		return;
		}
	tulprm.kohde = kohde;

   if (tulprm.kohde == L'N' || tulprm.kohde == L'M' || tulprm.kohde == L'I')
      status_on = 0;

   switch(tulprm.kohde) {
      case L'A' :
         autotulasetukset();
         return;
      case L'M' :
         muotoilu();
         return;
#if defined(TAULU_2)
      case L'V' :
         wselectopt(L"Näytä E)ro kärkeen, K)oko ajat", L"EK", &tauluval);
         break;
#endif
      case L'I' :
			tiedostovalinnat(&tulprm);
			tulprm.p_fld = fileflds;
			tulprm.tulmuot = filetulosmuot;
			break;
	  case L'P':
		  tulprm.p_fld = prtflds;
		  tulprm.tulmuot = tulosmuot;
		  break;
   }

#if N_PV > 1
   if (!k_pv || tulprm.kohde != L'A')
#endif
      {
      clrscr();
      kehys(1);
      prscr(&tulvalikko2);
      }

   if (valitsesisalto(oikaise, &tulprm))
	   return;

   // tulostettava : T: tulokset, O: osanottajat ilman tuloksia, V: väliajat, J: joukkuetulokset, Q: joukkuepisteet, Y: yhteenveto


   if (tulprm.tulostettava == L'T') 
	   tulprm.piste = 0;

#ifdef MERI5
	if (kilpparam.kilplaji == L'N' && tulprm.tulostettava != L'Y')
		meri3pisteet();
#endif
   if (tulprm.tulostettava == L'Y' ) {
      yhteenveto(&tulprm);
      }
#if !defined(AMPUSUUNN) && defined(EI_OLE)
   else if (tulprm.tulostettava == L'L') 
	   lehdisto();
#endif
#if defined(JOUKKUETUL)
   else if (tulprm.tulostettava == L'J') 
	   joukkuetul(&tulprm);
#endif
   else if (er != ESC) {
      if (tulprm.tulostettava == L'V' && !oikaise) {

  		  if (valitsevapiste(&tulprm))
			  return;

//		  if (tulprm.kohde == L'J') {
//			  tulprm.kohde = L'I';
//			  moniriviva = 1;
//			 }
		  }

      if (tulprm.kohde == L'H' && tulprm.tulostettava != L'E') {
         ch = htmlpitkat ? L'S' : L'L';
	     wselectopt(L"Tulostetaanko L)yhenne vai S)euranimikenttä", L"LS", &ch);
         htmlpitkat = ch == L'S';
         }

      if (tulprm.kohde == L'X') {

		  if (xmlvalinnat(&tulprm))
			  return;

	  }

      if (wcswcind(tulprm.kohde, L"IRJEHFSZ") >= 0) {
         if (avaa_tiedosto(&tulprm)) 
			 return;
#ifdef IOF
         if (tulprm.kohde == L'F')
            iofots();
#endif
         }
      if (tulprm.tulostettava == L'T')
         if (autofl && tulprm.kohde == L'P') {
			 ch = tulprm.nollaalaskuri ? L'K' : L'E';
			 wselectopt(L"Nollataanko tulostettujen "
				L"sarjojen taustatulostuslaskurit (K/E)", L"KE", &ch);
			tulprm.nollaalaskuri = ch == L'K';
			}
#if N_PV > 1

	  if (valitsetulpv(&tulprm, oikaise))
		  return;

#endif  // N_PV > 1

	  if (!oikaise) {
         if (tulprm.piiritulokset) 
			 tulprm.seuratulokset = true;

         if (tulprm.tulostettava == L'E') {
			 ch = L' ';
            wselectopt(L"H)yväksytyt, K)aikki vai P)arhaat", L"HKP", &ch);
            switch (ch) {
				case L'P':
					tulprm.rajaus = L'T';
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Montako parasta :");
					INPUTINTW(&tulprm.viimsija, 3, 18, ySize-3, L"\r", &ch);
					break;
				case L'K':
					tulprm.rajaus = L'L';
					break;
				case L'H':
					tulprm.rajaus = L'T';
					break;
				}
			}
         else
         if (tulprm.kohde != L'S') {
            if (tulostettavat(&tulprm) == ESC)
				goto loppu;
			}
         else {
            tulprm.tulostettava = L'S';
            }
         }
#ifdef MAKI
	if (k_pv == makipv) {
		if (makivalinnat(kohde, &ct))
			goto loppu;
		}
#endif
#if N_PV > 1
      if (wcswcind(tulprm.tulostettava, L"AE") >= 0) 
		  tulprm.yhttuljarj = false;
#endif
      lineno = 0;
      sv = 0;
      ss = L' ';
      srj = sarjaluku;
      vsrj = 0;
      enssrj = TRUE;
      for (is = 0; is < SEURALISTA; is++)
         seuranimi[is][0] = 0;
#ifdef SEURATUL
	  if (tulprm.seuratulokset || tulprm.piiritulokset) {
		
		if (seuratulval_pohja(&ss, &ssrj, &tulprm) == 2)
			goto loppu;
		is = -1;
		}
#endif

	do {
#ifdef SEURATUL
		if (tulprm.seuratulokset || tulprm.piiritulokset) {

			if (seuratulval_toist(ss, &is, &piiri, &tulprm))
				break;

			srj = -1;
			vsrj = sarjaluku-1;
#ifdef ORAVA
			if (ssrj != -1) {
				srj = ssrj - 1;
				vsrj = ssrj;
				}
			else
#endif
				enssrj = TRUE;

			if (tulprm.kohde == L'P') {
				if (lineno != 0) endpage(&tulprm);
				initpage(&tulprm, NULL);
				}
			lineno = 0;
			sv = 0;
			}
#endif
         if (tulprm.kohde == L'S') {
			ssltlsfl = 0;		 
#ifdef EMIT
            srj = -1;
            vsrj = sarjaluku-1;
				if (kilpparam.kilplaji == L'S') {
					ssltlsfl = 1;
					ssl_otsikot(k_pv, &tulprm);
					}
#else
//            lue_sr_nimet(1);
#endif
            }
// #if defined(UINTI) || defined(MERI5)
         if (tulera == -1)
            tulera = 1;
         else if (tulera > 0)
            tulera++;
//         uusisivu0 = uusisivu;
//         uusisivu = 0;
         do {
			if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && tulera) {
               clrln(ySize-3);
               vidspwmsg(ySize-3, 0, 7, 0, L"Erän numero :        Esc : poistu");
               INPUTINTW(&tulera,3,15,ySize-3,L"\r\x1b",&tc);
               if (tc == ESC) {
                  tulera = -1;
                  break;
                  }
               srj = -1;
               vsrj = sarjaluku-1;
               }
			else
				tulera = 0;
               // do-loop jatkuu
// #endif
//		if (uusisivu)
//			tulprm.optiot |= 0x80000000;
			 do {
				if( srj < vsrj ) {
					srj++;
					if (!srjtullkm(srj, &tulprm))
						continue;
					if (tulprm.kohde == L'P') {
						if (lineno == 0)
							initpage(&tulprm, NULL);
						}
					}
				else {
//#if defined(UINTI) || defined(MERI5)
					if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && tulera)
		              break;
//#endif
#ifdef SEURATUL
	               if (tulprm.seuratulokset || tulprm.piiritulokset) break;
#endif
				   if (tulprm.kohde == L'P' && lineno == 0) {
						if (inlistCS)
							LeaveCriticalSection(&print_CriticalSection);
						inlistCS = 0;
						}
				   if (wcswcind(tulprm.kohde, L"HIJRESFZ") >= 0 && !enssrj) {
					  tc = L' ';
#ifndef LANGSV_K
					  wselectopt(L"Pyydetyt sarjat kirjoitettu, kirjoitetaanko lisää (K/E)",
						 L"KE", &tc);
					  if (tc == L'E')
						 break;
					  }
				   clrln(ySize-3);
				   vidspwmsg(ySize-3,0,7,0,L"Ensimmäinen sarja :               "
					  L"<F2>:Alusta, <Esc>:Poistu");
#else // LANGSV_K
					  wselectopt(L"Pyydetyt sarjat kirjoitettu, kirjoitetaanko lisää (K/E)",
						 L"KE", &tc);
					  if (tc == L'E')
						 break;
					  }
				   clrln(ySize-3);
				   vidspwmsg(ySize-3,0,7,0,L"Första klassen    :               "
					  L"<F2>: Fr†n början, <Esc>: G† till huvudmeny");
#endif // LANGSV_K
				   sarjayhdfl = 0;
				   srj = luesarja(L"", &tc, true);
				   if( tc == ESC ) break;
						if( tc == L'+' ) vsrj = srj;
						else {
							clrln(ySize-3);
							vidspwmsg(ySize-3,0,7,0,L"Viimeinen sarja :               " 
								L"<F2>: Loppuun, <Esc>: Poistu päävalintaan");
							vsrj = luesarja((wchar_t *)Sarjat[srj].sarjanimi, &tc, true);
							clrln(ySize-1);
							if (tc == 202) {
								for (vsrj = sarjaluku + nsarjayhd - 1; Sarjat[vsrj].vakanttisarja && vsrj > 0; vsrj--) ;
								}
							if( tc == ESC ) break;
							}
						if (tulprm.kohde == L'P') {
							if (!inlistCS)
								EnterCriticalSection(&print_CriticalSection);
							inlistCS = 1;
							}
						if (!enssrj) {
							if (tulprm.kohde == L'P') {
								if (lineno == 0)
									initpage(&tulprm, NULL);
								}
							}
						else {
							if (tulprm.tulostettava != L'E') {
								if (tulprm.kohde == L'P') {
									initpage(&tulprm, NULL);
									if (paaots[0]) {
										if (tulprm.tulmuot.esirivit) {
											newline(&tulprm, tulprm.tulmuot.esirivit);
											lineno = tulprm.tulmuot.esirivit;
											}
										paaots_on(&tulprm);
										putfld(&tulprm, (wchar_t *)paaots,0,80,0,0);
										endline(&tulprm, -1);
										paaots_pois(&tulprm);
										potsfl = TRUE;
										lineno += 2;
										}
									}
								else if (tulprm.kohde == L'E') {
									if (paaots[0]) {
										tulprm.puts_f((wchar_t *)paaots);
										tulprm.puts_f(L"\n\n");
										}
									}
								}
							if (tulprm.kohde == L'H') {
								if (htmlrunko(&tulprm, tul_tied_nimi)) break;
								}
							enssrj = FALSE;
							}
						}
				if (srjtullkm(srj, &tulprm) || tulprm.ixjarj == 2 || tulprm.ixjarj == 12) {
				   if (tulprm.kohde == L'V') 
					   lineno = 0;
				   if (tulprm.kohde == L'H' && !tulprm.yksihtml) {
//					  wchar_t fsrjnimi[20];
					  htmlloppu(&tulprm);
					  closeprfile(tulprm.lstf);
//					  wcscpy(fsrjnimi, Sarjat[srj].sarjanimi);
//					  wcscat(fsrjnimi, L".html");
					  

					  if (avaa_tiedosto_nm(htmlSrjFName(tul_tied_nimi, srj), &tulprm)) continue;
					  htmlalku(0, 0, 1, &tulprm);
					  }
	               if (tulprm.tulostettava == L'E') {
		              emitvaliajat(&srj, &tulprm);
			          }
				   else
					  {
#ifdef MAKI
					  if (ct == L'J') {
						  tulostahyppyjarj();
						  srj = sarjaluku + 1;
						  break;
						  }
#endif
					  if (tulprm.kohde == L'P') {
							if (!inlistCS)
								EnterCriticalSection(&print_CriticalSection);
							inlistCS = 1;
							}

//						tulostettu = tulostasarja(&srj, cy == L'V' ? piste : 0, kohde, ct, cn, cd, cj,
//							day, np, aakk, &lineno, &sv, FALSE);
	                  tulostettu = tulostasarja(&srj, &tulprm, &lineno, &sv, FALSE);
#ifdef MAKI
					  if (tulprm.kohde == L'X') {
						  tulostettu += tulostasarja(&srj, -1, kohde, ct, cn, cd, cj,
							  day, np, aakk, &lineno, &sv, FALSE);
					  if (k_pv == makipv) {
						   tulostettu += tulostasarja(&srj, -2, kohde, ct, cn, cd, cj,
							   day, np, aakk, &lineno, &sv, FALSE);
							}
						 }
#endif
//					  if (tulprm.kohde == L'P' && uusisivu !tulprm.tarkastamattomat && tulprm.rajaus != L'A') {
					  if (tulprm.kohde == L'P' && !tulprm.tarkastamattomat && tulprm.rajaus != L'A') {
						  if (lineno != 0)
							  endpage(&tulprm);
						  lineno = 0;
						  }
					  if (tulprm.kohde == L'P' && lineno == 0 && tulostettu) {
						  if (tulostettu)
							  enddoc(&tulprm);
//						  else
//							  abortdoc();
						  tulostettu = 0;
						  }
					  }
						for (i = 0; i <= kilpparam.valuku + 1 + (k_pv != 0); i++) {
							if (yhdseur[i]) {
								free(yhdseur[i]);
								yhdseur[i] = 0;
								}
							}
				   }
#ifdef EMIT
//	            if (tulprm.kohde == L'S' && srj == sarjaluku-1) break;
#endif
		        } while (tc != L'+' && tc != ESC && !(tulprm.kohde == L'S' && srj == sarjaluku-1));
#ifdef SEURATUL
			 } while (srj <= sarjaluku && (seuranimi[0][0] || piiri));
#else
		 } while (0);
#endif
//#if defined(UINTI) || defined(MERI5)
         } while (tulera > 0); 
//#endif
      if (tulprm.kohde == L'P') {
         if (lineno != 0)  
			 endpage(&tulprm);
		 if (tulostettu)
			enddoc(&tulprm);
//		 else
//			 abortdoc();
	     tulostettu = 0;
		 if (inlistCS)
			LeaveCriticalSection(&print_CriticalSection);
		 inlistCS = 0;
         }
      }
   loppu:
   if (tulprm.kohde == L'H') htmlloppu(&tulprm);
#if defined(IOF) && !defined(WRKOODI)
   if (tulprm.kohde == L'F') writewline(L"E");
#endif
#ifdef XML
   if (tulprm.kohde == L'X') {
#ifdef XMLFIS
	   if (fiskoodi)
		   xmlfisloppu(&tulprm);
	   else
#endif
		   xmlloppu(&tulprm);
	   }
#endif
   if (wcswcind(tulprm.kohde, L"IRJEHSFXZ") >= 0) {
		closeprfile(tulprm.lstf);
      }
#ifdef SEURATUL
   if (seurat != NULL) {
      for (is = 0; is < MAXNSEURA; is++)
         if (seurat[is].seura) free(seurat[is].seura);
      free(seurat);
      seurat = NULL;
      }
#endif
#if defined(UINTI) || defined(MERI5)
//   uusisivu = uusisivu0;
#endif
//   list(kohde, tiedlaji, tulostettava, jarjestys, pisteval, rajaus, alaraja, ylaraja, sarjat, kohdenimi, pitkat);
}

