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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int mm[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

int DateNo(int y, int m, int d)
{
	int dt = y - 1900;
	if (dt < 0)
		dt = 0;
	dt = 365*dt + (dt-1)/4;
	dt += mm[m-1];
	dt += d-1;
	if (y > 1900 && y%4 == 0 && m > 2)
		dt++;
	return(dt+2);
}

int yDateNo(int dd)
{
	dd -= 2;
	dd -= (dd-365)/1461;
	dd = 1900 + dd/365;
	return(dd);
}

int mDateNo(int dd)
{
	int y, m;

	y = yDateNo(dd);
	dd = dd - DateNo(y, 1, 1);
	if (y > 1900 && y % 4 == 0 && dd > 59)
		dd--;
	for (m = 1; m < 12; m++)
		if (dd < mm[m])
			return(m);
	return(12);
}

int dDateNo(int dd)
{
	int y, m;

	y = yDateNo(dd);
	dd = dd - DateNo(y, 1, 1);
	if (y > 1900 && y % 4 == 0 && dd > 59) {
		if (dd == 60)
			return(29);
		else
			dd--;
		}
	for (m = 1; m < 12; m++) {
		if (dd < mm[m])
			break;
		}
	dd += -mm[m-1]+1;
	if (dd < 1)
		dd = 1;
	if (dd > 31)
		dd = 31;
	return(dd);
}

wchar_t *stDateNo(wchar_t *st, int pvm)
{
	static wchar_t st0[12];

	if (st == NULL)
		st = st0;
	if (pvm > 1 && pvm < 73110)
		swprintf(st, L"%4d-%02d-%02d", yDateNo(pvm), mDateNo(pvm), dDateNo(pvm));
	else
		st[0] = 0;
	return(st);
}

int DateNoFromSt(wchar_t *st)
{
	unsigned int y, m, d, pvm = 0;

	if (wcslen(st) <= 10 && (st[4] == L'-' || st[1] == L'.' || st[2] == L'.')) {
		wchar_t st1[12] = L"", *p;
		wcscpy(st1, st);
		if ((p = wcstok(st1, L".-")) != NULL) {
			if (st[4] == L'-')
				y = _wtoi(p);
			else
				d = _wtoi(p);
			if ((p = wcstok(NULL, L".-")) != NULL) {
				m = _wtoi(p);
				if (st[4] == L'-')
					d = _wtoi(p+wcslen(p)+1);
				else {
					y = _wtoi(p+wcslen(p)+1);
					if (y < 100)
						y += 2000;
					}
				}
			if (d > 0 && d <= 31 && m >= 1 && m <= 12)
				pvm = DateNo(y, m, d);
			if (pvm < 2 || pvm > 73109)
				pvm = 0;
			}
		}
	return(pvm);
}
