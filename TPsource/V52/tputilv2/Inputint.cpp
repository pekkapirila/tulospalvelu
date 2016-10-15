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
#include <ctype.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0


void inputint(short *i, int l,int x,int y, char *term, char *tc)
{
   char istr[7];

   _itoa(*i,istr,10);
   inputstr(istr,min(l,6),x,y,term,tc,TRUE);
   *i = (short) atoi(istr);
}

void inputintw(short *i, int l,int x,int y, wchar_t *wterm, wchar_t *wtc)
{
	char term[40], tc;
	int j;

	wcstooem(term, wterm, 39);
	for (j = 0; wterm[j]; j++)
		if (wterm[j] >= 201 && wterm[j] <= 210)
			term[j] = (char) wterm[j];
	inputint(i, l, x, y, term, &tc);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
}

