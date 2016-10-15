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
#include <bvideo.h>
#include "VDeclare.h"
#include <size.h>

int ExitHandler(DWORD fdwCtrlType);
void tulostus(int oikaise);
void algetaulu(void);
void yhteydet(void);

void valikko(void)
   {
   char  ch;
   extern scr paavalikko;

   do {
      clrscr();
	  status_on = 1;
      header=L"PÄÄVALIKKO";
      kehys(1);
      prscr(&paavalikko);
      listfl = TRUE;
      ch = ' ';
#ifdef EMIT
#ifdef LUENTA
      selectopt("K)orjaa, T)ulokset, E)mitluenta, Y)yhteys, A)setukset, "
         "P)oistu", "KTYOEAP", &ch);
#else
#ifdef SERVER
#ifdef ALGETAULU
      if (taulu_com || udptaulu)
      selectopt("M)aali, L)ask, K)orj, T)ulos, lE)imat, Y)hteys, A)setukset, "
         "P)oistu", "MLKTYOEAPU", &ch);
      else
#endif  // ALGETAULU
      selectopt("M)aali, L)ask, K)orj, T)ulos, lE)imat, Y)hteys, A)setukset, "
         "P)oistu", "MLKTYOEAP", &ch);
//      selectopt("S)erver, M)aali, L)ask, K)orj, T)ulos, lE)imat, A)setukset, "
//         "B)ackup, P)oistu", "SMLKTOEAPB", &ch);
#else
#ifdef ALGETAULU
      if (taulu_com || udptaulu)
         selectopt("M)aali, L)askenta, K)orjaa, T)ulos, lE)imat, A)set, taU)lu, "
            "P)oistu", "MLKTOEAPU", &ch);
      else
#endif  // ALGETAULU
         selectopt("M)aali, L)askenta, K)orjaa, T)ulokset, lE)imat, A)setukset, "
            "P)oistu", "MLKTOEAP", &ch);
#endif  // !SERVER
#endif  // !LUENTA
      switch(ch) {
         case 'E': leimat(); break;
#ifdef SERVER
         case 'Y': yhteydet(); break;
#endif  // SERVER
#else   // !EMIT
      selectopt("M)aali, L)askenta, K)orjaukset, T)ulokset, A)setukset, "
         "P)oistu", "MLKTOAP", &ch);
      switch(ch) {
#endif  // !EMIT
#if !defined(LUENTA)
        case 'M' : ajanotto(); break;
#endif
        case 'K' : korjaukset(); break;
#if !defined(LUENTA)
         case 'L' : laskenta(); break;
#endif
#if defined(ALGETAULU)
         case 'U' : algetaulu(); break;
#endif
         case 'O' :
         case 'T' : tulostus(ch == 'O'); break;
         case 'A' : asetukset(); break;
         case 'P' : ch = ' ';
                    selectopt("Haluatko varmasti poistua ohjelmasta (K/E)",
                       "KE", &ch);
                    if (ch == 'K') ch = 'P';
         }
      } while (ch != 'P');
   }

int wmain(int argc, wchar_t *argv[])
   {
//	wchar_t wst[80];

//   initcon(ansitowcs(wst, argv[0], 79));
   initcon(argv[0]);
   SetConsoleCtrlHandler((PHANDLER_ROUTINE)ExitHandler, TRUE);
   GetCurrentDirectoryW(sizeof(WorkingDir) / 2, WorkingDir);
   if (aloitus(argc, argv, 0))
	   lopetus();
	
   if (poistaSulje) {
	   HWND h;
	   HMENU sm;
	   int i, j, c;
	   LPTSTR buf;
	   h = GetConsoleWindow();
	   // get handle to the System Menu
	   sm = GetSystemMenu(h, 0);
	   // how many items are there?
	   c = GetMenuItemCount(sm);
	   j = -1;
	   buf = (TCHAR*)malloc(256 * sizeof(TCHAR));
	   for (i = 0; i<c; i++) {
		   // find the one we want
		   GetMenuString(sm, i, buf, 255, MF_BYPOSITION);
		   if (!wcscmp((wchar_t *)buf, L"&Close")) {
			   j = i;
			   break;
			   }
		   }
	   // if found, remove that menu item
	   if (j >= 0)
		   RemoveMenu(sm, j, MF_BYPOSITION);
	   free(buf);
	   }
//   if (ok) 
	   valikko();
   lopetus();
   return(0);
   }

