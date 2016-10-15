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
#include <stdlib.h>
#include <ctype.h>
#include <tputil.h>

int getfields(wchar_t *line, wchar_t *fields[], int maxfields, wchar_t *sep, wchar_t *quotes, bool singlesep, bool deleteold)
{
	int len, nfld = 0;
	wchar_t *p1, *p2, *buf;
	bool inquotes = false;

	if (deleteold) {
		for (int i = 0; i < maxfields; i++) {
			if (fields[i] != NULL)
				delete[] fields[i];
			fields[i] = NULL;
			}
		}
	if (!line || !sep || !sep[0] || (len = wcslen(line)) == 0)
		return(0);
	buf = new wchar_t[len+1];
	p2 = buf;
	wmemset(buf, 0, len+1);
	for (p1 = line+wcslen(line)-1; p1 >= line && *p1 == L'\n' || *p1 == L'\r'; p1--)
		*p1 = 0;
	for (p1 = line; ; p1++) {
		if (*p1 == 0 || (inquotes && wcswcind(*p1, quotes) >= 0) ||
			(!inquotes && wcswcind(*p1, sep) >= 0)) {
			fields[nfld] = new wchar_t[wcslen(buf)+1];
			wcscpy(fields[nfld++], buf);
			if (*p1 == 0)
				break;
			wmemset(buf, 0, len+1);
			p2 = buf;
			if (nfld >= maxfields)
				break;
			if (inquotes) {
				p1++;
				while (*p1 && wcswcind(*p1, sep) < 0)
					p1++;
				}
			inquotes = false;
			if (!singlesep)
				while(wcswcind(p1[1], sep) >= 0)
					p1++;
			continue;
			}
		if (quotes && quotes[0] && wcswcind(*p1, quotes) >= 0) {
			inquotes = true;
			continue;
			}
		*(p2++) = *p1;
		}
	delete[] buf;
	return(nfld);
}

void deletefields(wchar_t *fields[], int maxfields)
{
	getfields(0, fields, maxfields, 0, 0, 0, true);
}
