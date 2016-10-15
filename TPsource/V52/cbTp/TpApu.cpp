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

#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif

void rem_pr(void);

void wkirjloki(wchar_t *tmsg)
	{
/*
	wchar_t msg[500];
	char cmsg[500];
	int nsent;

	wcsncpy(msg, tmsg, 497);
	msg[497] = 0;
	wcscat(msg, L"\n");
	if (!lokiTCP && lokitulparam.lstf == NULL)
		return;
	EnterCriticalSection(&loki_CriticalSection);
	if (siritloki) {
		wchar_t buf[SIRITBUFLEN+2];
		int head;

		memset(buf, 0, sizeof(buf));
		head = sirithead;
		if (head > sirittail)
			wmemcpy(buf, siritbuf+sirittail, head-sirittail);
		else {
			wmemcpy(buf, siritbuf+sirittail, SIRITBUFLEN-sirittail);
			wmemcpy(buf+SIRITBUFLEN-sirittail, siritbuf, head);
			}
		sirittail = head;
		if (lokiTCP) {
			wcstoansi(cmsg, buf, 499);
			wrt_st_TCP(hLoki, strlen(cmsg), cmsg, &nsent);
			}
		else
			sendwline(buf, lokitulparam.lstf);
		}
	if (lokiTCP) {
		wcstoansi(cmsg, msg, 499);
		wrt_st_TCP(hLoki, strlen(cmsg), cmsg, &nsent);
		}
	else
		sendwline(msg, lokitulparam.lstf);
	LeaveCriticalSection(&loki_CriticalSection);
*/
	}

int wcstomaksu(wchar_t *st)
{
	wchar_t *p;

	if (!st || !*st)
		return(0);
	if ((p = wcsstr(st, L",")) != NULL )
		*p = L'.';
	return((int)(100*_wtof(st)+0.1));
}

void viwrrectw(int r1, int c1, int r2, int c2, const wchar_t *txt, int fg, int bg, int opt);

void vidint2(int y, int x, int l, long i)
	{
	if (i > 999) {
	  viwrrectw(y, x, y, x, L"*", 7, 0, 0);
	  i = i % 100;
	  x++;
		l--;
	  }
	vidint(y, x, l, i);
	}

wchar_t *poista_viiva(wchar_t *stout, wchar_t *stin, int size)
{
	wchar_t *pi, *po;

	memset(stout, 0, size);
	for (pi = stin, po = stout; *pi && po < stout + size - 1; pi++, po++) {
		if (*pi == L'_')
			*po = L' ';
		else
			*po = *pi;
		}
	return(stout);
}

/*
void putintrightw(int i, wchar_t *p)
   {
   if (i <= 0) return;
   for (;;) {
      *p = L'0' + (wchar_t) (i % 10);
      if (i < 10) break;
      p--;
      i = i/10;
	  }
   }
*/

INT16 i16swab(char *s)
{
	short k;
	_swab(s, (char *)&k, 2);
	return(k);
}

wchar_t *wcstokch(wchar_t *st, wchar_t erotin)
   {
   static wchar_t *p2;
   wchar_t *p;

   if (st) {
	  p2 = st;
	  }
   if (!p2 || !*p2)
	  return(0);
   p = p2;
   while (*p2 && *p2 != erotin)
	  p2++;
   if (*p2) {
	  *p2 = 0;
	  p2++;
	  }
   return(p);
   }

INT32 normkello_a(INT32 t) {
	if (t == (1000L/KSEK) * TMAALI0)
		return(t);
	while (t > 12L*3600*10000)
		t -= 24L*3600*10000;
	while (t <= -12*3600*10000)
		t += 24L*3600*10000;
	return(t);
}

INT32 normtulos_a(INT32 t) {
	while (t >= 24L*3600*10000)
		t -= 24L*3600*10000;
	while (t < 0)
		t += 24L*3600*10000;
	return(t);
}

wchar_t *aikatowstr_cols_n(wchar_t *as, INT32 tl, int t0, wchar_t sep, int len)
{
	AIKATOWSTRS(as, tl, t0);
	as[len] = 0;
	for (wchar_t *p = as; *p; p++) {
		if (*p == L'.')
			*p = L':';
		if (sep && *p == L',')
			*p = sep;
		}
	return(as);
}

char *aikatostr_cols_n(char *as, INT32 tl, int t0, char sep, int len)
{
	AIKATOSTRS(as, tl, t0);
	as[len] = 0;
	for (char *p = as; *p; p++) {
		if (*p == '.')
			*p = ':';
		if (sep && *p == ',')
			*p = sep;
		}
	return(as);
}

wchar_t *eroaikatowstr(wchar_t *as, int ero, int len)
{
	if (len < 8)
		len = 8;
	if (ero == 0) {
		wcscpy(as, L"0,00");
		as[len-7] = 0;
		}
	else {
		aikatowstr_cols_n(as, abs(ero), 0, 0, len);
		elimwzb1(as);
		wmemmove(as+1, as, wcslen(as)+1);
		as[0] = (ero < 0) ? L'-' : L'+';
		}
	return(as);
}

char *eroaikatostr(char *as, int ero, int len)
{
	wchar_t ws[20];

	wcstoansi(as, eroaikatowstr(ws, ero, len), len);
	return(as);
}

INT32 strtoaika_vap(char *str, int t0)
{
	wchar_t s[20];

	return(wstrtoaika_vap(ansitowcs(s, str, 19), t0));
}

INT32 wstrtoaika_vap(wchar_t *str, int t0)
{
	wchar_t s[20] = L"", *p, *p1;
	INT32 t = 0;

	for (p = str; *p; p++) {
		if (*p >= L'0' && *p <= L'9')
			break;
		}
	if (*p == 0)
		return(t0 ? TMAALI0 : 0);

	wcsncpy(s, str, 16);
	if (wcsstr(s, L":") != NULL) {
		if ((p = wcsstr(s, L".")) != NULL)
			*p = L',';
		for (p = s; *p; p++)
			if (*p == L':')
				*p = L'.';
		}
	p1 = s + wcslen(s);
	if ((p = wcsstr(s, L",")) != NULL) {
		p1 = p;
		wcscat(s, L"000");
		p[4] = 0;
		if (KSEK == 10)
			p[3] = 0;
		t = _wtol(p+1);
		}
	*p1 = 0;
	for (p = p1-1; p >= s && *p != L'.'; p--);
	if (p < p1-1)
		t += SEK*_wtol(p+1);
	p1 = p;
	if (p1 > s) {
		*p1 = 0;
		for (p = p1-1; p >= s && *p != L'.'; p--);
		if (p < p1-1)
			t += MINUUTTI*_wtol(p+1);
		p1 = p;
		if (p1 > s) {
			*p1 = 0;
			for (p = p1-1; p >= s && *p != L'.'; p--);
			if (p < p1-1)
				t += TUNTI*_wtol(p+1);
			}
		}
	return(t-t0*TUNTI);
}
#ifndef MAXOSUUSLUKU
int haesarja_w(wchar_t *snimi, bool salliyhd)
{
   int i;

   i = 0;
   upcasewstr(snimi);
   wstpcvt(snimi, 6);
   do {
	  if (!wcscmp(snimi, Sarjat[i].sarjanimi))
		 return(i);
   } while (++i < sarjaluku + (salliyhd ? nsarjayhd : 0));
   return(-1);
}
#endif
static struct {
	int cn;
	int cnino;
	int ino;
	} yhtj[4*MAXJONO];

static int nyhtjono;

void lueyhteysjonot(void)
{
	wchar_t line[100], *p, filenm[] = L"yhtjonot.txt";
	TextFl *yhtfile;

	line[sizeof(line)/2-1] = 0;
	yhtfile = new TextFl(filenm, L"rt");
	if (yhtfile->IsOpen()) {
		while (!yhtfile->Feof() && nyhtjono < 4*MAXJONO) {
			if (yhtfile->ReadLine(line, sizeof(line)/2-1) != NULL) {
				p = wcstok(line, L" ,;\t\n");
				if (p) {
					if ((yhtj[nyhtjono].cn = _wtoi(p)) > 0)
						p = wcstok(NULL, L" ,;\t\n");
					else
						p = NULL;
					}
				if (p) {
					if ((yhtj[nyhtjono].cnino = _wtoi(p)) > 0)
						p = wcstok(NULL, L" ,;\t\n");
					else
						p = NULL;
					}
				if (p) {
					if ((yhtj[nyhtjono].ino = _wtoi(p)) > 0
						&& yhtj[nyhtjono].ino <= MAXJONO) {
						nyhtjono++;
						if (yhtj[nyhtjono].ino > maxjono)
							maxjono = yhtj[nyhtjono].ino;
						}
					}
				}
			}
		delete yhtfile;
		}
}

int yhtjono(int cn, int ino)
{
	int j;

	for (j = 0; j < nyhtjono; j++)
		if (cn+1 == yhtj[j].cn && ino+1 == yhtj[j].cnino)
			return(yhtj[j].ino);
	return(0);
}


static FILE *err_file;
static int n_err_file;
int select3(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, TComponent *Sender);

void kirj_err_file(wchar_t *msg, int nayta)
	{
	static int lopilm;
	wchar_t msg2[200];

	if (nayta && !lopilm) {
#ifdef _CONSOLE
		wchar_t ch = L'J';

		vidspwmsg(ySize-1,0,0,7,msg);
		wcscpy(msg2, L"J)atka, L)opeta nämä virheilmoitukset");
		wselectopt(msg2, L"JL", &ch);
		if (ch == L'L')
			lopilm = 1;
#else
		int vast;

		swprintf(msg2, L"%s \nJatketaanko näiden virheilmoitusten näyttämistä", msg);
		vast = select3(2, msg2, L"Toistuva virheilmoitus", L"Näytä edelleen", L"Ohita ilmoitukset jatkossa", L"", 0);
		if (vast == 2)
			lopilm = 1;
#endif
		}
	if (!err_file) {
		err_file = _wfopen(L"DATA_ERR.TXT", L"wt");
		}
	if (err_file) {
		n_err_file++;
		fputws(msg, err_file);
		fputwc(L'\n', err_file);
		}
	}

void close_err_file(void)
	{
	if (err_file) {
		wchar_t msg[200];
		fclose(err_file);
		err_file = 0;
		swprintf(msg, L"Tiedostoon DATA_ERR.TXT kirjoitettu %d virhetietoa", n_err_file);
#ifdef _CONSOLE
		writeerror_w(msg, 0);
#else
		if (select3(2, msg, L"Virheitä tiedoissa", L"Näytä tiedot", L"Ohita", L"", 0) == 1)
			nayta_virhesanomat();
#endif
		}
	}

void Perr(int ok, wchar_t *api)
{
	wchar_t line[200];
	if (ok) return;
	swprintf(line, L"Virhe %d, API: %s", GetLastError(), api);
	writeerror_w(line, 0);
}

#ifndef MAXOSUUSLUKU
__fastcall seuraTieto::seuraTieto(void)
{
	memset(this, 0, sizeof(seuraTieto));
}
//---------------------------------------------------------------------------

__fastcall seuraTieto::~seuraTieto(void)
{
}
//---------------------------------------------------------------------------
int seuraTieto::tulkRivi(wchar_t *iBuf, wchar_t *erotin, int jarj, bool maakoodit)
{
	wchar_t *p, Buf[201];

	wcsncpy(Buf, iBuf, 200);
	Buf[200] = 0;
	p = wcstok(Buf, erotin);
	if (p) {
		piiri =  _wtoi(p);
		p = wcstok(NULL, erotin);
		}
	if (maakoodit) {
		if (p) {
			wcsncpy(maa, p, 3);
			p = wcstok(NULL, erotin);
			}
		}
	if (p) {
		wcsncpy(lyhenne, p, kilpparam.lseuralyh);
		}
	if (p) {
		for (int i = p - Buf + wcslen(p) + 1; i < (int) wcslen(iBuf); i++) {
			if (Buf[i] != 0) {
				p = Buf+i;
				break;
				}
			}
		wcscpy(Buf, p);
		}
	elimwbl2(Buf);
	Buf[kilpparam.lseura] = 0;
	wcscpy(nimi, Buf);
	if (kilpparam.seuralyh_on)
		keyfromwname(key, lyhenne, sizeof(key), 0);
	else
		keyfromwname(key, Buf, sizeof(key), 0);
	return(0);
}
//---------------------------------------------------------------------------
__fastcall seuraLuettelo::seuraLuettelo(wchar_t *SraFname, int Jarj)
{
	memset(this, 0, sizeof(seuraLuettelo));
	jarj = Jarj;
	if (SraFname)
		lueSeurat(SraFname);
}
//---------------------------------------------------------------------------

__fastcall seuraLuettelo::~seuraLuettelo(void)
{
	if (Seura)
		delete[] Seura;
}
//---------------------------------------------------------------------------

void seuraLuettelo::nollaa(void)
{
	if (Seura)
		delete[] Seura;
	memset(this, 0, sizeof(seuraLuettelo));
}
//---------------------------------------------------------------------------

int seuraLuettelo::lueSeurat(wchar_t *SraFName)
{
	EnterCriticalSection(&seura_CriticalSection);
	if (SraFName[0]) {
		wcsncpy(FName, SraFName, sizeof(FName)/2-1);
		TextFl *InFile = new TextFl(SraFName, L"t");
		if (InFile->IsOpen()) {
			wchar_t Buf[1000];
			int N = 0;

			while (!InFile->Feof()) {
				if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 5) {
					N++;
					}
				}
			if (N > 0) {
				bool ensrivi = true;

				NSeuraMax = N+1000;
				Seura = new seuraTieto[NSeuraMax];
				InFile->Rewind();
				NSeura = 0;
				while (!InFile->Feof()) {
					if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 5) {
						if (ensrivi && otsrivi) {
							ensrivi = false;
							continue;
							}
						if (!Seura[NSeura].tulkRivi(Buf, erotin, jarj, maakoodit))
							NSeura++;
						}
					}
				}
			InFile->Close();
			}
		delete InFile;
		}
	if (NSeura > 1) {
		quicksort((char *)Seura, NSeura, sizeof(Seura[0].key), sizeof(Seura[0]));
		}
	LeaveCriticalSection(&seura_CriticalSection);
	return(0);
}
//---------------------------------------------------------------------------

int seuraLuettelo::kirjSeurat(wchar_t *SraFName)
{
	TextFl *OutFile;

	EnterCriticalSection(&seura_CriticalSection);
	if (SraFName && SraFName[0])
		OutFile = new TextFl(SraFName, L"w");
	else
		OutFile = new TextFl(FName, L"w");
	if (OutFile->IsOpen()) {
		wchar_t Buf[1000], *p;

		for (int i = 0; i < NSeura; i++) {
			for (p = Seura[i].lyhenne; *p; p++) {
				if (*p == L' ')
					*p = L'_';
				}
			if (wcslen(Seura[i].lyhenne) == 0) {
				wcsncpy(Seura[i].lyhenne, Seura[i].nimi, kilpparam.lseuralyh);
				Seura[i].lyhenne[kilpparam.lseuralyh-1] = 0;
				if (wcslen(Seura[i].lyhenne) == 0)
					wcscpy(Seura[i].lyhenne, L"_");
				}
			if (wcslen(Seura[i].nimi) == 0) {
				wcscpy(Seura[i].nimi, Seura[i].lyhenne);
				}
			if (maakoodit)
				swprintf(Buf, L"%d\t%s\t%s\t%s\n", Seura[i].piiri, Seura[i].maa, Seura[i].lyhenne, Seura[i].nimi);
			else
				swprintf(Buf, L"%d\t%s\t%s\n", Seura[i].piiri, Seura[i].lyhenne, Seura[i].nimi);
			OutFile->WriteLine(Buf);
			}
		OutFile->Close();
		}
	delete OutFile;
	LeaveCriticalSection(&seura_CriticalSection);
	return(0);
}
//---------------------------------------------------------------------------

void _fastcall seuraLuettelo::TallennaSeura(int piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi, int kielto)
{
	char hkey[22];
	seuraTieto *pSeura, Sra;

	if (Seura) {
		if (jarj == 2)
			keyfromwname(hkey, lyhenne, sizeof(hkey), 0);
		else
			keyfromwname(hkey, nimi, sizeof(hkey), 0);
		pSeura = (seuraTieto *) bsrch(hkey, (char *) Seura, &NSeura, sizeof(Seura[0].key), sizeof(Seura[0]));
		if (!pSeura || wcscmp(pSeura->nimi, nimi)) {
			memset(&Sra, 0, sizeof(Sra));
			Sra.piiri = piiri;
			memcpy(Sra.key, hkey, sizeof(Sra.key));
			wcsncpy(Sra.maa, maa, 3);
			wcsncpy(Sra.lyhenne, lyhenne, kilpparam.lseuralyh);
			wcsncpy(Sra.nimi, nimi, kilpparam.lseura);
			if (NSeura < NSeuraMax) {
				EnterCriticalSection(&seura_CriticalSection);
				Seura[NSeura++] = Sra;
				quicksort((char *)Seura, NSeura, sizeof(Seura[0].key), sizeof(Seura[0]));
				LeaveCriticalSection(&seura_CriticalSection);
				kirjSeurat(FName);
				if (NSeura == NSeuraMax) {
					lueSeurat(FName);
					}
				}
			}
		}
	lah_seura(nimi, lyhenne, maa, piiri, kielto);
}
//---------------------------------------------------------------------------

seuraTieto *seuraLuettelo::haenimella(wchar_t *nimi, bool tarkka)
{
	if (NSeura == 0)
		return(NULL);
	if (!tarkka && jarj == 1) {
		char key[30];

		keyfromwname(key, nimi, sizeof(key), false);
		return((seuraTieto *)bsrch(key, (char *)Seura, &NSeura, sizeof(Seura[0].key), sizeof(Seura[0])));
		}
	for (int is = 0; is < NSeura; is++) {
		if (!wcscmp(Seura[is].nimi, nimi))
			return(Seura+is);
		}
	return(NULL);
}
//---------------------------------------------------------------------------
seuraTieto *seuraLuettelo::haelyhenteella(wchar_t *lyhenne, bool tarkka)
{
	if (NSeura == 0)
		return(NULL);
	if (!tarkka && jarj == 2) {
		char key[30];

		keyfromwname(key, lyhenne, sizeof(key), false);
		return((seuraTieto *)bsrch(key, (char *)Seura, &NSeura, sizeof(Seura[0].key), sizeof(Seura[0])));
		}
	for (int is = 0; is < NSeura; is++) {
		if (!wcscmp(Seura[is].lyhenne, lyhenne))
			return(Seura+is);
		}
	return(NULL);
}
//---------------------------------------------------------------------------
wchar_t *seuraLuettelo::nimi(wchar_t *lyhenne, bool tarkka)
{
	static wchar_t tyhja[2] = L"";
	seuraTieto *pSeura;

	if ((pSeura = haelyhenteella(lyhenne, tarkka)) != NULL) {
		return(pSeura->nimi);
		}
	return(tyhja);
}
//---------------------------------------------------------------------------
wchar_t *seuraLuettelo::lyhenne(wchar_t *nimi, bool tarkka)
{
	static wchar_t tyhja[2] = L"";
	seuraTieto *pSeura;

	if ((pSeura = haenimella(nimi, tarkka)) != NULL) {
		return(pSeura->lyhenne);
		}
	return(tyhja);
}
//---------------------------------------------------------------------------
#endif
#ifdef _CONSOLE
void clearframe63(void)
   {
   INT r;

   for (r = 2; r < ySize-4; r++) clrtxt(r,0,63);
   }

void clearframe(void)
   {
   INT r;

   for (r = 2; r < ySize-4; r++) clrln(r);
   }

INT luesarja(wchar_t *snimi, wchar_t *tc)
	{
	INT  srj = -1;
	wchar_t   sn[16] = L"         " ;

	if (snimi) {
		wcscpy(sn, snimi);
		srj = haesarja_w(sn, false);
		}
	do {
		vidspwmsg(ySize-1,0,7,0, L"Sarjoja voi selata näppäimillä \x19 \x18 PgDn PgUp");
		do {
			if (srj >= 0 && srj < sarjaluku)
				wcscpy(sn, Sarjat[srj].sarjanimi);
		 inputwstr(sn,10,20,ySize-3,L"\x0D\x1B\xCA\xCC\x05\x18\x03\x12",tc,FALSE);
			switch (*tc) {
				case PGUP :
					srj = 0;
					break;
				case PGDN :
					srj = sarjaluku-1;
					break;
				case UP :
					if (srj > 0) srj--;
					else srj = 0;
					break;
				case DOWN :
					if (srj < 0)
						srj = 0;
					else
						if (srj < sarjaluku-1)
							srj++;
					break;
				}
			} while(*tc == UP || *tc == DOWN || *tc == PGUP || *tc == PGDN);
		clrln(ySize-1);
		if (*tc == 202 || *tc == 204) srj = 0;
		else if(*tc != ESC) {
			srj = haesarja_w(sn, false);
			if (srj < 0) {
				erbeep();
				vidspwmsg(ySize-1,22,7,0,L"Virheellinen sarja");
				}
			}
		else srj = sarjaluku+1;
		} while (srj < 0 && *tc != ESC);
	clrln(ySize-1);
	return(srj);
	}

#else

void clearframe63(void)
   {
   }

void clearframe(void)
	{
   }

INT luesarja(wchar_t *snimi, wchar_t *tc)
{
	return(0);
}
#endif
