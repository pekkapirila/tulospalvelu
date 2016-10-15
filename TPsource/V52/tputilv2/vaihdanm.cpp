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

#include <ctype.h>
#include <string.h>
#include "tputil.h"
#ifdef TEST
#include <stdio.h>
#endif

void vaihdanm(char *sti, char *sto)
   {
   char c1, c2, *p;
   int len = 0;

   p = sti;
   if (sti[2] == ' ') {
      c1 = toupper(sti[0]);
      if ((c1 == 'A' && ((c2 = toupper(sti[1])) == 'V' || c2 == 'F')) ||
         (c1 == 'D' && toupper(sti[1]) == 'E'))
         p += 3;
      }
   if (sti[3] == ' ') {
      if (toupper(sti[0]) == 'V' && toupper(sti[2]) == 'N' &&
         ((c2 = toupper(sti[1])) == 'O' || c2 == 'A'))
         p += 4;
      }
   for (; *p && *p != ' '; p++) ;
   if (*p == ' ') {
      strcpy(sto, p+1);
      len = strlen(sto);
      sto[len++] = ' ';
      }
   memcpy(sto+len, sti, (int) (p - sti));
   sto[len+(int)(p-sti)] = 0;
   }

void vaihdawnm(wchar_t *sti, wchar_t *sto)
	{
	wchar_t c1, c2, *p;
	int len = 0;

	p = sti;
	if (sti[2] == ' ') {
		c1 = towupper2(sti[0]);
		if ((c1 == L'A' && ((c2 = towupper2(sti[1])) == L'V' || c2 == L'F')) ||
			(c1 == L'D' && towupper2(sti[1]) == L'E'))
			p += 3;
		}
	if (sti[3] == L' ') {
		if (towupper2(sti[0]) == L'V' && towupper2(sti[2]) == L'N' &&
			((c2 = towupper2(sti[1])) == L'O' || c2 == L'A'))
			p += 4;
		}
	for (; *p && *p != L' '; p++) ;
	if (*p == L' ') {
		wcscpy(sto, p+1);
		len = wcslen(sto);
		sto[len++] = L' ';
		}
	memcpy(sto+len, sti, 2*((int) (p - sti)));
	sto[len+(int)(p-sti)] = 0;
	}

#ifdef TEST
void main(void)
   {
   char inst[100], outst[100];

   for(;;) {
      puts("\nAnna nimi.   X lopettaa\n");
      gets(inst);
      if (!inst[1] && (inst[0] == 'x' || inst[0] == 'X')) return;
      puts("\nVaihdettuna\n");
      vaihdanm(inst, outst);
      puts(outst);
      }
   }
#endif
