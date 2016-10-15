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

#include <vcl.h>
#include <map>
#include <stdio.h>
#include <algorithm>
#include <functional>
using namespace std;
#include <stdlib>
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
#include <direct.h>

#include <wchar.h>
//#include <boxr.h>
//#include <tputil.h>
//#include "valinnat.h"
#include "HkDeclare.h"


#define N_PV_V 5

#define N_PV_VA N_PV_V

#define VALUKU_V 24

#define KILPRECSIZE_V 1000

#define LSARJA_V  10

#ifdef MAKI
#undef MAKI
#endif

#define LNIMI_V  36
#define LSEURA_V  14

//#define LSEURA_V 10

#define LSEURAM 30

#define LYHD_V 19
#define LARVO_V 11

#define LNIMI0  28
#define LSEURA0 10

#define LSAKKO_V 7

#pragma pack(push, 1)

class kilptietue_v {
public:
   int kilpstatus;
   int kilpno;
   INT32 lisno;
   int ilmlista;
   int piiri;
   UCHAR laina;
   UCHAR laina2;
   char selitys[19];
   char wrkoodi[10];
   INT32 piste[3];
   char arvo[LARVO_V+1];
   char nimi[LNIMI_V+1];
	char seura[LSEURAM+1];
   char yhdistys[LYHD_V+1];
   char maa[4];
   UCHAR sarja;
	INT16 arvryhma;
   INT32 *badge;
   INT32 *badge2;
   INT32 *tlahto1;
   INT32 *tlahto;
   UCHAR *keskhyl;
	char (* sakko)[LSAKKO_V+1];
   int *tasoitus;
   INT32 **vatulos;
   INT32 *tulos;
   int *sija;
   int **vasija;
   int *ysija;
   char  ** xtext;
   INT32 ** xint32;

   __fastcall kilptietue_v(void);
   __fastcall ~kilptietue_v(void);
	void nollaa(void);
   void operator=(kilptietue_v&);
   bool operator==(kilptietue_v&);
   INT32 p_sakko(INT piste);	void GETREC(INT d);
   void unpackkilp(char *buf);
   int readkilp(int d, int mista);
};

#define KNT_TEXT 0
#define KNT_CHAR_V 1
#define KNT_INT32_V 2
#define KNT_UINT32_V 3
#define KNT_INT16_V 4

typedef struct {
   int kilprecsize;
   char kilplaji;
   char alalaji[3];
   int n_pv;
   int valuku;
   int n_pv_va;
   int n_tlahto1;
   int n_xtext;
   int n_xint;
   int lnimi;
   int lseura;
   int arvo_on;
	int yhdistys_on;
	int lsakmax;
   }  kilpparamtp_v;

kilpparamtp_v kilpparam_v;
int lseuramax = 0;
int valuku_vanha;

class cl_knt {
public:
   AnsiString ots;
   int pos;
	int len;
   int lkm;
   int type;
   AnsiString dbtable;
   AnsiString dbcol;
   AnsiString dbid;

   __fastcall  cl_knt::cl_knt(AnsiString aots = "", int apos = 0, int alen = 0, int alkm = 1,
	  int atype = 0, AnsiString adbtable = "", AnsiString adbcol = "", AnsiString adbid = "");
   };

map<AnsiString,cl_knt> kentat;
typedef map<AnsiString,cl_knt>::const_iterator CI_knt;

__fastcall  cl_knt::cl_knt(AnsiString aots, int apos, int alen, int alkm,
   int atype, AnsiString adbtable, AnsiString adbcol, AnsiString adbid)
   {
   ots = aots;
   pos = apos;
   len = alen;
   lkm = alkm;
   type = atype;
   dbtable = adbtable;
   dbcol = adbcol;
   dbid = adbid;
   }

int luekentat_v(int mista)
   {
   int x = 0;

   if (mista != 99999) {
	  if (kilpparam_v.kilplaji == 'T') {
		 kilpparam_v.lnimi = LNIMI_V;
		 kilpparam_v.lseura = LSEURA_V;
		 }
	  else {
		 kilpparam_v.lnimi = LNIMI0;
		 kilpparam_v.lseura = LSEURA0;
		 }
	  if (lseuramax) {
		 kilpparam_v.lseura = LSEURAM;
	  }
	  kentat["kilpstatus"] = cl_knt("status",  x, 2, 1, KNT_INT16_V);
	  x += 2;
	  kentat["kilpno"] = cl_knt("kilpno",  x, 2, 1, KNT_INT16_V);
	  x += 2;
	  kentat["lisno"] = cl_knt("lisno",  x, 4, 1, KNT_INT32_V);
		x += 4;
	  kentat["ilmlista"] = cl_knt("ilmlista",  x, 2, 1, KNT_INT16_V);
	  x += 2;
	  kentat["piiri"] = cl_knt("piiri",  x, 2, 1, KNT_INT16_V);
	  x += 2;
	  kentat["badge"] = cl_knt("badge",  x, 4, kilpparam_v.n_pv, KNT_INT32_V);
	  x += 4*kilpparam_v.n_pv;
	  if (kilpparam_v.kilplaji == 'S' || kilpparam_v.kilplaji == 'Y') {
		 kentat["selitys"] = cl_knt("selitys",  x, 19, 1, KNT_TEXT);
		 x += 19;
		 kentat["laina"] = cl_knt("laina",  x, 1, 1, KNT_CHAR_V);
		 x += 1;
		 }
	  if (kilpparam_v.kilplaji == 'T') {
		 kentat["badge2"] = cl_knt("badge2",  x, 4, kilpparam_v.n_pv, KNT_INT32_V);
		 x += 4*kilpparam_v.n_pv;
		 kentat["selitys"] = cl_knt("selitys",  x, 10, 1, KNT_TEXT);
		 x += 10;
		 kentat["laina2"] = cl_knt("laina2",  x, 1, 1, KNT_CHAR_V);
		 x += 1;
		 kentat["laina"] = cl_knt("laina",  x, 1, 1, KNT_CHAR_V);
		 x += 1;
		 }
	  kentat["piste"] = cl_knt("piste",  x, 4, 3, KNT_INT32_V);
	  x += 3*4;
	  if (kilpparam_v.kilplaji == 'Y') {
		  kentat["arvo"] = cl_knt("arvo",  x, LARVO, 1, KNT_TEXT);
		  x += LARVO+1;
		  }
	  kentat["nimi"] = cl_knt("nimi",  x, kilpparam_v.lnimi, 1, KNT_TEXT);
	  x += kilpparam_v.lnimi+1;
	  kentat["seura"] = cl_knt("seura",  x, kilpparam_v.lseura, 1, KNT_TEXT);
	  x += kilpparam_v.lseura+1;
	  if (kilpparam_v.kilplaji == 'Y') {
		  kentat["yhdistys"] = cl_knt("yhdistys",  x, LYHD, 1, KNT_TEXT);
		  x += LYHD+1;
		  }
		kentat["maa"] = cl_knt("maa",  x, 4, 1, KNT_TEXT);
	  x += 4;
		kentat["sarja"] = cl_knt("sarja",  x, 1, 1, KNT_CHAR_V);
	  x += 2;
	  kentat["arvryhma"] = cl_knt("arvryhma",  x, 2, 1, KNT_INT16_V);
	  x += 2;
	  kentat["tlahto"] = cl_knt("tlahto",  x, 4, kilpparam_v.n_pv, KNT_INT32_V);
	  x += 4*kilpparam_v.n_pv;
	  kentat["keskhyl"] = cl_knt("keskhyl",  x, 1, kilpparam_v.n_pv, KNT_CHAR_V);
		x += 4*((kilpparam_v.n_pv+3)/4);
		if (kilpparam_v.kilplaji == 'S' || kilpparam_v.kilplaji == 'Y' || kilpparam_v.kilplaji == 'T') {
			kentat["tasoitus"] = cl_knt("tasoitus",  x, 2, kilpparam_v.n_pv, KNT_INT16_V);
			x += 4*((kilpparam_v.n_pv+1)/2);
			}
		else {
			kentat["sakko"] = cl_knt("sakko",  x, 8, kilpparam_v.n_pv, KNT_TEXT);
			x += 8*kilpparam_v.n_pv;
			}
		kentat["tlahto1"] = cl_knt("tlahto1",  x, 4, kilpparam_v.n_pv, KNT_INT32_V);
		x += 4*kilpparam_v.n_pv;
		kentat["vatulos"] = cl_knt("vatulos",  x, 4, kilpparam_v.n_pv_va*kilpparam_v.valuku, KNT_INT32_V);
		x += 4*kilpparam_v.n_pv_va*kilpparam_v.valuku;
		kentat["tulos"] = cl_knt("tulos",  x, 4, kilpparam_v.n_pv, KNT_INT32_V);
		x += 4*kilpparam_v.n_pv;
		kentat["sija"] = cl_knt("sija",  x, 2, kilpparam_v.n_pv, KNT_INT16_V);
		x += 4*((kilpparam_v.n_pv+1)/2);
		kentat["vasija"] = cl_knt("vasija",  x, 2, kilpparam_v.n_pv_va*kilpparam_v.valuku, KNT_INT16_V);
		x += 2*kilpparam_v.n_pv_va*kilpparam_v.valuku;
		if (kilpparam_v.n_pv > 1) {
			kentat["ysija"] = cl_knt("ysija",  x, 2, kilpparam_v.n_pv-1, KNT_INT16_V);
			x += 4*(kilpparam_v.n_pv/2);
			}
#ifdef MAKIxx
		if (kilpparam_v.kilplaji == 'M') {
			kentat["vuoro"] = cl_knt("vuoro",  x, 2, NKIERROS, KNT_INT16_V);
			x += 2 * NKIERROS;
			kentat["nopeus"] = cl_knt("nopeus",  x, 2, NKIERROS, KNT_INT16_V);
			x += 2 * NKIERROS;
			kentat["pituus"] = cl_knt("pituus",  x, 2, NKIERROS, KNT_INT16_V);
			x += 2 * NKIERROS;
			kentat["tuomari"] = cl_knt("tuomari",  x, 2, 4*5*NKIERROS, KNT_INT16_V);
			x += 2 * 4 * 5 * NKIERROS;
			kentat["mpisteet"] = cl_knt("mpisteet",  x, 2, NKIERROS, KNT_INT16_V);
			x += 2 * NKIERROS;
			kentat["msija"] = cl_knt("msija",  x, 2, NKIERROS, KNT_INT16_V);
			x += 2 * NKIERROS;
			kentat["pari"] = cl_knt("pari",  x, 2, 1, KNT_INT16_V);
			x += 2;
			kentat["mjoukkue"] = cl_knt("mjoukkue",  x, 1, kilpparam_v.lseura+1, KNT_CHAR_V);
			x += kilpparam_v.lseura+1;
			kentat["jkierr"] = cl_knt("jkierr",  x, 1, 1, KNT_CHAR_V);
			x += 1;
			}
#endif
		kilpparam_v.kilprecsize = x;
		}
	return(0);
	}

void kilptietue::unpack_v_kilp(char *buf)
{
	wchar_t wln[100];
   nollaa();
	for (CI_knt p = kentat.begin(); p != kentat.end(); ++p) {
	  if (!strcmp(p->first.c_str(), "kilpstatus")) {
		 memcpy(&kilpstatus, buf+p->second.pos, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "kilpno")) {
		 memcpy(&kilpno, buf+p->second.pos, p->second.len);
		 continue;
			}
	  if (!strcmp(p->first.c_str(), "lisno")) {
		 memcpy(lisno, buf+p->second.pos, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "ilmlista")) {
			memcpy(&ilmlista, buf+p->second.pos, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "piiri")) {
		 memcpy(&piiri, buf+p->second.pos, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "selitys")) {
		 oemtowcs(pv[0].selitys, buf+p->second.pos, p->second.len, 0);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "laina")) {
		 pv[0].laina[0] = oemtowchar(buf[p->second.pos]);
			continue;
		 }
	  if (!strcmp(p->first.c_str(), "laina2")) {
		 pv[0].laina[1] = oemtowchar(buf[p->second.pos]);
			continue;
		 }
		if (!strcmp(p->first.c_str(), "arvo")) {
			oemtowcs(arvo, buf+p->second.pos, p->second.len, 0);
			continue;
			}
		if (!strcmp(p->first.c_str(), "nimi")) {
			wesnimet(oemtowcs(wln, buf+p->second.pos, p->second.len, 0), etunimi, sukunimi);
			continue;
			}
		if (!strcmp(p->first.c_str(), "seura")) {
			oemtowcs(seura, buf+p->second.pos, p->second.len, 0);
			wcsncpy(seuralyh, seura, LLYH);
			continue;
			}
		if (!strcmp(p->first.c_str(), "yhdistys")) {
			oemtowcs(yhdistys, buf+p->second.pos, p->second.len, 0);
			continue;
			}
		if (!strcmp(p->first.c_str(), "maa")) {
			oemtowcs(maa, buf+p->second.pos, p->second.len, 0);
			continue;
			}
		if (!strcmp(p->first.c_str(), "sarja")) {
			memcpy(&sarja, buf+p->second.pos, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "arvryhma")) {
			memcpy(&arvryhma, buf+p->second.pos, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "badge")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pv[i].badge[0], buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
		if (!strcmp(p->first.c_str(), "badge2")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pv[i].badge[1], buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "piste")) {
		 for (int i = 0; i < p->second.lkm; i++)
			memcpy(&pisteet[i], buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "tlahto")) {
		 for (int i = 0; i < p->second.lkm; i++)
			memcpy(&pv[i].tlahto, buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "tlahto1")) {
			for (int i = 0; i < p->second.lkm; i++)
			memcpy(&pv[i].va[0].vatulos, buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "keskhyl")) {
		 for (int i = 0; i < p->second.lkm; i++)
			memcpy(&pv[i].keskhyl, buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
		 }
		if (!strcmp(p->first.c_str(), "sakko")) {
		 for (int i = 0; i < p->second.lkm; i++)
			oemtowcs(pv[i].asakot, buf+p->second.pos+i*p->second.len, p->second.len, 0);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "tasoitus")) {
		 for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pv[i].tasoitus, buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
	  if (!strcmp(p->first.c_str(), "tulos")) {
		 for (int i = 0; i < p->second.lkm; i++)
			memcpy(&pv[i].va[1].vatulos, buf+p->second.pos+i*p->second.len, p->second.len);
		 continue;
		 }
		if (!strcmp(p->first.c_str(), "sija")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pv[i].va[1].vasija, buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "ysija")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pv[i+1].ysija, buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "vatulos")) {
			if (p->second.lkm == kilpparam.n_pv_akt*kilpparam.valuku) {
				for (int i = 0; i < kilpparam.n_pv_akt; i++) {
					for (int j = 0; j < kilpparam.valuku; j++)
						memcpy(&pv[i].va[j+2].vatulos, buf+p->second.pos+(i*kilpparam.valuku+j)*p->second.len,
							p->second.len);
					}
				}
			continue;
			}
		if (!strcmp(p->first.c_str(), "vasija")) {
			if (p->second.lkm == kilpparam.n_pv_akt*kilpparam.valuku) {
				for (int i = 0; i < kilpparam.n_pv_akt; i++) {
					for (int j = 0; j < kilpparam.valuku; j++)
						memcpy(&pv[i].va[j+2].vasija, buf+p->second.pos+(i*kilpparam.valuku+j)*p->second.len,
							p->second.len);
					}
				}
			continue;
			}
#ifdef MAKIx
		if (!strcmp(p->first.c_str(), "vuoro")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&vuoro[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "nopeus")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&nopeus[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "pituus")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&pituus[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "tuomari")) {
			for (int i = 0; i < NKIERROS; i++) {
				for (int j = 0; j < 5; j++) {
					for (int k = 0; k < 4; k++)
						memcpy(&tuomari[i][j][k], buf+p->second.pos+((i*5+j)*4)*p->second.len,
							p->second.len);
					}
				}
			continue;
			}
		if (!strcmp(p->first.c_str(), "mpisteet")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&mpisteet[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "msija")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&msija[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "pari")) {
			memcpy(&pari, buf+p->second.pos, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "joukkue")) {
			for (int i = 0; i < p->second.lkm; i++)
				memcpy(&joukkue[i], buf+p->second.pos+i*p->second.len, p->second.len);
			continue;
			}
		if (!strcmp(p->first.c_str(), "jkierr")) {
			memcpy(&jkierr, buf+p->second.pos, p->second.len);
			continue;
			}
#endif // MAKI
		}
	for (int ipv = 0; ipv < kilpparam.n_pv; ipv++) {
		pv[ipv].bib = kilpno;
		pv[ipv].sarja = sarja;
		}
}

static datafile datf2v;

int readkilp_v(kilptietue *kilp_v, int d, int mista)
	{
	char buf[KILPRECSIZE_V];

	if (mista == 0) {
		getrec(&datf2v,d,buf);
		if (*(INT16 *)buf == 0) {
			kilp_v->unpack_v_kilp(buf);
			return(0);
			}
		else
			return(1);
		}
	return(0);
	}

int luevanha(wchar_t *dataf2nm)
{
	INT  d = 0,ld, dd, n = 0;
	INT  okk;
	kilptietue kilp_v;
	char cst[102];

	luekentat_v(0);
	if (openfile(&datf2v,wcstooem(cst, dataf2nm, 100),kilpparam_v.kilprecsize))
		return(0);
	ld = filelen(&datf2v)-1;
	EnterCriticalSection(&tall_CriticalSection);
	if (ld > 0) do {
		okk = FALSE;
		while (!okk && d < ld) {
			okk = !readkilp_v(&kilp_v, ++d, 0);
			}
		if (okk) {
			kilp_v.addtall(&dd, 0);
			n++;
			}
		}	while (okk);
	LeaveCriticalSection(&tall_CriticalSection);
	closefile(&datf2v);
	return(n);
}


