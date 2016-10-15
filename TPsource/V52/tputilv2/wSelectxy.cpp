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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <bstrings.h>
#include <bkeybrd.h>
#include <bvideo.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0

extern short ySize;

void wselectxy(int y, int x, wchar_t *prompt, wchar_t *term, wchar_t *tc)
{
	wchar_t ch, ch2;
	int l, wait = 1;

	kbflush();
	l = wcslen(prompt);
	scclrmsg(y, 0, 80);
	viwrrectw(y, x, y, x + l - 1, prompt, SEL_FORE, SEL_BACK, 0);
	l = min(l, 77 - x);
	viwrrectw(y, x + l, y, x + l + 1, L": ", SEL_FORE, SEL_BACK, 0);
	sccurset(y, x + l + 2);
	do {
		viwrrectw(y, x + l + 2, y, x + l + 2, tc, SEL_FORE, SEL_BACK, 0);
		ch = readkbd_w(&ch2, wait);
		if (ch == 0) {
			switch (ch2) {
			case 59:               /* F1 */
			case 60:                /* F2 */
			case 61:
			case 62:
			case 63:
			case 64:
			case 65:
			case 66:
			case 67:
			case 68:                /* F10 */
				ch = ch2 + 142;      /* 201..210 */
				break;
			case 73:                /* PgUp */
				ch = 160;
				break;
			case 81:                /* PgDn */
				ch = 161;
				break;
			default:
				ch = 255;
			}
		}
		else ch = towupper(ch);
		if (ch != 13 || wcswcind(13, term) >= 0) *tc = ch;
		if (wcswcind(*tc, term) == -1) erbeep();
	} while (wcswcind(*tc, term) == -1);
	if (*tc < 200) viwrrectw(y, x + l + 2, y, x + l + 2, tc, SEL_FORE, SEL_BACK, 0);
}

void wselectopt(wchar_t *wprompt, wchar_t *wterm, wchar_t *wtc)
{
	wselectxy(ySize - 3, 0, wprompt, wterm, wtc);
}
