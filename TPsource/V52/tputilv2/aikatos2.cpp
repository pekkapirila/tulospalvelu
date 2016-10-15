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

/*  aikatostr_l muuttaa 1/10 sekunneissa annetun ajan muotoon tt.mm.ss,s */
/*  tt0 on lis„ys aikaan */

#include <string.h>

char *aikatostr_l2(int aika, int tt0)

{
  long ta, tt;
  int tm, ts;
  static char astr[11];

  strcpy(astr, "  .  .  , ");
  if (aika != -24*36000L) {
     aika += tt0 + 48 * 36000L;
     if (aika < 0) aika += 59652L*36000L;
     tt = aika/ 36000L;
     ta = aika - 36000L * tt;
     tt = tt % 24;
     tm = (int) (ta / 600);
     ts = (int)(ta - 600L * tm);
     astr[0]=  '0' + (char)(tt/10);
     astr[1] = '0' + (char)(tt%10);
     astr[3] = '0' + (char)(tm/10);
     astr[4] = '0' + (char)(tm%10);
     tm = ts/100;
     ts = ts-100*tm;
     astr[6] = '0' + (char)(tm);
     astr[7] = '0' + (char)(ts/10);
     astr[9] = '0' + (char)(ts%10);
     }
  return(astr);
}
