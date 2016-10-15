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
#include <wchar.h>

void elimbl1(char *str)
	{
	char *p;

	for (p = str; *p && *p <= ' '; p++) ;
	memmove(str, p, strlen(p) + 1);
	}

void elimwbl1(wchar_t *str)
	{
	wchar_t *p;

	for (p = str; *p && *p <= L' '; p++) ;
	wmemmove(str, p, wcslen(p) + 1);
	}

void elimbl2(char *str)
	{
	char *p;

	for (p = str+strlen(str)-1; p >= str; p--)
		if (*p <= ' ')
			*p = 0;
		else
			break;
	for (p = str; *p && *p <= ' '; p++) ;
	memmove(str, p, strlen(p) + 1);
	}

void elimwbl2(wchar_t *str)
	{
	wchar_t *p;

	for (p = str+wcslen(str)-1; p >= str; p--)
		if (*p <= L' ')
			*p = 0;
		else
			break;
	for (p = str; *p && *p <= L' '; p++) ;
	wmemmove(str, p, wcslen(p) + 1);
	}

