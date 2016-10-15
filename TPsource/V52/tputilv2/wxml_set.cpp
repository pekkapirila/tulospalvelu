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


#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#include <stdio.h>
#include <string.h>
#include <tputil.h>

void wset_xml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value, int level)
	{
	wset_xml_s(vastr, tag, value, level, NULL);
	}

void wset_xml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value, int level, wchar_t *parstr)
	{
	wchar_t *p1, *p2, ch[2] = L" ", st[2] = L"";

	if (!parstr) {
		parstr = st;
		}
	else if (parstr[0])
		st[0] = L' ';
	for (int i = 0; i < level; i++)
		vastr[i] = L'\t';
	vastr[level] = 0;
	if (value == NULL || *value == 0) {
		swprintf(vastr+wcslen(vastr), L"<%s%s%s />\n", tag, st, parstr);
		return;
		}
	swprintf(vastr+wcslen(vastr), L"<%s%s%s>", tag, st, parstr);
	for (p1 = value, p2 = vastr+wcslen(vastr); *p1; p1++) {
		ch[0] = *p1;
		switch (ch[0]) {
			case L'&' :
				wcscpy(p2, L"&amp;");
				p2 += 5;
				break;
			case L'<' :
				wcscpy(p2, L"&lt;");
				p2 += 4;
				break;
			case L'>' :
				wcscpy(p2, L"&gt;");
				p2 += 4;
				break;
			default :
				*(p2++) = ch[0];
				break;
			}
		}
	swprintf(p2, L"</%s>\n", tag);
	}

void wset_xml_d(wchar_t *vastr, wchar_t *tag, INT32 value, int level)
	{
	wset_xml_d(vastr, tag, value, level, NULL);
	}

void wset_xml_d(wchar_t *vastr, wchar_t *tag, INT32 value, int level, wchar_t *parstr)
	{
	wchar_t st[2] = L"";
	if (!parstr) {
		parstr = st;
		}
	else if (parstr[0])
		st[0] = L' ';

	for (int i = 0; i < level; i++)
		vastr[i] = L'\t';
	vastr[level] = 0;
	swprintf(vastr+wcslen(vastr), L"<%s%s%s>%ld</%s>\n", tag, st, parstr, value, tag);
	}

void wset_xml_f(wchar_t *vastr, wchar_t *tag, double value, int prec, int level)
   {
	wset_xml_f(vastr, tag, value, prec, level, NULL);
   }

void wset_xml_f(wchar_t *vastr, wchar_t *tag, double value, int prec, int level, wchar_t *parstr)
   {
	wchar_t fmt[30];
	wchar_t st[2] = L"";
	if (!parstr) {
		parstr = st;
		}
	else if (parstr[0])
		st[0] = L' ';

	for (int i = 0; i < level; i++) {
		vastr[i] = L'\t';
		}
	vastr[level] = 0;
	if (prec < 0 || prec > 8)
		prec = 2;
   swprintf(fmt, L"<%cs%cs%cs>%c.%df</%cs>\n", L'%', L'%', L'%', L'%', prec, L'%');
   swprintf(vastr+wcslen(vastr), fmt, tag, st, parstr, value, tag);
   }

void wset_tag(wchar_t *vastr, wchar_t *tag, int level)
	{
	for (int i = 0; i < level; i++)
		vastr[i] = L'\t';
	vastr[level] = 0;
	swprintf(vastr+wcslen(vastr), L"<%s>\n", tag);
	}

void wset_antitag(wchar_t *vastr, wchar_t *tag, int level)
	{
	for (int i = 0; i < level; i++)
		vastr[i] = L'\t';
	vastr[level] = 0;
	swprintf(vastr+wcslen(vastr), L"</%s>\n", tag);
	}

void wset_tagparams(wchar_t *vastr, wchar_t *tag, wchar_t *params, bool empty, int level)
   {
	for (int i = 0; i < level; i++) {
		vastr[i] = L'\t';
		}
	vastr[level] = 0;
   swprintf(vastr+wcslen(vastr), L"<%s %s%s>\n", tag, params, empty ? L"/" : L"");
   }


