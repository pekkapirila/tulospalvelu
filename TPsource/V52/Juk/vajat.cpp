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
#pragma hdrstop

int normtt(int t)
{
	t = t + 24 * AIKAJAK * TUNTI - AIKAJAK*enslahto;
	while (t < 0) {
		t += 24 * AIKAJAK * TUNTI;
		}
	while (t >= 24 * AIKAJAK * TUNTI) {
		t -= 24 * AIKAJAK * TUNTI;
		}
	return(t);
}

aikatp *gettm(int rvno, aikatp *raika, int ino)
   {
   static aikatp caika;

   if (raika == NULL)
	  raika = &caika;
   if (aikajono[ino] && rvno < aikajono[ino]->rwtime)
	   memcpy(raika, aikajono[ino]->atime+rvno, sizeof(aikatp));
   else {
	   memset(raika, 0, sizeof(aikatp));
	   raika->t = AIKAJAK*TMAALI0;
       }
   return(raika);
   }

aikatp *puttm(int rvno, aikatp *raika, int ino)
   {
	  memcpy(aikajono[ino]->atime+rvno, raika, sizeof(aikatp));
   return(raika);
   }

static void poistarivi(int rtm, int ino)
   {
   memmove(aikajono[ino]->atime+rtm, aikajono[ino]->atime+rtm+1, (aikajono[ino]->rwtime - rtm)*sizeof(aikatp));
   aikajono[ino]->rwtime--;
   }

static void lisaarivi(int rtm, aikatp *tm, int ino)
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
	__int64 ii;
	ii = ((__int64)UINT_MAX+1)*tm->date + NORMKELLO_A(tm->t);
	return(ii);
}

// int tall_rivi(int ino, aikatp *utm, aikatp *vtm, int *uptr, int kysy,
//    int kielto, int pakota_lah)
//
//    utm      osoitin uuteen aikatietueeseen
//    vtm      osoitin korvattavaan aikatietueeseen
//    uptr     tässä palautetaan uusi osoitin aktiiviseen aikatietueeseen
//    kysy     ilmoittaa, että on kysyttävä vahvistusta. kysy == 1, jos
//             tietoja syötetään ko. tietokoneelta
//    kielto   sen yhteyden numero, josta sanoma on tullut (kielto = 1 ... 3)
//    pakota_lah  lähetä rivi, vaikka ei muutosta
//

int tall_rivi(int ino, aikatp *utm, aikatp *vtm, int *uptr, int kysy,
   int kielto, int pakota_lah)
   {
   aikatp tm;
   int rtm;
   char ch;
   char as[32],as1[20], as2[32],line[82];
   int errret = 0, tcomfl0, muutos = 0;
   __int64 it1, it2;

	if (!aikajono[ino] || !aikajono[ino]->mxtime)
		return(1);
   EnterCriticalSection(&ajat_CriticalSection);
   tcomfl0 = tcomfl;
   tcomfl = 0;
   if (loki) {
	  if (vtm)
		 sprintf(as,"%s %4d %2d %6ld %3d",
			AIKATOSTRS(as1, purajak(vtm->t),t0),vtm->kno,vtm->osuus+1,vtm->badge,vtm->lahde);
	  else as[0] = 0;
	  if (utm)
		 sprintf(as2,"%s %4d %2d %6ld %3d",
			AIKATOSTRS(as1, purajak(utm->t),t0),utm->kno,utm->osuus+1,utm->badge,utm->lahde);
	  else as2[0] = 0;
	  sprintf(line,"%4d %24.24s %24.24s %2d %4d\r\n",
		 aikajono[ino]->rwtime, as, as2, ino, kielto);
	  kirjloki(line);
	  }

   // Seuraava lohko käsitellään, jos vanhoja tietoja poistetaan tai korvataan

   if (vtm && purajak(vtm->t) != TMAALI0) {

	  // Seuraavassa etsitään korvattava rivi alkaen viimeisestä

	  it1 = datetime64(vtm);
	  for (rtm = aikajono[ino]->rwtime-1; rtm >= 0 &&
		 (it2 = datetime64(gettm(rtm, &tm, ino))) >= it1; rtm--) {
//		 normtt(gettm(rtm, &tm, ino)->t) >= normtt(vtm->t); rtm--) {
		 if (it2 == it1 && tm.kno == vtm->kno &&
			(!vtm->kno || tm.osuus == vtm->osuus)) {

			// Tässä on löydetty tietue, jossa aika, joukkue ja osuus oikeat
			// Jos 'kysy' voimassa ja rivi vaihtuu kysytään vahvistusta

			if (kysy && utm && utm->t != AIKAJAK*TMAALI0 &&
			   ((aikajono[ino]->rwtime - rtm > 1 && datetime64(utm) > datetime64(gettm(rtm+1,0, ino))) ||
			   (rtm > 0 && datetime64(utm) < datetime64(gettm(rtm-1,0, ino))))) {
//			   ((aikajono[ino]->rwtime - rtm > 1 && normtt(utm->t) > normtt(gettm(rtm+1,0, ino)->t)) ||
//			   (rtm > 0 && normtt(utm->t) < normtt(gettm(rtm-1,0, ino)->t)))) {
			   ch = ' ';
			   selectopt("Aika ei vastaa riviä - vahvista tallennus (K/E)",
				  "KE", &ch);
			   if (ch == 'E') {
				  if (loki)
					 kirjloki("Tallennus peruutettu (rivi)\r\n");
				  errret = 1;
				  goto poistu;
				  }
			   }

			//  Poista vanha tietue ja pienennä maalilaskuria

			poistarivi(rtm, ino);
			muutos = 1;
			memset(&tm, 0, sizeof(tm));
			if (uptr && *uptr - rtm > 0) (*uptr)--;
			if (aikajono[ino]->fp1 > rtm)
			   aikajono[ino]->fp1 = rtm;
			goto jatka;
			}
		 }
	  if (kysy) {
		  writeerror("Aikataulukkoon tullut muutossanoma - tallennus "
			  "peruutetaan", 0);
		  if (loki)
			  kirjloki("Tallennus peruutettu (muutos)\r\n");
		}
	  errret = 1;
	  goto poistu;
	  }
   jatka:

   // Seuraava lohko käsitellään, jos kirjataan uusia tietoja

   if (utm && purajak(utm->t) != TMAALI0) {
	  if((aikajono[ino]->rwtime - aikajono[ino]->mxtime) >= 0) {
		 writeerror("AIKATAULUKKO TÄYSI",0);
		  if (loki)
			  kirjloki("Aikataulukko täysi, tallennus peruutettu\r\n");
		 errret = 1;
		 goto poistu;
		 }

	  if (!vtm || utm->kno != vtm->kno || utm->osuus != vtm->osuus ||
		 utm->t != vtm->t)
		 if (!utm->lahde)
			utm->lahde = kone;

	  // Etsi rivi, johon uusi tieto kirjataan

	  it1 = datetime64(utm);
	  for (rtm = aikajono[ino]->rwtime; rtm > 0 &&
		 (it2 = datetime64(gettm(rtm-1, &tm, ino))) >= it1; rtm--) {
		 if (it1 == it2 && (tm.kno || (tm.badge != 0 && tm.badge == utm->badge)))
			break;
		 }
	  //  Tallenna uusi tietue ja kasvata maalilaskuria

	  if (it1 != it2 || (utm->kno && utm->kno != tm.kno) ||
		  tm.badge == 0 || tm.badge != utm->badge) {

		  // varmista, että aika poikkeaa aiemmista ajoista
		  while (it1 == it2) {
			  utm->t++;
			  it1 = datetime64(utm);
			  while (rtm < aikajono[ino]->rwtime - 1 && datetime64(gettm(rtm, &tm, ino)) <= it1)
				  rtm++;
			  it2 = datetime64(gettm(rtm-1, &tm, ino));
			  }
		  lisaarivi(rtm, utm, ino);
		  muutos = 1;
		  }

      //  Ilmoita, minkä rivin tulee jäädä aktiiviseksi
 
      if (uptr) {
         if (kysy) *uptr = rtm;
         else if (*uptr - rtm > 0) (*uptr)++;
         }
      if (aikajono[ino]->fp1 > rtm)
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

   // Tallenna uusi tietue struktuuriin viim_time

   if (utm && (!vtm || vtm->kno == 0))
	  viim_time = *utm;
   else viim_time.kno = 0;

   poistu:
   LeaveCriticalSection(&ajat_CriticalSection);
   tcomfl = tcomfl0;
   return(errret);
   }

int lisaa_aika(int ino, TMAALI t, int piste, int ed_t, int ed_date)
   {
   aikatp ut;

   memset(&ut, 0, sizeof(aikatp));
   ut.t = t;
	if (ed_t != AIKAJAK * TMAALI0)
		ut.date = tm_copydate(ut.t, ed_t, ed_date);
	else
		ut.date = tm_date(ut.t);
   ut.piste = piste;
   ut.jono = ino;
   tall_rivi(ino, &ut, NULL, NULL, 0, 0, 0);
   return(0);
   }

int poista_aika(int ino, aikatp *rtm)
   {

   tall_rivi(ino, NULL, rtm, NULL, 0, 0, 0);
   return(0);
   }

INT hae_aika(INT32 t, INT *wt, INT jono, INT *ino)
   {

   aikatp pt;
   INT tm;

   for (*ino = 0; *ino < MAXJONO; ++*ino) {
      if (!aikajono[*ino] || aikajono[*ino]->rwtime == 0)
         continue;
	  for (tm = aikajono[*ino]->rwtime-1; tm >= 0 && purajak(gettm(tm, &pt, *ino)->t - t) < 17L*TUNTI; tm--) {
         if (purajak(pt.t) == purajak(t) && pt.jono == jono) {
//         NORMTULOS_A(gettm(tm, &pt, *ino)->t - t) < 17L*TUNTI*AIKAJAK; tm--) {
//         if (NORMTULOS_A(pt.t - t) < 10 && NORMTULOS_A(pt.t) % 10 == jono) {
            *wt = tm;
            return(1);
            }
         }
      }
   *wt = 0;
   return(0);
   }

void writeaika(int ino, int p)
   {
   long l;
   char abuf[sizeof(aikatp)];

   if (_lseek(aikajono[ino]->afhandle, l = ((long)p + 1) * sizeof(aikatp), SEEK_SET) != l) {
	  writeerror_w(L"Virhe rutiinissa 'writeaika', koodi 1", 0);
	  return;
	  }
   gettm(p, (aikatp *)abuf, ino);
   if (_write(aikajono[ino]->afhandle, abuf, sizeof(aikatp)) != sizeof(aikatp))
	  writeerror_w(L"Virhe rutiinissa 'writeaika', koodi 2", 0);
   else {
      if (kirjheti & 8)
		 fflush(aikajono[ino]->fafhandle);
      }
   }
   
#define ABUFL 40

void writeajat(void)
   {
   int r,n,fl, ino;
   int l;
   char abuf[ABUFL*sizeof(aikatp)];

   for (ino = 0; ino <= maxjono; ino++) {
		if (!aikajono[ino])
			continue;
	   if ((fl = (aikajono[ino]->rwtime - aikajono[ino]->fp1)) <= 0)
		  continue;
		if (_lseek(aikajono[ino]->afhandle, l = (aikajono[ino]->fp1+1)*sizeof(aikatp), SEEK_SET)
			!= l) {
			writeerror_w(L"Virhe rutiinissa 'writeajat', koodi 1", 0);
			continue;
			}
		while (fl > 0) {
			n = min(ABUFL, fl);
			fl -= n;
		  for (r = 0; r < n; r++) {
			 gettm(aikajono[ino]->fp1++, (aikatp *)abuf+r, ino);
			 }
		  if (_write(aikajono[ino]->afhandle, abuf, n*sizeof(aikatp)) != n*sizeof(aikatp)) {
			 writeerror_w(L"Virhe rutiinissa 'writeajat', koodi 2", 0);
			 break;
			 }
		  }
	   if (kirjheti & 8)
		  fflush(aikajono[ino]->fafhandle);
	   aikajono[ino]->fp = aikajono[ino]->fp1;
	   }
   }

static struct {
	int cn;
	int cnino;
	int ino;
	} yhtj[4*MAXJONO];

static int nyhtjono;

int yhtjono(int cn, int ino)
{
	return(yhtaikajono[cn]);
}

void suljeajat(int s_ino, bool delfl)
   {
   int fl;
   int ino;

   writeajat();
	if (s_ino >= 0) {
	  ino = s_ino;
	  }
   else {
	  ino = 0;
		s_ino = maxjono;
		}
	for (; ino <= s_ino; ino++) {
		if (!aikajono[ino])
			continue;
		if (aikajono[ino]->afhandle) {
		   fl = (aikajono[ino]->rwtime+1)*sizeof(aikatp);
		   if (_filelength(aikajono[ino]->afhandle) > fl)
			   _chsize(aikajono[ino]->afhandle, fl);
			fclose(aikajono[ino]->fafhandle);
			aikajono[ino]->fafhandle = 0;
			aikajono[ino]->afhandle = 0;
			}
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
	wchar_t line[86],tark;
	INT ptime;
	aikatp ptm;
	INT d, ptpiste, ptosuus;
	kilptietue kilp;
	FILE *outfl = 0;
	wchar_t ln[120], st[80];

	if ((outfl = _wfopen(fname, L"wt")) != NULL) {
		fputws(L"Pvm\tKello\tJono\tStatus\tKilpno\tOsuus\tPiste\tBadge\tKanava\tLähde\tSeura\tJk\tTulos\tTark\tEro\n", outfl);
		for (ptime = 0; ptime - aikajono[akttm]->rwtime < 0; ptime++) {
			rivi[akttm]++;
			gettm(ptime, &ptm, akttm);
			ptpiste = ptm.piste;
			ptosuus = ptm.osuus;
			if (purajak(ptm.t) > TMAALI0)
				AIKATOWSTRS(line, purajak(ptm.t), t0);
			else
				wcscpy(line, L"  .  .  , ");
			swprintf(ln, L"%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d",
				time_t_date(ptm.date, NULL), line,
				(int)ptm.jono, (int)ptm.status, ptm.kno, ptosuus,
				ptpiste, ptm.badge, (int)ptm.kanava, (int)ptm.lahde);
			if (ptm.kno >= minkilpno && ptm.kno <= maxkilpno
				&& (d = getpos(ptm.kno)) != 0) {
				GETREC(&kilp, d);
				if (ptpiste > kilpparam.valuku || (ptpiste > -2 && kilp.Maali(ptosuus, ptpiste) != purajak(ptm.t)))
					tark = L'*';
				else
					tark = L' ';
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

void AjatTiedostosta(wchar_t *fname, int akttm, bool vainajat)
{
	TextFl *AikaFl;

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
					p = wcstok(NULL, L" ;\t");
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
							tp.osuus = _wtoi(p);
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
							tp.lahde = _wtoi(p);
							}
						}
					if (tp.t != AIKAJAK*TMAALI0)
						tall_rivi(akttm, &tp,NULL,NULL,0,0,0);
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
