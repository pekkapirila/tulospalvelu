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
#ifdef _BORLAND_
#include "UnitMySQL.h"
#endif

//#include "HkDef.h"

buf_fields va_fields[] =
	{
	{L"vatulos", KNT_INT32, 4, 1, 0},
	{L"vasija", KNT_INT16, 2, 1, 4},
	{L"vakoodi", KNT_INT16, 2, 1, 6},
	{L"", KNT_SIZE, 0, 0, 8}
	};

buf_fields pv_fields[] =
	{
	{L"txt", KNT_WTEXT, 2, 10, 0},
	{L"uusi", KNT_CHAR, 1, 1, 20},
	{L"vac", KNT_CHAR, 1, 1, 21},
	{L"flags", KNT_INT32, 4, 2, 22},
	{L"tav", KNT_INT32, 4, 1, 30},
	{L"enn", KNT_INT32, 4, 1, 34},
	{L"rata", KNT_WTEXT, 2, HAJONTA+1, 38},
	{L"sarja", KNT_INT16, 2, 1, 62},
	{L"era", KNT_INT16, 2, 1, 64},
	{L"bib", KNT_INT16, 2, 1, 66},
	{L"badge", KNT_INT32, 4, 2, 68},
	{L"laina", KNT_WCHAR, 2, 2, 76},
	{L"selitys", KNT_WTEXT, 2, 18, 80},
	{L"pvpisteet", KNT_INT32, 4, 2, 116},
	{L"tlahto_arv", KNT_INT32, 4, 1, 124},
	{L"keskhyl", KNT_WCHAR, 2, 1, 128},
	{L"ampsakot", KNT_WTEXT, 2, 6, 130},
	{L"tasoitus", KNT_INT32, 4, 1, 142},
	{L"sakko", KNT_INT32, 4, 1, 146},
	{L"ysija", KNT_INT16, 2, 1, 150},
	{L"va", KNT_VA, 0, 0, 154},
	{L"", KNT_SIZE, 0, 0, 154}
	};

buf_fields kilp_fields[] =
	{
	{L"kilpstatus", KNT_INT16, 2, 1, 0},
	{L"kilpno", KNT_INT16, 2, 1, 2},
	{L"lisno", KNT_INT32, 4, 2, 4},
	{L"wrkoodi", KNT_WTEXT, 2, 10, 12},
	{L"ilmlista", KNT_INT16, 2, 1, 32},
	{L"piiri", KNT_INT16, 2, 1, 34},
	{L"piste", KNT_INT32, 4, 3, 36},
	{L"sukunimi", KNT_WTEXT, 2, 25, 48},
	{L"etunimi", KNT_WTEXT, 2, 25, 98},
	{L"arvo", KNT_WTEXT, 2, 16, 148},
	{L"seura", KNT_WTEXT, 2, 32, 180},
	{L"seuralyh", KNT_WTEXT, 2, 16, 244},
	{L"yhdistys", KNT_WTEXT, 2, 16, 276},
	{L"joukkue", KNT_WTEXT, 2, 16, 308},
	{L"maa", KNT_WTEXT, 2, 4, 340},
	{L"sarja", KNT_INT16, 2, 1, 348},
	{L"sukup", KNT_INT16, 2, 1, 350},
	{L"ikasarja", KNT_INT16, 2, 1, 352},
	{L"alisarja", KNT_INT16, 2, 1, 354},
	{L"synt", KNT_INT16, 2, 1, 356},
	{L"arvryhma", KNT_INT16, 2, 1, 358},
	{L"pv", KNT_VA, 0, 0, 360},
	{L"", KNT_SIZE, 0, 0, 360}
	};

int luekentat(int mista)
	{
	int x, n_knt;
	buf_fields *fld;

	if (mista != 99999) {
		 kilpparam.lsnimi = LSNIMI;
		 kilpparam.lenimi = LENIMI;
		 kilpparam.lseura = LSEURA;
		 kilpparam.lseuralyh = LLYH;
		 kilpparam.lyhdistys = LYHD;
		 kilpparam.ljoukkue = LJK;
		 kilpparam.larvo = LARVO;
		 kilpparam.lwrkoodi = LWRKOODI;

		n_knt = sizeof(va_fields)/sizeof(buf_fields);
		x = 0;
		fld = va_fields;
		for (int i = 0; i < n_knt; i++, fld++) {
			fld->bufpos = x;
			x += fld->size*fld->count;
			}
		kilpparam.vatpsize = x;

		n_knt = sizeof(pv_fields)/sizeof(buf_fields)-1;
		x = 0;
		fld = pv_fields;
		for (int i = 0; i < n_knt; i++, fld++) {
			fld->bufpos = x;
			x += fld->size*fld->count;
			}
		kilpparam.kilppvtpsize0 = x;
		kilpparam.kilppvtpsize = x + (kilpparam.valuku+2)*kilpparam.vatpsize;

		n_knt = sizeof(kilp_fields)/sizeof(buf_fields)-1;
		x = 0;
		fld = kilp_fields;
		for (int i = 0; i < n_knt; i++, fld++) {
			fld->bufpos = x;
			if (!wcscmp(fld->name, L"sukunimi"))
				fld->count = kilpparam.lsnimi+1;
			if (!wcscmp(fld->name, L"etunimi"))
				fld->count = kilpparam.lenimi+1;
			if (!wcscmp(fld->name, L"seura")) {
				if (kilpparam.lseura)
					fld->count = kilpparam.lseura+1;
				else
					fld->count = 0;
				}
			if (!wcscmp(fld->name, L"seuralyh"))
				fld->count = kilpparam.lseuralyh+1;
			if (!wcscmp(fld->name, L"yhdistys")) {
				if (kilpparam.lyhdistys > 0)
					fld->count = kilpparam.lyhdistys+1;
				else
					fld->count = 0;
				}
			if (!wcscmp(fld->name, L"joukkue")) {
				if (kilpparam.ljoukkue > 0)
					fld->count = kilpparam.ljoukkue+1;
				else
					fld->count = 0;
				}
			if (!wcscmp(fld->name, L"arvo")) {
				if (kilpparam.larvo > 0)
					fld->count = kilpparam.larvo+1;
				else
					fld->count = 0;
				}
			x += fld->size*fld->count;
			}
		kilpparam.kilprecsize0 = x;
		kilpparam.kilprecsize = x + kilpparam.n_pv*kilpparam.kilppvtpsize;
		}
	return(0);
	}

__fastcall vatp::vatp(void)
{
//	nollaa();
}


__fastcall vatp::~vatp(void)
{

}

__fastcall kilppvtp::kilppvtp(void)
{
	va = new vatp[kilpparam.valuku+2];
//	nollaa();
}

__fastcall kilppvtp::~kilppvtp(void)
{
	delete[] va;
}

__fastcall kilptietue::kilptietue(void)
{
	pv = new kilppvtp[kilpparam.n_pv];

	nollaa();
}

__fastcall kilptietue::~kilptietue(void)
{
	delete[] pv;
}

void vatp::pack(char *buf)
{
	int n_knt;
	buf_fields *fld;

	memset(buf, 0, kilpparam.vatpsize);
	n_knt = sizeof(va_fields)/sizeof(buf_fields);
	fld = va_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"vatulos"))
			memcpy(buf + fld->bufpos, (char *) &vatulos, fld->size);
		if (fld->size && !wcscmp(fld->name, L"vasija"))
			memcpy(buf + fld->bufpos, (char *) &vasija, fld->size);
		if (fld->size && !wcscmp(fld->name, L"vakoodi"))
			memcpy(buf + fld->bufpos, (char *) &vakoodi, fld->size);
		}
}

void vatp::unpack(char *buf)
{
	int n_knt;
	buf_fields *fld;

	n_knt = sizeof(va_fields)/sizeof(buf_fields);
	fld = va_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"vatulos"))
			memcpy((char *) &vatulos, buf + fld->bufpos, fld->size);
		if (fld->size && !wcscmp(fld->name, L"vasija"))
			memcpy((char *) &vasija, buf + fld->bufpos, fld->size);
		if (fld->size && !wcscmp(fld->name, L"vakoodi"))
			memcpy((char *) &vakoodi, buf + fld->bufpos, fld->size);
		}
}

void kilppvtp::pack0(char *buf)
{
	int n_knt;
	buf_fields *fld;

	memset(buf, 0, kilpparam.kilppvtpsize0);
	n_knt = sizeof(pv_fields)/sizeof(buf_fields);
	fld = pv_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"txt"))
			memcpy(buf + fld->bufpos, (char *) txt, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tav"))
			memcpy(buf + fld->bufpos, (char *) &tav, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"enn"))
			memcpy(buf + fld->bufpos, (char *) &enn, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"uusi"))
			memcpy(buf + fld->bufpos, (char *) &uusi, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"vac"))
			memcpy(buf + fld->bufpos, (char *) &vac, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"flags"))
			memcpy(buf + fld->bufpos, (char *) flags, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"rata"))
			memcpy(buf + fld->bufpos, (char *) rata, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sarja"))
			memcpy(buf + fld->bufpos, (char *) &sarja, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"era"))
			memcpy(buf + fld->bufpos, (char *) &era, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"bib"))
			memcpy(buf + fld->bufpos, (char *) &bib, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"badge"))
			memcpy(buf + fld->bufpos, (char *) badge, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"laina"))
			memcpy(buf + fld->bufpos, (char *) laina, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"selitys"))
			memcpy(buf + fld->bufpos, (char *) selitys, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"pvpisteet"))
			memcpy(buf + fld->bufpos, (char *) pvpisteet, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tlahto_arv"))
			memcpy(buf + fld->bufpos, (char *) &tlahto, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"keskhyl"))
			memcpy(buf + fld->bufpos, (char *) &keskhyl, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ampsakot"))
			memcpy(buf + fld->bufpos, (char *) asakot, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tasoitus"))
			memcpy(buf + fld->bufpos, (char *) &tasoitus, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sakko"))
			memcpy(buf + fld->bufpos, (char *) &sakko, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ysija"))
			memcpy(buf + fld->bufpos, (char *) &ysija, fld->size*fld->count);
		}
}

void kilppvtp::pack(char *buf)
{
	pack0(buf);
	for (int i = 0; i < kilpparam.valuku+2; i++) {
		va[i].pack(buf+kilpparam.kilppvtpsize0+i*kilpparam.vatpsize);
	}
}

void kilppvtp::unpack0(char *buf)
{
	int n_knt;
	buf_fields *fld;

	n_knt = sizeof(pv_fields)/sizeof(buf_fields);
	fld = pv_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"txt"))
			memcpy((char *) txt, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tav"))
			memcpy((char *) &tav, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"enn"))
			memcpy((char *) &enn, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"uusi"))
			memcpy((char *) &uusi, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"vac"))
			memcpy((char *) &vac, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"flags"))
			memcpy((char *) &flags, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"rata"))
			memcpy((char *) rata, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sarja"))
			memcpy((char *) &sarja, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"era"))
			memcpy((char *) &era, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"bib"))
			memcpy((char *) &bib, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"badge"))
			memcpy((char *) badge, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"laina"))
			memcpy((char *) laina, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"selitys"))
			memcpy((char *) selitys, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"pvpisteet"))
			memcpy((char *) pvpisteet, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tlahto_arv"))
			memcpy((char *) &tlahto, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"keskhyl"))
			memcpy((char *) &keskhyl, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ampsakot"))
			memcpy((char *) asakot, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"tasoitus"))
			memcpy((char *) &tasoitus, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sakko"))
			memcpy((char *) &sakko, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ysija"))
			memcpy((char *) &ysija, buf + fld->bufpos, fld->size*fld->count);
		}
}

void kilppvtp::unpack(char *buf)
{
	unpack0(buf);
	for (int i = 0; i < kilpparam.valuku+2; i++) {
		va[i].unpack(buf+kilpparam.kilppvtpsize0+i*kilpparam.vatpsize);
	}
}

void kilptietue::pack0(char *buf)
{
	int n_knt;
	buf_fields *fld;

	memset(buf, 0, kilpparam.kilprecsize0);
	n_knt = sizeof(kilp_fields)/sizeof(buf_fields);
	fld = kilp_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"kilpstatus"))
			memcpy(buf + fld->bufpos, (char *) &kilpstatus, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"kilpno"))
			memcpy(buf + fld->bufpos, (char *) &kilpno, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"lisno"))
			memcpy(buf + fld->bufpos, (char *) lisno, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"wrkoodi"))
			memcpy(buf + fld->bufpos, (char *) wrkoodi, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ilmlista"))
			memcpy(buf + fld->bufpos, (char *) &ilmlista, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"piiri"))
			memcpy(buf + fld->bufpos, (char *) &piiri, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"piste"))
			memcpy(buf + fld->bufpos, (char *) pisteet, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sukunimi"))
			memcpy(buf + fld->bufpos, (char *) sukunimi, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"etunimi"))
			memcpy(buf + fld->bufpos, (char *) etunimi, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"arvo"))
			memcpy(buf + fld->bufpos, (char *) arvo, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"seura"))
			memcpy(buf + fld->bufpos, (char *) seura, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"seuralyh"))
			memcpy(buf + fld->bufpos, (char *) seuralyh, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"yhdistys"))
			memcpy(buf + fld->bufpos, (char *) yhdistys, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"joukkue"))
			memcpy(buf + fld->bufpos, (char *) joukkue, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"maa"))
			memcpy(buf + fld->bufpos, (char *) maa, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sarja"))
			memcpy(buf + fld->bufpos, (char *) &sarja, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sukup"))
			memcpy(buf + fld->bufpos, (char *) &sukup, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ikasarja"))
			memcpy(buf + fld->bufpos, (char *) &ikasarja, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"alisarja"))
			memcpy(buf + fld->bufpos, (char *) &alisarja, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"synt"))
			memcpy(buf + fld->bufpos, (char *) &synt, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"arvryhma"))
			memcpy(buf + fld->bufpos, (char *) &arvryhma, fld->size*fld->count);
		}
}

void kilptietue::pack(char *buf)
{
	pack0(buf);
	for (int i = 0; i < kilpparam.n_pv; i++) {
		pv[i].pack(buf+kilpparam.kilprecsize0+i*kilpparam.kilppvtpsize);
		}
}

void kilptietue::unpack0(char *buf)
{
	int n_knt;
	buf_fields *fld;

	n_knt = sizeof(kilp_fields)/sizeof(buf_fields);
	fld = kilp_fields;
	for (int i = 0; i < n_knt-1; i++, fld++) {
		if (fld->size && !wcscmp(fld->name, L"kilpstatus"))
			memcpy((char *) &kilpstatus, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"kilpno"))
			memcpy((char *) &kilpno, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"lisno"))
			memcpy((char *) lisno, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"wrkoodi"))
			memcpy((char *) wrkoodi, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ilmlista"))
			memcpy((char *) &ilmlista, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"piiri"))
			memcpy((char *) &piiri, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"piste"))
			memcpy((char *) pisteet, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sukunimi"))
			memcpy((char *) sukunimi, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"etunimi"))
			memcpy((char *) etunimi, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"arvo"))
			memcpy((char *) arvo, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"seura"))
			memcpy((char *) seura, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"seuralyh"))
			memcpy((char *) seuralyh, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"yhdistys"))
			memcpy((char *) yhdistys, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"joukkue"))
			memcpy((char *) joukkue, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"maa"))
			memcpy((char *) maa, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sarja"))
			memcpy((char *) &sarja, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"sukup"))
			memcpy((char *) &sukup, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"ikasarja"))
			memcpy((char *) &ikasarja, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"alisarja"))
			memcpy((char *) &alisarja, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"synt"))
			memcpy((char *) &synt, buf + fld->bufpos, fld->size*fld->count);
		if (fld->size && !wcscmp(fld->name, L"arvryhma"))
			memcpy((char *) &arvryhma, buf + fld->bufpos, fld->size*fld->count);
		}
}

void kilptietue::unpack(char *buf)
{
	nollaa();
	unpack0(buf);
	for (int i = 0; i < kilpparam.n_pv; i++) {
		pv[i].unpack(buf+kilpparam.kilprecsize0+i*kilpparam.kilppvtpsize);
		}
}


void vatp::nollaa(void)
{
	vatulos = 0;
	vasija = 0;
	vakoodi = 0;
}

#ifdef MAKI
void makitp::nollaa(void)
{
	memset(vuoro, 0, sizeof(makitp));
}
#endif // MAKI

void kilppvtp::nollaa(void)
{
	memset(txt, 0, (char *) &va - (char *) txt);
	keskhyl = L'-';
	tlahto = TMAALI0;
//	tlahto1 = TMAALI0;
	for (int i = 0; i < kilpparam.valuku+2; i++)
		va[i].nollaa();
	va[0].vatulos = TMAALI0;
#ifdef MAKIxx
	maki.nollaa();
#endif // MAKI
}

void kilptietue::nollaa(void)
{
	memset(&kilpstatus, 0, (char *) &pv - (char *) &kilpstatus);
	for (int i = 0; i < kilpparam.n_pv; i++) {
		pv[i].nollaa();
		}
}

wchar_t *kilptietue::nimi(wchar_t *buf, int len, bool vaihda)
{
	int elen;

	wmemset(buf, 0, len);
	if (pv[0].salaa == 'Y') {
		wcsncpy(buf, yhdistys, len-2);
		}
	else if (pv[0].salaa == 'J') {
		wcsncpy(buf, joukkue, len-2);
		}
	else if (!vaihda) {
		wcsncpy(buf, sukunimi, len-2);
		wcscat(buf, L" ");
		wcsncpy(buf+wcslen(buf), etunimi, len-wcslen(buf)-1);
		}
	else {
		elen = len - wcslen(sukunimi) - 3;
		if (elen >= (int) wcslen(etunimi)) {
			wcscpy(buf, etunimi);
			wcscat(buf, L" ");
			}
		else if (elen > 0) {
			wcsncpy(buf, etunimi, elen-1);
			buf[elen-1] = 0;
			wcscat(buf, L". ");
			}
		wcsncpy(buf+wcslen(buf), sukunimi, len-wcslen(buf)-1);
		}
	return(buf);
}

int kilptietue::read(int d, int mista)
	{
	char buf[KILPBUFSIZE];

	if (mista == 0) {
		EnterCriticalSection(&kilpdat_CriticalSection);
		getrec(&datf2,d,buf);
		LeaveCriticalSection(&kilpdat_CriticalSection);
		if (*(INT16 *)buf == 0) {
			unpack(buf);
			return(0);
			}
		else
			return(1);
		}
	return(0);
	}

int kilptietue::write(int d, int mihin)
	{
	char buf[KILPBUFSIZE];

	if (mihin == 0) {
		pack(buf);
		EnterCriticalSection(&kilpdat_CriticalSection);
		putrec(&datf2,d,buf);
		LeaveCriticalSection(&kilpdat_CriticalSection);
		}
	if (mihin == 1) {
		kirjKilpSQL(d);
		}
	return(0);
	}

void kilptietue::GETREC(INT d)
	{
	if (d < 0 || d > maxrec) {
		writeerror_w(L"Virhe: Ohjelma yritt‰‰ hakea olematonta kilpailijaa", 0);
		return;
		}
	this->nollaa();
	EnterCriticalSection(&kilpdat_CriticalSection);
	*this = kilparr[d];
	LeaveCriticalSection(&kilpdat_CriticalSection);
	}

void kilptietue::PUTREC(INT d, int tallenna)
	{
	if (d > maxrec) {
		writeerror_w(L"Yritys tallentaa liikaa klpailijoita", 2000);
		return;
		}
	EnterCriticalSection(&kilpdat_CriticalSection);
	kilparr[d] = *this;
	if (tallenna) {
		for (int i = 0; i < 2; ++i) {
		 if (datatallfl[i])
				this->write(d, i);
			}
		}
	LeaveCriticalSection(&kilpdat_CriticalSection);
	}

void kilptietue::addfrec(int *d)
	{
//   kilptietue akilp;
	char buf[KILPBUFSIZE];

	pack(buf);
	EnterCriticalSection(&kilpdat_CriticalSection);
	addrec(&datf2,(UINT *)d,buf);
	nrec = datf2.numrec;
	LeaveCriticalSection(&kilpdat_CriticalSection);
	PUTREC(*d, 0);
	}


void delfrec(INT d)
	{
	kilptietue kilp;

	EnterCriticalSection(&kilpdat_CriticalSection);
	kilp.GETREC(d);
	deleterec(&datf2, d);
	kilp.kilpstatus = datf2.firstfree;
	kilp.PUTREC(d, 0);
	LeaveCriticalSection(&kilpdat_CriticalSection);
//	kfirstfree=d;
	}

INT n_tarkastamatta(INT srj)
	{
	int n = 0, p = -1, lj = -1;
	
	while (lj <= 0) {
		kilptietue kilp;
		p = askellasarja(srj, 0, p, &lj);
		if (p < 0 || lj > 0)
			break;
		kilp.GETREC(p);
		if (wcswcind(kilp.tark(), L"-I") >= 0)
			n++;
		}
	return(n);
	}

INT n_avoin(INT srj)
	{
	return(nilm[srj]-ntulos[srj][0]-nhyl[srj]-nkesk[srj]-neil[srj]);
	}

void kilptietue::addtall(INT *dataf, INT kielto)
	{
	INT  i;
	char keyn[NIMIINDL+1];
	int keskeyta = 0;

//	EnterCriticalSection(&tall_CriticalSection);
	if (nrec >= maxrec) {
		writeerror_w(L"Yritys lis‰t‰ liikaa kilpailijoita", 0);
		return;
		}
	if (!kilpparam.sarjavaihtelee)
		setSarjaAll(Sarja());
	else {
		if (sarja < 0 || sarja >= sarjaluku)
			sarja = Sarja();
		for (i = 0; i < kilpparam.n_pv_akt; i++) {
			if (i == k_pv)
				continue;
			if (Sarja(i) < 0 || Sarja(i) >= sarjaluku)
				setSarja(Sarja(), i);
			}
		}
	if (id() > 0) {
		if (!kilpparam.bibvaihtelee)
			setIdAll(id());
		else {
			if (kilpno < minkilpno || kilpno > maxkilpno) {
				kilpno = haeSeurVapaaKno(-1, id());
				}
			for (i = 0; i < kilpparam.n_pv_akt; i++) {
				if (i == k_pv)
					continue;
				if (id(i) < minkilpno || id(i) > maxkilpno)
					setId(haeSeurVapaaKno(i, id()), i);
				}
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Sarjat[Sarja(ipv)].lsak[ipv]; i++) {
			if (pv[ipv].asakot[i] == 0)
				pv[ipv].asakot[i] = L'-';
			}
		}
	addfrec(dataf);
	if (id() >= minkilpno && id() <= maxkilpno)
		setpos(id(), *dataf);
	keyaak(this, keyn, nameindex.keyl, 0);
//	keyfromwname(keyn, nimi(st,sizeof(st)/2,false), NIMIINDL);
	addix(&nameindex, keyn, *dataf);
	addjarjarr(this, *dataf, 0, &keskeyta);
	if (emitfl && Badge(k_pv, 0))
		addbadge(pv[k_pv].badge[0], id(), 0);
	if (kilpparam.kaksibadge && emitfl && Badge(k_pv, 1))
		addbadge(pv[k_pv].badge[1], id(), 0);
	if (comfl && !keskeyta) {
		laheta(*dataf, 0, 0, 0, FALSE, kielto, 0);
		if (!kielto) {
			for (i = 0; i < kilpparam.n_pv_akt; i++) {
				laheta(*dataf, kilpno, -1-i, -1, FALSE, kielto, 0);
				}
			}
		}
//	LeaveCriticalSection(&tall_CriticalSection);
	lukumaarat();
	}

void deltall(INT d, INT kielto)
	{
	char keyn[NIMIINDL+1];
	kilptietue kilp;
	INT piste;
	int keskeyta = 0;

//	EnterCriticalSection(&tall_CriticalSection);
	kilp.GETREC(d);
	if (emitfl) {
		if (kilp.pv[k_pv].badge[0])
			rembadge(kilp.pv[k_pv].badge[0], kilp.id(), 0);
		if (kilpparam.kaksibadge && kilp.pv[k_pv].badge[1])
			rembadge(kilp.pv[k_pv].badge[1], kilp.id(), 0);
		}
	for (piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
		if (piste == 0 || kilp.p_aika(piste))
			remjarjarr(&kilp, d, piste, &keskeyta);
	if (kilp.id() >= minkilpno && kilp.id() <= maxkilpno)
		setpos(kilp.id(), 0);
	keyaak(&kilp, keyn, nameindex.keyl, 0);
//	keyfromwname(keyn, kilp.nimi(st, sizeof(st)/2, false), NIMIINDL);
	delix(&nameindex, keyn, d);
	delfrec(d);
//	deleterec(d);
	if (comfl && !keskeyta) {
		laheta(d, kilp.kilpno, 0, 0, FALSE, kielto, 0);
		}
//	LeaveCriticalSection(&tall_CriticalSection);
	lukumaarat();
	}

int haeseurvapaa(int seurvapaa)
	{
	INT i;

	i = haeSeurVapaaKno(k_pv, seurvapaa);
	if (i == 0) {
		writeerror_w(L"Ei vapaita kilpailijanumeroita, lis‰ys mahdotonta", 0);
		}
	return(i);
	}

void lisaToimet(kilptietue *kilp, kilptietue *ekilp, int kcmp, int *m_pv, int *m_ajat)
{
	if (wcswcind(ekilp->tark(), L"VE") >= 0 && kilp->tark() == L'-') {
		sivuTuloste(kilp);
		}
}

//  kilpcmp vertaa tietueita *kilp ja *ekilp. Palautettava arvo on
//   0   jos tietueet identtiset
//   1   jos eroa vain tuloksiin vaikuttamattomissa tiedoissa
//   2   jos eroa vain p‰iv‰n ajoissa, KHE-tiedossa ja/tai sakkotiedoissa

//   3   jos eroa molemmissa edellisiss‰
//   >3  jos sarja tai aiemman p‰iv‰n tulos tai hylk‰ys eroaa
//	Taulukon m_pv alkio m_pv[0] = 1, kun perustiedot muuttuneet
//  Taulukon m_pv alkio m_pv[i_pv+1] = 1, kun p‰iv‰tiedoissa muu muutos
//  kuin erikseen l‰hetett‰v‰ p‰iv‰n tulostieto
//  Samalla asetetaan taulukon m_ajat kaikki ne alkiot arvoon 1 tai 2,
//  joita vastaa muuttunut aika (m_ajat[piste+1] = 1).
//  Jos KHE-tieto muuttuu on m_ajat[kilpparam.valuku+2] = 1
//  Jos ammunnan sakkotieto muuttuu on m_ajat[kilpparam.valuku+3] = 1
//  Jos muu sakkotieto muuttuu on m_ajat[kilpparam.valuku+4] = 1
//
//  m_ajat[i] == 2, jos halutaan n‰ytt‰‰ seurannassa.

static INT kilpcmp(kilptietue *kilp, kilptietue *ekilp, INT *m_ajat, int *m_pv)
	{
	INT i, ret = 0;

	if ((ret |= kilp->vert0(*ekilp)) != 0) {
		m_pv[0] = 1;
		ret |= 1;
		}

	for (i = 0; i < kilpparam.n_pv_akt; i++) {
		int r = 0;

		if (i != k_pv && !(kilp->pv[i] == ekilp->pv[i])) {
			m_pv[i+1] = 1;
			ret |= 1;
			if ((r = kilp->pv[i].vert0(ekilp->pv[i])) > 1)
				ret |= 4;
			}
		if (i == k_pv && (r = kilp->pv[i].vert0(ekilp->pv[i])) != 0) {
			if (r & 1) {
				m_pv[i+1] = 1;
				ret |= 1;
				}
			if (r != 1) {
				if (r & 2) {
					ret |= 3;
					m_pv[i+1] = 1;
					}
				if (r & 8) {
					ret |= 2;
					m_ajat[kilpparam.valuku+3] = 2;
					}
				if (r & 16) {
					ret |= 2;
					m_ajat[kilpparam.valuku+4] = 1;
					}
				}
			}
		}

	if (kilpparam.sarjavaihtelee) {
		for (int i_pv = (ToimintaTila == 1 ? -1 : k_pv); i_pv < kilpparam.n_pv_akt; i_pv++) {
			if (kilp->Sarja(i_pv) != ekilp->Sarja(i_pv)) {
				ret |= 4;
				break;
				}
			}
		}
	else {
		if (kilp->Sarja() != ekilp->Sarja()) {
			ret |= 4;
			}
		}

	for (i = -1; i <= Sarjat[kilp->Sarja()].valuku[k_pv]; i++) {
		if ((kilp->p_aika(i) != ekilp->p_aika(i)) ||
			(i > 0 &&
			 ((kilp->Sarja() != ekilp->Sarja() && kilp->p_aika(i)) ||
			 (kilp->hyv() != ekilp->hyv() && kilp->p_aika(i))))) {
			ret |= 2;
			m_ajat[i+1] = (i == -1) ? 1 : 2;
			}
		}
	if ((Sarjat[kilp->Sarja()].pistekaava[k_pv].yhtpiston || wcswcind(kilpparam.kilplaji, L"NP") >= 0)
		&& kilp->pisteet[0] != ekilp->pisteet[0]) {
		ret |= 2;
		m_ajat[1] = 2;
		}
	if (kilp->tark(k_pv) != ekilp->tark(k_pv)) {
		ret |= 2;
		if (wcswcind(ekilp->tark(k_pv), L"HKOXM") >= 0 ||
			wcswcind(kilp->tark(k_pv), L"HKOXM") >= 0)
			m_ajat[kilpparam.valuku+2] = 2;
		else
			m_ajat[kilpparam.valuku+2] = 1;
		}
	return(ret);
	}

// Metodi: tallenna
//
// 		d			tietueen sijainti
//      comtarfl	pyyt‰‰ tarran kirjoittamisen (vanhentunut)
//		kielto		kertoo yhteyden, mist‰ muutos on tullut ja jonne ei l‰hetet‰
//					kielto == 0 kertoo, ett‰ muutos on syntynyt t‰ss‰ instanssissa
//		pakota_lah	pyyt‰‰ edelleenl‰hett‰m‰‰n myˆs muuttumattomat tiedot verkon p‰ivitt‰miseksi
//		entkilp		tietue, johon verrataaan muutoksia haettessa, jos NULL verrataan tallennettuun tietoon

void kilptietue::tallenna(INT d, INT comtarfl, INT kielto, INT pakota_lah, kilptietue *entkilp)
	{
	char keyn[80];
	wchar_t line[102], st[2*NIMIINDL], st1[40];
	kilptietue kilp;
	INT p, piste, kcmp, entno, korjaus = 0, n_av;
	INT m_ajat[VALUKU+5], m_pv[N_PV+1], tcomfl0, hyv_muutos = 0;
//	int tarflx = 0;
	wchar_t ss[16];
//	INT32 tls;
	INT keskeyta = 0;
	bool pak_ix = false;

	INT32 ebadge[2], ubadge[2];
	INT bkno, kysyfl;
	emittp em;
	INT32 ep;
	wchar_t ch;
#ifdef TV1
	int p1;
#endif
   if (d < 1 || d >= nrec) {
	   parametrivirhe(L"tallenna - d", d, L"", L' ');
	   return;
	   }

	if (ToimintaTila == 2) {
		int iid, dd;
		if (tuloskaavat(kielto))
			return;
		if ((iid = id()) == 0 || ((dd = getpos(iid)) > 0 && dd != d)) {
			parametrivirhe(L"tallenna - kilpno", iid, L"", L' ');
			setId(haeSeurVapaaKno(k_pv, iid), k_pv);
			if (loki) {
				swprintf(line, L"tallenna vaihtoi tietueen %d kilpnon %d -> %d", d, iid, id());
				wkirjloki(line);
				}
			}
		}

	if (comtarfl == -1) {
		pak_ix = true;
		comtarfl = 0;
		}
//	EnterCriticalSection(&tall_CriticalSection);
	if (Sarja() < 0) {
		if (!kielto) {
			writeerror_w(L"Yritys tallentaa kilpailija ilman sarjaa. Tallennus peruutettu", 0);
			return;
			}
		else
			setSarja(sarjaluku-1);
		}
	tcomfl0 = tcomfl;
	tcomfl = 0;
	kysyfl = !kielto;
	if (kielto == -1)
		kielto = 0;
//	if (p_aika(0))
//		piste = 0;
	memset(m_ajat, 0, sizeof(m_ajat));
	memset(m_pv, 0, sizeof(m_pv));
	if (!pvparam[k_pv].hiihtolahto)
		pv[k_pv].va[0].vatulos = pv[k_pv].tlahto;
	if (kilpparam.alalaji == L'D') {
		laskeKierrokset();
		}
	kilp.GETREC(d);
	n_av = n_avoin(Sarja());
	if (vaaditarkastus)
		n_av = n_tarkastamatta(Sarja());
	if (!entkilp)
		entkilp = &kilp;
	entno = kilp.kilpno;
	if (!kilpparam.sarjavaihtelee)
		setSarjaAll(Sarja());
	if (!kilpparam.bibvaihtelee)
		setIdAll(id());

		// M‰‰ritet‰‰n muutosten m‰‰r‰ ja luonne ohjelmalla kilpcmp
		// Ohitetaan kaikki, ellei muutoksia ole

	for (int kierr = 0; kierr == 0 && ((kcmp = kilpcmp(entkilp, this, m_ajat, m_pv)) > 0 || pakota_lah || pak_ix); kierr++) {

		if (ToimintaTila == 1) {
			if (kcmp)
				kcmp = 4;
			else
				break;
			}
/*
		if (kilpparam.kierroskilpailu) {
			for (piste = Sarjat[Sarja()].valuku[k_pv]; piste > 0; piste--) {
				if (m_ajat[piste+1]) {
					if (p_aika(piste, 0)) {
						set_tulos(0, maali(piste), false);
						m_ajat[1] = 1;
						}
					break;
					}
				}
			}
*/

		// Pyyt‰‰ p‰ivitt‰m‰‰n sijoitustiedot

		if (kcmp && kielto) {
			merk_sijat();
			}

		// Asettaa muuttujan hyv_muutos, jos muutos koskee vain hyv‰ksymist‰
		// aiemmin avoimesta (tai p‰invastoin). Est‰‰ turhia rivej‰ viimeisten
		// tapahtumien n‰ytˆll‰

		if (m_ajat[kilpparam.valuku+2] && hyv() == kilp.hyv())
			hyv_muutos = 1;

		// Asettaa muuttujan korjaus muutoslaskureiden m‰‰r‰‰mist‰ varten

		if(kilp.tulos_pv(k_pv) || !kilp.hyv()) {
			korjaus = TRUE;
			}
		pfl = FALSE;
		innpfl = FALSE;

		// Poistetaan muutoksissa vanhentuneet indeksoinnit sijoitusindekseist‰

		for (piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
			if (pak_ix || kcmp > 3 || m_ajat[piste+1] ||
				(!piste  && (m_ajat[kilpparam.valuku+2] || m_ajat[kilpparam.valuku+3])))
				remjarjarr(&kilp, d, piste, &keskeyta);

		// P‰ivitet‰‰n nimi-indeksi, jos nimi on muuttunut

		if (wcscmp(sukunimi, kilp.sukunimi) || wcscmp(etunimi, kilp.etunimi)) {
			keyaak(&kilp, keyn, nameindex.keyl, 0);
//				keyfromwname(keyn, kilp.nimi(st, sizeof(st)/2, false), NIMIINDL);
			delix(&nameindex,keyn,d);
			keyaak((kilptietue *)this, keyn, nameindex.keyl, 0);
//				keyfromwname(keyn, nimi(st, sizeof(st)/2, false), NIMIINDL);
			addix(&nameindex,keyn, d);
			}

		// Muuttuuko Emit-koodi?

		for (int ibdg = 0; kcmp && ibdg < (kilpparam.kaksibadge ? 2 : 1); ibdg++) {
			ep = -1;
			ebadge[ibdg] = kilp.Badge(k_pv, ibdg);
			ubadge[ibdg] = Badge(k_pv, ibdg);
			if (emitfl && (ibdg == 0 || kilpparam.kaksibadge == 2) && ebadge[ibdg] != ubadge[ibdg]) {
				INT16 jbdg = -1;

			// Jos uusi badge on jo k‰ytˆss‰, on bkno sit‰ k‰ytt‰neen kilpailijanumero
			// Koodimuutoksen tallennus peruutetaan, jos muutos tehty t‰ll‰ koneella
			// mutta hyv‰ksyt‰‰n tiedonsiirron tuoman muutoksen tapauksessa sek‰
			// kun argumentti kielto == -1
			// T‰llˆin

				bkno = bdg2kno(ubadge[ibdg]);
				getEmitJarr(bkno, &jbdg);
				if (bkno && kysyfl) {
					swprintf(line,
						L"Kilpailijalla %d on jo e-koodi %ld - poista ennen muutosta",
						bkno, ubadge[ibdg]);
					writeerror_w(line, timerfl*2000, true);
					pv[k_pv].badge[ibdg] = kilp.pv[k_pv].badge[ibdg];
					ubadge[ibdg] = ebadge[ibdg];
					bkno = 0;
					}
				else {
					if (bkno) {
						if (jbdg >= 0)
							ep = getem(&em, bkno, jbdg);
						}
					if (ep == -1 && ebadge[ibdg])
						ep = getem(&em, kilp.id(), ibdg);
					if (ep >= 0 && kysyfl) {
						if (ubadge[ibdg]) {
							if (!kuntosuunnmoodi)
								writeerror_w(L"Kilpailijalle luetut Emit-tiedot s‰ilyv‰t "
									L"kilpailijalla", timerfl*2000, true);
							}
						else {
#ifdef _CONSOLE
							if (!timerfl) {
								ch = L' ';
								wselectopt(L"Yhteys kilpailijalle luettuihin Emit-tietoihin"
								L"menetet‰‰n. Vahvista (K/E)", L"KE", &ch);
								}
							else
#endif
								{
//							if (tark(k_pv) != L'V')
//								writeerror_w(L"Yhteys kilpailijalle luettuihin Emit-tietoihin"
//									L"menetet‰‰n.", 2000);
								ch = L'K';
								}
							if (ch == L'E') {
								pv[k_pv].badge[ibdg] = kilp.pv[k_pv].badge[ibdg];
								ubadge[ibdg] = ebadge[ibdg];
								}
							}
						}
					}

				// Seuraavassa edet‰‰n, jos muutos on tapahtunut eik‰ sit‰ ole yll‰ peruttu

				if (ubadge[ibdg] != ebadge[ibdg]) {

				// Seuraava koskee tilannetta, jossa uusi koodi on toisella kilpailijalla bkno
				// Ennen seuraavaa on em kilpailijan bkno emittietue, jonka badge on ubadge.
				// Jos myˆs vanhaa koodia vastaa emitietue, poistetaan siit‰ kilpno
				// Sitten luetaan takaisin kilpailijan bkno emittiedot

					if (ebadge[ibdg] && bkno && em.kilpno == bkno) {
						if ((ep = getem(&em, kilp.id(), ibdg)) >= 0) {
							em.kilpno = 0;
							putem(&em, ep, 1);
							}
						if (jbdg >= 0)
							ep = getem(&em, bkno, jbdg);
						}

				// Vanhan kilpailijanumeron ja vanhan emit-tietueen yhteys poistetaan
				// tauluista bdg_kno ja emitjarr

					if (ebadge[ibdg]) {
						rembadge(ebadge[ibdg], kilp.id(), timerfl*kysyfl);
						}

				// Jos emittietue oli linkitetty toiseen kilpailijaan, poistetaan linkit
				// taulusta bdg_kno ja emitjarr sek‰ lis‰t‰‰n uusi linkki tauluun emitjarr

					if (bkno) {
						rembadge(ubadge[ibdg], bkno, timerfl*kysyfl);
						em.kilpno = id();
						if (jbdg >= 0)
							setEmitJarr(-1, bkno, jbdg);
						if (ubadge[ibdg])
							setEmitJarr(ep, id(), ibdg);
						}

				// Vaihdetaan uusi badge emittietueeseen. T‰ll‰ ei n‰ytt‰isi olevan vaikutusta

					em.badge = ubadge[ibdg];
					em.osuus = ibdg;

				// Tallennetaan emit-tietueeseen tulleet muutokset (l‰hinn‰ kilpno)

					putem(&em, ep, 1);
					}
				}

			// Onko kilpailijanumero muuttunut?

			if (id() != kilp.id() ) {

				// Tietueen sijainti tallennetaan uudelle kilpailijanumerolle

				if (id() >= minkilpno && id() <= maxkilpno)
					setpos(id(), d);

				// Jos kilpailijalla oli emitkoodi poistetaan vanhentuneet linkit
				// tauluista bdg_kno ja emitjarr, vaihdetaan kilpno emit-tietueessa (jos on)
				// ja kirjoitetaan uudet linkit tauluihin emitjarr ja bdg_kno
				// muutokset merkit‰‰n tehdyiksi asettamalla ebadge = ubadge

				if (emitfl && ebadge[ibdg]) {
					rembadge(ebadge[ibdg], kilp.id(), timerfl*kysyfl);
					if ((ibdg == 0 || kilpparam.kaksibadge == 2) && (ep = getem(&em, kilp.id(), ibdg)) >= 0) {
						setEmitJarr(-1, kilp.id(), ibdg);
						em.kilpno = id();
						putem(&em, ep, 1);
						if (ubadge[ibdg])
							setEmitJarr(ep, id(), ibdg);
						}
					addbadge(ubadge[ibdg], id(), timerfl*kysyfl);
					ebadge[ibdg] = ubadge[ibdg];
					}

				//  Poistetaan vanhaa kilpailijanumeroa vastaava linkki taulusta posarr

				if (kilp.id() >= minkilpno && kilp.id() <= maxkilpno)
					setpos(kilp.id(), 0);
				}

				// Poistetaan vanhentunut linkki taulusta emitjarr, kun sit‰ ei en‰‰ tarvita
				// myˆsk‰‰n kilpailijanumeromuutoksen tapauksessa

			else if (emitfl && (ibdg == 0 || kilpparam.kaksibadge == 2) && ebadge[ibdg] && !ubadge[ibdg]) {
				setEmitJarr(-1, kilp.id(), ibdg);
				}
			}  // for (int idbg = 0; ..

#if defined(GAZ)
		if (m_ajat[1] > 1 && kilp.p_aika(0) == 0 && p_aika(0))
			add_gaztapahtuma(this);
#endif

			// Vaihdetaan uudet tiedot tietueeseen kilp

//		kilp = *this;

			// P‰ivitet‰‰n sijoitustiedot

		for (p = 0; p <= kilpparam.valuku+1; p++) {
			if (p > Sarjat[Sarja()].valuku[k_pv] && (!k_pv || p != kilpparam.valuku+1))
				continue;
			if (kcmp > 3 || m_ajat[p+1])
				haesija(this, p, 0, &keskeyta);
			}

			// Tallennetaan uudet tiedot sek‰ taulukkoon kilparr ett‰ levylle (ja tietokantaan, jos k‰ytˆss‰)

		PUTREC(d, 1);

			// Lis‰t‰‰n linkki tauluun bdg_kno, jos taulu on k‰ytˆss‰, badge != 0 ja muuttunut

		for (int ibdg = 0; kcmp && ibdg < (kilpparam.kaksibadge ? 2 : 1); ibdg++) {
			ubadge[ibdg] = Badge(k_pv, ibdg);
			if (emitfl && ubadge[ibdg] && bdg2kno(ubadge[ibdg]) != id())
				addbadge(ubadge[ibdg], id(), timerfl*kysyfl);
			}

//		piste = 1;
			// Lis‰t‰‰n muutoksia vastaavat tietot sijoitusindekseihin

		for (p = 0; p <= Sarjat[Sarja()].valuku[k_pv]; p++) {
			if (pak_ix || kcmp > 3 || m_ajat[p+1] ||
				(!p && (m_ajat[kilpparam.valuku+2] || m_ajat[kilpparam.valuku+3]))) {
				addjarjarr(this, d, p, &keskeyta);
				tlsmuutos[Sarja()][p] = 1;
				}
			}
#if defined(TV1)
		if (tv_com) {
			for (p = 0; p <= kilpparam.valuku+2; p++) {
				if (m_ajat[p] && (!hyv_muutos || p < kilpparam.valuku+2)) {
					p1 = p == kilpparam.valuku+2 ? 0 : p-1;
					lah_tv(this, p1, p_sija(p1));
					}
				}
			}
#endif

			// Lis‰t‰‰n tiedot HkKisaWinin tapahtuman‰yttˆ‰ varten
			// (tyhj‰ funktio konsolisovelluksessa)

		for (p = 0; p <= kilpparam.valuku+2; p++) {
			if (m_ajat[p+1] > 1 && (!hyv_muutos || p > 0)) {
				addtapahtuma(this, p);
				}
			}
		if (m_ajat[kilpparam.valuku+3])
			add_sakko(d);

//#if defined(MERI5)
		if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && m_ajat[0] &&
			eralahto[era_akt] == AIKAJAK*TMAALI0 && era_akt == pv[k_pv].era) {
			eralahto[era_akt] = AIKAJAK*p_aika(-1);
			lahetaeralahto(era_akt, kielto);
			}
//#endif

		paivita_aikanaytto();

			// L‰hetet‰‰n muutostiedot tiedonsiirtoon

//
//  piste = 0  :						L‰het‰ kaikki perustiedot
//  piste < 0  :						L‰het‰ kaikki p‰iv‰n 1-piste tiedot
//										aktiivinen p‰iv‰ ilman aikoja, muut aikoineen
//  piste = 1 .. kilpparam.valuku+2     L‰het‰ pisteen piste-2 aika
//								        (-1: l‰htˆ, 0: maali, >0: v‰liaika)
//  piste = kilpparam.valuku+3          L‰het‰ keskhyl
//  piste = kilpparam.valuku+4          L‰het‰ ammunnan sakkotiedot
//  piste = kilpparam.valuku+5          L‰het‰ muu sakkotieto
//

		if (comfl) {
			int lahno;
			lahno = entno;

			// L‰hetet‰‰n kilpailijan perustiedot
			// Kilpailijanumero muuttuu tarvittaessa t‰m‰n sanoman perusteella
			// lahno sis‰lt‰‰ t‰m‰n j‰lkeen oikean numeron seuraavien sanomien pohjaksi

			if ((kcmp & 1) && m_pv[0]) {
				laheta(d, entno, 0 , 0, comtarfl && lahtarfl, kielto, pakota_lah);
				lahno = kilpno;
				}

			// L‰hetet‰‰n kunkin p‰iv‰n p‰iv‰kohtaiset tiedot

			for (p = 0; p < kilpparam.n_pv_akt; p++) {
				if ((kcmp & 1) && m_pv[p+1])
				laheta(d, lahno, -1-p, 0, 0, kielto, pakota_lah);
				}

			// L‰hetet‰‰n tarkistus- ja sakkotiedot

			for (p = kilpparam.valuku+1; p <= kilpparam.valuku+3; p++) {
				if (m_ajat[p+1]) {
					laheta(d, lahno, p+2, hyv_muutos ,comtarfl && lahtarfl, kielto,  pakota_lah);
					}
				}
			// L‰hetet‰‰n tulostiedot

			for (p = pvparam[k_pv].hiihtolahto ? -1 : 0; p < kilpparam.valuku+1; p++) {
				if (m_ajat[p+1]) {
					laheta(d, lahno, p+2, hyv_muutos ,comtarfl && lahtarfl, kielto,  pakota_lah);
					}
				}
			}

			// Kertoo, ett‰ sarjaan on tullut muutoksia

		sjfl[Sarja()] = TRUE;

			// Tallennetaan tieto maalin mno viimeisimm‰st‰ maaliajasta
			// Tiedon merkitys on nykyisin v‰h‰inen

		if (!korjaus && m_ajat[1] && (maali(0) != TMAALI0))
			maaliajat[Sarjat[Sarja()].mno[k_pv]-1] = maali(0);

			// Kasvattaa muutoslaskuria automaattitulostuksen ohjaamiseksi

		if (((autofileparam.autokomentolaji && autofileparam.afvali && !autofileparam.af_kaikki) ||
			autotulostusparam.automaali[Sarjat[Sarja()].mno[k_pv]-1]) &&
			(m_ajat[0] || m_ajat[1] || m_ajat[kilpparam.valuku+2] ||
			m_ajat[kilpparam.valuku+3]) &&
			(korjaus || tulos_pv(k_pv) || !hyv())
			&& (autotulostusparam.rajaus != L'P' ||
			(sija(k_pv) <= autotulostusparam.viimsija && sija(k_pv) > 0)
			|| (k_pv &&
			(pv[k_pv].ysija <= autotulostusparam.viimsija && pv[k_pv].ysija > 0))
			|| innpfl))
			{
			fprior[Sarja()]++;
			prior[Sarja()] += prior0[Sarja()];
			if (n_av > 0 && n_avoin(Sarja()) == 0 && (!vaaditarkastus || n_tarkastamatta(Sarja()) == 0))
				prior[Sarja()] = vaaditarkastus ? kynnys+1 : kynnys-1;
			for (int syhd = 0; syhd < nsarjayhd; syhd++) {
				if (yhdsarjat[syhd][Sarja()]) {
					prior[sarjaluku+syhd] += prior0[sarjaluku+syhd];
					}
				}
			}

			// Kirjoitetaan mahdolliset lokitiedostomerkinn‰t

		if (loki) {
			//       if (piste < 0 || piste > kilpparam.valuku) piste = 0;
			for (piste = -1; piste <= Sarjat[Sarja()].valuku[k_pv]; piste++)
				if (m_ajat[piste+1])
					break;
			if (piste <= Sarjat[Sarja()].valuku[k_pv] &&
				maali(piste) != TMAALI0) {
				AIKATOWSTRS(st1, maali(piste), t0);
				AIKATOWSTRS(ss, p_aika(piste), 0);
				}
			else {
				wcscpy(st1, L"        ");
				wcscpy(ss, L"        ");
				}
			st1[pvparam[k_pv].laika] = 0;
			ss[pvparam[k_pv].laika] = 0;
			swprintf(line, L"     %10.10s %c %4d %-20.20s %-10.10s %-10.10s %8s %c",
				st1,ch_piste[piste+2],id(),nimi(st,sizeof(st)/2,false),seuratieto(0),
				Sarjat[Sarja()].sarjanimi,ss,tark(k_pv));
			wkirjloki(line);
			if (keskeyta) {
				swprintf(line,L"Keskeyt‰ = %d, kello %s", keskeyta, wkello());
				wkirjloki(line);
				}
			}
		if (lisaToimetOn) {
			lisaToimet(this, &kilp, kcmp, m_pv, m_ajat);
			}
		if (keskeyta > 2) {
			writeerror_w(L"Indeksointivirhe, Ohjelma keskeytet‰‰n", 0, true);
			inLopetus = 1;
			lopetus();
			}
		}
	tcomfl = tcomfl0;
			//	LeaveCriticalSection(&tall_CriticalSection);
	lukumaarat();
	}

void lisaavakantit(int vakanttiluku)
{
	kilptietue kilp;
	int d, alku = 0;

	if (lisavakanttialku > 0)
		alku = lisavakanttialku;
	if (alku == 0 || alku > maxkilpno - vakanttiluku + 1)
		alku = maxkilpno - vakanttiluku + 1;
	for (int i = 0; i < vakanttiluku; i++) {
		kilp.nollaa();
		kilp.setIdAll(maxkilpno-i);
		kilp.setSarjaAll(kilpparam.vakanttisarja);
		_itow(maxkilpno-i, kilp.etunimi, 10);
		for (int k = 0; k < kilpparam.n_pv; k++)
			kilp.set_tark(L'V', k);
		kilp.addtall(&d, 0);
		}
}

void pohjustalisavaiheet(int ed_n_pv_akt)
{
	kilptietue kilp;

	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus == 0) {
			for (int ipv = ed_n_pv_akt; ipv < kilpparam.n_pv_akt; ipv++) {
				kilp.pv[ipv].nollaa();
				if (kilp.tark(0) == L'V')
					kilp.set_tark(L'V', ipv);
				kilp.setId(kilp.id(), ipv);
				kilp.setSarja(kilp.Sarja(), ipv);
				kilp.pv[ipv].badge[0] = kilp.pv[Sarjat[kilp.Sarja()].enspv].badge[0];
				}
			}
		kilp.tallenna(d, 0, 0, 0, 0);
		}
}

int kirjKilpDat(wchar_t *fname)
{
	datafile datf_tmp;
	char cst[202];
	kilptietue kilp;
	UINT d;
	char buf[KILPBUFSIZE];

	makefile(&datf_tmp,wcstooem(cst, fname, 200),kilpparam.kilprecsize);
	for (int i = 1; i < nrec; i++) {
		kilp.GETREC(i);
		kilp.pack(buf);
		if (i < nrec-1)
			addrec0(&datf_tmp, &d,buf);
		else
			addrec(&datf_tmp, &d,buf);
		}
	closefile(&datf_tmp);
	return(0);
}

