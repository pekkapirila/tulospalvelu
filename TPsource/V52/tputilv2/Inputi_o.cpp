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
#ifndef _CONSOLE
#define _CONSOLE
#endif
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0


void inputint_oik(int *i, int l,int x,int y, char *term, char *tc)
{
   char istr[15], *p;
   int l1;

   _itoa(*i,istr,10);
   if ((l1 = strlen(istr)) < l) {
      memmove(istr+l-l1, istr, l1+1);
      memset(istr, ' ', l - l1);
      }
   inputstr_oik(istr,l,x,y,term,tc,TRUE);
   for (p = istr; *p == ' '; p++) ;
   *i = atoi(p);
}



