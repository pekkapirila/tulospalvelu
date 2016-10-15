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
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#ifdef _CONSOLE
//#include <bvideo.h>
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include "HkDeclare.h"
#include "HkMuotoilu.h"
#include "TpLaitteet.h"
#include "BuildNo.h"

#define ALGE
#define NRIVI 15

void init_muotoilut(void);
void ReadComTestParams(void);
void addMonitori(wchar_t *str);
extern int monitoriVali;
void Perr(int ok, wchar_t *api);
void close_sakot(void);

wchar_t RaporttiHak[202];
extern bool hav_virheet;
INT16  setySize = 50;
INT32 sarjano = 3100010L;
static wchar_t snostr[16];
INT tarkkuusparam;
static INT tarra1, muotoilufl, ll_muotoilufl, cfgparam;
static wchar_t comdir[MAX_PORTTI];
wchar_t merkit = 'G';
static wchar_t cfgfilename[201] = L"LASKENTA.CFG", autostr[25] = L"";
extern wchar_t lainafname[200];
static INT korkeat = FALSE;
static INT star = FALSE;
static wchar_t baudst[MAX_PORTTI][20] = {L"9600,n,8,1"};
int portparam[MAX_PORTTI];
int aikakorjaus[MAXJONO];
static INT nopeuta = 0;
extern unsigned int prblocksize;
extern int ainauusirec;
extern INT korjaareaaliaika;
extern INT norm_fore, norm_back, inv_fore, inv_back;

extern int UDPviive_lue, UDPviive_lah, UDPviive_ts, UDPCliWait;
extern int UDPvarmistamaton[MAX_PORTTI];
extern int varaserver;
extern int tcpautoalku;
extern int udpjarru;
extern int SiritEventPort[NREGNLY];
extern int SiritCmdPort[NREGNLY];
extern int SiritPoll[NREGNLY];
extern char SiritMask[31];
wchar_t ikkunaots[61];
wchar_t	HenkTKantaNm[200] = L"Henkilot.csv";
#ifdef MAALI
#ifdef EMITLEIMAT
extern INT valinelen, vapagelen, valinelendef, vapagelendef, vapageskip, tarkrappageskip;
#endif
extern INT lainatied;
extern INT sail_ajat;
#ifdef HIIHTOLAHTO
static wchar_t tunn_srj_nimi[12];
#endif
#ifndef NOMOUSE
extern INT mouse;
#endif
#endif
#ifdef AUTOVA
int start_autova(void);

//static INT autova_fl;
#endif

#if defined(TAULU) || defined(TAULU_2)
void opentaulu(void);
#endif

#ifdef GAZ
extern INT gaz_com, gazvaihe;
void opengaz(void);
extern INT gazluku, gazrata[], gazpiste[];
#endif

#ifdef LAJUNEN
extern char siritarrive[80];
extern char siritdepart[80];
#endif

#ifdef SAKKOCOM
   void opensak(void);

   extern INT sak_com, sak_baud, sak_laji;
#endif

#if defined(TV1)
void opentv(void);
#endif

void as_muotoilu(void);
void addgatepiste(int gate, int seq);

void merkisto(wchar_t merkit)
   {
   TextFl *mf;
   INT i,j;
   INT32 len;
   wchar_t merkfname[] = L"MERKISTO.KOD";
   wchar_t psfname[] = L"POSTSCRI.INI";
   wchar_t line[200];
   char *utrlate = (char *) trlate;

   for (i = 0; i < 256; i++) utrlate[i] = i;
   switch (merkit) {
	  case L'A' : utrlate['ƒ'] = 91;
				 utrlate['÷'] = 92;
				 utrlate['è'] = 93;
				 utrlate['‹'] = 94;
				 utrlate['ê'] = L'E';
				 utrlate['‰'] = 123;
				 utrlate['ˆ'] = 124;
				 utrlate['Â'] = 125;
				 utrlate['Å'] = 126;
				 utrlate['Ç'] = L'e';
				 break;
	  case L'R' : utrlate['ƒ'] = 216;
				 utrlate['÷'] = 218;
				 utrlate['è'] = 208;
				 utrlate['‹'] = 219;
				 utrlate['ê'] = 220;
				 utrlate['‰'] = 204;
				 utrlate['ˆ'] = 206;
				 utrlate['Â'] = 212;
				 utrlate['Å'] = 207;
				 utrlate['Ç'] = 197;
#if defined(EMITLEIMAT)
				 wcscpy(initfont_emrap+wcslen(initfont_emrap)-3, L"8U");
				 initfont_emrap[0]--;
				 wcscpy(initfont_emva+wcslen(initfont_emva)-3, L"8U");
				 initfont_emva[0]--;
#endif
				 break;
	  case L'L' :
				mf = new TextFl(merkfname, L"r");
				if (mf->IsOpen()) {
					while (!mf->Feof()) {
						mf->ReadLine(line, 198);
					   if (swscanf(line, L"%d %d\n",&i,&j) != 2) break;
					   utrlate[i] = (char) j;
					   }
					}
				 else writeerror_w(L"Kirjoitinkoodien lukeminen "
					L"tiedostosta MERKISTO.KOD ei onnistu", 0);
				delete mf;
				 break;
	  case L'P' : 
				mf = new TextFl(psfname, L"r");
				if (mf->IsOpen()) {
					len = mf->Length();
					psinit = (wchar_t *) calloc(len+20, 2);
					if (psinit) {
						mf->ReadLine(psinit+2, len);
					   *(UINT16  *) psinit = (UINT16) len;
					   }
					}
				 if (!psinit) writeerror_w(L"Kirjoitinalustuksen lukeminen "
					L"tiedostosta POSTSCRI.INI ei onnistu", 0);
				delete mf;
				 pscr = 1;
	  case L'W' :                    // myˆs 'P' jatkuu
	  case L'G' :                    // myˆs 'P' jatkuu
				 utrlate['ƒ'] = 196;
				 utrlate['÷'] = 214;
				 utrlate['è'] = 197;
				 utrlate['‹'] = 220;
				 utrlate['ê'] = 201;
				 utrlate['‰'] = 228;
				 utrlate['ˆ'] = 246;
				 utrlate['Â'] = 229;
				 utrlate['Å'] = 252;
				 utrlate['Ç'] = 233;
				 utrlate['_'] = 32;
				 break;
	  }
	}

void lueKorRajat(void)
   {
   wchar_t rajaflnm[] = L"korostus.lst", buf[200], srjnm[200];
   int srj, rj1;
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
				  int d = getpos(rj1);
				  if (d > 0) {
					 vipLaji[d] = (wchar_t) rj2;
					 }
				  }
			   else if (!wcscmp(srjnm, L"VƒRI")) {
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

#ifdef _CONSOLE
static void nimio(INT clear)
   {

   if (clear) scpclr();
   scbox(1,12,9,67,15,0,7);
   viwrrectw(7,27,7,49,L"Ohjelma HkMaali, versio",7,0,0);
   vidspwmsg(7,51,7,0,VersioNo());
   vidspwmsg(8,42, 7, 0, L"Build");
   vidspwmsg(8, 48, 7, 0, Build());
   viwrrectw(3,15,3,63,L"HENKIL÷KOHTAISEN KILPAILUN TULOSPALVELUOHJELMISTO",
	  7,0,0);
   viwrrectw(5,34,5,45,L"Pekka Piril‰",7,0,0);
#ifdef DEMO
   vidspwmsg(8,24,7,0,L"DEMO korkeintaan 30 osanottajaa");   /* DEMO */
#endif
}
#endif

static int luevaihevalinta(void)
{
	TextFl *cfgfile;
	wchar_t line[80], *p, cfgflnm[] = L"vaihevalinta.cfg";
	int i_pv = 0;

	cfgfile = new TextFl(cfgflnm, L"rt");
	if (cfgfile->IsOpen()) {
		while (!cfgfile->Feof()) {
			cfgfile->ReadLine(line, 78);
			if ((p = wcstok(line, L"= \n")) != NULL) {
				if (wcscmpU(p, L"VAIHE") == 0 || wcscmpU(p, L"PƒIVƒ") == 0) {
					if ((p = wcstok(NULL, L"= \n")) != NULL) {
						i_pv = _wtoi(p) - 1;
						}
					break;
					}
				}
			}
		}
	delete cfgfile;
	return(i_pv);
}

void kirjvaihevalinta(void)
{
	TextFl *cfgfile;
	wchar_t line[80], cfgflnm[] = L"vaihevalinta.cfg";

	cfgfile = new TextFl(cfgflnm, L"wt");
	if (cfgfile->IsOpen()) {
		swprintf(line, L"VAIHE=%d\n", k_pv + 1);
		cfgfile->WriteLine(line);
		}
	delete cfgfile;
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
				swprintf(line, L"Konfiguraatiotiedostoa %s ei lˆydy",
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
			break;
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
	 if (!wcscmpU(ipparam[ny].destaddr, L"AUTO"))
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

 static void gettauluipparam(int ny, wchar_t *str)
	{
	wchar_t *p;

	if (ny < MAX_PORTTI) {
		comtype[ny] =  comtpUDPSTREAM;
		ipparam[ny].iptype = ipUDPBOTH;
		ipparam[ny].destport = 51702;
		p = str;
		if (wcswcind(*str, L":,/=") >= 0 && (p = wcstok(str+1, L":")) != NULL) {
			wcsncpy(ipparam[ny].destaddr, p, 63);
			if ((p = wcstok(NULL, L":")) != NULL && _wtoi(p) > 100)
				ipparam[ny].destport = _wtoi(p);
			}
		}
	}

#if defined(TCPLUKIJA) || defined(TCPSIIRTO) || defined(LAJUNEN)
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
	  if (pos1 > *pos && ((no = _wtoi(st+*pos)) > 0 && no <= MAX_LAHPORTTI))
         *pos = pos1;
      else
		 no = 0;
		}
   return(no);
   }

#ifdef SIIMPORT
static INT gateno(wchar_t *st, INT *pos)
   {
   INT no, pos1;

   if (st[*pos] == L'=' || !st[*pos]) no = 1;
   else {
      for (pos1 = *pos; st[pos1] >= L'0' && st[pos1] <= L'9'; pos1++);
	  if (pos1 > *pos && ((no = _wtoi(st+*pos)) > 0))
         *pos = pos1;
	  else
         no = 0;
      }
   return(no);
   }
#endif

wchar_t *aptr;

static void lue_parametrit(int argc, wchar_t* argv[], wchar_t *cfgflname)
	{
	wchar_t **fld, fldn[90];
	wchar_t *argptr[200];
	wchar_t *p;
	wchar_t msg[401];
	INT i, j, ny, nf,nc,ncl,pos;
	INT32 y, y1;

	aptr = (wchar_t *)argptr;

	if (cfgflname != NULL && *cfgflname) {
		wcsncpy(cfgfilename, cfgflname, 120);
		}
	memset(argptr, 0, sizeof(argptr));
	if (argc > 1) {
		wcsncpy(fldn, argv[1], 44);
		upcasewstr(fldn);
		if (!wmemcmpU(fldn, L"CFG=", 4))
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
			for (p = fld[ncl]; *p; p++) {
				switch (*p) {
					case 196 :
						*p = L'ƒ';
						break;
					case 228 :
					case 245 :
						*p = L'‰';
						break;
					case 214 :
					case 205 :
						*p = L'÷';
						break;
					case 246 :
					case 247 :
						*p = L'ˆ';
						break;
					case 197 :
						*p = L'è';
						break;
					case 229 :
					case 213 :
						*p = L'Â';
						break;
						}
					}
				wcsncpy(fldn, fld[ncl], 7);
				upcasewstr(fldn);
				if (!wmemcmpU(fldn, L"HTML", 4) || !wmemcmpU(fldn, L"LISTA=", 6)
					|| !wmemcmpU(fldn, L"SQL", 3)
					|| !wmemcmpU(fldn, L"LOKI=", 5) || !wmemcmpU(fldn, L"TARRA=", 6)
					|| !wmemcmpU(fldn, L"KOMENTO", 7) || !wmemcmpU(fldn, L"TAULUOT", 7)
					|| !wmemcmpU(fldn, L"SIRITAR", 7)
					|| !wmemcmpU(fldn, L"SIRITDE", 7) || !wmemcmpU(fldn, L"IKKUNAO", 7)) {
					if (wcslen(fld[ncl]) > 7)
						wcsncpy(fldn+7, fld[ncl]+7, 76);
					}
				else {
					wcsncpy(fldn, fld[ncl], 89);
					for (p = fldn; *p && *p != L' '; p++) ;
					if (*p == L' ')
						*p = 0;
					upcasewstr(fldn);
					}
				if(!wmemcmpU(fldn, L"CFG", 3)) {
					cfgparam = 1;
					continue;
					}
				if(!wmemcmpU(fldn, L"IKKUNAO", 7)) {
					p = wcsstr(fldn+6,L"=");
					if (p) {
						wcsncpy(ikkunaots, p+1, sizeof(ikkunaots)/2-1);
						}
					continue;
					}
				if(!wmemcmpU(fldn, L"IKKUNAT", 7)) {
					if (fldn[7] == L'=')
						wcsncpy(alkulayout, fldn+8, 120);
					else
						wcscpy(alkulayout, L"ikkunat.xml");
					continue;
					}
				if (!wmemcmpU(fldn, L"SALLISULJE", 10)) {
					poistaSulje = false;
					continue;
					}
				if (!wmemcmpU(fldn, L"KIRKAS", 5)) {
					norm_fore = 15;
					continue;
					}
			 if(!wmemcmpU(fldn, L"VƒRIT=", 6)) {
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
			 if(!wmemcmpU(fldn, L"RIVIT=25", 8)) {
				setySize = 25;
				continue;
				}
			 if(!wmemcmpU(fldn, L"RIVIT=50", 8)) {
				setySize = 50;
				continue;
				}
/*
			 if (!wmemcmpU(fldn, L"SAKOT",5)) {
					kilpparam.sakkoyks = 1;
				if (fldn[5] == L'=')
				   kilpparam.sakkoyks = _wtoi(fldn+6);
				continue;
				}
		 if(!wmemcmpU(fldn, L"NOLLAKƒRKI", 10)) {
			tulosmuot.nollakarki = 1;
			continue;
			}
		 if (!wmemcmpU(fldn, L"PƒIVIƒ=", 7)) {
			kilpparam.n_pv_akt = fldn[7] - L'0';
			continue;
			}
		 if (!wmemcmpU(fldn, L"VƒLIAIKOJA=", 11)) {
			kilpparam.valuku = fldn[11] - L'0';
			continue;
			}
*/
		 if (!wmemcmpU(fldn, L"PƒIVƒ=", 6) || !wmemcmpU(fldn, L"VAIHE=", 6)) {
			 if (fldn[6] == L'V')
				 vaihevalinta = 1;
			 else {
				 k_pv = fldn[6] - L'1';
				 if (k_pv < 0)
					 k_pv = 0;
				 }
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLDRIVER", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.drivername, p+1, sizeof(sqlparam.drivername)/2-1);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLHOST", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.hostname, p+1, sizeof(sqlparam.hostname)/2-1);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLDATABASE", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.database, p+1, sizeof(sqlparam.database)/2-1);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLUSER", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.User_Name, p+1, sizeof(sqlparam.User_Name)/2-1);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLPASSWORD", 7)) {
			p = wcsstr(fldn, L"=");
			if (p) {
				wcsncpy(sqlparam.Password, p+1, sizeof(sqlparam.Password)/2-1);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLSTART", 7)) {
			sqlparam.autostart = true;
			continue;
			}
		 if (!wmemcmpU(fldn, L"SQLTALLENNUS", 7)) {
			sqlparam.sqltallennus = true;
			continue;
			}
/*
		 if(!wmemcmpU(fldn, L"TAKAA-AJO", 9)) {
			if (!wmemcmpU(fldn+9, L"=E", 2))
					takaa_ajo = 0;
			else
			   takaa_ajo = 1;
			continue;
			}
*/
			 if(!wmemcmpU(fldn, L"JUOKSUILM", 9)) {
				ilmkentat = 1;
				continue;
				}
			 if(!wmemcmpU(fldn, L"HENKIL÷TIETOKANTA", 9)) {
				NimiHTHaku = 1;
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					if (!wcscmp(p+1, L"0") || !wcscmp(p+1, L"E") || !wcscmp(p+1, L"EI"))
						NimiHTHaku = 0;
					else {
						wcsncpy(HenkTKantaNm, p+1, sizeof(HenkTKantaNm)/2-1);
						}
					}
				continue;
				}
			 if(!wmemcmpU(fldn, L"SEURATIEDOSTO", 9)) {
				seurahaku = 2;
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					wcsncpy(seuraTiedNm, p+1, sizeof(seuraTiedNm)/2-1);
					}
				continue;
				}
			 if(!wmemcmpU(fldn, L"SEURATIETO", 9)) {
				if (wcsstr(fldn, L"=E") != NULL)
					seurahaku = 0;
				else if (wcsstr(fldn, L"=H") != NULL)
					seurahaku = 2;
				else
					seurahaku = 1;
				continue;
				}
			 if(!wmemcmpU(fldn, L"KUNTOSUUNN", 8)) {
				ilmkentat = 2;
				kuntosuunnmoodi = 1;
				autosarja = 1;
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					ilmkentat = 1+_wtoi(p+1);
					kuntosuunnmoodi = _wtoi(p+1);
					}
				continue;
				}
			 if(!wmemcmpU(fldn, L"KUNTOMAALI", 8)) {
				KuntoMaali = 1;
				continue;
				}
			 if(!wmemcmpU(fldn, L"KUNTOLƒHT÷", 8)) {
				KuntoMaali = -1;
				continue;
				}
			 if(!wmemcmpU(fldn, L"RASTIVATULOSTE", 9)) {
				emitTunnistus = 1;
				esiluenta = 2;
				emitfl = 1;
				kuntosuunnmoodi = 98;
				emitanal_fl = 1;
				lukutoisto = 1;
				continue;
				}
			 if(!wmemcmpU(fldn, L"VAKANTTILUKU=", 13)) {
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					minvakanttiluku = _wtoi(p+1);
					}
				continue;
				}
			 if(!wmemcmpU(fldn, L"VAKANTTIALKU=", 13)) {
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					vakanttialku = _wtoi(p+1);
					if (vakanttialku < minkilpno)
						vakanttialku = minkilpno;
					if (vakanttialku >= maxkilpno)
						vakanttialku = maxkilpno-1;
					}
				seurvapaa = vakanttialku;
				if (seurVapaaBadge == 1)
					seurVapaaBadge = vakanttialku;
				continue;
				}
			 if(!wmemcmpU(fldn, L"LISƒVAKANTTIALKU=", 7)) {
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					lisavakanttialku = _wtoi(p+1);
					seurvapaa = lisavakanttialku;
					}
				continue;
				}
			 if(!wmemcmpU(fldn, L"EMITALKU=", 9)) {
				if ((p = wcsstr(fldn, L"=")) != NULL) {
					seurVapaaBadge = _wtoi(p+1);
					}
				continue;
				}
			if(!wmemcmpU(fldn, L"POISSAMUKAAN", 9)) {
				kilpparam.poissamukaan = 4;
				continue;
				}
			if(!wmemcmpU(fldn, L"HYVHAVAITUT", 7)) {
				kilpparam.hyvHavaittu = true;
				continue;
				}
			 if(!wmemcmpU(fldn, L"VAHVISTAAINA", 10)) {
				if (wcsstr(fldn, L"=E") != NULL)
					vahvistaaina = false;
				else
					vahvistaaina = true;
				continue;
				}
			if(!wmemcmpU(fldn, L"LISƒƒEDELLE", 8)) {
				lisaa_edelle = 1;
				continue;
			}
		 if(!wmemcmpU(fldn, L"TARKNO=EI", 10) ||
			!wmemcmpU(fldn, L"TARKNO=0", 9) ) {
            tarknofl = 0;
			continue;
				}
		 if(!wmemcmpU(fldn, L"TARKNO=ON", 10) ||
			!wmemcmpU(fldn, L"TARKNO=1", 9) ) {
            tarknofl = 1;
            continue;
            }
		 if(!wmemcmpU(fldn, L"TARKNO=KA", 10) ||
			!wmemcmpU(fldn, L"TARKNO=2", 9) ) {
			tarknofl = 2;
			continue;
			}
		 if(!wmemcmpU(fldn, L"MUOTOILU", 5)) {
			muotoilufl = TRUE;
			if (fldn[8] == L'=') wcsncpy(luetfnm, fldn+9, 39);
			continue;
			}
/*
		 if(!wmemcmpU(fldn, L"LLMUOTOILU", 7)) {
			ll_muotoilufl = TRUE;
			if (fldn[10] == L'=') wcsncpy(ll_luetfnm, fldn+9, 39);
			continue;
			}
*/
		if(!wmemcmpU(fldn, L"VAADITARK", 8)) {
			vaaditarkastus = true;
			continue;
			}
		 if(!wmemcmpU(fldn, L"LƒHETƒCR=EI", 10)) {
			lahetacr = 0;
			continue;
			}
		 if(!wmemcmpU(fldn,L"KORKEAT",7 )) {
            korkeat = TRUE;
            if(!wmemcmpU(fldn+7,L"=STAR",5)) star = TRUE;
            continue;
			}
/*
		 if(!wmemcmpU(fldn, L"TARRA", 5)) {
			if (fldn[5] == L'=')
			   wcscpy(tarfname, &fldn[6]);
			else
			   wcscpy(tarfname, L"PRN");
			continue;
			}
		 if(!wmemcmpU(fldn, L"HPTARRA=", 8)) {
			int k;

			hptarra = 1;
			k = 1;
			rtarra = 1;
			if ((p = wcstok(fldn+8, L"/,")) != 0)
			   rtarra = _wtoi(p);
			if ((p = wcstok(NULL, L"/,")) != 0)
			   k = _wtoi(p);
			ntarra = k*rtarra;
			if ((p = wcstok(NULL, L"/,")) != 0)
			   ftarra = _wtoi(p);
			continue;
			}
*/
		 if(!wmemcmpU(fldn, L"LOKI", 4)) {
		 	loki = TRUE;
            if (wcslen(fldn) > 5) wcscpy(lokifname, &fldn[5]);
            continue;
            }
		 if (!wmemcmpU(fldn, L"LISTA", 5)) {
			if (fldn[5] == L'=') {
				wcsncpy(msg, fldn+6, 80);
				msg[79] = 0;
				upcasewstr(msg);
				elimwbl(msg);
				wcscpy(listfname, &fldn[6]);
				}
			if (!fldn[5] || !wcscmp(msg, L"OLETUS") || !wcscmp(msg, L"PRN"))
               wcscpy(listfname, L"PRN");
			continue;
			}
/*
		 if (!wmemcmpU(fldn, L"OTSIKOT", 3)) {
			tulosmuot.otsikot = 1;
			continue;
			}
*/
		 if( !wmemcmpU(fldn, L"MERKIT=", 7)) {
				merkit = fldn[7];
			if (wcswcind(L'K', fldn+8) >= 0) trloki = 1;
			if (wcswcind(L'T', fldn+8) >= 0) trtar = 1;
			trlst = !(trloki || trtar);
			if (wcswcind(L'L', fldn+8) >= 0) trlst = 1;
			continue;
			}
		 if (!wmemcmpU(fldn, L"DESEROTIN=", 10)) {
			wdec_sep = fldn[10];
			dec_sep = wchartoansi(wdec_sep);
			continue;
			}
		 if(!wmemcmpU(fldn, L"ESTƒMUUTOS", 8)) {
			esta_tall = 1;
			continue;
				}
#ifdef AUTOFILE
		 if (!wmemcmpU(fldn, L"HTML=", 5) || !wmemcmpU(fldn, L"MHTML=", 6) || !wmemcmpU(fldn, L"XML=", 4)) {
			wchar_t *p1;
			p = wcstok(fldn, L"=");
			aftulparam.kohde = *p;
			if ((p = wcstok(NULL, L",/")) == NULL)
				continue;
			for (p1 = p; isdigit(*p1); p1++);
			if (p1 > p && *p1 == 0) {
				aftulparam.yksihtml = 0;
				autofileparam.afvali = SEK * _wtoi(p);
				p = wcstok(NULL, L",/");
				}
			else {
				wcscpy(autofileparam.afname, p);
				if ((p = wcstok(NULL, L",/")) == NULL || (ny = _wtoi(p)) == 0) {
				   if (!autofileparam.afvali) autofileparam.afvali = (int) (60 * SEK);
				   }
				else
				   autofileparam.afvali = (int) (ny * SEK);
				}
			if (p && *p != L'M' && (*p == L'S' || ((p = wcstok(NULL, L",/")) != NULL) && *p == L'S')) {
			   aftulparam.yksihtml = 0;
			   autofileparam.vainMuutt = true;
			   }
			if (p && (*p == L'M' || ((p = wcstok(NULL, L",/")) != NULL) && *p == L'M')) {
			   aftulparam.vaihelkm = 2;
			   }
			continue;
			}
		 if( !wmemcmpU(fldn, L"HTMLTIT", 7)) {
			p = wcsstr(fldn, L"=");
			if (p && p[1])
				wcscpy(autofileparam.wtitlea, p+1);
			continue;
			}
		 if( !wmemcmpU(fldn, L"HTMLOTS", 7)) {
			p = wcsstr(fldn, L"=");
			if (p && p[1])
				wcscpy(autofileparam.wheadera, p+1);
			continue;
			}
		 if( !wmemcmpU(fldn, L"KOMENTO", 7)) {
			p = wcsstr(fldn, L"=");
			if (p && p[1]) {
				wcsncpy(autofileparam.autokomento, p+1, sizeof(autofileparam.autokomento)/2-1);
				autofileparam.autokomentolaji = 1;
				}
			continue;
			}
		 if( !wmemcmpU(fldn, L"AUTOJAKELU", 7)) {
			p = wcsstr(fldn, L"=");
			if (p && p[1]) {
				wcsncpy(autofileparam.jakelumaar, p+1, sizeof(autofileparam.jakelumaar)/2-1);
				}
			autofileparam.autokomentolaji = 2;
			continue;
			}
#endif
         if (!wmemcmpU(fldn, L"TCPTULOS=", 9)) {
            tcptulos = fldn[9];
            continue;
            }			
#ifdef TCPSIIRTO
		 if (!wmemcmpU(fldn, L"FILETCP", 7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			   fileyht = ny-1+MAX_LAHPORTTI+2*NREGNLY;
			if (fileyht < MAX_PORTTI) {
				gettcpipparam(fileyht, fldn+pos, 0);
				if (!autofileparam.afname[0]) {
				   aftulparam.kohde = L'X';
				   wcscpy(autofileparam.afname, L"__TMP__.XML");
				   autofileparam.afvali = 60*SEK;
				   }
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"XMLSARJA=", 9)) {
			xmlsarja = haesarja_w(fldn+9, false);
            continue;
			}
#endif
		 if( !wmemcmpU(fldn, L"BOOT", 4)) {
			bootfl = 1;
			if (!wmemcmpU(fldn + 4, L"=M", 2)) {
				bootfl = 8;
				if (fldn[6] > L'0' && fldn[6] <= L'0' + MAXJONO)
					bootfl |= 16 * (fldn[6] - L'1');
				}
			continue;
			}
		 if( !wmemcmpU(fldn, L"AUTO", 4) && (!fldn[4] || fldn[4] == L'=')) {
			   wcsncpy(autostr, fldn+4, sizeof(autostr)/2-2);
				autostr[sizeof(autostr)/2-1] = L'1';
			continue;
			}
		 if( !wmemcmpU(fldn, L"PUSKUROI",8)) {
				immed = 0;
            continue;
            }
		 if( !wmemcmpU(fldn, L"YHTEISLƒHT÷",7)) {
			yhtlahto = 1;
            continue;
            }
         if( !wmemcmpU(fldn, L"KONE=",5)) {
			wcstoansi((char *) konetunn, fldn+5, 3);
            continue;
			}
		 if( !wmemcmpU(fldn, L"SOTILAS", 7)) {
			sotilas = 1;
			continue;
			}

#if defined(SIIMPORT)
		 if ((!wmemcmpU(fldn, L"SIIMPORT", 8)) && (pos = 8) != 0 &&
            (ny = yhteys_no(fldn, &pos)) != 0) {
			gettcpipparam(ny-1, fldn+pos, 0);
			siimport = ny-1;
            comdir[ny-1] = L'I';
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"HYVKAIKKI",9)) {
			if ((ny = _wtoi(fldn+9)) > 0)
				UDPvarmistamaton[ny-1] |= 2;
			continue;
			}
		 if (!wmemcmpU(fldn, L"MONITORVƒLI=",12)) {
			monitoriVali = 1000 * _wtoi(fldn+12);
			continue;
			}
		 if (!wmemcmpU(fldn, L"MONITORI",8)) {
			addMonitori(fldn+8);
			continue;
			}
		 if (((!wmemcmpU(fldn, L"COM",3) && (pos = 3) != 0) ||
			(!wmemcmpU(fldn, L"YHTEYS",6) && (pos = 6) != 0)) &&
			fldn[pos] <= L'=' && (ny = yhteys_no(fldn, &pos)) != 0) {
			if (!wmemcmpU(fldn+pos+1, L"UDP", 3)) {
			   getipparam(ny-1, fldn+pos+4);
			   continue;
			   }
			if (!wmemcmpU(fldn+pos+1, L"BRO", 3)) {
			   UDPvarmistamaton[ny-1] |= 1;
			   getipparam(ny-1, fldn+pos+4);
//			   getipparam(MAX_PORTTI, fldn+pos+4);
			   continue;
			   }
			if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
			   j = 0;
			   if (fldn[pos+4] == L'X' || wmemcmpU(fldn+pos+4, L"SX", 2) == 0) {
				  pos++;
				  if (wmemcmpU(fldn + pos + 3, L"XS", 2) == 0 || wmemcmpU(fldn + pos + 3, L"SX", 2) == 0) {
					  j = 2;
					  pos++;
					  }
				  else
					  j = -1;
				  comdir[ny-1] = L'O';
				  aftulparam.kohde = L'X';
				  if (!autofileparam.sanafname[0])
					  wcscpy(autofileparam.sanafname, L"__TMP_SAN__.XML");
				  }
			   if (fldn[pos+4] == L'L') {
				  pos++;
				  comdir[ny-1] = L'O';
				  j = -2;
				  loki = 2;
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
			 if (!wmemcmpU(fldn, L"TCPAUTOALKU",10)) {
				tcpautoalku = 1;
				continue;
				}
			if (!wmemcmpU(fldn+pos+1, L"COM", 3)) pos += 3;
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
		if (!wmemcmpU(fldn, L"PORTBASE=",9)) {
			portbase = _wtoi(fldn+9);
            continue;
            }
		if (!wmemcmpU(fldn, L"MAXYHTEYS=",10)) {
			maxyhteys = _wtoi(&fldn[10]);
			if (maxyhteys > MAX_LAHPORTTI)
			   maxyhteys = MAX_LAHPORTTI;
			continue;
			}
		 if (!wmemcmpU(fldn, L"VƒLITƒ",6) && (pos = 6) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
				if (pos == 6) {
			   for (y = 0; y < MAX_LAHPORTTI; y++)
				  estavalitys[y] = 1;
			   }
            else
			   estavalitys[ny-1] = 1;
			continue;
			}
#if defined(SIIMPORT)
         if (!wmemcmpU(fldn, L"GATE",4) && (pos = 4) != 0 &&
			(ny = gateno(fldn, &pos)) != 0) {
				addgatepiste(ny, atoi(fldn+pos+1));
				continue;
				}
#endif
#ifdef SERVER
         if (!wmemcmpU(fldn, L"VARASERVER",10)) {
			for (ny = 0; ny < MAX_LAHPORTTI; ny++) {
               varaserver = 1;
               }
            continue;
            }
#endif
         if (!wmemcmpU(fldn, L"SULKUSALASANA=",14)) {
            wcsncpy(sulkusalasana, fldn+14, 10);
            continue;
            }
		 if (!wmemcmpU(fldn, L"NAKVIIVE=",9)) {
			nakviive = _wtoi(fldn+9);
				continue;
			}
         if (!wmemcmpU(fldn, L"UDPJARRU=",9)) {
				udpjarru = _wtoi(fldn+9);
				continue;
				}
         if (!wmemcmpU(fldn, L"UDPVIIVEET=",11)) {
			p = wcstok(fldn+11, L"/;,\n");
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
		 if (!wmemcmpU(fldn, L"TCPVIIVEET",10) && (pos = 10) != 0 &&
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
		 if( !wmemcmpU(fldn, L"BAUD",4) && (pos = 4) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
				wcscpy(baudst[ny-1], &fldn[pos+1]);
            continue;
			}
         if( !wmemcmpU(fldn, L"COMFILE",7)) {
				levypusk = 1;
            if (fldn[7] && (fldn[8] == L'0' || fldn[8] == L'E')) levypusk = 0;
            if (fldn[7] && fldn[8] == L'S') levypusk = 2;
			continue;
			}
#if defined(TV1) || defined(TXTTV)
		 if( !wmemcmpU(fldn, L"TV_COM=",7)) {
            if (fldn[7] == L'T') {
               tv_test = 1;
               }
				else {

               tv_com = _wtoi(fldn+7);
#if defined(TXTTV)
               if (fldn[8] == L'S') {
				  suoracom = 1;
                  }
#endif
               }
            continue;
            }
         if( !wmemcmpU(fldn, L"TV_BAUD=",8)) {
            y = _wtoi(fldn+8);
			tv_baud = 7;
            y1 = 9600;
            while (y1 > y && tv_baud > 2) {
			   tv_baud--;
               y1 = y1/2;
               }
            continue;
			}
#endif
		 if(!wmemcmpU(fldn, L"VAIKAVERT=", 10)) {
			vaikavert = _wtoi(fldn+10);
			continue;
			}
		 if(!wmemcmpU(fldn, L"ILMTUNT=EI", 9)) {
			ilmtunt = 0;
			continue;
			}
		 if(!wmemcmpU(fldn, L"ILMTUNT", 8)) {
			ilmtunt = 2;
			continue;
			}
		 if(!wmemcmpU(fldn, L"VA-AJAT", 7) || !wmemcmpU(fldn, L"YHTEYSAJAT", 10)) {
			vaajat = 1;
			ajanottofl = 1;
			if (ilmtunt == 1)
				ilmtunt = 0;
			continue;
			}
		if( !wmemcmpU(fldn, L"ƒƒNI=",5)) {
			aani = fldn[5] - L'0';
			continue;
			}
		 if( !wmemcmpU(fldn, L"LISƒƒ=",6)) {
            n_lisaa = _wtoi(fldn+6);
            continue;
			}
		 if (!wmemcmpU(fldn, L"KUNTO",5) || !wmemcmpU(fldn, L"SUORASY÷TT÷",5)) {
            if (!wmemcmpU(fldn, L"KUNTO",5))
               kunto = 2;
			else
			   kunto = 1;
            continue;
            }
		 if (!wmemcmpU(fldn, L"RTNM",4) && (pos = 4) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			regnly[ny-1] = LID_RTNM;
			if ((p = wcstok(fldn, L"=")) != NULL) {
				p = fldn+wcslen(p);
				if (!wmemcmpU(p+1, L"UDP", 3))
					gettauluipparam(cn_regnly[ny-1], p+4);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"CPOWER",6) && (pos = 6) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
//			cpower = 1;
			regnly[ny-1] = 23;
			if ((p = wcstok(fldn, L"=")) != NULL) {
				p = fldn+wcslen(p);
				if (!wmemcmpU(p+1, L"TCP", 3))
					gettcpipparam(cn_regnly[ny-1], p+4, 0);
				else if (p[1] >= L'0' && p[1] <= L'9' &&(p = wcstok(NULL,L":,-/")) != NULL)
					port_regnly[ny-1] = _wtoi(p);
				}
            continue;
            }
#if defined(TAULU) || defined(TAULU_2)
		 if (!wmemcmpU(fldn, L"TAULU_COM=",10)) {
			if (fldn[10] == L'U') {
				gettauluipparam(fldn+13);
				udptaulu = 1;
				}
			else {
				taulu_com = _wtoi(fldn+10);
				}
			continue;
			}
		 if (!wmemcmpU(fldn, L"TAULU_COM=",10)) {
			taulu_com = _wtoi(fldn+10);
			continue;
			}
		 if( !wmemcmpU(fldn, L"TAULU_BAUD=",11)) {
				y = _wtoi(fldn+11);
            taulu_baud = 7;
            y1 = 9600;
			while (y1 > y && taulu_baud > 2) {
			   taulu_baud--;
               y1 = y1/2;
               }
			continue;
            }
         if( !wmemcmpU(fldn, L"TAULUVIIVE=",11)) {
				tauluviive = _wtoi(&fldn[11]);
			continue;
            }
#endif
#ifdef GAZ
		 if( !wmemcmpU(fldn, L"GAZ=",4)) {
            gaz_com = fldn[4] - L'0';
			continue;
            }
		 if( !wmemcmpU(fldn, L"GAZVAIHE=",9)) {
			gazvaihe = _wtoi(&fldn[9]);
            continue;
			}
			if (!wmemcmpU(fldn, L"GAZRIVI",7) && wcslen(fldn) > 8) {
				y = fldn[7] - L'0';
				gazpiste[y-1] = fldn[9] == L'M' ? 0 : fldn[9] - L'0';
				if (fldn[10] > 0 && fldn[11] > L'0')
					gazrata[y-1] = fldn[11] - L'1';
				if (y > gazluku)
					gazluku = y;
				continue;
				}
#endif
		 if(!wmemcmpU(fldn, L"TESTAUS", 7)) {
			testi = 1;
			hav_virheet = true;
			continue;
			}
		 if (!wmemcmpU(fldn, L"COMTESTI", 8)) {
			 testi = 2;
			 continue;
			}
#ifdef DBGFILE
		 if(!wmemcmpU(fldn, L"DBGTOFILE", 7)) {
				dbgtofile = TRUE;
            continue;
            }
#endif
#ifdef MAALI
		 if(!wmemcmpU(fldn, L"AJAT", 4)) {
			ajanottofl = 1;
			if (fldn[wcslen(fldn)-2] == L',' ||
			   fldn[wcslen(fldn)-2] == L'/') {
				  if (fldn[wcslen(fldn)-1] == L'S') sail_ajat = 1;
				  fldn[wcslen(fldn)-2] = 0;
						}
			if (wcslen(fldn) > 5) wcscpy(aikaflname[0], fldn+5);
			continue;
			}
#ifdef AIKAVERT
		  if(!wmemcmpU(fldn, L"VERTRAJAT=", 10)) {
				asetavertajat(fldn+10);
			continue;
			}
#endif
		 if(!wmemcmpU(fldn, L"PAKOTALAIKA", 11)) {
				if (fldn[11] == L'=' && (fldn[12] == L'0' || fldn[12] == L'E'))
					pakotalaika = 0;
				else
					pakotalaika = 1;
				continue;
				}
//		 if(!wmemcmpU(fldn, L"YHTEYSJONOT", 10)) {
//			 yhtjonot = 1;
//			 continue;
//			 }
		 if(!wmemcmpU(fldn, L"LAJAT", 5)) {
			ajanottofl = 1;
			lajatfl = 1;
			if (fldn[wcslen(fldn)-2] == L',' ||
               fldn[wcslen(fldn)-2] == L'/') {
                  if (fldn[wcslen(fldn)-1] == L'S') sail_ajat = 1;
                  fldn[wcslen(fldn)-2] = 0;
                  }
			if (wcslen(fldn) > 6) wcscpy(aikaflname[1], fldn+6);
            continue;
            }
         if( !wmemcmpU(fldn, L"LƒHT÷RYHMƒ=",11)) {
			kilpparam.lahtoryhma = _wtoi(fldn+11);
            continue;
			}
		 if (!wmemcmpU(fldn, L"TAUSTA-AIKA", 8)) {
			 taustaaika = 1;
			 continue;
			 }
		 if( !wmemcmpU(fldn, L"AIKAKORJAUS",11)) {
			pos = 11;
			if ((ny = yhteys_no(fldn, &pos)) > 0 && ny <= MAXJONO) {
				aikakorjaus[ny-1] = _wtoi(fldn+pos+1) * SEK / 100;
				}
			continue;
			}
#ifdef _CONSOLE
		 if(!wmemcmpU(fldn, L"NƒPPƒIN=", 8)) {
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
		 if( !wmemcmpU(fldn, L"LƒHKELLO",8) && (pos = 8) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			lahkello[ny-1] = 600000;
			lahkelloraja[ny-1] = 1000;
			if (fldn[pos] == L'=') {
				if (fldn[pos+1] == L'V')
					lahkellovain[ny-1] = true;
				else
					lahkello[ny-1] = 1000 * _wtoi(fldn+pos+1);
				}
			if ((p = wcsstr(fldn+pos+1, L"/")) != NULL) {
				if (fldn[pos+1] == L'V')
					lahkellovain[ny-1] = true;
				else
					lahkelloraja[ny-1] = _wtoi(p+1);
				}
			if ((p = wcsstr(fldn+pos+1, L"/")) != NULL)
				if (fldn[pos+1] == L'V')
					lahkellovain[ny-1] = true;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LƒHAIKA",7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			ajanottofl = 1;
			com_ajat[ny-1] = 1;
			if (fldn[pos+1] == L'V')
				com_ajat[ny-1] = 2;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LƒHECODE",8) && (pos = 8) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			lahec[ny-1] = 1;
			if (fldn[pos+1] == L'V')
				lahec[ny-1] = 2;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LƒHTIEDOSTOT",8)) {
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
		 if( !wmemcmpU(fldn, L"SY÷TT÷", 6)) {
			ajanottofl = 1;
				syotto = 1;
			continue;
				}
#ifdef ENNAKOI
         if( !wmemcmpU(fldn, L"ENNAKOI",7)) {
			ennakoi = 1;
            continue;
            }
#endif
		 if (!wmemcmpU(fldn, L"AIKAERO", 7)) {
			if (fldn[7] == L'=')
				aikaero[NREGNLY] = SEK * _wtoi(fldn+8);
			else {
				pos = 7;
				if ((ny = yhteys_no(fldn, &pos)) != 0 && ny <= NREGNLY)
					aikaero[ny-1] = SEK * _wtoi(fldn+pos+1);
				}
			continue;
			}
#ifndef NOCOM_AIKA
		 if( !wmemcmpU(fldn, L"AIKA_COM=",9)) {
			aika_com = _wtoi(&fldn[9]);
			ajanottofl = 1;
			com_aika = 1;
			continue;
			}
		 if( !wmemcmpU(fldn, L"AIKA_ESTO=",10)) {
			aika_esto = _wtoi(&fldn[10]);
			continue;
			}
#endif
#ifndef NOREGNLY
		 if (!wmemcmpU(fldn, L"KELLO_ESTO=",11)) {
			kello_esto = _wtoi(&fldn[11]);
			continue;
			}
		if (!wmemcmpU(fldn, L"ESTƒNEG",7)) {
			estaneg = 1;
			continue;
			}
		if (!wmemcmpU(fldn, L"ESTƒHAAMUT",7)) {
			estaneg = 1;
			if (fldn[10] == L'=') {
				estaneg = MINUUTTI * _wtoi(fldn+11);
				}
			continue;
			}
         if (!wmemcmpU(fldn, L"ESTƒYLIM",8)) {
            estaylim = 1; 
			if (fldn[8] == L'=') {
				ylimraja = SEK * _wtoi(fldn+9);
				if (fldn[wcslen(fldn)-1] == L'K')
					estaylim = 2;
				}
            continue;
            }
		  if( !wmemcmpU(fldn, L"REGNLYVIIVE=",12)) {
			  pyyntoviive = _wtoi(fldn+12);
			  continue;
			  }
		  if ((!wmemcmpU(fldn, L"REGNLYNO",8) && (pos = 8) != 0)
			  || (!wmemcmpU(fldn, L"KELLONO",7) && (pos = 7) != 0)
			  || (!wmemcmpU(fldn, L"COMETNO",7) && (pos = 7) != 0)
			  || (!wmemcmpU(fldn, L"TIMYNO",6) && (pos = 6) != 0)
			  || (!wmemcmpU(fldn, L"TDCNO",5) && (pos = 5) != 0)
			  || (!wmemcmpU(fldn, L"ASCNO",5) && (pos = 5) != 0)) {
			  if (fldn[pos] < L'0' || fldn[pos] > L'9') {
				   if (!wmemcmpU(fldn+pos, L"=EI",3)) {
					  regnly_no[0] = 0;
					  }
				   else
					  regnly_no[0] = 1;
				   for (int i = 1; i < NREGNLY; i++)
					   regnly_no[i] = regnly_no[0];
				  }
			  ny = yhteys_no(fldn, &pos);
			  if (ny > 0 && ny <= NREGNLY) {
				   if (!wmemcmpU(fldn+pos, L"=EI",3)) {
					  regnly_no[ny-1] = 0;
					  }
				   else
					  regnly_no[ny-1] = 1;
				   continue;
				   }
               }
		  if (!wmemcmpU(fldn, L"REGNLY",5)) {
			   ajanottofl = 1;
               regnly[0] = LID_RTR;
               if (fldn[6] == L'2') regnly[0] = LID_RTR2;
               port_regnly[0] = 1;
               if ((p = wcstok(fldn, L"=")) != NULL) {
						if ((p = wcstok(NULL,L":,-/")) != NULL) {
                     port_regnly[0] = _wtoi(p);
							if ((p = wcstok(NULL,L":,-/")) != NULL) {
						od_regnly = _wtoi(p)*2;
						if (od_regnly < 4) od_regnly = 10;
                        if ((p = wcstok(NULL,L":,-/")) != NULL) {
						   maxero = _wtol(p) * 10;
						   if (maxero < 20) maxero = 600;
                           }
						}
					}
                  }
            continue;
            }
#endif
#ifdef MAKI
         if( !wmemcmpU(fldn, L"YHDISTETTY",9)) {
				yhdistetty = 1;
            continue;
            }
         if( !wmemcmpU(fldn, L"AUTOSELOS",9)) {
            autoselostaja = 1;
            continue;
				}
			if( !wmemcmpU(fldn, L"TUOMARI",7)) {
	         if( !wmemcmpU(fldn, L"TUOMARILUKU=",12)) {
					tuomariluku = fldn[12]-L'0';
					continue;
					}
            ny = NREGNLY-1;
            if (fldn[7] > L'0' && fldn[7] <= L'0'+NREGNLY)
			   ny = fldn[7] - L'1';
			regnly[ny] = LID_TUOMARI;
			port_regnly[ny] = 1;
			tuomari_yht = ny;
            if ((p = strstr(fldn, L"=")) != NULL) {
               port_regnly[ny] = _wtoi(p+1);
					}
            continue;
			}
         if( !wmemcmpU(fldn, L"YHTEISPIS",9)) {
			yhteispisteet = 1;
			continue;
			}
	      if( !wmemcmpU(fldn, L"NOPEUSCOMET=",12)) {
				nopeuscomet = fldn[12]-L'1';
				continue;
				}
	      if( !wmemcmpU(fldn, L"PITUUSCOMET=",12)) {
				pituuscomet = fldn[12]-L'1';
				continue;
				}
		  if( !wmemcmpU(fldn, L"COMETLUKU=",10)) {
				cometluku = fldn[10]-L'0';
				continue;
				}
		  if( !wmemcmpU(fldn, L"KPISTE=",7)) {
				kpiste = _wtoi(fldn+7);
				continue;
				}
	      if( !wmemcmpU(fldn, L"PTSPERMETRI=",12)) {
				pituuskerroin = _wtoi(fldn+12);
				continue;
				}
	      if( !wmemcmpU(fldn, L"PTSPERMIN=",10)) {
				ptspermin = _wtoi(fldn+10);
				continue;
				}
		  if( !wmemcmpU(fldn, L"TAULUOTS=",9)) {
				wcsncpy(makiotsikko, fldn+9, TAULU_RIVPIT);
				continue;
				}
#endif
		 if (!wmemcmpU(fldn, L"SALLITOISTO",10)) {
				p = wcsstr(fldn, L"=");
				if (p && (p[1] == L'E' || p[1] == L'0'))
					ohitatoisto = 1;
				else
					ohitatoisto = 0;
			continue;
			}
#if defined(LAJUNEN)
		 if (!wmemcmpU(fldn, L"SIRITLOKI",9)) {
			siritloki = true;
			continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITSYNC",8)) {
			siritsync = true;
			continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITREUNA=",11)) {
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
		 if (!wmemcmpU(fldn, L"SIRITARRIVE=",7)) {
			p = wcsstr(fldn, L"=");
			if (p)
				wcstoansi(siritarrive, p+1, sizeof(siritarrive)-1);
			continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITDEPART=",7)) {
			p = wcsstr(fldn, L"=");
			if (p)
				 wcstoansi(siritdepart, p+1, sizeof(siritdepart)-1);
			continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITEVENTPORT=", 14)) {
			int rno;
			 if ((rno = _wtoi(fldn+14)-1) < 0 || rno >= NREGNLY)
				rno = 0;
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				SiritEventPort[rno] = _wtoi(p+1);
			 continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITCMDPORT=", 12)) {
			int rno;
			 if ((rno = _wtoi(fldn+12)-1) < 0 || rno >= NREGNLY)
				rno = 0;
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				SiritCmdPort[rno] = _wtoi(p+1);
			 continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITPOLL=", 9)) {
			int rno;
			 if ((rno = _wtoi(fldn+9)-1) < 0 || rno >= NREGNLY)
				rno = 0;
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				 SiritPoll[rno] = 1000 * _wtoi(p+1);
			 continue;
			}
		 if (!wmemcmpU(fldn, L"SIRITMASK=", 10)) {
			 wcstoansi(SiritMask, fldn+10, 30);
			 continue;
			}
#endif
#ifdef ALGE
#if defined(EMITLEIMAT) || defined(COMETNO)
		 if (!wmemcmpU(fldn, L"TUNNKOODIT",10)) {
				if (fldn[10] == L'=' && fldn[11] == L'E')
					emitfl = 0;
				else
					emitfl = 1;
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"ALGE",4)
			  || !wmemcmpU(fldn, L"COMET",5)
			  || !wmemcmpU(fldn, L"RADIOEMIT",8)
			  || !wmemcmpU(fldn, L"TIMY",4)
			  || !wmemcmpU(fldn, L"FEIG",4)
			  || !wmemcmpU(fldn, L"SIRIT",5)
			  || !wmemcmpU(fldn, L"IMPINJ",6)
			  || !wmemcmpU(fldn, L"ARES",4)
			  || !wmemcmpU(fldn, L"SW2000",6)
			  || !wmemcmpU(fldn, L"ASC",3)) {
			 int rno = 0;
			if( !wmemcmpU(fldn, L"COMET",5))
			   {
			   regnly[0] = LID_COMET;
			   }
			else if (!wmemcmpU(fldn, L"TIMY",4)) {
				if ((rno = _wtoi(fldn+4)-1) < 0 || rno >= NREGNLY)
					rno = 0;
			   regnly[rno] = LID_TIMY;
			   }
			else if (!wmemcmpU(fldn, L"ARES",4)) {
				if ((rno = _wtoi(fldn+4)-1) < 0 || rno >= NREGNLY)
					rno = 0;
			   regnly[rno] = LID_ARES;
			   }
			else if (!wmemcmpU(fldn, L"ASC",3)) {
			   regnly[0] = LID_ASC;
			   }
			else if (!wmemcmpU(fldn, L"TDC",3)) {
			   regnly_no[0] = 1;
			   regnly[0] = LID_TDC;
			   }
			else if (!wmemcmpU(fldn, L"ALGE4",5) || !wmemcmpU(fldn, L"S4",2)) {
			   regnly[0] = LID_ALGE4;
			   }
			else if (!wmemcmpU(fldn, L"ALGE=",5) || !wmemcmpU(fldn, L"S3",2)) {
			   regnly[0] = LID_ALGE;
			   }
			else if (!wmemcmpU(fldn, L"RADIOEMIT",8)) {
			   regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = LID_RADIOEMIT;
			   }
#ifdef LAJUNEN
			else if (!wmemcmpU(fldn, L"FEIG",4)) {
			   regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = LID_FEIG;
			   }
			else if (!wmemcmpU(fldn, L"SIRIT",5)) {
			   if ((rno = _wtoi(fldn+5)-1) < 0 || rno >= NREGNLY)
					rno = 0;
			   regnly_no[rno] = 1;
			   emitfl = 1;
			   regnly[rno] = LID_SIRIT;
			   kaikki_ajat[rno+1] = 2;
			   }
			else if (!wmemcmpU(fldn, L"IMPINJ",6)) {
			   regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = LID_IMPINJ;
			   kaikki_ajat[1] = 2;
			   }
#endif
#ifdef TUTKA
			else if (!wmemcmpU(fldn, L"ALGET",5)) {
			   regnly_no[0] = 1;
			   emitfl = 1;
			   regnly[0] = LID_ALGETUTKA;
			   }
#endif
#ifdef SWIM2000
			else if (!wmemcmpU(fldn, L"SW2000",6)) {
//			   regnly_no[0] = 0;
			   regnly[0] = LID_SW2000;
			   }
#endif
			if (regnly[rno]) {
				port_regnly[rno] = 1;
				ajanottofl = 1;
				if ((p = wcstok(fldn, L"=")) != NULL) {
					p = fldn+wcslen(p);
					if (!wmemcmpU(p+1, L"TCP", 3)) {
						 gettcpipparam(MAX_LAHPORTTI+rno, p+4, 0);
						  continue;
						  }
					if (p[1] >= L'0' && p[1] <= L'9' &&(p = wcstok(NULL,L":,-/")) != NULL) {
						port_regnly[rno] = _wtoi(p);
						if ((p = wcstok(NULL,L":,-/")) != NULL) {
							maxero = _wtol(p) * 10L;
							if (maxero == 0) maxero = 2000000L;
							if (maxero < 20) maxero = 600;
							}
						}
					}
				continue;
				}
			}
		 if( !wmemcmpU(fldn, L"KELLOBAUD=",10)) {
			y = _wtol(fldn+10);
			kello_baud = 9;
			y1 = 38400L;
			while (y1 > y && kello_baud > 2) {
			   kello_baud--;
			   y1 = y1/2;
			   }
			continue;
			}
		 if( !wmemcmpU(fldn, L"KELLOBITS=",10)) {
			y = _wtol(fldn+10);
			kello_bits = y;
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"LƒHT÷IKKUNA=", 12)) {
			 tunn_raja = _wtoi(fldn + 12);
			 continue;
		 }
		 if (!wmemcmpU(fldn, L"KUOLLUTAIKA=", 12)) {
			va_kuollut = _wtoi(fldn+12) * SEK;
            continue;
				}
         if( !wmemcmpU(fldn, L"UUSINAIKA",6)) {
            uusinaika = 120*SEK;
			if ((p = wcsstr(fldn, L"=")) != NULL)
				uusinaika = _wtoi(p+1)*SEK;
			continue;
			}
         if( !wmemcmpU(fldn, L"MUUTOSKYSELY=", 13)) {
            if (fldn[13] == L'0' || fldn[13] == L'E')
				muutoskysely = 0;
            continue;
			}
         if( !wmemcmpU(fldn, L"VAVUOROT",5)) {
			va_vuoro = 1;
			if (fldn[8] == L'=') {
				p = wcstok(fldn+9, L"/");
				if (p) {
					samapisteraja = _wtoi(p) * SEK;
					if ((p = wcstok(NULL, L"/")) != NULL) {
						vavuoro_ala = _wtoi(p);
						if (vavuoro_ala < 1)
							vavuoro_ala = 1;
						if ((p = wcstok(NULL, L"/")) != NULL)
							vavuoro_yla = _wtoi(p);
						}
					}
				}
			continue;
            }
		 if( !wmemcmpU(fldn, L"JONOT",5) &&
			(fldn[5] == L'=' || (fldn[5] >= L'0' && fldn[5] <= L'9'))) {
			int na;
			pos = 5;
			ny = yhteys_no(fldn, &pos) - 1;
			if (ny > NREGNLY-1 || ny < 0)
				ny = NREGNLY-1;
			if (pos == 5) {
				na = 0;
				ny = NREGNLY-1;
				}
			else
				na = ny;
			for (p = fldn + pos + 1, y = 0; *p && y < 16; p++, y++) {
			   if (*p > L'0' && *p <= L'0'+MAXJONO) {
				  for (int r = na; r <= ny; r++)
					  aika_jono[r][y] = *p - L'1';
				  if (aika_jono[na][y] > maxjono)
					 maxjono = aika_jono[na][y];
				  }
			   }
			continue;
			}
		 if (!wmemcmpU(fldn, L"JONOPISTE",9)) {
			 pos = 9;
			 ny = yhteys_no(fldn, &pos);
			 if (ny > 0 && ny <= MAXJONO && fldn[pos] == L'=') {
				jonopiste[ny-1] = towupper(fldn[11]);
				continue;
				}
			}
		 if( !wmemcmpU(fldn, L"LƒHDEPISTEET",8)) {
			lahdepistehaku = 1;
			continue;
			}
		 if (!wmemcmpU(fldn, L"EMITJONO=", 9)) {
			 emitjono = fldn[9]-L'1';
			 if (maxjono < emitjono)
				 maxjono = emitjono;
			 continue;
			}
		 if (!wmemcmpU(fldn, L"MAXJONO=", 8)) {
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				maxjono = _wtoi(p+1)-1;
			 if (maxjono >= MAXJONO)
				 maxjono = MAXJONO-1;
			 continue;
			}
		 if (!wmemcmpU(fldn, L"TUNNJONO", 8)) {
			 tunnjono = 1;
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				tunnjono = p[1]-L'1';
			 if (maxjono < tunnjono)
				 maxjono = tunnjono;
			 continue;
			}
         if( !wmemcmpU(fldn, L"KAKSITUNN",9)) {
			if (kilpparam.kaksibadge == 0)
				kilpparam.kaksibadge = 1;
            continue;
			}
		 if (!wmemcmpU(fldn, L"TUNNRAJAT=", 10)) {
			 tunnrajat[0] = _wtoi(fldn+10);
			 for (p = fldn+10; *p >= L'0' && *p <= L'9'; p++) ;
			 if (*p && p[1])
				 tunnrajat[1] = _wtoi(p+1);
			 continue;
			}
		 if( !wmemcmpU(fldn, L"PISTEET=",8)) {
			wchar_t *p1, *p2 = NULL;
			int nn, na;
			pos = 7;
			ny = yhteys_no(fldn, &pos) - 1;
			if (ny > NREGNLY-1 || ny < 0)
				ny = NREGNLY-1;
			if (pos == 7) {
				na = 0;
				ny = NREGNLY-1;
				}
			else
				na = ny;
			p = fldn + 8;
			nc = 0;
			for (p1 = p; *p1 && wcswcind(*p1, L"/,;") < 0; p1++) ;
			if (p[2] == L'/' || (*p1 && *p1 != L'/')) {
				y = 0;
				p2 = wcsstr(p, L"/");
				if (p2)
					*p2 = 0;
				p1 = wcstok(p, L",;");
				while (p1 && p1[0]) {
					if (p1[1] == 0) {
					   if ((nn = wcswcind(*p1, ch_piste)) >= 0) {
						  for (int r_no = na; r_no <= ny; r_no++)
							  aika_tunnus[r_no][y] = nn - 2;
						  if (nn == 1) nc = 1;
						  }
					   }
					else {
						for (int r_no = na; r_no <= ny; r_no++)
							aika_tunnus[r_no][y] = _wtoi(p1);
						}
					y++;
					if (y >= sizeof(aika_tunnus[0])/sizeof(aika_tunnus[0][0]))
						break;
					p1 = wcstok(NULL, L",;");
					}
				if (p2) {
					*p2 = L'/';
					p = p2;
					}
				}
			else {
				for (y = 0; y < sizeof(aika_tunnus[0])/sizeof(aika_tunnus[0][0]); y++, p++) {
				   if ((nn = wcswcind(*p, ch_piste)) >= 0) {
					  for (int r_no = na; r_no <= ny; r_no++)
						  aika_tunnus[r_no][y] = nn - 2;
					  if (nn == 1) nc = 1;
					  }
				   else
					  break;
				   }
				}
			if (*p == L'/' && !wmemcmpU(p+1, L"VAIN", 4) && y > 0) {
			   vainoletus = 1;
			   for (int r_no = na; r_no <= ny; r_no++)
				   ol_piste = aika_tunnus[r_no][y-1];
			   }
			if (nc && *p && *++p) {
			   wcscpy(tunn_srj_nimi, p);
			   }
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"KIERROSVA",9)) {
			kierrosvafl = 1;
			y = fldn[9] - L'0';
			if (y > 0 && y <= kilpparam.valuku && fldn[10] == L'=') {
					kierrosva[y+2] = fldn[11] - L'0';
               if (fldn[12] == L'/') {
                  for (ny = 0; ny < MAXSARJALUKU; ny++)
				  Sarjat[ny].va_raja[k_pv][y-1] = _wtoi(fldn+13);
				  }
			   }
            continue;
				}
         if( !wmemcmpU(fldn, L"VAINVƒLIAJAT",12)) {
			vainvaliajat = 1;
			continue;
            }
         if( !wmemcmpU(fldn, L"VAINALARAJAT",12)) {
			if (fldn[12] == L'=' && fldn[13] == L'E')
               vainalarajat = 0;
            else
               vainalarajat = 1;
            continue;
            }
		 if (!wmemcmpU(fldn, L"RATATUNNISTUS=EI", 15)) {
			 rataTunnistus = false;
			 continue;
		 }
//#if defined(UINTI) || defined(MERI5)
		 if( !wmemcmpU(fldn, L"TALL-LƒHD",6)) {
            era_lahdot = 1;
            continue;
			}
//#endif
#ifdef SIIMPORT
         if( !wmemcmpU(fldn, L"SIIMPORT",6)) {
			regnly[0] = LID_SIIMPORT;
            continue;
            }
#endif
#ifdef SAKKOCOM
		 if (!wmemcmpU(fldn, L"SAKKO_YHT",9) ||
			 !wmemcmpU(fldn, L"SAKKO_COM",9)) {
			pos = 9;
			ny = yhteys_no(fldn, &pos) - 1;
			if (pos == 9 || ny > NREGNLY-1 || ny < 0)
				ny = NREGNLY-1;
			regnly[ny] = LID_KURVINEN;
			if (ajanottofl == -1)
			   ajanottofl = 0;
////			sak_com = _wtoi(fldn+10);
#ifdef TCPLUKIJA
			if (!wmemcmpU(fldn+pos+1, L"UDP", 3)) {
				if (fldn[pos+4] == L'S')
					pos++;
				getudpstreamipparam(ny+MAX_LAHPORTTI, fldn+pos+4);
				continue;
				}
			if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
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
				  }
			   }
			continue;
			}
		 if( !wmemcmpU(fldn, L"SAKKO_BAUD=",11)) {
			y = _wtoi(fldn+11);
			sak_baud = 9;
			y1 = 38400;
			while (y1 > y && sak_baud > 2) {
			   sak_baud--;
			   y1 = y1/2;
					}
			continue;
			}
		 if( !wmemcmpU(fldn, L"SAKKO_LAJI=",11)) {
			sak_laji = fldn[11] - L'0';
			if (sak_laji > 0) {
			   sak_baud = 6 + sak_laji;
			   }
			continue;
			}
#endif
			if( !wmemcmpU(fldn, L"RACPISTEET",5)) {
			racpisteet = 1;
			continue;
			}
#if defined(EMITLEIMAT) && defined(MAALI) 
		 if( !wmemcmpU(fldn, L"LAINALUETTELO",9)) {
				lainatied = 1;
			p = wcsstr(fldn, L"=");
			if (p != NULL) {
				wchar_t *p2;

				p2 = wcsstr(fldn, L"/");
				if (p2 == NULL)
					p2 = p;
				if (!wcscmp(p2+1, L"V") || !wcscmp(p2+1, L"VAIN")) {
					lainatied = 2;
					*p2 = 0;
					}
				if (p[0] != 0 && p[1] != 0) {
					wcsncpy(lainafname, p+1, sizeof(lainafname)/2-1);
					}
				}
            continue;
            }
		 if( !wmemcmpU(fldn, L"EMIT",5)) {
            emitfl = 1;
			continue;
            }
		 if( !wmemcmpU(fldn, L"EMLOKI",5)) {
            emitloki = 1;
			if (emitfl == 2)
				emitfl = 3;
            continue;
            }
         if( !wmemcmpU(fldn, L"ESITARK",5)) {
				esitark = 1;
			if (emitfl == 2)
				emitfl = 3;
			continue;
            }
         if( !wmemcmpU(fldn, L"ESILUENTA",5)) {
			esiluenta = 1;
			if (wcsstr(fldn, L"=K") != NULL)
				kuntosuunnmoodi = 99;
			if (emitfl == 2)
				emitfl = 3;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LAINAT",5)) {
			ilmlainat = 1;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LEIMAT=E",8)) {
			ohitaleimat = 1;
			continue;
			}
		 if( !wmemcmpU(fldn, L"EILƒHT=AVOIN", 8)) {
			eilavoin = TRUE;
			continue;
			}
		 if( !wmemcmpU(fldn, L"EMITAJAT",8)) {
			emitajat = 1;
			if (fldn[8] == L'=')
				emitajat = _wtoi(fldn+9);
			ajanottofl = 1;
			continue;
			}
		 if( !wmemcmpU(fldn, L"ELINKLƒHT÷",7)) {
			if (wcsstr(fldn, L"=E") != 0)
				eNollausAjat = 1;
			else
				eNollausAjat = 2;
			continue;
			}
		 if( !wmemcmpU(fldn, L"LƒHT÷VERT=",10)) {
			lahtopoikk = _wtoi(fldn+10);
			continue;
			}
		 if( !wmemcmpU(fldn, L"KARTTA=",7)) {
			wcsncpy(kilpparam.kartta, fldn+7, sizeof(kilpparam.kartta)/2-1);
			continue;
			}

		 if( !wmemcmpU(fldn, L"EMITKELLO",5)) {
			emitfl = 1;
			regnly[0] = LID_EMITKELLO;
			port_regnly[0] = 1;
				ajanottofl = 1;
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
		 if( !wmemcmpU(fldn, L"LUKIJA",6)) {
			pos = 6;
			ny = yhteys_no(fldn, &pos) - 1;
			if (pos == 6 || ny > NREGNLY-1 || ny < 0)
				ny = NREGNLY-1;
//            if (fldn[6] > L'0' && fldn[6] <= L'0'+NREGNLY)
//               ny = fldn[6] - L'1';
			regnly[ny] = LID_LUKIJA;
			port_regnly[ny] = 1;
			if (ajanottofl == -1)
			   ajanottofl = 0;
			emitfl = 1;
#ifdef TCPLUKIJA
			if (!wmemcmpU(fldn+pos+1, L"UDP", 3)) {
				if (fldn[pos+4] == L'S')
					pos++;
				getudpstreamipparam(ny+MAX_LAHPORTTI, fldn+pos+4);
				continue;
				}
			if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
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
			if (!wmemcmpU(fldn, L"EMITAGLISƒ=0", 12)) {
				emitaglisa = 0;
				continue;
				}
			if (!wmemcmpU(fldn, L"EMITAGLISƒ", 9)) {
				emitaglisa = 1;
				continue;
				}
			if (!wmemcmpU(fldn, L"EMITAG",6)) {
				ny = NREGNLY-1;
				if ((ny = _wtoi(fldn+6)-1) >= NREGNLY || ny < 0)
					ny = NREGNLY-1;
				regnly[ny] = LID_EMITAG;
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
			if (!wmemcmpU(fldn, L"EMITANAL",7)) {
#ifndef _CONSOLE
				emitanal_fl = 1;
				if ((p = wcsstr(fldn, L"=")) != NULL)
					kilpparam.analoolisa = _wtoi(p+1);
#endif
				continue;
				}
			if (!wmemcmpU(fldn, L"ECAIKA",4)) {
				emittime = 1;
				continue;
				}
#ifdef EMITHTTP
			if (!wmemcmpU(fldn, L"ETSQL", 5)) {
				ny = NREGNLY - 1;
				if ((ny = _wtoi(fldn + 5) - 1) >= NREGNLY || ny < 0)
					ny = NREGNLY - 1;
				regnly[ny] = LID_ETSQL;
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
			if (!wmemcmpU(fldn, L"ETGPRS", 6)) {
				ny = NREGNLY-1;
				if ((ny = _wtoi(fldn+6)-1) >= NREGNLY || ny < 0)
					ny = NREGNLY-1;
				regnly[ny] = LID_ETGPRS;
				eTParam.eTrno = ny;
				ajanottofl = 1;
				kaikki_ajat[ny+1] = 4;
				emitfl = 1;
				lahdepistehaku = 1;
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
			if (!wmemcmpU(fldn, L"ETHAKUVƒLI=",11)) {
				eTParam.eThakuvali = 2*_wtoi(fldn+11);
				emitfl = 1;
				if (wcsstr(fldn, L"/A") != NULL)
					eTParam.autostart = true;
				continue;
				}
			if (!wmemcmpU(fldn, L"ETDATE=",7)) {
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
//					wmemmove(eTdate+4, eTdate+5, 6);
//					wmemmove(eTdate+6, eTdate+7, 3);
					}
				continue;
				}
			if (!wmemcmpU(fldn, L"ETTIME=",7)) {
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
			if (!wmemcmpU(fldn, L"ETHOST=",7)) {
				if ((p = wcstok(fldn+7, L":")) != NULL) {
					wcsncpy(eTParam.eThost, p, sizeof(eTParam.eThost)/2-1);
					if ((p = wcstok(NULL, L":")) != NULL)
						eTParam.eTport = _wtoi(p);
					}
				continue;
				}
#endif
		 if( !wmemcmpU(fldn, L"MTR",3)) {
			ny = NREGNLY-1;
			if ((ny = _wtoi(fldn+3)-1) >= NREGNLY || ny < 0)
				ny = NREGNLY-1;
			regnly[ny] = LID_MTR;
			port_regnly[ny] = 1;
			if (ajanottofl == -1)
			   ajanottofl = 0;
			emitfl = 1;
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
#ifdef SPORTIDENT
		 if( !wmemcmpU(fldn, L"SPORTIDENT",10)) {
			ny = NREGNLY-1;
			if ((ny = _wtoi(fldn+3)-1) >= NREGNLY || ny < 0)
				ny = NREGNLY-1;
			regnly[ny] = LID_SPORTIDENT;
            port_regnly[ny] = 1;
            if (ajanottofl == -1)
			   ajanottofl = 0;
            emitfl = 1;
#ifdef TCPLUKIJA
            if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
				gettcpipparam(ny+MAX_LAHPORTTI, fldn+pos+4, 0);
				continue;
				}
#endif
			if ((p = wcstok(fldn, L"=")) != NULL) {
			   if ((p = wcstok(NULL,L":,-/")) != NULL) {
				  port_regnly[ny] = _wtoi(p);
				  }
			   }
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"LUKUTOISTO=0",12) || !wmemcmpU(fldn, L"LUKUTOISTO=1",12) ||
			!wmemcmpU(fldn, L"LUKUTOISTO=E",12)) {
			lukutoisto = 1;
			continue;
			}
		 if (!wmemcmpU(fldn, L"EMITVASIVU",10)) {
			if ((p = wcstok(fldn, L"=")) != NULL) {
					if ((p = wcstok(NULL,L":,/")) != NULL) {
						valinelendef = _wtoi(p);
						if (valinelendef)
							valinelen = valinelendef;
				   if ((p = wcstok(NULL,L":,/")) != NULL) {
							vapagelendef = _wtoi(p);
							if (vapagelendef)
								vapagelen = vapagelendef;
							if ((p = wcstok(NULL,L":,/")) != NULL)
								vapageskip = _wtoi(p);
								if (vapageskip)
									vapagelen -= vapageskip;
							}
						}
					}
            continue;
            }
         if( !wmemcmpU(fldn, L"TARKRAPOHITA",10)) {
			if ((p = wcstok(fldn, L"=")) != NULL) {
               if ((p = wcstok(NULL,L":,/")) != NULL) {
						tarkrappageskip = _wtoi(p);
						}
					}
            continue;
			}
		 if( !wmemcmpU(fldn, L"MAALILEIMASIN=E",15)) {
			maalileimasin = -1;
			continue;
			}
		 if( !wmemcmpU(fldn, L"AUTOKILP",8)) {
            autokilp = 1;
            ajat_on = 1;
            continue;
			}
		 if( !wmemcmpU(fldn, L"COMAUTORAP",10)) {
			comautorap = 1;
			if (!autorap)
			   autorap = 2;
			if (fldn[10] == L'=') {
			   switch (fldn[11]) {
				  case L'0':
				  case L'E':
					 autorap = 0;
					 break;
				  case L'1':
				  case L'H':
				  case L'S':
					 autorap = 1;
					 break;
				  case L'V':
					 autorap = -1;
					 break;
				  }
					}
			continue;
			}
		 if( !wmemcmpU(fldn, L"AUTORAP",7)) {
			if (!autorap)
			   autorap = 2;
			if (fldn[7] == L'=') {
					switch (fldn[8]) {
				  case L'0':
				  case L'E':
					 autorap = 0;
					 break;
				  case L'1':
				  case L'H':
				  case L'S':
					 autorap = 1;
					 break;
				  case L'V':
					 autorap = -1;
					 break;
				  }
			   }
			continue;
			}
         if (!wmemcmpU(fldn, L"SIVUJAKO",8)) {
            emitsivu = 1;
            if (fldn[8] == L'=')
               emitsivu = _wtoi(fldn+9) - 1;
            continue;
            }
		 if (!wmemcmpU(fldn, L"HTMLSƒILNIMI",9)) {
            html_sail_nimi = 1;
            continue;
			}
         if (!wmemcmpU(fldn, L"ORIGEMVABADGE",9)) {
			origemvabadge = true;
            continue;
			}
         if (!wmemcmpU(fldn, L"ESTƒEMITTOISTO",10)) {
			estaemittoisto = (wmemcmpU(fldn+wcslen(fldn)-2, L"EI", 2) != 0);
			continue;
            }
         if (!wmemcmpU(fldn, L"SALLIEMITTOISTO",10)) {
			estaemittoisto = (wmemcmpU(fldn+wcslen(fldn)-2, L"EI", 2) == 0);
			continue;
			}
		 if( !wmemcmpU(fldn, L"AIKALUKIJA",6)) {
			ny = 0;
			ohitatoisto = 1;
			ajanottofl = 1;
			if ((ny = _wtoi(fldn+10)) > NREGNLY || ny < 0)
				ny = 0;
			p = wcsstr(fldn, L"=");
			if (p)
				pos = p - fldn;
			else
				pos = 10;
			if (!wmemcmpU(fldn+pos,L"=V",2) || !wmemcmpU(fldn+pos,L"=K",2) || !wmemcmpU(fldn+pos,L"=L",2)) {
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
				else {
					if (fldn[pos] == L'A')
						vainpiste[ny] = -2;
					else if (fldn[pos] == L'L')
						vainpiste[ny] = -1;
					else {
						vainpiste[ny] = -3;
						kaikki_ajat[ny] = 4;
						}
					}
				}
			else
				kaikki_ajat[ny] = 1;
			continue;
			}
		if( !wmemcmpU(fldn, L"SJBOX",5) || !wmemcmpU(fldn, L"LƒHDEPISTEET",8)) {
			lahdepistehaku = 1;
			continue;
			}
		if( !wmemcmpU(fldn, L"VAADIA",6)) {
			vaadiaika = 1;
			continue;
			}
		if( !wmemcmpU(fldn, L"KUORMITUSTESTI",9)) {
			luesuoraan = 2;
			aani = 0;
			continue;
			}
		if( !wmemcmpU(fldn, L"LUESUORAAN",6)) {
			luesuoraan = 1;
			aani = 0;
			if (!wmemcmpU(fldn, L"LUESUORAAN=",11)) {
				luesuoraanviive = _wtoi(fldn+11);
				}
			continue;
			}
		if( !wmemcmpU(fldn, L"AUTOSARJA",7)) {
			autosarja = 1;
			if (wcsstr(fldn, L"=E"))
				autosarja = 0;
			continue;
			}
		if( !wmemcmpU(fldn, L"AUTORATA",7)) {
			autorata = 1;
			continue;
			}
		if( !wmemcmpU(fldn, L"ANNASEUR",8)) {
			annaseur = 1;
			continue;
			}
#endif
#if defined(EMIT) || defined(LAJUNEN)
         if (!wmemcmpU(fldn, L"VALONƒYTT÷=",11)) {
            valonaytto = _wtoi(fldn+11);
            continue;
            }
#ifdef LAJUNEN
		 if (!wmemcmpU(fldn, L"VALOT=", 6)) {
            regnly[NREGNLY-1] = LID_VALOT;
				port_regnly[NREGNLY-1] = _wtoi(fldn+6);
				if (!valonaytto)
					valonaytto = 10;
			continue;
            }
#endif
#endif
#if defined(EMIT) && (defined(MAALI) || defined(TALLEMIT))
			if( !wmemcmpU(fldn, L"LƒHEMIT",7) && (pos = 7) != 0 &&
				(ny = yhteys_no(fldn, &pos)) != 0) {
			emitfl = 1;
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
		 if( !wmemcmpU(fldn, L"LƒHEMVA",7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
            emitfl = 1;
            com_emitva[ny-1] = 1;
            continue;
            }
		 if (!wmemcmpU(fldn, L"EMVALOKI",8)) {
			emitvaloki = true;
			continue;
			}
#endif
#if defined(FISKOODIxx)
         if( !wmemcmpU(fldn, L"FISKOODI",5)) {
            fiskoodi = 1;
			if (fldn[wcslen(fldn)-1] == L'2')
				fiskoodi = 2;
            continue;
            }
#endif
#ifdef JAAMA
		 if (!wmemcmpU(fldn, L"PISTEPV=",8)) {
			jaamapisteet = _wtoi(fldn+8);
			continue;
			}
#endif
		 if (!wmemcmpU(fldn, L"BIBSIIRTOALUE=",8)) {
			p = wcsstr(fldn, L"=");
			if (p && p[1])
				bibsiirtoalue = _wtoi(p+1);
			continue;
			}
		 if (!wmemcmpU(fldn, L"RAPORTTIHAK=",12)) {
			wcsncpy(RaporttiHak, fldn+12, 200);
			continue;
			}
		 if (!wmemcmpU(fldn, L"DEVELOPER",9)) {
			DeveloperMode = true;
			continue;
			}
		 swprintf(msg, L"Tuntematon parametri %.17s %s %s", fldn,
			L"tiedostossa ",
            cfgfilename);
		 writeerror_w(msg,0);
			}
      if (i <= 1) {
         readcfg(&nf, argptr, i);
         }
      }
   }

static void luetlskertoimet(void)
	{
	TextFl *kfl;
	wchar_t s[100], *p;
	int i_pv, k, j, srj;


	for (i_pv = 0; i_pv <  kilpparam.n_pv_akt; i_pv++) {
	for (srj = 0; srj < sarjaluku; srj++) {
		 Sarjat[srj].tlskerroin[i_pv] = 1;
			Sarjat[srj].tlsjakaja[i_pv] = 1;
		 }
		}
	if (kilpparam.n_pv_akt < 2) {
		return;
		}
	kfl = new TextFl(L"TLSKERR.LST", L"rt");
	if (kfl->IsOpen()) {
		while (!kfl->Feof()) {
			kfl->ReadLine(s, 99);
			if ((p = wcstok(s, L" ,;\t\n")) != 0) {
				upcasewstr(s);
				elimwbl(s);
				j = 1;
				if (s[0] && (p = wcstok(NULL, L" ,;\t\n")) != 0) {
					i_pv = _wtoi(p) - 1;
					if (i_pv >= 0 && i_pv < kilpparam.n_pv_akt && (p = wcstok(NULL, L" ,;\t\n")) != 0) {
						k = _wtoi(p);
						if ((p = wcstok(NULL, L" ,;\t\n")) != 0)
							j = _wtoi(p);
						if (k < 1)
							k = 1;
						if (j < 1)
							j = 1;
						if (!wcscmp(s, L"KAIKKI")) {
							for (srj = 0; srj < sarjaluku; srj++) {
								Sarjat[srj].tlskerroin[i_pv] = k;
								Sarjat[srj].tlsjakaja[i_pv] = j;
								}
							}
						else if ((srj = haesarja_w(s, false)) >= 0) {
							Sarjat[srj].tlskerroin[i_pv] = k;
							Sarjat[srj].tlsjakaja[i_pv] = j;
							}
						}
					}
				}
			}
		}
	delete kfl;
	}

//#if defined(UINTI) || defined(MERI5)
void lueeratiedot(wchar_t *fname)
   {
   TextFl *erafl = NULL;
   int n = 0, k = 0, kno, era, rata, d;
   kilptietue kilp, entkilp;
   wchar_t ln[80], msg[80];

   if (!fname)
		fname = L"ERADAT.LST";
   erafl = new TextFl(fname, L"rt");
   if (erafl != NULL && erafl->IsOpen()) {
      while (!erafl->Feof()) {
		  erafl->ReadLine(ln, sizeof(ln)/2-1);
          if (swscanf(ln, L"%d %d %d\n", &kno, &era, &rata) == 3) {
            if ((d = getpos(kno)) > 0) {
			   kilp.GETREC(d);
               entkilp = kilp;
               kilp.pv[k_pv].era = era;
               _itow(rata, kilp.pv[k_pv].rata, 10);
			   kilp.tallenna(d, 0, 0, 0, &entkilp);
               n++;
               }
            else
               k++;
            }
         else
            k++;
         }
	  k--;
      swprintf(msg, L"Tietoja kirjattu %d, puutteellisia rivej‰ %d", n, k);
	  writeerror_w(msg, 0, true);
	  }
   else
	  writeerror_w(L"Tiedostoa ERADAT.LST ei lˆydy", 0, true);
   if (erafl != NULL)
		delete erafl;
   }
//#endif

#ifdef _CONSOLE

static int ConsoleAloitus(int *y)
{
	wchar_t ch, ch2, prs[100], msg[100];

   if (ikkunaots[0])
	   SetConsoleTitle(ikkunaots);
   nimio(1);
   viwrrectw(11,5,11,30,L"Voimassa olevat valinnat :",7,0,0);
   ch = 0;
   *y = 16;
korjval:
   clrln(ySize-3);
   if (ch) {
      for (*y=11; *y<ySize-3; (*y)++) 
		  clrln(*y);
      viwrrectw(11,5,11,23,L"Korjatut valinnat :",7,0,0);
      }
#ifdef EI_OLE
   if (ch) {
      clrln(ySize-3);
      viwrrect(ySize-3,5,ySize-3,24,"Anna tarratiedosto :",7,0,0);
      viwrrect(ySize-1,5,ySize-1,54,
         "NUL : Ei tarroja,  LƒHETƒ : Toisella tietokoneella",7,0,0);
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
   else if ( !memcmp(tarfname, "LƒHETƒ", 4) ) {
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
   if (tarfl && !ch && korkeat)
      {
	  viwrrect(y,5,y,45,"Tarrakirjoitin kirjoittaa korkeaa teksti‰",
         7,0,0);
      y++;
	  }
   if (ch && tarfl) {
      ch2 = korkeat ? 'K' : 'E';
      selectopt("Kirjoitetaanko leve‰t kirjasimet korkeina (K/E)","KE",
         &ch2);
      if (ch2 == 'K') {
         korkeat = TRUE;
         y = 16;
         viwrrect(y,5,y,45,"Tarrakirjoitin kirjoittaa korkeaa teksti‰",
            7,0,0);
         y++;
         ch2 = star ? 'K' : 'E';
         selectopt("Tunteeko kirjoitin STAR NL-10:n komennot (K/E)","KE",
            &ch2);
		 star = (ch2 == 'K');
         }
      }
   if (ch && tarfl) {
	  ch2 = tarra1 ? '1' : '2';
      selectopt("1 vai 2 tarrarivi‰","12",&ch2);
      tarra1 = (ch2 == '1');
      }
   if( tarra1 ) {
      viwrrect(y,5,y,18,"Yksi tarrarivi",7,0,0);
	  y++;
      }
#endif
   if (ch) {
      clrln(ySize-3);
	  viwrrectw(ySize-3,5,ySize-3,24,L"Anna listatiedosto :",7,0,0);
      inputwstr(listfname,39,26,ySize-3,L"\r",&ch2,FALSE);
	  }
   elimwbl(listfname);
	if (!wcscmp(listfname, L"PRN")) 
		wcscpy(listfname, L"OLETUS");
	swprintf(prs,L"Listakirjoitinportti tai -tiedosto = %s", listfname);
   vidspwmsg(*y,5,7,0,prs);
   if (ch) {
		wselectopt(L"Merkistˆ: I)BM, A)scii-7, R)oman-8, P)ostscrpt"
			L", W)indows,L)evylt‰", L"ARLIPWG",&merkit);
		if (merkit == L'W') {
			ch2 = L' ';
			wselectopt(L"K‰ytet‰‰nkˆ Windowsin kirjoitinajureita (K/E)", L"KE", &ch2);
			if (ch2 == L'K')
				merkit = L'G';
			}
		}
   clrln(ySize-3);
   switch (merkit) {
      case L'A' : viwrrectw(*y,63,*y,78,L"Merkistˆ Ascii-7",7,0,0);
                 break;
      case L'R' : viwrrectw(*y,63,*y,78,L"Merkistˆ Roman-8",7,0,0);
                 break;
      case L'P' : viwrrectw(*y,62,*y,79,L"Merkistˆ Postscript",7,0,0);
                 break;
	  case L'W' : viwrrectw(*y,63,*y,78,L"Merkistˆ Windows",7,0,0);
                 break;
      case L'L' : viwrrectw(*y,63,*y,78,L"Merkistˆ levylt‰",7,0,0);
                 break;
#ifdef WIN32
      case L'G' : viwrrectw(*y,63,*y,78,L"Windowskirjoitin",7,0,0);
                 break;
#endif
      default  : viwrrectw(*y,63,*y,79,L"Merkistˆ IBM (PC)",7,0,0);
      }
   (*y)++;
   if (ch) {
      switch (aani) {
         case  0 : ch2 = L'E'; break;
         case  1 : ch2 = L'V'; break;
         default : ch2 = L'H';
         }
      wselectopt(L"ƒ‰nimerkki: E)i k‰ytet‰,  vain V)irheist‰,  myˆs H)uomio‰‰ni"
         ,L"EVH", &ch2);
      switch (ch2) {
		 case L'E' : aani = 0; break;
		 case L'V' : aani = 1; break;
		 case L'H' : aani = 2; break;
         }
      }
   viwrrectw(*y,5,*y,16,L"ƒ‰nimerkki :",7,0,0);
   switch (aani) {
      case 0  : viwrrectw(*y,18,*y,28,L"Ei k‰ytˆss‰", 7, 0, 0); break;
      case 1  : viwrrectw(*y,18,*y,31,L"Vain virheist‰", 7, 0, 0); break;
      default : viwrrectw(*y,18,*y,39,L"Virhe- ja huomiomerkit", 7, 0, 0);
      }
   ++*y;
#ifndef COMOFF
	wcscpy(msg, L"K‰ytˆss‰ yhteydet:");
	for (int nc = 0; nc < MAX_LAHPORTTI; nc++) {
		if( portparam[nc] != 0) {
			lahcom[nc] = (comdir[nc] == L'I') ? FALSE : TRUE;
			vastcom[nc] = (comdir[nc] == L'O') ? FALSE : TRUE;
			}
		if (ch && nc < 8) {
			clrln(ySize-3);
			vidspwmsg(ySize-3,5,7,0,
				L"Anna portin   numero :    (0 : ei k‰ytet‰, -1 : UDP), Esc: pois yhteyksist‰");
			vidint(ySize-3,17,1,nc+1);
			INPUTINTW(&portparam[nc],2,28,ySize-3,L"\r\x1b",&ch2);
			if (ch2 == ESC)
				break;
			}
		if( portparam[nc] != 0) {
			portti[nc] = portparam[nc];
			if (comtype[nc] == comtpRS)
				swprintf(prs, L"Yhteys %d : COM%d", nc+1, portparam[nc]);
			else
				swprintf(prs, L"Yhteys %d : %s", nc+1, (comtype[nc]  & comtpUDP) ? L"UDP" : L"TCP");
			clrln(*y);
			vidspwmsg(*y,5,7,0,prs);
			lahcom[nc] = (comdir[nc] == L'I') ? FALSE : TRUE;
			vastcom[nc] = (comdir[nc] == L'O') ? FALSE : TRUE;
			if (!lahcom[nc])
				vidspwmsg(*y,53,7,0,L"Vain vastaanotto           ");
			if (!vastcom[nc])
				vidspwmsg(*y,53,7,0,L"Vain l‰hetys               ");
#if MAX_LAHPORTTI > 22
			if (wcslen(msg) < 73)
#endif
				swprintf(msg+wcslen(msg), L" %d", nc+1);
#if MAX_LAHPORTTI > 22
			else if (msg[wcslen(msg)-1] != L'.')
				wcscat(msg, L" ..");
#endif
			if (*y < 21)
				(*y)++;
			clrln(*y);
			vidspwmsg(*y, 5, 7, 0, msg);
			}
		}
#endif
   clrln(ySize-3);
   if (!bootfl) {
	  nimio(0);
      ch = L'H';
	  wselectopt(L"H)yv‰ksy valinnat,  K)orjaa valintoja,  <Esc>: keskeyt‰",
		 L"HK\x1B", &ch);
	  if (ch == 27) {
		 scpclr();
		 lopetus();
		 }
	  if (ch == L'K') goto korjval;
	  }
   for (int i = 11; i < ySize; i++) {
	  clrln(i);
	  }
	return(0);
}

#endif  // _CONSOLE

int aloitus(int argc, wchar_t* argv[], wchar_t *cfgflname)
{
	INT  y,bd,i,nc, er = 0;
	wchar_t prs[100], *p;
	char cst[102];
	FILE *tmpfile;
	bool kysy_pv = false;

	INT maalit[10] = {0,0,0,0,0,0,0,0,0,0};
	extern int naytajono;

//  Pyyd‰ tietokantaohjelmaa olemaan k‰ytt‰m‰tt‰ uudelleen vapautettuja tietueita. Nyt oletuksena.
//	ainauusirec = 1;

//	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//	GetVersionEx(&osvi);
	prblocksize = PRBLOCKSIZE;
	prblockbits = PRBLOCKBITS;
	_ltow(sarjano/10L,snostr,10);
	InitializeCriticalSection(&idindex_CriticalSection);
	InitializeCriticalSection(&tall_CriticalSection);
	InitializeCriticalSection(&loki_CriticalSection);
	InitializeCriticalSection(&inb_CriticalSection);
	InitializeCriticalSection(&outb_CriticalSection);
	InitializeCriticalSection(&kilpdat_CriticalSection);
	InitializeCriticalSection(&ajat_CriticalSection);
	InitializeCriticalSection(&emit_CriticalSection);
	InitializeCriticalSection(&rastiva_CriticalSection);
	InitializeCriticalSection(&tcpip_CriticalSection);
	InitializeCriticalSection(&seura_CriticalSection);
	InitializeCriticalSection(&seur_badge_CriticalSection);

#ifdef _DEBUG
	DeveloperMode = true;
#endif
	maxkilpno = 9999;
	autofileparam.af_xmlva = L'M';
	wcscpy(autofileparam.aflstnm, L"AUTOFILE.LST");
	wcscpy(autofileparam.jakelumaar, L"AutoJakelu.xml");
	autofileparam.sanafchars = L'8';
	for (i = 1; i < MAX_LAHPORTTI; i++) {
	  wcscpy(naapuri[i], naapuri[0]);
	  }
	for (i = 1; i < MAX_PORTTI; i++) {
	  wcscpy(baudst[i], baudst[0]);
	  baud[i] = baud[0];
	  pty[i] = pty[0];
	  combits[i] = combits[0];
	  stopbits[i] = stopbits[0];
	  }
	for (i = 0; i <= VALUKU+2; i++)
		kierrosva[i] = i - 2;

   for (i = 0; i < NREGNLY; i++) {
	   t0_regn[i] = t0_regnly;
	   cn_regnly[i] = MAX_LAHPORTTI + i;
	   cn_regnly[i + NREGNLY] = MAX_LAHPORTTI + i + NREGNLY;
	   if (i > 0)
		  memcpy(aika_tunnus[i], aika_tunnus[0], sizeof(aika_tunnus[0]));
		SiritEventPort[i] = 50008;
		SiritCmdPort[i] = 50007;
	   }

   for (i = 0; i <= NREGNLY; i++) {
	   vainpiste[i] = -9;
	   kaikki_ajat[i] = -1;
	   aikaero[i] = TMAALI0;
	   }

	for (i = 1; i < MAXERA; i++)
		eralahto[i] = eralahto[0];

	for (int i_pv = 0; i_pv < N_PV; i_pv++) {
		pvparam[i_pv].laika = 8;
		pvparam[i_pv].laikasj = 8;
		pvparam[i_pv].laika2 = 8;
		pvparam[i_pv].laikava = 8;
		pvparam[i_pv].pyor[0] = 1;
		pvparam[i_pv].pyor[1] = SEK;
		pvparam[i_pv].pyor[2] = SEK;
		pvparam[i_pv].pyor[3] = SEK;
		pvparam[i_pv].pyor[4] = SEK;
		pvparam[i_pv].pyor[5] = SEK;
		}

   if (ToimintaTila == 2)
		k_pv = -1;
	swprintf(kilpparam.tiedlaji, L"HK%d", VersioInt());
	kilpparam.maxasakko = -99999;
	kilpparam.vakanttisarja = -1;
	kilpparam.analoolisa = 20;
	kilpparam.yhttlslaji = L'S';
	aftulparam.tulostettava = L'T';
	aftulparam.kohde = L'H';
	aftulparam.yksihtml = 1;
	aftulparam.rajaus = L'L';

	lue_parametrit(argc, argv, cfgflname);

	Sleep(100);
	if (inLopetus)
		return(1);
	if (aikaero[NREGNLY] != TMAALI0) {
		for (i = 0; i < NREGNLY; i++) {
			if (aikaero[i] == TMAALI0)
				aikaero[i] = aikaero[NREGNLY];
			}
		}

	if (vaihevalinta)
		k_pv = luevaihevalinta();
   if (k_pv == -1) {
	   kysy_pv = true;
	   k_pv = 0;
	   }
	if (vainoletus && ol_piste > -2) {
		for (y = 0; y < aika_tunnus_lkm; y++) {
			for (int r_no = 0; r_no < NREGNLY; r_no++) {
				if (!(lajatfl && aika_tunnus[r_no][y] == -1))
					aika_tunnus[r_no][y] = ol_piste;
				}
			}
		}
	if (minvakanttiluku > n_lisaa) {
		writeerror_w(L"Parametrin VAKANTTILUKU arvo liian suuri. Parametri LISƒƒ ehk‰ syyn‰.", 0);
		return(1);
		}
   InitializeCriticalSection(&print_CriticalSection);
   InitializeCriticalSection(&autotul_CriticalSection);
#ifdef _CONSOLE
   ConsoleAloitus(&y);
#endif
	arv_pv = k_pv;
	if (ToimintaTila == 2) {
		for (nc = 0; nc < MAX_LAHPORTTI; nc++) {
			if( portparam[nc] != 0) {
				portti[nc] = portparam[nc];
				if (portti[nc] > 0) {
					p = wcstok(baudst[nc], L",:;");
					if (p && (i = _wtoi(p)) > 1000) {
						baud[nc] = 12;
						bd = 220000;
						while (i < bd && baud[nc] > 2) {
							baud[nc]--;
							if (bd == 55000)
								bd = 36000;
							else
								bd = bd/2;
							}
						p = wcstok(NULL, L",:;");
						if (p && *p) {
							pty[nc] = (char) towlower(*p);
							}
						p = wcstok(NULL, L",:;");
						if (p && *p) {
							combits[nc] = _wtoi(p);
							}
						p = wcstok(NULL, L",:;");
						if (p && *p) {
							stopbits[nc] = _wtoi(p);
							}
						}
					comkesto[nc] = (18 * 10 * 200) / bd + 20;
					}
				lahcom[nc] = (comdir[nc] == L'I') ? FALSE : TRUE;
				vastcom[nc] = (comdir[nc] == L'O') ? FALSE : TRUE;
				}
			}
		}

	backfl = FALSE;
	nimiixfl = FALSE;

	if (luesarjat()) {
		writeerror_w(L"Sarjatietojen lukeminen ei onnistunut", 0);
		return(1);
		}

	if (k_pv >= kilpparam.n_pv_akt)
		k_pv = kilpparam.n_pv_akt - 1;
	for (i = 0; i < MAXSARJALUKU; i++) {
		if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
/*
			Sarjat[i].tlsjakaja[0] = 2;
			Sarjat[i].tlskerroin[0] = 3500;
			Sarjat[i].tlsjakaja[1] = 2;
			Sarjat[i].tlskerroin[1] = 3000;
			Sarjat[i].tlsjakaja[2] = 5;
			Sarjat[i].tlskerroin[2] = 8904;
			Sarjat[i].tlsjakaja[3] = 1;
			Sarjat[i].tlskerroin[3] = 1950;
			Sarjat[i].tlsjakaja[4] = 5;
			Sarjat[i].tlskerroin[4] = 11604;
*/
			m3vert[0][i] = 30000;
			m3vert[1][i] = 300000;
			m3vert[2][i] = 600000;
			m3vert[3][i] = 100000;
			m3vert[4][i] = 100000;
			}
		else {
			for (nc = 0; nc < N_PV; nc++) {
				Sarjat[i].tlskerroin[nc] = 1;
				Sarjat[i].tlsjakaja[nc] = 1;
				}
			}
		}
	if ((nc = wcswcind(kilpparam.kilplaji, L"NP")) >= 0) {
		if (nc < 2)
			sotilas = 1;
		for (int ii = 0; ii < 12; ii++)
			for (int r_no = 0; r_no < NREGNLY; r_no++)
				aika_tunnus[r_no][ii] = L"000122223333"[ii] - L'2';
		}
	if (kilpparam.kilplaji == L'S' && kilpparam.alalaji == L'A') {
		kilpparam.kaksibadge = 2;
		}
	if (kilpparam.sakkoyks == 0)
		kilpparam.sakkoyks = 1;
	if (kilpparam.vakanttisarja < 0) {
		kilpparam.vakanttisarja = sarjaluku - 1;
		}
	if (ToimintaTila == 2 && kilpparam.badgelaji > L' ' && emitfl == 0) {
		if (vaajat)
			emitfl = -1;
		else {
			emitfl = 1;
			kaikki_ajat[0] = 2;
			}
		}
	if (vainalarajat == -1)
		vainalarajat = (wcswcind(kilpparam.kilplaji, L"SB") >= 0) ? 1 : 0;
	if (ohitatoisto == -1)
		ohitatoisto = (regnly[0] == LID_SIRIT || regnly[0] == LID_FEIG || regnly[0] == LID_IMPINJ) ? 1 : 0;
	if (ToimintaTila == 2) {
	   if (kilpparam.n_pv_akt > 1 && kysy_pv) {
		   wchar_t ch;

		  ch = L'1' + k_pv;
		  wcscpy(prs, L"12345678");
		  prs[kilpparam.n_pv_akt] = 0;
		  wselectopt(L"Vahvista kilpailup‰iv‰ (tai vaihe) tai vaihda antamalla numero",
			prs, &ch);
		 k_pv = ch - L'1';
		 }
		kilpparam.maxvaluku = 0;
		for (int srj = 0; srj < sarjaluku; srj++) {
			if (kilpparam.maxvaluku < Sarjat[srj].valuku[k_pv])
				kilpparam.maxvaluku = Sarjat[srj].valuku[k_pv];
			}
		luetlskertoimet();
		luesarjayhdistelmat();
		}
	if (kilpailu[0]) {
		wcsncpy(paaots, kilpailu, sizeof(paaots)/2-1);
		wcsncpy(sarjaots, kilpailu, sizeof(sarjaots)/2-1);
		}
#ifndef _CONSOLE
	wcsncpy(llparam.luetots0, kilpailu, 82);
#endif
	for (int rno = 0; rno <= NREGNLY; rno++)
		if (vainpiste[rno] > kilpparam.valuku)
			vainpiste[rno] = kilpparam.valuku;

	if (kilpparam.maxasakko == -99999)
		kilpparam.maxasakko =  kilpparam.kilplaji == L'A' ? 5 : 9999;
	for (int srj = 0; srj < sarjaluku; srj++)
		if (Sarjat[srj].lsak[k_pv] > kilpparam.lsakmax)
			kilpparam.lsakmax = Sarjat[srj].lsak[k_pv];
	if (wcswcind(kilpparam.kilplaji, L"ABNP") >= 0 || kilpparam.lsakmax) {
		for (int i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
			pvparam[i_pv].asakkolaji =
				(wcswcind(kilpparam.kilplaji, L"A") >= 0) ? 1 : 2;
			if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == L'H')
				pvparam[i_pv].asakkolaji = 3;
			}
		}
	luekentat(0);
	if (kilpparam.n_pv_akt == 1) {
		wmemmove(obfilename+7, obfilename+8, 5);
		wmemmove(aikaflname[0]+4, aikaflname[0]+5, 5);
		wmemmove(aikaflname[1]+5, aikaflname[1]+6, 5);
		}
	else {
		if (obfilename[7] == L'1') obfilename[7] = k_pv + L'1';
	  if (aikaflname[0][4] == L'1')
		  aikaflname[0][4] = k_pv + L'1';
	  if (aikaflname[1][5] == L'1')
		  aikaflname[1][5] = k_pv + L'1';
	  if (!wmemcmpU(lokifname, L"LOKI1.", 6)) lokifname[4] = k_pv + L'1';
	  }
#if defined(EMITLEIMAT) && defined(MAALI)
	if (kilpparam.n_pv_akt == 1)
		wmemmove(emitfname+4, emitfname+5, 5);
	else
		if (emitfname[4] == L'1') emitfname[4] = L'1' + k_pv;
#endif

	for ( i = 0 ; i < MAXSARJALUKU ; i++) sjfl[i] = TRUE;
	if (merkit != L'G' && merkit != L'W') {
		merkisto(merkit);
		}
	if (k_pv >= kilpparam.n_pv_akt)
		k_pv = kilpparam.n_pv_akt-1;
	for (i=0; i < sarjaluku + nsarjayhd; i++) {
		prior0[i] = 1;
		if ((int)sizeof(aktkilp->pv[k_pv].asakot)/2 - 1 < Sarjat[i].lsak[k_pv]) {
			Sarjat[i].lsak[k_pv] = sizeof(aktkilp->pv[k_pv].asakot)/2 - 1;
			}
		if (Sarjat[i].mno[k_pv] < 1)
			Sarjat[i].mno[k_pv] = 1;
		if (Sarjat[i].mno[k_pv] > 9)
			Sarjat[i].mno[k_pv] = 9;
		maalit[Sarjat[i].mno[k_pv]-1] = 1;
		}

	lsttulparam.tulmuot = tulosmuot;
	lokitulparam.tulmuot = tulosmuot;

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
			lopetus();
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
			}
		}

	initmstimer();

	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
//		filetulosmuot.tauluhtml = 0;
//		mobiltulosmuot.tauluhtml = 0;

		if (!kilpparam.lahtoryhma) {
			kilpparam.lahtoryhma = Sarjat[0].pari[k_pv];
			}
		if (kilpparam.lahtoryhma > 100) {
			int j, k;

			j = 0;
			k = 0;
			for (i = 0; i < aika_tunnus_lkm; i++) {
				for (int r_no = 0; r_no <= NREGNLY; r_no++) {
					if (aika_tunnus[r_no][i] == 0)
						aika_tunnus[r_no][i] += 100*(j++);
					if (aika_tunnus[r_no][i] == 1)
						aika_tunnus[r_no][i] += 100*(k++);
					}
				}
			}
		if (kilpparam.kilplaji == L'P') {
			tulosmuot.pistedesim = 1;
			filetulosmuot.pistedesim = 1;
			mobiltulosmuot.pistedesim = 1;
			}
		}
	if (!kilpparam.lahtoryhma)
		kilpparam.lahtoryhma = 1;

	init_muotoilut();
	if (muotoilufl)
		LueXMLMuotoilu(luetfnm);
	lsttulparam.tulmuot = tulosmuot;
    autotulostusparam = lsttulparam;
	emittulparam = &lsttulparam;
	if (autostr[sizeof(autostr)/2-1] == '1')
		autoalku(autostr);
	if ((tmpfile = _wfopen(dataf2name, L"rb")) == NULL) {
		writeerror_w(L"Tiedostoa KILP.DAT ei ole. Luodaan uusi tiedosto.", 0);
		makefile(&datf2,wcstooem(cst, dataf2name, 100),kilpparam.kilprecsize);
		  closefile(&datf2);
		}
	else {
		fclose(tmpfile);
		}
	if (inLopetus || openfile(&datf2,wcstooem(cst, dataf2name, 100),kilpparam.kilprecsize)) {
		if (!inLopetus) {
			swprintf(prs,L"Tiedosto %s puuttuu tai sen avaaminen ei onnistu", dataf2name);
			writeerror_w(prs, 0);
			}
		return(1);
		}
//   if (n_lisaa != 1000) {
//		swprintf(prs,L"Laajennusvara %d kilpailijaa", n_lisaa);
//	  viwrrectw(12,5,12,33,prs,7,0,0);
//		}
   for (i = 0; i < 9; i++) if (!maalit[i]) maaliajat[i] = TMAALI0;
	if (!UDPCliWait)
		UDPCliWait = (WinVersion() > 4) ? 10 : 1000;
	UDPsetCliWait(UDPCliWait);

	if (ToimintaTila == 1) {
		emitfl = 0;
		}
	else {
	   if (emitfl == 2)
		   emitfl = 0;
	   if (emitfl) {
			int rno, ka = -1, emitfl_1 = -1;
			// Katso, ovatko kaikki merkinn‰t yhdenmukaiset
			for (rno = 0; rno < NREGNLY; rno++) {
				if (IntInList(regnly[rno], L"11 12 13 14 21") >= 0 &&
					kaikki_ajat[rno+1] >= 0 && kaikki_ajat[0] >= 0 &&
					(ka = kaikki_ajat[rno+1]) != kaikki_ajat[0]) {
					break;
					}
				}
			// Jos merkinnˆiss‰ ei kirjattuja eroja, p‰ivit‰ tiedot, joita ei viel‰ kirjattu
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
			// Aseta emitfl_1 = 1, jos leimoja siirret‰‰n verkossa
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
			// Aseta kaikki_ajat[0] = 1, jos sek‰ ajanottoa ett‰ leimantarkastusta
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
		}

	if (teejarj(false)) {
#ifdef _CONSOLE
		writeerror_w(L"", 0);
#else
		writeerror_w(L"Kilpailijatietojen lukeminen keskeytyi", 0);
#endif
		return(1);
		}
	clrln(17);
	clrln(18);

	if (ToimintaTila == 2 && emitfl == 1 && !esitark && esiluenta != 1) {
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
		}
	if (ToimintaTila == 2 && emitfl == 1) {
		if (lueemitfile()) {
			writeerror_w(L"Leimaustietotiedoston lukeminen ei onnistunut", 0);
			return(1);
			}
		if (lainatied)
			lue_lainat(0, 1);

		if (Sarjat[kilpparam.vakanttisarja].enspv > k_pv)
			Sarjat[kilpparam.vakanttisarja].enspv = k_pv;
		if (Sarjat[kilpparam.vakanttisarja].viimpv < k_pv)
			Sarjat[kilpparam.vakanttisarja].viimpv = k_pv;
		}

	if (ToimintaTila == 2) {
#ifndef _CONSOLE
		lueKorRajat();
#endif
		if (lajatfl && !pvparam[k_pv].hiihtolahto) {
			writeerror_w(L"Parametri LAJAT, mutta l‰htˆportin aikojen k‰yttˆ‰ ei valittuna", 0);
			}
		if (pvparam[k_pv].hiihtolahto && *tunn_srj_nimi) {
			  if (!wcscmp(tunn_srj_nimi, L"KAIKKI"))
				 tunn_srj = sarjaluku;
				else
				 tunn_srj = haesarja_w(tunn_srj_nimi, false);
				}
	   kaynnistysvaihe = 2;
		for (nc = 0; nc < NREGNLY; nc++) {
			if (ajanottofl > 0 || regnly[nc]) {
				inittimer();
				break;
				}
			}
	   if (maxyhteys == 0) {
		  for (nc = 0; nc < MAX_LAHPORTTI; nc++) {
			 if (portparam[nc] != 0 && comtype[nc] != comtpTCPLOKI)
				maxyhteys = nc + 1;
			 }
		  }
//	   if (!maxyhteys)
//		  maxyhteys = 1;
	   EnterCriticalSection(&inb_CriticalSection);
	   for (nc = 0; nc < MAX_LAHPORTTI; nc++) {
#ifdef SIIMPORT
			if (nc == siimport)
				continue;
#endif
			if (nc >= maxyhteys && comtype[nc] != comtpTCPLOKI)
				continue;
			if (portparam[nc] != 0) {
				if (initcom(nc)) {
					writeerror_w(L"Tiedonsiirtotoimintojen k‰ynnistys ei onnistunut.", 0);
					return(1);
					}
				}
			}
	   if (ipparam[MAX_PORTTI].destport) {
		  broadcasts = openportBroadcast(ipparam+MAX_PORTTI);
		  }
	   LeaveCriticalSection(&inb_CriticalSection);
//   if (!comfl) lahtarfl = FALSE;
	   kaynnistysvaihe = 3;
#if defined(TV1)
		if (tv_com || tv_test)
			opentv();
#endif
#if defined(GAZ)
		if (gaz_com)
			opengaz();
#endif
#ifdef AUTOFILE
		if (autofileparam.afvali) {
			startaf(0);
			}
#endif
#ifdef AUTOVA
//		if (autova_fl)
		if (vaajat)
			start_autova();
#endif
#if defined(TAULU_2)
		if (taulu_com)
			opentaulu();
#endif
	   }

   taustaon = 1;

	if (loki) {
		wchar_t lmsg[60];
		swprintf(lmsg, L"Ohjelma k‰ynnistetty, klo %s", wkello());
		wkirjloki(lmsg);
		}

	getWinVersion();

#ifdef _CONSOLE
   initscreens();
   if (emitfl == 1) {
	   ProsEmitOn = true;
	   }
   taustaThread = _beginthread(taustaThr, 40960, NULL);
#endif

	close_err_file();
	kaynnistysvaihe = 4;
	if (testi == 2)
		ReadComTestParams();
	return(0);
}

void lopetus1(void);

void lopetus(void)
{
#ifdef _CONSOLE
	lopetus1();
#else
	SuljeKaikki();
#endif

}

void lopetus1(void)
	{

   // kaynnistysvaihe = 0    Ennen kaikkia initialiaointeja
	//                   1    Kun sarjat luettu ja EMS initialisoitu
   //                   2    KILP.DAT luettu kokonaan ja indeksoitu
   //                   3    ajanotto ja tiedonsiirto k‰ynnistetty
   //                   4    kaikki initialisoinnit tehty

	lkmThreadOn = 0;
	inkirjSQLThread = false;

	if (inLopetus)
		return;
	if (loki) {
		wchar_t lmsg[60];
		swprintf(lmsg, L"Ohjelmasta poistutaan, klo %s", wkello());
		wkirjloki(lmsg);
		}
	inLopetus = 1;
	Sleep(200);
#ifdef EMIT
	for (int i = 0; i < NREGNLY; i++)
		regnly[i] = 0;
	ProsEmitOn = false;
#endif
   taustaon = 0;
	if (lsttulparam.lstf) {
		closeprfile(lsttulparam.lstf);
	  }
	if (kaynnistysvaihe > 2) {
		close_sakot();
#ifdef TV1
		if (tv_com)
			closeport_x(cn_tv);
#endif
#ifdef TAULU_2
		if (taulu_com)
			closeport_x(cn_taulu);
#endif
#if defined(SEURAVAL) && defined(_CONSOLExxx)
	  kirj_seurat();
#endif
		}
	if (kaynnistysvaihe > 1) {
		 suljeajat(-1, true);
#ifdef PRACK
	  rem_pr();
#endif
	  closecom();
	  remtimer();
#ifdef DBGFILE
	if (dbgtofile)
		 closedbgfile();
#endif
#if defined(EMIT) && defined(MAALI)
		if (femitfile) {
			fclose(femitfile);
			}
#endif
		closefile(&datf2);
	  }
	if (kaynnistysvaihe > 0) {
		if (lokitulparam.lstf) {
			closeprfile(lokitulparam.lstf);
			lokitulparam.lstf = NULL;
		  }
		if (lokiTCP) {
			closeportTCP(&hLoki);
			}
		}
	loki = 0;
#ifdef EMITHTTP
	while (inhttpHaku)
		Sleep(200);
#endif
//   vapautaemitmuisti(true, true, true);
/*
	DeleteCriticalSection(&seur_badge_CriticalSection);
	DeleteCriticalSection(&seura_CriticalSection);
   DeleteCriticalSection(&autotul_CriticalSection);
   DeleteCriticalSection(&print_CriticalSection);
   DeleteCriticalSection(&tcpip_CriticalSection);
	DeleteCriticalSection(&loki_CriticalSection);
   DeleteCriticalSection(&inb_CriticalSection);
   DeleteCriticalSection(&outb_CriticalSection);
	DeleteCriticalSection(&emit_CriticalSection);
	DeleteCriticalSection(&rastiva_CriticalSection);
   DeleteCriticalSection(&ajat_CriticalSection);
   DeleteCriticalSection(&kilpdat_CriticalSection);
   DeleteCriticalSection(&tall_CriticalSection);
#ifdef TCPSIIRTO
	if (fileyht >= MAX_LAHPORTTI && kaynnistysvaihe > 1) {
		DeleteCriticalSection(&tcp_CriticalSection[fileyht]);
		}
#endif
*/
   cleanWSA();
   Sleep(200);
#ifdef _CONSOLE
   _exit(0);
#endif
	}

