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
#include <process.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include "VDeclare.h"
#include "VMuotoilu.h"

#if defined(EMITLEIMAT) && defined(MAALI)
#define EMIT_1 1
#define EMITXML
#else
#define EMIT_1 0
#endif

#define LBUFL 20000

#define K_TIED
#define SEURALISTA 15

int tulostasarja(INT *srj, tulostusparamtp *tulprm, INT *lineno, INT *sv, INT autotl);
INT tulkhp(wchar_t *fnt, wchar_t *pap, wchar_t *as, wchar_t *msto, wchar_t *extra,
   INT *mlj, INT *mtasm, INT *mkoko, INT *mrivi);
void metsassa(tulostusparamtp *tulprm);
void ongelmat(tulostusparamtp *tulprm);
void yhteenveto(tulostusparamtp *tulprm);
int seur_rata(int sarja, tulostusparamtp *tulprm);
void kaikki_tiedostoon(tulostusparamtp *tulprm, int sarja, int ens);
void htmlloppu(tulostusparamtp *tulprm);
INT htmlrunko(tulostusparamtp *tulprm, wchar_t *baseFName);
INT avaa_tiedosto(tulostusparamtp *tulprm);
INT avaa_tiedosto_nm(wchar_t *fname, tulostusparamtp *tulprm);
void sulje_tiedosto(TextFl **tul_tied);
void paaots_on(tulostusparamtp *tulprm);
void paaots_pois(tulostusparamtp *tulprm);

#define PRKOODIPIT 50

extern wchar_t tul_tied_nimi[80];
extern wchar_t *hajrata;
extern INT mrivi[3];
extern wchar_t palk[MAXSARJALUKU+MAXYHD];

extern wchar_t wtitle[80];
extern wchar_t wheader[80];
extern int potsfl;

//#if OSNIMIL >= 40

static void clrln1(int ln)
   {
   char l[66];

   memset(l, ' ', 66);
   viwrrect(ln,0,ln,64,l,7,0,0);
   }

void putflddsp(wchar_t *str, INT pos, INT len, INT oik, INT r)
   {
	INT v, o, ll, alku = 0;

	ll = wcslen(str);
	if (ll > len) {
		if (oik)
			alku = ll - len;
		ll = len;
		}
	if (ll <= 0) 
		return;
   
	if (oik) {
		o = pos + len - 1;
		v = o - ll + 1;
		}
	else {
		v = pos;
		o = v + ll - 1;
		}
	if (r > 0) {
		if (o > 79)
			o = 79;
		if (o < v)
			return;
		viwrrectw(r,v,r,o,str+alku,7,0,0);
		}
   }

static int fcomp(char *s1, char *s2)
   {
   while (*s1 && *s1 == *s2) {
      s1++;
      s2++;
      }
   return(*s1-*s2);
   }

#ifdef EI_OLE
static void lehdisto(void)
   {
   char *ptr, as[20], ch;
   int np,l,srj,os,osv,i,k,lgt,sj,sjy,nht,nkt;
   int p;
   kilptietue kilp;
   char *rivptr[500];
   static char cr;
   static int vaihda = FALSE;
   int enssarja = 1;
   TULOS tls,tulos0;
   char *buf;

   if ((buf = (char *) malloc(LBUFL)) == 0) {
      writeerror("Muisti ei riit‰",0);
      return;
      }
   header=L"LEHDIST÷TULOSTEN EDITOINTI";
   ch = vaihda ? 'K' : 'E';
   selectopt("Vaihdetaanko etu- ja sukunimien j‰rjestys (K/E)","KE",&ch);
   vaihda = (ch == 'K');
   srj = 0;
   np = 5;
   do {
      scpclr();
      kehys(0);
      do {
         if (sarjaluku > 1) {
            vidspmsg(ySize-3,0,7,0,"       Anna sarja :             " 
               "  <Esc> : Poistu p‰‰valintaan");
            srj = luesarja("", &tc);
            if (tc == ESC) goto escape;
            }
         else srj = 0;
         clrln(ySize-3);
         osv = Sarjat[srj].osuusluku - 1;
         vidspmsg(ySize-3,0,7,0,"Montako parasta :");
		 INPUTINT(&np,4,18,ySize-3,"\x1B\xD",&tc);
         clrln(ySize-3);
         if (tc == ESC) goto escape;
         np = min(np, ntulos[srj][osv][0]);
         if( np <= 0 ) {
            writeerror("Ei tulostettavia, tyhj‰ sarja ?",0);
            if (sarjaluku == 1) goto escape;
            }
         } while (np <= 0);
      EnterCriticalSection(&tall_CriticalSection);
	  ossijat(srj,-1,0, NULL, &nht, &nkt);
	  if (enssarja) {
         ptr = (char *) buf;
         l = 0;
         if ( strlen(kilpailu) > 0) {
			ptr = stpcpy(ptr, kilpailu);
            *(ptr++) = '|';
            *(ptr++) = '|';
            l += 2;
            }
         }
      vidspmsg(ySize-3,0,7,0," + : Hyv‰ksy,   <Esc> : Peruuta sarja,   "
         "<Ret.> : Uusi rivi (ruudulla |)");
      clrln(ySize-2);
      vidspmsg(ySize-2,0,7,0,"\x1A  \x1B      Ctrl-\x1A  Ctrl-\x1B  Home  "
         "End : liiku,   Ctrl-End : poista loput");
      ptr = stpcpy(ptr, (char *)Sarjat[srj].sarjanimi);
      *(ptr++) = ' ';
      *(ptr++) = '(';
      ptr = stpcpy(ptr, (char *)Sarjat[srj].matka[0]);
      for (os = 1; os <= osv; os++) {
         *(ptr++) = '+';
         ptr = stpcpy(ptr, (char *)Sarjat[srj].matka[os]);
         }
      ptr = stpcpy(ptr, " km): ");
	  p = jalku[srj][osv][0];
      tulos0 = 0;
      for (i = 0; i < np && ptr - (char *) buf < LBUFL - 100; i++) {
		 if (p < 0)
			break;
         GETREC(&kilp, p);
         p = JarjSeur(0, osv, 0, p);
         tls = tulos(&kilp, osv, 0);
         if (tls != tulos0) sjy = i + 1;
         tulos0 = tls;
         _itoa(sjy,ptr,10);
         ptr++;
         if (sjy > 9) ptr++;
         *(ptr++) = ')';
         *(ptr++) = ' ';
		 kilp.Seura(ptr);
         elimbl(ptr);
         ptr += strlen(ptr);
         if (kilp.joukkue) {
            *(ptr++) = '-';
            if (kilp.joukkue < 10)
               *(ptr++) = '0' + kilp.joukkue;
            else {
               *(ptr++) = '0' + kilp.joukkue/10;
               *(ptr++) = '0' + kilp.joukkue%10;
               }
            }
		 *(ptr++) = ',';
		 *(ptr++) = ' ';
         AIKATOSTRS(as, tls,0);
         as[kilpparam.laika] = 0;
         elimz(as);
		 elimbl(as);
         ptr = stpcpy(ptr, as);
         *(ptr++) = ' ';
         *(ptr++) = '(';
         for (os = 0; os <= osv; os++) {
            elimbl(kilp.ostiet[os].nimi);
            locasename(kilp.ostiet[os].nimi);
            if (vaihda) k = stschind(' ',kilp.ostiet[os].nimi);
            else k = -1;
            ptr = stpcpy(ptr, kilp.ostiet[os].nimi + k + 1);
            if (k > 0) {
               *(ptr++) = ' ';
               memcpy(ptr, kilp.ostiet[os].nimi, k);
               ptr += k;
               }
            *(ptr++) = ',';
            *(ptr++) = ' ';
            sj = kilp.Sija(os, 0); 
            _itoa(sj,ptr,10);
            if (sj >= 1000) ptr++;
            if (sj >= 100) ptr++;
            if (sj >= 10) ptr++;
            ptr++;
            *(ptr++) = '.';
            *(ptr++) = '/';
            AIKATOSTRS(as,tulos(&kilp,os, 0),0);
            as[kilpparam.laika] = 0;
            elimz(as);
            elimbl(as);
            ptr = stpcpy(ptr, as);
            *(ptr++) = ',';
            *(ptr++) = ' ';
            }
         ptr--;
         ptr--;
         *(ptr++) = ')';
         if (i < np-1) {
            *(ptr++) = ',';
            *(ptr++) = ' ';
            }
         else *(ptr++) = '.';
         *(ptr++) = ' ';
         }
      *(ptr++) = 0;
	  LeaveCriticalSection(&tall_CriticalSection);
	  pageedit((char *) buf,LBUFL,2,10,20,70,"+\x1B", &ch);
      lgt = strlen((char *) buf);
      if (ch == '+') {
         clrln(ySize-2);
		 if (enssarja) {
            cr += '1';
            selectopt("Tulostuksen riviv‰li 1) vai 2)", "12", &cr);
            cr -= '1';
            clrln(ySize-3);
            enssarja = 0;
            initpage(0,0);
            }
         else if (l > 0) {
            if (l > sivpit - 8) {
               endpage();
               initpage(0,0);
               l = 0;
               }
            else {
               newline(2);
               l++;
               l++;
               }
            }
         for (k = 0; (rivptr[k] - (char *) buf) < lgt; k++) {
            rivptr[k+1][-1] = 0;
            sendline("      ",lstf);
            initline(0);
            putfld(rivptr[k],prtflds[1].pos,80,0,0);
            endline(0);
            l++;
            if (l >= sivpit) {
               endpage();
               initpage(0,0);
               l = 0;
               }
            else if (cr) {
               newline(1);
               l++;
               }
            }
         }
      ptr = (char *) buf;
      } while (sarjaluku > 1 && srj >= 0);
   escape:
   if (l > 0) endpage();
   free(buf);
   }
#endif // EI_OLE

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
		wselectopt(L"U)TF-8 vai I)SO-8858-1 merkistˆ", L"UI", &ch);
		tulprm->merkit = (ch == L'I' ? L'A' : L'U');
		}

	tulprm->lstf = openprfile(tul_tied_nimi, -1, TRUE, FALSE, (char *) &tulprm->merkit, 0);
   if (!tulprm->lstf) {
      writeerror_w(L"Virhe tiedostoa avattaessa", 0);
      return(1);
      }
   ii = -1;
   for (i = 0; tul_tied_nimi[i]; i++)
      if (tul_tied_nimi[i] == L':' || tul_tied_nimi[i] == L'\\') ii = i;
   memset(tulprm->sline, 0, sizeof(tulprm->sline));
   wmemset(tulprm->sline, L' ', 120);
   return(0);
   }

static int tulostettavat(tulostusparamtp *tulprm)
{
	wchar_t cn;
	extern scr tulostettava;
	wchar_t ch;

	scpclr();
	kehys(1);
	prscr(&tulostettava);
	cn = tulprm->rajaus;
	if (tulprm->tulostettava == L'H') {
		tulprm->piiritulokset = false;
		tulprm->seuratulokset = false;
		tulprm->hajontatulokset = false;
		tulprm->tarkastamattomat = false;
		if (tulprm->rajaus != L'P') tulprm->rajaus = L'L';
		wselectopt(L"L)opettaneet, P)arhaat", L"LP",&tulprm->rajaus);
		}
	else {
		if (tulprm->piiritulokset || tulprm->seuratulokset || tulprm->hajontatulokset) 
			ch = L'V';
		else
			ch = tulprm->rajaus;
		wselectopt(L"I)lmoitt., L)opett., H)yv‰ks.,"
			L" P)arh., hY)l., K)esk., E)il., A)v., V)alitut", L"ILHPYKEAV-",&ch);
#ifdef LUENTA
		if (ch == L'A') {
			ch = L'P';
			wselectopt(L"P)uuttuvat koodit vai A)voimet tulokset", L"AP", &ch);
			if (ch == L'P') 
			ch = L'-';
			}
#endif
		if (ch == L'V') {
			cn = L' ';
			if (tulprm->piiritulokset) 
				cn = L'P';
			if (tulprm->seuratulokset) 
				cn = L'S';
			if (tulprm->hajontatulokset) 
				cn = L'R';
			wselectopt(L"Y)hteisl‰hdot, T)arkastamatta, S)euroittaiset, P)iireitt‰iset"
				, L"YTSP", &cn);
//				L", R)atakohtaiset", L"YTSPR", &cn);
			switch (cn) {
				case L'Y':
					tulprm->rajaus = L'X';
					break;
				case L'T':
					tulprm->rajaus = L'-';
					break;
				}
			tulprm->piiritulokset = cn == L'P';
			tulprm->seuratulokset = cn == L'S';
			tulprm->hajontatulokset = cn == L'R';
			tulprm->tarkastamattomat = cn == L'T';
			}
		else {
			tulprm->rajaus = ch;
			tulprm->piiritulokset = false;
			tulprm->seuratulokset = false;
			tulprm->hajontatulokset = false;
			tulprm->tarkastamattomat = false;
			}
		}
	if (tulprm->rajaus == L'P' || tulprm->hajontatulokset) {
		clrln(ySize-3);
		vidspmsg(ySize-3,0,7,0,"Alkaen sijasta :");
		INPUTINTW(&tulprm->enssija,4,17,ySize-3,L"\xD\x26",&cn);
		if (tulprm->enssija < 1)
			tulprm->enssija = 1;
		vidspmsg(ySize-3,24,7,0,"P‰‰ttyen sijaan :");
		INPUTINTW(&tulprm->viimsija,4,42,ySize-3,L"\xD\x26",&cn);
		if (tulprm->viimsija < tulprm->enssija) 
			erbeep();
		}
	return(0);
}

void tulaset_show(void)
   {
   char st[14];
   char sl[6];

   sl[0] = laajuus;
   sl[1] = 0;
   if (laajuus == 'P') {
      _itoa(nparas, st, 10);
      strcat(sl ,st);
      }
   sprintf(st, "Laajuus %5s", sl);    
//   viwrrect(0,65,ySize-4,79,kehys1,7,0,0);
   viwrrect(12,67,12,79,"Auto       ON",7,0,0);
   viwrrect(13,67,13,79,st,7,0,0);
   sprintf(st, "Kynnys %6d", kynnys);
   viwrrect(14,67,14,79,st,7,0,0);
   viwrrect(15,67,15,73,"Korkein",7,0,0);
//   virdrect(0,65,ySize-4,79,kehys1,0);
   }

#ifdef AUTOFILE
static int afsrj, afosuus, afva, afnparas = 30;

static void afasetukset(void)
   {
   wchar_t ch;
   int vali;

   if (!autofileparam.afvali) autofileparam.afvali = 60 * SEK;
   wselectopt(L"Tiedostomuoto: H)TML, X)ML", L"HX", &aftulparam.kohde);
   if (aftulparam.kohde == L'H')
      startaf(1);
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
   afosuus++;
   vidspmsg(ySize-3,0,7,0,"Osuus     V‰liaika     (0 : osuuden loppu)");
   INPUTINTW(&afosuus, 1, 6, ySize-3, L"\r", &ch);
   afosuus--;
#ifndef LANGSV_K
   vidspwmsg(ySize-3,0,7,0,L"Tulostusv‰li       sek");
#else // LANGSV_K
   vidspwmsg(ySize-3,0,7,0,L"Tulostusv‰li       sek");
#endif // LANGSV_K
   vali = autofileparam.afvali / SEK;
   INPUTINTW(&vali, 4, 13, ySize-3, L"\r", &ch);
   autofileparam.afvali = SEK * vali;
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
   wselectopt(L"K)‰ynnist‰, L)opeta automaattinen tulostus",L"KL",&co);
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
		  writeerror_w(L"Kirjoitinta ei m‰‰ritelty", 0);
		  return;
		  }
	  autotulostusparam.kohde = L'P';
	  autotulostusparam.nollaalaskuri = true;
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
      vidspwmsg(ySize-5,0,7,0,L"Anna tulostettavat maalinumerot yhten‰ numerosarjana :");
      inputwstr(maalistr, 9, 55, ySize-5,L"\r", &co, TRUE);
      for (p = maalistr; *p; p++)
         if (*p > L'0' && *p <= L'9') autotulostusparam.automaali[*p - L'1'] = 1;
      tulaset_show();
      autofl = TRUE;
      }
   }

static void editkoodi(wchar_t *s, wchar_t *nimi)
   {
   wchar_t ch,*p,ss[58],k;
   INT i,len,nml,el;

   clrln(ySize-3);
   nml = wcslen(nimi);
   viwrrectw(ySize-3,0,ySize-3,nml-1,nimi,7,0,0);
   vidspwmsg(ySize-1,0,7,0,L"Ascii-arvot huutomerkkien v‰liin "
      L"(Esim. ESC: !27!), Ei v‰lilyˆntej‰");
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
#ifdef EMITLEIMAT
   if (initfont_emva[0])
      tulkhp(initfont_emva, NULL, vaas, NULL, NULL, &mlj[3],
         &mtasm[3], &mkoko[3], &mrivi[3]);
#endif
   vidspwmsg(4,0,7,0,L"Mahdollinen lis‰initialisointi (ESC = \x1b) :");
   vidspwmsg(6,10,7,0,L"Merkkilaji  t‰smennys  koko (pistett‰)   riviv‰li (1/48 tuumaa)");
   viwrrectw(7,0,9,9,L"perus     "
                     L"p‰‰otsikko"
                     L"alaotsikot" , 7, 0, 0);
#ifdef EMITLEIMAT
   vidspwmsg(10,0,7,0,L"emitv‰liajat");
   vidspwmsg(10,62,7,0,L"Asento");
   vidspwmsg(10,70,7,0,vaas);
#endif
   vidspwmsg(12,0,7,0, L"Merkkilajeja. 4099: Courier, 4101: Times, 4113: Omega, "
      L"4148: Univers");
   vidspwmsg(13,0,7,0, L"Merkkilajit‰smennys: 1: lihav., 2: italic, "
      L"3: lihav. italic, 8: kapea");
   vidspwmsg(15,0,7,0,
      L"Siirry:  Tab Shift-Tab   Hyv‰ksy muutokset : +  Peruuta : Esc"); 
   i = 0;
   j = 0;
   do {
      swprintf(st, L"Paperikoko %2.2s   asento (P/V) %1.1s  merkistˆ "
         L"(PC-8/Roman-8) %1.1s", pap, as, msto);
      vidspwmsg(3,0,7,0, st);
      clrtxt(4, 43, 63);
      if (extra[0]) vidspwmsg(4,43,7,0,extra+1);
      for (k = 0; k < 3+EMIT_1; k++) {
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
            editkoodi(extra, L"Lis‰initialisointi");
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
            j = (j+1) % (4+EMIT_1);
            break;
         case BACKTAB:
            if (i < 4) i--;
            j = (j+3+EMIT_1) % (4+EMIT_1);
            break;
         }
         if (i != 7 && j == 4) j = 3;
         if (i == 7 && j == 0) j = 1;
         if (i < 0) i = 6+EMIT_1;
         if (i > 6+EMIT_1) {
            i = 0;
            j = 0;
            }
      } while (ch != ESC && ch != L'+');
   if (ch == L'+') {
//      suhtfont = mlj[0] = 4099 ? 0 : 1;
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
#ifdef EMITLEIMAT
      swprintf(initfont_emva+1,
         L"\x1b&l%sA\x1b&l%cO\x1b(%sU\x1b(s0p%dh%1.1ds%1.1db4099T\x1b&l%1.1dC",
         pap[1] == L'3' ? L"27" : L"26", vaas[0] == L'P' ? L'0' : L'1',
         msto[0] == L'P' ? L"10" : L"8", 120/mkoko[3], mtasm[3] / 2,
         3*(mtasm[3] & 1), mrivi[3]);
      initfont_emva[0] = (wchar_t) wcslen(initfont_emva+1);
#endif
      }
   }

static void GDIfontit(void)
   {
   char ch, st[80];
   INT i,j,k,nr = 4,mtasm;
   char term[] = "+\r\x1b\x5\x9\xb\x18";
	GDIfontTp fnt[4];

   clrscr();
   kehys(0);
	fnt[0] = DefFnt;
	fnt[1] = BoldFnt;
	fnt[2] = SmallFnt;
	fnt[3] = CourFnt;
   vidspmsg(6,10,7,0,"Merkkilaji  t‰smennys  koko (pistett‰)   riviv‰li (1/72 tuumaa)");
   viwrrect(7,0,10,9, "perus     "
					 "lihavoitu "
					 "pieni     "
							"vakiolev. ", 7, 0, 0);
   vidspmsg(12,0,7,0, "Merkkilajeja. 0: CourierNew, 1: TimesNewRoman, 2: Arial, ");
   vidspmsg(13,0,7,0, "Merkkilajit‰smennys: 1: lihav., 2: italic, "
	  "3: lihav. italic");
   vidspmsg(15,0,7,0,
	  "Siirry: .. Tab Shift-Tab   Hyv‰ksy muutokset : +  Peruuta : Esc");
   i = 0;
   j = 0;
   for (k = 0; k < nr; k++) {
		if (fnt[k].fontfamily == 0)
			fnt[k].size = 120/fnt[k].pitch;
		}
   do {
	  clrtxt(4, 43, 63);
	  for (k = 0; k < nr; k++) {
		 sprintf(st, "%4d %9d %13d %15d", fnt[k].fontfamily,
				fnt[k].bold+fnt[k].italic, fnt[k].size, fnt[k].line);
		 vidspmsg(7+k,14,7,0, st);
		 }
	  switch (j) {
		 case 0:
			INPUTINT(&fnt[i].fontfamily, 1, 17, i+7, term, &ch);
			break;
		 case 1:
				mtasm = fnt[i].bold+fnt[i].italic;
			INPUTINT(&mtasm, 1, 27, i+7, term, &ch);
				fnt[i].bold = mtasm & 1;
				fnt[i].italic = mtasm & 2;
			break;
		 case 2:
			INPUTINT(&fnt[i].size, 2, 40, i+7, term, &ch);
			break;
		 case 3:
			INPUTINT(&fnt[i].line, 2, 56, i+7, term, &ch);
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
	  } while (ch != ESC && ch != '+');
   if (ch == '+') {
	  for (k = 0; k < nr; k++) {
			if (fnt[k].fontfamily == 0) {
				fnt[k].pitch = 120/fnt[k].size;
				fnt[k].size = 0;
				}
			else
				fnt[k].pitch = 0;
			}
		DefFnt = fnt[0];
		BoldFnt = fnt[1];
		SmallFnt = fnt[2];
		CourFnt = fnt[3];
	  }
   }

int luemuotoilu(int kysy)
   {
   char ch;
   wchar_t wch;
   FILE *luetfmtf = NULL;
   int ret = 0;

   if (kysy) {
	  ch = ' ';
	  selectopt("Luetaanko muotoilu levylt‰ (K/E)", "KE", &ch);
	  clrln(ySize-3);
	  if (ch == 'K') {
		 vidspmsg(ySize-3,0,7,0,"Tiedoston nimi:");
		 for (;;) {
			inputwstr(luetfnm, 39, 16, ySize-3, L"\r\x1b", &wch, FALSE);
			clrln(ySize-1);
			if (ch == ESC) break;
			if ((luetfmtf = _wfopen(luetfnm,L"rt")) == NULL) {
			   erbeep();
			   vidspmsg(ySize-1,0,0,7,"EI L÷YDY!");
			   }
			else break;
			}
		 if (ch != ESC) {
			ch = ' ';
			selectopt("Muokataanko muotoilua (K/E)", "KE", &ch);
			if (ch == 'E') ret = 1;
			}
		 }
	  }
   else {
	  if ((luetfmtf = _wfopen(luetfnm,L"rt")) == NULL) {
		 erbeep();
		 writeerror("Tulostusmuotoilutiedostoa ei lˆydy!",0);
		 return(0);
		 }
	  }
   clrln(ySize-1);
   if (luetfmtf) {
	   fclose(luetfmtf);
	   lue_muotoilu(luetfnm, 0);
	   }
   else
	   lue_muotoilu(luetfnm, 1);
   return(0);
   }

static void muotoilu(void)
   {
   char ch, s[PRKOODIPIT], sline[200];
   wchar_t wch;
   int i,j;
   FILE *luetfmtf = NULL;
   FldFrmtTp *fld;
//   char av;

   clrscr();
   kehys(0);
   if (luemuotoilu(1)) return;
#ifdef WIN32
	if (lsttulparam.printer == GDIPRINTER) {
		vidspmsg(12,0,7,0,"K‰ytˆss‰ Windowsin kirjoitinajurit. Kohdistus rivin tuhannesosina");
		}
	else
#endif
		{
		viwrrect(12,0,12,34,"Valitse sarakkeiden kohdistustapa :",7,0,0);
		viwrrect(13,4,17,51,
			"1 : Merkkipohjainen kohdistus                   "
			"2 : Matriisikirjoitintyyppinen tabulaattori     "
			"3 : HP Laserjet kohdistus yksikˆiss‰ 1/72 tuumaa"
			"4 : Postscript kohdistus yksikˆiss‰ 1/72 tuumaa "
			"5 : Proprinteremulointi, yksikˆiss‰ 1 merkki    "
			,7,0,0);
	//      "6 : Canon laser kohdistus yksikˆiss‰ 1/72 tuumaa"
		ch = lsttulparam.printer + '1';
		if (lsttulparam.printer == 1 && !tulosmuot.tabs) ch = '1';
		selectopt("Valitse kohdistustapa 1, 2, 3, 4 tai 5", "123456", &ch);
		for (i = 12; i < ySize-6; i++) clrtxt(i, 0, 51);
		switch (ch) {
			case '1':
				lsttulparam.printer = EPSON;
				tulosmuot.tabs = 0;
				break;
			case '2':
				lsttulparam.printer = EPSON;
				tulosmuot.tabs = 1;
				break;
			case '3':
				lsttulparam.printer = LJETII;
				tulosmuot.tabs = 0;
				break;
			case '4':
				lsttulparam.printer = POSTSCRIPT;
				tulosmuot.tabs = 0;
				break;
			case '5':
				lsttulparam.printer = PROPRIEMU;
				tulosmuot.tabs = 0;
				break;
			case '6':
				lsttulparam.printer = CANON;
				tulosmuot.tabs = 0;
				break;
			}
		}
   viwrrect(2,0,2,21,"Sivun pituus      rivi‰",7,0,0);
   vidint(2,13,4,tulosmuot.sivpit);
   viwrrect(3,0,4,64,
      "P‰‰otsikko tulostetaan kerran erikseen pyydett‰v‰n tulosluettelon"
      "alussa.  Toistettava otsikko tulostetaan jokaisen sarjan alussa. ",
      7,0,0);
   viwrrect(6,0,6,12,"P‰‰otsikko :",7,0,0);
   vidspwmsg(7,0,7,0,paaots);
   viwrrect(9,0,9,21,"Toistettava otsikko :",7,0,0);
   vidspwmsg(10,0,7,0,kilpailu);
   INPUTINT(&tulosmuot.sivpit,4,13,2,"\r",&ch);
   inputwstr(paaots,80,0,7,L"\r",&wch,FALSE);
   inputwstr(kilpailu,80,0,10,L"\r",&wch,FALSE);
   ch = tulosmuot.uusisivu ? 'K' : 'E';
   selectopt("Tulostetaanko jokainen sarja uudelle sivulle (K/E)", "KE", &ch);
   tulosmuot.uusisivu = (ch == 'K');
   ch = tulosmuot.tpvafl ? 'K' : 'E';
   selectopt("Tulostetaanko otsikkotiedoissa tulostusp‰iv‰ (K/E)", "KE", &ch);
   tulosmuot.tpvafl = (ch == 'K');
   ch = tulosmuot.tklofl ? 'K' : 'E';
   selectopt("Tulostetaanko otsikkotiedoissa kellonaika (K/E)", "KE", &ch);
   tulosmuot.tklofl = (ch == 'K');
   //av = '0' + tulosmuot.aikapari;
   //selectopt("1: aika ja os.-aika, 2: aika ja ero k‰rkeen, 3: ero ja "
   //   "os.-aika", "123", &av);
   //tulosmuot.aikapari = av - '0';
   ch = tulosmuot.knofl ? 'K' : 'E';
   selectopt("Tulostetaanko kilpailunumerot valinnassa Kaikki (K/E)", "KE",
	  &ch);
   tulosmuot.knofl = (ch == 'K');
   ch = ' ';
   selectopt("Muutetaanko fonttien tai tiiviin lopputuloslistauksen m‰‰rittelyj‰ (K/E)",
      "KE", &ch);
   if (ch == 'K') {
      clrln(ySize-3);
      vidspmsg(ySize-3,0,7,0,"Anna tiiviin osuustietorivin pituus");
      INPUTINT(&tulosmuot.tiiv_len, 3, 36, ySize-3, "\r", &ch);
      ch = ' ';
#ifdef WIN32
		if (lsttulparam.printer == GDIPRINTER) {
			GDIfontit();
			}
		else
#endif
      if (lsttulparam.printer == LJETII) {
         selectopt("Asetetaanko HP PCL:N vakio-oletuskoodit (K/E)", "KE", &ch);
         if (ch == 'K') {
            init_fnt[0] = 0;
            lih_on[0] = 0;
            pienet_on[0] = 0;
            }
         hpkoodit();
         }
      else {
         editkoodi(init_fnt,L"Perusfontin valinta");
         editkoodi(lih_on,L"Joukkuerivi, alku");
         editkoodi(lih_pois,L"Joukkuerivi, loppu");
         editkoodi(pienet_on,L"Osuusrivit, alku");
         editkoodi(pienet_pois,L"Osuusrivit, loppu");
         }
      }
   clrln(ySize-3);
   fld = prtflds;
   for (i = 2; i < ySize-4; i++) clrln(i);
   vidspmsg(2,0,7,0,"Anna osuuksittaisen paperitulostuksen kenttien "
      "sijainnit");
#ifdef WIN32
	if (lsttulparam.printer == GDIPRINTER)
		strcpy(sline,"Sijainti 1/1000 rivist‰, leveys merkkein‰");
	else
#endif
	   sprintf(sline,"Sijainti %s, leveys merkkein‰",
		   (lsttulparam.printer == LJETII || lsttulparam.printer == POSTSCRIPT) ? "yksikˆiss‰ 1/72 in" :
			"merkkein‰");
   vidspmsg(18,0,7,0, sline);
   vidspmsg(4,3,7,0,"Kentt‰     Sijainti Leveys Tasaus");
   for (i = 0; i < 3; i++) {
      for (j = 0; j < 15; j++) {
         if (j < n_fld[i]) {
            sprintf(sline, "%-10.10s   %3d    %3d     %3.3s",
               fldname[j], fld[j].pos, fld[j].len,
               fld[j].oik ? "Oik" : "Vas");
			vidspmsg(j+5, 3, 7, 0, sline);
            }
         else clrln(j+5);
         }
      for (j = 0; j < n_fld[i]; j++) {
         INPUTINT(&fld[j].pos, 3, 16, j+5, "\r\x1b", &ch);
         if (ch == ESC) break;
         INPUTINT((int *)&fld[j].len, 3, 23, j+5, "\r\x1b", &ch);
         if (ch == ESC) break;
         strcpy(s, fld[j].oik ? "Oik" : "Vas");
         inputstr(s, 3, 31, j+5, "\r\x1b", &ch, 0);
         fld[j].oik = (s[0] == 'O' || s[0] == 'o');
         }
      if (ch == ESC) break;
      if (i == 0) {
         vidspmsg(2,0,7,0,"Anna osuuksittaisen n‰yttˆtulostuksen"
            " kenttien sijainnit");
         fld = dspflds;
         clrln(ySize-5);
			}
	  if (i == 1) {
		 vidspmsg(2,0,7,0,"Anna osuuksittaisen htmltulostuksen"
			" kenttien sijainnit");
		 fld = fileflds;
		 clrln(ySize-5);
		 }
	  }
   lsttulparam.hksijafl = (prtflds[F_OSASJ].len != 0);
   clrln(ySize-3);
   if (lsttulparam.printer == LJETII || lsttulparam.printer == POSTSCRIPT)
	  if (tulosmuot.numlev < 5) tulosmuot.numlev = 6;
   vidspmsg(ySize-3,0,7,0,"Anna numeromerkin leveys");
   INPUTINT(&tulosmuot.numlev, 3, 26, ySize-3, "\r", &ch);
   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,"Anna vasen marginaali");
   INPUTINT(&tulosmuot.marg, 3, 23, ySize-3, "\r", &ch);
   ch = ' ';
   selectopt("Tallennetaanko uusi muotoilu levylle (K/E)", "KE", &ch);
   if (ch == 'K') {
	  clrln(ySize-3);
	  vidspmsg(ySize-3,0,7,0,"Tiedoston nimi:");
	  inputwstr(luetfnm, 39, 16, ySize-3, L"\r\x1b", &wch, FALSE);
	  if (wch != ESC)
		  TallXMLMuotoilu(luetfnm);
	  }
   }

#ifdef HALIKKO

static void pisteet(int os, char co)
   {
   typedef struct {
      int piste;
      char seura[LSEURA+1];
      } pistetp;
   pistetp *pistetaul;
   char ch2, seura[LSEURA+1], line[40];
   jrec *p;
   int npiste = 60;
   int srj, sj, sj1, is, nseura = 0, l;
   TULOS tls1, tls;
   kilptietue kilp;
   FILE *srafl;

   clrln(ySize-3);
   pistetaul = (pistetp *) calloc(MAXNSEURA, sizeof(pistetp));
   if (!pistetaul) {
      writeerror("Muisti ei riit‰", 0);
      return;
      }
   srafl = fopen("PISTESRA.LST", "rt");
   if (srafl) {
      while (!feof(srafl)) {
         if (!fgets(line, 38, srafl)) break;
         if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = 0;
         elimbl(line);
         if (strlen(line) > 0) {
			strncpy(pistetaul[nseura].seura, line, LSEURA);
            nseura++;
            }
         }
      fclose(srafl);
      }
   for (srj = 0; srj < sarjaluku; srj++) {
      if (os > ntosuus[srj] - 1) continue;
      EnterCriticalSection(&tall_CriticalSection);
      p = jalku[srj][os][0]-1;
      sj = 1;
      tls1 = 0;
      while (1) {
         if (nseura >= MAXNSEURA-1) break;
         p++;
         if (p >= jalku[srj][os][0] + ntulos2[srj][os][0])
            break;
         GETREC(&kilp, p->dkilp);
		 if (tls1 != (tls = ttulos(&kilp, os, 0, 1))) {
            sj1 = sj;
            tls1 = tls;
            }
         if (sj1 > npiste) break;
         kilp.Seura(seura);
         for (is = 0; pistetaul[is].seura[0]; is++) {
            if (!strcmp(seura, pistetaul[is].seura)) break;
            }
         if (!pistetaul[is].seura[0] && !srafl) {
            strcpy(pistetaul[is].seura, seura);
            nseura++;
            }
         if (pistetaul[is].seura[0])
            pistetaul[is].piste += npiste + 1 - sj1;
         sj++;
         }
      LeaveCriticalSection(&tall_CriticalSection);
      }
   l = 0;
   if (nseura) {
	  for (is = 0; is < nseura; is++) {
		 _swab((char *) &pistetaul[is].piste,
			(char *) &pistetaul[is].piste, 2);
		 }
//      pushcmp();
//      setcmp(2,sizeof(pistetp));
      quicksort((void *) pistetaul, nseura, 2, sizeof(pistetp));
//      popcmp();
      if (co == 'P') {
         initpage(0,0);
         initline(0);
		 }
      npiste = 0;
      if (co == 'N') clearframe();
	  for (is = nseura-1; is >= 0 && pistetaul[is].piste; is--) {
         if (npiste != pistetaul[is].piste) {
            sj1 = nseura - is;
            npiste = pistetaul[is].piste;
            }
         if (co == 'P') {
            if (l >= sivpit) {
               l = 0;
               sendline(pscr ? "p\r\n\x04" : "\xC\r",lstf);
               }
            }
         else {
            if (l > 17) {
               vidspmsg(ySize-3,0,7,0,"Paina <Return> jatkaaksesi");
               do { } while (readkbd(&ch2,TRUE,spoolfl) != KB_CR);
			   clearframe();
               l = 0;
               }
            }
         if (co == 'P' && l == 0) {
            if (pscr) sendline("(",lstf);
            sendline("   Pistetilanne       ", lstf);
            if (os < kilpparam.ntosuus-1) {
               sprintf(line, "%d osuuden j‰lkeen  ", os+1);
               sendline(line,lstf);
               }
            sendline(kello(),lstf);
            sendline(pscr ? ") t0 v rv\r\n" : "\r\n",lstf);
            sendline("\r\n", lstf);
            sendline(pscr ? "rv\r\n" : "\n",lstf);
            l = 2;
            }
		 _swab((char *) &pistetaul[is].piste,
			(char *) &pistetaul[is].piste, 2);
         sprintf(line, "   %3d.  %4d   %s", sj1,
            pistetaul[is].piste, pistetaul[is].seura);
		 if (co == 'P') {
            sendline(line, lstf);
            sendline("\r\n", lstf);
            }
         else
            vidspmsg(l+3, 0, 7, 0, line);
         l++;
         }
      if (co == 'P')
         sendline(pscr ? "p\r\n\x04" : "\xC\r",lstf);
	  else {
         vidspmsg(ySize-3,0,7,0,"Paina <Return> jatkaaksesi");
         do { } while (readkbd(&ch2,TRUE,spoolfl) != KB_CR);
         }
      }
   free(pistetaul);
   }
#endif

int xmlvalinnat(tulostusparamtp *tulprm)
{
	wchar_t ch, xmlva;
	wchar_t xml_tied_nimi[60] = L"tulokset.xml";

#ifdef EMITXML
	if (emitfl || kilpparam.maxvaluku)
		wselectopt(L"Kirjoitetaanko E)mitv‰liajat, M)uut v‰liajat, I)lman v‰liaikoja", L"EMI", &xmlva);
	else
		xmlva = L'I';
#else
	if (kilpparam.maxvaluku)
		wselectopt(L"Kirjoitetaanko V)‰liajoin, I)lman v‰liaikoja", L"VI", &xmlva);
	else
		xmlva = L'I';
	wtransl(L"V", L"M", &xmlva);
#endif
    ch = L' ';
    wselectopt(L"U)TF-8 vai I)SO-8858-1 merkistˆ", L"UI", &ch);
    tulprm->merkit = (ch == L'I' ? L'A' : L'U');
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
		xmlots(0, tulprm);
		}
#endif
	return(0);
}


static void tiedostovalinnat(tulostusparamtp *tulostusparam)
	{
	wchar_t ch, opts[12], msg[90], co = L' ';

	if (emitfl) {
		wcscpy(msg, L"Tulokset T)eksti, C)SV, H)TML, X)ML, E)mit-v‰liajat (myˆs XML)");
		wcscpy(opts, L"TCHXE");
		}
	else {
		wcscpy(msg, L"T)eksti, C)SV, H)TML, X)ML, hI)ihtoliiton tulostiedosto");
		wcscpy(opts, L"TCHIX");
		}
	wselectopt(msg, opts, &co);

	tulostusparam->kohde = co != L'C' ? co : L'R';
	tulostusparam->emitva = co == L'E';

	switch (co) {
		case L'T':
            tulostusparam->kohde = L'I';
			break;
		case L'H':
			break;
		case L'X':
			break;
		case L'E':
			tulostusparam->kohde = L'X';
			break;
		case L'I':
            tulostusparam->kohde = L'S';
			tulostusparam->tulostettava = L'T';
			tulostusparam->viimos = L'K';
			tulostusparam->hksijafl = 1;
			clrln(ySize-3);
			vidspmsg(ySize-3, 0, 7, 0, "Anna kilpailun koodi");
			inputwstr(kilpparam.kilpkoodi,4,21,ySize-3,L"\r",&ch,TRUE);
			break;
		}

#ifndef LANGSV_K
//	wtransl(L"KELSIW", L"IRESFF", &co);
#else
//	wtransl(L"KETSIW", L"IRESFF", &co);
#endif
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
	}

int valitsekohde(wchar_t *co)
{
#if defined(TAULU_2)
   wchar_t msg[90];
   wchar_t opts[20];

      wcscpy(msg, L"N)‰yttˆ, P)aperi, A)utomaattinen, M)uotoilu, "
         L"tI)edostoon");
      wcscpy(opts, L"NPAMI\x1b");
      if (taulu_com) {
         wcscat(msg, L", V)alotaulu");
         wcscat(opts, L"V");
         }
      wselectopt(msg, opts, co);
#else
      if (spoolfl)
         wselectopt(L"N)‰ytˆlle, P)aperille, A)utomaattinen, M)uotoilu, "
            L"tI)edostoon", L"NPAMI\x1b",co);
      else {
#if defined(TCPSIIRTO)
		  if (aftulparam.kohde == L'X' && autofileparam.afvali == 0 && autofileparam.afname[0]) {
            wselectopt(L"N)‰ytˆlle, P)aperille, A)utomaattinen, M)uotoilu"
               L", tI)edostoon, L)‰het‰",L"NPAMIL\x1b",co);
            if (*co == L'L') {
               autofile();
			   *co = ESC;
               return(1);
               }
            }
         else
#endif
            wselectopt(L"N)‰ytˆlle, P)aperille, A)utomaattinen, M)uotoilu"
               L", tI)edostoon",L"NPAMI\x1b",co);
         }
#endif
	  return(*co == ESC);
}

int valitsesisalto(int oikaise, tulostusparamtp *tulprm)
{
	wchar_t cy = L'J';
	wchar_t msg[200], opts[20];

	if (oikaise)
		return(0);

	if (kilpparam.maxnosuus == 1) {
		wcscpy(msg, L"J)oukkueet, O)suus");
		wcscpy(opts, L"\x1BJO");
		if (valiajat) {
			wcscat(msg, L", V)‰liajat");
			wcscat(opts, L"V");
			}
		}
	else {
		if (tulprm->kohde != L'I') {
			wcscpy(msg, L"J)oukkueet, P)isteet");
			wcscpy(opts, L"\x1BJP");
			}
		else {
			wcscpy(msg, L"J)oukkueet");
			wcscpy(opts, L"\x1BJ");
			}
		}
	if (valiajat && tulprm->kohde != L'I') {
		wcscat(msg, L", V)‰liajat");
		wcscat(opts, L"V");
		}
	if (tulprm->kohde == L'N' || tulprm->kohde == L'P' || tulprm->kohde == L'I') {
		wcscat(msg, L", Y)hteenveto");
		wcscat(opts, L"Y");
		}
	if (emitfl && tulprm->kohde == L'I') {
		wcscat(msg, L", M)ets‰ss‰");
		wcscat(opts, L"M");
		}
	if (tulprm->kohde == L'I') {
		wcscat(msg, L", onG)elmat");
		wcscat(opts, L"G");
		}
	wcscat(msg, L", Esc: Peruuta");
	wselectopt(msg, opts, &cy);
	  switch (cy) {
		case ESC:
			return(ESC);
		case L'J':
			tulprm->tulostettava = L'T';
			break;
		case L'O':
			tulprm->tulostettava = L'H';
			break;
		case L'V':
			tulprm->tulostettava = L'V';
			break;
		case L'M':
			tulprm->tulostettava = L'M';
			break;
		case L'G':
			tulprm->tulostettava = L'G';
			break;
		case L'Y':
			tulprm->tulostettava = L'Y';
			break;
		}
	return(0);
}

int valitseosuudet(tulostusparamtp *tulprm)
{
	extern scr tulosuudet;
	wchar_t st[4] =L"", ch;
	int os;

	prscr(&tulosuudet);
	
	if (tulprm->viimos) {
		tulprm->osuus = -1;
		st[0] = L'0'; 
		}
	else
		_itow(tulprm->osuus+1, st, 10);

	vidspwmsg(ySize-3, 0, 7, 0, L"Anna osuus     (K)aikki, kaikki T)iiviisti, Y)hteisajat)");
	do {
		inputwstr(st, 3, 11, ySize-3, L"KTYkty\r", &ch, 0);
	   } while (ch == KB_CR && ((os = _wtoi(st)) < 1 || os >= kilpparam.osuusluku));
	clrln(ySize-3);
	ch = towupper(ch);
	switch (ch) {
		case ESC:
			return(ESC);
		case L'K':
			tulprm->viimos = L'K';
			tulprm->osuus = 0;
			break;
		case L'Y':
			tulprm->viimos = L'V';
			tulprm->osuus = 0;
			break;
		case L'T':
			tulprm->viimos = L'K';
			tulprm->osuus = 0;
			break;
		default:
			tulprm->viimos = 0;
			tulprm->osuus = _wtoi(st)-1;
		}
	tulprm->tiivis = ch == L'T';
	return(0);
}

void tulostus(int oikaise)
   {
	wchar_t kohde = L' ';
	static tulostusparamtp tulprm;

	static int os = 0;
	int lineno, srj;
   int  va=0, vsrj, sv, enssrj, ret = 0, inlistCS = 0;
   wchar_t ch, che, ss, cn;
   wchar_t tc;
	int tulostettu = 0;
   extern scr tulvalikko;
   extern scr tulvalikko2;

   tc = 0;
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
		tulprm.lahtoajat = false;
		tulprm.piilotasijat = false;
		}

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
		writeerror_w(L"Tulostinta ei ole m‰‰ritelty", 0);
		return;
		}

	tulprm.kohde = kohde;

	tulprm.tulmuot = tulprm.kohde == L'P' ? tulosmuot : filetulosmuot;


   if (tulprm.kohde == L'N' || tulprm.kohde == L'M' || tulprm.kohde == L'I')
      status_on = 0;


	switch(tulprm.kohde) {
		case L'A' :  
			autotulasetukset();
			return;
		case L'M' :
			muotoilu();
			return;
		case L'I' :  
			tiedostovalinnat(&tulprm);
			break;
		}

	clrscr();
	kehys(1);
	prscr(&tulvalikko2);

	if (valitsesisalto(oikaise, &tulprm))
		return;

	if (tulprm.kohde == L'N') {
		header = L"          ";
		status_on = 0;
		scpclr();
		kehys(0);
		}
	else 
		spoolstatus();
	if (tulprm.tulostettava == L'G') {
		ongelmat(&tulprm);
		return;
		}
	if (tulprm.tulostettava == L'M') {
		metsassa(&tulprm);
		return;
		}
	if (tulprm.tulostettava == L'Y' ) {
		if (wcswcind(tulprm.kohde, L"IRJEHFSZ") >= 0) {
			if (avaa_tiedosto(&tulprm)) 
			return;
			}
		yhteenveto(&tulprm);
		if (wcswcind(tulprm.kohde, L"IRJEHFSZ") >= 0) {
			closeprfile(tulprm.lstf);
			}
		return;
		}

    if (kilpparam.maxnosuus == 1 && tulprm.tulostettava == L'L') {
      status_on = 0;
//      lehdisto();
		return;
      }

	if (autofl && tulprm.tulostettava == L'T' && tulprm.kohde == L'P') {
		wselectopt(L"Nollataanko tulostettujen sarjojen taustatulostuslaskurit (K/E)", 
			L"KE", &cn);
		tulprm.nollaalaskuri = cn == 'K';
		}

	if (!oikaise) {
#if defined(HALIKKO)
		if (tulprm.tulostettava != 'P')
#endif
			tulostettavat(&tulprm);
		}
	if (tulprm.kohde != L'P') {
		switch (tulprm.kohde) {
			case L'X' :
				tulprm.ixjarj = 11;
				break;
			case L'S' :
			case L'I' :
				tulprm.ixjarj = 0;
				break;
			default:
				che = tulprm.ixjarj == 11 ? L'N' : L'T';
				wselectopt(L"N)umeroj‰rjestys vai T)ulosj‰rjestys", L"NT", &che);
				tulprm.ixjarj = (che == L'N' ? 11 : 0);
			}
		}
	if (tulprm.hajontatulokset) 
		tulprm.tulostettava = L'T';
	scpclr();
	kehys(1);
	if (!tulprm.seuratulokset) {
#if MAXOSUUSLUKU >= 970
		ch = ' ';
		selectopt("T)ilanne tai lopputulos, K)ierrostulokset, Y)ksi kierros", "TKY", &ch);
		if (ch == 'Y') {
			clrln(ySize-3);
			vidspmsg(ySize-3, 0, 7, 0, "Kierros");
			os = 1;
			INPUTINT(&os, 2, 10, ySize-3, "\r", &ch);
			os--;
			}
		else
			os = 100 + (ch == 'T');
		}
#else
		if (kilpparam.maxnosuus > 1 && tulprm.hajontatulokset) {
			tulprm.viimos = L'K';
			tulprm.osuus = -1;
			}
		else
			if (valitseosuudet(&tulprm) == ESC)
				return;

			if (tulprm.viimos == L'K')
				tulprm.hksijafl = true;

			if (emitfl && tulprm.viimos == L'K') {
				if (tulprm.seuratulokset || tulprm.piiritulokset || tulprm.kohde == L'X' || tulprm.kohde == L'I')
					tulprm.tulbadge = 0;
				else {
					if (tulprm.tulbadge) 
						che = L'E';
					else 
						che = tulprm.tulostettava == L'O' ? L'V' : L'T';
					wselectopt(L"Tulostetaanko E)mit-koodit, T)ulokset, V)ain nimet", L"ETV", &che);
					tulprm.tulbadge = che == L'E' ? 1 : 0;
					tulprm.tulostettava = che == L'T' ? L'T' : L'O';
					clrln(ySize-3);
					}
				}
         if (tulprm.tulostettava == L'V' &&  kilpparam.maxvaluku) {
			 clrln(ySize-3);
		    vidspwmsg(ySize-3, 0, 7, 0, L"Anna v‰liaikapisteen no     ");
			INPUTINTW(&va, 3, 24, ySize-3, L"\x1b\r", &ch);
            }
         else va = 0;
		 tulprm.piste = va;
         }
	else {
		tulprm.viimos = 1;
		tulprm.osuus = -1;
		}
#endif

	if (tulprm.tulostettava == L'H')
		tulprm.piste = kilpparam.valuku+1;

#if defined(HALIKKO)
      if (cy == 'P') {
         pisteet(os, co);
         goto loppu;
         }
#endif

	if (tulprm.kohde == L'X') {

		if (xmlvalinnat(&tulprm))
			return;

		}

	if (wcswcind(tulprm.kohde, L"IRJEHFSZ") >= 0) {
		if (avaa_tiedosto(&tulprm)) 
			return;
		}

	if (tulprm.kohde == L'I' && tulprm.viimos == L'K') {
		kaikki_tiedostoon(&tulprm, -1, 0);
		closeprfile(tulprm.lstf);
		return;
		}

	lineno = 0;
	sv = 0;
	srj = sarjaluku;
	vsrj = 0;
	enssrj = TRUE;
	ss = ' ';
/*
	  for (is = 0; is < SEURALISTA; is++)
         seuranimi[is][0] = 0;
      if (ct == 'S') {
         selectopt("Y)ksi, L)ista, K)aikki seurat,  <Esc> : Peruuta",
            "YLK\x1B", &ss);
         is = -1;
         switch (ss) {
            case ESC :
               goto loppu;
            case 'K' :
               seuraluettelo();
               if (!seurat) goto loppu;
            }
         }
      if (ct == 'D') {
         if (!initpiirit) {
            luepiirit();
            initpiirit = 1;
			}
         selectopt("V)alittavat vai K)aikki piirit,  <Esc> : Peruuta","VK\x1B",
            &ss);
         if (ss == ESC) goto loppu;
         }
      if ((ct == 'D' || ct == 'S') && !(co == 'I' && os == 100 && !html)) {
         clrln(ySize-3);
         vidspmsg(ySize-3,0,7,0,"Kuinka monta parasta tulostetaan, "
            "vaikka seura/piiri ei ole pyydetty");
         INPUTINT(&sj_raja, 2, 70, ySize-3, "\r", &ch);
         }
      else
         sj_raja = 0;
*/
	do {
/*
		  if (ct == 'S') {
            if ((ss == 'K') && seurat[++is])
               memmove(seuranimi[0], seurat[is], LSEURA+1);
            else if (ss == 'Y') haeseura(seuranimi[0]);
            else if (ss == 'L') seuralista();
            else seuranimi[0][0] = 0;
            seuranimi[0][LSEURA] = 0;
            if (!seuranimi[0][0]) break;
            clrln(ySize-3);
            strcpy(prs,"Tulostaa seuraa :");
            for (i = 0; seuranimi[i][0] && i < SEURALISTA; i++) {
               if (strlen(prs) > 65) break;
               strcat(prs, " ");
               strcat(prs, seuranimi[i]);
               vidspmsg(ySize-3,0,7,0,prs);
               }
            }
         if (ct == 'D') {
            piiri++;
            memset(piirifl, 0, sizeof(piirifl));
            if (ss == 'V') {
               clrln(ySize-3);
               for (r = 0; r < 19; r++) {
                  scclrmsg(r+2,0,63);
				  vidint(r+2,0,2,r+1);
                  vidspmsg(r+2,4,7,0,piirinimi[r]);
                  vidint(r+2,30,2,r+19);
                  vidspmsg(r+2,34,7,0,piirinimi[r+18]);
                  }
               vidspmsg(ySize-3,0,7,0,"Luettele piirien numerot ("
                  "Lopuksi 0)     Esc : keskeyt‰");
               r = 0;
               do {
                  INPUTINT(&piiri, 2, 37, ySize-3,"\r\x1B", &ch);
                  if (piiri > 0 && piiri < 39) {
                     piirifl[piiri-1] = 1;
                     r = piiri;
                     }
                  } while (ch != ESC && piiri);
               if (!r || ch == ESC) break;
               piiri = r;
               }
            else {
               piirifl[piiri-1] = 1;
               }
            if (piiri < 1 || piiri > piiriluku) {
               piiri = 0;
               break;
               }
            clrln(ySize-3);
            viwrrect(ySize-3,0,ySize-3,16,"Tulostaa piiri‰ :", 7, 0, 0);
            if (ss == 'K') vidspmsg(ySize-3,18,7,0,piirinimi[piiri-1]);
            }
         if (ct == 'S' || ct == 'D') {
            srj = -1;
            vsrj = sarjaluku-1;
            enssrj = TRUE;
            if( (co == 'P') && (lineno != 0) ) endpage();
            lineno = 0;
            sv = 0;
            }
*/
		do {
			char tc;

			if( srj < vsrj ) 
				srj++;
			else {
				if (tulprm.piiritulokset || tulprm.seuratulokset)
					break;
				if (sarjaluku > 1) {
					ch = L' ';
					if (tulprm.kohde == L'P' && tulprm.tulostettava == L'X') {
						wselectopt(L"Tulostetaanko sarjat yhdess‰ (K/E)", L"KE\x1b", &ch);
						}
					clrln(ySize-3);
					if (ch == L'K') {
						srj = -2;
						vsrj = -2;
						}
					else {
						vidspmsg(ySize-3,0,7,0,"Ensimm‰inen sarja :               "
						"<F2>:Alusta, <F4>:Yhdistelm‰, <Esc>:Poistu");
						srj = luesarja("", &tc);
						}
					if( tc == ESC ) break;
					}
				else {
					srj = 0;
					tc = 0;
					}
				if (tc == 204) {
					vsrj = srj;
					{
						if (srj < 0)
						break;
						}
					}
				else {
					if( tc == '+' ) 
						vsrj = srj;
					else if (srj >= 0) {
						if (sarjaluku > 1) {
							clrln(ySize-3);
							vidspmsg(ySize-3,0,7,0,"Viimeinen sarja :               " 
								"<F2>: Loppuun, <Esc>: Poistu p‰‰valintaan");
							vsrj = luesarja(Sarjat[srj].sarjanimi, &tc);
							if (tc == 202) 
								vsrj = sarjaluku - 1;
							if ((os == 100 || os == 102) && enssrj)
								lineno = Sarjat[srj].osuusluku + 1;
							}
						}
					}
				if (tulprm.kohde == L'P' && enssrj) {
					if (!inlistCS)
						EnterCriticalSection(&print_CriticalSection);
					inlistCS = 1;
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
				if (tulprm.kohde == L'H' && enssrj) {
					if (htmlrunko(&tulprm, tul_tied_nimi)) 
						break;
					}
				enssrj = FALSE;
				if( tc == ESC ) 
					break;
				}
			tulprm.hajrata.rata[0] = 0;
			do {
				if (tulprm.hajontatulokset) {
					if (seur_rata(srj, &tulprm))
						break;
					}

			//               if ((ret = tulostasarja(&srj, os + 200 * (cy == 'O'), numjarj, va,
//                  co, ct, np, &lineno, &sv, FALSE, html)) != 0) break;
				if (tulprm.kohde == L'P') {
					if (!inlistCS)
						EnterCriticalSection(&print_CriticalSection);
					inlistCS = 1;
					}
				tulostettu = tulostasarja(&srj, &tulprm, &lineno, &sv, FALSE); 

				if (tulprm.kohde == L'P' && !tulprm.tarkastamattomat && tulprm.rajaus != L'A') {
					if (lineno != 0)
						endpage(&tulprm);
					lineno = 0;
					}
				if (tulprm.kohde == L'P' && lineno == 0 && tulostettu) {
					if (tulostettu)
					enddoc(&tulprm);
					tulostettu = 0;
					}
				} while(tulprm.hajontatulokset);
			} while (!ret && tc != L'+' && tc != ESC && sarjaluku > 1 && vsrj != -2);
		} while (0);
//         } while (!ret && vsrj != -2 && srj <= sarjaluku && (seuranimi[0][0] || piiri));

	if (tulprm.kohde == L'P') {
		if (lineno != 0)  
			endpage(&tulprm);
		if (tulostettu)
			enddoc(&tulprm);
			tulostettu = 0;
		if (inlistCS)
			LeaveCriticalSection(&print_CriticalSection);
		inlistCS = 0;
		}
	if (tulprm.kohde == L'H') 
		htmlloppu(&tulprm);
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
   }
