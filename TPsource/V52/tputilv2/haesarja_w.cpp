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
#include <bstrings.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0


extern int sarjaluku;
extern wchar_t sarjanimi[][11];

int haesarja_w(wchar_t *snimi)
{
   int i;

   i = 0;
   upcasewstr(snimi);
   wstpcvt(snimi, 6);
   do {
	  if (!wcscmp(snimi, sarjanimi[i]))
		 return(i);
   } while (++i < sarjaluku);
   return(-1);
}



