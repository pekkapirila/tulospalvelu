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
/*  tt0 on lis„ys tunteihin */
#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#include <string.h>

char *aikatostr_ls(char *astr, int aika, int tt0)

{
  int ta, tt, tlisa = 0;
  int tm, ts;

  if (!tt0 && (aika > 24*36000L)) tlisa = 24;
  strcpy((char *) astr, "  .  .  , ");
  if (aika != -24*36000L) {
     aika += (tt0 + 48) * 36000L;
     while (aika < 0) aika += 24L*36000L;
     tt = aika / 36000L;
     ta = aika - 36000L * tt;
     tt = tt % 24 + tlisa;
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
