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
#include <process.h>
#include <time.h>
#ifdef _CONSOLE
#include <bkeybrd.h>
#endif
/*
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#include <butil.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <sys/timeb.h>
*/
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
#include "TpLaitteet.h"

#include <wincom.h>

#define MAX_R_OD 10
#define MTR_ST_LEN 59
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
23  CPower
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

static int emitcmp(char *buf1, char *buf2, int len);
static int lue_ARES(int r_no, int cn, san_type *vastaus, char **p, int *nmsg);
static int lue_FEIG(int r_no, int cn, san_type *vastaus, char **p, int *nmsg);
static int lue_EmitKello(int r_no, int cn, san_type *vastaus, char **p, int *nmsg, int r_msg_len, int *od);
static int lue_LUKIJA(int r_no, int cn, san_type *vastaus, char **p, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen, int r_msg_len);
static int lue_MTR(int r_no, int cn, san_type *vastaus, char **p, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen, int r_msg_len);
static int lue_EMITAG(int r_no, int cn, san_type *vastaus, char **p, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen);
static int lue_SI(int r_no, int cn, san_type *vastaus, char **p, int *nmsg,
	int r_buflen, int r_msg_len);
void paivitaEcdata(void);

#ifdef _CONSOLE
void paivitaEcdata(void){}
#endif

int in_aseta_mtr;
INT32 mtr_pyynto[NREGNLY];
INT mtr_yksi;
TauluParamTp TauluParam[NREGNLY];
#ifdef LAJUNEN
int SiritEventPort[NREGNLY] = {50008};
int SiritCmdPort[NREGNLY] = {50007};
int LatestSirit[NREGNLY];
int LatestSiritPoll[NREGNLY];
int SiritEiVastaa[NREGNLY];
int SiritPoll[NREGNLY];
int reconnectSirit[NREGNLY];
char siritarrive[80] = "event.tag.arrive";
char siritdepart[80] = "event.tag.depart";
char SiritMask[31];
#endif
int viimrivi;
static INT pyyntoviive = 200;
static int regnlystarted[NREGNLY];

extern int lue_rno;
ecdatatp ecdata[NREGNLY];
extern FILE *emittallfile;
extern int n_emittallfile;

#ifndef MAXOSUUSLUKU
#else
#endif

#ifdef DBGFILE
FILE *dbgfile[NREGNLY];

void opendbgfile(void)
{
	int i;
	wchar_t flnm[] = L"dbgfile00.dat";

	for (i = 0; i < NREGNLY; i++) {
		if (regnly[i]) {
			flnm[7] = L'0' + (i + 1) / 10;
			flnm[8] = L'0' + (i + 1) % 10;
			dbgfile[i] = _wfopen(flnm, L"ab");
		}
	}
}

void closedbgfile(void)
{
	int i;

	for (i = 0; i < NREGNLY; i++) {
		if (dbgfile[i])
			fclose(dbgfile[i]);
		dbgfile[i] = 0;
	}
}
#endif

aikajonotp::aikajonotp(int lkm)
{
	memset(this, 0, sizeof(aikajonotp));
	atime = new aikatp[lkm];
	aktrow = new int[lkm];
	memset(atime, 0, lkm*sizeof(atime[0]));
	memset(aktrow, 0, lkm*sizeof(aktrow[0]));
	mxtime = lkm - 2;
	atimemask = '\xff';
}

aikajonotp::~aikajonotp(void)
{
	delete[] atime;
	delete[] aktrow;
}

void aikajonotp::haeAktAjat(void)
{
	aikatp edtm;
	if (!atime || !aktrow || rwtime == 0)
		return;
	if (rtime > 0)
		getAktTime(&edtm, rtime);
	else
		memset(&edtm, 0, sizeof(edtm));

	memset(aktrow, 0, mxtime*sizeof(aktrow[0]));
	aktrows = 0;
	for (int r = 0; r < rwtime; r++) {
		if (atime[r].status & atimemask)
			continue;
		aktrow[aktrows++] = r+1;
		if (!memcmp(&edtm, atime + r, sizeof(aikatp)))
			rtime = aktrows-1;
		}
	if (rtime > aktrows)
		rtime = aktrows;
}

aikatp * aikajonotp::getTime(aikatp *raika, UINT32 rvno)
{
	static aikatp caika;
	if (raika == NULL)
		raika = &caika;
	if (rvno < (UINT32) mxtime) {
		memcpy(raika, atime+rvno, sizeof(aikatp));
		}
	else {
		memset(raika, 0, sizeof(aikatp));
		raika->t = AIKAJAK*TMAALI0;
		raika->jono = jono;
		}
	return(raika);
}

aikatp * aikajonotp::getAktTime(aikatp *rtm, UINT32 rvno)
{
	static aikatp caika;
	if (rtm == NULL)
		rtm = &caika;
	if (rvno < (UINT32) aktrows && aktrow[rvno] > 0)
		getTime(rtm, aktrow[rvno]-1);
	else {
		memset(rtm, 0, sizeof(aikatp));
		rtm->t = AIKAJAK*TMAALI0;
		rtm->jono = jono;
		}
	return(rtm);
}

INT16 r_tarksum(char *buf, INT len, char sum, INT16 xfl)
	{
	char xch = 0;

	if (sallisummavirhe) {
		return(0);
	}
	if (xfl) xch = '\xdf';
	for (; len; len--, buf++)
		sum -= *buf ^ xch;
	return(sum);
	}

static INT32 ec_strtoaika(char *st, int t0, int *i_pv)
	{
	char *p, *p1, as[20] = "";
	INT32 t, m, s, o = 0;

	strncpy(as, st, sizeof(as)-1);
	p = strstr(as, ":");
	if (!p)
		return(0);
	*p = 0;
	t = atoi(as);
	if (i_pv)
		*i_pv = t/24;
	t = t % 24;
	p1 = strstr(p+1, ":");
	if (!p1)
		return(0);
	*p1 = 0;
	m = atoi(p+1);
	p = strstr(p1+1, ".");
	if (!p)
		return(0);
	*p = 0;
	s = atoi(p1+1);
	if (p[1] >= '0' && p[1] <= '9' && p[2] >= '0' && p[2] <= '9' &&
		p[3] >= '0' && p[3] <= '9')
		o = 100*(p[1]-'0')+10*(p[2]-'0')+(p[3]-'0');
	return(1000*(3600L*(t-t0)+60*m+s)+o);
	}

int tulk_emiTag(char *buf, emittp *em, INT32 *emtm, int r_no)
{
	aikatp ut;
	UCHAR eccode = 0;
	char *p, *p1, *p2, st[30] = "";
   INT i, j, piste = 0, msgtp = 0, i_pv, nctrl = 0, ino = 0;
	INT32 t, badge = 0;
	typedef struct {
		UINT16 code;
		INT32 time;
		} controltp;
	controltp controls[500];
	bool lukijaOn = false;
#ifndef _CONSOLE
	bool emittall = (emittallfile != NULL);
#else
	bool emittall = false;
#endif

	memset(controls, 0, sizeof(controls));
	t = TMAALI0 * (1000/SEK);
	p = strtok(buf, "\t\x03");
	while(p) {
		switch (*p) {
			case 'I':
				ecdata[r_no].pctime = KSEK*t_time_l(biostime(0,0), t0);
				strncpy(ecdata[r_no].devtype, p+1, sizeof(ecdata[r_no].devtype)-1);
				msgtp = 1;
				break;
			case 'B':
				if (msgtp == 0 && p[1] == 'M')
					msgtp = 4;
				break;
			case 'C':
				if (msgtp == 1)
					ecdata[r_no].code = atoi(p+1);
				else if (msgtp == 4 && atoi(p+1) == 0)
					eccode = 255;
				else
					eccode = (UCHAR)(atoi(p+1)%256);
				break;
			case 'M':
				if (msgtp == 1) {
					strncpy(st, p+1, sizeof(st)-1);
					p1 = strstr(st, "-");
					if (p1 == NULL)
						break;
					*p1 = 0;
					ecdata[r_no].first = atoi(st);
					ecdata[r_no].next = atoi(p1+1);
					}
				break;
			case 'X':
				if (msgtp == 1) {
					ecdata[r_no].mode = atoi(p+1);
					}
				break;
			case 'Y':
				if (msgtp == 1) {
					ecdata[r_no].serial = atol(p+1);
					}
				break;
			case 'A':
				if (msgtp == 1) {
					strncpy(ecdata[r_no].health, p+1, sizeof(ecdata[r_no].health)-1);
					}
				break;
			case 'H':
				if (msgtp == 1) {
					strncpy(ecdata[r_no].comm, p+1, sizeof(ecdata[r_no].comm)-1);
					}
				break;
			case 'N':
				badge = atol(p+1);
				if (msgtp != 4)
					msgtp = 2;
				break;
			case 'E':
			case 'W':
				if (msgtp == 1)
					ecdata[r_no].devtime = ec_strtoaika(p+1,t0, 0)/(1000/SEK);
				else if (t == TMAALI0 * (1000/SEK)) {
					t = ec_strtoaika(p+1,t0, &i_pv);
					t += 24 * TUNTI * i_pv * (1000/SEK);
					}
				break;
			case 'P':
				strncpy(st, p+1, sizeof(st)-1);
				p1 = strstr(st, "-");
				if (p1 == NULL)
					break;
				*p1 = 0;
				i = atoi(st);
				p2 = strstr(p1+1, "-");
				if (p2 == NULL)
					break;
				j = atoi(p1+1);
//				if (j > 255)
//					break;
				controls[i].code = (UINT32) j;
				p1 = p2+1;
				if (*p1 < '0' || *p1 > '9')
					break;
				controls[i].time = ec_strtoaika(p1, 0, &i_pv);
				controls[i].time += 24 * TUNTI * i_pv * (1000/SEK);
				if (i >= nctrl)
					nctrl = i+1;
				break;
			case 'F':
				msgtp = 3;
				if (p[3] == '1') {
					t = ec_strtoaika(p+5, t0, &i_pv);
					piste = p[1] - '0' + 1;
					}
				break;
			}
		p = strtok(NULL, "\t");
		}
	paivitaEcdata();
	if (em) {
		em->badge = badge;
		memcpy(&em->sc, "emiTag", 6);
		}
	if (emtm)
		*emtm = t;
	if (!emittall && msgtp == 1 && piste) {
#ifndef LUENTA
		if (aikajono[ino]) {
			j = 0;
			memset(&ut, 0, sizeof(ut));
			ut.t = 10 * t + aikajono[ino]->aikakorjaus * AIKAJAK;
			ut.date = tm_date(ut.t);
#ifndef MAXOSUUSLUKU
			ut.kanava = (UCHAR) eccode;
			ut.piste = aika_tunnus[r_no][4-piste];
			if (ut.piste == -1) {
				if (lajatfl)
					ino = 1;
				ut.kno = auto_lahtija(t/(1000/SEK));
			}
			ut.jono = ino;
			tall_rivi(ino, &ut, 0, 0, 0, 0, 0, false);
#else
			ut.piste = j;
			ut.jono = ino;
			tall_rivi(ino, &ut, 0, 0, 0, 0, 0);
#endif
			}

#endif
		return(0);
		}
#ifndef MAXOSUUSLUKU
	if (msgtp == 4 && eccode == 255) {
		tall_elahto(badge, t);
		return(0);
		}
#endif
#ifdef EMITHTTP
	if (r_no == eTParam.eTrno) {
		if (badge && t != TMAALI0 * (1000/SEK)) {
#ifndef MAXOSUUSLUKU
			if (lahdepistehaku && vainpiste[r_no+1] <= -3 && vainpiste[0] <= -3)
				r_no = NREGNLY + eccode - 1;
			tall_etulos(badge, 0, t + 43200000L, r_no, -1);
#else
			int lahde = 0;
			if (lahdepistehaku && vainpiste[r_no+1] <= -2 && vainpiste[0] <= -2)
				lahde = eccode;
			tall_etulos(badge, 0, t + 43200000L, r_no, lahde);
#endif
			}
		return(0);
		}
#endif
	if (emittall)
		em->time = t;
	if (nctrl > 0) {
		for (j = nctrl; j > 0; j--)
			if (controls[j].code > 0 && controls[j].code < 256)
				break;
		nctrl = j + 1;
		i = 0;
		for (; j > 0; j--) {
			if (controls[j].code < 256) {
				if (++i >= MAXNLEIMA)
					break;
				}
			}
		for (i = 0; i < MAXNLEIMA; i++, j++) {
			while (j < nctrl && controls[j].code >= 256)
				j++;
			if (j >= nctrl)
				break;
			em->ctrlcode[i] = (UCHAR) controls[j].code;
			if (controls[nctrl-1].time > 30000000)
				em->ctrltime[i] = (UINT16) (30000 + (controls[j].time - controls[nctrl-1].time)/1000);
			else
				em->ctrltime[i] = (UINT16) (controls[j].time/1000);
			if (IntInList(controls[i].code, L"240 243 244 250 253") >= 0)
				lukijaOn = true;
			}
		}
	if (!emittall && badge && t != TMAALI0 * (1000/SEK))
		add_bdg_t(badge, r_no, eccode, emittime*(t + 43200000L));
	return(lukijaOn ? 0 : 1);
	}
//#endif  // !LUENTA

int valo_on;

static void naytatulosTh(LPVOID lpCn)
   {
   DCB dcb;
   GetCommState(hComm[cn_regnly[NREGNLY-1]], &dcb);
	switch (*(int *)lpCn) {
		case 1:
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fDtrControl = DTR_CONTROL_ENABLE;
			break;
		case 3:
			dcb.fRtsControl = RTS_CONTROL_DISABLE;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			break;
		default:
			dcb.fRtsControl = RTS_CONTROL_ENABLE;
			dcb.fDtrControl = DTR_CONTROL_DISABLE;
			break;
	  }
   SetCommState(hComm[cn_regnly[NREGNLY-1]], &dcb);
   Sleep(valonaytto*100);
   dcb.fRtsControl = RTS_CONTROL_ENABLE;
   dcb.fDtrControl = DTR_CONTROL_ENABLE;
   SetCommState(hComm[cn_regnly[NREGNLY-1]], &dcb);
   valo_on = 0;
   }

void naytatulos(int tulos)
   {
   HTHREAD hValoThread;
	static int tls;

   if (!comopen[cn_regnly[NREGNLY-1]] || !valonaytto)
	  return;
   if (!valo_on) {
	  valo_on = 1;
		tls = tulos;
	  hValoThread = _beginthread(naytatulosTh, 20480, &tls);
	  }
   }

static void naytaluentaTh(LPVOID lpCn)
   {
	int i;
   vidspmsg(ySize-6, 5, 7, 0, "Viimeisin luettu kortti");
   vidint(ySize-6, 30, 7, *((int *) lpCn));
   viwrrect(ySize-9, 5, ySize-7, 40,
		"                                    "
		"                                    "
		"                                    ", 0, 7, 0);
   for (i = 0; i < 12; i++) {
	   Sleep(200);
	   viwrrect(ySize-9, 5, ySize-7, 8+3*i,
		"                                    "
		"                                    "
		"                                    ", 7, 0, 0);
	   }
   }
   
void naytaluenta(int badge)
   {
   HTHREAD hLuentaThread;
	static int bdg;

	bdg = badge;
	hLuentaThread = _beginthread(naytaluentaTh, 20480, &bdg);
   }
#ifndef LUENTA
static bool ohitaSirit(char *st)
{
	int len, laji;
	UINT32 maskval, stval;
	char buf[32] = "0x";

	if ((len = strlen(SiritMask) - 1) < 1 || (laji = stschind(SiritMask[0], "XZVP")) < 0)
		return(false);
	memcpy(buf, st, len);
	stval = strtoul(buf, 0, 16);
	maskval = strtoul(SiritMask+1, 0, 16);
	switch (laji) {
		case 0:
			return(maskval == stval);
		case 1:
			return((maskval & stval) != 0);
		case 2:
			return(maskval != stval);
		case 3:
			return((maskval & stval) == 0);
		}
	return(false);
}

#define NSIRITTAP 100

int siritaika(INT32 *t, san_type *vastaus, aikatp *ut, INT *jono, int r_no)
	{
	char *p, st[20];
//	UINT16 ant;
	UINT32 bdg;

	*t = 20L*TMAALI0;
	p = strstr(vastaus->bytes, "tag_id=0x");
	if (p) {
		p += 9;
		if (ohitaSirit(p))
			return(1);
		strncpy(st, p+19, 5);
		st[5] = 0;
		bdg = atoi(st);
		if (bdg == 0) {
			strncpy(st, p+8, 7);
			st[7] = 0;
			bdg = strtoul(st, 0, 16);
//			sscanf(st, "%x", &bdg);
			}
		ut->badge = bdg;
		p = strstr(vastaus->bytes, "first=");
		if (!p) {
			p = strstr(vastaus->bytes, "last=");
			if (p) {
#ifndef MAXOSUUSLUKU
				ut->tapahtuma = 1;
#endif				
				p--;
				}
			}
		if (p) {
			strncpy(st, p+17, 12);
			st[12] = 0;
			st[2] = 0;
			st[5] = 0;
			st[8] = 0;
			*t = 3600000L * atol(st) + 60000L * atol(st+3) + 1000L * atol(st+6) + atol(st+9)
				+ t0_regn[r_no];
			*t = NORMKELLO_A(10*(*t));
			p = strstr(vastaus->bytes, "antenna=");
#ifndef MAXOSUUSLUKU
			if (p) {
				ut->kanava = 10*(p[8]-'0');
				}
			if (siritreuna == 3 && ut->tapahtuma)
				*jono = aika_jono[r_no][1];
			else
				*jono = aika_jono[r_no][0];
#else
			if (p) {
				ut->lahde = 10*(p[8]-'0');
				}

			*jono = aika_jono[r_no][0];
#endif

			}
		}
	return(0);
	}
#endif // LUENTA

#ifdef LAJUNEN
#define CRC_PRESET 0xffff
#define CRC_POLYNOM 0x8408

static UINT16 crc(char *buf, int len)
{
	UINT16 crc16 = CRC_PRESET;
	int i, j;

	for (i = 0; i < len; i++)
		{
		crc16 ^= buf[i];
		for (j = 0; j < 8; j++)
			{
			if (crc16 & 0x0001)
				crc16 = (crc16 >> 1) ^ CRC_POLYNOM;
			else
				crc16 = (crc16 >> 1);
			}
		}
	return crc16;
}
#endif

static int lue_ARES(int r_no, int cn, san_type *vastaus, int *nmsg)
{
	char chin, *p2;
	int er = 0, nq;

	er = quesize_x(cn, &nq);
	chin = 0;
	while (!er && nq && *nmsg < (int)sizeof(san_type)) {
		er = read_ch_x(cn,&chin,&nq);
		vastaus->bytes[*nmsg] = chin;
		++*nmsg;
		bytecount = (bytecount + 1) % bytecountmax;
		}


	while (*nmsg > 0 && vastaus->bytes[0] != SOH)
		memmove(vastaus->bytes, vastaus->bytes+1, --*nmsg);
	while (*nmsg >= 41) {
		vastaus->bytes[*nmsg] = 0;
		if (vastaus->bytes[19] == EOT && vastaus->bytes[40] == EOT) {
			tall_regnly(vastaus, r_no);
			*nmsg -= 41;
			if (*nmsg)
				memmove(vastaus->bytes, vastaus->bytes+41, *nmsg);
			}


		else if ((int)strlen(vastaus->bytes) < *nmsg) {
			*nmsg -= strlen(vastaus->bytes)+1;
			memmove(vastaus->bytes, vastaus->bytes+strlen(vastaus->bytes)+1, *nmsg);
			}


		else if ((p2 = strstr(vastaus->bytes, "\x01")) != NULL) {
			*nmsg -= (p2-vastaus->bytes);
			memmove(vastaus->bytes, p2, *nmsg);
			}
		}

	return(0);
	}

#ifdef LAJUNEN
static void lue_Sirit(int r_no, san_type *vastaus, int *nmsg, int nch)
{
	if (nch > 0) {
		LatestSirit[r_no] = mstimer();
		if (SiritEiVastaa[r_no]) {
			SiritEiVastaa[r_no] = 0;
			paivita_aikanaytto();
			}
		SiritEiVastaa[r_no] = 0;
		}
	vastaus->bytes[*nmsg] = 0;
	while (*nmsg > 10 && memcmp(vastaus->bytes, "event.tag.", 10)) {
		if (siritloki) {
			siritbuf[sirithead] = (vastaus->bytes[0] >= ' ' ? ansitowchar(vastaus->bytes[0]) : '#');
			sirithead = (sirithead + 1) % SIRITBUFLEN;
			}
		memmove(vastaus->bytes, vastaus->bytes + 1, nmsg[0]--);
		}
	while (*nmsg > 10 && !memcmp(vastaus->bytes, "event.tag.", 10)) {
		char *p2;

		p2 = strstr(vastaus->bytes, "\r");
		if (p2 && p2 < vastaus->bytes + *nmsg) {
			san_type vast;

			p2++;
			if (siritloki) {
				for (int isr = 0; isr < *nmsg /* && isr < FGp + 11 - vastaus[r_no]->bytes */; isr++) {
					if (vastaus->bytes[isr] == '\r') {
						siritbuf[sirithead] = L'\n';
						sirithead = (sirithead + 1) % SIRITBUFLEN;
						}
					siritbuf[sirithead] = ansitowchar(vastaus->bytes[isr]);
					sirithead = (sirithead + 1) % SIRITBUFLEN;
					}
				}
			memcpy(vast.bytes, vastaus->bytes, p2 - vastaus->bytes);
			vast.bytes[p2 - vastaus->bytes] = 0;
			if (!memcmp(vastaus->bytes + 10, "arrive", 6) || !memcmp(vastaus->bytes + 10, "depart", 6)) {
				tall_regnly(&vast, r_no);
				}
			else {
				if (loki) {
					char ln[80];
					sprintf(ln, "Ohitettu merkit: %s", vast.bytes);
					}
				}
			while ((*p2 == '\r' || *p2 == '\n') && p2 < vastaus->bytes + *nmsg)
				p2++;
			memmove(vastaus->bytes, p2, vastaus->bytes + *nmsg - p2 + 1);
			*nmsg -= (p2 - vastaus->bytes);
			}
		while (*nmsg > 10 && memcmp(vastaus->bytes, "event.tag.", 10)) {
			if (siritloki) {
				siritbuf[sirithead] = ansitowchar(vastaus->bytes[0]);
				sirithead = (sirithead + 1) % SIRITBUFLEN;
				}
			memmove(vastaus->bytes, vastaus->bytes + 1, nmsg[0]--);
			}
		}
}

static int lue_FEIG(int r_no, int cn, san_type *vastaus, int *nmsg)
{
	int er = 0, nq, nch, l, ndata;
	char FEIGpyynto[10] = "\x02\x00\x09\xff\x22\x00\x01\x88\x77";
	char FEIGnollaus[10] = "\x02\x00\x07\xff\x32\x54\x47";
	char *FGp, FEIGbuf[200];

	i_flush_x(cn);
	er = wrt_st_x(cn,9,FEIGpyynto,&nch);
	Sleep(40);
	er = read_st_x(cn, sizeof(FEIGbuf), FEIGbuf, &nch, &nq);
#ifdef DBGFILE
	if (dbgfile[r_no] && nch) {
		if (fwrite(FEIGbuf, nch, 1, dbgfile[r_no]) < 1)
			dbgfile[r_no] = 0;
		}
#endif
	bytecount = (bytecount + nch) % bytecountmax;
	if (nch > 5) {
		if (FEIGbuf[0] == 2) {
			l = 256*FEIGbuf[1]+FEIGbuf[2];
			FGp = FEIGbuf+7;
			}
		else {
			l = FEIGbuf[0];
			FGp = FEIGbuf+5;
			}
		if (crc(FEIGbuf, l-2) == *(UINT16 *)(FEIGbuf+l-2)) {
			if ((ndata = 256*FGp[0] + FGp[1]) > 0) {
				FGp += 2;
				for (int i = 0; i < ndata; i++) {
					l = 256*FGp[0] + FGp[1];
					memcpy(vastaus, FGp, l);
					er = tall_regnly(vastaus, r_no);
					if (er)
						break;
					FGp += l;
					}
				if (!er) {
					Sleep(10);
					wrt_st_x(cn,9,FEIGnollaus,&nch);
					}
				}
			}
		}
	return(er);
}
#endif

static int lue_EmitKello(int r_no, int cn, san_type *vastaus, int *nmsg, int r_msg_len, int *od)
{
	char pyynto[8] = "?\r", kuittaus[3] = "+\r";
	char *msg = NULL;
	int er = 0, nq, nch;
	static INT32 tviim[NREGNLY];
	static UINT32 vahvistus[NREGNLY];

	er = read_st_x(cn, r_msg_len- *nmsg, vastaus->bytes + *nmsg, &nch, &nq);
#ifdef DBGFILE
	if (dbgfile[r_no] && nch) {
		if (fwrite(vastaus->bytes + *nmsg, nch, 1, dbgfile[r_no]) < 1)
			dbgfile[r_no] = 0;
		}
#endif
	*nmsg += nch;
	bytecount = (bytecount + nch) % bytecountmax;
	if (er || *nmsg == 0) {
		 msg = pyynto;
		 *od = 0;
		 }
	else if (*nmsg == 1) {
		++*od;
		}
	else {
		switch (vastaus->r1.tunnus) {
			case '-' :
				if (*nmsg >= 2 && vastaus->r1.cr1 == 13) {
					msg = kuittaus;
					}
				else {
					msg = pyynto;
					}
				break;
			case 'A' :
			case 'B' :
			case 'C' :
			case 'D' :
			case 'E' :
			case 'F' :
			case 'G' :
			case 'J' :
			case 'K' :
			case 'L' :
				if (*nmsg < r_msg_len) {
					for (int i = 0; i < *nmsg; i++) {
						if (vastaus->bytes[i] == 13) {
							msg = pyynto;
							break;
							}
						}
					++*od;
					}
				else {
					if (regnly[r_no] > 10)
						er = tall_emit(vastaus,	&vahvistus[r_no], r_no);
					else
						er = tall_regnly(vastaus, r_no);
					if (er)
						msg = pyynto;
					else
						msg = kuittaus;
					}
				break;
			default:
				msg = pyynto;
			}
		}
	if (*od > MAX_R_OD)
		msg = pyynto;
	if (kerro_regnly && msg == pyynto)
		msg = "T\r";
	if (msg) {
		*od = 0;
		*nmsg = 0;
		vastaus->bytes[0] = 0;
		i_flush_x(cn);
		if (msg != pyynto || mstimer() > tviim[r_no]+pyyntoviive) {
			wrt_st_x(cn,2,msg,&nch);
			tviim[r_no] = mstimer();
			}
		}
	return(0);
}


// Tämä lohko ottaa vastaan lukijarastilta tulevat tiedot. Jos muuttuja
// tyhjpuskuri on asetettu tai jos portilta tulee virhesanoma, tyhjennetään
// puskuri. Muussa tapauksessa luetaan merkejä puskuriin, johon mahtuu
// reilut neljä sanomaa. Uudet merkit luetaan aina osoittimen p[r_no]
// osoitteesta alkavaan alueeseen.

static int lue_LUKIJA(int r_no, int cn, san_type *vastaus, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen, int r_msg_len)
{
	char chin, *p2;
	int t, dt, er = 0, nq, nread, nmsg0, siirra, varm_ok;
	static INT32 viimaika[NREGNLY];
	static INT32 viimTCPaika[NREGNLY];
	static INT32 tviim[NREGNLY];
	static UINT32 vahvistus[NREGNLY];

	t = biostime(0, 0);
	if (!viimaika[r_no])
		viimaika[r_no] = t - 20;
	if (viimaika[r_no] > t)
		viimaika[r_no] -= DAYTICKS;
	*tyhjpuskuri = 0;
#ifdef DBG
	if (!dbgf)
		dbgf = _wfopen(L"DBGFILE.TXT", L"wt");
#endif
	er = quesize_x(cn, &nq);
	if ((pollaa || TCPpoll[r_no]) && (comtype[cn] & comtpTCP)) {
		t = mstimer();
		if (nq && !pollaa)
			viimTCPaika[r_no] = t;
		else {
			if (pollaa || t > viimTCPaika[r_no] + TCPpoll[r_no]) {
				viimTCPaika[r_no] = t;
				pollaa = 0;
				wrt_ch_x(cn,NAK);
				}
			}
		}
	chin = 0;
	if (nq + *nmsg != *ntotviim) {
		tviim[r_no] = t;
		*ntotviim = nq + *nmsg;
		}
	else {
		if ((*nmsg + nq) &&
			((dt = (t + DAYTICKS - tviim[r_no]) % DAYTICKS) > 10
			&& dt < DAYTICKS/2))
			{
			*tyhjpuskuri = 1;
			}
		}
	if (!er && nq && *nmsg < r_buflen &&
		((kaikki_ajat[r_no+1] == 2 && nq + *nmsg >= 10) ||	*tyhjpuskuri ||
		nq + *nmsg >= lukutoisto*r_msg_len ||
		(kaikki_ajat[r_no+1] && nq + *nmsg >= r_msg_len))) {

		er = read_st_x(cn, r_buflen-*nmsg, vastaus->bytes + *nmsg, &nread, &nq);

#ifdef DBGFILE
		if (dbgfile[r_no] && nread) {
			if (fwrite(vastaus->bytes + *nmsg, nread, 1, dbgfile[r_no]) < 1)
				dbgfile[r_no] = 0;
			}
#endif
		*nmsg += nread;
		*ntotviim = nq + *nmsg;
		bytecount = (bytecount + nread) % bytecountmax;
		}
	if (er) {
#ifdef DBG
		fwprintf(dbgf, L"Tyhjennys, tyhjpuskuri = %d, *nmsg = %d\n",
			*tyhjpuskuri, *nmsg);
#endif
		*nmsg = 0;
		}
	else if (*nmsg >= 10 || *tyhjpuskuri) {

// Jos merkkejä on vastaanotettu, tarkastetaan, onko kyseessä virheettömän
// sanoman alku. Tarkastus koskee 2 tai 10 merkkiä tai koko sanomaa. Jos sanoma
// ei ala oikein, poistetaan alusta merkkejä yksi kerrallaan, kunnes sanoman
// rakenne on vaatimusten mukainen
// Lisäksi poistetaan sanomat, joiden badge-arvo on sama kuin edellisellä
// tallennetulla sanomalla

		do {
			nmsg0 = *nmsg;
			while ((*nmsg > 2 && vastaus->r12.alku != 0x2020) ||
				(*nmsg >= 10 && (
					r_tarksum(vastaus->bytes+2, 8, 0, 1) ||
					vahvistus[r_no] ==
						(((*(UINT32 *) vastaus->r12.badge) & 0xffffffL)	^ 0xdfdfdfL)
					)
				)) {
					if (vahvistus[r_no] ==
						(((*(UINT32 *) vastaus->r12.badge) & 0xffffffL)	^ 0xdfdfdfL))
						viimaika[r_no] = t;
					for (p2 = vastaus->bytes+1;	p2 < vastaus->bytes + *nmsg - 1; p2++)
						{
						if (*(INT16 *)p2 == 0x2020)
							break;
						}
					if (p2 == vastaus->bytes + *nmsg - 1) {
						if (*p2 != '\x20')
							p2++;
						}
					*nmsg -= (INT) (p2 - vastaus->bytes);
					if (*nmsg)
						memmove(vastaus->bytes, p2, *nmsg);
					}
				siirra = 0;
				if (*nmsg >= 10 &&
					(kaikki_ajat[r_no+1] == 2 || t - viimaika[r_no] > 6)) {
					add_bdg_t(((*(UINT32 *)vastaus->r12.badge) & 0xffffffL)	^ 0xdfdfdfL,
						r_no, vastaus->r12.fill2 ^ 0xdf, 0);
					if (kaikki_ajat[r_no+1] == 2)
						siirra = 9;
					viimaika[r_no] = t;
					}
				if (siirra || (*nmsg >= r_msg_len &&
					r_tarksum(vastaus->bytes, r_msg_len, 0, 1))) {
					memmove(vastaus->bytes, vastaus->bytes+siirra+1, *nmsg-siirra-1);
					*nmsg -= siirra+1;
					}
				} while (*nmsg < nmsg0);
			if (*tyhjpuskuri && *nmsg < 2*r_msg_len) {
				*nmsg = 0;
				}
			if (kaikki_ajat[r_no+1] < 2) {
				while (*nmsg >= lukutoisto * r_msg_len ||
				(kaikki_ajat[r_no+1] && *nmsg >= r_msg_len)) {

// Tähän tullaan vain, jos 1. sanoman rakenne on oikea. Arvo badge on tällöin
// kahden tarkistussumman varmentama ja siten miltei varmasti oikea.
// Luetut tiedot käsitellään vasta, kun puskuriin on tallennettu vähintään
// lukutoisto (1 tai 2) sanoman sisältö.
// Jos kaikki_ajat == TRUE hyväksytään jo yksi sanoma.
// Muuttuja vahvistus sisältää viimeisen tallennetun badge -arvon.
// Tallentaminen edellyttää, että puskurissa on kaksi identtistä sanomaa
// peräkkäin, jolloin molemmat sanomat poistetaan tallentamisen jälkeen.
// Muussa tapauksessa poistetaan yksi sanoma.

					varm_ok = (lukutoisto == 1);
					if (lukutoisto > 1 && !emitcmp(vastaus->bytes,
						vastaus->bytes+r_msg_len,
						r_msg_len)) {
						*nmsg -= r_msg_len;
						memmove(vastaus->bytes,	vastaus->bytes+r_msg_len, *nmsg);
						varm_ok = 1;
						}
					if (varm_ok || kaikki_ajat[r_no+1]) {
						vahvistus[r_no] =
							((*(UINT32 *) vastaus->r12.badge) & 0xffffffL)
							^ 0xdfdfdfL ;
						viimaika[r_no] = t;
						if (kaikki_ajat[r_no+1] < 2)
						tall_emit(vastaus, NULL, r_no);
#ifdef DBG
					 fwprintf(dbgf, L"Tallennettu, nmsg = %d\n", *nmsg);
#endif
					 while (*nmsg >= r_msg_len &&
						vahvistus[r_no] ==
						   (((*(UINT32 *) vastaus->r12.badge) & 0xffffffL)
						   ^ 0xdfdfdfL)) {
						*nmsg -= r_msg_len;
						memmove(vastaus, vastaus->bytes+r_msg_len, *nmsg);
						}
					 break;
					 }
				  else {
#ifdef DBG
					 fwprintf(dbgf, L"Ero sanomissa, nmsg = %d\n", *nmsg);
#endif
					 *nmsg -= r_msg_len;
					 memmove(vastaus, vastaus->bytes+r_msg_len,	*nmsg);
					 }
				}
			}
		}
	return(0);
}

static int lue_MTR(int r_no, int cn, san_type *vastaus, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen, int r_msg_len)
{
	char chin, *p2, pyynto[20];
	int t, dt, er = 0, nq, nch, nread, nmsg0;
	static INT32 tviim[NREGNLY];
	static UINT32 vahvistus[NREGNLY];

	t = biostime(0, 0);
	*tyhjpuskuri = 0;

// Tästä alkaa MTR-laitetta vastaava koodi
// MTR-laite tarkastaa kortilta luettujen tietojen tarkistussummat.
// Tästä johtuen virheellisiä tietoja tulee yleensä vain, kun summa
// on sattumalta oikein. Useimmissa tapauksissa on viimeinen samaa korttia
// vastaava tieto oikea. Kaksi peräkkäistä tietuetta eroavat aina toisistaan,
// joten niitä ei kannata verrata toisiinsa.

	er = quesize_x(cn, &nq);
	if (nq + *nmsg != *ntotviim)
		tviim[r_no] = t;
	else {
		if ((*nmsg + nq) &&	((dt = (t + DAYTICKS - tviim[r_no]) % DAYTICKS) > 10
			&& dt < DAYTICKS/2))
			{
			*tyhjpuskuri = 1;
			}
		}
	chin = 0;
	if (!*tyhjpuskuri && !er && nq &&
		nq + *nmsg >= r_msg_len && *nmsg < r_buflen) {
		er = read_st_x(cn, r_buflen-*nmsg, vastaus->bytes + *nmsg, &nread, &nq);
#ifdef DBGFILE
		if (dbgfile[r_no] && nread) {
			if (fwrite(vastaus->bytes + *nmsg, nread, 1, dbgfile[r_no]) < 1)
			dbgfile[r_no] = 0;
			}
#endif
#ifdef DBG
		if (!debugfile)
			debugfile = _wfopen(L"DBGFILE", L"wb");
		for (p2 = vastaus->bytes + *nmsg; p2 < vastaus->bytes + *nmsg + nread; p2++)
			fputwc(*p2, debugfile);
#endif
		*nmsg += nread;
		bytecount = (bytecount + nread) % bytecountmax;
		*ntotviim = nq + *nmsg;
		}
	nmsg0 = *nmsg;
	if (er || *tyhjpuskuri) {
		*nmsg = 0;
#ifdef DBGEMIT
		writeerror_w(L"Tyhjennä puskuri", 400);
#endif
		}
	else {

// Etsitään oikeaa sanoman alkua ja tarkistussummaa ohittamalla merkkejä
// yksi kerrallaan

		while ((*nmsg >= 4 && vastaus->r13.alku != 0xffffffffL)
			||
			(*nmsg >= 6 &&
				(vastaus->r13.msglen != r_msg_len - 4 ||
				vastaus->r13.tunnus != 'M'))
			||
			(*nmsg >= r_msg_len-1 &&
				(er = r_tarksum((char *) vastaus->bytes, r_msg_len-2,
			vastaus->r13.chksum2, 0)) != 0)) {
#ifdef DBGEMIT
			swprintf(erline, L"%d %d %d %2x %2x %2x %2x %2x %2x", *nmsg,
				er, (int)vastaus->r13.chksum2,
				(int)vastaus->bytes[0], (int)vastaus->bytes[1],
				(int)vastaus->bytes[2], (int)vastaus->bytes[3],
				(int)vastaus->bytes[4], (int)vastaus->bytes[5]);
			writeerror_w(erline, 0);
#endif
			memmove(vastaus->bytes, vastaus->bytes+1, --*nmsg);
			}
		while (*nmsg >= r_msg_len) {

// Tarkastetaan vielä, että uusi sanoma ei ala käsiteltävän keskeltä. Jos
// alkaa siirretään alku ko. pisteeseen ja poistutaan loopista.

			for (p2 = vastaus->bytes+r_msg_len-1; p2 > vastaus->bytes; p2--)
				{
				if (*(UINT32 *) p2 == 0xffffffffL)
					break;
				}
			if (p2 > vastaus->bytes) {
				*nmsg -= (INT)(p2 - vastaus->bytes);
				memmove(vastaus->bytes, p2, *nmsg);
#ifdef DBGEMIT
				writeerror_w(L"Poisto 2", 400);
#endif
				continue;
				}

// Jos puskurissa on jo samaa korttia vastaava uusi sanoma, ohitetaan
// vuorossa oleva sanoma, kasvatetaan pkg_alku[r_no] ja poistutaan loopista.

			if (*nmsg >= r_msg_len + 24 &&
				!memcmp(vastaus->bytes,	vastaus->bytes+r_msg_len, 8) &&
				!memcmp(vastaus->r13.badge,	vastaus->r13.badge+r_msg_len, 3))
				{
				*nmsg -= r_msg_len;
				memmove(vastaus->bytes,	vastaus->bytes+r_msg_len, *nmsg);
				if (vastaus->r13.pkgno == pkg_alku[r_no]+1)
					pkg_alku[r_no]++;
#ifdef DBGEMIT
				writeerror_w(L"Poisto 3", 400);
#endif
				continue;
				}

// Vain vuorossa oleva sanoma hyväksytään, mutta uudemman sanoman olemassa olo
// rekisteröidään uudelleen pyytämisen varmistamiseksi.
// Kun kaikki halutut sanomat on luettu, osoittaa pkg_loppu[r_no] viimeiseen luettuun
// sanomaan, muuten viimeiseen, joka halutaan vielä lukea.
// pkg_alku[r_no] osoittaa seuraavaan luettavaan sanomaan.

			if (vastaus->r13.pkgno <= pkg_alku[r_no]) {
				tall_emit(vastaus, &vahvistus[r_no], r_no);
#ifdef DBGEMIT
				writeerror_w(L"Tallennus", 400);
#endif
				pkg_alku[r_no] = vastaus->r13.pkgno;
				if (pkg_loppu[r_no] < pkg_alku[r_no])
					pkg_loppu[r_no] = pkg_alku[r_no];
				pkg_alku[r_no]++;
				}
			else {
				if (pkg_loppu[r_no] < vastaus->r13.pkgno)
					pkg_loppu[r_no] = vastaus->r13.pkgno;
				}
			*nmsg -= r_msg_len;
			memmove(vastaus->bytes,	vastaus->bytes+r_msg_len, *nmsg);
			}
		}

// Jos lukeminen perustuu käyttäjän tekemään pyyntöön, ilmoitetaan
// lukemisen valmistumisesta.

	if (mtr_pyynto[r_no] && (int) pkg_alku[r_no] > mtr_pyynto[r_no]) {
		mtr_pyynto[r_no] = 0;
		writeerror_w(L"Kaikki MTR-laitteelta pyydetyt tiedot luettu - odota hetki", 2000, true);
		}

// Jos puskuria on käsitelty ja pkg_alku[r_no] ja pkg_loppu[r_no] osoittavat, että
// MTR-laitteesta pitäisi vielä lukea tieto, lähetetään yhden sanoman pyyntö.
// Laskuria pkg_alku[r_no] kasvatetaan vasta, kun vastaava tieto on käsitelty.

	if (*nmsg < nmsg0 && pkg_alku[r_no] <= pkg_loppu[r_no] && !mtr_yksi) {
#ifdef DBGEMIT
		writeerror_w(L"Pyyntö", 400);
#endif
		memcpy(pyynto, "/GB", 3);
		*(INT32 *)(pyynto+3) = pkg_alku[r_no];
		quesize_x(cn, &nq);
		for (;;) {
			Sleep(20);
			quesize_x(cn, &nch);
			if (nch == nq)
				break;
			nq = nch;
			}
		wrt_st_x(cn,7,pyynto,&nch);
		}
	return(0);
}

static int lue_EMITAG(int r_no, int cn, san_type *vastaus, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen)
{
	char chin, *p2;
	int t, dt, er = 0, nq, nread;
	static INT32 tviim[NREGNLY];

// Tämä lohko ottaa vastaan nilkka-anturin tiedot.

	t = biostime(0, 0);
	*tyhjpuskuri = 0;

#ifdef DBG
	 if (!dbgf) dbgf = _wfopen(L"DBGFILE.TXT", L"wt");
#endif
	 er = quesize_x(cn, &nq);
	 chin = 0;
	 if (nq + *nmsg != *ntotviim) {
		tviim[r_no] = t;
		*ntotviim = nq + *nmsg;
		}
	 else {
		if ((*nmsg + nq) &&
			((dt = (t + DAYTICKS - tviim[r_no]) % DAYTICKS) > 10
			&& dt < DAYTICKS/2))
			{
			*tyhjpuskuri = 1;
			}
		}
	if (!er && nq && *nmsg < r_buflen) {

		er = read_st_x(cn, r_buflen-*nmsg, vastaus->bytes + *nmsg, &nread, &nq);

#ifdef DBGFILE
		if (dbgfile[r_no] && nread) {
			if (fwrite(vastaus->bytes + *nmsg, nread, 1, dbgfile[r_no]) < 1)
				dbgfile[r_no] = 0;
			}
#endif
		*nmsg += nread;
		bytecount = (bytecount + nread) % bytecountmax;
		*ntotviim = nq + *nmsg;
		}
	if (er) {
#ifdef DBG
		fwprintf(dbgf, L"Tyhjennys, tyhjpuskuri = %d, *nmsg = %d\n",
			*tyhjpuskuri, *nmsg);
#endif
		*nmsg = 0;
		}
	else {
		for (p2 = vastaus->bytes; p2 < vastaus->bytes + *nmsg; p2++) {
			if (*p2 == STX)
				break;
			}
		if (p2 > vastaus->bytes && p2 < vastaus->bytes + *nmsg) {
			*nmsg -= (INT) (p2 - vastaus->bytes);
			memmove(vastaus->bytes, p2, *nmsg);
			}
		for (p2 = vastaus->bytes; p2 < vastaus->bytes + *nmsg; p2++) {
			if (*p2 == ETX)
				break;
			}
		if (p2 > vastaus->bytes && p2 < vastaus->bytes + *nmsg) {
			if (p2 - vastaus->bytes > 15) {
				*p2 = 0;
				tall_emit(vastaus, NULL, r_no);
				}
			*nmsg -= (INT) (p2 + 1 - vastaus->bytes);
			memmove(vastaus->bytes, p2 + 1, *nmsg);
			}
		}
	return(0);
}


static int lue_CPower(int r_no, int cn, san_type *vastaus, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen)
{
	*nmsg = 0;

	return(0);
}

static int lue_Rtnm(int r_no, int cn, san_type *vastaus, int *nmsg,
	int *ntotviim, int *tyhjpuskuri, int r_buflen)
{
	*nmsg = 0;

	return(0);
}

#ifdef SPORTIDENT
static int lue_SI(int r_no, int cn, san_type *vastaus, int *nmsg,
	int r_buflen, int r_msg_len)
{
	char chin, *p2;
	int t, dt, er = 0, nq, nch, l, ndata, nread, nmsg0, siirra, varm_ok;
	char SI5pyynto[5] = "\002\002\061\003";
	char SI6pyynto[6] = "\002\002\141\008\003";
	char SI5code[5] = "\002FI\003";
	char SIack = ACK;
	char SIbuf[512], *SIbp;
	int SItype, SIdatalen[2] = {133, 402}, dle = 0;
	INT32 SIt;

	t = biostime(0, 0);

	msg = 0;
	er = read_st_x(cn, r_msg_len- *nmsg, vastaus->bytes + *nmsg, &nch,&nq);
	*nmsg += nch;
	bytecount = (bytecount + nch) % bytecountmax;
	if (er || *nmsg == 0) {
		od[r_no] = 0;
		}
	else {
		while ((vastaus->r21.stx != STX || vastaus->r21.tunnus == STX) && *nmsg > 1) {
			memmove(&vastaus->r21.stx, &vastaus->r21.tunnus, *nmsg-1);
			--*nmsg;
			}
		if (*nmsg < 4 || (vastaus->r21.tunnus == 102 && *nmsg < 10))
			od[r_no]++;
		else {
			if (!memcmp(&vastaus->r21.stx, SI5code, 4)) {
				msg = SI5pyynto;
				SItype = 5;
				}
			else if (vastaus->r21.tunnus == 102 && vastaus->r21.etx == ETX) {
				msg = SI6pyynto;
				SItype = 6;
				}
			od[r_no] = 0;
			*nmsg = 0;
			vastaus->bytes[0] = 0;
			}
		}
	if (msg) {
		SIt = biostime(0,0);
		od[r_no] = 0;
		*nmsg = 0;
		vastaus->bytes[0] = 0;
		i_flush_x(cn);
		wrt_st_x(cn,strlen(msg),msg,&nch);
		utsleep(2);
		SIbp = SIbuf;
		for(;;) {
			nq = 0;
			if (!read_ch_x(cn, &chin, &nq)) {
				bytecount = (bytecount + 1) % bytecountmax;
				if (dle) {
					*(SIbp++) = chin;
					dle = 0;
					}
				else {
					if (chin == 16) {
						dle = 1;
						}
					else {
						if (chin > 31 || chin == 2 || chin == 3) {
							*(SIbp++) = chin;
							}
						else
							break;
						}
					}
				}
			l = SIbp - SIbuf;
			if (l > 100) {
				dle = 2*dle;
				}
			if (l == SIdatalen[SItype-5]) {
				if (!tulkSI(SIbuf, vastaus, SIt, SItype)) {
					tall_emit(vastaus, 0, r_no);
					}
				break;
				}
			if ((biostime(0,0) + DAYTICKS - SIt) % DAYTICKS > 90)
				break;
			if (!nq)
				utsleep(1);
			}
		}
	return(0);
}
#endif

static int emitcmp(char *buf1, char *buf2, int len) {
	int i, c = 0, d;

	if (!memcmp(buf1, buf2, len))
		return(0);
	d = (int)buf1[len-1] - (int)buf2[len-1];
	if (d == -1 || d == 255) {
		for (i = 0; i < len-1; i++) {
			d = (int)buf1[i] - (int)buf2[i];
			if (d) {
				if (c > 0)
					return(1);
				if (d != 1 && d != -255)
					return(1);
				c = 1;
				}
			}
		return(0);
		}
	return(1);
	}

int lue_regnly(INT r_no)
	{
	char chin, *msg = NULL;
	static INT in_lue_regnly[NREGNLY];
	static INT od[NREGNLY],nmsg[NREGNLY];
	INT  nq,nch = 0,er,r_buflen;
	static san_type *vastaus[NREGNLY];
	static INT32 t_raja[NREGNLY];
	static INT ntotviim[NREGNLY];
	static INT tyhjpuskuri[NREGNLY];
	static int siritstate;
#ifdef DBGEMIT
	char erline[80];
#endif
#ifdef DBG
	static FILE *debugfile, *dbgf;
#endif

	if (!regnly_open[r_no])
		return(-1);
#ifdef LAJUNEN
	if (regnly[r_no] == LID_SIRIT) {
		if (!siritopen[r_no]) {
#ifdef _CONSOLE
			if (ySize == 50)
				vidspwmsg(45, 72, 0, 7, L"SULJETTU");
#endif
			return(0);
			}
		}
#endif
	if (!vastaus[r_no]) {
		if ((vastaus[r_no] = (san_type *) malloc(sizeof(san_type))) == 0) {
			writeerror_w(L"Muisti ei riitä sanomapuskureille", 0);
			return(-1);
			}
		}
	if (in_aseta_mtr == r_no + 1)
		return(0);

	if (in_lue_regnly[r_no])
		return(0);

	in_lue_regnly[r_no] = 1;

	if (nmsg[r_no] == 0) {
		tyhjpuskuri[r_no] = 0;
		ntotviim[r_no] = 0;
		}
	msg = NULL;
	if (regnly[r_no] < 12)
		r_buflen = r_msg_len[r_no];
	else
		r_buflen = R_BUFLEN;

	if (nmsg[r_no] > r_buflen || nmsg[r_no] < 0 || biostime(0,0) > t_raja[r_no]) {
		nmsg[r_no] = 0;
		nch = 0;
		od[r_no] = 0;
		t_raja[r_no] = 3*DAYTICKS;
		}

	if (regnly[r_no] == LID_ARES) {									// ARES

		lue_ARES(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no);

		in_lue_regnly[r_no] = 0;
		return(0);
		}														// ARES loppu
	else if (IntInList(regnly[r_no], L"3 4 5 6 7 8 9 19 28 30 33 40") >= 0) {
		er = quesize_x(cn_regnly[r_no], &nq);
#ifdef LAJUNENxx
		if (regnly[r_no] == LID_SIRIT && nq == 0) {
			int tt;
			tt = mstimer();
			if (LatestSirit[r_no] > tt - 5000)
				LatestSiritPoll[r_no] = 0;
			if (SiritPoll[r_no] && LatestSiritPoll[r_no] && (tt = mstimer()) >
				LatestSirit[r_no] + SiritPoll[r_no] + 10000 && tt > LatestSiritPoll[r_no] + 5000) {
#ifdef _CONSOLE
				LatestSiritPoll[r_no] = 0;
				LatestSirit[r_no] += 10000;
				if (LatestSirit[r_no] < tt - SiritPoll[r_no] + 5000)
					LatestSirit[r_no] = tt - SiritPoll[r_no] + 5000;
				writeerror_w(L"Sirit ei vastaa", 3000);
#else
				if (SiritEiVastaa[r_no] == 0) {
					SiritEiVastaa[r_no] = tt - LatestSirit[r_no];
					paivita_aikanaytto();
					}
				else
					SiritEiVastaa[r_no] = tt - LatestSirit[r_no];
#endif
				}
			}
#endif
		chin = 0;

		while (!er && nq && nmsg[r_no] < (int)sizeof(san_type) &&
			chin != '\r' &&
			!(chin == '\n' && IntInList(regnly[r_no], L"7 9 28 30") >= 0)
			) {
			er = read_ch_x(cn_regnly[r_no],&chin,&nq);
			if (regnly[r_no] == LID_CPOWER)
				continue;					// C-Powerin vastauksia ei käsitellä
			if (chin == 0 && IntInList(regnly[r_no], L"3 4 5 6 7 8 9 19 28 30 33 36 40") >= 0) {
				continue;
				}
//			if (regnly[r_no] == LID_SIRIT && !chin) {
//				continue;
//				}
#ifdef DBGFILE
			if (chin && dbgfile[r_no]) {
				if (fputc(chin, dbgfile[r_no]) == WEOF)
					dbgfile[r_no] = 0;
				}
#endif
			if (regnly[r_no] == LID_ASC &&
				(chin == '\x11' || chin == '\x13' ||
				(!nmsg[r_no] && (chin == '\r' || chin == '\n')))
				) {
				chin = 0;
				continue;
				}
			vastaus[r_no]->bytes[nmsg[r_no]] = chin;
			nmsg[r_no]++;
			nch++;
			bytecount = (bytecount + 1) % bytecountmax;
			}
		if (er || (nmsg[r_no] > 28 && regnly[r_no] != LID_TIMY && regnly[r_no] != LID_SIRIT && regnly[r_no] != LID_IMPINJ)
			|| (nmsg[r_no] > 33 && regnly[r_no] == LID_TIMY) || nmsg[r_no] > 200) {
			nmsg[r_no] = 0;
			}
		while (nmsg[r_no] &&
			(vastaus[r_no]->bytes[0] == '\r' || vastaus[r_no]->bytes[0] == '\n'))
			{
			nmsg[r_no]--;
			if (nmsg[r_no])
				memmove(vastaus[r_no]->bytes, vastaus[r_no]->bytes+1, nmsg[r_no]);
			}

		// Seuraavassa käsitellään Algen laitteet, IMPINJ, SAKKO_YHT			ALGE, COMET, TIMY, IMPINJ, SAKKO_YHT, ym.

		if (IntInList(regnly[r_no], L"3 4 5 6 7 8 9 19 28 33 36 40") >= 0) {
//	  if (wcswcind((wchar_t)regnly[r_no], L"\x03\x04\x05\x06\x07\x08\x09\x13\x1c\x21\x24\x28") >= 0)
			if (nmsg[r_no] && (chin == '\r' || chin == '\n')) {
#ifdef MAKI
				if (regnly[r_no] == LID_TUOMARI) {
					if (nmsg[r_no] == 22) {
						tall_maki((char *) vastaus[r_no]);
						}
					}
				else
#endif
				if (regnly[r_no] == LID_KURVINEN) {
					tall_ampsakko((char *)vastaus[r_no], r_no);
					}
				else if ((regnly[r_no] < 10 && nmsg[r_no] >= 22)
					|| (regnly[r_no] == LID_ALGE && nmsg[r_no] >= 20)
					|| (regnly[r_no] == LID_ASC && nmsg[r_no] > 16)
					|| (regnly[r_no] == LID_RADIOEMIT && nmsg[r_no] > 13)
//					|| (regnly[r_no] == LID_SIRIT && nmsg[r_no] > 22)
					|| (regnly[r_no] == LID_IMPINJ && nmsg[r_no] > 30)
					|| (regnly[r_no] == LID_SW2000 && nmsg[r_no] > 5)
					|| (regnly[r_no] == LID_ALGETUTKA && nmsg[r_no] > 15)
					) {
					if (nmsg[r_no] == 20 && regnly[r_no] == LID_ALGE)
						memmove(&vastaus[r_no]->r3.bl, &vastaus[r_no]->r3.z3, 14);
					tall_regnly(vastaus[r_no], r_no);
					}
				nmsg[r_no] = 0;
				memset(vastaus[r_no], ' ', sizeof(san_type));
				}
			}
		if (nmsg[r_no]) {
			if (t_raja[r_no] > 2*DAYTICKS)
				t_raja[r_no] = biostime(0,0L) + 10;
			}
		else
			t_raja[r_no] = 3*DAYTICKS;
		}

	  // Seuraavassa käsitellään Regnly / Emitin kellot

	if (regnly[r_no] == LID_RTR || regnly[r_no] == LID_RTR2 || regnly[r_no] == LID_EMITKELLO) {

	   lue_EmitKello(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no, r_msg_len[r_no], od+r_no);

	   in_lue_regnly[r_no] = 0;
	   return(nmsg[r_no]);
	   }
#ifdef LAJUNEN
	else if (regnly[r_no] == LID_FEIG) {								// FEIG
		lue_FEIG(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no);
		}
   else if (regnly[r_no] == LID_SIRIT) {									// SIRIT
		lue_Sirit(r_no, vastaus[r_no], nmsg + r_no, nch);
		}
#endif // LAJUNEN

	else if (regnly[r_no] == LID_LUKIJA) {											// LUKIJA
		lue_LUKIJA(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			ntotviim+r_no, tyhjpuskuri+r_no, r_buflen, r_msg_len[r_no]);
		}
	else if (regnly[r_no] == LID_MTR) {											// MTR
		lue_MTR(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			ntotviim+r_no, tyhjpuskuri+r_no, r_buflen, r_msg_len[r_no]);
		}
	else if (regnly[r_no] == LID_EMITAG) {    				  						// EMITAG
		lue_EMITAG(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			ntotviim+r_no, tyhjpuskuri+r_no, r_buflen);
		}
	else if (regnly[r_no] == LID_CPOWER) {    				  						// C-Power
		lue_CPower(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			ntotviim+r_no, tyhjpuskuri+r_no, r_buflen);
		}
	else if (regnly[r_no] == LID_RTNM) {    				  						// D-RTNM
		lue_Rtnm(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			ntotviim+r_no, tyhjpuskuri+r_no, r_buflen);
		}
#ifdef SPORTIDENT
	else if (regnly[r_no] == LID_SPORTIDENT) {											// SPORTIDENT
		lue_SI(r_no, cn_regnly[r_no], vastaus[r_no], nmsg+r_no,
			r_buflen, r_msg_len[r_no]);
		}
#endif // SPORTIDENT
   in_lue_regnly[r_no] = 0;
   return(nmsg[r_no]);
   }

void lue_regnlyThread(LPVOID lpCn)
{
	int cn, r_no;
	int t_edell = 0;
	int t_lah = 0;
	static bool SiritPollAvoin[NREGNLY];

	cn = *(int *)lpCn;
	r_no = cn - MAX_LAHPORTTI;
	if (comtype[cn] == comtpRS) {
		opencomRS(cn);
		}

	else if (comtype[cn] & comtpUDP)
			openlukijaUDP(cn);

	else if (comtype[cn] & comtpTCP) {
		while (comfl && !inLopetus) {
#ifdef LAJUNEN
			if (regnly[r_no] == LID_SIRIT) {
				if (openSirit2(r_no, false)) {
					regnly_open[r_no] = 0;
					regnlystarted[r_no] = 0;
					break;
					}
				}
			else
#endif
				{
				if (openlukijaTCP(cn)) {
					regnly_open[r_no] = 0;
					regnlystarted[r_no] = 0;
					break;
					}
				}
			if (comopen[cn] || comclose[cn])
				break;
			for (int i = 0; i < 10; i++) {
				if (!comfl || inLopetus)
					break;
				Sleep(500);
				}
			}
		}
	if (!comopen[cn])
		return;
	regnly_open[r_no] = 1;
	while (comfl && regnly_open[r_no] && !inLopetus) {

#ifdef LAJUNEN
		if (regnly[r_no] == LID_SIRIT) {
			t_lah = mstimer();
			if (reconnectSirit[r_no] || !TCPyht_on(hComm[cn_regnly[r_no]])) {
				for (int i = 0; i < 10; i++) {
					if (!comfl || inLopetus)
						break;
					Sleep(500);
					}
				if (!comfl || inLopetus)
					break;
				if (!openSirit2(r_no, true))
					regnly_open[r_no] = 0;
				reconnectSirit[r_no] = 0;
				}
			else {
				if (SiritPollAvoin[r_no] && t_lah > LatestSiritPoll[r_no] + 2000) {
					lue_SiritCmd(r_no);
					SiritPollAvoin[r_no] = false;
					}
				if (SiritPoll[r_no] && t_lah > LatestSirit[r_no] + SiritPoll[r_no] &&
					t_lah > LatestSiritPoll[r_no] + SiritPoll[r_no]) {
					SiritSync(false);
					LatestSiritPoll[r_no] = t_lah;
					SiritPollAvoin[r_no] = true;
					}
				lue_regnly(r_no);
				Sleep(50);
				}
			}
		else
#endif
			{
			if ((comtype[cn] & comtpTCP) && !TCPyht_on(hComm[cn_regnly[r_no]])) {
				for (int i = 0; i < 10; i++) {
					if (!comfl || inLopetus)
						break;
					Sleep(500);
					}
				if (!comfl || inLopetus)
					break;
				reconnectTCP(hComm[cn_regnly[r_no]]);
				}
			else {
				lue_regnly(r_no);
				Sleep(20);
				}
			}
		}
	regnly_open[r_no] = 0;
	if (!inLopetus && !katkaisuPyydetty)
		writeerror_w(L"Häiriö. Ajanoton/leimantarkastuksen jatko vaatii uudelleenkäynnistyksen", 0);
}

INT start_regnly(INT r_no)
   {
	   char parity = 'n';
#ifdef SPORTIDENT
	   INT nw;
#endif
	   INT er = 0, bd = 0, sbits = 0, bits = 8;
	   wchar_t wch, msg[80];
	   if (regnly[r_no] == LID_ETGPRS || regnly[r_no] == LID_ETSQL)
		   return(0);
#ifdef SIIMPORT
	   if (regnly[r_no] == LID_SIIMPORT) {
		   viwrrectw(ySize - 3, 0, ySize - 3, 31,
			   L"Anna maalikellon käynnistysaika:", 7, 0, 0);
		   INPUTAIKAW(&t0_regnly, t0, 8, 50, ySize - 3, L"\xD", &wch);
		   maaliajat[9] = t0_regnly;
		   return(0);
	   }
#endif
	   if (inLopetus || regnlystarted[r_no] && (ToimintaTila == 2 || regnly_open[r_no]))
		   return(0);
	   regnlystarted[r_no] = 1;
	   if (regnly[r_no] >= LID_ALGE && regnly[r_no] <= LID_ALGETUTKA) {
		   if (regnly[r_no] == LID_ALGE4) {
			   strcpy(alge_tunn[0], "SZ ");
			   strcpy(alge_tunn[1], "K01");
			   strcpy(alge_tunn[2], "K04");
			   strcpy(alge_tunn[3], "K07");
		   }
		   od_regnly = 1;
	   }
	   else if (regnly[r_no] >= LID_LUKIJA || regnly[r_no] <= LID_EMITAG) {
#ifdef EMITFILE
		   if (port_regnly[r_no] > 900) {
			   r_no_file = r_no;
			   writeerror_w(L"Ohjelma lukee EMIT-tietoja tiedostosta näppäimellä "
				   L"N valinnassa lE)imat", 2000);
			   return(0);
			   }
#endif
		   emitluenta = true;
		   od_regnly = 1;
		   }
	   else if (regnly[r_no] == LID_SPORTIDENT) {
		   od_regnly = 1;
		   }
	   if (!er) {
		   //      if (regnly[r_no] != 12 && regnly[r_no] != 21) {
		   if (regnly[r_no] == LID_MTR) {
			   t0_regn[r_no] = -12 * TUNTI;
			   }
		   if ((regnly[r_no] < LID_LUKIJA) || regnly[r_no] == LID_ARES ||
				regnly[r_no] == LID_SIRIT || regnly[r_no] == LID_FEIG ||
				regnly[r_no] == LID_IMPINJ || regnly[r_no] == LID_SW2000) {
			   t0_regn[r_no] = maaliajat[9];
			   clrln(ySize - 3);
#ifdef TUTKA
			   if (regnly[r_no] == LID_ALGETUTKA)
				   to_regn[r_no] = 0;
			   else
#endif
			   if (aikaero[r_no] != TMAALI0) {
				   t0_regn[r_no] = ((24 * TUNTI + aikaero[r_no]) % (24 * TUNTI) - t0 * TUNTI);
				   }
			   else {
				   if (regnly[r_no] == LID_MTR)
					   wcscpy(msg, L"Anna poikkeama: kilpailuaika - MTR-laitteen aika :");
				   else
				   if (regnly[r_no] <= LID_RTR2)
					   wcscpy(msg, L"Anna maalikellon käynnistysaika:");
				   else
					   wcscpy(msg, L"Anna poikkeama: kilpailuaika - maalikellon aika :");
#ifdef _CONSOLE
				   vidspwmsg(ySize - 3, 0, 7, 0, msg);
				   INPUTAIKAW(&t0_regn[r_no], t0, 8, 50, ySize - 3, L"\xD", &wch);
#else
				   inputaika_prompt(&t0_regn[r_no], t0, 8, msg, &wch);
#endif
				   }
			   maaliajat[9] = t0_regn[r_no];

			   // Jos maalikello näyttää valtakunnanaikaa, on t0_regn[r_no] = -12*TUNTI

			   }
		   switch (regnly[r_no]) {
		   case LID_RTR:
			   bd = 4;          //  1200
			   r_msg_len[r_no] = 19;
			   break;
		   case LID_RTR2:
			   bd = 4;          //  1200
			   r_msg_len[r_no] = 24;
			   break;
		   case LID_ALGE:
			   bd = 5;          //  2400
			   sbits = 1;
			   bits = 7;
			   r_msg_len[r_no] = sizeof(san_type);
			   break;
		   case LID_COMET:
			   bd = 5;          //  2400
			   r_msg_len[r_no] = 22;
			   break;
		   case LID_ALGE4:
			   bd = 6;          //  4800
			   r_msg_len[r_no] = sizeof(san_type);
			   break;
		   case LID_TIMY:
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 27;
			   break;
		   case LID_TDC:
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 27;
			   break;
		   case LID_ASC:
			   bd = 5;          //  2400
			   r_msg_len[r_no] = 19;
			   break;
#ifdef MAKI
		   case LID_TUOMARI:
			   bd = 5;          //  2400
			   r_msg_len[r_no] = 22;
			   if (ajanottofl < 0)
				   ajanottofl = 0;
			   break;
#endif
#ifdef TUTKA
		   case LID_ALGETUTKA:
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 21;
			   break;
#endif
		   case LID_EMITKELLO:
			   bd = 6;          //  4800
			   r_msg_len[r_no] = 462;
			   break;
		   case LID_LUKIJA:
			   if (usb_regnly[r_no])
				   bd = 11;          //  115200
			   else
				   bd = 7;          //  9600
			   sbits = 1;
			   r_msg_len[r_no] = 217;
			   break;
		   case LID_MTR:
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 234;
			   pkg_alku[r_no] = 0xffffffffL;
			   break;
		   case LID_EMITAG:
			   if (usb_regnly[r_no] == 1)
				   bd = 11;          //  115200
			   else if (usb_regnly[r_no] == 2)
				   bd = 8;          //  19200
			   else
				   bd = 7;          //  9600
			   r_msg_len[r_no] = R_BUFLEN;
			   break;
		   case LID_SPORTIDENT:
			   bd = 6;          //  4800
			   r_msg_len[r_no] = 10;
			   break;
		   case LID_ARES:
			   bd = 7;          //  9600
			   bits = kello_bits;
			   parity = bits == 8 ? 'n' : 'e';
			   r_msg_len[r_no] = 41;
			   break;
		   case LID_CPOWER:				// C-Power
			   bd = 11;          //  11520
			   r_msg_len[r_no] = 100;
			   break;
		   case LID_RTNM:				// D-RTNM
				ipparam[cn_regnly[r_no]].iptype = ipUDPBOTH;
				ipparam[cn_regnly[r_no]].srvport = ipparam[cn_regnly[r_no]].destport + 10;
			   r_msg_len[r_no] = 100;
			   break;
		   case LID_RADIOEMIT:           //  RADIOEMIT
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 20;
			   parity = 'n';
			   break;
#ifdef LAJUNEN
		   case LID_SIRIT:           //   SIRIT
			   r_msg_len[r_no] = 600;
			   ipparam[MAX_LAHPORTTI + r_no].destport = SiritEventPort[r_no];	// 50008;
			   ipparam[MAX_LAHPORTTI + r_no + NREGNLY] = ipparam[MAX_LAHPORTTI + r_no];
			   ipparam[MAX_LAHPORTTI + r_no + NREGNLY].destport = SiritCmdPort[r_no];	// 50007;
			   break;
		   case LID_FEIG:           //  FEIG
			   bd = 9;          //  38400
			   r_msg_len[r_no] = 29;
			   parity = 'e';
			   break;
		   case LID_IMPINJ:           //  IMPINJ
			   r_msg_len[r_no] = 600;
			   break;
		   case LID_VALOT:           //  VALO
			   bd = 7;          //  9600
			   r_msg_len[r_no] = 10;
			   parity = 'n';
			   break;
#endif
#ifdef SWIM2000
		   case LID_SW2000:
			   bd = 5;          //  2400
			   r_msg_len[r_no] = 26;
			   break;
#endif
		   case LID_KURVINEN:           //  Kurvinen
			   bd = sak_baud;
			   r_msg_len[r_no] = (sak_laji ? 15 : 9) + 1;
			   bits = sak_laji ? 8 : 7;
			   parity = 'n';
			   break;
		   }
		   if (kello_baud && (regnly[r_no] < 10 || regnly[r_no] == LID_ARES || regnly[r_no] == LID_FEIG)) {
			   bd = kello_baud;
		   }
	   }
	if (inLopetus)
		return(0);
	if (comtype[cn_regnly[r_no]] == comtpRS) {
	   portti[cn_regnly[r_no]] = port_regnly[r_no];
	   }
	   //   if (regnly[r_no] != 33)
//	   regnly_open[r_no] = 1;
#ifdef LAJUNEN
	   if (regnly[r_no] == LID_SIRIT)
		   openSirit(r_no);
	   else
#endif
		if (regnly[r_no] == LID_RTNM) {
			regnly_open[r_no] = !openportUDP(hComm+cn_regnly[r_no], ipparam+cn_regnly[r_no]);
			}
		else
		   opencom(cn_regnly[r_no], bd, parity, bits, sbits + 1);
	   if (comopen[cn_regnly[r_no]]) {

#ifdef SPORTIDENT
		   if (regnly[r_no] == LID_SPORTIDENT) {
			   wrt_st_x(cn_regnly[r_no], 4, L"\002\002\161\003", &nw);
		   }
#endif
	   }
	   return(0);
   }

void rem_regnly(INT r_no)
{
	regnly_open[r_no] = 0;
	Sleep(300);
	if (regnly[r_no])
		closeport_x(cn_regnly[r_no]);
	if (regnly[r_no] == LID_SIRIT) {
		closeport_x(cn_regnly[r_no + NREGNLY]);
		}
//	regnly[r_no] = 0;
	regnlystarted[r_no] = 0;
}

#ifdef LAJUNEN

void lue_SiritCmd(int r_no)
{
	char buf[2000] = "";
	int nread, que;


	if (TCPyht_on(hComm[cn_regnly[r_no + NREGNLY]])) {
		read_st_x(cn_regnly[r_no+NREGNLY], 1999, buf, &nread, &que);
		if (nread > 2) {
			if (siritloki)
				kirjloki(buf);
			LatestSirit[r_no] = mstimer();
			if (SiritEiVastaa[r_no]) {
				SiritEiVastaa[r_no] = 0;
				paivita_aikanaytto();
				}
			SiritEiVastaa[r_no] = 0;
			}
		else {
			int tt = mstimer();
			if (SiritEiVastaa[r_no] == 0) {
				SiritEiVastaa[r_no] = tt;
				paivita_aikanaytto();
				}
			SiritEiVastaa[r_no] = tt;
			}
		}
}

void openSirit(int r_no)
	{
	static int acn;

	acn = MAX_LAHPORTTI+r_no;
	comfl = 1;

	hLuepakThread[cn_regnly[r_no]] = _beginthread(lue_regnlyThread, 40960, &acn);
	}

void SiritSync(bool set_time)
{
	int r_no, nread;
	char line[40];
	SYSTEMTIME stm;
	Sleep(200);
	for (r_no = 0; r_no < NREGNLY; r_no++)
		if (regnly[r_no] == LID_SIRIT)
			break;
	if (regnly[r_no] != LID_SIRIT)
		return;
	if (set_time) {
		GetLocalTime(&stm);
		sprintf(line, "info.time=%04d-%02d-%02dT%02d:%02d:%02d.%03d\r\n",
			stm.wYear, stm.wMonth, stm.wDay, stm.wHour, stm.wMinute, stm.wSecond, stm.wMilliseconds);
		}
	else {
		strcpy(line, "info.time\r\n");
		}
	wrt_st_x(cn_regnly[r_no+NREGNLY], strlen(line), line, &nread);
}

int openSirit2(int r_no, bool reconnect)
	{
	char line[1000], *p;
	wchar_t wline[200];
	int nread, que, i, yht = 0;

	comtype[cn_regnly[r_no+NREGNLY]] = comtpTCP;
	ipparam[MAX_LAHPORTTI + r_no].noreconnect = 1;
	ipparam[MAX_LAHPORTTI + r_no + NREGNLY].noreconnect = 1;
	if (loki) {
		swprintf(wline, L"%d: Sirit-käynnistys aloitettu", mstimer());
		wkirjloki(wline);
		}
	if (!TCPyht_on(hComm[cn_regnly[r_no + NREGNLY]])) {
		if (reconnect) {
			closeportTCP(hComm + cn_regnly[r_no]);
			closeportTCP(hComm + cn_regnly[r_no + NREGNLY]);
//			reconnectTCP(hComm[cn_regnly[r_no + NREGNLY]]);
			}
//		else
			openportTCP(hComm+cn_regnly[r_no+NREGNLY], ipparam + MAX_LAHPORTTI + r_no + NREGNLY);
		for (i = 0; i < 5; i++) {
			if (TCPyht_on(hComm[cn_regnly[r_no+NREGNLY]]))
				break;
			Sleep(500);
			}
		if (i >= 5) {
			aikaTCPstatus = -(r_no+1);
			paivita_aikanaytto();
			return(0);
			}
		if (loki) {
			swprintf(wline, L"%d: TCP-portti %d avattu", mstimer(), ipparam[MAX_LAHPORTTI + r_no + NREGNLY].destport);
			wkirjloki(wline);
			}
		}
	if (reconnect)
		openportTCP(&hComm[cn_regnly[r_no]], ipparam + MAX_LAHPORTTI + r_no);
//		reconnectTCP(hComm[cn_regnly[r_no]]);
	else {
		if (openlukijaTCP(cn_regnly[r_no]))
			return(1);
		}
	for (i = 0; i < 5; i++) {
		if (TCPyht_on(hComm[cn_regnly[r_no]]))
			break;
		Sleep(500);
		}
	if (i >= 5)
		return(0);
	aikaTCPstatus = r_no+1;
	paivita_aikanaytto();
	if (loki) {
		swprintf(wline, L"%d: TCP-portti %d avattu", mstimer(), ipparam[MAX_LAHPORTTI + r_no].destport);
		wkirjloki(wline);
		}
	Sleep(200);
	memset(line, 0, sizeof(line));
	for (i = 0; i < 2; i++) {
		read_st_x(cn_regnly[r_no], 999-strlen(line), line+strlen(line), &nread, &que);
		if (siritloki) {
			kirjloki(line);
			}
		bytecount = (bytecount + nread) % bytecountmax;
		if ((p = strstr(line, "event.connection id = ")) != NULL && strlen(p+22) > 1) {
			yht = atoi(p+22);
			if (loki) {
				swprintf(wline, L"%d: Sirit-yhteys %d avattu", mstimer(), yht);
				wkirjloki(wline);
				}
			if (yht > 10 && yht < 99) {
			   comopen[cn_regnly[r_no]] = 1;
				break;
				}
			yht = 0;
			}
		Sleep(500);
		}
	if (yht) {
	  siritopen[r_no] = 1;
	  LatestSirit[r_no] = mstimer();
//		wcscpy(line, L"setup.operating_mode autonomous\r\n");
//		wrt_st_x(cn_regnly[r_no+NREGNLY], wcslen(line), line, &nread);
		if (siritreuna & 2) {
			strcpy(line, "tag.reporting.arrive_fields=tag_id time antenna\r\n");
			wrt_st_x(cn_regnly[r_no+NREGNLY], strlen(line), line, &nread);
			sprintf(line, "reader.register_event(%d, %s)\r\n", yht, siritarrive);
			wrt_st_x(cn_regnly[r_no+NREGNLY], strlen(line), line, &nread);
			}
		if (siritreuna & 1) {
			strcpy(line, "tag.reporting.depart_fields=tag_id time antenna\r\n");
			wrt_st_x(cn_regnly[r_no+NREGNLY], strlen(line), line, &nread);
			Sleep(300);
			sprintf(line, "reader.register_event(%d, %s)\r\n", yht, siritdepart);
			wrt_st_x(cn_regnly[r_no+NREGNLY], strlen(line), line, &nread);
			}
		if (siritsync) {
			SiritSync(true);
			siritsync = false;
			}
//      regnly[r_no] = LID_SIRIT;
		writeerrorOn  = 50;
		vidspwmsg(ySize - 1, 50, 7, 0, L"Sirit-yhteys avattu");
		if (loki) {
			swprintf(wline, L"%d: Sirit-yhteysparametrit asetettu", mstimer());
			wkirjloki(wline);
			}
		Sleep(2000);
		}
	else {
		if (loki) {
			swprintf(wline, L"%d: Sirit-yhteyden avaaminen ei onnistu", mstimer());
			wkirjloki(wline);
			}
		writeerror_w(L"Sirit-yhteyden avaaminen ei onnistu", 2000);
		}
	return(0);
	}
#endif

int ProcessSpecialKey(INPUT_RECORD *Buf)
   {
   int retval = 0;
   UCHAR ch, ch2;
   static UCHAR OEMtoISO[256] =
	{  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
	  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
	  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
	  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
	  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
	  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
	  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
	  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
	  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
	 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
	 120, 121, 122, 123, 124, 125, 126, 127, 199, 252,
	 233, 226, 228, 224, 134, 135, 136, 235, 232, 239,
	 238, 236, 196, 197, 201, 230, 198, 244, 246, 242,
	 251, 249, 253, 214, 220, 155, 156, 157, 158, 159,
	 225, 237, 243, 250, 241, 209, 166, 167, 168, 169,
	 170, 171, 172, 173, 174, 175, 176, 177, 178, 179,
	 180, 181, 182, 183, 184, 185, 186, 187, 188, 189,
	 190, 191, 192, 193, 194, 195, 196, 197, 198, 199,
	 200, 201, 202, 203, 204, 205, 206, 207, 208, 209,
	 210, 211, 212, 213, 214, 215, 216, 217, 218, 219,
	 220, 221, 222, 223, 224, 223, 226, 227, 228, 229,
	 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
	 240, 241, 242, 243, 244, 245, 246, 247, 248, 249,
	 250, 251, 252, 253, 254, 255};

   if (Buf->EventType == KEY_EVENT) {
      ch = (char) Buf->Event.KeyEvent.uChar.AsciiChar;
  	   ch2 = (char) Buf->Event.KeyEvent.wVirtualScanCode;
      if (ch2 >= 59 && ch < 69 && 
         (Buf->Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED|RIGHT_CTRL_PRESSED)))
         ch2 += 35;
	  ch = OEMtoISO[ch];
	  if (Buf->Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED) {
		  switch (ch) {
			  case 196:
				  ch = 198;
				  break;
			  case 'C':
				  ch = 199;
				  break;
			  case 214:
				  ch = 216;
				  break;
			  case 228:
				  ch = 230;
				  break;
			  case 'c':
				  ch = 231;
				  break;
			  case 246:
				  ch = 248;
				  break;
			}
		}
      retval = kbdiv1(ch, ch2);
	  Buf->Event.KeyEvent.uChar.AsciiChar = ch;
      }
   return(retval);
   }

#if !defined(NOCOM_AIKA)
void comajanotto(LPVOID lpCn)
	{
	int err;
	HANDLE hAC;
	DWORD EvtMask, ModemStat;
	DCB dcb;
	static long ted;

	err = openport(&hAC, *(int *)lpCn, 7, 'N', 8, 1, 0);
	if (!err) {
		GetCommState(hAC, &dcb);
//      BuildCommDCB(L"baud=9600 parity=N data=8 stop=1", &dcb);
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fOutxCtsFlow = 0;
		SetCommState(hAC, &dcb);
		GetCommState(hAC, &dcb);
		SetCommMask(hAC, EV_CTS);
		while(taustaon) {
			if (WaitCommEvent(hAC, &EvtMask, 0)) {
				if (EvtMask & EV_CTS) {
					if (GetCommModemStatus(hAC, &ModemStat) && (ModemStat & MS_CTS_ON)) {
						if (mstimer() - ted > 50*aika_esto)
							tee_aika(1);
						ted = mstimer();
						}
					}
				}
			}
		closeport(&hAC);
		}
	}
#endif

#ifdef SPORTIDENT
static int tulkSI(char *buf, san_type *vastaus, INT32 SIt, int SItype)
	{
	SI5tp *tp5;
	SI6tp *tp6;
	int r, i;

	memset(vastaus, 0, sizeof(vastaus->r21data));
	switch (SItype) {
		case 5:
			tp5 = (SI5tp *) buf;
			vastaus->r21data.badge = 256L * tp5->CN[0] + tp5->CN[1] +
				(tp5->CNS > 1 ? tp5->CNS * 100000L : 0);
			vastaus->r21data.lukija = t_time_l(SIt, t0);
			vastaus->r21data.start = 256L * tp5->ST[0] + tp5->ST[1];
			vastaus->r21data.check = 256L * tp5->CT[0] + tp5->CT[1];
			vastaus->r21data.finish = 256L * tp5->FT[0] + tp5->FT[1];
			for (r = 0; r < 6; r++) {
				vastaus->r21data.cc[31+r] = tp5->row[r].ccx;
				for (i = 0; i < 5; i++) {
					vastaus->r21data.cc[1+i+5*r] = tp5->row[r].c[i].cc;
					vastaus->r21data.ct[1+i+5*r] =
						256L*tp5->row[r].c[i].ct[0] + tp5->row[r].c[i].ct[1];
					if (r+i == 0) {
						if (vastaus->r21data.start != 61166L && vastaus->r21data.ct[1] &&
							vastaus->r21data.ct[1] < vastaus->r21data.start)
							vastaus->r21data.ct[1] += 43200L;
						}
					else {
						if (vastaus->r21data.ct[1+i+5*r] &&
							vastaus->r21data.ct[1+i+5*r] < vastaus->r21data.ct[i+5*r])
							vastaus->r21data.ct[1+i+5*r] += 43200L;
						}
					}
				}
			break;
		case 6:
			tp6 = (SI6tp *) buf;
			vastaus->r21data.badge =
				tp6->CN[3] + 256L * (tp6->CN[2] + 256L * (tp6->CN[1] + 256L * tp6->CN[0]));
			vastaus->r21data.lukija = t_time_l(SIt, t0);
			vastaus->r21data.start =
					256L*tp6->st.PT[0] + tp6->st.PT[1] +
					(tp6->st.PTD & 1) * 43200L;
			vastaus->r21data.check =
					256L*tp6->chk.PT[0] + tp6->chk.PT[1] +
					(tp6->chk.PTD & 1) * 43200L;
			vastaus->r21data.finish = 256L * tp5->FT[0] + tp5->FT[1];
					256L*tp6->fi.PT[0] + tp6->fi.PT[1] +
					(tp6->fi.PTD & 1) * 43200L;
			for (r = 0; r < 2; r++) {
				for (i = 0; i < 32; i++) {
					vastaus->r21data.cc[1+i] = tp6->pblk[r].punch[i].CN;
					vastaus->r21data.ct[1+i] =
						256L*tp6->pblk[r].punch[i].PT[0] + tp6->pblk[r].punch[i].PT[1] +
						(tp6->pblk[r].punch[i].PTD & 1) * 43200L;
					}
				}
			break;
		}
	return(0);
	}
#endif
   
#ifdef _CONSOLE
void aseta_emitag(int r_no)
{
	wchar_t ch = L' ', ch2;
	char *p, st[30], as[20];
	INT i, key;
	wchar_t msg[120], was[40], wch;
	char che, che2;

	if (r_no >= NREGNLY || regnly[r_no] != LID_EMITAG)
		return;
	//   in_aseta_mtr = r_no + 1;
	clearframe63();
	kbflush();
	for (;;) {
		clrln(ySize - 3);
		vidspwmsg(ySize - 3, 0, 7, 0, L"K)ellon synkronointi, aseta M)oodi, aseman kO)odi, U)udelleenläh., Esc: Poistu");
		swprintf(msg, L"Laiteversio: %s, toimintamoodi: %d    ", ansitowcs(was, ecdata[r_no].devtype, 39), ecdata[r_no].mode);
		if (ecdata[r_no].mode != 0)
			vidspwmsg(6, 15, 7, 0, L"Toiminta edellyttää, että moodi = 0");
		vidspwmsg(5, 5, 7, 0, msg);
		swprintf(msg, L"Aseman koodi: %d  Tallennetut sanomat %d - %d     ",
			ecdata[r_no].code, ecdata[r_no].first, ecdata[r_no].next - 1);
		vidspwmsg(7, 5, 7, 0, msg);
		swprintf(msg, L"Laitteen kellonaika         %s   ", AIKATOWSTRS(was, ecdata[r_no].devtime, t0));
		vidspwmsg(9, 5, 7, 0, msg);
		swprintf(msg, L"Vastaanotettu tietokoneelle %s   ", AIKATOWSTRS(was, ecdata[r_no].pctime, t0));
		vidspwmsg(10, 5, 7, 0, msg);
		if (ecdata[r_no].health[0]) {
			swprintf(msg, L"Pariston tila      %s   ", ansitowcs(was, ecdata[r_no].health, 39));
			vidspwmsg(12, 5, 7, 0, msg);
		}
		if (ecdata[r_no].comm[0]) {
			swprintf(msg, L"Tiedonsiirron tila %s   ", ansitowcs(was, ecdata[r_no].health, 39));
			vidspwmsg(14, 5, 7, 0, msg);
		}
		key = 0;
		ch = 0;
		if (kbready(&che, &key)) {
			readkbd(&che2, 0, 0);
			ch2 = che2;
			st[0] = 0;
			ch = ansitowchar(toupper(che));
			switch (ch) {
			case L'K':
				sprintf(st, "/SC%8.8s\r\n/ST\r\n", aikatostr_ls(as, t_time_l(biostime(0, 0), t0), t0));
				break;
			case L'M':
				wch = L'0' + ecdata[r_no].mode;
				wselectopt(L"Anna moodin numero", L"0123456789", &wch);
				if (wch != L'0') {
					ch2 = L' ';
					wselectopt(L"Haluatko varmasti asettaa ohjelman toimivuuden estävän moodin?", L"KE", &ch2);
					if (ch2 != L'K')
						break;
				}
				che = (char)wch;
				sprintf(st, "/PP%c\r\n/ST\r\n", che);
				break;
			case L'O':
				i = ecdata[r_no].code;
				for (;;) {
					clrln(ySize - 3);
					vidspwmsg(ySize - 3, 0, 7, 0, L"Anna aseman koodi: ");
					inputlongw(&i, 3, 20, ySize - 3, L"\r\x1b", &ch);
					if (i < 0 || (i > 240 && i != 243 && i != 244 && i != 248 && i != 250 && i != 253)) {
						writeerror_w(L"Koodi ei sallittu", 0, true);
						continue;
					}
					if (i == 0)
						wcscpy(msg, L"Lähtönollauksen koodi, vahvista (K/E)");
					else if (i < 66)
						swprintf(msg, L"Koodi %d - Alle 66 oleva koodi kieltää tiedonsiirron, vahvista (K/E)", i);
					else if (i < 240)
						swprintf(msg, L"Koodi %d - Asema lähettää tietoja, vahvista (K/E)", i);
					else if (i < 244)
						swprintf(msg, L"Koodi %d - Tietojen luenta, 5 viim. päivää, vahvista (K/E)", i);
					else if (i == 244)
						swprintf(msg, L"Koodi %d - Tietojen luenta, kaikki kortin tiedot, vahvista (K/E)", i);
					else if (i == 248)
						wcscpy(msg, L"Maaliajanoton koodi, vahvista (K/E)");
					else if (i < 254)
						swprintf(msg, L"Koodi %d - Tietojen luenta, viimeisin kilpailu, vahvista (K/E)", i);
					ch = L' ';
					wselectopt(msg, L"KE", &ch);
					if (ch == L'K')
						break;
				}
				if (ch != ESC) {
					sprintf(st, "/CD%d\r\n/ST\r\n", i);
				}
				break;
			case L'U':
				ch = L' ';
				wselectopt(L"P)äivän sanomat, sanomat N)umerosta, Y)ksi sanoma, K)aikki sanomat, Esc: peru", L"PNYK\x1b", &ch);
				if (ch == L'N' || ch == L'Y') {
					i = ecdata[r_no].next - 1;
					clrln(ySize - 3);
					vidspwmsg(ySize - 3, 0, 7, 0, L"Anna sanoman numero: ");
					inputlongw(&i, 3, 23, ySize - 3, L"\r\x1b", &ch2);
					if (ch2 == ESC)
						break;
					sprintf(st, "/Q%c%d\r\n", ch == L'N' ? 'F' : 'C', i);
				}
				else if (ch == L'P' || ch == L'K')
					sprintf(st, "/Q%c\r\n", ch == L'P' ? 'M' : 'D');
				if (st[0]) {
					ch = L' ';
					wselectopt(L"Vahvista uudelleenlähetyspyyntö (K/E)", L"KE", &ch);
					if (ch != L'K')
						st[0] = 0;
				}
				break;
			case L'0':
				ch = L' ';
				wselectopt(L"Haluatko varmasti tyhjentää aseman muistin (K/E)", L"KE", &ch);
				if (ch == L'K') {
					sprintf(st, "/CL\r\n/ST\r\n");
				}
				break;
			}
			if (strlen(st) > 0) {
				for (p = st; *p; p++) {
					wrt_ch_x(cn_regnly[r_no], *p);
					Sleep(*p == '\n' ? 100 : 5);
				}
			}
		}
		if (ch == ESC)
			break;
		Sleep(100);
	}
	clearframe63();
	//	in_aseta_mtr = 0;
}

void aseta_mtr(void)
{
	wchar_t ch = L' ', msg[120], as[20], ist[40];
	char san[40];
	INT i, j, nq, nch, r_no = NREGNLY - 1;
	INT32 ed_alku, luku, c_time;
	INT32 e_time;
	time_t et_time;
	san_type vastaus, vast1;
	struct tm *now;

	j = 0;
	for (i = 0; i < NREGNLY; i++)
	if (regnly[i] == LID_MTR || regnly[i] == LID_EMITAG) {
		j++;
		r_no = i;
	}
	if (j > 1) {
		vidspwmsg(ySize - 3, 0, 7, 0, L"Valitse MTR:n tai emiTag-aseman numero");
		r_no++;
		INPUTINTW(&r_no, 2, 39, ySize - 3, L"\r", &ch);
		r_no--;
		if (r_no < 0 || r_no >= NREGNLY)
			r_no = 0;
	}
	if (regnly[r_no] == LID_EMITAG) {
		aseta_emitag(r_no);
		return;
	}
	if (r_no >= NREGNLY || regnly[r_no] != LID_MTR)
		return;
	in_aseta_mtr = r_no + 1;
	clearframe63();
	clrln(ySize - 3);
	i_flush_x(cn_regnly[r_no]);
	wrt_st_x(cn_regnly[r_no], 3, "/ST", &nch);
	c_time = t_time_l(biostime(0, 0L), t0);
	for (i = 0; i < 5; i++) {
		utsleep(3);
		quesize_x(cn_regnly[r_no], &nq);
		if (nq >= MTR_ST_LEN) break;
#ifdef DBGFILE
		if (dbgfile[r_no]) {
			if (fwprintf(dbgfile[r_no], L"Uusintapyynto 1/%d\n\r", i + 1) < 10)
				dbgfile[r_no] = 0;
		}
#endif
	}
	read_st_x(cn_regnly[r_no], MTR_ST_LEN, vastaus.bytes, &nch, &nq);
	bytecount = (bytecount + nch) % bytecountmax;
#ifdef DBGFILE
	if (dbgfile[r_no]) {
		vastaus.bytes[nch] = 0;
		if (fputs(vastaus.bytes, dbgfile[r_no]) < 0)
			dbgfile[r_no] = 0;
	}
#endif
	i_flush_x(cn_regnly[r_no]);
	if (nch < MTR_ST_LEN || vastaus.r13st.alku != 0xffffffffL ||
		vastaus.r13st.msglen != MTR_ST_LEN - 4 || vastaus.r13st.tunnus != 'S' ||
		r_tarksum(vastaus.bytes, MTR_ST_LEN - 2, vastaus.r13st.chksum2, 0)) {
		writeerror_w(L"MTR:n tilatietojen lukeminen epäonnistui", 0);
		clearframe63();
		in_aseta_mtr = 0;
		return;
	}
	e_time = vastaus.r13st.t_ms / 100 + 10L * vastaus.r13st.t_sek +
		600L * vastaus.r13st.t_min + 36000L * vastaus.r13st.t_hour + t0_regn[r_no] / KSEK;
	vidspwmsg(2, 1, 7, 0, L"MTR-laite no");
	vidint(2, 17, 0, vastaus.r13st.mtrid);
	swprintf(msg, L"MTR-laitteen ilmoittama aika %02d.%02d.%02d  %s",
		vastaus.r13st.t_day, vastaus.r13st.t_month, vastaus.r13st.t_year,
		aikatowstr_ls(as, e_time, t0));
	vidspwmsg(4, 1, 7, 0, msg);
	e_time = (e_time - c_time) / 10;
	swprintf(msg, L"Ero tietokoneen kellosta %ld sek", e_time);
	vidspwmsg(5, 1, 7, 0, msg);
	swprintf(msg, L"Pariston tila %s", vastaus.r13st.battery ? L"HEIKKO" : L"OK");
	vidspwmsg(7, 1, 7, 0, msg);
	if (!vastaus.r13st.pkgno)
		vidspwmsg(8, 1, 7, 0, L"Laitteessa ei tietoja");
	else {
		vidspwmsg(8, 1, 7, 0, L"Laitteen muistissa olevat istunnot");
		vidspwmsg(9, 1, 7, 0, L"Istunto   aloituspvm ja -aika  vanhin nro  uusin nro  lukumäärä");
		ed_alku = vastaus.r13st.pkgno + 1;
		for (i = 0; i < 8; i++) {
			if (i == 0) wcscpy(ist, L"viimeisin");
			else swprintf(ist, L"       -%d", i);
			if (vastaus.r13st.s_start[i] >= vastaus.r13st.oldestpkgno &&
				vastaus.r13st.s_start[i] <= vastaus.r13st.pkgno) {
				utsleep(2);
				wrt_st_x(cn_regnly[r_no], 3, "/GB", &nch);
				wrt_st_x(cn_regnly[r_no], 4, (char *)(vastaus.r13st.s_start + i),
					&nch);
				for (j = 0; j < 10; j++) {
					utsleep(2);
					quesize_x(cn_regnly[r_no], &nq);
					if (nq >= r_msg_len[r_no]) break;
#ifdef DBGFILE
					if (dbgfile[r_no]) {
						if (fwprintf(dbgfile[r_no], L"Uusintapyynto %d/%d\n\r", i + 1, j + 1) < 10)
							dbgfile[r_no] = 0;
					}
#endif
				}
				read_st_x(cn_regnly[r_no], r_msg_len[r_no], vast1.bytes, &nch, &nq);
				bytecount = (bytecount + nch) % bytecountmax;
#ifdef DBGFILE
				if (dbgfile[r_no]) {
					vast1.bytes[nch] = 0;
					if (fputs(vast1.bytes, dbgfile[r_no]) < 0)
						dbgfile[r_no] = 0;
				}
#endif
				i_flush_x(cn_regnly[r_no]);
				if (nch < r_msg_len[r_no] || vast1.r13.alku != 0xffffffffL ||
					vast1.r13.msglen != r_msg_len[r_no] - 4 || vast1.r13.tunnus != 'M' ||
					r_tarksum(vast1.bytes, r_msg_len[r_no] - 2, vast1.r13.chksum2, 0)) {
					swprintf(msg, L"%s  Tilatietojen lukeminen epäonnistui", ist);
				}
				else {
					luku = ed_alku - vastaus.r13st.s_start[i];
					swprintf(msg, L"%s  %2.2d.%2.2d.%2.2d %2.2d.%2.2d.%2.2d %8ld%12ld%10ld",
						ist, vast1.r13.t_day, vast1.r13.t_month, vast1.r13.t_year,
						vast1.r13.t_hour, vast1.r13.t_min, vast1.r13.t_sek,
						vastaus.r13st.s_start[i], ed_alku - 1, luku);
				}
				ed_alku = vastaus.r13st.s_start[i];
			}
			else {
				swprintf(msg, L"%s  Ei tietoja", ist);
			}
			vidspwmsg(11 + i, 1, 7, 0, msg);
		}
		swprintf(msg, L"Kaikkiaan  %26ld%12ld%10ld",
			vastaus.r13st.oldestpkgno, vastaus.r13st.pkgno,
			vastaus.r13st.pkgno - vastaus.r13st.oldestpkgno + 1);
		vidspwmsg(ySize - 5, 1, 7, 0, msg);
	}
	ch = L' ';
	wselectopt(L"L)ue sanomia, siirrä tietokoneen A)ika MTR:ään, sulje I)stunto, Esc : poistu",
		L"LAIN\x1b", &ch);
	switch (ch) {
	case ESC:
		break;
	case L'A':
		memcpy(san, "/SC", 3);
		time(&et_time);
		now = localtime(&et_time);
		san[3] = now->tm_year % 100;
		san[4] = now->tm_mon + 1;
		san[5] = now->tm_mday;
		san[6] = now->tm_hour;
		san[7] = now->tm_min;
		san[8] = now->tm_sec;
		wrt_st_x(cn_regnly[r_no], 9, san, &nch);
		break;
	case L'I':
		ch = L'E';
		wselectopt(L"Haluatko varmasti sulkea istunnon (vain MTR3) (K/E)",
			L"KE", &ch);
		if (ch == L'K')
			memcpy(san, "/NS", 3);
		wrt_st_x(cn_regnly[r_no], 3, san, &nch);
		break;
	case L'N':
		ch = L'E';
		wselectopt(L"Haluatko varmasti poistaa KAIKKI tiedot MTR-laitteesta (K/E)",
			L"KE", &ch);
		if (ch == L'E')
			break;
		ch = L' ';
		wselectopt(L"V)ahvista, että haluat tyhjentää MTR-laitteen muistin (V/E)",
			L"VE", &ch);
		if (ch == L'V') {
			memcpy(san, L"/CL", 3);
			wrt_st_x(cn_regnly[r_no], 3, san, &nch);
		}
		break;
	case L'L':
		ch = mtr_yksi ? L'Y' : L'J';
		wselectopt(L"Lue tiedot: Y)ksi kerrallaan vai J)atkuvasti", L"YJ", &ch);
		if (ch == L'Y') {
			mtr_yksi = 1;
			lue_rno = r_no;
			writeerror_w(L"Pyydä seuraava tieto aina näppäimellä L'N'", 0);
		}
		else
			mtr_yksi = 0;
		ch = L' ';
		wselectopt(L"I)stunto, V)iimeiset, S)anomanumerot", L"IVS", &ch);
		switch (ch) {
		case L'I':
			ch = L'0';
			wselectopt(L"Istunnon numero (0 = viimeisin) - 7",
				L"V01234567\x1b", &ch);
			if (ch == ESC) break;
			if (ch == L'V') ch = L'0';
			if (ch == L'0')
				pkg_loppu[r_no] = vastaus.r13st.pkgno;
			else
				pkg_loppu[r_no] = vastaus.r13st.s_start[ch - L'1'] - 1;
			pkg_alku[r_no] = vastaus.r13st.s_start[ch - L'0'];
			break;
		case L'V':
			clrln(ySize - 3);
			pkg_loppu[r_no] = vastaus.r13st.pkgno;
			vidspwmsg(ySize - 3, 0, 7, 0, L"Viimeisten luku");
			pkg_alku[r_no] = 0;
			inputlongw((INT32 *)&pkg_alku[r_no], 4, 17, ySize - 3, L"\r", &ch);
			pkg_alku[r_no] = pkg_loppu[r_no] - pkg_alku[r_no] + 1;
			break;
		case L'S':
			clrln(ySize - 3);
			vidspwmsg(ySize - 3, 0, 7, 0, L"Ensimmäinen           viimeinen");
			pkg_alku[r_no] = 0;
			inputlongw((INT32 *)&pkg_alku[r_no], 4, 13, ySize - 3, L"\r", &ch);
			pkg_loppu[r_no] = 0;
			inputlongw((INT32 *)&pkg_loppu[r_no], 4, 33, ySize - 3, L"\r", &ch);
			break;
		}
		if (ch != ESC && pkg_alku[r_no] <= pkg_loppu[r_no]) {
			if (pkg_alku[r_no] < vastaus.r13st.oldestpkgno)
				pkg_alku[r_no] = vastaus.r13st.oldestpkgno;
			if (pkg_loppu[r_no] > vastaus.r13st.pkgno)
				pkg_loppu[r_no] = vastaus.r13st.pkgno;
			mtr_pyynto[r_no] = pkg_loppu[r_no];
			memcpy(san, "/GB", 3);
			*(INT32 *)(san + 3) = pkg_alku[r_no];
			wrt_st_x(cn_regnly[r_no], 7, san, &nch);
		}
		break;
	}
	in_aseta_mtr = 0;
	clearframe63();
}

void aseta_s4(void)
{
	wchar_t ch = L' ';
	char san[40];
	int i, nq, ens, viim;

	if (regnly[0] == LID_RTR2) {
		clrln(ySize - 3);
		vidspwmsg(ySize - 3, 0, 7, 0, L"Uudelleen lähetettävien aikojen luku (9999 = kaikki)");
		ens = 0;
		INPUTINTW(&ens, 4, 54, ySize - 3, L"\r\x1b", &ch);
		if (ch == L'\r' && ens > 0) {
			wrt_st_x(cn_regnly[0], 2, "+\r", &viim);
			utsleep(10);
			sprintf(san, "!-%04d\r", ens);
			i_flush_x(cn_regnly[0]);
			wrt_st_x(cn_regnly[0], strlen(san), san, &viim);
			ens = 2;
			for (i = 0; i < 40; i++) {
				utsleep(5);
				read_st_x(cn_regnly[0], ens, san, &viim, &nq);
				bytecount = (bytecount + viim) % bytecountmax;
				if (viim >= ens) break;
				ens -= viim;
			}
			if (i == 20)
				writeerror_w(L"RTR-kello ei vahvistanut pyynnön hyväksymistä", 0);
		}
	}
	if (regnly[0] == LID_ALGE4) {
		san[0] = 0;
		wselectopt(L"T)yhjennys, U)udelleenlähetys, V)iive  Esc : Peruuta", L"TUV\x1b",
			&ch);
		switch (ch) {
		case L'T':
			ch = L' ';
			wselectopt(L"Kaikki ajat poistetaan kellon muistista - vahvista (K/E)",
				L"KE", &ch);
			if (ch == L'K') strcpy(san, "\x95\r");
			break;
		case L'U':
			ch = L' ';
			wselectopt(L"K)aikki ajat, O)sa ajoista   Esc : Peruuta", L"KO\x1b", &ch);
			if (ch == L'K') strcpy(san, "\x85\r");
			if (ch == L'O') {
				clrln(ySize - 3);
				vidspwmsg(ySize - 3, 0, 7, 0,
					L"Alkurivi:        loppurivi:         (Viimeinen vastaanotettu     )");
				vidint(ySize - 3, 61, 4, viimrivi);
				ens = 1;
				viim = viimrivi;
				INPUTINTW(&ens, 4, 10, ySize - 3, L"\r\x1b", &ch);
				if (ch == ESC) break;
				INPUTINTW(&viim, 4, 28, ySize - 3, L"\r\x1b", &ch);
				if (ch == ESC) break;
				if (viim >= ens) sprintf(san, "\x84%04d%04d\r", ens, viim);
			}
			break;
		case L'V':
			clrln(ySize - 3);
			vidspwmsg(ySize - 3, 0, 7, 0, L"Toistoviive sekunnin kymmenyksinä");
			viim = 0;
			INPUTINTW(&viim, 2, 34, ySize - 3, L"\r\x1b", &ch);
			sprintf(san, "\x82%02d\r\x83%02d\r", viim, viim);
			break;
		}
		if (san[0]) {
			wrt_st_x(cn_regnly[0], strlen(san), san, &viim);
		}
	}
}
#endif  // _CONSOLE