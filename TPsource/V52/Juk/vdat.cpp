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
#include <string.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "VDeclare.h"

static int enst,innpfl[MAXOSUUSLUKU];
static int tls_muutos[MAXOSUUSLUKU];

//int tall_osuus = -1;

#ifdef SQLDATA
MYSQL  *sql;

void opensql(char *sqlhost, char *sqldatabase, char *sqluser, char *sqlpw)
   {
   sqlopen = 0;
   if ((sql = mysql_init(NULL)) == NULL)
      return;
   sql = mysql_real_connect(sql, sqlhost, sqluser, sqlpw, sqldatabase, 0, NULL, 0);
   if (sql)
      sqlopen = 1;
   }

void closesql()
   {
   mysql_close(*sql);
   }

void putrecsql(int d, kilptietue *kilp)
   {
   if (!sqlopen)
      return;

   }
#endif   

INT getpos(INT kno)
   {
	int d = -1;
//	if (ToimintaTila < 2 || inLopetus > 1)
	if (inLopetus > 1)
		return(d);
	if (kno >= minkilpno && kno <= maxkilpno) {
		d = kilpnoix[kno - minkilpno];
		}
	return(d);
   }

INT setpos(INT kno, int d)
   {
//   if (ToimintaTila < 2)
//	   return(0);
   if (d < 1 || (UINT32)d >= datf2.numrec) {
	   d = -1;
	   }
   if (kno >= minkilpno && kno <= maxkilpno) {
	  kilpnoix[kno-minkilpno] = d;
	  return(d);
	  }
   else
	  return(0);
   }

char *kilptietue::pack(char *buf)
{
	memmove(buf, this, kilpparam.kilprecsize0);
	for (int os = 0; os < kilpparam.n_os_akt; os++) {
		memmove(buf + kilpparam.kilprecsize0 +
			os * (kilpparam.osrecsize0 + (kilpparam.valuku+1) * kilpparam.vatpsize),
			ostiet+os,
			kilpparam.osrecsize0 + (kilpparam.valuku+1) * kilpparam.vatpsize);
		}
	return(buf);
}

void kilptietue::unpack(char *buf)
{
	nollaa();
	memmove(this, buf, kilpparam.kilprecsize0);
	for (int os = 0; os < kilpparam.n_os_akt; os++) {
		memmove(ostiet+os, buf + kilpparam.kilprecsize0 +
			os * (kilpparam.osrecsize0 + (kilpparam.valuku+1) * kilpparam.vatpsize),
			kilpparam.osrecsize0 + (kilpparam.valuku+1) * kilpparam.vatpsize);
		}
}

void getrec_1(kilptietue *kilp, INT d)
   {
   EnterCriticalSection(&kilpdat_CriticalSection);
   kilp->unpack(kilparr+d*kilpparam.kilprecsize);
   LeaveCriticalSection(&kilpdat_CriticalSection);
   }

void putrec_1(kilptietue *kilp, INT d)
   {
   EnterCriticalSection(&kilpdat_CriticalSection);
   kilp->pack(kilparr+d*kilpparam.kilprecsize);
   LeaveCriticalSection(&kilpdat_CriticalSection);
   }

INT32 oslahto(kilptietue *kilp, INT osuus)
   {
   if (!osuus) return(Sarjat[kilp->sarja].lahto);
   if (kilp->ostiet[osuus].ylahto != TMAALI0)
      return(NORMKELLO(kilp->ostiet[osuus].ylahto));
   if (kilp->Maali(osuus-1, 0) != TMAALI0)
      return(kilp->Maali(osuus-1, 0));
   return(Sarjat[kilp->sarja].lahto);
   }

INT32 purajak(INT32 tjak)
{
	return((tjak+1000000000)/AIKAJAK-1000000000/AIKAJAK);
}

void getrec_i(datafile *datf, int d, kilptietue *kilp)
   {
	char buf[KILPRECSIZE];

	getrec(datf,d,buf);
	kilp->unpack(buf);
   }

void putrec_i(datafile *datf, int d, kilptietue *kilp)
   {
	char buf[KILPRECSIZE];

	putrec(datf,d,kilp->pack(buf));
   if (kirjheti & 1)
	  FlushFileBuffers(datf->hDatf);
#ifdef SQLDATA
   if (sqlopen)
	  putrecsql(d, kilp);
#endif
   }

void GETREC(kilptietue *kilp, int d)
{
	if (d >= 0 && (UINT32)d < datf2.numrec) {
		kilp->unpack(kilparr+d*kilpparam.kilprecsize);
		}
}

void PUTREC(kilptietue *kilp, int d)
{
	if (d >= 0 && (UINT32)d < datf2.numrec) {
		kilp->pack(kilparr+d*kilpparam.kilprecsize);
		}
}

static void addfrec(kilptietue *kilp, int *d)
   {
   int l;
	char buf[KILPRECSIZE];

   EnterCriticalSection(&kilpdat_CriticalSection);
   l = filelen(&datf2);
   addrec(&datf2,(UINT *)d,kilp->pack(buf));
   if( filelen(&datf2) != l ) {
	  closefile(&datf2);
	  openfile(&datf2,dataf2name, kilpparam.kilprecsize);
	  }
   EnterCriticalSection(&kilpdat_CriticalSection);
   PUTREC(kilp, *d);
   }

static void delfrec(int d)
   {
   kilptietue kilp;

   EnterCriticalSection(&kilpdat_CriticalSection);
   kilp.getrec(d);
   deleterec(&datf2, d);
   kilp.kilpstatus = datf2.firstfree;
   kilp.putrec(d);
   LeaveCriticalSection(&kilpdat_CriticalSection);
   }

void paivjarjarr(int srj)
   {
   int os, i, va;
   int p = -1, lj = -1;
   kilptietue kilp;
//   __int64 tl;

   EnterCriticalSection(&tall_CriticalSection);
   for (va = 0; va <= kilpparam.maxvaluku; va++) {
	   for (os = 0; os < Sarjat[srj].ntosuus; os++) {
		  if (ntulosix[srj][os][va] > 0) {
			  p = -1;
			  lj = -1;
			 i = 0;
			 while (1) {
				 p = askellasarja(srj, os, 0, 0, p, &lj);
				 if (lj != 0 || p < 0)
					 break;
				if (++i > 3*ntulosix[srj][os][0])
				   break;
				GETREC(&kilp, p);
				remjarjarr(&kilp, p, os, 0, 0);
				addjarjarr(&kilp, p, os, 0, 0);
			 }
			 }
		 }
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   }

void merk_kilp_yl(kilptietue *kilp, int osuus)
   {
   int os;

   if (osuus > 0 && Sarjat[kilp->sarja].ylkynnys[osuus] != TMAALI0) {
	  if (kilp->Maali(osuus-1, 0) != TMAALI0 || stschind(kilp->Tark(osuus-1, 1), "KE") >= 0) {
		 if (NORMT(kilp->Maali(osuus-1, 0)) >
			NORMT(Sarjat[kilp->sarja].ylkynnys[osuus]) ||
				(kilp->Maali(osuus-1, 0) == TMAALI0 &&
				 (stschind(kilp->Tark(osuus-1, 1), "KE") >= 0))) {
			for (os = osuus; os < Sarjat[kilp->sarja].osuusluku; os++)
					if (kilp->ostiet[os].lahtolaji == 0)
						kilp->ostiet[os].ylahto = Sarjat[kilp->sarja].ylahto[os];
			}
		 else {
			if (kilp->ostiet[osuus].lahtolaji == 0)
				kilp->ostiet[osuus].ylahto = TMAALI0;
			for (os = osuus+1; os < Sarjat[kilp->sarja].osuusluku; os++) {
			   if ((kilp->Maali(os-1, 0) != TMAALI0 &&
				   NORMT(kilp->Maali(os-1, 0)) >
				   NORMT(Sarjat[kilp->sarja].ylkynnys[os])) ||
						(kilp->Maali(os-1, 0) == TMAALI0 &&
						 (stschind(kilp->Tark(os-1, 1), "KE") >= 0)))
				  break;
					if (kilp->ostiet[os].lahtolaji == 0)
						kilp->ostiet[os].ylahto = TMAALI0;
			   }
			}
		 }
	  }
   }

typedef struct {
	UINT32 ero;
	bool perustiedot;
	bool tark;
	bool hyvmol;
	bool tulosvaik;
	bool aika[VAIKALUKU+1];
	bool lahaika[VAIKALUKU+1];
	bool lahto;
} osuusero;

typedef struct {
	bool perustiedot;
	bool tulosvaik;
	osuusero os[MAXOSUUSLUKU];
} erostruct;

static int kilpTarkasta(kilptietue *kilp)
{
	int erflag = 0;
	if (kilp->sarja < 0 || kilp->sarja >= sarjaluku) {
		kilp->sarja = 0;
		erflag |= 1;
		}
	if (ToimintaTila > 1 && (kilp->kilpno < minkilpno || kilp->kilpno > maxkilpno)) {
		kilp->kilpno = haeSeurVapaaKno(0);
		erflag |= 2;
		}
	for (int os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
		if (kilp->ostiet[os].lahtolaji > 2) {
			kilp->ostiet[os].lahtolaji = 0;
			erflag |= 4;
			erflag |= 128*(os+1);
			}
		if (stschind(kilp->ostiet[os].keskhyl, "-TIKHEP") < 0) {
			kilp->ostiet[os].keskhyl = '-';
			erflag |= 8;
			erflag |= 128*(os+1);
			}
		}
	return(erflag);
}

static bool kilpcomp(kilptietue *kilp1, kilptietue *kilp2, erostruct *erot)
{
	bool ero = false;

	memset(erot, 0, sizeof(erostruct));
	if (kilp1->sarja != kilp2->sarja) {
		erot->perustiedot = true;
		erot->tulosvaik = true;
		for (int os = 0; os < Sarjat[kilp1->sarja].osuusluku; os++) {
			erot->os[os].tulosvaik = true;
			erot->os[os].perustiedot = true;
			}
		ero = true;
//		return(ero);
		}
	if (kilp1->kilpno != kilp2->kilpno ||
		strcmp(kilp1->seura, kilp2->seura) ||
		strcmp(kilp1->maa, kilp2->maa) ||
		strcmp(kilp1->joukknimi, kilp2->joukknimi) ||
		memcmp(kilp1->txt, kilp2->txt, sizeof(kilp1->txt)) ||
		memcmp(kilp1->pisteet, kilp2->pisteet, sizeof(kilp1->pisteet)) ||
		kilp1->joukkid != kilp2->joukkid ||
		kilp1->piiri != kilp2->piiri ||
		kilp1->joukkue != kilp2->joukkue ||
		kilp1->ilmlista != kilp2->ilmlista
		) {
		erot->perustiedot = true;
		ero = true;
		if (memcmp(kilp1->pisteet, kilp2->pisteet, sizeof(kilp1->pisteet)))
			erot->tulosvaik = true;
		}
	for (int os = 0; os < Sarjat[kilp1->sarja].osuusluku; os++) {
		if (strcmp(kilp1->ostiet[os].nimi, kilp2->ostiet[os].nimi)) {
			erot->os[os].ero |= 1;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].lisno != kilp2->ostiet[os].lisno) {
			erot->os[os].ero |= 2;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (strcmp(kilp1->ostiet[os].osseura, kilp2->ostiet[os].osseura)) {
			erot->os[os].ero |= 4;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (strcmp(kilp1->ostiet[os].arvo, kilp2->ostiet[os].arvo)) {
			erot->os[os].ero |= 8;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (strcmp(kilp1->ostiet[os].hajonta, kilp2->ostiet[os].hajonta)) {
			erot->os[os].ero |= 0x10;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].badge[0] != kilp2->ostiet[os].badge[0]) {
			erot->os[os].ero |= 0x20;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].badge[1] != kilp2->ostiet[os].badge[1]) {
			erot->os[os].ero |= 0x40;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].laina[0] != kilp2->ostiet[os].laina[0]) {
			erot->os[os].ero |= 0x80;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].laina[1] != kilp2->ostiet[os].laina[1]) {
			erot->os[os].ero |= 0x100;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (memcmp(kilp1->ostiet[os].selitys, kilp2->ostiet[os].selitys, sizeof(kilp2->ostiet[os].selitys))) {
			erot->os[os].ero |= 0x200;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].uusi != kilp2->ostiet[os].uusi) {
			erot->os[os].ero |= 0x400;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].seuranta != kilp2->ostiet[os].seuranta) {
			erot->os[os].ero |= 0x800;
			erot->os[os].perustiedot = true;
			ero = true;
			}
/*
		if (kilp1->ostiet[os].kone != kilp2->ostiet[os].kone) {
			erot->os[os].ero |= 0x1000;
			erot->os[os].perustiedot = true;
			ero = true;
			}
*/
		if (kilp1->ostiet[os].ossija != kilp2->ostiet[os].ossija) {
			erot->os[os].ero |= 0x2000;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].ylahto != kilp2->ostiet[os].ylahto) {
			erot->os[os].ero |= 0x4000;
			erot->os[os].tulosvaik = true;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->ostiet[os].lahtolaji != kilp2->ostiet[os].lahtolaji) {
			erot->os[os].ero |= 0x8000;
			erot->os[os].perustiedot = true;
			ero = true;
			}
		if (kilp1->Lahto(os,0) != kilp2->Lahto(os, 0)) {
			erot->os[os].lahto = true;
			ero = true;
			}
		if (kilp1->ostiet[os].keskhyl != kilp2->ostiet[os].keskhyl) {
			erot->os[os].ero |= 0x10000;
			erot->os[os].perustiedot = true;
			erot->os[os].tark = true;
			ero = true;
			if (kilp1->Hyv(os) && kilp2->Hyv(os) && kilp1->ostiet[os].keskhyl != kilp2->ostiet[os].keskhyl)
				erot->os[os].hyvmol = true;
			}
		if (memcmp(kilp1->ostiet[os].ampsakot, kilp2->ostiet[os].ampsakot, sizeof(kilp1->ostiet[0].ampsakot))) {
			erot->os[os].ero |= 0x20000;
			erot->os[os].perustiedot = true;
			erot->os[os].tark = true;
			erot->os[os].tulosvaik =
				memcmp(kilp1->ostiet[os].ampsakot, kilp2->ostiet[os].ampsakot, Sarjat[kilp1->sarja].paikat[os]) != 0;
			ero = true;
			}
		if (kilp1->ostiet[os].sakko != kilp2->ostiet[os].sakko) {
			erot->os[os].ero |= 0x40000;
			erot->os[os].perustiedot = true;
			erot->os[os].tark = true;
			erot->os[os].tulosvaik = true;
			ero = true;
			}
		if (kilp1->Tark(os, 1) != kilp2->Tark(os, 1)) {
			erot->os[os].tulosvaik = true;
			erot->os[os].ero |= 0x200000;
			ero = true;
			}
		for (int va = 0; va <= kilpparam.maxvaluku; va++) {
			if (kilp1->Maali(os, va) != kilp2->Maali(os, va)) {
				erot->os[os].aika[va] = true;
				erot->os[os].lahaika[va] = true;
				erot->os[os].ero |= 0x100000;
				ero = true;
				}
			}
		}
	return(ero);
}

void tallenna(kilptietue *ekilp, int d, int comtarfl, int kielto,
   int pakota_os, kilptietue *entkilp)
   {
   char keyn[NIMIINDL+2], line[82], kirjaika[20], st[80];
   kilptietue kilp;
   int os, entno, muuttjk = 0, yy;
   bool muutt = false;
   int lhyvfl[MAXOSUUSLUKU];
   int tulmuutt = 0, tcomfl0, lahTCP = 0;
   char ss[20];
   int peruuta[MAXOSUUSLUKU];
   erostruct erot;
   int va, p;
   int os1;
   INT32 ebadge[2];
   INT kysyfl = 0;
   int edyos;
#if !defined(LUENTA)
   emittp em;
   INT32 ep;
#endif

   if (d < 1 || d >= (int) datf2.numrec) {
	   parametrivirhe(L"tallenna", d, L"", L' ');
	   return;
	   }
   EnterCriticalSection(&tall_CriticalSection);
   kilpTarkasta(ekilp);
   if (kilpparam.alalaji == L'D') {
		ekilp->laskeKierrokset();
		}
   tcomfl0 = tcomfl;
   tcomfl = 0;
   if (mloki) {
	  aikatostr_ls(kirjaika, T_TIME(biostime(0,0)), t0);
	  kirjaika[8] = 0;
	  }
   kysyfl = !kielto;
   if (kielto == -1)
	  kielto = 0;
//   korjtarfl = FALSE;
   memset(lhyvfl,0,sizeof(lhyvfl));
   memset(peruuta, 0, sizeof(peruuta));
   GETREC(&kilp, d);
   if (!entkilp)
	  entkilp = &kilp;
   entno = kilp.kilpno;

   if (Sarjat[ekilp->sarja].ylkynnys[0] != TMAALI0) {
	  if (Sarjat[ekilp->sarja].maxnosuus > 1)
		  merk_yl(ekilp);
	  else {
		  yy = 0;
		  for (os = 1; os < Sarjat[ekilp->sarja].osuusluku; os++) {

			 if (!yy && ekilp->Maali(os-1, 0) != TMAALI0 &&
				NORMT(ekilp->Maali(os-1, 0)) >
				NORMT(Sarjat[ekilp->sarja].ylkynnys[os]))
				yy = 1;

				if (ekilp->ostiet[os].lahtolaji == 0) {
				 if (yy) {
						ekilp->ostiet[os].ylahto = Sarjat[ekilp->sarja].ylahto[os];
						}
					else {
						ekilp->ostiet[os].ylahto = TMAALI0;
						}
					}
			 }
		  }
		}

   //  Tarkastetaan osuuksittain, ovatko tiedot muuttuneet.
   // struktuuri erot kertoo löydetyt poikkeamat

   muutt = kilpcomp(ekilp, entkilp, &erot);

   if (pakota_os > 0) {
	   int pak_os = pakota_os / 10000 - 1;
	   int pak_va = (pakota_os % 10000) - 1;

	   if (pak_os == -1) {
		   erot.perustiedot = true;
		   muutt = true;
		   }
	   else if (pak_va < 0) {
			erot.os[pak_os].ero |= 0xffffffff;
			erot.os[pak_os].perustiedot = true;
			erot.os[pak_os].tark = true;
			muutt = true;
			}
	   else if (pak_os < Sarjat[ekilp->sarja].osuusluku &&
			pak_va <= Sarjat[ekilp->sarja].valuku[Sarjat[ekilp->sarja].yosuus[pak_os]]) {
			erot.os[pak_os].ero |= 0x100000;
			erot.os[pak_os].tulosvaik = true;
			muutt = true;
			erot.os[pak_os].aika[pak_va] = true;
			erot.os[pak_os].lahaika[pak_va] = true;
		   }
		}

#if defined(TCPSIIRTO)
	for (os = 0; os < Sarjat[ekilp->sarja].osuusluku; os++) {
		if (strcmp(kilp.ostiet[os].nimi, ekilp->ostiet[os].nimi) || erot.os[os].tulosvaik)
			lahTCP = 1;
		for (int va = 0; va <= kilpparam.maxvaluku; va++) {
			lahTCP = 1;
			break;
			}
#endif
		}
#if defined(TCPSIIRTO)
	if (erot.perustiedot || erot.tulosvaik)
		lahTCP = 1;
#endif

	if (muutt) {

		// Aseta flag lahaika pois, jos aika on viimeisin ajanoton aika eivätkä osuuden perustiedot ole muuttunnet

		if (!erot.os[viim_time.osuus].perustiedot && erot.os[viim_time.osuus].ero &&
			abs((int) NORMKELLO(purajak(viim_time.t) - ekilp->Maali(viim_time.osuus, viim_time.piste))) < kilpparam.pyor[1] &&
			purajak(viim_time.t) != TMAALI0 && viim_time.kno == ekilp->kilpno) {
			erot.os[viim_time.osuus].lahaika[viim_time.piste] = false;
//			ekilp->ostiet[viim_time.osuus].kone = viim_time.kone;
			}

		edyos = -1;
		for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {

		  if (erot.os[os].perustiedot || erot.os[os].aika[0]) {
			 if (kilp.Maali(os, 0) != TMAALI0 && kilp.Hyv(os)) {
				p = jalku[kilp.sarja][os][0];
				enst = entsija(d, &p, os, 0, 0, 0);
				enstls[kilp.sarja][os] = min(enstls[kilp.sarja][os],enst);
			   }
			}
			innpfl[os] = FALSE;
			for (va = 0; va <= kilpparam.maxvaluku; va++) {
				if (va == 0 || erot.os[os].tulosvaik || erot.os[os].aika[va]) {
					if (os == Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[os]]+1
						|| (va == 0 && edyos != Sarjat[kilp.sarja].yosuus[os]))
						remjarjarr(&kilp,d, Sarjat[kilp.sarja].yosuus[os],va,0);
					edyos = Sarjat[kilp.sarja].yosuus[os];
					}
			   }
			for (int ibdg = 0; ibdg < (kilpparam.kaksibadge ? 2 : 1); ibdg++) {
#if !defined(LUENTA)
				if (emitfl &&
					(kilp.ostiet[os].badge[ibdg] != ekilp->ostiet[os].badge[ibdg] ||
					kilp.kilpno != ekilp->kilpno))
					// Muutos Emit-koodeihin
					{
					if (kysyfl && !emittoisto && kilp.ostiet[os].badge[ibdg]) {
						for (os1 = 0; os1 < Sarjat[kilp.sarja].osuusluku; os1++) {
							if (os != os1 &&
								kilp.ostiet[os].badge[ibdg] == kilp.ostiet[os1].badge[ibdg])
								{
								writeerror("Sama Emit-koodi kahdella osuudella.", 0);
								}
							}
						}
					// Hae muutokseen liittyvä emit-tietue.
					// Tämä edellyttää, että emitjarr-linkkiä ei ole purettu aiemmin.
					if (emitfl > 0 && ibdg == 0)
						ep = getem(&em, kilp.kilpno, os);
					else
						ep = -1;
					if ((ebadge[ibdg] = kilp.ostiet[os].badge[ibdg]) != 0) {
						for (os1 = 0; os1 < Sarjat[ekilp->sarja].osuusluku; os1++) {
							if ((os != os1 && kilp.ostiet[os1].badge[ibdg] == kilp.ostiet[os].badge[ibdg]) ||
								(kilp.kilpno == ekilp->kilpno && ekilp->ostiet[os1].badge[ibdg] == kilp.ostiet[os].badge[ibdg]))
								break;
							}
						// Poista vanha koodi, jos se esiintyy vain yhdellä osuudella
						if (os1 == Sarjat[ekilp->sarja].osuusluku) {
							rembadge(kilp.kilpno, os, ibdg, d);

							if (ekilp->ostiet[os].badge[ibdg] == 0 && ep >= 0) {
								// Tee muutos myös emittietueeseen, jos on löydetty
								em.badge = 0;
								setEmitJarr(kilp.kilpno, os, -1);
								em.kilpno = 0;
								putem(&em, ep, 0);
								}

							}
						}
					if (ekilp->ostiet[os].badge[ibdg]) {
						// Lisää uusi koodi/kilpno taulukkoon
						if (addbadge(ekilp->ostiet[os].badge[ibdg], ekilp->kilpno, d, ibdg, kysyfl))
							{
							// Peruuta koodimuutos, jos taulukkoon lisääminen ei onnistu
							// Näin käy, jos koodi on jo toisella joukkueella
							ekilp->ostiet[os].badge[ibdg] = ebadge[ibdg];
							if (ebadge[ibdg])
								addbadge(ebadge[ibdg], kilp.kilpno, d, ibdg, kysyfl);
							if (kysyfl)
								writeerror("Muutos peruutettu", 0);
							}
						else if (ep >= 0)
							{
							// Taulukon päivitys onnistui. Tee muutos myös emittietueeseen, jos on löydetty
							em.badge = ekilp->ostiet[os].badge[0];
							if (kilp.kilpno != ekilp->kilpno) {
								// Päivitä myös linkki kilpno -> tietue, jos kilpailijan numero vaihtuu
								setEmitJarr(kilp.kilpno, os, -1);
								setEmitJarr(ekilp->kilpno, os, ep);
								em.kilpno = ekilp->kilpno;
								}
							putem(&em, ep, 0);
							}
						}
					}
			   if (sisaanluenta &&
				  (kilp.ostiet[os].badge[ibdg] != ekilp->ostiet[os].badge[ibdg] ||
				  kilp.kilpno != ekilp->kilpno)) {
				  if ((ebadge[ibdg] = kilp.ostiet[os].badge[ibdg]) != 0) {
					 for (os1 = 0; os1 < Sarjat[ekilp->sarja].osuusluku; os1++) {
						if (ekilp->ostiet[os1].badge[ibdg] == kilp.ostiet[os].badge[ibdg])
						   break;
						}
					 if (os1 == Sarjat[ekilp->sarja].osuusluku)
						rembadge(kilp.kilpno, os, ibdg, d);
					 }
				  if (ekilp->ostiet[os].badge[ibdg]) {
					 if (addbadge(ekilp->ostiet[os].badge[ibdg],
						ekilp->kilpno, d, ibdg, kysyfl)) {
						ekilp->ostiet[os].badge[ibdg] = ebadge[ibdg];
						addbadge(ebadge[ibdg], kilp.kilpno, d, ibdg,  kysyfl);
						if (kysyfl) writeerror("Muutos peruutettu", 0);
						}
					 }
				  }
#else
			   if (emitfl &&
				  (kilp.ostiet[os].badge[ibdg] != ekilp->ostiet[os].badge[ibdg] ||
				  kilp.kilpno != ekilp->kilpno)) {
				  if ((ebadge[ibdg] = kilp.ostiet[os].badge[ibdg]) != 0) {
					 for (os1 = 0; os1 < Sarjat[ekilp->sarja].osuusluku; os1++) {
						if (ekilp->ostiet[os1].badge[ibdg] == kilp.ostiet[os].badge[ibdg])
						   break;
						}
					 if (os1 == Sarjat[ekilp->sarja].osuusluku)
						rembadge(kilp.kilpno, os, ibdg, d);
					 }
				  if (ekilp->ostiet[os].badge[ibdg]) {
					 if (addbadge(ekilp->ostiet[os].badge[ibdg],
						ekilp->kilpno, d, 0, kysyfl)) {
						ekilp->ostiet[os].badge[ibdg] = ebadge[ibdg];
						addbadge(ebadge[ibdg], kilp.kilpno, d, ibdg, kysyfl);
						if (kysyfl) writeerror("Muutos peruutettu", 0);
						}
					 }
				  }
#endif
			   }
			}
		  if (strcmp(kilp.seura, ekilp->seura) || kilp.joukkue != ekilp->joukkue) {
			 memcpy(keyn, keyname(&kilp), NIMIINDL);
			 delix(&nameindex,keyn,d);
			 memcpy(keyn, keyname(ekilp), NIMIINDL);
			 addix(&nameindex,keyn, d);
			 }
		  kilp = *ekilp;
		  PUTREC(&kilp,d);
		  putrec_i(&datf2,d,&kilp);
		  if (kilp.sarja != ekilp->sarja) {
			  paivJoukkLkm(ekilp->sarja, -1);
			  paivJoukkLkm(kilp.sarja, +1);
			  }
		  if (entno != kilp.kilpno) {
			 setpos(kilp.kilpno, d);
			 setpos(entno, 0);
			 }
		  edyos = -1;
		  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				for (va = 0; va <= kilpparam.maxvaluku; va++) {
					if (va == 0 || erot.os[os].tulosvaik || erot.os[os].aika[va]) {
						if (os == Sarjat[kilp.sarja].aosuus[Sarjat[kilp.sarja].yosuus[os]]+1
							|| (va == 0 && edyos != Sarjat[kilp.sarja].yosuus[os]))
							addjarjarr(&kilp,d, Sarjat[kilp.sarja].yosuus[os],va,0);
						edyos = Sarjat[kilp.sarja].yosuus[os];
						}
				   }
			 }

#ifdef _CONSOLE
			if (comfl)
#endif
				{
				UINT32 maskTCP;
				maskTCP = lahTCP ? 0x0010 : 0;
				UINT32 luokka = 0;

				if (erot.perustiedot) {
					if (comfl)
						laheta(d,entno, 0, 0, comtarfl && lahtarfl, kielto, maskTCP, 0);
					entno = kilp.kilpno;
					}
				for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {

					if (erot.os[os].ero != 0) {

						bool addtap = false;

					// luokka ==  1,   tietoja ei lähetetä koneelle, jolle
					//                   lähetään aikataulukko; muuten vain aika
					// luokka == 2,     lähetään aika
					// luokka & 4,     lähetään muut osuustiedot, koodit sisältää sisältömaskin

						if (erot.os[os].perustiedot) {
							luokka = 4 + 16 * erot.os[os].ero;
							if (comfl)
								laheta(d,entno, os+1, 0, (erot.os[os].hyvmol ? 0 : (int)maskTCP) | (comtarfl && lahtarfl), kielto, luokka, pakota_os/10000 == os+1);
							if (erot.os[os].tark) {
								if (!erot.os[os].hyvmol || tarktapahtumat) {
									addtapahtuma(&kilp, os, 0);
									addtap = true;
									}
								}
							}

						for (int va = 0; va <= kilpparam.maxvaluku; va++) {
							if (erot.os[os].aika[va]) {
								luokka = erot.os[os].lahaika[va] ? 2 : 1;
								if (comfl)
									laheta(d,entno, os+1, va, (int)maskTCP | (comtarfl && lahtarfl), kielto, luokka, pakota_os == 10000*(os+1)+va+1);
								if (va || !addtap)
									addtapahtuma(&kilp, os, va);
								}
							}
						}
#ifndef LUENTA
					if (emitfl && emitvafl && erot.os[os].lahto)
						laheta_emva(&kilp, os);
#endif
					}
				}  // comfl
		  sjfl[kilp.sarja] = TRUE;
		  lukumaarat();
		  paivita_aikanaytto();
		  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			 if(automaali[Sarjat[kilp.sarja].mno-1] && (erot.os[os].aika[0] || (erot.os[os].tark && !erot.os[os].hyvmol)) &&
				(laajuus != 'P' || (kilp.Sija(os, 0) <= nparas &&
				kilp.Sija(os, 0) > 0) || innpfl[os])) {
				fprior[kilp.sarja][Sarjat[kilp.sarja].yosuus[os]]++;
				prior[kilp.sarja][Sarjat[kilp.sarja].yosuus[os]] += prior0[kilp.sarja];
				if ((enst = kilp.Sija(os, 0)) == 0)
				   enst = ntulosix[kilp.sarja][os][0] + 1;
				enstls[kilp.sarja][os] = min(enstls[kilp.sarja][os],enst);
				}
			 }
	   if (loki) {
		  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			 if (erot.os[os].ero) {
				if (kilp.Maali(os, 0) != TMAALI0)
				   AIKATOSTRS(keyn, kilp.Maali(os, 0), t0);
				else strcpy(keyn, "          ");
				keyn[kilpparam.laika] = 0;
				AIKATOSTRS(ss, tulos(&kilp,os,0),0);
				ss[kilpparam.laika] = 0;
				sprintf(line, "   %10.10s %4d %2d. %-20.20s %2d "
				   "%-10.10s %10s %c %u\r\n",keyn,kilp.kilpno,os+1,kilp.Seura(st),
				   (int)kilp.joukkue,(char *)Sarjat[kilp.sarja].sarjanimi,ss,
				   kilp.Tark(os, 0),kilp.ostiet[os].kone);
				kirjloki(line);
				}
			 }
		  }
	   tcomfl = tcomfl0;
	   }
   LeaveCriticalSection(&tall_CriticalSection);
   }

void addtall(kilptietue *kilp, int *dataf, int kielto)
   {
   int  i,os, kno = 0;
   char keyn[NIMIINDL+2];
   kilptietue tkilp;
#if defined(EMIT) && defined(MAALI)
   char msg[50];
#endif

   EnterCriticalSection(&tall_CriticalSection);
//   siirra_tila(kilp->sarja, sarjaluku-1);
   i = filelen(&datf2);
#if defined(EMIT) && defined(MAALI)
   if (emitfl || sisaanluenta) {
	  for (os = 0; os<Sarjat[kilp->sarja].osuusluku; os++) {
		 for (int ibdg = 0; ibdg < (kilpparam.kaksibadge ? 2 : 1); ibdg++) {
			 if (kilp->ostiet[os].badge[ibdg]) {
				if (addbadge(kilp->ostiet[os].badge[ibdg],
				   kilp->kilpno, *dataf, ibdg, 1)) {
				   sprintf(msg,"Emit-koodi %ld jo käytössä. Koodia ei tallennettu",
					  kilp->ostiet[os].badge[ibdg]);
				   kilp->ostiet[os].badge[ibdg] = 0;
				   writeerror(msg, 0);
				   }
				}
			}
		 }
	  }
#endif
   tkilp.nollaa();
   memcpy(&tkilp, kilp, kilpparam.kilprecsize0);
   addfrec(&tkilp,dataf);
   setpos(tkilp.kilpno, *dataf);
   memcpy(keyn, keyname(&tkilp), NIMIINDL);
   addix(&nameindex, keyn, *dataf);
   paivJoukkLkm(tkilp.sarja, +1);
   for (os = 0; os < Sarjat[tkilp.sarja].ntosuus; os++)
	  addjarjarr(&tkilp,*dataf,os,0,0);
   if (comfl)
	  laheta(*dataf, 0, 0, 0, FALSE, kielto, 8, 0);
   tallenna(kilp, *dataf, 0, kielto, 0, 0);
   LeaveCriticalSection(&tall_CriticalSection);
   }

void deltall(int d, int kielto)
   {
   int osuus;
   char keyn[NIMIINDL+2];
   kilptietue kilp;

   EnterCriticalSection(&tall_CriticalSection);
   GETREC(&kilp, d);
#if defined(EMIT) && defined(MAALI)
   if (emitfl || sisaanluenta) {
      for (osuus = 0; osuus < Sarjat[kilp.sarja].osuusluku; osuus++) {
		 if (kilp.ostiet[osuus].badge[0]) {
            rembadge(kilp.kilpno, osuus, 0, d);
            }
		 if (kilpparam.kaksibadge && kilp.ostiet[osuus].badge[1]) {
            rembadge(kilp.kilpno, osuus, 1, d);
            }
         }
      }
#endif
   paivJoukkLkm(kilp.sarja, -1);
   for (osuus = 0; osuus < Sarjat[kilp.sarja].ntosuus; osuus++)
	   for (int va = 0; va <= kilpparam.maxvaluku; va++)
		  remjarjarr(&kilp,d, osuus, va, 0);
//   siirra_tila(sarjaluku-1, kilp.sarja);
   memcpy(keyn, keyname(&kilp), NIMIINDL);
   delix(&nameindex, keyn, d);
   setpos(kilp.kilpno, 0);
   delfrec(d);
   if (comfl) laheta(d, kilp.kilpno, 0, 0, FALSE, kielto, 8, 0);
   lukumaarat();
   LeaveCriticalSection(&tall_CriticalSection);
   }

int kirjKilpDat(wchar_t *fname)
{
	datafile datf_tmp;
	char cst[202];
	kilptietue kilp;
	UINT d;

	makefile(&datf_tmp,wcstooem(cst, fname, 200),kilpparam.kilprecsize);
	for (int i = 1; (UINT32)i < datf2.numrec; i++) {
		kilp.getrec(i);
		addrec(&datf_tmp, &d, &kilp);
		}
	closefile(&datf_tmp);
	return(0);
}

void TallEhdoin(kilptietue *kilp, int pos)
{
	int d;

	addtall(kilp, &d, 0);
}
//---------------------------------------------------------------------------
void lahetaKnoSiirto(int sarja, int siirto, int kielto);

void tallKnoSiirto(int sarja, int siirto, int kielto)
{
	Sarjat[sarja].knosiirto = siirto;
	lahetaKnoSiirto(sarja, siirto, kielto);
}


