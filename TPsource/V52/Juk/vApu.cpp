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

/*
void wkirjloki(wchar_t *tmsg)
	{
	wchar_t msg[500];
	char cmsg[500];

	wcsncpy(msg, tmsg, 497);
	msg[497] = 0;
	wcscat(msg, L"\n");
	EnterCriticalSection(&loki_CriticalSection);
	if (lokiTCP) {
		wcstoansi(cmsg, msg, 499);
		wrt_st_TCP(hLoki, strlen(cmsg), cmsg, &nsent);
		}
	else
	sendline(wcstoansi(cmsg, msg, 498), lokifile);
	LeaveCriticalSection(&loki_CriticalSection);
	}
*/

#ifdef _DEBUG
bool hav_virheet = true;
#else
bool hav_virheet = false;
#endif

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



void wkirjloki(wchar_t *tmsg)
	{

	wchar_t msg[500], *p;
	char cmsg[500];
	int nsent;

	wcsncpy(msg, tmsg, 497);
	msg[497] = 0;
	for (p = msg+wcslen(msg)-1; p - msg >= 0 && (*p == L'\r' || *p == L'\n'); p--)
		*p = 0;
	wcscat(msg, L"\r\n");
	if (!lokiTCP && lokitulparam.lstf == NULL)
		return;
	if (inLopetus > 1)
		return;
	EnterCriticalSection(&loki_CriticalSection);
/*
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
*/
	if (lokiTCP) {
		wcstoansi(cmsg, msg, 499);
		wrt_st_TCP(hLoki, strlen(cmsg), cmsg, &nsent);
		}
	else
		sendwline(msg, lokitulparam.lstf);
	LeaveCriticalSection(&loki_CriticalSection);
	}

int haesarja(char *snimi, bool salliyhd)
{
   int i;

   i = 0;
   upcasestr(snimi);
   stpcvt(snimi, 6);
   do {
	  if (!memcmp(snimi, Sarjat[i].sarjanimi,
		strlen(Sarjat[i].sarjanimi) + 1))
		 return(i);
   } while (++i < sarjaluku + (salliyhd ? nsarjayhd : 0));
   return(-1);
}

int haesarja_w(wchar_t *snimi, bool salliyhd)
{
	char st[40];

	return(haesarja(wcstoansi(st, snimi, 38), salliyhd));
}

int haeSeurVapaaKno(int alku)
{
	UINT32 d;
	kilptietue kilp;
	int kno = 0;
	char var[MAXKILPNO+1];

	memset(var, 0, sizeof(var));
	if (alku < minkilpno) {
		alku = minkilpno;
		}

	for (d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus == 0) {
			var[kilp.kilpno] = 1;
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

void spoolstatus(void)
   {

   }

char *keyname(kilptietue *kilp)
   {
   static char key[NIMIINDL];
   wchar_t str[100];

   keyfromU16name(key, kilp->Seura(str, NIMIINDL-3), NIMIINDL-3, 0);
   _swab((char *) &kilp->sarja, key+NIMIINDL-3, 2);
   key[NIMIINDL-1] = (char) kilp->joukkue;
   return(key);
   }

char *osuuskoodi(int sarja, int osuus, char *kdi, int len)
{
	static char buf[5];
	wchar_t wkdi[5];
	int ll;

	if (len > 4)
		len = 4;
	ll = len;
	if (len < 1)
		ll = 4;
	if (kdi == 0)
		kdi = buf;
	wcstoansi(kdi, wosuuskoodi(sarja, osuus, wkdi, len), ll);
	return(kdi);
}

wchar_t *wosuuskoodi(int sarja, int osuus, wchar_t *kdi, int len)
{
	static wchar_t buf[5];
	wchar_t st[5];
	int pos, ll, os;

	if (kdi == 0)
		kdi = buf;
	ll = len;
	if (len > 4 || len < 1)
		ll = 4;
	wmemset(kdi, L' ', ll);
	kdi[ll] = 0;
	if (sarjaluku == 1)
		sarja = 0;
	if (sarja < 0 || osuus < 0) {
		_itow(osuus+1, st, 10);
		}
	else {
		os = Sarjat[sarja].yosuus[osuus];
		if (Sarjat[sarja].nosuus[os] > 1) {
			kdi[ll-1] = L'A' + osuus - Sarjat[sarja].aosuus[os] - 1;
			ll--;
			}
		_itow(os+1, st, 10);
		}
	pos = ll - wcslen(st);
	if (pos < 0)
		pos = 0;
	wmemcpy(kdi+pos, st, ll-pos);
	if (len == 0)
		elimwbl(kdi);
	return(kdi);
}

int tulkOsuuskoodi(int sarja, wchar_t *st)
{
	int alios = 0, yos = 0;

	if (sarjaluku == 1)
		sarja = 0;
	yos = _wtoi(st)-1;
	if (sarja < 0 || sarja >= sarjaluku || Sarjat[sarja].maxnosuus == 1 || yos >= Sarjat[sarja].ntosuus)
		return(yos);
	if (st[wcslen(st)-1] >= L'A')
		alios = towupper(st[wcslen(st)-1]) - L'A';
	return(Sarjat[sarja].aosuus[yos]+1+alios);
}

int sarjaKno(int kilpno)
{
	int d;
	kilptietue kilp;

	if ((d = getpos(kilpno)) > 0) {
		kilp.getrec(d);
		return(kilp.sarja);
		}
	return(-1);
}

INT va_piste(INT piste)
	{
	if (piste < -2)
		piste += 256;
	if (piste > VAIKALUKU)
		piste = VAIKALUKU;
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

int JosAlku(int srj, int os, int *aos)
{
	*aos = Sarjat[srj].aosuus[os] + 1 + josalku[srj][os] % kilpparam.maxnosuus;
	return(josalku[srj][os] / kilpparam.maxnosuus);
}

void setJosAlku(int srj, int aos, int jp)
{
	josalku[srj][Sarjat[srj].yosuus[aos]] = kilpparam.maxnosuus * jp +
		aos - Sarjat[srj].aosuus[Sarjat[srj].yosuus[aos]] - 1;
}

void asetaSrjLahto(int srj, int aika, bool siirto)
{
	kilptietue kilp;
	int tlahto;

	if (srj < 0)
		return;
	tlahto = aika;
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < (int)datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus == 0 && kilp.sarja == srj) {
			if (siirto) {
				tlahto = kilp.Lahto(0);
				if (tlahto != TMAALI0)
					tlahto += aika;
				}
			kilp.ostiet[0].ylahto = tlahto;
			kilp.ostiet[0].lahtolaji = 1;
			tallenna(&kilp, d, 0, 0, 0, 0);
		}
	}
	LeaveCriticalSection(&tall_CriticalSection);
}

void asetaEraLahto(int era, int aika)
{
	kilptietue kilp;
	int tlahto;

	if (era <= 0)
		return;
	tlahto = aika;
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < (int)datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus == 0 && (int)kilp.era == era) {
			kilp.ostiet[0].ylahto = tlahto;
			kilp.ostiet[0].lahtolaji = 1;
			tallenna(&kilp, d, 0, 0, 0, 0);
		}
	}
	LeaveCriticalSection(&tall_CriticalSection);
}

int psija(int kno, int srj, int os, int va)
   {
   int  sj,d,nt, p, yhd = 0;

   d = getpos(kno);
   if (va <= kilpparam.maxvaluku) {
	   p = jalku[srj][os][va ? va+4 : 0];
	   nt = ntulosix[srj][os][va];
	   if (va && nt > maxnvaika)
		  nt = maxnvaika;
	   sj = entsija(d,&p,os,va,yhd,0);
	   }
   else {
	   int yos, eos;

	   yos = Sarjat[srj].yosuus[os];
	   p = JosAlku(srj, yos, &eos);
	   nt = ntulosix[srj][yos][va];
	   sj = entossija(d, &p, srj, os, &eos, yhd, 0);
	   }
   if (sj > 9990)
	  sj = 0;
   return(sj);
   }

void merk_sijat(int srj, int osuus, int va, int sija)
   {
   int sj,sj0, yhd = 0;
   int jp;
   TMAALI tm0 = TMAALI0;
   kilptietue kilp;

   if (srj >= sarjaluku)
	   yhd = srj - sarjaluku + 1;
   if (va <= kilpparam.maxvaluku) {
	   if (srj < sarjaluku && va > Sarjat[srj].valuku[osuus])
			return;
	   sj = 1;
	   sj0 = 0;
	   jp = jalku[srj][osuus][va == 0 ? 0 : va+4];
	   while (jp > 0) {
		  if (sj >= sija) {
			 GETREC(&kilp, jp);
			 if (kilp.tTulos(osuus, va) != tm0)
				 sj0 = sj;
			 tm0 = kilp.tTulos(osuus, va);
			 for (int os = Sarjat[srj].aosuus[osuus]+1; os <= Sarjat[srj].aosuus[osuus+1]; os++)
				kilp.setSija(os, va, sj0);
			 PUTREC(&kilp, jp);
			 }
		  jp = JarjSeur(yhd, osuus, va, jp);
		  sj++;
		  }
	   }
   else {
	   int eos;
	   sj = 1;
	   sj0 = 0;
	   jp = JosAlku(srj, osuus, &eos);
	   while (jp > 0) {
		  if (sj >= sija) {
			 GETREC(&kilp, jp);
			 if (kilp.osTulos(eos, 0) != tm0)
				sj0 = sj;
			 tm0 = kilp.osTulos(eos, 0);
			 kilp.setSija(eos, kilpparam.valuku+1, sj0);
			 PUTREC(&kilp, jp);
			 }
		  jp = OsJarjSeur(yhd, srj, &eos, jp);
		  sj++;
		  }
	   }
   }

__int64 jtulos(kilptietue *kilp, int osuus, int va)
   {

   __int64 jt;
   char kh;

   int khi = 0, a = 0;

   // osuusaikoihin liittyvä jtulos on jtulosjak*ostulos
   // kun osuusaika puuttu jtulos = 4*jtulosaskel

   if (va == kilpparam.valuku+1) {
		if ((jt = pyoristatls(kilp->osTulos(osuus, 0, true), 2)) > 0)
			return(kilpparam.jtulosjak * jt);
		else
			return(4*kilpparam.jtulosaskel);
		}

   if (Sarjat[kilp->sarja].maxnosuus > 1) {
	   __int64 puutelisa;

	   jt = pyoristatls(kilp->tTulos(osuus, va, &puutelisa), 2);
	   if (jt) {
		   return(kilpparam.jtulosjak * jt + puutelisa);
//		   if (!puutelisa)
//			   return(kilpparam.jtulosjak * jt);
//			else
//			   jt += puutelisa;
			}
		if (va)
		   return(kilpparam.jtulosaskel);
	   if (osuus == 0) {
		  jt = ((kilpparam.jtulosaskel / (maxkilpno - minkilpno)) *
			 (kilp->kilpno - minkilpno));
		  }
	   else if (!jt) {
		  jt = jtulos(kilp, osuus-1, 0) / 16;
		  }
	   switch (kilp->tTark(osuus)) {
			 case 'K' : khi = max(khi, 1); break;
			 case 'H' : khi = max(khi, 2); break;
			 case 'P' :
			 case 'E' :
			 case 'S' : khi = max(khi, 3); break;
			 default:
				if (kilp->aTulos(osuus, 0) == 0)
					khi = 4;
			 }
	   return(jt + khi*kilpparam.jtulosaskel);
	   }

   jt = pyoristatls(kilp->Tulos(osuus, va), 2);
   if (kilpparam.alalaji == L'D' && osuus == Sarjat->osuusluku-1 && jt) {
		jt += (Sarjat->osuusluku - kilp->pisteet[0]) * 48LL * TUNTI;
		}
   else
		if (jt > 72LL*TUNTI) jt = 72LL*TUNTI;
   if (kilpparam.rogaining) {
	   if (jt) {
		   if (kilp->pisteet[0] < 0)
			   return((72LL*TUNTI*30001LL)-1);
		   else
			   return((30000LL - kilp->pisteet[0])*72LL*TUNTI + jt);
		   }
	   else {
		   switch (kilp->tTark(osuus)) {
			  case 'K' : return(72LL*TUNTI*30001LL);
			  case 'H' : return(72LL*TUNTI*30002LL);
			  case 'E' :
			  case 'S' : return(72LL*TUNTI*30003LL);
			  default  : return(72LL*TUNTI*30004LL);
			  }
		   }
	   }
   if (jt)
	   return(kilpparam.jtulosjak * jt);
   if (va)
	   return(kilpparam.jtulosaskel);
   if (osuus == 0) {
	  jt = ((kilpparam.jtulosaskel / (maxkilpno - minkilpno + 1)) *
		 (kilp->kilpno - minkilpno));
	  }
   else {
	  jt = jtulos(kilp, osuus-1, 0) / 16;
	  }
   kh = kilp->tTark(osuus);
   switch (kh) {
	  case 'K' : return(jt + kilpparam.jtulosaskel);
	  case 'H' : return(jt + 2*kilpparam.jtulosaskel);
	  case 'E' :
	  case 'S' : return(jt + 3*kilpparam.jtulosaskel);
	  default  : return(jt + 4*kilpparam.jtulosaskel);
	  }
   }

int KarkiTulos(int sarja, int os, int piste)
{
	int yhd = 0;
	if (sarja >= sarjaluku)
		yhd = sarja - sarjaluku + 1;
	return(JarjTulos(yhd, os, piste, jalku[sarja][os][piste]));
}

wchar_t *IOFStatus(kilptietue *kilp, int osuus)
{
	if (kilp->Tulos(osuus, 0) || !kilp->Hyv(osuus)) {
		switch (kilp->wTark(osuus, true)) {
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
			case L'S':
				return(L"DidNotStart");
			case L'P':
			case L'V':
				return(L"DidNotEnter");
			}
		}
	return(L"Active");
}

void nollaayhdlkm(int os)
	{
	ntulos[sarjaluku][os][0] = 0;
	ntulosix[sarjaluku][os][0] = 0;
	nkesk[sarjaluku][os] = 0;
	nhyl[sarjaluku][os] = 0;
	}

void merk_yl(kilptietue *kilp)
   {
   int os, tos, ny, ylf;
   TMAALI tm;
   static bool inmerk_yl;

   if (inmerk_yl)
	   return;
   if (Sarjat[kilp->sarja].ylkynnys[Sarjat[kilp->sarja].ntosuus-1] == TMAALI0)
	  return;

   inmerk_yl = true;
   if (Sarjat[kilp->sarja].maxnosuus == 1) {
	  ylf = 0;
	  for (os = 1; os < Sarjat[kilp->sarja].osuusluku; os++) {
		 if (kilp->Maali(os-1, 0) == TMAALI0) continue;
		 if (ylf ||
			NORMT(kilp->Maali(os-1, 0)) > NORMT(Sarjat[kilp->sarja].ylkynnys[os])
			) {
			ylf = 1;
			if (kilp->Maali(os, 0) == TMAALI0)
				continue;
			if (kilp->ostiet[os].ylahto != Sarjat[kilp->sarja].ylahto[os] && kilp->ostiet[os].lahtolaji == 0) {
			   kilp->ostiet[os].ylahto = Sarjat[kilp->sarja].ylahto[os];
			   }
			else
			   continue;
			}
		 else {
			if (kilp->ostiet[os].ylahto != TMAALI0 && kilp->ostiet[os].lahtolaji == 0)
			   kilp->ostiet[os].ylahto = TMAALI0;
			else
			   continue;
			}
		}
	  }
   else {

	   for (os = Sarjat[kilp->sarja].aosuus[1]+1; os < Sarjat[kilp->sarja].osuusluku; os++)
			if (kilp->ostiet[os].lahtolaji == 0)
				kilp->ostiet[os].ylahto = TMAALI0;
	   for (tos = 1; tos < Sarjat[kilp->sarja].ntosuus; tos++) {
		  ny = 0;
		  for (os = Sarjat[kilp->sarja].aosuus[tos-1] + 1; os <= Sarjat[kilp->sarja].aosuus[tos]; os++) {
			 tm = kilp->Maali(os, 0);
			 if (kilp->ostiet[os].ylahto != TMAALI0 ||
				 (tm != TMAALI0 && NORMT(tm) > NORMT(Sarjat[kilp->sarja].ylkynnys[tos]))
				 )
				 ny++;
			  }
		  if (ny) {
			 if (Sarjat[kilp->sarja].nosuus[tos-1] != Sarjat[kilp->sarja].nosuus[tos])
				 ny = Sarjat[kilp->sarja].nosuus[tos];
			 for (os = Sarjat[kilp->sarja].aosuus[tos+1]-ny+1; os <= Sarjat[kilp->sarja].aosuus[tos+1]; os++)
				if (kilp->ostiet[os].lahtolaji == 0)
					kilp->ostiet[os].ylahto = Sarjat[kilp->sarja].ylahto[tos];
			 }
		  }
	   }
   inmerk_yl = false;
   }

void merk_yht_lahdot(int srj, bool nayta)
   {
   char ch;
   int *d,i,os,p = -1, lj = -1;
   kilptietue kilp, entkilp;

   if (nilm[srj] == 0)
	   return;
   for (os = 1; os < Sarjat[srj].ntosuus; os++)
		if (Sarjat[srj].ylkynnys[os] != TMAALI0)
			break;
   if (os == Sarjat[srj].ntosuus)
	   return;
   if (nayta) {
	   vidspmsg(ySize-1,0,7,0,"Kirjaa yhteislähtöjä, sarja :");
	   vidspmsg(ySize-1,40,7,0,(char *)Sarjat[srj].sarjanimi);
	   }
   EnterCriticalSection(&tall_CriticalSection);
   if ((d = (int *) malloc(nilm[srj]*sizeof(int))) != 0) {
	   for (i = 0; i < nilm[srj]; i++) {
		   p = askellasarja(srj, 0, 0, 0, p, &lj);
		   if (p > 0)
				d[i] = p;
		 }
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   if (!d)
	  return;
	for (i = 0; i < nilm[srj]; i++) {
		if (d[i] <= 0)
			continue;
		while (comfl && !levypusk && ((cjseur[0] + OUTBUFL - cjens[0]) %
			OUTBUFL > (3*OUTBUFL)/4 ||
			(cjseur[1] + OUTBUFL - cjens[1]) % OUTBUFL > (3*OUTBUFL)/4)) {
#ifdef _CONSOLE
			vidspmsg(ySize-3,0,7,0,"Odottaa tiedonsiirron "
				"etenemistä.  'Esc' lopettaa"
				" yhteislähtöjen kirjauksen");
			if (readkbd(&ch, FALSE, spoolfl) == ESC)
				goto escape;
#endif
			Sleep(50);
			}
		EnterCriticalSection(&tall_CriticalSection);
		GETREC(&kilp, d[i]);
		entkilp = kilp;
		merk_yl(&kilp);
		if (!(kilp == entkilp)) {
			clrln(ySize-3);
//			tallenna(&kilp, d[i], 0, 0, 0, &entkilp);
			tallenna(&kilp, d[i], 0, 0, 0, 0);
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	escape:
	free(d);
	if (nayta)
	clrln(ySize-1);
}

int HaeSeuraLista(bool maafl, wchar_t (*sra)[LSEURA+5], int *nsra, int maxnsra)
{
	kilptietue kilp;
	int isra, err = 0;
	wchar_t st[LSEURA+5], st1[60];
	struct sratstr {
		char key[LSEURA+5];
		wchar_t seura[LSEURA+5];
		} *srat;

	*nsra = 0;
	for (int d = 1; d < (int)datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus)
			continue;
		memset(st, 0, sizeof(st));
		if (maafl) {
			wcscpy(st, L"    ");
			kilp.Maa(st1);
			wmemcpy(st, st1, wcslen(st1));
			}
		kilp.Seura(st+wcslen(st), kilpparam.lseura);
		for (isra = 1; isra <= *nsra; isra++) {
			 if (!wcscmp(st, sra[isra]))
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
	keyfromU16name(key, Buf, sizeof(key), 0);
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
//	EnterCriticalSection(&seura_CriticalSection);
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
//	LeaveCriticalSection(&seura_CriticalSection);
	return(0);
}
//---------------------------------------------------------------------------

int seuraLuettelo::kirjSeurat(wchar_t *SraFName)
{
	TextFl *OutFile;

//	EnterCriticalSection(&seura_CriticalSection);
	if (SraFName && SraFName[0])
		OutFile = new TextFl(SraFName, L"w");
	else
		OutFile = new TextFl(FName, L"w");
	if (OutFile->IsOpen()) {
		wchar_t Buf[1000];

		for (int i = 0; i < NSeura; i++) {
			if (maakoodit)
				swprintf(Buf, L"%d\t%s\t%s\n", Seura[i].piiri, Seura[i].maa, Seura[i].nimi);
			else
				swprintf(Buf, L"%d\t%s\n", Seura[i].piiri, Seura[i].nimi);
			OutFile->WriteLine(Buf);
			}
		OutFile->Close();
		}
	delete OutFile;
//	LeaveCriticalSection(&seura_CriticalSection);
	return(0);
}
//---------------------------------------------------------------------------

void _fastcall seuraLuettelo::TallennaSeura(int piiri, wchar_t *maa,
		wchar_t *nimi, int kielto)
{
	char hkey[22];
	seuraTieto *pSeura, Sra;

	if (Seura) {
		keyfromU16name(hkey, nimi, sizeof(hkey), 0);
		pSeura = (seuraTieto *) bsrch(hkey, (char *) Seura, &NSeura, sizeof(Seura[0].key), sizeof(Seura[0]));
		if (!pSeura || wcscmp(pSeura->nimi, nimi)) {
			memset(&Sra, 0, sizeof(Sra));
			Sra.piiri = piiri;
			memcpy(Sra.key, hkey, sizeof(Sra.key));
			wcsncpy(Sra.maa, maa, 3);
			wcsncpy(Sra.nimi, nimi, kilpparam.lseura);
			if (NSeura < NSeuraMax) {
//				EnterCriticalSection(&seura_CriticalSection);
				Seura[NSeura++] = Sra;
				quicksort((char *)Seura, NSeura, sizeof(Seura[0].key), sizeof(Seura[0]));
//				LeaveCriticalSection(&seura_CriticalSection);
				kirjSeurat(FName);
				if (NSeura == NSeuraMax) {
					lueSeurat(FName);
					}
				}
			}
		}
//	lah_seura(nimi, maa, piiri, kielto);
}
//---------------------------------------------------------------------------

seuraTieto *seuraLuettelo::haenimella(wchar_t *nimi, bool tarkka)
{
	if (NSeura == 0)
		return(NULL);
	if (!tarkka && jarj == 1) {
		char key[30];

		keyfromU16name(key, nimi, sizeof(key), false);
		return((seuraTieto *)bsrch(key, (char *)Seura, &NSeura, sizeof(Seura[0].key), sizeof(Seura[0])));
		}
	for (int is = 0; is < NSeura; is++) {
		if (!wcscmp(Seura[is].nimi, nimi))
			return(Seura+is);
		}
	return(NULL);
}
//---------------------------------------------------------------------------

unsigned checkstack(void);

void lopetus1(void);
void SuljeKaikki(void);

void lopetus(void)
{
#ifdef _CONSOLE
	lopetus1();
#else
	SuljeKaikki();
#endif
}

void close_profile(void);

void lopetus1(void)
   {

   // kaynnistysvaihe = 0    Ennen kaikkia initialiaointeja
   //                   1    Kun sarjat luettu
   //                   2    KILP.DAT luettu kokonaan ja indeksoitu
   //                   3    ajanotto ja tiedonsiirto käynnistetty
   //                   4    kaikki initialisoinnit tehty


	if (inLopetus)
		return;
	if (loki) {
		wchar_t lmsg[60];
		swprintf(lmsg, L"Ohjelmasta poistutaan, klo %s", wkello());
		wkirjloki(lmsg);
		}
	lkmThreadOn = false;
   sulkemassa = true;
   inLopetus = 1;
   taustaon = 0;
   ProsEmitOn = false;
   inkirjSQLThread = false;

   scpclr();
   vidspmsg(3,3,7,0,"Odota hetki, tiedostoja ja yhteyksiä suljetaan");
   if (kaynnistysvaihe > 2) {
#if defined(ALGETAULU) || defined(TAULU)
	  if (taulu_com || udptaulu) {
		 closetaulu();
		 }
#endif
	  }
   if (kaynnistysvaihe > 1) {
#ifndef LUENTA
//      if (rwtime[0] > 0 || (maxjono && rwtime[1] > 0)) {
		 suljeajat(-1, true);
//         }
#endif
		for (int i = 0; i < NREGNLY; i++)
			regnly[i] = 0;
		remtimer();
		closecom();
		close_profile();
//	  kirjsarjat();
		kirjEventXml(L"KilpSrj.xml", true, false, false);

		EnterCriticalSection(&tall_CriticalSection);
		closefile(&datf2);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	if (kaynnistysvaihe > 0) {
		if (lokitulparam.lstf) {
			closeprfile(lokitulparam.lstf);
			}
		if (lokiTCP) {
			closeportTCP(&hLoki);
			}
		}
	Sleep(200);
#ifdef DBGFILE
	if (dbgtofile)
		closedbgfile();
#endif
#ifdef EMITHTTP
	while (inhttpHaku)
		Sleep(200);
#endif
	inLopetus = 2;
/*
	Sleep(100);
   DeleteCriticalSection(&autotul_CriticalSection);
   DeleteCriticalSection(&tcpip_CriticalSection);
   DeleteCriticalSection(&print_CriticalSection);
   DeleteCriticalSection(&loki_CriticalSection);
   DeleteCriticalSection(&inb_CriticalSection);
   DeleteCriticalSection(&outb_CriticalSection);
	DeleteCriticalSection(&rastiva_CriticalSection);
   DeleteCriticalSection(&emit_CriticalSection);
   DeleteCriticalSection(&ajat_CriticalSection);
   DeleteCriticalSection(&kilpdat_CriticalSection);
   DeleteCriticalSection(&tall_CriticalSection);
#ifdef TCPSIIRTO
   if (fileyht >= MAX_LAHPORTTI && kaynnistysvaihe > 1)
	  DeleteCriticalSection(&tcp_CriticalSection[fileyht]);
#endif
*/
   cleanWSA();
   scpclr();
   sccurset(0,0);
#ifdef _CONSOLE
   exit(0);
#endif
   }
