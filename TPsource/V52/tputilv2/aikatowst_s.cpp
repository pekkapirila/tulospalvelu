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

/*  aikatostr muuttaa sekunneissa annetun ajan muotoon tt.mm.ss */
/*  tt0 on lis„ys tunteihin */

#include <string.h>

wchar_t *aikatowstr_s(wchar_t *astr, int aika, int tt0)

{
  int ta;
  int tt, tm, ts;

  wcscpy(astr, L"  .  .  ");
  if (aika != -24*3600L) {
	  aika += (tt0 + 48) * 3600L;
	  tt = (int) (aika/ 3600L);
	  ta = aika - 3600L * tt;
	  tt = tt % 24;
	  tm = (int) ta / 60;
	  ts = (int) ta - 60 * tm;
	  astr[0]=  L'0' + tt/10;
	  astr[1] = L'0' + tt%10;
	  astr[3] = L'0' + tm/10;
	  astr[4] = L'0' + tm%10;
	  astr[6] = L'0' + ts/10;
	  astr[7] = L'0' + ts%10;
	  }
  return(astr);
}
