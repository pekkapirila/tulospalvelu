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

#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#ifndef _CONSOLE
#define _CONSOLE
#endif
#include <stdlib.h>
#include <string.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0

void inputaika(short *t,int t0, int l, int x, int y, char *term, char *tc)
{
   int ok = 0, tt, i;
   char tstr[9],tstr0[9],trm[50];
 
   strncpy(trm+1, term, 48);
   trm[0] = '\x1b';
   if (*t <= -32767)  
	   strcpy(tstr, "  .  .  ");
   else 
	   aikatostr_s(tstr, *t, t0);
   strcpy(tstr0, tstr);
   l = min(l,8);
   do {
      inputstr(tstr,l,x,y,trm,tc,-1);
      tstr[2] = '.';
      tstr[5] = '.';
      if (memcmp(tstr, "  .  .  ", l) == 0 ||
          memcmp(tstr, "99.99.99", l) == 0) {
          tt=-32767;
          ok = 1;
          break;
          }
      if (!memcmp(tstr, tstr0, l)) break;
      if ((tt = (int) strtoaika(tstr,t0)) != -32767) {
         ok = 1;
         if (l == 5) {
            memcpy(tstr+5, ".00", 4);
            tt = (int) strtoaika(tstr,t0);
            }
         }
      else {
         erbeep();
         tstr0[0] = 0;
         for (i = 0; i < 8; i++) {
            if (i == 2 || i == 5) continue;
            if (tstr[i] > '9' || tstr[i] < '0') tstr[i] = '0';
            }
         }
   } while (!ok && (*tc != 27));
   if (ok)  *t = tt;
}


void inputaika_w(short *t, int t0, int l, int x, int y, wchar_t *wterm, wchar_t *wtc)
{
	char term[40] = "", tc;
	int i;

	wcstooem(term, (wchar_t *) wterm, 39);
	for (i = 0; wterm[i]; i++)
		if (wterm[i] >= 201 && wterm[i] <= 210)
			term[i] = (char) wterm[i];
	inputaika(t, t0, l, x, y, term, &tc);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
}