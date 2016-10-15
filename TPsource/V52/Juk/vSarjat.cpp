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
#ifdef _CONSOLE
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include <direct.h>
#include <dos.h>
//#include <size.h>
#include <sys/stat.h>
#include <tptype.h>
#include "wincom.h"
#include "VDeclare.h"

sarjatietue::sarjatietue(void)
{
	nollaa();
}

sarjatietue::~sarjatietue(void)
{

}

void sarjatietue::nollaa(void)
{
	memset(sarjanimi, 0, sizeof(sarjatietue));
	lno = 1;
	mno = 1;
	sarjaalku = 1;
	osuusluku = 4;
	ntosuus = 4;
	maxnosuus = 1;
	tuljarj = 1;
	aosuus[0] = -1;
	for (int i = 0; i < kilpparam.n_os; i++) {
		nosuus[i] = 1;
		aosuus[i+1] = i;
		yosuus[i] = i;
		ylahto[i] = TMAALI0;
		ylkynnys[i] = TMAALI0;
		valuku[i] = kilpparam.maxvaluku;
		tsak[i] = kilpparam.sakkoyks;
		amptsak[i] = 0;
		lahtovali[i] = 0;
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

int sarjatietue::aOsuus(int yos, int aos)
{
	return(aosuus[yos]+aos+1);
}

wchar_t *sarjatietue::Sarjanimi(wchar_t *st /* =NULL */, bool pitka /* = false */)
{
	static wchar_t buf[LPSARJA+1];

	if (st == NULL)
		st = buf;
	if (pitka && psarjanimi[0])
		ansitowcs(st, psarjanimi, LPSARJA);
	else
		ansitowcs(st, sarjanimi, LSARJA);
	return(st);
}

wchar_t *sarjatietue::pSarjanimi(wchar_t *st /* =NULL */)
{
	static wchar_t buf[LPSARJA+1];

	if (st == NULL)
		st = buf;
	if (psarjanimi[0] == 0)
		return(Sarjanimi(st));
	ansitowcs(st, psarjanimi, LPSARJA);
	return(st);
}

wchar_t *sarjatietue::Matka(int osuus, wchar_t *st /*=NULL*/ )
{
	static wchar_t buf[40];

	if (st == NULL)
		st = buf;
	MbsToWcs(st, matka[osuus], 38);
	return(st);
}


wchar_t *sarjatietue::vaMatka(int osuus, int va, wchar_t *st /*=NULL*/ )
{
	static wchar_t buf[40];

	if (st == NULL)
		st = buf;
	MbsToWcs(st, va_matka[osuus][va], 38);
	return(st);
}

int sarjatietue::Vaikaluku(void)
{
	int valkm = 0;

	for (int os = 0; os < osuusluku; os++)
		if (valkm < valuku[os])
			valkm = valuku[os];
	return(valkm);
}

static void trimline_ast(char *line)
{
  char *p;

  for (p = line+strlen(line)-1; p >= line && *p == '\n' || *p == '\r' || *p == '*'; p--)
	*p = 0;
}

void hae_enslahto(void)
{
	for (int i = 0; i < sarjaluku; i++) {
		if (nilm[i]) {
			if (enslahto == TMAALI0)
				enslahto = Sarjat[i].lahto;
			else {
				if (NORMT(Sarjat[i].lahto - enslahto) > 12L * TUNTI)
					enslahto = Sarjat[i].lahto;
			}
		}
	}
	if (kilpparam.alalaji != L'D')
		enslahto = NORMKELLO(enslahto - TUNTI);
}

int maaraaOsuusluku(bool kysy)
{
	int er = 0;

	kilpparam.osuusluku = 1;
	kilpparam.ntosuus = 1;
	kilpparam.maxvaluku = 0;
	kilpparam.maxnosuus = 1;
	for (int srj = 0; srj < sarjaluku; srj++) {
		memset(Sarjat[srj].aosuus, 0, sizeof(Sarjat[srj].aosuus));
		memset(Sarjat[srj].yosuus, 0, sizeof(Sarjat[srj].yosuus));
		Sarjat[srj].osuusluku = 0;
		Sarjat[srj].aosuus[0] = -1;
		Sarjat[srj].maxnosuus = 1;
		for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
			Sarjat[srj].aosuus[os+1] = Sarjat[srj].aosuus[os] + Sarjat[srj].nosuus[os];
			if (Sarjat[srj].osuusluku + Sarjat[srj].nosuus[os] > kilpparam.n_os) {
				Sarjat[srj].ntosuus = os;
				break;
				}
			Sarjat[srj].osuusluku += Sarjat[srj].nosuus[os];
			if (Sarjat[srj].maxnosuus < Sarjat[srj].nosuus[os])
				Sarjat[srj].maxnosuus = Sarjat[srj].nosuus[os];
			for (int yos = Sarjat[srj].aosuus[os]+1; yos <= Sarjat[srj].aosuus[os+1]; yos++)
				Sarjat[srj].yosuus[yos] = os;
			if (Sarjat[srj].paikat[os] > Sarjat[srj].maxpaikat)
				Sarjat[srj].maxpaikat = Sarjat[srj].paikat[os];
			if (Sarjat[srj].paikat[os] > kilpparam.lsakmax)
				kilpparam.lsakmax = Sarjat[srj].paikat[os];
			}
		if (kilpparam.ntosuus < Sarjat[srj].ntosuus) {
			kilpparam.ntosuus = Sarjat[srj].ntosuus;
			}
		if (kilpparam.osuusluku < Sarjat[srj].osuusluku) {
			kilpparam.osuusluku = Sarjat[srj].osuusluku;
			}
		if (kilpparam.maxnosuus < Sarjat[srj].maxnosuus) {
			kilpparam.maxnosuus = Sarjat[srj].maxnosuus;
			}
		if (kilpparam.maxvaluku < Sarjat[srj].Vaikaluku()) {
			kilpparam.maxvaluku = Sarjat[srj].Vaikaluku();
			}
		if (Sarjat[srj].maxpaikat && (Sarjat[srj].taslaji % 10) == 0)
			Sarjat[srj].taslaji += 1;
		if ((Sarjat[srj].taslaji / 10) > 0)
			kilpparam.aikasakot = 1;
		}
	if (!kysy) {
		if (er) {
			return(2);
			}
		if (kilpparam.n_os_akt < kilpparam.osuusluku) {
			return(1);
			}
		}
	else
		{
		if (er) {
			wchar_t ch = L' ';
			wselectopt(L"Osuuslukum‰‰riss‰ ristiriita. Yritet‰‰nkˆ lukea tiedot? (K/E)", L"KE", &ch);
			if (ch == L'E')
				lopetus();
			}
		if (kilpparam.n_os_akt < kilpparam.osuusluku) {
			kilpparam.n_os_akt = kilpparam.osuusluku;
			writeerror_w(L"Korjattu kilpailun ja sarjatietojen osuusluvuissa ristiriita. Tarkasta tiedot.", 0);
			}
		}
	return(0);
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
					if (wcscmp(p, L"KAIKKI") == 0 || (j = haesarja_w(p, false)) >= 0) {
						yhdsarjat[n][j] = 1;
						if (k == 0)
							memcpy(&Sarjat[sarjaluku+n], &Sarjat[j], sizeof(Sarjat[j]));
						k++;
						}
					}
				}
			if (k > 0) {
				wcstoansi(Sarjat[sarjaluku+n].sarjanimi, snimi, LSARJA);
				wcstoansi(Sarjat[sarjaluku+n].psarjanimi, pnimi, LPSARJA+1);
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

int luesarjat(void)
	{
	FILE *sarjaf;
	int   i,j,er,os, maxosuusluku, rr;
	TMAALI yl, yl2 = 0, ylk = 0;
	char s[82], *p, prs[100];
	int  va;
	int ok;

	if (!lueEventXml(sarjaxmlfname, true, false, false)) {
		maaraaOsuusluku(true);
		return(0);
		}

	sarjaf = fopen(sarjafname,"rt");
	ok = (sarjaf != NULL);
	if (ok) {
		for (rr = 0; ; rr++) {
			if (fgets(s, 82, sarjaf) == NULL)
				break;
			if (strlen(s) < 2)
				break;
			}
		rewind(sarjaf);
		er = 0;
		if (fgets(s, 82, sarjaf) == NULL) {
			er = 1;
			}
		else {
			trimline_ast(s);
			ansitowcs(kilpailu, s, sizeof(kilpailu)/2-1);
//			if (fscanf(sarjaf, "%d %d %d\n", &sarjaluku, &minkilpno, &maxkilpno) != 3) {
			if (fscanf(sarjaf, "%d %d %d\n", &sarjaluku, &i, &j) != 3) {
				er = 2;
				}
			else {
				maxosuusluku = (rr - 4) / sarjaluku;
				if (sarjaluku > MAXSARJALUKU) {
					sprintf(prs,"Suurin sallittu sarjaluku on %d",
						MAXSARJALUKU);
					writeerror(prs,0);
					return(1);
					}
				else if (maxosuusluku > MAXOSUUSLUKU) {
					sprintf(prs,"Suurin sallittu osuusluku on %d",
						MAXOSUUSLUKU);
					writeerror(prs,0);
					return(1);
					}
				else {
					for ( i = 0 ; i < sarjaluku && er == 0 ; i++) {
						yl = TMAALI0;
						yl2 = TMAALI0;
						ylk = TMAALI0;
						for (os = 0; os < maxosuusluku; os++) {
							Sarjat[i].ylahto[os] = TMAALI0;
							Sarjat[i].ylkynnys[os] = TMAALI0;
							}
						if (fgets(s, 80, sarjaf) == NULL) {
							er = 3 + (maxosuusluku+2) * i;
							}
						else {
							trimline_ast(s);
							strcpy(Sarjat[i].sarjanimi, s);
							if (fgets(s, 80, sarjaf) == NULL) {
								er = 4 + (maxosuusluku+2) * i;
								}
							else {
								int nfld;

								memset(Sarjat[i].paikat, '0', sizeof(Sarjat[i].paikat)-1);
								if ((nfld = sscanf(s, "%d %d %d %ld %d %ld %ld %ld %s",
									&Sarjat[i].osuusluku, &Sarjat[i].mno, &Sarjat[i].sarjaalku, &Sarjat[i].lahto,
									&Sarjat[i].maksu, &yl, &ylk, &yl2, Sarjat[i].paikat[0])) < 6) {
									er = 4 + (maxosuusluku+2) * i;
									}
								else {
									Sarjat[i].ntosuus = Sarjat[i].osuusluku;
									if (nfld > 6) {
										Sarjat[i].lahto *= KSEK;
										yl *= KSEK;
										ylk *= KSEK;
										if (nfld == 7)
											yl2 = yl;
										else
											yl2 *= KSEK;
										if (yl != TMAALI0 && yl2 == TMAALI0)
											yl2 = yl;
										if (Sarjat[i].osuusluku > maxosuusluku)
											maxosuusluku = Sarjat[i].osuusluku;
										if (Sarjat[i].ntosuus > Sarjat[i].osuusluku)
											Sarjat[i].ntosuus = Sarjat[i].osuusluku;
										}
									else {
										if ((nfld = sscanf(s, "%d %d %d %ld %d %s",
											&Sarjat[i].osuusluku, &Sarjat[i].mno, &Sarjat[i].sarjaalku, &Sarjat[i].lahto,
											&Sarjat[i].maksu, Sarjat[i].paikat[0])) < 6) {
											er = 4 + (maxosuusluku+2) * i;
											}
										}
									for (os=0; os<maxosuusluku; os++) {
										Sarjat[i].ylahto[os] = (os == Sarjat[i].osuusluku - 1) ? yl : yl2;
										Sarjat[i].ylkynnys[os] = ylk;
										if (os < Sarjat[i].osuusluku)
											Sarjat[i].paikat[os] -= '0';
										Sarjat[i].paikat[Sarjat[i].osuusluku] = 0;
										if (Sarjat[i].paikat[os] < 0 ||
											Sarjat[i].paikat[os] > LSAKOT)
											Sarjat[i].paikat[os] = 0;
										if (os < Sarjat[i].osuusluku && Sarjat[i].paikat[os] > Sarjat[i].maxpaikat)
											Sarjat[i].maxpaikat = Sarjat[i].paikat[os];
										if (fgets(s, 80, sarjaf) == NULL) {
											er = 5 + os + (maxosuusluku+2) * i;
											goto error;
											}
										if (os == 0 && *s != '*' && Sarjat[i].ylahto[0] >= 1 &&
											Sarjat[i].ylahto[0] < 10) {
											memmove(s+1, s, sizeof(s)-1);
											*s = Sarjat[i].ylahto[0] + '0';
											for (j = 0; j < maxosuusluku; j++)
												Sarjat[i].ylahto[j] = TMAALI0;
											}
										trimline_ast(s);
										p = strtok(s, " ");
										if (p) {
											strncpy((char *)Sarjat[i].matka[os], p, sizeof(Sarjat[i].matka[os])-1);
											for (va = 0; va < VAIKALUKU && p; va++) {
												if ((p = strtok(NULL, " ")) == NULL)
													break;
												Sarjat[i].valuku[os] = va+1;
												strncpy(Sarjat[i].va_matka[os][va], p, sizeof(Sarjat[i].va_matka[os][va])-1);
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
error :
		if (er != 0) {
			clrln(14);
			clrln(15);
			clrln(16);
			clrln(17);
			clrln(18);
			sccurset(15,0);
			sprintf(prs,"Tiedosto %s viallinen", sarjafname);
			vidspmsg(15,0,7,0,prs);
			sprintf(prs,"Virhe rivill‰ %d tai ennen sit‰", er);
			vidspmsg(16,0,7,0,prs);
			vidspmsg(17,0,7,0,
				"Korjaa tiedosto ennen uutta k‰ynnistyst‰");
			writeerror("Virhe vanhoja sarjatietoja (KILP.SRJ) luettaessa.", 0);
			return(1);
			}
		fclose(sarjaf);
		}
	else {
		strcpy(prs, "Sarjatietoja ei lˆydy, k‰yttˆ mahdotonta");
		writeerror(prs, 0);
		scpclr();
		return(1);
		}
	maaraaOsuusluku(true);
	return(0);
//	lueEventXml(L"KilpSrj.xml", true, false, false);
	}

void kirjsarjat(void)
	{
	FILE *sarjaf;
	int   i, er, os, va, ok;
	char pstr[MAXOSUUSLUKU+1], st[200];

	enslahto = TMAALI0;
	for (i = 0; i < sarjaluku; i++) {
		if (nilm[i] && (enslahto == TMAALI0 || NORMT(Sarjat[i].lahto - enslahto) > 12L * TUNTI))
			enslahto = Sarjat[i].lahto;
		}
	if (enslahto == TMAALI0)
		enslahto = Sarjat[0].lahto;

	sarjaf = fopen(sarjafname,"wt");
	ok = (sarjaf != NULL);
	if (ok) {
		er = fprintf(sarjaf, "%s*\n%5d %5d %5d\n", wcstoansi(st, kilpailu, 100),
			sarjaluku, minkilpno, maxkilpno);
		if (er != EOF) {
			for ( i = 0 ; i < sarjaluku && er != EOF ; i++) {
#ifdef MAALI
				for (os=0; os < Sarjat[i].osuusluku; os++)
					pstr[os] = Sarjat[i].paikat[os] + '0';
				pstr[Sarjat[i].osuusluku] = 0;
#else
				memcpy(pstr, Sarjat[i].paikat[0], MAXOSUUSLUKU+1);
#endif
/*
				if (!Sarjat[i].paikat[0]) {
					memset(Sarjat[i].paikat, '0', Sarjat[i].osuusluku);
					Sarjat[i].paikat[Sarjat[i].osuusluku] = 0;
					if (!Sarjat[i].osuusluku)
						strcpy(Sarjat[i].paikat, "0");
					}
*/
				er = fprintf(sarjaf,
					"%s*\n%5d %3d %5d %8ld %5d %8ld %8ld  %8ld%s\n", Sarjat[i].sarjanimi,
					Sarjat[i].osuusluku, Sarjat[i].mno, Sarjat[i].sarjaalku, Sarjat[i].lahto, Sarjat[i].maksu,
					Sarjat[i].ylahto[Sarjat[i].osuusluku-1], Sarjat[i].ylkynnys[Sarjat[i].osuusluku-1], Sarjat[i].ylahto[0], pstr);
				if (er == EOF) break;
				for (os = 0; os < MAXOSUUSLUKU; os++) {
					if (Sarjat[i].matka[os][0] > ' ') {
						er = fprintf(sarjaf,"%s",Sarjat[i].matka[os]);
						for (va = 0; va < VAIKALUKU; va++) {
							if (er == EOF || Sarjat[i].va_matka[os][va][0] <= ' ')
								break;
							er = fprintf(sarjaf," %s",Sarjat[i].va_matka[os][va]);
							}
						}
					er = fprintf(sarjaf,"*\n");
					if (er == EOF) break;
					}
				}
			}
		if (er == EOF)
			ioerr(sarjafname, 2, 0L);
		fclose(sarjaf);
		}
	else 
		writeerror("Sarjatietotiedoston avaus kirjoitusta varten ei onnistu", 0);
	}

void sarjatietue::tulkMTB(wchar_t *ln, int osuus)
{
	wchar_t *p;

	p = wcstok(ln, L" ;,\t");
	if (!p)
		return;
	laskemtb = _wtoi(p) > 0;
	for (int i = 0; i <= valuku[osuus]; i++) {
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[osuus][i].alkukdi = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[osuus][i].loppukdi = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[osuus][i].piste = _wtoi(p);
		if ((p = wcstok(NULL, L" ;,\t")) == NULL)
			break;
		mtb[osuus][i].pohja = _wtoi(p);
		}
}


