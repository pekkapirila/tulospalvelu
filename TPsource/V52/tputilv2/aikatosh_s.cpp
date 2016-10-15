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

/*  aikatostr_h muuttaa 1/100 sekunneissa annetun ajan muotoon tt.mm.ss,ss */
/*  tt0 on lis„ys tunteihin */
#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#include <string.h>
 
char *aikatostr_hs(char *astr, int aika, int tt0)

{
  int ta, tt, tlisa = 0;
  int tm, ts;

  if (!tt0 && (aika > 24*360000L)) tlisa = 24;
  strcpy(astr, "  .  .  ,  ");
  if (aika != -24*360000L) {
	 aika += (tt0 + 48) * 360000L;
	 while (aika < 0) aika += 24L*360000L;
	 tt = aika / 360000L;
	 ta = aika - 360000L * tt;
	 tt = tt % 24 + tlisa;
	 tm = (int) (ta / 6000);
	 ts = (int)(ta - 6000L * tm);
	 astr[0]=  '0' + (char)(tt/10);
	 astr[1] = '0' + (char)(tt%10);
	 astr[3] = '0' + tm/10;
	 astr[4] = '0' + tm%10;
	 tm = ts/1000;
	 ts = ts-1000*tm;
	 astr[6] = '0' + tm;
	 tm = ts/100;
	 ts = ts-100*tm;
	 astr[7] = '0' + tm;
	 astr[9] = '0' + ts/10;
	 astr[10] = '0' + ts%10;
	 }
  return(astr);
}

char *aikatostr_ts(char *astr, int aika, int tt0)

{
  int ta, tt, tlisa = 0;
  int tm, ts;

  if (!tt0 && (aika > 24*3600000L)) tlisa = 24;
  strcpy(astr, "  .  .  ,   ");
  if (aika != -24*3600000L) {
	 aika += (tt0 + 48) * 3600000L;
	 while (aika < 0) aika += 24L*3600000L;
	 tt = aika / 3600000L;
	 ta = aika - 3600000L * tt;
	 tt = tt % 24 + tlisa;
	 tm = (int) (ta / 60000);
	 ts = (int)(ta - 60000L * tm);
	 astr[0]=  '0' + (char)(tt/10);
	 astr[1] = '0' + (char)(tt%10);
	 astr[3] = '0' + tm/10;
	 astr[4] = '0' + tm%10;
	 tm = ts/10000;
	 ts = ts-10000*tm;
	 astr[6] = '0' + tm;
	 tm = ts/1000;
	 ts = ts-1000*tm;
	 astr[7] = '0' + tm;
	 tm = ts/100;
	 ts = ts-100*tm;
	 astr[9] = '0' + tm;
	 astr[10] = '0' + ts/10;
	 astr[11] = '0' + ts%10;
	 }
  return(astr);
}
