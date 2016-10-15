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
#include <ctype.h>
#include <string.h>

long atold(char *str)
	{
	char s1[40], *p;
	long n;
	int k;

	strncpy(s1, str, 39);
	s1[39] = 0;
	n = atol(s1) * 1000;
	p = strtok(s1, " ,.");
	if (p) {
		p = strtok(NULL, " ,.");
		if (p) {
			for (k = 0; k < 3; k++) {
				if (p[k] < '0' || p[k] > '9') break;
				switch (k) {
					case 0 :
						n += 100 * (p[k]-'0');
						break;
					case 1 :
						n += 10 * (p[k]-'0');
						break;
					case 2 :
						n += p[k]-'0';
						break;
					}
				}
			}
		}
	return(n);
	}

long watold(wchar_t *str)
	{
	wchar_t s1[40], *p;
	long n;
	int k;

	wcsncpy(s1, str, 39);
	s1[39] = 0;
	n = _wtol(s1) * 1000;
	p = wcstok(s1, L" ,.");
	if (p) {
		p = wcstok(NULL, L" ,.");
		if (p) {
			for (k = 0; k < 3; k++) {
				if (p[k] < L'0' || p[k] > L'9') break;
				switch (k) {
					case 0 :
						n += 100 * (p[k]-L'0');
						break;
					case 1 :
						n += 10 * (p[k]-L'0');
						break;
					case 2 :
						n += p[k]-L'0';
						break;
					}
				}
			}
		}
	return(n);
	}


