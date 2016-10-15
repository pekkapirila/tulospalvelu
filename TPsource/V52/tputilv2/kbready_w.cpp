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

#include <windows.h>

//extern "C" {
int ProcessSpecialKey(INPUT_RECORD *Buf);
extern HANDLE hConIn;
//}

static int ctrlkey[25] = {94,95,96,97,98,99,100,101,102,103,
      69,70,119,141,132,74,115,76,116,78,117,145,118,146,147};
static int altkey[25] = {104,105,106,107,108,109,110,111,112,113,
      69,70,151,152,153,74,155,76,157,78,159,160,161,162,163};
static int shiftkey[10]= {84,85,86,87,88,89,89,91,92,93};

/* kbready palauttaa arvon 1, jos näppäintä on painettu */
/*               ja arvon 0, jos näppäintä ei ole painettu */

int kbready_w(wchar_t *ch, int *key)
{
	INPUT_RECORD Buf;
	DWORD nRead;
	WORD KC;

	do {
		if (!PeekConsoleInputW(hConIn, &Buf, 1, &nRead)) {
			GetLastError();
			return(-1);
		}
		if (nRead && (Buf.EventType != KEY_EVENT || 
			!Buf.Event.KeyEvent.bKeyDown ||
			((KC = Buf.Event.KeyEvent.wVirtualKeyCode) >= VK_SHIFT && KC <= VK_CAPITAL) || 
			(KC > VK_F12 && KC < 0xBA) || KC >= VK_PROCESSKEY || ProcessSpecialKey(&Buf))) {
			if (!ReadConsoleInputW(hConIn, &Buf, 1, &nRead)) {
				GetLastError();
				return(-1);
			}
			continue;
		}
		break;
	} while(1);
	if (nRead) {
		*ch = Buf.Event.KeyEvent.uChar.UnicodeChar;
		*key = Buf.Event.KeyEvent.wVirtualScanCode;
      if (*ch == 224)
         *ch = 0;
      if ((Buf.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
         && *key >= 59 && *key <= 83)
         *key = ctrlkey[*key-59];
      if ((Buf.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED)
         && *key >= 59 && *key <= 68)
         *key = shiftkey[*key-59];
      if ((Buf.Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED)) {
         if (*key >= 59 && *key <= 83)
            *key = altkey[*key-59];
         if (*ch >= 97 && *ch <= 122)
            *ch = 0;
			if (*key >= 2 && *key <= 11) {
				*key += 118;
				*ch = 0;
				}
         }
	}
	return(nRead);
}

int kbdrdy_w(void)
{
	wchar_t ch;
	int key;

	return(kbready_w(&ch, &key));
}
