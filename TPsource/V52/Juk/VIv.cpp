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
#include <time.h>
#include <ctype.h>
#include <malloc.h>
#ifdef _CONSOLE
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#include <butil.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
#include "TpLaitteet.h"

#include <wincom.h>
#include <sys/timeb.h>

#ifdef LUENTA
#undef EMITFILE
#endif

#define ALGE

#define AUTOINTVAL 18*60
#define MSSTKSIZE 256
#define AIKA_LUKU 10
#define MAX_R_OD 10

/*
regnly
1	REGNLY
2	REGNLY2
3	ALGE
4	COMET
5	ALGE4
6	TIMY
7	ALGETUTKA
8	TDC
9	ASC
11	EMITKELLO
12	LUKIJA
13	MTR
14	EMITAG
15	ETGPRS
16	ETSQL
19	TUOMARI
21	SPORTIDENT
22	ARES
28	RADIOEMIT
29	VALOT
30	SIRIT
31	FEIG
32	??
33	IMPINJ
36	SW2000
40	SAKKO_YHT
99	SIIMPORT
*/

#ifdef ALGETAULU
void paiv_taulu(void);
extern INT taulu_com;
#endif

extern int initid[MAX_LAHPORTTI];
extern int keyclose[MAX_LAHPORTTI];
extern FILE *emittallfile;
extern int n_emittallfile;
//extern int odota;
static int esto = FALSE;
static int htime[MAXJONO];
int siritopen[NREGNLY];
extern int timerfl;
int intv[] = {AUTOINTVAL, COMINTVAL};
int aika_head, aika_tail;
INT32 int_aika[AIKA_LUKU];

static void vaihda_ajat(int ino);
void naytaluenta(int badge);
// aika_syy saa seuraavat arvot riippuen ajanottotavasta:
//      0 : RS-portti
//      1 : hiiri             EI KÄYTÖSSÄ
//      2 : näppäimistö       EI KÄYTÖSSÄ
//
int aika_syy[AIKA_LUKU];

INT32 ivtime[2] = {0L,0L};
IV_KEY keytab[] = { KB_C_C_A, KB_S_C_A};
int nivkeys = 1;

extern int viimrivi;
#ifdef MAALI

#ifdef EMITLEIMAT
INT32 uusi_emit;
void prtemitsivu(void);
void add_bdg_t(UINT32 badge, INT r_no, char byte9, UINT32 tms);

#ifdef LUENTA
void add_bdg_t(UINT32 badge, INT r_no, char byte9, UINT32 tms) {}
#endif

void tall_ampsakko(char *sakbuf, int r_no) {}

#define UL2CH(hl) (((hl).lo-48)+16*((hl).hi-48))


typedef struct {
   UINT32 badge;
   INT32 t;
   } bdg_t_tp;


#define N_BDG_T 8
static bdg_t_tp bdg_t[N_BDG_T];
UINT32 pkg_alku[NREGNLY], pkg_loppu[NREGNLY];
INT lue_rno;
char alge_tunn[10][4] = { "SZ", "ZZ", "ZW", "ZW ", "K03", "K06", "K09",
	"K02", "K05", "K08" };
#endif // EMITLEIMAT

extern INT32 viim_regnly;

INT tall_emit(san_type *vastaus, UINT32 *vahvistus, INT r_no);
INT r_msg_len[NREGNLY];
#endif

#if !defined(LUENTA)

void luelahdepisteet(void)
	{
	TextFl *infile = NULL;
	wchar_t *p, *p1, Buf[200];
	INT16 srj, pst, kdi, kdi2, os, osy;
	wchar_t flnm[2][20] = {L"LAHDEPISTEET.LST", L"BOXOHJE.LST"};
	int nkdi[MAXSARJALUKU+2][MAXOSUUSLUKU+1];
	static bool luettu = false;

	if (luettu)
		return;
	luettu = true;
	for (int i = 0; i < 2; i++) {
		infile = new TextFl(flnm[i], L"rt");
		if (infile->IsOpen())
			break;
		if (infile)
			delete infile;
		infile = NULL;
		}
	if (!infile)
		return;
	if (!infile->IsOpen()) {
		lahdepistehaku = 0;
		delete infile;
		return;
		}
	lahdepistehaku = 1;
	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		bool kaikki = false;

		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (!p)
			continue;
		if (!wcscmpU(p, L"KAIKKI")) {
			srj = sarjaluku;
			kaikki = true;
			}
		else if (!wcscmpU(p, L"LOPUT"))
			srj = sarjaluku;
		else
			srj = haesarja_w(p, false);
		if ((p = wcstok(NULL, L" ;\t\n")) != NULL) {
			if (!wcscmpU(p, L"KAIKKI")) {
				os = 0;
				if (srj >= 0 && srj < sarjaluku)
					osy = Sarjat[srj].ntosuus-1;
				else
					osy = kilpparam.ntosuus-1;
				}
			else {
				os = _wtoi(p) - 1;
				osy = os;
				}
			}
		if (!p || os >= kilpparam.ntosuus)
			continue;
		if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
			pst = _wtoi(p);
			while (pst < kilpparam.valuku+2 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				if ((p1 = wcsstr(p, L"-")) > NULL) {
					*p1 = 0;
					kdi = _wtoi(p);
					kdi2 = _wtoi(p1+1);
					if (kdi > 0 && kdi2 >= kdi) {
						for (int os1 = os; os1 <= osy; os1++)
							nkdi[srj][os1] += (kdi2-kdi+1)*(kaikki ? 1000000 : 1);
						}
					}
				else
					if (_wtoi(p) > 0)
						for (int os1 = os; os1 <= osy; os1++)
						nkdi[srj][os1] += (kaikki ? 1000000 : 1);
				}
			}
		}
	infile->Rewind();

	for (srj = 0; srj <= sarjaluku; srj++) {
		for (os = 0; os < kilpparam.ntosuus; os++) {
			if (lahdepisteet[srj][os])
				free(lahdepisteet[srj][os]);
			lahdepisteet[srj][os] = 0;
			if (nkdi[sarjaluku][os] >= 1000000)
				lahdepisteet[srj][os] =
					(lahdepistetp *) calloc(nkdi[srj][os]+nkdi[sarjaluku][os]/1000000+1, sizeof(lahdepistetp));
			else if (nkdi[srj][os])
				lahdepisteet[srj][os] =
					(lahdepistetp *) calloc(nkdi[srj][os]+1, sizeof(lahdepistetp));
			}
		}

	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		bool kaikki = false;

		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (!p)
			continue;
		if (!wcscmpU(p, L"KAIKKI")) {
			srj = sarjaluku;
			kaikki = true;
			}
		else if (!wcscmpU(p, L"LOPUT"))
			srj = sarjaluku;
		else
			srj = haesarja_w(p, false);
		if ((p = wcstok(NULL, L" ;\t\n")) != NULL) {
			if (!wcscmpU(p, L"KAIKKI")) {
				os = 0;
				if (srj >= 0 && srj < sarjaluku)
					osy = Sarjat[srj].ntosuus-1;
				else
					osy = kilpparam.ntosuus-1;
				}
			else {
				os = _wtoi(p) - 1;
				osy = os;
				}
			}
		if (!p || os >= kilpparam.ntosuus)
			continue;
		if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
			if (towupper(*p) == L'M' || (*p == L'0' && _wtoi(p) == 0))
				pst = 32767;
			else {
				pst = _wtoi(p);
				if (pst > kilpparam.valuku)
					pst = 0;
				}
			while (pst > 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				if ((p1 = wcsstr(p, L"-")) > NULL) {
					*p1 = 0;
					kdi = _wtoi(p);
					kdi2 = _wtoi(p1+1);
					}
				else {
					kdi = _wtoi(p);
					kdi2 = kdi;
					}
				while (kdi > 0 && kdi <= kdi2) {
					for (int os1 = os; os1 <= osy; os1++) {
						_swab((char *)&kdi, (char *)&lahdepisteet[srj][os1][nkdi[srj][os1]].kdi, 2);
						_swab((char *)&pst, (char *)&lahdepisteet[srj][os1][nkdi[srj][os1]].piste, 2);
						if (kaikki) {
							for (int srj1 = 0; srj1 < sarjaluku; srj1++) {
								if (lahdepisteet[srj1][os1]) {
									lahdepisteet[srj1][os1][nkdi[srj1][os1]].kdi =
										lahdepisteet[srj][os1][nkdi[srj][os1]].kdi;
									lahdepisteet[srj1][os1][nkdi[srj1][os1]].piste =
										lahdepisteet[srj][os1][nkdi[srj][os1]].piste;
									nkdi[srj1][os1]++;
									}
								}
							}
						nkdi[srj][os1]++;
						}
					kdi++;
					}
				}
			}
		}

	delete infile;

	for (srj = 0; srj <= sarjaluku; srj++) {
		for (os = 0; os < kilpparam.ntosuus; os++) {
			if (nkdi[srj][os] < 1)
				continue;
			quicksort((char *)(lahdepisteet[srj][os]), nkdi[srj][os], 4, 4);
			for (int i = 0; i < nkdi[srj][os]; i++) {
				_swab((char *)&lahdepisteet[srj][os][i].kdi, (char *)&kdi, 2);
				_swab((char *)&lahdepisteet[srj][os][i].piste, (char *)&pst, 2);
				if (pst == 32767)
					pst = 0;
				lahdepisteet[srj][os][i].kdi = kdi;
				lahdepisteet[srj][os][i].piste = pst;
				}
			}
		}
	}

int haelahdepiste(kilptietue *kilp, int os, INT16 kdi, int tm)
	{
	lahdepistetp *kp;
	int yos, srj = 0;

	if (kilp)
		srj = kilp->sarja;
	yos = os;
	if ((sarjaluku == 1 || kilp) && os < Sarjat[srj].osuusluku)
		yos = Sarjat[srj].yosuus[os];
	if (kilp && lahdepisteet[srj][yos]) {
		for (kp = lahdepisteet[srj][yos]; kp->kdi; kp++) {
			if (kp->kdi == kdi) {
				while (kp[1].kdi == kdi && kilp->Maali(os, kp->piste) != TMAALI0 && NORMTULOS(tm-kilp->Maali(os, kp->piste)) > samapisteraja)
					kp++;
				return(kp->piste);
				}
			}
		}
	else if (lahdepisteet[sarjaluku][yos]) {
		for (kp = lahdepisteet[sarjaluku][yos]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				return(kp->piste);
			}
		}
	return(-2);
	}

INT lahdejonohaku = 0;

void luelahdejonot(void)
	{
	TextFl * infile;
	wchar_t *p, *p1, Buf[200];
	INT16 srj, os, pst, kdi, kdi2;
	wchar_t flnm[20] = {L"LAHDEJONOT.LST"};
	int nkdi[MAXSARJALUKU+1][MAXOSUUSLUKU+1];
	static bool luettu;

	if (luettu)
		return;
	luettu = true;
	infile = new TextFl(flnm, L"rt");
	if (!infile)
		return;
	if (!infile->IsOpen()) {
		delete infile;
		return;
		}
	lahdejonohaku = 1;
	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (!p)
			continue;
		if (!wcscmpU(p, L"KAIKKI"))
			srj = sarjaluku;
		else
			srj = haesarja_w(p, false);
		if ((p = wcstok(NULL, L" ;\t\n")) != NULL) {
			if (!wcscmpU(p, L"KAIKKI"))
				os = MAXOSUUSLUKU;
			else
				os = _wtoi(p) - 1;
			}
		if (!p || (os >= kilpparam.osuusluku && os != MAXOSUUSLUKU))
			continue;
		if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
			pst = _wtoi(p);
			if (pst > MAXJONO)
				pst = 0;
			while (pst > 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				if ((p1 = wcsstr(p, L"-")) > NULL) {
					*p1 = 0;
					kdi = _wtoi(p);
					kdi2 = _wtoi(p1+1);
					if (kdi > 0 && kdi2 >= kdi)
						nkdi[srj][os] += kdi2-kdi+1;
					}
				else
					if (_wtoi(p) > 0)
						nkdi[srj][os]++;
				}
			}
		}
	infile->Rewind();
	for (srj = 0; srj <= sarjaluku; srj++) {
		for (os = 0; os <= MAXOSUUSLUKU; os++) {
			if (lahdejonot[srj][os])
				free(lahdejonot[srj][os]);
			lahdejonot[srj][os] = 0;
			if (nkdi[srj][os])
				lahdejonot[srj][os] = (lahdepistetp *) calloc(nkdi[srj][os]+1, sizeof(lahdepistetp));
			}
		}
	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (!p)
			continue;
		if (!wcscmpU(p, L"KAIKKI"))
			srj = sarjaluku;
		else
			srj = haesarja_w(p, false);
		if ((p = wcstok(NULL, L" ;\t\n")) != NULL) {
			if (!wcscmpU(p, L"KAIKKI"))
				os = MAXOSUUSLUKU;
			else
				os = _wtoi(p) - 1;
			}
		if (!p || (os >= kilpparam.osuusluku && os != MAXOSUUSLUKU))
			continue;

		if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
			pst = _wtoi(p);
			if (pst > MAXJONO)
				pst = 0;
			while (pst > 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				if ((kdi =_wtoi(p)) > 0) {
					if ((p1 = wcsstr(p, L"-")) > NULL) {
						*p1 = 0;
						kdi = _wtoi(p);
						kdi2 = _wtoi(p1 + 1);
						for (; kdi <= kdi2; kdi++) {
							_swab((char *)&kdi, (char *)&lahdejonot[srj][os][nkdi[srj][os]].kdi, 2);
							_swab((char *)&pst, (char *)&lahdejonot[srj][os][nkdi[srj][os]].piste, 2);
							nkdi[srj][os]++;
							}
						}
					else {
						_swab((char *)&kdi, (char *)&lahdejonot[srj][os][nkdi[srj][os]].kdi, 2);
						_swab((char *)&pst, (char *)&lahdejonot[srj][os][nkdi[srj][os]].piste, 2);
						nkdi[srj][os]++;
						}
					}
				}
			}
		}

	delete infile;

	for (srj = 0; srj <= sarjaluku; srj++) {
		for (os = 0; os <= MAXOSUUSLUKU; os++) {
			if (nkdi[srj][os] < 1)
				continue;
			quicksort((char *)(lahdejonot[srj][os]), nkdi[srj][os], 4, 4);
			for (int i = 0; i < nkdi[srj][os]; i++) {
				_swab((char *)&lahdejonot[srj][os][i].kdi, (char *)&kdi, 2);
				_swab((char *)&lahdejonot[srj][os][i].piste, (char *)&pst, 2);
				lahdejonot[srj][os][i].kdi = kdi;
				lahdejonot[srj][os][i].piste = pst;
				}
			}
		}
	}

static int haelahdejono(kilptietue *kilp, int os, INT16 kdi)
	{
	lahdepistetp *kp;
	int jono;

	jono = emitjono;
	if (kilp && lahdejonot[kilp->sarja][os]) {
		for (kp = lahdejonot[kilp->sarja][os]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste-1;
			}
		}
	else if (kilp && lahdejonot[kilp->sarja][MAXOSUUSLUKU]) {
		for (kp = lahdejonot[kilp->sarja][MAXOSUUSLUKU]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste-1;
			}
		}
	else if (lahdejonot[sarjaluku][os]) {
		for (kp = lahdejonot[sarjaluku][os]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste-1;
			}
		}
	else if (lahdejonot[sarjaluku][MAXOSUUSLUKU]) {
		for (kp = lahdejonot[sarjaluku][MAXOSUUSLUKU]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste-1;
			}
		}
	if (jono < 0)
		jono = 0;
	return(jono);
	}

void tall_etulos(INT32 badge, INT32 t, INT32 tms, int r_no, int lahde)
   {
   INT kno, os, osuus = -1, osrj, d, piste = -2, toisto = 0, ino = 0;
   kilptietue kilp, entkilp;
   INT32 t_ed;
   ratatp *rt = 0;
   aikatp itm;
   char msg[80], as[20];
	bool blUusinaika = false;

   kno = bdg2kno(badge, 0);
   if (kno == 0 && kilpparam.kaksibadge) {
	   int toinen = 1;
	   kno = bdg2kno(badge, &toinen);
	   }
   //   t = T_TIME(t);
   memset(&itm, 0, sizeof(itm));
   itm.t = (tms-43200000L) * 10L;
   t = purajak(itm.t);
//   itm.t = t + AIKAJAK*aikajono[ino]->aikakorjaus;
   itm.date = tm_date(itm.t);
   itm.badge = badge;
   itm.lahde = lahde;
   itm.kanava = r_no+1;
   if ((d = getpos(kno)) > 0) {
	  EnterCriticalSection(&tall_CriticalSection);
	  GETREC(&kilp, d);
	  memcpy(&entkilp, &kilp, sizeof(kilptietue));
	  if (ol_osuus) {
		 if (kilp.ostiet[ol_osuus-1].badge[0] == badge ||
			(kilpparam.kaksibadge && kilp.ostiet[ol_osuus-1].badge[1] == badge))
			osuus = ol_osuus-1;
		 }
	  else if (os_raja == -2) {
		 if (kilp.ostiet[Sarjat[kilp.sarja].osuusluku-1].badge[0] == badge ||
			(kilpparam.kaksibadge && kilp.ostiet[Sarjat[kilp.sarja].osuusluku-1].badge[1] == badge))
			osuus = Sarjat[kilp.sarja].osuusluku-1;
         }
      else {
         if (os_raja > 0)
            osrj = os_raja;
         else if (os_raja < 0)
            osrj = Sarjat[kilp.sarja].osuusluku-1;
         else
			osrj = Sarjat[kilp.sarja].osuusluku;
         if (osrj > Sarjat[kilp.sarja].osuusluku)
             osrj = Sarjat[kilp.sarja].osuusluku;
         for (os = osrj-1; os >= 0; os--) {
			if (val_korkein) {
			   if (kilp.ostiet[os].badge[0] == badge ||
			      (kilpparam.kaksibadge && kilp.ostiet[os].badge[1] == badge)) {
				  osuus = os;
				  break;
                  }
               }
			else if ((kilp.ostiet[os].badge[0] == badge ||
				  (kilpparam.kaksibadge && kilp.ostiet[os].badge[1] == badge) ||
				  (joustoviesti && kilp.ostiet[os].badge[0] == 0)) &&
			   (emitfl < 0 || EmitJarr(kilp.kilpno, os) < 0) &&
			   kilp.Maali(os, 0) == TMAALI0) {
               osuus = os;
               }
            }
         }
      if (osuus >= 0) {
		 if (r_no < NREGNLY && vainpiste[r_no+1] > -2) {
			piste = vainpiste[r_no+1];
			}
		 else if (vainpiste[0] > -2) {
			piste = vainpiste[0];
			}
		 if (piste < -1) {
			if (lahdepistehaku) {
				piste = haelahdepiste(&kilp, osuus, lahde, t);
				}
			else {
				piste = -3;
				}
            }
		if (piste == -3) {
			piste = 1;
			for (piste = 1; piste <= kilpparam.maxvaluku; piste++)
				if (kilp.Maali(osuus, piste) == TMAALI0)
					break;
			if (piste > kilpparam.maxvaluku)
				piste = kilpparam.valuku+1;
			}
		 if (kilpparam.vuoromaali && piste > Sarjat[kilp.sarja].valuku[Sarjat[kilp.sarja].yosuus[osuus]] && kilp.Maali(osuus, 0) == TMAALI0)
			 piste = 0;
		 itm.kno = kno;
		 itm.osuus = osuus;

		 t_ed = kilp.Maali(osuus, piste);
		 if (uusinaika && t_ed != TMAALI0 && NORMKELLO(t-t_ed) < uusinaika)
			 blUusinaika = true;
		 if ((piste >= 0 && (t_ed == TMAALI0 || blUusinaika)) || piste == kilpparam.valuku) {
			itm.piste = piste;
			}
		 else
			itm.piste = kilpparam.valuku+1;
		 if (lahdejonohaku)
			 ino = haelahdejono(&kilp, osuus, lahde);
		 itm.jono = ino;
		 tall_rivi(ino, &itm, NULL, NULL, 0, 0, 0);
		 if (piste >= 0 && piste <= kilpparam.valuku && (blUusinaika || t_ed == TMAALI0)) {
			kilp.setMaali(osuus, piste, pyoristatls(purajak(itm.t), 1));
			if (joustoviesti && kilp.ostiet[osuus].badge[0] == 0) {
				kilp.ostiet[osuus].badge[0] = badge;
				for (os = Sarjat[kilp.sarja].osuusluku-1; os >= 0; os--) {
					if (kilp.ostiet[os].badge[0] == badge && !kilp.ostiet[osuus].nimi[0] && kilp.ostiet[os].nimi[0]) {
						strcpy(kilp.ostiet[osuus].nimi, kilp.ostiet[os].nimi);
						break;
						}
					}
				}
			tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
			}
		 }
	  LeaveCriticalSection(&tall_CriticalSection);
	  }
   if (osuus < 0) {
	   itm.jono = ino;
	   tall_rivi(ino, &itm, NULL, NULL, 0, 0, 0);
		}
   if (loki) {
	  sprintf(msg, "Lukija-aika %s, bdg: %ld, kno: %d, d: %d, os: %d\n",
		 aikatostr_ls(as, t, t0), badge, kno, d, osuus+1);
	  kirjloki(msg);
	  }
   }

#define E_TLSLUKU 100

typedef struct {
	INT32  t1;
	INT32  t;
	UINT32 badge;
	INT    r_no;
	int lahde;
	} e_tulostp;

static e_tulostp *etls;
static int etls_head, etls_tail;
static CRITICAL_SECTION etls_CriticalSection;

void close_bdg_t(void)
   {
	DeleteCriticalSection(&etls_CriticalSection);
	if (etls)
		free(etls);
	}

static void tall_bdg_t(UINT32 badge, INT32 t, INT32 t1, int r_no, int lahde)
{
	int i, seuraava;
	regnlyhetki[r_no] = t_time_l(biostime(0,0), t0);
	if (kaikki_ajat[r_no+1] && !ohitatoisto) {
		tall_etulos(badge, t, t1, r_no, lahde);
		}
	seuraava = 0;
	for (i = 0; i < 8; i++) {
		if (bdg_t[i].badge == badge && t - bdg_t[i].t < 180) {
			seuraava = 1;
			break;
			}
		if (!bdg_t[i].badge) break;
		}
	if (seuraava)
		return;
	if (i >= 8) {
		memmove(bdg_t, bdg_t+1, 7 * sizeof(bdg_t_tp));
		i = 7;
		}
	bdg_t[i].badge = badge;
	bdg_t[i].t = t;
#ifndef _CONSOLE
	if (ToimintaTila == 1)
		i = 7; // Täytettä
	else
#endif
	if (kaikki_ajat[r_no+1] && ohitatoisto) {
		tall_etulos(badge, t, t1, r_no, lahde);
		}
}

static void pros_bdg_t(LPVOID lpCn)
   {
   INT r_no, lahde;
   INT32 t, t1;
	UINT32 badge;

	while (taustaon && timerfl) {
		while (etls_head != etls_tail) {
		    EnterCriticalSection(&etls_CriticalSection);
			etls_tail = (etls_tail+1) % E_TLSLUKU;
			t1 = etls[etls_tail].t1;
			t = etls[etls_tail].t;
			badge = etls[etls_tail].badge;
			r_no = etls[etls_tail].r_no;
			lahde = etls[etls_tail].lahde;
			LeaveCriticalSection(&etls_CriticalSection);

			tall_bdg_t(badge, t, t1, r_no, lahde);

			}
		Sleep(50);
		}
	close_bdg_t();
   }

void add_bdg_t(UINT32 badge, INT r_no, char byte9, UINT32 tms)
	{
	INT32 t, t1;
	INT16 lahde = 0, r_no2;
	static int init = 1;
	static HTHREAD hEtlsThread;
	static UINT32 edbadge[NREGNLY+256];

	if (!timerfl)
		return;
	if (init) {
		init = 0;
		etls = (e_tulostp *) calloc(E_TLSLUKU, sizeof(e_tulostp));
	   InitializeCriticalSection(&etls_CriticalSection);
      hEtlsThread = _beginthread(pros_bdg_t, 40960, &init);
		}
	r_no2 = r_no;
	if (lahdepistehaku) {
		if (lukijalahde[r_no])
			lahde = lukijalahde[r_no];
		else if (vainpiste[r_no+1] <= -2 && vainpiste[0] <= -2) {
			lahde = byte9;
			r_no2 = NREGNLY + lahde;
			}
		}
	if (badge == edbadge[r_no2])
		return;
	edbadge[r_no2] = badge;
	if (tms)
		t1 = tms;
	else
		t1 = msdaytime();
	t = biostime(0,0);
	EnterCriticalSection(&etls_CriticalSection);
	etls_head = (etls_head+1) % E_TLSLUKU;
	if (etls_tail == etls_head)
		etls_tail = (etls_head+1) % E_TLSLUKU;
	etls[etls_head].t1 = t1;
	etls[etls_head].t = t;
	etls[etls_head].badge = badge;
	etls[etls_head].r_no = r_no;
	etls[etls_head].lahde =  lahde;
    LeaveCriticalSection(&etls_CriticalSection);
	}

static INT32 hae_bdg_t(UINT32 badge)
   {
   INT i;
   INT32 t;

   for (i = 0; i < N_BDG_T; i++) {
      if (bdg_t[i].badge == badge) break;
      }
   if (i >= N_BDG_T)
      t = biostime(0,0);
   else {
      t = bdg_t[i].t;
      }
   return(T_TIME(t));
   }

#ifdef EMITFILE
int r_no_file;
   static int lukuvali = 0;

int file2emit(void)
   {
   static FILE *emitinfile;
   emittp em;
   san_type vastaus;
   long tm;
   int i;

   if (!emitinfile) {
	  emitinfile = fopen("EMIT_IN.DAT", "rb");
	  }
   if (!emitinfile)
	  return(1);
   do
	  {
	  if (!fread(&em, sizeof(emittp), 1, emitinfile)) {
		 writeerror("Kaikki tiedot luettu", 0);
		 return(1);
		 }
	  memset(&vastaus, 0, sizeof(vastaus));
	  vastaus.r13.pkgno = em.package;
	  memcpy(vastaus.r13.badge, (char *) &em.badge, 3);
	  vastaus.r13.week = (UCHAR) em.badgeweek;
	  vastaus.r13.year = (UCHAR) em.badgeyear;
	  tm = em.time*SEK/10 - t0_regnly + 48*TUNTI;
	  if (emitfiletest && taustaon) {
		 if (!lukuvali) {
			while (taustaon) {
			   if ((tm - T_TIME(biostime(0,0))*KSEK + 12*TUNTI)%(24*TUNTI) < 12*TUNTI)
				  break;
			   else
				  Sleep(50);
			   }
			}
		 else
			Sleep(100*lukuvali);
		 }
	  vastaus.r13.t_hour = (char) (tm/36000L);
	  tm = tm - TUNTI * vastaus.r13.t_hour;
	  vastaus.r13.t_hour = vastaus.r13.t_hour % 24;
	  vastaus.r13.t_min = (char) (tm/MINUUTTI);
	  tm = tm - MINUUTTI * vastaus.r13.t_min;
	  vastaus.r13.t_sek = (char) (tm/SEK);
	  tm = tm - SEK * vastaus.r13.t_sek;
	  vastaus.r13.t_ms = (UINT16) (tm*1000/SEK);
	  for (i = 0; i < 50; i++) {
		 vastaus.r13.c[i].code = em.ctrlcode[i];
		 vastaus.r13.c[i].t1 = em.ctrltime[i];
		 }
	  tall_emit(&vastaus, 0, r_no_file);
	  }
   while (emitfiletest && taustaon);
   return(0);
   }

void file2emitThread(LPVOID lpCn)
	{
	file2emit();
	}

void seur_mtr(void)
   {
   static HTHREAD fl2EmitThread;
   char ch;

   if (testi == 1)
	  emitfiletest = 1;
   if (emitfiletest) {
	  ch = ' ';
	  selectopt("Lukemisajat T)iedostosta EMIT_IN.DAT vai V)akiovälein", "TV", &ch);
	  if (ch == 'V') {
		 clrln(ySize-3);
		 vidspmsg(ySize-3, 0, 7, 0, "Anna aikaväli sekunnin kymmenyksinä");
		 INPUTINT(&lukuvali, 5, 37, ySize-3, "\r", &ch);
		 }
	  else
		 lukuvali = 0;
	  }
   fl2EmitThread = _beginthread(file2emitThread, 20480, 0);
   }
#endif

#else  // LUENTA
#ifdef EI_OLE

static emittp l_em;

INT addem(emittp *em, INT kielto)
	{
	l_em = *em;
	emithead = 1;
	emithead2 = 0;
   return(0);
	}

INT lueem(emittp *em, INT ep)
	{
	*em = l_em;
	emithead = 0;
	emithead2 = 0;
	return(0);
	}

#endif
#endif // !LUENTA

void addluenta(emittp *em)
{
	uusi_emit = em->badge;
}

static int samatleimat(emittp *em1, emittp *em2, int regnlaji)
{
	int i;

	for (i = 0; i < 50; i++) {
		if (((em1->ctrlcode[i] > 0 && em1->ctrlcode[i] < 240) ||
			(em2->ctrlcode[i] > 0 && em2->ctrlcode[i] < 240)) &&
			(em1->ctrlcode[i] != em2->ctrlcode[i] ||
			 em1->ctrltime[i] != em1->ctrltime[i]))
			 return(FALSE);
		}
	return(TRUE);
}

INT tall_emit(san_type *vastaus, UINT32 *vahvistus, INT r_no)
   {
   INT i, smm = 0, ssek = 0, sosat = 0;
   emittp em;
//	int emittime = 0;
static emittp *ed_em[NREGNLY];
#ifdef AJANOTTO
   INT mm = 0, sek = 0, osat = 0;
#endif

	regnlyhetki[r_no] = T_TIME(biostime(0,0));
	if (!ed_em[r_no]) {
	  if ((ed_em[r_no] = (emittp *) malloc(sizeof(emittp))) == 0) {
		 writeerror("Muisti ei riitä Emit-tiedoille", 0);
		 return(1);
		 }
	  }
   memset(&em, 0, sizeof(emittp));
   em.time = TMAALI0*10/SEK;
   em.maali = TMAALI0*10/SEK;
   em.kirjaus = TMAALI0*10/SEK;
   if (regnly[r_no] == 11) {
	  if (vastaus->r11.tunnus != 'K' && vastaus->r11.tunnus != 'L' ||
		 vastaus->r11.cr != 13)
		 return(1);
	  vastaus->r11.loppu[3] = 13;
	  vastaus->r11.loppu[4] = 10;
	  vastaus->r11.cr = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
#ifdef AJANOTTO
	  if (vastaus->r11.tunnus == 'K') {
		 vastaus->r11.kp = 0;
		 mm = atoi(vastaus->r11.min);
		 vastaus->r11.p = 0;
		 sek = atoi(vastaus->r11.sek);
		 vastaus->r11.osat[2] = 0;
		 osat = atoi(vastaus->r11.osat);
		 em.maali = osat/10 + 10L*sek + 600L*mm;
		 if (em.maali) em.maali += t0_regn[r_no]/KSEK;
		 else em.maali = TMAALI0/KSEK;
		 }
#endif
	  vastaus->r11.skp = 0;
	  smm = atoi(vastaus->r11.smin);
	  vastaus->r11.sp = 0;
	  ssek = atoi(vastaus->r11.ssek);
	  vastaus->r11.sosat[2] = 0;
	  sosat = atoi(vastaus->r11.sosat);
	  vastaus->r11.min[0] = 0;
	  em.package = 10000000L + atol(vastaus->r11.badge);
	  em.badge = UL2CH(vastaus->r11.u[0]) + 256L*UL2CH(vastaus->r11.u[1])
		 + 65536L*UL2CH(vastaus->r11.u[2]);
	  em.badgeweek = UL2CH(vastaus->r11.u[4]);
	  em.badgeyear = UL2CH(vastaus->r11.u[5]);
	  for (i = 0; i < 50; i++) {
		 em.ctrlcode[i] = UL2CH(vastaus->r11.u[8+3*i]);
		 em.ctrltime[i] = (unsigned) (UL2CH(vastaus->r11.u[9+3*i]) +
			256L*UL2CH(vastaus->r11.u[10+3*i]));
//         if (em.ctrltime[i] > ennakko) em.ctrltime[i] -= ennakko;
		 em.time = sosat/10 + 10L*ssek + 600L*smm + t0_regn[r_no]/KSEK;
		 }
	  }
   if (regnly[r_no] == 12) {
	  em.package = 20000000L + r_no;
	  for (i = 0; i < r_msg_len[r_no]; i++)
		 vastaus->bytes[i] ^= '\xdf';
	  em.badge = *(UINT32 *) vastaus->r12.badge & 0xffffffL;
	  em.badgeweek = vastaus->r12.week;
	  em.badgeyear = vastaus->r12.year;
#if !defined(LUENTA)
	  em.time = hae_bdg_t(em.badge);
#endif
	  em.maali = TMAALI0*10/SEK;
	  for (i = 0; i < 50; i++) {
		 em.ctrlcode[i] = vastaus->r12.c[i].code;
		 em.ctrltime[i] = vastaus->r12.c[i].t1;
		 }
	  if (vastaus->r12.chl == 'L') {
		 vastaus->r12.fill4 = 0;
		 em.lc = atoi(vastaus->r12.cl);
		 }
	  if (vastaus->r12.chp == 'P') {
		 vastaus->r12.chl = 0;
		 em.pc = atoi(vastaus->r12.cp);
		 }
	  if (vastaus->r12.chs == 'S') {
		 vastaus->r12.chp = 0;
		 em.sc = atoi(vastaus->r12.cs);
		 }
	  }
   if (regnly[r_no] == 13) {
	  em.package = vastaus->r13.pkgno;
	  em.badge = *(UINT32 *) vastaus->r13.badge & 0xffffffL;
	  em.badgeweek = vastaus->r13.week;
	  em.badgeyear = vastaus->r13.year;
	  em.time = vastaus->r13.t_ms/100 + 10L*vastaus->r13.t_sek +
		 600L*vastaus->r13.t_min + 36000L*vastaus->r13.t_hour + t0_regn[r_no]/KSEK;
	  em.maali = TMAALI0*10/SEK;
	  for (i = 0; i < 50; i++) {
		 em.ctrlcode[i] = vastaus->r13.c[i].code;
		 em.ctrltime[i] = vastaus->r13.c[i].t1;
		 }
	  if (vastaus->r13.chl == 'L') {
		 vastaus->r13.fill4 = 0;
		 em.lc = atoi(vastaus->r13.cl);
		 }
	  if (vastaus->r13.chp == 'P') {
		 vastaus->r13.chl = 0;
		 em.pc = atoi(vastaus->r13.cp);
		 }
	  if (vastaus->r13.chs == 'S') {
		 vastaus->r13.chp = 0;
		 em.sc = atoi(vastaus->r13.cs);
		 }
	  }
   if (regnly[r_no] == 14) {
		INT32 emtm;
		tulk_emiTag(vastaus->bytes+1, &em, &emtm, r_no);
		if (emittime && emtm != TMAALI0*(1000/SEK))
			em.time = emtm/100;
#if !defined(LUENTA)
		else
			em.time = hae_bdg_t(em.badge);
#endif
//			em.time = hae_bdg_t(em.badge, uusin)/KSEK;
		if (!em.badge || kaikki_ajat[r_no+1] == 2)
			return(0);
		// Vaadi että leimojen lopussa on lukijakoodi
		for (i = MAXNLEIMA-1; i > 1; i--)
			if (em.ctrlcode[i] > 0)
				break;
		if (IntInList(em.ctrlcode[i], L"240 243 244 250 253") < 0 &&
			IntInList(em.ctrlcode[i-1], L"240 243 244 250 253") < 0)
			return(0);
	  em.maali = TMAALI0*10/SEK;
	  }
   if (em.badge == 0) {
	   return(0);
	   }
   em.badge0 = em.badge;
#ifndef _CONSOLE
   if (emittallfile) {
	   fwrite(&em, sizeof(em), 1, emittallfile);
	   n_emittallfile++;
	   return(0);
	   }
#endif
   if (em.badge && em.badge == ed_em[r_no]->badge &&
		samatleimat(&em, ed_em[r_no], regnly[r_no])) {
	  if (vahvistus) *vahvistus = em.badge;
	  }
   else {
	  *(ed_em[r_no]) = em;
		if (sisaanluenta || RastiVaTuloste) {
			addluenta(&em);
			return(0);
			}
	if (ySize == 50)
		naytaluenta(em.badge);
#ifdef LUENTA
	  uusi_emit = em.badge;
#else
	  addem(&em, 0);
#endif
	  }
   return(0);
   }

#ifndef LUENTA
#ifdef _CONSOLE
INT pros_emit(wchar_t *msg1)
   {
   static INT in_pros_emit;
   INT i, piste = 0, d = -1, kno, tallkilp = 0, os = 0, korjaus_fl = 0;
   INT kh = 0, j, vkoodi, ino = 0, rm;
   char ch = ' ', msg[91];
   char *svscr;
   emittp em;
   kilptietue kilp, entkilp;
   ratatp *rt;
   aikatp itm, etm;
   int ptm;
   bool tallfl = false;
#ifdef JOUKKUEEMIT
	int tulkinta[50];
	INT32 enn;
#endif

   if (!ProsEmitOn || in_pros_emit || emithead2 >= emithead || (testi == 1 && !emitfiletest))
	  return(1);
   in_pros_emit = 1;
   lueem(&em, emithead2);
   if (em.kirjaus != TMAALI0/KSEK) {
	  emithead2++;
      in_pros_emit = 0;
	  return(1);
      }

   em.kirjaus = T_TIME(biostime(0,0));
   vkoodi = 0;
#ifdef JOUKKUEEMIT
	if (joukkueemit) {
		if (!jalkisyotto) {
	      if (em.badge && (kno = bdg2kno(em.badge)) != 0) {
				d = getpos(kno);
				}
			else
				writeerror("Tuntematon Emit-koodi", 0);
			}
		if (d < 1) {
			for (;;) {
			INPUTINT(&kno, 4, 13, ySize-3, "%\r\x1b", &ch);
			 if (ch == ESC)
		         break;
            if ((d = getpos(kno)) < 1) {
               writeerror("Tuntematon joukkuenumero", 0);
               continue;
			   }
            GETREC(&kilp, d);
	         memcpy(&entkilp, &kilp, sizeof(kilptietue));
		      if (kilp.ostiet[0].badge[0]) {
			      ch = 'E';
				   selectopt("Joukkueella jo emit-koodi. Tallennetaanko silti (K/E)", "KE", &ch);
					if (ch == 'E')
						continue;
					}
		      if ((j = bdg2kno(em.badge)) != 0 && j != kno) {
				  remjkbadge(j, em.badge);
				   }
	         kilp.ostiet[0].badge[0] = em.badge;
			  tallenna(&kilp,d,0,0,0,&entkilp);
				break;
				}
			}
		if (d > 0) {
			em.kilpno = kno;
			em.osuus = 0;
			putem(&em, emithead2, 0);
			EnterCriticalSection(&tall_CriticalSection);
			GETREC(&kilp, d);
			tarkista(&em, &kilp, tulkinta, -1, 0);
			for (i = 49; i > 0; i--) {
				if (tulkinta[i]) {
					os = tulkinta[i];
					if (os < 0)
						os = -os;
					os--;
					enn = haeennakko(&em, &kilp, 0);
					if (kilp.Maali(os, 0) == TMAALI0)
						kilp.Maali(os, 0) =
							NORMKELLO(Sarjat[kilp.sarja].lahto + SEK*(em.ctrltime[i]-enn));
					}
				}
			tallenna(&kilp,d,0,0,0,0);
			setEmitJarr(em.kilpno, 0, emithead2);
			LeaveCriticalSection(&tall_CriticalSection);
			}
		else
			putem(&em, emithead2, 0);
	  emithead2++;
	  in_pros_emit = 0;
	  return(0);
		}
#endif
   if (jalkisyotto) {
      clrln(ySize-3);
	  kno = 0;
	  os = -1;
      do {
		 vidspmsg(ySize-3,0,7,0,"Anna joukkue      ja osuus         tai lue viivakoodi,  Esc: Ohita kortti");
         if (emitfiletest)
			ch = ESC;
		 else
			   naytatulos(3);
			INPUTINT(&kno, 4, 13, ySize-3, "%\r\x1b", &ch);
         if (ch == ESC)
			break;
         if (ch == '%') {
            vkoodi = 1;
            fgets(msg, 20, stdin);
	        if (strlen(msg) > 1 && msg[strlen(msg)-1] == '\n')
	           msg[strlen(msg)-1] = 0;
            if (strlen(msg) == 5) {
               os = msg[4]-'0';
			   msg[4] = 0;
               kno = atol(msg);
               }
			sprintf(msg, "Joukkue %d, osuus %d,  H)yväksy, M)uuta, Esc: Peruuta", kno, os);
            ch = 'H';
			selectopt(msg, "HM\x1b", &ch);
            clrln(ySize-3);
            if (ch == 'M')
               continue;
			if (ch == ESC)
               break;
			if ((d = getpos(kno)) < 1) {
               writeerror("Tuntematon joukkuenumero", 0);
               continue;
               }
            GETREC(&kilp, d);
            }
         else {
			if ((d = getpos(kno)) < 1) {
               writeerror("Tuntematon joukkuenumero", 0);
               continue;
               }
			GETREC(&kilp, d);
            if (os < 0) {
			   for (os = 0; os < Sarjat[kilp.sarja].osuusluku-1 && !kilp.ostiet[os].badge[0]; os++) ;
               os++;
               }
			INPUTINT(&os, 2, 28, ySize-3, "\r\x1b", &ch);
            if (ch == ESC)
			   break;
            }
         os--;
         memcpy(&entkilp, &kilp, sizeof(kilptietue));
		 if (kilp.ostiet[os].badge[0]) {
            ch = 'E';
			selectopt("Osuudella jo emit-koodi. Tallennetaanko silti (K/E)", "KE", &ch);
            if (ch == 'E')
               continue;
            }
		 if ((j = bdg2kno(em.badge, 0)) != 0 && j != kno) {
            remjkbadge(j, em.badge);
			}
         kilp.ostiet[os].badge[0] = em.badge;
		 tallenna(&kilp,d,0,0,0,&entkilp);
         break;
		 } while(1);
	  }
   else {
      if (em.badge && (kno = bdg2kno(em.badge, 0)) != 0) {
		 svscr = savescr(0, 0, ySize-1, 79);
         if (!emitfiletest) 
            {
			clrtxt(0,0,64);
			rm = ySize == 50 ? 41 : ySize-4;
			for (i = 2; i < rm; i++)
               clrtxt(i,0,64);
			}
         d = getpos(kno);
         GETREC(&kilp, d);
         memcpy(&entkilp, &kilp, sizeof(kilptietue));
		 for (os = Sarjat[kilp.sarja].osuusluku-1; os >= 0; os--) {
			if (kilp.ostiet[os].badge[0] == em.badge) break;
            }
		 if (esitark >= 0 && os >= 0 && EmitJarr(kno, os) != -1
			|| (huomaatark && stschind(kilp.Tark(os, 0), "TIH") >= 0)) {
			if (kaikki_ajat[0] || emitfiletest) {
			   if (!emitfiletest)
               writeerror("Emit-kortille ei sopivaa osuutta vapaana", 2000);
               }
            else {
				   naytatulos(3);
               sprintf(msg, "Emit-kortille %ld ei sopivaa osuutta vapaana",
                  em.badge);
               i = etsiesarja(&em);
			   for (j = 0; j < Sarjat[kilp.sarja].osuusluku; j++) {
				  if (!strcmpU(rata[i].rata, kilp.ostiet[j].hajonta))
					 break;
                  }
               vidspmsg(ySize-6,0,7,0,msg);
               sprintf(msg, "Luettu aiemmin osuudelle %d-%d", kno, os+1);
			   if (j < Sarjat[kilp.sarja].osuusluku) {
                  sprintf(msg+strlen(msg), " Rata osuudelta %d", j+1);
                  }
               vidspmsg(ySize-5,0,7,0,msg);
               ch = ' ';
               selectopt("Lue S)amalle osuudelle, T)oiselle osuudelle, O)hita   (tai lue viivakoodi)",
				  "STO%", &ch);
               if (ch == 'S') {
                  ch = ' ';
                  selectopt("Vaihdetaanko aiempi tieto muulle osuudelle (K/E)", "KE", &ch);
                  if (ch == 'K') {
					 vaihdakilp(kno, os, 1);
                     }
				  ch = ' ';
                  }
               if (ch == '%') {
                  vkoodi = 1;
                  fgets(msg, 20, stdin);
				  if (strlen(msg) > 1 && msg[strlen(msg)-1] == '\n')
					 msg[strlen(msg)-1] = 0;
                  if (strlen(msg) == 5) {
					 i = msg[4]-'0';
                     msg[4] = 0;
					 j = atol(msg);
					 }
                  sprintf(msg, "Joukkue %d, osuus %d,  H)yväksy, M)uuta, Esc: Peruuta", j, i );
                  clrln(ySize-3);
                  ch = 'H';
                  selectopt(msg, "HM\x1b", &ch);
				  if (ch == 'H') {
                     if (j != kno) {
                        if ((d = getpos(j)) > 0) {
                            remjkbadge(kno, em.badge);
                            kno = j;
                            d = getpos(kno);
                            GETREC(&kilp,d);
                            }
                        else {
                           writeerror("Tuntematon joukkue", 0);
                           ch = ESC;
                           }
						}
                     }
				  if (ch == 'M')
                    ch = 'T';
				  if (ch == ESC) {
                   ch = 'O';
                   }
                  }
               if (ch == 'T' || ch == 'H') {
                  if (ch != 'H')
                     i = 0;
                  do {
					 if (ch != 'H') {
						clrln(ySize-3);
                        vidspmsg(ySize-3,0,7,0,"Anna osuus          Esc: Ohita kortti");
                        INPUTINT(&i, 2, 13, ySize-3, "\r\x1b", &ch);
                        }
					 if (ch == ESC) {
                        ch = 'O';
                        break;
                        }
                     if (i < 1 || i > Sarjat[kilp.sarja].osuusluku) {
                        writeerror("Virheellinen osuus", 0);
                        ch = 0;
                        continue;
                        }
					 i--;
					 if (EmitJarr(kno, i) != -1) {
						writeerror("Osuudelle jo luettu Emit-tiedot", 0);
						ch = 0;
						i++;
						continue;
                        }
                     if (ch == 'H')
                        ch = KB_CR;
                     } while(ch != KB_CR);
				  if (ch == '\r') {
					 em.badge = haeVapaaKoodi(em.badge);
					 kilp.ostiet[i].badge[0] = em.badge;
					 tallenna(&kilp,d,0,0,0,&entkilp);
                     }
				  }
			   }
            }
         }
      }
   if (em.badge && !bdg2kno(em.badge, 0) && !emitfiletest) {
#ifdef ESITARK
	  if (esitark < 0) {
         clrtxt(0,0,64);
		 for (i = 2; i < ySize-4; i++)
            clrtxt(i,0,64);
         sprintf(msg, "Tuntematon Emit-koodi %7ld.", em.badge);
			vidspmsg(4,0,0,7,msg);
		 writeerror(msg, 0);
			if (loki) {
				strcat(msg, "\r\n");
				kirjloki(msg);
				}
         putem(&em, emithead2, 0);
		 emithead2++;
         in_pros_emit = 0;
         return(0);
         }
#endif
      kno = 0;
	   naytatulos(3);
      erbeep();
      svscr = savescr(0, 0, ySize-1, 79);
      clrtxt(0,0,64);
      for (i = 2; i < ySize-4; i++)
         clrtxt(i,0,64);
	  do {
         ch = ' ';
		 if (kaikki_ajat[0]) {
            kbflush();
			sprintf(msg,
               "Uusi Emit-koodi%6ld. Paina viestin poistuttua Enter "
			   "käsitelläksesi", em.badge);
			writeerror(msg, 2000);
            utsleep(36);
			ch = readkbd(&ch, 0, 0);
            if (ch == '\r') ch = ESC;
            else ch = ' ';
            }
         if (ch == ' ') {
            sprintf(msg, "Uusi Emit-koodi %d Anna K)ilpailijanro, H)ae, "
			   "Esc:ohita (tai lue viivakoodi)", em.badge);
            selectopt(msg, "KH%\x1b", &ch);
			}
         if (ch == '%') {
            vkoodi = 1;
            fgets(msg, 20, stdin);
	        if (strlen(msg) > 1 && msg[strlen(msg)-1] == '\n')
			   msg[strlen(msg)-1] = 0;
			if (strlen(msg) == 5) {
               os = msg[4]-'0';
			   msg[4] = 0;
               kno = atol(msg);
			   }
			sprintf(msg, "Joukkue %d, osuus %d,  H)yväksy, M)uuta, Esc: Peruuta", os, kno );
            clrln(ySize-3);
            ch = 'H';
			selectopt(msg, "HM\x1b", &ch);
            if (ch == 'H')
               ch = 'T';
            if (ch == 'M')
			   ch = 'K';
            if (ch == ESC) {
               kno = 0;
               }
			}
         switch (ch) {
            case 'K':
            case 'T':
               clrln(ySize-3);
               for(;;) {
                  if (ch != 'T') {
                     sprintf(msg, "Anna korttia %6ld vastaava kilpailijanumero:         "
                        "Esc: Peruuta", em.badge);
                     vidspmsg(ySize-3,0,7,0, msg);
                     INPUTINT(&kno, 4, 47, ySize-3, "\r\x1b", &ch);
                     if (ch == 27) {
						break;
                        }
					 }
                  if ((d = getpos(kno)) < 1) {
					 writeerror("Tuntematon kilpailijanumero", 0);
					 }
                  else {
					 if (ch != 'T') {
                        vidspmsg(ySize-3,51,7,0,
                           "  Anna osuus:       ");
                        os++;
						INPUTINT(&os, 2, 66, ySize-3, "\r", &ch);
                        }
					 os--;
                     }
				  if (EmitJarr(kno, os) != -1) {
                     writeerror("Kilpailijan tiedot jo tallennettu. "
                        "Tarkista numero ja osuus.", 0);
                     }
				  else break;
                  }
			   break;
			case 'H':
               do {
				  if (find(&d, &kilp, &os)) {
                     ch = ESC;
                     break;
                     }
                  kno = kilp.kilpno;
				  sprintf(msg, "No %d, osuus %d.  H)yväksy valinta, U)usi "
					 "valinta, Esc : Peruuta", kno, os+1);
				  selectopt(msg, "HU\x1b", &ch);
				  } while (ch == 'U');
               break;
			}
		 if (ch == 27) {
            kno = 0;
            break;
            }
         else {
            GETREC(&kilp, d);
			entkilp = kilp;
//            memcpy(&entkilp, &kilp, sizeof(kilptietue));
/*
			if (kilp.ostiet[os].badge && kilp.ostiet[os].badge[0] != em.badge) {
               ch = ' ';
               selectopt("Kilpailijalle tallennettu toinen EMIT-koodi, "
				  "vaihdetaanko (K/E)", "KE", &ch);
               }
            else 
*/
			if (ch != 'L' && !vkoodi) {
               ch = ' ';
			   selectopt("Haluatko muokata kilpailijan tietoja (K/E)", "KE",
                  &ch);
			   if (ch == 'K') {
                  outkilp(&kilp, 0);
                  korjaa(d, &kilp, &os, 1, &entkilp);
				  GETREC(&kilp, d);
                  kno = kilp.kilpno;
				  }
               ch = 'L';
               }
            }
         } while (ch != 'K' && ch != 'L');
	  restorescr(0, 0, ySize-1, 79, svscr);
      if (!kno) {
         putem(&em, emithead2, 0);
		 emithead2++;
         in_pros_emit = 0;
		 return(0);
		 }
	  memcpy(&entkilp, &kilp, sizeof(kilptietue));
	  if (em.badge == em.badge0)
		  em.badge = haeVapaaKoodi(em.badge);
      kilp.ostiet[os].badge[0] = em.badge;
	  tallenna(&kilp, d, 0, 0, 0,&entkilp);
      }
   if (ch == 'O' || (em.package >= 20000000L && !em.badge)) {
      putem(&em, emithead2, 0);
      emithead2++;
      in_pros_emit = 0;
	  return(0);
	  }
	d = -1;
	if (!em.badge) {
		kno = 0;
		em.kilpno = 0;
		os = 0;
		}
	else {
		bool jatka = true;
		if ((em.kilpno = bdg2kno(em.badge, 0)) < minkilpno || (d = getpos(em.kilpno)) <= 0) {
			sprintf(msg, "Korttia %ld vastaavaa kilpailijaa ei löytynyt",
				em.badge);
			em.kilpno = 0;
			writeerror(msg, 0);
			}
		while (jatka && d > 0 && esitark >= 0) {
			jatka = false;
			GETREC(&kilp, d);
			entkilp = kilp;

			// Aliohjelma tark_badge hakee osuuden ja tarkastaa leimat

			if ((kh = tark_badge(&em, &kilp, false)) < 0 && !emitfiletest) {
				naytatulos(3);
				sprintf(msg, "Korttia %ld vastaavaa rataa ei löytynyt",
					em.badge);
				writeerror(msg, 0);
				}
			tallfl = kh > 0;
			os = em.osuus;

			rt = getrata(kilp.ostiet[os].hajonta);
			if (rt && rt->maalilaji == 4 && em.maali == TMAALI0/KSEK) {
				em.maali = e_maaliaika(&em, &kilp, os)/KSEK;
				if (em.maali == TMAALI0/KSEK) {
					if (!emitfiletest) {
						naytatulos(3);
						writeerror("Maaliaikaa ei pystytty määräämään", 0);
						}
					}
				else
					tallkilp = 1;
				}
			itm.t = em.maali*SEK / 10 * AIKAJAK;
			if (aikajono[ino]) {
				if (em.maali != TMAALI0 / KSEK) {
					itm.t = (em.maali*KSEK + aikajono[ino]->aikakorjaus)*AIKAJAK;
					itm.date = tm_date(itm.t);
					if (!esitark && ((em.package > 10000000L && em.package < 11000000L &&
						(!rt || rt->maalilaji != 4)))) {
						piste = 0;
						itm.kno = em.kilpno;
						itm.osuus = os;
						itm.piste = piste;
						itm.badge = em.badge;
						//					itm.lahde = kone;
						itm.jono = ino;
						tall_rivi(ino, &itm, NULL, NULL, 0, 0, 0);
						tallkilp = 1;
					}
				}
				else if (autokilp && seurkrivi) {
					aikatp ttm;
					bool loytyi = false;
					for (ptm = aikajono[ino]->aktrows - 1; ptm >= 0; ptm--) {
						if (em.kilpno == aikajono[ino]->getAktTime(&ttm, ptm)->kno &&
							em.osuus == ttm.osuus) {
							loytyi = true;
							break;
						}
					}
					if (loytyi) {
						if (ptm - seurkrivi > -10) seurkrivi = ptm;
					}
					else {
						while (seurkrivi - aikajono[ino]->aktrows < 0 &&
							aikajono[ino]->getAktTime(&ttm, seurkrivi)->kno)
							seurkrivi++;
						if (seurkrivi - aikajono[ino]->aktrows < 0) {
							aikajono[ino]->getAktTime(&etm, seurkrivi);
							itm.t = etm.t;
							itm.date = etm.date;
							itm.kno = em.kilpno;
							itm.osuus = os;
							itm.piste = piste;
							itm.badge = em.badge;
							//						itm.lahde = kone;
							itm.jono = ino;
							tall_rivi(ino, &itm, &etm, NULL, 0, 0, 0);
							seurkrivi++;
							tallkilp = 1;
						}
					}
					aikaruutu(0);
				}
			}
			if (!korjaus_fl && tallkilp && itm.t != TMAALI0*AIKAJAK && os >= 0 && os < Sarjat[kilp.sarja].osuusluku &&
				piste >= 0 && piste <= Sarjat[kilp.sarja].valuku[Sarjat[kilp.sarja].yosuus[os]] && !esitark && kilp.Maali(os, piste) == TMAALI0) {
				kilp.setMaali(os, piste, pyoristatls(purajak(itm.t), 1));
				tallfl = true;
				}
			if (em.badge == em.badge0) {
				em.badge = haeVapaaKoodi(em.badge);
				kilp.ostiet[os].badge[0] = em.badge;
				tallfl = true;
				}
//		 if (tallfl)
//			tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
			}
		if (loki) {
			sprintf(msg, "Kortti %d, joukkue %4d, osuus %2d\r\n", em.badge, kno, os+1);
			kirjloki(msg);
			}
		if (tallfl)
			tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
		}
	j = putem(&em, emithead2, 0);
	if (em.badge && em.kilpno) {
		if (!j) {
			setEmitJarr(em.kilpno, os, emithead2);
			laheta_em(&em, 1, 0);
			}
#ifdef PARTIO
		if (!esitark && kilpparam.partio) {
			memcpy(&entkilp, &kilp, sizeof(kilptietue));
			if (tark_partio(emittulparam, &kilp, (autorap & 2) || (autorap && kh == 2), 0) >= 0) {
				tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
				}
			}
#endif
		if (emittulparam && em.kilpno &&
			(autorap == 2 || (autorap == 1 && kh == 2) || autorapsrj[kilp.sarja]))
			emit_rap(emittulparam, em.kilpno, em.osuus);
		}
	if (valonaytto) {
		naytatulos(kilp.Tark(os, 0) == 'T');
		}
	if (d >= 0 && ilmlainat && lainakortti(&kilp, os)) {
		writeerror("  LAINAKORTTI  ", 0);
		}
	if (vaadiaika && em.kilpno && kilp.Maali(os, 0) == TMAALI0)
		writeerror("Kilpailijalla ei aikaa", 0);
	emithead2++;
	in_pros_emit = 0;
	return(0);
}

#endif // _CONSOLE
#endif

int haeseurosuus(kilptietue *kilp)
{
	int os;

	for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++)
		if (kilp->Maali(os, 0) == TMAALI0)
			return(os);
	return(-1);
}

int haeseurpiste(kilptietue *kilp, int os)
{
	int pst = 1;

	if (os >= Sarjat[kilp->sarja].osuusluku)
		return(-2);
	for (int i = 1; i <= Sarjat[kilp->sarja].valuku[os]; i++)
		if (kilp->Maali(os, i) != TMAALI0)
			pst = i+1;
	if (!vainvaliajat && pst > Sarjat[kilp->sarja].valuku[os] && kilp->Maali(os, 0) == TMAALI0)
		pst = 0;
	return(pst);
}

static int haejoustoosuus(kilptietue *kilp, aikatp ut)
{
	int osuus, os;

	// Hae ensimmäinen osuus, jolla ei aikaa
	for (osuus = Sarjat[kilp->sarja].osuusluku -2; osuus >= 0 && kilp->Maali(osuus, 0) == TMAALI0; osuus--);
	osuus++;

	// Jos edellinen aika poikkeaa väemmän kuin (ylimraja+1sek), valitse edellinen osuus
	if (osuus > 0 && NORMKELLO(purajak(ut.t) - kilp->Maali(osuus-1, 0)) < ylimraja+SEK) {
		osuus--;
		}
	// Muussa tapauksessa valitse ensimmäinen ilman aikaa ollut osuus ja etsi osuuksien lopusta alkaen
	// badge ja poimi sieltä osanottajan nimi
	else if (osuus < Sarjat[kilp->sarja].osuusluku) {
		kilp->ostiet[osuus].badge[0] = ut.badge;
		if (kilp->ostiet[osuus].nimi[0] == 0 || strcmp(kilp->ostiet[osuus].nimi, "|") == 0 || joustopakotavaihto) {
			for (os = Sarjat[kilp->sarja].osuusluku - 1; os >= 0; os--) {
				if (kilp->ostiet[os].badge[0] == ut.badge && kilp->ostiet[os].nimi[0] != 0 &&
					strcmp(kilp->ostiet[os].nimi, "|") != 0) {
					strcpy(kilp->ostiet[osuus].nimi, kilp->ostiet[os].nimi);
					break;
					}
				}
			}
		}
	return(osuus);
}

int tall_regnly(san_type *vastaus, int r_no)
   {
#if !defined(LUENTA)
	int kno = 0, mm = 0, sek = 0, osat = 0, d, tall_fl = 0, ino = 0, piste = -2;
   long t = -2000000000L, bt;
   kilptietue kilp, entkilp;
   char *as, as2[20], sum;
   static int yritys, eosat;
   aikatp ut, ttm;
   int wt;
   static long et = TMAALI0*10;

   memset(&ut, 0, sizeof(ut));
   ut.piste = -2;
   regnlyhetki[r_no] = T_TIME(biostime(0,0));
   if (regnly[r_no] == 1) {
      if (vastaus->r1.s1 != ' ' || vastaus->r1.s2 != ' ' ||
         vastaus->r1.cr != 13)
         return(1);
      vastaus->r1.cr = 13;
	  vastaus->r1.loppu = 10;
	  vastaus->r1.loppu2 = 0;
      }
   if (regnly[r_no] == 2) {                              // RTR2
      sum = 0;
      as = &vastaus->r1.tunnus;
      for (d = 2; d < r_msg_len[r_no]; d++) sum += *(as++);
      sum &= '\x7f';
      if (sum < 32) sum += 32;
      if (sum != vastaus->r2.tark) return(1);
      vastaus->r1.cr = 13;
	  vastaus->r1.loppu = 10;
      vastaus->r1.loppu2 = 0;
      }
   if (regnly[r_no] == 3 || regnly[r_no] == 5) {
      vastaus->r3.cr2 = 13;
      vastaus->r3.cr3 = 10;
      vastaus->r3.cr4 = 0;
	  }
   if (regnly[r_no] == 4) {
      vastaus->r4.cr = 13;
      vastaus->r4.lf = 10;
	  vastaus->r4.z = 0;
      }
   if (regnly[r_no] == 6) {                                //  Timy
      vastaus->r6.cr = 13;
      vastaus->r6.lf = 10;
	  vastaus->r6.z = 0;
      }
   if (loki)
	  kirjloki((char *) vastaus);
   if (regnly[r_no] == 3 || regnly[r_no] == 5) {
	   int draja;
      if (regnly[r_no] == 5 && vastaus->r1.tunnus == ' ' &&
         vastaus->r3.cr1 == ' ') return(0);
      if (vastaus->r3.kp1 != ':' || vastaus->r3.kp2 != ':' ||
         vastaus->r3.p != '.')
		 return(1);
      if (regnly[r_no] == 5) {
         vastaus->r3.bl = 0;
         d = atoi(&vastaus->r3.z1);
		 if (d > viimrivi) viimrivi = d;
         }
      vastaus->r3.tunnus = 'F';
      vastaus->r3.kp1 = 0;
      vastaus->r3.kp2 = 0;
	  vastaus->r3.p = 0;
      vastaus->r3.cr2 = 0;
	  draja = (regnly[r_no] == 3) ? 3 : 10;
	  for (int dd = 0; dd < draja; dd++) {
		  if (vastaus->r3.z1 == alge_tunn[dd][0]
			  && vastaus->r3.z2 == alge_tunn[dd][1]
			  && (regnly[r_no] == 3 || (vastaus->r3.z3 == alge_tunn[dd][2]))) {
			  ino = aika_jono[0][dd + 3];
			  piste = aika_tunnus[0][dd + 3];
			  ut.kanava = dd;
			  break;
		  }
	  }
	  t = atol(vastaus->r3.tunti);
      mm = atoi(vastaus->r3.min);
      sek = atoi(vastaus->r3.sek);
	  osat = atoi(vastaus->r3.osat);
	  t = (SEK*osat)/100 + SEK*sek + MINUUTTI*mm + TUNTI*t + t0_regn[r_no];
	  t = NORMKELLO_A(10*t);
	  if (t == et)
		  return(0);
	  bt = 0;
	  if (regnly[r_no] == 5 && aikajono[ino]) {
		  aikatp wtm;
		  ttm.t = t;
		  ttm.date = tm_date(t);
         for (wt = aikajono[ino]->rwtime-1; wt >= 0 && datetime64(aikajono[ino]->getTime(&wtm, wt)) >= datetime64(&ttm); wt--)
			if (datetime64(&wtm) == datetime64(&ttm)) 
				return(0);
         }
      }
   else if (regnly[r_no] == 4) {
	   int dd = 0;
      if (vastaus->r4.kp1 != ':' || vastaus->r4.kp2 != ':' ||
		 vastaus->r4.p != '.')
         return(1);
	  vastaus->r4.tunnus = regnly_no[r_no] ? 'A' : 'F';
	  vastaus->r4.kp1 = 0;
      vastaus->r4.kp2 = 0;
      vastaus->r4.p = 0;
//      vastaus->r4.tuh = 0;
	  if ((dd = stschind(vastaus->r4.piste, (char *) "2418")) > -1) {
		  ino = aika_jono[0][dd + 3];
		  piste = aika_tunnus[0][dd + 3];
		  ut.kanava = dd;
	  }
	  t = atol(vastaus->r4.tunti);
      mm = atoi(vastaus->r4.min);
      sek = atoi(vastaus->r4.sek);
      osat = atoi(vastaus->r4.osat);
	  t = (SEK*osat)/1000 + SEK*sek + MINUUTTI*mm + TUNTI*t + t0_regn[r_no];
	  t = NORMKELLO_A(10*t);
	  if (t == et)
		  return(0);
	  bt = 0;
	  }
   else if (regnly[r_no] == 6) {				// Timy
      if (vastaus->r6.p != '.')
         return(1);
	  vastaus->r6.bl1 = 0;
	  vastaus->r6.kp1 = 0;
      vastaus->r6.kp2 = 0;
      vastaus->r6.p = 0;
	  if ((d = vastaus->r6.piste - '0') >= 0 && d <= 8) {
		  ino = aika_jono[0][d + 3];
		  piste = aika_tunnus[0][d + 3];
		  ut.kanava = d;
	  }
	  t = atol(vastaus->r6.tunti);
      mm = atoi(vastaus->r6.min);
      sek = atoi(vastaus->r6.sek);
	  for (int d = 0; d < 4; d++)
		 if (vastaus->r6.osat[d] < '0')
			vastaus->r6.osat[d] = '0';
	  vastaus->r6.osat[3] = '0';
	  osat = atoi(vastaus->r6.osat);
	  t = osat + 10000L*sek + 600000L*mm + 36000000L*t + 10L*t0_regn[r_no];
	  t = NORMKELLO_A(t);
	  if (t == et)
		  return(0);
      bt = 0;
/*
      switch (vastaus->r6.tunnus) {
		 case 'c':
            if (hae_aika(t, &wt, ino, &ino) && !aikajono[ino]->getAktTime(0, wt)->kno)
               tall_rivi(ino, 0, aikajono[ino]->getAktTime(0, wt), 0, 0, 0, 0);
            return(0);
         default:
				if (regnly_no[r_no]) {
					if (hae_aika(t, &wt, ino, &ino) &&
						 aikajono[ino]->getAktTime(0, wt)->kno == atoi(vastaus->r6.kno))
						 return(0);
					}
            else if (hae_aika(t, &wt, ino, &ino))
               return(0);
            break;
		 }
*/
	  vastaus->r6.tunnus = regnly_no[r_no] ? 'A' : 'F';
	  }
#ifdef LAJUNEN
	else if (regnly[r_no] == 30) {           // SIRIT
		siritaika((TMAALI *)&t, vastaus, &ut, &ino, r_no);
		if (t == 20L*TMAALI0)
			return(1);
		bt = 0;
		vastaus->r1.tunnus = 'A';
		kno = bdg2kno(ut.badge, 0);
		if (kno == 0 && kilpparam.kaksibadge) {
			int toinen = 1;
			kno = bdg2kno(ut.badge, &toinen);
			}
		//	naytatulos(ut.kanava == 10);
		}
	else if (regnly[r_no] == 33) {           // IMPINJ
		__int64 t64, pv64;
		char *p;
		while (vastaus->bytes[0] && (vastaus->bytes[0] < '0' || vastaus->bytes[0] > '9'))
			memmove(vastaus->bytes+1, vastaus->bytes, strlen(vastaus->bytes));
		if (strlen(vastaus->bytes) < 30)
			return(1);
		p = strtok(vastaus->bytes, ",\r\n");
		if (!p)
			return(1);
		p = strtok(NULL, ",\r\n");
		if (!p || strlen(p) < 7)
			return(1);
		ut.badge = atoi(p + strlen(p) - 7);
		p = strtok(NULL, ",\r\n");
		if (!p || strlen(p) < 7)
			return(1);
		p[strlen(p)-3] = 0;
		t64 = _atoi64(p);
		pv64 = t64/(24*TUNTI);
		t = (int) (t64 - pv64*(24*TUNTI));
		t = t + t0_regn[r_no];
		t = NORMKELLO_A(10*t);
		bt = 0;
		kno = bdg2kno(ut.badge, 0);
		if (kno == 0 && kilpparam.kaksibadge) {
			int toinen = 1;
			kno = bdg2kno(ut.badge, &toinen);
			}
		vastaus->r1.tunnus = 'A';
		}
#endif
	else if (regnly[r_no] == 1 || regnly[r_no] == 2) {                      // RTR
		if (vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'C' ||
			vastaus->r1.tunnus == 'E' || vastaus->r1.tunnus == 'F' ||
			vastaus->r1.tunnus == 'G') {
			ino = aika_jono[0][4];
			piste = aika_tunnus[0][4];
			if (regnly[r_no] == 2) {                                       // RTR2
				int ii;
				ii = (vastaus->r2.cr1 - '0' + 1) % 10 - 1;
				if (ii < 4) {
					ino = aika_jono[0][ii + 4];
					ut.kanava = ii;
					if (vastaus->r2.n1 > '1') {
						ino = aika_jono[0][ii + 8];
						ut.kanava = ii + 4;
						}
					piste = aika_tunnus[0][ii + 4];
					if (vastaus->r2.n1 > '1') piste = aika_tunnus[0][ii + 8];
				}
				vastaus->r2.pakka = 0;
			}
			if (regnly[r_no] == 1) {
				if (vastaus->r1.kp != ':' || vastaus->r1.p != '.') return(1);
			}
			vastaus->r1.kp = 0;
			vastaus->r1.p = 0;
			vastaus->r1.cr = 0;
			mm = atoi(vastaus->r1.min);
			sek = atoi(vastaus->r1.sek);
			osat = atoi(vastaus->r1.osat);
			t = osat*KSEK / 10 + SEK*sek + MINUUTTI*mm + t0_regn[r_no];
			t = NORMKELLO_A(10 * t);
			bt = purajak(t) / KSEK - T_TIME(biostime(0, 0L));
		}
	}
//   else t = t0_regn[r_no];
   if ((vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'B'
	  || vastaus->r1.tunnus == 'E') && regnly_no[r_no] && regnly[r_no] != 30 && regnly[r_no] != 33) {
	  switch (regnly[r_no]) {
		 case 1:
			vastaus->r1.s2 = 0;
			kno = atoi(vastaus->r1.kno);
			break;
		 case 2:
			vastaus->r2.min[0] = 0;
			kno = atoi(vastaus->r2.kno+1);
			break;
		 case 4:
			vastaus->r4.L_C[1] = 0;
		 case 6:
		 case 8:
			kno = atoi(vastaus->r6.kno);
			break;
		 }
	  }
	if (vastaus->r1.tunnus >= 'A' && vastaus->r1.tunnus <= 'F') {
		if ((vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'F') &&
			labs(bt) > maxero && yritys++ == 0)
			return(1);
		if (aikajono[ino]->rwtime - aikajono[ino]->mxtime >= 0) {
#ifndef LANGSV_K
			writeerror("AIKATAULUKKO TÄYSI",0);
#else // LANGSV_K
			writeerror("TIDTAVLAN FULL",0);
#endif // LANGSV_K
			return(1);
			}
		wt = aikajono[ino]->aktrows;
		if (vastaus->r1.tunnus == 'B' || vastaus->r1.tunnus == 'D')
			t = et;
		t += aikajono[ino]->aikakorjaus*AIKAJAK;
		ut.t = t;
		ut.date = tm_date(ut.t);
		if (ol_osuus)
		ut.osuus = ol_osuus - 1;

		EnterCriticalSection(&tall_CriticalSection);

		if (vastaus->r1.tunnus >= 'B' && vastaus->r1.tunnus <= 'D') {        // RTR-nollaustieto
			if (vastaus->r1.tunnus == 'B') kno += 10000;
			else kno = 19999 + vastaus->r1.tunnus - 'D';
			ut.kno = kno;
#ifdef _CONSOLE
			vidint(ySize - 1, 54, 4, aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows-1)->kno);
#endif
			if (loki)
				kirjloki("NOLLAUS\r\n");
			}

		else if ((d = getpos(kno)) > 0) {                  // Aikatieto kilpailijanumeron kera
			int bdgosuus = -1;
			ut.kno = kno;
			GETREC(&kilp, d);
			memcpy(&entkilp, &kilp, sizeof(kilptietue));
			if (ylimraja) {
				int ttm;
				for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
					// Ohita väärät badget
					if (ut.badge && ut.badge != kilp.ostiet[os].badge[0] &&
						(!kilpparam.kaksibadge || ut.badge != kilp.ostiet[os].badge[1]))
						continue;

					// Etsi aikaa, joka poikkeaa vähemmän kuin ylimraja
					for (int va = 0; va <= Sarjat[kilp.sarja].valuku[os]; va++) {
						// Katso ensin, löytyykö eroaako aika vähemmän jostain kilpailijan (väli)ajasta
						if ((ttm = kilp.Maali(os, va)) != TMAALI0 &&
							abs((int) NORMTULOS(purajak(ut.t) - ttm)) < ylimraja) {
							aikatp vertt;
							// Jos tällainen piste löytyi, etsi ajanottojonosta lopusta alkaen
							// riviä, jolla sama kilpailija ja piste. Jos on aseta ut.status = 2
							for (int rtm = aikajono[ino]->aktrows - 1; rtm >= 0 &&
								NORMKELLO(ttm - purajak(aikajono[ino]->getAktTime(&vertt, rtm)->t)) < ylimraja;
								rtm--) {
								if (vertt.kno == kilp.kilpno && vertt.osuus == os && vertt.piste == va)
									ut.status = 2;
								}
							}
						}
					}
				}
			if (os_raja == -2)
				osat = Sarjat[kilp.sarja].osuusluku - 1;
			else if (joustoviesti && (kno || ut.badge)) {
				osat = haejoustoosuus(&kilp, ut);
				}
			else if (!ol_osuus) {
				int ttm;
				for (osat = 0; osat < Sarjat[kilp.sarja].osuusluku; osat++) {
					if (ut.badge && (ut.badge == kilp.ostiet[osat].badge[0] || (kilpparam.kaksibadge && ut.badge == kilp.ostiet[osat].badge[1])))
						bdgosuus = osat;
					if ((ut.badge == 0 || bdgosuus == osat) &&
						(ttm = kilp.Maali(osat, 0)) == TMAALI0 && stschind(kilp.Tark(osat, 0), "KH") < 0)
						break;
					}

//						(((ttm = kilp.Maali(osat, 0)) != TMAALI0 && (ttm = abs(NORMTULOS(purajak(ut.t) - ttm))) > ylimraja) ||
//				if (ylimraja && ttm != TMAALI0 && ttm <= ylimraja)
//					return(0);
				if (vastaus->r1.tunnus == 'E' && osat > 0) osat--;
				if (osat >= Sarjat[kilp.sarja].osuusluku && !ut.badge) {
#ifdef _CONSOLE
					vidspmsg(ySize - 4, 0, 0, 7,
						"JOUKKUEEN      VIIMEISEN OSUUDEN TULOS VAIHDETTU");
					vidint(ySize - 4, 10, 4, kno);
#endif
					if (loki) {
						kirjloki("TULOKSET JO ANNETTU, VIIMEINEN VAIHDETAAN\r\n");
						}
					osat = Sarjat[kilp.sarja].osuusluku - 1;
					}
				}
			if (piste < -1 && lahdepistehaku) {
				piste = haelahdepiste(&kilp, osat, ut.lahde, purajak(t));
				}
			if (piste > -2)
				ut.piste = piste;
			else if (va_mpiste[ino] < -1) {
				ut.piste = haeseurpiste(&kilp, osat);
				}
			else
				ut.piste = va_mpiste[ino];
			if (lahdejonohaku)
				ino = haelahdejono(&kilp, osat, ut.lahde);
			if (ut.kno > 0 && ut.piste >= 0 && osat < Sarjat[kilp.sarja].osuusluku &&
				ut.piste <= Sarjat[kilp.sarja].valuku[osat] &&
				(uusinaika || kilp.Maali(osat, ut.piste) == TMAALI0)) {
				tall_fl = 1;
				kilp.setMaali(osat, ut.piste, pyoristatls(purajak(t), 1));
				if (kilp.Tark(osat, 0) == 'E') {
					LeaveCriticalSection(&tall_CriticalSection);
#ifdef _CONSOLE
					vidspmsg(ySize - 4, 0, 0, 7, "Joukkueen      osuus on jo merkitty ei"
						" lähteneeksi");
					vidint(ySize - 4, 10, 4, kno);
#endif
					if (loki)
						kirjloki("OSUUS ON JO MERKITTY EI LÄHTENEEKSI\r\n");
					EnterCriticalSection(&tall_CriticalSection);
					}
				else if (varkynnys[kilp.sarja][osat] && (osuustulos(&kilp, osat, 0) < varkynnys[kilp.sarja][osat]
					|| osuustulos(&kilp, osat, 0) > 20 * TUNTI)) {
					LeaveCriticalSection(&tall_CriticalSection);
#ifdef _CONSOLE
					vidspmsg(ySize - 4, 0, 0, 7, "Joukkueella      poikkeavan hyvä osuusaika");
					vidint(ySize - 4, 12, 4, kno);
#endif
					if (loki) {
						kirjloki("POIKKEAVAN HYVÄ OSUUSAIKA\r\n");
						}
					EnterCriticalSection(&tall_CriticalSection);
					}
				}
			if (estaneg && osat < Sarjat[kilp.sarja].osuusluku && ut.piste >= 0) {
				int tl1, tt;
				tl1 = kilp.Lahto(osat);
				tt = NORMTULOS(purajak(ut.t) - tl1);
				if (tl1 == TMAALI0 || tt < estaneg || tt > 20 * TUNTI) {
					ut.status = 1;
					}
				}
			if (osat >= Sarjat[kilp.sarja].osuusluku) {
				if (bdgosuus >= 0)
					osat = bdgosuus;
				else
					osat = Sarjat[kilp.sarja].osuusluku - 1;
				}
			ut.osuus = osat;
			}
		else {
			if (piste > -2)
				ut.piste = piste;
			}

		ut.jono = ino;
		tall_rivi(ino, &ut, NULL, NULL, 0, 0, 0);
		if (ut.status == 0) {
			if (lahtoryhma > 1 && ut.piste == -1) {
				for (int ii = 1; ii < lahtoryhma; ii++) {
					ut.t += 1;
					tall_rivi(ino, &ut, NULL, NULL, 0, 0, 0);
					}
				}
			if (tall_fl) {
				tallenna(&kilp,d,TRUE, 0, 0, &entkilp);
				}
			if (od_fl && kno < minkilpno && purajak(od_aika) == TMAALI0 &&
				ut.t == aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows - 1)->t) {
				od_aika = ut.t;
				odota = wt;
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		if (aikajono[ino]->aktrows - htime[ino] > 0)
			htime[ino] = aikajono[ino]->aktrows;
#ifdef _CONSOLE
		as = AIKATOSTRS(as2, purajak(t),t0);
		if (tall_fl)
			vidint(ySize-1,54,4,kno);
		vidspmsg(ySize-1,60,7,0,as);
		vidint(ySize-1,71,4,aikajono[ino]->aktrows);
		vidint(ySize-1,76,4,aikajono[ino]->rtime+1);
#endif
		}
	else if (vastaus->r1.tunnus == 'G' && kerro_regnly) {
		kerro_regnly = 0;
#ifdef _CONSOLE
		clrln(ySize-1);
#ifndef LANGSV_K
		vidspmsg(ySize-1,0,7,0,"Maalikello on tietokoneen kelloa      s");
#else // LANGSV_K
		vidspmsg(ySize-1,0,7,0,"Maalikello on tietokoneen kelloa      s");
#endif // LANGSV_K
		vidint(ySize-1,32,5,(int)(labs(bt) / 10));
#ifndef LANGSV_K
		as = (bt > 0) ? "edellä" : "jäljessä";
#else // LANGSV_K
		as = (bt > 0) ? "edellä" : "jäljessä";
#endif // LANGSV_K
		vidspmsg(ySize-1,40,7,0,as);
#endif
		}
	if (vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'C' ||
		vastaus->r1.tunnus == 'E' || vastaus->r1.tunnus == 'F' ||
		vastaus->r1.tunnus == 'G') et = t;
	yritys = 0;
#endif
	return(0);
}

//#endif
//#endif
/* Ensimmäinen endif lopettaa regnly-määrityksen, toinen MAALI-osat */
#ifdef KUULDEMO
extern int demofl;
void demo(void);
#endif

#if defined(AUTOVA) || defined(VALUKIJA)
INT16 varasti[MAXSARJALUKU][MAXOSUUSLUKU][VAIKALUKU+1];

int start_autova(void)
   {
   wchar_t ch;

   luelahdepisteet();
   autova_open = 1;
   t0_autova = -t0*3600L*SEK;
   if (aikaero[0] != TMAALI0) {
		t0_autova += aikaero[0];
		if (aikaero[0] < 0)
			t0_autova += 24*TUNTI;
		}
   else {
#ifdef _CONSOLE
	   clrln(ySize-3);
	   viwrrectw(ySize-3,0,ySize-3,48,
		  L"Anna poikkeama: kilpailuaika - väliaikojen aika :",7,0,0);
	   INPUTAIKAW(&t0_autova,t0,8,50,ySize-3,L"\xD",&ch);
#else
		inputaika_prompt(&t0_autova,t0,8,L"Anna poikkeama: kilpailuaika - väliaikojen aika",&ch);
#endif
		}
   return(0);
   }

void laheta_ec(UINT32 badge, int piste, INT32 aika, int kielto);

void tall_ec(UINT32 bdg, INT valeim, INT32 aika, INT kielto)
{
	long vatime, t, t_ed;
	int kno, pst, d, os, osuus, ino = 0, toinen = 0;
	kilptietue kilp;
	char msg[120], st[20];
	kilptietue entkilp;
	aikatp itm;

	ino = aika_jono[0][1];
	vatime = KSEK*(aika - 12*36000L) + aikajono[ino]->aikakorjaus;
	memset(&itm, 0, sizeof(itm));
	itm.t = AIKAJAK*vatime;
	itm.date = tm_date(itm.t);
	itm.badge = bdg;
	itm.kanava = kielto;
	itm.lahde = (INT16)valeim;
	itm.jono = ino;
	laheta_ec(bdg, valeim, aika, kielto);
	if ((kno = bdg2kno(bdg, 0)) > 0 || (kilpparam.kaksibadge && (toinen = 1) != 0 && (kno = bdg2kno(bdg, &toinen)) > 0)) {
		t = T_TIME(biostime(0,0));
		t = t-vatime/KSEK;
		if (t < 0)
			t = -t;
		if (t > 12*36000L)
			t = 24*36000L - t;
		if ((!vaikavert || t < 600*vaikavert) && (d = getpos(kno)) > 0) {
			EnterCriticalSection(&tall_CriticalSection);
			GETREC(&kilp, d);
			entkilp = kilp;
			itm.kno = kno;
			if (joustoviesti && itm.badge) {
				osuus = haejoustoosuus(&kilp, itm);
				}
			else {
				osuus = -1;
				for (os = Sarjat[kilp.sarja].osuusluku-1; os >= 0; os--) {
					if (bdg == kilp.ostiet[os].badge[toinen]) {
						if (val_korkein || osuus < 0 ||
							kilp.Maali(os, 0) == TMAALI0) {
							osuus = os;
							if (val_korkein)
								break;
							}
						}
					if (!val_korkein && osuus >= 0 && (t_ed = kilp.Maali(osuus, 0)) != TMAALI0 &&
						!(uusinaika && NORMKELLO(vatime-t_ed) < uusinaika))
						break;
					}
				}
			if (osuus >= 0) {
				pst = haelahdepiste(&kilp, osuus, valeim, vatime);
				if (lahdejonohaku)
					ino = haelahdejono(&kilp, osuus, valeim);
				itm.jono = ino;
				itm.osuus = osuus;
				itm.piste = pst;
				if (pst >= 0 && pst <= Sarjat[kilp.sarja].valuku[Sarjat[kilp.sarja].yosuus[osuus]] &&
					((t_ed = kilp.Maali(osuus, pst)) == TMAALI0 || (uusinaika && NORMKELLO(vatime-t_ed) < uusinaika))) {
					kilp.setMaali(osuus, pst, pyoristatls(vatime, 1));
					tallenna(&kilp, d, TRUE, 0, 0, &entkilp);
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
		else {
			sprintf(msg, "Yli %d min aikaero saapuneen aikatiedon ja tämän koneen välillä", vaikavert);
			msg[80] = 0;
			writeerror(msg, 4000);
			}
		}
	else {
		if (ilmtunt) {
			AIKATOSTRS(st, vatime, t0);
			sprintf(msg, "Tuntematon Emit-koodi %d saapunut, aika: %8.8s, piste: %d", bdg, st, valeim);
			msg[80] = 0;
			writeerror(msg, 4000);
			if (loki) {
				kirjloki(msg);
				}
			}
		}
	if (vaajat) {
		tall_rivi(ino, &itm, NULL, NULL, 0, 0, 0);
		}
}

#endif  // AUTOVA
#ifdef EMITHTTP
void vahaku(LPVOID lpCn)
{
  wchar_t edate[16], page[100], st[16];
  wchar_t msg[200];
  int er = 0, keTSerial;

  inhttpHaku = 1;
  keTSerial = *(int *) lpCn;
  if (eTParam.eTdate && wcslen(eTParam.eTdate) == 10)
	  wcscpy(edate, eTParam.eTdate);
  else {
	  wcscpy(st, wpaiva());
	  wmemcpy(edate+2, st+6, 2);
	  wmemcpy(edate+5, st+3, 2);
	  wmemcpy(edate+8, st, 2);
	  edate[0] = L'2';
	  edate[1] = L'0';
	  edate[4] = L'-';
	  edate[7] = L'-';
	  edate[10] = 0;
	  }

  swprintf(page, L"/emitphp/get_stream.php?Y=%d&time=%s%s%s", eTParam.eTserial[keTSerial], edate, L"%20",
	wcsncpy(st, eTParam.eTtime, 15));
  if (eTParam.msgno[keTSerial] > 1)
	  swprintf(page+wcslen(page), L"&Mmin=%d", eTParam.msgno[keTSerial]);

  if ((er = httphaku(eTParam.eThost , eTParam.eTport, page, 0, eTParam.buf, eTParam.buflen, &eTParam.haettu)) != 0) {
	  swprintf(msg, L"Virhe %d väliaikahaussa", er);
	  writeerror_w(msg, 2000, true);
	  }
	inhttpHaku = 0;
}
#endif

void tausta(int spoolfl)
   {
   static int init = 1, msgno = 1, keTserial;
   static char saika[20] = "          ";
   static char httpbuf[100000];
   long t,tdif;
   int i,j;
   static char *eTp, *eTp1;
   static int tfl = FALSE;
   static long prev_t = -1, edhaku = -1, inhttpVa;
   static int alku_pv;
   int os;
#if defined(AUTOFILE) || defined(TAULU)
   long t_ta;
   static long t_af = TMAALI0/KSEK;
#endif

   autotulostus();
   
#if defined(ALGETAULU) && defined(_CONSOLE)
   if (taulu_com || udptaulu) paiv_taulu();
#endif

   if (prev_t < 0) {
      prev_t = biostime(0,0L);
      alku_pv = lue_v_pv();
      }
   t = biostime(0,0L);
#ifdef _CONSOLE
   aikatostr_ls(saika+1, T_TIME(t),t0);
   saika[9] = ' ';
   viwrrect(0,35,0,44,saika,7,0,0);
#if defined(AUTOFILE) || defined(TAULU)
   if (autofileparam.afvali) {
	  t_ta = T_TIME(t);
	  if (t_af > t_ta + autofileparam.afvali / KSEK) t_af = t_ta;
	  if (t_ta >= t_af) {
		 if (t_af != TMAALI0/KSEK) {
			if (autofile())
			   t_af = t_ta + 50;
			else
			   t_af = t_ta + autofileparam.afvali / KSEK;
			}
		 else {
			autofile();
			t_af = t_ta + autofileparam.afvali / KSEK;
			}
		 }
	  t_af = t_af % (24L * TUNTI/KSEK);
	  }
#endif
#endif
   if (t < prev_t - DAYTICKS + 1000) {
	  }
   prev_t = t;
 #ifdef EMITHTTP
	if (taustaon && eTParam.eTserial[0] > 0 && eTParam.eThaku && !inhttpHaku && !inhttpVa) {
		inhttpVa = 1;
		if (eTParam.haettu == 0) {
			if (eTParam.eThaku < 0)
				for (int ii = 0; ii < 4; ii++)
					eTParam.msgno[ii] = 1;
			eTParam.eThaku = 1;
			if (t < edhaku || t > edhaku + eTParam.eThakuvali) {
				edhaku = t;
				if (keTserial < sizeof(eTParam.eTserial)/sizeof(eTParam.eTserial[0])-1 && eTParam.eTserial[keTserial+1])
					keTserial++;
				else
					keTserial = 0;
				eTParam.buf = httpbuf;
				eTParam.buflen = sizeof(httpbuf);
				httphakuThread = _beginthread(vahaku, 40960, &keTserial);
				eTp = httpbuf;
				}
			}
		else {
			for (i = 0; taustaon && i < 10; i++) {
				for (eTp1 = eTp; *eTp1 && *eTp1 != '\r' && *eTp1 != '\n'; eTp1++);
				if (eTp1 > eTp+30 && (*eTp1 == '\r' || *eTp1 == '\n')) {
					*eTp1 = 0;
					tulk_emiTag((char *)eTp, NULL, NULL, eTParam.eTrno);
					for (eTp = eTp1+1; *eTp && (*eTp == '\r' || *eTp == '\n'); eTp++);
					if (*eTp == 0)
						break;
					for (; eTp1 > eTp-50 && eTp1[-1] != 'M'; eTp1--) ;
					eTParam.msgno[keTserial] = atoi(eTp1)-10;
					if (eTParam.msgno[keTserial] < 1)
						eTParam.msgno[keTserial] = 1;
					}
				else
					*eTp = 0;
				}
			if (*eTp == 0)
				eTParam.haettu = 0;
			}
		inhttpVa = 0;
		}
#endif
     i = 0;
      if ((tdif = t - ivtime[i]) > 0 || tdif + intv[i] < 0) {
         if (i == 0) {
#ifndef LUENTA
            if (timerfl) writeajat();
#endif
            tauko = FALSE;
            for (j = 0; j < sarjaluku; j++)
               for (os = 0; os < Sarjat[j].ntosuus; os++)
                  if( (prior[j][os] < 16000) )
                     prior[j][os] = (prior[j][os] << 1) -
                        (prior[j][os] >> 1);
            timefl = TRUE;
            }
         ivtime[i] = (t + intv[i]) % DAYTICKS;
         }
   }

#if defined(_CONSOLE) && !defined(LUENTA)
void lahestyjat(void);

static void tausta200(void)
{
	lahestyjat();
}
#endif

void taustaThr(LPVOID lpCn)
   {
   int runtime = 0, edtime = 0;

   while(taustaon) {
	  runtime = mstimer();
#if defined(_CONSOLE) && !defined(LUENTA)
	  if (runtime > edtime+200) {
		  edtime = runtime;
		  tausta200();
		  }
#endif
      tausta(0);
      Sleep(10);
      }
   }

void tee_aika(int laji)
   {
#if defined(MAALI) && !defined(LUENTA)
   INT32 tm;
   aikatp utime;
   int wt;
   char as[20];
   int ino = 0;

   if (maxjono)
	   ino = aika_jono[0][laji - 1];
   if (aikajono[ino]->rwtime - aikajono[ino]->mxtime >= 0) {
      writeerror("AIKATAULUKKO TÄYSI",0);
	  return;
      }
   tm = AIKAJAK*(T_TIME(biostime(0,0))*KSEK + aikajono[ino]->aikakorjaus);

   wt = aikajono[ino]->aktrows;
   
   
   memset(&utime, 0, sizeof(utime));
   utime.t = tm;
   utime.date = tm_date(utime.t);
   utime.jono = ino;
   utime.piste = aika_tunnus[0][laji-1];
   tall_rivi(ino, &utime, NULL, NULL, 0, 0, 0);
   if (od_fl && purajak(od_aika) == TMAALI0 &&
	   utime.t == aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows - 1)->t) {
      od_aika = utime.t;
      odota = wt;
      }
   tick();
   vidint(ySize - 1, 54, 4, aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows - 1)->kno);
   AIKATOSTRS(as, purajak(tm),t0);
   vidspmsg(ySize-1,60,7,0,as);
   if (aikajono[ino]->aktrows - htime[ino] > 0) htime[ino] = aikajono[ino]->aktrows;
   vidint(ySize-1,71,4,aikajono[ino]->aktrows);
   vidint(ySize-1,76,4,aikajono[ino]->rtime+1);
#endif
   }


int kbdiv1(char ch, char ch2)
   {
   int retval = 0;
#ifdef _CONSOLE
   static int helpfl = FALSE;
   char tc;
   char saveline[80];
   int x,y,h,l,cc;
#if defined(MAALI) && !defined(LUENTA)
   extern int maali_on;
#endif
#ifdef TESTBDG
extern void bdgstatus(void);
#endif

   if (nivkeys && ch == keytab[0].ch && ch2 == keytab[0].keycode) {
	  tee_aika(3);
      return(1);
	  }

   cc = 256*ch + ch2;
   switch (cc) {
      case 59:
#ifdef TESTBDG
         bdgstatus();
#else
         if (!helpfl) {
            helpfl = TRUE;
            help(0);
            helpfl = FALSE;
            }
#endif
         return(1);
      case 256:
      case 256+30 : retval = 1;
               if (esto) break;                           // Ctrl-A
               esto = TRUE;
               sccurst(&y, &x, &h, &l);
               virdrect(ySize-3,0,ySize-3,79,saveline,0);
               tc = 'K';
               if (timerfl) {
                  selectopt("Vaihdetaanko ajanottotaulukko (K/E)", "KE", &tc);
                  if (tc == 'K') vaihda_ajat(akttm);
                  }
               viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
               sccurset(y,x);
               esto = FALSE;
			   break;
      }
   if (retval) {
#if defined(MAALI) && !defined(LUENTA)
	  if (aikajono[akttm]->aktrows - aikajono[akttm]->mxtime > 0) aikajono[akttm]->aktrows = aikajono[akttm]->mxtime;
      if (aikajono[akttm]->aktrows > 0) {
	     aikatp ttm;
		 vidint(ySize - 1, 54, 4, aikajono[akttm]->getAktTime(&ttm, aikajono[akttm]->aktrows - 1)->kno);
         VIDSPAIKA(ySize-1,60,ttm.t,t0);
         }
      if (aikajono[akttm]->aktrows - htime[akttm] > 0) htime[akttm] = aikajono[akttm]->aktrows;
	  vidint(ySize-1,71,4,aikajono[akttm]->aktrows);
	  vidint(ySize - 1, 76, 4, aikajono[akttm]->rtime + 1);
//      if (maali_on) naytamaali(&x, &y, 0);
#endif
	  }
#endif //  _CONSOLE
   return(retval);
   }

void initajat(int ino, int vaihda)
{
#if !defined(LUENTA)
   wchar_t ch;
   UINT32 fl,l;
   bool vanha = true;
	aikatp ttm;
	char buf[100];
	int reclen, pos;
	int date = 0;
	int ed_t = TMAALI0;

   if (!vaihda) {
#ifdef _CONSOLE
	   clrln(ySize - 3);
	   char msg[40];
	  if (maxjono > 0)
		 sprintf(msg, "    Aikatiedosto %d:", ino+1);
	  else if (!ino)
		 strcpy(msg, "     Aikatiedosto :");
	  vidspmsg(ySize-3,0,7,0,msg);
	  inputwstr(aikajono[ino]->aikafname ,37,20,ySize-3,L"\r",&ch,FALSE);
#endif //  _CONSOLE
	  }
   else if (vaihda != -1) {
	  for (aikajono[ino]->rtime = 0; aikajono[ino]->rtime - aikajono[ino]->mxtime <= 1; aikajono[ino]->rtime++) {
		  memset(&ttm, 0, sizeof(ttm));
		 ttm.t = TMAALI0*AIKAJAK;
		 ttm.jono = ino;
		 puttm(aikajono[ino]->rtime, &ttm, ino);
		 }
	  aikajono[ino]->rtime = 0;
	  aikajono[ino]->rwtime = 0;
	  }
   aikajono[ino]->fp = 0;
   if ( (aikajono[ino]->afhandle = _wopen(aikajono[ino]->aikafname, O_RDWR | O_BINARY | O_CREAT,
	  S_IREAD | S_IWRITE)) >= 0 ) {
	  fl = _filelength(aikajono[ino]->afhandle);
	  if (fl >= 10) {
		 _lseek(aikajono[ino]->afhandle, 0, SEEK_SET);
		 if(_read(aikajono[ino]->afhandle, buf, 10) != 10) {
			  iowerr(aikajono[ino]->aikafname,1,0);
			  _close(fl);
			  return;
			  }
		 if (memcmp(buf, "AJAT_V", 6) == 0) {
			 vanha = false;
			 reclen = sizeof(aikatp);
			 _lseek(aikajono[ino]->afhandle, reclen, SEEK_SET);
			 fl -= reclen;
			 }
		 else {
			 vanha = true;
			 reclen = sizeof(aikatp) - sizeof(ttm.date);
			 _lseek(aikajono[ino]->afhandle, 0, SEEK_SET);
			 }
		 if (fl > (UINT32)(aikajono[ino]->mxtime*reclen))
			fl = aikajono[ino]->mxtime*reclen;
		 if (sail_ajat)
			ch = L'K';
		 else
			{
#ifdef _CONSOLE
			ch = L' ';
#ifndef LANGSV_K
			wselectopt(L"AJANOTTOTOIMINNON KÄYNNISTYS - Luetaanko aiemmat "
			   L"tiedot levyltä (K/E)", L"KE", &ch);
#else
			wselectopt(L"STARTEN AV FUNKTIONEN ML - Skall tidigare data  "
			   L"läsas från fil (J/E)", L"JE", &ch);
			wtransl(L"J", L"K", &ch);
#endif
#else
			int i;
			i = select3(2, L"AJANOTTOTIEDOSTON AVAAMINEN - Luetaanko aiemmat "
				L"tiedot levyltä", L"Valitse", L"Luetaan", L"Aiemmat tuhotaan",
				L"Säilytä vanhat ja poistu", 0);
			ch = (L"\x1bKE\x1b")[i];
#endif
			clrln(ySize-5);
			}
		 if (ch == L'K') {
			sail_ajat = 1;
			while (fl > 0) {
			   if(_read(aikajono[ino]->afhandle, buf, reclen) != reclen)
				  iowerr(aikajono[ino]->aikafname,1,aikajono[ino]->fp);
			   fl -= reclen;
			   if (vanha) {
				   ttm.t = *(INT32 *)buf;
				   memcpy(((char *) &ttm)+8, buf+4, reclen-4);
				   if (ttm.t < ed_t)
					  date++;
				   ed_t = ttm.t;
				   ttm.date = date;
				   }
			   else
				   memcpy(&ttm, buf, reclen);
			   puttm(aikajono[ino]->fp, &ttm, ino);
			   aikajono[ino]->fp++;
			   }
			}
		 }
	  aikajono[ino]->rwtime = aikajono[ino]->fp;
	  while ((aikajono[ino]->rwtime > 0) && purajak(aikajono[ino]->getTime(0, aikajono[ino]->rwtime - 1)->t) == TMAALI0)
		  aikajono[ino]->rwtime--;
	  htime[ino] = aikajono[ino]->rwtime;
	  aikajono[ino]->rtime = aikajono[ino]->rwtime;
	  while ((aikajono[ino]->rtime > 0) && aikajono[ino]->getTime(0, aikajono[ino]->rtime - 1)->kno == 0)
		  aikajono[ino]->rtime--;
	  aikajono[ino]->fp1 = aikajono[ino]->fp;
	  }
   else
	  iowerr(aikajono[ino]->aikafname,3,-1L);
   _lseek(aikajono[ino]->afhandle, 0, SEEK_SET);
	  pos = 0;
   if ((fl = aikajono[ino]->mxtime + 1 -  pos) > 0) {
	   l = pos*sizeof(aikatp);
	  if (_lseek(aikajono[ino]->afhandle, l, SEEK_SET) != l)
		 iowerr(aikajono[ino]->aikafname,0,l);
	  else {
		 int nn = 0;
		 for (int n = pos; n < aikajono[ino]->mxtime; n++) {
			if (n == 0) {
				memset(buf, 0, sizeof(aikatp));
				sprintf(buf, "AJAT_V%d", VersioInt());
				}
			else {
				aikatp ttm;
				gettm(nn++, &ttm, ino);
				memcpy(buf, &ttm, sizeof(aikatp));
				}
			if (_write(aikajono[ino]->afhandle, buf, sizeof(aikatp)) !=
			   sizeof(aikatp)) {
			   iowerr(aikajono[ino]->aikafname,2,n*sizeof(aikatp));
			   break;
			   }
			}
		 }
	  _close(aikajono[ino]->afhandle);
	  if ( (aikajono[ino]->afhandle = _wopen(aikajono[ino]->aikafname, O_RDWR | O_BINARY,
		 S_IREAD | S_IWRITE)) < 0 )
		 iowerr(aikajono[ino]->aikafname,3,-1L);
	  }
   if (aikajono[ino]->afhandle > 0) {
	  aikajono[ino]->fafhandle = _fdopen(aikajono[ino]->afhandle, "r+b");
	  setvbuf(aikajono[ino]->fafhandle, NULL, _IONBF, 0);
	  }
	aikajono[ino]->haeAktAjat();
#endif // !LUENTA
   }

static void vaihda_ajat(int ino)
   {
#if !defined(LUENTA)
#ifdef _CONSOLE
   wchar_t ch, vanhanimi[102];

   wcsncpy(vanhanimi, aikajono[ino]->aikafname, 100);
   clrln(ySize-3);
#ifndef LANGSV_K
   viwrrect(ySize-3,0,ySize-3,18,"Uusi aikatiedosto :",7,0,0);
#else // LANGSV_K
   viwrrect(ySize-3,0,ySize-3,18,"Nytt tidfil       :",7,0,0);
#endif // LANGSV_K
   inputwstr(aikajono[ino]->aikafname,37,20,ySize-3,L"\r\x1b",&ch,FALSE);
   if (ch == ESC) {
	  wcsncpy(aikajono[ino]->aikafname, vanhanimi, 38);
	  return;
	  }
   if (!wcscmpU(aikajono[ino]->aikafname, vanhanimi))
		return;
   suljeajat(ino, false);
   initajat(ino, 1);
#endif // _CONSOLE
#endif
   }

void inittimer(void)
{
	static bool initialized = false;
	for (int r_no = 0; r_no < NREGNLY; r_no++) {
		if (!regnly[r_no])
			continue;
		if ((initialized || r_no) || !ajanottofl) {
			if (regnly[r_no])
				start_regnly(r_no);
			}
		}
	if (!ajanottofl) {
		initialized = true;
		return;
		}

#if !defined(LUENTA)
   char ch, tc;
   char prs[80], st[14];
   int i, ino;
   static int maxtime;
   char key;
   int err;
   char msg[80];
	extern int aikakorjaus[MAXJONO];

   esto = TRUE;
   if (maxtime == 0) {
	   maxtime = 40;
	   for (i = 0; i < sarjaluku; i++) maxtime += nilm[i] * Sarjat[i].osuusluku * (kilpparam.valuku+1);
	   maxtime += maxtime / 4;
	   maxtime += 5000;
	   }
   for (ino = 0; ino <= maxjono; ino++) {
		if (aikajono[ino])
			continue;
		aikatp ttm;
		aikajono[ino] = new aikajonotp(maxtime);
		aikajono[ino]->aikakorjaus = aikakorjaus[ino];
		memset(&ttm, 0, sizeof(aikatp));
		ttm.t = TMAALI0*AIKAJAK;
		ttm.jono = ino;
		for (i = 0; i < maxtime; i++) {
			puttm(i, &ttm, ino);
			}
		aikajono[ino]->rwtime = 0;
		if (maxjono > 0)
			swprintf(aikajono[ino]->aikafname, L"ajat%d.lst", ino+1);
		else
			wcsncpy(aikajono[ino]->aikafname, aikaflname, sizeof(aikajono[ino]->aikafname)/2-1);
		initajat(ino, -ajanottofl);
	   if (ino == 0 && autokilp)
		   seurkrivi = aikajono[ino]->rwtime;
	   }
	tauko = FALSE;
	if (!initialized) {
		initialized = true;
		if (lahdepistehaku) {
			luelahdepisteet();
			luelahdejonot();
			}
#ifdef _CONSOLE
	   if (keytab[0].ch == KB_C_C_A && keytab[0].keycode == KB_S_C_A) {
		   tc = 0;
		  do {
			 clrln(ySize-3);

#ifndef LANGSV_K
			 vidspmsg(ySize-3,0,7,0,"Paina ajanottoon varattavaa näppäintä");
#else // LANGSV_K
			 vidspmsg(ySize-3,0,7,0,"Tryck tangenten för tidtagningen     ");
#endif // LANGSV_K
			 i = nivkeys;
			 nivkeys = 0;
			 ch = readkbd(&key, 1, 0);
			 nivkeys = i;
			 if (ch == '\r' || ch == 27 || (ch >= '0' && ch <= '9') || (toupper(ch) >= 'A' && toupper(ch) <= 'Z')) {
#ifndef LANGSV_K
				writeerror("Ajanottonäppäin ei voi olla 'Enter', 'Esc', numero tai kirjain",0);
#else // LANGSV_K
				writeerror("Tangentes kan inte vara 'Enter' eller 'Esc'",0);
#endif // LANGSV_K
				continue;
				}
			 if (ch > 32) {
				strcpy(st, "(merkki \' \') ");
				st[9] = ch;
				}
			 else st[0] = 0;
#ifndef LANGSV_K
			 sprintf(prs,"Näppäinkoodi (%d,%d) %s varattu ajanotolle, vahvista "
				"(K/E)", (int) ch, (int) key, st);
			 tc = ' ';
			 selectopt(prs, "KE", &tc);
#else // LANGSV_K
			 sprintf(prs,"Tangentkod (%d,%d) %s reserverats för tidtagningen, bekräfta "
				"(J/E)", (int) ch, (int) key, st);
			 tc = ' ';
			 selectopt(prs, "JE", &tc);
			 transl("J", "K", &tc);
#endif // LANGSV_K
			 } while (tc != 'K');
		  keytab[0].ch = ch;
		  keytab[0].keycode = key;
		  }
#endif // _CONSOLE
		err = 0;
		if (regnly[0])
			start_regnly(0);

		if (com_aika) {
			hAikacomThread = _beginthread(comajanotto, 20480, &aika_com);
			}
		if (err) {
			_itoa(err, msg+38, 10);
			writeerror(msg, 0);
			}
		timerfl = TRUE;
		}
	esto = FALSE;
#endif  // !LUENTA
   }

void remtimer()
   {
   int r;

   for (r = 0; r < NREGNLY; r++)
   if (regnly_open[r])
	   regnly_open[r] = 0;;
   if (timerfl) {
	   timerfl = 0;
	   Sleep(200);
   }
   for (r = 0; r < NREGNLY; r++)
      if (regnly[r]) rem_regnly(r);
#ifdef DBGFILE
	if (dbgtofile)
		closedbgfile();
#endif
   }

