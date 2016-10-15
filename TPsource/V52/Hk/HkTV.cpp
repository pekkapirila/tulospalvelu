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
#include <stdlib>
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

#if defined(TV1)
#define tvqsize 2000

INT openport(INT portti, INT irq, INT addr, INT baud, INT inqsize,
	INT outqsize, char  *buf);

FILE *tv_file;
int tv_val[MAXSARJALUKU][VALUKU+3];

void opentv(void)
	{
	char  *tvbuf, line[100], *p;
	INT er = 1, i, k, l, srj = 0;
	FILE *tvparfl;

	for (k = 0; k < MAXSARJALUKU; k++) {
		for (i = 0; i < kilpparam.valuku+3; i++)
			tv_val[k][i] =  i-1;
		}
	if ((tvparfl = _wfopen(L"TVPISTE.LST", L"rt")) != 0) {
		for (k = 0; k < MAXSARJALUKU; k++) {
			for (i = 2; i < kilpparam.valuku+2; i++)
				tv_val[k][i] =  -2;
			}
		while (!feof(tvparfl)) {
			if (fgetws(line, 98, tvparfl)) {
				if ((p = wcstok(line, L" \t\n")) == 0)
					continue;
				if ((k = haesarja_w(p, false)) >= 0) {
					srj = k;
					}
				else {
					k = _wtoi(p);
					if ((p = wcstok(NULL, L" \t\n")) != 0) {
						l = _wtoi(p);

						if (k >= 1 && k <= kilpparam.valuku) {
							tv_val[srj][k+1] = l;
							}
						}
					}
				}
			}
		fclose(tvparfl);
		}
	if (tv_test)
		tv_com = 999;
	if (tv_com == 999) {
		tv_file = _wfopen(L"TVFILE.TXT", L"wb");
		if (tv_file) er = 0;
		}
	else {
		cn_tv = TV1COM;
		if (openport_x(cn_tv, tv_com, tv_baud, L'n', 8, 1, 0)) {
			writeerror_w(L"TV-tiedonsiirron avaaminen ei onnistunut", 0);
			tv_com = 0;
			return;
			}
			}
		}
	if (er) {
		writeerror_w(L"Tiedonsiirtoa TV:lle ei avata", 0);
		tv_com = 0;
		}
	}

static void near lah_tv(kilptietue *kilp, INT piste, INT sj)
	{
	char rivi[26], ast[16], tulkp[] = L"LM123456789:;";
	static INT32 edkarki;
	INT n, piste2;

	static INT edsrj = -1;
	kilptietue kilp0;


	if (piste > kilpparam.valuku)
		return;
	piste2 = piste;
//      piste = 0;
	if (piste > 0)
		piste2 = tv_val[kilp->Sarja()][piste+1];
	if (piste2 < -1)
		return;

	if (k_pv && tulosmuot.nollakarki) {
	  if (edsrj != kilp->Sarja()) {
			n = getpos(Sarjat[kilp->Sarja()].sarjaalku);
			if (n > 0)
				kilp0.GETREC(n);
			if (n <= 0 || kilp0.Sarja() != kilp->Sarja()) {
				writeerror_w(L"Sarjatiedossa väärä alkunumero, takaa-ajon ajat TV:lle väärin", 0);
				}
			else {
				edkarki = kilp0.ytulos(k_pv - 1);
#ifdef HIIHTO
				edkarki = 10L*(edkarki/10L);
#endif
				}
			edsrj = kilp->Sarja();
			}
		tulkp[kilpparam.valuku+2] = L'M';
		if (!piste) piste = kilpparam.valuku+1;
		}
	if ((piste == 0 || piste == kilpparam.valuku+1) && !hyv(kilp))
		wcscpy(ast, L"00.00.00,00");
	else {
		if (k_pv && !piste)
			piste = kilpparam.valuku+1;
		AIKATOWSTRS(ast, kilp->tulos2(piste)-edkarki, piste < 0 ? t0 : 0));
		}
#ifndef SADASOSA_XX
	swprintf(rivi, L"#%1.1c%3.3d%2.2s%2.2s%2.2s%1.1s%3.3d\r\n", tulkp[piste2+1],
		kilp->id(), ast, ast+3, ast+6, ast+9, sj);
#else
	swprintf(rivi, L"#%1.1c%3.3d%1.1s%2.2s%2.2s%2.3s%3.3d\r\n", tulkp[piste2+1],
		kilp->id(), ast+1, ast+3, ast+6, ast+9, sj);
#endif
	if (tv_test)
		fputws(rivi, tv_file);
	else
		wrt_st_x(cn_tv, wcslen(rivi), rivi, &n);
	}

void tv_asetus(void)
	{
	static INT in_tv, tv_com0;
	INT piste, sj, p, i, tv0, d, lj, srj = 0;
	INT32 tls0 = 0;
	kilptietue kilp;
	char ch;

	tv_fl = 0;
	if (in_tv) return;
	in_tv = 1;
	if (tv_com) tv_com0 = tv_com;
	ch = L' ';
	while (!nilm[srj]) srj++;
	Wselectopt(L"K)äynnista lähetys, L)opeta lähetys, U)usintalähetys", L"KLU", &ch);
	clrln(ySize-3);
	switch (ch) {
		case L'K' :
			tv_com = tv_com0;
			break;
		case L'L' :
			tv_com = 0;
			break;
		case L'U':
			ch = L' ';
			Wselectopt(L"L)ähtöajat, T)ulokset vai väliaika 12345", L"LT12345", &ch);
			clrln(ySize-3);
			tv0 = tv_com;
			piste = wcswcind(ch, L"LT12345") -1;
			tv_com = tv_com0;

			if (k_pv == !piste) {

				piste = kilpparam.valuku+1;
				}

			if (piste <= 0) {
				p = jalku[srj][0];
				lj = 0;
			}
			else {
				p = jalku[srj][4+piste];
				lj = piste;
				}
			i = 0;
			for (;;) {
				if (piste == -1) {
					while (p < 0 && lj++ < 4) {
						p = jalku[srj][lj];
						}
					if (lj > 4) break;
					}
				if (p < 0) break;
				i++;
				d = p;
			p = JarjSeur(0, piste > 0 ? piste : 0, d);

				kilp.GETREC(d);
				if (piste == -1) {
					sj = 0;
					}
				else {
					if (kilp.tulos2(piste) != tls0) sj = i;
					tls0 = kilp.tulos2(piste);
					}
				lah_tv(&kilp, piste, sj);
				}
			tv_com = tv0;
			break;
		}
	in_tv = 0;
	}
#endif


