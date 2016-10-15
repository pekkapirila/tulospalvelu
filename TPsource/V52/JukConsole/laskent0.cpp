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
#include <bvideo.h>
#include <bkeybrd.h>
#include <bstrings.h>

#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>

#include "VDeclare.h"
#pragma hdrstop

void addtapahtuma(kilptietue *kilp, int osuus, int piste) {}
void paivita_aikanaytto(void) {}
void sendTestLopetus(void) {}

int ExitHandler(DWORD fdwCtrlType)
{
	wchar_t ch = L' ';

	switch (fdwCtrlType)
	{
		// CTRL-CLOSE: confirm that the user wants to exit. 
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:
		lopetus();
		return(TRUE);
	default:
		return FALSE;
	}
}

void writeerror_w(wchar_t *txt, int wait, bool vainok)
{
	writeerror_w(txt, wait);
}

void vidint2(int y, int x, int l, long i)
   {
   if (i > 999) {
      viwrrect(y, x, y, x, "*", 7, 0, 0);
      i = i % 100;
      x++;
      l--;
      }
   vidint(y, x, l, i);
   }

char *poista_viiva(char *stout, char *stin, int size)
{
	char *pi, *po;

	memset(stout, 0, size);
	for (pi = stin, po = stout; *pi && po < stout + size - 1; pi++, po++) {
		if (*pi == '_')
			*po = ' ';
		else
			*po = *pi;
		}
	return(stout);
}

int inputosuus(int *osuus, int sarja, int len, int x, int y, wchar_t *opts, wchar_t *tc)
{
	wchar_t osst[6];

	if (sarja < 0)
		sarja = 1;
	wosuuskoodi(sarja, *osuus, osst, len);
	elimwbl(osst);
	if (kilpparam.maxnosuus == 1) {
		if (kilpparam.ntosuus < 10)
			len = 1;
		else if (kilpparam.ntosuus < 100)
			len = 2;
		}
	inputwstr(osst, len, x+3-len, y, opts, tc, 0);
	*osuus = tulkOsuuskoodi(sarja, osst);
	return(*osuus);
}

void lukumaarat(void)
   {
   int yb, fg, bg, nc,nc1,os,ntulostt,nkesktt,nhyltt,neiltt,navointt,rl = 1;
   char ln[40], as[20];
   int yhtlkm = 0, yhtavattu = 0, jonossa = 0, jonoja = 0;

#if defined(EMITLEIMAT) && !defined(LUENTA)
   if (ajat_on) aikaruutu(0);
#endif
   if (!status_on) return;
   draw_hline2(16,66,14);
   if (comfl) {
      viwrrect(17,66,17,78," TIED.SIIRTO ",7,0,0);
      if (ySize <= 25)
         {
         viwrrect(18,66,20,71," Saap:"
                              " Läht:"
                              " Yht.:",7,0,0);
         draw_vline2(17, 65, 4);
         vidint(17,78,2, dcom+1);
         for (nc1 = 0; nc1 < 3 && nc1 < maxyhteys; nc1++) {
            nc = (nc1+dcom) % maxyhteys;
            if (!comopen[nc]) continue;
            if (levypusk) {
               vidint2(19,71+3*nc1,3, lahettamatta(nc) + lah_alku[nc]);
               }
            else {
               vidint(19,71+3*nc1,3,
                  (int) ((cjseur[nc] + OUTBUFL - cjens[nc]) % OUTBUFL) +
                  lah_alku[nc]);
               }
            }
         }
      else {
         viwrrect(18,66,18,71," Saap:",7,0,0);
         for (nc1 = 0; nc1 < ySize-24; nc1++) {
            if (nc1 >= maxyhteys)
               break;
            if (maxyhteys > ySize-24)
               nc = (nc1+dcom) % maxyhteys;
            else
               nc = nc1;
            vidint(19+nc1, 67, 2, nc+1); 
            vidint(19+nc1, 71, 5, lahettamatta(nc) + lah_alku[nc]); 
            }
         }
		for (nc = 0; nc < maxyhteys; nc++) {
			if (comopen[nc] && lahcom[nc]) {
				yhtlkm++;
				jonossa += lahettamatta(nc);
				if (lahettamatta(nc) >= jonokynnys)
					jonoja++;
				if (yhteys_on[nc] > 0)
					yhtavattu++;
				}
			}
		lahetaMonitorille(yhtlkm, yhtavattu, jonossa, jonoja);
      }
#if !defined(LUENTA)
   if (aikajono[akttm] && aikajono[akttm]->aktrows > 0 ) {
	   aikatp ttm;
      vidint(ySize-1,54,4,aikajono[akttm]->getAktTime(&ttm, aikajono[akttm]->aktrows-1)->kno);
      VIDSPAIKA(ySize-1,60,ttm.t,t0);
      vidint(ySize-1,71,4,aikajono[akttm]->aktrows);
      vidint(ySize-1,76,4,aikajono[akttm]->rtime+1);
      }
#endif
   draw_vline2(0,65,21);
   draw_grchar2(1,65,14);
   draw_grchar2(7,65,11);
   draw_grchar2(16,65,11);
   draw_grchar2(21,65,7);
   if (ySize == 50) {
      draw_vline2(21,65,25);
      draw_grchar2(46,65,7);
      }
   draw_hline2(1,60,5);
   draw_hline2(7,76,4);
#ifdef EMIT
   if (emitfl) {              
      rl = 0;   
      viwrrect(6,66,6,75," Esitys   ",7,0,0);
      }
#endif
   viwrrect(rl,66,5+rl,75," Ilmoitt. "
                      " Tulos    "
                      " Keskeytt."
                      " Hylätty  "
                      " Ei läht. "
                      " Avoinna  ",7,0,0);
   viwrrect(8,66,9,75," TULOSTUS "
                      " Tausta   ",7,0,0);
   draw_hline2(7,66,10);
   viwrrect(12,66,12,75," Auto     ",7,0,0);
   if (spoolfl) {
      viwrrect(9,76,9,79,"  ON",7,0,0);
      viwrrect(10,66,11,75," Tilaa    "
                           " Jono     ",7,0,0);
	  vidint(10,76,4,maxnblock);
      }
   else 
      viwrrect(9,76,9,79,"  EI",7,0,0);
   if (autofl)
      viwrrect(12,76,12,79,"  ON",7,0,0);
   else
	  viwrrect(12,76,12,79,"  EI",7,0,0);
#if defined(EMIT) && !defined(KUUL)
   if (emitfl) {    
      vidint(6,76,4,nesitt);
      }
#endif
    ntulostt = 0;
    nkesktt = 0;
    nhyltt = 0;
    neiltt = 0;
	navointt = 0;
#ifdef PARTIO
   if (kilpparam.partio) {
      int srj;

      for (srj = 0; srj < sarjaluku; srj++) {
         os = Sarjat[srj].osuusluku-1;
         ntulostt += ntulos[srj][os][0];
         nkesktt += nkesk[srj][os];
         nhyltt += nhyl[srj][os];
         neiltt += neil[srj][os];
         navointt += navoin[srj][os];
         }
      vidint(rl,75,5,nilmt);
      vidint(rl+1,75,5,ntulostt);
      vidint(rl+2,76,4,nkesktt);
      vidint(rl+3,75,5,nhyltt);
      vidint(rl+4,75,5,neiltt);
      vidint(rl+5,75,5,navointt);
      }
   else
#endif
      {    
      if (lkm_os) {
         vidint(rl,76,4,nilm[lkm_srj]);
         vidint(rl+1,76,4,ntulos[lkm_srj][lkm_os-1][0]);
         vidint(rl+2,76,4,nkesk[lkm_srj][lkm_os-1]);
         vidint(rl+3,76,4,nhyl[lkm_srj][lkm_os-1]);
         vidint(rl+4,76,4,neil[lkm_srj][lkm_os-1]);
         vidint(rl+5,76,4,navoin[lkm_srj][lkm_os-1]);
         }
      else {
         for (os=0; os<MAXOSUUSLUKU; os++) {
            ntulostt += ntulost[os];
            nkesktt += nkeskt[os];
			nhyltt += nhylt[os];
            neiltt += neilt[os];
			navointt += navoint[os];
            }
         vidint(rl,75,5,nilmtt);
         vidint(rl+1,75,5,ntulostt);
         vidint(rl+2,76,4,nkesktt);
         vidint(rl+3,75,5,nhyltt);
         vidint(rl+4,75,5,neiltt);
         vidint(rl+5,75,5,navointt);
         }
      }
	nc = 0;
	for (nc1 = 0; nc1 < NREGNLY; nc1++) {
		if (regnly[nc1] > 0)
			nc++;
		}
	if (nc > 10)
		nc = 10;
	if (nc) {
		yb = ySize-5-nc;
	    viwrrect(yb,66, yb, 79, " LUKIJA/KELLO ", 7,0,0);
		for (nc1 = 0; nc1 < NREGNLY; nc1++) {
			if (regnly[nc1] > 0) {
				yb++;
				sprintf(ln, " %2d %8.8s  ", nc1+1, aikatostr_ls(as, regnlyhetki[nc1], t0));
				if ((comtype[MAX_LAHPORTTI+nc1] & comtpTCP) && !TCPyht_on(hComm[MAX_LAHPORTTI+nc1])) {
					fg = 0;
					bg = 7;
					}
				else {
					fg = 7;
					bg = 0;
					}
				viwrrect(yb,66, yb, 79, ln, fg,bg,0);
				}
			}
		}
    if (autofl)
       tulaset_show();
   }

void kehys(int kl)
   {
   draw_hline2(1,0,80);
   draw_hline2(ySize-4, 0, 80);
   draw_hline2(ySize-2, 0, 80);
   if (kl) {
	  lukumaarat();
      }
   if (header[0])
      vidspwmsg(0,0,7,0,header);

   }

void copydfile(datafile *datfb, int oldfile)
   {
   char *buf;
   char ch;
   int rr;
   DWORD nread,nwrit;
   long ll;
   char *ptr, *buflim, prs[82];
   INT16 *iptr;
   extern scr backupohje;
	char *dfnm = datafbname;
   HANDLE thandle;
	static char backfname[50] = "KILP.BAK";

	if ((buf = (char *) malloc(CBUFL)) == 0) {
      writeerror("Muisti ei riitä kopioinnille", 0);
      return;
      }
	memset(buf, 0, CBUFL);

   if( oldfile == 1) {

      header = L"BACK-UP";
      scpclr();
      kehys(1);
      prscr(&backupohje);
		vidspmsg(ySize-3,0,7,0,"Backup-tiedoston nimi, Esc: Peruuta");
		inputstr(backfname, 42, 36, ySize-3, "\r\x1b", &ch, 0);
	  tcomfl = FALSE;
	  if (ch == ESC) {
		 free(buf);
		 return;
		 }
		dfnm = backfname;
	  tcomfl = TRUE;
	  clrln(ySize-3);
	  EnterCriticalSection(&tall_CriticalSection);
	  }
   else {
      EnterCriticalSection(&tall_CriticalSection);
      CloseHandle(datfb->hDatf);
      dfnm = datfb->flnm;
      }
	thandle = CreateFileA(dfnm, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
   rr = 0;

	  iptr = (INT16 *) buf;
	  iptr[0] = datf2.firstfree;
	  iptr[1] = datf2.numberfree;
	  iptr[2] = 0;
	  iptr[3] = datf2.numrec;
	  memmove((char *)iptr+8, (char *) maaliajat, 10*sizeof(INT32));
	  ll = datf2.numrec - 1;
	  rr = 1;
	  nread = KILPRECSIZE;
	  buflim = buf + CBUFL - KILPRECSIZE;
	  ptr = buf + KILPRECSIZE;
   do {
		 while (ptr < buflim && ll > 0) {
			GETREC((kilptietue *) ptr, rr++);
			ptr += KILPRECSIZE;
			nread += KILPRECSIZE;
			ll--;
			}
	  WriteFile(thandle, buf, nread, &nwrit, NULL);
	  if (nwrit != nread) {
		 ioerr(dfnm,2,(INT32) nwrit);
		 break;
		 }
	  ptr = buf;
	  nread = 0;
	  sprintf(prs,"%4d tietuetta kopioitu", rr);
	  vidspmsg(ySize-1,9,7,0,prs);
	  } while (ll > 0);
   clrln(ySize-1);
   if (oldfile != 2)
	  CloseHandle(thandle);
   else datfb->hDatf = thandle;
   LeaveCriticalSection(&tall_CriticalSection);
   free(buf);
   }

void kaynnistatesti(int kielto);

void asetukset()
   {
   int m, l, r, srj, os, ylm;
   int varfhandle;
   static char varfname[] = "VAROKYNN.TAU";
   char ch, tc, *ylmsg;
   static int tarfl0 = FALSE;
   extern scr asetusvalikko;
   extern int errno;
   extern int reconnectSirit[NREGNLY];
   //extern int siritopen[NREGNLY];
   char msg[80], opts[MAX_PORTTI + 2], *p;
   TMAALI tl[MAXSARJALUKU], tl2[MAXSARJALUKU], ylk[MAXSARJALUKU];
   char *spawnargs[2] = {NULL, NULL};
   int rk = 2;
   for (l=2; l<ySize-4; l++) clrtxt(l,0,64);
   ch = ' ';
   header = L"ASETUKSET";
   clrscr();
   kehys(1);
   prscr(&asetusvalikko);
   selectopt("T)arrat, A)jat, V)ar.-kynnys, L)ähdöt, U)DP, D)os, loK)i, "
      "<Esc>: Poistu", "TAVLUDKC\x1B",&ch);
   clrln(ySize-3);
   switch (ch) {
	  case 'C' :
		  kaynnistatesti(0);
		  break;
      case 'T' : ch = ' ';
                 selectopt("K)eskeytä, L)ähetä toiselle PC:lle, T)ulosta"
                    ", E)i-lähteneet","KLTE", &ch);
                 if (tarfl) tarfl0 = TRUE;
                 switch (ch) {
                    case 'K' :
                       tarfl = FALSE;

                       lahtarfl = FALSE;
                       break;
                    case 'L' :
                       if (comfl && (lahcom[0] || lahcom[1] ||
                          lahcom[MAX_PORTTI-1])) {
                          lahtarfl = TRUE;
                          tarfl = FALSE;
                          tarfl = FALSE;
                          clrln(ySize-3);
                          viwrrect(ySize-3,0,ySize-3,39,"Luettele tarralähetyksen "
                             "tiedonsiirrot: ",7,0,0);
						  p = msg;
						  for (l = 0; l < MAX_PORTTI; l++) {
							 if (tarlahcom[l]) *(p++) = l + '1';
							 }
						  *p = 0;
						  inputstr(msg,MAX_PORTTI,40,ySize-3,"\r",&ch,TRUE);
						  memset(tarlahcom, 0, sizeof(tarlahcom));
						  for (p = msg; *p; p++) {
							 if (*p > '0' && *p < '1' + MAX_PORTTI)
								tarlahcom[*p - '1'] = 1;
							 }
						  }
					   else writeerror("Tiedonsiirto ei käytössä",0);
					   break;
					case 'T' :
					   break;
                    case 'E' :
                       ch = ' ';
                       selectopt("T)arrat ei-lähteneille,   "
                          "E)i tarroja", "TE", &ch);
                       eiltarra = (ch == 'T');
                       break;
                    }
                 break;
      case 'V' : for (r = 2; r < ySize-4; r++) clrtxt(r,0,64);
                 if ( (varfhandle = _open(varfname, O_RDWR | O_BINARY |
                    O_CREAT, S_IREAD | S_IWRITE)) >= 0 ) {
                    if (_filelength(varfhandle) > 0) {
                       _lseek(varfhandle,0,0);
                       ch = ' ';
                       selectopt("Luetaanko aiemmat kynnykset levyltä (K/E)"
                          , "KE", &ch);

                       if (ch == 'K') _read(varfhandle, (char *)
                          varkynnys[0], 2*MAXOSUUSLUKU*sarjaluku);
                       }
                    }
                 _close(varfhandle);
                 vidspmsg(2,0,7,0,"Varoituskynnyksen alittavista "
                    "osuusajoista huomautetaan");
                 vidspmsg(3,20,7,0,"Ajat minuutteina");
                 for (srj = 0; srj < sarjaluku && srj < ySize-8; srj++) {
                    vidspmsg(srj+4, 0, 7, 0, (char *)Sarjat[srj].sarjanimi);
                    for (os = 0; os < Sarjat[srj].osuusluku; os++)
                    vidint(srj+4, 15+6*os, 3, (int)(varkynnys[srj][os] /
                       MINUUTTI));
                    }
                 vidspmsg(ySize-3,0,7,0,"<Tab>, <Shift-Tab>, \x19, \x18 : "
                    "Siirry,  + : Hyväksy");
                 srj = 0;
                 os = 0;
                 do {
                    l = (int) (varkynnys[srj][os] / MINUUTTI);
                    r = min(srj + 4, ySize-5);
                    if (srj > ySize-10) vidspmsg(r, 0, 7, 0, (char *)Sarjat[srj].sarjanimi);
                    INPUTINT(&l, 3, 15+6*os, r,
                       "+\x05\x09\x0B\x0D\x18\xCD\xD1", &ch);
                    varkynnys[srj][os] = (unsigned) (MINUUTTI * l);
                    switch (ch) {
                       case KB_CR      :
                       case KB_TAB     : os++;
                                      break;
                       case BACKTAB : os--;
                                      break;
					   case DOWN    :
                       case 210     : srj = (srj + 1) % sarjaluku;
                                      break;
                       case UP      :

                       case 206     : srj--;
                                      if (srj < 0) srj = sarjaluku-1;
                                      break;
                       }
                    if (os >= Sarjat[srj].osuusluku) {
                       if (ch == KB_CR) srj++;
                       os = 0;
                       }
                    if (os < 0) os = Sarjat[srj].osuusluku-1;
                    } while (ch != '+');
                 ch = ' ';
                 selectopt("Tallennetaanko kynnykset levylle (K/E)","KE",
                    &ch);
                 clrln(ySize-3);
                 if (ch == 'K') {
                    varfhandle = _creat(varfname, S_IREAD | S_IWRITE);
                    if (_write(varfhandle, (char *) varkynnys[0],
                       2*MAXOSUUSLUKU*sarjaluku) != 2*MAXOSUUSLUKU*
                          sarjaluku)

                       writeerror("Virhe kynnysten tallentamisessa",
                          2000);
                    _close(varfhandle);
                    }
                 break;
      case 'L' :
         ch = ' ';
         selectopt("L)ähtöajat vai Y)hteislähtö     Esc  : Peruuta",
            "LY\x1B", &ch);
         switch (ch) {
            case 'L' :
               if (sarjaluku == 1) {
                  clrln(ySize-3);
                  viwrrect(ySize-3,0,ySize-3,16,"Anna lähtöaika : ",7,0,0);
                  tl[0] = Sarjat[0].lahto;
				  INPUTAIKA(&tl[0],t0,8,18,ySize-3,"\x0D\x1B",&ch);
                  if (ch == KB_CR) ch = '+';
                  }
               else {

                  for (l=2; l<ySize-4; l++) clrtxt(l,0,64);
                  for (l = 0; l < sarjaluku; l++) {
                     r = l % (ySize-6) + 3;
                     m = (l / (ySize-6)) * 21;
                     vidspmsg(r, m, 7 , 0, (char *)Sarjat[l].sarjanimi);
                     tl[l] = Sarjat[l].lahto;
                     VIDSPAIKA(r, m+10 , tl[l],t0);
                     }
                  vidspmsg(ySize-3,0,7,0, "\x19\x18 : Siirry, + : Hyväksy "
                     "kaikki, Esc : Peruuta muutokset");
                  for (l = 0; ; ) {
                     r = l % (ySize-6) + 3;
                     m = (l / (ySize-6)) * 21 + 10;
                     INPUTAIKA(tl+l, t0, 8, m, r, "+\r\x18\x05\x1b", &ch);
                     if (ch == '+' || ch == ESC) break;
                     if (ch == '\r' || ch == DOWN)
                        l = (l+1) % sarjaluku;
                     else
                        l = (l+sarjaluku-1) % sarjaluku;
                     }
                  }
               if (ch == '+') {
                  m = 0;
                  for (l = 0; l < sarjaluku; l++) {
                     if (tl[l] != Sarjat[l].lahto) {
                        Sarjat[l].lahto = tl[l];
                        laheta_yl(-l-1, 0);
                        m = 1;
						if (kilpparam.maxnosuus == 1)
	                        paivjarjarr(l);
                        }
                     }
                  if (m) 
						kirjEventXml(sarjaxmlfname, true, false, false);
                  }
               break;
            case 'Y' :
               scpclr();
               kehys(1);
			   for (srj = 0; srj < sarjaluku; srj++) {
				   tl2[srj] = Sarjat[srj].ylahto[Sarjat[srj].osuusluku-1]; 
				   tl[srj] = Sarjat[srj].ylahto[0]; 
				   ylk[srj] = Sarjat[srj].ylkynnys[Sarjat[srj].osuusluku-1]; 
				   }
			   ylm = ylmyoh;
               l = 0;
               do {
                  for (r=2; r<21; r++) clrtxt(r,0,64);
                  vidspmsg(2,0,7,0,
                     "Sarja     Yhteislähtö Vaihto kiinni");
                  ylmsg =

                     "Yhteislähtö merkitään au-"
                     "tomaattisesti, jos vaih- "
                     "don sulkemisaika annettu "
                     "Kumoa 'Y' saman näppäimen"
                     "toistolla.               "
                     "                         "
                     " P : Poista sulkemisaika "
                     " C : Kopioi ensimmäisen  "
                     "     sarjan tiedot kai-  "
                     "     kille sarjoille     "

                     " Y : Käsinmerkintä yksi  "
                     "     osuus kerrallaan (ei"
                     "     automaattisesti myö-"
                     "     hemmille osuuksille)"
                     " S : Seuraava sivu       "
                     " + : Hyväksy valinnat    "
                     "                         "
                     "Käsinmerk yksi os:       ";
                  viwrrect(2,40,19,64,ylmsg,7,0,0);
                  viwrrect(19,60,19,64,ylm ? "Ei   " : "Kyllä",7,0,0);
                  paivita:
                  for (srj = l; srj < sarjaluku && srj < (l+ySize-7)/rk; srj++) {
                     r = rk*srj - l + 3;

					 vidspmsg(r, 0, 7 , 0, (char *)Sarjat[srj].sarjanimi);
                     VIDSPAIKA(r, 12 , tl[srj],t0);
					 vidspmsg(r+1, 0, 7, 0, "- ankkurit");
                     VIDSPAIKA(r+1, 12 , tl2[srj],t0);
                     if (ylk[srj] == TMAALI0)
                        vidspmsg(r,22,7,0, "Ei automaattinen");
                     else {
                        vidspmsg(r,22,7,0, "                ");
                        VIDSPAIKA(r, 24 , ylk[srj],t0);
                        }
                     }
                  vidspmsg(ySize-3,0,7,0, "\x19\x18 : Siirry, + : Hyväksy "
                     "kaikki, P: Poista, Esc : Peruuta muutokset");
                  for (srj = l; ; ) {
					 r = rk*srj - l + 3;

                     INPUTAIKA(tl+srj, t0, 8, 12, r,
                        "pPcCyYsS+\r\x18\x05\x09\x1b", &ch);
					 if (ch == '\r')
	                     INPUTAIKA(tl2+srj, t0, 8, 12, r+1,
		                    "pPcCyYsS+\r\x18\x05\x09\x1b", &ch);
					 if (ch == '\r') {
                        vidspmsg(r,22,7,0, "                ");
                        INPUTAIKA(ylk+srj, t0, 8, 24, r,
                           "pPcCyYsS+\r\x18\x05\x09\x1b", &ch);
                        }

                     ch = toupper(ch);
                     if (ch == '+' || ch == ESC) break;
                     switch (ch) {
                        case 'P' :
                           ylk[srj] = TMAALI0;
                           vidspmsg(r,22,7,0, "Ei automaattinen");
                        case '\r':
                        case DOWN:
                           srj = (srj+1) % sarjaluku;
                           break;
                        case  UP :
                           srj = (srj+sarjaluku-1) % sarjaluku;
                           break;
                        case 'S' :
                           if (sarjaluku > l + ySize-8) l += ySize-7;
                           else l = 0;
                           srj = l;
                           break;
                        case 'Y' :
                           ylm = !ylm;
                           viwrrect(ySize-6,60,ySize-6,64,
                              ylm ? "Ei   " : "Kyllä",7,0,0);
                           break;
                        case 'C' :
                           for (m = 1; m < sarjaluku; m++) {
                              tl[m] = tl[0];
                              ylk[m] = ylk[0];

                              }
                           goto paivita;
                        }
                     if (rk*srj >= l + ySize-7) {
                        l += ySize-7;
                        break;                                    
						}
                     if (rk*srj < l) {

                        l -= ySize-7;
                        break;
                        }
                     }
                  } while (ch != ESC && ch != '+');
               if (ch == ESC) break;
               m = 0;
               ylmyoh = ylm;
               for (srj = 0; srj < sarjaluku; srj++) {
                  if (Sarjat[srj].ylahto[0] != tl[srj] || Sarjat[srj].ylkynnys[0] != ylk[srj]
				     || Sarjat[srj].ylahto[Sarjat[srj].osuusluku-1] != tl2[srj]
					 ) {
					 for (os = 0; os < MAXOSUUSLUKU; os++) {
						Sarjat[srj].ylahto[os] = tl[srj];
						Sarjat[srj].ylkynnys[os] = ylk[srj];
						}
 					 Sarjat[srj].ylahto[Sarjat[srj].osuusluku-1] = tl2[srj];
                     laheta_yl(srj, 0);
                     if (Sarjat[srj].ylkynnys[0] != TMAALI0) merk_yht_lahdot(srj, true);
                     m = 1;
                     }
                  }
               if (m) {
				  kirjEventXml(sarjaxmlfname, true, false, false);
                  }
               laheta_yl(0,0);
               break;

            }
         break;
      case 'A' : 
		  ch = ' ';
		  switch (regnly[0]) {
				case 1:
				case 2:
					selectopt("L)ähetys, A)jantarkkuus, M)aalikellon aika", "ALM", &ch);
					break;
				default:
					selectopt("L)ähetys, A)jantarkkuus, M)aalikello, S)irit", "ALMS", &ch);
					break;
				}
			switch (ch) {
				 case 'S':
#ifdef LAJUNEN
					 ch = ' ';
					 selectopt("Käynnistä yhteydet uudelleen (K/E)", "KE", &ch);
					 if (ch == 'K')
						 reconnectSirit[0] = 1;
#endif
					 break;
				 case 'L':
					   ch = 0;
					   memset(msg, 0, sizeof(msg));
					   memset(opts, 0, sizeof(opts));
					   strcpy(msg, "Yhteys");
					   opts[0] = '\x1b';
					   for (m = 0; m < MAX_PORTTI; m++) {
						  if (comopen[m]) {
							 strcat(msg, "  ,");
							 msg[strlen(msg)-2] = m + '1';
							 opts[strlen(opts)] = m + '1';
							 }
						  }
					   if (strlen(opts) < 2)
						  tc = ESC;
					   else if (strlen(opts) == 2)
						  tc = opts[1];

					   else {
						  memmove(msg + strlen(msg), msg + strlen(msg) - 3 , 2);
						  memcpy(msg + strlen(msg) - 6, " vai ", 5);
						  tc = ' ';
						  selectopt(msg, opts, &tc);
						  }
					   if (tc != ESC) {
						  ch = ' ';
						  selectopt("Vahvista lähetys (K/E)", "KE", &ch);
						  if (ch == 'K') lah_kello_nyt[tc - '1'] = 1;
						  }
					   break;
					case 'M':
					   if (regnly[0] == 1 || regnly[0] == 2) {
						  vidspmsg(ySize-1,0,7,0,"Odottaa vastausta maalikellolta");
						  kerro_regnly = 1;
						  while (kerro_regnly &&
							 readkbd(&ch, FALSE, spoolfl) != ESC) {
							 Sleep(50);
							 }
						  kerro_regnly = 0;
						  clrln(ySize-3);
						  viwrrect(ySize-3,0,ySize-3,36,
							 "Anna maalikellon nolla-aika :        ",7,0,0);
						  INPUTAIKA(&t0_regn[0],t0,8,30,ySize-3,"\xD",&ch);
						  maaliajat[9] = t0_regn[0];
						  }
					   if (regnly[0] == 5) aseta_s4();
					   break;
					case 'A':
					   ch = ' ';
					   selectopt("Näytä sekunnin kymmenykset (K/E)", "KE", &ch);
					   kilpparam.laika = ch == 'K' ? 10 : 8;
					   break;
                    }
                 break;
         case 'U':
					  clrln(ySize-3);
					  vidspmsg(ySize-3,0,7,0,"Anna nak-viive");
                 INPUTINT(&nakviive,5,16,ySize-3,"\x0D",&ch);
				 break;
	  case 'D' :
		 break;
	  case 'K' :
		 break;
	  }
   }

void SendCopyThread(LPVOID lpCn);
void lahetatestikaynnistys(INT kielto);

void kaynnistatesti(int kielto)
{
	lahetatestikaynnistys(kielto);
   _beginthread(SendCopyThread, 10240, NULL);
}
