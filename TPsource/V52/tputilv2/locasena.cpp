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

#include <ctype.h>
#include "tputil.h"

char *locasename(char *s)
   {
   char *p;
   int alku;

   alku = 1;
   p = s;
   while (*p != 0) {
      if (!alku) *p = locasesc(*p);
      if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') ||
         *p >= 128) alku = 0;
      else alku = 1;
      p++;
      }
   return(s); 
	}

wchar_t *locasewname(wchar_t *s)
	{
	wchar_t *p;
	int alku;

	alku = 1;
	p = s;
	while (*p != 0) {
		if (!alku) *p = locasewsc(*p);
		if ((*p >= L'A' && *p <= L'Z') || (*p >= L'a' && *p <= L'z') ||
			*p >= 128) alku = 0;
		else alku = 1;
		p++;
		}
	return(s);
	}
