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

#ifndef _CONSOLE
#define _CONSOLE
#endif
#include <string.h>
#include <bvideo.h>
#include <tputil.h>

char *inputstr_pr(char *prompt, char *ss, int l, int x, int y,
   const char *term, char *tc, int numfl)
   {
   int pl;

	pl = strlen(prompt);
   pl = min(x - 1, pl);
   viwrrect(y, x - 1 - pl, y, x - 2, prompt, 7, 0, 0);
   return(inputstr(ss, l, x, y, term, tc, numfl));
	}

wchar_t *inputwstr_pr(wchar_t *wprompt, wchar_t *wss, int l, int x, int y,
	const wchar_t *wterm, wchar_t *wtc, int numfl)
	{
	int pl, i;
	char prompt[80], str[82], term[40], tc;

	wcstooem(prompt, wprompt, 79);
	wcstooem(str, wss, l);
	pl = strlen(prompt);
	pl = min(x - 1, pl);
	viwrrect(y, x - 1 - pl, y, x - 2, prompt, 7, 0, 0);
	wcstooem(term, (wchar_t *) wterm, 39);
	for (i = 0; wterm[i]; i++)
		if (wterm[i] >= 201 && wterm[i] <= 210)
			term[i] = (char) wterm[i];
	inputstr(str, l, x, y, term, &tc, numfl);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
	return(oemtowcs(wss, str, l, 0));
	}

