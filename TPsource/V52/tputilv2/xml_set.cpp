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

void set_xml_s(char *vastr, char *tag, char *value)
   {
	char *p1, *p2, ch[2] = " ";

   sprintf(vastr, "<%s>", tag);
	for (p1 = value, p2 = vastr+strlen(vastr); *p1; p1++) {
		ch[0] = *p1;
	   oemtoansi(ch, 0);
		switch (ch[0]) {
			case '&' :
				strcpy(p2, "&amp;");
				p2 += 5;
				break;
			case '<' :
				strcpy(p2, "&lt;");
				p2 += 4;
				break;
			case '>' :
				strcpy(p2, "&gt;");
				p2 += 4;
				break;
			default :
				*(p2++) = ch[0];
				break;
			}
		}
   sprintf(p2, "</%s>\n", tag);
   }

void set_xml_d(char *vastr, char *tag, INT32 value)
   {
   sprintf(vastr, "<%s>%ld</%s>\n", tag, value, tag);
   }

void set_tag(char *vastr, char *tag)
   {
   sprintf(vastr, "<%s>\n", tag);
   }

void set_antitag(char *vastr, char *tag)
   {
   sprintf(vastr, "</%s>\n", tag);
	}

void set_wxml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value)
	{
	wchar_t *p1, *p2, ch[2] = L" ";

	swprintf(vastr, L"<%s>", tag);
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

void set_wxml_d(wchar_t *vastr, wchar_t *tag, INT32 value)
	{
	swprintf(vastr, L"<%s>%ld</%s>\n", tag, value, tag);
	}

void set_wtag(wchar_t *vastr, wchar_t *tag)
	{
	swprintf(vastr, L"<%s>\n", tag);
	}

void set_wantitag(wchar_t *vastr, wchar_t *tag)
	{
	swprintf(vastr, L"</%s>\n", tag);
	}

