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
#include <process.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#pragma hdrstop
#include <bstrings.h>
#ifdef _CONSOLE
#include <bkeybrd.h>
#endif
#include <direct.h>
#include <dos.h>
//#include <size.h>
#include <sys/stat.h>
#include <tptype.h>
#include "wincom.h"
#include "VDeclare.h"
#include "VMuotoilu.h"
#include "TpLaitteet.h"

#define ALGE

void addMonitori(wchar_t *str);
int luesarjat(void);
void luesarjayhdistelmat(void);
void ReadComTestParams(void);
void init_muotoilut(void);
int cpower;
wchar_t RaporttiHak[2][202];
extern int monitoriVali;
extern int UDPviive_lue, UDPviive_lah, UDPviive_ts, UDPCliWait;
extern int UDPvarmistamaton[MAX_PORTTI];
extern int varaserver;
extern int tcpautoalku;
extern bool hav_virheet;
INT16  setySize = 50;
int   afvali0;
wchar_t ikkunaots[61];
#ifdef LAJUNEN
extern char siritarrive[80];
extern char siritdepart[80];
extern int SiritEventPort[NREGNLY];
extern int SiritCmdPort[NREGNLY];
extern int SiritPoll[NREGNLY];
#endif
extern IV_KEY keytab[];
static FILE *err_file;
long sarjano = 3100010L;
static char snostr[16];
static int muotoilufl, cfgparam;
char merkit = 'G';
static wchar_t comdir[MAX_PORTTI];
static wchar_t cfgfilename[41] = L"LASKENTA.CFG";
static wchar_t autostr[25] = L"";
static int korkeat = FALSE;
static int star = FALSE;
static wchar_t baudst[MAX_PORTTI][20] = {L"9600,n,8,1"};
static int portparam[MAX_PORTTI];
static int nopeuta = 0;
static wchar_t oletuskieli = L'S';
static int pisteparam;
int aikakorjaus[MAXJONO];
extern int korjaareaaliaika;
extern INT norm_fore, norm_back, inv_fore, inv_back;
bool luentaloki;
#ifdef MAALI
extern int lainatied;
#endif
extern INT32 vapaaemit;
INT tarkavoimet, luentatesti;

#ifdef KUUL
static tietofl;
#endif
#ifdef TAULU
void opentaulu(void);
#endif
#ifdef ALGETAULU
extern wchar_t tauluflnm[];
#endif
#if defined(TV1) || defined(AUTOVA)
void opentv(void);
#endif

#if defined(TXTTV)
extern int suoracom;
#endif

void chgchar(char *buf, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		switch(buf[i]) {
			case 'Ä':
				buf[i] = '÷';
				break;
			case 'Å': 
				buf[i] = '°';
				break;
			}
		}
}

void kirj_err_file_w(wchar_t *wmsg, int nayta)
   {
   char msg[400];
   kirj_err_file(wcstoansi(msg, wmsg, 399), nayta);
   }

void kirj_err_file(char *msg, int nayta)
   {
   static int lopilm;
   char ch, msg2[200];
   if (nayta && !lopilm) {
      clrln(ySize-1);
	  vidspmsg(ySize-1,0,0,7,msg);
      erbeep();
      ch = 'J';
#ifdef _CONSOLE
      sprintf(msg2, "J)atka, L)opeta nämä virheilmoitukset");
#else	  
	  sprintf(msg2, "%s\nJ)atka, L)opeta nämä virheilmoitukset", msg);
#endif	  
      selectopt(msg2, "JL", &ch);
      if (ch == 'L')
         lopilm = 1;
      }
   if (!err_file) {
      err_file = fopen("DATA_ERR.TXT", "wt");
      }
   if (err_file) {
      fputs(msg, err_file);
      fputc('\n', err_file);
      }
   }

static void close_err_file(void)
   {
   if (err_file) {
      writeerror("Tiedostoon DATA_ERR.TXT kirjoitettu luettelo virhetiedoista", 0);
	  fclose(err_file);
	  err_file = 0;
	  }
   }

#ifdef _CONSOLE
extern void resizeConBufAndWindow(HANDLE hConsole, SHORT xSize,
	SHORT ySize);

void initscreens(void)
   {
   int i, maxrv;
   ySize = setySize;
	osd = 2;
	if (ySize - 9 >= ((kilpparam.maxvaluku +3)/4+1) * kilpparam.n_os_akt)
		osd = (kilpparam.maxvaluku+3)/4+1;
   if (ySize == 50)
	   resizeConBufAndWindow(hConOut, xSize, ySize);
   scpclr();
   viwrrect(NOY,1,NOY,14,"Kilp.numero : ",7,0,0);
   viwrrect(MAY,MAX-12,MAY,MAX-2,"Maaliaika :",7,0,0);
   viwrrect(SAY,1,SAY,8,"Sarja : ",7,0,0);
   viwrrect(SEY,1,SEY,8,"Seura : ",7,0,0);
   viwrrect(COY,COX-6,COY,COX-1,"Maa : ",7,0,0);
   viwrrect(JOY,JOX-10,JOY,JOX-1,"Joukkue : ",7,0,0);
   if (kilpparam.n_os < 11) {
		if (ampusakot) {
			viwrrect(OTSY,1,OTSY,63,
				"Os|Nimi                      |Aika      |Os.aika   |KHE|Sj.|Sak.",
				7,0,0);
			if (emitfl)
				viwrrect(OTSY-1, 61, OTSY-1, 61, "Y", 7, 0, 0);
			}
		else
			viwrrect(OTSY,1,OTSY,61,
				"Os|Nimi                      |Aika      |Os.aika   |KHE|Sj.|Y",
				7,0,0);
	   draw_vline(OTSY, 3, 1);
	   draw_vline(OTSY, 30, 1);
	   draw_vline(OTSY, 41, 1);
	   draw_vline(OTSY, 52, 1);
	   draw_vline(OTSY, 56, 1);
	   draw_vline(OTSY, 60, 1);
	   }
	else {
	   viwrrect(OSY-1,1,OSY-1,63,
		  "Os|Nimi                      |Aika      |Os.aika   |KHE|Sj.|Y  ",
		  7,0,0);
	   draw_vline(OSY-1, 3, 1);
	   draw_vline(OSY-1, 30, 1);
	   draw_vline(OSY-1, 41, 1);
	   draw_vline(OSY-1, 52, 1);
	   draw_vline(OSY-1, 56, 1);
	   draw_vline(OSY-1, 60, 1);
		}

   maxrv = kilpparam.n_os_akt*osd;
   if (maxrv > ySize-4)
	   maxrv = ySize-4;

   draw_vline(OSY, 3, maxrv);
   draw_vline(OSY, 30, maxrv);
   draw_vline(OSY, 41, maxrv);
   draw_vline(OSY, 52, maxrv);
   draw_vline(OSY, 56, maxrv);
   draw_vline(OSY, 60, maxrv);

	if (osd > 1 && kilpparam.badgelaji <= L' ') {
	   for (i=0; i < kilpparam.n_os_akt && i < (ySize-OSY-3)/osd; i++)
		   viwrrect(OSY+1+i*osd,8,OSY+1+i*osd,17,
			   "Väliajat :", 7,0,0);
		}
   for (i=0; i < kilpparam.n_os_akt && i < (ySize-OSY-3)/osd; i++) {
	   vidint(OSY+osd*i,1,2,i+1);
	   }
   virdrect(2,0,ySize-4,64,form,0);
	chgchar(form, 65*(ySize-5));
   scpclr();
   }

static void luekorkeat(void)
   {
   FILE *korkeatf;
   int   i,er,nc;
   char ch;
   char krkfname[] = "KORKEAT.KOD";

   er = 0;
   korkeatf = fopen(krkfname,"rt");
   if (korkeatf != NULL) {
      if (fscanf(korkeatf, "%d %d\n", &nchdble, &nchsgle) != 2 &&
         nchdble < 11 && nchsgle < 11) er = 1;
      else {
         for (i = 0; i < nchdble; i++) {
			if (fscanf(korkeatf, "%d ", &nc) != 1) {
               er = 2;
               break;
               }
			else dble[i] = nc;
			}
         if (!er)
            for (i = 0; i < nchsgle; i++) {
               if (fscanf(korkeatf, "%d ", &nc) != 1) {
				  er = 2;
                  break;
                  }
               else sgle[i] = nc;
               }
         }
      fclose(korkeatf);
      if (!er)
         viwrrect(13,0,13,63,"Korkeiden kirjainten ohjauskoodit luettu"
            " tiedostosta KORKEAT.KOD",7,0,0);
      else
         writeerror("Korkeiden kirjainten kooditiedosto virheellinen",0);
      }
   clrln(ySize-3);
   do {
      ch = ' ';
      selectopt("Annetaanko tai muutetaanko levennetyn tarratekstin "
         "ohjauskoodeja (K/E)", "KE", &ch);
      clrln(ySize-3);
      if (ch == 'E') break;
      for (i=0; i < nchdble; i++) vidint(17,5+4*i,3,dble[i]);
      viwrrect(16,0,16,72,"Anna levennetyn tekstin käynnistysmerkkijono "
         "ascii-arvoina, lopuksi <Esc>",7,0,0);
      for (i=0; i < 10; i++) {
         nc = dble[i];
         INPUTINT(&nc, 3, 5+4*i, 17, "\r\x1B",&ch);
         if (ch == 27) break;
         dble[i] = nc;
         }
      nchdble = i;
      scclrmsg(17, 5+4*i, 3);
      for (i=0; i < nchsgle; i++) vidint(19,5+4*i,3,sgle[i]);
      viwrrect(18,0,18,69,"Anna levennetyn tekstin lopetusmerkkijono "
         "ascii-arvoina, lopuksi <Esc>",7,0,0);
      for (i=0; i < 10; i++) {
         nc=sgle[i];
         INPUTINT(&nc, 3, 5+4*i, 19, "\r\x1B",&ch);
         if (ch == 27) break;
         sgle[i] = nc;
         }
	  nchsgle = i;
	  scclrmsg(19, 5+4*i, 3);
	  } while (ch != 'E');
   ch = 'K';
   selectopt("Tallennetaanko koodit levytiedostoon 'KORKEAT.KOD' (K/E)",
	  "KE", &ch);
   if (ch == 'K') {
	  if((korkeatf = fopen(krkfname,"wt")) != NULL) {
		 fprintf(korkeatf, "%d %d\n", nchdble, nchsgle);
		 for (i=0; i< nchdble; i++) fprintf(korkeatf, "%d ", dble[i]);
		 fprintf(korkeatf,"\n");
		 for (i=0; i< nchsgle; i++) fprintf(korkeatf, "%d ", sgle[i]);
		 fprintf(korkeatf,"\n");
		 fclose(korkeatf);
		 }
	  }
   clrln(16);
   clrln(17);
   clrln(18);
   clrln(19);
   clrln(ySize-3);
   }

static void nimio(void)
{
   scpclr();
   scbox(0,15,9,64,15,0,7);
   viwrrect(2,19,2,60,"VIESTISUUNNISTUKSEN TULOSPALVELUOHJELMISTO",
	  7,0,0);
   viwrrect(4,34,4,45,"Pekka Pirilä",7,0,0);
#ifndef LUENTA
   viwrrect(6,23,6,41,"Ohjelma ViestiMaali",7,0,0);
#else
   viwrrect(6,22,6,41,"Ohjelma ViestiLuenta",7,0,0);
#endif
   viwrrect(6,45,6,55,"-osuuksinen",7,0,0);
   vidint(6,43,2,MAXOSUUSLUKU);
   vidspwmsg(8, 20, 7, 0, L"Versio");
   vidspwmsg(8,27,7,0,VersioNo());
   vidspwmsg(8, 35, 7, 0, L"Build");
   vidspwmsg(8, 41, 7, 0, Build());
#ifdef DEMO
   vidspmsg(3,24,7,0,"DEMO korkeintaan 20 joukkuetta");   /* DEMO */
#else
#endif
}
#endif // _CONSOLE

void merkisto(char merkit)
   {
   FILE *mf;
   int i,j,len;
   char merkfname[] = "MERKISTO.KOD";
   char psfname[] = "POSTSCRI.INI";

   for (i = 0; i < 256; i++) trlate[i] = i;
   switch (merkit) {
      case 'A' : trlate['Ž'] = 91;
                 trlate['™'] = 92;
                 trlate[''] = 93;
                 trlate['š'] = 94;
                 trlate[''] = 'E';
                 trlate['„'] = 123;
                 trlate['”'] = 124;
				 trlate['†'] = 125;
				 trlate[''] = 126;
                 trlate['‚'] = 'e';
                 break;
      case 'R' : trlate['Ž'] = 216;
                 trlate['™'] = 218;
                 trlate[''] = 208;
                 trlate['š'] = 219;
                 trlate[''] = 220;
                 trlate['„'] = 204;
				 trlate['”'] = 206;
                 trlate['†'] = 212;
                 trlate[''] = 207;
                 trlate['‚'] = 197;
                 break;
      case 'L' : if ((mf = fopen(merkfname, "rt")) != NULL) {
                    while (!feof(mf)) {
                       if (fscanf(mf, "%d %d\n",&i,&j) != 2) break;
                       trlate[i] = (char) j;
                       }
                    fclose(mf);
                    }
                 else writeerror("Kirjoitinkoodien lukeminen "
                    "tiedostosta MERKISTO.KOD ei onnistu", 0);
                 break;
      case 'P' : 
				 if ((mf = fopen(psfname, "rb")) != NULL) {
                    fseek(mf, 0, SEEK_END);
                    len = ftell(mf);
		    
														  fseek(mf, 0, SEEK_SET);
						  if (len)
							  psinit = (wchar_t *) calloc(len+2, sizeof(wchar_t));
					if (psinit) {
//					   len = fread(psinit+2, 1, (size_t) len, mf);
//					   *(UINT16 *) psinit = (UINT16) len;
					   }
					fclose(mf);
					}
//                 if (!psinit) writeerror("Kirjoitinalustuksen lukeminen "
//                    "tiedostosta POSTSCRI.INI ei onnistu", 0);
                 pscr = 1;
      case 'W' :                    // myös 'P' jatkuu
      case 'G' :
                 trlate['Ž'] = 196;
                 trlate['™'] = 214;
                 trlate[''] = 197;
                 trlate['š'] = 220;
                 trlate[''] = 201;
				 trlate['„'] = 228;
				 trlate['”'] = 246;
                 trlate['†'] = 229;
				 trlate[''] = 252;
                 trlate['‚'] = 233;
				 trlate['_'] = 32;
                 break;
      }
   }

static INT readcfg(INT *nf, wchar_t *argptr[], int kierros)
   {
   TextFl *cfgfile;
	wchar_t line[80],*p, *cfgflnm;
   INT l;

   if (*nf == 0) {
		if (kierros == 0)
			cfgflnm = L"peruscfg.cfg";
		else
			cfgflnm = cfgfilename;
		cfgfile = new TextFl(cfgflnm, L"rt");
		if (!cfgfile->IsOpen()) {
			delete cfgfile;
			if (kierros > 0 && cfgparam) {
				swprintf(line, L"Konfiguraatiotiedostoa %s ei löydy",
					cfgflnm);
				writeerror_w(line ,0);
				}
			return(1);
			}
	  *nf = 1;
	  while(1) {
		if (cfgfile->ReadLine(line, 78) == NULL) {
			if (cfgfile->Feof())
				goto esc;
			swprintf(line, L"Virhe tiedostoa %s luettaessa",
			   cfgflnm);
			writeerror_w(line ,0);
			goto esc;
			}
		 line[79] = 0;
		 if (line[(l=wcslen(line))-1] == L'\n') line[--l] = 0;
			if (!line[0] || line[0] == L';' || (line[0] == L'/' && line[1] == L'/'))
			continue;
//         p = wcstok(line, L" ");
		 p = wcstok(line, L";");
		 elimwbl(p);
		 l = wcslen(p);
			if (l == 0) continue;
		 if (*nf < 200) {
			argptr[*nf] = (wchar_t *) malloc((l+1)*sizeof(wchar_t));
			wcscpy(argptr[*nf], p);
			++*nf;
			}
		else {
			writeerror_w(L"Liikaa parametreja", 0);
			return(0);
			}
		 }
	  }
   else {
		for (l = 1; l < *nf; l++) free(argptr[l]);
	  return(0);
	  }
   esc:
   delete cfgfile;
	return(0);
   }

static void getipparam(INT ny, wchar_t *str)
	{
	wchar_t *p;
	int k;

	p = str;
	if (ny < MAX_PORTTI) {
		comtype[ny] = comtpUDP;
		ipparam[ny].iptype = ipUDPBOTH;
		portparam[ny] = -1;
		ipparam[ny].srvport = portbase + 1 + ny;
		wcscpy(ipparam[ny].destaddr, L"AUTO");
		ipparam[ny].destport = portbase + 1;
	    if (*p == L'I' || *p == L'O') {
		    comdir[ny] = *p;
			p++;
			}
		}
	if (wcswcind(*p, L":,/=") >= 0) {
		p = wcstok(p+1, L":,/=");
		if (p && (k = _wtoi(p)) >= 0) {
			  if (k)
					ipparam[ny].srvport = (USHORT) k;
			  p = wcstok(NULL, L":,/=");
			  if (p) {
				wcsncpy(ipparam[ny].destaddr, p, 63);
				  p = wcstok(NULL, L":,/=");
				  if (p && (k = _wtoi(p)) >= 0) {
					  if (!k) {
						 if (*p == L'Y') {
							  while (*p > L'9')
								  p++;
							  if ((k = _wtoi(p)) > 0)
								  ipparam[ny].destport = portbase + k;
							  }
						  }
					  else
						  ipparam[ny].destport = (USHORT) k;
					  }
				  }
			  }
		}
	 if (!wcscmp(ipparam[ny].destaddr, L"AUTO"))
		 ipparam[ny].destport = 0;
	}

 static void getudpstreamipparam(INT ny, wchar_t *str)
    {
	wchar_t *p;
	int k;

    p = str;
	if (ny < MAX_PORTTI) {
		comtype[ny] = comtpUDPSTREAM;
		ipparam[ny].iptype = ipUDPSTREAM;
		portparam[ny] = -1;
		if (wcswcind(*p, L":,/=") >= 0) {
			p = wcstok(p+1, L":,/=");
			if (p && (k = _wtoi(p)) > 0) {
				ipparam[ny].srvport = (USHORT) k;
				}
			}
		}
	}

#if defined(TAULU) || defined(ALGETAULU)
static void gettauluipparam(wchar_t *str)
    {
    wchar_t *p;
    int ny;

	ny = GAZCOM;
    comtype[ny] =  comtpUDP;
	ipparam[ny].iptype = ipUDPBOTH;
	ipparam[ny].srvport = 0;
    ipparam[ny].destport = 51702;
    p = str;
    if (wcswcind(*p, L":,/=") >= 0) {
        wcsncpy(ipparam[ny].destaddr, p+1, 63);
        }
    }
#endif

#if defined(TCPLUKIJA) || defined(TCPSIIRTO)
static void gettcpipparam(INT ny, wchar_t *str, int server)
    {
    wchar_t *p;
    int k;

	p = str;
	if (ny >= MAX_PORTTI) {
       writeerror_w(L"TCP-yhteyden numero virheellinen", 0);
       return;
       }
	 if (wcswcind(*p, L":,/=") >= 0) {
		p = wcstok(p+1, L":,/=");
		if (!p)
            return;
		if (server <= 0) {
			switch (server) {
				case -1:
					comtype[ny] = comtpTCPXML;
					break;
				case -2:
					comtype[ny] = comtpTCPLOKI;
					break;
				default:
					comtype[ny] = comtpTCP;
					break;
				}
			ipparam[ny].iptype = ipTCPCLIENT;
			wcsncpy(ipparam[ny].destaddr, p, 63);
		    p = wcstok(NULL, L":,/=");
			if (p && (k = _wtoi(p)) > 0)
				ipparam[ny].destport = (USHORT) k;
			}
		else {
			comtype[ny] = server == 1 ? comtpTCPSRV : comtpTCPSRVXML;
			ipparam[ny].iptype = ipTCPSERVER;
			ipparam[ny].srvport = (USHORT) _wtoi(p);
			}
		portparam[ny] = -2;
		}
	}
#endif

INT yhteys_no(wchar_t *st, INT *pos)
   {
   INT no, pos1;

   if (st[*pos] == L'=' || !st[*pos])
		no = 1;
   else {
	  for (pos1 = *pos; st[pos1] >= L'0' && st[pos1] <= L'9'; pos1++);
	  if (pos1 > *pos && (no = _wtoi(st+*pos)) > 0)
		 *pos = pos1;
	  else
		 no = 0;
		}
   if (no > MAX_LAHPORTTI) {
		writeerror_w(L"Yhteyden numero liian suuri tälle ohjelmaversiolle", 0);
		no = 0;
		}
   return(no);
   }

void lue_parametrit(int argc, wchar_t *argv[], wchar_t *cfgflname)
   {
   wchar_t **fld, fldn[90];
   wchar_t *argptr[200];
   wchar_t *p;
   wchar_t msg[201];
   int i,j, nf,y,ncl,ny,pos;
   INT32 y1, y2;
   UINT32 mask;
   wchar_t wst[80];

	if (cfgflname != NULL && *cfgflname) {
		wcsncpy(cfgfilename, cfgflname, 120);
		}
   memset(argptr, 0, sizeof(argptr));
   if (argc > 1) {
      wcsncpy(fldn, argv[1], 44);
      upcasewstr(fldn);
	  if (!wmemcmp(fldn, L"CFG=", 4))
		 wcsncpy(cfgfilename, fldn+4, 40);
	  }
   for (i = 0; i < 3; i++) {
	  if (i <= 1) {
		 nf = 0;
		 if (readcfg(&nf, argptr, i) == 1) continue;
		 fld = argptr;
		 }
	  else {
		 fld = argv;
		 nf = argc;
		 }
	  for ( ncl = 1 ; ncl < nf ; ncl++) {
		 memset(fldn, 0, sizeof(fldn));
		 wmemcpy(fldn, fld[ncl], 7);
		 upcasewstr(fldn);
//		 oemtoansi(fldn, 0);
		 pos = 5;
         if (!wmemcmp(fldn, L"HTML", 4) || !wmemcmp(fldn, L"LISTA=", 6)
			|| !wmemcmp(fldn, L"SQL", 3)
			|| !wmemcmp(fldn, L"LOKI=", 5) || !wmemcmp(fldn, L"TARRA=", 6)
            || (!wmemcmp(fldn, L"KOMENTO", 7) && (pos = 7) !=0)) {
            if (wcslen(fld[ncl]) > (UINT32) pos) 
               wcsncpy(fldn+pos, fld[ncl]+pos, 76);
            }
         else
			{
            wcsncpy(fldn, fld[ncl], 89);
            for (p = fldn; *p && *p != L' '; p++) ;
			if (*p == L' ') *p = 0;
            upcasewstr(fldn);
            }
		 if(!wmemcmp(fldn, L"CFG", 3))
            continue;
#ifndef _CONSOLE
		if(!wmemcmp(fldn, L"IKKUNAT", 7)) {
			if (fldn[7] == L'=')
				wcsncpy(alkulayout, fldn+8, 39);
			else
				wcscpy(alkulayout, L"ikkunat.xml");
			continue;
			}
		if(!wmemcmp(fldn, L"KUULUTTAJA", 7)) {
			kuuluttaja = true;
			continue;
			}
#endif
		 if(!wmemcmp(fldn, L"IKKUNAO", 7)) {
		    p = wcsstr(fldn+6,L"=");
			if (p) {
				wcsncpy(ikkunaots, p+1, sizeof(ikkunaots)/2-1);
				}
            continue;
			}
		 if (!wmemcmp(fldn, L"SALLISULJE", 10)) {
			 poistaSulje = false;
			 continue;
		 }
		 if (!wmemcmp(fldn, L"KIRKAS", 5)) {
            norm_fore = 15;
            continue;
            }
         if(!wmemcmp(fldn, L"VÄRIT=", 6)) {
			p = wcstok(fldn+6,L"/");
			if (p) {
				norm_fore = _wtoi(p);
			    p = wcstok(NULL,L"/");
				if (p) {
					norm_back = _wtoi(p);
					p = wcstok(NULL,L"/");
					if (p) {
						inv_fore = _wtoi(p);
						p = wcstok(NULL,L"/");
						if (p) {
							inv_back = _wtoi(p);
							}
						}
					}
				}
            continue;
            }
         if(!wmemcmp(fldn, L"RIVIT=25", 8)) {
            setySize = 25;
            continue;
			}
         if(!wmemcmp(fldn, L"RIVIT=50", 8)) {
            setySize = 50;
            continue;
			}
		 if (!wmemcmp(fldn, L"SAKOT",5)) {
			writeerror_w(L"Parametri SAKOT poistettu käytöstä. Katso ohjeita", 0, true);
			continue;
			}
		 if (!wmemcmp(fldn, L"AMPUSAKOT",9)) {
			writeerror_w(L"Parametri AMPUSAKOT poistettu käytöstä. Katso ohjeita", 0, true);
			continue;
			}
		 if(!wmemcmp(fldn, L"HIIHTO", 6)) {
			writeerror_w(L"Parametri HIIHTO poistettu käytöstä. Katso ohjeita", 0, true);
			continue;
			}
		 if(!wmemcmp(fldn, L"MAALILUKIJA", 6)) {
			lisaa_edelle = 1;
			continue;
			}
		 if(!wmemcmp(fldn, L"SISÄÄNLUENTA", 9)) {
			sisaanluenta = 2;
			if (wcsstr(fldn, L"=M") != NULL)
				sisaanluenta = 1;
			continue;
			}
		 if(!wmemcmp(fldn, L"LUENTALOKI", 8)) {
			luentaloki = true;
			continue;
			}
		 if(!wmemcmp(fldn, L"PARTIO", 6)) {
			 writeerror_w(L"Parametri PARTIO poistettu käytöstä. Katso ohjeita", 0, true);
			continue;
			}
		 if(!wmemcmp(fldn, L"ROGAI", 5)) {
			 writeerror_w(L"Parametri ROGAINING poistettu käytöstä. Katso ohjeita", 0, true);
            continue;
            }
		 if(!wmemcmp(fldn, L"KIERROKSET", 10)) {
			kierrokset = 1;
			vuorokaudet = 1;
			continue;
			}
		 if(!wmemcmp(fldn, L"VUOROKAUDET", 10)) {
			vuorokaudet = 1;
			continue;
			}
		 if(!wmemcmp(fldn, L"TARKNO=", 7)) {
			tarkfl = fldn[7] - L'0';
            continue;
			}
         if(!wmemcmp(fldn, L"MUOTOILU", 5)) {
            muotoilufl = TRUE;
			if (fldn[8] == L'=') 
				wcsncpy(luetfnm, fldn+9, 39);
            continue;
            }
#ifndef _CONSOLE
		 if (!wmemcmp(fldn, L"SQLDRIVER", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.drivername, p+1, sizeof(sqlparam.drivername)/2-1);
				}
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLHOST", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.hostname, p+1, sizeof(sqlparam.hostname)/2-1);
				}
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLDATABASE", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.database, p+1, sizeof(sqlparam.database)/2-1);
				}
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLUSER", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.User_Name, p+1, sizeof(sqlparam.User_Name)/2-1);
				}
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLPASSWORD", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.Password, p+1, sizeof(sqlparam.Password)/2-1);
				}
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLSTART", 7)) {
			sqlparam.autostart = true;
			continue;
			}
		 if (!wmemcmp(fldn, L"SQLTALLENNUS", 7)) {
			sqlparam.sqltallennus = true;
			continue;
			}
#endif // !_CONSOLE
/*
		 if(!wmemcmp(fldn, L"TARRA", 5)) {
			if (fldn[5] == L'=')
			   wcscpy(tarfname, &fldn[6]);
			else
			   wcscpy(tarfname, L"PRN");
			continue;
			}
*/
		 if(!wmemcmp(fldn, L"KIELI=", 6)) {
			oletuskieli = fldn[6];
			continue;
			}
		 if(!wmemcmp(fldn, L"LOKI", 4)) {
			loki = TRUE;
			if (wcslen(fldn) > 5)
				wcsncpy(lokifname, &fldn[5], 60);
			continue;
			}
		 if(!wmemcmp(fldn, L"ESTÄMUUTOS", 8)) {          //  Estää tallennuksen 
			esta_tall = 1;
			continue;
			}
		 if (!wmemcmp(fldn, L"TESTAUS",5)) {
			testi = 1;
			hav_virheet = true;
			continue;
			}
		 if (!wmemcmp(fldn, L"COMTESTI", 8)) {
			 testi = 2;
			 continue;
			}
		 if (!wmemcmp(fldn, L"LISTA", 5)) {
				if (fldn[5] == L'=') {
					wcsncpy(msg, fldn+6, 80);
					msg[79] = 0;
					upcasewstr(msg);
					elimwbl(msg);
					wcsncpy(listfname, &fldn[6], 80);
					}
			if (!fldn[5] || !wcscmp(msg, L"OLETUS") || !wcscmp(msg, L"PRN"))
			   wcscpy(listfname, L"PRN");
            continue;
            }
         if (!wmemcmp(fldn, L"OTSIKOT", 3)) {
            tulosmuot.otsikot = 1;
            continue;
            }
         if( !wmemcmp(fldn, L"MERKIT=", 7)) {
            merkit = wchartoansi(fldn[7]);
			if (wcswcind(L'K', fldn+8) >= 0) trloki = 1;
            if (wcswcind(L'T', fldn+8) >= 0) trtar = 1;
			trlst = !(trloki || trtar);
            if (wcswcind(L'L', fldn+8) >= 0) trlst = 1;
            continue;
			}
		 if( !wmemcmp(fldn, L"RATAVAL=", 8)) {          // vaikuttaa hajontatiedon tulostuksen tilankäyttöön
            ratavalpit=_wtoi(fldn+8);
            continue;
            }
#ifdef AUTOFILE
         if( !wmemcmp(fldn, L"XML=", 4)) {
			if ((p = wcstok(fldn+4, L",/")) == NULL) continue;
			aftulparam.kohde = L'X';
//            afhtml = -1;
			wcsncpy(autofileparam.afname, p, 70);
			if ((p = wcstok(NULL, L",/")) == NULL || (ny = _wtoi(p)) == 0) {
			   if (!autofileparam.afvali) autofileparam.afvali = (int) (60 * SEK);
			   }
			else
			   autofileparam.afvali = (int) (ny * SEK);
			continue;
			}
		 if( !wmemcmp(fldn, L"HTML=", 5)) {
			if ((p = wcstok(fldn+5, L",/")) == NULL) continue;
			aftulparam.kohde = L'H';
//			afhtml = 1;
			wcsncpy(autofileparam.afname, p, 70);
			if ((p = wcstok(NULL, L",/")) == NULL || (ny = _wtoi(p)) == 0) {
			   if (!autofileparam.afvali) autofileparam.afvali = (int) (60 * SEK);
			   }
			else
			   autofileparam.afvali = (int) (ny * SEK);
			continue;
			}
		 if( !wmemcmp(fldn, L"HTMLTIT", 7)) {
			 wcsncpy(autofileparam.wtitlea, fldn+10, 78);
			continue;
			}
		 if( !wmemcmp(fldn, L"HTMLOTS", 7)) {
			wcsncpy(autofileparam.wheadera, fldn+8, 78);
			continue;
			}
		 if (!wmemcmp(fldn, L"KOMENTO", 7)) {
			wcsncpy(autofileparam.autokomento, fldn+8, sizeof(autofileparam.autokomento)/2-1);
//            wcscat(autokomento, L" > NUL 2>> cmderrors.txt");
			continue;
			}
#endif
#ifdef TCPSIIRTO
		 if (!wmemcmp(fldn, L"FILETCP", 7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			   fileyht = ny-1+MAX_LAHPORTTI+NREGNLY;
			gettcpipparam(fileyht, fldn+pos, 0);
			if (!autofileparam.afname[0]) {
			   autofileparam.aflaji = L'X';
			   wcscpy(autofileparam.afname, L"__TMP__.XML");
			   afvali0 = (int) (60 * SEK);
			   }
			continue;
			}
		 if (!wmemcmp(fldn, L"XMLSARJA=", 9)) {
			xmlsarja = haesarja_w(fldn+9, 0);
			continue;
			}
		 if( !wmemcmp(fldn, L"TCPHAJONTA",6)) {   // Määrää lähetetäänkä hajonta TCP-sanomissa
			lahhajonta = 1;
			}
#endif
		 if( !wmemcmp(fldn, L"BOOT", 4)) {
			bootfl = 1;
			if (!wmemcmp(fldn+4, L"=S", 2))
               bootfl = 2;
            continue;
			}
         if( !wmemcmp(fldn, L"PUSKUROI",8)) {   // immed: tiedostoonkirjoitus viedään heti levylle (ei Textfl)
			immed = 0;
			if (fldn[8] == L'=')
               kirjheti = (UINT32) _wtoi(fldn+9);
            continue;
			}
		 if( !wmemcmp(fldn, L"SÄILHYL", 7)) {
			if (fldn[8] == L'E' || fldn[8] == L'0') kilpparam.sailhyl = FALSE;
			else kilpparam.sailhyl = TRUE;
            continue;
            }
         if( !wmemcmp(fldn, L"KONE=",5)) {
            wcstoansi(konetunn, fldn+5, 3);
            continue;
            }
         if( !wmemcmp(fldn, L"VÄLIAJAT",8)) {   // Pyytää väliaokojen käsittelyä. Yleensä tarpeeton
			if (!valiajat)
			   valiajat = -1;
			if ((y = _wtoi(fldn+9)) != 0) maxnvaika = y;
			continue;
			}
		 if (!wmemcmp(fldn, L"CPOWER",6) && (pos = 6) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			cpower = 1;
			regnly[ny-1] = 23;
			if ((p = wcstok(fldn, L"=")) != NULL) {
				p = fldn+wcslen(p);
				if (!wmemcmp(p+1, L"TCP", 3)) {
					gettcpipparam(cn_regnly[ny-1], p+4, 0);
					if (ipparam[cn_regnly[ny-1]].destport == 0)
						ipparam[cn_regnly[ny-1]].destport = 5200;
					}
				else if (p[1] >= L'0' && p[1] <= L'9' &&(p = wcstok(NULL,L":,-/")) != NULL)
					port_regnly[ny-1] = _wtoi(p);
				}
            continue;
            }
#if defined(TAULU) || defined(ALGETAULU)
         if( !wmemcmp(fldn, L"TAULU_COM=",10)) {
			if (fldn[10] == L'U') {
				gettauluipparam(fldn+13);
				udptaulu = 1;
			    }
			else {
	            taulu_com = _wtoi(fldn+10);
#if defined(ALGETAULU)
		        if (fldn[10] == L'J') {
			       jatktaulu = 1;
				   }
				if ((fldn[10] == L'J' || fldn[10] == L'T') && fldn[11] && fldn[12]) {
		           wcsncpy(tauluflnm, fldn+12, 40);
				   }
#endif
				}
			continue;
            }
         if( !wmemcmp(fldn, L"TAULU_BAUD=",11)) {
			y = _wtoi(fldn+11);
            taulu_baud = 7;
            ny = 9600;
			while (ny > y && taulu_baud > 2) {
               taulu_baud--;
               ny = ny/2;
			   }
			continue;
			}
#endif
		 if( !wmemcmp(fldn, L"HYVKAIKKI",9)) {
			if ((ny = _wtoi(fldn+9)) > 0)
				UDPvarmistamaton[ny-1] |= 2;				 
            continue;
			}
		 if (!wmemcmp(fldn, L"MONITORVÄLI=",12)) {
			monitoriVali = 1000 * _wtoi(fldn+12);
			continue;
			}
		 if (!wmemcmp(fldn, L"MONITORI",8)) {
			addMonitori(wcsncpy(wst, fldn+8, 80));
			continue;
			}
         if (((!wmemcmp(fldn, L"COM",3) && (pos = 3) != 0) ||
            (!wmemcmp(fldn, L"YHTEYS",6) && (pos = 6) != 0)) &&
            fldn[pos] <= L'=' && (ny = yhteys_no(fldn, &pos)) != 0) {
            if (!wmemcmp(fldn+pos+1, L"UDP", 3)) {
			   getipparam(ny-1, fldn+pos+4);
			   continue;
			   }
			if (!wmemcmp(fldn+pos+1, L"BRO", 3)) {
				UDPvarmistamaton[ny-1] |= 1;
			   getipparam(ny-1, fldn+pos+4);
			   continue;
			   }
            if (!wmemcmp(fldn+pos+1, L"TCP", 3)) {
				j = 0;
				if (fldn[pos + 4] == L'X' || wmemcmp(fldn + pos + 4, L"SX", 2) == 0 ||
					fldn[pos + 4] == L'Z' || wmemcmp(fldn + pos + 4, L"SZ", 2) == 0) {
					if (fldn[pos + 4] == L'Z' || fldn[pos + 5] == L'Z')
						XMLtietue = false;
					else
						XMLtietue = true;
					pos++;
					if (wmemcmp(fldn + pos + 3, L"XS", 2) == 0 || wmemcmp(fldn + pos + 3, L"SX", 2) == 0 ||
						wmemcmp(fldn + pos + 3, L"ZS", 2) == 0 || wmemcmp(fldn + pos + 3, L"SZ", 2) == 0) {
						j = 2;
						pos++;
						}
					else
						j = -1;
					comdir[ny-1] = L'O';
					if (!autofileparam.sanafname[0])
						wcscpy(autofileparam.sanafname, L"__TMP_SAN__.XML");
					}
			   if (fldn[pos+4] == L'S') {
				  j = 1;
				  pos++;
                  }
			   if (fldn[pos+4] == L'I' || fldn[pos+4] == L'O') {
				  comdir[ny-1] = fldn[pos+4];
				  pos++;
                  }
               gettcpipparam(ny-1, fldn+pos+4, j);
			   continue;
               }
         if (!wmemcmp(fldn, L"TCPAUTOALKU",10)) {
            tcpautoalku = 1;
            continue;
            }
			if (!wmemcmp(fldn+pos+1, L"COM", 3))
				pos += 3;
			pos++;
			portparam[ny-1] = fldn[pos++] - L'0';
			if (fldn[pos] >= L'0' && fldn[pos] <= L'9')
			   portparam[ny-1] = 10*portparam[ny-1] + fldn[pos++] - L'0';
            if (fldn[pos] == L'I' || fldn[pos+1] == L'I')
               comdir[ny-1] = L'I';
            if (fldn[pos] == L'O' || fldn[pos+1] == L'O')
			   comdir[ny-1] = L'O';
            continue;
            }
		 if( !wmemcmp(fldn, L"PORTBASE=",9)) {
			portbase = _wtoi(fldn+9);
			continue;
            }
         if( !wmemcmp(fldn, L"MAXYHTEYS=",10)) {
            maxyhteys = _wtoi(&fldn[10]);
            if (maxyhteys > MAX_LAHPORTTI)
               maxyhteys = MAX_LAHPORTTI;
            continue;
            }
         if (!wmemcmp(fldn, L"VÄLITÄ",6) && (pos = 6) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
			y = 0;
			if (fldn[pos] == L'=' && wcslen(fldn+pos) > 3)
				y = _wtoi(fldn+pos+3);
			if (y > 0 && y <= 32)
				mask = _lrotl(1, y-1);
			else
				mask = 0xffffffff;
            if (pos == 6) {
               for (y = 0; y < MAX_LAHPORTTI; y++)
				  estavalitys[y] |= mask;
               }
            else
               estavalitys[ny-1] |= mask;
            continue;
            }
         if (!wmemcmp(fldn, L"KATKKIER",6)) {
            ny = fldn[wcslen(fldn)-1] - L'1';
            if (ny >= 0 || ny < MAX_LAHPORTTI) {
               katkkierto[ny] = 1;
               continue;
			   }
            }
#ifdef PROFILOI
         if (!wmemcmp(fldn, L"PROFILOI",8)) {
			profiloi = 1;
            if (!wmemcmp(fldn+8, L"=2",2))
               profiloi = 2;
			continue;
            }
#endif
      if (!wmemcmp(fldn, L"SULKUSALASANA=",14)) {
          wcsncpy(sulkusalasana, fldn+14, 10);
		  continue;
		  }
      if (!wmemcmp(fldn, L"NAKVIIVE=",9)) {
		   nakviive = _wtoi(fldn+9);
			  continue;
			}
#ifdef SERVER
         if (!wmemcmp(fldn, L"VARASERVER",10)) {
			for (ny = 0; ny < MAX_LAHPORTTI; ny++) {
               varaserver = 1;
               }
            continue;
            }
#endif
         if (!wmemcmp(fldn, L"UDPVIIVEET=",10)) {
            p = wcstok(fldn+10, L"/;,\n");
            if (p) {
			   if ((ny = _wtoi(p)) > 3)
                  UDPviive_lah = ny;
               p = wcstok(NULL, L"/;,\n");
               if (p) {
                  if ((ny = _wtoi(p)) > 3)
                     UDPviive_ts = ny;
                  p = wcstok(NULL, L"/;,\n");
                  if (p) {
                     if ((ny = _wtoi(p)) > 3)
                     UDPviive_lue = ny;
                     p = wcstok(NULL, L"/;,\n");
                     if (p) {
                        if ((ny = _wtoi(p)) > 3)
                            UDPCliWait = ny;
                        }
                     }
                  }
               }
			continue;
            }
         if (!wmemcmp(fldn, L"TCPVIIVEET",10) && (pos = 10) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			p = wcstok(fldn+pos+1, L"/;,\n");
            if (p) {
				TCPviive_lah[ny-1] = _wtoi(p);
				p = wcstok(NULL, L"/;,\n");
				if (p) {
		            TCPviive_nak[ny-1] = _wtoi(p);
					p = wcstok(NULL, L"/;,\n");
					if (p) {
						TCPviive_pak[ny-1] = _wtoi(p);
						}
					}
				}
            continue;
			}
         if( !wmemcmp(fldn, L"BAUD",4) && (pos = 4) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
            wcscpy(baudst[ny-1], &fldn[pos+1]);
            continue;
            }
         if( !wmemcmp(fldn, L"CHECKST",7)) {
            chkstk = 1;
            continue;
            }
         if( !wmemcmp(fldn, L"COMFILE",7)) {
            levypusk = 1;
//            if (fldn[7] && (fldn[8] == L'0' || fldn[8] == L'E')) levypusk = 0;
            if (fldn[7] && fldn[8] == L'S') levypusk = 2;
            continue;
            }
         if( !wmemcmp(fldn, L"ÄÄNI=",5)) {
            aani = fldn[5] - L'0'; 
            continue;
            }
		 if( !wmemcmp(fldn, L"NÄYTÄHAJONTA=EI", 14)) {     // Vaikuttaa ViestiWinin osuustietoikkunaan
				naytahajonta = 0;
			continue;
            }
#ifdef DBGFILE
		 if( !wmemcmp(fldn, L"DBGTOFILE", 7)) {
				dbgtofile = TRUE;
            continue;
            }
#endif
		 if(!wmemcmp(fldn, L"AJAT", 4)) {
            ajanottofl = 1;
			if (fldn[wcslen(fldn)-2] == L',' ||
               fldn[wcslen(fldn)-2] == L'/') {
                  if (fldn[wcslen(fldn)-1] == L'S') sail_ajat = 1;
                  fldn[wcslen(fldn)-2] = 0;
                  }
			if (wcslen(fldn) > 5)
				wcsncpy(aikaflname, &fldn[5], sizeof(aikaflname)/2-1);
            continue;
            }
#ifdef _CONSOLE
         if(!wmemcmp(fldn, L"NÄPPÄIN=", 8)) {
            if ((p = wcstok(fldn+8, L",/")) == NULL) continue;
            y = _wtoi(p);
			if ((p = wcstok(NULL, L",/")) == NULL || (ny = _wtoi(p)) == 0)
               continue;
            ajanottofl = 1;
            keytab[0].ch = (char) y;
            keytab[0].keycode = (char) ny;
            continue;
			}
#endif
         if( !wmemcmp(fldn, L"LÄHAIKA",7) && (pos = 7) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
            ajanottofl = 1;
            com_ajat[ny-1] = 1;
			if (fldn[pos+1] == L'V')
				com_ajat[ny-1] = 2;
			if (fldn[pos+1] == L'P')
				com_ajat[ny-1] = 3;
            continue;
            }
		 if( !wmemcmp(fldn, L"LÄHECODE",8) && (pos = 8) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			lahec[ny-1] = 1;
			if (fldn[pos+1] == L'V')
				lahec[ny-1] = 2;
			continue;
			}
		 if( !wmemcmp(fldn, L"LÄHTIEDOSTOT",8)) {
			y = -1;
			if ((p = wcsstr(fldn, L"/")) != NULL) {
				if (p[1] == L'K')
					lahkaikkitied = true;
				*p = 0;
				}
			if ((p = wcsstr(fldn, L"=")) == NULL)
				p = fldn+wcslen(fldn);
			else
				y = _wtoi(p+1);
			for (; p[-1] >= L'0' && p[-1] <= L'9'; p--) ;
			if ((ny = _wtoi(p)) > 0)
				lah_tiedostot[ny-1] = y;
			else
				for (int i = 0; i < MAX_LAHPORTTI; i++)
					lah_tiedostot[i] = y;
			continue;
			}
         if( !wmemcmp(fldn, L"MAXJONO=",8)) {
			j = _wtoi(fldn+8);
			if (maxjono < j-1) 
				maxjono = j-1;
			continue;
            }
		 if( !wmemcmp(fldn, L"JONOT=",6)) {
            for (p = fldn + 6, j = 0; *p && j < (int)(sizeof(aika_jono[0])/sizeof(aika_jono[0][0])); p++, j++) {
			   if (*p > L'0' && *p <= L'0'+MAXJONO) {
                  aika_jono[0][j] = *p - L'1';
                  if (aika_jono[0][j] > maxjono)
					 maxjono = aika_jono[0][j];
                  }
               }
            continue;
            }
		 if( !wmemcmp(fldn, L"YHTAIKAJONO",11) && (pos = 11) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			if (ny > 0 && ny <= MAX_LAHPORTTI) {
				yhtaikajono[ny-1] = _wtoi(fldn+pos+1)-1;
				if (yhtaikajono[ny-1] < 0 || yhtaikajono[ny-1] >= MAXJONO)
					yhtaikajono[ny-1] = 0;
				if (maxjono < yhtaikajono[ny-1])
					maxjono = yhtaikajono[ny-1];
				if (yhtaikajono[ny-1] > 0)
					yhtjonot = 1;
				}
            continue;
            }
         if( !wmemcmp(fldn, L"UUSINAIKA",6)) {
            uusinaika = 120*SEK;
			if ((p = wcsstr(fldn, L"=")) != NULL)
				uusinaika = _wtoi(p+1)*SEK;
			continue;
            }
		 if (!wmemcmp(fldn, L"ESTÄNEG", 7)) {
			 estaneg = 1;
			 continue;
		 }
		 if (!wmemcmp(fldn, L"ESTÄHAAMUT", 7)) {
			 estaneg = 1;
			 if (fldn[10] == L'=') {
				 estaneg = MINUUTTI * _wtoi(fldn + 11);
			 }
			 continue;
		 }
		 if (!wmemcmp(fldn, L"ESTÄYLIM", 8)) {
			 if (fldn[8] == L'=') {
				 ylimraja = SEK * _wtoi(fldn + 9);
			 }
			 else
				 ylimraja = 10 * SEK;
			 continue;
			}
		 if (!wmemcmp(fldn, L"SAMAPISTE", 9)) {
			if ((p = wcsstr(fldn, L"=")) != NULL)
				samapisteraja = _wtoi(p+1)*SEK;
			continue;
            }
         if( !wmemcmp(fldn, L"MUUTOSKYSELY=", 13)) {       // MUUTOSKYSELY=E vähentää vahvistusten kysymistä ajanotossa
			if (fldn[13] == L'0' || fldn[13] == L'E')
				muutoskysely = 0;
            continue;
            }
		 if (!wmemcmp(fldn, L"OSUUSOLETUS=",12)) {
            if (fldn[12] == L'A') {
			   ol_osuus = 0;
			   os_raja = -2;
               }
			else if (fldn[12] >= L'0' && fldn[12] <= L'9')
               {
               ol_osuus = _wtoi(fldn+12);
               os_raja = 0;
			   }
            continue;
            }
         if (!wmemcmp(fldn, L"OSUUSRAJA=",10)) {
            if (fldn[10] == L'A') {
               ol_osuus = 0;
               os_raja = -1;
               }
			else if (fldn[10] >= L'0' && fldn[10] <= L'9')
			   {
			   ol_osuus = 0;
			   os_raja = _wtoi(fldn+10);
			   }
			continue;
			}
		 if (!wmemcmp(fldn, L"PISTEET=", 8)) {
			 wchar_t *p1, *p2 = NULL;
			 pisteparam |= 2;
			 p = fldn + 8;
			 for (p1 = p; *p1 && wcswcind(*p1, L"/,;") < 0; p1++);
			 if (p[2] == L'/' || (*p1 && *p1 != L'/')) {
				 y = 0;
				 p2 = wcsstr(p, L"/");
				 if (p2)
					 *p2 = 0;
				 p1 = wcstok(p, L",;");
				 while (p1 && p1[0]) {
					 if (p1[1] == 0) {
						 if ((ny = wcswcind(*p1, ch_piste)) >= 0) {
							 aika_tunnus[0][y] = ny - 2;
							 }
						}
					 else
						 aika_tunnus[0][y] = _wtoi(p1);
					 y++;
					 if (y >= sizeof(aika_tunnus[0]) / sizeof(aika_tunnus[0][0]))
						 break;
					 p1 = wcstok(NULL, L",;");
					}
				 if (p2) {
					 *p2 = L'/';
					 p = p2;
					}
				}
			 else {
				 for (y = 0; y < sizeof(aika_tunnus[0]) / sizeof(aika_tunnus[0][0]); y++, p++) {
					 if ((ny = wcswcind(*p, ch_piste)) >= 0) {
						 aika_tunnus[0][y] = ny - 2;
					 }
					 else
						 break;
				 }
			 }
			 continue;
		 }
		 if (((!wmemcmp(fldn, L"PISTE", 5) && (pos = 5) > 0) || (!wmemcmp(fldn, L"JONOPISTE", 9) && (pos = 9) > 0))
				&& (fldn[pos] == L'=' || (fldn[pos] >= L'0' && fldn[pos] <= L'9'))) {
			 int a = 0, y = MAXJONO-1;
			 pisteparam |= 1;
			 if (fldn[pos] != L'=') {
				 ny = yhteys_no(fldn, &pos)-1;
				 a = ny;
				 y = ny;
				 if (y > maxjono)
					 maxjono = y;
				 }
			 pos++;
			 for (ny = a; ny <= y; ny++) {
				 if (fldn[pos] == L'M')
					 va_mpiste[ny] = 0;
				 else if (fldn[pos] == L'L')
					 va_mpiste[ny] = -1;
				 else if (fldn[pos] == L'A')
					 va_mpiste[ny] = -2;
				 else
					 va_mpiste[ny] = _wtoi(fldn + pos);
				}
			if (!valiajat)
			   valiajat = -1;
			continue;
			}
		 if( !wmemcmp(fldn, L"VAINVÄLIAJAT",12)) {
			vainvaliajat = 1;
			continue;
			}
		 if (!wmemcmp(fldn, L"LÄHTÖRYHMÄ=", 11)) {
			 lahtoryhma = _wtoi(fldn + 11);
			 continue;
		 }
		 if (!wmemcmp(fldn, L"VUOROMAALI", 8)) {        // Antaa ajan maaliajaksi, kun väliajat ovat täynnä, vaikka ei muuten antaisi
            kilpparam.vuoromaali = 1;
			continue;
            }
#ifdef ENNAKOI
		 if( !wmemcmp(fldn, L"ENNAKOI",7)) {
			ennakoi = 1;
			continue;
			}
#endif
		 if( !wmemcmp(fldn, L"AIKAERO=", 8)) {
			for (int i = 0; i <= NREGNLY; i++)
				aikaero[i] = SEK * _wtoi(fldn+8);
			continue;
			}
#ifndef NOCOM_AIKA
		 if( !wmemcmp(fldn, L"AIKA_COM=",9)) {
			aika_com = _wtoi(fldn+9);
			ajanottofl = 1;
			com_aika = 1;
            continue;
            }
         if( !wmemcmp(fldn, L"AIKA_MASK=",10)) {
            aika_int_mask = (char) _wtoi(&fldn[10]);
            continue;
            }
         if( !wmemcmp(fldn, L"AIKA_ESTO=",10)) {
            aika_esto = _wtoi(&fldn[10]); 
			continue;
			}
#endif
#if NREGNLY > 0
		 if ((!wmemcmp(fldn, L"REGNLYNO", 8) && (pos = 8) != 0)
			 || (!wmemcmp(fldn, L"KELLONO", 7) && (pos = 7) != 0)
			 || (!wmemcmp(fldn, L"COMETNO", 7) && (pos = 7) != 0)
			 || (!wmemcmp(fldn, L"TIMYNO", 6) && (pos = 6) != 0)
			 || (!wmemcmp(fldn, L"TDCNO", 5) && (pos = 5) != 0)
			 || (!wmemcmp(fldn, L"ASCNO", 5) && (pos = 5) != 0)) {
			 if (fldn[pos] < L'0' || fldn[pos] > L'9') {
				 if (!wmemcmp(fldn + pos, L"=EI", 3)) {
					 regnly_no[0] = 0;
				 }
				 else
					 regnly_no[0] = 1;
				 for (int i = 1; i < NREGNLY; i++)
					 regnly_no[i] = regnly_no[0];
			 }
			 ny = yhteys_no(fldn, &pos);
			 if (ny > 0 && ny <= NREGNLY) {
				 if (!wmemcmp(fldn + pos, L"=EI", 3)) {
					 regnly_no[ny - 1] = 0;
				 }
				 else
					 regnly_no[ny - 1] = 1;
				 continue;
			 }
		 }
		 if (!wmemcmp(fldn, L"REGNLY", 6)) {
			   ajanottofl = 1;
			   regnly[0] = 1;
			   if (fldn[6] == L'2') regnly[0] = 2;
			   port_regnly[0] = 1;
			   if ((p = wcstok(fldn, L"=")) != NULL) {
				  if ((p = wcstok(NULL,L":,-/")) != NULL) {
					 port_regnly[0] = _wtoi(p);
					 if ((p = wcstok(NULL,L":,-/")) != NULL) {
						od_regnly = _wtoi(p)*2;
						if (od_regnly < 4) od_regnly = 10;
						if ((p = wcstok(NULL,L":,-/")) != NULL) {
						   maxero = _wtoi(p) * 10L;
						   if (maxero == 0) maxero = 2000000L;
						   if (maxero < 20) maxero = 600;
						   }
						}
					 }
				  }
			continue;
			}
#endif
         if (!wmemcmp(fldn, L"SALLITOISTO",10)) {
				p = wcsstr(fldn, L"=");
				if (p && (p[1] == L'E' || p[1] == L'0'))
					ohitatoisto = 1;
				else
					ohitatoisto = 0;
            continue;
            }
#ifdef LAJUNEN
         if (!wmemcmp(fldn, L"SIRITREUNA=",11)) {
				switch (fldn[11]) {
					case L'E':
						siritreuna = 2;
						break;
					case L'T':
						siritreuna = 1;
						break;
					case L'M':
						siritreuna = 3;
						break;
					}
            continue;
            }
		 if (!wmemcmp(fldn, L"SIRITSYNC",8)) {
			siritsync = true;
			continue;
			}
         if (!wmemcmp(fldn, L"SIRITARRIVE=",7)) {
			p = wcsstr(fldn, L"=");
			if (p)
				wcstoansi((char *)siritarrive, p+1, sizeof(siritarrive)-1);
            continue;
            }
         if (!wmemcmp(fldn, L"SIRITDEPART=",7)) {
			p = wcsstr(fldn, L"=");
			if (p)
			    wcstoansi((char *) siritdepart, p+1, sizeof(siritdepart)-1);
			continue;
            }
		 if (!wmemcmp(fldn, L"SIRITEVENTPORT=", 15)) {
			 SiritEventPort[0] = _wtoi(fldn + 15);
			 continue;
			}
		 if (!wmemcmp(fldn, L"SIRITCMDPORT=", 13)) {
			 SiritCmdPort[0] = _wtoi(fldn + 13);
			 continue;
			}
		 if (!wmemcmp(fldn, L"SIRITPOLL=", 10)) {
			 SiritPoll[0] = 1000 * _wtoi(fldn + 10);
			 continue;
			}
#endif
#ifdef ALGE
         if (!wmemcmp(fldn, L"ALGE",4) || !wmemcmp(fldn, L"COMET",5)
			|| !wmemcmp(fldn, L"TIMY",4)
			|| !wmemcmp(fldn, L"FEIG",4)
			|| !wmemcmp(fldn, L"IMPINJ",6)
			|| !wmemcmp(fldn, L"SIRIT",5)) {
            if( !wmemcmp(fldn, L"COMET",5))
			   regnly[0] = 4;
            else if (!wmemcmp(fldn, L"TIMY",4))
               regnly[0] = 6;
#ifdef LAJUNEN
			else if (!wmemcmp(fldn, L"FEIG",4)) {
			   regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = 31;
			   }
			else if (!wmemcmp(fldn, L"SIRIT",5)) {
				regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = 30;
			   kaikki_ajat[1] = 2;
			   }
			else if (!wmemcmp(fldn, L"IMPINJ",6)) {
				regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = 33;
			   kaikki_ajat[1] = 2;
			   }
#endif
			else if (fldn[4] == L'4')
               regnly[0] = 5;
            else
               regnly[0] = 3;
            port_regnly[0] = 1;
			ajanottofl = 1;
			if ((p = wcstok(fldn, L"=")) != NULL) {
				p = fldn+wcslen(p);
				if (!wmemcmp(p+1, L"TCP", 3)) {
					 gettcpipparam(MAX_LAHPORTTI, p+4, 0);
					  continue;
					  }
				if (p[1] >= L'0' && p[1] <= L'9' &&(p = wcstok(NULL,L":,-/")) != NULL) {
					port_regnly[0] = _wtoi(p);
					if ((p = wcstok(NULL,L":,-/")) != NULL) {
						maxero = _wtol(p) * 10L;
						if (maxero == 0) maxero = 2000000L;
						if (maxero < 20) maxero = 600;
						}
					}
				}
            continue;
            }
#endif
         if( !wmemcmp(fldn, L"KELLOBAUD=",10)) {
            y2 = _wtoi(fldn+10);
			kello_baud = 9;
            y1 = 38400L;
            while (y1 > y2 && kello_baud > 2) {
               kello_baud--;
               y1 = y1/2;
			   }
            continue;
			}
		 if (!wmemcmp(fldn, L"VAROITUSKYNNYS=", 15)) {   // Kertoo, milloin varoitetaan poikkeavan hyvästä osuusajasta
			 varkynnys[0][0] = _wtoi(fldn + 15)*SEK;
			 continue;
		 }
		 if (!wmemcmp(fldn, L"VAIKAVERT=", 10)) {
            vaikavert = _wtoi(fldn+10);
            continue;
            }
		 if(!wmemcmp(fldn, L"ILMTUNT=EI", 9)) {
            ilmtunt = 0;
            continue;
            }
         if(!wmemcmp(fldn, L"ILMTUNT", 6)) {
			ilmtunt = 2;
            continue;
            }
         if(!wmemcmp(fldn, L"VA-AJAT", 7) || !wmemcmp(fldn, L"YHTEYSAJAT", 10)) {
            if (!valiajat)
               valiajat = -1;
            vaajat = 1;
			lahdepistehaku = 1;
            ajanottofl = 1;
			if (ilmtunt == 1)
				ilmtunt = 0;
            continue;
			}
         if( !wmemcmp(fldn, L"AIKAKORJAUS",10)) {
			pos = 11;
			if ((ny = yhteys_no(fldn, &pos)) > 0 && ny <= MAXJONO) {
				aikakorjaus[ny-1] = _wtoi(fldn+pos+1) * SEK;
				}
            continue;
			}
         if(!wmemcmp(fldn, L"LISÄÄEDELLE", 8)) {
            lisaa_edelle = 1;
            continue;
            }
#if defined(EMIT)
#if !defined(LUENTA)
         if( !wmemcmp(fldn, L"LAINALUETTELO",9)) {
			lainatied = 1;
            if (!wmemcmp(fldn+13, L"=V", 2))
			   lainatied = 2;
            continue;
            }
#endif
         if( !wmemcmp(fldn, L"JÄLKISY",7)) {
            jalkisyotto = 1;
            continue;
			}
#ifdef JOUKKUEEMIT
		 if( !wmemcmp(fldn, L"JOUKKUEEMIT",9)) {
			joukkueemit = 1;
			continue;
            }
#endif
         if( !wmemcmp(fldn, L"EMIT",5)) {
			emitfl = 1;
            continue;
			}
#ifndef _CONSOLE
		 if( !wmemcmp(fldn, L"EMITANALYYSI",8)) {
			emitfl = 1;
			emitanal_fl = 1;
			if ((p = wcsstr(fldn, L"=")) != NULL)
				kilpparam.analoolisa = _wtoi(p+1);
			continue;
			}
		 if(!wmemcmp(fldn, L"RASTIVATULOSTE", 9)) {
			RastiVaTuloste = 1;
			emitfl = 1;
			emitanal_fl = 1;
			continue;
			}
		 if( !wmemcmp(fldn, L"KARTTA=",7)) {
			wcsncpy(kilpparam.kartta, fldn+7, sizeof(kilpparam.kartta)/2-1);
			continue;
			}
#endif
		if( !wmemcmp(fldn, L"LAINAT",5)) {
			ilmlainat = 1;
			continue;
			}
		 if( !wmemcmp(fldn, L"LEIMAT=E",8)) {
			ohitaleimat = 1;
			continue;
			}
#if NREGNLY > 0
		 if( !wmemcmp(fldn, L"EMITKELLO",5)) {
			emitfl = 1;
            regnly[0] = 11;
			ajanottofl = 1;
            port_regnly[0] = 1;
            if ((p = wcstok(fldn, L"=")) != NULL) {
               if ((p = wcstok(NULL,L":,-/")) != NULL) {
                  port_regnly[0] = _wtoi(p);
                  if ((p = wcstok(NULL,L":,-/")) != NULL) {
                     od_regnly = _wtoi(p)*2;
                     if (od_regnly < 4) od_regnly = 10;
                     }
                  }
               }
            continue;
			}
		 if( !wmemcmp(fldn, L"LUKIJA",6)) {
		    pos=6;
            ny = yhteys_no(fldn, &pos) - 1;
			if (pos == 6 || ny > NREGNLY-1 || ny < 0)
	            ny = NREGNLY-1;
			regnly[ny] = 12;
            port_regnly[ny] = 1;
            if (ajanottofl == -1)
               ajanottofl = 0;
			emitfl = 1;
#ifdef TCPLUKIJA
            if (!wmemcmp(fldn+pos+1, L"UDP", 3)) {
				if (fldn[pos+4] == L'S')
					pos++;
				getudpstreamipparam(ny+MAX_LAHPORTTI, fldn+pos+4);
                continue;
				}
			if (!wmemcmp(fldn+pos+1, L"TCP", 3)) {
				if (fldn[pos+4] == L'S')
					pos++;
				if ((p = wcsstr(fldn+pos+4, L"/P")) != NULL) {
					TCPpoll[ny] = _wtoi(p+2) * 1000;
					*p = 0;
					}
                gettcpipparam(ny+MAX_LAHPORTTI, fldn+pos+4, fldn[pos+3] == L'S');
                continue;
                }
#endif
            if ((p = wcstok(fldn, L"=")) != NULL) {
               if ((p = wcstok(NULL,L":,-/")) != NULL) {
                  port_regnly[ny] = _wtoi(p);
                  if ((p = wcstok(NULL,L":,-/")) != NULL) {
					if (*p == L'U')
						usb_regnly[ny] = 1;
                     }
                  }
               }
			continue;
			}
         if( !wmemcmp(fldn, L"MTR",3)) {
            ny = NREGNLY-1;
            if (fldn[3] > L'0' && fldn[3] <= L'0'+NREGNLY)
			   ny = fldn[3] - L'1';
            if (ajanottofl == -1)
               ajanottofl = 0;
			emitfl = 1;
            regnly[ny] = 13;
            port_regnly[ny] = 1;
			if ((p = wcstok(fldn, L"=")) != NULL) {
               if ((p = wcstok(NULL,L":,-/")) != NULL) {
					if (*p == L'T')
						port_regnly[ny] = 999;
					else
		                port_regnly[ny] = _wtoi(p);
				  }
               }
            continue;
            }
         if (!wmemcmp(fldn, L"EMITAG",6)) {
			ny = NREGNLY-1;
            if (fldn[6] > L'0' && fldn[6] <= L'0'+NREGNLY)
               ny = fldn[6] - L'1';
            regnly[ny] = 14;
			port_regnly[ny] = 1;
			if (ajanottofl == -1)
               ajanottofl = 0;
			emitfl = 1;
            if ((p = wcstok(fldn, L"=")) != NULL) {
               if ((p = wcstok(NULL,L":,-/")) != NULL) {
                  port_regnly[ny] = _wtoi(p);
                  if ((p = wcstok(NULL,L":,-/")) != NULL) {
						if (*p == L'U')
							usb_regnly[ny] = 1;
						if (*p == L'C')
							usb_regnly[ny] = 2;
                     }
                  }
               }
            continue;
			}
			if (!wmemcmp(fldn, L"ECAIKA",4)) {
				emittime = 1;
				continue;
				}
#ifdef EMITHTTP
			if (!wmemcmp(fldn, L"ETSQL", 5)) {
				ny = NREGNLY - 1;
				if ((ny = _wtoi(fldn + 5) - 1) >= NREGNLY || ny < 0)
					ny = NREGNLY - 1;
				regnly[ny] = 16;
				eTParam.eTrno = ny;
				ajanottofl = 1;
				emitfl = 1;
				lahdepistehaku = 1;
				if (fldn[wcslen(fldn) - 1] != L'L')
					eTParam.eThaku = 1;
				if ((p = wcstok(fldn, L"=")) != NULL) {
					y = 0;
					while ((p = wcstok(NULL, L":,-/")) != NULL) {
						if (y < sizeof(eTParam.eTserial) / sizeof(eTParam.eTserial[0]))
							eTParam.eTserial[y++] = _wtoi(p);
					}
				}
				continue;
			}
			if (!wmemcmp(fldn, L"ETGPRS",6)) {
			ny = NREGNLY-1;
			if (fldn[6] > L'0' && fldn[6] <= L'0'+NREGNLY)
			   ny = fldn[6] - L'1';
			regnly[ny] = 15;
				eTParam.eTrno = ny;
			ajanottofl = 1;
			emitfl = 1;
				if (fldn[wcslen(fldn)-1] != L'L')
					eTParam.eThaku = 1;
			if ((p = wcstok(fldn, L"=")) != NULL) {
					y = 0;
			   while ((p = wcstok(NULL,L":,-/")) != NULL) {
				  if (y < sizeof(eTParam.eTserial)/sizeof(eTParam.eTserial[0]))
							eTParam.eTserial[y++] = _wtoi(p);
				  }
			   }
			continue;
			}
			if (!wmemcmp(fldn, L"ETHAKUVÄLI=",11)) {
				eTParam.eThakuvali = 2*_wtoi(fldn+11);
				emitfl = 1;
				if (wcsstr(fldn, L"/A") != NULL)
					eTParam.autostart = true;
				continue;
				}
			if (!wmemcmp(fldn, L"ETDATE=",7)) {
				wcsncpy(eTParam.eTdate, fldn+7, 10);
				if (wcslen(eTParam.eTdate) == 8) {
					wmemmove(eTParam.eTdate+8, eTParam.eTdate+6, 3);
					wmemmove(eTParam.eTdate+5, eTParam.eTdate+4, 2);
					eTParam.eTdate[4] = L'-';
					eTParam.eTdate[7] = L'-';
					}
				else if (wcslen(eTParam.eTdate) == 10) {
					eTParam.eTdate[4] = L'-';
					eTParam.eTdate[7] = L'-';
					}
				continue;
				}
			if (!wmemcmp(fldn, L"ETTIME=",7)) {
				wcsncpy(eTParam.eTtime, fldn+7, 8);
				if (wcslen(eTParam.eTtime) == 2)
					wcscat(eTParam.eTtime, L":00:00");
				if (wcslen(eTParam.eTtime) == 6) {
					wmemmove(eTParam.eTtime+6, eTParam.eTtime+4, 3);
					wmemmove(eTParam.eTtime+3, eTParam.eTtime+2, 2);
					}
				if (wcslen(eTParam.eTtime) == 8) {
					eTParam.eTtime[2] = eTParam.eTtime[5] = L':';
					}
				continue;
				}
			if (!wmemcmp(fldn, L"ETHOST=",7)) {
				if ((p = wcstok(fldn+7, L":")) != NULL) {
					wcsncpy(eTParam.eThost, p, sizeof(eTParam.eThost)/2-1);
					if ((p = wcstok(NULL, L":")) != NULL)
						eTParam.eTport = _wtoi(p);
					}
				continue;
				}
#endif
         if( !wmemcmp(fldn, L"AUTOKILP",8)) {
			autokilp = 1;
            ajat_on = 1;
			continue;
            }
         if( !wmemcmp(fldn, L"COMAUTORAP",10)) {
			comautorap = 1;
			autorap = 2;
            if (fldn[10] == L'=') {
			   ny = 11;
			   if (fldn[11] == L'P' || fldn[12] == L'P') {
				  autorap |= 8;
                  if (fldn[11] == L'P')
                     ny = 12;
                  }
               switch (fldn[ny]) {
                  case L'0':
                  case L'E':
                     autorap = 0;
                     break;
                  case L'1':
                  case L'H':
                  case L'S':
					 autorap &= 8;
                     autorap |= 1;
                     break;
                  }
               }
            continue;
            }
         if( !wmemcmp(fldn, L"AUTORAP",7)) {
            autorap = 2;
            if (fldn[7] == L'=') {
			   ny = 8;
               if (fldn[8] == L'P' || fldn[9] == L'P') {
                  autorap |= 8;
                  if (fldn[8] == L'P')
					 ny = 9;
                  }
               switch (fldn[ny]) {
                  case L'0':
                  case L'E':
                     autorap = 0;
					 break;
                  case L'1':
                  case L'H':
                  case L'S':
					 autorap &= 8;
                     autorap |= 1;
                     break;
                  }
               }
			continue;
			}
		 if (!wmemcmp(fldn, L"SIVUJAKO",8)) {
            emitsivu = 1;
            if (fldn[8] == L'=')
			   emitsivu = _wtoi(fldn+9) - 1;
			continue;
            }
         if (!wmemcmp(fldn, L"ESTÄEMITTOISTO",10)) {
            estaemittoisto = (wmemcmp(fldn+wcslen(fldn)-2, L"EI", 2) != 0);
            continue;
            }
         if (!wmemcmp(fldn, L"SALLIEMITTOISTO",10)) {
			estaemittoisto = (wmemcmp(fldn+wcslen(fldn)-2, L"EI", 2) == 0);
            continue;
            }
         if( !wmemcmp(fldn, L"LÄHEMIT",7) && (pos = 7) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
			emitfl = TRUE;
            com_emit[ny-1] = 1;
            if (fldn[pos] == L'=') {
               if (fldn[pos+1] == L'I')
                  com_emit[ny-1] = -1;
               if (fldn[pos+1] == L'O')
                  com_emit[ny-1] = 2;
			   if (fldn[pos+1] == L'V')
				  com_emit[ny-1] = 3;
			   }
			continue;
            }
         if( !wmemcmp(fldn, L"LÄHEMVA",7) && (pos = 7) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
			emitfl = TRUE;
            com_emitva[ny-1] = 1;
			emitvafl = true;
			continue;
			}
		 if (!wmemcmp(fldn, L"EMVALOKI",8)) {
			emitvaloki = true;
			continue;
			}
		 if( !wmemcmp(fldn, L"AIKALUKIJA",6)) {
            ny = 0;
			if ((ny = _wtoi(fldn+10)) > NREGNLY || ny < 0)
				ny = 0;
			p = wcsstr(fldn, L"=");
			if (p)
				pos = p - fldn;
			else
				pos = 10;
			if (!wmemcmp(fldn+pos,L"=V",2) || !wmemcmp(fldn+pos,L"=K",2) || !wmemcmp(fldn+pos,L"=L",2)) {
               ohitatoisto = 1;
               if (fldn[pos+1] == L'K')
				  ohitatoisto = 0;
			   if (kaikki_ajat[ny] < 2)
				  kaikki_ajat[ny] = 2;
			   if (fldn[pos+1] == L'L') {
					 if (ny == 0)
						 continue;
					 vainpiste[ny] = -3;
					 kaikki_ajat[ny] = 4;
					 lahdepistehaku = 1;
					 
					 pos = wcslen(fldn) - 1;
	                 if (fldn[pos] >= L'0' && fldn[pos] <= L'9') {
						while(fldn[pos-1] >= L'0' && fldn[pos-1] <= L'9')
							pos--;
						lukijalahde[ny-1] = _wtoi(fldn+pos);
						}
					 else
						lukijalahde[ny-1] = ny;
					 continue;
				  }
               pos = wcslen(fldn) - 1;
			   if (fldn[pos] >= L'0' && fldn[pos] <= L'9') {
				  while(fldn[pos-1] >= L'0' && fldn[pos-1] <= L'9')
					  pos--;
				  vainpiste[ny] = _wtoi(fldn+pos);
				  if (kaikki_ajat[ny] < 4)
					 kaikki_ajat[ny] = 3;
				  }
			   else if (fldn[pos] == L'A') {
				   vainpiste[ny] = -2;
				   }
			   else {
				  vainpiste[ny] = -3;
				  kaikki_ajat[ny] = 4;
				  }
			   }
			else
			   kaikki_ajat[ny] = 1;
			ajanottofl = 1;
            continue;
            }
#if defined(EMITLEIMAT)
         if( !wmemcmp(fldn, L"MAXEMVALAH",8)) {
			maxemvalah = 49;
            continue;
			}
         if( !wmemcmp(fldn, L"LÄHDEPISTEET",8)) {
			lahdepistehaku = 1;
			continue;
			}
         if( !wmemcmp(fldn, L"SJBOX",5)) {
            lahdepistehaku = 1;
            continue;
            }
		 if( !wmemcmp(fldn, L"KAKSITUNN",9)) {
			if (kilpparam.kaksibadge == 0)
				kilpparam.kaksibadge = 1;
			continue;
			}
#endif
         if( !wmemcmp(fldn, L"VAADIA",6)) {
            vaadiaika = 1;
            continue;
            }
         if( !wmemcmp(fldn, L"EMITTO",6)) {          // varoittaa saman Emitkoodin käytästä kahdella osuudella
            if (fldn[wcslen(fldn)-1] == L'E' || fldn[wcslen(fldn)-2] == L'E' )
			   emittoisto = 0;
            else
               emittoisto = 1;
			continue;
            }        
#ifdef ESITARK
		 if (!wmemcmp(fldn, L"ESITA",5)) {
				esitark = -1;
            p = wcstok(fldn, L"=/");
				if (p) {
               p = wcstok(NULL, L"=/");
					if (p) {
						switch (*p) {
							case L'T':
								esitark = 1;
								break;
							case L'E':
							case L'0':
								esitark = 0;
								break;
							}
						}
					}
            continue;
            }
#endif
         if( !wmemcmp(fldn, L"VAPAAEMIT=",10)) {
			vapaaemit =  _wtoi(fldn+10);
            continue;
            }
		 if( !wmemcmp(fldn, L"JOUSTOVIESTI",9)) {
			joustoviesti = 1;
			continue;
			}
		 if( !wmemcmp(fldn, L"JOUSTOPAKOTA",9)) {
			joustopakotavaihto = 1;
			continue;
			}
		 if( !wmemcmp(fldn, L"LUENTATESTI",8)) {
			sisaanluenta = 2;
			luentatesti = 1;
			if (wcslen(fldn) > 12 && fldn[12] == L'A')
			   luentatesti = 2;
			continue;
			}
#ifdef LUENTA
		 if( !wmemcmp(fldn, L"TARKAVOIMET",6)) {
			tarkavoimet = 1;
			continue;
			}
		 if( !wmemcmp(fldn, L"SALLIEMITENSIN",8)) {
            salli_emit_ensin = 1;
            continue;
            }
#endif
		 if( !wmemcmp(fldn, L"SALLIUUSINTALUKU",8)) {
			huomaatark = 0;
			continue;
			}
#endif
		 if( !wmemcmp(fldn, L"EMITLUKU",6)) {
			val_korkein = 1;
			continue;
			}
		 if (!wmemcmp(fldn, L"VALONÄYTTÖ=",11)) {
			valonaytto = _wtoi(fldn+11);
			continue;
			}
#endif
		 if (!wmemcmp(fldn, L"RAPORTTIHAK=",12)) {
			if (RaporttiHak[0][0])
				wcsncpy(RaporttiHak[1], fldn+12, 200);
			else
				wcsncpy(RaporttiHak[0], fldn+12, 200);
			continue;
			}
		 if (!wmemcmp(fldn, L"DEVELOPER",9)) {
			DeveloperMode = true;
			continue;
			}
		 swprintf(msg, L"Tuntematon parametri %.17s %s %s", fldn,
			(fld == argv) ? L"komentorivillä" : L"tiedostossa ",
			(fld == argv) ? L"" : cfgfilename);
		 writeerror_w(msg,0);
         }
      if (i <= 1) {
         readcfg(&nf, argptr, i);
         }
      }
	}

#ifndef _CONSOLE
void lueKorRajat(void)
   {
   wchar_t rajaflnm[] = L"korostus.lst", buf[200], srjnm[200];
   int srj, rj1, d;
   unsigned int rj2;
   TextFl *rajafl;

	if (!korRaja)
		korRaja = (int (*)[2]) calloc(sarjaluku, 2*sizeof(int));
	if (!vipLaji)
		vipLaji = (wchar_t *) calloc(maxrec+1, 2);
	else
		wmemset(vipLaji, 0, maxrec+1);
	for (srj = 0; srj < sarjaluku; srj++) {
	  korRaja[srj][0] = 3;
	  korRaja[srj][1] = 5;
	  }
   rajafl = new TextFl(rajaflnm, L"rt");
   if (rajafl->IsOpen()) {
	  while (!rajafl->Feof()) {
		 if (rajafl->ReadLine(buf, sizeof(buf)/2-2)) {
		   upcasewstr(buf);
			if (swscanf(buf, L"%s %d %ud", srjnm, &rj1, &rj2) == 3) {
			   if (!wcscmp(srjnm, L"VIP")) {
				  if ((d = getpos(rj1)) > 0) {
					 vipLaji[d] = (wchar_t) rj2;
					 }
				  }
			   else if (!wcscmp(srjnm, L"VÄRI")) {
				  if (rj1 >= 1 && rj1 <= (int)(sizeof(korVari)/sizeof(korVari[0]))) {
					 korVari[rj1-1] =
						rj2%1000 + 256*((rj2/1000)%1000) + 65536*(rj2/1000000);
					 }
				  }
			   else if (!wmemcmp(srjnm, L"OLETUS", 6)) {
				  for (srj = 0; srj < sarjaluku; srj++) {
							korRaja[srj][0] = rj1;
					 korRaja[srj][1] = rj2;
					 }
				  }
			   else if ((srj = haesarja_w(srjnm, false)) >= 0) {
				  korRaja[srj][0] = rj1;
				  korRaja[srj][1] = rj2;
				  }
				}
			}
		 }
	  }
	delete rajafl;
	}
#endif

#ifdef _CONSOLE

static int ConsoleAloitus(int *y)
{
	char ch, ch2, prs[120], msg[100];
	wchar_t wch, wst[100];

   if (ikkunaots[0])
	   SetConsoleTitleW(ikkunaots);
   viwrrect(11,5,11,30,"Voimassa olevat valinnat :",7,0,0);
   ch = 0;
   *y = 13;
korjval:
   clrln(ySize-3);
   if (ch) {
	  for (*y=11; *y<ySize-3; (*y)++) clrln(*y);
	  viwrrect(11,5,11,23,"Korjatut valinnat :",7,0,0);
	  clrln(ySize-3);
      }
/*
	  viwrrect(ySize-3,5,ySize-3,24,"Anna tarratiedosto :",7,0,0);
	  viwrrect(ySize-1,5,ySize-1,54,
		 "NUL : Ei tarroja,  LÄHETÄ : Toisella tietokoneella",7,0,0);
	  inputstr(tarfname,39,26,ySize-3,"\r",&ch2,FALSE);
	  clrln(ySize-1);
	  upcasestr(tarfname);
      }
   lahtarfl = FALSE;
   elimbl(tarfname);
   if ( !memcmp(tarfname, "NUL", 4) ) {
	  tarfl = FALSE;
	  sprintf(prs,"Tarroja ei kirjoiteta");
	  }
   else if ( !memcmp(tarfname, "LÄHETÄ", 4) ) {
	  tarfl = FALSE;
	  lahtarfl = TRUE;
	  if (strlen(tarfname) > 6) {
		 memset(tarlahcom, 0, sizeof(tarlahcom));
		 for (p = tarfname+6; *p; p++) {
			if (*p > '0' && *p < '1' + MAX_LAHPORTTI)
			   tarlahcom[*p - '1'] = 1;
			}
         }
      sprintf(prs,"Tarrojen kirjoitus toisella tietokoneella");
      }
   else {
	  tarfl = TRUE;
      sprintf(prs,"Tarrakirjoitinportti tai -tiedosto = %s",tarfname);
      }
   vidspmsg(15,5,7,0,prs);
   y = 16;
*/
   if (ch) {
	  clrln(ySize-3);
      viwrrect(ySize-3,5,ySize-3,24,"Anna listatiedosto :",7,0,0);
	  inputwstr(listfname,39,26,ySize-3,L"\r",&wch,FALSE);
	  upcasewstr(listfname);
      }
   elimwbl(listfname);
   swprintf(wst,L"Listakirjoitinportti tai -tiedosto = %s",listfname);
   vidspwmsg(*y,5,7,0,wst);
   if (ch) {
		selectopt("Merkistö: I)BM, A)scii-7, R)oman-8, P)ostscrpt"
			", W)indows,L)evyltä", "ARLIPWG",&merkit);
		if (merkit == 'W') {
			ch2 = ' ';
			selectopt("Käytetäänkö Windowsin kirjoitinajureita (K/E)", "KE", &ch2);
			if (ch2 == 'K')
				merkit = 'G';
			}
		}
   clrln(ySize-3);
   switch (merkit) {
      case 'A' : viwrrect(*y,63,*y,78,"Merkistö Ascii-7",7,0,0);
                 break;
      case 'R' : viwrrect(*y,63,*y,78,"Merkistö Roman-8",7,0,0);
				 break;
      case 'P' : viwrrect(*y,63,*y,79,"Merkistö Postscr.",7,0,0);
                 break;
	  case 'W' : viwrrect(*y,63,*y,78,"Merkistö Windows",7,0,0);
                 break;
	  case 'L' : viwrrect(*y,63,*y,78,"Merkistö levyltä",7,0,0);
				 break;
      case 'G' : viwrrect(*y,63,*y,78,"Windowskirjoitin",7,0,0);
                 break;
	  default  : viwrrect(*y,63,*y,79,"Merkistö IBM (PC)",7,0,0);
      }
   ++*y;
   ++*y;
   if (ch) {
	  switch (aani) {
         case  0 : ch2 = 'E'; break;
         default : ch2 = 'K';
         }
	  selectopt("Äänimerkki: E)i käytössä,  K)äytössä" ,"EK", &ch2);
      switch (ch2) {
         case 'E' : aani = 0; break;
		 case 'K' : aani = 1; break;
         }
      }
   viwrrect(*y,5,*y,16,"Äänimerkki :",7,0,0);
   switch (aani) {
	  case 0  : viwrrect(*y,18,*y,28,"Ei käytössä", 7, 0, 0); break;
      default : viwrrect(*y,18,*y,25,"Käytössä", 7, 0, 0); break;
      }
   ++*y;
   strcpy(msg, "Käytössä yhteydet:");
   for (int nc = 0; nc < MAX_LAHPORTTI; nc++) {
      if (ch && nc < 8) {
         clrln(ySize-3);
         viwrrect(ySize-3,5,ySize-3,46,
            "Anna portin   numero :    (0 : ei käytetä)",7,0,0);
         vidint(ySize-3,17,1,nc+1);
         INPUTINT(&portparam[nc],2,28,ySize-3,"\r",&ch2);
         }
      if( portparam[nc] != 0)
        {
		int i, bd;
		portti[nc] = portparam[nc];
		i = _wtoi(baudst[nc]);
        if (i < 0) {
          if (i > -12000) {
            bd = 5760;
            baud[nc] = 10;
            }
          else if (i > -20000) {
            bd = 11520;
			baud[nc] = 11;
            }
		  else {
			bd = 3840;
			baud[nc] = 9;
            }
          }
        else {
          baud[nc] = 8;
		  bd = 19200;
		  while ( (i < bd) && (baud[nc] > 2) )
			{
            baud[nc]--;
            bd=bd/2;
			}
		  bd /= 10;
          }
        _itow(bd*10L, baudst[nc], 10);
        comkesto[nc] = (18 * 10 * 200) / bd + 20;
        if (comtype[nc] == comtpRS)
		   sprintf(prs, "Yhteys %d : COM%d, nopeus %d0 baud", nc+1, portparam[nc], bd);
        else
           sprintf(prs, "Yhteys %d : %s", nc+1, comtype[nc] & comtpUDP ? "UDP" : "TCP");
        vidspmsg(*y,5,7,0,prs);
        }
	 if( portparam[nc] != 0) {
        lahcom[nc] = (comdir[nc] == L'I') ? FALSE : TRUE;
        vastcom[nc] = (comdir[nc] == L'O') ? FALSE : TRUE;
		if (!lahcom[nc])
           vidspmsg(*y,53,7,0,"Vain vastaanotto           ");
        if (!vastcom[nc])
           vidspmsg(*y,53,7,0,"Vain lähetys               ");
#if MAX_LAHPORTTI > 22
        if (strlen(msg) < 73)
#endif
           sprintf(msg+strlen(msg), " %d", nc+1);
#if MAX_LAHPORTTI > 22
        else if (msg[strlen(msg)-1] != '.')
           strcat(msg, " ..");
#endif
		if (*y < 21) ++*y;
        clrln(*y);
        vidspmsg(*y, 5, 7, 0, msg);
        }
     }
   clrln(ySize-3);
   if (!bootfl) {
	  sccurset(ySize-3,70);
	  vidspmsg(ySize-3,5,7,0,
		 "<Return>: hyväksy valinnat,  K)orjaa valintoja,  <Esc>: keskeytä");
      do {
		 ch = readkbd(&ch2,TRUE,FALSE);
		 ch = toupper(ch);
		 } while (ch != 13 && ch != 27 && ch != 'K');
	  if( ch == 27 ) {
		 scpclr();
		 return(1);
		 }
	  if (ch == 'K') goto korjval;
	  }
   return(0);
	}
#endif // _CONSOLE

void maaraaPuutelisat(void)
{
	if (kilpparam.maxnosuus == 1)
		return;
	for (int srj = 0; srj < sarjaluku; srj++) {
		__int64 lisa = kilpparam.jtulosaskel;
		if (Sarjat[srj].maxnosuus == 1)
			continue;
		for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
			if (Sarjat[srj].nosuus[os] == 1)
				continue;
			lisa /= Sarjat[srj].nosuus[os];
			if (lisa < kilpparam.minpuutelisa)
				lisa = kilpparam.minpuutelisa;
			Sarjat[srj].puutelisa[os] = lisa;
			}
		}
}

void asetaOletusarvot(void)
{
	kilpparam.valuku = 4;
	kilpparam.n_os = 4;
	kilpparam.n_os_akt = kilpparam.n_os;
	kilpparam.osuusluku = kilpparam.n_os_akt;
	kilpparam.ntosuus = kilpparam.osuusluku;
	kilpparam.maxnosuus = 1;
	kilpparam.valuku = 1;
	kilpparam.vatpsize = VASIZE;
	kilpparam.osrecsize = OSRECSIZE;
	kilpparam.osrecsize0 = OSRECSIZE0;
	kilpparam.kilprecsize = KILPRECSIZE;
	kilpparam.kilprecsize0 = KILPRECSIZE0;
	kilpparam.jtulosjak = JTULOSJAK;
	kilpparam.jtulosaskel = JTULOSASKEL;
	kilpparam.minpuutelisa = 50LL*TUNTI;
	kilpparam.laika = 11;
	kilpparam.laikasj = 11;
	kilpparam.laika2 = 8;
	kilpparam.pyor[0] = 1;
	kilpparam.pyor[1] = SEK/100;
	kilpparam.pyor[2] = SEK/100;
	kilpparam.pyor[3] = SEK;
	kilpparam.pyor[4] = SEK;
	kilpparam.chars = '8';
	kilpparam.etusuku = 1;
	kilpparam.lseura = LSEURA;
	kilpparam.lnimi = OSNIMIL;
	kilpparam.sakkoyks = 0;
	kilpparam.WisV = true;
	kilpparam.analoolisa = 20;
	swprintf(kilpparam.tiedlaji, L"VI%d", VersioInt());
	for (int i = 1; i < MAX_LAHPORTTI; i++) {
		kellovali[i] = kellovali[0];
		tarlahcom[i] = 1;
		wcscpy(naapuri[i], naapuri[0]);
		}
	for (int i = 1; i < MAX_LAHPORTTI; i++) {
		wcscpy(naapuri[i], naapuri[0]);
		}
	for (int i = 1; i < MAX_PORTTI; i++) {
		wcscpy(baudst[i], baudst[0]);
		baud[i] = baud[0];
		pty[i] = pty[0];
		combits[i] = combits[0];
		stopbits[i] = stopbits[0];
		}
	for (int i = 0; i < NREGNLY; i++) {
		t0_regn[i] = t0_regnly;
		cn_regnly[i] = MAX_LAHPORTTI + i;
		cn_regnly[i + NREGNLY] = MAX_LAHPORTTI + i + NREGNLY;
		TauluParam[i].DevID = 0xffffffff;
		TauluParam[i].CardID = 1;
		TauluParam[i].Leveys = 128;
		TauluParam[i].Korkeus = 32;
		}
	for (int i = 0; i < NREGNLY + 2; i++) {
		vainpiste[i] = -9;
		}
	for (int i = 0; i <= NREGNLY; i++) {
		kaikki_ajat[i] = -1;
		aikaero[i] = TMAALI0;
		}
	aftulparam.tulostettava = L'T';
	aftulparam.kohde = L'H';
	aftulparam.yksihtml = 0;
	aftulparam.rajaus = L'L';
	aftulparam.language = (oletuskieli == L'E' ? 1 : 0);
	aftulparam.tulmuot = filetulosmuot;
	filetulparam = aftulparam;
	wcscpy(autofileparam.aflstnm, L"AUTOFILE.LST");
	wcscpy(autofileparam.jakelumaar, L"AutoJakelu.xml");
	autofileparam.sanafchars = L'8';

	for (int i = 0; i < MAXSARJALUKU+MAXYHD; i++)
		Sarjat[i].nollaa();
}

int aloitus(int argc, wchar_t *argv[], wchar_t *cfgflnm)
   {
   int  y,bd,i,nc, ntls, ret = 0;
   static char msg0[81] = "PROMPT=KIRJOITA EXIT PALATAKSESI$_";
   wchar_t msg[200], st[100];
	kilptietue kilpx;

	kilpx.kilpno = 0;
#ifdef EMIT
	kilpx.ostiet[1].badge[0] = 0;
	kilpx.ostiet[1].badge[1] = 0;
#endif
   _ltoa(sarjano/10L,snostr,10);
#ifdef _CONSOLE
   nimio();
#else
//   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//   GetVersionEx(&osvi);
#endif
   InitializeCriticalSection(&tall_CriticalSection);
   InitializeCriticalSection(&loki_CriticalSection);
   InitializeCriticalSection(&inb_CriticalSection);
   InitializeCriticalSection(&outb_CriticalSection);
   InitializeCriticalSection(&kilpdat_CriticalSection);
   InitializeCriticalSection(&ajat_CriticalSection);
   InitializeCriticalSection(&emit_CriticalSection);
   InitializeCriticalSection(&rastiva_CriticalSection);
   InitializeCriticalSection(&tcpip_CriticalSection);
   InitializeCriticalSection(&autotul_CriticalSection);

   asetaOletusarvot();

   lue_parametrit(argc, argv, cfgflnm);

	Sleep(100);
	if (inLopetus)
		return(1);

	if (aikaero[NREGNLY] != TMAALI0) {
		for (i = 0; i < NREGNLY; i++) {
			if (aikaero[i] == TMAALI0)
				aikaero[i] = aikaero[NREGNLY];
			}
		}

	if (pisteparam == 1) {
		for (int i = 0; i < MAXJONO; i++)
			va_mpiste[i] = -2;
		}

#ifdef _CONSOLE
   if (ConsoleAloitus(&y))
	   return(1);

#endif  // _CONSOLE

	if (ToimintaTila == 2) {
		for (nc = 0; nc < MAX_LAHPORTTI; nc++) {
			wchar_t *wp;
			if( portparam[nc] != 0) {
				portti[nc] = portparam[nc];
				if (portti[nc] > 0) {
					wp = wcstok(baudst[nc], L",:;");
					if (wp && (i = _wtoi(wp)) > 1000) {
						baud[nc] = 12;
						bd = 220000;
						while (i < bd && baud[nc] > 2) {
							baud[nc]--;
							if (bd == 55000)
								bd = 36000;
							else
								bd = bd/2;
							}
						wp = wcstok(NULL, L",:;");
						if (wp && *wp) {
							pty[nc] = tolower(wchartoansi(*wp));
							}
						wp = wcstok(NULL, L",:;");
						if (wp && *wp) {
							combits[nc] = _wtoi(wp);
							}
						wp = wcstok(NULL, L",:;");
						if (wp && *wp) {
							stopbits[nc] = _wtoi(wp);
							}
						}
					comkesto[nc] = (18 * 10 * 200) / bd + 20;
					}
				lahcom[nc] = (comdir[nc] == L'I') ? FALSE : TRUE;
				vastcom[nc] = (comdir[nc] == L'O') ? FALSE : TRUE;
				}
			}
		}
   InitializeCriticalSection(&print_CriticalSection);
   for (i=11; i <=ySize-1; i++) clrln(i);
   nimiixfl = FALSE;
   for ( i = 0 ; i < MAXSARJALUKU+MAXYHD ; i++) sjfl[i] = TRUE;
/*
   if( tarra1 ) kaksoistar = FALSE;
   if (merkit != 'I') {
      if (!trtar && !trloki) trlst = 1;
      merkisto(merkit);
		if (merkit == 'G') {
			trlst = 2;
			if (trloki)
				trloki = 2;
			}
	  }
   else {
	  trtar = 0;
	  trloki = 0;
	  trlst = 0;
	  }
*/

   if (luesarjat())
		return(1);

   for (int i = 1; i < sarjaluku; i++) {
	   for (int os = 0; os<Sarjat[i].osuusluku; os++)
		   varkynnys[i][os] = varkynnys[0][0];
		}
   kilpparam.osrecsize = kilpparam.osrecsize0 + (kilpparam.valuku + 1) * kilpparam.vatpsize;
   kilpparam.kilprecsize = kilpparam.kilprecsize0 + kilpparam.n_os * kilpparam.osrecsize;
    ylmyoh = kilpparam.maxnosuus > 1 ? 0 : 1;

    maaraaPuutelisat();
	lsttulparam.tulmuot = tulosmuot;
	lsttulparam.language = (oletuskieli == L'E' ? 1 : 0);
	lokitulparam.tulmuot = tulosmuot;
	filetulparam.language = lsttulparam.language;
	aftulparam.language = lsttulparam.language;

	if (loki == 1) {
		if ((lokitulparam.lstf = openprfile(lokifname, -1, FALSE, 1,
			0, immed)) == NULL) {
			writeerror_w(L"",0);
		 return(1);
		 }
	  }

	if (lokitulparam.lstf && lokitulparam.lstf->prno == 2)
		initgdiprinter(&lokitulparam);

#if !defined(_CONSOLE)
	if (!wcscmp(listfname, L"NUL")) {
		if (autostr[sizeof(autostr)/2-1] == '1' || autorap)
			wcscpy(listfname, L"PRN");
		}
#endif
	if (wcscmp(listfname, L"NUL")) {
		if (!wcscmp(listfname, L"OLETUS"))
			wcscpy(listfname, L"PRN");
		if ((lsttulparam.lstf = openprfile(listfname, merkit == L'G' ? 1 : -1, TRUE, FALSE,
			trlst ? trlate : 0, immed)) == NULL) {
			writeerror_w(L"",0);
			return(1);
			}
		if (lsttulparam.lstf) {
			if (lsttulparam.lstf->prno == 2) {
				lsttulparam.printer = GDIPRINTER;
//#if defined(_CONSOLE)
				initgdiprinter(&lsttulparam);
//#else
//				closeprfile(lsttulparam.lstf)
//				lsttulparam.lstf = NULL;
//#endif
				}
			else if (merkit == 'G')
				merkit = 'W';
			emittulparam = &lsttulparam;
			}
//		if (autostr[sizeof(autostr)/2-1] == '1')
//			autoalku(autostr);
		}

	init_muotoilut();
	if (muotoilufl)
		LueXMLMuotoilu(luetfnm);

	for (i=0; i < sarjaluku; i++)
		prior0[i] = 1;
   if (openfile(&datf2,dataf2name,kilpparam.kilprecsize)) {
	   FILE *fl;
	   if ((fl = fopen(dataf2name, "rb")) != NULL) {
		   fseek(fl, 0, SEEK_END);
			fpos_t len;
			fgetpos(fl, &len);
			if (len > kilpparam.kilprecsize) {
				swprintf(msg,L"Tiedoston %s avaaminen ei onnistu. Tiedosto ei vastaa ohjelmaa.",
					ansitowcs(st, dataf2name, 98));
				writeerror_w(msg, 0);
				return(1);
				}
			fclose(fl);
			}
#ifndef _CONSOLE
		swprintf(msg,L"Tiedostoa %s ole. Luodaanko uusi tyhjä tiedosto ilmoittautumistilassa?",
			ansitowcs(st, dataf2name, 98));
		if (Application->MessageBoxW(msg, L"Ei tiedostoa", MB_YESNO) == IDNO)
			return(1);
		ToimintaTila = 1;
		makefile(&datf2,dataf2name,kilpparam.kilprecsize);
		}
#else
		swprintf(msg,L"Tiedostoa %s ole. Ohjelma suljetaan", ansitowcs(st, dataf2name, 98));
		writeerror_w(msg, 0, true);
		return(1);
		}
   viwrrect(17,5,17,43,"KILPAILIJATIETOJEN LATAUS - ODOTA HETKI",7,0,0);
#endif
   if (ToimintaTila < 2) {
	   emitfl = 0;
	   valiajat = 0;
	   }
   else {
	    luesarjayhdistelmat();
		}
	if (!UDPCliWait)
		UDPCliWait = (WinVersion() > 4) ? 10 : 1000;
	UDPsetCliWait(UDPCliWait);

	if ((kilpparam.badgelaji > L' ' || vaajat) && emitfl == 0)
		emitfl = -1;

	if (sisaanluenta > 1)
		emitfl = 0;
#ifndef LUENTA
	if (emitfl) {
#ifdef _CONSOLE
		ProsEmitOn = true;
#endif
		if (esitark)
			estaemittoisto = 0;
		int rno, ka = -1, emitfl_1 = -1;

		// Katso, ovatko kaikki merkinnät yhdenmukaiset
		for (rno = 0; rno < NREGNLY; rno++) {
			if (IntInList(regnly[rno], L"11 12 13 14 21") >= 0 &&
				kaikki_ajat[rno+1] >= 0 && kaikki_ajat[0] >= 0 &&
				(ka = kaikki_ajat[rno+1]) != kaikki_ajat[0]) {
				break;
				}
			}
		// Jos merkinnöissä ei kirjattuja eroja, päivitä tiedot, joita ei vielä kirjattu
		if (rno == NREGNLY) {
			if (kaikki_ajat[0] >= 0)
				ka = kaikki_ajat[0];
			else
				kaikki_ajat[0] = ka;
			for (rno = 0; rno < NREGNLY; rno++) {
				if (IntInList(regnly[rno], L"11 12 13 14 21") >= 0 &&
					kaikki_ajat[rno+1] < 0) {
					kaikki_ajat[rno+1] = ka;
					}
				}
			}
		else
			ka = -1;

		// Aseta emitfl_1 = 1, jos leimantarkastusyhteys mukana
		for (rno = 0; rno < NREGNLY; rno++) {
			if (IntInList(regnly[rno], L"11 12 13 14 21") >= 0 &&
				kaikki_ajat[rno+1] <= 1) {
				emitfl_1 = 1;
				break;
				}
			}
		// Aseta emitfl_1 = 1, jos leimoja siirretään verkossa
		if (emitfl_1 < 0) {
			for (i = 0; i < MAX_LAHPORTTI; i++) {
				if (com_emit[i]) {
					emitfl_1 = 1;
					break;
					}
				}
			}
		// Katso, onko mukana ajanottotoimintoja ja aseta emitfl = -1, jos
		// leimantarkastusta ei ollut.
		for (rno = 0; rno < NREGNLY; rno++) {
			if (kaikki_ajat[rno+1] >= 2) {
				if (lahdepistehaku && kaikki_ajat[rno+1] == 4)
					kaikki_ajat[rno+1] = 3;
				kaikki_ajat[0] = 2;
				emitfl = emitfl_1;
				}
			}
		// Poista suuremmat arvot kuin 2,
		// Aseta kaikki_ajat[0] = 1, jos sekä ajanottoa että leimantarkastusta
		for (rno = 0; rno < NREGNLY; rno++) {
			if (kaikki_ajat[rno+1] > 2)
				kaikki_ajat[rno+1] = 2;
			if (kaikki_ajat[rno+1] < 0)
				kaikki_ajat[rno+1] = 0;
			if (IntInList(regnly[rno], L"11 12 13 14 21") >= 0 &&
				kaikki_ajat[rno+1] <= 1 && kaikki_ajat[0] > 1) {
				kaikki_ajat[0] = 1;
				}
			}
		if (kaikki_ajat[0] >= 2) {
			kaikki_ajat[0] = 2;
			emitfl = -1;
			}
		if (kaikki_ajat[0] < 0)
			kaikki_ajat[0] = 0;
		}

	if (teejarj())
		return(1);

	ntls = 0;
   for (i=0; i < sarjaluku; i++)
	  ntls += ntulost[i];
   if (ntls == 0) {
	   for (i=0; i < sarjaluku; i++) {
		  for (y = 0; y < Sarjat[i].ntosuus; y++) {
			  if (Sarjat[i].ylkynnys[y] != TMAALI0) {
				  writeerror("VAROITUS: Ei tuloksia, mutta yhteislähtöautomatiikka käytössä", 0);
				  i = sarjaluku;
				  break;
				  }
			  }
		  }
	  }
   clrln(17);
   clrln(18);


	if (ToimintaTila == 2 && emitfl == 1) {
		int er;
		if ((er = lue_radat_xml(17)) == -1) {
			if (lue_leimat(17)) {
				return(1);
				}
			if (lue_radat(19)) {
				writeerror_w(L"Ratatietojen lukeminen ei onnistunut", 0);
				return(1);
				}
			}
		else if (er) {
			writeerror_w(L"Ratatietojen lukeminen xml-tiedostosta ei onnistunut", 0);
				return(1);
				}
		if (lueemitfile()) {
			writeerror_w(L"Leimaustietotiedoston lukeminen ei onnistunut", 0);
			return(1);
			}
		}

	if (ToimintaTila == 2 && emitfl == 1) {
		if (lainatied)
			lue_lainat(0, 1);
		}
#endif
	if (!UDPCliWait)
		UDPCliWait = (WinVersion() > 4) ? 10 : 1000;
	UDPsetCliWait(UDPCliWait);
	clrln(ySize-3);
	kaynnistysvaihe = 2;
	initmstimer();
	if (ToimintaTila == 2) {

#ifndef _CONSOLE
	   lueKorRajat();
#endif

		for (nc = 0; nc < NREGNLY; nc++) {
			if (ajanottofl > 0 || regnly[nc]) {
				Sleep(300);
				inittimer();
				break;
				}
			}
	   clrln(ySize-3);
	   if (maxyhteys == 0) {
		  for (nc = 0; nc < MAX_LAHPORTTI; nc++) {
			 if (portparam[nc] != 0)
				maxyhteys = nc + 1;
			 }
		  }
	   if (maxyhteys == 0)
		  maxyhteys = 1;
	   EnterCriticalSection(&inb_CriticalSection);
	   for (nc = 0; nc < maxyhteys; nc++) {
		  if (portparam[nc] != 0) {
			  Sleep(50);
			  initcom(nc);
			  }
		  }
	   LeaveCriticalSection(&inb_CriticalSection);
	   kaynnistysvaihe = 3;
#if defined(TAULU)
	   if (taulu_com || udptaulu) opentaulu();
#endif
		if (!comfl) lahtarfl = FALSE;
#ifdef AUTOFILE
		initHtmlHeader(false);
	   if (afvali0) {
		  autofileparam.afvali = afvali0;
		  startaf(0);
		  }
#endif
#ifdef AUTOVA
	   if (vaajat)
		   start_autova();
#endif
		}
#ifdef _CONSOLE

   initscreens();
#endif

   if (loki) {
		wchar_t lmsg[60];
		swprintf(lmsg, L"Ohjelma käynnistetty, klo %s", wkello());
		wkirjloki(lmsg);
		}
	getWinVersion();

   taustaon = 1;
   taustaThread = _beginthread(taustaThr, 20480, NULL);
   close_err_file();
   kehys(1);
   kaynnistysvaihe = 4;
   if (testi == 2)
	   ReadComTestParams();

   return(0);
   }
