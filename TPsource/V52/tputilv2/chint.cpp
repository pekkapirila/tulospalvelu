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

#include <string.h>

int chint(char ch)
{
   if (ch >= '0' && ch <= '9') return(ch - 47);
   if (ch >= 'A' && ch <= '\x5d') return(ch - 54);
   return(0);
}

int wchint(wchar_t ch)
{
	if (ch >= L'0' && ch <= L'9') return(ch - 47);
	if (ch >= L'A' && ch <= L'\x5d') return(ch - 54);
	return(0);
}
