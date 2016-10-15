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
#include "VDeclare.h"

int EmitHtmlSrjOts(int sarja, tulostusparamtp *tulprm)
   {
   wchar_t buf[40];

   tulprm->writehtml(L"<h2>");
   tulprm->writehtml(Sarjat[sarja].Sarjanimi(buf));
   tulprm->writehtml(L"</h2>\n");
   return(0);
   }

int EmitHtmlSrjLoppu(tulostusparamtp *tulprm)
   {
   return(0);
   }

void EmitHtmlTulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm)
   {
#ifndef LUENTA
   wchar_t as[16], st[80], st1[40], line[1000], stspan[20];
   INT edva, va[51];
   emittp em;
   ratatp *rt;
   INT ep = -1, os, i, sj1, maxrlkm;
   INT32 tl, tl0 = TMAALI0;

	if (kilp.Maali(0, 0) != TMAALI0 ||
		(wcswcind(kilp.Tark(0, 1), L"E-") < 0)) {

		if (kilp.Hyv()) {
			if ((tl = tulos(&kilp, Sarjat[kilp.sarja].osuusluku-1, 0)) != 0) {
				if (tl != tl0)
					sj1 = sj;
				tl0 = tl;
				aikatowstr_cols_n(as, tl, 0, tulprm->decsep, kilpparam.laika2);
				wpoistanollat(as);
				}
			swprintf(st, L"<p><b>%d. %d ", sj1, kilp.kilpno);
			}
		else {
			swprintf(st, L"<p><b>%d ", kilp.kilpno);
			as[0] = 0;
			if (wcswcind(kilp.Tark(Sarjat[kilp.sarja].osuusluku-1, 1), L"KHSE") >= 0) {
				if (tulprm->language == 0)
					wcscpy(as, L"Keskeyttänyt");
				else
					wcscpy(as, L"DNF");
				for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
					if (kilp.Tark(os, 1) == L'H') {
						if (tulprm->language == 0)
							wcscpy(as, L"Hylätty");
						else
							wcscpy(as, L"DQ");
						break;
						}
					}
				}
			}
		tulprm->writehtml(st);
		tulprm->writehtml(kilp.Joukkue(st, 60));
		tulprm->writehtml(L" ");
		tulprm->writehtml(kilp.Maa());
		tulprm->writehtml(L" ");
		tulprm->writehtml(as);
		maxrlkm = 0;
		for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			rt = getrata(kilp.ostiet[os].hajonta);
			if (rt && rt->rastiluku > maxrlkm)
				maxrlkm = rt->rastiluku;
			}
		swprintf(stspan, L"<th colspan=%d>", maxrlkm > 4 ? 4 : maxrlkm);
		tulprm->writehtml(L"</b></p>\n");
		tulprm->writehtml(L"<table class='jkrva'>\n");
		if (tulprm->language == 0) {
			tulprm->writehtml(L"<tr><th>Osuus</th><th>Nimi</th><th>Rata</th><th>OsTls</th>");
			tulprm->writehtml(stspan);
			if (tulprm->rastivali)
				tulprm->writehtml(L"<em>Rastikoodi:</em> Rastivälin aika</th>");
			else
				tulprm->writehtml(L"<em>Rastikoodi:</em> Aika rastille</th>");
			}
		else {
			tulprm->writehtml(L"<tr><th>Leg</th><th>Name</th><th>Course</th><th>Legtime</th>");
			tulprm->writehtml(stspan);
			if (tulprm->rastivali)
				tulprm->writehtml(L"<em>Control code:</em> Control to control</th>");
			else
				tulprm->writehtml(L"<em>Control code:</em> Start to control</th>");
			}
		line[0] = 0;
		if (maxrlkm > 4) {
			for (int ii = 5; ii <= maxrlkm; ii++)
				swprintf(line+wcslen(line), L"<th>%d</th>", ii);
			}
		wcscat(line, L"</tr>\n");
		tulprm->writehtml(line);
		for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			ep = getem(&em, kilp.kilpno, os);
			if (kilp.Maali(os, 0) != TMAALI0 || ep >= 0) {
				if (kilp.osHyv(os)) {
					aikatowstr_cols_n(as, kilp.osTulos(os, 0), 0, tulprm->decsep, kilpparam.laika2);
					wpoistanollat(as);
					}
				else {
					wcscpy(as, kilp.TarkStr(os, false, tulprm->language));
					}
				swprintf(line, L"<tr>\n<td>%d</td><td>%s</td><td>%s</td><td>%s</td>",
					os+1, kilp.Nimi(st, kilpparam.lnimi, os, -1), kilp.Hajonta(os, st1), as);
//				tulprm->writehtml(line);
				if (ep >= 0) {
					vatulkinta(&em, va);
					rt = getrata(kilp.ostiet[os].hajonta);
					if (rt && rt->rastiluku != 0) {
						for (i = 0; i < rt->rastiluku; i++) {
							if (va[i]) {
								if (!tulprm->rastivali)
									aikatowstr_cols_n(as, SEK*va[i], 0, tulprm->decsep, 8);
								else {
									if (i > 0)
										edva = va[i-1];
									else
										edva = 0;
									aikatowstr_cols_n(as, SEK*(va[i]-edva), 0, tulprm->decsep, 8);
									}
								wpoistanollat(as);
								if (tulprm->tulmuot.tauluhtml & 4) {
									while (wcslen(as) < (tulprm->rastivali ? 5 : 6)) {
										wmemmove(as+1, as, wcslen(as)+1);
										as[0] = L' ';
										}
									}
								swprintf(line+wcslen(line), L"<td><em>%d:</em> %s</td>", rt->rastikoodi[i], as);
								}
							else {
								wcscat(line, L"<td/>");
								}
							}
						if (!rt || maxrlkm > rt->rastiluku)
							swprintf(line+wcslen(line), L"<td colspan=%d></td>",
								rt == NULL ? maxrlkm : maxrlkm - rt->rastiluku);
						wcscat(line, L"\n");
						tulprm->writehtml(line);
						}
					}
				else

				tulprm->writehtml(L"</tr>\n");
				}
			}
		tulprm->writehtml(L"</table>\n");
		}
#endif
}

void EmitHtmlOsuus(kilptietue& kilp, int osuus, INT sj, tulostusparamtp *tulprm)
   {
#ifndef _CONSOLE
	extern rastivatp *rastiva;
	wchar_t as[16], st[80], st1[40], st2[40], st3[20], line[1000], stspan[20];
   INT edva, va[51];
   emittp em;
   ratatp *rt;
   INT ep = -1, os, i, sj1, maxrlkm;
   INT32 tl, tl0 = TMAALI0;
	kilptietue kilp1;
	emitvaanaltp *eva = 0;
	rastivaanaltp *rstva = 0;
	int d, r, p, lj, ntlsrivi = 50, aos, yos;
	int edaika, tls, t1, karki, emvaluku, srj, ero;
   static bool otsrivit = false;

	rt = haerata(&kilp, osuus);
	if (!rt || !rt->rastiluku) return;
	srj = kilp.sarja;
	yos = Sarjat[srj].yosuus[osuus];
	EnterCriticalSection(&print_CriticalSection);
	initline(tulprm);
	tls = kilp.osTulos(osuus, 0);
	swprintf(st2, L"%d-%s", kilp.kilpno, wosuuskoodi(srj, osuus, 0, 0));
	if (tls && kilp.osHyv(osuus)) {
		AIKATOWSTRS(st, tls, 0);
		st[kilpparam.laika2] = 0;
		elimwzb1(st);
		swprintf(st1, kilp.osSija(osuus), st);
		}
	getem(&em, kilp.kilpno, osuus);
	if (tulprm->rivi == 0)
		otsrivit = true;

	if (emitanal_fl && em.kilpno == kilp.kilpno) {
		EnterCriticalSection(&rastiva_CriticalSection);
		laskeemitvaliajat(&em, &kilp, &eva, &rstva, &emvaluku);
		if (eva) {
			if (otsrivit) {
				otsrivit = false;
				tulprm->writehtml(L"<tr><th>Sija</th><th>No-Os</th><th>Joukkue</th><th>Nimi</th><th>OsTls</th>");
				line[0] = 0;
				for (int i = 0; i < rt->rastiluku; i++)
					swprintf(line+wcslen(line), L"<th>%d. <em>(%d)</em></th>", i+1, rt->rastikoodi[i]);
				wcscat(line, L"</tr>\n");
//				wcscat(line, L"<th>Maali</th></tr>\n");
				tulprm->writehtml(line);
				if (tulprm->rastivali) {
					tulprm->writehtml(L"<tr><th></th><th></th><th>Rastivälivertailu</th><th><em>Lkm</em> ja paras aika</th><th></th>");
					line[0] = 0;
					for (int i = 0; i < emvaluku; i++) {
						int n;
						aikatowstr_cols_n(as, SEK*rvalit[eva[i].rvno].ajat[srj][yos][0], 0, 0, 8);
						if (tulprm->monios) {
							n = 0;
							for (int oo = 0; oo < Sarjat[srj].ntosuus; oo++)
								n += rvalit[eva[i].rvno].lkm[srj][oo];
							}
						else
							n = rvalit[eva[i].rvno].lkm[srj][yos];
						elimwzb1(as);
						while (wcslen(as) < 6) {
							wmemmove(as+1, as, wcslen(as)+1);
							as[0] = L' ';
							}
						swprintf(line+wcslen(line), L"<th><em>%d</em> %s</th>", n, as);
						}
					wcscat(line, L"</tr>\n");
					tulprm->writehtml(line);
					}
				}
			if ((tulprm->rivi + tulprm->tulmuot.riviryhma) % (2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
				wcscpy(st3, L"<tr class='aa'>");
			else
				wcscpy(st3, L"<tr class='ab'>");

			aikatowstr_cols_n(as, kilp.osTulos(osuus, 0), 0, 0, 8);
			elimwzb1(as);
			swprintf(line, L"%s<td>%d.</td><td>%s</td><td>%s</td><td>%s</td><td>%s</td>",
				st3, sj, st2, kilp.Joukkue(st, 30), kilp.Nimi(st1, 30, osuus, -1), as);

			for (i = 0; i < emvaluku; i++) {
				int sj1 = 0;
				if (tulprm->rastivali) {
					aikatowstr_cols_n(as, SEK*eva[i].rvaika, 0, 0, 8);
					sj1 = bSrchInt(eva[i].rvaika, rvalit[eva[i].rvno].ajat[srj][yos],
						rvalit[eva[i].rvno].lkm[srj][yos]) + 1;
					}
				else {
					aikatowstr_cols_n(as, SEK*eva[i].aika, 0, 0, 8);
					elimwzb1(as);
					sj1 = rstva[i].sj;
					}
				elimwzb1(as);
				while (wcslen(as) < (tulprm->rastivali ? 5 : 6)) {
					wmemmove(as+1, as, wcslen(as)+1);
					as[0] = L' ';
					}
				swprintf(line+wcslen(line), L"<td><em>%d.</em> %s</td>", sj1, as);
				}
			wcscat(line, L"</tr>\n");
			tulprm->writehtml(line);
			}
		}
#endif
}

