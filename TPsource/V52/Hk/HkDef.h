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

#ifndef HKDEF_DEFINED
#define HKDEF_DEFINED

#include <wchar.h>
#include <boxr.h>
#include <tputil.h>
#include "valinnat.h"
#include "TpDef.h"

#define MILLISEKAJAT

#define MAXKILPNO 9999
#define RRSAKKO 600

#define BADGEASKEL 1000000

#ifdef MAKI
#undef MAKI
#endif

#ifndef MERI5
#define MERI5
#endif

#ifndef JOUKKUEPISTEET
#define JOUKKUEPISTEET
#endif

#ifndef N_PV
#define N_PV 5
#endif

#define EML  8

//#define N_PV_VA N_PV

#if !defined(AUTOFILE) && defined(TCPSIIRTO)
#define AUTOFILE
#endif

#ifndef VALUKU
#define VALUKU 250
#endif

#define MAXERA 200

#ifndef MAXSARJALUKU
#define MAXSARJALUKU 200
#endif

#ifndef MAXYHD
#define MAXYHD 40
#endif

#define MAXYHDTUL 100
#define SIRITBUFLEN 2000

#define MAXJONO 4

#define HTHREAD int

#define MAXRIVI 50
#define NOMOUSE

#ifndef MAX_LAHPORTTI
#define MAX_LAHPORTTI 64
#endif

#ifndef NREGNLY
#define NREGNLY 32
#endif

#ifndef NCOMEXTRA
#define NCOMEXTRA 4
#endif
#define TXTTVCOM (MAX_LAHPORTTI+2*NREGNLY+3)
#define GAZCOM TXTTVCOM
#define TV1COM TXTTVCOM

#define MAX_PORTTI (MAX_LAHPORTTI+2*NREGNLY+NCOMEXTRA)

#if VALUKU<31
#define KILPRECSIZE 2500
#define KILPBUFSIZE 2500
#else
#define KILPRECSIZE 10000
#define KILPBUFSIZE 10000
#endif

#define NINDEX 12

//#define NOCOM_AIKA

#if (defined(MAALI) || defined(AUTOVA)) && defined(EMIT)
#if !defined(EILEIMAT)
#define EMITLEIMAT
#endif
#endif

#define MAXEMVALAH 49

#ifdef HIIHTO
#undef HIIHTO
#define HIIHTO 1
#endif
#define HIIHTOLAHTO

#ifndef SAKKOCOM
#define SAKKOCOM
#endif

#define LISNOL 9

#ifndef MAXNSEURA
#define MAXNSEURA 600
#endif

#define EMBUFSIZE  500
#define MAXNLEIMA  50
#define MAXNRASTI  (MAXNLEIMA-2)
#define MAXNRATA   500
#define MAXNPISTE  500
#define MAXVARIATION 160

#define  INQSIZE 700
#define  OUTQSIZE 400
#define  INBUFL 30
#define  OUTBUFL 50
#define LISTBLKNO 128
#define TARBLKNO 32
#define LOKIBLKNO 32

#define NKIERROS 3

#define MAXNBLOCK 1024
#define CBUFL 2048
#define SIVPIT 62
#define NIMIINDL 32
#define MAXINDL 60
#define MAXBLKNO 1024
#define NUMERSTKS     2
#define ERSTKSIZE  3000

#define LAIKA 12
#define PISTEKATTO 1000000000

#define MRAJA  720
#define SWAB(i, s) swab((char *)(i), (s), 2)
#define VIDSPAIKA(y,x,t,t0) vidspaika_t((y),(x),(t),(t0),pvparam[k_pv].laika);

#ifndef LSAKKO
#define LSAKKO 6
#endif

#ifndef HAJONTA
#define HAJONTA 11
#endif

#ifndef LSARJA
#define LSARJA  10
#endif

#ifndef LPSARJA
#define LPSARJA  31
#endif

#ifndef LARVO
#define LARVO   15
#endif

#ifndef LYHD
#define LYHD    15
#endif

#ifndef LLYH
#define LLYH    15
#endif

#ifndef LJK
#define LJK     15
#endif

#ifndef LSEURA
#define LSEURA  31
#endif

#ifndef LSNIMI
#define LSNIMI  24
#endif

#ifndef LENIMI
#define LENIMI  24
#endif

#ifndef LWRKOODI
#define LWRKOODI  9
#endif

#define TAULU_RIVPIT 32

typedef UINT32 jarjtulostp[2];

#pragma pack(push, 1)

#ifdef __cplusplus

typedef struct {
	int tagid;
	wchar_t *tag;
} tagListTp;

typedef struct {
	bool on;
	int kesto;
	wchar_t msg[600];
	} virhesantp;

typedef struct {
	wchar_t name[20];
	INT16  type;
	INT16  size;
	INT16  count;
	INT16  bufpos;
	} buf_fields;

class tulosmuottp {
public:
    int snimifl;
	int tpvafl;
	int tklofl;
	int lkmfl;
	int matkafl;
	int otsikot;
	int vaihdanimet;
	int nollakarki;
	int marg;
	int omarg;
	int ymarg;
	int amarg;
	int tabs;
	int numlev;
	int levunit;  // 0: merkkejä, 1: poins, 2: 1/1000 rivi
	int sivpit;
	int esirivit;
	int sarjalinkit; // 0: Ei, 1: kerran, 2: toistuu
	int tauluhtml;
	int riviryhma;
	int fontfamily;
	int fontsize;
	int paaotskoko;
	int alaotskoko;
	int border;
	int vpad;
	int hpad;
	int sarotsbold;
	int bgcolor;
	int fontcolor;
	int thcolor;
	int thfontcolor;
	int td1color;
	int td1fontcolor;
	int td2color;
	int td2fontcolor;
	int tdhcolor;
	int tdhfontcolor;
	int framecolor;
	int framefontcolor;
	int bordercolor;
	int efontsize;
	int eborder;
	int thecolor;
	int thefontcolor;
	int tdecolor;
	int tdefontcolor;
	int cssfl;
	int pistedesim;
	wchar_t des_sep;
	tulosmuottp(void);
	~tulosmuottp(void);
	void operator=(tulosmuottp&);
	void nollaa(void);
} ;

#define MAXAFLDS  600

typedef struct {
	int id;
	int race;
	int point;
	int pos;
	int len;
	int align;
	wchar_t title[30];
	} actFldTp;

class tulostusparamtp {
public:
	PRFILE *lstf;
	int  printer;
	wchar_t sline[2000];
	wchar_t kohde;
	wchar_t rajaus;
	wchar_t tulostettava;
	int  vaihelkm;
	int TlsJaRva;
	bool yhttuljarj;
	bool yhttulpaino;
	int  piste;
	int  enssija;
	int  viimsija;
	bool tarkastamattomat;
	bool seuratulokset;
	bool piiritulokset;
	bool piirisijat;
	int  sj_raja;
	bool karjet;
	bool piilotatulos;
	bool piilotasijat;
	bool lahtoajat;
	bool nollaalaskuri;
	int	 ixjarj;
	int (*ypJrj)[4];
	bool kaikkisijat;
	bool kaikkiva;
	void *p_fld;
	int n_fld;
	int n_aFlds;
	int totW;
	actFldTp *aFld;
	int vpos;
	int rivi;
	int kynnys;
	int optiot;
	int optiot2;
	tulosmuottp tulmuot;
	int language;
	wchar_t merkit;
//	bool ansi;
	bool yksihtml;
	bool htmlkno;
	wchar_t xmlstd;	    // O: oma, I: IOF3.0, L: laajennettu, F: FIS
	wchar_t xmlva;      // E: emitväliajat, M: Muut väliajat
	wchar_t muottied[202];
//	bool lukumfl;
//	bool af_flag;
	wchar_t erotin;
	int automaali[9];
	INT16 *sarjalista;
	wchar_t *pistelista;
	wchar_t *sukuprajaus;
	INT16 *ikasarjat;
	bool paa_alkuteksti;
	bool sivu_alkuteksti;
	bool sivu_alateksti;
	bool lopputeksti;
	tulostusparamtp(void);
	~tulostusparamtp(void);
	void operator=(tulostusparamtp&);
	void nollaa(void);
	void writehtml(wchar_t *rivi);
	void writeline(wchar_t *rv);
	int puts1(wchar_t *rivi);
	void puts_f(wchar_t *rivi);
	bool mahtuusivulle(int l, int vm);
	void setActFlds(int srj);
	int Initdoc(const wchar_t *fnt);
	int Initpage(const wchar_t *fnt);
	void Putfld_int(int k, INT pos, INT len, INT oik, INT r);
	void Putfld(wchar_t *str, INT pos, INT len, INT oik, INT r);
	void Putcell(wchar_t *str, INT pos, INT len, INT oik, wchar_t *param, int cellfl);
	void Newline(INT n);
	void Enddoc(void);
	void Abortdoc(void);
	void Endpage(void);
	void Initline(void);
	void Endline(INT n);
	void Initfont(const wchar_t *fnt);
	void Bold_on(void);
	void Bold_pois(void);
	void Paaots_on(void);
	void Paaots_pois(void);
	void Aliots_on(void);
	void Aliots_pois(void);
};

class vatp {
public:
	INT32 vatulos;
	INT16 vasija;
	INT16 vakoodi;

	__fastcall vatp(void);
	__fastcall ~vatp(void);
	void nollaa(void);
	void operator=(vatp&);
	bool operator==(vatp&);
	void pack(char *buf);
	void unpack(char *buf);
	};

#define VASIZE sizeof(vatp)

#ifdef MAKI
class makitp {
public:
	INT16 vuoro[NKIERROS];
	INT16 nopeus[NKIERROS];
	INT16 pituus[NKIERROS];
	INT16 tuomari[NKIERROS][5][4];
	INT16 mpisteet[NKIERROS];
	INT16 msija[NKIERROS];
	INT16 pari;
	wchar_t mjoukkue[LJK+1];
	wchar_t jkierr;

	__fastcall makitp(void);
	__fastcall ~makitp(void);
	void nollaa(void);
	void pack(char *buf);
	void unpack(char *buf);
}
#endif

class kilppvtp {
public:
	wchar_t txt[10];
	char uusi;
	char vac;
	union {
		UINT32 flags[2];
		struct {
			char salaa;
			char gps;
			char qual;
			char piikit;
			char luonne;
			char ratk_1;
			char myonto;
			char ratk_2;
			};
		};
	int tav;
	int enn;
	wchar_t rata[HAJONTA+1];
	INT16 sarja;
	INT16 era;
	UINT16 bib;
	INT32 badge[2];
	wchar_t laina[2];
	wchar_t selitys[18];
	INT32 pvpisteet[2];
	INT32 tlahto;
	wchar_t keskhyl;
	wchar_t asakot[LSAKKO+1];
	INT32 tasoitus;
	INT32 sakko;
	INT16 ysija;
	vatp *va;

	__fastcall kilppvtp(void);
	__fastcall ~kilppvtp(void);
	void nollaa(void);
	void operator=(kilppvtp&);
	bool operator==(kilppvtp&);
	int	 vert0(kilppvtp &pv1);
	void pack0(char *buf);
	void pack(char *buf);
	void unpack0(char *buf);
	void unpack(char *buf);
	int read(int d, int k_pv, int mista);
	int write(int d, int k_pv, int mihin);
};

#define KILPPVSIZE sizeof(kilppvtp)

#ifdef MAKI
	makitp maki;
#endif

class kilptietue {
public:
	INT16 kilpstatus;
	UINT16 kilpno;
	INT32 lisno[2];
	wchar_t wrkoodi[LWRKOODI+1];
	UINT16 ilmlista;
	UINT16 piiri;
	INT32 pisteet[3];
	wchar_t sukunimi[LSNIMI+1];
	wchar_t etunimi[LENIMI+1];
	wchar_t arvo[LARVO+1];
	wchar_t seura[LSEURA+1];
	wchar_t seuralyh[LLYH+1];
	wchar_t yhdistys[LYHD+1];
	wchar_t joukkue[LJK+1];
	wchar_t maa[4];
	INT16  sarja;
	wchar_t sukup;
	INT16  ikasarja;
	INT16  alisarja;
	INT16  synt;
	INT16 arvryhma;
	kilppvtp *pv;

	__fastcall kilptietue(void);
	__fastcall ~kilptietue(void);
	void nollaa(void);
	void operator=(kilptietue&);
	bool operator==(kilptietue&);
	int	 vert0(kilptietue &kilp1);
	int id(int i_pv = -1);
	void setId(int id);
	void setId(int id, int ipv);
	void setIdAll(int id);
	INT16 Sarja(void);
	INT16 Sarja(int ipv);
	void setSarja(int srj);
	void setSarja(int srj, int ipv);
	void setSarjaAll(int srj);
	bool onSarja(wchar_t *snimi, int i_pv, bool hyvYhd);
	bool onSarja(wchar_t *snimi, bool hyvYhd = false);
	wchar_t *SarjaNimi(int ipv, wchar_t *st = NULL, bool pitka = false);
	INT32 p_sakko(INT piste);
	INT32 sakko_pv(INT i_pv);
	INT32 tulos1(INT piste, INT32 tm, int pyorista = 1);
	INT32 k_tulos(INT piste, INT32 tm, int pyorista = 1);
	INT32 tall_tulos(INT piste, INT32 tls);
	INT32 set_tulos(INT piste, INT32 tm, int pyorista = 0);
	INT32 tall_tulos_pv(INT32 tls, int i_pv, int piste = 0);
	INT32 tall_lajat(INT32 tls, bool molemmat = true);
	INT32 tall_lajat_pv(INT32 tls, int i_pv, bool molemmat = true);
	INT32 tall_laika(INT32 tls);    // Tallentaa aina aktiivisen ajan ja myös arvotun, jos ei hiihtolahto
	wchar_t *asakkoStr(int i_pv = -1);
	void tall_asakot(wchar_t *asakot, int i_pv=-1);
	void tall_sakko(int sakko, int i_pv=-1);
	void set_Asakot(wchar_t *asakot, int i_pv=-1);
	void set_sija(INT piste, INT sj);
	void merk_sijat(void);
	INT32 p_aika(INT piste, int pyorista = 1, int sakotpois = 0);
	INT32 TLahto(int i_pv = -1);
	INT32 TLahto(int i_pv, bool tot, int pyorista = 1);
	INT sija(int i_pv);
	INT p_sija(INT piste, int sarja = -1);
	INT32 maali(INT piste);
	INT32 tmpmaali(INT piste, INT32 tls);
	wchar_t tark(int ipv = -1);
	void set_tark(wchar_t kh, int i_pv = -1);
	wchar_t *tarkstr(wchar_t *st, int i_pv = -1, bool yhttul = false, int len = 8, int lang = 0);
	INT hyv(void);
	INT hyv_pv(INT i_pv);
	INT y_hyv(INT i_pv);
	bool lasna();
	bool lasna(int i_pv);
	INT32 tulos_pv(INT i_pv, bool vaadi_hyv = false, int pyorista = 1);
	INT32 tulos2(INT piste, int pyorista = 1);
	INT32 ytulos_p(INT piste, int pyorista = 1);
	INT32 ytulos(int i_pv, int pyorista = 1);
	wchar_t *ytulos_wstr_cols_n(wchar_t *as, int i_pv, wchar_t sep, int pistedesim,
		int (*ypJrj)[4], int len, int pyorista = 1);
	INT32 ypistetulos(int i_pv);
	INT y_sija(int i_pv);
	wchar_t ytark(int i_pv);
	wchar_t *nimi(wchar_t *buf, int len, bool vaihda);
	wchar_t *seuratieto(int *len);
	wchar_t *seuratieto(int *len, bool lyhenne);
	wchar_t *yhteisotieto(int laji);
	void set_seuratieto(wchar_t *snimi);
	void set_seuratieto(wchar_t *snimi, bool lyhenne);
	int Badge(int pv, int tn, bool myosPoissa = false);
	void GETREC(INT d);
	INT find(INT *d, INT nimihaku);
	void outkilp(int ntlisa);
	int inputkilp(int addfl);
	void pack0(char *buf);
	void pack(char *buf);
	void unpack0(char *buf);
	void unpack(char *buf);
	void unpack_v_kilp(char *buf);
	int read(int d, int mista);
	int write(int d, int mihin);
	wchar_t *strval(tulostusparamtp *tulprm, wchar_t *st, int len, int knt, int pv, int piste, int sj, int opt);

	void TallEhdoin(int pos);
	void SiirraPvEhdoin(kilppvtp *Pv, int ipv);
	bool kirjoitaTiedostoon(void);
	void PUTREC(INT d, int tallenna);
	void addfrec(int *d);
	void addtall(INT *dataf, INT kielto);
	void tallenna(INT d, INT tarbuffl, INT kielto, INT pakota_lah, kilptietue *entkilp);
	int tuloskaavat(int kielto);
	int as_sakko(int piste);
	int asakkolkm(int ipv = -1);
	void laskeKierrokset(void);
	int TakaaAjoLisa(int ipv = -1);
};

// Kaavalajit:
//   0: tuloksista tuloksiin jatkuvasti laskettava
//   1: tuloksesta lineaarisesti pisteiksi vakioiden mukaan
//   2: tuloksesta inverssinä pisteiksi vakioiden mukaan
//   3: tuloksesta lineaarisesti pisteiksi suhteessa voittajaan
//   4: tuloksesta inverssinä pisteiksi suhteessa voittajaan

typedef struct {
	bool on;
	int laji;
	int yla;
	int ala;
	bool vaadiala;
	int kerroin;
	int jakaja;
	bool sakotpois;
	int nollataso;
	int minimi;
	int maksimi;
	bool pyorista;
} kaavatp;

typedef struct {
	bool on;
	int kerroin1[N_PV];
	int kerroin2[N_PV];
	int jakaja1[N_PV];
	int jakaja2[N_PV];
	int minimi;
	int maksimi;
} ypkaavatp;

typedef struct {
	int vaiheon;
	double vpist;
	int laji;
	int plaji;
	int vaika;
	int kerr;
	int jak;
	int optiot;
	int a[5];
	int y[5];
	int d[5];
	int p[5];
	int yhtpiston;
	int yhtpistvaihelkm;
	int yhtpistosottomin;
	int yhtpisthyvmin;
	bool yhtp;
	} pistekaavatp;

typedef struct {
	INT16 alkukdi;
	INT16 loppukdi;
	INT16 piste;
	INT16 pohja;
	} mtbtp;

class sprintsarja {
public:
	int vaihelkm;
	int eralkm[N_PV-1];
	int erakoko[N_PV-1];
	int sarjat[N_PV-1][6];
	wchar_t srjnm[N_PV-1][6][LSARJA+1];

	sprintsarja(void);
	~sprintsarja(void);
	void nollaa(void);
	void operator=(sprintsarja&);
	bool operator==(sprintsarja&);
	};

class sarjatietue {
public:
   wchar_t sarjanimi[LSARJA+1];
   wchar_t  psarjanimi[LPSARJA+1];
   wchar_t  pvsarjanimi[N_PV][LPSARJA+1];
   INT  enspv;
   INT  viimpv;
   INT  sarjaalku;
   int  luonne[N_PV];    // 0: erillinen, 1: jatkoa aiemmille, 2: takaa-ajo, 9: ohitetaan
   INT  valuku[N_PV];
   INT  bibalku[N_PV];
   INT32 enslahto[N_PV];
   INT32 ensjlahto[N_PV];
   INT  lahtovali[N_PV];
   INT  pari[N_PV];
   INT  lno[N_PV];
   int  portti[N_PV];
   int  mno[N_PV];
   int lsak[N_PV];
   INT tsak[N_PV];
   int npist[N_PV];
   int rrtas;
   INT  maksu;
   INT  maksu1;
   INT  maksual;
   INT  tuljarj;
   bool piilotatulos;
   bool vakanttisarja;
   wchar_t tapa[N_PV][3];
   wchar_t matka[N_PV][22];
   wchar_t va_matka[N_PV][VALUKU][22];
   int va_koodi[N_PV][VALUKU];
   int va_piilota[N_PV][VALUKU];
   INT  tul_raja[N_PV];
   INT  va_raja[N_PV][VALUKU];
   int  va_sakot[N_PV][VALUKU+1];
   INT  tlskerroin[N_PV];
   INT  tlsjakaja[N_PV];
   kaavatp kaava[N_PV][VALUKU+3];
   pistekaavatp pistekaava[N_PV];
   mtbtp mtb[N_PV][VALUKU+1];
   bool laskemtb[N_PV];
   wchar_t flags[N_PV][12];
   wchar_t rata[N_PV][20];
   wchar_t asuunnlaji[N_PV][8];
   int asuunnsakot[N_PV][7];
   sprintsarja sprintSrj;
   int pistekarki[N_PV];

	__fastcall sarjatietue(void);
	__fastcall ~sarjatietue(void);
	void nollaa(void);
	void operator=(sarjatietue&);
	bool operator==(sarjatietue&);
	void tulkFormula(wchar_t *ln, int i_pv, int va);
	void tulkPointFormula(wchar_t *ln, int i_pv, int iRule);
	void tulkMTB(wchar_t *ln, int i_pv);
	wchar_t *writeFormula(wchar_t *ln, int i_pv, int va);
	wchar_t *writePointFormula(wchar_t *ln, int i_pv, int iRule);
	bool yhtpistkaikille(void);
	bool YhtpistOn(void);
	bool pistetulos(void);
	bool pisteytulos(void);
	bool nouspisteet(void);
	void AlustaKaavat(wchar_t orig_laji, wchar_t *orig_alalajit);
   };

#define KNT_TEXT 0
#define KNT_WTEXT 1
#define KNT_CHAR 2
#define KNT_WCHAR 3
#define KNT_INT32 4
#define KNT_UINT32 5
#define KNT_INT16 6
#define KNT_VA 97
#define KNT_PV 98
#define KNT_SIZE 99

typedef struct {
	int WinMajorVersion;
	int WinMinorVersion;
	wchar_t CSDVersion[128];
	bool isWow64_32;
	bool isWinServer;
	int IEVersio;
	int IEEmul;
	} envdatatp;

typedef struct {
	int kilprecsize0;
	int kilprecsize;
	int kilppvtpsize0;
	int kilppvtpsize;
	int vatpsize;
	int makitpsize;
	wchar_t tiedlaji[6];
	int n_pv;
	int valuku;
	int vakanttisarja;
	wchar_t kilplaji;
	wchar_t alalaji;
	wchar_t badgelaji;
	int kaksibadge;
	int kvlisno;			// 0: normaali, >= 4 : alkaa maakoodilla ja numeroiden minimimäärä
	int n_pv_akt;
	int maxvaluku;
	bool pyorlahimpaan;
	wchar_t yhttlslaji;    // S: summa, L: sarjan lähdöstä, P: paras, V: vaihtuva sarja sallitaan
	int tarkkuus;
	int lsnimi;
	int lenimi;
	int larvo;
	int lseura;
	int lseuralyh;
	int ljoukkue;
	int lyhdistys;
	int lwrkoodi;
	int lsakmax;
	int n_xtext;
	int n_xint;
	int sakkoyks;
	bool seuralyh_on;
	bool bibvaihtelee;
	bool sarjavaihtelee;
	bool ikasarjat;
	bool sukup;
	bool kierroskilpailu;
	int kierrospiste;
	int  vuosi;
	wchar_t kilpkoodi[25];
	int aktbib;
	int maxasakko;
	bool WisV;
	int poissamukaan;
	int analoolisa;
	int lahtoryhma;
	bool hyvHavaittu;
	wchar_t asuunnlaji[8];
	wchar_t kartta[200];
	}  kilpparamtp;

typedef struct {
	wchar_t RaceId[20];
	wchar_t Title[82];
	int Date;
	wchar_t kilplaji;
	wchar_t alalaji;
	int hiihtolahto;
	unsigned int laika;
	unsigned int laikasj;
	unsigned int laika2;
	unsigned int laikava;
	int asakkolaji;      // 0: ei, 1: ampumahiihdon (max/piste = 9), 2: muu
	int pyor[6];             // 0: ei, 1: tall, 2: jarj, 3: tuloste, 4: takaa-ajolähtö, 5: väliaika
	int valuku;
//	int takaaAjoLisa;
	int fvalue;
	int minpenalty;
	int officialid[4];
	wchar_t OffType[4][20];
	wchar_t OffSNimi[4][30];
	wchar_t OffENimi[4][30];
	wchar_t OffMaa[4][4];
	}  pvparamtp;

class henkTieto {
public:
	char	 key[40];
	int 	 id;
	int		 intid;
	int 	 bib;
	int 	 badge;
	wchar_t  sukunimi[LSNIMI];
	wchar_t  etunimi[LSNIMI];
	wchar_t  arvo[LARVO];
	wchar_t  seura[LSEURA];
	wchar_t  seuralyh[LLYH];
	wchar_t  maa[4];
	wchar_t  joukkue[LJK];
	wchar_t	 sarja[12];
	wchar_t  sukupuoli;
	int		 synt;
	int 	 ika;
	double   ranki;
	wchar_t  maksettu;
	int		 oolkm;
	wchar_t	 viimoo[20];
	wchar_t	 lisatieto[60];

	__fastcall henkTieto(void);
	__fastcall ~henkTieto(void);
	int 	tulkRivi(wchar_t *Buf);
	int 	tulk_HenkRivi(wchar_t *rivi, int lineno, wchar_t erotin);
	int 	tee_HenkRivi(wchar_t *rivi, int len, wchar_t erotin);
};

class seuraTieto {
public:
	char	 key[30];
	int 	 piiri;
	wchar_t  lyhenne[LLYH+1];
	wchar_t  nimi[LSEURA+1];
	wchar_t  maa[4];

	__fastcall seuraTieto(void);
	__fastcall ~seuraTieto(void);
	int 	tulkRivi(wchar_t *Buf, wchar_t *erotin, int jarj, bool maakoodit);
};

class seuraLuettelo {
public:
	seuraTieto	*Seura;
	int			NSeura;
	int			NSeuraMax;
	int			jarj;
	bool		maakoodit;
	bool		otsrivi;
	wchar_t		erotin[20];
	wchar_t		FName[200];

	__fastcall seuraLuettelo(wchar_t *SraFname = NULL, int jarj = 0);
	__fastcall ~seuraLuettelo(void);
	void nollaa(void);
	int lueSeurat(wchar_t *SraFname);
	int kirjSeurat(wchar_t *SraFname);
	void _fastcall TallennaSeura(int piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi, int kielto);
	seuraTieto *haenimella(wchar_t *nimi, bool tarkka = true);
	seuraTieto *haelyhenteella(wchar_t *lyhenne, bool tarkka = true);
	wchar_t *nimi(wchar_t *lyhenne, bool tarkka = true);
	wchar_t *lyhenne(wchar_t *nimi, bool tarkka = true);
};

typedef struct {
	int x;
	int y;
	int size;
	wchar_t txt[200];
	} txtTp;

typedef struct {
	int ytila;
	int ypos;
	int n_txt;
	txtTp *txts;
	} lisatxttp;

typedef struct {
	char key[8];
	int ero;
	int kilpno;
	int pos;
	wchar_t kh;
} emiterotp;

typedef struct {
	INT32 t;
	INT32 date;
	INT16 kno;
	INT16 piste;
	INT16 tapahtuma;
	INT32 badge;
	INT16 kanava;
	UINT8 jono;
	UINT8 status;
} aikatp;

typedef aikatp *aikatp_ptr;

class aikajonotp {
public:
	int jono;
	aikatp *atime;
	int rtime;
	int rwtime;
	int mxtime;
	int fp1;
	int *aktrow;
	int aktrows;
	UINT8 atimemask;
	int aikakorjaus;
	int afhandle;
	FILE * fafhandle;
	wchar_t aikafname[200];
	aikajonotp(int lkm);
	~aikajonotp(void);
	void haeAktAjat(void);
	aikatp * getTime(aikatp *rtm, UINT32 rvno);
	aikatp * getAktTime(aikatp *rtm, UINT32 rvno);
};

#endif // __cplusplus

typedef enum {
	Nm, Lis, Sra, Srj, Kno, Tls, TlsY, Ksk, Hl, El, Maika,
	Kaika, Tas, Sakko, Taso, Sako, O1, O2, O3, O4, O5, O6, O7, RNo
} tuntype;

typedef struct {
   tuntype tunnus;
   INT ehto;
   INT x, y, w;
	INT df, rf;
} tarrak;

typedef struct {
   INT r;
   INT c;
   char *t;
   } line;

typedef struct {
   INT nl;
   line l[18];
	} scr;

typedef struct {
	INT16 kdi;
	INT16 piste;
	} lahdepsttp;

typedef struct {
	wchar_t drivername[40];
	wchar_t librarynameosx[30];
	wchar_t vendorlibwin64[30];
	wchar_t vendorlibosx[30];
	int blobsize;
	wchar_t database[50];
	wchar_t hostname[50];
	wchar_t localecode[10];
	wchar_t Password[50];
	wchar_t User_Name[50];
	int  port;
	bool compressed;
	bool encrypted;
	bool autostart;
	bool sqltallennus;
	}sqlparamtp;

typedef struct {
   INT kno;
	fpos_t ptr;
   } TIETOIX;

typedef struct {
	wchar_t name[20];
	INT pos;
	UINT len;
	INT oik;
	} FLD;

typedef struct {
	int id;
	wchar_t name[20];
	int vpos;
	int hpos;
	int len;
	int koko;
	int oik;
	} PageFieldTp;

typedef struct {
	INT kilpno;
	wchar_t vanha;
	wchar_t sarja;
	INT sija;
	INT piste;
	wchar_t tark;
	} sanomatp;

class IOFPoint {
	public:
	wchar_t  PointType;		// S: Start, C: Control, F: Finish
	wchar_t	 Code[6];
	double CY;
	double CX;
	double MapY;
	double MapX;
	int	 ECodes[10];
	IOFPoint(void);
	~IOFPoint(void);
	void operator=(IOFPoint&);
	void nollaa(void);
	};

class IOFCourseControl {
	public:
	int	 iPoint;
	wchar_t  Code[6];
	int	 LegLength;
	IOFCourseControl(void);
	~IOFCourseControl(void);
	void operator=(IOFCourseControl&);
	void nollaa(void);
	};

class IOFClassAssignment {
	public:
	wchar_t Class[LSARJA+1];
	wchar_t Course[30];
	IOFClassAssignment(void);
	~IOFClassAssignment(void);
	void operator=(IOFClassAssignment&);
	void nollaa(void);
	};

class IOFCourseVariation {
	public:
	wchar_t	Name[30];
	wchar_t Id[20];
	int Length;
	int Climb;
	int StartLeg;
	int FinLeg;
	wchar_t Start[6];
	wchar_t Finish[6];
	int ennakko;
	int ajanottotapa;
	IOFCourseControl *CourseControl[MAXNRASTI+5];
	IOFCourseVariation(void);
	~IOFCourseVariation(void);
	void operator=(IOFCourseVariation&);
	void nollaa(void);
	};

class IOFCourse {
	public:
	wchar_t	Name[30];
	wchar_t Id[20];
	wchar_t ShortName[10][20];
	IOFCourseVariation *CourseVariation[MAXVARIATION];
	IOFCourse(void);
	~IOFCourse(void);
	void operator=(IOFCourse&);
	void nollaa(void);
	};

//  EMIT-määrityksiä

// maalilajit: 1: ajanotto erikseen, viim. rastiväli tuloksesta
//             2: ajanotto erikseen, viim. rastiväli korjattu kortin aika
//             3: ajanotto RTR-2:lla
//             4: lopputulos kortilta viimeisen rastin ajasta

typedef struct {
	wchar_t tunnus[12];
	INT rastiluku;
	INT ennakko;     // nollaus ennakko sek ennen lähtöä, -32767: vaihtelee
	INT maalilaji;
	int viitmatka;
	int ratapit;
	int nousu;
	wchar_t lahto[6];
	wchar_t maali[6];
	INT rastikoodi[MAXNRASTI];
	int matka[MAXNRASTI+1];
	} ratatp;

typedef struct {
   wchar_t tunnus[6];
   double mapX;
   double mapY;
   double X;
   double Y;
   } rastikoordtp;

typedef struct {
   int scale;
   double leftX;
   double topY;
   double rightX;
   double btmY;
   double bgscale;
   double bgshiftX;
   double bgshiftY;
   } maptp;

typedef struct {
   INT16 rasti;
   INT16 koodi;
   } ekditp;

class IOFCourseData {
public:
	wchar_t DocType[100];
	wchar_t IOFVersion[20];
	wchar_t ModifyDate[20];
	wchar_t ModifyTime[20];
	int	MapScale;
	double MapTY;
	double MapLX;
	double MapBY;
	double MapRX;
	IOFPoint *Point[MAXNPISTE];
	IOFCourse *Course[MAXNRATA];
	IOFClassAssignment *ClassAssignment[MAXSARJALUKU];
	IOFCourseData(void);
	~IOFCourseData(void);
	void operator=(IOFCourseData&);
	void nollaa(void);
	void CopyMap(IOFCourseData&);
	int siirraXmlKartta(maptp *map);
	int siirraXmlKuviot(ekditp **kdi, int *nkoodi, int *maxnkoodi);
	int siirraXmlRadat(ratatp **rt, int *nrata, wchar_t nimilaji, int maalikoodi, int ajanottotapa, int ennakko);
	void siirraRataKohdennukset(void);
	int haeXmlKartta(maptp *map);
	int haeXmlKuviot(ekditp *kdi, rastikoordtp *koord, int nkoodi);
	int haeXmlRadat(ratatp *rt, int nrata);
};

class emittp {
public:
	UINT32 package;
	INT32 badge;
	INT16 badgeyear;
	INT16 badgeweek;
	INT32 maali;          // RTR2-kellon ottama maaliaika
	INT32 time;           // kortin lukemishetki
	char ctrlcode[MAXNLEIMA];
	UINT16 ctrltime[MAXNLEIMA];
	INT16 sc;
	INT16 pc;
	INT16 lc;
	INT16 kilpno;
	INT16 osuus;
	INT32 badge0;
	INT32 kirjaus;
	void nollaa(void);
	int get(int i_ptr);
	int getbykno(int kno, int toinen);
	int put(int i_ptr, bool inCriticalSection);
	bool LueSeuraava(int *kno);
	bool LueEdellinen(int *kno);
	};

typedef struct {
	INT16 kilpno;
	INT16 osuus;
	INT32 badge;
	INT32 tulos;
	INT16 rastiluku;
	wchar_t rata[12];
	INT16 ok;
	UINT16 rastit[MAXNLEIMA-1][2];
	} emitvatp;

typedef struct {
   INT32 badge;
   INT16 kilpno;
   } bdg_kno_tp;

typedef int rmaartp[120];

typedef struct {                      /* Tiedonsiirtopaketti */
		INT32 lahetetty;
		INT16 portti;
		char      so,id,iid;         /* Paketin id */
		char      pkgclass;          /* Paketin laji */
		UINT16     len;               /* Paketin pituus */
		UINT16     checksum;          /* Tarkistussumma */
		  union {
			  struct {
				  char			tunn;			// avaussanoman tunnus (ascii-arvo 1)
				  char			konetunn[2];	// lähettävän koneen tunnuskoodi
				  UINT8			vaihe;			// kilpailun vaihe (k_pv+1)
				  UINT16		nrec;			// tietueiden lkm
				  UINT32		flags;			// flags
				  } alku;
			  struct {
				  char           tarf;
				  char           pakota;      // Pakota tallennus ja lähetys
				  INT16          dk;          /* Osoite taulukossa kilparr */
				  INT16          entno;       /* Vanha kilpno, 0 : uusi */
				  char           ckilp[400];             /* Tiedot */
				  } k;
			  struct {
				  char           tarf;
				  char           pakota;      // Pakota tallennus ja lähetys
				  INT16          dk;          /* Osoite taulukossa kilparr */
				  INT16          pv;         /* Vanha kilpno, 0 : uusi */
				  INT16			 valuku;		// Lähettävien väliakojen määrä
				  char           cpv[KILPPVSIZE+(VALUKU+2)*VASIZE]; /* Tiedot */
				  } pv;
			  struct {
				  char           tarf;
				  char           pakota;      // Pakota tallennus ja lähetys
				  INT16          dk;          /* Osoite taulukossa kilparr */
				  INT16			 bib;
				  INT16			 k_pv;
				  INT16          vali;
				  INT32          aika;
				  } v;
			struct {
				  char           pakota;      // Pakota tallennus ja lähetys
				  aikatp         daika;
				  aikatp         iaika;
				  } a;
			  struct {
				  char           pakota;      // Pakota tallennus ja lähetys
				  INT16			 k_pv;
				  wchar_t        keskhyl;
				  } lm;
			  struct {
				  wchar_t        snimi[LSEURA+1];
				  wchar_t        lyhenne[LLYH+1];
				  wchar_t        maa[4];
				  INT16          piiri;
				  } sra;
			  struct {
				  INT32           badge;
				  INT32           aika;
				  INT16           piste;
				  } ec;
			struct {
				  INT32           d1;
				  INT32           d2;
				  INT32           d3;
				  INT32				d4;
				  } x;
			struct {
				  UINT32		  flags;
				  UINT32		  koodi;
				  INT32		  	  pos;			  // -1: alku, >= 0: positio, -2: datan loppu
				  INT32			  len;			  // sisällön pituus
				  char		     buf[2];          // Alussa tiedoston nimi (suhteellinen polku), lopussa ei sisältöä,
				 } fl;
#if defined(EMIT)
			emittp             em;
			emitvatp			  emva;
#endif
			  } d;
		  } combufrec;

typedef struct {
   INT16  kilpno;
   INT16  era;
   INT16  rata;
   } eratp;

typedef struct {
	wchar_t tunnus;
	wchar_t nimi[40];
	int kerroin;
	int jakaja;
	int vertaika;
	int vertpiste;
	int yks;
	} sotlajitp;

typedef struct {
	char *buf;
	int buflen;
	wchar_t eThost[100];
	int eTport;
	int eTserial[4];
	int eThakuvali;
	int eTrno;
	int eThaku;
	wchar_t eTdate[12];
	wchar_t eTtime[10];
	int msgno[4];
	int haettu;
	bool autostart;
} eThakuParamtp;

#pragma pack(pop)

#if defined(EMIT)

class rpistevatp {
public:
   int aika;
   INT16 sarja;
   INT16 kilpno;
   rpistevatp(void);
   ~rpistevatp(void);
   };

class ratavatp {
public:
	int npiste;
	rpistevatp *pisteva;
	ratavatp(void);
	~ratavatp(void);
	int lisaa(int kilpno, int sarja, int nva, int aika);
	int poista(int kilpno, int nva);
	};

class rastivatp {
public:
   ratatp *rata;
   int nva;
   int maxnva;
   ratavatp *ratava;
   rastivatp(void);
   ~rastivatp(void);
   void poista(int kilpno);
   };

typedef struct {
	int aika;
	int sj;
	int kaika;
	} rastivaanaltp;

typedef struct {
   INT32 kdi;
   int matka;
   int maxlkm[MAXSARJALUKU];
   int lkm[MAXSARJALUKU];
   int sum[MAXSARJALUKU];
   int vertaika[MAXSARJALUKU];
   int voittaika[MAXSARJALUKU];
   int *ajat[MAXSARJALUKU];
   int *kilpno[MAXSARJALUKU];
   } rvalitp;

typedef struct {
   int rkoodi;
	int aika;
   int rvaika;
   int rvno;
   } emitvaanaltp;

#endif

#pragma pack(push, 1)
//#pragma pack(1)

#define R_BUFLEN 15000

typedef struct {
   char hi;
   char lo;
	} hltype;

typedef struct {
   char code;
   UINT16 t1;
   } ctrltype;

typedef union {
   struct {
	  char tunnus;
		char cr1;
      char n1;
      char s1;
	  char kno[4];
      char s2;
      char min[3];
      char kp;
      char sek[2];
      char p;
	  char osat[2];
      char cr;
      char loppu;
      char loppu2;
      char loppu3;
      } r1;
   struct {
      char tunnus;
      char cr1;
		char n1;
      char s1;
      char kno[5];
      char min[3];
      char kp;
      char sek[2];
      char p;
	  char osat[2];
      char tuh;
      char pakka;
      char portti;
      char rac;
      char tark;
	  char cr;
      char loppu;
      } r2;
   struct {
      char tunnus;
      char cr1;
      char n1;
	  char s1;
      char tyhja;
      char z1;
      char z2;
      char z3;
      char bl;
	  char tunti[2];
      char kp1;
      char min[2];
	  char kp2;
      char sek[2];
	  char p;
	  char osat[2];
      char cr2;
      char cr3;
      char cr4;
      char cr5;
      char cr6;
      char cr7;
      } r3;
   struct {
      char tunnus;
      char kno[3];
      char L_C[3];
      char piste;
      char bl;
      char tunti[2];
      char kp1;
      char min[2];
      char kp2;
      char sek[2];
      char p;
      char osat[2];
      char tuh;
      char cr;
      char lf;
	  char z;
      } r4;
   struct {
		char tunnus;
      char kno[4];
      char bl1;
      char cc;
      char piste;
      char cM;
      char bl2;
	  char tunti[2];
	  char kp1;
      char min[2];
      char kp2;
	  char sek[2];
      char p;
      char osat[2];
      char tuh[2];
      char cr;
      char lf;
	  char z;
      } r6;
   struct {
	  char tunnus;
		char piste;
      char bl1;
	  char tunti[2];
      char min[2];
      char sek[2];
      char osat[2];
      char tuh[1];
      char bl2;
      char kno[4];
      char cr;
      char lf;
		char z;
      } r9;
#ifdef EMITLEIMAT
   struct {
      char tunnus;
      char piste;
      char laiteno;
      char badge[6];
      char min[3];
      char kp;
      char sek[2];
      char p;
      char osat[3];
      char smin[3];
	  char skp;
      char ssek[2];
      char sp;
      char sosat[3];
	  hltype u[214];
      char loppu[4];
      char cr;
	  } r11;
   struct {
      UINT16 alku;
      char badge[3];
      char fill1;
      char week;
      char year;
      char fill2;
      char chksum;
	  ctrltype c[MAXNLEIMA];
      char fill3[40];
      char chs;
      char cs[4];
      char chp;
	  char cp[4];
      char chl;
      char cl[4];
	  char fill4;
      char chksum2;
      } r12;
   struct {
		UINT32 alku;
      char msglen;
      char tunnus;
      UINT16 mtrid;
      char t_year;
      char t_month;
      char t_day;
      char t_hour;
      char t_min;
      char t_sek;
      UINT16 t_ms;
      UINT32 pkgno;
      char badge[3];
      char week;
      char year;
	  char chksum;
	  ctrltype c[MAXNLEIMA];
      char fill3[40];
      char chs;
      char cs[4];
      char chp;
	  char cp[4];
      char chl;
      char cl[4];
      char fill4;
	  char chksum2;
      char fill5;
      } r13;
   struct {
      UINT32 alku;
      char msglen;
      char tunnus;
      UINT16 mtrid;
      char t_year;
      char t_month;
      char t_day;
      char t_hour;
      char t_min;
      char t_sek;
      INT16 t_ms;
      char battery;
      UINT32 pkgno;
	  UINT32 oldestpkgno;
	  UINT32 s_start[8];
      char chksum2;
      char fill4;
	  } r13st;
#ifdef SPORTIDENT
   struct {
	   char stx;
	   char tunnus;
	   char csi;
	   char t[2];
	   char  cn[3];
	   char etx;
		} r21;
   struct {
	   INT32 badge;
		INT32 lukija;
		INT32 start;
		INT32 check;
		INT32 finish;
		char cc[66];
		INT32 ct[66];
		} r21data;
#endif
#endif
#ifdef LAJUNEN
	struct {
		char len[2];
		char trtype;
		char iddt;
		char iddlen;
		char idd[14];
		char tm[4];
		char dt[5];
		char antenna;
		} r31;
#endif
#ifdef SWIM2000
	struct {
		char rata[2];
		char valid;
		char source[2];
		char end;
		char count[3];
		char fill1[1];
		char h[2];
		char colon1;
		char m[2];
		char colon;
		char s[2];
		char point;
		char osa[2];
		char cr;
		char lf;
		char z;
		} r32;
#endif
   char bytes[R_BUFLEN+1];
	} san_type;

typedef struct {
	char devtype[32];
	INT16 mode;
	int first;
	int next;
	INT32 code;
	INT32 devtime;
	INT32 pctime;
	UINT32 serial;
	char health[32];
	char comm[12];
	} ecdatatp;

#pragma pack(pop)


#endif  // !LASKDEF_DEFINED


