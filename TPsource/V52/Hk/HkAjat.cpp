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

#ifdef _BORLAND_
#include <vcl.h>
#endif
#include <map>
#include <stdio.h>
#include <algorithm>
#include <functional>
 using namespace std;
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>
#include "HkDeclare.h"

INT32 purajak(INT32 tjak)
	{
	if (tjak >= 0)
		return(tjak/AIKAJAK);
	return((tjak+1)/AIKAJAK-1);
	}

/*
int haejono(INT32 jono_t)
{
	return((jono_t+1000000000L)%10);
}
*/

aikatp *gettm(INT rvno, aikatp *raika, INT ino)
	{
	static aikatp caika;

   if (raika == NULL)
		raika = &caika;
   if (rvno < 0 || rvno > aikajono[ino]->mxtime) {
	  memset(raika, 0, sizeof(aikatp));
		raika->t = TMAALI0*AIKAJAK;
		return(raika);
	  }
	  memcpy(raika, aikajono[ino]->atime+rvno, sizeof(aikatp));
   return(raika);
   }

aikatp *puttm(INT rvno, aikatp *raika, INT ino)
   {
   if (rvno < 0 || rvno > aikajono[ino]->mxtime) {
	  return(raika);
	  }
	  memcpy(aikajono[ino]->atime+rvno, raika, sizeof(aikatp));
   return(raika);
   }

#if defined(MAALI) && !defined(EIMAALI)
static void poistarivi(int rtm, INT ino)
   {
   memmove(aikajono[ino]->atime+rtm, aikajono[ino]->atime+rtm+1, (aikajono[ino]->rwtime - rtm)*sizeof(aikatp));
   aikajono[ino]->rwtime--;
   memset(aikajono[ino]->atime+aikajono[ino]->rwtime, 0, sizeof(aikatp));
   aikajono[ino]->atime[aikajono[ino]->rwtime].t = TMAALI0*AIKAJAK;
	}

static void lisaarivi(int rtm, aikatp *tm, INT ino)
   {
	  {
	  if (aikajono[ino]->rwtime > rtm)
			memmove(aikajono[ino]->atime+rtm+1, aikajono[ino]->atime+rtm, (aikajono[ino]->rwtime - rtm)*sizeof(aikatp));
		memcpy(aikajono[ino]->atime+rtm, tm, sizeof(aikatp));
      }
   aikajono[ino]->rwtime++;
   }

__int64 datetime64(aikatp *tm)
{
	return(((__int64)UINT_MAX+1)*tm->date + NORMKELLO_A(tm->t));
}

// INT tall_rivi(INT ino, aikatp *utm, aikatp *vtm, aikatp_ptr *uptr, INT kysy,
//    INT kielto, INT pakota_lah)
//
//    ino      ajanottojonon numero (1 : lähtöajat, 0: muu)
//    utm      osoitin uuteen aikatietueeseen
//    vtm      osoitin korvattavaan aikatietueeseen
//    uptr     tässä palautetaan uusi osoitin aktiiviseen aikatietueeseen
//    kysy     ilmoittaa, että on kysyttävä vahvistusta. kysy == 1, jos
//             tietoja syötetään ko. tietokoneelta
//    kielto   sen yhteyden numero, josta sanoma on tullut (kielto = 1 ... 3)
//    pakota_lah  lähetä rivi, vaikka ei muutosta
//

INT tall_rivi(INT ino, aikatp *utm, aikatp *vtm, INT *uptr, INT kysy,
   INT kielto, INT pakota_lah, bool InCriticalSection)
   {
   aikatp tm;
   int rtm;
	wchar_t ch;
	wchar_t as[40],as2[40],as3[20],line[66];
	INT errret = 0, tcomfl0, muutos = 0;
   __int64 it1, it2;


	if (!aikajono[ino]->mxtime)
		return(1);
	if (!InCriticalSection)
		EnterCriticalSection(&ajat_CriticalSection);
	tcomfl0 = tcomfl;
	tcomfl = 0;
	if (loki) {
		if (vtm)
#if defined(COMETNO)
			swprintf(as,L"%s %4d %-2s%8ld", AIKATOWSTRS(as3,purajak(vtm->t),t0),vtm->kno,
				str_piste(vtm->piste), vtm->badge);
#else
			swprintf(as,L"%s %4d %s", AIKATOWSTRS(as3, purajak(vtm->t),t0),vtm->kno,
				str_piste(vtm->piste));
#endif
		else as[0] = 0;
		if (utm)
#if defined(COMETNO)
			swprintf(as2,L"%s %4d %s", AIKATOWSTRS(as3, purajak(utm->t),t0),utm->kno,
			str_piste(utm->piste));
#else
		 swprintf(as2,L"%s %4d %s", AIKATOWSTRS(as3,purajak(utm->t),t0),utm->kno,
			str_piste(utm->piste));
#endif
	  else as2[0] = 0;
#if defined(COMETNO)
		 swprintf(line,L"%4d %26.26s %26.26s %d", aikajono[ino]->rwtime, as, as2, kielto);
#else
		 swprintf(line,L"%4d %17.17s %17.17s %d", aikajono[ino]->rwtime, as, as2, kielto);
#endif
		wkirjloki(line);
	  }

   // Seuraava lohko käsitellään, jos vanhoja tietoja poistetaan tai korvataan

   if (vtm && vtm->t != TMAALI0*AIKAJAK) {

	  // Seuraavassa etsitään korvattava rivi alkaen viimeisestä
	  it1 = datetime64(vtm);
	  for (rtm = aikajono[ino]->rwtime-1; rtm >= 0 &&
		 (it2 = datetime64(gettm(rtm, &tm, ino))) >= it1; rtm--) {
//		 NORMTULOS_A(gettm(rtm, &tm, ino)->t - vtm->t) < 12L*TUNTI*AIKAJAK; rtm--) {
//		 if (NORMTULOS_A(tm.t - vtm->t) == 0 && tm.kno == vtm->kno &&
		 if (it2 == it1 && tm.kno == vtm->kno &&
			(tm.piste == vtm->piste || tm.piste == -2)) {

#ifdef _CONSOLE

		  // Tässä on löydetty tietue, jossa aika, kilpailija ja piste oikeat
		  // Jos L'kysy' voimassa ja rivi vaihtuu kysytään vahvistusta

				if (kysy && utm && utm->t != TMAALI0*AIKAJAK && (
				  (
					 aikajono[ino]->rwtime - rtm > 1 &&
						datetime64(utm) > datetime64(gettm(rtm+1, 0, ino))
//						NORMTULOS_A(utm->t - gettm(rtm+1, 0, ino)->t) < 12L*TUNTI*AIKAJAK
					 )
				  || (
					 rtm > 0 &&
						datetime64(utm) < datetime64(gettm(rtm-1, 0, ino))
//						NORMTULOS_A(utm->t - gettm(rtm-1, 0, ino)->t) >= 12L*TUNTI*AIKAJAK
					 )
				  )
			   ) {
				ch = L' ';
				wselectopt(L"Aika ei vastaa riviä - vahvista tallennus (K/E)",
					L"KE", &ch);
				if (ch == L'E') {
					if (loki)
						wkirjloki(L"Tallennus peruutettu (rivi)");
					errret = 1;
					goto poistu;
					}
				}
#endif

				//  Poista vanha tietue

			poistarivi(rtm, ino);
			muutos = 1;
			memset(&tm, 0, sizeof(tm));
			if (uptr && *uptr - rtm > 0) (*uptr)--;
			if (aikajono[ino]->fp1 - rtm > 0)
			   aikajono[ino]->fp1 = rtm;
			goto jatka;
			}
		 }

		// Tähän tullaan, jos looppi on käyty loppuun löytämättä oikeaa riviä

	  if (!kielto) {
		 writeerror_w(L"Tallennus ei onnistu. Aikataulukkoon tullut mahdollisesti "
			L"muutossanoma", 2000);
		 if (loki) wkirjloki(L"Tallennus peruutettu (muutos)");
		 errret = 1;
		 goto poistu;
		 }
	  else {
			if (loki) wkirjloki(L"Rivin poisto peruutettu (muutos)");
		 }
	  }
   jatka:

   // Seuraava lohko käsitellään, jos kirjataan uusia tietoja

	if (utm && utm->t != TMAALI0*AIKAJAK) {
		utm->t = NORMKELLO_A(utm->t);
	  if((aikajono[ino]->rwtime - aikajono[ino]->mxtime) >= 0) {
			writeerror_w(L"AIKATAULUKKO TÄYSI",0);
		 if (loki) wkirjloki(L"Aikataulukko täysi, tallennus peruutettu");
		 errret = 1;
		 goto poistu;
			}

	  // Etsi rivi, johon uusi tieto kirjataan

	  it1 = datetime64(utm);
	  for (rtm = aikajono[ino]->rwtime; rtm > 0 &&
		 (it2 = datetime64(gettm(rtm-1, &tm, ino))) >= it1;
		 rtm--)
		 if (it2 == it1 && ((tm.kno && tm.kno > utm->kno) || (tm.badge != 0 && tm.badge == utm->badge)))
			 break;

		//  Nyt on joko it2 < it1 tai aika on sama ja (uusi kilpailunumero on pienempi tai badge vahvistaa tiedon samaksi)

		//  Tallenna uusi tietue, ellei sitä tulkita aiemman tiedon toistoksi

		if (it2 != it1 || tm.kno != utm->kno || tm.badge != utm->badge ||
			(tm.kno == 0 && tm.badge == 0 && memcmp(&tm, utm, sizeof(aikatp)))) {

			// varmista, että aika poikkeaa aiemmista ajoista kasvattamalla aikaa, jos se on sama kuin aiemmin tallennettu
			while (it1 == it2) {
				utm->t++;
				it1 = datetime64(utm);
				while (rtm < aikajono[ino]->rwtime - 1 && datetime64(gettm(rtm, &tm, ino)) <= it1)
				   rtm++;
				it2 = datetime64(gettm(rtm-1, &tm, ino));
				}

			lisaarivi(rtm, (aikatp *)utm, ino);
			muutos = 1;
			}
	  //  Ilmoita, minkä rivin tulee jäädä aktiiviseksi

	  if (uptr) {
		 if (kysy) *uptr = rtm;
		 else if (*uptr - rtm > 0) (*uptr)++;
		 }
	  if (aikajono[ino]->fp1 - rtm > 0)
		 aikajono[ino]->fp1 = rtm;
		}

   // Näytä päivitetty aikataulukko

   writeajat();

   // Lähetä muutostieto toisille tietokoneille

	if (muutos) {
		aikajono[ino]->haeAktAjat();
		paivita_aikanaytto();
		laheta_aika(vtm, utm, kielto, pakota_lah);
		}
	poistu:
	if (!InCriticalSection)
		LeaveCriticalSection(&ajat_CriticalSection);
	tcomfl = tcomfl0;
	return(errret);
	}
#endif

#if defined(MAALI) && !defined(EIMAALI)
INT lisaa_aika(INT ino, INT32 t, int piste, int ed_t, int ed_date)
	{
	aikatp ut;

	memset(&ut, 0, sizeof(aikatp));
	ut.t = NORMKELLO_A(t);
	if (ed_t != AIKAJAK * TMAALI0)
		ut.date = tm_copydate(ut.t, ed_t, ed_date);
	else
		ut.date = tm_date(ut.t);
//	ut.piste = ol_piste;
	ut.piste = piste;
	ut.jono = ino;
#ifdef AIKAVERT
	ut.kanava = 255;
#endif
	tall_rivi(ino, &ut,NULL,NULL,0,0,0, false);
	return(0);
	}

INT poista_aika(INT ino, aikatp *rtm)
	{

	tall_rivi(ino, NULL, rtm, NULL, 0, 0, 0, false);
	return(0);
	}
#endif

void writeaika(INT ino, int p)
	{
	INT32 l;
   aikatp abuf;
	if (aikajono[ino]->afhandle == 0)
		return;
   if (_lseek(aikajono[ino]->afhandle, l = (p + 1) * sizeof(aikatp),
	  SEEK_SET) != l) {
	  iowerr(aikajono[ino]->aikafname,0,l);
	  return;
	  }
   gettm(p, &abuf, ino);
   if (_write(aikajono[ino]->afhandle, &abuf, sizeof(aikatp)) != (int) sizeof(aikatp))
	  iowerr(aikajono[ino]->aikafname,2,l);
	if (aikajono[ino]->fafhandle == NULL)
		return;
   if (kirjheti & 8)
	  fflush(aikajono[ino]->fafhandle);
   }

#define ABUFL 40

void writeajat(void)
   {
   INT r, n,ino;
   INT32 fl,l;
   aikatp abuf[ABUFL];
   for (ino = 0; ino <= maxjono || (lajatfl && ino < 2); ino++) {
		if (!aikajono[ino])
			continue;
		if ((fl = aikajono[ino]->rwtime - aikajono[ino]->fp1 + 1) <= 0)
			continue;
		if (aikajono[ino]->afhandle == 0)
			continue;
	  l = (aikajono[ino]->fp1+1) * sizeof(aikatp);
	  if (_lseek(aikajono[ino]->afhandle, l, SEEK_SET) != l) {
		 iowerr(aikajono[ino]->aikafname,0,l);
		 return;
		 }
	  while (fl > 0) {
		 n = (INT) min(ABUFL, fl);
		 fl -= n;
		 for (r = 0; r < n; r++) {
			gettm(aikajono[ino]->fp1++, abuf+r, ino);
			}
		 n *= sizeof(aikatp);
		 if (_write(aikajono[ino]->afhandle, abuf, n) != n) {
			iowerr(aikajono[ino]->aikafname,2,aikajono[ino]->fp1*(sizeof(aikatp)));
			break;
			}
		 }
		if (aikajono[ino]->fafhandle == NULL)
			continue;
	  if (kirjheti & 8)
		 fflush(aikajono[ino]->fafhandle);
	  }
   }

void suljeajat(INT s_ino, bool delfl)
   {
   INT32 fl;
   INT ino;

   writeajat();
	if (s_ino >= 0) {
	  ino = s_ino;
	  }
   else {
	  ino = 0;
		s_ino = lajatfl ? 1 : maxjono;
		}
	for (; ino <= s_ino; ino++) {
		if (!aikajono[ino])
			continue;
		fl = (aikajono[ino]->rwtime+1) * sizeof(aikatp);
		if ( _filelength(aikajono[ino]->afhandle) > fl)
			_chsize(aikajono[ino]->afhandle, fl);
		fclose(aikajono[ino]->fafhandle);
		aikajono[ino]->fafhandle = 0;
		aikajono[ino]->afhandle = 0;
		if (delfl) {
			delete aikajono[ino];
			aikajono[ino] = NULL;
			}
		}
	}

void AjatTiedostoon(wchar_t *fname, int akttm)
{
   static INT alku[MAXJONO] = {1, 1};
   static INT rivi[MAXJONO];
   wchar_t line[86],tark, wbuf[80];
   INT ptime, ip;
   aikatp pt;
   INT d;
   kilptietue kilp;
   INT32 tm;
	FILE *outfl = 0;
	wchar_t ln[120];

	if ((outfl = _wfopen(fname, L"wt")) != NULL) {
		fputws(L"Pvm\tKello\tJono\tStatus\tKilpno\tPiste\tBadge\tKanava\tTap\tTark\tNimi\tSeura\tSarja\tTulos\tEro\n", outfl);
		for (ptime = 0; ptime - aikajono[akttm]->rwtime < 0;
			ptime++) {
			if (gettm(ptime, &pt, akttm)->t > TMAALI0*AIKAJAK) {
				AIKATOWSTRS(line, purajak(pt.t), t0);
				}
			else
				line[0] = 0;
			ip = va_piste(pt.piste);
			swprintf(ln, L"%s\t%s\t%d\t%d\t%d\t%d\t%ld\t%d",
				time_t_date(pt.date, NULL), line, (int)pt.jono, (int)pt.status,
				pt.kno, ip, pt.badge, (int)pt.kanava);
			swprintf(ln+wcslen(ln), L"\t%c", pt.tapahtuma < 4 ? L"ADLT"[pt.tapahtuma] : pt.tapahtuma+L'0');
			if ((d = getpos(pt.kno)) > 0) {
				kilp.GETREC(d);
				line[0] = 0;
				if (ip >= 0) {
					AIKATOWSTRS(line, kilp.p_aika(kierrosva[ip+2]),0);
					tark = L' ';
					if (pt.t > TMAALI0*AIKAJAK) {
						tm = NORMKELLO(kilp.maali(kierrosva[ip+2]) -
							purajak(pt.t));
						if (tm/pvparam[k_pv].pyor[1])
							tark = L'*';
						}
					}
				else if (pvparam[k_pv].hiihtolahto && ip == -1) {
					tm = NORMKELLO(kilp.p_aika(-1) - kilp.pv[k_pv].tlahto);
					if (tm >= 0)
						swprintf(line,L"+%6.6s", AIKATOWSTRS(wbuf,tm,0)+4);
					else
						swprintf(line,L"-%6.6s", AIKATOWSTRS(wbuf, -tm,0)+4);
					tark = L' ';
					if (NORMKELLO(kilp.p_aika(-1) - purajak(pt.t))/pvparam[k_pv].pyor[1] != 0)
						tark = L'*';
					}
				swprintf(ln+wcslen(ln), L"\t%c\t%s\t%s\t%s\t%s\t%c",
					kilp.tark(k_pv), kilp.nimi(wbuf, 30, 0), kilp.seuratieto(0),
					Sarjat[kilp.Sarja()].sarjanimi, line, tark);
				}
			fputws(ln, outfl);
			fputwc(L'\n', outfl);
			}
		}
	fclose(outfl);
}

void AjatTiedostosta(wchar_t *fname, int akttm, bool vainajat)
{
	TextFl *AikaFl;
	int k;

	AikaFl = new TextFl(fname, L"rt");
	if (AikaFl && AikaFl->IsOpen()) {
		while (!AikaFl->Feof()) {
			aikatp tp;
			wchar_t line[200], *p;
			memset(&tp, 0, sizeof(tp));
			if (!AikaFl->ReadLine(line, 198))
				break;
			if (wcslen(line) < 5)
				continue;
			if (line[0] >= L'A')
				continue;
			p = wcstok(line, L" ;\t");
			if (p) {
				tp.date = (int) date_time_t(p);
				if (tp.date == 0)
					tp.date = (int)(time(0)/86400);
				p = wcstok(NULL, L" ;\t");
				if (p && wcslen(p) > 4) {
					tp.t = wstrtoaika_vap(p, t0) * AIKAJAK;
					if (p) {
						tp.jono = _wtoi(p);
						p = wcstok(NULL, L" ;\t");
						}
					if (p) {
						tp.status = _wtoi(p);
						p = wcstok(NULL, L" ;\t");
						}
					if (!vainajat) {
						if (p) {
							tp.kno = _wtoi(p);
							p = wcstok(NULL, L" ;\t");
							}
						if (p) {
							tp.piste = _wtoi(p);
							p = wcstok(NULL, L" ;\t");
							}
						if (p) {
							tp.piste = _wtoi(p);
							p = wcstok(NULL, L" ;\t");
							}
						if (p) {
							tp.badge = _wtoi(p);
							p = wcstok(NULL, L" ;\t");
							}
						if (p) {
							tp.kanava = _wtoi(p);
							p = wcstok(NULL, L" ;\t");
							}
						if (p) {
							if ((k = wcswcind(*p, L"ADLT")) >= 0)
								tp.tapahtuma = k;
							else
								tp.tapahtuma = _wtoi(p);
							}
						}
					if (tp.t != AIKAJAK*TMAALI0)
						tall_rivi(akttm, &tp,NULL,NULL,0,0,0, false);
					}
				}

			}
		}
#ifndef _CONSOLE
	else {
		Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
		}
#endif
	if (AikaFl)
		delete AikaFl;
}
