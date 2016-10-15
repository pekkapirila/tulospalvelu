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
#include <string.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#define TMAALI0 -864000L

/*  Strtoaika muuttaa muodossa tt.mm.ss,s annetun ajan 0,1 sekunneiksi */
/*  tt0 on lis„ys tunteihin */

long str2aika(char *straika,int tt0)

   {
   long t1, t = TMAALI0;
   int th;
   char str[11], *p;

   if (strlen(straika) < 5)
      return(t);
   strncpy(str, straika, 10);
	str[10] = 0;
   if (strlen(straika) == 5) {
      strcat(straika, ".00");
      }
   p = strtok(str, ".:");
   if (!p || strlen(p) != 2)
      return(t);
   th = atoi(p);
   t = 36000L * th;
   p = strtok(NULL, ".:");
   if (!p || strlen(p) != 2 || (t1 = atol(p)) > 59)
      return(-TMAALI0);
   t += 600L * t1;
   if (strlen(p) == 4)
      t += p[3] - '0';
   p = strtok(NULL, ".,");
   if (!p || strlen(p) != 2 || (t1 = atol(p)) > 59)
      return(-TMAALI0);
	t += 10L * t1;
   if (tt0 != 0) {
      th = (th - tt0 + 36) % 24 - 12 - th;
      t += 36000L * th;
      }
   return(t);
   }

long wstr2aika(wchar_t *straika,int tt0)
{
	char str[20];

	wcstoansi(str, straika, 16);
	return(str2aika(str, tt0));
}

