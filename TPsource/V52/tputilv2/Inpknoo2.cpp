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
#include <string.h>
#include <bstrings.h>
#include <bkeybrd.h>
#include <bvideo.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#define NNUM 4
#define CPOS 3

extern int spoolfl;

void inputkno_os2(int *kno, int *os, int x, int y, char *term, char *tc)
   {
   char ch;
   int i;

   kbready(&ch, &i);
   i = 0;
   if (ch >= '0' && ch <= '9')
      *kno = 0;
   inputint_oik(kno, 4, x, y, term, &ch);
   if (ch == 13 || ch == '*') {
      if (*kno == 0) *os = 0;
      vidint(y,x,4,*kno);
      for (;;) {
		 inputint_oik(os, 2, x+5, y, "+\r\x1b", &ch);
         vidint(y,x+5,2,*os);
         if (!*kno || *os > 0) break;
         writeerror("Virheellinen osuus",1000);
         } 
      }
   *tc = ch;
   }


void inputkno_os2w(int *kno, int *os, int x, int y, wchar_t *wterm, wchar_t *wtc)
{
	char tc, term[40];
	int j;

	wcstooem(term, wterm, 39);
	for (j = 0; wterm[j]; j++)
		if (wterm[j] >= 201 && wterm[j] <= 210)
			term[j] = (char) wterm[j];
	term[39] = 0;
	inputkno_os2(kno, os, x, y, term, &tc);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
}
