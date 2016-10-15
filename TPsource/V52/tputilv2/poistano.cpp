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

#include <memory.h>
#include <string.h>

void poistanollat(char *st)
   {
   while(!memcmp(st, "00:", 3))
      memmove(st, st+3, strlen(st)-2);
   if (st[0] == '0')
      memmove(st, st+1, strlen(st));
	}

void wpoistanollat(wchar_t *st)
	{
	while(!memcmp(st, L"00:", 6))
		memmove(st, st+3, 2*(wcslen(st)-2));
	if (st[0] == L'0')
		memmove(st, st+1, 2*wcslen(st));
	}
