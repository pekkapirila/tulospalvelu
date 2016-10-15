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
#include "VDef.h"

   CRITICAL_SECTION tall_CriticalSection;
   CRITICAL_SECTION loki_CriticalSection;
   CRITICAL_SECTION print_CriticalSection;
   CRITICAL_SECTION autotul_CriticalSection;
   CRITICAL_SECTION inb_CriticalSection;
   CRITICAL_SECTION outb_CriticalSection;
   CRITICAL_SECTION kilpdat_CriticalSection;
   CRITICAL_SECTION ajat_CriticalSection;
   CRITICAL_SECTION emit_CriticalSection;
	CRITICAL_SECTION rastiva_CriticalSection;
   CRITICAL_SECTION tcp_CriticalSection[MAX_PORTTI];
   CRITICAL_SECTION tcpip_CriticalSection;
   HTHREAD hTarkcomThread, hTsThread[MAX_PORTTI];
   HTHREAD hLuepakThread[MAX_PORTTI];
   HTHREAD taustaThread, hLkmThread;
   HTHREAD hAikacomThread;
	HTHREAD hProsEmitThread;
	int ToimintaTila = 2;
	envdatatp EnvData;
    wchar_t exeName[50];
	bool DeveloperMode;
	wchar_t WorkingDir[200];
	bool poistaSulje = true;
	int inLopetus;
	int katkaisuPyydetty;
	bool sulkemassa;
	bool suljesuoraan;
   long tcpkaynnistys[MAX_PORTTI+1];
//   extern HANDLE hStdIn, hStdOut, hConIn, hConOut;
   bool kuuluttaja;
   bool naytavaroitukset;
   int taustaon = 0;
   int inGDIdoc;
   int lkmThreadOn;
   wchar_t *autokomento;
   INT spoolfl = FALSE;
   INT aani = 1;
#ifdef _CONSOLE
   INT16 errMsgBox = 0;
#else
   INT16 errMsgBox = 1;
#endif
   INT16 xSize = 80, ySize = 25;
   int testi;
   int nakviive = 1000;
	int maxemvalah = MAXEMVALAH;
	bool emitvafl;
	bool emitvaloki;
#ifdef EMITLEIMAT
   int pros_emit_on;
   int emitfiletest;
   int lahdepistehaku;
#endif
	int sisaanluenta;
	bool RastiVaTuloste;
	int analmaxosuuskesto = 10800;
   int kbdwait;
   INT16 osd = OSD;

kilpparamtp kilpparam;

sarjatietue Sarjat[MAXSARJALUKU+MAXYHD];

void ajanotto(void);

	bool ProsEmitOn;
	wchar_t sulkusalasana[12];
   INT  max_lahportti = MAX_LAHPORTTI, max_portti = MAX_PORTTI;
   int chkstk;
   unsigned kone;
   int lisaa_edelle;
//   int laika = 8;
   char dhigh[10] = "\x1B\x6A\x15\x1B\x68\x1";
   char shigh[10] = "\x1B\x68\x0\x1B\x6A\x3";
   int  ndhigh = 6;
   int  nshigh = 6;
   char  sgl[] = "\x14";
   extern int   nchdble;
   extern int   nchsgle;
   extern char  sgle[10];
   extern char  dble[10];
   int   nchsgle = 1;
   char  dbl[] = "\xE";
   int   nchdble = 1;
   char  sgle[10] = "\x14";
   char  dble[10] = "\xE";

   wchar_t  alkulayout[40];
   char  dataf2name[]    = "KILP.DAT";
   char  dataf3name[]    = "KILP2.DAT";
   char  datafbname[]    = "KILP3.DAT";
   char  namefname[]     = "";
   char  sarjafname[]    = "KILP.SRJ";
	wchar_t  sarjaxmlfname[200]    = L"KilpSrj.xml";
	wchar_t rataxmlfname[200] = L"radat.xml";
   wchar_t leimafname[200] = L"leimat.lst";
	wchar_t ratafname[200] = L"radat.lst";
   wchar_t  obfilename[40]    = L"COMFILE.DAT";
   wchar_t  luetfnm[40] = L"TULLUET.FMT";
   wchar_t lokifname[62] = L"LOKI.LST";
   wchar_t aikaflname[200] = L"AJAT.LST";
   wchar_t emitfname[13] = L"EMIT.DAT";
   char  tarstr[13][13] = {
				  "",
				  "Keskeytt.",
				  "Hylätty",
				  "Ei läht.",
				  ". os",
				  "Korj.",
				  "Maali:",
				  "Kirj.:",
				  "Poista",
				  "",
				  "",
				  "",
				  ""
             };
#ifdef MAALI
   int  nktarra = 18;
#else
   int  nktarra = 17;
#endif
   extern int iobufr;
   int iobufr = 90;
   int iobufmaxnr = 9;
   int kaksoistar = TRUE;
   int wtarra = 40;
   int nrtarra = 9;
   tarrak kentta[] = {
      Srj,    0,  1,  1, 8, FALSE, FALSE,
      Os,     0, 10,  1, 2, FALSE,  TRUE,
	  OsO,    0, 12,  1, 4, FALSE, FALSE,
      O2,     1, 19,  1, 6, FALSE, FALSE,
      Maika,  1, 25,  1, 8, FALSE, FALSE,
	  O3,    -1, 19,  1, 6, FALSE, FALSE,
	  Kaika, -1, 25,  1, 8, FALSE, FALSE,
      O1,     5, 33,  1, 5, FALSE, FALSE,
      Kno,    0,  1,  3, 4,  TRUE,  TRUE,
      Jk,     0, 12,  3,13,  TRUE, FALSE,
      Nm,     0,  1,  5,21, FALSE, FALSE,
	  Tls,    1, 21,  7, 8,  TRUE,  TRUE,
      Ksk,    2, 21,  7, 8,  TRUE, FALSE,
	  Hl,     3, 21,  7, 8,  TRUE, FALSE,
      El,     4, 21,  7, 8,  TRUE, FALSE,
      OTls,   7,  1,  7, 8, FALSE,  TRUE,
      O4,     6, 23,  5, 6,  TRUE, FALSE, 
#ifdef MAALI
      RNo,    0, 34,  3, 4, FALSE,  TRUE,
#else
	  Nm,     0,  1,  3, 0,  TRUE, FALSE,
#endif
	  Nm,     0,  1,  3, 0,  TRUE, FALSE,
      Nm,     0,  1,  3, 0,  TRUE, FALSE, 
      Nm,     0,  1,  3, 0,  TRUE, FALSE,
	  Nm,     0,  1,  3, 0,  TRUE, FALSE,
      Nm,     0,  1,  3, 0,  TRUE, FALSE
   };

   char tyhjarivi[81] = "                                             "
      "                                   ";
   char *kilparr;
   int maxrec = 3000;
   int njrec;
   int kaynnistysvaihe;
   datafile datf2;
   FILE *fdatf2;
//   int ok;
   kilpindex nameindex;
   int keylen;
	localparamtp localparam;
	tulostusparamtp lsttulparam;
	tulostusparamtp filetulparam;
	tulostusparamtp tartulparam;
	tulostusparamtp lokitulparam;
	tulostusparamtp aftulparam;
	tulostusparamtp *emittulparam;
   wchar_t  kilpailu[83];
   char tcptulos = 'M';
   INT server_on;
   int lahhajonta;
   int prblockbits=12;
   int origverify;
   int lataus = 1;
   int bootfl;
   int esta_tall;
   char konetunn[3];
   wchar_t naapuri[MAX_PORTTI][3] = {L"On"};
   int jjfl = FALSE;
   int talltaul = FALSE;
   int iono = 0;
   int tarkfl = 0;
   int tarfl = 0;
   int eiltarra = FALSE;
   int lahtarfl = FALSE;
   int listfl;
   int autofl = FALSE;
	struct {
		wchar_t afname[202];
		int aflaji;
		int autokomentolaji;
		wchar_t autokomento[202];
		wchar_t jakelumaar[202];
		bool vainMuutt;
		int afvali;
		bool af_flag;
		wchar_t aflstnm[202];
		wchar_t sanafname[202];
		wchar_t sanafchars;
		wchar_t wtitlea[202];
		wchar_t wheadera[202];
		} autofileparam;
   int pscr;
   wchar_t *psinit;
   int  kynnys = 10;
   int  pfl = FALSE;
   int stopprt = FALSE;
   wchar_t listfname[81] = L"NUL";
   int  t0=12;
   long t0tics = (12L * DAYTICKS) / 24L;
   TMAALI  enslahto = TMAALI0;
   TMAALI  tmaali, maaliajat[10];
   int  nilmt;
   int  nilmtt=0;
   int  minkilpno = 1;
   int  maxkilpno = MAXKILPNO;
   int  maxosuusluku;
   int  sarjaluku;
   int  nsarjayhd, nsarjayhd_tul, sarjayhdva[MAXYHD];
   wchar_t yhdsarjat[MAXYHD][MAXSARJALUKU];

   int  osuus[MAXSARJALUKU+MAXYHD+1];
   int  nilm[MAXSARJALUKU+MAXYHD+1];
   int  varkynnys[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int  lkm_srj, lkm_os;
   wchar_t *header=L"";
   int  ntulost[MAXOSUUSLUKU+1];
   int  nhylt[MAXOSUUSLUKU+1],nkeskt[MAXOSUUSLUKU+1];
   int  neilt[MAXOSUUSLUKU+1],nost[MAXOSUUSLUKU+1];
   int  navoint[MAXOSUUSLUKU+1];
   int  ntulos[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU][VAIKALUKU+2];
   int  ntulosix[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU][VAIKALUKU+2];
   int  tlsmuutos[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU][VAIKALUKU+2];
   int  nekoodi[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int  ntulos2[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU][VAIKALUKU+1];
   int  ylmyoh = 1;
   int  njoukhyl[MAXSARJALUKU+MAXYHD+1];
   int  njoukkesk[MAXSARJALUKU+MAXYHD+1];
   int  nhyl[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   int  nkesk[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   int  neil[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   int  navoin[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   int  nylahto[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   int  sjfl[MAXSARJALUKU+MAXYHD];
   INT jalku[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU][VAIKALUKU+6];
   __int64 *jarjtulos[MAXYHD+1][MAXOSUUSLUKU][VAIKALUKU+4];
   INT16 *jrjseur[MAXYHD+1];
   INT josalku[MAXSARJALUKU+MAXYHD+1][MAXOSUUSLUKU];
   __int64 *jarjostulos[MAXYHD+1][MAXOSUUSLUKU];
   INT16 *jrjosseur[MAXYHD+1];
   int  *kilpnoix;
   int  nimiixfl;
   int  korjfl;
   int  initkehys;
   int  initform;
   int  enstls[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int  automaali[9] = {1,1,1,1,1,1,1,1,1};
   int  fprior[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int  prior[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int  prior0[MAXSARJALUKU+MAXYHD];
   int  tauko;
   char laajuus;
   int  nparas;
   int  tofs;
   int  wincom;
   int  maxyhteys;
   int  portbase = PORTBASE;
   int  portti[MAX_PORTTI];
   char ch_pty[6] = "noems";
   int  maxnblock;
   char nprch;
   char trlate[256];
   int  trlst, trtar, trloki;
   char form[65*(MAXRIVI-5)];
   char kehys1[5000];
   char savebox[65*(MAXRIVI-5)], errbox[400];
   tulostusparamtp autotulostusparam;
   int  tulfl = FALSE;
   int  comfl = FALSE;
   int  tcomfl = TRUE;
   int  estavalitys[MAX_LAHPORTTI];
   int  immed = TRUE;
   UINT32 kirjheti = 0xffffffff;
   int  comopen[MAX_PORTTI];
   int  comclose[MAX_PORTTI];
   int  leimacomopen[MAX_PORTTI];
   int leimacom = -1;
   INT  lahec[MAX_LAHPORTTI];
   INT  lahkello[MAX_LAHPORTTI];
   INT  lahkelloraja[MAX_LAHPORTTI];
   bool  lahkellovain[MAX_LAHPORTTI];
   int katkkierto[MAX_LAHPORTTI];
   int baud[MAX_PORTTI];
   char pty[MAX_PORTTI] = {'n'};
   int combits[MAX_PORTTI] = {8};
   int stopbits[MAX_PORTTI] = {1};
   int comkesto[MAX_LAHPORTTI];
   int nprtch = 0;
   INT inbens;
   INT inbseur;
   long cjens[MAX_LAHPORTTI];
   long cjseur[MAX_LAHPORTTI];
   int obfile = -1, levypusk = 1;
   FILE *fobfile;
   int lah_alku[MAX_LAHPORTTI];
   INT lah_vaihto[MAX_LAHPORTTI];
   int tarlahcom[MAX_LAHPORTTI] = {1 , 1};
   int lahcom[MAX_LAHPORTTI];
   int vastcom[MAX_LAHPORTTI];
   INT yhteys_on[MAX_LAHPORTTI];
   int lah_tiedostot[MAX_LAHPORTTI];
   bool lahkaikkitied;
   int monitoriLkm;
   int dcom = 0;
   int jonokynnys = 5;
   int ennakoi;
   int aikaero[NREGNLY+1];
   int estaneg, ylimraja, uusinaika, samapisteraja = 10 * SEK;
   int muutoskysely = 1;
   int maxjono, yhtjonot, akttm;
   int  lukijalahde[NREGNLY];
   int aika_jono[1][45], emitjono;
   int yhtaikajono[MAX_LAHPORTTI];
   aikajonotp *aikajono[MAXJONO];
/*
   int rwtime[MAXJONO], rtime[MAXJONO], mxtime[MAXJONO], muut_time;
   aikatp *atime[MAXJONO];
   int afhandle[MAXJONO];
   FILE *fafhandle[MAXJONO];
   int fp[MAXJONO], fp1[MAXJONO];
   int aikakorjaus[MAXJONO];
*/
   aikatp viim_time;
   int muut_time;
   TMAALI od_aika;
   int od_fl;
   int aikaTCPstatus;
	lahdepistetp *lahdepisteet[MAXSARJALUKU+1][MAXOSUUSLUKU+1];
	lahdepistetp *lahdejonot[MAXSARJALUKU+1][MAXOSUUSLUKU+1];
   int modif[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU];
   int timefl;
   int loki, lokiTCP, mloki;
   HANDLE hLoki;
   int com_ajat[MAX_LAHPORTTI];
   long rukkausvali,rukkaa;
   int edista;
   int lah_kello[MAX_LAHPORTTI];
   int lah_kello_nyt[MAX_LAHPORTTI];
   long kellovali[MAX_LAHPORTTI] = {900L*18L, 900L*18L};
   int ajat_on, status_on = 1;
   int printer;
   PRFILE *lokifile;
   TMAALI tvalmis = TMAALI0;
   int valiajat = -1;
   int maxnvaika = MAXNVAIKA;
   int vainvaliajat;
   char aika_int_mask = 17;    // Nämä kaksi riviä ovat aina mukana
   int aika_esto=2;            // koska muuttujat esiintyvät ASYNCH:ssa
   int va_mpiste[MAXJONO];
   int ol_osuus, os_raja = MAXOSUUSLUKU;
   int aika_com=0;
   int com_aika;
   int regnly[NREGNLY];
//   int regnly_no = 0;
   int od_regnly=10;
   int t0_regnly = -12 * TUNTI;
   int t0_regn[NREGNLY];
   int port_regnly[NREGNLY];
   int regnlyhetki[NREGNLY];
   int regnly_open[NREGNLY];
   INT cn_regnly[2 * NREGNLY];
   long maxero = 2000000L;
   int kerro_regnly;
   int usb_regnly[NREGNLY];
   int t0_autova;
   int int_autova;
   int addr_autova;
   int autova_open;
   INT nbadge[2];
   bdg_kno_tp *bdg_kno[2];
   INT  emittoisto = 1, vaikavert = 0, ilmtunt = 1, vaajat;
 //  char paikat[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU+1];
   int maxpaikat_t;
 //  int maxpaikat[MAXSARJALUKU];
   int taulu_com, taulu_baud = 7, udptaulu = 0;
   INT sak_com, sak_baud = 8, sak_laji = 2;
   int ratavalpit = HAJONTA+1;
   int ampusakot = 0;
//   int partio, rogaining
   int emitfl;
   int maxaika[MAXSARJALUKU], sakkoaskel[MAXSARJALUKU], askelsakko[MAXSARJALUKU];
   int joustoviesti, joustopakotavaihto;
   bool esta_uusikaytto = true;

   INT  ajatfl, ilmlainat, emitkd, relax, emitsivu;
   INT  ohitaleimat;
   INT  ohitatoisto;
	INT  siritreuna = 1;
	bool siritloki = false;
	bool siritsync = false;
	int sirithead, sirittail;
	wchar_t siritbuf[SIRITBUFLEN+1];
   INT jalkisyotto;
   INT  joukkueemit;
   INT salli_emit_ensin;
   ratatp *rata;
   INT  nrata, kaikki_ajat[NREGNLY+1], vaadiaika;
   INT  vainpiste[NREGNLY+2] = {-9};
   wchar_t ch_piste[] = L"ALM123456789XY";
   INT aika_tunnus[1][45] = { -2, -9, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2 };
   INT aika_tunnus_lkm;
   INT lahtoryhma;
   INT  nesitt, nesit[MAXSARJALUKU+MAXYHD];
   char maalitp[10];
   INT kello_baud, regnly_no[NREGNLY], kello_bits = 8;

   int bytecount, bytecountmax = 2000;
   INT com_emit[MAX_LAHPORTTI];
   INT *emitjarr[MAXOSUUSLUKU];
   int  emitfile;
   INT com_emitva[MAX_LAHPORTTI];
   FILE *femitfile;
   INT emitptr = -1, emithead, emithead2;
   UINT *emitpos;
   INT autorap, comautorap;
   char autorapsrj[MAXSARJALUKU+MAXYHD];
   INT ennakko;
   char viimtark;
   INT huomaatark = 1;
   int emitaglisa = 1;
   int pollaa, TCPpoll[NREGNLY + 1];
   INT emitanal_fl;
   INT esitark;
   bool emitluenta;
   INT maalirastipois;
   INT bdg_muutt;
   INT autokilp;
   int seurkrivi;
   INT estaemittoisto = 1;
   int sallisummavirhe, lukutoisto = 2;
   int naytahajonta = 1;
   INT valonaytto;
   INT val_korkein;
   int profiloi;
   INT  comtype[MAX_PORTTI];
   INT  fileyht = -1, tcpyhteys;
   INT xmlsarja = -1;
   bool XMLtietue = true;
   int ajanottofl = -1, sail_ajat;
   INT32 viim_regnly;
   ipparamtp ipparam[MAX_PORTTI+1];
   int timerfl = FALSE;
   int odota;
   int datatallfl[2] = {1, 0};
#ifdef DBGFILE
   int dbgtofile;
#endif

//   OSVERSIONINFO osvi;
	int pkarki[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU][VAIKALUKU+2];
	int tarktapahtumat;
	int vuorokaudet, kierrokset;
	int (*korRaja)[2];
	wchar_t *vipLaji;
	unsigned int korVari[6] = {
		65280, 65535, 16711935, 116763572, 16776960, 32896
		};

	INT emittime;
	bool inkirjSQLThread = false;
#ifdef EMITHTTP
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
		{0, 0, 0, 0},			//	int msgno;
		0};						//	int haettu;

	int inhttpHaku;
	HTHREAD httphakuThread;

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

#endif
