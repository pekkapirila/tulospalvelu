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

#ifndef LASKDEFDEFINED
#define LASKDEFDEFINED

#define  MAXYHD 10

#include <boxr.h>
#include <tputil.h>
#include <valinnat.h>
#include <TpDef.h>
#ifndef _CONSOLE
#include <VWinDef.h>
#else
#define __fastcall
#endif

#ifndef VAIKALUKU
#define VAIKALUKU 12
#endif

#define MAXJONO 24

#ifdef VALUKIJA
#define AUTOVA
#endif

#ifndef MAX_LAHPORTTI
#define MAX_LAHPORTTI 32
#endif

#define MAXRIVI 50
#define HTHREAD int

#if defined(LUENTA) && !defined(NOCOM_AIMA)
#define NOCOM_AIKA
#endif

#define AMPU
#ifndef NREGNLY
#define NREGNLY 32
#endif
#ifndef NCOMEXTRA
#define NCOMEXTRA 4
#define TXTTVCOM (MAX_LAHPORTTI+2*NREGNLY+3)
#define GAZCOM (MAX_LAHPORTTI+2*NREGNLY+2)
#endif
#define MAX_PORTTI (MAX_LAHPORTTI+2*NREGNLY+NCOMEXTRA)
#define  INBUFL 50 
#define  OUTBUFL 500
#define SIRITBUFLEN 2000

#define MAXNLEIMA 50
#define MAXEMITVA 50
#define MAXEMVALAH 45
#define MAXKILPNO 9999

#define BADGEASKEL 1000000

#if defined(MAALI) && defined(EMIT) && !defined(EILEIMAT)
#define EMITLEIMAT
#endif

#define MAXNVAIKA 9999

#ifndef MAXSARJALUKU
 
//   Määritysten HALIKKO jne. merkitys:
//
//   HALIKKO:  Rakenne 1 + 3 + 3 + 3 + 3 + 1 + 1
//             Vain yksi sarja
//             Oletusosuuden valinta
//   NV     :  Rakenne 1 + 1 + 2 + 2 + 1 tai normaali
//             Useita sarjoja
//             Oletusosuuden valinta
//   EPMKV  :  Rakenne 1 + 2 + 2 + 1 + 1 tai normaali
//             Useita sarjoja
//             Oletusosuuden valinta
//   SKMKV  :  Rakenne 1 + 3 + 2 + 1  tai 1 + 2 + 1 tai normaali
//             3. osuus lähtee 2. osuuden valmistuttua
//             Useita sarjoja
//             Oletusosuuden valinta
//   MKV    :  Rakenne 3 + 3 + 3 + 1 + 1 tai normaali
//             Useita sarjoja
//             Oletusosuuden valinta
//   MKV6   :  Rakenne 2 + 2 + 1 + 1 tai 2 + 1 + 1 tai normaali
//             Useita sarjoja
//             Oletusosuuden valinta
//   MKV41  :  Keski-Suomen MKV
//					Rakenne 4+4+4+1+1 tai 3+3+3+1+1 tai 2+2+2+1+1 tai normaali
//             Useita sarjoja
//             Oletusosuuden valinta
//

#if defined(HALIKKO)
#define MAXSARJALUKU 1
#else
#if defined(MKV) || defined(MKV6)
#define MAXSARJALUKU 3
#else

#define MAXSARJALUKU 200
#endif  // MKV MKV6
#endif  // HALIKKO
#endif !defined(MAXSARJALUKU)

#if !defined(MAXNOSUUS)
#define MAXNOSUUS 4
#endif

#define MAXNSEURA 2000

#if VAIKALUKU > 4 && MAXOSUUSLUKU < 5
#define OSD25 3
#else
#if (VAIKALUKU > 0 || MAXOSUUSLUKU < 6) && MAXOSUUSLUKU < 9
#define OSD25 2
#else
#define OSD25 1
#endif
#endif

#if VAIKALUKU > 16
#define OSD 6
#else
#if VAIKALUKU > 12
#define OSD 5
#else
#if VAIKALUKU > 8
#define OSD 4
#else
#if VAIKALUKU > 4
#define OSD 3
#else
#define OSD 2
#endif
#endif
#endif
#endif

#define MAXNRASTI  50
#define MAXNRATA   6000
#define MAXNPISTE  500
#define MAXVARIATION 160

#ifndef MAXMAALI
#define MAXMAALI 3
#endif

#define TVMAXSIJA 2000
#define EPSON  1
#define LJETII 2
#define POSTSCRIPT 3
#define PROPRIEMU 4
#define TXTTAB 5
#define CANON  6
#define GDIPRINTER 7
#define INQSIZE 600
#define OUTQSIZE 600
#define LISTBLKNO 256
#define TARBLKNO 32
#define LOKIBLKNO 32

#define JTULOSJAK  1048576LL    // 2^20
//#define JTULOSASKEL  (JTULOSJAK*I64MAXUINT)   // 2^52
#define JTULOSASKEL (I64MAXUINT*(I64MAXUINT/16))
#define MRAJA  720
#define LAIKA 10

#define LSEURA  50
#define LJK  30
#ifndef OSNIMIL
#define OSNIMIL 40
#endif
#define OSSEURAL 30
#define LARVO 15

#ifndef HAJONTA
#define HAJONTA  8
#endif

#define SELITYS 9

#ifndef LSAKOT
#define LSAKOT 5
#endif

#define LSARJA 10
#define LPSARJA 40

#define NOX 15
#define SEX 9
#define COX 45
#define JOX 60

#if MAXOSUUSLUKU < 7
#define OSY 9
#define MAY 3
#define SAY 4
#define NOY 3
#define SEY 5
#define COY 5
#define JOY 5
#define YAY 4
#define OTSY 7

#else
#define NOY 2
#define SEY 4
#define COY 4
#define JOY 4
#define YAY 3
#define OTSY 5
#define MAY 2
#define SAY 3

#if MAXOSUUSLUKU < 8
#define OSY 7
#else
#define OSY 6
#endif

#endif
#define SAX 9

/* #else
#define NOY 3
#define SEY 7
#define JOY 7
#define YAY 5
#define OSY 11
#define MAY 3
#define SAX 9
#define SAY 5
#endif  
*/
#define OSX 1
#define NMX 4
#define TUX 31
#define OTX 42
#define KHX 53
#define SJX 57
#define YLX 61
#define YAX 54
#define MAX 54
#define NSEURAMAX 250
#define SARJAINDL  16
#define NIMIINDL   50
#define MAXINDL   52
#define CBUFL 4096
#define SIVPIT 62
#define MAXBLKNO 1024
#define NUMERSTKS     2
#define ERSTKSIZE  3000

#ifndef max
#define max(a, b)  (((a)>(b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b)  (((a)<(b)) ? (a) : (b))
#endif
#ifdef _CONSOLE
#define clrln(Y) scclrmsg((Y), 0, 80)
#define clrtxt(Y, X0, X1) viwrrect((Y),(X0),(Y),(X1),tyhjarivi,7,0,0)
#define selectopt(x1,x2,x3)          selectxy(ySize-3,0,(x1),(x2),(x3))
#endif

#define VIDSPAIKA(y,x,t,t0) vidspaika_t((y),(x),(t),(t0),kilpparam.laika);
#define NORMT(t) (((t) + 48*TUNTI - enslahto) % (24*TUNTI))
#define NORMTT(t) normtt(t)
#define NIMIINT(x) nimilong((x))

struct perr_ptrs_tp {
	wchar_t *ots;
	wchar_t *msg;
	int vast;
	};

extern wchar_t *aptr;

#pragma pack(push,1)

typedef struct {
	int tagid;
	wchar_t *tag;
} tagListTp;

typedef struct {
   __int64 jarjtulos;
   INT16 dkilp;
} jrec;

typedef struct {
	bool koodiEil;
} localparamtp;

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
	int osrecsize0;
	int osrecsize;
	int vatpsize;
	wchar_t tiedlaji[8];
	int n_os;
	int n_os_akt;
	int osuusluku;
	int ntosuus;
	int maxnosuus;
	__int64 jtulosjak;
	__int64 jtulosaskel;
	__int64 minpuutelisa;
	wchar_t kilplaji;
	wchar_t alalaji;
	wchar_t badgelaji;
	int kaksibadge;
	int maxvaluku;
	int valuku;
	bool pyorlahimpaan;
	int tarkkuus;
	unsigned int laika;
	unsigned int laikasj;
	unsigned int laika2;
	unsigned int laikava;
	int pyor[5];             // 0: ei, 1: tall, 2: jarj, 3: tuloste, 4: väliaika
	bool sailhyl;
	int lnimi;
	int lseura;
	int lsakmax;
	int aikasakot;
	int sakkoyks;
	int etusuku;
	char chars;
#if  defined(AMPUSUUNN)
	wchar_t ryhma[2];
	INT16  ohi[2];
	INT16  mmsakko;
#endif
	int  vuosi;
	wchar_t kilpkoodi[25];
	wchar_t Name[62];
	wchar_t Date[14];
	bool WisV;
	int vuoromaali;
	int partio;
	bool rogaining;
	int analoolisa;
	wchar_t kartta[200];
	}  kilpparamtp;

typedef struct {
	INT16 alkukdi;
	INT16 loppukdi;
	INT16 piste;
	INT16 pohja;
	} mtbtp;

class sarjatietue {
public:
   char sarjanimi[LSARJA+1];
   char  psarjanimi[LPSARJA+1];
   INT  osuusluku;
   int  ntosuus;
   int  maxnosuus;
   int  aosuus[MAXOSUUSLUKU+1];
   int  nosuus[MAXOSUUSLUKU];
//   int  n_os[MAXOSUUSLUKU];
   int  yosuus[MAXOSUUSLUKU];
   __int64 puutelisa[MAXOSUUSLUKU];
   INT  sarjaalku;
   INT32 lahto;
   INT  valuku[MAXOSUUSLUKU];
   INT32 ylahto[MAXOSUUSLUKU];
   INT32 ylkynnys[MAXOSUUSLUKU];
   INT  lahtovali[MAXOSUUSLUKU];
   INT  lno;
   int  mno;
   INT tsak[MAXOSUUSLUKU];
   int paikat[MAXOSUUSLUKU];
   INT amptsak[MAXOSUUSLUKU];
   int maxpaikat;
   int taslaji;
   INT  maksu;
   INT  maksu1;
   INT  maksual;
   INT  tuljarj;
   bool piilotatulos;
   int  partiotls;
   int  knosiirto;
   char matka[MAXOSUUSLUKU][8];
   char va_matka[MAXOSUUSLUKU][VAIKALUKU][8];
   int va_koodi[MAXOSUUSLUKU][VAIKALUKU];
   int va_piilota[MAXOSUUSLUKU][VAIKALUKU];
   mtbtp mtb[MAXOSUUSLUKU][VAIKALUKU+1];
   bool laskemtb;

	sarjatietue(void);
	~sarjatietue(void);
	void nollaa(void);
	void operator=(sarjatietue&);
	bool operator==(sarjatietue&);
	int aOsuus(int yos, int aos);
	int Vaikaluku(void);
	wchar_t *Sarjanimi(wchar_t *st=NULL, bool pitka=false);
	wchar_t *pSarjanimi(wchar_t *st=NULL);
	wchar_t *Matka(int osuus, wchar_t *st=NULL);
	wchar_t *vaMatka(int osuus, int va, wchar_t *st=NULL);
	void tulkMTB(wchar_t *ln, int osuus);
   };

class seuraTieto {
public:
	char	 key[50];
	int 	 piiri;
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
		wchar_t *nimi, int kielto);
	seuraTieto *haenimella(wchar_t *nimi, bool tarkka = true);
};

class tulosmuottp {
public:
   int snimifl;
   int knofl;
   int tpvafl;
   int tklofl;
   int lkmfl;
   int otsikot;
   int etusuku;
   int nollakarki;
   int marg;
   int omarg;
   int ymarg;
   int amarg;
   int tabs;
   int numlev;
   int levunit;  // 0: merkkejä, 1: poins, 2: 1/1000 rivi
   int sivpit;
   int rivpit;
   int aikapari;
   int uusisivu;
   int esirivit;
	int tiiv_len;
	int tiiv_rivit;
	int tiiv_fontit;
	int tiiv_osno;
	int tiiv_arvo;
   int sarjalinkit; // 0: Ei, 1: on, 2: toistuu
   int tauluhtml;
   int levkerr;
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
	int tdjkcolor;
	int tdjkfontcolor;
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
	tulosmuottp(void);
	~tulosmuottp(void);
	void operator=(tulosmuottp&);
	void nollaa(void);
};

typedef struct {
   wchar_t rata[HAJONTA+1];
   wchar_t ratapit[6];
   void *seur;
   } hajtp;

class tulostusparamtp {
public:
	PRFILE *lstf;
	int  printer;
	wchar_t sline[10000];
	wchar_t kohde;       // Paperi, tIedosto (teksti), Html, Xml, eRottimin, Jatk-teksti
	wchar_t rajaus;	     // Tulos, Hyl., Kesk., Eil/sulj,
	wchar_t tulostettava;  // Joukkue, Osuus
	int  vaihelkm;
	int  osuus;
	wchar_t  viimos;      // Kaikki, Viim,
	int  piste;       // 0: maali, väliaika
	int  enssija;      //
	int  viimsija;     //
	bool hksijafl;     //
	int yhteislahdot;     // yhteislähtöön osallistuneet, 2: kaikista sarjoista
	bool tarkastamattomat;  //
	bool seuratulokset;     //
	bool piiritulokset;     //
	bool piirisijat;
	bool hajontatulokset;   //
	hajtp hajrata;
	int  sj_raja;           // piiritulosten kärki
	bool piilotatulos;      // ?
	bool piilotasijat;      // ?
	bool lahtoluettelo;     // ?
	bool lahtoajat;        // partion ym. lähtöajat
	bool nollaalaskuri;    // autotulostuksen laskurin nollaus
	int	 ixjarj;           //  0: vaihto, 10: osuus, 1: aakkos, 11: numero
	bool kaikkisijat;      //  ?
	bool kaikkiva;
	bool kaikkisarjat;
	void *p_fld;
	int vpos;
	int rivi;
	int kynnys;
	int optiot;
	tulosmuottp tulmuot;
	int language;
	wchar_t merkit;
//	bool ansi;
	int tulbadge;
	bool emitva;
	bool rastivali;
	bool monios;           // liittyy hajontatulostukseen
	bool tiivis;            //  tiivistetty moniosuuksinen
	bool yksihtml;         // sarjat yhteen tiedostoon
	bool htmlkno;          // rintanumero mukaan
	wchar_t title[122];
	wchar_t header[122];
	wchar_t htmlNimirunko[202];
//	bool af_flag;          // poistumassa
	wchar_t erotin;        // csv-tiedoston erotin
	wchar_t decsep;		   // desimaalierotin
	bool poistanollat;
	int automaali[9];
	INT16 sarjalista[MAXSARJALUKU+MAXYHD];
	wchar_t pistelista[VAIKALUKU+2];
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
	void Lihav_on(void);
	void Lihav_pois(void);
	void Pien_on(void);
	void Pien_pois(void);
	void Paaots_on(void);
	void Paaots_pois(void);
	void Aliots_on(void);
	void Aliots_pois(void);
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

class vatp {
public:
	INT32 aika;
	INT16 sija;

	void nollaa(void);
	void operator=(vatp&);
	bool operator==(vatp&);
/*
	vatp(void);
	~vatp(void);
	void pack(char *buf);
	void unpack(char *buf);
*/
	};

#define VASIZE sizeof(vatp)

class osrec {
public:
   char nimi[OSNIMIL+1];
   INT32 lisno;
   char osseura[OSSEURAL+1];
   char arvo[LARVO+1];
   char hajonta[HAJONTA+1];
   INT32 badge[2];
   char laina[2];
   union {
	   struct {
			char selitys[SELITYS+1];
			char piikit;
			char luonne;
			char ratk_1;
			char myonto;
			char ratk_2;
			};
	   struct {
			char ufill_1[3];
			int pisteet[3];
			};
	   };
   char uusi;
   char seuranta;
   UINT16 kone;
   INT16  ossija;
   INT32 ylahto;
   UINT8 lahtolaji;
   char keskhyl;
   char ampsakot[LSAKOT+1];
   INT32 sakko;
   vatp   va[VAIKALUKU+1];

   void nollaa(void);
   void operator=(osrec&);
   bool operator==(osrec&);
   int	 vert0(osrec &ostiet);
   char *Nimi(char* st, int len, int etusuku=0, char chars=' ');
   void setNimi(char *etunimi, char *sukunimi, char chars=' ');
   wchar_t *Nimi(wchar_t *st, int len, int etusuku=0, char chars=' ');
   wchar_t *SukuNimi(wchar_t *st, int len, char chars=' ');
   wchar_t *EtuNimi(wchar_t *st, int len, char chars=' ');
   void setNimi(wchar_t *etunimi, wchar_t *sukunimi, char chars=' ');
   void setNimi(wchar_t *nimistr, int etusuku=0, char chars=' ');
   void setNimi(char *nimistr, int etusuku=0, char chars=' ');
   void setEtuNimi(wchar_t *etunimi, char chars=' ');
   void setSukuNimi(wchar_t *sukunimi, char chars=' ');
   TMAALI Maali(int piste);
   INT16 Sija(int piste);
   INT16 osSija(void);
   void setMaali(int piste, int aika);
   void setSija(int piste, int sija);
   wchar_t Laina(int kortti=0);
   wchar_t Seuranta(void);
   wchar_t *Arvo(wchar_t *st=NULL);
   wchar_t *OsSeura(wchar_t *st=NULL);
   wchar_t *Selitys(wchar_t *st=NULL);
   wchar_t *Hajonta(wchar_t *st=NULL);
   wchar_t *Ampsakot(wchar_t *st=NULL);
   void setLaina(bool lainafl, int kortti=0);
   void setLaina(wchar_t wlaina, int kortti=0);
   void setSeuranta(wchar_t ch);
   void setSelitys(wchar_t *st);
   int setHajonta(wchar_t *st);
   void setAmpsakot(int i, int ohi);
   void setAmpsakot(wchar_t *st);
   };

#define OSRECSIZE sizeof(osrec)
#define OSRECSIZE0 (OSRECSIZE-(VAIKALUKU+1)*VASIZE)

class kilptietue {
public:
	INT16  kilpstatus;
	INT16  kilpno;
	INT32 paivitys;
	char seura[LSEURA+1];
	INT16 joukkue;
	INT16 sarja;
	char maa[4];
	INT32 joukkid;
	char joukknimi[LJK+1];
	INT16 piiri;
	INT16 ilmlista;
	int  pisteet[3];
	UINT16 era;
	char txt[14];
	UCHAR flag[4];
	osrec ostiet[MAXOSUUSLUKU];

	void nollaa(void);
	void operator=(kilptietue&);
	bool operator==(kilptietue&);
	int	 vert0(kilptietue &kilp1);
	int KilpNo(bool siirra = false);
	void setKilpNo(int kno, bool siirra = false);
	char *Nimi(char *st, int len, int osuus, int etusuku=0, char chars=' ');
	wchar_t *Nimi(wchar_t *st, int len, int osuus, int etusuku=0, char chars=' ');
	wchar_t *EtuNimi(wchar_t *st, int len, int osuus, char chars=' ');
	wchar_t *SukuNimi(wchar_t *st, int len, int osuus, char chars=' ');
	void setNimi(char *etunimi, char *sukunimi, int osuus, char chars=' ');
	char Tark(int osuus, int naytasulj = 0);
	char tTark(int osuus);
	bool Hyv(int osuus=-1);
	bool tHyv(int osuus=-1);
	bool osHyv(int osuus);
	bool Sulj(int osuus);
	bool tSulj(int osuus);
	void SetTark(int osuus, char trk);
	void setNimi(wchar_t *etunimi, wchar_t *sukunimi, int osuus, char chars=' ');
	void setNimi(wchar_t *nimistr, int osuus, int etusuku=0, char chars=' ');
	void setNimi(char *nimistr, int osuus, int etusuku=0, char chars=' ');
	void setEtuNimi(wchar_t *etunimi, int osuus, char chars=' ');
	void setSukuNimi(wchar_t *sukunimi, int osuus, char chars=' ');
	wchar_t wTark(int osuus, int naytasulj = 0);
	wchar_t wtTark(int osuus);
	wchar_t *TarkStr(int osuus, bool naytasulj = false, int lang = 0);
	wchar_t *tTarkStr(int osuus, int lang = 0);
	void SetTark(int osuus, wchar_t trk);
	char *Seura(char *st, int len=LSEURA, char chars=' ');
	void setSeura(char *seura, char chars=' ');
	wchar_t *Seura(wchar_t *st, int len=LSEURA, char chars=' ');
	void setSeura(wchar_t *seura, char chars=' ');
	wchar_t *Joukknimi(wchar_t *st, int len=LJK, char chars=' ');
	void setJoukknimi(wchar_t *st, char chars=' ');
	wchar_t *Joukkue(wchar_t *st, int len=LSEURA+5, char chars=' ');
	char *Joukkue(char *st, int len=LSEURA+5, char chars=' ');
	int yOsuus(int osuus);
	int aOsuus(int osuus);
	int Osuus(int yos, int aos);
	TMAALI Lahto(int osuus, int *laji = NULL);
	TMAALI Maali(int osuus, int piste);
	TMAALI Tulos(int osuus, int piste, bool sekvajaat = false);
	TMAALI osTulos(int osuus, int piste, bool vaadiJoukkueHyv=true);
	TMAALI tTulos(int osuus, int piste, __int64 *tlslisa = NULL);
	TMAALI aTulos(int osuus, int piste);
	TMAALI kokTulos(bool sekvajaat = false);
	int ASakkoLkm(int osuus);
	int TSakkoLkm(int osuus);
	int SakkoAika(int tosuus, bool kumul = false);
	INT16 Sija(int osuus, int piste);
	INT16 osSija(int osuus);
	void setMaali(int osuus, int piste, int aika);
	void setSija(int osuus, int piste, int sija);
	void settSija(int osuus, int piste, int sija);
	wchar_t *Maa(wchar_t *st=NULL);
	char *Maa(char *st);
	void setMaa(wchar_t *st);
	wchar_t Laina(int os, int kortti=0);
	wchar_t Seuranta(int os);
	wchar_t *Selitys(int os, wchar_t *st=NULL);
	wchar_t *Hajonta(int os, wchar_t *st=NULL);
	wchar_t *Ampsakot(int os, wchar_t *st=NULL);
	wchar_t *Arvo(int osuus, wchar_t *st=NULL);
	wchar_t *OsSeura(int osuus, wchar_t *st=NULL);
	void setLaina(int os, bool lainafl, int kortti=0);
	void setLaina(int os, wchar_t wlaina, int kortti=0);
	void setSeuranta(int os, wchar_t ch);
	void setSelitys(int os, wchar_t *st);
	int setHajonta(int os, wchar_t *st);
	void setAmpsakot(int os, int i, int ohi);
	void setAmpsakot(int os, wchar_t *st);
	void laskeKierrokset(void);
	wchar_t *tarkstr(wchar_t *st, int osuus, bool yhttul, int len, int lang);
	wchar_t *strval(wchar_t *st, int len, int knt, int osuus, int pst, int sj, int opt, int lang);
	void getrec(INT d);
	void putrec(INT d, bool tiedostoon=false);
	void Tallenna(int d, int tarbuffl, int kielto, int pakota_os, kilptietue *entkilp);
	char *pack(char *buf);
	void unpack(char *buf);
};

#define KILPRECSIZE sizeof(kilptietue)
#define KILPRECSIZE0 (KILPRECSIZE-MAXOSUUSLUKU*OSRECSIZE)
#define MAXKILPRECSIZE KILPRECSIZE

typedef struct {
	wchar_t tunnus[12];
	char rata[HAJONTA+1];
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

#if !defined(LUENTA)
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

typedef struct {
   INT16 rasti;
   INT16 koodi;
   int pisteet;
   } ekditp;

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
	int haeXmlKartta(maptp *map);
	int haeXmlKuviot(ekditp *kdi, rastikoordtp *koord, int nkoodi);
	int haeXmlRadat(ratatp *rt, int nrata);
};
#endif  // !LUENTE

typedef enum {
   Jk, Nm, Sra, Srj, Kno, Os, OsO, Tls, OTls, Ksk, Hl, El, Maika,
   Kaika, O1, O2, O3, O4, O5, O6, O7, O8, RNo
} tuntype;

typedef struct {
   tuntype tunnus;
   int ehto;
   int x, y, w;
   int df, rf;
} tarrak;

typedef struct {
   int r;
   int c;
   char *t;
   } line;

typedef struct {
   int nl;
   line l[18];
   } scr;

typedef struct {
	INT16 kdi;
	INT16 piste;
	} lahdepistetp;

typedef struct {
   int pos;
   int len;
   int oik;
   } FLD;

typedef struct {
   TMAALI t;
   INT32 date;
   INT16 kno;
   INT16 osuus;
   INT16 piste;
   INT16 kanava;
   INT32 badge;
   INT16 lahde;
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
	int fp;
	int fp1;
	int *aktrow;
	int aktrows;
	UINT8 atimemask;
	int aikakorjaus;
	int afhandle;
	FILE * fafhandle;
	wchar_t aikafname[102];
	aikajonotp(int lkm);
	~aikajonotp(void);
	void haeAktAjat(void);
	aikatp * getTime(aikatp *rtm, UINT32 rvno);
	aikatp * getAktTime(aikatp *rtm, UINT32 rvno);
	} ;

typedef struct {
   int kno;
   fpos_t ptr;
   } TIETOIX;

typedef struct {
   INT16 osuus;
   TMAALI aika;
   } valmistp;

#ifdef EMIT
//  EMIT-määrityksiä

// maalilajit: 1: ajanotto erikseen, viim. rastiväli tuloksesta
//             2: ajanotto erikseen, viim. rastiväli korjattu kortin aika
//             3: ajanotto RTR-2:lla
//             4: lopputulos kortilta viimeisen rastin ajasta

typedef struct {
	char key[8];
	int ero;
	int kilpno;
	int osuus;
	int pos;
	wchar_t kh;
} emiterotp;

class emittp {
   public:
   UINT32 package;
   INT32 badge;
   INT16 badgeyear;
   INT16 badgeweek;
   INT32 maali;          // RTR2-kellon ottama maaliaika
   INT32 time;           // kortin lukemishetki
   UCHAR ctrlcode[50];
   UINT16 ctrltime[50];
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
   };

typedef struct {
	INT16 kilpno;
	INT16 osuus;
	INT32 badge;
	INT32 tulos;
	INT16 rastiluku;
	char rata[8];
	INT16 ok;
	UINT16 rastit[MAXEMVALAH][2];
	} emitvatp;

typedef struct {
   INT32 badge;
   INT16 kilpno;
   } bdg_kno_tp;

typedef int rmaartp[120];

typedef struct {
   char hi;
   char lo;
	} hltype;

typedef struct {
   char code;
   UINT16 t1;
   } ctrltype;

#endif

#define R_BUFLEN 15000

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

typedef struct {                      /* Tiedonsiirtopaketti */
		INT32 lahetetty;
		INT16 portti;
		UCHAR      so,id,iid;         /* Paketin id */
		UCHAR      pkgclass;          /* Paketin laji */
		UINT16     len;               /* Paketin pituus */
		UINT16     checksum;          /* Tarkistussumma */
		union {
			struct {
				char		tunn;			// avaussanoman tunnus (ascii-arvo 1)
				char		konetunn[2];	// lähettävän koneen tunnuskoodi
				UINT8		vaihe;			// kilpailun vaihe, viestissä 0
				UINT16		nrec;			// tietueiden lkm
				UINT32		flags;			// flags
			    } alku;
			struct {
			  char           tarf;
			  char           pakota;      // Pakota tallennus ja lähetys
			  INT16          dk;          /* Osoite taulukossa kilparr */
			  INT16          entno;       /* Vanha kilpno, 0 : uusi */
			  char           ckilp[KILPRECSIZE0];     /* Tiedot */
			  } k;
		   struct {
			  char           tarf;
			  char           pakota;      // Pakota tallennus ja lähetys
			  INT16          dk;          // Osoite taulukossa kilparr */
			  INT16          kno;         // kilpno
			  INT16          osuus;       // Osuus (0 = perustiedot) */
			  UINT32		 muutos;	  // flagit, jotka kertovat sanoman sisällön
			  char           cos[OSRECSIZE0];     /* Tiedot */
			  } os;
		   struct {
			  char           tarf;
			  char           pakota;      // Pakota tallennus ja lähetys
			  INT16          dk;          // Osoite taulukossa kilparr */
			  INT16          kno;         // kilpno
			  INT16          osuus;       // Osuus (0 = perustiedot) */
			  char 			 ampsakot[LSAKOT+1];
			  INT16 		 sakko;
			  char 			 keskhyl;
			  } trk;
		   struct {
			  char           tarf;
			  char           pakota;      // Pakota tallennus ja lähetys
			  INT16          dk;          /* Osoite taulukossa kilparr */
			  INT16          kno;         // kilpno
			  INT16          osuus;       /* Osuus (0 = perustiedot) */
			  INT16          piste;       /* Piste (0 = vaihto/maali) */
			  char           cva[VASIZE];
			  UINT16         kone;
			  } p;
		   struct {
			  char           pakota;      // Pakota tallennus ja lähetys
			  aikatp         daika;
			  aikatp         iaika;
			  } a;
		   struct {
			  INT16          fl;
			  INT16          sarja;
			  INT32          ylahto[MAXOSUUSLUKU][2];
//			  INT32          kynnys[MAXOSUUSLUKU];
//			  UINT8          yltila[MAXOSUUSLUKU];
			  } y;
		   struct {
			  INT16          ekno;
			  INT16          eos;
			  INT16          kno;
			  INT16          os;
			  INT32			 badge;
			  } ev;
		   struct {
			  INT32          badge;
			  INT32          aika;
			  INT16          piste;
			  } ec;
		   struct {
			  INT32          d1;
			  INT32          d2;
			  INT32          d3;
			  INT32          d4;
			  INT32          d5;
			  } x;
		   struct {
			  UINT32		  flags;
			  UINT32		  koodi;
			  INT32		  	  pos;			  // -1: alku, >= 0: positio, -2: datan loppu
			  INT32			  len;			  // sisällön pituus
			  char		     buf[2];          // Alussa tiedoston nimi (suhteellinen polku), lopussa ei sisältöä,
		   	 } fl;
#if defined(EMIT)
		   emittp            em;
		   emitvatp			 emva;
#endif
		   } d;
		} combufrec;

#ifndef _CONSOLE
class rpistevatp {
public:
   int aika;
   INT16 sarja;
   INT16 kilpno;
   INT16 osuus;
   rpistevatp(void);
   ~rpistevatp(void);
   };

class ratavatp {
public:
	int npiste;
	rpistevatp *pisteva;
	ratavatp(void);
	~ratavatp(void);
	int lisaa(int kilpno, int sarja, int osuus, int nva, int aika);
	int poista(int kilpno, int osuus, int nva);
	};

class rastivatp {
public:
   ratatp *rata;
   int nva;
   int maxnva;
   ratavatp *ratava;
   rastivatp(void);
   ~rastivatp(void);
   void poista(int kilpno, int osuus);
   };

typedef struct {
	int aika;
	int sj;
	int kaika;
	} rastivaanaltp;

typedef struct {
   INT32 kdi;
   int matka;
   int maxlkm[MAXSARJALUKU][MAXOSUUSLUKU];
   int lkm[MAXSARJALUKU][MAXOSUUSLUKU];
   int sum[MAXSARJALUKU][MAXOSUUSLUKU];
   int vertaika[MAXSARJALUKU][MAXOSUUSLUKU];
   int voittaika[MAXSARJALUKU][MAXOSUUSLUKU];
   int *ajat[MAXSARJALUKU][MAXOSUUSLUKU];
   int *kilpno[MAXSARJALUKU][MAXOSUUSLUKU];
   } rvalitp;

typedef struct {
   int rkoodi;
	int aika;
   int rvaika;
   int rvno;
   } emitvaanaltp;
#endif


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
      char cr1;
      char n1;
      char s1;
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
      ctrltype c[50];
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
      ctrltype c[50];
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

#endif  // !LASKDEFDEFINED