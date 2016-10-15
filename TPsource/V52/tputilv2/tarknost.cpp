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

#include <string.h>

int tarknost(char *st)
   {
   int t = 0, k = 3;
   char *p;

   for (p = st + strlen(st) - 1; p >= st; p--) {
      t += k * (*p - 48);
      switch (k) {
         case 3 :
            k = 7;
            break;
         case 7 :
            k = 1;
            break;
         case 1 :
            k = 3;
            break;
         }
      }
   return((200 - t) % 10);
   }
