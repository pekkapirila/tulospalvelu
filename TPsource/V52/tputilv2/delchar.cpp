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

void delchar(char *s, int pos, int n)
{
	int  i;

	if (strlen(s) <= (unsigned) (pos+n))
		s[pos]=0;
	else
	{
		i=pos-1;
		do
		 {
			++i;
			s[i]=s[i+n];
		 }
		while (s[i+n] != 0);
	}
}

void delwchar(wchar_t *s, int pos, int n)
{
	int  i;

	if (wcslen(s) <= (unsigned) (pos+n))
		s[pos]=0;
	else
	{
		i=pos-1;
		do
		 {
			++i;
			s[i]=s[i+n];
		 }
		while (s[i+n] != 0);
	}
}



