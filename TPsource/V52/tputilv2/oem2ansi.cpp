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
#include <string.h>
#include <tputil.h>

char oemtoansich(char ch)
{
	switch (ch) {
		case 142 :
			return(196);
		case 153 :
			return(214);
		case 143 :
			return(197);
		case 154 :
			return(220);
		case 144 :
			return(201);
		case 132 :
			return(228);
		case 148 :
			return(246);
		case 134 :
			return(229);
		case 129 :
			return(252);
		case 130 :
			return(233);
		}
	return(ch);
}

char *oemtoansi(char *line, int ppiste)
	{
	char *p;

	for (p = line; *p; p++) {
		switch (*p) {
			case '.' : if (ppiste) *p = ':';
						  break;
			case 142 : *p = 196; break;
			case 153 : *p = 214; break;
			case 143 : *p = 197; break;
			case 154 : *p = 220; break;
			case 144 : *p = 201; break;
			case 132 : *p = 228; break;
			case 148 : *p = 246; break;
			case 134 : *p = 229; break;
			case 129 : *p = 252; break;
			case 130 : *p = 233; break;
			}
		}
	return(line);
	}

wchar_t ansitowchar(char ch)
{
	wchar_t buf[3];

	if (mbtowc(buf, &ch, 1) < 0)
		return(L'?');
	return(buf[0]);
}

wchar_t *ansitowcs(wchar_t *wline, char *line, int len)
{
	int i;
	wchar_t *wc;

	wc = wline;
	for (i = 0; i < len; i++) {
		*(wc++) = ansitowchar(*(line++));
		if (line[-1] == 0)
			break;
		}
	return(wline);
}

wchar_t oemtowchar(char ch)
{
	return(ansitowchar(oemtoansich(ch)));
}

wchar_t *oemtowcs(wchar_t *wline, char *line, int len, int ppiste)
{
	char *aline;

	aline = (char *)calloc(len+1, 1);
	strncpy(aline, line, len);
	aline[len] = 0;
	oemtoansi(aline, ppiste);
	ansitowcs(wline, aline, len);
	free(aline);
	return(wline);
}
