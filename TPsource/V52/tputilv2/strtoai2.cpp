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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#define DAY (24*36000L)

/*  Strtoaika_l2 muuttaa muodossa tt.mm.ss,s annetun ajan 0,1 sekunneiksi */
/*  tt0 on lis„ys aikaan */
/*  Palautettava aika on v„lill„ 0 <= aika < DAY, jos tt0 = 0 */
/*                       v„lill„ -DAY/2 <= aika < DAY/2, jos tt0 != 0 */
/*                       -DAY, jos merkkijono on virheellinen */

long strtoaika_l2(char *straika,long tt0)

{
  int th, tm, ts, nf, tx;
  long tl;

  if (strlen(straika) == 5)  strcat(straika, ".00");
  if (strlen(straika) == 8)  strcat(straika, ",0");
  nf = sscanf(straika, "%2d.%2d.%2d,%1d", &th, &tm, &ts, &tx);
  if ((nf == 4) && (straika[2] == '.') && (straika[5] == '.') &&
	  (straika[8] == ',') && (straika[3] < '6') && (straika[6] < '6')) {
	  tl = 36000L * th + 600L * tm + 10 * ts + tx;
	  if (tt0 != 0) {
		  tl = (tl - tt0 + DAY) % DAY - 12*36000L;
		  }
	  return(tl);
	  }
  return(-DAY);
}

long wstrtoaika_l2(wchar_t *straika,long tt0)
{
	char str[20];

	wcstoansi(str, straika, 16);
	return(strtoaika_l2(str, tt0));
}

