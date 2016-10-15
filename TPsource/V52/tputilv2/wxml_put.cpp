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
#include <tputil.h>

void wput_xml_s(TextFl *fl, wchar_t *tag, wchar_t *value, int level)
   {
	wchar_t *p1, ch[2] = L" ", ln[300] = L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
	if (!value[0]) {
		swprintf(ln+wcslen(ln), L"<%s></%s>\n", tag, tag);
		}
	else {
		swprintf(ln+wcslen(ln), L"<%s>", tag);
		for (p1 = value; *p1; p1++) {
			ch[0] = *p1;
			switch (*p1) {
				case L'&' :
					wcscat(ln, L"&amp;");
					break;
				case L'<' :
					wcscat(ln, L"&lt;");
					break;
				case L'>' :
					wcscat(ln, L"&gt;");
					break;
				default :
					wcscat(ln, ch);
					break;
				}
			}
		swprintf(ln+wcslen(ln), L"</%s>\n", tag);
		}
   fl->WriteLine(ln);
   }

void wput_xml_d(TextFl *fl, wchar_t *tag, INT32 value, int level)
   {
	wchar_t ln[300]=L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
   swprintf(ln+wcslen(ln), L"<%s>%ld</%s>\n", tag, value, tag);
   fl->WriteLine(ln);
   }

void wput_xml_f(TextFl *fl, wchar_t *tag, double value, int prec,  int level)
   {
	wchar_t ln[300]=L"", fmt[20];

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
	if (prec < 0 || prec > 8)
		prec = 2;
   swprintf(fmt, L"<%cs>%c.%df</%cs>\n", L'%', L'%', prec, L'%');
   swprintf(ln+wcslen(ln), fmt, tag, value, tag);
   fl->WriteLine(ln);
   }

void wput_tag(TextFl *fl, wchar_t *tag, int level)
   {
	wchar_t ln[100]=L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
   swprintf(ln+wcslen(ln), L"<%s>\n", tag);
   fl->WriteLine(ln);
   }

void wput_antitag(TextFl *fl, wchar_t *tag, int level)
   {
	wchar_t ln[100]=L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
   swprintf(ln+wcslen(ln), L"</%s>\n", tag);
   fl->WriteLine(ln);
   }

void wput_tagparams(TextFl *fl, wchar_t *tag, wchar_t *params, bool empty, int level)
   {
	wchar_t ln[100]=L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
   swprintf(ln+wcslen(ln), L"<%s %s%s>\n", tag, params, empty ? L"/" : L"");
   fl->WriteLine(ln);
   }

void wput_xml_params_s(TextFl *fl, wchar_t *tag, wchar_t *params, wchar_t *value, int level)
   {
	wchar_t *p1, ch[2] = L" ", ln[300] = L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
	if (!value[0]) {
		swprintf(ln+wcslen(ln), L"<%s %s></%s>\n", tag, params, tag);
		}
	else {
		swprintf(ln+wcslen(ln), L"<%s %s>", tag, params);
		for (p1 = value; *p1; p1++) {
			ch[0] = *p1;
			switch (*p1) {
				case L'&' :
					wcscat(ln, L"&amp;");
					break;
				case L'<' :
					wcscat(ln, L"&lt;");
					break;
				case L'>' :
					wcscat(ln, L"&gt;");
					break;
				default :
					wcscat(ln, ch);
					break;
				}
			}
		swprintf(ln+wcslen(ln), L"</%s>\n", tag);
		}
   fl->WriteLine(ln);
   }

void wput_xml_params_d(TextFl *fl, wchar_t *tag, wchar_t *params, int value, int level)
   {
	wchar_t ln[300]=L"";

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
   swprintf(ln+wcslen(ln), L"<%s %s>%ld</%s>\n", tag, params, value, tag);
   fl->WriteLine(ln);
   }

void wput_xml_params_f(TextFl *fl, wchar_t *tag, wchar_t *params, double value, int prec, int level)
   {
	wchar_t ln[300]=L"", fmt[20];

	for (int i = 0; i < level; i++) {
		ln[i] = L'\t';
		}
	if (prec < 0 || prec > 8)
		prec = 2;
   swprintf(fmt, L"<%cs %cs>%c.%df</%cs>\n", L'%', L'%', L'%', prec, L'%');
   swprintf(ln+wcslen(ln), fmt, tag, params, value, tag);
   fl->WriteLine(ln);
   }

