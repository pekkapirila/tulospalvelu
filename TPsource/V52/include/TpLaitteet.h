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

﻿#define LID_RTR			1
#define LID_RTR2		2
#define LID_ALGE		3
#define LID_COMET		4
#define LID_ALGE4		5
#define LID_TIMY		6
#define LID_ALGETUTKA	7
#define LID_TDC			8
#define LID_ASC			9
#define LID_ASC3		10
#define LID_EMITKELLO	11
#define LID_LUKIJA		12
#define LID_MTR			13
#define LID_EMITAG		14
#define LID_ETGPRS		15
#define LID_ETSQL		16
#define LID_TUOMARI		19
#define LID_SPORTIDENT	21
#define LID_ARES		22
#define LID_CPOWER		23
#define LID_RTNM		24
#define LID_RADIOEMIT	28
#define LID_VALOT		29
#define LID_SIRIT		30
#define LID_FEIG		31
#define LID_RESERV_1	32
#define LID_IMPINJ		33
#define LID_SW2000		36
#define LID_KURVINEN	40
#define LID_SIIMPORT	99

INT16 r_tarksum(char *buf, INT len, char sum, INT16 xfl);
void openSirit(int r_no);
int openSirit2(int r_no, bool reconnect);
int siritaika(INT32 *t, san_type *vastaus, aikatp *ut, INT *jono, int r_no);
int lue_regnly(INT r_no);
void lue_SiritCmd(int r_no);
void comajanotto(LPVOID lpCn);
void naytatulos(int tulos);
int tulk_emiTag(char *buf, emittp *em, INT32 *emtm, int r_no);
void aseta_mtr(void);
void aseta_emitag(int r_no);
void aseta_s4(void);
void tall_ampsakko(char *sakbuf, int r_no);
int kbdiv1(char ch,char ch2);
void tee_aika(int laji);
INT start_regnly(INT r_no);
void rem_regnly(INT r_no);

/*
static INT32 ec_strtoaika(char *st, int t0, int *i_pv);
static void naytatulosTh(LPVOID lpCn);
static void naytaluentaTh(LPVOID lpCn);
void naytaluenta(int badge);
static UINT16 crc(char *buf, int len);
void SiritSync(bool set_time);
int ProcessSpecialKey(INPUT_RECORD *Buf);
static int tulkSI(char *buf, san_type *vastaus, INT32 SIt, int SItype);

extern int valo_on;
*/

int auto_lahtija(INT32 tlahto);
void tall_etulos(INT32 badge, INT32 t, INT32 tms, INT r_no, int Jono);
void tall_elahto(int badge, int t);
void add_bdg_t(UINT32 badge, INT r_no, char byte9, UINT32 tms);
int tall_regnly(san_type *vastaus, INT r_no);
INT tall_emit(san_type *vastaus, UINT32 *vahvistus, INT r_no);

typedef struct {
	char alku1[3]; // "\x01\x02\x08"
	char sanlaji;  // 1: virall loppu, 2: online, 3: kuluva erä, 4: kuluva erä varmistuksin, 5-6: edell. erä
	char piste;    // S: lähtö, I: väliaika, A: maali, D: vaihto, R: reaktioaika
	char aikalaji; // C: korjattu, E: poistettu, F: vaihtorikko, I: lisätty, M: käsiaika, N: puuttuva, T: varmistuspuute, +: vaihdon viive (?)
	char radat[2]; // käytössä olevat radat bitteinä kahden tavun 5 alimmassa bitissä
	char kierrlkm[2]; // kierrosluku kahdella numerolla
	char tap[3];   // tapahtuma
	char era[2];   // erä kahdella numerolla
	char var1[2];  // varaus
	char sija[2];  // sijoitus kahdella numerolla
	char eot1;     // '\x04'
	char alku2[4]; // "\x01\x02\x08\x0a"
	char rata;     // rata
	char kierr[2]; // kierros kahdella numerolla
	char stx3;     // '\x02'
	char tt[2];    // tunti
	char vali1;
	char mm[2];    // minuutti
	char vali2;
	char ss[2];    // sek
	char vali3;
	char oo[3];    // millisek
	char eot2;     // '\x04'
} aresmsg;

typedef struct {
	UINT32 DevID;
	UCHAR CardID;
	UINT16 Leveys;
	UINT16 Korkeus;
} TauluParamTp;

extern UINT32 pkg_alku[NREGNLY], pkg_loppu[NREGNLY];
extern ecdatatp ecdata[NREGNLY];
extern int siritopen[NREGNLY];
extern INT r_msg_len[NREGNLY];
extern int SiritPoll[NREGNLY];
extern int LatestSirit[NREGNLY];
extern int LatestSiritPoll[NREGNLY];
extern int SiritEiVastaa[NREGNLY];
extern int SiritEventPort[NREGNLY];
extern int SiritCmdPort[NREGNLY];
extern int reconnectSirit[NREGNLY];
extern char alge_tunn[10][4];
extern int r_no_file;
extern INT sak_com, sak_baud, sak_laji;
extern TauluParamTp TauluParam[NREGNLY];

