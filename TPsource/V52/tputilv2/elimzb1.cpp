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

void elimzb1(char *aikastr)
	{
	char *p;

	for (p = aikastr; *p == '0' || *p == '.' || *p == ':'; p++) ;
	if (p - aikastr > 6)
		p = aikastr+7;
	if (!*p && p - aikastr > 0)
		p--;
	memmove(aikastr, p, strlen((char *)p)+1);
	}

void elimwzb1(wchar_t *aikastr)
	{
	wchar_t *p;

	for (p = aikastr; *p == L'0' || *p == L'.' || *p == L':'; p++) ;
	if (p - aikastr > 6)
		p = aikastr+7;
	if (!*p && p - aikastr > 0)
		p--;
	wmemmove(aikastr, p, wcslen(p)+1);
	}

