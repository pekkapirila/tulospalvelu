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

// #define ENNAKOI

#ifdef MAALI
static INT pkno = 0;
#endif
extern EDITFLD editfld[];

#ifdef EI_OLE
static void tarra(kilptietue *kilp)
   {
   INT  i, sd;
   wchar_t st[81], *buf;
   INT  wf[12];
   INT32 tls;
   
   if (!tarfl) {
    writeerror_w(L"Tarratulostus ei käytössä tällä koneella", 2000);
    return;
    }
   if (!eiltarra && kilp->tark(k_pv) == L'E') return;
   if (hptarra && pfl) {
      return;
      }
//   if (!ktarra) {
//      inittrhp(tarf);
//      }
   if ((buf = (wchar_t *) malloc(CBUFL)) == 0)
      return;
   memset(buf, ' ', CBUFL);
   wf[0] = TRUE;
   wf[1] = (tls = kilp->tulos_pv(k_pv)) != 0;
   wf[4] = kilp->tark(k_pv) == L'E';
//   wf[5] = korjtarfl;
   wf[8] = pfl;
#ifndef HIIHTO
   wf[2] = kilp->tark(k_pv) == L'K' && Sarjat[kilp->Sarja()].rrtas != 1;
   wf[3] = kilp->tark(k_pv) == L'H' && Sarjat[kilp->Sarja()].rrtas != 1;
   wf[6] = wf[1] && (Sarjat[kilp->Sarja()].rrtas == 3);
   wf[7] = wf[1] && (Sarjat[kilp->Sarja()].rrtas == 2);
   wf[9] = kilp->tark(k_pv) == L'K' && Sarjat[kilp->Sarja()].rrtas == 1;
   wf[10] = kilp->tark(k_pv) == L'H' && Sarjat[kilp->Sarja()].rrtas == 1;
#else
   wf[2] = kilp->tark(k_pv) == L'K';
   wf[3] = kilp->tark(k_pv) == L'H';
   wf[6] = 0;
   wf[7] = 0;
   wf[9] = 0;
   wf[10] = 0;
#endif
   for ( i = 0; i < nktarra; i++) {
      st[0] = 0;
      if( kentta[i].df ) sd = 2;
            else sd = 1;
      kentta[i].w = min(kentta[i].w,(wtarra - kentta[i].x + 1) / sd);
      sd = 0;
      switch (kentta[i].tunnus) {
         case Nm    : kilp->nimi(st, 40, 0); break;
         case Lis   : _itow(kilp->lisno[0], st, 10); break;
         case Sra     : wcscpy(st, kilp->seuratieto(0)); break;
         case Srj     : wcscpy(st, Sarjat[kilp->Sarja()].sarjanimi); break;
#ifdef TARKNO
         case Kno     : swprintf(st,L"%4d-%d",kilp->id(), 
                                 tarkno(kilp->id())); break;
#else
         case Kno     : _itow(kilp->id(), st, 10); break;
#endif
         case Tls     : if (tls) {
                         AIKATOWSTRS(st, tls,0);
                         st[laika] = 0;
                         elimwz(st);
                         } 
                       else wmemcpy(st, L"00.00", 6); break;
#if N_PV > 1
         case TlsY    : if (kilp->ytulos(k_pv)) {
                         AIKATOWSTRS(st, kilp->ytulos(k_pv), 0);
                         st[laika] = 0;
                         elimwz(st);
                         } 
                       else wmemcpy(st, L"00.00", 6); break;
#endif
         case Ksk     : sd = 1; break;
         case Hl      : sd = 2; break;
         case El      : sd = 3; break;
         case Maika   :
                       AIKATOWSTRS(st, kilp->maali(0),t0);
                       st[8] = 0;
                       break;
         case Kaika   : wmemcpy(st, wkello(),9); break;
         case Sakko   : wcscpy(st,kilp->pv[k_pv].asakot); break;
         case Tas     : _itow(-kilp->pv[k_pv].tasoitus,st,10); break;
         case Sakko   : _itow(kilp->pv[k_pv].tasoitus,st,10); break;
         case Taso    : sd = 4; break;
         case Sako    : sd = 5; break;
         case O1      : sd = 6; break;
         case O2      : sd = 7; break;
         case O3      : sd = 8; break;
         case O4      : sd = 9; break;
         case O5      : sd = 10; break;
         case O6      : sd = 11; break;
         case O7      : sd = 12; break;
#ifdef MAALI
         case RNo     : if (pkno != 0) _itow(pkno, st, 10);
                       break;
#endif
         } 
      if (sd > 0) wcscpy(st, tarstr[sd]);
      if (((kentta[i].ehto >= 0) && wf[kentta[i].ehto]) ||
          ((kentta[i].ehto < 0) && ! wf[-kentta[i].ehto])) {
         if (hptarra) {
            putstrhp(st, kentta[i].x+((ktarra%rtarra)*wtarra)/10, 
               kentta[i].y+((ktarra/rtarra)*nrtarra)/10,kentta[i].w,
               kentta[i].df, tarf);
            }
         else {
#ifdef MAALI
            if (!kaksoistar || kentta[i].tunnus != RNo)
#endif
               putwstr(buf, st,kentta[i].x,kentta[i].y,kentta[i].w,kentta[i].rf,
                  kentta[i].df);
            if( kaksoistar )
               putwstr(buf, st,kentta[i].x+wtarra,kentta[i].y,kentta[i].w,
                  kentta[i].rf,FALSE);
            }
         }
      }
   if (hptarra) {
      ktarra++;
      if (ktarra == ntarra) {
         printbufhp(tarf);
         ktarra = 0;
         }
      }
   else
      printbuf(tarf,buf, nrtarra);
   free(buf);
   }

static void near testitarra(void)
   {
   INT i_pv;
   kilptietue tkilp;

   wcscpy(tkilp.sukunimi, L"Sukunimi");
   wcscpy(tkilp.etunimi, L"Etunimi");
   tkilp.setId(1234);
   tkilp.lisno[0] = 11111;
   wcscpy(tkilp.seura, L"Seurannimi");
   tkilp.setSarja(sarjaluku-1);
   for (i_pv = 0; i_pv < N_PV; pv++) {
      tkilp.set_tark(L'-', i_pv);
      wcscpy(tkilp.pv[i_pv].asakot, L"1020");
      tkilp.pv[i_pv].tasoitus = 12;
      tkilp.pv[i_pv].tlahto = 3783*SEK;
      tkilp.tall_tulos_pv(4345*SEK, i_pv);
      }
   tarra(&tkilp);
   }
#endif

INT kilptietue::find(INT *d, INT nimihaku)
   {
	static int edkno,nimifl;
	static char edkey[21];
	INT kno, exfl, valmis = 0;
	wchar_t ch, tc;
	char keyn[NIMIINDL+1];
	wchar_t str[81];
	INT32 badge;
	#ifdef MAKI
	static int srj;
	#endif
	wchar_t msg24[81];
	msg24[80] = 0;

#ifdef MAKI
	if (k_pv == makipv) {
		wcsncpy(msg24, L"PgDn / PgUp: Hyppyjärjestyksessä seuraava/edellinen", 80);
		}
	else
#endif
#if LANG == SV
		wcsncpy(msg24, L"Ctrl-F2 : M†l, Ctrl-F3/Ctrl-F4/.. : Mellatider ", 80);
#else
		wcsncpy(msg24, L"PgDn/PgUp: Vaiheen vaihto, Ctrl-PgDn/Ctrl-PgUp: Väliaikapiste", 80);
#endif

	exfl = FALSE;
#ifndef MAKI
	clrscr();
	kehys(1);
	outform();
#if !defined(AMPUSUUNN)
	if( ! korjfl ) VIDSPAIKA(editfld[MAE-1].y, editfld[MAE-1].x,tmaali,t0);
#endif
#else  // MAKI
	if (makihaku)
		outmakiform();
	else {
		kehys(1);
		outform();
		}
#endif
	if( nilmt > 0 ) {
		kno = 0;
		clrln(ySize-3);
#if !defined(AMPUSUUNN)
#ifndef MAKI
#if LANG == SV
		if (autofl) 
			vidspwmsg(ySize-3,0,7,0,L"<Ret>: Godk, F)ölj, B)ak, E)mitsokning, "
				L"<Esc>: Sluta, <F3>: Resultater,");
		else
			vidspwmsg(ySize-3,0,7,0,L"<Ret>: Godk, F)ölj, B)ak, E)mitsökning, "
				L"<Esc>: Sluta");
#else
		if (autofl) 
			vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyv, S)eur, E)dell, eM)ithaku, "
				L"<Esc>: Poistu, <F3>: Tul. yht.veto,");
		else
			vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyv, S)eur., E)dell., eM)ithaku, "
				L"<Esc>: Poistu, <F4>: Testitarra");
#endif
#else
		if (k_pv == makipv) {
			if (autofl)
				vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyv, S)eur, E)d, A)lku, K)ierros, <Esc>: Poistu, "
					L"<F3>: Tul.yht.veto");
			else
				vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyväksy valinta, S)eur., E)dell.,  A)lku, K)ierros, "
					L"<Esc>: Poistu");
			}
		else {
			if (autofl)
				vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyv, S: Seur, E: Edell, <Esc>: Poistu, "
					L"<F3>: Tul. yht.veto");
			else
				vidspwmsg(ySize-3,0,7,0,L"<Return>: Hyväksy valinta, S: Seur., E: Edell., "
					L"<Esc>: Poistu");
			}
#endif
#else   // AMPUSUUNN
		if (autofl)
			vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyväksy kenttä, <Esc>: Poistu, "
				L"<F3>: Tulosta yht.veto");
		else
			vidspwmsg(ySize-3,0,7,0,L"<Return> : Hyväksy kenttä,   <Esc> : Poistu");
#endif
		while (!valmis) {
			exfl = 0;
			vidspwmsg(ySize-1,0,7,0,msg24);
			*d = 0;
			while (!nimihaku && *d <= 0) {


				if (tarknofl) {
					viwrrectw(editfld[NOE-1].y, editfld[NOE-1].x+4,
						editfld[NOE-1].y, editfld[NOE-1].x+4,L"-",7,0,0);
					}
				sccurset(editfld[NOE-1].y, editfld[NOE-1].x+(tarknofl ? 5 : 3));
			//		     tcomfl = FALSE;
#if LANG == SV
				if (tarknofl)
					inputknow(&kno,editfld[NOE-1].x, editfld[NOE-1].y,L"fFbBeE\r\x1B\xCB\xCC*",&tc);
				else
					input_now(&kno,editfld[NOE-1].x, editfld[NOE-1].y,L"fFbBeE\r\x1B\xCB\xCC*",&tc);
				tc = towupper(tc);
				wtransl(L"FBE", L"SEM", &tc);
#else
				if (tarknofl)
					inputknow(&kno,editfld[NOE-1].x, editfld[NOE-1].y,L"eEsSmM\r\x1B\xCB\xCC*",&tc);
				else
#ifdef MAKI
				if (k_pv == makipv)
					input_now(&kno,editfld[NOE-1].x, editfld[NOE-1].y,L"aAkKeEsSmM\r\x03\x12\x1B\xCB\xCC*",&tc);
				else
#endif
					input_now(&kno,editfld[NOE-1].x, editfld[NOE-1].y,L"eEsSmM\r\x1B\xCB\xCC*",&tc);
#endif
			//       tcomfl = TRUE;
				tc = towupper(tc);
#ifndef MAKI
				if (tc == L'M') {
					if (emitfl) {
						badge = 0;
						do {
							inputlongw(&badge,editfld[EME-1].len ,editfld[EME-1].x, 
							editfld[EME-1].y,L"\r\x1b",&tc);
							if (tc == ESC) break;
							kno = bdg2kno((UINT32)badge);
							if (!kno) writeerror_w(L"Ei löydy", 0);
							} while(!kno);
						}
					else
						writeerror_w(L"Emit-toimintoja ei käynnistetty",0);
					tc = L'\r';
					}
#endif
				if ((tc == L'E' || tc == L'S') && nimifl) {
					memcpy(keyn, edkey, sizeof(keyn));
					ch = tc;
					goto seurnimi;
					}
				if (tc == L'E') {
					kno = edkno-1;
					while (kno > minkilpno && getpos(kno) < 1)
					kno--;
					}
				if (tc == L'S') {
					kno = edkno + 1;
					while (kno < maxkilpno && getpos(kno) < 1)
					kno++;
					}
#ifdef MAKI
				if (tc == L'A') {
					kno = makivuoro[0];
					if (!kno)
					kno = seurvuoro(1);
					}
				if (tc == PGDN) {
					kno = seurvuoro(1);
					}
				if (tc == PGUP) {
					kno = seurvuoro(-1);
					}
				if (tc == L'K') {
					tc = aktkierros ? L'0' + aktkierros : L'K';
					ch = tc;
					wselectopt(L"Valitse kierros: K)oe, 1), 2), P)oista tulokset, A)ktivointi pois", L"K12PA", &tc);
					if (tc == L'P') {
						swprintf(str, L"Poistetaan kierroksen %c tulokset sarjasta %s. Vahvista (K/E)", ch, Sarjat[srj].sarjanimi);
						tc = L' ';
						wselectopt(str, L"KE", &tc);
						if (tc == L'K')
						poistakierrostulokset(srj, aktkierros);
						tc = 0;
						}
					if (tc == L'A') {
						aktivointi(0, aktkierros, 0);
						}
					if (tc == L'K')
						tc = L'0';
					if (tc >= L'0' && tc <= L'0' + nkierros[srj] && (tc - L'0' != aktkierros)) {
						vaihdakierros(tc - L'0', 0);
						teevuorotaulu(srj, 1);
						outmakiform();
						}
					vidspwmsg(ySize-3,0,7,0,L"<Ret>: Hyväksy valinta, S)eur., E)dell.,  A)lku, K)ierros, "
						L"<Esc>: Poistu");
					continue;
					}
#endif
				tcomfl = TRUE;
				if( tc == 204 ) {
#if !defined(AMPUSUUNN)
//		            testitarra();
#endif
					continue;
					}
				if( tc == 203 ) {
					yvfl = TRUE;
					continue;
					}
				if(tc == ESC || kno == 0) 
					break;
				if( (kno < minkilpno) || (kno > maxkilpno) )
					writeerror_w(L"Numero liian suuri tai pieni",2000);
				else {
					edkno = kno;
					nimifl = 0;
					*d = getpos(kno);
					if( *d > 0 ) {
						GETREC(*d);
#ifdef MAKI
						srj = Sarja();
						if (makihaku)
							outmakikilp(this, 0);
						else
#endif
							outkilp(0);
						}
					else writeerror_w(L"EI LÖYDY!",2000); 
					}
				}
			if (tc == ESC || kno > 0)
				valmis = 1;
			else
				nimihaku = 1;
			if(tc != ESC) {
				if( kno == 0 ) {
					str[0] = 0;
					clrln(ySize-3);
#if LANG == SV
					vidspwmsg(ySize-3,0,7,0,L"<Return> : Acceptera,   <Esc> : Sluta");
#else
					vidspwmsg(ySize-3,0,7,0,L"<Return> : Hyväksy kenttä,   <Esc> : Poistu");
#endif
					inputwstr(str,30,editfld[SNME-1].x, editfld[SNME-1].y,L"\xD\x1B\x5\x9\xB",&tc,FALSE);
					tcomfl = TRUE;
					if (tc <= 11) {
						nimihaku = 0;
						continue;
						}
					if( tc != ESC ) {
						keyfromwname(keyn, str, NIMIINDL, 0);
						searchix(&nameindex, keyn, d);
						do {
							GETREC(*d);
#ifdef MAKI
							srj = kilp->Sarja();
							if (makihaku)
								outmakikilp(kilp, 0);
							else
#endif
								outkilp(0);
							listfl = TRUE;
#if LANG == SV
							ch = L'A';
							wselectopt(L"Soka : F)öljande, B)ak†t, A)cceptera,"
								L" <Esc> : Sluta",L"FBA\x1B",&ch);
							wtransl(L"FBA", L"SEH", &ch);
#else
							ch = L'H';
							wselectopt(L"Hae : S)euraava, E)dellinen, H)yväksy valinta,"
								L" <Esc> : Poistu",L"SEH\x1B",&ch);
#endif
							seurnimi:
							switch(ch) {
								case L'S' :
									nextix(&nameindex, keyn, d);
									break;
								case L'E' :
									previx(&nameindex, keyn, d);
									break;
								case ESC :
									exfl = TRUE;
									break;
								}
							} while (ch != L'H' && ch != ESC);
						if (ch == L'H') {
							memcpy(edkey, keyn, sizeof(edkey));
							nimifl = 1;
							valmis = 1;
							}
						}
					}
				}
			}
		if( tc == ESC) 
			exfl = TRUE;
		} /* of nilmt > 0 .. */
	else erbeep();
	return(exfl);
}

void korjaa(INT d, kilptietue *kilp, INT tausta, kilptietue *entkilp)
   {
   INT  exfl, comtarfl;
   kilptietue ekilp, kilp1;

   if (!tausta) header=L"KORJAA    ";
   kehys(2);
   for (;;) {
      ekilp = *kilp;
      exfl = ekilp.inputkilp(0);
      tcomfl = TRUE;
      if (exfl) break;
      comtarfl = FALSE;
      if ((tarfl || lahtarfl) && 
         (ekilp.tulos_pv(k_pv) || !ekilp.hyv()))
         comtarfl = TRUE;
      kilp1.GETREC(d);
      if (!(*entkilp == kilp1)) {
         writeerror_w(L"Kilpailijatietoihin tullut muutossanoma - tehdyt "
            L"muutokset peruutetaan", 0);
         if (*entkilp == *kilp) {
            *kilp = kilp1;
            *entkilp = kilp1;
            continue;
            }
         else break;
         }
	  EnterCriticalSection(&tall_CriticalSection);
      ekilp.tallenna(d,comtarfl, 0, 0, 0);
	  LeaveCriticalSection(&tall_CriticalSection);
      break;
      }
   }

INT32 maaliaika(INT32 tmaali, INT t0, INT maikay, INT maikax, wchar_t *wkh)
	{
	char tarr[7], tarrold[7], prs[100];
	char ch, ch2, kh;
	INT  i, j, ltarr;
	INT  tt, tt1, tm, kuulpiste0;
	char st[4];
	INT32  ttm;
	INT32  tmaali0;

	ltarr = pvparam[k_pv].laika == 10 ? 7 : 6;

	tmaali0 = tmaali;
	kuulpiste0 = kuulpiste;
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
	if (pvparam[k_pv].laika == 10) tarrold[6] = st[2];
	tarrold[4] = st[0];
	tarrold[5] = st[1];
	for ( i = 0; i < ltarr; i++) tarr[i] = tarrold[i];
#if !defined(AMPUSUUNN)
#ifdef MAALI
	if (!syotto)
#endif
		{
		viwrrect(maikay-1,maikax-22,maikay-1,maikax-2,
			"Edellinen maaliaika :",7,0,0);
		if (pvparam[k_pv].laika == 10)
			sprintf(prs,"%c%c.%c%c.%c%c,%c",tarrold[0],tarrold[1],
				tarrold[2],tarrold[3],tarrold[4],tarrold[5],tarrold[6]);
		else
			sprintf(prs,"%c%c.%c%c.%c%c",tarrold[0],tarrold[1],
				tarrold[2],tarrold[3],tarrold[4],tarrold[5]);
		vidspmsg(maikay-1, maikax, 7, 0, prs);
		}
#endif
	if (pvparam[k_pv].laika == 10)
		sprintf(prs,"%c%c.%c%c.%c%c,%c",tarr[0],tarr[1],
					  tarr[2],tarr[3],tarr[4],tarr[5],tarr[6]);
	else
		sprintf(prs,"%c%c.%c%c.%c%c",tarr[0],tarr[1],
					  tarr[2],tarr[3],tarr[4],tarr[5]);
	vidspmsg(maikay, maikax, 7, 0, prs);
lue:
	clrln(ySize-3);
	viwrrect(ySize-3,0,ySize-3,73,"<Ret.> : Hyväksy aika K)esk. H)yl. E)i läht. "
		" <Esc> : Poistu",7,0,0);
	i = 0;
	do {
		 kh = 0;
		 ch = readkbd(&ch2,TRUE,spoolfl);
		 switch(ch) {
			case 13 :
			case '+':   break;
			case 8  :   if( i>0 ) {
								i--;
								if( i > 0 ) for ( j = 0; j <= i; j++)
									tarr[ltarr-1-j] = tarr[ltarr-2-j];
								tarr[ltarr-1-i] = tarrold[ltarr-1-i];
							} break;
			case 48 :
			case 49 :
			case 50 :
			case 51 :
			case 52 :
			case 53 :
			case 54 :
			case 55 :
			case 56 :
			case 57 :   if( i==ltarr ) erbeep();
							else {
								i++;
								for (j = ltarr - i; j < ltarr-1; j++)
									tarr[j] = tarr[j+1];
								tarr[ltarr-1] = ch;
							} break;
			case 'e':
			case 'E':   kh = 'E';
							ch = 200;
							break;
			case 'h':
			case 'H':   kh = 'H';
							ch = 200;
							break;
			case 'k':
			case 'K':   kh = 'K';
							ch = 200;
							break;
			case ESC:   kh = ESC;
							break;
#if defined(AMPUSUUNN)
			case 0 :
				switch (ch2) {
					case 64 :                      /* F6 */
					case 73 : kh = 211;           /*pgup*/
								 ch = 200;
								 break;
					case 68 :                      /* F10 */
					case 81 : kh = 212;           /*pgdn*/
								 ch = 200;
								 break;
					default : erbeep();
					}
				break;
#endif
			default :   erbeep();
			}
		if (pvparam[k_pv].laika == 10)
			sprintf(prs,"%c%c.%c%c.%c%c,%c",tarr[0],tarr[1],
					  tarr[2],tarr[3],tarr[4],tarr[5],tarr[6]);

		else
			sprintf(prs,"%c%c.%c%c.%c%c",tarr[0],tarr[1],
					  tarr[2],tarr[3],tarr[4],tarr[5]);
		vidspmsg(maikay, maikax, 7, 0, prs);

		if (kuulpiste0 != kuulpiste) ch = ESC;
		} while (ch != KB_CR && ch != 200 && ch != ESC);
	if(ch == KB_CR) {
		tt = 10 * (tarr[0] - '0') + tarr[1] - '0';
		tt1 = 10 * (tarr[2] - '0') + tarr[3] - '0';
		tm = 10 * (tarr[4] - '0') + tarr[5] - '0';
		tm *= 10;
		if (pvparam[k_pv].laika == 10) tm += tarr[6] - '0';
		if( 60 * (tt-t0) + tt1 < -MRAJA ) tt = tt + 24;
		if( (tm < MINUUTTI) && (tt1 < 60) && (abs(60*(tt-t0)+tt1) < MRAJA))
			tmaali = tm + MINUUTTI*tt1 + TUNTI*(tt-t0);
		else {
			erbeep();
			goto lue;
			}
		}
	if (ch != ESC && ABS(tmaali - tmaali0) > 15*MINUUTTI && ntulost > 0) {
		erbeep();
		wchar_t wch = L' ';
		wselectopt(L"Maaliaika muuttunut yli 15 min - Hyväksytäänkö (K/E)?",
			L"KE", &wch);
		if (wch == L'E') goto lue;
		}
	*wkh = kh;
	return(tmaali);
	}

static void editkilp(void)
   {
   INT  d, key;
   kilptietue kilp, entkilp;
	char che;

   do {
      if (kilp.find(&d, 0))
         break;
		if (!selostaja) {
	      entkilp = kilp;
		   korjaa(d, &kilp, 0, &entkilp);
			}
		else {
			while(!kbready(&che, &key)) {
				Sleep(100);
				}
			if (che == ESC) {
				readkbd(&che, 0, 0);
				break;
				}
			}
      } while (1);
   }

static void near deletekilp(void)
   {
   INT  d, i;
   wchar_t cs;
   char che;
   kilptietue kilp;

   do {
      header=L"POISTO    ";
      if (kilp.find(&d, 0) || d <= 0)
         return;
      for (i = 0; i < Sarjat[kilp.Sarja()].valuku[k_pv]; i++)
         if (kilp.p_aika(i+1) != 0) break;
      if (kilp.p_aika(0) || !kilp.hyv()
         || i < Sarjat[kilp.Sarja()].valuku[k_pv]
         ) {
         clrln(ySize-3);
         vidspwmsg(ySize-3,0,7,0,
            L"Ei voi enää poistaa, merkitse ei läht.,  Paina <Return>");
         erbeep();
         do {} while (readkbd(&che,TRUE,spoolfl) != KB_CR);
         }
      else {
         cs = L' ';
         wselectopt(L"P)oista kilpailija,  L)uovu poistosta",L"PL",&cs);
         if( cs == L'P' ) {
			 EnterCriticalSection(&tall_CriticalSection);
			 deltall(d, 0);
			 LeaveCriticalSection(&tall_CriticalSection);
			}
         }
      } while (1);
   }

void korjaukset()
   {
   wchar_t ch;
   extern scr korjvalikko;

   korjfl = TRUE;
   do {
      clrscr();
      header=L"KORJAUKSET";
      kehys(1);
      prscr(&korjvalikko);
      listfl = TRUE;
      ch = L' ';
#ifndef TESTI
//#if defined(UINTI) || defined(MERI5)
	  if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
		  wselectopt(L"L)isää, K)orjaa, eR)ätiedot, P)oista,  <Esc> : poistu päävalintaan",
			L"LKRP\x1B",&ch);
		 }
	  else {
//#else
		  if (emitfl) {
			 emitkd = 0;
#if LANG == SV
			 wselectopt(L"T)illägg, F)örändra, E)mitkoder, A)vskriva,  <Esc> : backa",
				L"TFEA\x1B",&ch);
			 wtransl(L"TFEA", L"LKEP", &ch);
#else
			 wselectopt(L"L)isää, K)orjaa, E)mitkoodit, P)oista,  <Esc> : poistu päävalintaan",
				L"LKEP\x1B",&ch);
#endif
				if (nilmt < 1 && ch != L'L') {
					writeerror_w(L"Ei kilpailijoita, vain lisääminen mahdollista", 0);
					continue;
					}
			 if (ch == L'E') {
				ch = L'S';
				wselectopt(L"S)yötä koodeja, L)ue tiedostosta", L"SL", &ch);
				wtransl(L"SL", L"KE", &ch);
				emitkd = 1;
				}
			 }
		  else {
#if LANG == SV
			 wselectopt(L"T)illägg, F)örändra, A)vskriva,  <Esc> : backa",
				L"TFA\x1B",&ch);
			 wtransl(L"TFA", L"LKP", &ch);
#else
			 wselectopt(L"L)isää, K)orjaa, P)oista,  <Esc> : poistu päävalintaan",
				L"LKP\x1B",&ch);
#endif
		    }
	    }
//#endif // !UINTI
		if (nilmt < 1 && ch != L'L') {
			writeerror_w(L"Ei kilpailijoita, vain lisääminen mahdollista", 0);
			continue;
			}
      switch(ch) {
#else
      wselectopt(L"L)isää, K)orjaa, P)oista, T)estaa <Esc> : poistu päävalintaan",
                                                      L"LKPT\x1B",&ch);
      switch(ch) {
         case L'T' : testaa(); break;
#endif
         case L'L' : add(0); break;
         case L'K' : editkilp(); break;
         case L'P' : deletekilp(); break;
// #if defined(UINTI) || defined(MERI5)
         case L'R' : lueeratiedot(0); break;
//#endif
         case L'E' : lueemitkoodit(); break;
         }
      } while (ch != ESC);
   }

void laskenta()
   {
   INT  exfl, d, piste;
   INT nl;
   wchar_t cs, wkh;
   char kh;
   kilptietue kilp,ekilp,entkilp;
#if defined(AMPUSUUNN)
   INT maikay;
#endif

   korjfl = FALSE;
   do {
      header=L"LASKENTA  ";
      exfl = kilp.find(&d, 0);
	  entkilp = kilp;
      k_alku:
      if (exfl) {
         return;
         }
      piste = kuulpiste;
#if defined(AMPUSUUNN)
      if (kilp.p_aika(piste)) {
         if (piste == 1 && !kilp.p_aika(2)) piste = 2;
         else if (!kilp.p_aika(0)) piste = 0;
         else if (!kilp.p_aika(1)) piste = 1;
         else if (!kilp.p_aika(2)) piste = 2;
         kuulpiste = piste;
         }
      if (!kilp.p_aika(piste)) {
         ekilp = kilp;
         do {
            piste = kuulpiste;
            maikay = piste ? (MAY - 6 + 2 * piste) : MAY;
            tmaali = maaliaika(tmaali, t0, maikay, MAX, &kh);
            if (kh == 211 || kh == 212) {
               kuulpiste = (kuulpiste + 213 - kh) % 3;
               }
            } while (kh == 211 || kh == 212);
#else
      if (piste > Sarjat[kilp.Sarja()].valuku[k_pv]) piste = 0;
      if ((!piste || va_kello) && !kilp.p_aika(piste)) {
         ekilp = kilp;
         tmaali = maaliaika(tmaali, t0, editfld[MAE-1].y, editfld[MAE-1].x, &wkh);
		 kh = wchartoansi(wkh);
#endif
         if (piste != kuulpiste) {
            piste = kuulpiste;
            if (piste > Sarjat[kilp.Sarja()].valuku[k_pv]) piste = 0;
            kilp.outkilp(0);
            goto k_alku;
            }
         if (kh != ESC) {
            if (kh != 0 && kh != 199) ekilp.set_tark(oemtowchar(kh) ,k_pv);
            else {
#ifndef LVAYLAX
                 tmaali = pyoristatls(tmaali, 1);
#endif
#if !defined(AMPUSUUNN)
               if (Sarjat[ekilp.Sarja()].rrtas > 1) {
				  int yks = Sarjat[ekilp.Sarja()].tsak[k_pv];
				  if (yks == 0)
					 yks = kilpparam.sakkoyks ? kilpparam.sakkoyks : 1;
                  VIDSPAIKA(editfld[MAE-1].y, editfld[MAE-1].x,tmaali,t0);
                  clrln(ySize-3);
                  viwrrectw(ySize-3,0,ySize-3,30,L"Anna sakko,  <Return> : Hyväksy",
                     7,0,0);
                  nl = ekilp.pv[k_pv].tasoitus/yks;
                  INPUTINTW(&nl,5,editfld[TAE-1].x, editfld[TAE-1].y,L"\xD",&cs);
                  ekilp.pv[k_pv].tasoitus = nl*yks;
                  }
#endif
               if (!kh)
                  ekilp.set_tulos(piste, tmaali);
               kh = 0;
               }
            do {
               EnterCriticalSection(&tall_CriticalSection);
               haesija(&ekilp, 0, 0, 0);
               LeaveCriticalSection(&tall_CriticalSection);
               if( kh == 0 ) nl = 1;
               else nl = 0;
               ekilp.outkilp(nl);
               cs = L' ';
                  wselectopt(L"+) hyväksy, K)orjaa, <Esc> : peruuta",
                     L"+KE\x1B",&cs);
               switch(cs) {
                  case L'K' :  clrln(ySize-3);
                              viwrrectw(ySize-3,0,ySize-3,77,L"<Return> : hyväksy "
                                 L"kenttä,  + : hyväksy korjaukset, "
                                 L" <Esc> : peruuta ja poistu",7,0,0);
                              exfl = ekilp.inputkilp(0);
                              break;
				  case L'E':  if (aikajono[akttm] && aikajono[akttm]->rtime > 1)
                                 aikajono[akttm]->rtime--;
							  if (aikajono[akttm] && aikajono[akttm]->aktrows > 0)
                                 aikajono[akttm]->rtime--;
                  case ESC :  exfl = TRUE;
                  }
               } while (cs != L'+' && !exfl);
            if( ! exfl ) {
			   EnterCriticalSection(&tall_CriticalSection);
               ekilp.tallenna(d,TRUE, 0, 0, &entkilp);
			   LeaveCriticalSection(&tall_CriticalSection);
               }
            exfl = FALSE;
            }
         tcomfl = TRUE;
         }
      else {
         cs = L'K';
         if (!va_kello) {
            erbeep();
            wselectopt(L"Tulos jo annettu, korjataanko? (K/E)",L"KE\x1B",&cs);
            }
         if( cs == L'K' ) {
            korjfl = TRUE;
            korjaa(d, &kilp, 0, &entkilp);
            korjfl = FALSE;
            }
         }
      } while (1);
   }

#if defined(MAALI) && !defined(EIMAALI)
INT maali_on = 0;
INT rivimuutos = 0;

static void haevirhe(void)
   {
   kilptietue kilp;
   aikatp ut;
   INT pt, piste;
   INT32 tls;

   for (pt = aikajono[akttm]->rtime+1; ; pt++) {
      if (pt - aikajono[akttm]->aktrows >= 0) pt = 0;
      if (pt - aikajono[akttm]->rtime == 0) {
         writeerror_w(L"Ei löydy", 0);
         break;
         }
	  kilp.nollaa();
      if (aikajono[akttm]->getAktTime(&ut, pt)->kno >= minkilpno && getpos(ut.kno) > 0) {
         kilp.GETREC(getpos(ut.kno));
		 piste = va_piste(ut.piste);
         tls = kilp.maali(kierrosva[piste+2]);
         tls = NORMKELLO(tls - purajak(ut.t));
		 if (abs(tls) > pvparam[k_pv].pyor[2]) {
            aikajono[akttm]->rtime = pt;
            break;
            }
         }
      }
   }

static void poista_tlahto(kilptietue *kilp)
   {
	if (pvparam[k_pv].hiihtolahto)
	   kilp->tall_tulos(-1, kilp->pv[k_pv].tlahto);
   }


static void val_piste(INT *piste, INT valuku, INT paattele)
   {
   wchar_t tc, msg[70], st[16];
   INT i;

   memset(st, 0, sizeof(st));
   if (paattele) {
      wcscpy(msg, L"Oletuspisteen valinta: A)utom., L)ähtö, M)aali");
      wcscpy(st, L"ALM");
      }
   else {
      wcscpy(msg, L"Hyväksy tai valitse piste: L)ähtö, M)aali");
      wcscpy(st, L"LM");
      }
   if (valuku > 0) {
		wcscat(msg, L", Väliaika");
		if (kilpparam.maxvaluku > 9) {
			vidspwmsg(ySize-3, 0, 7, 0, msg);
			wcscpy(st, str_piste(*piste));
			inputwstr(st, 3, wcslen(msg) + 2, ySize-3, L"\r", &tc, 0);
			if ((i = _wtoi(st)) > 0 && i <= valuku)
				*piste = i;
			else
			   *piste = wcswcind(st[0], ch_piste) - 2;
			if (*piste < -2)
				*piste = 0;
			return;
			}
		else {
		  for (i = 1; i <= valuku; i++) {
			 wcscat(msg, L",  ");
			 msg[wcslen(msg)-1] = L'0'+i;
			 st[wcslen(st)] = L'0'+i;
			 }
		}
      }
   tc = ch_piste[*piste+2];
   wselectopt(msg, st, &tc);
   *piste = wcswcind(tc, ch_piste) - 2;
   }

static void maalikorj(void)
   {
   wchar_t ch, tc, msg[80];
   INT32 muutos = 0, kerroin = 0, talku;
   INT ptime;
   aikatp vt, ut;

   ch = L' ';
   wselectopt(L"L)isää aikoihin, V)ähennä ajoista, P)ienennä / K)asvata eroja, Esc : Peruuta",
      L"LVPK\x1b", &ch);
   clrln(ySize-3);
   if (ch == ESC)
      return;
   if (ch == L'L' || ch == L'V') {
      swprintf(msg, L"Anna %syksen määrä sekunnin kymmenyksinä", ch == L'L' ? L"Lisä" : L"Vähenn");
      vidspwmsg(ySize-3,0,7,0,msg);
      inputlongw(&muutos, 5, 45, ySize-3, L"\r", &tc);
      if (ch == L'V') muutos = -muutos;
      }
   if (ch == L'P' || ch == L'K') {
      swprintf(msg, L"Anna erojen %sys sekunnin kymmenyksinä tunnissa", ch == L'K' ? L"Lisä" : L"Vähenn");
      vidspwmsg(ySize-3,0,7,0,msg);
      kerroin = 0;
      inputlongw(&kerroin, 5, 50, ySize-3, L"\r", &tc);
      if (ch == L'P') kerroin = -kerroin;
      }
   if (!muutos && !kerroin)
      return;
   talku = aikajono[akttm]->atime->t;
   if (muutos > 0 || kerroin > 0)
      ptime = aikajono[akttm]->aktrows-1;
   else
      ptime = 0;
   while (ptime >= 0 && ptime - aikajono[akttm]->aktrows < 0) {
	   int tt;

      if (purajak(aikajono[akttm]->getAktTime(&vt, ptime)->t) > 12*TUNTI || purajak(vt.t) < -12*TUNTI)
         continue;
	  ut = vt;
	  tt = ut.t;
      ut.t = ut.t + AIKAJAK*((purajak(ut.t - talku)*kerroin)/TUNTI + muutos);
	  ut.date = tm_copydate(ut.t, tt, ut.date);
	  tall_rivi(akttm, &ut, &vt, 0, 0, 0, 0, 0);
      if (muutos > 0 || kerroin > 0)
         ptime--;
      else
         ptime++;
      }
   }

static void near naytarivi(INT ptime, INT r, INT r0, INT r1, INT k)
   {
   kilptietue kilp;
   INT l,n,kno,ip, lisa = 0;
   wchar_t line[86], st[80];
   INT32 tls;
   aikatp pt;

   if (kilpparam.maxvaluku > 9)
		lisa = 1;
   if (kilpparam.maxvaluku > 99)
		lisa = 2;
   wmemset(line, L' ', 65);
   if (ptime >= 0) {
      n = ptime;
      itow1(line,n+1,4);
	  ip = va_piste(aikajono[akttm]->getAktTime(&pt, ptime)->piste);
      if (purajak(pt.t) > TMAALI0)
         wmemcpy(line+5,AIKATOWSTRS(st, purajak(pt.t),t0),8);
      itow1(line+14, pt.kno, 5);
      if (pt.kno > 10000 && pt.kno < 19998)
         kno = pt.kno - 10000;
      else kno = pt.kno;
      if (autokilp && seurkrivi == ptime) {
         line[13] = L'\x10';
         if (pt.kno < 10000) line[14] = L'\x10';
         }
      if (tarknofl == 2) {
         line[19] = L'-';
         line[20] = tarkno(kno)+L'0';
         }
	  if (kilpparam.maxvaluku < 10) {
//		  if (!vainoletus) 
			  line[20] = ch_piste[ip+2];
		  }
	  else {
//		if (!vainoletus) 
			{
			if (ip < 10)
				line[20] = ch_piste[ip+2];
			else {
				if (kilpparam.maxvaluku >= 100) {
					if (ip > 99) 
						line[19] = L'0' + ip/100;
				}
				line[20] = L'0' + (ip/10)%10;
				line[21] = L'0' + ip%10;
				}
			}
		}
      if (getpos(kno) > 0) {
         kilp.GETREC(getpos(kno));
         l = wcslen(kilp.nimi(st, 30, 0));
         if (l > 17-lisa)
            l = 17-lisa;
         wmemcpy(line+22+lisa, kilp.nimi(st, 30, 0), l);
         l = wcslen(kilp.seuratieto(0));
         if (l > 7)
            l = 7;
         wmemcpy(line+40, kilp.seuratieto(0), l);
         l = wcslen(Sarjat[kilp.Sarja()].sarjanimi);
         if (l > 8)
            l = 8;
         wmemcpy(line+48, Sarjat[kilp.Sarja()].sarjanimi, l);
         if (purajak(pt.t) > TMAALI0) {
            if (ip >= 0) {
               if ((tls = kilp.p_aika(kierrosva[ip+2])) == 0) {
                  tls = kilp.set_tulos( kierrosva[ip+2], purajak(pt.t));
                  line[64] = L'*';
                  }
               wmemcpy(line+57, AIKATOWSTRS(st, tls, 0) + 1, 7);
               if (line[64] != L'*' &&
                  (kilp.tark(k_pv) == L'T' || kilp.tark(k_pv) == L'I'))
                  line[64] = kilp.tark(k_pv);
               tls = kilp.maali(kierrosva[ip+2]);
               tls = NORMKELLO(tls - purajak(pt.t));
//               if (tls && (pvparam[k_pv].laika == LAIKA || tls > 9 || tls < -9))
               if (tls/pvparam[k_pv].pyor[1])
                  line[64] = L'*';
               }
            else if (pvparam[k_pv].hiihtolahto && ip == -1) {
               tls = NORMKELLO(kilp.p_aika(-1) - kilp.pv[k_pv].tlahto);
               if (tls >= 0)
                  swprintf(line+57,L"+%6.6s ", AIKATOWSTRS(st, tls,0)+4);
               else
                  swprintf(line+57,L"-%6.6s ", AIKATOWSTRS(st, -tls,0)+4);
               if (NORMKELLO(kilp.p_aika(-1) - purajak(pt.t))/pvparam[k_pv].pyor[1] != 0)
                  line[64] = L'*';
               }
            }
         if (pt.kno > 10000) 
            wcscpy(line+57,L"NOLLAUS");
         else if (ip == 0) {
            switch (kilp.tark(k_pv)) {
               case L'H':
                  wmemcpy(line+57,L"HYLÄTTY", 7);
                  break;
               case L'K':
                  wmemcpy(line+57,L"KESKEYT", 7);
                  break;
               case L'E':
                  wmemcpy(line+57,L"EI LÄHT", 7);
                  break;
               }
            }
         }
      else if (pt.kno >= 10000) {
         switch (pt.kno) {
            case 10000: wmemcpy(line+22,L"TAHDISTUSAIKA", 13);
                        break;
            case 19998: wmemcpy(line+22,L"NOLLAUS - YLIMÄÄRÄINEN AIKA", 27);
                        break;
            case 19999: wmemcpy(line+22,L"NOLLAUKSEN LOPPU", 16);
                        break;
			default:
				if (pt.kno > 30000 && pt.kno <= 30000 + sarjaluku) {
					wmemcpy(line + 14, L" S", 2);
					wmemcpy(line + 19 + lisa, L"L  Sarjalähtö: ", 15);
					wmemcpy(line + 34 + lisa, Sarjat[pt.kno - 30001].sarjanimi, wcslen(Sarjat[pt.kno - 30001].sarjanimi));
					}
				if (pt.kno > 31000 && pt.kno < 32000) {
					wmemcpy(line + 19 + lisa, L"L  Erälähtö: ", 13);
					line[32+lisa] = L'0' + (pt.kno % 1000) / 100;
					line[33+lisa] = L'0' + (pt.kno % 100) / 10;
					line[34+lisa] = L'0' + (pt.kno % 10);
					}
			}
         }
      else if (pt.t != TMAALI0*AIKAJAK) {
			swprintf(line+22, L"Koodi%7ld Lähde %5d %c", pt.badge, (int)pt.kanava, 
				(pt.badge && pt.tapahtuma < 4) ? L"ADLT"[pt.tapahtuma] : pt.tapahtuma+L'0');
			line[wcslen(line+22)+22] = L' ';
         }
#if defined(SWIM2000)
      else if (pt.kanava && pt.t != TMAALI0*AIKAJAK) {
         swprintf(line+22, L"Rata %d, Pää %c, Kosketus %d", ((int)pt.kanava)%10,
				pt.kanava >= 100 ? L't' : L's', (((int)pt.kanava)/10)%10);
         line[wcslen(line+22)+22] = L' ';
         }
#endif
	  if (pt.status)
		  line[64] = '#';
      }
   if (r == r1+2) {
      line[4] = 16;
      line[13] = 17;
      }
   if (r == r0+2) {
      if (k == 1) {
         viwrrectw(r,0,r,14,line,0,7,0);
         viwrrectw(r,15,r,21+lisa,line+15,7,0,0);
         viwrrectw(r,22+lisa,r,64,line+22+lisa,0,7,0);
         }
      else {
         viwrrectw(r,0,r,4,line,0,7,0);
         viwrrectw(r, 5, r, 12, line+5, 7,0,0);
         viwrrectw(r,13,r,64,line+13,0,7,0);
         }
#ifdef EMIT
      if (line[64] == L'I')
         viwrrectw(r,64,r,64,line+64,7,0,0);
#endif
      }
   else {
      viwrrectw(r,0,r,64,line,7,0,0);
#ifdef EMIT
      if (line[64] == L'I')
         viwrrectw(r,64,r,64,line+64,0,7,0);
#endif
      }
   }

static void tulostamaali(void)
   {
   static INT alku[MAXJONO] = {1, 1};
   static INT rivi[MAXJONO];
   wchar_t tc, line[86],*ss,tark, *p, wbuf[80];
   INT ptime, ip;
   aikatp pt;
   INT d;
   kilptietue kilp;
   INT32 tm;

	FILE *outfl = 0;
//	wchar_t ln[120];

	tc = L' ';
	wselectopt(L"T)iedostoon vai K)irjoittimelle", L"TK", &tc);
	if (tc == L'T') {
	   clrln(ySize-3);
		vidspwmsg(ySize-3, 0, 7, 0, L"Tiedostoon:");
		wcscpy(line, L"ajat.txt");
		inputwstr(line, 30, 13, ySize-3, L"\x1B\r", &tc, 0);
		if (tc == KB_CR) {
			AjatTiedostoon(line, akttm);
			}
		return;
		}

   clrln(ySize-3);
   viwrrectw(ySize-3,0,ySize-3,39,L"Alkaen numerosta :         Esc : Peruuta",7,0,0);
   INPUTINTW(&alku[akttm],4,19,ySize-3,L"\x1B\xD",&tc);
   if (tc != ESC) {
	  p = 0;
	  if (lsttulparam.printer == LJETII) 
		 p = initfont_emva;
	  if (lsttulparam.printer == GDIPRINTER)
		 p = L"C";
	  initpage(&lsttulparam, p);
      tc = 1;
      for (ptime = alku[akttm]-1; ptime - aikajono[akttm]->aktrows < 0;
         ptime++, alku[akttm]++) {
         if (rivi[akttm] == 0 || tc == 1) {
            tc = 2;
            sendwline(L"Tulostettu ",lsttulparam.lstf);
            sendwline(wkello(),lsttulparam.lstf);
            sendwline(L"\r\n\n",lsttulparam.lstf);
            rivi[akttm] += 2;
            }
         wcscpy(line, L"      ");
         itow1(line,alku[akttm],4);
         sendwline(line,lsttulparam.lstf);
		 if (aikajono[akttm]->getAktTime(&pt, ptime)->t > TMAALI0*AIKAJAK) {
            ss = AIKATOWSTRS(wbuf, purajak(pt.t), t0);
            wbuf[pvparam[k_pv].laika] = 0;
            sendwline(wbuf,lsttulparam.lstf);
            }
		 ip = va_piste(pt.piste);
			swprintf(line, L"Emit:%7ld, Lähde:%5d %c", pt.badge, (int)pt.kanava, 
				pt.tapahtuma < 4 ? L"ADLT"[pt.tapahtuma] : pt.tapahtuma+L'0');
			sendwline(line,lsttulparam.lstf);
         if ((d = getpos(pt.kno)) > 0) {
            kilp.GETREC(d);
            ss[0] = 0;
            if (ip >= 0) {
               ss = AIKATOWSTRS(wbuf, kilp.p_aika(kierrosva[ip+2]),0);
               tark = L' ';
               if (pt.t > TMAALI0*AIKAJAK) {
                  tm = NORMKELLO(kilp.maali(kierrosva[ip+2]) -
                     purajak(pt.t));
                  if (tm/pvparam[k_pv].pyor[2])
                     tark = L'*';
                  }
               }
            else if (pvparam[k_pv].hiihtolahto && ip == -1) {
               tm = NORMKELLO(kilp.p_aika(-1) - kilp.pv[k_pv].tlahto);
               if (tm >= 0)
                  swprintf(ss,L"+%6.6s ", AIKATOWSTRS(wbuf, tm,0)+4);
               else
                  swprintf(ss,L"-%6.6s ", AIKATOWSTRS(wbuf,-tm,0)+4);
               if (NORMKELLO(kilp.p_aika(-1) - purajak(pt.t))/pvparam[k_pv].pyor[2] != 0)
                  tark = L'*';
               }
            swprintf(line, L" %c %4d %-20.20s %-10.10s %-10.10s %s%c",
               kilp.tark(k_pv), pt.kno, kilp.nimi(wbuf, 22, 0), kilp.seuratieto(0),
               Sarjat[kilp.Sarja()].sarjanimi, ss, tark);
            sendwline(line,lsttulparam.lstf);
            }
         sendwline(L"\r\n",lsttulparam.lstf);
         rivi[akttm]++;
         if (rivi[akttm] >= lsttulparam.tulmuot.sivpit) {
			endpage(&lsttulparam);
            rivi[akttm] = 0;
            }
         }
      if (rivi[akttm] < lsttulparam.tulmuot.sivpit - 8)
		 endpage(&lsttulparam);
      rivi[akttm] = 0;
 	  enddoc(&lsttulparam);
	  LeaveCriticalSection(&print_CriticalSection);
     }
   }

static int jakorivi;

void naytamaali(INT *r1, INT *r0, INT k)
   {
   int perusrivi;
   INT r2, r02, n1, n2, r, r_alin = ySize-5;
   static INT k0 = 1;

   perusrivi = (jakorivi-2)/2;
   if (ennakoi)
      r_alin = ySize-6;
   if (k) k0 = k;
   *r1 = aikajono[akttm]->rtime;
   r2 = aikajono[akttm]->aktrows;
   if (r2 < *r1 + r_alin - perusrivi-1) {
      *r0 = max(1, r2 - r_alin+3) - 1;
//      r02 = *r0 + 10;
      n1 = r2 - *r0 + 1;
      n2 = 0;
      for (r = n1+2; r < r_alin+1; r++) 
         naytarivi(-1,r,-1,-1,k0);
      }
   else {
      *r0 = max(0, *r1 - perusrivi);
      r02 = r2 - r_alin + jakorivi+1;
      n1 = jakorivi - 2;
      n2 = r_alin - jakorivi;
      naytarivi(-1, jakorivi,-1,-1,k0);
      }
   for (r = 0; r < n1; r++) 
      naytarivi(*r0 + r, r + 2, *r1-*r0, r2-*r0,k0);
   for (r = 0; r < n2; r++) 
      naytarivi(r02 + r, r + jakorivi+1, *r1-*r0, r2-r02+jakorivi-1,k0);
   }

static wchar_t tulk(wchar_t ch, INT key)
   {
   if (ch == 2 && key == 48)    // Ctl-B
	  return(249);
   if (ch == 6 && key == 33)    // Ctl-F
	  return(247);
   if (ch == 7 && key == 34)    // Ctl-G
	  return(243);
   if (ch == 8 && key == 35)    // Ctl-H
	  return(234);
   if (ch == 9 && key == 23)    // Ctl-I
	  return(241);
   if (ch == 11 && key == 37)    // Ctl-K
	  return(246);
   if (ch == 15 && key == 24)    // Ctl-O
	   return(261);
   if (ch == 16 && key == 25)    // Ctl-P
	  return(231);
   if (ch == 17 && key == 16)    // Ctl-Q
	   return(263);
   if (ch == 18 && key == 19)    // Ctl-R
	  return(235);
   if (ch == 19 && key == 31)    // Ctl-S
	  return(233);
   if (ch == 20 && key == 20)    // Ctl-T
	   return(262);
   if (ch == 21 && key == 22)    // Ctl-U
	  return(240);
   if (ch == 22 && key == 47)    // Ctl-V
	  return(232);
   if (ch == 23 && key == 17)    // Ctl-W
	  return(250);
   if (ch == 25 && key == 21)    // Ctl-Y
	  return(260);
   if (ch == 26 && key == 44)    // Ctl-Z
	   return(261);
   if (ch) return(towupper(ch));
   switch (key) {
      case 15 : ch = 11; break;       /* backtab */
      case 59 :                         /* F1 */
      case 60 :                         /* F2 */
      case 61 :                         /* F3 */
      case 62 :                         /* F4 */
      case 64 :                         /* F6 */ 
      case 65 :                         /* F7 */ 
      case 66 :                         /* F8 */ 
      case 68 : ch = key + 142; break;  /* F10   201-210  */   
      case 19 : ch = 235; break;        /* Alt-R */
      case 35 : ch = 234; break;        /* Alt-H */
      case 22 : ch = 240; break;        /* Alt-U */
      case 25 : ch = 231; break;        /* Alt-P */
      case 30 : ch = 241; break;        /* Alt-A */
#ifdef AIKAVERT
		case 31 : ch = 233; break;        /* Alt-S */
		case 47 : ch = 232; break;        /* Alt-V */
#endif
#ifdef EMITHTTP
	  case 17 : ch = 250; break;        /* Alt-W */
#endif
      case 48 : ch = 249; break;        /* Alt-B */
	  case 33 : ch = 247; break;		/* Alt-F */
      case 38 : ch = 242; break;        /* Alt-L */
      case 34 : ch = 243; break;        /* Alt-G */
      case 129: ch = 244; break;        /* Alt-0 */
      case 45 : ch = 245; break;        /* Alt-X */
      case 37 : ch = 246; break;        /* Alt-K */
      case 107: ch = 248; break;        /* Alt-F4 */
      case 109: ch = 236; break;        /* Alt-F6 */
      case 111: ch = 238; break;        /* Alt-F8 */
      case 63 :                         /* F5  */
      case 72 : ch = 5;
                break;                  /*up*/
      case 67 :                          /* F9 */
      case 80 : ch = 24;
                break;               /*down*/
      case 73 : ch = 18; break;      /*pgup*/
      case 81 : ch = 3; break;      /*pgdn*/
      case 118: ch = 2; break;     /*Ctrl-PgDn*/
      case 132: ch = 19; break;     /*Ctrl-PgUp*/
      }
   return(ch);
   }

void aikakorj(int jono)
	{
	wchar_t ch;

	clrln(ySize-3);
	vidspwmsg(ySize-3, 0, 7, 0, L"Anna aktiivisen jonon aikakorjaus        (1/100 s)");
	inputlongw(&aikajono[jono]->aikakorjaus, 5, 34, ySize - 3, L"\r\x1b", &ch);
	}

void ajanottoheader(int akttm)
{
   static wchar_t headerline[30];

// #if defined(UINTI) || defined(MERI5)
  if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
	   header = L"ERÄ     ";
	  }
// #else
   else {
	   if (!akttm || !lajatfl) {
		  wcscpy(headerline, L"AJANOTTO  ");
		  if (maxjono) {
			headerline[9] = L'1'+akttm;
			}
		  if (vainoletus) {
			  if (ol_piste == 0)
				 wcscat(headerline, L" - Maali");
			  else if (ol_piste > 0)
				 swprintf(headerline+wcslen(headerline), L" - Piste : %d", ol_piste);
			  }
		  header = headerline;
		  }
	   else
		  header = L"LÄHTÖAJAT";
	}
// #endif
   clrtxt(0,0,25);
   vidspwmsg(0,0,7,0,header);
}

//extern "C" int writeerrorOn;

void ajanotto()
   {
   INT r1, r0, k, l, key, kno, ekno, edkno, d, de, uusi, x[2] = {18,5};
   INT edpiste, piste, piste0, vainrivi, lisa = 0;
   wchar_t tc, ch, ch2, line[86], as[40], st[40] = L" ", wbuf[80];
   char tce;
   kilptietue kilp, entkilp;
   INT32 tm, tm0;
   aikatp pt;
   INT edaika, htime;
   static aikatp vert_time, utime;
   static INT askel=1;
   INT s_aika = 0, haku = 0;
#ifdef HIIHTOLAHTO
   INT32 tl;
	INT pst;
#endif
#ifdef ENNAKOI
   int njono, jkno = 0, jono[5], j;
   int maxennjono = 5;
#endif

   clrscr();
   ajanottoheader(akttm);
   kehys(1);
// #if defined(UINTI) || defined(MERI5)
   if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
		vidint(0, 4, 3, era_akt);
		}
//#endif
   jakorivi=3*ySize/4-7;
   maali_on = 1;
   lukum_fl = !lajat_p;
   ajanottofl = 1;
   if (aikajono[0] == NULL || aikajono[0]->atime == NULL) {
      ch = L' ';
      wselectopt(L"Ajanottoa ei käynnistetty, käynnistetääkö (K/E)", L"KE", &ch);
      if (ch == L'E')
         return;
      inittimer();
      }
   if (kilpparam.maxvaluku > 9)
		lisa = 1;
   if (kilpparam.maxvaluku > 99)
		lisa = 2;
   k = 1;
   do {
      tc = 0;
      ch = 0;
	  tm0 = 0;
	  vainrivi = 0;
#if !defined(AMPUSUUNN)
#if !defined(LVAYLAX)
      viwrrectw(ySize-3,0,ySize-2,79,
         L"Up Dn PgUp PgDn Alt-R: vaihda riviä, "
         L"Tab : vaihda kenttää, Ret : hyväksy,       "
         L"F2/F3 : lisää/poista rivi, F4 : testitarra"
         L", F6/F8 : ota seur./edell. kilpailija,",7,0,0);
#else
      viwrrectw(ySize-3,0,ySize-2,79,
         L"Up Dn PgUp PgDn Alt-R: vaihda riviä,     "
         L"Tab : vaihda kenttää, Ret : hyväksy,   "
         L"F2/F3 : lisää/poista rivi, F4 : lisää 10 s"
         L", F6/F8 : ota seur./edell. kilpailija,",7,0,0);
#endif
#else
      viwrrectw(ySize-3,0,ySize-2,79,
         L"Up Dn PgUp PgDn Alt-R: vaihda riviä,     "
         L"Tab : vaihda kenttää, Ret : hyväksy,   "
         L"F2/F3 : lisää/poista rivi"
         L", F6/F8 : ota seur./edellinen kilpailija,              ",7,0,0);
#endif
	  if (writeerrorOn == 0)
		  viwrrectw(ySize - 1, 0, ySize - 1, 58, 
			  L"F10 : tulosta, Esc : peruuta ja poistu                     ",
			  7,0,0);
      naytamaali(&r1, &r0, k);
      Sleep(50);
      sccurset(r1-r0+2, x[k-1]);
	  if (vaiheenvaihto || !taustaon)
		  break;
      muut_time = aikajono[akttm]->mxtime;
	  aikajono[akttm]->getAktTime(&vert_time, aikajono[akttm]->rtime);
      od_fl = 0;
      if ((s_aika && k == 2) || (kbready(&tce,&key) && (tc = tce) != ESC)) {
		  tc = tce;
         s_aika = 0;
         switch (k) {
            case 1 :
               uusi = 0;
               do {
                  d = 0;
#ifdef ENNAKOI
                  if (jkno) {
                     kno = jkno;
                     piste = 0;
                     }
                  else
#endif
                     {   
					 kno = aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime)->kno;
                     piste = va_piste(pt.piste);
                     }
                  od_fl = (vert_time.t == TMAALI0*AIKAJAK);
                  utime.t= vert_time.t;
				  utime.date = vert_time.date;
                  od_aika = TMAALI0*AIKAJAK;
                  if ((tc >= L'0' && tc <= L'9') || (tc == 8 && key == 14) ||
                     (tc == 0 && key == 75)) {
                     if (vainoletus)
                        piste = akttm ? -1 : ol_piste;
                     else {
					    wcscpy(st, L"   ");
                        if (piste < -1) piste = ol_piste;
								st[3] = 0;
					  if (kilpparam.maxvaluku < 10)
                          st[2] = ch_piste[piste+2];
					  else {
							if (piste < 10)
						        st[2] = ch_piste[piste+2];
							else {
								if (piste > 99)
									st[0] = L'0'+piste/100;
								st[1] = L'0'+(piste/10)%10;
								st[2] = L'0'+piste%10;
								}
					      }
                        viwrrectw(r1-r0+2,19,r1-r0+2,21,st,7,0,0);
                        }
                     input_now(&kno, 15, r1-r0+2,
                           L"+sS\r\t\x1B\x0B\xCA\xCB\xCC"
                           L"\x05\x18\x12\x03\xCE\xD0\xD2\x04\x15\x02\x13",&tc);
                     if (vainoletus || kno == 0) 
                        piste = (lajatfl && akttm) ? -1 : ol_piste;
                     if (maxjono && jonopiste[akttm] && kno == 0)
                        piste = jonopiste[akttm];
                     }
                  else {
                     tc = tulk(tc, key);
                     kbflush();
                     }
#ifdef SIIMPORT
					  if (tc == 247) {
						  si_tcpimport();
						  tc = 0;
						  goto pois;
						  }

#endif
				if (tc == L'S') {
					int srj1;
					clrln(ySize - 2);
					clrln(ySize - 1);
					vidspwmsg(ySize - 3, 0, 7, 0, L"Anna lähtöajan sarj                                       ");
					srj1 = luesarja(0, &ch);
					if (ch != ESC && srj1 >= 0) {
						ch = L' ';
						wselectopt(L"Sarjan kaikkien osanottajien lähtöaika vaihdetaan. Vahvista (K/E)", L"KE", &ch);
						if (ch == L'K') {
							asetaSrjLahto(srj1, purajak(utime.t), false, true, false);
							utime.kno = 30001 + srj1;
							kno = utime.kno;
							utime.piste = -1;
							tc = 0;
							utime.kanava = vert_time.kanava;
							utime.jono = vert_time.jono;
							tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0, false);
							}
						}
					continue;
					}
				if (tc == L'R') {
					int ryh = 0;

					vidspwmsg(r1 - r0 + 2, 17, 7, 0, L"     Eränumero lähtöajalle     ");
					inputlongw(&ryh, 4, 17, r1 - r0 + 2, L"+\r\x1b", &ch);
					if (ch != ESC && ryh > 0) {
						ch = L' ';
						wselectopt(L"Erän kaikkien osanottajien lähtöaika vaihdetaan. Vahvista (K/E)", L"KE", &ch);
						if (ch == L'K') {
							asetaEraLahto(ryh, purajak(utime.t), true, false);
							utime.kno = 31001 + ryh;
							kno = utime.kno;
							utime.piste = -1;
							tc = 0;
							utime.kanava = vert_time.kanava;
							utime.jono = vert_time.jono;
							tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0, false);
							}
						}
					continue;
					}
				if (tc == L'T') {
                     kno = 10000;
                     piste = 0;
                     tc = KB_CR;
                     }
                  if (tc == 206 || tc == 208) {
                     viwrrectw(ySize-1,0,ySize-1,41,
                        L"Siirtoaskel =      (Alt-F6 : Vaihda askel)",7,0,0);
                     vidint(ySize-1, 14, 2, askel);
                     }
                  if (tc == 231) {
					 wchar_t pst[8];
                     clrln(ySize-3);
                     vidspwmsg(ySize-3,0,7,0,L"Anna käsiteltävä piste (V: valittavissa, A: automaattinen):");
					 if (!vainoletus)
						wcscpy(pst, L"A");
					 else {
						switch (ol_piste) {
							case -1:
								wcscpy(pst, L"L");
								break;
							case 0:
								wcscpy(pst, L"M");
								break;
							default:
								_itow(ol_piste, pst, 10);
								break;
							}
						}	
					inputwstr(pst, 3, 60, ySize-3, L"\r", &ch, 0);
					switch (towupper(pst[0])) {
						case L'A':
							ol_piste = -2;
						case L'V':
							vainoletus = 0;
							break;
						case L'L':
							ol_piste = -1;
							vainoletus = 1;
							break;
						case L'M':
							ol_piste = 0;
							vainoletus = 1;
							break;
						default:
							ol_piste = _wtoi(pst);
							vainoletus = 1;
							break;
						}
                     tc = 0;
					 ajanottoheader(akttm);
                     goto pois;
                     }
                  if (tc == 234) {
                     clrln(ySize-3);
                     vidspwmsg(ySize-3,0,7,0,L"Anna haettava numero:");
                     INPUTINTW(&haku, 4, 22, ySize-3, L"\r", &ch);
                     }
                  if (tc == 244)
                     haku = 0;
                  if (tc == 234 || tc == 243 || tc == 244) {
                     htime = aikajono[akttm]->rtime+1;
                     if (aikajono[akttm]->aktrows - htime <= 0)
                        htime = 0;
                     for (; htime <= aikajono[akttm]->aktrows; htime++)
						if (haku == aikajono[akttm]->getAktTime(0, htime)->kno) break;
					 if (haku != aikajono[akttm]->getAktTime(0, htime)->kno)
                        for (htime = 0; htime <= aikajono[akttm]->rtime; htime++)
							if (haku == aikajono[akttm]->getAktTime(0, htime)->kno) break;
					 if (haku != aikajono[akttm]->getAktTime(0, htime)->kno)
                        writeerror_w(L"EI LÖYDY",0);
                     else aikajono[akttm]->rtime = htime;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 235) {
                     clrln(ySize-3);
                     d = (int) (aikajono[akttm]->rtime+1);
                     vidspwmsg(ySize-3,0,7,0,L"Anna rivi:");
                     INPUTINTW(&d, 4, 11, ySize-3, L"\r", &tc);
                     d = max(d, 1);
                     aikajono[akttm]->rtime = d - 1;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 245) {
                     haevirhe();
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 252) {			// 
					 pollaa = 1;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 246) {
                     maalikorj();
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 251) {			// Alt-J
                     aikakorj(akttm);
                     tc = 0;
                     goto pois;
                     }
				  if (tc == 261) {			// Ctrl-O
					  utime = vert_time;
					  utime.status = 1;
					  tall_rivi(akttm, &utime, &vert_time, NULL, 0, 0, 0, false);
					  tc = 0;
					  goto pois;
				     }
				  if (tc == 262) {			// Ctrl-T
					  utime = vert_time;
					  utime.status = 0;
					  tall_rivi(akttm, &utime, &vert_time, NULL, 0, 0, 0, false);
					  tc = 0;
					  goto pois;
					  }
				  if (tc == 263) {			// Ctrl-Q
					  aikajono[akttm]->atimemask = aikajono[akttm]->atimemask ? 0 : '\xff';
					  aikajono[akttm]->haeAktAjat();
					  tc = 0;
					  goto pois;
				      }
#ifdef AIKAVERT
						if (tc == 232) {
							aikavert();
                     tc = 0;
						   clrscr();
						   kehys(1);
                     goto pois;
                     }
#endif
#ifdef LAJUNENx
						if (tc == 233) {
							rem_regnly(0);
                     openSirit(0);
                     tc = 0;
                     goto pois;
                     }
#endif
						if (tc == 236 || tc == 238) {
                     clrln(ySize-3);
                     vidspwmsg(ySize-3,0,7,0,L"Anna siirtoaskeleen pituus:");
                     INPUTINTW(&askel, 2, 28, ySize-3, L"\r", &tc);
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 239) {  // Alt-P
					 pistetunn();
					 ajanottoheader(akttm);
//                     val_piste(&ol_piste, VALUKU, 1);
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 260) {  // Ctl-Y
					 SiritSync(true);
                     tc = 0;
                     goto pois;
                     }
#ifdef EMITHTTP
						if (tc == 250) {		// Alt-W
							ch = L' ';
							wselectopt(L"Väliaikojen haku: J)atka, K)eskeytä, hae A)lusta uudelleen", L"JKA", &ch);
							switch (ch) {
								case L'J':
									eTParam.eThaku = 1;
									break;
								case L'K':
									eTParam.eThaku = 0;
									break;
								case L'A':
									eTParam.eThaku = -1;
									break;
								}


                     tc = 0;
                     goto pois;
                     }
#endif
                  if (tc == 22 && akttm == 0) {
					 seurkrivi = aikajono[0]->rtime;
                     tc = 0;
                     goto pois;
                     }
                  if (!esta_tall && tc == 206) {
                     if ((aikajono[akttm]->rtime + askel - aikajono[akttm]->mxtime) <= 0) {
                        edaika = aikajono[akttm]->rtime+askel;
						kno = aikajono[akttm]->getAktTime(&pt, edaika)->kno;
                        edkno = kno;
                        piste = va_piste(pt.piste);
                        edpiste = piste;
                        }
                     else tc = 0;
                     }
                  else if (!esta_tall && tc == 208) {
                     if (aikajono[akttm]->rtime < askel) {
                        tc = 0;
                        }
                     else
                        {
						 aikatp pt1;
						 aikajono[akttm]->getAktTime(&pt1, aikajono[akttm]->rtime);
						 for (edaika = aikajono[akttm]->rtime - askel; edaika > 0; edaika--) {
							aikajono[akttm]->getAktTime(&pt, edaika);
							if (pt.date != pt1.date || pt.t != pt1.t)
								break;
						    }
						if (pt.date == pt1.date && pt.t == pt1.t)
                           tc = 0;
                        else {
							edkno = aikajono[akttm]->getAktTime(&pt, edaika)->kno;
                           edpiste = va_piste(pt.piste);
                           ch = L'K';
                           if (aikajono[akttm]->rtime - edaika > askel) {
                              ch = L' ';
                              wselectopt(L"Edellinen maaliaika sama, poimitaanko"
                                 L" viimeisin eroava (K/E)", L"KE", &ch);
                              if (ch == L'E') tc = 0;
                              }
                           if (ch == L'K') {
                              kno = pt.kno;
                              piste = va_piste(pt.piste);
                              }
                           }
                        }
                     }
                  else if (tc == 241) {    // Alt-A tai Ctl-I
                     if (maxjono) {
                        akttm = (akttm + 1) % (maxjono+1);
						ajanottoheader(akttm);
                        }
                     if (lajatfl) {
                        akttm = 1 - akttm;
                        if (akttm) viwrrectw(0,0,0,8,L"LÄHTÖAJAT",7,0,0);
                        else viwrrectw(0,0,0,8,L"AJANOTTO ",7,0,0);
                        }
                     }
                  else if (tc == 242) {
                     if (lajatfl|| taustaaika) lajat_p = 1 - lajat_p;
                     lukum_fl = !lajat_p;
                     lukumaarat();
                     }
                  else if (tc != 240 && tc != 249) 
					  uusi = 1;
                  if (tc != KB_CR && tc != L'*' && tc != 206 && tc != 208
                     && tc != 240&& tc != 249) {
                     if (tc == ESC) tc = 0;
                     goto pois;
                     }
                  if(kno < minkilpno || kno > maxkilpno) {
                     if (kno != 0 && kno != 10000) {
                        tc = 255;
                        writeerror_w(L"Numero liian suuri tai pieni",0);
                        }
                     }
                  else {
                     if ((d = getpos(kno)) <= 0) {
                        tc = 255;
                        writeerror_w(L"EI LÖYDY",2000);
                        }
                     }
                  } while (tc == 255);
               if (kno == 10000) {
                  viwrrectw(r1-r0+2, 22, r1-r0+2, 38, L"TAHDISTUSAIKA    ",
                     0,7,0);
                  }
               else if (!esta_tall && kno >= minkilpno && kno <= maxkilpno) {
                  kilp.GETREC(d);
				  aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime);
                  if (tc != 240 && tc != 249 && kno != pt.kno && piste == -2) {
                     piste = maaraa_piste(&kilp, purajak(pt.t));
                     }
                  if (vainoletus) piste0 = ol_piste;
                  if (tc != 240 && tc != 249 && !vainoletus) {
                     do {
						if (kilpparam.maxvaluku < 10) {
	                        st[0] = ch_piste[piste+2];
		                    inputwstr(st, 1, 20, r1-r0+2, L"+\r\x1B",
			                   &tc,0);
						    }
						else {
							wcscpy(st, str_piste(piste));
							inputwstr(st, 3, 19, r1-r0+2, L"+\r\x1B",
								&tc,0);
							}
						if (tc == ESC) break;
						if (kilpparam.maxvaluku >= 10 && (piste0 = _wtoi(st)) >= 10) {
							if (piste0 <= kilpparam.maxvaluku) {
								piste = piste0;
								break;
								}
							else erbeep();
							}
						else
							{
							st[0] = towupper(st[0]);
#if !defined(AMPUSUUNN)
	                        if ((piste0 = wcswcind(st[0], ch_piste)) > 0
		                        && piste0 <= Sarjat[kilp.Sarja()].valuku[k_pv]+2) {
#else
			                if ((piste0 = wcswcind(st[0], ch_piste)) > 0
				                  && piste0 <= 4) {
#endif
					             piste = piste0 - 2;
						         break;
								 }
	                        else 
								erbeep();
							}
                        } while (1);
                     }
                  if (tc == ESC) break;
                  wcscpy(as,L"       ");
                  if (kierrosva[piste+2] >= 0) {
                     if (pt.t > TMAALI0*AIKAJAK) {
                        wmemcpy(as,AIKATOWSTRS(wbuf, kilp.tulos1(kierrosva[piste+2],
                           purajak(pt.t)),0) + 1, 7); 
                        }
                     }
                  else if (pvparam[k_pv].hiihtolahto) {
                     utime.t = pt.t;
					 utime.date = pt.date;
                     tm = NORMKELLO(purajak(pt.t) - kilp.pv[k_pv].tlahto);
                     if (tm >= 0)
                        swprintf(as,L"+%6.6s", AIKATOWSTRS(wbuf, tm,0)+4);
                     else
                        swprintf(as,L"-%6.6s", AIKATOWSTRS(wbuf, -tm,0)+4);
                     }
				  if (kilpparam.maxvaluku >= 10 && !vainoletus)
		              swprintf(line, L"%2d% -16.16s %-7.7s %-8.8s %7.7s",
			              piste, kilp.nimi(wbuf, 30, 0),
				          kilp.seuratieto(0), Sarjat[kilp.Sarja()].sarjanimi,as);
				  else
				      swprintf(line, L"%c %-17.17s %-7.7s %-8.8s %7.7s",
					      vainoletus ? L' ' : ch_piste[piste+2], kilp.nimi(wbuf, 30, 0),
					      kilp.seuratieto(0), Sarjat[kilp.Sarja()].sarjanimi,as);
                  viwrrectw(r1-r0+2, 20, r1-r0+2, 63, line, 0,7,0);
				  tm = purajak(pt.t);
                     tm = pyoristatls(tm , 1);
						if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1) {
							if (!pakotalaika && pt.t != TMAALI0*AIKAJAK &&
								NORMKELLO(tm - kilp.pv[k_pv].tlahto) - tl_ylaraja > 0) {
								ch = L'K';
								erbeep();
								wselectopt(L"Asetetaanko ajaksi peruslähtöaika (K/E)",
									L"KE",&ch);
								if (ch == L'K') 
									utime.t = kilp.pv[k_pv].tlahto*AIKAJAK;
								utime.date = tm_date(utime.t);
								tm = kilp.pv[k_pv].tlahto;
								}
							if (NORMKELLO(kilp.p_aika(-1) - kilp.pv[k_pv].tlahto) != 0 &&
								NORMKELLO(tm - kilp.p_aika(-1)) != 0) {
								ch = L' ';
								wselectopt(L"Lähtöaika jo annettu, muutetaanko (K/E)",
									L"KE", &ch);
								if (ch == L'E') break;
								}
						}
                  if (tc != ESC) {
                     if (kierrosva[piste+2] >= 0 && uusi && (kilp.p_aika(kierrosva[piste+2])
                        || !kilp.hyv())) {
                        ch = L' ';
                        erbeep();
                        switch (kilp.tark(k_pv)) {
                           case L'-':
                           case L'I':
                           case L'T':
                              wcscpy(line,
                                 AIKATOWSTRS(wbuf, kilp.p_aika(kierrosva[piste+2]),0)+1);
                              break;
                           case L'K':
                              wcscpy(line, L"KESKEYT");
                              break;
                           case L'H':
                              wcscpy(line, L"HYLÄTTY");
                              break;
                           case L'E':
                           case L'P':
                              wcscpy(line, L"EI LÄHT");
                              break;
                           }
                        viwrrectw(r1-r0+2, 57, r1-r0+2, 63, line, 0,7,0);
						vainrivi = 0;
						if (muutoskysely)
		                    wselectopt(L"Tulos jo annettu, muutetaanko (K/E)", L"KE",
	                           &ch);
						else {
							ch = L'K';
							vainrivi = 1;
							}
                        if (ch == L'E') {
                           tc = 0;
                           break;
                           }
                        if (kilp.tark(k_pv) != L'T' && kilp.tark(k_pv) != L'I')
                           kilp.set_tark(L'-', k_pv);
                        viwrrectw(r1-r0+2, 57, r1-r0+2, 63,
                           AIKATOWSTRS(wbuf,kilp.tulos1(kierrosva[piste+2],
                           purajak(pt.t)), 0)+1,0, 7, 0);
                        }
                     }
                  ch = L' ';
                  ch2 = 0;
                  if (!esta_tall && vert_time.t == TMAALI0*AIKAJAK) {
                     if (syotto) {
                        do {
                           if (piste < 0) piste = 0;
                           tmaali = maaliaika(tmaali, t0, r1-r0+2, 5, &ch2);
                           if (!ch2) {
                              viwrrectw(r1-r0+2, 5, r1-r0+2, 12,
                                 AIKATOWSTRS(wbuf,tmaali, t0), 0,7,0);
                              viwrrectw(r1-r0+2, 57, r1-r0+2, 63,
                                 AIKATOWSTRS(wbuf,kilp.tulos1(kierrosva[piste+2], tmaali),
                                 0)+1, 0, 7, 0);
                              s_aika = 1;
                              }
                           ch = ch2;
                           } while (ch2 && ch2 != L'H' && ch2 != L'K' &&
                              ch2 != L'E' && ch2 != ESC);
                        }
                     else {
                        vidspwmsg(ySize-3,0,0,7, L"Odottaa aikaa.     Paina <Esc> "
                           L"keskeyttääksesi odotus.");
#ifdef ENNAKOI
                        njono = 0;
                        jkno = 0;
#endif
                        while (ch != ESC) {
                           if (od_aika != TMAALI0*AIKAJAK) {
                              viwrrectw(r1-r0+2, 5, r1-r0+2, 12,
                                 AIKATOWSTRS(wbuf,purajak(od_aika), t0), 0,7,0);
                              if (kierrosva[piste+2] >= 0)
                                 viwrrectw(r1-r0+2, 57, r1-r0+2, 63,
                                    AIKATOWSTRS(wbuf,kilp.tulos1(kierrosva[piste+2],
                                    purajak(od_aika)),0)+1, 0, 7, 0);
#ifdef ENNAKOI
//                              if (!njono)
                              if (!ennakoi)
#endif
                                 break;
                              }
                           Sleep(50);
						   if (!taustaon || vaiheenvaihto) {
							   ch = ESC;
							   break;
							   }
#ifdef ENNAKOI
                           sccurset(ySize-5, 18+6*njono);
#endif
                           ch = readkbd(&tce,FALSE,spoolfl);
						   ch2 = tce;
#ifdef ENNAKOI
                           if (ch == ESC) {
                              clrtxt(ySize-5,0,64);
                              break;
                              }
                           if (ennakoi) {
                              if (ch >= L'0' && ch <= L'9') {
                                 jkno = 10*jkno + ch - L'0';
                                 }
                              if (ch == 8) jkno = jkno/10;
                              if (jkno && ch == L'\r') {
                                 if (njono < maxennjono) {
                                    jono[njono] = jkno;
                                    njono++;
                                    }
                                 jkno = 0;
                                 }
                              for (j = 0; j < njono; j++)
                                 vidint(ySize-5,15+6*j,4,jono[j]);
                              if (jkno) {
                                 wmemset(line, L' ', 5);
                                 putintrightw(jkno, line+3);
                                 viwrrectw(ySize-5, 15+6*njono, ySize-5, 18+6*njono, line, 0, 7, 0);
                                 }
                              clrtxt(ySize-5, 15+6* (njono + (jkno!=0)) , 64);
                              if (od_aika != TMAALI0*AIKAJAK) {
                                 aikajono[akttm]->rtime = odota;
                                 vert_time.t = od_aika;
                                 vert_time.piste = ol_piste;
                                 utime.t = vert_time.t;
								 vert_time.date = tm_date(vert_time.t);
								 utime.date = vert_time.date;
				                 utime.jono = vert_time.jono;
                                 utime.kno = kno;
                                 utime.piste = piste;
                                 if (tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0, 0, false))
                                    break;
                                 if((d = getpos(kno)) > 0) {
									if (!vainrivi) {
										 EnterCriticalSection(&tall_CriticalSection);
										kilp.GETREC(d);
										if (muutoskysely || kilp.p_aika(kierrosva[piste+2]) == 0) {
											entkilp = kilp;
											tm = purajak(utime.t);
										    tm = pyoristatls(tm , 1);
											kilp.set_tulos( kierrosva[piste+2], tm);
											pkno = aikajono[akttm]->rtime + 1;
											kilp.tallenna(d,TRUE, 0, 0, &entkilp);
											}
										 LeaveCriticalSection(&tall_CriticalSection);
										}
                                    pkno = 0;
                                    naytamaali(&r1, &r0, k);
                                    if (njono) vidint(ySize-6,15,4,jono[0]);
                                    tc = 255;
                                    }
                                 else {
                                    tall_rivi(akttm, &vert_time, &utime, NULL, 0, 0, 0, false);
                                    }
                                 clrtxt(ySize-5, 9+6*(njono + (jkno!=0)) , 64);
                                 if ((++aikajono[akttm]->rtime - aikajono[akttm]->aktrows) > 0) 
                                    aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
                                 if (njono) {
                                    kno = jono[0];
                                    if (kno < minkilpno || kno > maxkilpno)
                                       kno = 0;
                                    vert_time.t = TMAALI0*AIKAJAK;
                                    od_aika = TMAALI0*AIKAJAK;
                                    njono--;
                                    for (j = 0; j < njono; j++)
                                       jono[j] = jono[j+1];
//                                    for (j = 0; j < njono; j++)
//                                       vidint(ySize-5,15+6*j,4,jono[j]);
//                                    if (jkno)
//                                       vidint(ySize-5,15+6*njono,4,jkno);
                                    }
                                 else {
                                    tc = 0;
                                    goto pois;
                                    }
                                 }
                              }
#endif
                           }
                        }
                     }
                  }
			   if (kno >= 30000)
				  break;
               if (ch == ESC) break;
               if (tc == 240 || tc == 249) ch = L'+';
               else {
                  ch = L' ';
                  wselectopt(L" + : Tallenna,  <Esc> : Peruuta",L"+\x1b", &ch);
                  }
               if (!esta_tall && ch == L'+') {
                  if ((tc == 206 || tc == 208) &&
					 (aikajono[akttm]->getAktTime(&pt, edaika)->kno != edkno || va_piste(pt.piste) != edpiste)) {
                     writeerror_w(L"Siirto peruutettu muutosanoman takia",0);
                     break;
                     }
                  if (s_aika) {
                     utime.t = tmaali*AIKAJAK;
					 utime.date = tm_date(utime.t);
				  }
                  else {
                     if (od_fl && odota) {
                        aikajono[akttm]->rtime = odota;
                        vert_time.t = od_aika;
                        vert_time.piste = ol_piste;
                        utime.t = vert_time.t;
						tm = purajak(utime.t);
						tm = pyoristatls(tm , 1);
                        if (pvparam[k_pv].hiihtolahto && !pakotalaika && kierrosva[piste+2] == -1 &&
                           tm > (kilp.pv[k_pv].tlahto + tl_ylaraja)) {
                           ch = L'K';
                           wselectopt(L"Asetetaanko ajaksi peruslähtöaika (K/E)",
                              L"KE",&ch);
                           if (ch == L'K') 
										utime.t = kilp.pv[k_pv].tlahto*AIKAJAK;
                           }
						utime.date = tm_date(utime.t);
                        }
                     }
                  utime.kno = kno;
                  utime.piste = piste;
				  utime.kanava = vert_time.kanava;
                  utime.jono = vert_time.jono;
#if defined(COMETNO) || defined(EMIT)
                  utime.badge= vert_time.badge;
#endif
                  ekno = vert_time.kno;
				  if (ekno > 0) {
					  piste0 = va_piste(vert_time.piste);
					  }
                  if (tall_rivi(akttm, &utime, &vert_time, NULL, 1, 0,
                     tc == 240 || tc == 249, false)) break;
                  tm = purajak(utime.t);
#ifndef LVAYLAX
				  tm = pyoristatls(tm , 1);
#endif
                  if ((de = getpos(ekno)) > 0) {
					 EnterCriticalSection(&tall_CriticalSection);
                     kilp.GETREC(de);
                     entkilp = kilp;
                     if ((piste0 < 0 && kilp.p_aika(-1) == tm) ||
                        (piste0 >= 0 && kilp.maali(kierrosva[piste0+2]) == tm)) {
                        if (piste0 >= 0)
                           kilp.tall_tulos( kierrosva[piste0+2], 0);
                        else if (pvparam[k_pv].hiihtolahto)
                           poista_tlahto(&kilp);
						pkno = aikajono[akttm]->rtime + 1;
                        kilp.tallenna(de,TRUE, 0, tc == 240 || tc == 249, &entkilp);
                        pkno = 0;
                        }
					 LeaveCriticalSection(&tall_CriticalSection);
                     }
                  if (kno != 0 && kno < 10000) {
                     if((d = getpos(kno)) > 0) {
						EnterCriticalSection(&tall_CriticalSection);
                        kilp.GETREC(d);
                        entkilp = kilp;
                        if (kno == ekno && piste0 != piste) {
                           if (pvparam[k_pv].hiihtolahto && piste0 == -1 && kilp.p_aika(-1) == tm)
                              poista_tlahto(&kilp);
                           if (piste0 >= 0 && kilp.maali(kierrosva[piste0+2]) == tm)
                              kilp.tall_tulos( kierrosva[piste0+2], 0);
                           }
                        if (kierrosva[piste+2] >= 0)
                           kilp.set_tulos( kierrosva[piste+2], tm);
                        else if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1) {
                           kilp.tall_tulos(-1, tm);
                           }
                        pkno = aikajono[akttm]->rtime + 1;
                        if (ch2 == L'H' || ch2 == L'K' || ch2 == L'E')
                           kilp.set_tark(ch2, k_pv);
						if (!vainrivi)
	                        kilp.tallenna(d,TRUE, 0, 0, &entkilp);
                        pkno = 0;
						LeaveCriticalSection(&tall_CriticalSection);
                        if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1 && era_akt && kilp.p_aika(-1) != TMAALI0 &&
							eralahto[era_akt] != AIKAJAK*kilp.p_aika(-1)) {
							eralahto[era_akt] = AIKAJAK*kilp.p_aika(-1);
							lahetaeralahto(era_akt, 0);
							}
                        }
                     else {
	                    utime.jono = vert_time.jono;
                        tall_rivi(akttm, &vert_time, &utime, NULL, 0, 0, 0, false);
                        writeerror_w(L"EI LÖYDY!",2000);
                        tc = 0;
                        }
                     }
                  if (!esta_tall && (tc == 206 || tc == 208) && 
					 aikajono[akttm]->getAktTime(&pt, edaika)->kno != 0 &&
					 pt.kno == edkno && va_piste(pt.piste) == edpiste) {
                     vert_time = pt;
                     utime.t = vert_time.t;
					 utime.date = vert_time.date;
                     utime.jono = vert_time.jono;
#if defined(COMETNO) || defined(EMIT)
                     utime.badge= vert_time.badge;
#endif
                     utime.kno = 0;
                     utime.piste = -2;
                     tall_rivi(akttm, &utime, &vert_time, NULL, 0, 0, 0, false);
                     }
                  }
               else tc = 0;
               break;
            case 2 :
               d = 0;
			   aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime);
               piste = va_piste(pt.piste);
               if (pt.kno < 30000 && (d = getpos(pt.kno)) <= 0 && pt.kno) {
                  writeerror_w(L"KILPAILUNUMEROA EI LÖYDY!",2000);
                  tc = 0;
                  break;
                  }
               if (d > 0)
                  kilp.GETREC(d);
               entkilp = kilp;
               if (syotto && !pt.kno && pt.t == TMAALI0*AIKAJAK) {
                  do {
                     tmaali = maaliaika(tmaali, t0, r1-r0+2, 5, &ch2);
                     if (!ch2) {
                        viwrrectw(r1-r0+2, 5, r1-r0+2, 12,
                           AIKATOWSTRS(wbuf,tmaali, t0), 0,7,0);
                        s_aika = 1;
                        tc = KB_CR;
                        tm = tmaali;
                        }
                     } while (ch2 && ch2 != ESC);
                  }
               else {
                  tm = purajak(pt.t);
						tm0 = 0;
                  l = tm == TMAALI0 ? pvparam[k_pv].laika : LAIKA;
                  if ((tc >= L'0' && tc <= L'9') || (tc == 0 && key == 65)
                     || (tc == 0 && key == 66) || (tc == 0 && key == 77)) {
					 tm0 = tm;
					 tm0 = pyoristatls(tm0, 1);
                     INPUTAIKAW(&tm,t0,l,5,r1-r0+2,
                        L"hHkK-\r\t\x1B\x0B\xCA\xCB\xCC\xD2\x05\x18\x12\x03"
                        L"\x02\x13",&tc);
                      tm0 -= pyoristatls(tm , 1);
					  }
                  else {
                     tc = tulk(tc, key);
                     kbflush();
                     }
                  }
               if (tc == ESC) tc = 0;
               tc = towupper(tc);
               wcscpy(line,L"       ");
               if (kierrosva[piste+2] >= 0) {
                  switch (tc) {
                     case L'K':
                        wcscpy(line, L"KESKEYT");
                        break;
                     case L'H':
                        wcscpy(line, L"HYLÄTTY");
                        break;
                     case L'E':
                        wcscpy(line, L"EI LÄHT");
                        break;
                     case KB_CR :
                     case L'-':
                     case L'T':
                     case L'I':
                        if (d > 0) {
                           if (kierrosva[piste+2] >= 0) {
                              wcscpy(line,
                                 AIKATOWSTRS(st, kilp.tulos1(kierrosva[piste+2],tm), 0)+1);
                              }
                           }
                        break;
                     default :
                        goto pois;
                     }
                  }
               else if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1) {
                  tl = NORMKELLO(tm - kilp.pv[k_pv].tlahto);
                  if (tl >= 0)
                     swprintf(line,L"+%6.6s", AIKATOWSTRS(wbuf,tl,0)+4);
                  else
                     swprintf(line,L"-%6.6s", AIKATOWSTRS(wbuf,-tl,0)+4);
                  }
               if (d > 0 && (tc != KB_CR || kilp.hyv()))
                  viwrrectw(r1-r0+2, 57, r1-r0+2, 63, line, 0, 7, 0);
               if (s_aika && tc == KB_CR) ch = L'+';
               else {
                  ch = L' ';
                  wselectopt(L" + : Tallenna,  <Esc> : Peruuta",L"+\x1b", &ch);
                  }
               if (!esta_tall && ch == L'+') {
                  switch (tc) {
                     case L'-':
                     case L'I':
                     case L'T':
                        kilp.set_tark(tc, k_pv);
                     case KB_CR :
                        utime.t = tm*AIKAJAK;
						utime.date = tm_copydate(utime.t, vert_time.t, vert_time.date);
						utime.jono = akttm;
                        utime.kno = vert_time.kno;
                        utime.piste = vert_time.piste;
	                    utime.jono = vert_time.jono;
                        if (tall_rivi(akttm, &utime, &vert_time,
                           &aikajono[akttm]->rtime, 1, 0, 0, false))
                           break;
						tm = pyoristatls(tm , 1);
                        if( d > 0 && (
                           (kierrosva[piste+2] >= 0 &&
                              (kilp.maali(kierrosva[piste+2]) != tm ||
                              tc == L'-' || tc == L'T' || tc == L'I'))
                           || (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1 && NORMKELLO(kilp.p_aika(-1) - tm) != 0)
                           )) {
                           kilp.set_tulos( kierrosva[piste+2], tm);
							if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1 && tm0) {
								for (pst = 0; pst <= kilpparam.maxvaluku; pst++)
									if (kilp.p_aika(pst))
										kilp.tall_tulos(pst, kilp.p_aika(pst)+tm0);
								}
                           pkno = aikajono[akttm]->rtime + 1;
							EnterCriticalSection(&tall_CriticalSection);
                           kilp.tallenna(d,TRUE, 0, 0, &entkilp);
							LeaveCriticalSection(&tall_CriticalSection);
                           pkno = 0;
                           }
                        break;
                     case L'H' :
                     case L'K' :
                        if (d > 0 && kilp.tark(k_pv) != tc) {
                           kilp.set_tark(tc, k_pv);
							EnterCriticalSection(&tall_CriticalSection);
                           kilp.tallenna(d,TRUE, 0, 0, &entkilp);
							LeaveCriticalSection(&tall_CriticalSection);
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
            case 24 :
            case 240:
               aikajono[akttm]->rtime++;
               break;
            case 208:
               if (aikajono[akttm]->rtime-askel == edaika) aikajono[akttm]->rtime--;
               else aikajono[akttm]->rtime = edaika;
               break;
            case  5 :
				case 249:
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
            case 11 :
               k = k % 2 + 1;
               break;
            case 204:
#if !defined(AMPUSUUNN) && !defined(LVAYLA10)
//               testitarra();
#endif
               break;
            case 202:
               if (!esta_tall) {
				  aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime);
                  if (lisaa_edelle && pt.t != TMAALI0*AIKAJAK) {
                     tm = purajak(pt.t) - 1;
                     }
                  else
                     tm = aikajono[akttm]->rtime ?
					 purajak(pt.t) + 1 : purajak(aikajono[akttm]->getAktTime(0, 0)->t) + 1;
				     lisaa_aika(akttm, tm*AIKAJAK, ol_piste, pt.t, pt.date);
					 aikajono[akttm]->rtime++;
                  }
               break;
            case 203:
               if (!esta_tall) {
				   if (aikajono[akttm]->getAktTime(0, aikajono[akttm]->rtime)->kno)
                     writeerror_w(L"Poista ensin kilpailijanumero", 0);
                  else if (aikajono[akttm]->rtime - aikajono[akttm]->aktrows < 0) {
                     ch = L' ';
                     wselectopt(L"Poistetaanko (K/E)", L"KE", &ch);
                     if (ch == L'K') poista_aika(akttm, &vert_time);
                     }
                  }
               break;
				case 248:
					ch = L' ';
					wselectopt(L"Aika katkaistaan tasasekuntiin (K/E)", L"KE", &ch);
					if (ch == L'K') {
	                    utime.jono = vert_time.jono;
						utime.t = ((utime.t + 1000000000L) / (AIKAJAK*SEK)) * AIKAJAK*SEK - 1000000000L;
						tall_rivi(akttm, &utime, &vert_time, &aikajono[akttm]->rtime, 1, 0, 0, false);
						}
					break;
            case 210:
               tulostamaali();
               break;
            }
         if ((aikajono[akttm]->rtime - aikajono[akttm]->aktrows) > 0) aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
         if (tc != 9 && tc != 11 && tc != 204 && !s_aika) k = 1;
         }
      } while(tc != ESC);
   lukum_fl = 1;
   maali_on = 0;
   }
#endif

#if defined(AIKAVERT)
static INT avpkno = 0;

static int vert_on, aktv[2] = {-1,-1}, puoli, vertskrollaa = 1;
static INT32 tjono[MAXJONO], traja[2] = {30000, 100000};
static bool numerovertailu;

void asetavertajat(wchar_t *ln)
	{
	wchar_t *p;
	int j = 0;
	INT32 t;

	p = wcstok(ln, L" ;/\t\n");
	while (p && *p && j < MAXJONO+2) {
		t = _wtol(p);
		if (j < 2)
			traja[j] = 1000*t;
		else
			tjono[j-2] = 1000*t;
		p = wcstok(NULL, L" ;/\t\n");
		j++;
		}
	}

static void naytavertrivi(INT ptime, INT r, INT r0, INT puoli)
   {
	   int n, kva[2] = {0,0} ,ip, pl, bg, fg, kno[2] = {0, 0};
   wchar_t ln[2][40], as[20];
	INT32 td, t[2];
   aikatp pt;

   wmemset(ln[0], L' ', sizeof(ln)/2);
   if (ptime >= 0) {
      n = ptime;
		vidint(r, 0, 4, n+1);
		for (pl = 0; pl < 2; pl++) {
			if (ptime <= aikajono[aktv[pl]]->aktrows) {
				ip = va_piste(aikajono[aktv[pl]]->getAktTime(&pt, ptime)->piste);
				t[pl] = pt.t;
				kva[pl] = (int) pt.kanava;
		      if (purajak(pt.t) > TMAALI0)
				   wmemcpy(ln[pl],AIKATOWSTRS(as, purajak(pt.t),t0),11);
			  kno[pl] = pt.kno;
		      itow1(ln[pl]+12, pt.kno, 5);
		      ln[pl][18] = ch_piste[pt.piste+2];
#if defined(COMETNO) || defined(EMIT)
//				if (pt.badge && pt.t != TMAALI0*AIKAJAK) 
					{
					swprintf(ln[pl]+20, L"%7ld %-5d%c", pt.badge, (int)pt.kanava, 
						pt.tapahtuma < 4 ? L"ADLT"[pt.tapahtuma] : pt.tapahtuma+L'0');
					ln[pl][wcslen(ln[pl]+20)+20] = L' ';
					}
#endif
#if defined(SWIM2000)
		      if (pt.kanava && pt.t != TMAALI0*AIKAJAK) {
				   swprintf(ln[pl]+20, L"R %d, P %c, K %d", ((int)pt.kanava)%10,
						pt.kanava >= 100 ? L't' : L's', (((int)pt.kanava)/10)%10);
					ln[pl][strlen(ln[pl]+20)+20] = L' ';
					}
#endif
				}
	      }
      }
	for (pl = 0; pl < 2; pl++) {
		if (pl == puoli && r == r0+2) {
			bg = 7;
			fg = 0;
			}
		else {
			bg = 0;
			fg = 7;
			}
		if (kva[pl] == 255)
			fg = fg ? 10 : 2;
		else if (pl == 1) {
			if (numerovertailu) {
				if (kno[0] != kno[1])
					fg = fg ? 13 : 5;
				}
			else {
				td = t[1] - t[0] + tjono[aktv[1]] - tjono[aktv[0]];
				if (td < 0) td = -td;
				if (td > traja[1])
					fg = fg ? 13 : 5;
				else if (td > traja[0])
					fg = fg ? 12 : 4;
				}
			}
	   viwrrectw(r,5+35*pl,r,36+35*pl,ln[pl],fg,bg,0);
		}
   viwrrect(r,37,r,39," | ",7,0,0);
   }

void naytavertailu(INT *r1, INT *r0, INT puoli)
   {
   int perusrivi;
   int n1, r, r_alin = ySize-5;
   static INT k0 = 1;

   perusrivi = (r_alin-2)/2;
   if (vertskrollaa && (aikajono[aktv[0]]->aktrows - aikajono[akttm]->rtime == r_alin - perusrivi - 1 || 
	   aikajono[aktv[1]]->aktrows - aikajono[akttm]->rtime == r_alin - perusrivi - 1))
		aikajono[akttm]->rtime++;
   *r1 = aikajono[akttm]->rtime;
   *r0 = max(0, *r1 - perusrivi);
   n1 = r_alin - 2;
   for (r = 0; r <= n1; r++) 
      naytavertrivi(*r0 + r, r + 2, *r1-*r0, puoli);
   }

static wchar_t verttulk(wchar_t ch, INT key)
   {
   if (ch == 2 && key == 48)    // Ctl-B
	  return(249);
   if (ch == 6 && key == 33)    // Ctl-F
	  return(247);
   if (ch == 7 && key == 34)    // Ctl-G
	  return(243);
   if (ch == 8 && key == 35)    // Ctl-H
	  return(234);
   if (ch == 9 && key == 23)    // Ctl-I
	  return(241);
   if (ch == 10 && key == 36)    // Ctl-J
	  return(236);
   if (ch == 11 && key == 37)    // Ctl-K
	  return(246);
   if (ch == 12 && key == 38)    // Ctl-L
	  return(242);
   if (ch == 16 && key == 25)    // Ctl-P
	  return(231);
   if (ch == 18 && key == 19)    // Ctl-R
	  return(235);
   if (ch == 19 && key == 31)    // Ctl-S
	  return(233);
   if (ch == 21 && key == 22)    // Ctl-U
	  return(240);
   if (ch == 22 && key == 47)    // Ctl-V
	  return(232);
   if (ch == 23 && key == 17)    // Ctl-W
	  return(250);
   if (ch == 24 && key == 45)    // Ctl-X
	  return(245);
   if (ch) return(towupper(ch));
   switch (key) {
      case 15 : ch = 11; break;       /* backtab */
      case 59 :                         /* F1 */
      case 60 :                         /* F2 */
      case 61 :                         /* F3 */
      case 62 :                         /* F4 */
      case 64 :                         /* F6 */ 
      case 65 :                         /* F7 */ 
      case 66 :                         /* F8 */ 
      case 68 : ch = key + 142; break;  /* F10 */
		case 31 : ch = 233; break;        /* Alt-S */
      case 19 : ch = 235; break;        /* Alt-R */
      case 35 : ch = 234; break;        /* Alt-H */
	  case 36 : ch = 236; break;		// Alt-J
      case 22 : ch = 240; break;        /* Alt-U */
      case 30 : ch = 241; break;        /* Alt-A */
      case 48 : ch = 249; break;        /* Alt-B */
 	   case 33 : ch = 247; break;			 /* Alt-F */
      case 38 : ch = 242; break;        /* Alt-L */
      case 34 : ch = 243; break;        /* Alt-G */
	  case 11:
      case 129: ch = 244; break;        /* Alt-0 */
      case 45 : ch = 245; break;        /* Alt-X */
      case 37 : ch = 246; break;        /* Alt-K */
      case 107: ch = 248; break;        /* Alt-F4 */
      case 109:                         /* Alt-F6 */
      case 111:                         /* Alt-F8 */
                ch = key + 127; break;
      case 63 :                         /* F5  */
      case 72 : ch = 5;
                break;                  /*up*/
      case 67 :                          /* F9 */
      case 80 : ch = 24;
                break;               /*down*/
      case 73 : ch = 18; break;      /*pgup*/
      case 81 : ch = 3; break;      /*pgdn*/
      case 118: ch = 2; break;     /*Ctrl-PgDn*/
      case 132: ch = 19; break;     /*Ctrl-PgUp*/
      }
   return(ch);
   }

void aikavert(void)
   {
   INT r1, r0, k, l, key, kno, d, x[2] = {5,38};
   INT piste;
   wchar_t tc, ch, ch2, line[66], as[20];
   char che;
   kilptietue kilp, entkilp;
   INT32 tm, tm0;
   aikatp pt;
   INT edaika = -32737, htime;
   static aikatp vert_time, utime;
   static INT askel=1;
   INT s_aika = 0, haku = 0;
   INT32 tl;
	INT pst;

	if (maxjono < 1)
		return;
	if (aktv[0] < 0)
		aktv[0] = akttm;
	if (aktv[1] < 0) {
		if (maxjono > 0)
			aktv[1] = aktv[0] ? 0 : 1;
		else
			aktv[1] = 0;
		}
   clrscr();
   header = L"AJANOTTO";
	lukum_fl = 0;
   kehys(0);
	vidspwmsg(0, 60, 7, 0, header);
   vert_on = 1;
   status_on = 0;
   k = 1;
   do {
      tc = 0;
      ch = 0;
	  tm0 = 0;
      viwrrectw(ySize-3,0,ySize-2,79,
         L"Up Dn PgUp PgDn Alt-R: vaihda riviä, "
         L"Tab : vaihda puoli                         "
			L"F2/F3 : lisää/poista rivi                    "
			L"Esc: peruuta ja poist              ",7,0,0);
		clrln(ySize-1);
		vidint(0, 9, 0, aktv[0]+1);
		vidint(0, 69, 0, aktv[1]+1);
      naytavertailu(&r1, &r0, puoli);
	  Sleep(50);
	  if (vaiheenvaihto || !taustaon)
		  break;
	  aikajono[akttm]->getAktTime(&vert_time, aikajono[akttm]->rtime);
	  sccurset(r1 - r0 + 2, x[puoli]);
      od_fl = 0;
      if (kbready(&che,&key)) {
		 tc = readkbd(&che, 1, 0);
		 if (tc == ESC)
				break;
         switch (k) {
            case 1 :
				tc = verttulk(tc, key);
				if (tc == 233) {
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Anna jonon lisäaika kymmenyksinä:          N: vertaa numeroita");
					kno = tjono[akttm]/1000;
					INPUTINTW(&kno, 5, 35, ySize-3, L"nN\r", &ch);
					if (towupper(ch) == L'N') {
						numerovertailu = true;
						goto pois;
						}
					numerovertailu = false;
					tjono[akttm] = 1000 * kno;
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Anna alempi aikaraja kymmenyksinä:");
					kno = traja[0]/1000;
					INPUTINTW(&kno, 4, 36, ySize-3, L"\r", &ch);
					traja[0] = 1000 * kno;
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Anna ylempi aikaraja kymmenyksinä:");
					kno = traja[1]/1000;
					INPUTINTW(&kno, 4, 36, ySize-3, L"\r", &ch);
					traja[1] = 1000 * kno;
					tc = 0;
					goto pois;
					}
				if (tc == 234) {
					clrln(ySize-3);
					vidspwmsg(ySize-3,0,7,0,L"Anna haettava numero:");
					INPUTINTW(&haku, 4, 22, ySize-3, L"\r", &ch);
					}
				if (tc == 244)
                    haku = 0;
				if (tc == 234 || tc == 243 || tc == 244) {
                     htime = aikajono[akttm]->rtime+1;
                     if (aikajono[akttm]->aktrows - htime <= 0)
                        htime = 0;
                     for (; htime <= aikajono[akttm]->aktrows; htime++)
						if (haku == aikajono[akttm]->getAktTime(0, htime)->kno) break;
					 if (haku != aikajono[akttm]->getAktTime(0, htime)->kno)
                        for (htime = 0; htime <= aikajono[akttm]->rtime; htime++)
							if (haku == aikajono[akttm]->getAktTime(0, htime)->kno) break;
					 if (haku != aikajono[akttm]->getAktTime(0, htime)->kno)
                        writeerror_w(L"EI LÖYDY",0);
                     else aikajono[akttm]->rtime = htime;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 235) {
                     clrln(ySize-3);
                     d = (int) (aikajono[akttm]->rtime+1);
                     vidspwmsg(ySize-3,0,7,0,L"Anna rivi:");
                     INPUTINTW(&d, 4, 11, ySize-3, L"\r", &tc);
                     d = max(d, 1);
                     aikajono[akttm]->rtime = d - 1;
                     tc = 0;
                     goto pois;
                     }
                  if (tc == 236) {     // Alt-J
					 vertskrollaa = !vertskrollaa;
                     tc = 0;
                     goto pois;
                     }					
                  else if (tc == 241) {    // Alt-A tai Ctl-I
                     if (maxjono) {
                        aktv[puoli] = (aktv[puoli] + 1) % (maxjono+1);
								akttm = aktv[puoli];
                        }
                     }
                  if (tc != KB_CR && tc != L'*' && tc != 206 && tc != 208
                     && tc != 240&& tc != 249) {
                     if (tc == ESC) tc = 0;
                     goto pois;
                     }
               break;
            case 2 :
               d = 0;
			   aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime);
               piste = pt.piste;
               if((d = getpos(pt.kno)) <= 0 && pt.kno) {
                  writeerror("KILPAILUNUMEROA EI LÖYDY!",2000);
                  tc = 0;
                  break;
                  }
               if (d > 0)
                  kilp.GETREC(d);
               entkilp = kilp;
               if (syotto && !pt.kno && pt.t == TMAALI0*AIKAJAK) {
                  do {
                     tmaali = maaliaika(tmaali, t0, r1-r0+2, 5, &ch2);
                     if (!ch2) {
                        viwrrectw(r1-r0+2, 5, r1-r0+2, 12,
                           AIKATOWSTRS(as, tmaali, t0), 0,7,0);
                        s_aika = 1;
                        tc = KB_CR;
                        tm = tmaali;
                        }
                     } while (ch2 && ch2 != ESC);
                  }
               else {
                  tm = purajak(pt.t);
						tm0 = 0;
                  l = tm == TMAALI0 ? pvparam[k_pv].laika : LAIKA;
                  if ((tc >= L'0' && tc <= L'9') || (tc == 0 && key == 65)
                     || (tc == 0 && key == 66) || (tc == 0 && key == 77)) {
					 tm0 = tm;
					 tm0 = pyoristatls(tm0 , 1);
                     INPUTAIKAW(&tm,t0,l,5,r1-r0+2,
                        L"hHkK-\r\t\x1B\x0B\xCA\xCB\xCC\xD2\x05\x18\x12\x03"
                        L"\x02\x13",&tc);
					tm0 -= pyoristatls(tm , 1);
					 }
                  else {
                     tc = verttulk(tc, key);
                     kbflush();
                     }
                  }
               if (tc == ESC) tc = 0;
               tc = towupper(tc);
               wcscpy(line, L"       ");
               if (kierrosva[piste+2] >= 0) {
                  switch (tc) {
                     case L'K':
                        wcscpy(line, L"KESKEYT");
                        break;
                     case L'H':
                        wcscpy(line, L"HYLÄTTY");
                        break;
                     case L'E':
                        wcscpy(line, L"EI LÄHT");
                        break;
                     case KB_CR :
                     case L'-':
                     case L'T':
                     case L'I':
                        if (d > 0) {
                           if (kierrosva[piste+2] >= 0) {
                              wcscpy(line,
                                 AIKATOWSTRS(as, kilp.tulos1(kierrosva[piste+2],tm), 0)+1);
                              }
                           }
                        break;
                     default :
                        goto pois;
                     }
                  }
               else if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1) {
                  tl = NORMKELLO(tm - kilp.pv[k_pv].tlahto);
                  if (tl >= 0)
                     swprintf(line, L"+%6.6s", AIKATOWSTRS(as, tl,0)+4);
                  else
                     swprintf(line, L"-%6.6s", AIKATOWSTRS(as, -tl,0)+4);
                  }
               if (d > 0 && (tc != KB_CR || kilp.hyv()))
                  viwrrectw(r1-r0+2, 57, r1-r0+2, 63, line, 0, 7, 0);
               if (s_aika && tc == KB_CR) ch = L'+';
               else {
                  ch = L' ';
                  wselectopt(L" + : Tallenna,  <Esc> : Peruuta",L"+\x1b", &ch);
                  }
               if (!esta_tall && ch == L'+') {
                  switch (tc) {
                     case L'-':
                     case L'I':
                     case L'T':
                        kilp.set_tark(tc, k_pv);
                     case KB_CR :
                        utime.t = tm*AIKAJAK;
						utime.date = tm_date(utime.t);
						utime.jono = akttm;
                        utime.kno = vert_time.kno;
                        utime.piste = vert_time.piste;
                        if (tall_rivi(akttm, &utime, &vert_time,
                           &aikajono[akttm]->rtime, 1, 0, 0, false))
                           break;
						tm = pyoristatls(tm , 1);
                        if( d > 0 && (
                           (kierrosva[piste+2] >= 0 &&
                              (kilp.maali(kierrosva[piste+2]) != tm ||
                              tc == L'-' || tc == L'T' || tc == L'I'))
                           || (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] < 0 && NORMKELLO(kilp.p_aika(-1) - tm) != 0)
                           )) {
                           kilp.set_tulos(kierrosva[piste+2], tm);
							if (pvparam[k_pv].hiihtolahto && kierrosva[piste+2] == -1 && tm0) {
								for (pst = 0; pst <= kilpparam.maxvaluku; pst++)
									if (kilp.p_aika(pst))
										kilp.tall_tulos(pst, kilp.p_aika(pst)+tm0);
								}
                           avpkno = aikajono[akttm]->rtime + 1;
							EnterCriticalSection(&tall_CriticalSection);
                           kilp.tallenna(d,TRUE, 0, 0, &entkilp);
							LeaveCriticalSection(&tall_CriticalSection);
                           avpkno = 0;
                           }
                        break;
                     case L'H' :
                     case L'K' :
                        if (d > 0 && kilp.tark(k_pv) != tc) {
                           kilp.set_tark(tc, k_pv);
							EnterCriticalSection(&tall_CriticalSection);
                           kilp.tallenna(d,TRUE, 0, 0, &entkilp);
							LeaveCriticalSection(&tall_CriticalSection);
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
            case 24 :
            case 240:
               aikajono[akttm]->rtime++;
               break;
            case 208:
               if (aikajono[akttm]->rtime-askel == edaika) aikajono[akttm]->rtime--;
               else aikajono[akttm]->rtime = edaika;
               break;
            case  5 :
				case 249:
               if (aikajono[akttm]->rtime > 0) aikajono[akttm]->rtime--;
               break;
            case  3 :
               aikajono[akttm]->rtime += ySize-7;
               break;
            case 18 :
               if(aikajono[akttm]->rtime > ySize-7) 
                  aikajono[akttm]->rtime -= ySize-7;
               else aikajono[akttm]->rtime = 0;
               break;
            case  2 :
               aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
               break;
            case 19 :
               aikajono[akttm]->rtime = 0;
               break;
            case  9 :
            case 11 :
               puoli = 1-puoli;
				akttm = aktv[puoli];
				aikajono[akttm]->rtime = aikajono[aktv[1 - puoli]]->rtime;
               break;
            case 204:
               break;
            case 202:
               if (!esta_tall) {
				  aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rtime);
//                  if (lisaa_edelle && pt.t != TMAALI0*AIKAJAK) {
                  if (pt.t != TMAALI0*AIKAJAK) {
                     tm = purajak(pt.t) - 1;
                     }
                  else
                     tm = aikajono[akttm]->rtime ?
						purajak(pt.t) + 1 : purajak(aikajono[akttm]->getAktTime(0, 0)->t)+1;
				  lisaa_aika(akttm, tm*AIKAJAK, ol_piste, pt.t, pt.date);
				  aikajono[akttm]->rtime++;
                  }
               break;
            case 203:
               if (!esta_tall) {
				  if (aikajono[akttm]->getAktTime(0, aikajono[akttm]->rtime)->kno) 
                     writeerror_w(L"Poista ensin kilpailijanumero", 0);
                  else if (aikajono[akttm]->rtime - aikajono[akttm]->aktrows < 0) {
                     ch = L' ';
                     wselectopt(L"Poistetaanko (K/E)", L"KE", &ch);
                     if (ch == L'K') 
						 poista_aika(akttm, &vert_time);
                     }
                  }
               break;
				case 248:
					ch = L' ';
					wselectopt(L"Aika katkaistaan tasasekuntiin (K/E)", L"KE", &ch);
					if (ch == L'K') {
						utime.t = ((utime.t + 1000000000L) / (AIKAJAK*SEK)) * AIKAJAK*SEK - 1000000000L;
						tall_rivi(akttm, &utime, &vert_time, &aikajono[akttm]->rtime, 1, 0, 0, false);
						}
					break;
            }
         if ((aikajono[akttm]->rtime - aikajono[akttm]->aktrows) > 0) aikajono[akttm]->rtime = aikajono[akttm]->aktrows;
         if (tc != 9 && tc != 11 && tc != 204 && !s_aika) k = 1;
         }
      } while(tc != ESC);
   lukum_fl = 1;
   vert_on = 0;
   status_on = 1;
   }
#endif // AIKAVERT
