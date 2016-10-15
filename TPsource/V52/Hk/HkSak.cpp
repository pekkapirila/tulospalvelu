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

#include <wincom.h>
#include <sys/timeb.h>
#define MAXPAIKKA 99
#define SAKQSIZE 400

void ampPaikka(int paikka);
void ampujaPaikalle(int paikka, int kno);
void Laukaus(int paikka, bool osuma, char *taulut);
extern INT sak_com, sak_baud, sak_laji;
static INT sak_msglen = 9, paikat[MAXPAIKKA];
void lahetaKurvinen(char *sanoma, int kielto);

static int cn_sakko;
static int err_kno, err_paikka;
static FILE *paikkafl;

#ifdef _CONSOLE
void ampPaikka(int paikka) {}
void ampujaPaikalle(int paikka, int kno) {}
void Laukaus(int paikka, bool osuma, char *taulut) {}
#endif

static INT tall_sak(char *cbuf)
   {
   wchar_t buf[20], *p, sakstr[20];
   INT kno, d, paikka, k, sak;
   kilptietue kilp;

   ansitowcs(buf, cbuf, sak_laji ? 14 : 10);
   if (sak_laji == 0) {
	  if (buf[4] != L' ' || buf[6] != L' ' ||
		 buf[7] < L'0' || buf[7] > L'0'+kilpparam.maxasakko) return(-2);
	  buf[4] = 0;
	  for (p = buf; *p == L' '; p++);
	  if (p - buf > 3) return(-2);
	  kno = _wtoi(p);
	  paikka = buf[5] - L'1';
	  k = paikka;
	  sak = buf[7];
	  }
   else {
	  k = 10*(buf[1]-L'0') + buf[2]-L'0';
	  if (k <= 0 || k > MAXPAIKKA)
		 return(-4);
	  kno = paikat[k-1];
	  paikat[k-1] = 0;
	  }
   if ((d = getpos(kno)) <= 0) {
	  err_kno = kno;
	  err_paikka = k;
	  return(-2);
	  }
   EnterCriticalSection(&tall_CriticalSection);
   kilp.GETREC(d);
   wmemcpy(sakstr, kilp.pv[k_pv].asakot, Sarjat[kilp.Sarja()].lsak[k_pv]+1);
   if (sak_laji) {
	  for (paikka = 0; paikka < Sarjat[kilp.Sarja()].lsak[k_pv] - 1; paikka++)
		 if (sakstr[paikka] < L'0') break;
	  sak = L'0' + (L'1'-buf[9]) + (L'1'-buf[10]) + (L'1'-buf[11]) +
		 (L'1'-buf[12]) + (L'1'-buf[13]);
//      sak = (int)buf[9]+buf[10]+buf[11]+buf[12]+buf[13]-4*'0';
	  }
   if (paikka < 0 || paikka >= Sarjat[kilp.Sarja()].lsak[k_pv]) return(-3);
   sakstr[paikka] = sak;
   kilp.tall_asakot(sakstr);
   kilp.tallenna(d,TRUE, 0, 0, 0);
   LeaveCriticalSection(&tall_CriticalSection);
//   add_sakko(d);
   return(0);
   }

void tall_ampsakko(char *sakbuf, int r_no)
   {
   static INT inrec, valid = 1;
   INT nmsg,er = 0, k;
   char *p, buf[10];
   wchar_t msg[100], *selitys;
//   static char sakbuf[50];
	wchar_t *selitykset[4] =
		{L"", L"(kilpailijanumero)", L"(ampumakerta)", L"(ampumapaikka)"};

	if (!paikkafl && er == 1) {
		int pos;
		paikkafl = _wfopen(L"__paikat__.txt", L"r+b");
		fseek(paikkafl, 0, SEEK_END);
		pos = ftell(paikkafl);
		if (pos != 150) {
			if (pos)
				rewind(paikkafl);
			for (int i = 0; i < 30; i++)
				fwrite("000\r\n", 5, 1, paikkafl);
			fclose(paikkafl);
			paikkafl = _wfopen(L"__paikat__.txt", L"r+b");
			}
		else {
			for (int i = 0; i < 30; i++) {
				fread(buf, 5, 1, paikkafl);
				buf[3] = 0;
				paikat[i] = atoi(buf);
				}
			}
		}
	if (r_no != 999)
		lahetaKurvinen(sakbuf, 0);
	if (inrec)
		return;
	inrec = 1;
	sak_msglen = sak_laji ? 15 : 9;
	sakbuf[20] = 0;
	p = strstr(sakbuf, "\r");
	if (p)
		p[1] = 0;
	nmsg = strlen(sakbuf);
   if (sak_laji == 0) {
	   er = tall_sak(sakbuf);
	  }
   else {
		if (valid) {
			switch (sakbuf[0]) {
				case '1':
					k = 10*(sakbuf[1]-'0') + sakbuf[2]-'0';
					paikat[k-1] = 0;
					if (paikkafl) {
						fseek(paikkafl, 5*(k-1), SEEK_SET);
						fwrite("000\r\n", 5, 1, paikkafl);
						fflush(paikkafl);
						}
					ampPaikka(k);
					break;
				case '2':
					if (nmsg == 7) {
						k = 10*(sakbuf[1]-'0') + sakbuf[2]-'0';
						if (k > 0 && k <= MAXPAIKKA) {
							paikat[k-1] = 100*(sakbuf[3]-'0') + 10*(sakbuf[4]-'0') + sakbuf[5]-'0';
							if (paikkafl) {
								fseek(paikkafl, 5*(k-1), SEEK_SET);
								memcpy(buf, sakbuf+3, 3);
								memcpy(buf+2, "\r\n", 3);
								fwrite(buf, 5, 1, paikkafl);
								fflush(paikkafl);
								}
							ampujaPaikalle(k, paikat[k-1]);
							}
						}
					else
						er = -1;
					break;
				case '5':
					if (nmsg == 15) {
						if (paikkafl) {
							k = 10*(sakbuf[1]-'0') + sakbuf[2]-'0';
							fseek(paikkafl, 5*(k-1), SEEK_SET);
							fwrite("000\r\n", 5, 1, paikkafl);
							fflush(paikkafl);
							}
						if (r_no != 999)
							er = tall_sak(sakbuf);
						}
					else
						er = -1;
					break;
				case '4':
				case '9':
						Laukaus(10*(sakbuf[1]-'0')+sakbuf[2]-'0', sakbuf[0] == L'4', sakbuf+3);
					break;
				default:
					break;
				}
			}
			valid = 1;
		 }
   if (er) {
		if (er == -2) {
			swprintf(msg, L"Virheellinen sanoma ampumapaikalta. Kilpailija no %d paikalla %d",
				err_kno, err_paikka);
			}
		else {
			if (er < -1 && er > -5)
				selitys = selitykset[-er-1];
			else
				selitys = selitykset[0];
			swprintf(msg, L"Viallinen sanoma ampumapaikalta, virhe %d %s",
				er, selitys);
			}
		writeerror_w(msg, 2000, true);
		if (loki) {
			wcscat(msg, L": ");
			sakbuf[nmsg] = 0;
			if (nmsg > 0 && sakbuf[nmsg-1] == '\r')
				sakbuf[nmsg-1] = 0;
			ansitowcs(msg+wcslen(msg), sakbuf, 20);
			wkirjloki(msg);
			}
	  }
   inrec = 0;
   }

void tulkKurvinen(char *sanoma, int kielto)
{
	char buf[50] = "";

	strncpy(buf, sanoma, 15);
	tall_ampsakko(buf, 999);
}

void close_sakot(void)
{
	if (paikkafl)
		fclose(paikkafl);
}

