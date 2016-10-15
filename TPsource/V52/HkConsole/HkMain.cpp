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
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "HkDeclare.h"

int ExitHandler(DWORD fdwCtrlType);
int aloitus(int argc, wchar_t* argv[], wchar_t *cfgflname);
void tausta(INT spoolfl);
void copydfile(datafile *datfb, INT oldfile);
void asetukset(void);
void laskenta(void);
void korjaukset(void);
void lopetus(void);
void tulostus(INT oikaise);
void talljarj(void);
void txttv(void);
#ifdef TAULU
void taulu(void);
#endif
#ifdef MAALI
void ajanotto(void);
#endif
void yhteydet(void);

void valikko(void)
   {
   wchar_t  ch, stvaihe[] = L"VAIHE   ";
   char st[102];
   extern scr paavalikko;
#ifdef MAKI
	if (k_pv == makipv)
		paavalikko.l[1].t = L"M)äki           Mäkikilpailun tulospalvelu";
#endif
	if (bootfl & 8) {
		int jj = (bootfl / 16) & 15;
		if (jj && jj <= maxjono)
			akttm = jj;
		ajanotto();
		}
	do {
      clearscreen();
      header=L"PÄÄVALIKKO";
      ajat_on = 0;
      status_on = 1;
      kehys(1);
      prscr(&paavalikko);
	  if (kilpparam.n_pv_akt > 1) {
		  stvaihe[6] = L'1' + k_pv;
		  vidspwmsg(0, 55, 7, 0, stvaihe);
		  }
      listfl = TRUE;
      ch = ' ';
#ifdef TAULU
#ifdef MAKI
      if (bootfl < 2) {
			if (k_pv == makipv)
				wselectopt(L"M)äki, K)orjaa, S)elost., T)ulos, taU)lu, A)setus, Y)hteys, "
					L"B)ackup, P)ois", L"MKSTOAUYPB", &ch);
			else
				wselectopt(L"M)aali, K)orjaa, S)elost., T)ulos, taU)lu, A)setus, Y)hteys, "
					L"B)ackup, P)ois", L"MKSTOAUYPB", &ch);
			}
      else if (bootfl == 2) {
			ch = 'S';
         bootfl = 0;
			}
		else {
			ch = 'U';
			vaintaulu = 1;
         }
#else
		wselectopt(L"M)aali, L)ask, K)orj, T)ulos, Y)hteys, A)setus, taU)lu, "
         L"B)ackup, P)oistu", L"MLKTOYAUPB", &ch);
#endif // !MAKI
#else  // !TAULU
#if defined(EMITLEIMAT) 
      wselectopt(L"M)aali, L)ask, K)orj, T)ulos, lE)imat, Y)hteys, A)setukset, "
         L"B)ackup, P)oistu", L"MLKTOEYAPB", &ch);
#else   // !EMITLEIMAT
      wselectopt("M)aali, L)askenta, K)orjaa, T)ulokset, Y)hteys, A)setukset, "
         "B)ackup, P)oistu", "YMLKTOAPB", &ch);
#endif // !EMITLEIMAT
#endif // !TAULU
      switch(ch) {
         case L'M' :
#ifdef MAKI
				if (makipv == k_pv)
					editmaki();
				else
#endif
			      ajanotto();
            break;
#ifdef TAULU
         case L'U' :
            taulu();
				if (vaintaulu)
					ch = L'P';
            break;
#endif
         case L'L' :
				if (nilmt < 1) {
					writeerror_w(L"Ei kilpailijoita", 0);
					break;
					}
            status_on = 1;
            laskenta();
            break;
         case L'K' :
            status_on = 1;
            korjaukset();
            break;
         case L'O' :
         case L'T' :
            tulostus(ch == L'O');
            break;
         case L'A' :
            asetukset();
            break;
         case L'B' :
            copydfile(&datf3,TRUE);
            if( backfl )
				openfile(&datf3,wcstoansi(st, dataf3name, 100),kilpparam.kilprecsize);
            break;
         case L'E' : 
				if (nilmt < 1) {
					writeerror_w(L"Ei kilpailijoita", 0);
					break;
					}
            leimat();
            break;
         case L'Y': 
			 yhteydet(); 
			 break;
         }
		if (ch == L'P') {
         ch = L' ';
#if LANG == SV
         wselectopt(L"Sluta och stänga av? (J/N)",
            L"JN", &ch);
         if (ch == L'J') ch = L'P';
#else
         wselectopt(L"Haluatko varmasti poistua ohjelmasta (K/E)",
            L"KE", &ch);
         if (ch == L'K') ch = L'P';
#endif
			}
      } while (ch != L'P');
   }

int wmain(int argc, wchar_t* argv[])
{
	initcon(argv[0]);
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitHandler, TRUE);
	GetCurrentDirectory(sizeof(WorkingDir) / 2, WorkingDir);
	kilpparam.kilplaji = L'S';
	kilpparam.alalaji = L' ';
	kilpparam.badgelaji = L'E';
	kilpparam.lsnimi = LSNIMI;
	kilpparam.lenimi = LENIMI;
	kilpparam.larvo = LARVO;
	kilpparam.lseura = LSEURA;
	kilpparam.lseuralyh = LLYH;
	kilpparam.ljoukkue = LJK;
	kilpparam.lyhdistys = LYHD;
	kilpparam.lwrkoodi = LWRKOODI;
	kilpparam.maxasakko = 5;
	kilpparam.WisV = true;
	kilpparam.seuralyh_on = true;
	kilpparam.n_pv = 2;
	kilpparam.n_pv_akt = 1;
	kilpparam.valuku = 10;
	kilpparam.sakkoyks = RRSAKKO;
	if (!aloitus(argc, argv, NULL)) {
		if (poistaSulje) {
			HWND h;
			HMENU sm;
			int i, j, c;
			LPWSTR buf;
			h = GetConsoleWindow();
			// get handle to the System Menu
			sm = GetSystemMenu(h, 0);
			// how many items are there?
			c = GetMenuItemCount(sm);
			j = -1;
			buf = (wchar_t*)malloc(256 * sizeof(TCHAR));
			for (i = 0; i<c; i++) {
				// find the one we want
				GetMenuStringW(sm, i, buf, 255, MF_BYPOSITION);
				if (!wcscmp(buf, L"&Close")) {
					j = i;
					break;
				}
				}
			// if found, remove that menu item
			if (j >= 0)
				RemoveMenu(sm, j, MF_BYPOSITION);
			free(buf);
			}
		valikko();
		talljarj();
		}
	lopetus();
	return(0);
}
