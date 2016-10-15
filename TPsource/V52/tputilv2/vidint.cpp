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
#include <string.h>
#include <bvideo.h>
#include <tputil.h>


void vidint(int y, int x, int l, long i)
{
   char s[20];
   int k,ls;

   if (l > 19) l = 19;
   if (l < -19) l = -19;
   _ltoa(i, s, 10);
   ls = strlen(s);
   if (l > ls)
      for (k = ls; k < l; k++) inschar(' ',s,0);
   else if (-l > ls)
      for (k = ls; k < -l; k++) inschar(' ',s,k);
   if ((l > 0 && ls > l) || (l < 0 && ls > -l)) {
      memset(s, '*', l);
      s[l] = 0;
      }
   vidspmsg(y,x,7,0,s);
}



