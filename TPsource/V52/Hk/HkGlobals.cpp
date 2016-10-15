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
#include "HkDef.h"

	CRITICAL_SECTION idindex_CriticalSection;
	CRITICAL_SECTION tall_CriticalSection;
	CRITICAL_SECTION print_CriticalSection;
	CRITICAL_SECTION autotul_CriticalSection;
	CRITICAL_SECTION loki_CriticalSection;
	CRITICAL_SECTION inb_CriticalSection;
	CRITICAL_SECTION outb_CriticalSection;
	CRITICAL_SECTION kilpdat_CriticalSection;
	CRITICAL_SECTION ajat_CriticalSection;
	CRITICAL_SECTION emit_CriticalSection;
	CRITICAL_SECTION rastiva_CriticalSection;
	CRITICAL_SECTION tcpip_CriticalSection;
	CRITICAL_SECTION tcp_CriticalSection[MAX_PORTTI];
	CRITICAL_SECTION seura_CriticalSection;
	CRITICAL_SECTION seur_badge_CriticalSection;
	HTHREAD hTarkcomThread, hTsThread[MAX_PORTTI];
	HTHREAD hLuepakThread[MAX_PORTTI];
	HTHREAD taustaThread, hLkmThread;
	HTHREAD hAikacomThread;
	HTHREAD httphakuThread;
	HTHREAD hProsEmitThread;

//---------------------------------------------------------------------------
	int taustaon = 1, taustasulj = 0, inGDIdoc;
	int inLopetus;
	int katkaisuPyydetty;
	bool TestiToiminnot = false;
	int ok;
	wchar_t exeName[50];
#ifdef _DEBUG
	bool hav_virheet = true;
#else
	bool hav_virheet = false;
#endif
	struct {
		wchar_t afname[202];
		int autokomentolaji;
		wchar_t autokomento[202];
		wchar_t jakelumaar[202];
		bool vainMuutt;
		int afvali;
		bool af_kaikki;
		bool af_flag;
		wchar_t af_xmlva;
		wchar_t aflstnm[202];
		wchar_t sanafname[202];
		wchar_t sanafchars;
		int kopiofl;
		wchar_t wtitlea[202];
		wchar_t wheadera[202];
		} autofileparam;
	wchar_t WorkingDir[200];
	int nakviive = 1000;
	int ohitavirheet;
	int maxemvalah = MAXEMVALAH;
	int pros_emit_on;
	bool emitluenta = false;
	int emitTunnistus = 0;
	HTHREAD hPros_EmitThread;
	bool suljesuoraan = false;
	int siimport = -1;
	wchar_t sulkusalasana[12];
   INT  max_lahportti = MAX_LAHPORTTI, max_portti = MAX_PORTTI;
	int minvakanttiluku = 20;
	int lisavakanttialku;
	bool sailTiedPos = false;

#if !defined(MAASTO)
   wchar_t ch_piste[] = L"ALM123456789XY";
#else
   wchar_t ch_piste[] = L"ALMHK23456789X";
#endif
   wchar_t aika_jono[NREGNLY][16];
   wchar_t jonopiste[MAXJONO];
   INT  maxjono;
   INT32 tl_ylaraja = 3*SEK;
   INT tunn_srj = -1;
   INT tunn_raja = 3*SEK;
   char tyhjarivi[] = "                                              "
	  "                                  ";
   char dhigh[10] = "\x1B\x6A\x15\x1B\x68\x1";
   char shigh[10] = "\x1B\x68\x0\x1B\x6A\x3";
   INT  ndhigh = 6;
   INT  nshigh = 6;
#ifdef MAALI
   INT kierrosva[VALUKU+3];
	INT kierrosvafl;
	INT va_vuoro;
	INT vavuoro_ala = 1;
	INT vavuoro_yla = VALUKU;
#endif
	int JoKirjattuKesto = 0;
	INT  nktarra = 23;
	INT  wtarra = 40;
   INT  nrtarra = 9;
   char  sgl[] = "\x14";
   INT   nchsgle = 1;
   char  dbl[] = "\xE";
   INT   nchdble = 1;
   char  sgle[10] = "\x14";
	char  dble[10] = "\xE";
	wchar_t  alkulayout[200];
	wchar_t  dataf2name[200]    = L"KILP.DAT";
	wchar_t  dataf3name[200]    = L"KILP.DAT";
	wchar_t  datafbname[200]    = L"KILP.DAT";
	wchar_t  namefname[200]     = L"";
	wchar_t  sarjafname[200]    = L"KILP.SRJ";
	wchar_t  sarjaxmlfname[200]    = L"KilpSrj.xml";
	wchar_t  obfilename[200]    = L"COMFILE1.DAT";
	wchar_t lokifname[200] = L"LOKI1.LST";
	wchar_t leimafname[200] = L"LEIMAT.LST";
	wchar_t ratafname[200] = L"RADAT1.LST";
	wchar_t rataxmlfname[200] = L"RADAT1.XML";
	wchar_t aikaflname[2][200] = {L"AJAT1.LST" , L"LAJAT1.LST"};
	int ratatiedot;

	wchar_t  luetfnm[40] = L"TULLUET.FMT";
	wchar_t  ll_luetfnm[40] = L"LAHTLUET.FMT";
	wchar_t tarstr[14][13] = {
						L"",
						L"Keskeytt.",
						L"Hylätty",
						L"Ei läht.",
						L"Tasoitus =",
						L"Sakko =",
						L"Korj.",
						L"Maali:",
						L"Kirj.:",
						L"Poista",
				  L"--.--.--",
						L"Puuttuu",
				  L"Virhe",
						L""
				 };
   tarrak kentta[] = {
	  Srj,    0,  1,  1, 9, TRUE, FALSE,
      O2,     1, 19,  1, 6, FALSE, FALSE,
      Maika,  1, 25,  1, 8, FALSE, FALSE,
      O3,    -1, 19,  1, 6, FALSE, FALSE,
	  Kaika, -1, 25,  1, 8, FALSE, FALSE,
      O1,     5, 33,  1, 5, FALSE, FALSE,
      Nm,     0,  1,  3,19,  TRUE, FALSE,
      Lis,    0, 21,  5, 5, FALSE,  TRUE,
      Sra,    0,  1,  5,10,  TRUE, FALSE,
		Tls,    1,  1,  6, 8,  TRUE, FALSE,
		TlsY,  11,  1,  7, 8,  TRUE, FALSE,
		Ksk,    2,  1,  6, 8,  TRUE, FALSE,
		Hl,     3,  1,  6, 8,  TRUE, FALSE,
		El,     4,  1,  6, 8,  TRUE, FALSE,
		Taso,   6, 18,  6,10, FALSE,  TRUE,
		Tas,    6, 30,  6, 2, FALSE, FALSE,
		Sako,   7, 18,  6,10, FALSE,  TRUE,
		Sakko,  7, 30,  6, 2, FALSE, FALSE,
		O4,     8, 27,  5, 6,  TRUE, FALSE,
		O5,   -11,  1,  7, 8,  TRUE, FALSE,
		O6,     9,  1,  6, 8,  TRUE, FALSE,
		O7,    10,  1,  6, 8,  TRUE, FALSE,
#ifdef MAALI
		RNo,    0, 34,  3, 4, FALSE,  TRUE,
#else
		Nm,     0,  1,  3, 0,  TRUE, FALSE,
#endif
		Nm,     0,  1,  3, 0,  TRUE, FALSE,
		Nm,     0,  1,  3, 0,  TRUE, FALSE,
		Nm,     0,  1,  3, 0,  TRUE, FALSE

   };


#ifdef _CONSOLE
   int ToimintaTila = 2;      // O: kilpailua ei avattu, 1: esivalmistelu, 2: tulospalvelu
#else
   int ToimintaTila = 0;      // O: kilpailua ei avattu, 1: esivalmistelu, 2: tulospalvelu
#endif
	envdatatp EnvData;
   bool poistaSulje = true;
   INT k_pv, arv_pv;
   int vaihevalinta;
   INT valparas;
   bool vaiheenvaihto;
	sarjatietue Sarjat[MAXSARJALUKU+MAXYHD];
   INT32 pkarki[MAXSARJALUKU+MAXYHD][VALUKU+2];
   kilptietue *kilparr;
   INT ilmkentat;
   int kuntosuunnmoodi;
   INT vakanttialku = 0;
   int seurVapaaBadge = 1;
	bool ProsEmitOn = false;
//#if defined(MERI5)
//	wchar_t *lajinimet[5], *lajinm[5] = {L"Obst", L"Lifesv", L"Seam", L"Util", L"CC"};
	int meri3 = 0;
	INT32 m3vert[5][MAXSARJALUKU];
//#endif
	int sotilas;
	INT timerfl;
	INT selostaja;
	INT bootfl;
	INT kfirstfree;
	INT nrec;
	INT maxrec;
	datafile datf2,datf3;
//	INT *posarr;
	jarjtulostp *jarjtulos[MAXYHD+1][VALUKU+4];
//	INT *jarjseur[MAXYHD+1][VALUKU+4];
	INT16 *jrjseur[MAXYHD+1];
   INT xmlsarja = -1;
   wchar_t tcptulos = L'M';
   INT server_on;
#if defined(TAULU) || defined(TAULU_2)
	int vaintaulu;
   int udptaulu;
#endif
	kilpindex  ixarr[NINDEX];
	kilpindex  *ljarjindex,  *sarjaindex;
	kilpindex  *srjaakindex,  *seuraindex, *seuranoindex;
	kilpindex  *lahtoindex, *aakkosindex;
	kilpindex  *nroindex, *srjnroindex, *idindex;
	kilpindex  *rataindex;
	kilpindex nameindex;
	kilpindex pstindex;
	kilpindex vaihetlsindex;
	int reindex, finfirst, lahtojarjfl, takaa_ajofl;
	INT keylen;

	wchar_t  kilpailu[83];
	tulostusparamtp lsttulparam;
	tulostusparamtp tartulparam;
	tulostusparamtp lokitulparam;
	tulostusparamtp aftulparam;
	tulostusparamtp *emittulparam;
	tulostusparamtp autotulostusparam;
   	tulostusparamtp sivutulostusparam;
	bool lisaToimetOn;
	INT prblockbits;
	INT origverify;
	INT esta_tall;
	char konetunn[3];
	wchar_t naapuri[MAX_PORTTI][3] = {L"On"};
	INT talltaul = FALSE;
	INT iono = 0;
	INT tarfl;
	INT eiltarra = TRUE;
   INT lahtarfl = FALSE;
   INT hptarra, ktarra, rtarra = 2 , ntarra = 16, ftarra = 16;
	INT listfl = FALSE;
   INT yvfl = FALSE;
   INT lisaa_edelle;
//	UINT  laika = 8;
//	UINT  laika2 = 8;
//	INT  pyor_t = 1;
//	INT  pyor_k = SEK;
	char dec_sep = L',';
	wchar_t wdec_sep = L',';
	INT  tarknofl = 0;
	INT lajat_p;
	INT autofl = FALSE;
	INT pscr;
	wchar_t *psinit;
	INT innpfl;
	INT  kynnys = 10;
	INT  pfl = FALSE;
	 INT stopprt = FALSE;
   INT verify = -1;
   wchar_t listfname[201] = L"NUL";
   wchar_t tarfname[201] = L"NUL";
   INT  t0 = 12;
   INT32  t0tics = (12 * DAYTICKS) /24;
	int t0_regnly = -12*TUNTI;
	int t0_regn[NREGNLY];
   INT  minkilpno = 1;
   INT  maxkilpno = MAXKILPNO;
	INT  sarjaluku;
	INT  maxlsak[N_PV];
	INT nsarjayhd, nsarjayhd_tul, sarjayhdva[MAXYHD];
   wchar_t yhdsarjat[MAXYHD][MAXSARJALUKU];
//   wchar_t yhdnimet[MAXYHD][LSARJA+1];
   INT  vainalarajat = -1;
   INT  vainvaliajat;
   INT  pistefl;
   INT  ol_piste = -2;
   wchar_t *header = L"";
   INT  kaynnistysvaihe;
   INT  nilmt;
   INT  ntulost;
   INT  nhylt,nkeskt;
   INT  neilt, nvakt;
   INT  nva[VALUKU];
   INT  nilm[MAXSARJALUKU+MAXYHD];
   INT  nyilm[MAXSARJALUKU+MAXYHD];
   INT  ntulos[MAXSARJALUKU+MAXYHD][VALUKU+2];
   INT  nhyl[MAXSARJALUKU+MAXYHD];
   INT  nkesk[MAXSARJALUKU+MAXYHD];
   INT  neil[MAXSARJALUKU+MAXYHD];
   INT  nvak[N_PV][MAXSARJALUKU+MAXYHD];
   INT  neilaika[MAXSARJALUKU+MAXYHD];
   int  nilm_pv[N_PV][MAXSARJALUKU+MAXYHD];
   INT16  *yhdsija[MAXYHD][VALUKU+2];
   INT  backfl;
   INT  nimiixfl;
   INT  korjfl;
   INT  initkehys;
   INT  initform;
   INT  fprior[MAXSARJALUKU+MAXYHD];
   INT  prior[MAXSARJALUKU+MAXYHD];
   INT  prior0[MAXSARJALUKU+MAXYHD];
   bool vaaditarkastus;
   INT  tauko;
   INT  tofs;
//   char copybuf[CBUFL];
   int  maxyhteys;
   int portbase = PORTBASE;
   INT  portti[MAX_PORTTI];
   INT  comtype[MAX_PORTTI];
   ipparamtp ipparam[MAX_PORTTI+1];
   int  broadcasts;
#ifdef TCPSIIRTO
   INT  fileyht = -1, tcpyhteys;
   long tcpkaynnistys[MAX_PORTTI+1];
#endif
   char trlate[256];
   INT  trlst = 1,trtar,trloki;
   INT  f_ansi = 1;
   INT  iobufr = 90;
   INT  iobufmaxnr = 9;
   INT  kaksoistar = TRUE;
   char savebox[400], errbox[400];
   INT  tulfl = FALSE;
   INT  comfl = FALSE;
   INT  tcomfl = TRUE;
   INT  estavalitys[MAX_LAHPORTTI];
   INT  immed = TRUE;
   UINT32 kirjheti = 0xffffffffL;
   INT  comopen[MAX_PORTTI];
   int  comclose[MAX_PORTTI];
   INT  lahec[MAX_LAHPORTTI];
   INT  lahkello[MAX_LAHPORTTI];
   INT  lahkelloraja[MAX_LAHPORTTI];
   bool  lahkellovain[MAX_LAHPORTTI];
   INT baud[MAX_PORTTI] = {7};
   char pty[MAX_PORTTI] = {'n'};
   int combits[MAX_PORTTI] = {8};
   int stopbits[MAX_PORTTI] = {1};
   INT comkesto[MAX_LAHPORTTI];
   INT nprtch = 0;
   INT obfile = -1, levypusk = 1;
   FILE *fobfile;
   INT inbens;
   INT inbseur;
   INT32 cjens[MAX_LAHPORTTI];
   INT32 cjseur[MAX_LAHPORTTI];
   INT lah_alku[MAX_LAHPORTTI];
   INT lah_vaihto[MAX_LAHPORTTI];
   INT tarlahcom[MAX_LAHPORTTI] = { 1, 1 };
   INT lahcom[MAX_LAHPORTTI];
   INT vastcom[MAX_LAHPORTTI];
   INT yhteys_on[MAX_LAHPORTTI];
   int lah_tiedostot[MAX_LAHPORTTI];
   bool lahkaikkitied;
   INT dcom = 0;
   int jonokynnys = 5;
   int monitoriLkm;
   virhesantp virhesanoma;
   INT taulu_com, taulu_irq, taulu_addr, taulu_baud = 7;

   int bytecount, bytecountmax = 2000;
  INT ennakoi;
   int aikaero[NREGNLY+1];
   aikajonotp *aikajono[MAXJONO];
   int  muut_time;
   INT od_fl, lajatfl, akttm, taustaaika, yhtjonot;
   INT odota;
   INT32 od_aika;
   INT modif[MAXSARJALUKU+MAXYHD];                               /* 25.1.88 */
   char tlsmuutos[MAXSARJALUKU][VALUKU+1];
   INT timefl;
   INT n_lisaa = 1000;
   INT kunto, seurvapaa = 8000;
   INT loki, lokiTCP;
   HANDLE hLoki;
   INT com_ajat[MAX_LAHPORTTI];
   INT32 rukkausvali,rukkaa;
   INT edista;
	INT ajat_on, status_on = 1;
	INT kuulpiste;
	INT  pakotalaika;
	INT  html_sail_nimi;
//	INT  takaa_ajo;
   INT  nyilmt;
   INT  nytulost;
//   INT  nytulos[MAXSARJALUKU];
	INT  sjfl[MAXSARJALUKU+MAXYHD];
   INT  jalku[MAXSARJALUKU+MAXYHD][11+VALUKU];
   INT  va_kello = 1;
   INT  va_kuollut = 5 * SEK;
   INT  yhtlahto;
#ifdef AUTOVA
   INT vaikavert, ilmtunt = 1, vaajat;
#endif
#ifdef TV1
   INT tv_fl, tv_test;
   INT tv_com, tv_baud = 5;
	int cn_tv;
#endif
   INT lukum_fl = 1;
   char form[65*(MAXRIVI-6)], kehys1[15*(MAXRIVI-3)];
   char aika_int_mask = 17;
   INT aika_esto=2;
	INT kello_esto = 0;
	INT vainoletus;
   INT maxntime;
   INT mxtime[MAXJONO];
   INT ajanottofl = -1, sail_ajat;
#ifdef MAALI
   INT syotto;
#ifndef NOCOM_AIKA
   INT aika_com=0;
   INT com_aika;
#endif
#ifndef NOREGNLY
   INT regnly[NREGNLY];
   int regnlyhetki[NREGNLY];
   INT port_regnly[NREGNLY];
	int regnly_open[NREGNLY];
	INT cn_regnly[2*NREGNLY];
   INT racpisteet;
	INT estaylim, ylimraja, estaneg, uusinaika, samapisteraja = 10 * SEK;
#if defined(UINTIxx)
	eratp *erataul;
	INT aika_tunnus[NREGNLY][45] = {-2, -2, -2, -1, 1, 4, 7, 3, 6, -2, 2, 5, 8, -2, -2};
#else
	INT aika_tunnus[NREGNLY][45] = {-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2};
#endif
	INT swimlaji = 0;
	INT aika_tunnus_lkm = sizeof(aika_tunnus[0])/sizeof(aika_tunnus[0][0]);
	lahdepsttp *lahdepisteet[MAXSARJALUKU+1];
	lahdepsttp *lahdejonot[MAXSARJALUKU+1];
   INT kello_baud, regnly_no[NREGNLY], kello_bits = 8;
   INT sak_com, sak_baud = 8, sak_laji = 2;
   INT od_regnly = 10;
   INT32 maxero = 1000000L;
   INT kerro_regnly;
	int usb_regnly[NREGNLY];
   INT pyyntoviive = 200;
   int aikaTCPstatus;
#endif
#endif  // MAALI
#if defined(TXTTV)
   int tv_com = 1, tv_baud = 7;
   int tv_pty = 2, tv_bits = 2, tv_fc = 1;
   int tv_open, suoracom, tv_test;
#endif
#ifdef MAASTO
   INT32 kl_kaava = 20000;
   INT32 kl_ohjeaika;
   INT kl_x[KL_LUKU], kl_y[KL_LUKU];
   INT osarja[4][MAXSARJALUKU];
#endif
	INT32 eralahto[MAXERA] = {AIKAJAK*TMAALI0};
   INT16 era_akt;
   INT era_lahdot;

//	tunnus, nimi[40], kerroin, jakaja, vertaika, vertpiste, yks
	sotlajitp sotlajit[10] = {
		L'H', L"Ammunta", -7, 100, 262860, 300, 10,
		L'O', L"Esterata", -7, 100, 302860, 0, 10,
		L'W', L"Esteuinti", -24, 100, 73170, 0, 10,
		L'T', L"Heitto", -4, 1000, 3800000, 3000, 10,
		L'C', L"Juoksu", -2, 100, 1340000, 0, 10,
		L'O', L"Esterata", -1, 200, 350000, 0, 1,
		L'L', L"Pelastusuinti", -1, 200, 300000, 0, 1,
		L'U', L"Uinti", -1, 100, 195000, 0, 1,
		L'F', L"Merimiestaito", -1, 500, 890400, 0, 1,
		L'C', L"Maastokisa", -1, 500, 1160400, 0, 1
		};
	int emitjono;
	int elahtojono = -1;
	int tunnjono = -1;
	int tunnrajat[2] = {-5, 2};
   bdg_kno_tp *bdg_kno;
   INT emitfl;
   INT nbadge;
   INT emitloki;
   INT *emitjarr[2];
	INT emittime;
	int pollaa, TCPpoll[NREGNLY+1];
	INT  ohitatoisto = -1;
	INT  siritreuna = 1;
	bool siritloki = false;
	bool siritsync = false;
	int sirithead, sirittail;
	wchar_t siritbuf[SIRITBUFLEN+1];
	int muutoskysely = 1;
   int valonaytto;
   INT com_emit[MAX_LAHPORTTI];
   int  emitfile;
	wchar_t emitfname[13] = L"EMIT1.DAT";
	INT emithead, emithead2, annaseur;
	INT com_emitva[MAX_LAHPORTTI];
	FILE *femitfile;
	INT estaemittoisto = 1;
	int sallisummavirhe, lukutoisto = 2;
	int emitajat, lahtopoikk;
	int eNollausAjat;
	int emitaglisa = 1;
	bool emitvaloki;
	IOFCourseData CourseData;
	bool DeveloperMode = false;

#ifdef MAKI
	int aktkilpailija;
	int kilpailijataulu = 1;
	int tuomari_yht;
	int yhdistetty;
	int aktkierros;
	int viimkierros = 2;
	int makipv = -1;
	int nkierros[MAXSARJALUKU];
	int ptspermin = 12;
	int tuomariluku = 5;
	int pituuskerroin = 18;
	int kpiste = 1200;
	int pituuscomet = 5, nopeuscomet = 6;
	int cometluku = 7;
	wchar_t makiotsikko[TAULU_RIVPIT+1];
	int makihaku;
	int autoselostaja;
	int yhteispisteet;
	int yhdist_katto = 4000;
#endif


	INT16 xSize = 80, ySize = 25;
	INT spoolfl = FALSE;
	INT aani = 1;
	//int ok;
#ifdef _CONSOLE
	INT16 errMsgBox = 0;
#else
	INT16 errMsgBox = 1;
#endif

	int  tmaali, maaliajat[10];

	INT  ajatfl, ilmlainat, emitkd, relax;
	int  monirata = 2;
	int  emitsivu;
	INT ohitaleimat, eilavoin;
	int maalileimasin;
	ratatp *rata;
	INT  nrata;
	//  kaikki_ajat[0]=  0: ei emit-ajanottoa, 1: sekä leimat että ajanotto, 2: vain ajanotto
	//  kaikki_ajat[r_no+1]= 0: laitteelta r_no+1 ei ajanottoa, >=1: laite r_no+1 ottaa aikoja
	int  kaikki_ajat[NREGNLY+1];
	int  vaadiaika, esitark, esiluenta, vainpiste[NREGNLY+1], luesuoraan;
    int  lukijalahde[NREGNLY];
	INT  luesuoraanviive = 20;
	INT  autosarja, autorata, nesitt, nesit[MAXSARJALUKU+MAXYHD];
	wchar_t maalitp[10];
	INT emitptr = -1;
	UINT  *emitpos;
	INT autorap, comautorap;
	wchar_t autorapsrj[MAXSARJALUKU];
	INT ennakko;
	wchar_t viimtark;
	INT autokilp;
	INT seurkrivi;
	INT selaus;
	INT lahdepistehaku = 0;
	int lkmThreadOn;
//	OSVERSIONINFO osvi;
	kilpparamtp kilpparam;
	bool rataTunnistus = true;

   int datatallfl[2] = {1, 0};
   int in_data;
//   rvalitp *rvalit;
//   int rvluku;

	wchar_t fnameRacelist[] = L"KilpailuLuettelo.xml";
	wchar_t seuraTiedNm[200] = L"seurat.csv";
	int KuntoMaali = 0;

	pvparamtp pvparam[N_PV];
	int NimiHTHaku = -1;
	int seurahaku = -1;
	bool vahvistaaina = true;
   int emitanal_fl;
   bool origemvabadge = false;
   int (*korRaja)[2];
   wchar_t *vipLaji;
   unsigned int korVari[6] = {
		65280, 65535, 16711935, 116763572, 16776960, 32896
		};
	int bibsiirtoalue = 8000;

	int inhttpHaku;

	eThakuParamtp eTParam = {
		NULL, 					//	char *buf;
		0, 						//	int buflen;
		L"195.159.103.189",		//	wchar_t eThost[100];
		1379,					//	int eTport;
		{0, 0, 0, 0},			//	int eTserial[4];
		54, 					//	int eThakuvali;
		-1,						//	int eTrno;
		0,						//	int eThaku;
		L"",					//	wchar_t eTdate[12];
		L"05:00:00",			//	wchar_t eTtime[10];
		0,						//	int msgno;
		0};						//	int haettu;

	sqlparamtp sqlparam = {
		L"MYSQL",
		L"libsqlmys.dylib",
		L"libmysql.dll",
		L"libmysqlclient.dylib",
		-1,
		L"kilp",
		L"localhost",
		L"0000",
		L"hkkisawin",
		L"kilp",
		3306,
		false,
		false,
		false,
		false};

	sqlparamtp sqlEparam = {
		L"MYSQL",
		L"libsqlmys.dylib",
		L"libmysql.dll",
		L"libmysqlclient.dylib",
		-1,
		L"ecb",
		L"localhost",
		L"0000",
		L"emitag",
		L"EmitRO",
		53306,
		false,
		false,
		false,
		false};

	bool inkirjSQLThread = false;

int testi;

#ifdef DBGFILE
int dbgtofile;
#endif
