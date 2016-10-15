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

char *upcasestr(char *s)
{
   int p;

   p = 0;
   while (s[p] != 0)
      {
         s[p] = upcasesc(s[p]);
         p++;
      }
   return(s);
}

wchar_t *upcasewstr(wchar_t *s)
{
	int p;

	p = 0;
	while (s[p] != 0)
		{
			s[p] = upcasewsc(s[p]);
			p++;
		}
	return(s);
}


