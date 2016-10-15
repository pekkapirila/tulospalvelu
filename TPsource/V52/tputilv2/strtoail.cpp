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

/*  Strtoaika muuttaa muodossa tt.mm.ss,s annetun ajan 0,1 sekunneiksi */
/*  tt0 on lis„ys tunteihin */

long strtoaika_l(char *straika,int tt0)

{
  int th, tm, ts, nf, tx;

  if (strlen(straika) == 5)  strcat(straika, ".00");
  if (strlen(straika) == 8)  strcat(straika, ",0");
  nf = sscanf(straika, "%2d.%2d.%2d,%1d", &th, &tm, &ts, &tx);
  if (tt0 != 0) {
     th = (th - tt0 + 36) % 24 - 12;
     }
  if ((nf == 4) && (straika[2] == '.') && (straika[5] == '.') &&
     (straika[8] == ',') && (straika[3] < '6') && (straika[6] < '6'))
     return(36000L * th + 600L * tm + 10 * ts + tx);
  return(-24*36000L);
}

long wstrtoaika_l(wchar_t *straika,int tt0)
{
	char str[20];

	wcstoansi(str, straika, 16);
	return(strtoaika_l(str, tt0));
}

