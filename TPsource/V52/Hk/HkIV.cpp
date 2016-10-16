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

#ifdef TAULU
void paiv_taulu(void);
extern INT taulu_com;
#endif
void opendbgfile(void);

extern int initid[MAX_LAHPORTTI];
extern int keyclose[MAX_LAHPORTTI];
extern FILE *emittallfile;
extern int n_emittallfile;
	extern int aikakorjaus[MAXJONO];
//extern int odota;
static int esto = FALSE;
static int htime[MAXJONO];
int siritopen[NREGNLY];

int intv[] = {AUTOINTVAL, COMINTVAL};
int aika_head, aika_tail;
INT32 int_aika[AIKA_LUKU];
int r_no_file;

static void vaihda_ajat(int ino, int era);

// aika_syy saa seuraavat arvot riippuen ajanottotavasta:
//      0 : RS-portti
//      1 : hiiri             EI KÄYTÖSSÄ
//      2 : näppäimistö       EI KÄYTÖSSÄ
//
int aika_syy[AIKA_LUKU];

INT32 ivtime[2] = {0L,0L};
IV_KEY keytab[] = { KB_C_C_A, KB_S_C_A};
int nivkeys = 1;

#ifdef SAKKOCOM
extern INT sak_baud, sak_laji;
#endif

#ifdef MAALI

#ifdef EMITLEIMAT

#ifndef _CONSOLE
void tall_badge(UINT32 badge);
#endif

#define UL2CH(hl) (((hl).lo-48)+16*((hl).hi-48))

typedef struct {
   UINT32 badge;
   INT32 t;
   } bdg_t_tp;

#define N_BDG_T 8
static bdg_t_tp bdg_t[N_BDG_T];
UINT32 pkg_alku[NREGNLY], pkg_loppu[NREGNLY];
INT lue_rno;
char alge_tunn[10][4] = {"SZ", "ZZ", "ZW", "ZW ", "K03", "K06", "K09",
			"K02", "K05", "K08"};
#endif // EMITLEIMAT

INT32 viim_regnly;

INT r_msg_len[NREGNLY];
static INT ratakno[8];

INT lahtija(INT32 tlahto)
   {
   INT p,srj;
   kilptietue kilp;

   if (!pvparam[k_pv].hiihtolahto || tunn_srj < 0) return(0);
	if (tunn_srj == sarjaluku) srj = 0;
	else srj = tunn_srj;
	for (;;srj++) {
		EnterCriticalSection(&tall_CriticalSection);
		p = jalku[srj][4];
		while (p >= 0) {
			kilp.GETREC(p);
			if (labs(NORMKELLO(kilp.pv[k_pv].tlahto - tlahto)) <= tunn_raja) {
				LeaveCriticalSection(&tall_CriticalSection);
				return(p);
				}
			p = JarjSeur(0, 0, p);
		 }
		LeaveCriticalSection(&tall_CriticalSection);
	  if (tunn_srj < sarjaluku || srj == sarjaluku-1) break;
	  }
   return(0);
   }

int auto_lahtija(INT32 tlahto)
	{
	int d, kno = 0;
	kilptietue kilp, entkilp;

	if ((d = lahtija(tlahto)) != 0) {
		EnterCriticalSection(&tall_CriticalSection);
		kilp.GETREC(d);
		entkilp = kilp;
		kilp.tall_tulos(-1, pyoristatls(tlahto, 1));
		kilp.tallenna(d, TRUE, 0, 0, &entkilp);
		LeaveCriticalSection(&tall_CriticalSection);
		kno = kilp.id();
		}
	return(kno);
	}

	void luelahdepisteet(void)
	{
	TextFl * infile;
	wchar_t *p, *p1, Buf[200];
	INT16 srj, pst, kdi, kdi2;
	wchar_t flnm[3][20] = {L"LAHDEPISTEET1.LST", L"LAHDEPISTEET.LST", L"BOXOHJE.LST"};
	int nkdi[MAXSARJALUKU+2];
	static bool luettu = false;

	if (luettu)
		return;
	luettu = true;
	flnm[0][12] = L'1' + k_pv;
	for (int i = 0; i < 3; i++) {
		infile = new TextFl(flnm[i], L"rt");
		if (infile->IsOpen())
			break;
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
		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (p) {
			if (!wcscmpU(p, L"KAIKKI"))
				srj = sarjaluku+1;
			else if (!wcscmpU(p, L"LOPUT"))
				srj = sarjaluku;
			else
				srj = haesarja_w(p, false);
			if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				pst = _wtoi(p);
				while (pst < kilpparam.valuku+2 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
					if ((p1 = wcsstr(p, L"-")) > NULL) {
						*p1 = 0;
						kdi = _wtoi(p);
						kdi2 = _wtoi(p1+1);
						if (kdi > 0 && kdi2 >= kdi)
							nkdi[srj] += kdi2-kdi+1;
						}
					else
						if (_wtoi(p) > 0)
							nkdi[srj]++;
					}
				}
			}
		}
	infile->Rewind();

	for (srj = 0; srj <= sarjaluku; srj++) {
		if (lahdepisteet[srj])
			free(lahdepisteet[srj]);
		lahdepisteet[srj] = 0;
		if (nkdi[srj] || (srj == sarjaluku && nkdi[srj+1] > 0))
			lahdepisteet[srj] = (lahdepsttp *) calloc(nkdi[srj]+nkdi[sarjaluku+1]+1, sizeof(lahdepsttp));
		}
	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		bool kaikki = false;

		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (p) {
			if (!wcscmpU(p, L"KAIKKI")) {
				srj = sarjaluku;
				kaikki = true;
				}
			else if (!wcscmpU(p, L"LOPUT"))
				srj = sarjaluku;
			else
				srj = haesarja_w(p, false);
			if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				if (towupper(*p) == L'M' || (*p == L'0' && _wtoi(p) == 0))
					pst = 32766;
				else if (towupper(*p) == L'L' || _wtoi(p) == -1)
					pst = 32767;
				else {
					pst = _wtoi(p);
					if (pst > kilpparam.valuku)
						pst = kilpparam.valuku+1;
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
						_swab((char *)&kdi, (char *)&lahdepisteet[srj][nkdi[srj]].kdi, 2);
						_swab((char *)&pst, (char *)&lahdepisteet[srj][nkdi[srj]].piste, 2);
						if (kaikki) {
							for (int srj1 = 0; srj1 < sarjaluku; srj1++) {
								if (lahdepisteet[srj1]) {
									lahdepisteet[srj1][nkdi[srj1]].kdi = lahdepisteet[srj][nkdi[srj]].kdi;
									lahdepisteet[srj1][nkdi[srj1]].piste = lahdepisteet[srj][nkdi[srj]].piste;
									nkdi[srj1]++;
									}
								}
							}
						nkdi[srj]++;
						kdi++;
						}
					}
				}
			}
		}
	delete infile;

	for (srj = 0; srj <= sarjaluku; srj++) {
		if (nkdi[srj] < 1)
			continue;
		quicksort((char *)(lahdepisteet[srj]), nkdi[srj], 4, 4);
		for (int i = 0; i < nkdi[srj]; i++) {
			_swab((char *)&lahdepisteet[srj][i].kdi, (char *)&kdi, 2);
			_swab((char *)&lahdepisteet[srj][i].piste, (char *)&pst, 2);
			if (pst == 32766)
				pst = 0;
			if (pst == 32767)
				pst = -1;
			lahdepisteet[srj][i].kdi = kdi;
			lahdepisteet[srj][i].piste = pst;
			}
		}
	}

static int haelahdepiste(kilptietue *kilp, INT16 kdi, int tm)
	{
	lahdepsttp *kp, *kp0;

	if (kdi <= 0)
		return(kilpparam.valuku+1);
	if (kilp && (lahdepisteet[kilp->Sarja()] || lahdepisteet[sarjaluku])) {
		int pst = -3;
		if (lahdepisteet[kilp->Sarja()])
			kp0 = lahdepisteet[kilp->Sarja()];
		else
			kp0 = lahdepisteet[sarjaluku];
		for (int i = 0; i <= Sarjat[kilp->Sarja()].valuku[k_pv]; i++) {
			if (kilp->p_aika(i)) {
				pst = i;
				if (i == 0)
					break;
				}
			}
		for (kp = kp0; kp->kdi; kp++) {
			if (kp->kdi == kdi) {
				while (kp[1].kdi == kdi && ((pst == 0 && kp->piste != 0) || kp->piste < pst ||
					(kilp->p_aika(kp->piste) && NORMKELLO(tm-kilp->maali(kp->piste)) > samapisteraja)))
					kp++;
				return(kp->piste < -2 ? kilpparam.valuku+1 : kp->piste);
				}
			}
		return(kilpparam.valuku+1);
		}
	else if (lahdepisteet[sarjaluku]) {
		for (kp = lahdepisteet[sarjaluku]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				return(kp->piste < -2 ? kilpparam.valuku+1 : kp->piste);
			}
		return(kilpparam.valuku+1);
		}
	return(kilpparam.valuku+1);
	}

INT lahdejonohaku = 0;

void luelahdejonot(void)
	{
	TextFl * infile;
	wchar_t *p, Buf[200];
	INT16 srj, pst, kdi;
	wchar_t flnm[2][20] = {L"LAHDEJONOT1.LST", L"LAHDEJONOT.LST"};
	int nkdi[MAXSARJALUKU+1];

	flnm[0][10] = L'1' + k_pv;
	for (int i = 0; i < 2; i++) {
		infile = new TextFl(flnm[i], L"rt");
		if (infile->IsOpen())
			break;
		delete infile;
		infile = NULL;
		}
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
		if (p) {
			if (!wcscmp(p, L"KAIKKI"))
				srj = sarjaluku;
			else
				srj = haesarja_w(p, false);
			if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				pst = _wtoi(p);
				while (pst < kilpparam.valuku+2 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
					if (_wtoi(p) > 0)
						nkdi[srj]++;
					}
				}
			}
		}
	infile->Rewind();
	for (srj = 0; srj <= sarjaluku; srj++) {
		if (lahdejonot[srj])
			free(lahdejonot[srj]);
		lahdejonot[srj] = 0;
		if (nkdi[srj])
			lahdejonot[srj] = (lahdepsttp *) calloc(nkdi[srj]+1, sizeof(lahdepsttp));
		}
	memset(nkdi, 0, sizeof(nkdi));
	while (!infile->Feof()) {
		infile->ReadLine(Buf, 200);
		p = wcstok(Buf, L" ;\t\n");
		if (p) {
			if (!wcscmp(p, L"KAIKKI"))
				srj = sarjaluku;
			else
				srj = haesarja_w(p, false);
			if (srj >= 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
				pst = _wtoi(p);
				if (pst > MAXJONO)
					pst = 0;
				while (pst > 0 && (p = wcstok(NULL, L" ;\t\n")) != NULL) {
					if ((kdi =_wtoi(p)) > 0) {
						_swab((char *)&kdi, (char *)&lahdejonot[srj][nkdi[srj]].kdi, 2);
						_swab((char *)&pst, (char *)&lahdejonot[srj][nkdi[srj]].piste, 2);
						nkdi[srj]++;
						}
					}
				}
			}
		}
	delete infile;
	for (srj = 0; srj <= sarjaluku; srj++) {
		if (nkdi[srj] < 1)
			continue;
		quicksort((char *)(lahdejonot[srj]), nkdi[srj], 4, 4);
		for (int i = 0; i < nkdi[srj]; i++) {
			_swab((char *)&lahdejonot[srj][i].kdi, (char *)&kdi, 2);
			_swab((char *)&lahdejonot[srj][i].piste, (char *)&pst, 2);
			lahdejonot[srj][i].kdi = kdi;
			lahdejonot[srj][i].piste = pst;
			}
		}
	}

static int haelahdejono(kilptietue *kilp, INT16 kdi)
	{
	lahdepsttp *kp;
	int jono;

	jono = emitjono;
	if (kilp && lahdejonot[kilp->Sarja()]) {
		for (kp = lahdejonot[kilp->Sarja()]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste;
			}
		}
	else if (lahdejonot[sarjaluku]) {
		for (kp = lahdejonot[sarjaluku]; kp->kdi; kp++) {
			if (kp->kdi == kdi)
				jono = kp->piste;
			}
		}
	if (jono < 0)
		jono = 0;
	return(jono);
	}

#ifdef EMITLEIMAT

static int tunn_on_jo(INT32 t, int kno)
{
	int ero, rtm, ino;
	aikatp tm;
	kilptietue kilp;

	ino = tunnjono;
	if (!aikajono[ino])
		return(0);
	for (rtm = aikajono[ino]->aktrows-1; rtm >= 0; rtm--) {
		 ero = NORMKELLO(purajak(aikajono[ino]->getAktTime(&tm, rtm)->t) - t);
		 if (tm.kno == kno) {
			 if (ero >= SEK*tunnrajat[0] && ero <= SEK*tunnrajat[1])
				 return(1);
			 else
				 return(-1);
			}
		}
	return(0);
}

int tunn_hae_kno(INT32 t)
{
	int ero, rtm, d, ino, kno = 0;
	aikatp tm;
	kilptietue kilp;

	ino = tunnjono;
	if (!aikajono[ino])
		return(0);
	for (rtm = aikajono[ino]->aktrows - 1; rtm >= 0 &&
		(ero = NORMKELLO(purajak(aikajono[ino]->getAktTime(&tm, rtm)->t)) - t) >= SEK*tunnrajat[0]; rtm--) {
		 if (tm.kno != 0 && ero <= SEK*tunnrajat[1]) {
			if ((d = getpos(tm.kno)) > 0)
				kilp.GETREC(d);
			if (kilp.p_aika(0) == 0)
				kno = tm.kno;
			}
		}
	return(kno);
}

static INT32 tunn_hae_aika(INT32 t, int kno)
{
	INT32 t1 = TMAALI0;
	int ero, rtm, ino = 0;
	aikatp tm, tm1;

	if (tunnjono == 0)
		ino = 1;
	if (!aikajono[ino])
		return(0);
	EnterCriticalSection(&ajat_CriticalSection);
	for (rtm = aikajono[ino]->aktrows-1; rtm >= 0 &&
		(ero = NORMKELLO(t - purajak(aikajono[ino]->getAktTime(&tm, rtm)->t))) <= SEK*tunnrajat[1]; rtm--) {
		 if (tm.kno == 0 && ero >= SEK*tunnrajat[0]) {
			 t1 = purajak(aikajono[ino]->getAktTime(&tm, rtm)->t);
			tm1 = tm;
			tm1.kno = kno;
			tall_rivi(ino, &tm1, &tm, &rtm, 0, 0, 0, true);
			break;
			}
		}
	LeaveCriticalSection(&ajat_CriticalSection);
	return(t1);
}

void tall_elahto(int badge, int t)
{
	int d, jono = 0;
	kilptietue kilp;

	if (aikajono[emitjono]) {
		aikatp itm;
		memset(&itm, 0, sizeof(itm));
		itm.badge = badge;
		itm.kno = bdg2kno(badge);
		itm.piste = -1;
		t += aikajono[emitjono]->aikakorjaus;
		itm.t = AIKAJAK * t;
		itm.date = tm_date(itm.t);
		tall_rivi(emitjono, &itm, NULL, NULL, 0, 0, 0, false);
		}
	if ((d = getpos(bdg2kno(badge))) > 0) {
		EnterCriticalSection(&tall_CriticalSection);
		kilp.GETREC(d);
		if ((kilp.tark() == L'E' || kilp.tark() == L'V') || eNollausAjat > 1) {
			kilp.set_tark(L'-');
			if (eNollausAjat)
				kilp.tall_laika(t);
			kilp.tallenna(d, 0, 0, 0, 0);
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
}

void tall_etulos(INT32 badge, INT32 t, INT32 tms, INT r_no, int Jono)
   {
   INT kno, d, piste = -2, jono = 0;
   kilptietue kilp, entkilp;
   aikatp itm;

	if (Jono >= 0)
		jono = Jono;
	else if (tunnjono >= 0)
		jono = tunnjono;
	else
		jono = emitjono;
	if (!aikajono[jono])
		return;
	memset(&itm, 0, sizeof(itm));
   itm.badge = badge;
   itm.kanava = r_no+1;
	if (r_no >= NREGNLY) {
		itm.kanava = r_no - NREGNLY + 1;
		}
	else if (lukijalahde[r_no]) {
		itm.kanava = lukijalahde[r_no];
		itm.tapahtuma = r_no+4;
		}
   kno = bdg2kno(badge);
   itm.t = (tms-43200000L) * 10L;
   itm.date = tm_date(itm.t);
   t = purajak(itm.t);
	//
	// Pisteen määrääminen
	//
	// Katsotaan ensin, määrääkö AIKALUKIJAx=VAINy pisteen
	// vainpiste[x] == -9 kertoo, että parametria ei ole annettu
	// vainpiste[x] == -3 kertoo, että käytetään lähdepistehakua
	// Jos lukijalahde[r_no] != käytetään aina lähdepsitehakua
	// Jos parametri on AIKALUKIJAx=VAINA, saa piste nyt arvon -2
	//
	if (lahdepistehaku)
		piste = -3;
	if (r_no < NREGNLY && !lukijalahde[r_no]) {
	   if (vainpiste[r_no+1] != -9) {
		   piste = vainpiste[r_no+1];
			}
	   else if (vainpiste[0] != -9) {
			piste = vainpiste[0];
			}
		if (piste > kilpparam.valuku)
			piste = -3;
		}

	if ((d = getpos(kno)) > 0) {
		bool tallfl = false;
		int tt;

		itm.kno = kno;
		kilp.GETREC(d);
		entkilp = kilp;
		t += aikajono[jono]->aikakorjaus;
		tt = itm.t;
		itm.t += aikajono[jono]->aikakorjaus * AIKAJAK;
		itm.date = tm_copydate(itm.t, tt, itm.date);

		// Lähdepistehakua käytetään, ellei piste määräytynyt yllä tai
		// arvolla piste == -2 ole ilmaistu, että lähdepistehakua ei käytetä

		if (lahdepistehaku) {
			if (piste < 0 && piste != -2)
				piste = haelahdepiste(&kilp, itm.kanava, t);
			jono = haelahdejono(&kilp, itm.kanava);
			}
		itm.jono = jono;

		// piste == -2, jos AIKALUKIJAx=VAINA on sitä pyytänyt

		if (piste == -2) {
			piste = maaraa_piste(&kilp, t);
			}
		if (piste == -3) {
			piste = 0;
			}
		 if (tunnjono >= 0) {
			t = tunn_hae_aika(t, kno);
			}
		if (piste >= 0 && t != TMAALI0) {
			INT32 tls0;

			itm.piste = piste;
			if (estaneg &&
				(NORMTULOS(purajak(itm.t) - kilp.pv[k_pv].tlahto) > 20*TUNTI ||
				 NORMTULOS(purajak(itm.t) - kilp.pv[k_pv].tlahto) < estaneg))
				 return;
			if (kilp.TLahto() != TMAALI0 && (tls0 = kilp.maali(piste)) == TMAALI0 || (uusinaika && (abs((long) NORMKELLO(t - tls0)) < uusinaika))) {
				tls0 = kilp.set_tulos(piste, pyoristatls(t, 1));
				if (kilpparam.hyvHavaittu && kilp.tark() == L'E' && tls0 > 0 && tls0 < 12 * TUNTI)
					kilp.set_tark(L'B');
				tallfl = true;
				}
			}
		if (piste == -1) {
			itm.piste = piste;
			if (pvparam[k_pv].hiihtolahto && kilp.TLahto(k_pv, true) == kilp.TLahto(k_pv, false)) {
				kilp.set_tulos(-1, t, 1);
				tallfl = true;
				}
			if (!pvparam[k_pv].hiihtolahto && (kilp.tark() == L'E' || kilp.tark() == L'B')) {
				kilp.set_tark(L'-');
				tallfl = true;
				}
			}
		if (tallfl) {
			EnterCriticalSection(&tall_CriticalSection);
			kilp.tallenna(d,TRUE, 0, 0, &entkilp);
			LeaveCriticalSection(&tall_CriticalSection);
			}
		}
	else {
		itm.piste = (piste < -2) ? 0 : piste;
		}
   tall_rivi(jono, &itm, NULL, NULL, 0, 0, 0, false);
   }

#endif // EMITLEIMAT || COMETNO

#if defined(EMITLEIMAT)

#define E_TLSLUKU 100

typedef struct {
	INT32  t1;
	INT32  t;
	UINT32 badge;
	INT    r_no;
	} e_tulostp;

static e_tulostp *etls;
static int etls_head, etls_tail;
static CRITICAL_SECTION etls_CriticalSection;

void close_bdg_t(void)
   {
	DeleteCriticalSection(&etls_CriticalSection);
	if (etls)
		free(etls);
	etls = NULL;
	}

static void tall_bdg_t(UINT32 badge, INT32 t, INT32 t1, int r_no)
{
	int i, seuraava;
   static UINT32 edbadge[NREGNLY+256];

    regnlyhetki[r_no] = t_time_l(biostime(0,0), t0);
	if (kaikki_ajat[r_no+1] && !ohitatoisto) {
		if (badge != edbadge[r_no])
			tall_etulos(badge, t, t1, r_no, -1);
		edbadge[r_no] = badge;
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
		tall_badge(badge);
	else
#endif
	if (kaikki_ajat[r_no+1] && ohitatoisto) {
		tall_etulos(badge, t, t1, r_no, -1);
		}
}

static void pros_bdg_t(LPVOID lpCn)
   {
   INT r_no;
   INT32 t, t1;
	UINT32 badge;

	while (taustaon && !vaiheenvaihto && timerfl) {
		while (etls_head != etls_tail) {
		   EnterCriticalSection(&etls_CriticalSection);
			etls_tail = (etls_tail+1) % E_TLSLUKU;
			t1 = etls[etls_tail].t1;
			t = etls[etls_tail].t;
			badge = etls[etls_tail].badge;
			r_no = etls[etls_tail].r_no;

			LeaveCriticalSection(&etls_CriticalSection);

			tall_bdg_t(badge, t, t1, r_no);

			}
		Sleep(50);
		}
	close_bdg_t();
   }

void add_bdg_t(UINT32 badge, INT r_no, char byte9, UINT32 tms)
   {

   INT32 t, t1;
   int n;

	static HTHREAD hEtlsThread;
	static UINT32 edbadge[NREGNLY+256];

	if (!timerfl)
		return;
	if (etls == NULL) {
		etls = (e_tulostp *) calloc(E_TLSLUKU, sizeof(e_tulostp));
		etls_head = 0;
		etls_tail = 0;
		InitializeCriticalSection(&etls_CriticalSection);
		hEtlsThread = _beginthread(pros_bdg_t, 40960, &n);
		}
	if (lahdepistehaku && byte9 > 0 && !lukijalahde[r_no] &&
		vainpiste[r_no+1] <= -3 && vainpiste[0] <= -3)
		r_no = byte9 + NREGNLY - 1;
	if (badge == edbadge[r_no])
		return;
	edbadge[r_no] = badge;
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
   LeaveCriticalSection(&etls_CriticalSection);
	}

static INT32 hae_bdg_t(UINT32 badge, INT uusin)
   {
   INT i,k;
   INT32 t;

   for (i = 0; i < 8; i++) {
      if (uusin)
         k = 7-i;
		else
         k = i;
      if (bdg_t[k].badge == badge) break;
      }
   if (i >= 8)
      t = biostime(0,0);
   else {
      t = bdg_t[k].t;
	  memmove(bdg_t+k, bdg_t+k+1, (7-k)*sizeof(bdg_t_tp));
      bdg_t[7].badge = 0;
      }
	return(KSEK*t_time_l(t, t0));
   }

#ifdef EMITFILE
FILE *emitinfile;

int file2emit(void)
	{
	emittp em;
   san_type vastaus;
   long tm;
   int i;
   wchar_t ch;
   char che;

   if (!emitinfile) {
	  emitinfile = _wfopen(L"EMIT_IN.DAT", L"rb");
	  }
   if (!emitinfile)
		return(1);
#ifdef _CONSOLE
   if (luesuoraan == 1) {
	  clrln(ySize-3);
	  vidspwmsg(ySize-3,0,7,0,L"Paina ESC keskeyttääksesi lukeminen");
	  }
#endif
   do {
	  if (!fread(&em, sizeof(emittp), 1, emitinfile)) {
		 writeerror_w(L"Kaikki tiedot luettu", 0, true);
         return(1);
         }
      memset(&vastaus, 0, sizeof(vastaus));
      vastaus.r13.pkgno = em.package;
		memcpy(vastaus.r13.badge, (char *) &em.badge, 3);
      vastaus.r13.week = (char) em.badgeweek;
      vastaus.r13.year = (char) em.badgeyear;
	  tm = KSEK*em.time - t0_regnly + 48*TUNTI;
      vastaus.r13.t_hour = (char) (tm/TUNTI);
      tm = tm - TUNTI * vastaus.r13.t_hour;
      vastaus.r13.t_hour = vastaus.r13.t_hour % 24;
      vastaus.r13.t_min = (char) (tm/MINUUTTI);
      tm = tm - MINUUTTI * vastaus.r13.t_min;
	  vastaus.r13.t_sek = (char) (tm/SEK);
      tm = tm - SEK * vastaus.r13.t_sek;
      vastaus.r13.t_ms = (UINT) (tm*(1000L/SEK));
		for (i = 0; i < MAXNLEIMA; i++) {
         vastaus.r13.c[i].code = em.ctrlcode[i];
         vastaus.r13.c[i].t1 = em.ctrltime[i];
         }

	  tall_emit(&vastaus, 0, r_no_file);
		if (luesuoraan == 1) {
		 ch = readkbd(&che, 0, spoolfl);
		 if (ch == ESC) {
			break;
			}
		 }
	  } while (luesuoraan == 1);
   if (luesuoraan == 1) {
	  clrln(ySize-3);
	  }
   return(0);
   }
#endif

static bool samatleimat(emittp *em1, emittp *em2, int regnlaji)
{
//	if (regnlaji < 14)
//		return(memcmp(em1->ctrlcode, em2->ctrlcode,
//			sizeof(em1->ctrlcode)+sizeof(em1->ctrltime)) == 0);

	for (int i = 0; i < MAXNLEIMA; i++) {
		if (((em1->ctrlcode[i] > 0 && em1->ctrlcode[i] < 240) ||
			(em2->ctrlcode[i] > 0 && em2->ctrlcode[i] < 240)) &&
			(em1->ctrlcode[i] != em2->ctrlcode[i] ||
			 em1->ctrltime[i] != em1->ctrltime[i]))
			 return(false);
		}
	return(true);
}

INT tall_emit(san_type *vastaus, UINT32 *vahvistus, INT r_no)
   {
   static INT uusin = 0;
   INT i, smm, ssek, sosat;
   emittp em;
	INT32 emtm;
   static emittp *ed_em[NREGNLY];
   INT mm, sek, osat;
#ifdef SPORTIDENT
	INT32 start;
#endif

	regnlyhetki[r_no] = t_time_l(biostime(0,0), t0);
	if (!ed_em[r_no]) {
	  if ((ed_em[r_no] = (emittp *) malloc(sizeof(emittp))) == 0) {
		 writeerror_w(L"Muisti ei riitä Emit-tiedoille", 0);
		 return(1);
		 }
	  }
	memset(&em, 0, sizeof(emittp));
   em.time = TMAALI0/KSEK;
   em.maali = TMAALI0/KSEK;
   em.kirjaus = TMAALI0/KSEK;
   if (regnly[r_no] == LID_EMITKELLO) {
	  if (vastaus->r11.tunnus != 'K' && vastaus->r11.tunnus != 'L' ||
		 vastaus->r11.cr != 13)
		 return(1);
	  vastaus->r11.loppu[3] = 13;
	  vastaus->r11.loppu[4] = 10;
	  vastaus->r11.cr = 0;
	  if (loki)
			kirjloki((char *) vastaus);
	  if (vastaus->r11.tunnus == 'K') {
         vastaus->r11.kp = 0;
		 mm = atoi(vastaus->r11.min);
		 vastaus->r11.p = 0;
         sek = atoi(vastaus->r11.sek);
         vastaus->r11.osat[2] = 0;
         osat = atoi(vastaus->r11.osat);
         em.maali = (osat*SEK/100 + SEK*sek + MINUUTTI*mm)/KSEK;
		 if (em.maali) em.maali += t0_regn[r_no]/KSEK;
         else em.maali = TMAALI0/KSEK;
			}
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
      for (i = 0; i < MAXNLEIMA; i++) {
         em.ctrlcode[i] = UL2CH(vastaus->r11.u[8+3*i]);
		 em.ctrltime[i] = (unsigned) (UL2CH(vastaus->r11.u[9+3*i]) +
			256L*UL2CH(vastaus->r11.u[10+3*i]));
//         if (em.ctrltime[i] > ennakko) em.ctrltime[i] -= ennakko;
		 em.time = (sosat*SEK/100 + SEK*ssek + MINUUTTI*smm)/KSEK + t0_regn[r_no]/KSEK;
			}
	  }
   if (regnly[r_no] == LID_LUKIJA) {
	  em.package = 20000000L + r_no;
	  for (i = 0; i < r_msg_len[r_no]; i++)
		 vastaus->bytes[i] ^= '\xdf';
	  em.badge = *(UINT32 *) vastaus->r12.badge & 0xffffffL;
	  em.badgeweek = vastaus->r12.week;
	  em.badgeyear = vastaus->r12.year;

	  em.time = hae_bdg_t(em.badge, uusin)/KSEK;
	  em.maali = TMAALI0/KSEK;
		for (i = 0; i < MAXNLEIMA; i++) {
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
	if (regnly[r_no] == LID_MTR) {
	  em.package = vastaus->r13.pkgno;
	  em.badge = *(UINT32 *) vastaus->r13.badge & 0xffffffL;
	  em.badgeweek = vastaus->r13.week;
	  em.badgeyear = vastaus->r13.year;
	  em.time = (vastaus->r13.t_ms*SEK/1000 + SEK*vastaus->r13.t_sek +
		 MINUUTTI*vastaus->r13.t_min + TUNTI*vastaus->r13.t_hour)/KSEK + t0_regn[r_no]/KSEK;
	  em.maali = TMAALI0/KSEK;
		for (i = 0; i < MAXNLEIMA; i++) {
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
   if (regnly[r_no] == LID_EMITAG) {
		tulk_emiTag((char *) vastaus->bytes+1, &em, &emtm, r_no);
		if (emittime && emtm != TMAALI0*(1000/SEK))
			em.time = emtm/100;
		else
			em.time = hae_bdg_t(em.badge, uusin)/KSEK;
		if (!em.badge || kaikki_ajat[r_no+1] == 2)
			return(0);
		// Vaadi että leimojen lopussa on lukijakoodi
		for (i = MAXNLEIMA-1; i > 1; i--)
			if (em.ctrlcode[i] > 0)
				break;
		if (IntInList(em.ctrlcode[i], L"240 243 244 250 253") < 0 &&
			IntInList(em.ctrlcode[i-1], L"240 243 244 250 253") < 0)
			return(0);
	  em.maali = TMAALI0/KSEK;
	  }
   if (emitTunnistus) {
	   emitLuettu(em.badge);
	   return(0);
	   }
#ifdef SPORTIDENT
   if (regnly[r_no] == LID_SPORTIDENT) {
      em.badge = vastaus->r21data.badge;
		em.time = vastaus->r21data.lukija;
		if (vastaus->r21data.start == 61166L)
			vastaus->r21data.start = TMAALI0;
		start = vastaus->r21data.start;
		if (vastaus->r21data.check == 61166L)
			vastaus->r21data.check = TMAALI0;
		if (vastaus->r21data.finish == 61166L)
			vastaus->r21data.finish = TMAALI0;
      em.maali = TMAALI0/KSEK;
      for (i = 0; i < MAXNLEIMA; i++) {
         em.ctrlcode[i] = vastaus->r21data.cc[i];
         em.ctrltime[i] = vastaus->r21data.ct[i];
			if (start == TMAALI0 && em.ctrltime[i])
				start = em.ctrltime[i];
			if (em.ctrltime[i] && start != TMAALI0) {
				em.ctrltime[i] =
					(vastaus->r21data.ct[i] - start + 86400L) % 86400L;
				}
         }
		for (i = MAXNLEIMA; i > 1; i--)
			if (em.ctrlcode[i-1])
				break;
		if (i < MAXNLEIMA-1 && i > 1) {
			if (vastaus->r21data.finish != TMAALI0 && start != TMAALI0) {
				em.ctrlcode[i] = 240;
				em.ctrltime[i] =
					(vastaus->r21data.finish - start + 86400L) % 86400L;
				i++;
				}
			em.ctrlcode[i] = 250;
			if (start != TMAALI0)
				em.ctrltime[i] =
					(vastaus->r21data.lukija/10 - start + 86400L) % 86400L;
			else
				em.ctrltime[i] = (vastaus->r21data.lukija/10 + 86400L) % 86400L;
			}
		}
#endif
   if (em.time != TMAALI0/KSEK)
      em.time = ((em.time + 36*TUNTI/KSEK)%(24*TUNTI/KSEK) - 12*TUNTI/KSEK);
	em.badge0 = em.badge;
#ifndef _CONSOLE
	if (emittallfile) {
		fwrite(&em, sizeof(em), 1, emittallfile);
		n_emittallfile++;
		return(0);
		}
#endif
   if (em.badge && em.badge == ed_em[r_no]->badge && samatleimat(&em, ed_em[r_no], regnly[r_no])) {
	  if (vahvistus) *vahvistus = em.badge;
	  }
   else if (em.badge > 0) {
	  *(ed_em[r_no]) = em;
	  addem(&em, 0);
	  }
   return(0);
   }

static int in_pros_emit;

void lueSeuraavaEmit(void)
{
	in_pros_emit = 0;
}

INT pros_emit(kilptietue *kilp0, wchar_t *msgtxt)
   {
   static INT uusin = 1, edkno = -1;
   INT16 ibdg = 0;
   INT kbg, aika_err, i, piste = 0, d, kno, kno1 = 0, tallkilp = 0, korjaus_fl = 0;
   INT j, kysytty = 0, luetoiselle = 0;
   INT32 tlahto, rbadge = 0;
   char ch;
   char che;
   wchar_t msg[121], as[16], wch, wch2, tark = L'-';
#ifndef __linux__
   char *svscr = 0;
#else
   USHORT *svscr = 0;
#endif
   char savebuf[65*(MAXRIVI-3)];
   emittp em;
   kilptietue kilp, entkilp;
   ratatp *rt;
   aikatp itm, etm;
   INT ptm;

   if (in_pros_emit || emithead2 >= emithead || !ProsEmitOn)
	  return(0);
   in_pros_emit = 1;
   lueem(&em, emithead2);
   if (em.kirjaus != TMAALI0/KSEK) {
	  emithead2++;
	  in_pros_emit = 0;
	  return(0);
	  }
   if (luesuoraan == 1)
	  Sleep(luesuoraanviive);
   em.kirjaus = t_time_l(biostime(0,0), t0);
   if (em.package > 10000000L && em.package < 11000000L)
	  rbadge = em.package - 10000000L;
   if (em.badge == 0) {
	   em.kilpno = 0;
	   putem(&em, emithead2, 0);
	   in_pros_emit = 0;
	   return(0);
	   }
#ifdef _CONSOLE
	if (em.badge == 200 && memcmp(&em.sc, "emiTag", 6) != 0) {
		kno = 0;
		kbg = 0;
		writeerror_w(L"Koodinsa menettänyt kortti (näyttää 200). Vastaa jatkokysymyksiin", 0);
		for (;;) {
			wch = L' ';
			wselectopt(L"A)nna kortin koodi, anna kilpailijan N)umero, H)ae nimellä", L"ANH", &wch);
			if (wch == L'A') {
				clrln(ySize-3);
				vidspwmsg(ySize-3, 0, 7, 0, L"Kortin koodi :               Esc: peruuta valinta");
				INPUTINTW(&kbg, 8, 16, ySize-3, L"\r\x1b", &wch2);
				if (wch2 == ESC)
					continue;
				if ((kno = bdg2kno(kbg)) < minkilpno) {
					wch2 = L' ';
					wselectopt(L"Koodia vastaavaa kilpailijaa ei löydy. K)äytä silti, U)usi valinta", L"KU", &wch2);
					if (wch2 == L'U')
						continue;
					}
				else if (getEmitJarr(kno, &ibdg) > 0) {
					wch2 = L' ';
					wselectopt(L"Koodia vastaavalla kilpailijalla jo leimat. K)äytä silti, U)usi valinta", L"KU", &wch2);
					if (wch2 == L'U')
						continue;
					}
				em.badge = kbg;
				break;
				}
			if (wch == L'N' || wch == L'H') {
				if (wch == L'N') {
					clrln(ySize-3);
					vidspwmsg(ySize-3, 0, 7, 0, L"Kilpailijan numero :               Esc: peruuta valinta");
					INPUTINTW(&kno, 4, 22, ySize-3, L"\r", &wch2);
					if (wch2 == ESC)
						continue;
					if ((d = getpos(kno)) <= 0) {
						writeerror_w(L"Virheellinen kilpailijanumero", 0);
						continue;
						}
					kilp.GETREC(d);
					}
				else {
					virdrect(0,0,ySize-4,64,savebuf,0);
					if (kilp.find(&d, 0)) {
						viwrrect(0,0,ySize-4,64,savebuf,7,0,0);
						continue;
						}
					viwrrect(0,0,ySize-4,64,savebuf,7,0,0);
					kno = kilp.id();
					}
				if (kilp.tark(k_pv) == L'P') {
					writeerror_w(L"Kilpailija merkitty poissa olevaksi. Ei käytettävissä.", 0);
					continue;
					}
				if (kilp.pv[k_pv].badge[0] == 0) {
					wch2 = L' ';
					wselectopt(L"Kilpailijalla ei Emit-koodia. V)alitse uudelleen. L)uo koodi automaattisesti", L"VL", &wch2);
					if (wch2 == L'V')
						continue;
					kilp.pv[k_pv].badge[0] = haeSeurVapaaBadge(0);
					kilp.PUTREC(d, 1);
					if (comfl)
						laheta(d, kilp.kilpno, -1-k_pv, 0, 0, 0, 0);
					addbadge(kilp.pv[k_pv].badge[0], kno, 1);
					}
				em.badge = kilp.pv[k_pv].badge[0];
				break;
				}
			}
		}

   if (em.badge && (kno = bdg2kno(em.badge)) > 0) {
	   kno1 = kno;
	   if ((d = getpos(kno)) > 0) {
		  kilp.GETREC(d);
		  if (kilpparam.kaksibadge == 2 && em.badge == kilp.pv[k_pv].badge[1])
			  em.osuus = 1;
		  tark = kilp.tark(k_pv);
		  }

	  if (!korjaus_fl && (getEmitJarr(bdg2kno(em.badge), &em.osuus) != -1 || (tark != L'-' && tark != L'B' && (eilavoin || tark != L'E')))) {
		 clrleimat();
		 kysytty = 1;
		 if (luesuoraan == 1) {
			wch = L'T';
			}
		 else {
			swprintf(msg, L"Käsittelyssä kortti %ld    ", em.badge);
				vidspwmsg(ySize-1, 0, 7, 0, msg);
			   naytatulos(3);
			if (getEmitJarr(bdg2kno(em.badge), &em.osuus) != -1)
				swprintf(msg, L"Kortti %ld luettu jo kilpailijalle %d "
				   L"lue S)amalle, T)oiselle, O)hita", em.badge, bdg2kno(em.badge));
			else
				swprintf(msg, L"Kortin %ld kilpailijalla %d jo merkintä %c."
				   L"Lue S)amalle, T)oiselle, O)hita", em.badge, bdg2kno(em.badge), tark);
			wch = L' ';
			erbeep();
			erbeep();
			erbeep();
			if (!selaus)
			   outleimat(0, 0, 0, 0);
			   wselectopt(msg, L"OST", &wch);
			}
		 if (wch == L'S') {
			wch = L' ';
			wselectopt(L"Aiemmin luetut tiedot korvataan uusilla - vahvista (K/E)", L"KE", &wch);
			if (wch == L'E')
			   wch = L'O';
			else
			   kysytty = 2;
			}
		 if (wch == L'S') {
			   kysytty = 2;
			}
		 if (wch == L'O') {
//            em.kirjaus = t_time_l(biostime(0,0), t0);
				if (emithead2 < emithead-1) {
					wch = L'E';
					swprintf(msg, L"Käsittelemättä %d tietuetta. J)atka seuraavasta, K)eskeytä", emithead-emithead2);
					wselectopt(msg, L"JK", &wch);
					if (wch == L'K') {
						in_pros_emit = 0;
						return(-1);
						}
					}
			putem(&em, emithead2, 0);
			emithead2++;
			in_pros_emit = 0;
			return(em.badge0);
			}
		 if (wch == L'T')
				luetoiselle = 1;
		 }
	  }
   if (em.badge && (luetoiselle || !bdg2kno(em.badge))) {
	  kno = 0;
	  if (!luesuoraan) {
		 erbeep();
		 }
	  svscr = savescr(0, 0, ySize-1, 79);
	  do {
		 wch = L' ';
		 if (!selaus)
		   outleimat(0, 0, 0, 0);
		 clrleimat();
		 if (!kysytty) {
			if (!luesuoraan) {
			   naytatulos(3);
			   erbeep();
			   erbeep();
			   }
			// Seuraavan tarkoitus on ohittaa tilanne mmutamassa sekunnissa, kun
			// ajanotto on mukana
			if (kaikki_ajat[0]) {
			   kbflush();
			   swprintf(msg,
				  L"Uusi Emit-koodi %ld. Paina viestin poistuttua Enter käsitelläksesi",
				  em.badge);
			   writeerror_w(msg, 2000);
			   for (i = 0; i < 4; i++) {
				  utsleep(18);
				  ch = readkbd(&che, 0, 0);
				  if (ch == '\r') break;
				  }
			   if (ch != '\r')
					wch = ESC;
			   else
					wch = L' ';
			   }
			else if (!luesuoraan) {
			   swprintf(msg,
				  L"Uusi Emit-koodi %ld - vastaa jatkokysymyksiin.", em.badge);
			   writeerror_w(msg, 0);
			   }
			}
		 if (wch == L' ') {
			if (luesuoraan) {
			   wch = L'L';
			   }
			else {
				swprintf(msg, L"Käsittelyssä kortti %ld    ", em.badge);
				vidspwmsg(ySize-1, 0, 7, 0, msg);
			   if (kysytty) {
				  wcscpy(msg, L"Anna K)ilpailijanro, H)ae, L)isää uusi tietue, Esc: ohita");
				  }
			   else {
					   naytatulos(3);
				  swprintf(msg, L"Uusi Emit-koodi %ld Anna K)ilpailijanro, H)ae, "
					 L"L)isää, Esc: ohita", em.badge);
						}
			   wselectopt(msg, L"KHL\x1b", &wch);
			   }
			}
		 switch (wch) {
			case L'K':
			   clrln(ySize-3);
			   for(;;) {
				  if (em.time != TMAALI0/KSEK)
					 AIKATOWSTRS(as, em.time*KSEK, t0);
				  else
					 as[0] = 0;
				  as[8] = 0;
				  swprintf(msg, L"Anna korttia %ld vastaava kilpailijanro:      "
					 L"   %s%8s. Esc: Peru", em.badge, as[0] ? L"Luettu: " : L"", as);
					if (annaseur)
						kno = edkno+1;
				  vidspwmsg(ySize-3,0,7,0, msg);
				  INPUTINTW(&kno, 4, 44, ySize-3, L"\r\x1b", &wch);
				  edkno = kno;
				  if (wch == 27) {
					 break;
					 }
				  if ((d = getpos(kno)) < 1) {
					 writeerror_w(L"Tuntematon kilpailijanumero", 0);
					 }
				  else if (getEmitJarr(kno, 0) != -1
//						&& (kilpparam.kaksibadge != 2 || getEmitJarr(kno, 1) != -1)
						) {
					 writeerror_w(L"Kilpailijanumeron tiedot jo tallennettu. "
						L"Anna oikea numero tai varanumero", 0);
					 }
				  else break;
				  }
			   break;
			case L'H':
					do {
						do {
							if (kilp.find(&d, 0)) {
								wch = ESC;
								break;
								}
							kno = kilp.id();
						swprintf(msg, L"Käsittelyssä kortti %ld    ", em.badge);
							vidspwmsg(ySize-1, 0, 7, 0, msg);
							wselectopt(L"H)yväksy valinta, U)usi valinta, Esc : Peruuta",
								L"HU\x1b", &wch);
							} while (wch == L'U');
						for (i = 2; i < ySize-4; i++)
							clrtxt(i, 0, 64);
						if (wch == ESC) {
							wch = L' ';
							wselectopt(L"Haku peruttu, L)isää tietue, U)usi haku, O)hita tieto", L"LUO", &wch);
							if (wch == L'O')
								wch = ESC;
							}
					} while (wch == L'U');
					if (wch != L'L')
						break;
			case L'L':
			   if (kysytty) {
				  }
					if (luetoiselle)
						vaihda_badge(em.badge);
					luetoiselle = 0;
			   if ((d = add(&em)) == 0)
				  wch = ESC;
			   for (i = 2; i < ySize-4; i++)
				  clrtxt(i, 0, 64);
			   break;
			}
		 if (wch == ESC) {
			kno = 0;
				if (emithead2 < emithead-1) {
					wch = L'E';
					swprintf(msg, L"Käsittelemättä %d tietuetta. J)atka seuraavasta, K)eskeytä", emithead-emithead2);
					wselectopt(msg, L"JK", &wch);
					if (wch == L'K') {
						in_pros_emit = 0;
						return(-1);
						}
					}
				wch = ESC;
			break;
			}
			else {
				kilp.GETREC(d);
				entkilp = kilp;
			kno = kilp.id();
			if (wch != L'L') {
					wch = L' ';
			   wselectopt(L"Haluatko muokata kilpailijan tietoja (K/E)", L"KE",
				  &wch);
			   if (wch == L'K') {
				   for (kbg = k_pv; kbg < kilpparam.n_pv_akt; kbg++)
					  kilp.pv[k_pv].badge[0] = em.badge;
						kilp.outkilp(0);
						korjaa(d, &kilp, 1, &entkilp);
						kilp.GETREC(d);
				  kno = kilp.id();
				  }
			   wch = L'L';
			   }
			}
		 } while (wch != L'I' && wch != L'O' && wch != L'L');
		if (luetoiselle)
		   vaihda_badge(em.badge);
	  if (!kno) {
//         em.kirjaus = t_time_l(biostime(0,0), t0);
		 putem(&em, emithead2, 0);
		 emithead2++;
		 in_pros_emit = 0;
			if (svscr)
				restorescr(0, 0, ySize-1, 79, svscr);
		 return(em.badge);
		 }
		entkilp = kilp;
		for (kbg = k_pv; kbg < kilpparam.n_pv_akt; kbg++)
		  kilp.pv[k_pv].badge[0] = em.badge;
		EnterCriticalSection(&tall_CriticalSection);
		kilp.tallenna(d, 0, 0, 0, &entkilp);
		LeaveCriticalSection(&tall_CriticalSection);
	  }

#else   // !_CONSOLE

	if (kilp0)
		kilp0->nollaa();
	if (em.badge == 200 && memcmp(&em.sc, "emiTag", 6) != 0) {
		if (esiluenta) {
			wcscat(msgtxt, L"Viallinen kortti (koodi 200). Käyttö estetty. ");
			em.badge = 0;
			putem(&em, emithead2, 0);
			emithead2++;
			in_pros_emit = 0;
			return(-1);
			}
		else {
			wcscat(msgtxt, L"Luettu koodi 200 - vaihdettu. ");
//			for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
//			if (kuntosuunnmoodi == 1 && KuntoMaali >= 0 && seurVapaaBadge > 1 && bdg2kno(seurVapaaBadge-1) == 0)
//				seurVapaaBadge--;
			em.badge = haeSeurVapaaBadge(0);
			}
		}
   if (em.badge && (kno = bdg2kno(em.badge)) > 0) {
	   kno1 = kno;
	   if ((d = getpos(kno)) > 0) {
		  kilp.GETREC(d);
		  if (kilpparam.kaksibadge == 2 && em.badge == kilp.pv[k_pv].badge[1])
			  em.osuus = 1;
		  tark = kilp.tark(k_pv);
		  if (kilp0)
			 *kilp0 = kilp;
		  }
	  if (!korjaus_fl && (getEmitJarr(bdg2kno(em.badge), &em.osuus) != -1 ||
			(((kilpparam.kaksibadge != 2 && tark != L'-' && tark != L'B') || (kilpparam.kaksibadge == 2 && kilp.pv[k_pv].asakot[em.osuus] >= L'A')) &&
			(eilavoin || (tark != L'E' && tark != L'V'))))) {
		 kysytty = 1;
		 if (luesuoraan) {
			wch = L'T';
			}
		 else {
			   naytatulos(3);
			if (getEmitJarr(bdg2kno(em.badge), &em.osuus) != -1)
				swprintf(msg, L"Kortti %ld luettu jo kilpailijalle %d. ", em.badge, bdg2kno(em.badge));
			else
				swprintf(msg, L"Kortin %ld kilpailijalla %d jo merkintä %c. ", em.badge, bdg2kno(em.badge), tark);
			wch = L' ';
			erbeep();
			erbeep();
			erbeep();
			if (msgtxt)
				wcscpy(msgtxt, msg);
			vidspwmsg(0,0,0,0,msg);
			luetoiselle = 1;
			}
		 }
	  }

//   if ((kuntosuunnmoodi >= 1 && kuntosuunnmoodi <= 4 || kuntosuunnmoodi == 99)
//		&& em.badge && !bdg2kno(em.badge)) {
	if (em.badge && (!bdg2kno(em.badge) || luetoiselle)) {
		if (luetoiselle)
			vaihda_badge(em.badge);
		kno = haeSeurVakantti(0);
		if (kno <= 0) {
			wcscat(msgtxt, L"Kilpailija lisätty (ei vapaata vakanttipaikkaa) - käsittele");
			kno = haeseurvapaa(seurvapaa);
			kilp.nollaa();
			kilp.setId(kno);
			kilp.setSarjaAll(kilpparam.vakanttisarja);
			kilp.pv[k_pv].badge[0] = em.badge;
			for (int i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
				kilp.set_tark(L'V', i_pv);
				}
			kilp.set_tark(L'-', k_pv);
			kilp.pv[k_pv].uusi = 'U';
			kilp.pv[k_pv].vac = 'V';
			kilp.addtall(&d, 0);
			}
		else {
			if (kuntosuunnmoodi != 1 && kuntosuunnmoodi != 2 && kuntosuunnmoodi != 99)
				wcscat(msgtxt, L"Kortti luettu vakanttipaikalle - käsittele");
			d = getpos(kno);
			kilp.GETREC(d);
			if (kilp0)
				*kilp0 = kilp;
//			VakanttiSarja = kilp.Sarja();
			kilp.pv[k_pv].badge[0] = em.badge;
			kilp.set_tark(L'-', k_pv);
			kilp.pv[k_pv].uusi = 'U';
			kilp.pv[k_pv].vac = 'V';
			kilp.tallenna(d, 0, 0, 0, NULL);
			}
		if (!kno) {
			putem(&em, emithead2, 0);
			emithead2++;
			in_pros_emit = 0;
			return(em.badge);
			}
		entkilp = kilp;
		for (kbg = k_pv; kbg < kilpparam.n_pv_akt; kbg++)
			kilp.pv[k_pv].badge[0] = em.badge;
		EnterCriticalSection(&tall_CriticalSection);
		kilp.tallenna(d, 0, 0, 0, &entkilp);
		LeaveCriticalSection(&tall_CriticalSection);
		}
#endif  // !_CONSOLE
   if (em.package >= 20000000L && !em.badge) {
	  emithead2++;
	  putem(&em, emithead2, 0);
	  in_pros_emit = 0;
	  if (svscr)
			restorescr(0, 0, ySize-1, 79, svscr);
	  return(em.badge);
	  }
   if (em.badge && !esiluenta) {
	  if ((aika_err = tark_badge(&em)) != 0 && kno1 > 0) {
		   naytatulos(3);
		 if (aika_err > 0) {
			if (kuntosuunnmoodi != 1 && kuntosuunnmoodi != 3  && !autosarja) {
#ifdef CONSOLE
				swprintf(msg, L"Kortin %d kilpailijalla %d jo tark-merkintä tai rataa ei löytynyt.",
					em.badge, kno1);
#else
				swprintf(msg, L"Kortin %d kilpailijalla %d jo tarkastusmerkintä tai vastaavaa rataa ei löytynyt."
				L" Syötä muutoskaavakkeella oikea kilpailijanumero (aiempi tai uusi, jos toinen käyttäjä).",
					em.badge, kno1);
#endif
				 writeerror_w(msg, 0, true);
				}
			}
		 else {
			swprintf(msg, L"Ajan oikea laskenta mahdotonta kortin kellon nollaantumisen takia");
			writeerror_w(msg, 0, true);
			}
		 }
	  }
   else if (em.badge) {
	   em.kilpno = bdg2kno(em.badge);
	   }
   kno = em.badge ? em.kilpno : (INT) rbadge;
   if ((d = getpos(kno)) > 0) {
		kilp.GETREC(d);
		if (kilp0 && !kilp0->kilpno)
			*kilp0 = kilp;
		entkilp = kilp;
#if defined(_BORLAND_)
		 if ((kuntosuunnmoodi == 1 || kuntosuunnmoodi == 2) && kilp.pv[k_pv].uusi != 'U') {
//			for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
			kilp.pv[k_pv].badge[1] = em.badge;
			em.badge = haeSeurVapaaBadge(em.badge);
			kilp.pv[k_pv].badge[0] = em.badge;
			kilp.pv[k_pv].laina[1] = kilp.pv[k_pv].laina[0];
			tallkilp = 1;
			em.kilpno = kno;
			}
#endif
	  if (!esiluenta && !esitark) {
		  if ((autosarja || autorata) && !esitark && !esiluenta &&
			(em.kilpno > 0 || (em.kilpno = bdg2kno(em.badge)) > 0)) {
			 i = etsierata(&em);
			 if (i >= 0) {
				 int srj;
				 srj = haeradansarja(rata[i].tunnus, NULL);
				 rt = NULL;
				 if (!Sarjat[kilp.Sarja()].vakanttisarja)
					 rt = haerata(&kilp);
				 if (!rt || (rt != rata+i && (rata[i].rastiluku > rt->rastiluku || tarkista(&em, NULL, 0, &kilp) != 0))) {
					if (autosarja && srj >= 0) {
						kilp.setSarja(srj);
						kilp.set_tark(L'T', k_pv);
						}
					else if (autorata) {
						wcsncpy(kilp.pv[k_pv].rata, rata[i].tunnus, sizeof(kilp.pv[k_pv].rata)/2-1);
						kilp.set_tark(L'T', k_pv);
						}
					}
				 }
			 else if ((kuntosuunnmoodi == 1 || kuntosuunnmoodi == 3) && kilp.pv[k_pv].uusi != 'U') {
				kilp.set_tark(L'I', k_pv);
				}
			 }
		  rt = haerata(&kilp);
		  if (rt && rt->maalilaji == 4 && em.maali == TMAALI0/KSEK) {
#ifdef EBA
			 eba_ajat(&kilp, &em, 0);
			  EnterCriticalSection(&tall_CriticalSection);
				kilp.tallenna(d,TRUE, 0, 0, &entkilp);
			  LeaveCriticalSection(&tall_CriticalSection);
#else
			 em.maali = e_maaliaika(&em, &kilp, &tlahto)/KSEK;
			 if (em.maali != TMAALI0/KSEK) {
				tallkilp = 1;
				}
			 else if (kuntosuunnmoodi != 1 && kuntosuunnmoodi != 3) {
				   naytatulos(3);
				swprintf(msg, L"Kortin %ld maaliaikaa ei pystytty määräämään", em.badge);
				writeerror_w(msg, luesuoraan ? 2000 : 0, true);
				}
			 if (rt->ennakko >= 0 && tlahto != TMAALI0) {
				 if (lahtopoikk && kilp.pv[k_pv].tlahto != TMAALI0 && (kilp.pv[k_pv].tlahto - tlahto > SEK*lahtopoikk ||
					kilp.pv[k_pv].tlahto - tlahto < -SEK*lahtopoikk)) {
					swprintf(msg, L"Lasketun lähtöajan poikkeama %d sek",  (tlahto - kilp.pv[k_pv].tlahto)/SEK);
					writeerror_w(msg, luesuoraan ? 2000 : 0, true);
					}
				tlahto = ((tlahto + 24*TUNTI)/SEK)*SEK - 24*TUNTI;
				kilp.pv[k_pv].tlahto = tlahto;
				kilp.tall_tulos(-1, tlahto);
				if (em.maali != TMAALI0/KSEK)
					kilp.set_tulos(0, pyoristatls(KSEK*em.maali, 1));
				tallkilp = 1;
				}
			 else {
				if (kilp.pv[k_pv].tlahto == TMAALI0) {
				   kilp.pv[k_pv].tlahto = 0;
				   kilp.tall_tulos(-1, 0);
				   if (rt->ennakko < 0) {
						naytatulos(3);
						swprintf(msg, L"Kortin %ld lähtöaika puuttui - annettu 12.00.00", em.badge);
						writeerror_w(msg, luesuoraan ? 2000 : 0, true);
						}
				   tallkilp = 1;
				   }
				}
#if !defined(EBA)
//               if (rt->ennakko >= 0 && em.ctrltime[1] == 0)
//                  writeerror_w(L"Kortti käynnistynyt ykkösrastilla", 0);
#endif
#endif
			 }
		  itm.t = AIKAJAK*TMAALI0;
		  if (em.maali != TMAALI0/KSEK) {
			 if (emitajat && aikajono[emitajat-1])
				em.maali += aikajono[emitajat-1]->aikakorjaus / KSEK;
			 else
				em.maali += aikakorjaus[0] / KSEK;
			 itm.t = KSEK*em.maali*AIKAJAK;
			 itm.date = tm_date(itm.t);
			 if (!esitark && !esiluenta
				 &&
				 (
					(
					   em.package > 10000000L && em.package < 11000000L &&
					   (!rt || rt->maalilaji != 4)
					)
				 ||
					emitajat
				 )
				) {
				itm.kno = kno;
				if (piste < 0)
				   piste = 0;
				itm.piste = piste;
				itm.kanava = 0;
				j = emitajat ? emitajat-1 : aika_jono[0][0];
				itm.jono = j;
				tall_rivi(j, &itm, NULL, NULL, 0, 0, 0, false);
				tallkilp = 1;
				}
			 else
				piste = 0;
			 }
		  else if (autokilp && aikajono[0] && seurkrivi) {
			 for (ptm = 0; ptm < aikajono[0]->aktrows; ptm++)
				if (em.kilpno == aikajono[0]->getAktTime(0, ptm)->kno)
					break;
			if (em.kilpno == aikajono[0]->getAktTime(0, ptm)->kno) {
				if (ptm - seurkrivi > -10) seurkrivi = ptm;
				}
			 else {
				while (seurkrivi < aikajono[0]->aktrows &&
					aikajono[0]->getAktTime(0, seurkrivi)->kno) seurkrivi++;
				if (seurkrivi < aikajono[0]->aktrows) {
				   aikajono[0]->getAktTime(&etm, seurkrivi);
				   itm.t = etm.t;
				   itm.date = etm.t;
				   itm.kno = em.kilpno;
				   itm.piste = 0;
				   itm.kanava = 0;
				   tall_rivi(aika_jono[0][0], &itm, &etm, NULL, 0, 0, 0, false);
				   seurkrivi++;
				   tallkilp = 1;
				   }
				}
			 aikaruutu();
			 }
		  if (!esitark && !esiluenta && !korjaus_fl && tallkilp) {
			 if (itm.t != TMAALI0*AIKAJAK && !kilp.p_aika(piste)) {
				 int t2 = TMAALI0;
				 if (kilpparam.kaksibadge == 2 && kilp.pv[k_pv].badge[1-em.osuus] != 0) {
					 int ip2;
					 emittp em2;
					 if ((ip2 = em2.getbykno(kilp.id(), 1-em.osuus)) >= 0) {
						 t2 = KSEK*em2.maali;
						 if (NORMKELLO(purajak(itm.t) - t2) > 0)
							 t2 = purajak(itm.t);
						 }
					 }
				else
					t2 = purajak(itm.t);
				if (t2 != TMAALI0)
					kilp.set_tulos(piste, pyoristatls(t2, 1));
				 }
			  EnterCriticalSection(&tall_CriticalSection);
			  kilp.tallenna(d,TRUE, 0, 0, &entkilp);
			  LeaveCriticalSection(&tall_CriticalSection);
			 }
		  }
	   else {
	   // esitark tai esiluenta
		   em.kilpno = kilp.id();
		   kilp.set_tark(L'-', k_pv);
		   EnterCriticalSection(&tall_CriticalSection);

		   kilp.tallenna(d,TRUE, 0, 0, &entkilp);
		   LeaveCriticalSection(&tall_CriticalSection);
		   }
	   if (kilpparam.kaksibadge == 2 && em.badge == kilp.pv[k_pv].badge[1]) {
			em.osuus = 1;
			}
	  }
   else {
		if (esiluenta && kilp.pv[k_pv].badge[0] == em.badge)
			em.kilpno = kilp.id();
		else
			em.kilpno = 0;
		}
//   em.kirjaus = t_time_l(biostime(0,0), t0);
   j = putem(&em, emithead2, 0);
   if (em.badge && em.kilpno && !j) {
	  if (em.kilpno && em.badge)
		  setEmitJarr(emithead2, em.kilpno, em.osuus);
	  laheta_em(&em, 1, 0);
	  if (emitanal_fl && kilp.pv[k_pv].vac != 'V')
		  addemitanal(&em, 1);
	  }
   naytatulos(kilp.tark(k_pv) == L'T');
   if (em.kilpno && kilp.id() == em.kilpno && Sarjat[kilp.Sarja()].laskemtb[k_pv])
	   laskeEm_VA(d, false);
   if (d >= 0 && ilmlainat && lainakortti(&kilp)) {
	  writeerror_w(L"  LAINAKORTTI  ", 0, true);
	  }
   if (emittulparam && autorap && em.kilpno &&
	  (autorap > 1 || kilp.tark(k_pv) == L'I' || autorapsrj[kilp.Sarja()]))
	  emit_rap(emittulparam, em.kilpno);
   if (vaadiaika && em.kilpno && kilp.maali(0) == TMAALI0)
	  writeerror_w(L"Kilpailijalla ei aikaa", 0, true);
   emithead2++;
   in_pros_emit = 0;

   return(em.badge);
   }
#endif

static INT hae_aika(INT32 t, int kanava, INT *wt, INT jono, INT *ino)
   {
   aikatp pt;
   INT tm;

   for (*ino = 0; *ino <= maxjono || (lajatfl && *ino < 2); ++*ino) {
	  if (!aikajono[*ino] || aikajono[*ino]->aktrows == 0)
		 continue;
	  for (tm = aikajono[*ino]->aktrows-1; tm >= 0 &&
		  NORMTULOS_A(aikajono[*ino]->getAktTime(&pt, tm)->t - t) < 17L * TUNTI*AIKAJAK; tm--) {
		 if (abs(NORMTULOS_A(pt.t - t)) < 10 && pt.jono == jono && pt.kanava == kanava) {
			*wt = tm;
			return(1);
			}
		 }
	  }
   *wt = 0;
   return(0);
   }

#if defined(UINTIxx)
static INT haeratakilp(INT era, INT rata)
   {
   eratp *pk;

   if (!erataul)
	  return(0);
   for (pk = erataul; pk->kilpno > 0; pk++) {
      if (pk->era == era && pk->rata == rata)
         return(pk->kilpno);
      }
   return(0);
   }
#endif

static INT haeratakilp(INT era, INT rata)
   {
   kilptietue kilp;
	INT kno = 0, d;

	if (!rataTunnistus)
		return(0);
	EnterCriticalSection(&idindex_CriticalSection);
	d = getpos(minkilpno, true);
	while (d > 0) {
		kilp.GETREC(d);
		if (kilp.pv[k_pv].era == era && _wtoi(kilp.pv[k_pv].rata) == rata) {
			kno = kilp.id();
			break;
			}
		d = nextpos();
		}
	LeaveCriticalSection(&idindex_CriticalSection);
	return(kno);
	}

#ifdef SIIMPORT
typedef struct {
	int gate;
	int seq;
	} gatetp;

gatetp gates[10];

void addgatepiste(int gate, int seq)
{
	int k;

	for (k = 0; k < 10; k++) {
		if (gates[k].gate == 0) {
			gates[k].gate = gate;
			gates[k].seq = seq;
			break;
			}
		}
	}

int gatepiste(wchar_t *str)
{
	int k, no;

	no = _wtoi(str);
	for (k = 0; k < 10; k++)
		if (gates[k].gate == no)
			return(gates[k].seq);
	return(0);
}

void importsifile(void)
{
	static wchar_t ifilename[60] = L"import.csv";
	wchar_t buf[100], ch;
	FILE *ifile;

	clrln(ySize-3);
	vidspwmsg(ySize-3,0,7,0,L"Tiedosto:");
	inputstr(&ifilename, 58, 10, ySize-3, L"\r\x1b", &ch, 0);
	if (ch == ESC)
		return;
	if ((ifile = _wfopen(ifilename, L"rt")) == NULL) {
		writeerror_w(L"Tiedostoa ei ole", 0);
		return;
	}
	while (!feof(ifile)) {
		if (fgetws(buf, sizeof(buf)-2, ifile) && wcslen(buf) > 10) {

			tall_regnly((san_type *) buf, -1);
		}
	}
	fclose(ifile);
}
#endif

INT tall_regnly(san_type *vastaus, INT r_no)
   {
   INT i, k, ino = 0, kno = 0, mm = 0, sek = 0, osat = 0, d, tall_fl = 0, piste = -2, draja;
   INT jono = 0, rata = 0, olitulos = 0;
   INT32 t = -2000000000L,bt = 0, tls, tls0, tmli = TMAALI0;
   kilptietue kilp, entkilp;
   char *as, sum, *p;
   static INT yritys[NREGNLY];
//   static INT eosat;
   aikatp ut, vt;
   INT wt, rtm;
   static INT32 et[NREGNLY], edaika[NREGNLY][MAXJONO];
   static INT edtunn[NREGNLY];
   wchar_t msg[81], wst[40];
   bool inAjatCriticalSection = false;
#ifdef SWIM2000
	static char edrivit[8][20];
#endif
   INT32 tm[VALUKU+1];
   INT is, ls;
#ifdef TUTKA
#define N_ED  8
#else
#define N_ED  2
#endif
   static INT32 ed_bdg[NREGNLY][N_ED], ed_t[NREGNLY][N_ED];
#ifdef SIIMPORT
   int badger;
   char *p1;
#endif
   INT32 bdg = 0;
   aresmsg *aresvast;
#ifdef _CONSOLE
   draw_hline2(ySize - 4, 0, 60);
#endif
   aresvast = (aresmsg *)vastaus;
   memset(&ut, 0, sizeof(ut));
   memset(&vt, 0, sizeof(vt));
   vt.t = TMAALI0*AIKAJAK;
   regnlyhetki[r_no] = t_time_l(biostime(0,0), t0);
#ifdef SIIMPORT
   if (regnly[r_no] == LID_SIIMPORT || (siimport >= 0 && r_no < 0)) {
	  p = (char *) vastaus;
	  if (*p == '#')
		  return(0);
	  p = strtok(p, ";\t\r\n");
	  for (i = 0; i < 7; i++) {
		  switch (i) {
		  case 0:
			  break;
		  case 1:
			  ut.kanava = gatepiste(p);
			  piste = aika_tunnus[r_no][ut.kanava];
			  jono = aika_jono[r_no][ut.kanava];
			  break;
		  case 2:
			  badger = atoi(p);
			  break;
		  case 3:
			  kno = atol(p);
			  break;
		  case 4:
			  t = atol(p) + t0_regn[r_no];
			  t = NORMKELLO_A(10*t);
			  break;
		  case 5:
			  if (toupper(*p) == 'F')
				  return(0);
			  break;
		  case 6:
			  if (toupper(*p) == 'T')
				  kno = 0;
			  break;
		  }
		  if (i < 6)
			  p = strtok(NULL, ";\t\r\n");
	  }
	  bt = 0;
	  if (hae_aika(t, ut.kanava, &wt, jono, &ino))
		 return(0);
	  vastaus->r6.tunnus = 'A';
   }
   else
#endif
   if (regnly[r_no] == LID_RTR) {                                //  RTR-1

	  if (vastaus->r1.s1 != ' ' || vastaus->r1.s2 != ' ' ||
		 vastaus->r1.cr != 13)
		 return(1);
	  vastaus->r1.cr = '0';
	  vastaus->r1.loppu = 13;
	  vastaus->r1.loppu2 = 10;
	  vastaus->r1.loppu3 = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_RTR2) {                                //  RTR-2
	  if (loki)
		 kirjloki((char *) vastaus);
	  sum = 0;
	  as = &vastaus->r1.tunnus;
	  for (d = 2; d < r_msg_len[0]; d++) sum += *(as++);
	  sum &= '\x7f';
	  if (sum < 32) sum += 32;
	  if (sum != vastaus->r2.tark) return(1);
	  vastaus->r2.tark = 13;
	  vastaus->r2.cr = 10;
	  vastaus->r2.loppu = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_ALGE || regnly[r_no] == LID_ALGE4) {              //  S3, S4
	  vastaus->r3.cr2 = 13;
	  vastaus->r3.cr3 = 10;
	  vastaus->r3.cr4 = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_COMET) {                                //  Comet
	  vastaus->r4.cr = 13;
	  vastaus->r4.lf = 10;
	  vastaus->r4.z = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_TIMY || regnly[r_no] == LID_TDC) {             //  Timy / TDC8001
	  vastaus->r6.cr = 13;
	  vastaus->r6.lf = 10;
	  vastaus->r6.z = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_ASC) {                                //  ASC1
	  vastaus->r9.cr = 13;
	  vastaus->r9.lf = 10;
	  vastaus->r9.z = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
   if (regnly[r_no] == LID_ARES) {                                //  ARES
	  aresvast->eot2 = 0;
	  if (loki)
		 kirjloki((char *) vastaus);
	  }
#ifdef SWIM2000
	if (regnly[r_no] == LID_SWIM2000) {
		if (swimlaji == 0) {
			if (vastaus->r32.rata[0] >= '0' && vastaus->r32.rata[0] <= '9' &&
				vastaus->r32.rata[1] >= '0' && vastaus->r32.rata[1] <= '9') {
				vastaus->r32.cr = '\r';
				vastaus->r32.lf = '\n';
				vastaus->r32.z = 0;
				}
			else if (!memcmp(vastaus->r32.rata, "START", 5)) {
				vastaus->bytes[23] = '\r';
				vastaus->bytes[24] = '\n';
				vastaus->bytes[25] = 0;
				}
			}
		else {
			if (vastaus->bytes[19] == '\r') {
				vastaus->bytes[20] = '\n';
				vastaus->bytes[21] = 0;
				}
			if (vastaus->bytes[20] == '\r' && vastaus->bytes[19] >= '0'
				&& vastaus->bytes[17] == '.' && vastaus->bytes[14] == ':') {
				memset(vastaus->bytes+20, ' ', 3);
				vastaus->bytes[23] = '\r';
				vastaus->bytes[24] = '\n';
				vastaus->bytes[25] = 0;
				}
			else if (vastaus->bytes[23] == '\r') {
				vastaus->bytes[24] = '\n';
				vastaus->bytes[25] = 0;
				}
			else
				vastaus->bytes[0] = 0;
			}
		if (loki)
		   kirjloki((char *) vastaus);
		}
#endif
#ifdef TUTKA
   if (regnly[r_no] == LID_ALGETUTKA) {                                //  Tutka
	  vastaus->bytes[20] = 10;
	  vastaus->bytes[21] = 0;
	  if (loki)
		  kirjloki((char *) vastaus);
	  if (memcmp(vastaus->bytes, "!01/", 4) || vastaus->bytes[12] != '/')
		 return(1);
	  vastaus->bytes[12] = 0;
	  if ((bdg = _wtol(vastaus->bytes+4)) == 0)
		 return(1);
	  for (i = 0; i < N_ED; i++)
		 if (bdg == ed_bdg[r_no][i])
			break;
	  t = KSEK*t_time_l(biostime(0, 0), t0);
	  if (i < N_ED && bdg == ed_bdg[r_no][i] && t < ed_t[r_no][i] + 10 * SEK && t > ed_t[r_no][i])
		 return(0);
	  memcpy(ed_bdg[r_no]+1, ed_bdg[r_no], (N_ED-1)*sizeof(INT32));
	  memcpy(ed_t[r_no]+1, ed_t[r_no], (N_ED-1)*sizeof(INT32));
	  ed_bdg[r_no][0] = bdg;
	  ed_t[r_no][0] = t;
	  vastaus->r1.tunnus = 'A';
      ut.badge = bdg;
      t = t*AIKAJAK;
      }
#endif
   if (regnly[r_no] == LID_RADIOEMIT) {                                //  Radioemit
	  if (memcmp(vastaus->bytes, "\x02N", 2))
         return(1);
      vastaus->bytes[15] = 0;
		p = strtok(vastaus->bytes+2, "\x09");
		if (p)
			bdg = atol(p);
		if (!p || !bdg)
			return(1);
	  for (i = 0; i < N_ED; i++)
         if (bdg == ed_bdg[r_no][i])
            break;
	  t = KSEK*t_time_l(biostime(0, 0), t0);
      if (i < N_ED && bdg == ed_bdg[r_no][i] && t < ed_t[r_no][i] + 10 * SEK && t > ed_t[r_no][i])
		 return(0);
	  memcpy(ed_bdg[r_no]+1, ed_bdg[r_no], (N_ED-1)*sizeof(INT32));
	  memcpy(ed_t[r_no]+1, ed_t[r_no], (N_ED-1)*sizeof(INT32));
      ed_bdg[r_no][0] = bdg;
      ed_t[r_no][0] = t;
      vastaus->r1.tunnus = 'A';
      ut.badge = bdg;
      t = t*AIKAJAK;
		ut.t = t;
		ut.date = tm_date(ut.t);
		p = strtok(NULL, "\x09");
		if (p && *p == 'C')
			ut.kanava = atoi(p+1);
		else
			ut.kanava = 0;
	  }
   if (regnly[r_no] == LID_ALGE || regnly[r_no] == LID_ALGE4) {

	  if (regnly[r_no] == LID_ALGE4 && vastaus->r3.tunnus == ' ' &&
		 vastaus->r3.cr1 == ' ') return(0);
	  if (vastaus->r3.kp1 != ':' || vastaus->r3.kp2 != ':' ||
		 vastaus->r3.p != '.')
		 return(1);
	  vastaus->r3.tunnus = 'F';
	  vastaus->r3.kp1 = 0;
	  vastaus->r3.kp2 = 0;
	  vastaus->r3.p = 0;
	  vastaus->r3.cr2 = 0;
	  draja = (regnly[r_no] == LID_ALGE) ? 3 : 10;
	  for (d = 0; d < draja; d++) {
		 if (vastaus->r3.z1 == alge_tunn[d][0]
			&& vastaus->r3.z2 == alge_tunn[d][1]
			&& (regnly[r_no] == LID_ALGE || (vastaus->r3.z3 == alge_tunn[d][2]))) {
			jono = aika_jono[r_no][d+3];
			piste = aika_tunnus[r_no][d+3];
			ut.kanava = d+3;
			break;
			}
		 }
	  if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && aika_tunnus[r_no][5] < 98) {
		  rata = 1;
		  for (int dd = d - 1; dd >= 0 && aika_tunnus[r_no][dd + 3] == aika_tunnus[r_no][d + 3]; dd--)
			  rata++;
		  ut.tapahtuma = rata;
	  }
#if defined(UINTI__X)
	  if (regnly[r_no] == LID_ALGE4 && piste == -2 && vastaus->r3.z1 == 'K') {
         piste = vastaus->r3.z3 - '0';
         }
#endif
	  t = atol(vastaus->r3.tunti);
	  mm = atoi(vastaus->r3.min);
	  sek = atoi(vastaus->r3.sek);
	  osat = atoi(vastaus->r3.osat);
	  t = SEK/100*osat + SEK*sek + MINUUTTI*mm + TUNTI*t + t0_regn[r_no];
	  t = NORMKELLO_A((10000/SEK)*t);
	  if (t == et[r_no] && edtunn[r_no] == d)

		  return(0);
//      eosat = osat;
	  edtunn[r_no] = d;
      et[r_no] = t;
	  bt = 0;
      }
   if (regnly[r_no] == LID_COMET) {												// Comet

      if (vastaus->r4.kp1 != ':' || vastaus->r4.kp2 != ':' ||
		 vastaus->r4.p != '.')
         return(1);
      memmove(vastaus->r4.kno, &vastaus->r4.tunnus, 4);
	  vastaus->r4.tunnus = regnly_no[r_no] ? 'A' : 'F';
      vastaus->r4.kp1 = 0;
	  vastaus->r4.kp2 = 0;
	  vastaus->r4.p = 0;
//      vastaus->r4.tuh = 0;
      vastaus->r4.cr = 0;
      if ((d = stschind(vastaus->r4.piste, (char *) "2418")) > -1) {
		 jono = aika_jono[r_no][d+3];
		 piste = aika_tunnus[r_no][d+3];
         ut.kanava = d+3;
         }
	  t = atol(vastaus->r4.tunti);
	  mm = atoi(vastaus->r4.min);
	  sek = atoi(vastaus->r4.sek);
	  osat = atoi(vastaus->r4.osat);
	  t = osat + 1000L*sek + 60000L*mm + 3600000L*t + t0_regn[r_no];
      t = NORMKELLO_A(10*t);
//      if (t == et[r_no] && edtunn[r_no] == d)
//         return(0);
//      if (t == et[r_no] && osat == eosat) return(0);
//      eosat = osat;
//      edtunn[r_no] = d;
//      et[r_no] = t;
      bt = 0;
      }
   if (regnly[r_no] == LID_TIMY || regnly[r_no] == LID_TDC) {					// Timy, TDC8001

      if (vastaus->r6.p != '.' ||
			 (vastaus->r6.cc != 'C' && vastaus->r6.cc != 'F')
			)
		 return(1);
	  vastaus->r6.bl1 = 0;
	  vastaus->r6.kp1 = 0;
	  vastaus->r6.kp2 = 0;
	  vastaus->r6.p = 0;
	  vastaus->r6.cr = 0;
//	  vastaus->r6.tuh[1] = 0;
		if (regnly[r_no] == LID_TDC && vastaus->r6.cc == 'F')
			piste = 0;
	  else if ((d = vastaus->r6.piste - '0') >= 0 && d <= 8) {
		 piste = aika_tunnus[r_no][d+3];
		 if (maxjono == 0 && lajatfl && piste == -1)
			jono = 1;
		 else
			jono = aika_jono[r_no][d + 3];
		 ut.kanava = d + 3;
		 }
	  if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && aika_tunnus[r_no][5] < 98) {
		 rata = 1;
		 for (int dd = d-1; dd >= 0 && aika_tunnus[r_no][dd+3] == aika_tunnus[r_no][d+3]; dd--)
			rata++;
		 ut.tapahtuma = rata;
		 }
	  t = atol(vastaus->r6.tunti);
	  mm = atoi(vastaus->r6.min);
	  sek = atoi(vastaus->r6.sek);
	  for (d = 0; d < 4; d++)
		 if (vastaus->r6.osat[d] < '0')
			vastaus->r6.osat[d] = '0';
	  vastaus->r6.osat[3] = '0';
	  osat = atoi(vastaus->r6.osat);
	  t = osat + 10000L*sek + 600000L*mm + 36000000L*t + 10*t0_regn[r_no];
	  t = NORMKELLO_A(t);
//	  t = NORMKELLO_A(10*t);
//      if (t == et[r_no])
//         return(0);
//      if (t == et[r_no] && osat == eosat && d == edtunn[r_no]) return(0);
//      eosat = osat;
//      edtunn[r_no] = d;
//      et[r_no] = t;
	  bt = 0;
	  switch (vastaus->r6.tunnus) {
		 case 'c':
			 if (hae_aika(t, ut.kanava, &wt, jono, &ino) && aikajono[ino] && !aikajono[ino]->getAktTime(&vt, wt)->kno)
			   tall_rivi(ino, 0, &vt, 0, 0, 0, 0, false);
			return(0);
		 case 'h':
		 case 'H':
		 case 'k':
		 case 'K':
		 case 'e':
		 case 'E':
			 if (regnly_no[r_no] && (d = getpos(atoi(vastaus->r6.kno))) > 0) {
				EnterCriticalSection(&tall_CriticalSection);
				kilp.GETREC(d);
				kilp.set_tark(toupper(vastaus->r6.tunnus));
				kilp.tallenna(d,TRUE, 0, 0, &entkilp);
				LeaveCriticalSection(&tall_CriticalSection);
				}
			return(0);
		 default:
			 if (regnly_no[r_no]) {
				 if (hae_aika(t, ut.kanava, &wt, jono, &ino) && aikajono[ino] &&
					aikajono[ino]->getAktTime(&vt, wt)->kno == atoi(vastaus->r6.kno))
					return(0);
				}
			else if (hae_aika(t, ut.kanava, &wt, jono, &ino))
			   return(0);
			break;
		 }
	  vastaus->r6.tunnus = regnly_no[r_no] ? 'A' : 'F';
	  }
   if (regnly[r_no] == LID_ARES) {					// ARES
      if (aresvast->stx3 != 2 || stschind(aresvast->piste, "SIA") < 0 || stschind(aresvast->sanlaji, "234") < 0 ||
		  stschind(aresvast->aikalaji, "CI") < 0)
		 return(1);
	  aresvast->vali1 = 0;
	  aresvast->vali2 = 0;
	  aresvast->vali3 = 0;
	  aresvast->var1[0] = 0;
	  switch (aresvast->piste) {
		  case 'S':
			  piste = -1;
			  break;
		  case 'A':
			  piste = 0;
			  break;
		  case 'I':
			  piste = atoi(aresvast->kierr);
			  break;
		  }
	  rata = aresvast->rata-'0';
	  t = atoi(aresvast->tt);
	  mm = atoi(aresvast->mm);
	  sek = atoi(aresvast->ss);
	  for (d = 0; d < 3; d++)
		 if (aresvast->oo[d] < '0' || aresvast->oo[d] > '9')
			aresvast->oo[d] = '0';
	  osat = atoi(aresvast->oo);
	  t = osat + 1000L*sek + 60000L*mm + 3600000L*t + t0_regn[r_no];
	  t = NORMKELLO_A(10*t);
	  if (piste >= 0 && eralahto[era_akt] != AIKAJAK*TMAALI0 && NORMTULOS(t/10 - t0_regn[r_no]) < 2*TUNTI)
		  t = NORMKELLO_A(eralahto[era_akt] + t - 10 * t0_regn[r_no]);
	  if (era_akt > 0 && piste == -1 && eralahto[era_akt] == 10*TMAALI0) {
		  eralahto[era_akt] = t;
		  lahetaeralahto(era_akt, 0);
		  }
//      if (t == et[r_no])
//         return(0);
//      if (t == et[r_no] && osat == eosat && d == edtunn[r_no]) return(0);
//      eosat = osat;
//      edtunn[r_no] = d;
//      et[r_no] = t;
	  bt = 0;
	  switch (aresvast->aikalaji) {
		 case 'C':
			 if (hae_aika(t, ut.kanava, &wt, jono, &ino) && aikajono[ino] && !aikajono[ino]->getAktTime(&vt, wt)->kno)
               tall_rivi(ino, 0, &vt, 0, 0, 0, 0, false);
            return(0);
		 default:
            if (hae_aika(t, ut.kanava, &wt, jono, &ino))
			   return(0);
            break;
		 }
      vastaus->r6.tunnus = 'F';
	  }
#ifdef TUTKA
   if (regnly[r_no] == LID_ALGETUTKA) {                                //  Tutka
      bt = 0;
      }
#endif
	if (regnly[r_no] == LID_ASC) {
      if (vastaus->r9.tunnus != 'C' || vastaus->r9.bl1 != ' ' || vastaus->r9.bl2 != ' ')
		 return(1);
	  jono = aika_jono[r_no][vastaus->r9.piste-'0'+2];
	  piste = aika_tunnus[r_no][vastaus->r9.piste-'0'+2];
      ut.kanava = vastaus->r9.piste-'0'+2;
		vastaus->r9.bl2 = 0;
		osat = atol(vastaus->r9.osat);
		t = 36000L*(vastaus->r9.tunti[0]-'0')+3600L*(vastaus->r9.tunti[1]-'0')
			+600*(vastaus->r9.min[0]-'0')+60*(vastaus->r9.min[1]-'0')
			+10*(vastaus->r9.sek[0]-'0')+(vastaus->r9.sek[1]-'0');
		t = 1000*t + osat + t0_regn[r_no];
	  t = NORMKELLO_A(10*t);
		vastaus->r1.tunnus = 'A';
		bt = 0;
		}
#ifdef LAJUNEN
	if (regnly[r_no] == LID_SIRIT) {           // SIRIT
		if (loki && !siritloki)
			kirjloki((char *) vastaus);
		if (siritaika(&t, vastaus, &ut, &jono, r_no))
			return(1);
		if (ut.kanava == 0)
			ut.kanava = r_no+1;
		if (t == 20L*TMAALI0)
			return(1);
		bt = 0;
		bdg = ut.badge;
		vastaus->r1.tunnus = 'A';
		naytatulos(ut.kanava == 10);
		}
	else if (regnly[r_no] == LID_IMPINJ) {           // IMPINJ
		__int64 t64, pv64;

		while (vastaus->bytes[0] && (vastaus->bytes[0] < '0' || vastaus->bytes[0] > '9'))
			memmove(vastaus->bytes+1, vastaus->bytes, strlen(vastaus->bytes));
		if (loki)
			kirjloki((char *) vastaus);
		if (strlen(vastaus->bytes) < 30)
			return(1);
		p = strtok(vastaus->bytes, ",\r\n");
		if (!p)
			return(1);
		ut.kanava = atoi(p);
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
		ut.t = AIKAJAK*t;
		ut.date = tm_date(ut.t);
		bt = 0;
		bdg = ut.badge;
		vastaus->r1.tunnus = 'A';
		}
	else if (regnly[r_no] == LID_FEIG) {			  // FEIG
		if (256*vastaus->r31.len[0]+vastaus->r31.len[1] != 29)
			return(1);
		t = 3600000L * vastaus->r31.tm[0] + 60000L * vastaus->r31.tm[1]
			+ 256 * vastaus->r31.tm[2] + vastaus->r31.tm[3] + t0_regn[r_no];
		t = NORMKELLO_A(10*t);
		bdg = 4096*vastaus->r31.idd[7] + 16 * vastaus->r31.idd[8] + vastaus->r31.idd[9]/16;
		ut.badge = bdg;
		ut.kanava = 10*vastaus->r31.antenna + vastaus->r31.idd[6]/16;
		jono = aika_jono[r_no][0];
		bt = 0;
		vastaus->r1.tunnus = 'A';
		}
#endif
#ifdef SWIM2000
	if (regnly[r_no] == LID_SW2000) {
		if (swimlaji == 0) {
			if (!memcmp(vastaus->r32.rata, "START", 5)) {
				if (vastaus->r32.colon1 != ':' || vastaus->r32.colon != ':' || vastaus->r32.point != '.')
					return(1);
				vastaus->r32.colon1 = 0;
				vastaus->r32.colon = 0;


				vastaus->r32.point = 0;
				vastaus->bytes[21] = 0;
				t = 3600000L*_wtol(vastaus->r32.h) + 60000L*_wtol(vastaus->r32.m)
					+ 1000L*_wtol(vastaus->r32.s) + 10*_wtol(vastaus->r32.osa)+ t0_regn[r_no];
				t = NORMKELLO_A(10*t);
				eralahto[era_akt] = t;
				lahetaeralahto(era_akt, 0);
				piste = -1;
				}
			else {
				if (vastaus->r32.valid != ' ' || vastaus->r32.colon != ':' ||
					vastaus->r32.point != '.' || vastaus->r32.source[0] != 'T')
					return(1);
				mm = vastaus->r32.m[1] - '0' +
					(vastaus->r32.m[0] <= '0' ? 0 : 10*(vastaus->r32.m[0] - '0'));
				sek = vastaus->r32.s[1] - '0' + 10*(vastaus->r32.s[0] - '0');
				osat = (vastaus->r32.osa[1] > '0') * (vastaus->r32.osa[1] - '0') + 10*(vastaus->r32.osa[0] - '0');
				t = 100*osat + 10000L*sek + 600000L*mm;
				if (eralahto[era_akt] != AIKAJAK*TMAALI0) {
					t += eralahto[era_akt];
					rata = vastaus->r32.rata[1] - '0';
					piste = -2;
					}
				else
					t += 10*t0_regn[r_no];
				t = NORMKELLO_A(t);
				ut.kanava = vastaus->r32.rata[1] - '0' +
					10 * (vastaus->r32.count[2] - '0') +
					(vastaus->r32.end == 't' ? 100 : 0);
				}
			vastaus->r1.tunnus = 'F';
			}
		else {
			if ((strlen(vastaus->bytes) < 25 && strcmp(vastaus->bytes+1, "   .        0:00.0\r\n"))
				|| vastaus->bytes[14] != ':')
				return(1);
			if (!strcmp(vastaus->bytes+1, "   .        0:00.0\r\n")) {
				t = KSEK*t_time_l(biostime(0,0), t0);
				t = AIKAJAK*NORMKELLO(t);
				eralahto[era_akt] = t;
				lahetaeralahto(era_akt, 0);

				vastaus->r1.tunnus = 'F';
				piste = -1;
				}
			else {
				for (d = 0; d < sizeof(edrivit)/sizeof(edrivit[0]); d++) {
					if (!memcmp(vastaus->bytes, edrivit[d], sizeof(edrivit[0])))
						return(1);
					memmove(edrivit[1], edrivit[0], sizeof(edrivit)-sizeof(edrivit[0]));
					memmove(edrivit[0], vastaus->bytes, sizeof(edrivit[0]));
					}
				vastaus->bytes[14] = 0;
				vastaus->bytes[17] = 0;
				vastaus->bytes[20] = 0;
				mm = _wtoi(vastaus->bytes+11);
				sek = _wtoi(vastaus->bytes+15);
				osat = 10*_wtoi(vastaus->bytes+18);
				t = osat + 1000L*sek + 60000L*mm;
				t = NORMKELLO_A(10*t+eralahto[era_akt]);
				rata = vastaus->bytes[10] - '0';
				piste = -2;
				vastaus->r1.tunnus = 'F';
				}
			}
		}
#endif
   if (regnly[r_no] == LID_RTR || regnly[r_no] == LID_RTR2) {
      if (vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'C' ||
         vastaus->r1.tunnus == 'E' || vastaus->r1.tunnus == 'F' ||
         vastaus->r1.tunnus == 'G') {
		 jono = aika_jono[r_no][4];
		 piste = aika_tunnus[r_no][4];
		 if (regnly[r_no] == LID_RTR2) {
            ino = (vastaus->r2.cr1 - '0' + 1) % 10 - 1;
			if (ino < 4) {
			   jono = aika_jono[r_no][ino+4];
			   ut.kanava = ino+4;
			   if (vastaus->r2.n1 > '1') {
				  jono = aika_jono[r_no][ino+8];
                  ut.kanava = ino+8;
				  }
			   piste = aika_tunnus[r_no][ino+4];
			   if (vastaus->r2.n1 > '1') piste = aika_tunnus[r_no][ino+8];
               }
			vastaus->r2.pakka = 0;
            }
		 if (regnly[r_no] == LID_RTR) {
			if (vastaus->r1.kp != ':' || vastaus->r1.p != '.') return(1);
            }
		 vastaus->r1.kp = 0;
		 vastaus->r1.p = 0;
		 mm = atoi(vastaus->r1.min);
		 sek = atoi(vastaus->r1.sek);
         osat = atoi(vastaus->r1.osat);
		 t = osat*SEK/1000 + SEK*sek + MINUUTTI*mm + t0_regn[r_no];
         t = NORMKELLO_A((10000/SEK)*t);
         bt = purajak(t)/KSEK - (INT32) T_TIME(biostime(0,0L));
         }
	  else if(regnly[r_no] == LID_RTR2 && vastaus->r2.tunnus == 'J') {
         vastaus->r2.min[0] = 0;
         kno = atoi(vastaus->r2.kno+1);
         if ((d = getpos(kno)) > 0) {
				kilp.GETREC(d);
				entkilp = kilp;
			if ((ls = Sarjat[kilp.Sarja()].lsak[k_pv]) > 0) {
               if (racpisteet)
                  is = vastaus->r2.rac-'1';
               else
				  for (is = 0; is < ls && kilp.pv[k_pv].asakot[is] != L'-'; is++) ;
			   if (is >= 0 && is < ls && vastaus->r2.tuh >= '0'
				  && vastaus->r2.tuh <= '9') {
						for (piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
							tm[piste] = kilp.maali(piste);
                  kilp.pv[k_pv].asakot[is] = vastaus->r2.tuh;
				  for (piste = 0; piste <= Sarjat[kilp.Sarja()].valuku[k_pv]; piste++)
                     if (tm[piste] != TMAALI0)
							kilp.set_tulos(piste, tm[piste]);
					  EnterCriticalSection(&tall_CriticalSection);
					  kilp.tallenna(d,TRUE, 0, 0, &entkilp);
					  LeaveCriticalSection(&tall_CriticalSection);
				  }
               else {
				  swprintf(msg, L"Kilpailijalle %d ylimääräinen ampumatieto %d sakkoa",
                     kno, (int) (vastaus->r2.tuh-'0'));
				  writeerror_w(msg, 0, true);
				  }
               }
            }
		 }
      }

//   else t = t0_regn[r_no];
//#ifdef MERI5
	if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && regnly[r_no] != LID_RESERV_1 && rata == 0) {
//		rata = 1;
		if (piste >= 98) {
			rata = (piste+2) / 100;
			}
		ut.tapahtuma = rata;
		}
//#endif
	piste = (piste+2) % 100 - 2;
   
   if (vainoletus && !(lajatfl && piste == -1))
      piste = ol_piste;
   if (jonopiste[jono] && wcswcind(jonopiste[jono], ch_piste) > -1)
	  piste = wcswcind(jonopiste[jono], ch_piste) - 2;

  if (tunnjono >= 0 && jono != tunnjono && piste == 0) {
	EnterCriticalSection(&ajat_CriticalSection);
	inAjatCriticalSection = true;
	kno = tunn_hae_kno(purajak(t));
	  }
  else {
	   if ((vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'B'
		  || vastaus->r1.tunnus == 'E') && regnly_no[r_no]) {
		  switch (regnly[r_no]) {
			 case LID_RTR:
				vastaus->r1.s2 = 0;
				kno = atoi(vastaus->r1.kno);
				break;
			 case LID_RTR2:
				vastaus->r2.min[0] = 0;
				kno = atoi(vastaus->r2.kno+1);
				break;
			 case LID_COMET:
				vastaus->r4.L_C[1] = 0;
			 case LID_TIMY:
			 case LID_TDC:
				kno = atoi(vastaus->r4.kno);
	#ifdef COMETNO_X
				kno = bdg2kno(kno);
	#endif
				break;
	#ifdef TUTKA
			 case LID_ALGETUTKA:
				kno = bdg2kno(bdg);
				break;
	#endif
	#ifdef SIIMPORT
			 case LID_SIIMPORT:
				 break;
	#endif
	#ifdef EMIT
			 case LID_RADIOEMIT:
				kno = bdg2kno(bdg);
				break;
	#endif
	#ifdef LAJUNEN
			 case LID_SIRIT:
			 case LID_FEIG:
			 case LID_IMPINJ:
				kno = bdg2kno(bdg);
				break;
	#endif
			 default:
				kno = 0;
				break;
			 }
		  }
	  }
	k = ut.kanava+NREGNLY;
#ifdef LAJUNEN
	if (siritreuna == 3 && ut.tapahtuma)
		k += 100;
#endif
	if (lahdepistehaku || lahdejonohaku) {
		if (kno && (d = getpos(kno)) > 0) {
			kilp.GETREC(d);
			if (lahdepistehaku)
				piste = haelahdepiste(&kilp, k-NREGNLY, purajak(t));
			if (lahdejonohaku)
				jono = haelahdejono(&kilp, k-NREGNLY);
			}
		else {
			if (lahdepistehaku)
				piste = haelahdepiste(NULL, k-NREGNLY, 0);
			if (lahdejonohaku)
				jono = haelahdejono(NULL, k-NREGNLY);
			}
		}
#if defined(UINTIxx)
   if (era_akt > 0) {
      if (piste > 0) {
		 kno = haeratakilp(era_akt, piste);
         piste = 0;
		 }
	  else if (piste == -1) {
		 eralahto[era_akt] = t;
		 lahetaeralahto(era_akt, 0);
		 }
	  }
#endif
   if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && era_akt > 0) {
	  if (piste != -1 && rata > 0) {
		 kno = haeratakilp(era_akt, rata);
		 }
	  }
   if (maxjono > 0)
      ino = jono;
   else if (lajatfl && piste == -1)
      ino = 1;
   else
      ino = 0;
	if (kello_esto && edaika[r_no][ino] && t > edaika[r_no][ino] && t < edaika[r_no][ino] + 100*kello_esto)
		return(0);
	edaika[r_no][ino] = t;
   if (vastaus->r1.tunnus >= 'A' && vastaus->r1.tunnus <= 'F') {
      if ((vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'F') &&
		 labs(bt) > maxero && yritys[r_no]++ == 0)
		 return(1);
	  if (!aikajono[ino] || (aikajono[ino]->rwtime - aikajono[ino]->mxtime) >= 0) {
		  if (!aikajono[ino] || !aikajono[ino]->mxtime)
				writeerror_w(L"AJANOTTOA EI KÄYNNISTETTY, MUTTA YRITETÄÄN KÄYTTÄÄ",0, true);
			else
				writeerror_w(L"AIKATAULUKKO TÄYSI",0);
         return(1);
		 }
      wt = aikajono[ino]->aktrows;
      if (vastaus->r1.tunnus == 'B' || vastaus->r1.tunnus == 'D') t = et[r_no];
	  ut.t = t;
	  ut.date = tm_date(ut.t);
	  ut.jono = ino;
      ut.kno = 0;
#ifdef SIIMPORT
		if (siimport >= 0)
			ut.kno = kno;
#endif
		ut.piste = piste;
		if (vastaus->r1.tunnus >= 'B' && vastaus->r1.tunnus <= 'D') {
			if (vastaus->r1.tunnus == 'B') 
				kno += 10000;
			else 
				kno = 19999 + vastaus->r1.tunnus - 'D';
			ut.kno = kno;
#ifdef _CONSOLE
			vidint(ySize - 1, 54, 4, aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows - 1)->kno);
#endif 	//  Vaatii uuden käsittelyn
			if (loki)
				wkirjloki(L"NOLLAUS");
			}
		else if((d = getpos(kno)) > 0) {
			ut.kno = kno;
			kilp.GETREC(d);
			entkilp = kilp;
			if (piste == -2) {
				piste = maaraa_piste(&kilp, purajak(t));
				ut.piste = piste;
				}
//#ifdef SIIMPORT
//         if (siimport < 0 && p_aika(&kilp, kierrosva[piste+2]) || !hyv(&kilp)) 
//         if (p_aika(&kilp, kierrosva[piste+2]) || !hyv(&kilp)) 
//#else
//         if (p_aika(&kilp, kierrosva[piste+2]) || !hyv(&kilp)) 
//#endif
			if (jono == tunnjono) {
				int t_on;
				if ((t_on = tunn_on_jo(purajak(t), kno)) != 0) {
					if (t_on > 0)
						goto loppu;
					if (kilp.p_aika(0))
						goto loppu;
					}
				if (estaneg && 
					((t_on = NORMTULOS(purajak(t) - kilp.pv[k_pv].tlahto)) > 20*TUNTI || 
					 t_on < estaneg))
					 goto loppu;
				tmli = tunn_hae_aika(purajak(t), kno);
				}
			else {
				if (!kilp.hyv() && piste != -1) {
					wcscpy (msg, L"JO MERKITTY ");
					switch (kilp.tark(k_pv)) {
						case 'E':
						case 'B':
							wcscat(msg, L"EI-LÄHTENEEKSI");
							break;
						case 'K':
						case 'O':
							wcscat(msg, L"KESKEYTTÄNEEKSI");
							break;
						case 'H':
							wcscat(msg, L"HYLÄTYKSI");
							break;
						}
					if (!kilp.p_aika(kierrosva[piste+2])) {
#ifdef _CONSOLE
						vidspwmsg(ySize-4,0,0,7,msg);
#else
						if (JoKirjattuKesto)
							writeerror_w(msg, JoKirjattuKesto, true);
#endif
						if (loki) {
							wkirjloki(msg);
							}
						}
					}
				if (estaneg && kierrosva[piste+2] >= 0 &&
					(NORMTULOS(purajak(ut.t) - kilp.pv[k_pv].tlahto) > 20*TUNTI ||
					NORMTULOS(purajak(ut.t) - kilp.pv[k_pv].tlahto) < estaneg)) {
					olitulos = 2;
					if (loki) {
						wkirjloki(L"NEGATIIVINEN TAI HAAMUAIKA");
						}
					}
				else while (kierrosva[piste+2] >= 0 && kilp.p_aika(kierrosva[piste+2]) != 0) {
					bool osuma = false;

					tls0 = kilp.maali(kierrosva[piste+2]);
					tls = purajak(t);

					// Etsitään aiempaa ajanottotietoa, joka koskee samaa kilpailijaa ja pistettä. 
					// Ellei löydy ja estaylim == 1 ja uusinaika == false, sallitaan aikarivin tallentaminen
					// mutta ei tuloksen tallentamista kilpailijalle

					if (estaylim) {
						int raja = std::min(4*TUNTI, (long)ylimraja);

						aikatp vertt;
						for (rtm = aikajono[ino]->aktrows - 1; rtm >= 0 &&
							NORMKELLO(tls - purajak(aikajono[ino]->getAktTime(&vertt, rtm)->t)) < raja; rtm--) {
							if (vertt.kno == kilp.id() && vertt.piste == piste) {
								osuma = true;
								break;
								}
							}
						if (!osuma && estaylim < 2 && !uusinaika) {
							olitulos = -1;
							break;             // poistuu tästä tarkastelusta
							}
						}

					// Jos osuma tuli ja käytössä on ESTÄYLIM, mutta ei UUSINAIKA asetetaan olitulos = 1 rajoittamaan 
					// sekä ajanottotiedon että tuloksen tallennusta

					if (!uusinaika && estaylim && osuma) {
						olitulos = 1;
						break;
						}

					// Kun estaylim == 0 tai uusinaika == true asetetaan olitulos = 1, jos uusi tulos on joko aiempaa 
					// pienempi tai suurempi enemmän kuin ylimraja
					// uusinaika siis voittaa, kun saatu aika on rajoitetusti myöhäisempi kuin aiemmin tallennettu

					if (tls - tls0 < 0 || tls - tls0 >= ylimraja) {

						olitulos = estaylim ? 1 : -1;
#ifdef _CONSOLE
						vidspwmsg(ySize-4,0,0,7,L"TULOS JO ANNETTU");
#else
						if (JoKirjattuKesto)
							writeerror_w(L"TULOS JO ANNETTU", JoKirjattuKesto, true);
#endif
						if (loki)
							wkirjloki(L"TULOS JO ANNETTU");
						break;
						}

					// Jos käytössä on UUSINAIKA ja ESTÄYLIM=xxK, joka pyytää korvaamaan aiemman uudella aikarajan ylimraja 
					// rajoissa haetaan aiempaa tunnistetta jonosta edeltävältä jaksolta ylimraja. 
					// Ellei osumaa ole muutetaan vertailutietueen vt aika, jolloin mikään vanha tieto ei poistu, vaan uusi tulee
					// eri riville

					if (estaylim == 2 && tls - tls0 > 0) {
						bool osuma = false;
						for (rtm = aikajono[ino]->aktrows-1; rtm >= 0 &&
							NORMTULOS(tls - purajak(aikajono[ino]->getAktTime(&vt, rtm)->t)) < ylimraja; rtm--) {
							if (vt.badge == ut.badge || (kilpparam.kaksibadge &&
								(vt.badge == kilp.pv[k_pv].badge[0] || vt.badge == kilp.pv[k_pv].badge[1]))) {
								osuma = true;
								break;
								}
							}
						if (!osuma)
							vt.t = TMAALI0*AIKAJAK;
						}
					break;
					}
				tmli = purajak(t);
				}
			if (olitulos == 0 || (uusinaika && olitulos == 1 && abs((long) NORMKELLO(tls - tls0)) < uusinaika)) {
				tall_fl = 1;
#if defined(UINTIxx)
				if (era_lahdot && eralahto[era_akt] != TMAALI0*AIKAJAK) {
					tls = purajak(eralahto[era_akt]);
					kilp.set_tulos(-1, tls, 1);
					tmli = purajak(t) - (purajak(eralahto[era_akt]) - tls);
					}
//				else
#endif
//					tls = purajak(t);
				if (tmli != TMAALI0)
					kilp.set_tulos(kierrosva[piste+2], tmli, 1);
				}
			}
		if (pvparam[k_pv].hiihtolahto && piste == -1) {
			if ((pakotalaika && (d = getpos(ut.kno)) > 0) ||
				 (!pakotalaika && (d = lahtija(purajak(ut.t))) != 0)) {
				kilp.GETREC(d);
				entkilp = kilp;
			ut.kno = kilp.id();
			kilp.tall_tulos(-1, pyoristatls(purajak(ut.t), 1));
			if (kilp.tark() == L'E' || kilp.tark() == L'B')
				kilp.set_tark(L'-');
			tall_fl = 1;
			}
		 else
			tall_fl = 0;
		 }
	  if (!pvparam[k_pv].hiihtolahto && piste == -1) {
			if ((d = getpos(ut.kno)) > 0) {
				kilp.GETREC(d);
				entkilp = kilp;
				ut.kno = kilp.id();
				if (kilp.tark() == L'E' || kilp.tark() == L'B') {
					kilp.set_tark(L'-');
					tall_fl = 1;
					}
				}
			}
	   if (ut.piste == -1 && kilpparam.lahtoryhma > 1) {
			if (eralahto[era_akt] == TMAALI0*AIKAJAK) {
			  eralahto[era_akt] = AIKAJAK*KSEK*t_time_l(biostime(0,0), t0);
			  lahetaeralahto(era_akt, 0);
			  }
		   for (i = 0; i < kilpparam.lahtoryhma; i++) {
			   ut.kno = 0;
//#ifdef MERI5
			if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0)
				ut.kno = haeratakilp(era_akt, i+1);
//#endif
	         tall_rivi(ino, &ut, NULL, NULL, 0, 0, 0, inAjatCriticalSection);
				if (ut.kno > 0 && (d = getpos(ut.kno)) > 0) {
					EnterCriticalSection(&tall_CriticalSection);
					kilp.GETREC(d);
					entkilp = kilp;
					kilp.tall_tulos(-1, pyoristatls(purajak(ut.t), 1));
					kilp.tallenna(d,TRUE, 0, 0, &entkilp);
					LeaveCriticalSection(&tall_CriticalSection);
					}
			   ut.t += 1;
			   ut.date = tm_date(ut.t);
		 	   }
		   }
		else {
			if ((!estaylim && !estaneg) || olitulos <= 0 || va_piste(ut.piste) < 0) {
				tall_rivi(ino, &ut, &vt, NULL, 0, 0, 0, inAjatCriticalSection);
				}
			else {
				tall_fl = 0;
				ut.status = (UINT8) olitulos;
				tall_rivi(ino, &ut, &vt, NULL, 0, 0, 0, inAjatCriticalSection);
/*
				if (loki) {
					swprintf(msg, L"%s %4d %d %d  Aika ohitettu",
						AIKATOWSTRS(wst, purajak(ut.t), t0), (int) ut.kno, (int) va_piste(ut.piste), (int) ut.kanava);
					wkirjloki(msg);
					}
*/
				}
			}
	  if (inAjatCriticalSection)
		  LeaveCriticalSection(&ajat_CriticalSection);
	  inAjatCriticalSection = false;
	  if (tall_fl && (jono != tunnjono || tmli != TMAALI0)) {
		  EnterCriticalSection(&tall_CriticalSection);
//		  kilp.tallenna(d,TRUE, 0, estaylim, &entkilp);

		  kilp.tallenna(d,TRUE, 0, 0, &entkilp);
		  LeaveCriticalSection(&tall_CriticalSection);
#ifdef _CONSOLE
		 vidint(ySize-1,54,4,kno);
#endif
		 }

	  if (ino == 0 && od_fl && kno < minkilpno && od_aika == TMAALI0*AIKAJAK &&
		  NORMKELLO_A(ut.t - aikajono[ino]->getAktTime(0, aikajono[ino]->aktrows - 1)->t) == 0) {
		 od_aika = ut.t;
		 odota = wt;
		 }

	  AIKATOWSTRS(wst, purajak(t),t0);

	  if ( (aikajono[akttm]->aktrows - htime[akttm]) > 0) 
		  htime[akttm] = aikajono[akttm]->aktrows;
#ifdef _CONSOLE
	  vidspwmsg(ySize-1,60,7,0,wst);
	  vidint(ySize-1,71,4,aikajono[akttm]->aktrows);
	  vidint(ySize-1,76,4,aikajono[akttm]->rtime+1);
#endif
	  }

   if (vastaus->r1.tunnus == 'G' && kerro_regnly) {
	  kerro_regnly = 0;
#ifdef _CONSOLE
	  clrln(ySize-1);
	  writeerrorOn = 50;
	  vidspwmsg(ySize-1,0,7,0,L"Maalikello on tietokoneen kelloa      s");
	  vidint(ySize-1,32,5,(int)(labs(bt) / 10));
	  wcscpy(wst, (bt > 0) ? L"edellä" : L"jäljessä");
	  vidspwmsg(ySize-1,40,7,0,wst);
#endif
	  }
   if (regnly[r_no] <= LID_RTR2 && (vastaus->r1.tunnus == 'A' || vastaus->r1.tunnus == 'C' ||
	   vastaus->r1.tunnus == 'E' || vastaus->r1.tunnus == 'F' ||
	   vastaus->r1.tunnus == 'G')) et[r_no] = t;
   yritys[r_no] = 0;
loppu:
   return(0);
   }

void seur_mtr(void)
   {
   char msg[8];
   INT nch;
#ifdef EMITFILE
   if (port_regnly[r_no_file] > 900) {
	  file2emit();
	  return;
	  }
#endif
   if (pkg_alku[lue_rno] <= pkg_loppu[lue_rno]) {
#ifdef DBGEMIT
	  writeerror_w(L"Pyyntö", 400);
#endif
	  memcpy(msg, "/GB", 3);
	  *(INT32 *)(msg+3) = pkg_alku[lue_rno];
	  wrt_st_x(cn_regnly[lue_rno],7,msg,&nch);
	  }
   else
	  writeerror_w(L"Kaikki pyydetyt tiedot käsitelty", 0, true);
   }

#endif
/* Ensimmäinen endif lopettaa regnly-määrityksen, toinen MAALI-osat */

#ifdef GAZ
#define MAXGAZ 4
INT gaz_com, gaz_irq, gaz_addr, gazvaihe;
static INT gaz_kno[MAXGAZ], nayta_gaz = 1, gaztapptr = 9, gazviive = 100;
int gaz_comopen, cn_gaz;
static char gazline[MAXGAZ][26], gazline0[MAXGAZ][26];
INT gazrata[MAXGAZ], gazpiste[MAXGAZ], gazluku = 1;

typedef struct {
    INT32 t;
    INT kno;
    } gaztaptp;

static gaztaptp gaztap[10];

void add_gaztapahtuma(kilptietue *kilp)
    {
    INT32 t;

    t = biostime(0,0);
    if (gaztapptr && t < gaztap[gaztapptr].t + gazviive) {
       gaztapptr--;
       }
    memmove(gaztap, gaztap+1, 9*sizeof(gaztaptp));
    gaztap[9].t = t;
    gaztap[9].kno = kilp->id();
    }

void opengaz(void)
   {
   int gaz_baud, er = 1;

   if (gaz_com > 8) {
      return;
	  }
   gaz_baud = 5;
   cn_gaz = GAZCOM;
   if (openport_x(cn_gaz, gaz_com, gaz_baud, 'n', 8, 1, 0)) {
	  writeerror_w(L"GAZ-tiedonsiirron avaaminen ei onnistunut", 0, true);
      gaz_com = 0;
      return;
	  }
   }

void close_gaz(void)
	{
	closeport_x(cn_gaz);
	}

void val_gaz(void)

   {
   static INT esto;
   INT x, y, h, l, kno;
   char saveline[80];
   wchar_t wtc;

   if (esto) return;
   esto = TRUE;
   sccurst(&y, &x, &h, &l);
   virdrect(ySize-3,0,ySize-3,79,saveline,0);
   kno = gaz_kno[0];
   for (;;) {
      clrln(ySize-3);
	  vidspwmsg(ySize-3,0, 7, 0, L"Anna kilpailunumero          A)utomaattinen, N)äyttö ruudulla, Esc: peruuta");
      INPUTINTW(&kno, 4, 20, ySize-3, L"aAnN\x1b\r", &wtc);
      if (wtc == ESC)
         break;
      if (wtc == L'n' || wtc == L'N') {
         nayta_gaz = !nayta_gaz;
         continue;
         }
      if (wtc == L'a' || wtc == L'A')
         kno = -1;
      else
         if (kno && getpos(kno) <= 0)
			writeerror_w(L"Virheellinen kilpailijanumero", 0, true);
	  break;
      }
   if (wtc != ESC) {
	  gaz_kno[0] = kno;
/*
	  tc = nayta_gaz ? L'K' : L'E';
	  wselectopt(L"Näytetäänkö tulostaulurivi ruudulla (K/E)", L"KE", &tc);
      nayta_gaz = tc == L'K';
*/
      }
   viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
   sccurset(y,x);
   esto = FALSE;
   }

#if N_PV > 1

static HTHREAD hgazThread;
static char gazline1[26];
static int ingazThread;

void gazTh(LPVOID lpCn)
	{
	int d;

	ingazThread = 1;
   wrt_st_x(cn_gaz, strlen(gazline[0]), gazline[0], &d);
   strcpy((char *)gazline0[0], (char *)gazline[0]);
   if (nayta_gaz) {
	  gazline[0][23] = ' ';
	  memmove(gazline[0]+1, gazline[0], 25);
	  gazline[0][0] = ' ';
	  vidspmsg(ySize-2,45,7,0,gazline[0]);
		strcpy(gazline[0], gazline0[0]);
	  }
	Sleep(gazvaihe);
   wrt_st_x(cn_gaz, strlen(gazline1), gazline1, &d);
   strcpy(gazline0[0], gazline1);
	strcpy(gazline[0], gazline1);
   if (nayta_gaz) {
	  gazline1[23] = ' ';
	  memmove(gazline1+1, gazline1, 25);
	  gazline1[0] = ' ';
	  vidspmsg(ySize-2,45,7,0,gazline1);
		strcpy(gazline1, gazline0[0]);
      }
	ingazThread = 0;
	}
#endif

void writegaz(void)
   {
   INT d, sj, kno, gazrivi, ensaika = 1;
   INT32 tls, tls1, t;
   static INT32 t_ed;
   kilptietue kilp;
	static INT edkno = -1;
	static INT32 edtls = -1;
	static int inwritegaz;
	char as[20];

   if (!gaz_com || inwritegaz)
      return;
#if N_PV > 1
	if (ingazThread)
		return;
#endif
	inwritegaz = 1;
	for (gazrivi = 0; gazrivi < gazluku; gazrivi++) {
		t = biostime(0, 0);
//		if (t > t_ed && t < t_ed + 5)
//			return;
//#ifdef MERI5
		if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0 && gazluku > 1)
			gaz_kno[gazrivi] = ratakno[gazrata[gazrivi]];
//#endif
		memset(gazline[gazrivi], ' ', 23);
		gazline[gazrivi][23] = '\r';
		gazline[gazrivi][24] = 0;
		gazline[gazrivi][0] = 'A'+gazrivi;
		if (gaz_kno[gazrivi] < 0) {
			if (gaztapptr < 9 && t > gaztap[gaztapptr].t + gazviive) {
				gaztapptr++;
				gaztap[gaztapptr].t = t;
				for (d = gaztapptr+1; d < 10; d++) {
					t += gazviive;
					gaztap[d].t = t;
					}
				}
			kno = gaztap[gaztapptr].kno;
			}
		else
			kno = gaz_kno[gazrivi];
		putintright(kno % 1000, gazline[gazrivi]+3);
#if N_PV > 1
		strcpy(gazline1, gazline[gazrivi]);
#endif
		if (kno) {
			d = getpos(kno);
			if( d > 0 ) {
				kilp.GETREC(d);
				tls = kilp.p_aika(gazpiste[gazrivi]);
#if N_PV > 1
				if (k_pv > 0)
					tls1 = kilp.ytulos(k_pv);
#endif
				if (!tls) {
//#ifdef MERI5
					if (wcswcind(kilpparam.kilplaji, L"NP") < 0 || gazluku == 1 || (eralahto[era_akt] != TMAALI0*AIKAJAK && ensaika))
//#endif
						{
						ensaika = 0;
						kilp.set_tulos(gazpiste[gazrivi], T_TIME(t)*(SEK/10));
						}
					}
				EnterCriticalSection(&tall_CriticalSection);
				kilp.merk_sijat();
				LeaveCriticalSection(&tall_CriticalSection);
				sj = kilp.p_sija(gazpiste[gazrivi]);

				memcpy(gazline[gazrivi]+9, AIKATOSTRS((char *)as, kilp.p_aika(gazpiste[gazrivi]), 0), tls ? pvparam[k_pv].laikasj : 8);
				gazline[gazrivi][11] = gazline[gazrivi][14] = ':';
				if (pvparam[k_pv].laikasj > 8) {
					if (tls) {
						gazline[gazrivi][17] = '.';
						}
					else
						memset(gazline[gazrivi]+17, ' ', 4);
					}
/*
				memcpy(gazline[gazrivi]+10, AIKATOSTRS(as, ytulos(&kilp, k_pv), 0), tls ? laika : 8);
				gazline[gazrivi][12] = gazline[gazrivi][15] = ':';
				if (laika > 8) {
					if (tls) {
						gazline[gazrivi][18] = '.';
						}
					else
						memset(gazline[gazrivi]+18, ' ', 3);
					}
*/
				if (sj)
					putintright(sj%100, gazline[gazrivi]+22);
#if N_PV > 1
				if (k_pv > 0 && gazvaihe > 0) {
					if (tls) {
						sj = kilp.y_sija(k_pv);
						memcpy(gazline1+9, AIKATOSTRS((char *)as, tls1, 0), pvparam[k_pv].laikasj);
						gazline1[11] = gazline1[14] = ':';
							if (pvparam[k_pv].laikasj > 8) {
							if (tls1) {
								gazline1[17] = '.';
								}
							else
								memset(gazline1+17, ' ', 4);
							}
						if (sj)
							putintright(sj%100, gazline1+22);
						if (tls == edtls && kno == edkno)
							strcpy(gazline[gazrivi], gazline1);
						edtls = tls;
						edkno = kno;
						}
					else
						strcpy(gazline1, gazline[gazrivi]);
					}
#endif
				}
			}
#if N_PV > 1
		if (k_pv > 0 && gazvaihe > 0 &&
			(strcmp(gazline[gazrivi], gazline0[0]) || strcmp(gazline1, gazline0[0])) && strcmp(gazline1, gazline[gazrivi])) {
			if (!ingazThread)
				hgazThread = _beginthread(gazTh, 40960, &kno);
			}
		else
#endif
		if (strcmp(gazline[gazrivi], gazline0[gazrivi])) {
			wrt_st_x(cn_gaz, strlen(gazline[gazrivi]), gazline[gazrivi], &d);
		   t_ed = t;
			strcpy(gazline0[gazrivi], gazline[gazrivi]);
			if (nayta_gaz) {
				gazline[gazrivi][23] = ' ';
				memmove(gazline[gazrivi]+1, gazline[gazrivi], 25);
				gazline[gazrivi][0] = ' ';
				vidspmsg(ySize-2,45,7,0,gazline[gazrivi]);
				strcpy(gazline[gazrivi], gazline0[gazrivi]);
				}
			}
		}
	inwritegaz = 0;
   }

#endif

#ifdef AUTOVA
int varasti[MAXSARJALUKU][VALUKU+1];
INT port_autova, autova_open;
INT32 t0_autova;

int start_autova(void)
   {
   wchar_t ch;

   luelahdepisteet();
   autova_open = 1;
   t0_autova = -t0*3600L*SEK;
   if (aikaero[1] != TMAALI0) {
		t0_autova += aikaero[1];
		if (aikaero[1] < 0)
			t0_autova += 24*TUNTI;
		}
   else if (aikaero[NREGNLY] != TMAALI0) {
		t0_autova += aikaero[NREGNLY];
		if (aikaero[1] < 0)
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
   long vatime, t, tt;
//   long ttmax = 0;
   int kno, pst, d;
   kilptietue kilp;
   wchar_t msg[81], st[16];
   kilptietue entkilp;
	aikatp itm;

	if (!aikajono[aika_jono[0][1]])
		return;
   vatime = SEK/10*(aika - 12*36000L) + aikajono[aika_jono[0][1]]->aikakorjaus;
   memset(&itm, 0, sizeof(itm));
   itm.t = AIKAJAK * vatime;
   itm.date = tm_date(itm.t);
   itm.jono = (UINT8) aika_jono[0][1];
   itm.badge = bdg;
   itm.kanava = (INT16)valeim;
   laheta_ec(bdg, valeim, aika, kielto);
   if ((kno = bdg2kno(bdg)) > 0) {
	  t = t_time_l(biostime(0,0), t0);
	  t = t-vatime*10/SEK;
	  if (t < 0)
		 t = -t;
	  if (t > 12*36000L)
		 t = 24*36000L - t;
	  if ((!vaikavert || t < 600*vaikavert) && (d = getpos(kno)) > 0) {
			kilp.GETREC(d);
			entkilp = kilp;
			pst = haelahdepiste(&kilp, itm.kanava, vatime);
			itm.kno = kno;
			itm.piste = pst;
			if (pst >= 0 && estaneg &&
				(NORMTULOS(vatime - kilp.pv[k_pv].tlahto) > 20*TUNTI ||
				NORMTULOS(vatime - kilp.pv[k_pv].tlahto) < estaneg)) {
				itm.status = '\x01';
				if (vaajat)
					tall_rivi(aika_jono[0][1], &itm, NULL, NULL, 0, 0, 0, false);
//				if (loki) {
//					wkirjloki(L"NEGATIIVINEN TAI HAAMUAIKA");
//					}
				}
			else {
				if (vaajat)
					tall_rivi(aika_jono[0][1], &itm, NULL, NULL, 0, 0, 0, false);
				tt = kilp.maali(pst);
				if (pst >= 0 && (tt == TMAALI0 || (uusinaika && abs((long) NORMKELLO(vatime - tt)) < uusinaika))) {
					vatime =  pyoristatls(vatime , 1);
					kilp.set_tulos(pst, vatime);
					EnterCriticalSection(&tall_CriticalSection);
					kilp.tallenna(d, 0, kielto, 0, 0);
					LeaveCriticalSection(&tall_CriticalSection);
					}
				}
		 }
	  else {
		 swprintf(msg, L"Yli %d min aikaero saapuneen väliaikatiedon ja tämän koneen välillä", vaikavert);
		 writeerror_w(msg, 4000, true);
		 }
	  }
   else {
	  if (ilmtunt) {
		 AIKATOWSTRS(st, vatime, t0);
		 swprintf(msg, L"Tuntematon Emit-koodi %d saapunut, aika: %8.8s, piste: %d", bdg, st, valeim);
		 writeerror_w(msg, 4000, true);
		 }
	  if (loki) {
		 wkirjloki(msg);
		 }
	  }
	if (vaajat && !itm.piste) {
		tall_rivi(aika_jono[0][1], &itm, NULL, NULL, 0, 0, 0, false);
		}
	}
#endif  // AUTOVA

#ifdef EMITHTTP
void vahaku(LPVOID lpCn)
{
	wchar_t page[100], edate[16], st[16], msg[200], *pMsg;

	int er = 0, keTserial;
	static wchar_t *eMsg[] = {
		L"Muistin varaaminen ei onnistunut",
		L"Virhe pyyntöä lähetettäessä",
		L"Yhteyden avaaminen ei onnistunut",
		L"Istunnon avaaminen ei onnistunut",
		L""};

	inhttpHaku = 1;
	keTserial = * (int *)lpCn;
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

	swprintf(page, L"/emitphp/get_stream.php?Y=%d&time=%s%s%s", eTParam.eTserial[keTserial], edate, L"%20",
		eTParam.eTtime);
	if (eTParam.msgno[keTserial] > 1)
		swprintf(page+wcslen(page), L"&Mmin=%d", eTParam.msgno[keTserial]);

	if (testi && loki) {
		wchar_t msg[100];
		swprintf(msg, L"%10d http-haku alkaa", mstimer());
		wkirjloki(msg);
		}
	if ((er = httphaku(eTParam.eThost , eTParam.eTport, page, 0, eTParam.buf, eTParam.buflen, &eTParam.haettu)) != 0) {
		if (er <= -3) {
			swprintf(msg, L"Virhe väliaikahaussa: %s", eMsg[-3-er]);
			}
		else {
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL, er, 0, (LPWSTR) &pMsg, 0, 0);
			swprintf(msg, L"Virhe väliaikahaussa: %s", pMsg);
			}
		writeerror_w(msg, 2000, true);
		}
	if (testi && loki) {
		wchar_t msg[100];
		swprintf(msg, L"%10d http-haku valmis, haettu %d merkkiä", mstimer(), eTParam.haettu);
		wkirjloki(msg);
		}
	inhttpHaku = 0;
}

void etHaku(void)
{
	static int edhaku = -1, inhttpVa, keTserial;
	static char httpbuf[100000];
	static char *eTp, *eTp1;
	int t;

	if (taustaon && !vaiheenvaihto && eTParam.eTserial[0] > 0 && eTParam.eThaku && !inhttpHaku && !inhttpVa) {
		t = biostime(0,0L);
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
			for (int i = 0; taustaon && !vaiheenvaihto && i < 10; i++) {
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
}
#endif

void tausta(INT spoolfl)
	{
   static int init = 1, msgno = 1, keTserial, t_edell;
   static wchar_t saika[] = L"          ", st[20];
	static char httpbuf[100000];
   INT32 t,tdif, t_lah;
   INT i,j;
	static char *eTp, *eTp1;
	static UINT32 ed_lkm_t;
	static int edlahkello[MAX_LAHPORTTI];
	static INT32 prev_t = -1, edhaku = -1, inhttpVa;
	static INT alku_pv;
#if defined(AUTOFILE) || defined(TAULU_2)
	long t_ta;
	static long t_af = TMAALI0;
#endif

   autotulostus();
   t_lah = mstimer();
	for (int cn = 0; cn < maxyhteys; cn++) {
		if (lahkello[cn] && t_lah > edlahkello[cn] + lahkello[cn] &&
			lahettamatta(cn) == 0) {
			lahetakello(cn, 0);
			edlahkello[cn] = t_lah;
			}
		}
#ifdef TAULU
   if (taulu_com || udptaulu) paiv_taulu();
#endif

#ifdef GAZ
   if (gaz_com) writegaz();
#endif

#ifdef SAKKOCOM_xx
   if (sak_com) rec_sak();
#endif

#if defined(TV1)
   if (tv_fl) tv_asetus();
#endif

   if (prev_t < 0) {
      prev_t = biostime(0,0L);
      alku_pv = lue_v_pv();
	  }
	t = biostime(0,0L);
#ifdef _CONSOLE
	if (t_lah - t_edell > 100) {
		if (writeerrorOn > 2)
			writeerrorOn--;
		else if (writeerrorOn == 2)
			writeerrorOn = 0;
		t_edell = t_lah;
		}
	wmemcpy(saika+1,AIKATOWSTRS(st,KSEK*T_TIME(t),t0),8);

    viwrrectw(0,35,0,44,saika,7,0,0);
   if ((T_TIME(t) - ed_lkm_t > 9) || (T_TIME(t) - ed_lkm_t < 0)) {
		lukumaarat();
		ed_lkm_t = T_TIME(t);
		}
#if defined(AUTOFILE) || defined(TAULU_2)
   if (autofileparam.afvali) {
	  t_ta = KSEK*T_TIME(t);
	  if (t_af == TMAALI0 || NORMKELLO(t_ta - t_af) >= 0) {
		 if (t_af != TMAALI0) {
			if (autofile(0))
			   t_af = t_ta + 50*KSEK;
			else
			   t_af = t_ta + autofileparam.afvali;
			}
			else t_af = t_ta + autofileparam.afvali;
		 }
	  t_af = NORMKELLO(t_af);
	  }
#endif
#endif
#ifdef EMITHTTP
	etHaku();
#endif
		i = 0;
		if ((tdif = t - ivtime[i]) > 0 || tdif + intv[i] < 0) {
		 if (i == 0) {

//            if (timerfl) writeajat();

				tauko = FALSE;
			for (j = 0; j < sarjaluku + nsarjayhd; j++)

			   if( (prior[j] < 1000000000L) )
				  prior[j] = (prior[j] << 1) - (prior[j] >> 1);

			timefl = TRUE;
			}

			ivtime[i] = (t + intv[i]) % DAYTICKS;
		 }
	}

void taustaThr(LPVOID lpCn)
   {
   while(taustaon) {
	  if (!vaiheenvaihto)
		  tausta(0);
#ifdef _CONSOLE
	  Sleep(10);
#else
	  Sleep(100);
#endif
		}
	taustasulj = 1;
	}

void tee_aika(int laji)
   {
   INT ino, wt;

   INT32 tm;
   aikatp utime;
   INT tall_fl;
   INT d;
   kilptietue kilp, entkilp;

   tm = KSEK*T_TIME(biostime(0,0));
   memset(&utime, 0, sizeof(utime));
   if (vainoletus && !(lajatfl && aika_tunnus[0][0] == -1))
	  utime.piste = ol_piste;
   else
	  utime.piste = aika_tunnus[0][laji-1];
   if (maxjono > 0)
	  ino = aika_jono[0][laji-1];
   else if (lajatfl && utime.piste == -1)
	  ino = 1;
   else
	  ino = 0;
   if (!aikajono[ino]) {
	   writeerror_w(L"AJANOTTOA EI KÄYNNISTETTY, MUTTA YRITETÄÄN KÄYTTÄÄ", 0, true);
	   return;
	   }
   tm += aikajono[ino]->aikakorjaus;
   utime.t = NORMKELLO(tm)*AIKAJAK;
   utime.date = tm_date(utime.t);
   if (aikajono[ino]->rwtime - aikajono[ino]->mxtime >= 0) {
		if (lajatfl && ino == 1)
			writeerror_w(L"LÄHTÖAIKATAULUKKO TÄYSI",0);
		else
			writeerror_w(L"AIKATAULUKKO TÄYSI",0);
	    aika_tail = aika_head;
	    return;
		}
   wt = aikajono[ino]->aktrows;
   tall_fl = 0;
   if (tunnjono >= 0) {
	   utime.kno = tunn_hae_kno(purajak(utime.t));
	   if ((d = getpos(utime.kno)) > 0) {
			kilp.GETREC(d);
			entkilp = kilp;
			kilp.tall_tulos(0, purajak(utime.t));
		    tall_fl = 1;
			}
	   }
   if (pvparam[k_pv].hiihtolahto && utime.piste == -1) {
	  if ((d = lahtija(purajak(utime.t))) != 0) {
			kilp.GETREC(d);
			entkilp = kilp;
		 utime.kno = kilp.id();
		 kilp.tall_tulos(-1, purajak(utime.t));
		 tall_fl = 1;
		 }
	  }
   utime.jono = ino;

   tall_rivi(ino, &utime, NULL, NULL, 0, 0, 0, false);
   if (tall_fl) {
	  EnterCriticalSection(&tall_CriticalSection);
	  kilp.tallenna(d,TRUE, 0, 0, &entkilp);
	  LeaveCriticalSection(&tall_CriticalSection);
#ifdef _CONSOLE
	  vidint(ySize-1,54,4,utime.kno);
#endif
	  }
   if (ino == 0 && od_fl && od_aika == TMAALI0*AIKAJAK &&
	   NORMKELLO_A(utime.t - aikajono[ino]->getAktTime(0, aikajono[ino]->rwtime - 1)->t) == 0) {
	  od_aika = utime.t;
	  odota = wt;
	  }
	}

#ifndef _CONSOLE
int kbdiv1(char ch, char ch2)
   {
   return(0);
   }

int ProcessSpecialKey(INPUT_RECORD *Buf)
   {
   return(0);
   }

#else // _CONSOLE
int kbdiv1(char ch,char ch2)
   {
   static INT helpfl = FALSE, inkbdiv1;
   char tc;
   wchar_t wtc;
   char saveline[80];
   INT k,x,y,h,l,retval = 0,cc;
   aikatp pt;

//   void naytamaali(INT *r1, INT *r0, INT k);
   extern INT maali_on;
#ifdef MAKI
	static char tmsg[22] = "MRI  Resend points\r";
#endif
	kilptietue kilp;
	INT d;

   if (nivkeys && ch == keytab[0].ch && ch2 == keytab[0].keycode) {
      tee_aika(3);
      retval = 1;
      goto escape;
      }
   cc = 256*ch + ch2;
   switch (cc) {
      case 59:
         if (!helpfl) {
            helpfl = TRUE;
            help(0);
            helpfl = FALSE;
            }
		retval = 1;
		goto escape;
      case 94:
      case 95:
      case 96:
      case 97:
      case 98:
      case 99:
      case 100:
      case 101:
      case 102:
      case 103:
         kuulpiste = ch2 - 95;
         if (kuulpiste < 0) kuulpiste = k_pv ? kilpparam.valuku+1 : 0;
         retval = 1;
         goto escape;
      case 3*256 :                                        // Ctrl-C
      case 3*256+46 :                                     // Ctrl-C
			   retval = 1;
               break;
      case 256 : 
      case 256+30 : 
				retval = 1;
               if (esto) break;                           // Ctrl-A
               esto = TRUE;
			   sccurst(&y, &x, &h, &l);
               virdrect(ySize-3,0,ySize-3,79,saveline,0);
               tc = 'K';
               if (timerfl) {

				  wselectopt(L"Vaihdetaanko ajanottotaulukko (K/E)", L"KE", &wtc);
                  if (wtc == L'K') {
                     for (k = 0; k <= maxjono; k++)
                        vaihda_ajat(k, 0);
					 if (!maxjono && lajatfl) vaihda_ajat(1, 0);
                     }
                  }
               viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
               sccurset(y,x);
               esto = FALSE;
               break;
	  case  5*256:
	  case  5*256+18:
														  // Ctrl-E
			   if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
				   if (inkbdiv1) {
					  retval = 1;
					  goto escape;
					  }
				   inkbdiv1 = 1;
				   sccurst(&y, &x, &h, &l);
				   virdrect(ySize-3,0,ySize-3,79,saveline,0);
				   clrln(ySize-3);
				   vidspwmsg(ySize-3,0,7,0,L"Anna erän numero:");
				   inputintw(&era_akt, 3, 18, ySize-3, L"\r", &wtc);
				   if (era_akt < 0 || era_akt >= MAXERA)
					   era_akt = 0;
//				   eralahto[era_akt] = TMAALI0*AIKAJAK;
					for (k = 0; k < kilpparam.lahtoryhma; k++)
						ratakno[k] = haeratakilp(era_akt, k+1);
					if (gaz_com && ratakno[0] && eralahto[era_akt] == TMAALI0*AIKAJAK) {
						wtc = L'E';
						wselectopt(L"Onko erä jo lähtenyt? (K/E)", L"KE", &wtc);
						if (wtc == L'K' && (d = getpos(ratakno[0])) > 0) {
							kilp.GETREC(d);
							eralahto[era_akt] = kilp.p_aika(-1);
							lahetaeralahto(era_akt, 0);
							}
						}
				   viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
				   sccurset(y,x);
				   vidint(0, 4, 3, era_akt);
				   inkbdiv1 = 0;
				   retval = 1;
				   }
			   goto escape;				
#if defined(GAZ)
      case  7*256: 
      case  7*256+34: 
               val_gaz();                                 // Ctrl-G
			   retval = 1;
			   goto escape;
#endif
#if defined(MAKI)
      case 12*256: 
	  case 12*256+38:	                                 // Ctrl-L
					if (tuomari_yht) {
						tc = ' ';
                  virdrect(ySize-3,0,ySize-3,79,saveline,0);
						wselectopt(L"Huomautettavan tuomarin tunnus (ABCDEFG), Esc : Peruuta", L"ABCDEFG\x1b", &tc);
						if (tc != ESC) {
							tmsg[1] = 2*'A' + cometluku - 1 - tc;
					      wrt_st_x(cn_regnly[tuomari_yht],strlen(tmsg),tmsg,&k);
							}
			         viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
						}
               retval = 1;
               goto escape;
#endif
      }
   if (retval && aikajono[akttm]) {
	  if ( (aikajono[akttm]->rwtime - aikajono[akttm]->mxtime) > 0)
	      aikajono[akttm]->rwtime = aikajono[akttm]->mxtime;
      if (aikajono[akttm]->aktrows > 0) {
		  vidint(ySize - 1, 54, 4, aikajono[akttm]->getAktTime(&pt, aikajono[akttm]->rwtime - 1)->kno);
		 VIDSPAIKA(ySize-1,60,purajak(pt.t),t0);
		 }
	  if ( (aikajono[akttm]->aktrows - htime[akttm]) > 0) htime[akttm] = aikajono[akttm]->aktrows;
	  vidint(ySize-1,71,4,aikajono[akttm]->aktrows);
	  vidint(ySize-1,76,4,aikajono[akttm]->rtime+1);
//	  if (maali_on) naytamaali(&x, &y, 0);
      }
escape:
   return(retval);
   }
#endif // _CONSOLE

typedef struct {
   INT32 t;
   INT16 kno;
   INT8 piste;
   UINT8 tapahtuma;
   INT32 badge;
   INT16 kanava;
   INT16 jono;
   } aikatp_v;

void initajat(INT ino, INT vaihda)
{
	INT i, n;
	wchar_t ch, msg[30];
	INT32 fl;
	aikatp pt;
	static wchar_t chs;
	bool vanha = true;
	int reclen;
	int date = 0;
	int ed_t = TMAALI0;
	char buf[100];
	aikatp_v *vpt = (aikatp_v *)buf;

	if (aikajono[ino] == NULL) {
		aikajono[ino] = new aikajonotp(maxntime);
		aikajono[ino]->jono = ino;
		aikajono[ino]->aikakorjaus = aikakorjaus[ino];
		}
	if (!vaihda) {
	  clrln(ySize-3);
	  if (maxjono > 0)
		 swprintf(msg, L"    Aikatiedosto %d:", ino+1);
	  else if (!ino)
         wcscpy(msg, L"     Aikatiedosto :");
	  else
		 wcscpy(msg, L"Lähtöaikatiedosto :");
#ifdef _CONSOLE
	  vidspwmsg(ySize-3,0,7,0,msg);
	  inputwstr(aikajono[ino]->aikafname,37,20,ySize-3,L"\r",&ch,FALSE);
#else
	  inputstr_prompt(aikajono[ino]->aikafname,37,msg, &ch);
#endif
		}
	else if (vaihda != -1) {
		for (i = 0; i <= aikajono[ino]->mxtime+1; i++) {
			gettm(i, &pt, ino)->kno = 0;
			pt.t = TMAALI0*AIKAJAK;
			pt.piste = -2;
			puttm(i, &pt, ino);
			}
		memset(aikajono[ino]->aktrow, 0, aikajono[ino]->mxtime*sizeof(aikajono[ino]->aktrow[0]));
		aikajono[ino]->rtime = 0;
		aikajono[ino]->rwtime = 0;
		aikajono[ino]->aktrows = 0;
		}
	aikajono[ino]->fp1 = 0;
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
				reclen = sizeof(aikatp_v);
				_lseek(aikajono[ino]->afhandle, 0, SEEK_SET);
				}
			if (fl > (aikajono[ino]->mxtime)*reclen)
			fl = (aikajono[ino]->mxtime)*reclen;
			if (sail_ajat)
				ch = L'K';
			else
				{
				ch = L' ';
				if (!ino || (!lajatfl && !chs)) {
#ifdef _CONSOLE
					wselectopt(L"AJANOTTOTIEDOSTON AVAAMINEN - Säilytetäänkö aiemmat "
						L"ajanottotiedot (K/E)", L"KE", &ch);
#else
					i = select3(2, L"AJANOTTOTIEDOSTON AVAAMINEN - Luetaanko aiemmat "
						L"tiedot levyltä", L"Valitse", L"Luetaan", L"Aiemmat tuhotaan",
						L"Säilytä vanhat ja poistu", 0);
					ch = (L"\x1bKE\x1b")[i];
#endif
					chs = ch;
					}
				else if (!maxjono) {
#ifdef _CONSOLE
					wselectopt(L"LÄHTÖAIKATIEDOSTON AVAAMINEN - Säilytetäänkö aiemmat "
						L"lähtöaikatiedot (K/E)", L"KE", &ch);
#else
					i = select3(2, L"LÄHTÖAIKATIEDOSTON AVAAMINEN - Luetaanko aiemmat "
						L"lähtöajat levyltä", L"Valitse", L"Luetaan", L"Aiemmat tuhotaan",
						L"Säilytä vanhat ja poistu", 0);
					ch = (L"\x1bKE\x1b")[i];
#endif
					chs = ch;
					}
				else
					ch = chs;
				clrln(ySize-3);
				}
			if (ch == ESC) {
				inLopetus = 1;
				lopetus();
				}
			if (ch == L'K') {
				while (fl > 0) {
					if(_read(aikajono[ino]->afhandle, buf, reclen) != reclen)
						iowerr(aikajono[ino]->aikafname,1,aikajono[ino]->fp1);
					fl -= reclen;
					if (vanha) {
						pt.t = vpt->t;
						pt.kno = vpt->kno;
						pt.piste = vpt->piste;
						pt.tapahtuma = vpt->tapahtuma;
						pt.badge = vpt->badge;
						pt.kanava = vpt->kanava;
						pt.jono = (UINT8)vpt->jono;
						if (pt.t < ed_t)
							date++;
						ed_t = pt.t;
						pt.date = date;
						}
					else
						memcpy(&pt, buf, reclen);
					puttm(aikajono[ino]->fp1, &pt, ino);
					aikajono[ino]->fp1++;
					}
				}
			}
		aikajono[ino]->rwtime = aikajono[ino]->fp1;
		while ((aikajono[ino]->rwtime > 0) && gettm(aikajono[ino]->rwtime-1, 0, ino)->t == TMAALI0*AIKAJAK)
			aikajono[ino]->rwtime--;
		htime[ino] = aikajono[ino]->rwtime;
		aikajono[ino]->rtime = aikajono[ino]->rwtime;
		while ((aikajono[ino]->rtime > 0) && gettm(aikajono[ino]->rtime-1, 0, ino)->kno == 0)
			aikajono[ino]->rtime--;
		}
	else
		iowerr(aikajono[ino]->aikafname,3,-1L);
	_lseek(aikajono[ino]->afhandle, 0, SEEK_SET);
	n = 0;
	for (i = 0; i <= aikajono[ino]->mxtime; i++) {
		if (i == 0) {
			memset(buf, 0, sizeof(aikatp));
			sprintf(buf, "AJAT_V%d", VersioInt());
			}
		else {
			gettm(n++, &pt, ino);
			memcpy(buf, &pt, sizeof(aikatp));
			}
		if (_write(aikajono[ino]->afhandle, buf, sizeof(aikatp)) !=
			sizeof(aikatp)) {
			iowerr(aikajono[ino]->aikafname,2,i*sizeof(aikatp));
			break;
			}
		}
	_close(aikajono[ino]->afhandle);
	if ((aikajono[ino]->afhandle = _wopen(aikajono[ino]->aikafname, O_RDWR | O_BINARY,
		S_IREAD | S_IWRITE)) < 0 ) {
		iowerr(aikajono[ino]->aikafname,3,-1L);
		}
	if (aikajono[ino]->afhandle > 0) {
		aikajono[ino]->fafhandle = _wfdopen(aikajono[ino]->afhandle, L"r+b");
		setvbuf(aikajono[ino]->fafhandle, NULL, _IONBF, 0);
		}
	aikajono[ino]->haeAktAjat();
	}

static void vaihda_ajat(INT ino, INT era)
   {
   wchar_t ch, vanhanimi[38], buf[60];

   if (!era) {
	  clrln(ySize-3);
	  if (!ino || maxjono) {
		 wcscpy(vanhanimi, L"    Uusi aikatiedosto  :");
		 if (maxjono)
			vanhanimi[22] = ino + L'1';
		 }
	  else
		 wcscpy(vanhanimi, L"Uusi lähtöaikatiedosto :");
	  wcsncpy(buf, aikajono[ino]->aikafname, 38);

#ifdef _CONSOLE
	  viwrrectw(ySize-3,0,ySize-3,23,vanhanimi,7,0,0);
	  inputwstr(aikajono[ino]->aikafname,37,25,ySize-3,L"\r\x1b",&ch,FALSE);
#else
	  inputstr_prompt(aikajono[ino]->aikafname,37,vanhanimi,&ch);
#endif
	  if (ch == ESC) {
		 wcsncpy(aikajono[ino]->aikafname, buf, 38);
		 return;
		 }
	  if (!wcscmpU(aikajono[ino]->aikafname, buf)) return;
	  }
   else {
	  swprintf(aikajono[ino]->aikafname, L"AJAT%1.1d%03.3d.LST", ino+1,era);
	  }
	suljeajat(ino, false);
   initajat(ino, 1);
   }

void inittimer(void)
{
	INT i,ino, pt;
	char key, che;
	wchar_t ch, tc;
	wchar_t prs[80], st[14];
	aikatp ttm;
	static bool initialized = false;

	INT err, r_no;
	wchar_t msg[81];

	if (emitajat > MAXJONO)
		emitajat = 1;
	if (emitajat-1 > maxjono)
		maxjono = emitajat-1;
	for (r_no = 0; r_no < NREGNLY; r_no++) {
		if (!regnly[r_no])
			continue;
		if ((initialized || r_no) || !ajanottofl) {
			if (regnly[r_no] && regnly[r_no] != LID_ETGPRS && regnly[r_no] != LID_ETSQL)
				start_regnly(r_no);
			}
		}
	if (!ajanottofl) {
		initialized = true;
		return;
		}
	esto = TRUE;
#ifdef DBGFILE
	if (dbgtofile)
		opendbgfile();
#endif
	memset(&ttm, 0, sizeof(aikatp));
	ttm.t = TMAALI0*AIKAJAK;
	ttm.piste = -2;
	if (maxntime == 0) {
		maxntime = (kilpparam.maxvaluku + (lajatfl ? 2 : 1)) * maxrec
			+ 50 + maxrec / 4;
		maxntime = std::max(maxntime, 1000);
		maxntime += 5000;
		}
	for (ino = 0; ino <= maxjono || (lajatfl && ino < 2); ino++) {
		if (aikajono[ino])
			continue;
		aikajono[ino] = new aikajonotp(maxntime);
		aikajono[ino]->jono = ino;
		aikajono[ino]->aikakorjaus = aikakorjaus[ino];
		for (pt = 0; pt <= aikajono[ino]->mxtime; pt++) {
			puttm(pt, &ttm, ino);
			}
		aikajono[ino]->rtime = 0;
		aikajono[ino]->rwtime = 0;
		wcscpy(aikajono[ino]->aikafname, aikaflname[lajatfl && ino == 1 ? 1 : 0]);
		if (maxjono) {
			aikajono[ino]->aikafname[wcslen(aikajono[ino]->aikafname)-1] = ino + L'1';
			}
		initajat(ino,-ajanottofl);
		if (ino == 0 && autokilp)
			seurkrivi = aikajono[0]->rwtime;
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
			do {
				clrln(ySize-3);
				vidspwmsg(ySize-3,0,7,0,L"Paina ajanottoon varattavaa näppäintä");
				i = nivkeys;
				nivkeys = 0;
				che = readkbd(&key, 1, 0);
				ch = oemtowchar(che);
				nivkeys = i;
				if (ch == L'\r' || ch == 27 || (towupper(ch) >= 'A' && towupper(ch) <= 'Z')) {
					writeerror_w(L"Ajanottonäppäin ei voi olla kirjain, 'Enter' tai 'Esc'",0);
					tc = L'E';
					continue;
					}
				if (ch > 32) {
					wcscpy(st, L"(merkki \' \') ");
					st[9] = ch;
					}
				else
					st[0] = 0;

				swprintf(prs,L"Näppäinkoodi (%d,%d) %s varattu ajanotolle, vahvista "
					L"(K/E)", (int) ch, (int) key, st);
				tc = L' ';
				wselectopt(prs, L"KE", &tc);
				} while (tc != L'K');
			keytab[0].ch = che;
			keytab[0].keycode = key;
			}
#endif
		err = 0;
		if (regnly[0])
			start_regnly(0);
		if (com_aika) {
			hAikacomThread = _beginthread(comajanotto, 40960, &aika_com);
			}
		if (err) {
			_itow(err, msg+38, 10);
			writeerror_w(msg, 0, true);
			}
		timerfl = TRUE;
		}
	esto = FALSE;
}

void remtimer()
{
	int r;

	for (r = 0; r < NREGNLY; r++)
		if (regnly_open[r])
			regnly_open[r] = 0;;
	if (timerfl) {
		timerfl = 0;
		}
	Sleep(200);
	for (r = 0; r < NREGNLY; r++)
		if (regnly[r])
			rem_regnly(r);
}

