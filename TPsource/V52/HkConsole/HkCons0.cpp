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

#include <map>
#include <stdio.h>
#include <algorithm>
#include <functional>
 using namespace std;
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>
#include "HkDeclare.h"
#include "console.h"

void rem_pr(void);
void kaynnistatesti(int k);

#ifdef SAKKOCOM
	extern INT sak_com;
#endif

int maxEditNo = MAXE;
int valuku_vanha;
int iscurpos;

EDITFLD editfld[] = {
	{L"Nimi :",		0,		7,		7, LSNIMI,	1},
	{L"",			0, LSNIMI+8,	7, LENIMI,	1},
	{L"Arvo :",		0,		7,		8, LARVO,	1},
	{L"Lisenssi :", 0,		11,		9,		8,	1},
	{L"WREkoodi :",	0,		11,		10,		9,	0},
	{L"Kilpailukortti :", 22, 39,	9,		8,	1},
	{L"",			0,		50,		9,		5,	1},
	{L"Seura :",	0,		8,		11, LSEURA, 1},
	{L"Seuralyh :", 0,		11,		12,	 LLYH,	0},
	{L"Alue :",		45,		52,		11,		2,	1},
	{L"Maa :",		56,		62,		11,		3,	1},
	{L"Yhdistys :", 37,		48,		12,	 LYHD,	0},
	{L"",			0,		5,		16,		10, 1},
	{L"",			0,		17,		16,		10, 1},
	{L"",			0,		30,		16,		5,	1},
	{L"",			0,		36,		16,		10, 1},
	{L"",			0,		48,		16,		3,	1},
	{L"",			0,		56,		16,		3,	1},
	{L"",			0,		17,		20,		10, 1},
	{L"Sarja :",	0,		8,		2, LSARJA,	1},
	{L"Kilpno : ",	0,		8,		4,		4,	1},
	{L"Maaliaika :", 42,	54,		3,		11, 1},
	{L"Tot.lähtö :", 42,	54,		4,		11, 1},
	{L"Tasoitus/sakko :", 37, 54,	6,		4,	0},
	{L"Amp.sakot :", 42,	54,		5,		1,	0},
	{L"Rata :",		46,		54,		5,		8,	0},
	{L"Erä :",		32,		38,		5,		2,	0}
};

void addtapahtuma(kilptietue *kilp, int piste) { }

void paivita_aikanaytto(void) { }

void add_sakko(int d) { }

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

bool kilptietue::kirjoitaTiedostoon(void)
{
	return(true);
}

void kilptietue::TallEhdoin(int pos)
{
	int d;

	addtall(&d, 0);
}

void kilptietue::SiirraPvEhdoin(kilppvtp *Pv, int ipv)
{
	if (ipv >= 0 && ipv < kilpparam.n_pv_akt)
		pv[ipv] = *Pv;
}

void kirjKilpSQL(int d)
{
}

void WRITEYHT(wchar_t *st, INT cn)
   {
   INT r, y;

//   if (!status_on || comtype[cn] == 2)
   if (!status_on)
      return;
   y = (maxyhteys+cn-dcom)%maxyhteys;
   if (ySize <= 25) {
      if (y < 3)
         viwrrectw(20,72+3*y,20,73+3*y,(st),7,0,0);
      }
   else {
      if ((r = y) < ySize - 23) {
         if (maxyhteys < ySize-23)
            r = cn;
         viwrrectw(19+r, 78, 19+r, 79, st ,7,0,0);
         }
      }
   if (server_on) {
      if (cn >= yhteysalku && cn < yhteysalku+ySize-7) 
         viwrrectw(cn-yhteysalku+3, 43,cn-yhteysalku+3, 44, st, 7, 0, 0);
      }
   }

void DSPQ(INT r, INT cn) 
   {
   INT nj, y;

   if (!status_on)
      return;
   nj = lahettamatta(cn);
   if (!levypusk) nj = (nj + OUTBUFL) % OUTBUFL;
   nj += lah_alku[cn];
   y = (maxyhteys+cn-dcom)%maxyhteys;
   if (ySize <= 25) {
      if (y < 3) {
         vidint((r),71+3*y,3,nj%1000);
         if (nj > 999)
            vidspmsg(r, 71+3*y, 7, 0, "*");
         }
      }
   else {
      if ((r = y) < ySize - 23) {
         if (maxyhteys < ySize-23)
            r = cn;
         vidint(19+r, 66, 3, cn+1); 
         vidint(19+r, 70, 5, nj); 
         }
      }
   if (server_on) {
      if (cn >= yhteysalku && cn < yhteysalku+ySize-7) {
         vidint(cn-yhteysalku+3, 47, 5, nj);
         vidint(cn-yhteysalku+3, 56, 5, cjens[cn]);
         }
      }
   }

void DSPIQ(int nj)
   {
   vidint(18, 75, 5, nj);
   }

void lukumaarat()
{
	INT nc,nc1,fg,bg;
	aikatp pt;
	char ln[15], as[20];
	int yb;
	int yhtlkm = 0, yhtavattu = 0, jonossa = 0, jonoja = 0;

	if (comfl && monitoriLkm > 0) {
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
	if (!status_on) {
		aikaruutu();
		return;
		}
	else
		viwrrect(0,65,6,79,(char *)kehys1,7,0,0);
	if (lukum_fl) {
		vidint(LUKUMY,LUKUMX,4,nilmt);
		vidint(LUKUMY+1,LUKUMX,4,ntulost);
		vidint(LUKUMY+2,LUKUMX,4,nkeskt);
		vidint(LUKUMY+3,LUKUMX,4,nhylt);
		if (emitfl)
			vidint(LUKUMY+6,LUKUMX,4,nesitt);
		vidint(LUKUMY+4,LUKUMX,4,neilt);
		vidint(LUKUMY+5,LUKUMX,4,nilmt-ntulost-nkeskt-nhylt-neilt);
		}
	else if (lajat_p && aikajono[1] && aikajono[1]->aktrows > 0) {
		for (nc = 0; nc < 7; nc++) {
			if (aikajono[1]->aktrows > nc) {
				aikajono[1]->getAktTime(&pt, aikajono[1]->aktrows - nc - 1);
				sprintf(ln, "%4d %8.8s", pt.kno,
				AIKATOSTR(purajak(pt.t), t0));
				viwrrect(6-nc, 67, 6-nc, 79, ln, 7, 0, 0);
				}
			else
				clrtxt(6-nc, 67, 79);
			}
		}
	if (comfl) {
		if (ySize <= 25)
			{
			vidint(17,78,2, dcom+1);
			for (nc1 = 0; nc1 < 3 && nc1 < maxyhteys; nc1++) {
				nc = (nc1+dcom) % maxyhteys;
				if (!comopen[nc]) 
					continue;
				if (levypusk) {
					vidint(19,71+3*nc1,3, lahettamatta(nc) + lah_alku[nc]);
					}
				else {
					vidint(19,71+3*nc1,3,
						(int) ((cjseur[nc] + OUTBUFL - cjens[nc]) % OUTBUFL) +
						lah_alku[nc]);
					}
				}
			}
		else {
			yb = 18;
			vidint(yb-1,78,2, dcom+1);
			viwrrect(yb,66,yb,76," Saap:     ",7,0,0);
			for (nc1 = 0; nc1 < ySize-24; nc1++) {
				if (yb+nc1+1 < ySize-4)
					viwrrect(yb+nc1+1,66,yb+nc1+1,76,"           ",7,0,0);
				if (nc1 >= maxyhteys)
					continue;
				if (maxyhteys > ySize-yb-6)
					nc = (nc1+dcom) % maxyhteys;
				else
					nc = nc1;
				if (yb+nc1 < ySize-5) {
					sprintf(ln, "%3d%6ld ", nc+1, lahettamatta(nc) + lah_alku[nc]); 
					viwrrect(yb+nc1+1, 66, yb+nc1+1, 75, ln, 7, 0, 0);
					}
				draw_vline2(yb, 65, ySize-yb-4);
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
			}
		}
}

void kehys(INT kl)
{
  draw_hline2(1,0,80);
  draw_hline2(ySize-4,0,80);
  draw_hline2(ySize-2,0,80);
  if (kl) {
     viwrrect(0,65,ySize-4,79,(char *)kehys1,7,0,0);
     lukumaarat();
     }
  vidspwmsg(0,0,7,0,header);
  if (kl)
	  autoruutu();
}

#ifdef _CONSOLE
void copydfile(datafile *datfb, INT oldfile)
{
	wchar_t wch;
	static wchar_t fname[50];

	header = L"BACK-UP";
	clrln(ySize-3);
	vidspwmsg(ySize-3, 0, 7, 0, L"Anna backup-tiedoston nimi");
	inputwstr(fname, 49, 27, ySize-3, L"\r\x1b", &wch, 0);
	if (wch == ESC) {
		return;
		}
	clrln(ySize-3);
	kirjKilpDat(fname);
	vidspwmsg(ySize-5,0,7,0,L"Tiedosto kirjoitettu  ");
	}
#endif


void valiajat(void)
   {
   static INT srj = 0;
   extern scr valiaikaohje;
   INT k, l, jrj[VALUKU];
   INT16 rajat[VALUKU+1];
   INT32 m;
   wchar_t wtc, pisteet[VALUKU+1][6], *p;
   wchar_t wterm[] = L"+\x5\xB\t\xD\x18\x1b";

   for (;;) {
      clearframe63();
      prscr(&valiaikaohje);
      clrln(ySize-3);
      vidspwmsg(ySize-3,0,7,0,L"Anna sarja                     Esc : Poistu");
	  srj = luesarja(NULL, &wtc);
	  if (wtc == ESC || srj < 0) break;
      vidspwmsg(4,3,7,0,Sarjat[srj].sarjanimi);
      vidspwmsg(4,23,7,0,L"Sijainti (km)      Minimiaika");
      vidspwmsg(ySize-3,0,7,0,L" + : Hyväksy,  Esc : Peruuta sarjan muutokset");
      rajat[0] = Sarjat[srj].tul_raja[k_pv];
      memcpy(rajat+1, Sarjat[srj].va_raja[k_pv], kilpparam.valuku*sizeof(rajat[0]));
      wcscpy(pisteet[0],L"Maali");
      memcpy(pisteet+1, Sarjat[srj].va_matka[k_pv], kilpparam.valuku*sizeof(pisteet[0]));
      for (k = 0; k <= Sarjat[srj].valuku[k_pv]; k++) {
         for (l = 0; l < 2; l++)
            {
            vidspwmsg(6+k, 25, 7, 0, pisteet[k]);
            vidspaika(6+k, 43, rajat[k], 0);
            }
         }
      k = 0;
		l = 1;
      do {
         do {
            switch (l) {
               case 0:
                  if (k) {
					 inputwstr(pisteet[k], 5, 25, 6+k, wterm, &wtc, 0);

					 elimwbl(pisteet[k]);
                     for (p = pisteet[k]; *p && *p != L' '; p++) ;
					 if (*p == L' ') *p = 0;
                     }
                  break;
               case 1:
				  inputaika_w(&rajat[k], 0, 8, 43, 6+k, wterm, &wtc);
                  break;
               }
			if (wtc == KB_CR || wtc == KB_TAB || wtc == BACKTAB) {
			   if (!l) wtc = KB_TAB;
			   l = 1-l;
			   }
			 } while (wtc == KB_TAB || wtc == BACKTAB);
		 if (wtc == DOWN || wtc == KB_CR) {
            k++;
            if (k > Sarjat[srj].valuku[k_pv]) k = 0;
            }
		 else if (wtc == UP) {
			k--;
			if (k < 0) k = Sarjat[srj].valuku[k_pv];
			}
		  } while (wtc != L'+' && wtc != ESC);
	  if (wtc == L'+') {
         memset(jrj, 0, sizeof(jrj));
         for (k = 0; k < Sarjat[srj].valuku[k_pv]; k++) {
				if ((m = _wtol(pisteet[k+1])) > 0) {
			   for (l = 0; l < k && jrj[l] &&
                  m > _wtol(pisteet[jrj[l]]); l++) ;
               memmove(jrj+l+1, jrj+l, 2*(Sarjat[srj].valuku[k_pv]-l-1));
               jrj[l] = k+1;
               }
            }
         for (k = 0; k < Sarjat[srj].valuku[k_pv]; k++) {
            Sarjat[srj].va_raja[k_pv][k] = rajat[jrj[k]];
            wcscpy(Sarjat[srj].va_matka[k_pv][k], pisteet[jrj[k]]);
            }
         Sarjat[srj].tul_raja[k_pv] = rajat[0];
         }
      }
   }

#ifdef _CONSOLE
static void valitselahtosarja(void)
	{
	wchar_t ch, msg[20];

   if (tunn_srj == sarjaluku)
      wcscpy(msg, L"kaikki");
   else if (tunn_srj >= 0)
      wcscpy(msg, Sarjat[tunn_srj].sarjanimi);
   else msg[0] = 0;
   clrln(ySize-3);
   viwrrectw(ySize-3,0,ySize-3,18,
      L"Anna lähtevä sarja tai 'kaikki':",7,0,0);
   do {
      inputwstr(msg,10,29,ySize-3,L"\r\x1b",&ch,0);
      if (ch == ESC) break;
      if (!wcscmp(L"KAIKKI",upcasewstr(msg)))
         tunn_srj = sarjaluku;
      else {
         tunn_srj = haesarja_w(msg, false);
         if(tunn_srj < 0)
            writeerror_w(L"Virheellinen sarja",0);
         }
      } while (tunn_srj < 0);
	}
#endif

/*
static void	sprintsiirrot(void)
	{
   wchar_t ch,tc,ce = L'O';
   static INT tarfl0;
   extern INT errno;
   wchar_t msg[80], ln[80] ,s1[40], s2[40], *p;
   static wchar_t ohjefl[41] = L"m-ohje.txt";
   int m1, m2;
   INT l, k, m, lk, r, lj, d, sj, sarja, srj, kars_srj = -1, muod_srj = -1, n_kars_srj = 5;
   INT32 siirto, tls;
   kilptietue kilp, entkilp, kilp1;
   TextFl *ohjefile = NULL;
//	INT sfsarja[5][2] = {(0,0,0,0,1), (0,1,1,1,1)};
	INT sfsarja[5][2] = {{0,0}, {0,0}, {1,0} ,{1,1} ,{1,1}};
	typedef struct {
		INT d;
		INT32 tls;
		} paras_tp;
	paras_tp parhaat[6];

	tc = L' ';
	wselectopt(L"Muodosta S)emifinaalit, F)inaalit, O)hje tiedostosta, Esc: Poistu", L"SFO\x1b", &tc);
	if (tc == ESC)
		return;
	switch (tc) {
		case L'O':
	      clrln(ySize-3);
		   vidspwmsg(ySize-3,0,7,0,L"Ohjetiedoston nimi:");
			inputwstr(ohjefl, 39, 20, ySize-3, L"\r\x1b", &ch, 0);
	      if (ch == ESC)
		      return;
		    ohjefile = new TextFl(ohjefl, L"r");
			if (!ohjefile->IsOpen()) {
				writeerror_w(L"Tiedostoa ei löydy", 0);
				delete ohjefile;
				return;
				}
			break;
		case L'F':
			n_kars_srj = 2;
		case L'S':
		   clrln(ySize-3);
			if (tc == L'S')
				vidspwmsg(ySize-3,0,7,0, L"    Anna sarja :                 ensimmäinen neljännesfinaalisarja");
			else
				vidspwmsg(ySize-3,0,7,0, L"    Anna sarja :                 ensimmäinen semifinaalisarja");
			kars_srj = luesarja(Sarjat[0].sarjanimi, &ch);
		   clrln(ySize-3);
			if (kars_srj+n_kars_srj-1 >= sarjaluku) {
				writeerror_w(L"Virheellinen valinta", 0);
				ch = ESC;
			   }
			if (ch == ESC) break;
			if (tc == L'S')
				vidspwmsg(ySize-3,0,7,0, L"    Anna sarja :                 ensimmäinen semifinaalisarja");
			else
				vidspwmsg(ySize-3,0,7,0, L"  A-finaalisarja :                 Esc ; peruuta");
			muod_srj = luesarja(Sarjat[0].sarjanimi, &ch);
			if (muod_srj+1 >= sarjaluku) {
				writeerror_w(L"Virheellinen valinta", 0);
				ch = ESC;
			   }
		   clrln(ySize-3);
			if (ch == ESC) break;
         vidspwmsg(ySize-3,0,7,0, L"Anna lisäys edellisen kierroksen kilpailijanumeroihin");
         siirto = 0;
         inputlongw(&siirto, 6, 55, ySize-3, L"\r\x1b", &ch);
			if (ch == ESC) break;
			break;
      }
	if (tc == L'F' || tc == L'S') {
		if (ch == ESC)
			return;
		memset(parhaat, 0, sizeof(parhaat));
		for (sarja = kars_srj; sarja < kars_srj+n_kars_srj; sarja++) {
			d = -1;
			lj = -1;
			sj = 0;
			for (;;) {
				while (d < 0 && lj < 2) {
					lj++;
					d = jalku[sarja][lj];
					}
				if (d < 0)
					break;
				m1 = d;
	         d = seuraava(0, 0, d);
				kilp1.GETREC(m1);
				if (kilp1.hyv())
					sj++;
				else
					sj = 0;
				entkilp = kilp1;
				k = kilp1.id() + (INT)siirto;
				if (k >= minkilpno && k <= maxkilpno && getpos(k) < 0)
					 kilp1.setId(k);
				else {
					writeerror_w(L"Numeroiden siirto ei onnistu, toiminta keskeytetty", 0);
					return;
					}
				kilp1.tallenna(m1, 0, 0, 0, &entkilp);
				tls = kilp1.tulos_pv(k_pv);
				kilp.nollaa();
				if (tc == L'F') {
					if (sj > 0 && sj < 4)
						srj = muod_srj;
					else
						srj = muod_srj+1;
					memcpy(kilp.lisno, kilp1.lisno, (int)((char *)&kilp.sarja - (char *)kilp.lisno));
					kilp.setId(kilp1.id()-siirto);
					kilp.sarja = srj;
					kilp.pv[k_pv].tlahto = Sarjat[kilp.sarja].enslahto[k_pv];
					kilp.tall_tulos(-1, kilp.pv[k_pv].tlahto);
					kilp.addtall(&m2, 0);
					if (sj == 3 || sj == 4) {
						parhaat[2*(kilp1.sarja-kars_srj)+sj-3].d = m2;
						parhaat[2*(kilp1.sarja-kars_srj)+sj-3].tls = tls;
						}
					}
				else {
					if (sj == 1 || sj == 2) {
						srj = muod_srj + sfsarja[kilp1.sarja-kars_srj][sj-1];
						memcpy(kilp.lisno, kilp1.lisno, (int)((char *)&kilp.sarja - (char *)kilp.lisno));
						kilp.setId(kilp1.id()-siirto);
						kilp.sarja = srj;
						kilp.pv[k_pv].tlahto = Sarjat[kilp.sarja].enslahto[k_pv];
						kilp.tall_tulos(-1, kilp.pv[k_pv].tlahto);
						kilp.addtall(&m2, 0);
						}
					else if (sj > 2 && sj < 5) {
						for (k = 0; k < 2; k++) {
							if (parhaat[k].d == 0 || tls < parhaat[k].tls) {
								if (k == 0)
									parhaat[1] = parhaat[0];
								parhaat[k].d = m1;
								parhaat[k].tls = tls;
								break;
								}
							}
						}
					}
				}
			}
		if (tc == L'F') {
			m2 = 0;
			if (parhaat[0].tls > parhaat[3].tls) {
				m2 = parhaat[0].d;
				m1 = parhaat[3].d;
				}
			else if (parhaat[2].tls > parhaat[1].tls) {
				m2 = parhaat[2].d;
				m1 = parhaat[1].d;
				}
			if (m2) {
				for (k = 0; k < 2; k++) {
					kilp.GETREC(m1);
					entkilp = kilp;
					kilp.sarja = muod_srj+k;
					kilp.pv[k_pv].tlahto = Sarjat[kilp.sarja].enslahto[k_pv];
					kilp.tall_tulos(-1, kilp.pv[k_pv].tlahto);
					kilp.tallenna(m1, 0, 0, 0, &entkilp);
					m1 = m2;
					}
				}
			}
		else {
			for (k = 0; k < 2; k++) {
				kilp.nollaa();
				if (parhaat[k].d) {
					kilp1.GETREC(parhaat[k].d);
					memcpy(kilp.lisno, kilp1.lisno, (int)((char *)&kilp.sarja - (char *)kilp.lisno));
					kilp.setId(kilp1.id()-siirto);
					kilp.sarja = muod_srj + k;
					kilp.pv[k_pv].tlahto = Sarjat[kilp.sarja].enslahto[k_pv];
					kilp.tall_tulos(-1, kilp.pv[k_pv].tlahto);
					kilp.addtall(&m2, 0);
					}
				}
			}
		return;
		}
	r = 0;
	do {
		lk = 0;
		if (tc == L'O') {
			if (ohjefile->Feof()) {
				break;
				}
			siirto = 0;
			if (ohjefile->ReadLine(ln, 78)) {
				r++;
				p = wcstok(ln, L" ,;\t\n");
				if (p) {
					wcscpy(s1, p);
					p = wcstok(NULL, L" ,;\t\n");
					if (p) {
						wcscpy(s2, p);
						p = wcstok(NULL, L" ,;\t\n");
						if (p) {
							siirto = _wtol(p);
							p = wcstok(NULL, L" ,;\t\n");
							if (p) {
								lk = _wtoi(p);
								}
							}
						}
					}
				}
			if (siirto) {
				upcasewstr(s1);
				if (!wcscmp(s1, L"MUUTA")) {
					upcasewstr(s2);
					if ((l = haesarja_w(s2)) >= 0)
						ch = L'N';
					else {
						swprintf(msg, L"Ohjerivillä %d (MUUTA-rivi) virheellinen sarja %s", r, s2);
						ch = ESC;
						}
					}
				else {
					upcasewstr(s1);
					m = -1;
					if ((l = haesarja_w(s1)) >= 0 && (k = _wtoi(s2)) > 0) {
						ch = L'K';
						m = jalku[l][0];
						while(m >= 0 && k > 1) {
							m = JarjSeur(0, 0, m);
							k--;
							}
						if (m >= 0) {
							k = (INT) siirto;
							}
						}
					if (m < 0) {
						if (l < 0)
							swprintf(msg, L"Ohjerivillä %d virheellinen sarja %s", r, s1);
						else
							swprintf(msg, L"Ohjerivillä %d sarjaan %s virheellinen sijoitus %s", r, s1, s2);
						ch = ESC;
						}
					}
				if (ch == ESC) {
					if (ce != L'J') {
						writeerror_w(msg, 0);
						ce  = L'O';
						wselectopt(L"L)opeta muutokset, O)hita rivi, J)atka ilman virheilmoituksia", L"LOJ", &ce);
						}
					if (ce == L'L')
						break;
					else
						continue;
					}
				}
			else
				continue;
         }
      switch (ch) {
         case L'K' :
            if (m > 0) {
               kilp1.GETREC(m);
               if ((m = getpos(k)) > 0) {
                  kilp.GETREC(m);
                  entkilp = kilp;
                  memcpy(kilp.lisno, kilp1.lisno, (int)((char *)&kilp.sarja - (char *)kilp.lisno));
                  if (lk) {
                     if (kilp1.id()+lk >= minkilpno && kilp1.id()+lk <= maxkilpno && 
                        getpos(kilp1.id()+lk) < 0) {
						kilp.setId(kilp1.id()+lk);
                        }
                     else
                        m = -1;
                     }
                  if (m > 0)
                     kilp.tallenna(m, 0, 0, 0, &entkilp);
                  }
               }
            if (m <= 0)
               writeerror_w(L"Virheellinen tai varattu kilpailijanumero", 0);
            break;
         case L'N' :
            if (siirto) {
               for (m = 0; m < (int) filelen(&datf2); m++) {
                  kilp.GETREC(m);
                  if (kilp.kilpstatus || kilp.sarja != l)
                     continue;
                  entkilp = kilp;
                  k = kilp.id() + (INT)siirto;
                  if (k >= minkilpno && k <= maxkilpno && getpos(k) < 0)
					  kilp.setId(k);
                  kilp.tallenna(m, 0, 0, 0, &entkilp);
                  }
               }
            break;
         }
      } while (1);
	}
*/

void pistetunn(void)
{
   wchar_t ch;
   wchar_t msg[80];
   int k, m;
   extern scr pistetunnistus;
   wchar_t *porttinimi[7] = {L"oletusvalinnan", L"sarjaporttinäppäimen", L"hiiren",
      L"näppäimistön", L"maalikello/lähdön", L"maalikello/maalin",
      L"maalikello/väliaikasanoman"};
   wchar_t *porttinimi2[8] = {L"oletusvalinnan", L"sarjaporttinäppäimen", L"hiiren",
      L"näppäimistön", L"maalikellon (banaani)", L"maalikellon (DIN)",
      L"maalikellon (A)", L"maalikellon (C)"};

	if (tunn_srj >= 0 && tunn_srj < sarjaluku) {
		ch = L' ';
		wselectopt(L"V)aihda lähdön automaattisen tunnistuksen sarja, T)ee muita muutoksia", L"VT", &ch);
		if (ch == L'V') {
			valitselahtosarja();
			return;
			}
		}
      ch = L' ';
      wselectopt(L"Käsitelläänkö vain yhden pisteen aikoja (K/E)",
         L"KE", &ch);
      vainoletus = ch == L'K';
      if (vainoletus) {
				if (ol_piste > 12)
					ol_piste = 12;
         ch = ch_piste[ol_piste+2];
         wselectopt(L"Anna ajanoton käyttökohde (LM123456789X)",
            L"LM123456789X", &ch);
         ol_piste = wcswcind(ch, ch_piste) - 2;
				if (ol_piste > kilpparam.maxvaluku)
					ol_piste = kilpparam.maxvaluku;
         return;
         }
      if (regnly[0] == 1)
      porttinimi[4] = L"maalikellon";
      clrscr();
      kehys(1);
      prscr(&pistetunnistus);
      for (m = -1; m < 7; m++) {
		 if (m == 1)
		 continue;
         if (regnly[0] != 4 && m == 6) break;
		 if (ol_piste > 12)
			ol_piste = 12;
         if (m == -1) 
            ch = ch_piste[ol_piste+2];
         else
            ch = ch_piste[aika_tunnus[0][m]+2];
		 if (pvparam[k_pv].hiihtolahto) {
            if (regnly[0] != 4)
               swprintf(msg, L"Anna %s käyttökohde"
	               L" (ALM123456789X)", porttinimi[m+1]);
		    else
			   swprintf(msg, L"Anna %s käyttökohde"
			       L" (ALM123456789X)", porttinimi2[m+1]);
			wselectopt(msg, L"ALM123456789X", &ch);
			}
		 else
			{
#ifndef MAASTO
             if (!m) continue;
             swprintf(msg, L"Anna %s käyttökohde"
                 L" (AM123456789X)", porttinimi[m+1]);
	         wselectopt(msg, L"AM123456789X", &ch);
#else
		     if (!m) continue;
			 swprintf(msg, L"Anna %s käyttökohde"
			     L" (AMK)", porttinimi[m+1]);
			 wselectopt(msg, L"AMK", &ch);
#endif
			 }
          k = wcswcind(ch, ch_piste) - 2;
          if (m == -1) {
             ol_piste = k;
             if (k != -2) break;
             }
          else
             aika_tunnus[0][m] = k;
         if (pvparam[k_pv].hiihtolahto && aika_tunnus[0][m] == -1) {
            ch = tunn_srj >= 0 ? L'K' : L'E';
            wselectopt(L"Tunnistetaanko lähtijä lähtöajasta "
               L"(K/E)",L"KE", &ch);
            if (ch == L'E') tunn_srj = -1;
            else {
				valitselahtosarja();
               }
            }
         if ((!regnly[0] && m == 2) ||
            (regnly[0] == 1 && m == 3))
         break;
		 }
	  }

int pyyda_vaihdavaihe(int vaihe, int kielto)
{
	return(vaihdavaihe(vaihe, kielto));
}

void asetukset()
   {
   wchar_t ch,tc,ce = L'O';
   char che;
   extern scr asetusvalikko;
   extern scr modemohje;
   extern INT errno;
   extern int reconnectSirit[NREGNLY];
   extern int siritopen[NREGNLY];
   wchar_t msg[80], ln[80] ,s1[40], s2[40], *p;
   static wchar_t ohjefl[41] = L"m-ohje.txt";
   INT16 m;
   INT l,lj,k,lk,r;
   INT32 siirto;
   kilptietue kilp, entkilp, kilp1;
   char *spawnargs[2] = {NULL, NULL};
   FILE *ohjefile = NULL;
   extern scr pistetunnistus;
	wchar_t *porttinimi[7] = {L"oletusvalinnan", L"sarjaporttinäppäimen", L"hiiren",
      L"näppäimistön", L"maalikello/lähdön", L"maalikello/maalin",
      L"maalikello/väliaikasanoman"};
   wchar_t *porttinimi2[8] = {L"oletusvalinnan", L"sarjaporttinäppäimen", L"hiiren",
	  L"näppäimistön", L"maalikellon (banaani)", L"maalikellon (DIN)",
      L"maalikellon (A)",L"maalikellon (C)"};

   clearscreen();
   kehys(1);
   prscr(&asetusvalikko);
   ch = L' ';
#ifdef LAJUNEN
   if (siritopen[0])
	   wselectopt(L"S)arjat, M)aaliajat, A)jat, loK)i, sI)rit, "
	   L"<Esc>: Poistu", L"STMAOKIC\x1B", &ch);
   else
#endif
   if (emitfl)
      wselectopt(L"S)arjat, M)aaliajat, A)jat, E)mit, mO)demi, loK)i, "
         L"<Esc>: Poistu", L"STMAEOKWC\x1B",&ch);
   else
      wselectopt(L"S)arjat, M)aaliajat, A)jat, mO)demi, loK)i, "
         L"<Esc>: Poistu", L"STMALOKWC\x1B",&ch);
	  clrln(ySize - 3);
   switch (ch) {
#ifdef LAJUNEN
   case L'I':
	   ch = L' ';
	   wselectopt(L"Käynnistä yhteydet uudelleen (K/E)", L"KE", &ch);
	   if (ch == L'K')
		   reconnectSirit[0] = 1;
	   break;
#endif
   case 'C':
	   kaynnistatesti(0);
	   break;
   case L'W':
	   ch = (k_pv >= kilpparam.n_pv_akt - 1) ? L'1' : L'2' + k_pv;
		  wselectopt(L"Anna uuden vaiheen numero, <Esc>: Peruuta", L"12345\x1b", &ch);
		  if (ch > L'0' && ch <= L'0' + kilpparam.n_pv_akt && ch != L'1' + k_pv)
			  vaihdavaihe(ch - L'1', 0);
		  break;
      case L'S':
         ch = L' ';
			wselectopt(L"Muuta L)ähtöajat, N)umerot, K)opioi tietoja, O)hje tiedostosta, <Esc>: Pois", L"OKLN\x1b", &ch);
         tc = ch;
         if (tc == L'O') {
            clrln(ySize-3);
            vidspwmsg(ySize-3,0,7,0,L"Ohjetiedoston nimi:");
            inputwstr(ohjefl, 39, 20, ySize-3, L"\r\x1b", &ch, 0);
            if (ch == ESC)
			   break;
            if ((ohjefile = _wfopen(ohjefl, L"rt")) == NULL) {
               writeerror_w(L"Tiedostoa ei löydy", 0);
               break;
               }
            }
         r = 0;
         do {
            lk = 0;
            if (tc == L'O') {
               if (feof(ohjefile)) {
                  break;
                  }
               siirto = 0;
               if (fgetws(ln, 78, ohjefile)) {
                  r++;
                  p = wcstok(ln, L" ,;\t\n");
                  if (p) {
                     wcscpy(s1, p);
                     p = wcstok(NULL, L" ,;\t\n");
                     if (p) {
                        wcscpy(s2, p);
                        p = wcstok(NULL, L" ,;\t\n");
                        if (p) {
                           siirto = _wtol(p);
                           p = wcstok(NULL, L" ,;\t\n");
									if (p) {
                              lk = _wtoi(p);
                              }
                           }
                        }
                     }
                  }
               if (siirto) {
                  upcasewstr(s1);
                  if (!wcscmp(s1, L"MUUTA")) {
                     upcasewstr(s2);
                     if ((l = haesarja_w(s2, false)) >= 0)
                        ch = L'N';
                     else {
                        swprintf(msg, L"Ohjerivillä %d (MUUTA-rivi) virheellinen sarja %s", r, s2);
                        ch = ESC;
                        }
                     }
                  else {
                     upcasewstr(s1);
                     m = -1;
                     if ((l = haesarja_w(s1, false)) >= 0 && (k = _wtoi(s2)) > 0) {
                        ch = L'K';
                        m = jalku[l][0];
						while(m >= 0 && k > 1) {
                           m = JarjSeur(0, 0, m);
                           k--;
                           }
                        if (m >= 0) {
                           k = (INT) siirto;
                           }
                        }
                     if (m < 0) {
								if (l < 0)
                           swprintf(msg, L"Ohjerivillä %d virheellinen sarja %s", r, s1);
                        else
                           swprintf(msg, L"Ohjerivillä %d sarjaan %s virheellinen sijoitus %s", r, s1, s2);
                        ch = ESC;
                        }
                     }
                  if (ch == ESC) {
                     if (ce != L'J') {
                        writeerror_w(msg, 0);
                        ce  = L'O';
                        wselectopt(L"L)opeta muutokset, O)hita rivi, J)atka ilman virheilmoituksia", L"LOJ", &ce);
                        }
                     if (ce == L'L')
                        break;
                     else
                        continue;
                     }
                  }
               else
                  continue;
               }
            switch (ch) {
               case L'K' :
                  for(;;) {
                     if (tc != L'O') {
                        clrln(ySize-3);
                        vidspwmsg(ySize-3,0,7,0,L"Lähtötiedon kilpailijanumero:      Kohteen kilpailijanumero:       Esc: Lopeta");
                        INPUTINTW(&l, 4, 30, ySize-3, L"\r\x1b", &ch);
                        if (ch == ESC) break;
                        m = getpos(l);
                        }
                     if (m > 0) {
								kilp1.GETREC(m);
                        if (tc != L'O') {
                           INPUTINTW(&k, 4, 61, ySize-3, L"\r\x1b", &ch);
                           if (ch == ESC) break;
                           }
                        if ((m = getpos(k)) > 0) {
                           kilp1.GETREC(m);
                           entkilp = kilp;
						   memcpy(kilp.lisno, kilp1.lisno, (char *)&kilp.sarja - (char *)kilp.lisno);
                           if (lk) {
                              if (kilp1.id()+lk >= minkilpno && kilp1.id()+lk <= maxkilpno && 
                                 getpos(kilp1.id()+lk) <= 0) {
								 kilp.setId(kilp1.id()+lk);
                                 }
                              else
                                 m = -1;
                              }
                           if (m > 0) {
								EnterCriticalSection(&tall_CriticalSection);
								kilp.tallenna(m, 0, 0, 0, &entkilp);
								LeaveCriticalSection(&tall_CriticalSection);
								}
                           }
                        }
                     if (m <= 0)
                        writeerror_w(L"Virheellinen tai varattu kilpailijanumero", 0);
                     if (tc == L'O')
                        break;
                     }
                  break;
               case L'L' :
#ifdef MERI5
					wselectopt(L"Laaditaanko takaa-ajokisa (K/E)", L"KE", &ch);
					if (ch == L'K') {
						takaa_ajolahdot();
						break;
						}
#endif
               case L'N' :
                  if (tc != L'O') {
                     clrln(ySize-6);
                     clrln(ySize-5);
                     clrln(ySize-3);
							swprintf(msg, L"Sarjakohtainen %sen muutos", ch == L'L' ? L"lähtöaikoj" : L"numeroid");
                     vidspwmsg(ySize-5,0,7,0, msg);
                     swprintf(msg, L"Anna sarja              ja %s",
                        ch == L'L' ? L"lähtöajan siirto         sek" : L"numeroiden muutos");
                     vidspwmsg(ySize-3,9,7,0, msg);
                     l = luesarja(NULL, &ch);
                     if (ch == ESC)
                        break;
                     siirto = 0;
                     inputlongw(&siirto, 6, 55, ySize-3, L"\r\x1b", &ch);
                     }
                  if (ch != ESC && siirto) {
                     if (tc == L'L')
                        siirto *= SEK;
                     lj = 0;
                     for (m = 0; m < (int) filelen(&datf2); m++) {
                        kilp.GETREC(m);
                        if (kilp.kilpstatus || kilp.Sarja() != l)
                           continue;
                        entkilp = kilp;
                        switch (tc) {
                           case L'L':
                              if (kilp.pv[k_pv].tlahto != TMAALI0) {
                                 kilp.pv[k_pv].tlahto += siirto;
                                 if (kilp.p_aika(-1) != TMAALI0)
                                    kilp.tall_tulos(-1, kilp.p_aika(-1)+siirto);
                                 if (kilp.p_aika(0) != 0)
                                    kilp.tall_tulos(0, kilp.p_aika(0)-siirto);
                                 }
                              break;
                           case L'N':
									case L'O':
                              k = kilp.id() + (INT)siirto;
                              if (k >= minkilpno && k <= maxkilpno && getpos(k) <= 0)
								 kilp.setId(k);
                              break;
                           }
						EnterCriticalSection(&tall_CriticalSection);
                        kilp.tallenna(m, 0, 0, 0, &entkilp);
						LeaveCriticalSection(&tall_CriticalSection);
                        }
                     }
                  break;
               }
            } while (tc == L'O');
         break;
#ifdef TV1
      case L'V':
         tv_asetus();
         break;
#endif
      case L'E' :
         ch = L' ';
         wselectopt(L"A)utomaattitulostus, S)ivujako, M)TR/emiTag-asetukset Esc: Peruuta", L"ASM\x1b", &ch);
         switch (ch) {
            case L'M' :
				aseta_mtr();
				break;
            case L'A' :
               if (emitfl) {
                  switch (autorap) {
                     case 2 :
                        ch = L'K';
                        break;
                     case 1 :
                        ch = L'O';
                        break;
                     default:
								ch = L'E';
                        break;
                     }
                  wselectopt(L"Leimausraporttien automaattitulostus: K)aikki, O)sa, E)i",
                     L"KOE", &ch);
                  switch (ch) {
                     case L'K' :
                        autorap = 2;
                        break;
                     case L'E' :
                        autorap = 0;
                        break;
                     case L'O' :
                        autorap = 1;
                        memset(autorapsrj, 0, MAXSARJALUKU);
                        clrln(ySize-3);
                        vidspwmsg(ySize-3,0,7,0,L"Anna sarja");
                        do {
                           clrln(ySize-1);
                           vidspwmsg(ySize-1,0,7,0,L"Luettele tulostettavat sarjat, lopuksi L'Esc'");
                           l = luesarja(NULL, &ch);
                           if (ch == ESC) break;
                           if (l >= 0 && l < sarjaluku) autorapsrj[l] = L'\x01';
                           } while (l >= 0 && l < sarjaluku);
                        break;
                     }
                  if (timerfl) {
                     ch = autokilp ? L'K' : L'E';
                     wselectopt(L"Liitetäänkö kortit automaattisesti aikoihin (K/E)", L"KE", &ch);
                     autokilp = ch == L'K';
					 if (autokilp && !seurkrivi && aikajono[0]) 
						 seurkrivi = aikajono[0]->aktrows;
                     }
                  }
					break;
            case L'S':
               ch = L'1' + emitsivu;
               wselectopt(L"Sivulle tulostettavien luku (1-6)", L"123456", &ch);
               emitsivu = ch - L'1';
               break;
            }
         break;
      case L'T' :
         break;
      case L'M' :
         viwrrectw(ySize-3,0,ySize-3,30,L"Anna maalin   viimeinen aika : ",7,0,0);
         for (m = 0; m < 9; m++)
            if (maaliajat[m] != TMAALI0) {
               ch = m + L'1';
               viwrrectw(ySize-3,12,ySize-3,12,&ch,7,0,0);
					INPUTAIKAW(&maaliajat[m],t0,pvparam[k_pv].laika,31,ySize-3,L"\xD",&ch);
               }
         break;
      case L'A' :
         ch = L' ';
         wselectopt(L"T)arkkuus"
#if defined(MAALI) && !defined(EIMAALI)
            L", N)äppäily"
            L", M)aalikello"
            ,L"TNM", &ch);
#else
            ,L"T", &ch);
#endif
         switch (ch) {
#if defined(MAALI) && !defined(EIMAALI)
            case L'N':
               ch = syotto ? L'E' : L'K';
               wselectopt(L"Annetaanko maaliajat näppäimistöltä (K/E)",
                  L"KE", &ch);
               syotto = ch == L'K';
               break;
            case L'M':
               if (regnly[0] == 5 || regnly[0] == 2) {
                  wselectopt(L"P)isteen tunnistus, N)olla-ajan asetus, "
                     L"K)ellon muisti ja asetukset",
                     L"PNK", &ch);
                  if (ch == L'K') {
                     aseta_s4();
                     break;
                     }
                  }
               else
                  wselectopt(L"P)isteen tunnistus, N)olla-ajan asetus",
                     L"PN", &ch);
               if (ch == L'P') {
				   pistetunn();
				   }	
               else
                  {
                  if (regnly[0] == 1 || regnly[0] == 2 || regnly[0] == 12) {
                     vidspwmsg(ySize-1,0,7,0,
                        L"Odottaa vastausta maalikellolta");
                     kerro_regnly = 1;
                     while (kerro_regnly && 
                        readkbd(&che, FALSE, spoolfl) != ESC);
                     kerro_regnly = 0;
                     }
                  clrln(ySize-3);
                  viwrrectw(ySize-3,0,ySize-3,36,
                     L"Anna maalikellon nolla-aika :        ",7,0,0);
                  INPUTAIKAW(&t0_regn[0],t0,8,30,ySize-3,L"\xD",&ch);
						maaliajat[9] = t0_regn[0];
                  }
               break;
#endif
            case L'T':
               switch (pvparam[k_pv].laika) {
                  case 11 :
                     ch = L'S';
                     break;
                  case 10 :
                     ch = L'K';
                     break;
                  default :
                     ch = L'T';
                     break;
                  }
					wselectopt(L"Tarkkuus: S)adasosa, K)ymmenys, T)asasekunti", L"SKT", &ch);
					switch (ch) {
                  case L'S':
							pvparam[k_pv].laika = LAIKA;
							pvparam[k_pv].pyor[1] = 1;
                     break;
                  case L'K':
							pvparam[k_pv].laika = 10;
							pvparam[k_pv].pyor[1] = 10;
                     break;
                  case L'T' :
							pvparam[k_pv].laika = 8;
							pvparam[k_pv].pyor[1] = 100;
                     break;
                  }
					if (pvparam[k_pv].laika > 8) {
                  switch (pvparam[k_pv].laika2) {
							case 11 :
                        ch = L'S';
                        break;
                     case 10 :
                        ch = L'K';
                        break;
                     default :
                        ch = L'T';
                        break;
                     }
                  wselectopt(L"Kirjoita: S)adasosat, K)ymmenykset, T)asasekunnit", L"SKT", &ch);
                  switch (ch) {
                     case L'S':
                        pvparam[k_pv].laika2 = LAIKA;
                        break;
                     case L'K':
								pvparam[k_pv].laika2 = 10;
								break;
							case L'T' :
								pvparam[k_pv].laika2 = 8;
								break;
							}
						}
					else
						pvparam[k_pv].laika2 = pvparam[k_pv].laika;
					break;
				}
			break;
		case L'D' :
			break;
		case L'K' :
			break;
		}
	if (ohjefile)
		fclose(ohjefile);
	}

void sendTestLopetus(void) {}

void SendCopyThread(LPVOID lpCn);
void lahetatestikaynnistys(INT kielto);

void kaynnistatesti(int kielto)
	{
		lahetatestikaynnistys(kielto);
		_beginthread(SendCopyThread, 10240, NULL);
	}
