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
#include <stdlib.h>
#include <time.h>
#include <tputil.h>

char *kello(void)
{
   time_t clock;
   char *ct;

   time(&clock);
   ct = ctime(&clock);
   delchar(ct,0,11);
   ct[8] = 0;
   ct[2] = '.';
   ct[5] = '.';
   return(ct);
}

wchar_t *wkello(void)
{
	static wchar_t wct[20];

	ansitowcs(wct, kello(), 9);
	return(wct);
}


