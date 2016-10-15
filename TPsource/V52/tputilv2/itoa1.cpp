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

#include <stdlib.h>

char *itoa1(char *s, int n, int l)
	{
	int k;
	char *s1;

	s1 = s + l;
	while (n > 9) {
		k = n / 10;
		if (s1 > s) *--s1 = n - 10*k + '0';
		n = k;
		}
	if (s1 > s) *--s1 = n + '0';
	return(s);
	}

wchar_t *itow1(wchar_t *s, int n, int l)
	{
	int k;
	wchar_t *s1;

	s1 = s + l;
	while (n > 9) {
		k = n / 10;
		if (s1 > s) *--s1 = n - 10*k + L'0';
		n = k;
		}
	if (s1 > s) *--s1 = n + L'0';
	return(s);
	}
