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
#ifdef TEST
#include <stdio.h>
#endif
void esnimet2(char *sti, char *etu, char *suku)
   {
   char c1, c2, *p;

   etu[0] = 0;
   suku[0] = 0;
   for (p = sti; *p && *p != '_'; p++);
   if (*p == 0) {
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
	   }
   if ((*p == ' ' && p[1] > ' ') || *p == '_') {
	  strcpy(etu, p+1);
	  }
   memcpy(suku, sti, (int) (p - sti));
   suku[(int)(p-sti)] = 0;
   for (p = suku; *p; p++)
	   if (*p == '_')
		   *p = ' ';
   for (p = etu; *p; p++)
	   if (*p == '_')
		   *p = ' ';
   }

#ifdef TEST
void main(void)
   {
   char inst[100], etu[50], suku[50];

   for(;;) {
      puts("\nAnna nimi.   X lopettaa\n");
      gets(inst);
      if (!inst[1] && (inst[0] == 'x' || inst[0] == 'X')) return;
      esnimet(inst, etu, suku);
      printf("\nEtunimi: %s, sukunimi: %s\n", etu, suku);
      }
   }
#endif
