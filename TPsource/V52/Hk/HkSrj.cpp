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
#ifndef __linux__
#include <io.h>
#include <process.h>
#include <direct.h>
#include <dos.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#ifdef _CONSOLE
//#include <bvideo.h>
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include "HkDeclare.h"

extern INT tarkkuusparam;
extern int valuku_vanha;

void alustaSarjatiedot(void)
{
	for (int i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
		pvparam[i_pv].valuku = 0;
		for (int srj = 0; srj < sarjaluku; srj++) {
			if (i_pv < Sarjat[srj].enspv || i_pv > Sarjat[srj].viimpv)
				Sarjat[srj].luonne[i_pv] = -1;
			if (Sarjat[srj].luonne[i_pv] >= 0 &&
				pvparam[i_pv].valuku < Sarjat[srj].valuku[i_pv])
				pvparam[i_pv].valuku = Sarjat[srj].valuku[i_pv];
			Sarjat[srj].asuunnsakot[i_pv][2] = 60;
			Sarjat[srj].asuunnsakot[i_pv][4] = 60;
			}
		}
	kilpparam.sakkoyks = Sarjat[0].tsak[k_pv];
}

int luesarjat(void)
	{
	TextFl *sarjaf;
	INT   i = -1, j, er = 0, i_pv = 0, epv, vpv, nva[N_PV], m1, makipv = -1;
	int laika = 8, laika2 = 8;
	int srjrno;
	wchar_t s[300], *p, *p1, prs[100];
	bool vanhat = false;

	if (!lueEventXml(sarjaxmlfname, true, false, false)) {
		alustaSarjatiedot();
		return(0);
		}
	sarjaf = new TextFl(sarjafname,L"rt");
	if (!sarjaf->IsOpen()) {
		wcscpy(prs, L"Tiedosto ");
		writeerror_w(wcscat(wcscat(prs, sarjaxmlfname), L" puuttuu"),0);
		scpclr();
		delete sarjaf;
		return(1);
		}
	memset(nva, 0, sizeof(nva));
	while (1) {
		if (i >= sarjaluku) {
			 break;
		}
		if (sarjaf->ReadLine(s, 102) == NULL) {
			er = 1;
			break;
			}
		s[80] = 0;
		wcscpy(kilpailu, s);
		wtrimline(kilpailu);
		if (kilpailu[wcslen(kilpailu)-1] == L'*')
        	kilpailu[wcslen(kilpailu)-1] = 0;
		if (sarjaf->ReadLine(s, 78) == NULL) {
			er = 2;
			break;
			}
		if ((p = wcstok(s, L" \t\n")) == NULL) {
			er = 2;
			break;
			}
		kilpparam.vuosi = _wtoi(p);
		if ((p = wcstok(NULL, L" \t\n")) == NULL) {
			er = 2;
			break;
			}
		sarjaluku = _wtoi(p);
		if ((p = wcstok(NULL, L" \t\n")) != NULL) {
			if (*p >= L'0' && *p <= L'9')
				vanhat = true;
			}
		if (vanhat) {
			if (p) p = wcstok(NULL, L" \t\n");
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL) {
				kilpparam.kilplaji = *p;
				if (kilpparam.kilplaji == L'H')
					kilpparam.tarkkuus = 2;
				else
					kilpparam.tarkkuus = 0;
				}
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL) {
				kilpparam.n_pv_akt = _wtoi(p);
				if (kilpparam.n_pv < kilpparam.n_pv_akt)
					kilpparam.n_pv = kilpparam.n_pv_akt;
				}
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL) {
				valuku_vanha = _wtoi(p);
				if (valuku_vanha > kilpparam.valuku)
					kilpparam.valuku = valuku_vanha;
				}
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL)
				wcsncpy(kilpparam.kilpkoodi, p, sizeof(kilpparam.kilpkoodi)/2-1);
		}
		else {
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL)
				kilpparam.n_pv = _wtoi(p);
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL)
				kilpparam.valuku = _wtoi(p);
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL)
				wcsncpy(kilpparam.kilpkoodi, p, sizeof(kilpparam.kilpkoodi)/2-1);
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL) {
				kilpparam.kilplaji = *p;
				if (p[1])
					kilpparam.alalaji = *p;
				}
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL)
				kilpparam.n_pv_akt = _wtoi(p);
			if (p && (p = wcstok(NULL, L" \t\n")) != NULL && !tarkkuusparam)
				kilpparam.tarkkuus = _wtoi(p);
			}
		if (wcslen(kilpparam.kilpkoodi) == 0) {
			wcscpy(kilpparam.kilpkoodi, L"KOODI");
			}
		if (kilpparam.n_pv > N_PV || kilpparam.valuku > VALUKU) {
			writeerror_w(L"Ohjelmaversio ei tue sarjatiedon mukaista p‰ivien tai v‰liaikojan m‰‰r‰‰", 0);
			return(0);
			}
		switch (kilpparam.tarkkuus) {
			case 0:
				laika = 8;
				laika2 = 8;
				break;
			case 1 :
				laika = 10;
				laika2 = 8;
				break;
			case 2 :
				laika = 10;
				laika2 = 10;
				break;
			case 3 :
				laika = 11;
				laika2 = 10;
				break;
			case 4 :
				laika = 11;
				laika2 = 11;
				break;
			case 5 :
				laika = 11;
				laika2 = 8;
				break;
			}
		for (i = 0; i < kilpparam.n_pv; i++) {
			pvparam[i].laika = laika;
			pvparam[i].laikasj = laika;
			pvparam[i].laika2 = laika2;
			pvparam[i].laikava = laika2;
			}
		for (i = 0 ; i < sarjaluku && er == 0 ; i++) {
			if (sarjaf->ReadLine(s, 100) == NULL) {
				er = 3 + (N_PV+2) * i;
				break;
				}
			wtrimline(s);

			wcstok(s, L"*");
			if (wcslen(s) > 10) {
				er = 3 + (N_PV+2) * i;
				break;
				}
			for (p = s; *p; p++)
				if (*p == L'_')
			*p = L' ';
			wcsncpy(Sarjat[i].sarjanimi, s, LSARJA);
			srjrno = kilpparam.n_pv_akt+2+(vanhat ? valuku_vanha : kilpparam.valuku);

			if ((p = wcstok(NULL, L"\n")) != NULL && wcslen(p) > 2)
				wcsncpy(Sarjat[i].psarjanimi, p, sizeof(Sarjat[0].psarjanimi)/2-1);
			if (sarjaf->ReadLine(s, 100) == NULL) {
				er = 4 + srjrno * i;
				break;
				}
			p = wcstok(s, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			epv = _wtoi(p);
			p = wcstok(NULL, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			vpv = _wtoi(p);
			p = wcstok(NULL, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			Sarjat[i].rrtas = _wtoi(p);
			p = wcstok(NULL, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			if (!vanhat) {
				Sarjat[i].tuljarj = _wtoi(p);
				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 4 + srjrno * i;
					break;
					}
				}
			Sarjat[i].sarjaalku = _wtoi(p);
			p = wcstok(NULL, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			Sarjat[i].maksu = _wtoi(p);
			if (Sarjat[i].maksu < 200)
				Sarjat[i].maksu *= 100;

			p = wcstok(NULL, L" \n");
			if (!p) {
				er = 4 + srjrno * i;
				break;
				}
			m1 = _wtoi(p);
			if (kilpparam.n_pv_akt == 2) {
				Sarjat[i].maksu1 = m1;
				}
			if (vanhat) {
				if (kilpparam.n_pv_akt > 2) {
					Sarjat[i].maksu1 = (int) (m1%1000);
					if (m1 > 1000)
						Sarjat[i].maksual = (int) (m1/1000);
					else
						Sarjat[i].maksual = Sarjat[i].maksu;
					}
				}
			else {
				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 4 + srjrno * i;
					break;
					}
				Sarjat[i].maksual = _wtoi(p);
				if (Sarjat[i].maksual < 200)
					Sarjat[i].maksual *= 100;
				}
			if (vanhat && Sarjat[i].maksu1 < 200)
				Sarjat[i].maksu1 *= 100;

			if (vanhat) {
				for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
					p = wcstok(NULL, L" \n");
					if (!p) {
						break;
						}
					Sarjat[i].tul_raja[i_pv] = _wtoi(p);
					}
				}
			if (epv < 1)
			   epv = 1;
			if (vpv > kilpparam.n_pv_akt)
			   vpv = kilpparam.n_pv_akt;
			if (vpv < epv)
				vpv = epv;
			if (kilpparam.n_pv_akt > 1) {
				Sarjat[i].enspv = epv - 1;
				Sarjat[i].viimpv = vpv - 1;
				}
			else {
				Sarjat[i].enspv = 0;
				Sarjat[i].viimpv = 0;
				}

			if (Sarjat[i].maksu1 == 0)
				Sarjat[i].maksu1 = Sarjat[i].maksu / (Sarjat[i].viimpv - Sarjat[i].enspv + 1);
			if (Sarjat[i].maksual == 0)
				Sarjat[i].maksual = Sarjat[i].maksu;

			for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
				if (sarjaf->ReadLine(s, 78) == NULL) {
					er = 5 + srjrno * i;
					break;
					}
				p = wcstok(s, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
#ifdef MAKIxx
				if ((m1 = _wtoi(p)) < 0) {
					makipv = i_pv;
					*p = L'+';
					}
				else
#endif
				Sarjat[i].lno[i_pv] = _wtoi(p);

				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
#ifdef MAKIxx
				if (i_pv == makipv)
					nkierros[i] = _wtoi(p);
				else
#endif
				Sarjat[i].mno[i_pv] = _wtoi(p);

				if (!vanhat) {
					p = wcstok(NULL, L" \n");
					if (!p) {
						er = 5 + srjrno * i;
						break;
						}
					Sarjat[i].bibalku[i_pv] = _wtoi(p);
					}
				else
					Sarjat[i].bibalku[i_pv] = Sarjat[i].sarjaalku;

				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
				if (i_pv == k_pv && yhtlahto)
					Sarjat[i].enslahto[i_pv] = TMAALI0;
				else
					Sarjat[i].enslahto[i_pv] = _wtol(p);
				p = wcstok(NULL, L" \n");
				Sarjat[i].pari[i_pv] = 1;
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
				if (vanhat && kilpparam.kilplaji == L'H') {
					Sarjat[i].pari[i_pv] = _wtoi(p);
					}
				else
					Sarjat[i].ensjlahto[i_pv] = _wtol(p);
				if (!vanhat) {
					p = wcstok(NULL, L" \n");
					if (!p) {
						er = 5 + srjrno * i;
						break;
						}
					Sarjat[i].pari[i_pv] = _wtoi(p);
					}
				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
				Sarjat[i].lahtovali[i_pv] = _wtoi(p);
				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
				Sarjat[i].lsak[i_pv] = _wtoi(p);
				if (Sarjat[i].lsak[i_pv] > LSAKKO)
					Sarjat[i].lsak[i_pv] = LSAKKO;
				if (i_pv == k_pv &&
					Sarjat[i].lsak[i_pv] > kilpparam.lsakmax) {
					kilpparam.lsakmax = Sarjat[i].lsak[i_pv];
					}
				p = wcstok(NULL, L" \n");
				if (!p) {
					er = 5 + srjrno * i;
					break;
					}
				Sarjat[i].tsak[i_pv] = _wtoi(p);
				if (!vanhat) {
					p = wcstok(NULL, L" \n");
					if (!p) {
						er = 5 + srjrno * i;
						break;
						}
					Sarjat[i].npist[i_pv] = _wtoi(p);
					p = wcstok(NULL, L" \n");
					if (!p) {
						er = 5 + srjrno * i;
						break;
						}
					Sarjat[i].tul_raja[i_pv] = _wtoi(p);
					}
				p = wcstok(NULL, L" \n");
				if (p) {
					if (p[wcslen(p)-1] == L'*')
						p[wcslen(p)-1] = 0;
					wcscpy(Sarjat[i].matka[i_pv], p);
					}
				}
			if (er)
				break;
			memset(nva, 0, sizeof(nva));
#ifdef MAKIxx
			if (k_pv != makipv) {
#else
				{
#endif
				for (j = 0; j < (vanhat ? valuku_vanha : kilpparam.valuku); j++) {
					if (sarjaf->ReadLine(s, 78) == NULL) {
						er = 6 + srjrno * i;
						break;
						}
					for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
						if (!i_pv) {
							p = wcstok(s, L" \t\n");
							}
						else
							p = wcstok(NULL, L" \t\n");
						if (!p) {
							if (!i_pv)
								er = 6 + srjrno * i;
							break;
							}
						p1 = wcstok(NULL, L" \t\n");
						if (p[wcslen(p)-1] == L'*')
							p[wcslen(p)-1] = 0;
						if (*p) {
							wcsncpy(Sarjat[i].va_matka[i_pv][j], p, 5);
							nva[i_pv] = j+1;
							}
						if (*p1)
							Sarjat[i].va_raja[i_pv][j] = _wtoi(p1);
						}
					if (er) {
						if (i_pv || er == -1)
							er = 0;
						break;
						}
					}
				}
			for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
				Sarjat[i].valuku[i_pv] = nva[i_pv];
				}
			if (nva[k_pv] > kilpparam.maxvaluku)
				kilpparam.maxvaluku = nva[k_pv];
			if (er) break;
			}
		}
	if (!kilpparam.maxvaluku && !pvparam[k_pv].hiihtolahto) {
		vainoletus = 1;
		ol_piste = 0;
		}
	if (er != 0) {
		clrln(14);
		clrln(15);
		clrln(16);
		clrln(17);
		clrln(18);
		swprintf(prs,L"Tiedosto %s viallinen", sarjafname);
		vidspwmsg(15,0,7,0,prs);
		swprintf(prs,L"Virhe rivill‰ %d tai ennen sit‰", er);
		vidspwmsg(16,0,7,0,prs);
		vidspwmsg(17,0,7,0,
			L"Poista tai korjaa tiedosto ennen uutta k‰ynnistyst‰");
		writeerror_w(L"",0);
		}
	delete sarjaf;
	return(er);
	}

void luesarjayhdistelmat(void)
	{
	TextFl *yhdfile;
	wchar_t *yhdfilename = L"SARJAYHD.LST";
	wchar_t buf[500] = L"", snimi[LSARJA+1] = L"", *p, *p1, pnimi[LPSARJA+1];
	int j, n, k, lsnm;

	yhdfile = new TextFl(yhdfilename,L"rt");
	if (!yhdfile->IsOpen()) {
		delete yhdfile;
		return;
		}
	n = 0;
	while (!yhdfile->Feof()) {
		yhdfile->ReadLine(buf, sizeof(buf)/2-1);
		p = wcstok(buf, L" \t\n");
		if (p) {
			if (n >= MAXYHD) {
				swprintf(buf, L"Liikaa sarjayhdistelmi‰. Maksimim‰‰r‰ on %d.", MAXYHD);
				writeerror_w(buf, 0);
				}
//			wcsncpy(yhdnimet[n], p, LSARJA);
			memset(pnimi, 0, sizeof(pnimi));
			memset(snimi, 0, sizeof(snimi));
			lsnm = LSARJA;
			if ((p1 = wcsstr(p, L":")) != NULL) {
				wcsncpy(pnimi, p1+1, LPSARJA);
				if (p1 - p < lsnm)
					lsnm = p1 - p;
				for (p1 = pnimi; *p1; p1++)
					if (*p1 == L'_')
						*p1 = L' ';
				}
			wcsncpy(snimi, p, lsnm);
			upcasewstr(snimi);
			k = 0;
			while (p) {
				p = wcstok(NULL, L" \t\n");
				if (p) {
					upcasewstr(p);
					j = -1;
					if (wcscmp(p, L"KAIKKI") == 0 || (j = haesarja_w(p, false)) >= 0) {
						int esrj, vsrj;
						if (j < 0) {
							esrj = 0;
							vsrj = sarjaluku - 1;
							}
						else {
							esrj = j;
							vsrj = j;
							}
						for (j = esrj; j <= vsrj; j++) {
							yhdsarjat[n][j] = 1;
							if (k == 0)
								memcpy(&Sarjat[sarjaluku+n], &Sarjat[j], sizeof(Sarjat[j]));
							else {
								if (Sarjat[sarjaluku+n].valuku[k_pv] < Sarjat[j].valuku[k_pv]) {
									Sarjat[sarjaluku+n].valuku[k_pv] = Sarjat[j].valuku[k_pv];
									memcpy(Sarjat[sarjaluku+n].va_matka, Sarjat[j].va_matka, sizeof(Sarjat[sarjaluku+n].va_matka));
									}
								if (NORMKELLO(Sarjat[sarjaluku+n].enslahto[k_pv] - Sarjat[j].enslahto[k_pv]) > 0)
									Sarjat[sarjaluku+n].enslahto[k_pv] = Sarjat[j].enslahto[k_pv];
								}
							k++;
                            }
						}
					}
				}
			if (k > 0) {
				wcsncpy(Sarjat[sarjaluku+n].sarjanimi, snimi, LSARJA);
				wmemcpy(Sarjat[sarjaluku+n].psarjanimi, pnimi, LPSARJA+1);
				n++;
				}
			}
		}
	nsarjayhd = n;
	if (n > 0) {
		swprintf(buf, L"Tiedostosta %s luettu %d sarjayhdistelm‰‰", yhdfilename, n);
		vidspwmsg(1,5,7,0,buf);
		}
	delete yhdfile;
	}

void kirjsarjat(void)
	{
	TextFl *sarjaf;
	INT   i, j, er, pv, epv, vpv;
	INT32  m1 = 0;
	wchar_t st[12], *p, Buf[200];

	if (wcslen(kilpparam.kilpkoodi) == 0) {
		wcscpy(kilpparam.kilpkoodi, L"KOODI");
		}
	sarjaf = new TextFl(sarjafname,L"wt");
	kilpparam.tarkkuus = (pvparam[k_pv].laika+pvparam[k_pv].laika2)/2-8 + (pvparam[k_pv].laika > 10);
	if (pvparam[k_pv].laika-pvparam[k_pv].laika2 == 3)
	   kilpparam.tarkkuus = 5;
	if (sarjaf->IsOpen()) {
		er = swprintf(Buf, L"%s*\n%5d %5d %s %2d %2d %s %c%c %2d %d\n", kilpailu,
		kilpparam.vuosi, sarjaluku, kilpparam.tiedlaji, kilpparam.n_pv,
			kilpparam.valuku, kilpparam.kilpkoodi, kilpparam.kilplaji, kilpparam.alalaji,
			kilpparam.n_pv_akt, kilpparam.tarkkuus);
		sarjaf->WriteLine(Buf);
		for ( i = 0 ; i < sarjaluku; i++) {
			epv = Sarjat[i].enspv+1;
			vpv = Sarjat[i].viimpv+1;
			m1 = Sarjat[i].maksu1;
			if (kilpparam.n_pv_akt > 1) {
				if (Sarjat[i].maksual != Sarjat[i].maksu)
					m1 += 1000L*Sarjat[i].maksual;
				}
			wcscpy(st, Sarjat[i].sarjanimi);
			for (p = st; *p; p++)
				if (*p == L' ')
					*p = L'_';
			swprintf(Buf,L"%s*%s\n", st, Sarjat[i].psarjanimi);
			sarjaf->WriteLine(Buf);
			swprintf(Buf,L"%3d %3d %3d %3d %5d %6d %6d %6d", epv, vpv,
				Sarjat[i].rrtas, Sarjat[i].tuljarj, Sarjat[i].sarjaalku, Sarjat[i].maksu, Sarjat[i].maksu1, Sarjat[i].maksual);
			sarjaf->WriteLine(Buf);
			sarjaf->WriteChar(L'\n');
			for (pv = 0; pv < kilpparam.n_pv_akt; pv++) {
				swprintf(Buf, L"%3d %3d %6d %6ld %6ld % 2d %6d %3d %4d %3d %6d %s*\n",
					Sarjat[i].lno[pv], Sarjat[i].mno[pv], Sarjat[i].bibalku[pv], Sarjat[i].enslahto[pv], Sarjat[i].ensjlahto[pv], Sarjat[i].pari[pv],
					Sarjat[i].lahtovali[pv], Sarjat[i].lsak[pv], Sarjat[i].tsak[pv], Sarjat[i].npist[pv], Sarjat[i].tul_raja[pv], Sarjat[i].matka[pv]);
				sarjaf->WriteLine(Buf);
				}
			for (j = 0; j < kilpparam.valuku; j++) {
				for (pv = 0; pv < kilpparam.n_pv_akt; pv++) {
					swprintf(Buf, L"%s* %d", Sarjat[i].va_matka[pv][j],
						Sarjat[i].va_raja[pv][j]);
					sarjaf->WriteLine(Buf);
					if (pv == kilpparam.n_pv_akt - 1) {
						sarjaf->WriteChar(L'\n');
						}
					else {
						sarjaf->WriteChar(L' ');
						}
					}
				}
			}
		if (er == EOF) iowerr(sarjafname, 2, 0L);
		}
	else
		writeerror_w(
			L"Sarjatietotiedoston avaus kirjoitusta varten ei onnistu", 0);
	delete sarjaf;
	}

__fastcall sarjatietue::sarjatietue(void)
{
	nollaa();
}

__fastcall sarjatietue::~sarjatietue(void)
{

}

void sarjatietue::nollaa(void)
{
	memset(sarjanimi, 0, sizeof(sarjatietue));
	viimpv = kilpparam.n_pv_akt-1;
	for (int i = 0; i < N_PV; i++) {
		lno[i] = 1;
		mno[i] = 1;
		pari[i] = 1;
		sarjaalku = 1;
		bibalku[i] = 1;
		lahtovali[i] = 120;
		tlskerroin[i] = 1;
		tlsjakaja[i] = 1;
		if (i > 0)
			luonne[i] = 1;
		if (kilpparam.kilplaji == L'B') {
			kilpparam.lsakmax = 5;
			lsak[i] = 5;
			wcsncpy(asuunnlaji[i], kilpparam.asuunnlaji, sizeof(asuunnlaji[i])/2-1);
			for (UINT j = 0; j < wcslen(asuunnlaji[i]); j++) {
				switch (asuunnlaji[i][j]) {
					case L'S':
						asuunnsakot[i][j] = 120;
						break;
					case L'L':
						asuunnsakot[i][j] = 60;
						break;
					default:
						asuunnsakot[i][j] = 60;
						break;
					}
				}
			}
		for (int iva = 0; iva < VALUKU+1; iva++) {
			if (iva > 0 && iva < LSAKKO)
				va_sakot[i][iva] = iva;
			else
				va_sakot[i][iva] = LSAKKO;
			}
		}
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		AlustaKaavat(0, 0);
		}
}

void sarjatietue::operator=(sarjatietue& Sarja2)
{
	memcpy(sarjanimi, Sarja2.sarjanimi, sizeof(sarjatietue));
}

bool sarjatietue::operator==(sarjatietue& Sarja2)
{
	if (memcmp(sarjanimi, Sarja2.sarjanimi, sizeof(sarjatietue)))
		return(false);
	return(true);
}

void sarjatietue::tulkMTB(wchar_t *ln, int i_pv)
{
	wchar_t *p;

	p = wcstok(ln, L" ;,\t");
	if (!p)
		return;
	laskemtb[i_pv] = _wtoi(p) > 0;
	for (int i = 0; i <= valuku[i_pv]; i++) {
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[i_pv][i].alkukdi = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[i_pv][i].loppukdi = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[i_pv][i].piste = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[i_pv][i].pohja = _wtoi(p);
		}
}

void sarjatietue::AlustaKaavat(wchar_t orig_laji, wchar_t *orig_alalajit)
{
	int k;

	if (kilpparam.kilplaji == L'P') {
		int nsak[5] = {2, 1, 1, 2, 1};
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			if (kilpparam.kilplaji == orig_laji && orig_alalajit &&
				pvparam[i].alalaji == orig_alalajit[i])
				continue;
			k = wcswcind(pvparam[i].alalaji, L"HOWTC");
			kaava[i][VALUKU+1].ala = -1;
			kaava[i][VALUKU+1].yla = 0;
			kaava[i][VALUKU+1].kerroin = sotlajit[k].kerroin;
			kaava[i][VALUKU+1].jakaja = sotlajit[k].jakaja;
			kaava[i][VALUKU+1].nollataso = sotlajit[k].vertaika;
			kaava[i][VALUKU+1].on = 1;
			tsak[i] = 1;
			lsak[i] = nsak[k];
			if (luonne[i] >= 0) {
				pistekaava[i].yhtpiston = 7;
				pistekaava[i].yhtpistvaihelkm = i - enspv + 1;
				pistekaava[i].yhtpisthyvmin = i - enspv + 1;
				pistekaava[i].yhtpistosottomin = i - enspv + 1;
				}
			}
		}
	if (kilpparam.kilplaji == L'N') {
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			if (kilpparam.kilplaji == orig_laji && orig_alalajit &&
				pvparam[i].alalaji == orig_alalajit[i])
				continue;
			k = wcswcind(pvparam[i].alalaji, L"OLUFC");
			kaava[i][VALUKU+1].ala = -1;
			kaava[i][VALUKU+1].yla = 0;
			kaava[i][VALUKU+1].kerroin = sotlajit[k+5].kerroin;
			kaava[i][VALUKU+1].jakaja = sotlajit[k+5].jakaja;
			kaava[i][VALUKU+1].nollataso = sotlajit[k+5].vertaika;
			kaava[i][VALUKU+1].on = (viimpv - enspv > 2);
			tsak[i] = 1;
			lsak[i] = 1;
			if (luonne[i] >= 0) {
				pistekaava[i].yhtpiston = 7;
				pistekaava[i].yhtpistvaihelkm = i - enspv + 1;
				pistekaava[i].yhtpisthyvmin = i - enspv + 1;
				pistekaava[i].yhtpistosottomin = i - enspv + 1;
				}
			}
		}
}
//---------------------------------------------------------------------------

void sarjatietue::tulkFormula(wchar_t *ln, int i_pv, int va)
{
	int iv, vd, sp, npar;

	kaava[i_pv][va].nollataso = 0;
	swscanf(ln,L"%d;", &iv);
	if (iv <= VALUKU || iv > VALUKU+2)
		iv = VALUKU+1;
	npar = swscanf(ln,L"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d", &iv, &kaava[i_pv][va].ala, &kaava[i_pv][va].yla,
		&vd, &kaava[i_pv][va].kerroin, &kaava[i_pv][va].jakaja,
		&kaava[i_pv][va].minimi, &kaava[i_pv][va].maksimi, &kaava[i_pv][va].nollataso, &sp);
	kaava[i_pv][va].vaadiala = (vd != 0);
	kaava[i_pv][va].on = true;
	if (npar > 9)
		kaava[i_pv][va].sakotpois = (sp != 0);
	else
		kaava[i_pv][va].sakotpois = false;
}

wchar_t *sarjatietue::writeFormula(wchar_t *ln, int i_pv, int va)
{
	swprintf(ln, L"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d", va, kaava[i_pv][va].ala, kaava[i_pv][va].yla,
		kaava[i_pv][va].vaadiala ? 1 : 0, kaava[i_pv][va].kerroin, kaava[i_pv][va].jakaja,
		kaava[i_pv][va].minimi, kaava[i_pv][va].maksimi, kaava[i_pv][va].nollataso,
		kaava[i_pv][va].sakotpois ? 1 : 0);
	return(ln);
}

void sarjatietue::tulkPointFormula(wchar_t *ln, int i_pv, int iRule)
{
	if (iRule >= 0) {
		swscanf(ln, L"%d;%d;%d;%d;%d", &pistekaava[i_pv].a[iRule], &pistekaava[i_pv].y[iRule],
			&pistekaava[i_pv].p[iRule], &pistekaava[i_pv].d[iRule], &pistekaava[i_pv].vaiheon);
		if (pistekaava[i_pv].p[iRule])
			pistekaava[i_pv].laji |= 1;
		}
	if (iRule == -1) {
		swscanf(ln, L"%d;%d;%lf;%d;%d;%d;%d", &pistekaava[i_pv].plaji, &pistekaava[i_pv].vaika,
			&pistekaava[i_pv].vpist, &pistekaava[i_pv].kerr, &pistekaava[i_pv].jak,
			&pistekaava[i_pv].optiot, &pistekaava[i_pv].vaiheon);
		if (pistekaava[i_pv].plaji > 0) {
			pistekaava[i_pv].laji |= 2;
			}
		}
	if (iRule == -2) {
		swscanf(ln, L"%d;%d;%d;%d", &pistekaava[i_pv].yhtpistvaihelkm,
			&pistekaava[i_pv].yhtpistosottomin, &pistekaava[i_pv].yhtpisthyvmin, &pistekaava[i_pv].yhtpiston);
		}
	if (i_pv == k_pv && (pistekaava[i_pv].yhtpiston & 2) == 2)
		kilpparam.poissamukaan |= pistekaava[i_pv].yhtpiston / 2;
}

wchar_t *sarjatietue::writePointFormula(wchar_t *ln, int i_pv, int iRule)
{
	if (iRule >= 0) {
		swprintf(ln, L"%d;%d;%d;%d;%d", pistekaava[i_pv].a[iRule], pistekaava[i_pv].y[iRule],
			 pistekaava[i_pv].p[iRule], pistekaava[i_pv].d[iRule], pistekaava[i_pv].vaiheon);
		}
	if (iRule == -1) {
		swprintf(ln, L"%d;%d;%.3lf;%d;%d;%d;%d", pistekaava[i_pv].plaji, pistekaava[i_pv].vaika,
			pistekaava[i_pv].vpist, pistekaava[i_pv].kerr, pistekaava[i_pv].jak,
			pistekaava[i_pv].optiot, pistekaava[i_pv].vaiheon);
		}
	if (iRule == -2) {
		swprintf(ln, L"%d;%d;%d;%d", pistekaava[i_pv].yhtpistvaihelkm,
			pistekaava[i_pv].yhtpistosottomin, pistekaava[i_pv].yhtpisthyvmin, pistekaava[i_pv].yhtpiston);
		}
	return(ln);
}

bool sarjatietue::YhtpistOn(void)
{
	return(pistekaava[k_pv].yhtpiston & 1);
}

bool sarjatietue::yhtpistkaikille(void)
{
	return((pistekaava[k_pv].yhtpiston & 3) == 3);
}

bool sarjatietue::pistetulos(void)
{
	return(0);
}

bool sarjatietue::pisteytulos(void)
{
	return((pistekaava[k_pv].yhtpiston & 4) == 4);
}

bool sarjatietue::nouspisteet(void)
{
	return(wcswcind(kilpparam.kilplaji, L"NP") >= 0 || (pistekaava[k_pv].laji == 2 && pistekaava[k_pv].kerr * pistekaava[k_pv].jak < 0) ||
		(pistekaava[k_pv].laji != 2 && pistekaava[k_pv].d[0] > 0));
}


void vaihdakilpsrj(int ala, int yla, int muutos, bool indeksoi)
   {
   int d;
   kilptietue kilp;

   if (nrec <= 1) return;
   for (d = 1; d < nrec; d++) {
	  kilp.GETREC(d);
	  if (kilp.kilpstatus == 0) {
		  if (kilp.Sarja() >= ala && kilp.Sarja() <= yla) {
			  kilp.setSarjaAll(kilp.Sarja() + muutos);
			  kilp.PUTREC(d, 1);
			  }
		  }
	  }
   if (indeksoi)
	  uusijarj();
   }

sprintsarja::sprintsarja(void)
{
	nollaa();
}
//---------------------------------------------------------------------------
sprintsarja::~sprintsarja(void)
{
}
//---------------------------------------------------------------------------
void sprintsarja::nollaa(void)
{
	memset(this, 0, sizeof(sprintsarja));
}
//---------------------------------------------------------------------------
void sprintsarja::operator=(sprintsarja &ss)
{
	memcpy(this, &ss, sizeof(sprintsarja));
}
//---------------------------------------------------------------------------
bool sprintsarja::operator==(sprintsarja &ss)
{
	return(memcmp(this, &ss, sizeof(sprintsarja)) == 0);
}
//---------------------------------------------------------------------------


