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

void rem_pr(void);

int powi(int i, int j)
{
	int k = 1;

	for (; j > 0; j--)
		k *= i;
	return(k);
}

wchar_t *otsTeksti(wchar_t *out, wchar_t *in, int len, int i_pv)
{
	wchar_t *pi, *po, st[20];
	static wchar_t buf[400];

	if (out == NULL) {
		out = buf;
		if (len > sizeof(buf)/2)
			len = sizeof(buf)/2;
		}
	po = out;
	for (pi = in; *pi && po - out < len - 1; ) {
		if (*pi != L'[') {
			*po++ = *pi++;
			continue;
			}
		if (wmemcmpU(pi, L"[vaihe]", 7) == 0) {
			if (i_pv >= 0)
				*po++ = L'1' + i_pv;
			pi += 7;
			continue;
			}
		if (wmemcmpU(pi, L"[v-ots]", 7) == 0) {
			if (i_pv >= 0 && out + len - po - 1 > wcslen(pvparam[i_pv].Title)) {
				wmemcpy(po, pvparam[i_pv].Title, wcslen(pvparam[i_pv].Title));
				po += wcslen(pvparam[i_pv].Title);
				}
			pi += 7;
			continue;
			}
		if (wmemcmpU(pi, L"[pvm]", 5) == 0) {
			if (i_pv >= 0) {
				int pvm = pvparam[i_pv].Date;
				swprintf(st, L"%d.%d.%d", dDateNo(pvm), mDateNo(pvm), yDateNo(pvm));
				if (out + len - po - 1 > wcslen(st)) {
					wmemcpy(po, st, wcslen(st));
					po += wcslen(st);
					}
				}
			pi += 5;
			continue;
			}
		}
	*po = 0;
	return(out);
}

wchar_t *insdes(wchar_t *st, int des, wchar_t dessign)
{
	if (des <= 0)
		return(st);
	if ((int) wcslen(st) < des+1) {
		wmemmove(st+des-wcslen(st)+2, st, wcslen(st)+1);
		wmemcpy(st, L"0000000", des-wcslen(st)+2);
		}
	else {
		wmemmove(st+wcslen(st)-des+1, st+wcslen(st)-des, des+1);
		}
	st[wcslen(st)-des-1] = dessign;
	return(st);
}

int parametrivirhe(wchar_t *sub, int intparam, wchar_t *strparam, wchar_t charparam)
{
	wchar_t msg[200], st[2] = L"", *str;

	if (strparam != NULL)
		str = strparam;
	else
		str = st;
	if (hav_virheet) {
		swprintf(msg, L"Parametrivirhe, aliohjelma %s, parametrit %d, %s, %c", sub, intparam, str, charparam);
		writeerror_w(msg, 0);
		}
	return(0);
}

int vaiheLkm(int sarja)
{
	int n = 0;
	for (int i = Sarjat[sarja].enspv; i <= Sarjat[sarja].viimpv; i++)
		if (Sarjat[sarja].luonne[i] >= 0)
			n++;
	return(n);
}

INT va_piste(INT piste)
	{
	if (piste < -2)
		piste += 256;
	if (piste > VALUKU)
		piste = VALUKU;
	return(piste);
	}

wchar_t *str_piste(int piste)
	{
	static wchar_t st[4];

	piste = va_piste(piste);
	memset(st, 0, sizeof(st));
	if (piste >= -2 && piste < 10) {
		st[0] = ch_piste[piste+2];
		}
	else if (piste >= 10 && piste < 1000)
		_itow(piste, st, 10);
	return(st);
	}

wchar_t *IOFStatus(kilptietue *kilp)
{
	if (kilp->p_aika(0) || !kilp->hyv()) {
		switch (kilp->tark()) {
			case L'T':
				return(L"OK");
			case L'-':
			case L'I':
				return(L"Finished");
			case L'H':
				return(L"Disqualified");
			case L'K':
				return(L"DidNotFinish");
			case L'E':
			case L'B':
				return(L"DidNotStart");
			case L'P':
			case L'V':
				return(L"DidNotEnter");
			}
		}
	return(L"Active");
}

int etsilisno(int lisno, int idno)
{
	int d;
	kilptietue kilp;

	if (lisno == 0)
	   return(0);
	for (d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0 && kilp.lisno[idno] == lisno)
			return(d);
		}
	return(0);
}

int haeSeurVapaaKno(int ipv, int alku)
{
	int d;
	kilptietue kilp;
	int kno = 0;
	char var[MAXKILPNO+1];

	memset(var, 0, sizeof(var));
	if (alku < minkilpno) {
		alku = minkilpno;
		}

	for (d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0) {
			if (ipv < 0)
				var[kilp.kilpno] = 1;
			else
				var[kilp.id(ipv)] = 1;
			}
		}
	for (kno = alku; kno <= maxkilpno; kno++)
		if (var[kno] == 0)
			return(kno);
	for (kno = minkilpno; kno < alku; kno++)
		if (var[kno] == 0)
			return(kno);
	return(0);
}

int haeSeurVapaaBadge(int entbadge)
{
	int bdg = 0;

	EnterCriticalSection(&seur_badge_CriticalSection);
	if (entbadge > 30000) {
		bdg = entbadge;
		do {
			bdg += BADGEASKEL;
			} while (haebdg(bdg) >= 0);
		}
	if (bdg <= 30000 || bdg > 9999999) {
		for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
		bdg = seurVapaaBadge++;
		}
	LeaveCriticalSection(&seur_badge_CriticalSection);
	return(bdg);
}

int haeSeurVakantti(int alku)
{
	int d;
	kilptietue kilp;
	int kno = 0, kno2 = 0;

	// Jos alku == 0 jatketaan aiemmasta arvosta vakanttialku
	// muuten alustetaan arvoon alku
	// lisäksi varmistetaan alueella minkilpno .. maxkilpno pysyminen

	if (alku > maxkilpno)
		alku = minkilpno;
	if (alku >= minkilpno)
		vakanttialku = alku;
	else {
		if (vakanttialku < minkilpno || vakanttialku > maxkilpno)
			vakanttialku = minkilpno;
		alku = vakanttialku;
		}
	for (; ;) {
		if ((d = getpos(vakanttialku)) > 0) {
			kilp.GETREC(d);
			if (kilp.tark(k_pv) == L'V') {
				if (Sarjat[kilp.Sarja()].vakanttisarja) {
					kno = vakanttialku;
					break;
					}
				else if (kno2 == 0)
					kno2 = vakanttialku;
				}
			}
		vakanttialku++;
		if (vakanttialku > maxkilpno)
			vakanttialku = minkilpno;
		if (vakanttialku == alku)
			break;
		}
	if (kno == 0) {
		kno = kno2;
		vakanttialku = kno;
    	}
	return(kno);
}

#ifndef _CONSOLE
void teevuorotaulu(int sarja, int uudet)
	{

	}
#endif

void talljarj(void)
	{
	INT srj;
	int tmploki;

	if (ToimintaTila < 2 || inLopetus)
		return;
	tmploki = loki;
	loki = 0;
	EnterCriticalSection(&tall_CriticalSection);
	for (srj = 0; srj < sarjaluku; srj++)
		sijat2(srj, true, false);
	LeaveCriticalSection(&tall_CriticalSection);
	loki = tmploki;
	}

INT maaraa_piste(kilptietue *kilp, INT32 t)
   {
   INT piste, p1, va_yla = 0;
   INT32 tls, yraja, edraja, edtls;
   static int td;

   piste = ol_piste;
	if (ol_piste == -2 && t > TMAALI0) {
		tls = kilp->tulos1(0, t, 0);
		if (pvparam[k_pv].hiihtolahto && (tls > 23 * TUNTI || (
			(Sarjat[kilp->Sarja()].valuku == 0 && Sarjat[kilp->Sarja()].tul_raja[k_pv] && tls <  Sarjat[kilp->Sarja()].tul_raja[k_pv]*SEK) ||
			(Sarjat[kilp->Sarja()].valuku > 0 && Sarjat[kilp->Sarja()].va_raja[k_pv][0] && tls <  Sarjat[kilp->Sarja()].va_raja[k_pv][0]*SEK)
			))) {
			piste = -1;
			}
		else if (va_vuoro && vavuoro_yla && Sarjat[kilp->Sarja()].valuku)
			{
			va_yla = Sarjat[kilp->Sarja()].valuku[k_pv];
			if (va_yla > vavuoro_yla)
				va_yla = vavuoro_yla;
			piste = 0;

			if (va_yla < vavuoro_ala)
				p1 = 0;
			else {
				for (p1 = va_yla; p1 >= vavuoro_ala; p1--) {
					if ((edtls = kilp->p_aika(p1, 0)) != 0)
					break;
					}
				if (p1 < va_yla)
					piste = p1 + 1;
				}

			if (p1 > 0 && samapisteraja > 0 && (td = abs(NORMKELLO(tls - edtls))) < samapisteraja)
				piste = p1;

			if (piste == 0) {
				yraja = Sarjat[kilp->Sarja()].tul_raja[k_pv]*SEK;
				p1 = va_yla;
				edraja = Sarjat[kilp->Sarja()].va_raja[k_pv][p1-1]*SEK;
				}
			else {
				yraja = Sarjat[kilp->Sarja()].va_raja[k_pv][piste-1]*SEK;
				if (piste > 1) {
					p1 = piste - 1;
					edraja = Sarjat[kilp->Sarja()].va_raja[k_pv][p1-1]*SEK;
					}
				else {
					p1 = pvparam[k_pv].hiihtolahto ? -1 : -2;
					edraja = 0;
					}
				}

			if (yraja) {
				if (tls - edtls < yraja - edraja)
					piste = p1;
				else  if (!vainalarajat) {
					p1 = piste;
					while (p1) {
						p1++;
						if (p1 > va_yla) {
							p1 = 0;
							yraja = Sarjat[kilp->Sarja()].tul_raja[k_pv]*SEK;
							}
						else
							yraja = Sarjat[kilp->Sarja()].va_raja[k_pv][p1-1]*SEK;
						if (!yraja || tls - edtls < yraja - edraja)
							break;
						piste = p1;
						}
					}
				}
			}
		else
			{
			if (tls > Sarjat[kilp->Sarja()].tul_raja[k_pv]*SEK)
				piste = 0;
			else {
				for (piste = 1; piste < Sarjat[kilp->Sarja()].valuku[k_pv]; piste++) {
					if (kierrosvafl && !Sarjat[kilp->Sarja()].va_raja[k_pv][piste])
						break;
					if (tls < Sarjat[kilp->Sarja()].va_raja[k_pv][piste]*SEK)
						break;
					}
				}
			}
		}
	if (vainvaliajat && piste == 0)
		{
		if (Sarjat[kilp->Sarja()].valuku[k_pv])
		piste = va_yla ? va_yla : Sarjat[kilp->Sarja()].valuku[k_pv];
		else
		piste = 1;
		}
	return(piste);
}

void asetaSrjLahto(int srj, int aika, bool siirto, bool korjaatulos, bool aina_arvottu)
{
	kilptietue kilp;
	int muutos;
	int tlahto;

	if (srj < 0)
		return;
	tlahto = aika;
	muutos = aika;
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0 && kilp.Sarja() == srj) {
			if (siirto)
				tlahto = kilp.TLahto() + aika;
			if (korjaatulos) {
				if (!siirto)
					muutos = aika - kilp.TLahto();
				for (int va = 0; va <= Sarjat[srj].valuku[k_pv]; va++) {
					int tls;
					if ((tls = kilp.p_aika(va)) != 0) {
						kilp.tall_tulos(va, tls - muutos);
					}
				}
			}
			if (aina_arvottu || pvparam[k_pv].hiihtolahto == 0)
				kilp.tall_lajat(tlahto, true);
			else
				kilp.tall_tulos(-1, tlahto);
			kilp.tallenna(d, 0, 0, 0, 0);
		}
	}
	LeaveCriticalSection(&tall_CriticalSection);
}

void asetaEraLahto(int ryh, int aika, bool korjaatulos, bool aina_arvottu)
{
	kilptietue kilp;
	int muutos;

	if (ryh <= 0)
		return;
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0 && kilp.pv[k_pv].era == ryh) {
			if (korjaatulos) {
				muutos = aika - kilp.TLahto();
				for (int va = 0; va <= Sarjat[kilp.Sarja()].valuku[k_pv]; va++) {
					int tls;
					if ((tls = kilp.p_aika(va)) != 0) {
						kilp.tall_tulos(va, tls - muutos);
					}
				}
			}
			if (aina_arvottu || pvparam[k_pv].hiihtolahto == 0)
				kilp.tall_lajat(aika, true);
			else
				kilp.tall_tulos(-1, aika);
			kilp.tallenna(d, 0, 0, 0, 0);
		}
	}
	LeaveCriticalSection(&tall_CriticalSection);
}

int HaeSeuraLista(int sralaji, bool maafl, int Vaihe, wchar_t (*sra)[LSEURA+5], int *nsra, int maxnsra)
{
	kilptietue kilp;
	int i_pv, isra, err = 0;
	wchar_t st[LSEURA+5];
	struct sratstr {
		char key[LSEURA+5];
		wchar_t seura[LSEURA+5];
		} *srat;

	*nsra = 0;
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus)
			continue;
		if (Vaihe > 0 && (kilp.tark(Vaihe-1) == L'P' || Sarjat[kilp.Sarja(Vaihe-1)].luonne[Vaihe-1] < 0))
			continue;
		if (Vaihe == 0) {
			for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
				if (kilp.tark(i_pv) != L'P' && Sarjat[kilp.Sarja(i_pv)].luonne[i_pv] >= 0)
					break;
				}
			if (i_pv >= kilpparam.n_pv_akt)
				continue;
			}
		memset(st, 0, sizeof(st));
		if (maafl) {
			wcscpy(st, L"    ");
			wmemcpy(st, kilp.maa, wcslen(kilp.maa));
			}
		 switch (sralaji) {
			case 0:
				wcscat(st, kilp.seuratieto(0));
				break;
			case 1:
				wcscat(st, kilp.seura);
				break;
			case 2:
				wcscat(st, kilp.seuralyh);
				break;
			}
		for (isra = 1; isra <= *nsra; isra++) {
			 if (!wcscmpU(st, sra[isra]))
				break;
			 }
		if (isra > *nsra) {
			if (*nsra >= maxnsra) {
				err = 1;
				break;
				}
			(*nsra)++;
			wcscpy(sra[isra], st);
			}
		}
	srat = new struct sratstr[*nsra];

	for (isra = 0; isra < *nsra; isra++) {
		w_aakjarjstr(srat[isra].key, sra[isra], LSEURA+4, 0, 0);
		wcscpy(srat[isra].seura, sra[isra]);
		}
	quicksort((char *) srat, *nsra, LSEURA+4, sizeof(srat[0]));
	for (isra = 0; isra < *nsra; isra++) {
		wcscpy(sra[isra], srat[isra].seura);
		}
	delete[] srat;
	return(err);
}
//---------------------------------------------------------------------------

void wkirjloki(wchar_t *tmsg)
	{

	wchar_t msg[500];
	char cmsg[500];
	int nsent;

	if (!loki)
		return;
	wcsncpy(msg, tmsg, 497);
	msg[497] = 0;
	wcscat(msg, L"\n");
	if (!lokiTCP && lokitulparam.lstf == NULL)
		return;
	EnterCriticalSection(&loki_CriticalSection);
	if (siritloki && sirittail != sirithead) {
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
	}

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
		if (!pSeura || wcscmpU(pSeura->nimi, nimi)) {
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
		if (!wcscmpU(Seura[is].nimi, nimi))
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
		if (!wcscmpU(Seura[is].lyhenne, lyhenne))
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


